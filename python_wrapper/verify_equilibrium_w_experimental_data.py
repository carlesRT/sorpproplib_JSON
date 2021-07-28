"""Script 'verify_equilibrium_w_experimental_data'.

This script is an example script to verifiy all working pairs implemented into SorpPropLib with
experimental data provided in the original literature and storted into SorpPropLib. The results
are plotted and saved at '.\sorpproplib\data\verification\EQUATION' as *.png figures.

Revisions:
----------
    Version     Date        Author              Changes
    1.0.0       2021-06-30  Mirko Engelpracht   First implementation
"""

# %% Import relevant modules.
#
import matplotlib.pyplot as plt
import numpy as np
import os
import struct
import sys

from CoolProp.CoolProp import PropsSI
from functools import partial as partial_func
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


# %% Definition of functions.
#
def exceptions_ads_isotherm(EQNAME, wpair, wpair_eq, wpair_class):
    r"""Exceptions for refrigerant functions of adsorption working pairs with isothermal data."""
    # Use nested check to get correct entry of SorpPropLib:
    # First, check for equation type
    # Second, check for literature
    # Second, check for refrigerant
    # Fourth, check for sorbent
    # Fifth, check for subtype
    #
    if EQNAME == 'DubininArctan1' or EQNAME == 'DubininAstakhov':
        # For volumetric approaches, special function for vapor pressure and / or saturated
        # liquid density may be used
        #
        if wpair_eq['_c_'] == 'Rahman, Kazi Afzalur; Chakraborty, Anutosh; Saha, Bidyut Baran; Ng, Kim Choon (2012): On thermodynamics of methane+carbonaceous materials adsorption. In: International Journal of Heat and Mass Transfer 55 (4), S. 565–573. DOI: 10.1016/j.ijheatmasstransfer.2011.10.056.':
            if wpair['k']['_rf_'] == 'Methane':
                if wpair['k']['_as_'] == 'activated carbon powder':
                    if wpair['k']['_st_'] == 'Maxsorb III':
                        def func_w_pT_fix(p, T, wpair_class):
                            p_sat = 4599200 * (T / 190.564) ** 2 if T > 188 else \
                                wpair_class.Ref.p_sat_T(T)
                            rho_ad = 1 / (2.3677e-3 * np.exp(0.0043 * (T - 111.67)))
                            return wpair_class.Ads.Vol.w_pTpsatrho(p, T, p_sat, rho_ad)

                        return partial_func(func_w_pT_fix, wpair_class=wpair_class)


        elif wpair_eq['_c_'] == 'Askalany, Ahmed A.; Saha, Bidyut B.; Uddin, Kutub; Miyzaki, Takahiko; Koyama, Shigeru; Srinivasan, Kandadai; Ismail, Ibrahim M. (2013): Adsorption Isotherms and Heat of Adsorption of Difluoromethane on Activated Carbons. In: J. Chem. Eng. Data 58 (10), S. 2828–2834. DOI: 10.1021/je4005678.':
            if wpair['k']['_rf_'] == 'R-32':
                if wpair['k']['_as_'] == 'activated carbon powder':
                    if wpair['k']['_st_'] == 'Maxsorb III':
                        def func_w_pT_fix(p, T, wpair_class):
                            rho_g = PropsSI('D', 'T', T, 'P', p, 'R32')
                            rho_ad = 1.546*T - rho_g
                            return wpair_class.Ads.Vol.w_pTpsatrho(p, T, wpair_class.Ref.p_sat_T(T), rho_ad)

                    return partial_func(func_w_pT_fix, wpair_class=wpair_class)

                elif wpair['k']['_as_'] == 'activated carbon fiber':
                    if wpair['k']['_st_'] == 'A-20':
                        def func_w_pT_fix(p, T, wpair_class):
                            rho_g = PropsSI('D', 'T', T, 'P', p, 'R32')
                            rho_ad = 1.022*T - rho_g
                            return wpair_class.Ads.Vol.w_pTpsatrho(p, T, wpair_class.Ref.p_sat_T(T), rho_ad)

                    return partial_func(func_w_pT_fix, wpair_class=wpair_class)


        elif wpair_eq['_c_'] == 'Akkimaradi, Basavaraj S.; Prasad, Madhu; Dutta, Pradip; Srinivasan, Kandadai (2001): Adsorption of 1,1,1,2-Tetrafluoroethane on Activated Charcoal. In: J. Chem. Eng. Data 46 (2), S. 417–422. DOI: 10.1021/je000277e.':
            if wpair['k']['_rf_'] == 'R-134a':
                def func_w_pT_fix(p, T, wpair_class):
                    omega = np.log(9.39e-4 / 7.2643e-4) / (374.21 - 246.78)
                    rho_ad = 1 / (7.2643e-4 * np.exp(omega * (T - 246.78)))
                    return wpair_class.Ads.Vol.w_pTpsatrho(p, T, wpair_class.Ref.p_sat_T(T), rho_ad)

                return partial_func(func_w_pT_fix, wpair_class=wpair_class)


        elif wpair_eq['_c_'] == 'Saha, Bidyut B.; Habib, Khairul; El-Sharkawy, Ibrahim I.; Koyama, Shigeru (2009): Adsorption characteristics and heat of adsorption measurements of R-134a on activated carbon. In: International Journal of Refrigeration 32 (7), S. 1563–1569. DOI: 10.1016/j.ijrefrig.2009.03.010.':
            if wpair['k']['_rf_'] == 'R-134a':
                def func_w_pT_fix(p, T, wpair_class):
                    omega = np.log(9.39e-4 / 7.26e-4) / (374.21 - 246.78)
                    rho_ad = 1 / (7.26e-4 * np.exp(omega * (T - 246.78)))
                    return wpair_class.Ads.Vol.w_pTpsatrho(p, T, wpair_class.Ref.p_sat_T(T), rho_ad)

                return partial_func(func_w_pT_fix, wpair_class=wpair_class)


        elif wpair_eq['_c_'] == 'Saha, Bidyut Baran; El-Sharkawy, Ibrahim I.; Thorpe, Roger; Critoph, Robert E. (2012): Accurate adsorption isotherms of R134a onto activated carbons for cooling and freezing applications. In: International Journal of Refrigeration 35 (3), S. 499–505. DOI: 10.1016/j.ijrefrig.2011.05.002.':
            if wpair['k']['_rf_'] == 'R-134a':
                def func_w_pT_fix(p, T, wpair_class):
                    omega = np.log(9.39e-4 / 7.2643e-4) / (374.21 - 246.78)
                    rho_ad = 1 / (7.2643e-4 * np.exp(omega * (T - 246.78)))
                    return wpair_class.Ads.Vol.w_pTpsatrho(p, T, wpair_class.Ref.p_sat_T(T), rho_ad)

                return partial_func(func_w_pT_fix, wpair_class=wpair_class)


        elif wpair_eq['_c_'] == 'Askalany, Ahmed A.; Saha, Bidyut B.; Ismail, Ibrahim M. (2014): Adsorption isotherms and kinetics of HFC410A onto activated carbons. In: Applied Thermal Engineering 72 (2), S. 237–243. DOI: 10.1016/j.applthermaleng.2014.04.075.':
            if wpair['k']['_rf_'] == 'R-407c':
                def func_w_pT_fix(p, T, wpair_class):
                    rho_g = PropsSI('D', 'T', T, 'P', p, 'R407c')
                    rho_ad = 1.0015*T - rho_g
                    return wpair_class.Ads.Vol.w_pTpsatrho(p, T, wpair_class.Ref.p_sat_T(T), rho_ad)

                return partial_func(func_w_pT_fix, wpair_class=wpair_class)


