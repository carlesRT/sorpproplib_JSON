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

df_ref_wpair_columns = [ 'sorbent', 'refrigerant', 'sorbent-subtype', 'type']
df_equation_columns = ['literature']
df_prop_columns = ['prop-diameter-crystal', 'prop-diameter-pellet', 
                   'prop-diameter-extrudate', 'prop-length-extrudate', 
                   'props-porosity-pellet', 'props-density-bulk', 
                   'props-density-pellet', 'props-density-solid']
df_validity_columns = ['validity-pressure-min', 'validity-pressure-max',
                      'validity-temperature-min', 'validity-temperature-max',
                      'validity-loading-min', 'validity-loading-max']
df_error_columns = ['error-are', 'error-rmse']
df_measurement_columns = ['measurements-procedure', 'measurements-type']
df_uncertainty_columns = ['uncertainty-pressure-abs', 
                          'uncertainty-pressure-rel',
                          'uncertainty-temperature-abs',
                          'uncertainty-temperature-rel',
                          'uncertainty-loading-abs',
                          'uncertainty-loading-rel',
                          'uncertainty-adsorptionPotential-abs',
                          'uncertainty-adsorptionPotential-rel',
                          'uncertainty-volumetricLoading-abs',
                          'uncertainty-volumetricLoading-rel']

dict_base_keys = ['k', 'v']
dict_k_keys = ['_as_', '_rf_', '_st_', '_tp_']
dict_v_keys = ['_ep_', '_ed_', '_r_', '_s_', '_t_']
dict_equations_keys = ['_c_', '_e_', '_pr_', '_va_', '_er_', '_p_']
dict_ed_keys = ['_c_', '_pr_', '_m_', '_u_', '_d_']
dict_prop_keys = ['diameter-crystal', 'diameter-pellet', 
                   'diameter-extrudate', 'length-extrudate', 
                   'porosity-pellet', 'density-bulk', 
                   'density-pellet', 'density-solid']
dict_validity_keys = ['pressure-min', 'pressure-max',
                      'temperature-min', 'temperature-max',
                      'loading-min', 'loading-max']
dict_error_keys = ['are', 'rmse']
dict_measurement_keys = ['procedure', 'type']
dict_uncertainty_keys =['pressure-abs', 'pressure-rel', 'temperature-abs',
                        'temperature-rel', 'loading-abs', 'loading-rel',
                        'adsorptionPotential-abs', 'adsorptionPotential-rel',
                        'volumetricLoading-abs', 'volumetricLoading-rel']


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
files_experiments = [file for file in listdir(path_experiments)
    if isfile(join(path_experiments, file))]

# Save all *.csv-files as pandas data frames into lists
#
list_coefficients = []
for _, val in enumerate(files_coefficients):
    tmp_df = pandas.read_csv(path_coefficients + val, 
                             sep = ';',
                             encoding = 'utf_8',
                             keep_default_na = False)
    tmp_df = tmp_df.drop(0)
    tmp_df = tmp_df.reset_index(drop = True)
    list_coefficients.append(tmp_df)

