within SorpPropLib.DirectFunctionCalls.Refrigerant;
function dp_dT_sat_T
  "Function that calculates partial derivative of vapor pressure wrt. 
  temperature depending on temperature"
  extends DirectFunctionCalls.Internal.partialDirectFunctionCall;

  //
  // Definition of inputs
  //
  input Modelica.SIunits.Temperature T_sat
    "Saturation temperature"
    annotation(Dialog(tab="General", group="Inputs"));

  //
  // Definition of outputs
  //
  output Real dp_dT_sat(unit = "Pa/K")
    "Derivative of pressure wrt. temperature"
    annotation (Dialog(tab="General", group = "Inputs", enable = false));

  //
  // Function call
  //
  external"C" dp_dT_sat = direct_ref_dp_sat_dT_T(T_sat, path_db,
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
</html>",
        info="<html>
<p>Calculates derivative of saturation pressure in Pa/K wrt to temperature depending on equilibrium temperature T in K.</p>
</html>"));
end dp_dT_sat_T;
