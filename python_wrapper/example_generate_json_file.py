"""Script 'example_generate_json_file'.

This script is an example script to demonstrate how to create the JSON database from *csv-files
stored at 'python_wrapper\sorpproplib\data\JSON\ ...' (see below). The JSON database is saved
at 'python_wrapper\sorpproplib\data\JSON\'. 

Revisions:
----------
    Version     Date        Author              Changes
    1.0.0       2021-07-09  Mirko Engelpracht   First implementation
"""

# %% Import relevant modules.
#
import os

from sorpproplib.data.create_json_file import create_json_file


# %% Example script to generate JSON database from *csv-files.
#
# Set up paths
#
PATH_JSON = os.path.join(os.getcwd().split('sorpproplib_JSON')[0],
                         'sorpproplib_JSON', 'python_wrapper', 'sorpproplib', 'data',
                         'JSON')

PATH_COEFFICIENTS = os.path.join(PATH_JSON, 'equation_coefficients')
PATH_EXPERIMENTS = [os.path.join(PATH_JSON, 'experimental_data', 'ads'),
                    os.path.join(PATH_JSON, 'experimental_data', 'abs')]

# Create JSON files
#
JSON_FILE, JSON_STR_COMPLETE, JSON_STR_FORMATTED = \
    create_json_file(PATH_COEFFICIENTS, PATH_EXPERIMENTS, PATH_JSON)
