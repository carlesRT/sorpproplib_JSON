within SorpPropLib.WorkingPair.WPairFunctions.Absorption.Mixing;
function p_Tx "Function that calculates equilibrium pressure depending on molar fraction of 
  first component in liquid phase and temperature"
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
  output Modelica.SIunits.Pressure p
    "Equilibrium pressure"
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
  external"C" p = abs_mix_p_Tx(y_comp_1, y_comp_2, T, x, extObj)
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
<p>Calculates equilibrium pressure p_Pa in Pa depending on temperature T_K in K and mole fraction in liquid phase x_molmol in mol/mol.</p>
</html>"));
end p_Tx;
