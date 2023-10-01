within SorpPropLib.DirectFunctionCalls.Adsorption.Volumetric;
function dw_dT_pTpsatRho "Function that calculates partial derivative of equilibrium loading wrt. 
  temperature depending on pressure and temperature"
  extends DirectFunctionCalls.Internal.partialDirectFunctionCall;

  //
  // Definition of inputs
  //
  input Modelica.Units.SI.Pressure p
    "Equilibrium pressure"
    annotation(Dialog(tab="General", group="Inputs"));
  input Modelica.Units.SI.Temperature T
    "Equilibrium temperature"
    annotation(Dialog(tab="General", group="Inputs"));
  input Modelica.Units.SI.Pressure p_sat
    "Vapor pressure"
    annotation(Dialog(tab="General", group="Inputs"));
  input Modelica.Units.SI.Density d_liq
    "Saturated liquid density (i.e., density of adsorpt)"
    annotation(Dialog(tab="General", group="Inputs"));
  input Real dp_sat_dT(unit = "Pa/K")
    "Derivative of vapor pressure wrt. temperature"
    annotation(Dialog(tab="General", group="Inputs"));
  input Real d_liq_dT(unit = "kg/(m3.K)")
    "Derivative of saturated liquid density (i.e., density of adsorpt) wrt. temperature"
    annotation(Dialog(tab="General", group="Inputs"));

  //
  // Definition of outputs
  //
  output Real dw_dT(unit="kg/(kg.K)")
    "Derivative of equilibrium loading wrt. temperature"
    annotation (Dialog(tab="General", group = "Inputs", enable = false));

  //
  // Function call
  //
  external"C" dw_dT = direct_ads_vol_dw_dT_pTpsatRho(p, T, p_sat, d_liq,
    dp_sat_dT, d_liq_dT, path_db,
    name_sorbent, name_sorbent_sub_type, name_refrigerant,
    func_isotherm, func_isotherm_ID,
    func_vapor_pressure, func_vapor_pressure_ID,
    func_saturated_liquid_density, func_saturated_liquid_density_ID)
    annotation (Include = "#include \"workingPair.h\"",
                Library = "libsorpPropLib",
                IncludeDirectory = "modelica://SorpPropLib/Resources/Include/",
                LibraryDirectory = "modelica://SorpPropLib/Resources/Library/");

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
</html>", info="<html>
<p>Calculates derivative of equilibrium loading dw_dp with respect to temperature in kg/kg/K depending on equilibrium pressure p in Pa, equilibrium temperature T in K, saturation pressure p_Sat in Pa, density of adsorpt rho in kg/m&sup3;, derivative of saturation pressure with respect to temperature dp_sat_dT in Pa/K and derivative of density of adsorpt with respect to temperature drho_dT in kg/m&sup3;/K.</p>
</html>"));
end dw_dT_pTpsatRho;
