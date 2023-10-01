within SorpPropLib.DirectFunctionCalls.Adsorption.Volumetric;
function dW_dA_ARho
  "Function that calculates derivative of volumetric loading wrt. adsorption 
  potential depending on adsorption potential and saturated liquid density"
  extends DirectFunctionCalls.Internal.partialDirectFunctionCall;

  //
  // Definition of inputs
  //
  input Modelica.Units.SI.MolarEnergy A
    "Adsorption potential"
    annotation(Dialog(tab="General", group="Inputs"));
  input Modelica.Units.SI.Density d_liq
    "Saturated liquid density (i.e., density of adsorpt)"
    annotation(Dialog(tab="General", group="Inputs"));

  //
  // Definition of outputs
  //
  output Real dW_dA(unit="m3.mol/(kg.J)")
    "Derivative of volumetric loading wrt. adsorption potential"
    annotation (Dialog(tab="General", group = "Inputs", enable = false));

  //
  // Function call
  //
  external"C" dW_dA = direct_ads_vol_dW_dA_ARho(A, d_liq, path_db,
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
<p>Calculates derivative of equilibrium volumetric loading dW_dA in m&sup3;mol/kg/J depending on equilibrium adsorption potential A in J/mol and saturated liquid density of adsorpt rho_l in kg/m3.</p>
<h4>Remarks</h4>
<p>Saturated liquid density of adsorpt is required to calculate volumetric loading from characteristic curve in m3/kg when coefficients would directly return loading w in kg/kg. Therefore, function checks unit of coefficients by checking isotherm coefficients.</p>
</html>"));
end dW_dA_ARho;
