# -*- coding: utf-8 -*-
"""
Created on Thu Mar  5 17:55:58 2020

@author: Engelpracht
"""

#%% Import relevant libraries
#
import json
import pandas

from os import listdir
from os.path import isfile, join


#%% Global variables
#
path_coefficients = 'data\\JSON\\data_coefficients\\'
path_experiments = 'data\\JSON\\data_experiments\\'
path_json = 'data\\JSON\\'

df_ref_wpair_columns = ['refrigerant', 'sorbent', 'sorbent-subtype', 'type']

dict_base_keys = ['k', 'v']
dict_k_keys = ['_as_', '_rf_', '_st_', '_tp_']
dict_v_keys = ['_ep_', '_ed_', '_r_', '_s_', '_t_']
dict_equations_keys = ['_c_', '_e_', '_p_']

df_equation_columns = ['literature', 'refrigerant', 'sorbent', 
                       'sorbent-subtype', 'type']


#%% Load *.csv-files
#
# Load all *.csv-files that contain coefficients or experimental data of
# refrigerants, adsorption isotherms, and absorption isotherms
#
# 1.) Get all files that include coefficients
#
files_coefficients = [file for file in listdir(path_coefficients)
    if isfile(join(path_coefficients, file))]

# 2.) Get all files that include experimental data
#
#files_experiments = [file for file in listdir(path_experiments)
#    if isfile(join(path_experiments, file))]

# Save all *.csv-files as pandas data frames into lists
#
list_coefficients = []
for _, val in enumerate(files_coefficients):
    list_coefficients.append(pandas.read_csv(path_coefficients + val, 
                                             sep = ';',
                                             keep_default_na = False))

list_experiments = []
#for _, val in enumerate(files_experiments):
#    list_experiments.append(pandas.read_csv(path_experiments + val, 
#                                            sep = ';',
#                                            keep_default_na = False))


#%% Create JSON-file
#
# Structure of the JSON-file:
# ---------------------------
#
# TBD
#
# 1) Get number of working pairs and refrigerants:
#
#       - Combine lists
#       - Remove duplicates
#       - Calculate lengths
#
list_complete = list_coefficients + list_experiments
df_ref_wpair = pandas.DataFrame(columns = df_ref_wpair_columns)

for _, val in enumerate(list_complete):
    df_ref_wpair = df_ref_wpair.append(val[df_ref_wpair_columns], 
                                       ignore_index = True)

df_ref_wpair = df_ref_wpair.drop_duplicates()
df_ref_wpair = df_ref_wpair.reset_index(drop = True)
no_ref_wpair = len(df_ref_wpair) + 1

# 2) Create dict 'k' used to save general information of all refrigerants and
#    working pairs:
#
#       - Iterate over all refrigerants and working pairs
#       - Create dicts by zip-function
#
list_dict_k = []
for _, row in df_ref_wpair.iterrows():
    list_dict_k.append(dict(zip(dict_k_keys, row)))
    
# 3) Create dummy dict 'v' to save coefficients of equations and experimental
#    data:
#
#       - Iterate over all refrigerants and working pairs
#       - Create dicts and save dummy values for keys that will be later used
#         coefficients of equations and experimental data
#
list_dict_v = []
for _, row in df_ref_wpair.iterrows():
    values = [[], 
              [], 
              row[df_ref_wpair_columns[0]], 
              row[df_ref_wpair_columns[1]], 
              row[df_ref_wpair_columns[2]]]
    list_dict_v.append(dict(zip(dict_v_keys, values)))

# 4) Create dict '_ep_' to save coefficients of equations:
#
#       - Create dict that contains names of all equations as keys and empty
#         lists as attirbutes
#       - Create list for all refrigerants and working pairs wither former
#         created dict as attribute
#       - For each refrigerant and working pair, count how often an equation
#         occurs --> iterate over refrigerants and working pairs first and
#         over equations second
#       - For each equation, iterate over available sets of coefficients -->
#         for each set of coefficient, save coefficients as dict
#
list_equations = []  
for ind in range(no_ref_wpair -1):
    # Create empty dictionaries
    #
    dict_equations = dict()
    for _, val in enumerate(files_coefficients):
        dict_equations.update(zip([val[:-4]], [[]]))
        
    list_equations.append(dict_equations)
    
