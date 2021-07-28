"""Package 'utilities' cotains functions used throughout the package 'doc'.

Routine Listings
----------------
str_latex:
    Manipulates string in a way that it is usable by LaTeX, UTF-8 encoding.
str_file:
    Manipulates string in a way it can be saved as a seperate file and included by LaTeX manual.
fuct_w_pT___ads_isotherm:
    Define function w(p,T) of adsorption working pairs to account for exceptions of refrigerant functions if necessary.
fuct_p_wT___abs_isosteres:
    Define function p(w,T) of absorption working pairs to account for exceptions of refrigerant functions if necessary.
fuct_w_pT___abs_isotherms:
    Define function w(p,T) of absorption working pairs to account for exceptions of refrigerant functions if necessary.

Revisions
---------
    Version     Date        Author              Changes
    1.0.0       2020-07-21  Mirko Engelpracht   First implementation
"""

# %% Import relevant modules.
#
import numpy as np

from CoolProp.CoolProp import PropsSI
from functools import partial as partial_func
from typing import Callable
from ..wpair.wpair_struct import WPairStruct


# %% Define functions.
#
def str_latex(str_in: str) -> str:
    r"""Manipulate string in a way that it is usable by LaTeX, UTF-8 encoding.

    Parameters
    ----------
    str_in : str
        String to manipulate.
        
    Returns
    -------
    str
        Manipulated string.

    """
    return str_in.replace("β", 'ss').replace('ı́', 'i').replace('_', '\_').replace('^', '\^')

def str_file(str_in: str) -> str:
    r"""Manipulate string in a way it can be saved as a seperate file and included by LaTeX manual.

    Parameters
    ----------
    str_in : str
        String to manipulate.
        
    Returns
    -------
    str
        Manipulated string.

    """
    return str_in.replace(" ", '_').replace('/', '_').replace('%', 'perct').replace('.', '').replace('Ü', 'UE')
        
