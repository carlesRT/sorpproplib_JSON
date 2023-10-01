within SorpPropLib.DirectFunctionCalls.Adsorption.SurfaceVapor;
function dw_dp_pTpsat "Function that calculates partial derivative of equilibrium loading wrt. 
  pressure depending on pressure and temperature"
  extends DirectFunctionCalls.Internal.partialDirectFunctionCall;

  //
  // Definition of inputs
  //
  input Modelica.Units.SI.Pressure p
    "Equilibrium pressure"
    annotation(Dialog(tab="General", group="Inputs"));
  input Modelica.Units.SI.Temperature T
    "Equilibrium temperature"
    annotation(Dialog(tab="General", group="Inputs"));
  input Modelica.Units.SI.Pressure p_sat
    "Vapor pressure"
    annotation(Dialog(tab="General", group="Inputs"));

  //
  // Definition of outputs
  //
  output Real dw_dp(unit="kg/(kg.Pa)")
    "Derivative of equilibrium loading wrt. pressure"
    annotation (Dialog(tab="General", group = "Inputs", enable = false));

  //
  // Function call
  //
  external"C" dw_dp = direct_ads_sur_dw_dp_pTpsat(p, T, p_sat, path_db,
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
<p>Calculates derivative of equilibrium loading w with respect to pressure p in kg/kg/Pa depending on equilibrium pressure p in Pa, equilibrium temperature T in K, and saturation pressure p_sat in Pa.</p>
</html>"));
end dw_dp_pTpsat;
