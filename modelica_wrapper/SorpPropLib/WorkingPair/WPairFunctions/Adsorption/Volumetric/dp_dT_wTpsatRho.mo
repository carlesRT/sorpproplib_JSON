within SorpPropLib.WorkingPair.WPairFunctions.Adsorption.Volumetric;
function dp_dT_wTpsatRho "Function that calculates partial derivative of equilibrium pressure wrt. 
  temperature depending on loading and temperature"
  extends Modelica.Icons.Function;

  //
  // Definition of inputs
  //
  input Real w(unit="kg/kg")
    "Equilibrium loading"
    annotation(Dialog(tab="General", group="Inputs"));
  input Modelica.SIunits.Temperature T
    "Equilibrium temperature"
    annotation(Dialog(tab="General", group="Inputs"));
  input Modelica.SIunits.Pressure p_sat
    "Vapor pressure"
    annotation(Dialog(tab="General", group="Inputs"));
  input Modelica.SIunits.Density d_liq
    "Saturated liquid density (i.e., density of adsorpt)"
    annotation(Dialog(tab="General", group="Inputs"));
  input Real dp_sat_dT(unit = "Pa/K")
    "Derivative of vapor pressure wrt. temperature"
    annotation(Dialog(tab="General", group="Inputs"));
  input Real d_liq_dT(unit = "kg/(m3.K)")
    "Derivative of saturated liquid density (i.e., density of adsorpt) wrt. temperature"
    annotation(Dialog(tab="General", group="Inputs"));
  input WPair extObj "External working pair object"
    annotation (Dialog(tab="General", group="Inputs"));

  //
  // Definition of outputs
  //
  output Real dp_dT(unit = "Pa/K")
    "Derivative of equilibrium pressure wrt. temperature"
    annotation (Dialog(tab="General", group = "Inputs", enable = false));

  //
  // Function call
  //
  external"C" dp_dT = ads_vol_dp_dT_wTpsatRho(w, T, p_sat, d_liq,
    dp_sat_dT, d_liq_dT, extObj)
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
<p>Calculates derivative of equilibrium pressure p with respect to temperature T in kg/kg/K depending on equilibrium loading w in kg/kg, equilibrium temperature T in K, saturation pressure p_Sat in Pa and density of adsorpt rho in kg/m&sup3;, derivative of saturation pressure with respect to temperature dp_sat_dT in Pa/K and derivative of density of adsorpt with respect to temperature drho_dT in kg/m&sup3;/K.</p>
</html>"));
end dp_dT_wTpsatRho;