#        elif wpair_eq['_c_'] == 'El-sharkawy, M. M.; Askalany, A. A.; Harby, K.; Ahmed, M. S. (2016): Adsorption isotherms and kinetics of a mixture of Pentafluoroethane, 1,1,1,2-Tetrafluoroethane and Difluoromethane (HFC-407C) onto granular activated carbon. In: Applied Thermal Engineering 93, S. 988–994. DOI: 10.1016/j.applthermaleng.2015.10.077.':
#            if wpair['k']['_rf_'] == 'R-404a':
#                def func_w_pT_fix(p, T, wpair_class):
#                    rho_g = PropsSI('D', 'T', T, 'P', p, 'R404a')
#                    rho_ad = A - B*T - rho_g
#                    return wpair_class.Ads.Vol.w_pTpsatrho(p, T, wpair_class.Ref.p_sat_T(T), rho_ad)
#
#                return partial_func(func_w_pT_fix, wpair_class=wpair_class)


        elif wpair_eq['_c_'] == 'Saha, Bidyut B.; El-Sharkawy, Ibrahim I.; Habib, Khairul; Koyama, Shigeru; Srinivasan, Kandadai (2008): Adsorption of Equal Mass Fraction Near an Azeotropic Mixture of Pentafluoroethane and 1,1,1-Trifluoroethane on Activated Carbon. In: J. Chem. Eng. Data 53 (8), S. 1872–1876. DOI: 10.1021/je800204p.':
            if wpair['k']['_rf_'] == '507a':
                def func_w_pT_fix(p, T, wpair_class):
                    return wpair_class.Ads.Vol.w_pTpsatrho(p, T, wpair_class.Ref.p_sat_T(T), -1)

                return partial_func(func_w_pT_fix, wpair_class=wpair_class)


        elif wpair_eq['_c_'] == 'Loh, Wai Soong; Ismail, Azhar Bin; Xi, Baojuan; Ng, Kim Choon; Chun, Won Gee (2012): Adsorption Isotherms and Isosteric Enthalpy of Adsorption for Assorted Refrigerants on Activated Carbons. In: J. Chem. Eng. Data 57 (10), S. 2766–2773. DOI: 10.1021/je3008099.':
            if wpair['k']['_rf_'] == '507a':
                def func_w_pT_fix(p, T, wpair_class):
                    return wpair_class.Ads.Vol.w_pTpsatrho(p, T, wpair_class.Ref.p_sat_T(T), -1)

                return partial_func(func_w_pT_fix, wpair_class=wpair_class)

    # Return standard approach
    #
    return wpair_class.Ads.w_pT


