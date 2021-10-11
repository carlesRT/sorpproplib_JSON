within SorpPropLib.DirectFunctionCalls.Absorption.Activity;
function p_Txv1v2psat
  "Function that calculates equilibrium pressure depending on temperature, molar
  molar fraction of refrigerant in liquid pahse, saturated liquid molar volumes 
  of both components, and vapor pressure of first component"
  extends DirectFunctionCalls.Internal.partialDirectFunctionCall;

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
  input Modelica.SIunits.Pressure p_sat_comp1
    "Vapor pressure of first component"
    annotation(Dialog(tab="General", group="Inputs"));

  //
  // Definition of outputs
  //
  output Modelica.SIunits.Pressure p
    "Equilibrium pressure"
    annotation (Dialog(tab="General", group = "Inputs", enable = false));

  //
  // Function call
  //
  external"C" p = direct_abs_act_p_Txv1v2psat(T, x, v_m_liq_comp1, v_m_liq_comp2,
    p_sat_comp1, path_db,
    name_sorbent, name_sorbent_sub_type, name_refrigerant,
    func_isotherm, func_isotherm_ID,
    func_vapor_pressure, func_vapor_pressure_ID,
    func_saturated_liquid_density, func_saturated_liquid_density_ID)
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
<p>Calculates equilibrium pressure p_Pa in Pa of first component depending on temperature T_K in K, mole fraction in liquid phase x_molmol in mol/mol, molar volume of first component in m&sup3;/mol, molar volume of second component in m&sup3;/mol,and saturation pressure of first component p_sat_Pa in Pa.</p>
<p><b>Remarks</b></p>
<p>Molar volumes may are not required and ignored. When molar volumes are required, function uses molar volumes stored in JSON file when input v1_m3mol or v2_m3mol is -1. Otherwise, function uses molar volumes given by inputs.</p>
</html>"));
end p_Txv1v2psat;
