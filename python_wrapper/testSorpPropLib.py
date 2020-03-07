# -*- coding: utf-8 -*-
"""
Created on Wed Feb 12 08:19:25 2020

@author: Engelpracht
"""

#
# Import relevant libraries
#
from ctypes import c_int, c_double, c_char_p, cdll

#
# Import DLL and set information about inputs and outputs of functions
#
basic_function_lib = cdll.LoadLibrary('data\\64bit\\libsorpPropLib.dll')

basic_function_lib.direct_ads_w_pT_workingPair.restype = c_double
basic_function_lib.direct_ads_w_pT_workingPair.argtypes = [c_double, c_double, c_char_p, c_char_p, c_char_p, c_char_p, c_char_p, c_int, c_char_p, c_int, c_char_p, c_int]


#
# Define parameters
#
p = c_double(5000)
T = c_double(303.15)

path_db = b"C:\\Users\\Mirko\\OneDrive\\Dokumente\\Git_Projects\\sorpproplib_JSON\\python_wrapper\\data\\64bit\\sorpproplib.json"
wp_as =  b"zeolite"
wp_st =  b"5a"
wp_rf =  b"water"
wp_iso =  b"toth"
no_iso = c_int(1)
rf_psat =  b"EoS_vapourPressure"
no_p_sat = c_int(1)
rf_rhol =  b"EoS_saturatedLiquidDensity"
no_rhol =  c_int(1)


w = basic_function_lib.direct_ads_w_pT_workingPair(p, T, path_db, wp_as, wp_st,
                                                   wp_rf, wp_iso, no_iso, 
                                                   rf_psat, no_p_sat,
                                                   rf_rhol, no_rhol)