list_experiments = []
for _, val in enumerate(files_experiments):
    tmp_df = pandas.read_csv(path_experiments + val, 
                             sep = ';',
                             encoding = 'utf_8',
                             keep_default_na = False)
    tmp_df = tmp_df.drop(0)
    tmp_df = tmp_df.reset_index(drop = True)
    list_experiments.append(tmp_df)


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
              row[df_ref_wpair_columns[1]], 
              row[df_ref_wpair_columns[0]], 
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
            #   1) Literature
            #
            tmp_dict[dict_equations_keys[0]] = equ_set[df_equation_columns[0]]
            
            #   2) Name of equation
            #
            tmp_dict[dict_equations_keys[1]] = name_equ.capitalize()
            
            #   3) Further properties of sorbent
            #          
            tmp_dict_prop = equ_set[df_prop_columns]
            tmp_name_par = tmp_dict_prop.index            
            tmp_value = []
            for ind in range(len(tmp_name_par)):
                tmp_value.append(tmp_dict_prop[tmp_name_par[ind]])       
                
            tmp_dict[dict_equations_keys[2]] = dict(zip(dict_prop_keys, 
                                                        tmp_value))         
            
            #   4) Valid range
            #
            tmp_dict_valid = equ_set[df_validity_columns]
            tmp_name_par = tmp_dict_valid.index            
            tmp_value = []
            for ind in range(len(tmp_name_par)):
                if tmp_dict_valid[tmp_name_par[ind]] == '':
                    tmp_value.append(tmp_dict_valid[tmp_name_par[ind]])
                else:
                    tmp_value.append(float(tmp_dict_valid[tmp_name_par[ind]]))    
                
            tmp_dict[dict_equations_keys[3]] = dict(zip(dict_validity_keys, 
                                                        tmp_value))     
            
            #   5) Error
            #
            tmp_dict_error = equ_set[df_error_columns]
            tmp_name_par = tmp_dict_error.index            
            tmp_value = []
            for ind in range(len(tmp_name_par)):
                if tmp_dict_error[tmp_name_par[ind]] == '':
                    tmp_value.append(tmp_dict_error[tmp_name_par[ind]])
                else:
                    tmp_value.append(float(tmp_dict_error[tmp_name_par[ind]]))   
            
            tmp_dict[dict_equations_keys[4]] = dict(zip(dict_error_keys, 
                                                        tmp_value))
            
            #   6) Coefficients
            #
            equ_set = equ_set.drop(df_ref_wpair_columns +
                                   df_equation_columns +
                                   df_prop_columns +
                                   df_validity_columns +
                                   df_error_columns)
            
            tmp_name_par = equ_set.index
            tmp_no_par = len(tmp_name_par)
            
            tmp_name = []
            tmp_value = []
            for ind in range(tmp_no_par):
                tmp_name.append(tmp_name_par[ind])
                tmp_value.append(float(equ_set[tmp_name_par[ind]]))
                
            tmp_dict[dict_equations_keys[5]] = dict(zip(tmp_name,tmp_value))
                        
            # Update list that contains all sets of coefficients
            #
            tmp_list_equ_set.append(tmp_dict)
        
        # Update dict that contains different equations
        #
        tmp_dict_equ[name_equ] = tmp_list_equ_set
            
    # Update dict 'v' by adding coefficients of equations
    #
    list_dict_v[ind_ref_wpair][dict_v_keys[0]] = tmp_dict_equ

# g) Create dict '_ed_' to save experimental data:
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
    
df_exp_data = pandas.DataFrame(columns = df_ref_wpair_columns)

for _, val in enumerate(list_experiments):
    df_exp_data = df_exp_data.append(val[df_ref_wpair_columns], 
                                       ignore_index = True)
