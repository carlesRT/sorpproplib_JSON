within SorpPropLib.WorkingPair.WPairFunctions.Absorption.Mixing;
function dp_dT_Tx "Function that calculates partial derivative of equilibrium pressure wrt. 
  temperature depending on molar fraction of first component in liquid phase and 
  temperature"
  extends Modelica.Icons.Function;

  //
  // Definition of inputs
  //
  input Modelica.SIunits.Temperature T
    "Equilibrium temperature"
    annotation(Dialog(tab="General", group="Inputs"));
  input Modelica.SIunits.MoleFraction x
    "Equilibrium mole fraction of refrigerant in liquid phase"
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
  external"C" dp_dT = abs_mix_dp_dT_Tx(T, x, extObj)
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
<p>Calculates derivative of equilibrium pressure with respect to temperature in Pa/K depending on temperature T_K in K and mole fraction in liquid phase x_molmol in mol/mol.</p>
<p><b>Remarks</b></p>
<p>Derivative is calculated numerically by the symmetric derivative using h = 0.0001 K as small change.</p>
</html>"));
end dp_dT_Tx;
