within SorpPropLib.DirectFunctionCalls.Adsorption.Volumetric;
function dp_dw_wTpsatRho "Function that calculates partial derivative of equilibrium pressure wrt. 
  loading depending on loading and temperature"
  extends DirectFunctionCalls.Internal.partialDirectFunctionCall;

  //
  // Definition of inputs
  //
  input Real w(unit="kg/kg")
    "Equilibrium loading"
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

  //
  // Definition of outputs
  //
  output Real dp_dw(unit = "(Pa.kg)/kg")
    "Derivative of equilibrium pressure wrt. loading"
    annotation (Dialog(tab="General", group = "Inputs", enable = false));

  //
  // Function call
  //
  external"C" dp_dw = direct_ads_vol_dp_dw_wTpsatRho(w, T, p_sat, d_liq, path_db,
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
<p>Calculates derivative of equilibrium pressure p with respect to loading w in kgPa/kg depending on equilibrium loading w in kg/kg, equilibrium temperature T in K, saturation pressure p_Sat in Pa and density of adsorpt rho in kg/m&sup3;.</p>
</html>"));
end dp_dw_wTpsatRho;
