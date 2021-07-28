within SorpPropLib.WorkingPair.WPairFunctions.Adsorption.Surface;
function dp_dT_wT
  "Function that calculates partial derivative of equilibrium pressure wrt. 
  temperature depending on loading and temperature"
  extends Modelica.Icons.Function;

  //
  // Definition of inputs
  //
  input Real w(unit="kg/kg")
    "Equilibrium loading"
    annotation(Dialog(tab="General", group="Inputs"));
  input Modelica.SIunits.Temperature T
    "Equilibrium temperature"
    annotation(Dialog(tab="General", group="Inputs"));
  input WPair extObj "External working pair object"
    annotation (Dialog(tab="General", group="Inputs"));

  //
  // Definition of outputs
  //
  output Real dp_dT(unit = "Pa/K")
    "Derivative of equilibrium pressure wrt. temperature"
    annotation (Dialog(tab="General", group = "Inputs", enable = false));

  //
  // Function call
  //
  external"C" dp_dT = ads_sur_dp_dT_wT(w, T, extObj)
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
<p>Calculates derivative of equilibrium pressure p with respect to temperature T in kg/kg/K depending on equilibrium loading w in kg/kg and equilibrium temperature T in K.</p>
</html>"));
end dp_dT_wT;
