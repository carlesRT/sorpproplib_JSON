"""Script 'example_generate_content_list'.

This script is an example script to generate data frames containing all approaches implemented
for a) vapor pressures, b) saturated liquid densities, c) adsorption isotherms, d) absorption
isotherms. Data frames are saved at top-level of the SorpPropLib as *.xlsx-files.

Revisions:
----------
    Version     Date        Author              Changes
    1.0.0       2021-07-13  Mirko Engelpracht   First implementation
"""

# %% Import relevant modules.
#
import pandas as pd
import os

from json import load


# %% Setup.
#
# Set up paths depending on plattform and architecture
#
PATH_SORPPROPLIB = os.path.join(os.getcwd().split("sorpproplib_JSON")[0], 'sorpproplib_JSON')
PATH_DB = os.path.join(PATH_SORPPROPLIB, 'python_wrapper', 'sorpproplib',
                       'data', 'JSON', 'sorpproplib.json').encode()


# %% Generate data frames containing contents of the SorpPropLib.
#
# Prepare data frames and load data base
#
df_vapor_pressure = pd.DataFrame(columns=['refrigerant',
                                          'name of equation',
                                          'ID of equation',
                                          'remarks',
                                          'reference'])

df_saturated_liquid_density = pd.DataFrame(columns=['refrigerant',
                                                    'name of equation',
                                                    'ID of equation',
                                                    'remarks',
                                                    'reference'])

df_ads_working_pairs = pd.DataFrame(columns=['refrigerant',
                                             'sorbent',
                                             'subtype',
                                             'name of equation',
                                             'ID of equation',
                                             'remarks',
                                             'reference'])

df_abs_working_pairs = pd.DataFrame(columns=['refrigerant',
                                             'sorbent',
                                             'subtype',
                                             'name of equation',
                                             'ID of equation',
                                             'remarks',
                                             'reference'])


# Get unique refrigerants, adsorption working pairs, and absorption working pairs
#
with open(PATH_DB, "r") as read_file:
    JSON_FILE = load(read_file)

list_refrigerants = []
list_ads_working_pairs = []
list_abs_working_pairs = []

for _, entry in enumerate(JSON_FILE):
    if entry['k']['_tp_'] == 'refrig':
        list_refrigerants.append(entry)

    elif entry['k']['_tp_'] == 'ads':
        list_ads_working_pairs.append(entry)

    elif entry['k']['_tp_'] == 'abs':
        list_abs_working_pairs.append(entry)

# Get approaches implemented for refrigerants
#
for _, entry in enumerate(list_refrigerants):
    approaches = entry['v']['_ep_']

    approaches_names = list(approaches.keys())
    if 'Refrigerants' in approaches_names:
        approaches_names.remove('Refrigerants')

    # Iterate over appraoches
    #
    for approach_name in approaches_names:
        # Check for equation type: Vapor pressure or saturated liquid density
        # Then, iterate over all entries and add to corresponding data frame
        #
        if approach_name.split('_')[0] == 'VaporPressure':
            for approach_name_id, approach_name_i in enumerate(approaches[approach_name]):
                tmp_dict = {'refrigerant':        entry['k']['_rf_'],
                            'name of equation':   approach_name,
                            'ID of equation':     approach_name_id + 1,
                            'remarks':            approach_name_i['_n_'],
                            'reference':          approach_name_i['_c_']}

                df_vapor_pressure = \
                    df_vapor_pressure.append(tmp_dict, ignore_index = True)

        else:
            for approach_name_id, approach_name_i in enumerate(approaches[approach_name]):
                tmp_dict = {'refrigerant':        entry['k']['_rf_'],
                            'name of equation':   approach_name,
                            'ID of equation':     approach_name_id + 1,
                            'remarks':            approach_name_i['_n_'],
                            'reference':          approach_name_i['_c_']}

                df_saturated_liquid_density = \
                    df_saturated_liquid_density.append(tmp_dict, ignore_index = True)

# Get approaches implemented for adsorption working pairs
#
for _, entry in enumerate(list_ads_working_pairs):
    approaches = entry['v']['_ep_']
    approaches_names = list(approaches.keys())

    # Iterate over appraoches and entries within approaches
    #
    for approach_name in approaches_names:
        for approach_name_id, approach_name_i in enumerate(approaches[approach_name]):
            tmp_dict = {'refrigerant':          entry['k']['_rf_'],
                        'sorbent':              entry['k']['_as_'],
                        'subtype':              entry['k']['_st_'],
                        'name of equation':     approach_name,
                        'ID of equation':       approach_name_id + 1,
                        'remarks':              approach_name_i['_n_'],
                        'reference':            approach_name_i['_c_']}

            df_ads_working_pairs = \
                df_ads_working_pairs.append(tmp_dict, ignore_index = True)

# Get approaches implemented for absorption working pairs
#
for _, entry in enumerate(list_abs_working_pairs):
    approaches = entry['v']['_ep_']
    approaches_names = list(approaches.keys())

    # Iterate over appraoches and entries within approaches
    #
    for approach_name in approaches_names:
        for approach_name_id, approach_name_i in enumerate(approaches[approach_name]):
            tmp_dict = {'refrigerant':          entry['k']['_rf_'],
                        'sorbent':              entry['k']['_as_'],
                        'subtype':              entry['k']['_st_'],
                        'name of equation':     approach_name,
                        'ID of equation':       approach_name_id + 1,
                        'remarks':              approach_name_i['_n_'],
                        'reference':            approach_name_i['_c_']}

            df_abs_working_pairs = \
                df_abs_working_pairs.append(tmp_dict, ignore_index = True)

# Sort lists and save them
#
df_vapor_pressure = \
    df_vapor_pressure.sort_values(['refrigerant', 'name of equation', 'ID of equation'])
df_saturated_liquid_density = \
    df_saturated_liquid_density.sort_values(['refrigerant', 'name of equation', 'ID of equation'])
df_ads_working_pairs = \
    df_ads_working_pairs.sort_values(['refrigerant', 'sorbent', 'subtype',
                                      'name of equation', 'ID of equation'])
df_abs_working_pairs = \
    df_abs_working_pairs.sort_values(['refrigerant', 'sorbent', 'subtype',
                                      'name of equation', 'ID of equation'])

df_vapor_pressure.to_excel(os.path.join(PATH_SORPPROPLIB,
                                        'content_refrigerants_vapor_pressure.xlsx'),
                           index=False)
df_saturated_liquid_density.to_excel(os.path.join(PATH_SORPPROPLIB,
                                        'content_refrigerants_saturated_liquid_density.xlsx'),
                                     index=False)
df_ads_working_pairs.to_excel(os.path.join(PATH_SORPPROPLIB,
                                        'content_adsorption_working_pairs.xlsx'),
                              index=False)
df_abs_working_pairs.to_excel(os.path.join(PATH_SORPPROPLIB,
                                        'content_absorption_working_pairs.xlsx'),
                              index=False)
