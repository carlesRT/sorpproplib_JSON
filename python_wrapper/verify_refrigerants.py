"""Script 'verify_refrigerants'.

This script is an example script to check all approaches implemented into SorpPropLib that
calculate refrigerant properties. The results are plotted and saved at '.\sorpproplib\data\
verification\Refrigerants' as *.png figures.

Revisions:
----------
    Version     Date        Author              Changes
    1.0.0       2021-06-10  Mirko Engelpracht   First implementation
"""

# %% Import relevant modules.
#
import matplotlib.pyplot as plt
import numpy as np
import os
import struct
import sys

from json import load
from sorpproplib.wpair.wpair_struct import WPairStruct


# %% Setup.
#
# Set up paths depending on plattform and architecture
#
if sys.platform == "win32":
    # Windows platform
    #
    if struct.calcsize("P") * 8 == 64:
        REQUIRED_DLL = "win64bit"
    else:
        REQUIRED_DLL = "win32bit"

elif (sys.platform == "linux") or (sys.platform == "linux2"):
    # Linux platform
    #
    if struct.calcsize("P") * 8 == 64:
        REQUIRED_DLL = "linux64bit"

else:
    # Ohter platform: Not supported
    #
    sys.exit()

PATH_WRAPPER = os.path.join(os.getcwd().split("sorpproplib_JSON")[0],
                            'sorpproplib_JSON', 'python_wrapper')
PATH_DB = os.path.join(PATH_WRAPPER, 'sorpproplib',
                       'data', 'JSON', 'sorpproplib.json').encode()
PATH_SORPPROPLIB = os.path.join(PATH_WRAPPER,  'sorpproplib',
                                'data', REQUIRED_DLL, 'libsorpPropLib.dll')


# %% Plot refrigereant properties.
#
# Load JSON data base
#
with open(PATH_DB, "r") as read_file:
    json_file = load(read_file)

# Get lists containing all refrigerants and all working pairs
#
list_refrigerants = []
list_working_pairs = []

for _, entry in enumerate(json_file):
    if entry['k']['_tp_'] == 'refrig':
        list_refrigerants.append(entry)

for _, entry in enumerate(json_file):
    if entry['k']['_tp_'] != 'refrig':
        list_working_pairs.append(entry)

