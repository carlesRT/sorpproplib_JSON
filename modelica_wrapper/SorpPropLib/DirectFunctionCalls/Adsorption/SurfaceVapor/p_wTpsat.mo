within SorpPropLib.DirectFunctionCalls.Adsorption.SurfaceVapor;
function p_wTpsat "Function that calculates equilibrium pressure depending on loading and 
  temperature"
  extends DirectFunctionCalls.Internal.partialDirectFunctionCall;

  //
  // Definition of inputs
  //
  input Real w(unit="kg/kg")
    "Equilibrium loading"
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
  output Modelica.Units.SI.Pressure p
    "Equilibrium pressure"
    annotation (Dialog(tab="General", group = "Inputs", enable = false));

  //
  // Function call
  //
  external"C" p = direct_ads_sur_p_wTpsat(w, T, p_sat, path_db,
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
<p>Calculates equilibrium pressure p in Pa depending on equilibrium loading w in kg/kg, equilibrium temperature T in K, and saturation pressure p_sat in Pa.</p>
</html>"));
end p_wTpsat;
