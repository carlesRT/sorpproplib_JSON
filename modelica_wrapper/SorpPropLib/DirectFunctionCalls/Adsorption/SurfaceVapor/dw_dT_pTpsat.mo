within SorpPropLib.DirectFunctionCalls.Adsorption.SurfaceVapor;
function dw_dT_pTpsat "Function that calculates partial derivative of equilibrium loading wrt. 
  temperature depending on pressure and temperature"
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
  input Modelica.SIunits.Pressure p_sat
    "Vapor pressure"
    annotation(Dialog(tab="General", group="Inputs"));
  input Real dp_sat_dT(unit = "Pa/K")
    "Derivative of vapor pressure wrt. temperature"
    annotation(Dialog(tab="General", group="Inputs"));

  //
  // Definition of outputs
  //
  output Real dw_dT(unit="kg/(kg.K)")
    "Derivative of equilibrium loading wrt. temperature"
    annotation (Dialog(tab="General", group = "Inputs", enable = false));

  //
  // Function call
  //
  external"C" dw_dT = direct_ads_sur_dw_dT_pTpsat(p, T, p_sat, dp_sat_dT, path_db,
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
<p>Calculates derivative of equilibrium loading w with respect to temperature T in kg/kg/K depending on equilibrium pressure p in Pa, equilibrium temperature T in K, saturation pressure p_sat in Pa, and derivative of saturation pressure wrt. temperature dp_sat_dT in Pa/K.</p>
</html>"));
end dw_dT_pTpsat;
