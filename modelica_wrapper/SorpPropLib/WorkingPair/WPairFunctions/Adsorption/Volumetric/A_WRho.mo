within SorpPropLib.WorkingPair.WPairFunctions.Adsorption.Volumetric;
function A_WRho
  "Function that calculates adsorption potential depending on volumetric loading
  and saturated liquid density"
  extends Modelica.Icons.Function;

  //
  // Definition of inputs
  //
  input Real W(unit="m3/kg")
    "Volumetric loading"
    annotation(Dialog(tab="General", group="Inputs"));
  input Modelica.SIunits.Density d_liq
    "Saturated liquid density (i.e., density of adsorpt)"
    annotation(Dialog(tab="General", group="Inputs"));
  input WPair extObj "External working pair object"
    annotation (Dialog(tab="General", group="Inputs"));

  //
  // Definition of outputs
  //
  output Modelica.SIunits.MolarEnergy A
    "Adsorption potential"
    annotation (Dialog(tab="General", group = "Inputs", enable = false));

  //
  // Function call
  //
  external"C" A = ads_vol_A_WRho(W, d_liq, extObj)
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
<p>Calculates equilibrium adsorption potential A in J/mol depending on equilibrium volumetric loading W in m&sup3;/kg and saturated liquid density of adsorpt rho_l in kg/m3.</p>
<h4>Remarks</h4>
<p>Saturated liquid density of adsorpt is required to calculate volumetric loading from characteristic curve in m3/kg when coefficients would directly return loading w in kg/kg.</p>
</html>"));
end A_WRho;
