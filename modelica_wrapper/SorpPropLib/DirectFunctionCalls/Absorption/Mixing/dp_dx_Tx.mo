within SorpPropLib.DirectFunctionCalls.Absorption.Mixing;
function dp_dx_Tx "Function that calculates partial derivative of equilibrium pressure wrt. 
  loading depending on molar fraction of first component in liquid phase and 
  temperature"
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

  //
  // Definition of outputs
  //
  output Real dp_dx(unit = "(Pa.mol)/mol")
    "Derivative of equilibrium pressure wrt. molar fraction"
    annotation (Dialog(tab="General", group = "Inputs", enable = false));

  //
  // Function call
  //
  external"C" dp_dx = direct_abs_mix_dp_dx_Tx(T, x, path_db,
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
<p>Calculates derivative of equilibrium pressure with respect to liquid mole fraction in Pa depending on temperature T_K in K and mole fraction in liquid phase x_molmol in mol/mol.</p>
<p><b>Remarks</b></p>
<p>Derivative is calculated numerically by the symmetric derivative using h = 0.00000001 mol/mol as small change.</p>
</html>"));
end dp_dx_Tx;