def exceptions_abs_isosteres(EQNAME, wpair, wpair_eq, wpair_class):
    r"""Exceptions for refrigerant functions of absorption working pairs with isosteric data."""
    # Use nested check to get correct entry of SorpPropLib:
    # First, check for equation type
    # Second, check for literature
    # Second, check for refrigerant
    # Fourth, check for sorbent
    # Fifth, check for subtype
    #
    if EQNAME == 'Antoine' or EQNAME == 'Duehring':
        # Pass correct low-level function
        #
        return wpair_class.Abs.Con.p_XT

    elif EQNAME == 'MixingRule':
        # Pass correct low-level function and get correct element from tuple
        #
        def func_p_wT_correct(w, T, wpair_class):
                return wpair_class.Abs.Mix.p_Tx(T, w)[0]

        return partial_func(func_p_wT_correct, wpair_class=wpair_class)

    else:
        # Pass correct low-level function. Either use molar volumes as stored in data base or
        # use molar volumes given as inputs
        if wpair_eq['_c_'] == 'Burton, C.; Jacobi, A. M.; Mehendale, S. S. (1999): Vapor-liquid equilibrium for R-32 and R-410A mixed with a polyol ester. Non-ideality and local composition modeling. In: International Journal of Refrigeration 22 (6), S. 458–471. DOI: 10.1016/S0140-7007(99)00012-2.':
            if wpair['k']['_rf_'] == 'R-32':
                if wpair['k']['_as_'] == 'lubricant':
                    if wpair['k']['_st_'] == 'POE':
                        def func_x_pT_correct(w, T, wpair_class):
                                return wpair_class.Abs.Act.p_Txv1v2(T, w,
                                                                    1/PropsSI('D', 'T', T, 'Q', 0, 'R32')*0.052024,
                                                                    1/(993.89-0.75658*(T-273.15))*0.7)
                        return partial_func(func_x_pT_correct, wpair_class=wpair_class)

            elif wpair['k']['_rf_'] == 'R-125':
                if wpair['k']['_as_'] == 'lubricant':
                    if wpair['k']['_st_'] == 'POE':
                        def func_x_pT_correct(w, T, wpair_class):
                                return wpair_class.Abs.Act.p_Txv1v2(T, w,
                                                                    1/PropsSI('D', 'T', T, 'Q', 0, 'R125')*0.12002,
                                                                    1/(993.89-0.75658*(T-273.15))*0.7)
                        return partial_func(func_x_pT_correct, wpair_class=wpair_class)


        elif wpair_eq['_c_'] == 'Bock, Jessica (2015): VAPOR-LIQUID EQUILIBRIA OF A LOW GWP REFRIGERANT, R-1234ZE(E), MIXED WITH A POE LUBRICANT. PhD Thesis. University of Illinois at Urbana-Champaign, Illinois.':
            if wpair['k']['_rf_'] == 'R-1234ze(E)':
                if wpair['k']['_as_'] == 'lubricant':
                    if wpair['k']['_st_'] == 'POE':
                        def func_x_pT_correct(w, T, wpair_class):
                                return wpair_class.Abs.Act.p_Txv1v2psat(T, w,
                                                                        1/PropsSI('D', 'T', T, 'Q', 0, 'R1234ze(E)')*0.1140415928,
                                                                        1/(993.89-0.75658*(T-273.15))*0.7,
                                                                        PropsSI('P', 'T', T, 'Q', 0, 'R1234ze(E)'))
                        return partial_func(func_x_pT_correct, wpair_class=wpair_class)


        elif wpair_eq['_c_'] == 'Grebner, J. J. (1992): The Effects of Oil on the Thermodynamic Properties of Dichlorodifluoromethane (R-12) and Tetrafluoroethane (R-134a). Hg. v. Air Conditioning and Refrigeration Center. College of Engineering. University of Illinois at Urbana-Champaign. Air Conditioning and Refrigeration Center TR-13. Online verfügbar unter http://hdl.handle.net/2142/9702.':
            if wpair['k']['_rf_'] == 'R-12':
                if wpair['k']['_as_'] == 'naphthenic':
                    if wpair['k']['_st_'] == '':
                        def func_x_pT_correct(w, T, wpair_class):
                                return wpair_class.Abs.Act.p_Txv1v2(T, w,
                                                                    1/PropsSI('D', 'T', T, 'Q', 0, 'R12')*0.120913,
                                                                    1/((56.211-0.020808*((T-273.15)*1.8+32))*16.0185)*0.325)
                        return partial_func(func_x_pT_correct, wpair_class=wpair_class)

                elif wpair['k']['_as_'] == 'paraffinic':
                    if wpair['k']['_st_'] == '':
                        def func_x_pT_correct(w, T, wpair_class):
                                return wpair_class.Abs.Act.p_Txv1v2(T, w,
                                                                    1/PropsSI('D', 'T', T, 'Q', 0, 'R12')*0.120913,
                                                                    1/((55.37-0.020428*((T-273.15)*1.8+32))*16.0185)*0.5)
                        return partial_func(func_x_pT_correct, wpair_class=wpair_class)

            if wpair['k']['_rf_'] == 'R-134a':
                if wpair['k']['_as_'] == 'lubricant':
                    if wpair['k']['_st_'] == 'POE':
                        def func_x_pT_correct(w, T, wpair_class):
                                return wpair_class.Abs.Act.p_Txv1v2(T, w,
                                                                    1/PropsSI('D', 'T', T, 'Q', 0, 'R134a')*0.102032,
                                                                    1/((61.387-0.024138*((T-273.15)*1.8+32))*16.0185)*0.59)
                        return partial_func(func_x_pT_correct, wpair_class=wpair_class)


        elif wpair_eq['_c_'] == 'Martz, W. L.; Jacobi, A. M. (1994): Refrigerant-Oil Mixtures and Local Composition Modeling. Hg. v. Air Conditioning and Refrigeration Center. College of Engineering. University of Illinois at Urbana-Champaign. Air Conditioning and Refrigeration Center TR-68. Online verfügbar unter http://hdl.handle.net/2142/10986.':
            if wpair['k']['_rf_'] == 'R-22':
                if wpair['k']['_as_'] == 'lubricant':
                    if wpair['k']['_st_'] == 'POE':
                        def func_x_pT_correct(w, T, wpair_class):
                                return wpair_class.Abs.Act.p_Txv1v2(T, w,
                                                                    1/PropsSI('D', 'T', T, 'Q', 0, 'R12')*0.086468,
                                                                    1/((61.387-0.024138*((T-273.15)*1.8+32))*16.0185)*0.59)
                        return partial_func(func_x_pT_correct, wpair_class=wpair_class)

            if wpair['k']['_rf_'] == 'R-134a':
                if wpair['k']['_as_'] == 'lubricant':
                    if wpair['k']['_st_'] == 'POE':
                        def func_x_pT_correct(w, T, wpair_class):
                                return wpair_class.Abs.Act.p_Txv1v2(T, w,
                                                                    1/PropsSI('D', 'T', T, 'Q', 0, 'R134a')*0.102032,
                                                                    1/((61.387-0.024138*((T-273.15)*1.8+32))*16.0185)*0.59)
                        return partial_func(func_x_pT_correct, wpair_class=wpair_class)

                    elif wpair['k']['_st_'] == 'PAG':
                        def func_x_pT_correct(w, T, wpair_class):
                                return wpair_class.Abs.Act.p_Txv1v2(T, w,
                                                                    1/PropsSI('D', 'T', T, 'Q', 0, 'R134a')*0.102032,
                                                                    1/((62.365-0.024864*((T-273.15)*1.8+32))*16.0185)*2)
                        return partial_func(func_x_pT_correct, wpair_class=wpair_class)


        def func_p_wT_correct(w, T, wpair_class):
                return wpair_class.Abs.Act.p_Txv1v2(T, w, -1, -1)

        return partial_func(func_p_wT_correct, wpair_class=wpair_class)


