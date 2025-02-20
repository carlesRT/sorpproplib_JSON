within SorpPropLib.WorkingPair.WPairFunctions.Adsorption.General;
function piStar_pyxgTM
  "Calculates reduced spreading pressure depending on total pressure, molar 
  fractions in vapor and adsorbed phase, activity coefficient in adsorbed phase, 
  temperature, and molar mass of adsorptive"
  extends Modelica.Icons.Function;

  //
  // Definition of inputs
  //
  input Modelica.SIunits.Pressure p_total
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
  input Modelica.SIunits.Temperature T
    "Equilibrium temperature"
    annotation(Dialog(tab="General", group="Inputs"));
  input Modelica.SIunits.MolarMass M
    "Molar mass of adsorptive"
    annotation(Dialog(tab="General", group="Inputs"));
  input WPair extObj "External working pair object"
    annotation (Dialog(tab="General", group="Inputs"));

  //
  // Definition of outputs
  //
  output Real piStar(unit = "kg/mol")
    "Reduced spreading pressure"
    annotation (Dialog(tab="General", group = "Inputs", enable = false));

  //
  // Function call
  //
  external"C" piStar = ads_piStar_pyxgTM(p_total, y, x, g, T, M, extObj)
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
<p>Calculates reduced spreading pressure piStar in kg/mol depending on equilibrium pressure p in Pa, molar fraction of refrigerant in vapor phase in mol/mol, molar fraction of refrigerant in adsorbed phase in mol/mol, activity coefficient of refrigerant in adsorbed phase, equilibrium temperature T in K and molar mass of refrigerant M in kg/mol.</p>
<p>The reduced spreading pressure is defined as follows:</p>
<p>piStar = A * pi / (R * T * m_sorbent) = 1 / M * Integral_0^p0{w(p,T) / p * dp}, where p0 = p_total*y / (gamma*x)</p>
</html>"));
end piStar_pyxgTM;
