within SorpPropLib.WorkingPair.WPairFunctions.Adsorption.Volumetric;
function dW_dA_ARho
  "Function that calculates derivative of volumetric loading wrt. adsorption 
  potential depending on adsorption potential and saturated liquid density"
  extends Modelica.Icons.Function;

  //
  // Definition of inputs
  //
  input Modelica.SIunits.MolarEnergy A
    "Adsorption potential"
    annotation(Dialog(tab="General", group="Inputs"));
  input Modelica.SIunits.Density d_liq
    "Saturated liquid density (i.e., density of adsorpt)"
    annotation(Dialog(tab="General", group="Inputs"));
  input WPair extObj "External working pair object"
    annotation (Dialog(tab="General", group="Inputs"));

  //
  // Definition of outputs
  //
  output Real dW_dA(unit="m3.mol/(kg.J)")
    "Derivative of volumetric loading wrt. adsorption potential"
    annotation (Dialog(tab="General", group = "Inputs", enable = false));

  //
  // Function call
  //
  external"C" dW_dA = ads_vol_dW_dA_ARho(A, d_liq, extObj)
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
