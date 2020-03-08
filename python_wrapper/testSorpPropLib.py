# -*- coding: utf-8 -*-
"""
Created on Wed Feb 12 08:19:25 2020

@author: Engelpracht
"""

#%% Import relevant libraries
#
import matplotlib.pyplot as plt
import numpy as np

from ctypes import c_int, c_double, c_char_p, cdll
from json import load


#%% Import DLL and set information about inputs and outputs of functions
#
basic_function_lib = cdll.LoadLibrary('data\\64bit\\libsorpPropLib.dll')

basic_function_lib.direct_ads_w_pT_workingPair.restype = c_double
basic_function_lib.direct_ads_w_pT_workingPair.argtypes = [c_double, c_double, c_char_p, c_char_p, c_char_p, c_char_p, c_char_p, c_int, c_char_p, c_int, c_char_p, c_int]


#%% Wrapper functions
#

def w_pT(p, T, wp_as, wp_st, wp_rf):
    path_db = b"C:\\Users\\Mirko\\OneDrive\\Dokumente\\Git_Projects\\sorpproplib_JSON\\python_wrapper\\data\\64bit\\sorpproplib.json"
    wp_iso =  b"toth"
    no_iso = c_int(1)
    rf_psat =  b"EoS_vapourPressure"
    no_p_sat = c_int(1)
    rf_rhol =  b"EoS_saturatedLiquidDensity"
    no_rhol =  c_int(1)


    return basic_function_lib.direct_ads_w_pT_workingPair(p, T, path_db, wp_as.encode(), wp_st.encode(),
                                                       wp_rf.encode(), wp_iso, no_iso,
                                                       rf_psat, no_p_sat,
                                                       rf_rhol, no_rhol)


#%% Get experimental data
#
PATH_JSON = 'data\\JSON\\sorpproplib.json'
with open(PATH_JSON, "r") as read_file:
    json_file = load(read_file)

# Search for working pair and return it
#
wp_as =  "zeolite"
wp_st =  "5a"
wp_rf =  "co2"

for _, wpair in enumerate(json_file):
    dict_k = wpair['k']
    if (dict_k['_as_'] == wp_as) & (dict_k['_st_'] == wp_st) & (dict_k['_rf_'] == wp_rf):
        wpair_search = wpair
        break

# Search for experimental data
#
list_ed = wpair_search['v']['_ed_']
if not len(list_ed) == 0:
#    dict_ed = dict()
    list_type = [exp_data['_m_']['type'] for _, exp_data in enumerate(list_ed)]
    list_p = [exp_data['_d_']['pressures'] for _, exp_data in enumerate(list_ed)]
    list_T = [exp_data['_d_']['temperatures'] for _, exp_data in enumerate(list_ed)]
    list_w = [exp_data['_d_']['loadings'] for _, exp_data in enumerate(list_ed)]


#%% Base plot
#
fig, ax = plt.subplots()

for ind, val in enumerate(list_p):
    ax.scatter(np.array(val), list_w[ind], marker = 'X', s=25)

    w = [w_pT(c_double(val_p), c_double(list_T[ind][ind_p]), wp_as, wp_st, wp_rf) for ind_p, val_p in enumerate(val)]
    ax.scatter(np.array(val), w, marker = 'o', s=5)
#    break