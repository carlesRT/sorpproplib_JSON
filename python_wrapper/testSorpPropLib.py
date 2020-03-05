# -*- coding: utf-8 -*-
"""
Created on Wed Feb 12 08:19:25 2020

@author: Engelpracht
"""

#
# Import relevant libraries
#
from ctypes import c_double, c_char_p, c_wchar_p, cdll

#
# Import DLL and set information about inputs and outputs of functions
#
basic_function_lib = cdll.LoadLibrary('64bit\libsorpPropLib.dll')

basic_function_lib.direct_iso_w_pT_workingPair.restype = c_double
basic_function_lib.direct_iso_w_pT_workingPair.argtypes = [c_double, c_double, c_char_p, c_char_p, c_char_p, c_char_p, c_char_p, c_char_p, c_char_p, c_char_p]


#
# Define parameters
#
path = b"D:\\Engelpracht\\04-C\\sorpproplib_JSON\\python_wrapper\\64bit\\sorpproplib_ValidationCInterface.json"
ads =  b"carbon"
x =  b"maxsorb-iii"
KM =  b"r-134a"
Iso =  b"dubinin-astakov"
Pd =  b"EoS_vapourPressure"
roh =  b"EoS_saturatedLiquidDensity"
koef =  b"NoActivityCoefficients"


p = c_double(1e5)
T = c_double(303.15)
w = basic_function_lib.direct_iso_w_pT_workingPair(p, T, path, ads, x, KM, Iso, Pd, roh, koef)
