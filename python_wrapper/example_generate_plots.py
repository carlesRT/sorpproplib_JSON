"""Script 'example_generate_plots'.

This script is an example script to demonstrate the plotting capabilities of the
python interface of the SorpPropLib.

Revisions:
----------
    Version     Date        Author              Changes
    1.0.0       2021-07-09  Mirko Engelpracht   First implementation
"""

# %% Import relevant modules.
#
import os
import struct
import sys

from functools import partial as partial_func
from sorpproplib.plot.equilibrium import plot_isosteres, plot_isotherms, plot_isobars
from sorpproplib.plot.equilibrium import plot_char_curve, plot_duehring
from sorpproplib.wpair.wpair_struct import WPairStruct


# %% Setup.
#
# Identify platform and architecture
#
if sys.platform == 'win32':
    # Windows platform
    #
    if struct.calcsize('P') * 8 == 64:
        REQUIRED_DLL = 'win64bit'
    else:
        REQUIRED_DLL = 'win32bit'

elif (sys.platform == 'linux') or (sys.platform == 'linux2'):
    # Linux platform
    #
    if struct.calcsize('P') * 8 == 64:
        REQUIRED_DLL = 'linux64bit'

else:
    # Ohter platform: Not supported
    #
    sys.exit()

# Set up paths
#
PATH_WRAPPER = os.path.join(os.getcwd().split("sorpproplib_JSON")[0],
                            'sorpproplib_JSON', 'python_wrapper')
PATH_DB = os.path.join(PATH_WRAPPER, 'sorpproplib',
                       'data', 'JSON', 'sorpproplib.json').encode()
PATH_SORPPROPLIB = os.path.join(PATH_WRAPPER,  'sorpproplib',
                                'data', REQUIRED_DLL, 'libsorpPropLib.dll')


# %% Plotting capabilites for adsorption working pairs.
#
# Plot isosteres.
#
# Define working pair.
#
wpair_name = {"sorbent": 'silica gel pellet',
              "subtype": '123',
              "refrigerant": 'Water'}
wpair_approach = {"isotherm": 'DubininArctan1',
                  "id_isotherm": 1,
                  "vapor_pressure": "VaporPressure_EoS1",
                  "id_vapor_pressure": 1,
                  "sat_liq_density": 'SaturatedLiquidDensity_EoS1',
                  "id_sat_liq_density": 1}
wpair = WPairStruct(wpair_name, wpair_approach, PATH_SORPPROPLIB, PATH_DB)

# Set up
#
T_calc = [val + 273.15 for val in range(10,400)]
wxX_calc = [0.01, 0.02, 0.03, 0.04, 0.05, 0.1, 0.15, 0.2,
            0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6]

T_axis = [val + 273.15 for val in [10, 20, 30, 40, 50, 75, 100, 150, 200, 250, 300, 350, 400]]
p_axis = [10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000]

# Plot data
#
fig_data = plot_isosteres(wpair.Ref.p_sat_T,  wpair.Ads.p_wT,  wpair.Ads.w_pT,
                          T_calc, wxX_calc, T_axis, p_axis)

# Delete working pair.
#
del(wpair)

# %% Plotting capabilites for adsorption working pairs.
#
# Plot isosteres.
#
wpair_name = {"sorbent": 'silica gel pellet',
              "subtype": '',
              "refrigerant": 'CarbonDioxide'}
wpair_approach = {"isotherm": 'Toth',
                  "id_isotherm": 1,
                  "vapor_pressure": "VaporPressure_EoS1",
                  "id_vapor_pressure": 1,
                  "sat_liq_density": 'SaturatedLiquidDensity_EoS1',
                  "id_sat_liq_density": 1}
wpair = WPairStruct(wpair_name, wpair_approach, PATH_SORPPROPLIB, PATH_DB)

# Set up
#
T_calc = [val + 273.15 for val in range(0,150)]
wxX_calc = [0.01, 0.02, 0.03, 0.04, 0.05, 0.1, 0.15, 0.2,
            0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6]

T_axis = [val + 273.15 for val in [0, 10, 20, 30, 40, 50, 75, 100, 150]]
p_axis = [1000, 10000, 100000, 1000000, 10000000]

# Plot data
#
fig_data = plot_isosteres(wpair.Ref.p_sat_T,  wpair.Ads.p_wT,  wpair.Ads.w_pT,
                          T_calc, wxX_calc, T_axis, p_axis)

# Delete working pair.
#
del(wpair)


# %% Plot isotherms.
#
# Define working pair.
#
wpair_name = {"sorbent": 'silica gel pellet',
              "subtype": '123',
              "refrigerant": 'Water'}
wpair_approach = {"isotherm": 'DubininArctan1',
                  "id_isotherm": 1,
                  "vapor_pressure": "VaporPressure_EoS1",
                  "id_vapor_pressure": 1,
                  "sat_liq_density": 'SaturatedLiquidDensity_EoS1',
                  "id_sat_liq_density": 1}
