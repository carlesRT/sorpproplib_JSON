within SorpPropLib.WorkingPair.WPairFunctions.Absorption.Mixing;
function x_pT
  "Function that calculates equilibrium molar fraction of first component in 
  liquid phase depending on pressure and temperature"
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
  output Modelica.SIunits.MoleFraction x_comp_1
    "Equilibrium mole fraction of refrigerant in liquid phase"
    annotation (Dialog(tab="General", group = "Inputs", enable = false));
  output Modelica.SIunits.MoleFraction y_comp_1
    "Equilibrium mole fraction of refrigerant in vapor phase"
    annotation (Dialog(tab="General", group = "Inputs", enable = false));
  output Modelica.SIunits.MoleFraction y_comp_2
    "Equilibrium mole fraction of second component in vapor phase"
    annotation (Dialog(tab="General", group = "Inputs", enable = false));

  //
  // Function call
  //
  external"C" x_comp_1 = abs_mix_x_pT(y_comp_1, y_comp_2, p, T, extObj)
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
<p>Calculates equilibrium liquid mole fraction of first component in mol/mol depending on pressure in Pa and temperature T_K in K.</p>
</html>"));
end x_pT;