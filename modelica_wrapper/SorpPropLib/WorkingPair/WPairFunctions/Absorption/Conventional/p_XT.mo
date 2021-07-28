within SorpPropLib.WorkingPair.WPairFunctions.Absorption.Conventional;
function p_XT
  "Function that calculates equilibrium pressure depending on concentration and 
  temperature"
  extends Modelica.Icons.Function;

  //
  // Definition of inputs
  //
  input Real X(unit="kg/kg")
    "Equilibrium concentration"
    annotation(Dialog(tab="General", group="Inputs"));
  input Modelica.SIunits.Temperature T
    "Equilibrium temperature"
    annotation(Dialog(tab="General", group="Inputs"));
  input WPair extObj "External working pair object"
    annotation (Dialog(tab="General", group="Inputs"));

  //
  // Definition of outputs
  //
  output Modelica.SIunits.Pressure p
    "Equilibrium pressure"
    annotation (Dialog(tab="General", group = "Inputs", enable = false));

  //
  // Function call
  //
  external"C" p = abs_con_p_XT(X, T, extObj)
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
<p>Calculates equilibrium pressure p in Pa depending on equilibrium concentration X in kg/kg and equilibrium temperature T in K.</p>
</html>"));
end p_XT;
