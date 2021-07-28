within SorpPropLib.DirectFunctionCalls.Absorption.Mixing;
function T_px "Function that calculates equilibrium temperature depending on pressure and 
  molar fraction of first component in liquid phase"
  extends DirectFunctionCalls.Internal.partialDirectFunctionCall;

  //
  // Definition of inputs
  //
  input Modelica.SIunits.Pressure p
    "Equilibrium pressure"
    annotation(Dialog(tab="General", group="Inputs"));
  input Modelica.SIunits.MoleFraction x
    "Equilibrium mole fraction of refrigerant in liquid phase"
    annotation(Dialog(tab="General", group="Inputs"));

  //
  // Definition of outputs
  //
  output Modelica.SIunits.Temperature T
    "Equilibrium temperature"
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
  external"C" T = direct_abs_mix_T_px(y_comp_1, y_comp_2, p, x, path_db,
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
<p>Calculates equilibrium temperature T_K in K depending on pressure p_Pa in Pa and mole fraction in liquid phase x_molmol in mol/mol.</p>
</html>"));
end T_px;
