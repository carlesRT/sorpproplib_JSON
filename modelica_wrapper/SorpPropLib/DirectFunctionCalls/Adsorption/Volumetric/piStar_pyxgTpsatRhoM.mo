within SorpPropLib.DirectFunctionCalls.Adsorption.Volumetric;
function piStar_pyxgTpsatRhoM "Calculates reduced spreading pressure depending on total pressure, molar 
  fractions in vapor and adsorbed phase, activity coefficient in adsorbed phase, 
  temperature, and molar mass of adsorptive"
  extends DirectFunctionCalls.Internal.partialDirectFunctionCall;

  //
  // Definition of inputs
  //
  input Modelica.Units.SI.Pressure p_total
    "Total pressure"
    annotation(Dialog(tab="General", group="Inputs"));
  input Real y(unit = "mol/mol")
    "Molar fraction of adsorptive in vapor phase"
    annotation(Dialog(tab="General", group="Inputs"));
  input Real x(unit = "mol/mol")
    "Molar fraction of adsorptive in adsorbed phase"
    annotation(Dialog(tab="General", group="Inputs"));
  input Real g(unit = "1")
    "Activity coefficient of adsorptive in adsorbed phase"
    annotation(Dialog(tab="General", group="Inputs"));
  input Modelica.Units.SI.Temperature T
    "Equilibrium temperature"
    annotation(Dialog(tab="General", group="Inputs"));
  input Modelica.Units.SI.Pressure p_sat
    "Vapor pressure"
    annotation(Dialog(tab="General", group="Inputs"));
  input Modelica.Units.SI.Density d_liq
    "Saturated liquid density (i.e., density of adsorpt)"
    annotation(Dialog(tab="General", group="Inputs"));
  input Modelica.Units.SI.MolarMass M
    "Molar mass of adsorptive"
    annotation(Dialog(tab="General", group="Inputs"));

  //
  // Definition of outputs
  //
  output Real piStar(unit = "kg/mol")
    "Reduced spreading pressure"
    annotation (Dialog(tab="General", group = "Inputs", enable = false));

  //
  // Function call
  //
  external"C" piStar = direct_ads_vol_piStar_pyxgTpsatRhoM(
    p_total, y, x, g, T, p_sat, d_liq, M, path_db,
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
<p>Calculates reduced spreading pressure in kg/mol depending on equilibrium pressure p in Pa, molar fraction of refrigerant in vapor phase in mol/mol, molar fraction of refrigerant in adsorbed phase in mol/mol, activity coefficient of refrigerant in adsorbed phase, equilibrium temperature T in K, saturation pressure p_Sat in Pa and density of adsorpt rho in kg/m&sup3; and molar mass of refrigerant M in kg/mol.</p>
<p>The reduced spreading pressure is defined as follows:</p>
<p>piStar = A * pi / (R * T * m_sorbent) = 1 / M * Integral_0^p0{w(p,T) / p * dp}, where p0 = p_total*y / (gamma*x)</p>
</html>"));
end piStar_pyxgTpsatRhoM;
