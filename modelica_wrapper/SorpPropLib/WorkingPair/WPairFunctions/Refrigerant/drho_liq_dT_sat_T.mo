within SorpPropLib.WorkingPair.WPairFunctions.Refrigerant;
function drho_liq_dT_sat_T
  "Function that calculates partial derivative of saturated liquid density wrt.
  temperature depending on temperature"
  extends Modelica.Icons.Function;

  //
  // Definition of inputs
  //
  input Modelica.SIunits.Temperature T_sat
    "Saturation temperature"
    annotation(Dialog(tab="General", group="Inputs"));
  input WPair extObj "External working pair object"
    annotation (Dialog(tab="General", group="Inputs"));

  //
  // Definition of outputs
  //
  output Real drho_liq_dT_sat(unit = "kg/(m3.K)")
    "Derivative of saturated liquid density wrt. temperature"
    annotation (Dialog(tab="General", group = "Inputs", enable = false));

  //
  // Function call
  //
  external"C" drho_liq_dT_sat = ref_drho_l_dT_T(T_sat, extObj)
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
<p>Calculates derivative of saturated liquid density wrt temperature in kg/m&sup3;/K depending on equilibrium temperature T in K.</p>
</html>"));
end drho_liq_dT_sat_T;
