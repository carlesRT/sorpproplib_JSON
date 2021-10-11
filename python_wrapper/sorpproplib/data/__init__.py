"""Package 'data' contains functions to create JSON data base.

Extended Summary
----------------
This module creates the JSON data base, which contains equation coefficients and experimental data.
For this purpose, *.csv-files are used that contain either equation coefficients or experimental
data. The *.csv-files are loaded, converted into pandas data frames, edited, and saved as nested
list/dict-structure. Finally, the nested structure is saved as JSON file using the built-in module
json.


Routine Listings
----------------
create_json_file:
    Create JSON data base containing equation coefficients and experimental data.


Revisions
---------
    Version     Date        Author              Changes
    1.0.0       2021-06-10  Mirko Engelpracht   First implementation
"""

# %% Definition of local references.
#
__all__ = []