def fuct_w_pT___ads_isotherm(WPAIR: dict, EQU_NAME: str, EQU_ID: int,
                             WPAIR_CLASS: WPairStruct) -> Callable[[float, float], float]:
    r"""Define function w(p,T) of adsorption working pairs to account for exceptions of refrigerant functions if necessary.

    Parameters
    ----------
    WPAIR : dict
        Dict containing data of current 
    EQU_NAME : str
        Name of isotherm function.
    EQU_ID : int
        ID of isotherm function.
    WPAIR_CLASS :
        Adsorption working pair object to perform calculations.
        
    Returns
    -------
    Callable[[float, float], float]
        Function calculating loading depending on pressure and temperature.

    """
    # Use nested check to get correct entry of SorpPropLib:
    # First, check for equation type
    # Second, check for literature
    # Second, check for refrigerant
    # Fourth, check for sorbent
    # Fifth, check for subtype
    #
    if EQU_NAME == 'DubininArctan1' or EQU_NAME == 'DubininAstakhov':
        # For volumetric approaches, special function for vapor pressure and / or saturated
        # liquid density may be used
        #
        if WPAIR['v']['_ep_'][EQU_NAME][EQU_ID]['_c_'] == 'Rahman, Kazi Afzalur; Chakraborty, Anutosh; Saha, Bidyut Baran; Ng, Kim Choon (2012): On thermodynamics of methane+carbonaceous materials adsorption. In: International Journal of Heat and Mass Transfer 55 (4), S. 565–573. DOI: 10.1016/j.ijheatmasstransfer.2011.10.056.':
            if WPAIR['k']['_rf_'] == 'Methane':
                if WPAIR['k']['_as_'] == 'activated carbon powder':
                    if WPAIR['k']['_st_'] == 'Maxsorb III':
                        def func_w_pT_fix(p, T, WPAIR_CLASS):
                            p_sat = 4599200 * (T / 190.564) ** 2 if T > 188 else \
                                WPAIR_CLASS.Ref.p_sat_T(T)
                            rho_ad = 1 / (2.3677e-3 * np.exp(0.0043 * (T - 111.67)))
                            return WPAIR_CLASS.Ads.Vol.w_pTpsatrho(p, T, p_sat, rho_ad)

                        return partial_func(func_w_pT_fix, WPAIR_CLASS=WPAIR_CLASS)


        elif WPAIR['v']['_ep_'][EQU_NAME][EQU_ID]['_c_'] == 'Askalany, Ahmed A.; Saha, Bidyut B.; Uddin, Kutub; Miyzaki, Takahiko; Koyama, Shigeru; Srinivasan, Kandadai; Ismail, Ibrahim M. (2013): Adsorption Isotherms and Heat of Adsorption of Difluoromethane on Activated Carbons. In: J. Chem. Eng. Data 58 (10), S. 2828–2834. DOI: 10.1021/je4005678.':
            if WPAIR['k']['_rf_'] == 'R-32':
                if WPAIR['k']['_as_'] == 'activated carbon powder':
                    if WPAIR['k']['_st_'] == 'Maxsorb III':
                        def func_w_pT_fix(p, T, WPAIR_CLASS):
                            rho_g = PropsSI('D', 'T', T, 'P', p, 'R32')
                            rho_ad = 1.546*T - rho_g
                            return WPAIR_CLASS.Ads.Vol.w_pTpsatrho(p, T, WPAIR_CLASS.Ref.p_sat_T(T), rho_ad)

                    return partial_func(func_w_pT_fix, WPAIR_CLASS=WPAIR_CLASS)

                elif WPAIR['k']['_as_'] == 'activated carbon fiber':
                    if WPAIR['k']['_st_'] == 'A-20':
                        def func_w_pT_fix(p, T, WPAIR_CLASS):
                            rho_g = PropsSI('D', 'T', T, 'P', p, 'R32')
                            rho_ad = 1.022*T - rho_g
                            return WPAIR_CLASS.Ads.Vol.w_pTpsatrho(p, T, WPAIR_CLASS.Ref.p_sat_T(T), rho_ad)

                    return partial_func(func_w_pT_fix, WPAIR_CLASS=WPAIR_CLASS)


        elif WPAIR['v']['_ep_'][EQU_NAME][EQU_ID]['_c_'] == 'Akkimaradi, Basavaraj S.; Prasad, Madhu; Dutta, Pradip; Srinivasan, Kandadai (2001): Adsorption of 1,1,1,2-Tetrafluoroethane on Activated Charcoal. In: J. Chem. Eng. Data 46 (2), S. 417–422. DOI: 10.1021/je000277e.':
            if WPAIR['k']['_rf_'] == 'R-134a':
                def func_w_pT_fix(p, T, WPAIR_CLASS):
                    omega = np.log(9.39e-4 / 7.2643e-4) / (374.21 - 246.78)
                    rho_ad = 1 / (7.2643e-4 * np.exp(omega * (T - 246.78)))
                    return WPAIR_CLASS.Ads.Vol.w_pTpsatrho(p, T, WPAIR_CLASS.Ref.p_sat_T(T), rho_ad)

                return partial_func(func_w_pT_fix, WPAIR_CLASS=WPAIR_CLASS)


        elif WPAIR['v']['_ep_'][EQU_NAME][EQU_ID]['_c_'] == 'Saha, Bidyut B.; Habib, Khairul; El-Sharkawy, Ibrahim I.; Koyama, Shigeru (2009): Adsorption characteristics and heat of adsorption measurements of R-134a on activated carbon. In: International Journal of Refrigeration 32 (7), S. 1563–1569. DOI: 10.1016/j.ijrefrig.2009.03.010.':
            if WPAIR['k']['_rf_'] == 'R-134a':
                def func_w_pT_fix(p, T, WPAIR_CLASS):
                    omega = np.log(9.39e-4 / 7.26e-4) / (374.21 - 246.78)
                    rho_ad = 1 / (7.26e-4 * np.exp(omega * (T - 246.78)))
                    return WPAIR_CLASS.Ads.Vol.w_pTpsatrho(p, T, WPAIR_CLASS.Ref.p_sat_T(T), rho_ad)

                return partial_func(func_w_pT_fix, WPAIR_CLASS=WPAIR_CLASS)


        elif WPAIR['v']['_ep_'][EQU_NAME][EQU_ID]['_c_'] == 'Saha, Bidyut Baran; El-Sharkawy, Ibrahim I.; Thorpe, Roger; Critoph, Robert E. (2012): Accurate adsorption isotherms of R134a onto activated carbons for cooling and freezing applications. In: International Journal of Refrigeration 35 (3), S. 499–505. DOI: 10.1016/j.ijrefrig.2011.05.002.':
            if WPAIR['k']['_rf_'] == 'R-134a':
                def func_w_pT_fix(p, T, WPAIR_CLASS):
                    omega = np.log(9.39e-4 / 7.2643e-4) / (374.21 - 246.78)
                    rho_ad = 1 / (7.2643e-4 * np.exp(omega * (T - 246.78)))
                    return WPAIR_CLASS.Ads.Vol.w_pTpsatrho(p, T, WPAIR_CLASS.Ref.p_sat_T(T), rho_ad)

                return partial_func(func_w_pT_fix, WPAIR_CLASS=WPAIR_CLASS)


        elif WPAIR['v']['_ep_'][EQU_NAME][EQU_ID]['_c_'] == 'Askalany, Ahmed A.; Saha, Bidyut B.; Ismail, Ibrahim M. (2014): Adsorption isotherms and kinetics of HFC410A onto activated carbons. In: Applied Thermal Engineering 72 (2), S. 237–243. DOI: 10.1016/j.applthermaleng.2014.04.075.':
            if WPAIR['k']['_rf_'] == 'R-407c':
                def func_w_pT_fix(p, T, WPAIR_CLASS):
                    rho_g = PropsSI('D', 'T', T, 'P', p, 'R407c')
                    rho_ad = 1.0015*T - rho_g
                    return WPAIR_CLASS.Ads.Vol.w_pTpsatrho(p, T, WPAIR_CLASS.Ref.p_sat_T(T), rho_ad)

                return partial_func(func_w_pT_fix, WPAIR_CLASS=WPAIR_CLASS)