for ind_ref_wpair, row in df_ref_wpair.iterrows():
    for ind_equ, name_equ in enumerate(dict_equations):
        # Search current working pair within current equation
        #
        tmp_df = list_coefficients[ind_equ]
        tmp_df_Res = tmp_df.loc[(tmp_df[df_ref_wpair_columns[0]] == 
                    row[df_ref_wpair_columns[0]]) &
                   (tmp_df[df_ref_wpair_columns[1]] == 
                    row[df_ref_wpair_columns[1]]) &
                   (tmp_df[df_ref_wpair_columns[2]] == 
                    row[df_ref_wpair_columns[2]]) &
                   (tmp_df[df_ref_wpair_columns[3]] == 
                    row[df_ref_wpair_columns[3]])]
                   
        tmp_df_Res = tmp_df_Res.reset_index(drop = True)
        if tmp_df_Res.empty:
            tmp_df_Res = []
                   
        list_equations[ind_ref_wpair].update(dict(zip([name_equ],
                                                      [tmp_df_Res])))

for ind_ref_wpair, _ in enumerate(list_dict_v):
    # Count number of equations
    #
    tmp_name_equ = []
    for _, name_equ in enumerate(list_equations[ind_ref_wpair]):           
        if not type(list_equations[ind_ref_wpair][name_equ]) == list:
            tmp_name_equ.append(name_equ)
    
    # Create dummy dict to save coefficient sets for each identified equations
    #
    tmp_list_equ = [ [] for i in range(len(tmp_name_equ))]
    tmp_dict_equ = dict(zip(tmp_name_equ, tmp_list_equ))
    
    # For each equation, iterate over available sets of coefficients
    # For each set of coefficient, save coefficients as dict
    #
    for ind_equ, name_equ in enumerate(tmp_dict_equ):
        tmp_list_equ_set = []
        for ind_equ_set, equ_set in list_equations[ind_ref_wpair][name_equ].iterrows():
            # Create dummy dict for each set of coefficients
            #
            tmp_list = [ [] for i in range(len(dict_equations_keys))]
            tmp_dict = dict(zip(dict_equations_keys, 
                                tmp_list))
            
            # Read coefficients and further information for *.csv-files
            #
            tmp_dict[dict_equations_keys[0]] = equ_set[df_equation_columns[0]]
            tmp_dict[dict_equations_keys[1]] = name_equ.capitalize()
            
            equ_set = equ_set.drop(df_equation_columns)
            
            tmp_name_par = equ_set.index
            tmp_no_par = len(tmp_name_par)
            
            tmp_name = []
            tmp_value = []
            for ind in range(tmp_no_par):
                tmp_name.append(tmp_name_par[ind])
                tmp_value.append(equ_set[tmp_name_par[ind]])
                
            tmp_dict[dict_equations_keys[2]] = dict(zip(tmp_name,tmp_value))
                        
            # Update list that contains all sets of coefficients
            #
            tmp_list_equ_set.append(tmp_dict)
        
        # Update dict that contains different equations
        #
        tmp_dict_equ[name_equ] = tmp_list_equ_set
            
    # Update dict 'v' by adding coefficients of equations
    #
    list_dict_v[ind_ref_wpair][dict_v_keys[0]] = tmp_dict_equ

# 6) Create base dict that containts dicts 'k' and 'v':
#
#       - Iterate over all refrigerants and working pairs
#       - Create dicts by zip-function
#
list_json = []
for ind, val in enumerate(list_dict_k):
    list_json.append(dict(zip(dict_base_keys, [val, list_dict_v[ind]])))


#%% Save JSON-files
#
with open(path_json + "sorpproplib.json", "w") as text_file:
    text_file.write(json.dumps(list_json))
    
with open(path_json + "sorpproplib_formatted.json", "w") as text_file:
    text_file.write(json.dumps(list_json, indent=4))

