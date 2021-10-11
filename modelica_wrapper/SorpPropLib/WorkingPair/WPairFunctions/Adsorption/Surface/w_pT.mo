within SorpPropLib.WorkingPair.WPairFunctions.Adsorption.Surface;
function w_pT
  "Function that calculates equilibrium loading depending on pressure and 
  temperature"
  extends Modelica.Icons.Function;

  //
  // Definition of inputs
  //
  input Modelica.SIunits.Pressure p
    "Equilibrium pressure"
    annotation(Dialog(tab="General", group="Inputs"));
  input Modelica.SIunits.Temperature T
    "Equilibrium temperature"
    annotation(Dialog(tab="General", group="Inputs"));
  input WPair extObj "External working pair object"
    annotation (Dialog(tab="General", group="Inputs"));

  //
  // Definition of outputs
  //
  output Real w(unit="kg/kg")
    "Equilibrium loading"
    annotation (Dialog(tab="General", group = "Inputs", enable = false));

  //
  // Function call
  //
  external"C" w = ads_sur_w_pT(p, T, extObj)
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
<p>Calculates equilibrium loading w in kg/kg depending on equilibrium pressure p in Pa and equilibrium temperature T in K.</p>
</html>"));
end w_pT;
