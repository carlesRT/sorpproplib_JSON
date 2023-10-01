within SorpPropLib.DirectFunctionCalls.Tester;
model Test_WPair_adsorption_surface_vapor
  "Test model that creates an external adsorption working pair object based on 
  the surface approach and using the vapor pressure as additional input, and 
  calls its functions"
  extends Modelica.Icons.Example;

  //
  // Definition of parameters
  //
  parameter String path_db=
    "PC-SIM1/PC-Sim1-Daten/Carles/rep/SorpProp/sorpproplib_JSON/modelica_wrapper/SorpPropLib/Resources/Data/sorpproplib.json"
    "Path to database, i.e., JSON-file of database."
    annotation (Dialog(tab="General", group = "Working pair"));

  parameter String name_sorbent = "silica gel pellet"
    "Name of sorbent, e.g., 'zeolite pellet'"
    annotation (Dialog(tab="General", group = "Working pair"));
  parameter String name_sorbent_sub_type = "Fuji A"
    "Sub-type of sorbent, e.g., '13X'"
    annotation (Dialog(tab="General", group = "Working pair"));
  parameter String name_refrigerant = "Water"
    "Name of refrigerant, e.g., 'Water'"
    annotation (Dialog(tab="General", group = "Working pair"));

  parameter String func_isotherm = "Freundlich"
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
  Modelica.Units.SI.Pressure p = 1e5 -250*time
    "Equilibrium pressure";
  Modelica.Units.SI.Temperature T = 303.15 + 1*time
    "Equilibrium temperature";

  //
  // Definition of variables for refrigerant functions
  //
  Modelica.Units.SI.Pressure p_sat
    "Vapor pressure";
  Real dp_dT_sat(unit = "Pa/K")
    "Derivative of pressure wrt. temperature";

  //
  // Definition of variables for adsorption functions that are always defined
  //
  Real w(unit="kg/kg")
    "Equilibrium loading";
  Modelica.Units.SI.Pressure p_inv
    "Equilibrium pressure";
  Modelica.Units.SI.Temperature T_inv
    "Equilibrium temperature";

  Real dw_dp(unit="kg/(kg.Pa)")
    "Derivative of equilibrium loading wrt. pressure";
  Real dw_dT(unit="kg/(kg.K)")
      "Derivative of equilibrium loading wrt. temperature";
  Real dp_dw(unit = "(Pa.kg)/kg")
    "Derivative of equilibrium pressure wrt. loading";
  Real dp_dT(unit = "Pa/K")
    "Derivative of equilibrium pressure wrt. temperature";

//   Real piStar(unit = "kg/mol")
//     "Reduced spreading pressure";

  //
  // Definition of variables for adsorption functions that are only defined for
  // isotherms based on the surface approach and using the vapor pressure as
  // additional input
  //
  Real w_sur_vap(unit="kg/kg")
    "Equilibrium loading";
  Modelica.Units.SI.Pressure p_inv_sur_vap
    "Equilibrium pressure";
  Modelica.Units.SI.Temperature T_inv_sur_vap
    "Equilibrium temperature";

  Real dw_dp_sur_vap(unit="kg/(kg.Pa)")
    "Derivative of equilibrium loading wrt. pressure";
  Real dw_dT_sur_vap(unit="kg/(kg.K)")
      "Derivative of equilibrium loading wrt. temperature";
  Real dp_dw_sur_vap(unit = "(Pa.kg)/kg")
    "Derivative of equilibrium pressure wrt. loading";
  Real dp_dT_sur_vap(unit = "Pa/K")
    "Derivative of equilibrium pressure wrt. temperature";

//   Real piStar_sur_vap(unit = "kg/mol")
//     "Reduced spreading pressure";

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
  dp_dT_sat = Refrigerant.dp_dT_sat_T(
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
    T) "Calculate derivative of pressure wrt. temperature";

  //
  // Call external functions that are generally defined for adsorption working
  // pairs
  //
  w = Adsorption.General.w_pT(
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
    T) "Calculate loading depending on pressure and temperature";

  p_inv = Adsorption.General.p_wT(
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
    w,
    T) "Calculate loading depending on pressure and temperature";
  T_inv = Adsorption.General.T_pw(
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
    w) "Calculate loading depending on pressure and temperature";

  dw_dp = Adsorption.General.dw_dp_pT(
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
    T) "Derivative of equilibrium loading wrt. pressure";
  dw_dT = Adsorption.General.dw_dT_pT(
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
    T) "Derivative of equilibrium loading wrt. temperature";
  dp_dw = Adsorption.General.dp_dw_wT(
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
    w,
    T) "Derivative of equilibrium pressure wrt. loading";
  dp_dT = Adsorption.General.dp_dT_wT(
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
    w,
    T) "Derivative of equilibrium pressure wrt. temperature";

  // piStar = WPairFunctions.General.piStar_pyxgTM(
  //   path_db,
  //   name_sorbent,
  //   name_sorbent_sub_type,
  //   name_refrigerant,
  //   func_isotherm,
  //   func_isotherm_ID,
  //   func_vapor_pressure,
  //   func_vapor_pressure_ID,
  //   func_saturated_liquid_density,
  //   func_saturated_liquid_density_ID,
  //   p, 1, 1, 1, T,
  //   0.018015)
  // "Derivative of equilibrium pressure wrt. temperature";
  //
  // Call external functions that are defined for adsorption working pairs based
  // on the surface approach and using the vapor pressure as additional input
  //
  w_sur_vap = Adsorption.SurfaceVapor.w_pTpsat(
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
    p_sat) "Calculate loading depending on pressure and temperature";

  p_inv_sur_vap = Adsorption.SurfaceVapor.p_wTpsat(
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
    w,
    T,
    p_sat) "Calculate loading depending on pressure and temperature";
  T_inv_sur_vap = Adsorption.SurfaceVapor.T_pwpsat(
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
    w,
    p_sat) "Calculate loading depending on pressure and temperature";

  dw_dp_sur_vap =
    Adsorption.SurfaceVapor.dw_dp_pTpsat(
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
    p_sat) "Derivative of equilibrium loading wrt. pressure";
  dw_dT_sur_vap =
    Adsorption.SurfaceVapor.dw_dT_pTpsat(
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
    p_sat,
    dp_dT_sat) "Derivative of equilibrium loading wrt. temperature";
  dp_dw_sur_vap =
    Adsorption.SurfaceVapor.dp_dw_wTpsat(
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
    w,
    T,
    p_sat) "Derivative of equilibrium pressure wrt. loading";
  dp_dT_sur_vap =
    Adsorption.SurfaceVapor.dp_dT_wTpsat(
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
    w,
    T,
    p_sat,
    dp_dT_sat) "Derivative of equilibrium pressure wrt. temperature";

//    piStar_sur_vap = SurfaceVapor.piStar_pyxgTpsatM(
//     path_db,
//     name_sorbent,
//     name_sorbent_sub_type,
//     name_refrigerant,
//     func_isotherm,
//     func_isotherm_ID,
//     func_vapor_pressure,
//     func_vapor_pressure_ID,
//     func_saturated_liquid_density,
//     func_saturated_liquid_density_ID,
//     p, 1, 1, 1, T, p_sat, 0.018015)
//      "Derivative of equilibrium pressure wrt. temperature";

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
end Test_WPair_adsorption_surface_vapor;
