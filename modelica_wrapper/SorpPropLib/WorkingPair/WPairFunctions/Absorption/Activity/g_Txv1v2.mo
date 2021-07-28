within SorpPropLib.WorkingPair.WPairFunctions.Absorption.Activity;
function g_Txv1v2
  "Function that calculates activity coefficient of the refrigerant depending on
  temperature, molar fraction of refrigerant in liquid pahse, saturated liquid
  molar volumes of both components"
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
  input Modelica.SIunits.MolarVolume v_m_liq_comp1(min=-1)
    "Saturated liquid molar volume of first component (i.e., refrigerant)"
    annotation(Dialog(tab="General", group="Inputs"));
  input Modelica.SIunits.MolarVolume v_m_liq_comp2(min=-1)
    "Saturated liquid molar volume of second component"
    annotation(Dialog(tab="General", group="Inputs"));
  input WPair extObj "External working pair object"
    annotation (Dialog(tab="General", group="Inputs"));

  //
  // Definition of outputs
  //
  output Real g(unit="1")
    "Activity coefficient of first component (i.e., refrigerant)"
    annotation (Dialog(tab="General", group = "Inputs", enable = false));

  //
  // Function call
  //
  external"C" g = abs_act_g_Txv1v2(T, x, v_m_liq_comp1, v_m_liq_comp2, extObj)
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
<p>Calculates activity coefficient of first component depending on temperature T_K in K, mole fraction in liquid phase x_molmol in mol/mol, molar volume of first component in m&sup3;/mol, and molar volume of second component in m&sup3;/mol.</p>
<p><b>Remarks</b></p>
<p>Molar volumes may are not required and ignored. When molar volumes are required, function uses molar volumes stored in JSON file when input v1_m3mol or v2_m3mol is -1. Otherwise, function uses molar volumes given by inputs.</p>
</html>"));
end g_Txv1v2;
