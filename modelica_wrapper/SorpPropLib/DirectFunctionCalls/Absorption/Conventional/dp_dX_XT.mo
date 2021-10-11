within SorpPropLib.DirectFunctionCalls.Absorption.Conventional;
function dp_dX_XT "Function that calculates partial derivative of equilibrium pressure wrt. 
  concentration depending on concentration and temperature"
  extends DirectFunctionCalls.Internal.partialDirectFunctionCall;

  //
  // Definition of inputs
  //
  input Real X(unit="kg/kg")
    "Equilibrium concentration"
    annotation(Dialog(tab="General", group="Inputs"));
  input Modelica.SIunits.Temperature T
    "Equilibrium temperature"
    annotation(Dialog(tab="General", group="Inputs"));

  //
  // Definition of outputs
  //
  output Real dp_dX(unit = "(Pa.kg)/kg")
    "Derivative of equilibrium pressure wrt. concentration"
    annotation (Dialog(tab="General", group = "Inputs", enable = false));

  //
  // Function call
  //
  external"C" dp_dX = direct_abs_con_dp_dX_XT(X, T, path_db,
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
<p>Calculates derivative of equilibrium pressure p with respect to concentration w in kgPa/kg depending on equilibrium concentration X in kg/kg and equilibrium temperature T in K.</p>
</html>"));
end dp_dX_XT;