def exceptions_abs_isotherms(EQNAME, wpair, wpair_eq, wpair_class):
    r"""Exceptions for refrigerant functions of absorption working pairs with isothermal data."""
    # Use nested check to get correct entry of SorpPropLib:
    # First, check for equation type
    # Second, check for literature
    # Second, check for refrigerant
    # Fourth, check for sorbent
    # Fifth, check for subtype
    #
    if EQNAME == 'Antoine' or EQNAME == 'Duehring':
        # Pass correct low-level function
        #
        return wpair_class.Abs.Con.X_pT

    elif EQNAME == 'MixingRule':
        # Pass correct low-level function and get correct element from tuple
        #
        def func_x_pT_correct(p, T, wpair_class):
                return wpair_class.Abs.Mix.x_pT(p, T)[0]

        return partial_func(func_x_pT_correct, wpair_class=wpair_class)

    else:
        # Pass correct low-level function. Either use molar volumes as stored in data base or
        # use molar volumes given as inputs
        if wpair_eq['_c_'] == 'Marchi, Paolo; Scalabrin, Giancarlo; Ihmels, E. Christian; Fischer, Kai; Gmehling, Jürgen (2006): Bubble pressure measurements for the (1,1,1,2-tetrafluoroethane+triethylene glycol dimethyl ether) system. In: The Journal of Chemical Thermodynamics 38 (11), S. 1247–1253. DOI: 10.1016/j.jct.2006.03.004.':
            if wpair['k']['_rf_'] == 'R-134':
                if wpair['k']['_as_'] == 'TriEGDME':
                    if wpair['k']['_st_'] == '':
                        def func_x_pT_correct(p, T, wpair_class):
                                return wpair_class.Abs.x_pTv1v2(p, T,
                                                                1/PropsSI('D', 'T', T, 'Q', 0, 'R134')*0.102032,
                                                                1)
                        return partial_func(func_x_pT_correct, wpair_class=wpair_class)


        def func_x_pT_correct(p, T, wpair_class):
                return wpair_class.Abs.Act.x_pTv1v2(p, T, -1, -1)
        return partial_func(func_x_pT_correct, wpair_class=wpair_class)


