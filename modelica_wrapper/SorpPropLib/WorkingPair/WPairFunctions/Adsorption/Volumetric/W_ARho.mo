within SorpPropLib.WorkingPair.WPairFunctions.Adsorption.Volumetric;
function W_ARho
  "Function that calculates volumetric loading depending on adsorption potential
  and saturated liquid density"
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
  output Real W(unit="m3/kg")
    "Volumetric loading"
    annotation (Dialog(tab="General", group = "Inputs", enable = false));

  //
  // Function call
  //
  external"C" W = ads_vol_W_ARho(A, d_liq, extObj)
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
<p>Calculates equilibrium volumetric loading W in m&sup3;/kg depending on equilibrium adsorption potential A in J/mol and saturated liquid density of adsorpt rho_l in kg/m3.</p>
<h4>Remarks</h4>
<p>Saturated liquid density of adsorpt is required to calculate volumetric loading from characteristic curve in m3/kg when coefficients would directly return loading w in kg/kg. Therefore, function checks unit of coefficients by checking isotherm coefficients.</p>
</html>"));
end W_ARho;