#        elif WPAIR['_c_'] == 'El-sharkawy, M. M.; Askalany, A. A.; Harby, K.; Ahmed, M. S. (2016): Adsorption isotherms and kinetics of a mixture of Pentafluoroethane, 1,1,1,2-Tetrafluoroethane and Difluoromethane (HFC-407C) onto granular activated carbon. In: Applied Thermal Engineering 93, S. 988–994. DOI: 10.1016/j.applthermaleng.2015.10.077.':
#            if WPAIR['k']['_rf_'] == 'R-404a':
#                def func_w_pT_fix(p, T, WPAIR_CLASS):
#                    rho_g = PropsSI('D', 'T', T, 'P', p, 'R404a')
#                    rho_ad = A - B*T - rho_g
#                    return WPAIR_CLASS.Ads.Vol.w_pTpsatrho(p, T, WPAIR_CLASS.Ref.p_sat_T(T), rho_ad)
#
#                return partial_func(func_w_pT_fix, WPAIR_CLASS=WPAIR_CLASS)


        elif WPAIR['v']['_ep_'][EQU_NAME][EQU_ID]['_c_'] == 'Saha, Bidyut B.; El-Sharkawy, Ibrahim I.; Habib, Khairul; Koyama, Shigeru; Srinivasan, Kandadai (2008): Adsorption of Equal Mass Fraction Near an Azeotropic Mixture of Pentafluoroethane and 1,1,1-Trifluoroethane on Activated Carbon. In: J. Chem. Eng. Data 53 (8), S. 1872–1876. DOI: 10.1021/je800204p.':
            if WPAIR['k']['_rf_'] == '507a':
                def func_w_pT_fix(p, T, WPAIR_CLASS):
                    return WPAIR_CLASS.Ads.Vol.w_pTpsatrho(p, T, WPAIR_CLASS.Ref.p_sat_T(T), -1)

                return partial_func(func_w_pT_fix, WPAIR_CLASS=WPAIR_CLASS)


        elif WPAIR['v']['_ep_'][EQU_NAME][EQU_ID]['_c_'] == 'Loh, Wai Soong; Ismail, Azhar Bin; Xi, Baojuan; Ng, Kim Choon; Chun, Won Gee (2012): Adsorption Isotherms and Isosteric Enthalpy of Adsorption for Assorted Refrigerants on Activated Carbons. In: J. Chem. Eng. Data 57 (10), S. 2766–2773. DOI: 10.1021/je3008099.':
            if WPAIR['k']['_rf_'] == '507a':
                def func_w_pT_fix(p, T, WPAIR_CLASS):
                    return WPAIR_CLASS.Ads.Vol.w_pTpsatrho(p, T, WPAIR_CLASS.Ref.p_sat_T(T), -1)

                return partial_func(func_w_pT_fix, WPAIR_CLASS=WPAIR_CLASS)

    # Return standard approach
    #
    return WPAIR_CLASS.Ads.w_pT

