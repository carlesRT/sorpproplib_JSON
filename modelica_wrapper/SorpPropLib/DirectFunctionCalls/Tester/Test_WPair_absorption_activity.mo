within SorpPropLib.DirectFunctionCalls.Tester;
model Test_WPair_absorption_activity "Test model that creates an external absorption working pair object based on 
  activity coefficients"
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
  parameter String name_sorbent_sub_type = "POE"
    "Sub-type of sorbent, e.g., '13X'"
    annotation (Dialog(tab="General", group = "Working pair"));
  parameter String name_refrigerant = "R-134a"
    "Name of refrigerant, e.g., 'Water'"
    annotation (Dialog(tab="General", group = "Working pair"));

  parameter String func_isotherm = "WilsonFixedDl"
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
  // Definition of inputs
  //
  Modelica.Units.SI.Pressure p = 5e5 +150*time
    "Equilibrium pressure";
  Modelica.Units.SI.Temperature T = 333.15 + 0.25*time
    "Equilibrium temperature";

  //
  // Definition of variables for refrigerant functions
  //
  Modelica.Units.SI.Pressure p_sat
    "Vapor pressure";

  //
  // Definition of variables for adsorption functions that are always defined
  //
  Modelica.Units.SI.MoleFraction x
    "Equilibrium mole fraction of refrigerant in liquid phase";
  Modelica.Units.SI.Pressure p_inv
    "Equilibrium pressure";

  Real g(unit="1")
    "Activity coefficient of first component (i.e., refrigerant)";

  Modelica.Units.SI.MoleFraction x_vap
    "Equilibrium mole fraction of refrigerant in liquid phase";
  Modelica.Units.SI.Pressure p_inv_vap
    "Equilibrium pressure";

equation
  //
  // Call external functions to calculate properties of refrigerant
  //
  p_sat = Refrigerant.p_sat_T(
    path_db,
    name_sorbent,
    name_sorbent_sub_type,
    name_refrigerant,
    func_isotherm,
    func_isotherm_ID,
    func_vapor_pressure,
    func_vapor_pressure_ID,
    func_saturated_liquid_density,
    func_saturated_liquid_density_ID,
    T)
    "Calculate vapor pressure";

  //
  // Call external functions that are generally defined for adsorption working
  // pairs
  //
  x = Absorption.Activity.x_pTv1v2(
    path_db,
    name_sorbent,
    name_sorbent_sub_type,
    name_refrigerant,
    func_isotherm,
    func_isotherm_ID,
    func_vapor_pressure,
    func_vapor_pressure_ID,
    func_saturated_liquid_density,
    func_saturated_liquid_density_ID,
    p,
    T,
    -1,
    -1)
    "Calculate molar fraction depending on pressure and temperature";
  p_inv = Absorption.Activity.p_Txv1v2(
    path_db,
    name_sorbent,
    name_sorbent_sub_type,
    name_refrigerant,
    func_isotherm,
    func_isotherm_ID,
    func_vapor_pressure,
    func_vapor_pressure_ID,
    func_saturated_liquid_density,
    func_saturated_liquid_density_ID,
    T,
    x,
    -1,
    -1)
    "Calculate pressure depending on molar fraction and temperature";

  g = Absorption.Activity.g_Txv1v2(
    path_db,
    name_sorbent,
    name_sorbent_sub_type,
    name_refrigerant,
    func_isotherm,
    func_isotherm_ID,
    func_vapor_pressure,
    func_vapor_pressure_ID,
    func_saturated_liquid_density,
    func_saturated_liquid_density_ID,
    T,
    x,
    -1,
    -1)
    "Calculate activity coefficient ddepending on molar fraction and temperature";

  x_vap = Absorption.Activity.x_pTv1v2psat(
    path_db,
    name_sorbent,
    name_sorbent_sub_type,
    name_refrigerant,
    func_isotherm,
    func_isotherm_ID,
    func_vapor_pressure,
    func_vapor_pressure_ID,
    func_saturated_liquid_density,
    func_saturated_liquid_density_ID,
    p,
    T,
    -1,
    -1,
    p_sat)
    "Calculate molar fraction depending on pressure and temperature";
  p_inv_vap = Absorption.Activity.p_Txv1v2psat(
    path_db,
    name_sorbent,
    name_sorbent_sub_type,
    name_refrigerant,
    func_isotherm,
    func_isotherm_ID,
    func_vapor_pressure,
    func_vapor_pressure_ID,
    func_saturated_liquid_density,
    func_saturated_liquid_density_ID,
    T,
    x,
    -1,
    -1,
    p_sat)
    "Calculate pressure depending on molar fraction and temperature";

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
</html>"), experiment(
      StopTime=25,
      Interval=1,
      __Dymola_Algorithm="Dassl"));
end Test_WPair_absorption_activity;