df_exp_data = df_exp_data.drop_duplicates()
df_exp_data = df_exp_data.reset_index(drop = True)
no_ref_wpair_exp_data = len(df_exp_data) + 1
    
    
for ind_ref_wpair, _ in enumerate(list_dict_v):
    tmp_list_exp_data = []
    
    # Check if experimental data exists for current working pair
    # Thus save time and skip iterations that are not necessary
    #
    tmp_df_cur_ref_wpair = df_ref_wpair.loc[ind_ref_wpair, :]
    tmp_cur_ref_wpair = tmp_df_cur_ref_wpair.tolist()
    
    tmp_df_Res = df_exp_data.loc[(df_exp_data[df_ref_wpair_columns[0]] == 
                                  tmp_cur_ref_wpair[0]) &
                                 (df_exp_data[df_ref_wpair_columns[1]] == 
                                  tmp_cur_ref_wpair[1]) &
                                 (df_exp_data[df_ref_wpair_columns[2]] == 
                                  tmp_cur_ref_wpair[2]) &
                                 (df_exp_data[df_ref_wpair_columns[3]] == 
                                  tmp_cur_ref_wpair[3])]
    
    # Exists
    #
    if not tmp_df_Res.empty:
        
        for _, df_exp in enumerate(list_experiments):
            tmp_df_exp = df_exp.loc[(df_exp[df_ref_wpair_columns[0]] == 
                                     tmp_cur_ref_wpair[0]) &
                                    (df_exp[df_ref_wpair_columns[1]] == 
                                     tmp_cur_ref_wpair[1]) &
                                    (df_exp[df_ref_wpair_columns[2]] == 
                                     tmp_cur_ref_wpair[2]) &
                                    (df_exp[df_ref_wpair_columns[3]] == 
                                     tmp_cur_ref_wpair[3])]
            
            # Found experimental data set: Only exists within one data frame!
            # Start to save data
            #
            if not tmp_df_exp.empty:
                
                # For each source saved
                #
                for _, exp_set in tmp_df_exp.iterrows():
                
                    # Dummy dict to save
                    #
                    tmp_list = [ [] for i in range(len(dict_ed_keys))]
                    tmp_dict = dict(zip(dict_ed_keys, 
                                        tmp_list))
                    
                    # 1) Source
                    #
                    tmp_dict[dict_ed_keys[0]] = exp_set[df_equation_columns[0]]
                    
                    # 2) Sorbent properties
                    #
                    tmp_series_prop = exp_set[df_prop_columns]
                    tmp_name_par = tmp_series_prop.index            
                    tmp_value = []
                    for ind in range(len(tmp_name_par)):
                        tmp_value.append(tmp_series_prop[tmp_name_par[ind]])       
                        
                    tmp_dict[dict_ed_keys[1]] = dict(zip(dict_prop_keys, 
                                                         tmp_value))
                    
                    # 3) Measurement information
                    #
                    tmp_series_measure = exp_set[df_measurement_columns]
                    tmp_name_par = tmp_series_measure.index      
                    tmp_value = []
                    for ind in range(len(tmp_name_par)):
                        tmp_value.append(tmp_series_measure[tmp_name_par[ind]])   
                        
                    tmp_dict[dict_ed_keys[2]] = dict(zip(dict_measurement_keys, 
                                                         tmp_value))
                    
                    # 4) Uncertainty information
                    #
                    tmp_series_unc = exp_set[df_uncertainty_columns]
                    tmp_name_par = tmp_series_unc.index      
                    tmp_value = []
                    for ind in range(len(tmp_name_par)):
                        tmp_value.append(tmp_series_unc[tmp_name_par[ind]])  
                        
                    tmp_dict[dict_ed_keys[3]] = dict(zip(dict_uncertainty_keys, 
                                                         tmp_value))
                    
                    # 5) Measurements
                    #
                    mea_set = exp_set.drop(df_ref_wpair_columns +
                                           df_equation_columns +
                                           df_prop_columns +
                                           df_measurement_columns +
                                           df_uncertainty_columns)
                    
                    tmp_name_par = mea_set.index
                    tmp_no_par = len(tmp_name_par)
                    
                    tmp_value = []
                    for ind in range(tmp_no_par):                        
                        # Split string and ensure empty lists
                        #
                        tmp_data = mea_set[tmp_name_par[ind]].split(',')
                        if tmp_data[0] == '':
                            tmp_data = []
                        else:
                            tmp_data = [float(item) for item in tmp_data]
                        
                        
                        tmp_value.append(tmp_data)
                        
                    tmp_dict[dict_ed_keys[4]] = dict(zip(tmp_name_par.to_list(),
                                                         tmp_value))
                                
                    # Updata list
                    #
                    tmp_list_exp_data.append(tmp_dict)  
            
                # Updata v dict
                #
                list_dict_v[ind_ref_wpair]['_ed_'] = tmp_list_exp_data
            
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