def fuct_p_wT___abs_isosteres(WPAIR: dict, EQU_NAME: str, EQU_ID: int,
                              WPAIR_CLASS: WPairStruct) -> Callable[[float, float], float]:
    r"""Define function p(w,T) of absorption working pairs to account for exceptions of refrigerant functions if necessary.

    Parameters
    ----------
    WPAIR : dict
        Dict containing data of current 
    EQU_NAME : str
        Name of isotherm function.
    EQU_ID : int
        ID of isotherm function.
    WPAIR_CLASS :
        Adsorption working pair object to perform calculations.
        
    Returns
    -------
    Callable[[float, float], float]
        Function calculating loading depending on pressure and temperature.

    """
    # Use nested check to get correct entry of SorpPropLib:
    # First, check for equation type
    # Second, check for literature
    # Second, check for refrigerant
    # Fourth, check for sorbent
    # Fifth, check for subtype
    #
    if EQU_NAME == 'Antoine' or EQU_NAME == 'Duehring':
        # Pass correct low-level function
        #
        return WPAIR_CLASS.Abs.Con.p_XT

    elif EQU_NAME == 'MixingRule':
        # Pass correct low-level function and get correct element from tuple
        #
        def func_p_wT_correct(w, T, WPAIR_CLASS):
                return WPAIR_CLASS.Abs.Mix.p_Tx(T, w)[0]

        return partial_func(func_p_wT_correct, WPAIR_CLASS=WPAIR_CLASS)

    else:
        # Pass correct low-level function. Either use molar volumes as stored in data base or
        # use molar volumes given as inputs
        if WPAIR['v']['_ep_'][EQU_NAME][EQU_ID]['_c_'] == 'Burton, C.; Jacobi, A. M.; Mehendale, S. S. (1999): Vapor-liquid equilibrium for R-32 and R-410A mixed with a polyol ester. Non-ideality and local composition modeling. In: International Journal of Refrigeration 22 (6), S. 458–471. DOI: 10.1016/S0140-7007(99)00012-2.':
            if WPAIR['k']['_rf_'] == 'R-32':
                if WPAIR['k']['_as_'] == 'lubricant':
                    if WPAIR['k']['_st_'] == 'POE':
                        def func_x_pT_correct(w, T, WPAIR_CLASS):
                                return WPAIR_CLASS.Abs.Act.p_Txv1v2(T, w,
                                                                    1/PropsSI('D', 'T', T, 'Q', 0, 'R32')*0.052024,
                                                                    1/(993.89-0.75658*(T-273.15))*0.7)
                        return partial_func(func_x_pT_correct, WPAIR_CLASS=WPAIR_CLASS)

            elif WPAIR['k']['_rf_'] == 'R-125':
                if WPAIR['k']['_as_'] == 'lubricant':
                    if WPAIR['k']['_st_'] == 'POE':
                        def func_x_pT_correct(w, T, WPAIR_CLASS):
                                return WPAIR_CLASS.Abs.Act.p_Txv1v2(T, w,
                                                                    1/PropsSI('D', 'T', T, 'Q', 0, 'R125')*0.12002,
                                                                    1/(993.89-0.75658*(T-273.15))*0.7)
                        return partial_func(func_x_pT_correct, WPAIR_CLASS=WPAIR_CLASS)


        elif WPAIR['v']['_ep_'][EQU_NAME][EQU_ID]['_c_'] == 'Bock, Jessica (2015): VAPOR-LIQUID EQUILIBRIA OF A LOW GWP REFRIGERANT, R-1234ZE(E), MIXED WITH A POE LUBRICANT. PhD Thesis. University of Illinois at Urbana-Champaign, Illinois.':
            if WPAIR['k']['_rf_'] == 'R-1234ze(E)':
                if WPAIR['k']['_as_'] == 'lubricant':
                    if WPAIR['k']['_st_'] == 'POE':
                        def func_x_pT_correct(w, T, WPAIR_CLASS):
                                return WPAIR_CLASS.Abs.Act.p_Txv1v2psat(T, w,
                                                                        1/PropsSI('D', 'T', T, 'Q', 0, 'R1234ze(E)')*0.1140415928,
                                                                        1/(993.89-0.75658*(T-273.15))*0.7,
                                                                        PropsSI('P', 'T', T, 'Q', 0, 'R1234ze(E)'))
                        return partial_func(func_x_pT_correct, WPAIR_CLASS=WPAIR_CLASS)


        elif WPAIR['v']['_ep_'][EQU_NAME][EQU_ID]['_c_'] == 'Grebner, J. J. (1992): The Effects of Oil on the Thermodynamic Properties of Dichlorodifluoromethane (R-12) and Tetrafluoroethane (R-134a). Hg. v. Air Conditioning and Refrigeration Center. College of Engineering. University of Illinois at Urbana-Champaign. Air Conditioning and Refrigeration Center TR-13. Online verfügbar unter http://hdl.handle.net/2142/9702.':
            if WPAIR['k']['_rf_'] == 'R-12':
                if WPAIR['k']['_as_'] == 'naphthenic':
                    if WPAIR['k']['_st_'] == '':
                        def func_x_pT_correct(w, T, WPAIR_CLASS):
                                return WPAIR_CLASS.Abs.Act.p_Txv1v2(T, w,
                                                                    1/PropsSI('D', 'T', T, 'Q', 0, 'R12')*0.120913,
                                                                    1/((56.211-0.020808*((T-273.15)*1.8+32))*16.0185)*0.325)
                        return partial_func(func_x_pT_correct, WPAIR_CLASS=WPAIR_CLASS)

                elif WPAIR['k']['_as_'] == 'paraffinic':
                    if WPAIR['k']['_st_'] == '':
                        def func_x_pT_correct(w, T, WPAIR_CLASS):
                                return WPAIR_CLASS.Abs.Act.p_Txv1v2(T, w,
                                                                    1/PropsSI('D', 'T', T, 'Q', 0, 'R12')*0.120913,
                                                                    1/((55.37-0.020428*((T-273.15)*1.8+32))*16.0185)*0.5)
                        return partial_func(func_x_pT_correct, WPAIR_CLASS=WPAIR_CLASS)

            if WPAIR['k']['_rf_'] == 'R-134a':
                if WPAIR['k']['_as_'] == 'lubricant':
                    if WPAIR['k']['_st_'] == 'POE':
                        def func_x_pT_correct(w, T, WPAIR_CLASS):
                                return WPAIR_CLASS.Abs.Act.p_Txv1v2(T, w,
                                                                    1/PropsSI('D', 'T', T, 'Q', 0, 'R134a')*0.102032,
                                                                    1/((61.387-0.024138*((T-273.15)*1.8+32))*16.0185)*0.59)
                        return partial_func(func_x_pT_correct, WPAIR_CLASS=WPAIR_CLASS)


        elif WPAIR['v']['_ep_'][EQU_NAME][EQU_ID]['_c_'] == 'Martz, W. L.; Jacobi, A. M. (1994): Refrigerant-Oil Mixtures and Local Composition Modeling. Hg. v. Air Conditioning and Refrigeration Center. College of Engineering. University of Illinois at Urbana-Champaign. Air Conditioning and Refrigeration Center TR-68. Online verfügbar unter http://hdl.handle.net/2142/10986.':
            if WPAIR['k']['_rf_'] == 'R-22':
                if WPAIR['k']['_as_'] == 'lubricant':
                    if WPAIR['k']['_st_'] == 'POE':
                        def func_x_pT_correct(w, T, WPAIR_CLASS):
                                return WPAIR_CLASS.Abs.Act.p_Txv1v2(T, w,
                                                                    1/PropsSI('D', 'T', T, 'Q', 0, 'R12')*0.086468,
                                                                    1/((61.387-0.024138*((T-273.15)*1.8+32))*16.0185)*0.59)
                        return partial_func(func_x_pT_correct, WPAIR_CLASS=WPAIR_CLASS)

            if WPAIR['k']['_rf_'] == 'R-134a':
                if WPAIR['k']['_as_'] == 'lubricant':
                    if WPAIR['k']['_st_'] == 'POE':
                        def func_x_pT_correct(w, T, WPAIR_CLASS):
                                return WPAIR_CLASS.Abs.Act.p_Txv1v2(T, w,
                                                                    1/PropsSI('D', 'T', T, 'Q', 0, 'R134a')*0.102032,
                                                                    1/((61.387-0.024138*((T-273.15)*1.8+32))*16.0185)*0.59)
                        return partial_func(func_x_pT_correct, WPAIR_CLASS=WPAIR_CLASS)

                    elif WPAIR['k']['_st_'] == 'PAG':
                        def func_x_pT_correct(w, T, WPAIR_CLASS):
                                return WPAIR_CLASS.Abs.Act.p_Txv1v2(T, w,
                                                                    1/PropsSI('D', 'T', T, 'Q', 0, 'R134a')*0.102032,
                                                                    1/((62.365-0.024864*((T-273.15)*1.8+32))*16.0185)*2)
                        return partial_func(func_x_pT_correct, WPAIR_CLASS=WPAIR_CLASS)


        def func_p_wT_correct(w, T, WPAIR_CLASS):
                return WPAIR_CLASS.Abs.Act.p_Txv1v2(T, w, -1, -1)

        return partial_func(func_p_wT_correct, WPAIR_CLASS=WPAIR_CLASS)