wpair = WPairStruct(wpair_name, wpair_approach, PATH_SORPPROPLIB, PATH_DB)

# Set up
#
p_calc = [val for val in range(0,250000, 25)]
T_calc = [val + 273.15 for val in [10, 20, 30, 40, 50, 75, 100, 150, 200, 250, 300, 350, 400]]

p_axis = [0, 50000, 100000, 150000, 200000, 250000]
wxX_axis = [0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5]

# Plot data:
#   a) Use refrigerant function built into the SorpPropLib.
#   b) Demonstrate the use of custom refrigerant functions instead of refrigerant functions
#      built into the SorpPropLib.
#
fig_data = plot_isotherms(wpair.Ref.p_sat_T, wpair.Ads.w_pT,
                          p_calc, T_calc, p_axis, wxX_axis,
                          flag_p_rel = False)

def func_wxX_pT_custom(p, T, wpair):
    """Wrap function to use custom refrigerant functions."""
    return wpair.Ads.Vol.w_pTpsatrho(p, T, wpair.Ref.p_sat_T(T), wpair.Ref.rho_sat_l_T(T))

fig_data = plot_isotherms(wpair.Ref.p_sat_T,  partial_func(func_wxX_pT_custom, wpair=wpair),
                          p_calc, T_calc, p_axis, wxX_axis,
                          flag_p_rel = False)

# Delete working pair.
#
del(wpair)


# %% Plot isosteres.
#
# Define working pair.
#
wpair_name = {"sorbent": 'silica gel pellet',
              "subtype": '',
              "refrigerant": 'CarbonDioxide'}
wpair_approach = {"isotherm": 'Toth',
                  "id_isotherm": 1,
                  "vapor_pressure": "VaporPressure_EoS1",
                  "id_vapor_pressure": 1,
                  "sat_liq_density": 'SaturatedLiquidDensity_EoS1',
                  "id_sat_liq_density": 1}
wpair = WPairStruct(wpair_name, wpair_approach, PATH_SORPPROPLIB, PATH_DB)

# Set up
#
p_calc = [val for val in range(0,10000000, 250)]
T_calc = [val + 273.15 for val in [10, 20, 30, 40, 50, 75, 100, 150, 200, 250, 300, 350, 400]]

p_axis = [0, 7500000]
wxX_axis = [0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5]

# Plot data
#   a) Use refrigerant function built into the SorpPropLib.
#   b) Demonstrate the use of custom refrigerant functions instead of refrigerant functions
#      built into the SorpPropLib.
#
fig_data = plot_isotherms(wpair.Ref.p_sat_T, wpair.Ads.w_pT,
                          p_calc, T_calc, p_axis, wxX_axis,
                          flag_p_rel = True)

def func_wxX_pT_custom(p, T, wpair):
    """Wrap function to use custom functions."""
    return wpair.Ads.Sur.w_pT(p, T)

fig_data = plot_isotherms(wpair.Ref.p_sat_T,  partial_func(func_wxX_pT_custom, wpair=wpair),
                          p_calc, T_calc, p_axis, wxX_axis,
                          flag_p_rel = True)

# Delete working pair.
#
del(wpair)


# %% Plot isobars.
#
# Define working pair.
#
wpair_name = {"sorbent": 'silica gel pellet',
              "subtype": '123',
              "refrigerant": 'Water'}
wpair_approach = {"isotherm": 'DubininArctan1',
                  "id_isotherm": 1,
                  "vapor_pressure": "VaporPressure_EoS1",
                  "id_vapor_pressure": 1,
                  "sat_liq_density": 'SaturatedLiquidDensity_EoS1',
                  "id_sat_liq_density": 1}
wpair = WPairStruct(wpair_name, wpair_approach, PATH_SORPPROPLIB, PATH_DB)

# Set up
#
p_calc = [1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 20000000, 100000000]
T_calc = [val + 273.15 for val in range(0,400)]

T_axis = [val + 273.15 for val in [0, 50, 100, 150, 200, 250, 300, 350, 400]]
wxX_axis = [0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45]

# Plot data
#
fig_data = plot_isobars(wpair.Ref.p_sat_T, wpair.Ref.T_sat_p, wpair.Ads.w_pT,
                        p_calc, T_calc, T_axis, wxX_axis)

# Delete working pair.
#
del(wpair)


# %% Plot isobars.
#
# Define working pair.
#
wpair_name = {"sorbent": 'silica gel pellet',
              "subtype": '',
              "refrigerant": 'CarbonDioxide'}
wpair_approach = {"isotherm": 'Toth',
                  "id_isotherm": 1,
                  "vapor_pressure": "VaporPressure_EoS1",
                  "id_vapor_pressure": 1,
                  "sat_liq_density": 'SaturatedLiquidDensity_EoS1',
                  "id_sat_liq_density": 1}