# Loop through all refrigerants and check if ad- or absorption isotherm
# exist using the current refrigerant. The isotherm is needed to create a
# dummy working pair. The dummy working pair is needed because an instance
# of the class WorkingPair can only be created if an isotherm is selected
# that is implemented into the SorpPropLib (see C code).
#
for _, refrigerant in enumerate(list_refrigerants):
    # Check if data for isotherm exists
    #
    for _, working_pair in enumerate(list_working_pairs):
        if refrigerant['k']['_rf_'] == working_pair['k']['_rf_']:
            # Get all calculation approaches of refrigerant functions
            # implemented for current refrigerant
            #
            refrigerant_equations = refrigerant['v']['_ep_']

            refrigerant_equation_names = list(refrigerant_equations.keys())
            if 'Refrigerants' in refrigerant_equation_names:
                refrigerant_equation_names.remove('Refrigerants')

            # Split identified calculation approaches into
            # approaches for vapor pressure and saturated
            # liquid density
            #
            refrigerant_equations_vapor_pressure =[]
            refrigerant_equations_saturated_liquid_density =[]

            for _, refrigerant_equation_name in enumerate(refrigerant_equation_names):
                if refrigerant_equation_name.split('_')[0] == 'VaporPressure':
                    refrigerant_equations_vapor_pressure.append(refrigerant_equation_name)

                else:
                    refrigerant_equations_saturated_liquid_density.append(refrigerant_equation_name)

            # Plot implemented calculation approaches
            #
            fig, ax = plt.subplots(1, 2, figsize=(20.5/2.54, 20/2.54))

            # Plot vapor pressure
            #
            for _, refrigerant_equation_name in \
            enumerate(refrigerant_equations_vapor_pressure):
                for no_approach, approach in \
                enumerate(refrigerant['v']['_ep_'][refrigerant_equation_name]):
                    # Initialize working pair
                    #
                    wpair = {"sorbent": working_pair['k']['_as_'],
                             "subtype": working_pair['k']['_st_'],
                             "refrigerant": working_pair['k']['_rf_']}
                    wpair_approach = {"isotherm": list(working_pair['v']['_ep_'].keys())[0],
                                      "id_isotherm": 1,
                                      "vapor_pressure": refrigerant_equation_name,
                                      "id_vapor_pressure": no_approach+1,
                                      "sat_liq_density": "NoSaturatedLiquidDensity",
                                      "id_sat_liq_density": 1}
                    wpair_class = WPairStruct(wpair, wpair_approach, PATH_SORPPROPLIB, PATH_DB)

                    # Get valid range and calculate properties
                    #
                    T_min = approach['_va_']['temperature-min'] if \
                        approach['_va_']['temperature-min'] > 0 else \
                            approach['_va_']['temperature-max']
                    T_max = approach['_va_']['temperature-max']

                    T = [val for val in range(int(np.ceil(T_min)), int(np.floor(T_max)))]
                    p_sat = [wpair_class.Ref.p_sat_T(val) / 1e5 for val in T]

                    # Plot properties
                    #
                    ax[0].plot(T, p_sat,
                               label = str(refrigerant_equation_name.split('_')[1]) + \
                                       ' - ID: ' + str(no_approach+1))

                    # Delete working pair
                    #
                    del(wpair_class)

            # Set up axes
            #
            ax[0].spines['top'].set_visible(False)
            ax[0].spines['right'].set_visible(False)
            ax[0].spines['left'].set_linewidth(0.5)
            ax[0].spines['bottom'].set_linewidth(0.5)

            ax[0].set_xlabel('Temperature T in K')
            ax[0].set_ylabel('Vapor pressure p_sat in bar')

            ax[0].legend(loc='best', labelspacing=0.05, frameon=False)

            # Plot vapor pressure
            #
            for _, refrigerant_equation_name in \
            enumerate(refrigerant_equations_saturated_liquid_density):
                for no_approach, approach in \
                enumerate(refrigerant['v']['_ep_'][refrigerant_equation_name]):
                    # Initialize working pair
                    #
                    wpair = {"sorbent": working_pair['k']['_as_'],
                             "subtype": working_pair['k']['_st_'],
                             "refrigerant": working_pair['k']['_rf_']}
                    wpair_approach = {"isotherm": list(working_pair['v']['_ep_'].keys())[0],
                                      "id_isotherm": 1,
                                      "vapor_pressure": "NoVaporPressure",
                                      "id_vapor_pressure": 1,
                                      "sat_liq_density": refrigerant_equation_name,
                                      "id_sat_liq_density": no_approach+1}
                    wpair_class = WPairStruct(wpair, wpair_approach, PATH_SORPPROPLIB, PATH_DB)

                    # Get valid range and calculate properties
                    #
                    T_min = approach['_va_']['temperature-min'] if \
                        approach['_va_']['temperature-min'] > 0 else \
                            approach['_va_']['temperature-max']
                    T_max = approach['_va_']['temperature-max']

                    T = [val for val in range(int(np.ceil(T_min)), int(np.floor(T_max)))]
                    rho_sat_l = [wpair_class.Ref.rho_sat_l_T(val) for val in T]

                    # Plot properties
                    #
                    ax[1].plot(T, rho_sat_l,
                                label = str(refrigerant_equation_name.split('_')[1]) + \
                                        ' - ID: ' + str(no_approach+1))

                    # Delete working pair
                    #
                    del(wpair_class)

            # Set up axes
            #
            ax[1].spines['top'].set_visible(False)
            ax[1].spines['right'].set_visible(False)
            ax[1].spines['left'].set_linewidth(0.5)
            ax[1].spines['bottom'].set_linewidth(0.5)

            ax[1].set_xlabel('Temperature T in K')
            ax[1].set_ylabel('Saturated liquid density in kg/m³')

            ax[1].legend(loc='best', labelspacing=0.05, frameon=False)

            # Tight layout
            #
            fig.suptitle('Selected refrigerant: ' + refrigerant['k']['_rf_'])

            fig.align_ylabels()
            fig.tight_layout()

            # Save figure
            #
            tmp_path = os.path.join(PATH_WRAPPER,
                                    'sorpproplib',
                                    'data',
                                    'verification',
                                    'Refrigerants')
            tmp_name = refrigerant['k']['_rf_'] + '.png'

            if not os.path.exists(tmp_path):
                os.mkdir(tmp_path)

            fig.savefig(os.path.join(tmp_path, tmp_name))
            plt.close()

            # Break inner loop as dummy working pair is found
            #
            break