def fuct_w_pT___abs_isotherms(WPAIR: dict, EQU_NAME: str, EQU_ID: int,
                              WPAIR_CLASS: WPairStruct) -> Callable[[float, float], float]:
    r"""Define function w(p,T) of absorption working pairs to account for exceptions of refrigerant functions if necessary.

    Parameters
    ----------
    WPAIR : dict
        Dict containing data of current 
    EQU_NAME : str
        Name of isotherm function.
    EQU_ID : int
        ID of isotherm function.
    WPAIR_CLASS :
        Adsorption working pair object to perform calculations.
        
    Returns
    -------
    Callable[[float, float], float]
        Function calculating loading depending on pressure and temperature.

    """
    # Use nested check to get correct entry of SorpPropLib:
    # First, check for equation type
    # Second, check for literature
    # Second, check for refrigerant
    # Fourth, check for sorbent
    # Fifth, check for subtype
    #
    if EQU_NAME == 'Antoine' or EQU_NAME == 'Duehring':
        # Pass correct low-level function
        #
        return WPAIR_CLASS.Abs.Con.X_pT

    elif EQU_NAME == 'MixingRule':
        # Pass correct low-level function and get correct element from tuple
        #
        def func_x_pT_correct(p, T, WPAIR_CLASS):
                return WPAIR_CLASS.Abs.Mix.x_pT(p, T)[0]

        return partial_func(func_x_pT_correct, WPAIR_CLASS=WPAIR_CLASS)

    else:
        # Pass correct low-level function. Either use molar volumes as stored in data base or
        # use molar volumes given as inputs
        if WPAIR['v']['_ep_'][EQU_NAME][EQU_ID]['_c_'] == 'Marchi, Paolo; Scalabrin, Giancarlo; Ihmels, E. Christian; Fischer, Kai; Gmehling, Jürgen (2006): Bubble pressure measurements for the (1,1,1,2-tetrafluoroethane+triethylene glycol dimethyl ether) system. In: The Journal of Chemical Thermodynamics 38 (11), S. 1247–1253. DOI: 10.1016/j.jct.2006.03.004.':
            if WPAIR['k']['_rf_'] == 'R-134':
                if WPAIR['k']['_as_'] == 'TriEGDME':
                    if WPAIR['k']['_st_'] == '':
                        def func_x_pT_correct(p, T, WPAIR_CLASS):
                                return WPAIR_CLASS.Abs.x_pTv1v2(p, T,
                                                                1/PropsSI('D', 'T', T, 'Q', 0, 'R134')*0.102032,
                                                                1)
                        return partial_func(func_x_pT_correct, WPAIR_CLASS=WPAIR_CLASS)


        def func_x_pT_correct(p, T, WPAIR_CLASS):
                return WPAIR_CLASS.Abs.Act.x_pTv1v2(p, T, -1, -1)
        return partial_func(func_x_pT_correct, WPAIR_CLASS=WPAIR_CLASS)
