within SorpPropLib.WorkingPair.Tester;
model Test_WPair_absorption_mixing "Test model that creates an external absorption working pair object based on 
  mixing rules"
  extends Modelica.Icons.Example;

  //
  // Definition of parameters
  //
  parameter String path_db=
    "PC-SIM1/PC-Sim1-Daten/Carles/rep/SorpProp/sorpproplib_JSON/modelica_wrapper/SorpPropLib/Resources/Data/sorpproplib.json"
    "Path to database, i.e., JSON-file of database."
    annotation (Dialog(tab="General", group = "Working pair"));

  parameter String name_sorbent = "lubricant"
    "Name of sorbent, e.g., 'zeolite pellet'"
    annotation (Dialog(tab="General", group = "Working pair"));
  parameter String name_sorbent_sub_type = "POE ISO7"
    "Sub-type of sorbent, e.g., '13X'"
    annotation (Dialog(tab="General", group = "Working pair"));
  parameter String name_refrigerant = "Isobutane"
    "Name of refrigerant, e.g., 'Water'"
    annotation (Dialog(tab="General", group = "Working pair"));

  parameter String func_isotherm = "MixingRule"
    "Name of functional approach for isotherm, e.g., 'Toth'"
    annotation (Dialog(tab="General", group = "Working pair"));
  parameter Integer func_isotherm_ID = 1
    "ID of functional approach for isotherm, e.g., '1'"
    annotation (Dialog(tab="General", group = "Working pair"));

  parameter String func_vapor_pressure = "VaporPressure_EoS1"
    "Name of functional approach for vapor pressure, e.g., 'VaporPressure_EoS1'"
    annotation (Dialog(tab="General", group = "Working pair"));
  parameter Integer func_vapor_pressure_ID = 1
    "ID of functional approach for vapor pressure, e.g., '1'"
    annotation (Dialog(tab="General", group = "Working pair"));

  parameter String func_saturated_liquid_density = "SaturatedLiquidDensity_EoS1"
    "Name of functional approach for saturated liquid density, e.g. 'SaturatedLiquidDensity_EoS1'"
    annotation (Dialog(tab="General", group = "Working pair"));
  parameter Integer func_saturated_liquid_density_ID = 1
    "ID of functional approach for saturated liquid density, e.g., '1'"
    annotation (Dialog(tab="General", group = "Working pair"));

  //
  // Definition of external object
  //
  WPair extObj=WorkingPair.WPair(path_db,
        name_sorbent, name_sorbent_sub_type, name_refrigerant,
        func_isotherm, func_isotherm_ID,
        func_vapor_pressure, func_vapor_pressure_ID,
        func_saturated_liquid_density, func_saturated_liquid_density_ID)
    "Initialize external object of working pair by calling its class";

  //
  // Definition of inputs
  //
  Modelica.SIunits.Pressure p = 5e5 +150*time
    "Equilibrium pressure";
  Modelica.SIunits.Temperature T = 333.15 + 0.25*time
    "Equilibrium temperature";

  //
  // Definition of variables for refrigerant functions
  //
  Modelica.SIunits.Pressure p_sat
    "Vapor pressure";

  //
  // Definition of variables for adsorption functions that are always defined
  //
  Modelica.SIunits.MoleFraction x
    "Equilibrium mole fraction of refrigerant in liquid phase";
  Modelica.SIunits.Pressure p_inv
    "Equilibrium pressure";
  Modelica.SIunits.Temperature T_inv
    "Equilibrium temperature";

  Real dp_dx(unit = "(Pa.mol)/mol")
    "Derivative of equilibrium pressure wrt. mole fraction";
  Real dp_dT(unit = "Pa/K")
    "Derivative of equilibrium pressure wrt. temperature";

equation
  //
  // Call external functions to calculate properties of refrigerant
  //
  p_sat =WPairFunctions.Refrigerant.p_sat_T(
    T,
    extObj=extObj) "Calculate vapor pressure";

  //
  // Call external functions that are generally defined for adsorption working
  // pairs
  //
  (x,,) =WPairFunctions.Absorption.Mixing.x_pT(
    p,
    T,
    extObj=extObj)
    "Calculate molar fraction depending on pressure and temperature";
  (p_inv,,) =WPairFunctions.Absorption.Mixing.p_Tx(
    T,
    x,
    extObj=extObj)
    "Calculate pressure depending on molar fraction and temperature";
  (T_inv,,) =WPairFunctions.Absorption.Mixing.T_px(
    p,
    x,
    extObj=extObj)
    "Calculate temperature depending on pressure and molar fraction";

  dp_dx =WPairFunctions.Absorption.Mixing.dp_dx_Tx(
    T,
    x,
    extObj=extObj) "Derivative of equilibrium pressure wrt. mole fraction";
  dp_dT =WPairFunctions.Absorption.Mixing.dp_dT_Tx(
    T,
    x,
    extObj=extObj) "Derivative of equilibrium pressure wrt. temperature";

  //
  // Annotations
  //
  annotation (Documentation(revisions="<html>
<ul>
  <li>
  July 13, 2021, by Mirko Engelpracht:<br/>
  First implementation.
  </li>
</ul>
</html>"), experiment(StopTime=150, __Dymola_Algorithm="Dassl"));
end Test_WPair_absorption_mixing;
