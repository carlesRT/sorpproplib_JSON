"""Script 'verify_equilibrium_w_verification_table'.

This script is an example script to verifiy all working pairs implemented into SorpPropLib with
data provided in the original literature.

Revisions:
----------
    Version     Date        Author              Changes
    1.0.0       2021-06-15  Mirko Engelpracht   First implementation
"""

# %% Import relevant modules.
#
import os
import pandas as pd
import struct
import sys

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
PATH_VERIFICATION = os.path.join(PATH_WRAPPER, 'sorpproplib',
                                 'data', 'verification', 'verification_table.xlsx')


# %% Verification.
#
# Load data
#
DF_VERIFICATION_DATA = pd.read_excel(PATH_VERIFICATION)

df_verification_results = pd.DataFrame(columns=['sorbent',
                                                'subtype',
                                                'refrigerant',
                                                'type',
                                                'isotherm',
                                                'ID',
                                                'pressure',
                                                'temperature',
                                                'loading-literature',
                                                'loading-calculated',
                                                'difference-absolute',
                                                'difference-relative',
                                                'literature'])

# Iterate over verification data
#
for index, row in DF_VERIFICATION_DATA.iterrows():
    # Initialize working pair
    #
    wpair = {'sorbent': row['sorbent'],
             'subtype': '' if isinstance(row['sorbent-subtype'], float) else
                 row['sorbent-subtype'],
             'refrigerant': row['refrigerant']}
    wpair_approach = {'isotherm': row['isotherm'],
                      'id_isotherm': row['ID_isotherm'],
                      'vapor_pressure': row['vaporPressure'],
                      'id_vapor_pressure': row['ID_vaporPressure'],
                      'sat_liq_density': row['saturatedLiquidDensity'],
                      'id_sat_liq_density': row['ID_saturatedLiquidDensity']}
    wpair_class = WPairStruct(wpair, wpair_approach, PATH_SORPPROPLIB, PATH_DB)

    # Calculate properties
    #
    p = row['pressure']
    T = row['temperature']
    w_literature = row['loading']

    if row['type'] == 'ads':
        w_sorpproplib = wpair_class.Ads.w_pT(p, T)

    elif wpair_approach['isotherm'] == 'Antoine' or \
         wpair_approach['isotherm'] == 'Duehring':
        w_sorpproplib = wpair_class.Abs.Con.X_pT(p, T)

    elif wpair_approach['isotherm'] == 'MixingRule':
        w_sorpproplib = wpair_class.Abs.Mix.x_pT(p, T)[0]

    else:
        w_sorpproplib = wpair_class.Abs.Act.x_pTv1v2(p, T, -1, -1)


    # Update data frame containing verification results
    #
    df_verification_results.loc[index, 'sorbent'] = wpair['sorbent']
    df_verification_results.loc[index, 'subtype'] = wpair['subtype']
    df_verification_results.loc[index, 'refrigerant'] = wpair['refrigerant']
    df_verification_results.loc[index, 'type'] = row['type']
    df_verification_results.loc[index, 'isotherm'] = wpair_approach['isotherm']
    df_verification_results.loc[index, 'ID'] = wpair_approach['id_isotherm']
    df_verification_results.loc[index, 'pressure'] = p
    df_verification_results.loc[index, 'temperature'] = T
    df_verification_results.loc[index, 'loading-literature'] = w_literature
    df_verification_results.loc[index, 'loading-calculated'] = w_sorpproplib
    df_verification_results.loc[index, 'difference-absolute'] = w_literature-w_sorpproplib
    df_verification_results.loc[index, 'difference-relative'] = \
        (w_literature-w_sorpproplib)/w_literature*100
    df_verification_results.loc[index, 'literature'] = row['Literature']

    # Delete working pair
    #
    del(wpair_class)
