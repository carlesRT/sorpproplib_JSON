within SorpPropLib.DirectFunctionCalls.Adsorption.SurfaceVapor;
function T_pwpsat "Function that calculates equilibrium temperature depending on pressure and 
  loading"
  extends DirectFunctionCalls.Internal.partialDirectFunctionCall;

  //
  // Definition of inputs
  //
  input Modelica.Units.SI.Pressure p
    "Equilibrium pressure"
    annotation(Dialog(tab="General", group="Inputs"));
  input Real w(unit="kg/kg")
    "Equilibrium loading"
    annotation(Dialog(tab="General", group="Inputs"));
  input Modelica.Units.SI.Pressure p_sat
    "Vapor pressure"
    annotation(Dialog(tab="General", group="Inputs"));

  //
  // Definition of outputs
  //
  output Modelica.Units.SI.Temperature T
    "Equilibrium temperature"
    annotation (Dialog(tab="General", group = "Inputs", enable = false));

  //
  // Function call
  //
  external"C" T = direct_ads_sur_T_pwpsat(p, w, p_sat, path_db,
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
<p>Calculates equilibrium temperature in K depending on equilibrium pressure p in Pa, equilibrium loading w in kg/kg, and saturation pressure p_sat in Pa.</p>
<h4>Remarks</h4>
<p>Uses internal equation of states to calculate vapor pressure and derivative of vapor pressure wrt. temperature. If equation of state is not implemented for refrigerant, function returns -1 and throws warning. If user want to use his own equation of state, this function cannot be used and determination of root to calculate T must be implemented by user.</p>
</html>"));
end T_pwpsat;
