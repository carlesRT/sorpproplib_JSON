within SorpPropLib.DirectFunctionCalls.Adsorption.General;
function T_pw
  "Function that calculates equilibrium temperature depending on pressure and 
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

  //
  // Definition of outputs
  //
  output Modelica.Units.SI.Temperature T
    "Equilibrium temperature"
    annotation (Dialog(tab="General", group = "Inputs", enable = false));

  //
  // Function call
  //
  external"C" T = direct_ads_T_pw(p, w, path_db,
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
<p>Calculates equilibrium temperature in K depending on equilibrium pressure p in Pa and equilibrium loading w in kg/kg.</p>
<h4>Remarks</h4>
<p>Function uses equation of states implemented into SorpPropLib for calculation of vapor pressure, saturated liquid density, derivative of vapor pressure wrt. temperature, and derivative of saturated liquid density wrt. temperature when isotherm of working pair is based on volumetric approach (e.g., Dubinin theory). As initial guess value is fixed, function may not converge.</p>
</html>"));
end T_pw;
