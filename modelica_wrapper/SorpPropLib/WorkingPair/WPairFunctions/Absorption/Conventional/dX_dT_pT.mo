within SorpPropLib.WorkingPair.WPairFunctions.Absorption.Conventional;
function dX_dT_pT "Function that calculates partial derivative of equilibrium concentration wrt. 
  temperature depending on pressure and temperature"
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
  output Real dX_dT(unit="kg/(kg.K)")
    "Derivative of equilibrium concentration wrt. temperature"
    annotation (Dialog(tab="General", group = "Inputs", enable = false));

  //
  // Function call
  //
  external"C" dX_dT = abs_con_dX_dT_pT(p, T, extObj)
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
<p>Calculates derivative of equilibrium concentration X with respect to temperature T in kg/kg/K depending on equilibrium pressure p in Pa and equilibrium temperature T in K.</p>
</html>"));
end dX_dT_pT;