wpair = WPairStruct(wpair_name, wpair_approach, PATH_SORPPROPLIB, PATH_DB)

# Set up
#
p_calc = [1, 10, 100, 1000, 10000, 100000, 1000000, 7000000, 10000000]
T_calc = [val + 273.15 for val in range(-100,150)]

T_axis = [val + 273.15 for val in [-100, -50, 0, 50, 100, 150]]
wxX_axis = [0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65]

# Plot data
#
fig_data = plot_isobars(wpair.Ref.p_sat_T, wpair.Ref.T_sat_p, wpair.Ads.w_pT,
                        p_calc, T_calc, T_axis, wxX_axis)

# Delete working pair.
#
del(wpair)


# %% Plot characteristic curve.
#
# Define working pair.
#
wpair_name = {"sorbent": 'silica gel pellet',
              "subtype": '123',
              "refrigerant": 'Water'}
wpair_approach = {"isotherm": 'DubininArctan1',
                  "id_isotherm": 1,
                  "vapor_pressure": "VaporPressure_EoS1",
                  "id_vapor_pressure": 1,
                  "sat_liq_density": 'SaturatedLiquidDensity_EoS1',
                  "id_sat_liq_density": 1}
wpair = WPairStruct(wpair_name, wpair_approach, PATH_SORPPROPLIB, PATH_DB)

# Set up
#
A_calc = [val for val in range(0,35000)]

A_axis = [0, 5000, 10000, 15000, 20000, 25000, 30000, 35000]
W_axis = [val / 10000 for val in [0, 1, 2, 3, 4, 5]]

# Plot data
#
def func_W_A_custom(A, wpair):
    """Wrap function as density is not needed for selected working pair."""
    return wpair.Ads.Vol.W_ARho(A, -1)

fig_data = plot_char_curve(partial_func(func_W_A_custom, wpair=wpair),
                           A_calc, A_axis, W_axis)

# Delete working pair.
#
del(wpair)


# %% Plot Dühring diagram.
#
# Define working pair.
#
wpair_name = {"sorbent": 'silica gel pellet',
              "subtype": '123',
              "refrigerant": 'Water'}
wpair_approach = {"isotherm": 'DubininArctan1',
                  "id_isotherm": 1,
                  "vapor_pressure": "VaporPressure_EoS1",
                  "id_vapor_pressure": 1,
                  "sat_liq_density": 'SaturatedLiquidDensity_EoS1',
                  "id_sat_liq_density": 1}
wpair = WPairStruct(wpair_name, wpair_approach, PATH_SORPPROPLIB, PATH_DB)

# Set up
#
T_calc = [val + 273.15 for val in range(0,400)]
wxX_calc = [0.01, 0.02, 0.03, 0.04, 0.05, 0.1, 0.15, 0.2,
            0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6]

T_axis_x = [val + 273.15 for val in [0, 25, 50, 75, 100, 150, 200, 250, 300, 350, 400]]
T_axis_y = T_axis_x

# Plot data
#
fig_data = plot_duehring(wpair.Ref.p_sat_T,  wpair.Ref.T_sat_p,
                         wpair.Ads.p_wT,  wpair.Ads.w_pT,
                         T_calc, wxX_calc, T_axis_x, T_axis_y)

# Delete working pair.
#
del(wpair)


# %% Plot Dühring diagram.
#
# Define working pair.
#
wpair_name = {"sorbent": 'silica gel pellet',
              "subtype": '',
              "refrigerant": 'CarbonDioxide'}
wpair_approach = {"isotherm": 'Toth',
                  "id_isotherm": 1,
                  "vapor_pressure": "VaporPressure_EoS1",
                  "id_vapor_pressure": 1,
                  "sat_liq_density": 'SaturatedLiquidDensity_EoS1',
                  "id_sat_liq_density": 1}
wpair = WPairStruct(wpair_name, wpair_approach, PATH_SORPPROPLIB, PATH_DB)

# Set up
#
T_calc = [val + 273.15 for val in range(0,150)]
wxX_calc = [0.01, 0.02, 0.03, 0.04, 0.05, 0.1, 0.15, 0.2,
            0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6]

T_axis_x = [val + 273.15 for val in [0, 10, 20, 30, 40, 50, 75, 100, 150]]
T_axis_y = [val + 273.15 for val in [0, 5, 10, 15, 20, 25, 30]]

# Plot data
#
fig_data = plot_duehring(wpair.Ref.p_sat_T,  wpair.Ref.T_sat_p,
                         wpair.Ads.p_wT,  wpair.Ads.w_pT,
                         T_calc, wxX_calc, T_axis_x, T_axis_y)

# Delete working pair.
#
del(wpair)
