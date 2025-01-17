within SorpPropLib.DirectFunctionCalls.Tester;
model Test_WPair_absorption_conventional
  "Test model that creates an external absorption working pair object based on 
  the conventional approach"
  extends Modelica.Icons.Example;

  //
  // Definition of parameters
  //
  parameter String path_db=
    "PC-SIM1/PC-Sim1-Daten/Carles/rep/SorpProp/sorpproplib_JSON/modelica_wrapper/SorpPropLib/Resources/Data/sorpproplib.json"
    "Path to database, i.e., JSON-file of database."
    annotation (Dialog(tab="General", group = "Working pair"));

  parameter String name_sorbent = "NaOH-KOH-CsOH"
    "Name of sorbent, e.g., 'zeolite pellet'"
    annotation (Dialog(tab="General", group = "Working pair"));
  parameter String name_sorbent_sub_type = ""
    "Sub-type of sorbent, e.g., '13X'"
    annotation (Dialog(tab="General", group = "Working pair"));
  parameter String name_refrigerant = "Water"
    "Name of refrigerant, e.g., 'Water'"
    annotation (Dialog(tab="General", group = "Working pair"));

  parameter String func_isotherm = "Duehring"
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
  Modelica.Units.SI.Pressure p = 0.25e5 -150*time
    "Equilibrium pressure";
  Modelica.Units.SI.Temperature T = 373.15 + 0.25*time
    "Equilibrium temperature";

  //
  // Definition of variables for adsorption functions that are always defined
  //
  Real X(unit="kg/kg")
    "Equilibrium concentration";
  Modelica.Units.SI.Pressure p_inv
    "Equilibrium pressure";
  Modelica.Units.SI.Temperature T_inv
    "Equilibrium temperature";

  Real dX_dp(unit="kg/(kg.Pa)")
    "Derivative of equilibrium concentration wrt. pressure";
  Real dX_dT(unit="kg/(kg.K)")
      "Derivative of equilibrium concentration wrt. temperature";
  Real dp_dX(unit = "(Pa.kg)/kg")
    "Derivative of equilibrium pressure wrt. concentration";
  Real dp_dT(unit = "Pa/K")
    "Derivative of equilibrium pressure wrt. temperature";

equation
  //
  // Call external functions that are generally defined for adsorption working
  // pairs
  //
  X = Absorption.Conventional.X_pT(
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
    T)
    "Calculate concentration depending on pressure and temperature";

  p_inv = Absorption.Conventional.p_XT(
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
    X,
    T)
    "Calculate concentration depending on pressure and temperature";
  T_inv = Absorption.Conventional.T_pX(
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
    X)
    "Calculate concentration depending on pressure and temperature";

  dX_dp = Absorption.Conventional.dX_dp_pT(
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
    T) "Derivative of equilibrium concentration wrt. pressure";
  dX_dT = Absorption.Conventional.dX_dT_pT(
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
    T) "Derivative of equilibrium concentration wrt. temperature";
  dp_dX = Absorption.Conventional.dp_dX_XT(
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
    X,
    T) "Derivative of equilibrium pressure wrt. concentration";
  dp_dT = Absorption.Conventional.dp_dT_XT(
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
    X,
    T) "Derivative of equilibrium pressure wrt. temperature";

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
end Test_WPair_absorption_conventional;