def compare_data(PATH_DB: str, EQNAME: str):
    """Compare experimental and calculated data.

    This functions calculates and plots data for comparison with experimental data if
    experimental data exists for selected isotherm type.

    Parameters
    ----------
    PATH_DB : str
        Path to json data base.
    EQNAME : str
        Isotherm type.

    Returns
    -------
    None.

    """
    # Load JSON data base
    #
    with open(PATH_DB, "r") as read_file:
        json_file = load(read_file)

    # Get list containing working pairs for specified isotherm type
    #
    list_working_pairs = []

    for _, entry in enumerate(json_file):
        if EQNAME in entry['v']['_ep_']:
            list_working_pairs.append(entry)

    # Iterate over selected working pairs
    #
    for _, wpair in enumerate(list_working_pairs):
        # Iterate over all fits
        #
        for ind_eq, wpair_eq in enumerate(wpair['v']['_ep_'][EQNAME]):
            # Check for experimental data and add data to list: Use literature as criterion
            #
            list_wpair_ed = []

            for _, wpair_ed in enumerate(wpair['v']['_ed_']):
                if wpair_ed['_c_'] == wpair_eq['_c_']:
                    list_wpair_ed.append(wpair_ed)

            # Create working pair class: First, check if approaches exist for vapor pressure and
            # saturated liquid density
            #
            for _, entry in enumerate(json_file):
                if entry['k']['_rf_'] == wpair['k']['_rf_'] and entry['k']['_tp_'] == 'refrig':
                    if 'VaporPressure_EoS1' in entry['v']['_ep_']:
                        tmp_vap_pressure = 'VaporPressure_EoS1'
                    elif 'VaporPressure_EoS2' in entry['v']['_ep_']:
                        tmp_vap_pressure = 'VaporPressure_EoS2'
                    elif 'VaporPressure_EoS3' in entry['v']['_ep_']:
                        tmp_vap_pressure = 'VaporPressure_EoS3'
                    elif 'VaporPressure_EoSCubic' in entry['v']['_ep_']:
                        tmp_vap_pressure = 'VaporPressure_EoSCubic'
                    elif 'VaporPressure_Antoine' in entry['v']['_ep_']:
                        tmp_vap_pressure = 'VaporPressure_Antoine'
                    else:
                        tmp_vap_pressure = 'NoVaporPressure'


                    if 'SaturatedLiquidDensity_EoS1' in entry['v']['_ep_']:
                        tmp_sat_liq_density = 'SaturatedLiquidDensity_EoS1'
                    else:
                        tmp_sat_liq_density = 'NoSaturatedLiquidDensity'

                    break

            wpair_name = {'sorbent': wpair['k']['_as_'],
                          'subtype': wpair['k']['_st_'],
                          'refrigerant': wpair['k']['_rf_']}
            wpair_approach = {'isotherm': EQNAME,
                              'id_isotherm': ind_eq+1,
                              'vapor_pressure': tmp_vap_pressure,
                              'id_vapor_pressure': 1,
                              'sat_liq_density': tmp_sat_liq_density,
                              'id_sat_liq_density': 1}
            wpair_class = WPairStruct(wpair_name, wpair_approach, PATH_SORPPROPLIB, PATH_DB)

            # Perform calculations
            #
            if not list_wpair_ed:
                print('No experimental data available: ' + \
                      wpair['k']['_as_'] + ' ' +  wpair['k']['_st_'] + ' & ' + \
                      wpair['k']['_rf_'] + '!')


            else:
                # Kind of calculation depends on type: Adsorption or absorption
                #
                if wpair['k']['_tp_'] == 'ads':
                    # Iterate over experimental data: Calculate and plot data
                    #
                    for ind_ed, wpair_ed in enumerate(list_wpair_ed):
                        # Plot depends on type of experimental data: isotherms, isobars, isosteres,
                        # adsorption potentials, or volumetric loadings
                        #
                        if wpair_ed['_m_']['type'] == 'isotherms':
                            # Get raw data
                            #
                            p_raw = wpair_ed['_d_']['pressures']
                            T_raw = wpair_ed['_d_']['temperatures']
                            w_raw = wpair_ed['_d_']['loadings']

                            T_unique = sorted(list(set(T_raw)))

                            # Hard-coded exception as defined working pairs do not have function
                            # for saturated liquid density OR neeed special function for density
                            # of adsorpt that are not provided into the SorpPropLib.
                            #
                            func_w_pT = exceptions_ads_isotherm(EQNAME, wpair, wpair_eq, wpair_class)

                            # Create figure
                            #
                            fig, ax = plt.subplots(1, 1, figsize=(25.5/2.54, 25/2.54))

                            # Iterate over unique temperatures to calculate and plot data
                            #
                            mape = 0

                            for _, T_unique_i in enumerate(T_unique):
                                p_plot = []
                                T_plot = []
                                w_plot = []

                                for tmp_ind, tmp_val in enumerate(T_raw):
                                    # Get data sorted by temperatures
                                    #
                                    if T_unique_i == tmp_val:
                                        p_plot.append(p_raw[tmp_ind])
                                        T_plot.append(tmp_val)
                                        w_plot.append(w_raw[tmp_ind])

                                        mape += np.abs(w_raw[tmp_ind] -
                                                       func_w_pT(p_raw[tmp_ind], tmp_val)) \
                                                / max(w_raw[tmp_ind], 1e-12)

                                w_calc = [func_w_pT(tmp_val, T_unique_i) for \
                                          tmp_val in sorted(p_plot)]

                                # Plot data
                                #
                                ax.scatter(p_plot, w_plot,
                                           label='Experiment: ' + str(T_unique_i) + ' K')
                                ax.plot(sorted(p_plot),  w_calc,
                                        label='Fit: ' + str(T_unique_i) + ' K')

                                # Set axe, legends, and font
                                #
                                ax.spines['top'].set_visible(False)
                                ax.spines['right'].set_visible(False)
                                ax.spines['left'].set_linewidth(0.5)
                                ax.spines['bottom'].set_linewidth(0.5)

                                ax.set_xlabel('Pressure p in Pa')
                                ax.set_ylabel('Loading in kg/kg')

                                fig.suptitle(wpair['k']['_as_'] + ' ' +
                                             wpair['k']['_st_'] + ' & ' +
                                             wpair['k']['_rf_'] + ' | ' +
                                             EQNAME + ' ID = ' +
                                             str(ind_eq+1) + ' | ' +
                                             'Exp. data ID = ' +
                                             str(ind_ed+1) + ' | ' +
                                             'MAE = ' +
                                             str(round(mape/len(p_raw)*100, 2)) + ' %')

                                ax.legend(loc='best', labelspacing=0.05, frameon=False)

                                # Tight layout
                                #
                                fig.align_ylabels()
                                fig.tight_layout()

                                # Save figure
                                #
                                tmp_path = os.path.join(PATH_WRAPPER,
                                                        'sorpproplib',
                                                        'data',
                                                        'verification',
                                                        EQNAME)
                                tmp_name = wpair['k']['_as_'].replace('/', '_') + '_' + \
                                           wpair['k']['_st_'].replace('/', '_') + '__' + \
                                           wpair['k']['_rf_'] + '__' + \
                                           EQNAME + '_' + \
                                           str(ind_eq+1) + '__' + \
                                           'ExpData_' + \
                                           str(ind_ed+1) + \
                                           '.png'

                                if not os.path.exists(tmp_path):
                                    os.mkdir(tmp_path)

                                fig.savefig(os.path.join(tmp_path, tmp_name))
                                plt.close()

                        elif wpair_ed['_m_']['type'] == 'isobars':
                            # Get raw data
                            #
                            p_raw = wpair_ed['_d_']['pressures']
                            T_raw = wpair_ed['_d_']['temperatures']
                            w_raw = wpair_ed['_d_']['loadings']

                            p_unique = sorted(list(set(p_raw)))

                            # Create figure
                            #
                            fig, ax = plt.subplots(1, 1, figsize=(25.5/2.54, 25/2.54))

                            # Iterate over unique temperatures to calculate and plot data
                            #
                            mape = 0

                            for _, p_unique_i in enumerate(p_unique):
                                p_plot = []
                                T_plot = []
                                w_plot = []

                                for tmp_ind, tmp_val in enumerate(p_raw):
                                    # Get data sorted by temperatures
                                    #
                                    if p_unique_i == tmp_val:
                                        p_plot.append(tmp_val)
                                        T_plot.append(T_raw[tmp_ind])
                                        w_plot.append(w_raw[tmp_ind])

                                        mape += np.abs(w_raw[tmp_ind] -
                                                       wpair_class.Ads.w_pT(tmp_val,
                                                                            T_raw[tmp_ind])) \
                                                / max(w_raw[tmp_ind], 1e-12)

                                w_calc = [wpair_class.Ads.w_pT(p_unique_i, tmp_val) for \
                                          tmp_val in sorted(T_plot)]

                                # Plot data
                                #
                                ax.scatter(T_plot, w_plot,
                                           label='Experiment: ' + str(p_unique_i) + ' Pa')
                                ax.plot(sorted(T_plot), w_calc,
                                        label='Fit: ' + str(p_unique_i) + ' Pa')

                                # Set axe, legends, and font
                                #
                                ax.spines['top'].set_visible(False)
                                ax.spines['right'].set_visible(False)
                                ax.spines['left'].set_linewidth(0.5)
                                ax.spines['bottom'].set_linewidth(0.5)

                                ax.set_xlabel('Temperature T in K')
                                ax.set_ylabel('Loading in kg/kg')

                                fig.suptitle(wpair['k']['_as_'] + ' ' +
                                             wpair['k']['_st_'] + ' & ' +
                                             wpair['k']['_rf_'] + ' | ' +
                                             EQNAME + ' ID = ' +
                                             str(ind_eq+1) + ' | ' +
                                             'Exp. data ID = ' +
                                             str(ind_ed+1) + ' | ' +
                                             'MAE = ' +
                                             str(round(mape/len(p_raw)*100, 2)) + ' %')

                                ax.legend(loc='best', labelspacing=0.05, frameon=False)

                                # Tight layout
                                #
                                fig.align_ylabels()
                                fig.tight_layout()

                                # Save figure
                                #
                                tmp_path = os.path.join(PATH_WRAPPER,
                                                        'sorpproplib',
                                                        'data',
                                                        'verification',
                                                        EQNAME)
                                tmp_name = wpair['k']['_as_'] + '_' + \
                                           wpair['k']['_st_'].replace('/', '_') + '__' + \
                                           wpair['k']['_rf_'] + '__' + \
                                           EQNAME + '_' + \
                                           str(ind_eq+1) + '__' + \
                                           'ExpData_' + \
                                           str(ind_ed+1) + \
                                           '.png'

                                if not os.path.exists(tmp_path):
                                    os.mkdir(tmp_path)

                                fig.savefig(os.path.join(tmp_path, tmp_name))
                                plt.close()

                        elif wpair_ed['_m_']['type'] == 'isosteres':
                            # Get raw data
                            #
                            p_raw = wpair_ed['_d_']['pressures']
                            T_raw = wpair_ed['_d_']['temperatures']
                            w_raw = wpair_ed['_d_']['loadings']

                            w_unique = sorted(list(set(w_raw)))

                            # Create figure
                            #
                            fig, ax = plt.subplots(1, 1, figsize=(25.5/2.54, 25/2.54))

                            # Iterate over unique temperatures to calculate and plot data
                            #
                            mape = 0

                            for _, w_unique in enumerate(w_unique):
                                p_plot = []
                                T_plot = []
                                w_plot = []

                                for tmp_ind, tmp_val in enumerate(w_raw):
                                    # Get data sorted by temperatures
                                    #
                                    if w_unique == tmp_val:
                                        p_plot.append(p_raw[tmp_ind])
                                        T_plot.append(T_raw[tmp_ind])
                                        w_plot.append(tmp_val)

                                        mape += np.abs(p_raw[tmp_ind] -
                                                       wpair_class.Ads.p_wT(tmp_val,
                                                                            T_raw[tmp_ind])) \
                                                / max(p_raw[tmp_ind], 1e-12)

                                p_calc = [wpair_class.Ads.p_wT(w_unique, tmp_val) for \
                                          tmp_val in sorted(T_plot)]

                                # Plot data
                                #
                                ax.scatter(-1/np.array(T_plot), p_plot,
                                           label='Experiment: ' + str(w_unique*100) + ' %')
                                ax.plot(-1/np.array(sorted(T_plot)), p_calc,
                                        label='Fit: ' + str(w_unique*100) + ' %')

                                # Set axe, legends, and font
                                #
                                ax.spines['top'].set_visible(False)
                                ax.spines['right'].set_visible(False)
                                ax.spines['left'].set_linewidth(0.5)
                                ax.spines['bottom'].set_linewidth(0.5)

                                ax.set_yscale('log')

                                ax.set_xlabel('Negative inverse temperature in -1/K')
                                ax.set_ylabel('Pressure in Pa')

                                fig.suptitle(wpair['k']['_as_'] + ' ' +
                                             wpair['k']['_st_'] + ' & ' +
                                             wpair['k']['_rf_'] + ' | ' +
                                             EQNAME + ' ID = ' +
                                             str(ind_eq+1) + ' | ' +
                                             'Exp. data ID = ' +
                                             str(ind_ed+1) + ' | ' +
                                             'MAE = ' +
                                             str(round(mape/len(p_raw)*100, 2)) + ' %')

                                ax.legend(loc='best', labelspacing=0.05, frameon=False)

                                # Tight layout
                                #
                                fig.align_ylabels()
                                fig.tight_layout()

                                # Save figure
                                #
                                tmp_path = os.path.join(PATH_WRAPPER,
                                                        'sorpproplib',
                                                        'data',
                                                        'verification',
                                                        EQNAME)
                                tmp_name = wpair['k']['_as_'] + '_' + \
                                           wpair['k']['_st_'].replace('/', '_') + '__' + \
                                           wpair['k']['_rf_'] + '__' + \
                                           EQNAME + '_' + \
                                           str(ind_eq+1) + '__' + \
                                           'ExpData_' + \
                                           str(ind_ed+1) + \
                                           '.png'

                                if not os.path.exists(tmp_path):
                                    os.mkdir(tmp_path)

                                fig.savefig(os.path.join(tmp_path, tmp_name))
                                plt.close()

                        elif wpair_ed['_m_']['type'] == 'adsorption potentials':
                            # Get raw data
                            #
                            A_raw = wpair_ed['_d_']['adsorptionPotentials']
                            W_raw = wpair_ed['_d_']['volumetricLoadings']

                            # Create figure
                            #
                            fig, ax = plt.subplots(1, 1, figsize=(25.5/2.54, 25/2.54))

                            # Iterate over unique temperatures to calculate and plot data
                            #
                            mape = 0

                            for tmp_ind, tmp_val in enumerate(A_raw):
                                mape += np.abs(W_raw[tmp_ind] -
                                               wpair_class.Ads.Vol.W_ARho(tmp_val, 1)) \
                                        / max(W_raw[tmp_ind], 1e-12)

                            W_calc = [wpair_class.Ads.Vol.W_ARho(tmp_val, 1) for \
                                      tmp_val in sorted(A_raw)]

                            # Plot data
                            #
                            ax.scatter(A_raw, W_raw, label='Experiment')
                            ax.plot(sorted(A_raw), W_calc, label='Fit')

                            # Set axe, legends, and font
                            #
                            ax.spines['top'].set_visible(False)
                            ax.spines['right'].set_visible(False)
                            ax.spines['left'].set_linewidth(0.5)
                            ax.spines['bottom'].set_linewidth(0.5)

                            ax.set_xlabel('Adsorption potential A in J/mol')
                            ax.set_ylabel('Volumetric loading W in m³/kg')

                            fig.suptitle(wpair['k']['_as_'] + ' ' +
                                         wpair['k']['_st_'] + ' & ' +
                                         wpair['k']['_rf_'] + ' | ' +
                                         EQNAME + ' ID = ' +
                                         str(ind_eq+1) + ' | ' +
                                         'Exp. data ID = ' +
                                         str(ind_ed+1) + ' | ' +
                                         'MAE = ' +
                                         str(round(mape/len(A_raw)*100, 2)) + ' %')

                            ax.legend(loc='best', labelspacing=0.05, frameon=False)

                            # Tight layout
                            #
                            fig.align_ylabels()
                            fig.tight_layout()

                            # Save figure
                            #
                            tmp_path = os.path.join(PATH_WRAPPER,
                                                    'sorpproplib',
                                                    'data',
                                                    'verification',
                                                    EQNAME)
                            tmp_name = wpair['k']['_as_'] + '_' + \
                                       wpair['k']['_st_'].replace('/', '_') + '__' + \
                                       wpair['k']['_rf_'] + '__' + \
                                       EQNAME + '_' + \
                                       str(ind_eq+1) + '__' + \
                                       'ExpData_' + \
                                       str(ind_ed+1) + \
                                       '.png'

                            if not os.path.exists(tmp_path):
                                os.mkdir(tmp_path)

                            fig.savefig(os.path.join(tmp_path, tmp_name))
                            plt.close()


                elif wpair['k']['_tp_'] == 'abs':
                    # Iterate over experimental data: Calculate and plot data
                    #
                    for ind_ed, wpair_ed in enumerate(list_wpair_ed):
                        # Plot depends on type of experimental data: isotherms, isobars, isosteres,
                        # adsorption potentials, or volumetric loadings
                        #
                        if wpair_ed['_m_']['type'] == 'isotherms':
                            # Get raw data
                            #
                            p_raw = wpair_ed['_d_']['pressures']
                            T_raw = wpair_ed['_d_']['temperatures']
                            w_raw = wpair_ed['_d_']['loadings']

                            T_unique = sorted(list(set(T_raw)))

                            # Hard-coded exception as defined working pairs do not have function
                            # for molar volumes of both components and as correct low-level
                            # function must be passed.
                            #
                            func_w_pT = exceptions_abs_isotherms(EQNAME, wpair, wpair_eq, wpair_class)

                            # Create figure
                            #
                            fig, ax = plt.subplots(1, 1, figsize=(25.5/2.54, 25/2.54))

                            # Iterate over unique temperatures to calculate and plot data
                            #
                            mape = 0

                            for _, T_unique_i in enumerate(T_unique):
                                p_plot = []
                                T_plot = []
                                w_plot = []

                                for tmp_ind, tmp_val in enumerate(T_raw):
                                    # Get data sorted by temperatures
                                    #
                                    if T_unique_i == tmp_val:
                                        p_plot.append(p_raw[tmp_ind])
                                        T_plot.append(tmp_val)
                                        w_plot.append(w_raw[tmp_ind])

                                        mape += np.abs(w_raw[tmp_ind] -
                                                       func_w_pT(p_raw[tmp_ind], tmp_val)) \
                                                / max(w_raw[tmp_ind], 1e-12)

                                w_calc = [func_w_pT(tmp_val, T_unique_i) for \
                                          tmp_val in sorted(p_plot)]

                                # Plot data
                                #
                                ax.scatter(p_plot,
                                           w_plot,
                                           label='Experiment: ' + str(T_unique_i) + ' K')
                                ax.plot(sorted(p_plot),
                                        w_calc,
                                        label='Fit: ' + str(T_unique_i) + ' K')

                                # Set axe, legends, and font
                                #
                                ax.spines['top'].set_visible(False)
                                ax.spines['right'].set_visible(False)
                                ax.spines['left'].set_linewidth(0.5)
                                ax.spines['bottom'].set_linewidth(0.5)

                                ax.set_xlabel('Pressure p in Pa')
                                ax.set_ylabel('Loading in kg/kg or mol/mol')

                                fig.suptitle(wpair['k']['_as_'] + ' ' +
                                             wpair['k']['_st_'] + ' & ' +
                                             wpair['k']['_rf_'] + ' | ' +
                                             EQNAME + ' ID = ' +
                                             str(ind_eq+1) + ' | ' +
                                             'Exp. data ID = ' +
                                             str(ind_ed+1) + ' | ' +
                                             'MAE = ' +
                                             str(round(mape/len(p_raw)*100, 2)) + ' %')

                                ax.legend(loc='best', labelspacing=0.05, frameon=False)

                                # Tight layout
                                #
                                fig.align_ylabels()
                                fig.tight_layout()

                                # Save figure
                                #
                                tmp_path = os.path.join(PATH_WRAPPER,
                                                        'sorpproplib',
                                                        'data',
                                                        'verification',
                                                        EQNAME)
                                tmp_name = wpair['k']['_as_'] + '_' + \
                                           wpair['k']['_st_'].replace('/', '_') + '__' + \
                                           wpair['k']['_rf_'] + '__' + \
                                           EQNAME + '_' + \
                                           str(ind_eq+1) + '__' + \
                                           'ExpData_' + \
                                           str(ind_ed+1) + \
                                           '.png'

                                if not os.path.exists(tmp_path):
                                    os.mkdir(tmp_path)

                                fig.savefig(os.path.join(tmp_path, tmp_name))
                                plt.close()

                        elif wpair_ed['_m_']['type'] == 'isotherms_p':
                            # Get raw data
                            #
                            p_raw = wpair_ed['_d_']['pressures']
                            T_raw = wpair_ed['_d_']['temperatures']
                            w_raw = wpair_ed['_d_']['loadings']

                            T_unique = sorted(list(set(T_raw)))

                            # Hard-coded exception as defined working pairs do not have function
                            # for molar volumes of both components and as correct low-level
                            # function must be passed.
                            #
                            func_w_pT = exceptions_abs_isosteres(EQNAME, wpair, wpair_eq, wpair_class)

                            # Create figure
                            #
                            fig, ax = plt.subplots(1, 1, figsize=(25.5/2.54, 25/2.54))

                            # Iterate over unique temperatures to calculate and plot data
                            #
                            mape = 0

                            for _, T_unique_i in enumerate(T_unique):
                                p_plot = []
                                T_plot = []
                                w_plot = []

                                for tmp_ind, tmp_val in enumerate(T_raw):
                                    # Get data sorted by temperatures
                                    #
                                    if T_unique_i == tmp_val:
                                        p_plot.append(p_raw[tmp_ind])
                                        T_plot.append(tmp_val)
                                        w_plot.append(w_raw[tmp_ind])

                                        mape += np.abs(p_raw[tmp_ind] -
                                                       func_w_pT(w_raw[tmp_ind], tmp_val)) \
                                                / max(p_raw[tmp_ind], 1e-12)

                                p_calc = [func_w_pT(tmp_val, T_unique_i) for \
                                          tmp_val in sorted(w_plot)]

                                # Plot data
                                #
                                ax.scatter(w_plot,
                                           p_plot,
                                           label='Experiment: ' + str(T_unique_i) + ' K')
                                ax.plot(sorted(w_plot),
                                        p_calc,
                                        label='Fit: ' + str(T_unique_i) + ' K')

                                # Set axe, legends, and font
                                #
                                ax.spines['top'].set_visible(False)
                                ax.spines['right'].set_visible(False)
                                ax.spines['left'].set_linewidth(0.5)
                                ax.spines['bottom'].set_linewidth(0.5)

                                ax.set_xlabel('Loading in kg/kg or mol/mol')
                                ax.set_ylabel('Pressure p in Pa')

                                fig.suptitle(wpair['k']['_as_'] + ' ' +
                                             wpair['k']['_st_'] + ' & ' +
                                             wpair['k']['_rf_'] + ' | ' +
                                             EQNAME + ' ID = ' +
                                             str(ind_eq+1) + ' | ' +
                                             'Exp. data ID = ' +
                                             str(ind_ed+1) + ' | ' +
                                             'MAE = ' +
                                             str(round(mape/len(p_raw)*100, 2)) + ' %')

                                ax.legend(loc='best', labelspacing=0.05, frameon=False)

                                # Tight layout
                                #
                                fig.align_ylabels()
                                fig.tight_layout()

                                # Save figure
                                #
                                tmp_path = os.path.join(PATH_WRAPPER,
                                                        'sorpproplib',
                                                        'data',
                                                        'verification',
                                                        EQNAME)
                                tmp_name = wpair['k']['_as_'] + '_' + \
                                           wpair['k']['_st_'].replace('/', '_') + '__' + \
                                           wpair['k']['_rf_'] + '__' + \
                                           EQNAME + '_' + \
                                           str(ind_eq+1) + '__' + \
                                           'ExpData_' + \
                                           str(ind_ed+1) + \
                                           '.png'

                                if not os.path.exists(tmp_path):
                                    os.mkdir(tmp_path)

                                fig.savefig(os.path.join(tmp_path, tmp_name))
                                plt.close()

                        elif wpair_ed['_m_']['type'] == 'isosteres':
                            # Get raw data and correct function
                            #
                            p_raw = wpair_ed['_d_']['pressures']
                            T_raw = wpair_ed['_d_']['temperatures']
                            w_raw = wpair_ed['_d_']['loadings']

                            w_unique = sorted(list(set(w_raw)))

                            # Hard-coded exception as defined working pairs do not have function
                            # for molar volumes of both components and as correct low-level
                            # function must be passed.
                            #
                            func_p_wT = exceptions_abs_isosteres(EQNAME, wpair, wpair_eq, wpair_class)

                            # Create figure
                            #
                            fig, ax = plt.subplots(1, 1, figsize=(25.5/2.54, 25/2.54))

                            # Iterate over unique temperatures to calculate and plot data
                            #
                            mape = 0

                            for _, w_unique in enumerate(w_unique):
                                p_plot = []
                                T_plot = []
                                w_plot = []

                                for tmp_ind, tmp_val in enumerate(w_raw):
                                    # Get data sorted by temperatures
                                    #
                                    if w_unique == tmp_val:
                                        p_plot.append(p_raw[tmp_ind])
                                        T_plot.append(T_raw[tmp_ind])
                                        w_plot.append(tmp_val)

                                        mape += np.abs(p_raw[tmp_ind] -
                                                       func_p_wT(tmp_val, T_raw[tmp_ind])) \
                                                / max(p_raw[tmp_ind], 1e-12)

                                p_calc = [func_p_wT(w_unique, tmp_val) for \
                                          tmp_val in sorted(T_plot)]

                                # Plot data
                                #
                                ax.scatter(-1/np.array(T_plot), p_plot,
                                           label='Experiment: ' + str(w_unique*100) + ' %')
                                ax.plot(-1/np.array(sorted(T_plot)), p_calc,
                                        label='Fit: ' + str(w_unique*100) + ' %')

                                # Set axe, legends, and font
                                #
                                ax.spines['top'].set_visible(False)
                                ax.spines['right'].set_visible(False)
                                ax.spines['left'].set_linewidth(0.5)
                                ax.spines['bottom'].set_linewidth(0.5)

                                ax.set_yscale('log')

                                ax.set_xlabel('Negative inverse temperature in -1/K')
                                ax.set_ylabel('Pressure in Pa')

                                fig.suptitle(wpair['k']['_as_'] + ' ' +
                                             wpair['k']['_st_'] + ' & ' +
                                             wpair['k']['_rf_'] + ' | ' +
                                             EQNAME + ' ID = ' +
                                             str(ind_eq+1) + ' | ' +
                                             'Exp. data ID = ' +
                                             str(ind_ed+1) + ' | ' +
                                             'MAE = ' +
                                             str(round(mape/len(p_raw)*100, 2)) + ' %')

                                ax.legend(loc='best', labelspacing=0.05, frameon=False)

                                # Tight layout
                                #
                                fig.align_ylabels()
                                fig.tight_layout()

                                # Save figure
                                #
                                tmp_path = os.path.join(PATH_WRAPPER,
                                                        'sorpproplib',
                                                        'data',
                                                        'verification',
                                                        EQNAME)
                                tmp_name = wpair['k']['_as_'].replace('/', '_') + '_' + \
                                           wpair['k']['_st_'].replace('/', '_') + '__' + \
                                           wpair['k']['_rf_'] + '__' + \
                                           EQNAME + '_' + \
                                           str(ind_eq+1) + '__' + \
                                           'ExpData_' + \
                                           str(ind_ed+1) + \
                                           '.png'

                                if not os.path.exists(tmp_path):
                                    os.mkdir(tmp_path)

                                fig.savefig(os.path.join(tmp_path, tmp_name))
                                plt.close()

            # Delete working pair class
            #
            del(wpair_class)


# %% Compare data.
#
compare_data(PATH_DB, 'Toth')
compare_data(PATH_DB, 'DualSiteSips')
compare_data(PATH_DB, 'Langmuir')
compare_data(PATH_DB, 'Freundlich')
compare_data(PATH_DB, 'DubininArctan1')
compare_data(PATH_DB, 'DubininAstakhov')

compare_data(PATH_DB, 'Duehring')
compare_data(PATH_DB, 'Antoine')
compare_data(PATH_DB, 'MixingRule')
compare_data(PATH_DB, 'WilsonFixedDl')
compare_data(PATH_DB, 'Heil')
compare_data(PATH_DB, 'WangChao')
compare_data(PATH_DB, 'TsubokaKatayama')
compare_data(PATH_DB, 'WilsonTemperatureDl')
compare_data(PATH_DB, 'NrtlFixedDg')
compare_data(PATH_DB, 'UniquacFixedDu')
compare_data(PATH_DB, 'NrtlTemperatureDg')
compare_data(PATH_DB, 'UniquacTemperatureDu')
compare_data(PATH_DB, 'FloryHuggins')
