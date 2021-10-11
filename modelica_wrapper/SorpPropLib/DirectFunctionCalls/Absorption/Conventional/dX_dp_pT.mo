within SorpPropLib.DirectFunctionCalls.Absorption.Conventional;
function dX_dp_pT
  "Function that calculates partial derivative of equilibrium concentration wrt. 
  pressure depending on pressure and temperature"
  extends DirectFunctionCalls.Internal.partialDirectFunctionCall;

  //
  // Definition of inputs
  //
  input Modelica.SIunits.Pressure p
    "Equilibrium pressure"
    annotation(Dialog(tab="General", group="Inputs"));
  input Modelica.SIunits.Temperature T
    "Equilibrium temperature"
    annotation(Dialog(tab="General", group="Inputs"));

  //
  // Definition of outputs
  //
  output Real dX_dp(unit="kg/(kg.Pa)")
    "Derivative of equilibrium concentration wrt. pressure"
    annotation (Dialog(tab="General", group = "Inputs", enable = false));

  //
  // Function call
  //
  external"C" dX_dp = direct_abs_con_dX_dp_pT(p, T, path_db,
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
<p>Calculates derivative of equilibrium concentration X with respect to pressure p in kg/kg/Pa depending on equilibrium pressure p in Pa and equilibrium temperature T in K.</p>
</html>"));
end dX_dp_pT;
