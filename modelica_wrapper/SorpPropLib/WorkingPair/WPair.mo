within SorpPropLib.WorkingPair;
class WPair
  "Working pair class containing constructor / destructor functions of external
  object"
  extends ExternalObject;
  extends Modelica.Icons.MaterialProperty;
  function constructor
    "Function that initializes an external working pair object"

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
    // Definition of inputs
    //
    output WPair extObj
    "External working pair object"
      annotation (Dialog(tab="General", group = "Inputs", enable = false));

    //
    // Definie external function call
    //
    external"C" extObj = newWorkingPair(path_db,
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
<p>Initialization function to create a new WorkingPair-struct. Therefore, function checks first if enough memory available to allocate WorkingPair- struct and then reads coefficients of functions from database and sets pointers within the struct depending on working pair given as input. If memory allocation fails, functions returns NULL and prints a warning. </p>
</html>"));
  end constructor;

  function destructor
    "Function that destroys an external working pair object"

    //
    // Definition of inputs
    //
    input WPair extObj
      "External working pair object"
      annotation (Dialog(tab="General", group="Inputs"));

    //
    // Definie external function call
    //
    external"C" delWorkingPair(extObj)
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
<p>Destructor function to free allocated memory of a WorkingPair-struct.</p>
</html>"));
  end destructor;
  annotation (Documentation(revisions="<html>
<ul>
  <li>
  July 13, 2021, by Mirko Engelpracht:<br/>
  First implementation.
  </li>
</ul>
</html>", info="<html>
<p>This working pair object is a reference to a C-struct that contains everythin to calculate sorption equilibrium properties.</p>
</html>"));
end WPair;
