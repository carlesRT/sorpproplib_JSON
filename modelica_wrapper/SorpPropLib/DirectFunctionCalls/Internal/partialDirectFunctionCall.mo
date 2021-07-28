within SorpPropLib.DirectFunctionCalls.Internal;
partial function partialDirectFunctionCall
  "Partial function for all functions that are based on direct calls of the
  SorpPropLib (i.e., without a working pair object)"
  extends Modelica.Icons.Function;

  //
  // Definition of inputs
  //
  input String path_db
    "Path to database, i.e., JSON-file of database."
    annotation (Dialog(tab="General", group = "Inputs"));

  input String name_sorbent
    "Name of sorbent, e.g., 'zeolite pellet'"
    annotation (Dialog(tab="General", group = "Inputs"));
  input String name_sorbent_sub_type
    "Sub-type of sorbent, e.g., '13X'"
    annotation (Dialog(tab="General", group = "Inputs"));
  input String name_refrigerant
    "Name of refrigerant, e.g., 'Water'"
    annotation (Dialog(tab="General", group = "Inputs"));

  input String func_isotherm
    "Name of functional approach for isotherm, e.g., 'Toth'"
    annotation (Dialog(tab="General", group = "Inputs"));
  input Integer func_isotherm_ID
    "ID of functional approach for isotherm, e.g., '1'"
    annotation (Dialog(tab="General", group = "Inputs"));

  input String func_vapor_pressure
    "Name of functional approach for vapor pressure, e.g., 'VaporPressure_EoS1'"
    annotation (Dialog(tab="General", group = "Inputs"));
  input Integer func_vapor_pressure_ID
    "ID of functional approach for vapor pressure, e.g., '1'"
    annotation (Dialog(tab="General", group = "Inputs"));

  input String func_saturated_liquid_density
    "Name of functional approach for saturated liquid density, e.g., 'SaturatedLiquidDensity_EoS1'"
    annotation (Dialog(tab="General", group = "Inputs"));
  input Integer func_saturated_liquid_density_ID
    "ID of functional approach for saturated liquid density, e.g., '1'"
    annotation (Dialog(tab="General", group = "Inputs"));


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
</html>",
        info="<html>
</html>"));
end partialDirectFunctionCall;
