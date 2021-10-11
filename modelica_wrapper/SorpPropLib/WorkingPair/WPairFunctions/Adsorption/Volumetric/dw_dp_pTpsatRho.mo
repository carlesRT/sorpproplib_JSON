within SorpPropLib.WorkingPair.WPairFunctions.Adsorption.Volumetric;
function dw_dp_pTpsatRho "Function that calculates partial derivative of equilibrium loading wrt. 
  pressure depending on pressure and temperature"
  extends Modelica.Icons.Function;

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
  input Modelica.SIunits.Density d_liq
    "Saturated liquid density (i.e., density of adsorpt)"
    annotation(Dialog(tab="General", group="Inputs"));
  input WPair extObj "External working pair object"
    annotation (Dialog(tab="General", group="Inputs"));

  //
  // Definition of outputs
  //
  output Real dw_dp(unit="kg/(kg.Pa)")
    "Derivative of equilibrium loading wrt. pressure"
    annotation (Dialog(tab="General", group = "Inputs", enable = false));

  //
  // Function call
  //
  external"C" dw_dp = ads_vol_dw_dp_pTpsatRho(p, T, p_sat, d_liq, extObj)
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
<p>Calculates derivative of equilibrium loading dw_dp with respect to pressure in kg/kg/Pa depending on equilibrium pressure p in Pa, equilibrium temperature T in K, saturation pressure p_Sat in Pa and density of adsorpt rho in kg/m&sup3;.</p>
</html>"));
end dw_dp_pTpsatRho;
