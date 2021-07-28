within SorpPropLib.DirectFunctionCalls.Adsorption.Volumetric;
function T_pwpsatRho "Function that calculates equilibrium temperature depending on pressure and 
  loading"
  extends DirectFunctionCalls.Internal.partialDirectFunctionCall;

  //
  // Definition of inputs
  //
  input Modelica.SIunits.Pressure p
    "Equilibrium pressure"
    annotation(Dialog(tab="General", group="Inputs"));
  input Real w(unit="kg/kg")
    "Equilibrium loading"
    annotation(Dialog(tab="General", group="Inputs"));
  input Modelica.SIunits.Pressure p_sat
    "Vapor pressure"
    annotation(Dialog(tab="General", group="Inputs"));
  input Modelica.SIunits.Density d_liq
    "Saturated liquid density (i.e., density of adsorpt)"
    annotation(Dialog(tab="General", group="Inputs"));

  //
  // Definition of outputs
  //
  output Modelica.SIunits.Temperature T
    "Equilibrium temperature"
    annotation (Dialog(tab="General", group = "Inputs", enable = false));

  //
  // Function call
  //
  external"C" T = direct_ads_vol_T_pwpsatRho(p, w, p_sat, d_liq, path_db,
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
<p>Calculates equilibrium temperature T in K depending on equilibrium pressure in Pa, equilibrium loading w in kg/kg, saturation pressure p_Sat in Pa and density of adsorpt rho in kg/m&sup3;.</p>
<h4>Remarks</h4>
<p>Uses internal equations of states to calculate vapor pressure, saturated liquid density of adsorpt, derivative of vapor pressure wrt. temperature and derivative of saturated liquid density of adsorpt wrt. temperature. If equation of states are not implemented for refrigerant, function returns -1  and throws warning. If user want to use his own equation of states, this  function cannot be used and determination of root to calculate T must  be implemented by user.</p>
</html>"));
end T_pwpsatRho;
