"""Script 'example_calling_DLL'.

This script is an example script to demonstrate all capabilities of the module 'wpair'. First,
some standard functions are defined that execute different working pair functions implemented in
the SorpPropLob. Second, different working pairs are defined. Third, the functions defined in
step 1 are exectued for some random inputs.

Revisions:
----------
    Version     Date        Author              Changes
    1.0.0       2020-05-22  Mirko Engelpracht   First implementation
"""

# %% Import relevant modules.
#
import matplotlib.pyplot as plt
import os
import struct
import sys

from sorpproplib.wpair.wpair_struct import WPairStruct


# %% Definition of functions to test working pairs.
#
def test_adsorption_sur(p_input, T_input, M, WPAIR):
    r"""Execute functions defined for adsorption isotherms based on the surface approach."""
    # Print working pair
    #
    WPAIR.show_wpair(True)

    # Calculate some values using functions that are defined for all isotherms
    #
    w = [WPAIR.Ads.w_pT(val, T_input) for _, val in enumerate(p_input)]
    p = [WPAIR.Ads.p_wT(val, T_input) for _, val in enumerate(w)]
    T = [WPAIR.Ads.T_pw(p[ind], val) for ind, val in enumerate(w)]

    dw_dp = [WPAIR.Ads.dw_dp_pT(val, T_input) for _, val in enumerate(p_input)]
    dw_dT = [WPAIR.Ads.dw_dT_pT(val, T_input) for _, val in enumerate(p_input)]
    dp_dw = [WPAIR.Ads.dp_dw_wT(val, T_input) for _, val in enumerate(w)]
    dp_dT = [WPAIR.Ads.dp_dT_wT(val, T_input) for _, val in enumerate(w)]

    piStar = [WPAIR.Ads.piStar_pyxgTM(val, 1, 1, 1, T_input, M) for _, val in enumerate(p_input)]

    # Calculate some values using functions that are defined for all isotherms
    #
    w_specific = [WPAIR.Ads.Sur.w_pT(val, T_input) for _, val in enumerate(p_input)]
    p_specific = [WPAIR.Ads.Sur.p_wT(val, T_input) for _, val in enumerate(w_specific)]
    T_specific = [WPAIR.Ads.Sur.T_pw(p_specific[ind], val) for ind, val in enumerate(w_specific)]

    dw_dp_specific = [WPAIR.Ads.Sur.dw_dp_pT(val, T_input) for _, val in enumerate(p_input)]
    dw_dT_specific = [WPAIR.Ads.Sur.dw_dT_pT(val, T_input) for _, val in enumerate(p_input)]
    dp_dw_specific = [WPAIR.Ads.Sur.dp_dw_wT(val, T_input) for _, val in enumerate(w_specific)]
    dp_dT_specific = [WPAIR.Ads.Sur.dp_dT_wT(val, T_input) for _, val in enumerate(w_specific)]

    piStar_specific = [WPAIR.Ads.Sur.piStar_pyxgTM(val, 1, 1, 1, T_input, M) for _, val in enumerate(p_input)]

    # Plot calculated values
    #
    fig, ax = plt.subplots(4, 2, sharex=True, figsize=(30.5/2.54, 40/2.54))

    ax[0,0].plot(p, w, label='General')
    ax[0,1].plot(p, p, label='General')

    ax[1,0].plot(p, T, label='General')
    ax[1,1].plot(p, piStar, label='General')

    ax[2,0].plot(p, dw_dp, label='General')
    ax[2,1].plot(p, dw_dT, label='General')

    ax[3,0].plot(p, dp_dw, label='General')
    ax[3,1].plot(p, dp_dT, label='General')

    ax[0,0].plot(p, w_specific, label='Sur')
    ax[0,1].plot(p, p_specific, label='Sur')

    ax[1,0].plot(p, T_specific, label='Sur')
    ax[1,1].plot(p, piStar_specific, label='Sur')

    ax[2,0].plot(p, dw_dp_specific, label='Sur')
    ax[2,1].plot(p, dw_dT_specific, label='Sur')

    ax[3,0].plot(p, dp_dw_specific, label='Sur')
    ax[3,1].plot(p, dp_dT_specific, label='Sur')

    # Set axe, legends, and font
    #
    ax[3,0].set_xlabel('Pressure p in Pa')
    ax[3,1].set_xlabel('Pressure p in Pa')

    ax[0,0].set_ylabel('Loading in kg_refrigerant/kg_sorbent')
    ax[0,1].set_ylabel('Pressure in Pa')
    ax[1,0].set_ylabel('Temperature in K')
    ax[1,1].set_ylabel('Reduced spreading pressure in kg/mol')
    ax[2,0].set_ylabel('Der. of loading wrt. pressure in kg/kg/Pa')
    ax[2,1].set_ylabel('Der. of loading wrt. temperature in kg/kg/K')
    ax[3,0].set_ylabel('Der. of pressure wrt. loading in Pa.kg/kg')
    ax[3,1].set_ylabel('Der. of pressure wrt. temperature in Pa/K')


    ax[0,0].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[0,1].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[1,0].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[1,1].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[2,0].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[2,1].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[3,0].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[3,1].legend(loc='best', labelspacing=0.05, frameon=False)

    # Tight layout
    #
    fig.align_ylabels()
    fig.tight_layout()

def test_adsorption_surref(p_input, T_input, M, WPAIR):
    r"""Execute functions defined for adsorption isotherms based on the surface approach and
        using the vapor pessure as additional input."""
    # Print working pair
    #
    WPAIR.show_wpair(True)

    # Calculate some values using functions that are defined for all isotherms
    #
    w = [WPAIR.Ads.w_pT(val, T_input) for _, val in enumerate(p_input)]
    p = [WPAIR.Ads.p_wT(val, T_input) for _, val in enumerate(w)]
    T = [WPAIR.Ads.T_pw(p[ind], val) for ind, val in enumerate(w)]

    dw_dp = [WPAIR.Ads.dw_dp_pT(val, T_input) for _, val in enumerate(p_input)]
    dw_dT = [WPAIR.Ads.dw_dT_pT(val, T_input) for _, val in enumerate(p_input)]
    dp_dw = [WPAIR.Ads.dp_dw_wT(val, T_input) for _, val in enumerate(w)]
    dp_dT = [WPAIR.Ads.dp_dT_wT(val, T_input) for _, val in enumerate(w)]

    piStar = [WPAIR.Ads.piStar_pyxgTM(val, 1, 1, 1, T_input, M) for _, val in enumerate(p_input)]

    # Calculate some values using functions that are defined for all isotherms
    #
    w_specific = [WPAIR.Ads.SurRef.w_pTpsat(val, T_input, WPAIR.Ref.p_sat_T(T_input)) for _, val \
                  in enumerate(p_input)]
    p_specific = [WPAIR.Ads.SurRef.p_wTpsat(val, T_input, WPAIR.Ref.p_sat_T(T_input)) for _, val \
                  in enumerate(w_specific)]
    T_specific = [WPAIR.Ads.SurRef.T_pwpsat(p_specific[ind], val,
                                            WPAIR.Ref.p_sat_T, WPAIR.Ref.dp_sat_dT_T) for ind, val \
                  in enumerate(w_specific)]

    dw_dp_specific = [WPAIR.Ads.SurRef.dw_dp_pTpsat(val, T_input, WPAIR.Ref.p_sat_T(T_input)) \
                      for _, val in enumerate(p_input)]
    dw_dT_specific = [WPAIR.Ads.SurRef.dw_dT_pTpsat(val, T_input, WPAIR.Ref.p_sat_T(T_input),
                                                    WPAIR.Ref.dp_sat_dT_T(T_input)) for _, val \
                      in enumerate(p_input)]
    dp_dw_specific = [WPAIR.Ads.SurRef.dp_dw_wTpsat(val, T_input, WPAIR.Ref.p_sat_T(T_input)) for _, val \
                      in enumerate(w_specific)]
    dp_dT_specific = [WPAIR.Ads.SurRef.dp_dT_wTpsat(val, T_input, WPAIR.Ref.p_sat_T(T_input),
                                                    WPAIR.Ref.dp_sat_dT_T(T_input)) for _, val \
                      in enumerate(w_specific)]

    piStar_specific = [WPAIR.Ads.SurRef.piStar_pyxgTpsatM(val, 1, 1, 1, T_input,
                                                          WPAIR.Ref.p_sat_T(T_input), M) for _, val \
                       in enumerate(p_input)]

    # Plot calculated values
    #
    fig, ax = plt.subplots(4, 2, sharex=True, figsize=(30.5/2.54, 40/2.54))

    ax[0,0].plot(p, w, label='General')
    ax[0,1].plot(p, p, label='General')

    ax[1,0].plot(p, T, label='General')
    ax[1,1].plot(p, piStar, label='General')

    ax[2,0].plot(p, dw_dp, label='General')
    ax[2,1].plot(p, dw_dT, label='General')

    ax[3,0].plot(p, dp_dw, label='General')
    ax[3,1].plot(p, dp_dT, label='General')

    ax[0,0].plot(p, w_specific, label='SurRef')
    ax[0,1].plot(p, p_specific, label='SurRef')

    ax[1,0].plot(p, T_specific, label='SurRef')
    ax[1,1].plot(p, piStar_specific, label='SurRef')

    ax[2,0].plot(p, dw_dp_specific, label='SurRef')
    ax[2,1].plot(p, dw_dT_specific, label='SurRef')

    ax[3,0].plot(p, dp_dw_specific, label='SurRef')
    ax[3,1].plot(p, dp_dT_specific, label='SurRef')

    # Set axe, legends, and font
    #
    ax[3,0].set_xlabel('Pressure p in Pa')
    ax[3,1].set_xlabel('Pressure p in Pa')

    ax[0,0].set_ylabel('Loading in kg_refrigerant/kg_sorbent')
    ax[0,1].set_ylabel('Pressure in Pa')
    ax[1,0].set_ylabel('Temperature in K')
    ax[1,1].set_ylabel('Reduced spreading pressure in kg/mol')
    ax[2,0].set_ylabel('Der. of loading wrt. pressure in kg/kg/Pa')
    ax[2,1].set_ylabel('Der. of loading wrt. temperature in kg/kg/K')
    ax[3,0].set_ylabel('Der. of pressure wrt. loading in Pa.kg/kg')
    ax[3,1].set_ylabel('Der. of pressure wrt. temperature in Pa/K')


    ax[0,0].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[0,1].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[1,0].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[1,1].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[2,0].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[2,1].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[3,0].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[3,1].legend(loc='best', labelspacing=0.05, frameon=False)

    # Tight layout
    #
    fig.align_ylabels()
    fig.tight_layout()

def test_adsorption_vol(p_input, T_input, M, WPAIR):
    r"""Execute functions defined for adsorption isotherms based on the volumetric approach."""
    # Print working pair
    #
    WPAIR.show_wpair(True)

    # Calculate some values using functions that are defined for all isotherms
    #
    w = [WPAIR.Ads.w_pT(val, T_input) for _, val in enumerate(p_input)]
    p = [WPAIR.Ads.p_wT(val, T_input) for _, val in enumerate(w)]
    T = [WPAIR.Ads.T_pw(p[ind], val) for ind, val in enumerate(w)]

    dw_dp = [WPAIR.Ads.dw_dp_pT(val, T_input) for _, val in enumerate(p_input)]
    dw_dT = [WPAIR.Ads.dw_dT_pT(val, T_input) for _, val in enumerate(p_input)]
    dp_dw = [WPAIR.Ads.dp_dw_wT(val, T_input) for _, val in enumerate(w)]
    dp_dT = [WPAIR.Ads.dp_dT_wT(val, T_input) for _, val in enumerate(w)]

    piStar = [WPAIR.Ads.piStar_pyxgTM(val, 1, 1, 1, T_input, M) for _, val in enumerate(p_input)]

    # Calculate some values using functions that are defined for all isotherms
    #
    w_specific = [WPAIR.Ads.Vol.w_pTpsatrho(val, T_input, WPAIR.Ref.p_sat_T(T_input),
                                            WPAIR.Ref.rho_sat_l_T(T_input)) for _, val in \
                  enumerate(p_input)]
    p_specific = [WPAIR.Ads.Vol.p_wTpsatrho(val, T_input, WPAIR.Ref.p_sat_T(T_input),
                                            WPAIR.Ref.rho_sat_l_T(T_input)) for _, val in \
                  enumerate(w_specific)]
    T_specific = [WPAIR.Ads.Vol.T_pwpsatrho(p_specific[ind], val, WPAIR.Ref.p_sat_T,
                                            WPAIR.Ref.dp_sat_dT_T, WPAIR.Ref.rho_sat_l_T,
                                            WPAIR.Ref.drho_sat_l_dT_T) for ind, val in \
                  enumerate(w_specific)]

    dw_dp_specific = [WPAIR.Ads.Vol.dw_dp_pTpsatrho(val, T_input, WPAIR.Ref.p_sat_T(T_input),
                                                    WPAIR.Ref.rho_sat_l_T(T_input)) for _, val in \
                      enumerate(p_input)]
    dw_dT_specific = [WPAIR.Ads.Vol.dw_dT_pTpsatrho(val, T_input, WPAIR.Ref.p_sat_T(T_input),
                                                    WPAIR.Ref.dp_sat_dT_T(T_input),
                                                    WPAIR.Ref.rho_sat_l_T(T_input),
                                                    WPAIR.Ref.drho_sat_l_dT_T(T_input)) for _, val \
                      in enumerate(p_input)]
    dp_dw_specific = [WPAIR.Ads.Vol.dp_dw_wTpsatrho(val, T_input, WPAIR.Ref.p_sat_T(T_input),
                                                    WPAIR.Ref.rho_sat_l_T(T_input)) for _, val in \
                      enumerate(w_specific)]
    dp_dT_specific = [WPAIR.Ads.Vol.dp_dT_wTpsatrho(val, T_input, WPAIR.Ref.p_sat_T(T_input),
                                                    WPAIR.Ref.dp_sat_dT_T(T_input),
                                                    WPAIR.Ref.rho_sat_l_T(T_input),
                                                    WPAIR.Ref.drho_sat_l_dT_T(T_input)) for _, val \
                      in enumerate(w_specific)]

    piStar_specific = [WPAIR.Ads.Vol.piStar_pyxgTpsatrhoM(val, 1, 1, 1, T_input,
                                                          WPAIR.Ref.p_sat_T(T_input),
                                                          WPAIR.Ref.rho_sat_l_T(T_input), M) for _, val \
                       in enumerate(p_input)]

    # Plot calculated values
    #
    fig, ax = plt.subplots(4, 2, sharex=True, figsize=(30.5/2.54, 40/2.54))

    ax[0,0].plot(p, w, label='General')
    ax[0,1].plot(p, p, label='General')

    ax[1,0].plot(p, T, label='General')
    ax[1,1].plot(p, piStar, label='General')

    ax[2,0].plot(p, dw_dp, label='General')
    ax[2,1].plot(p, dw_dT, label='General')

    ax[3,0].plot(p, dp_dw, label='General')
    ax[3,1].plot(p, dp_dT, label='General')

    ax[0,0].plot(p, w_specific, label='Vol')
    ax[0,1].plot(p, p_specific, label='Vol')

    ax[1,0].plot(p, T_specific, label='Vol')
    ax[1,1].plot(p, piStar_specific, label='Vol')

    ax[2,0].plot(p, dw_dp_specific, label='Vol')
    ax[2,1].plot(p, dw_dT_specific, label='Vol')

    ax[3,0].plot(p, dp_dw_specific, label='Vol')
    ax[3,1].plot(p, dp_dT_specific, label='Vol')

    # Set axe, legends, and font
    #
    ax[3,0].set_xlabel('Pressure p in Pa')
    ax[3,1].set_xlabel('Pressure p in Pa')

    ax[0,0].set_ylabel('Loading in kg_refrigerant/kg_sorbent')
    ax[0,1].set_ylabel('Pressure in Pa')
    ax[1,0].set_ylabel('Temperature in K')
    ax[1,1].set_ylabel('Reduced spreading pressure in kg/mol')
    ax[2,0].set_ylabel('Der. of loading wrt. pressure in kg/kg/Pa')
    ax[2,1].set_ylabel('Der. of loading wrt. temperature in kg/kg/K')
    ax[3,0].set_ylabel('Der. of pressure wrt. loading in Pa.kg/kg')
    ax[3,1].set_ylabel('Der. of pressure wrt. temperature in Pa/K')


    ax[0,0].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[0,1].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[1,0].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[1,1].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[2,0].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[2,1].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[3,0].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[3,1].legend(loc='best', labelspacing=0.05, frameon=False)

    # Tight layout
    #
    fig.align_ylabels()
    fig.tight_layout()

def test_absorption_con(p_input, T_input, WPAIR):
    r"""Execute functions defined for absorption isotherms based on classical approaches."""
    # Print working pair
    #
    WPAIR.show_wpair(True)

    # Calculate some values using functions that are defined for all isotherms
    #
    X = [WPAIR.Abs.Con.X_pT(val, T_input) for _, val in enumerate(p_input)]
    p = [WPAIR.Abs.Con.p_XT(val, T_input) for _, val in enumerate(X)]
    T = [WPAIR.Abs.Con.T_pX(p[ind], val) for ind, val in enumerate(X)]

    dX_dp = [WPAIR.Abs.Con.dX_dp_pT(val, T_input) for _, val in enumerate(p_input)]
    dX_dT = [WPAIR.Abs.Con.dX_dT_pT(val, T_input) for _, val in enumerate(p_input)]
    dp_dX = [WPAIR.Abs.Con.dp_dX_XT(val, T_input) for _, val in enumerate(X)]
    dp_dT = [WPAIR.Abs.Con.dp_dT_XT(val, T_input) for _, val in enumerate(X)]

    # Plot calculated values
    #
    fig, ax = plt.subplots(4, 2, sharex=True, figsize=(30.5/2.54, 40/2.54))

    ax[0,0].plot(p, X, label='General')
    ax[0,1].plot(p, p, label='General')

    ax[1,0].plot(p, T, label='General')

    ax[2,0].plot(p, dX_dp, label='General')
    ax[2,1].plot(p, dX_dT, label='General')

    ax[3,0].plot(p, dp_dX, label='General')
    ax[3,1].plot(p, dp_dT, label='General')

    # Set axe, legends, and font
    #
    ax[3,0].set_xlabel('Pressure p in Pa')
    ax[3,1].set_xlabel('Pressure p in Pa')

    ax[0,0].set_ylabel('Loading in kg_refrigerant/kg_sorbent')
    ax[0,1].set_ylabel('Pressure in Pa')
    ax[1,0].set_ylabel('Temperature in K')
    ax[2,0].set_ylabel('Der. of loading wrt. pressure in kg/kg/Pa')
    ax[2,1].set_ylabel('Der. of loading wrt. temperature in kg/kg/K')
    ax[3,0].set_ylabel('Der. of pressure wrt. loading in Pa.kg/kg')
    ax[3,1].set_ylabel('Der. of pressure wrt. temperature in Pa/K')


    ax[0,0].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[0,1].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[1,0].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[1,1].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[2,0].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[2,1].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[3,0].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[3,1].legend(loc='best', labelspacing=0.05, frameon=False)

    # Tight layout
    #
    fig.align_ylabels()
    fig.tight_layout()

def test_absorption_act(p_input, T_input, v1, v2, WPAIR):
    r"""Execute functions defined for absorption isotherms based on activity coefficients."""
    # Print working pair
    #
    WPAIR.show_wpair(True)

    # Calculate some values using functions that are defined for all isotherms
    #
    x = [WPAIR.Abs.Act.x_pTv1v2(val, T_input, v1, v2) for _, val in enumerate(p_input)]
    p = [WPAIR.Abs.Act.p_Txv1v2(T_input, val, v1, v2) for _, val in enumerate(x)]

    x_ref = [WPAIR.Abs.Act.x_pTv1v2psat(val, T_input, v1, v2, WPAIR.Ref.p_sat_T(T_input)) for _, val \
             in enumerate(p_input)]
    p_ref = [WPAIR.Abs.Act.p_Txv1v2psat(T_input, val, v1, v2, WPAIR.Ref.p_sat_T(T_input)) for _, val \
             in enumerate(x_ref)]

    g = [WPAIR.Abs.Act.g_Txv1v2(T_input, val, v1, v2) for _, val in enumerate(x)]

    # Plot calculated values
    #
    fig, ax = plt.subplots(3, 1, figsize=(30.5/2.54, 40/2.54))

    ax[0].plot(x, g, label='General')
    ax[1].plot(p, x, label='General')
    ax[1].plot(p, x_ref, label='Ref')

    ax[2].plot(p, p, label='General')
    ax[2].plot(p, p_ref, label='Ref')

    # Set axe, legends, and font
    #
    ax[0].set_xlabel('Mole fraction in liquid phase in mol/mol')
    ax[1].set_xlabel('Pressure p in Pa')
    ax[2].set_xlabel('Pressure p in Pa')

    ax[0].set_ylabel('Activity coefficient')
    ax[1].set_ylabel('Mole fraction in liquid phase in mol/mol')
    ax[2].set_ylabel('Pressure p in Pa')

    ax[0].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[1].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[2].legend(loc='best', labelspacing=0.05, frameon=False)

    # Tight layout
    #
    fig.align_ylabels()
    fig.tight_layout()

def test_absorption_mix(p_input, T_input, WPAIR):
    r"""Execute functions defined for absorption isotherms based on equations of state."""
    # Print working pair
    #
    WPAIR.show_wpair(True)

    # Calculate some values using functions that are defined for all isotherms
    #
    x = [WPAIR.Abs.Mix.x_pT(val, T_input)[0] for _, val in enumerate(p_input)]
    p = [WPAIR.Abs.Mix.p_Tx(T_input, val)[0] for _, val in enumerate(x)]
    T = [WPAIR.Abs.Mix.T_px(p[ind], val)[0] for ind, val in enumerate(x)]

    dp_dx = [WPAIR.Abs.Mix.dp_dx_Tx(T_input, val) for _, val in enumerate(x)]
    dp_dT = [WPAIR.Abs.Mix.dp_dT_Tx(T_input, val) for _, val in enumerate(x)]

    # Plot calculated values
    #
    fig, ax = plt.subplots(3, 2, sharex=True, figsize=(30.5/2.54, 40/2.54))

    ax[0,0].plot(p, x, label='General')
    ax[0,1].plot(p, p, label='General')

    ax[1,0].plot(p, T, label='General')

    ax[2,0].plot(p, dp_dx, label='General')
    ax[2,1].plot(p, dp_dT, label='General')

    # Set axe, legends, and font
    #
    ax[2,0].set_xlabel('Pressure p in Pa')
    ax[2,1].set_xlabel('Pressure p in Pa')

    ax[0,0].set_ylabel('Mole fraction in liquid phase in mol/mol')
    ax[0,1].set_ylabel('Pressure in Pa')
    ax[1,0].set_ylabel('Temperature in K')
    ax[2,0].set_ylabel('Der. of pressure wrt. mole fraction in Pa.mol/mol')
    ax[2,1].set_ylabel('Der. of pressure wrt. temperature in Pa/K')


    ax[0,0].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[0,1].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[1,0].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[1,1].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[2,0].legend(loc='best', labelspacing=0.05, frameon=False)
    ax[2,1].legend(loc='best', labelspacing=0.05, frameon=False)

    # Tight layout
    #
    fig.align_ylabels()
    fig.tight_layout()

# %% Setup.
#
# Set up paths depending on plattform and architecture
#
PATH_WRAPPER = os.path.join(os.getcwd().split("sorpproplib_JSON")[0],
                            'sorpproplib_JSON', 'python_wrapper')
PATH_DB = os.path.join(PATH_WRAPPER, 'sorpproplib',
                       'data', 'JSON', 'sorpproplib_minimalExample.json').encode()

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

PATH_SORPPROPLIB = os.path.join(PATH_WRAPPER,  'sorpproplib',
                                'data', REQUIRED_DLL, 'libsorpPropLib.dll')

# Define some working pairs
#
wpair = {"sorbent": "carbon", "subtype": "norit-rb1", "refrigerant": "carbonDioxide"}
wpair_approach = {"isotherm": "Langmuir", "id_isotherm": 1,
                  "vapor_pressure": "VaporPressure_EoS1", "id_vapor_pressure": 1,
                  "sat_liq_density": "SaturatedLiquidDensity_EoS1", "id_sat_liq_density": 1}
WPAIR_LAGNMUIR = WPairStruct(wpair, wpair_approach, PATH_SORPPROPLIB, PATH_DB)

wpair = {"sorbent": "zeolite", "subtype": "5a", "refrigerant": "water"}
wpair_approach = {"isotherm": "Toth", "id_isotherm": 1,
                  "vapor_pressure": "VaporPressure_EoS1", "id_vapor_pressure": 1,
                  "sat_liq_density": "SaturatedLiquidDensity_EoS1", "id_sat_liq_density": 1}
WPAIR_TOTH = WPairStruct(wpair, wpair_approach, PATH_SORPPROPLIB, PATH_DB)

wpair = {"sorbent": "mof", "subtype": "cubtc", "refrigerant": "propane"}
wpair_approach = {"isotherm": "DualSiteSips", "id_isotherm": 1,
                  "vapor_pressure": "VaporPressure_EoS1", "id_vapor_pressure": 1,
                  "sat_liq_density": "SaturatedLiquidDensity_EoS1", "id_sat_liq_density": 1}
WPAIR_DSS = WPairStruct(wpair, wpair_approach, PATH_SORPPROPLIB, PATH_DB)

wpair = {"sorbent": "carbon", "subtype": "maxsorb-iii", "refrigerant": "R-134a"}
wpair_approach = {"isotherm": "DubininAstakhov", "id_isotherm": 1,
                  "vapor_pressure": "VaporPressure_EoS1", "id_vapor_pressure": 1,
                  "sat_liq_density": "SaturatedLiquidDensity_EoS1", "id_sat_liq_density": 1}
WPAIR_DASTAKHOV = WPairStruct(wpair, wpair_approach, PATH_SORPPROPLIB, PATH_DB)

wpair = {"sorbent": "silicagel", "subtype": "123", "refrigerant": "water"}
wpair_approach = {"isotherm": "DubininArctan1", "id_isotherm": 1,
                  "vapor_pressure": "VaporPressure_EoS1", "id_vapor_pressure": 1,
                  "sat_liq_density": "SaturatedLiquidDensity_EoS1", "id_sat_liq_density": 1}
WPAIR_DARCTAN1 = WPairStruct(wpair, wpair_approach, PATH_SORPPROPLIB, PATH_DB)

wpair = {"sorbent": "naoh-koh-csoh", "subtype": "", "refrigerant": "water"}
wpair_approach = {"isotherm": "Duehring", "id_isotherm": 1,
                  "vapor_pressure": "VaporPressure_EoS1", "id_vapor_pressure": 1,
                  "sat_liq_density": "SaturatedLiquidDensity_EoS1", "id_sat_liq_density": 1}
WPAIR_DUEHRING = WPairStruct(wpair, wpair_approach, PATH_SORPPROPLIB, PATH_DB)

wpair = {"sorbent": "il", "subtype": "[bmim][(cf3so2)2n]", "refrigerant": "water"}
wpair_approach = {"isotherm": "WilsonFixedDl", "id_isotherm": 1,
                  "vapor_pressure": "VaporPressure_EoS1", "id_vapor_pressure": 1,
                  "sat_liq_density": "SaturatedLiquidDensity_EoS1", "id_sat_liq_density": 1}
WPAIR_WILSON = WPairStruct(wpair, wpair_approach, PATH_SORPPROPLIB, PATH_DB)

wpair = {"sorbent": "oil", "subtype": "", "refrigerant": "R-600a"}
wpair_approach = {"isotherm": "MixingRule", "id_isotherm": 1,
                  "vapor_pressure": "NoVaporPressure", "id_vapor_pressure": 1,
                  "sat_liq_density": "NoSaturatedLiquidDensity", "id_sat_liq_density": 1}
WPAIR_MIX = WPairStruct(wpair, wpair_approach, PATH_SORPPROPLIB, PATH_DB)


# %% Test working pairs
# Calculations of reduced spreading pressure and inverse temperature take most of the time.
#
test_adsorption_sur([val for val in range(25,5001)], 293.15, 0.04401, WPAIR_LAGNMUIR)

test_adsorption_sur([val for val in range(25,51)], 293.15, 0.018015, WPAIR_TOTH)

test_adsorption_sur([val for val in range(25,5001)], 293.15, 0.0441, WPAIR_DSS)

test_adsorption_vol([val for val in range(25,31)], 293.15, 0.10203, WPAIR_DASTAKHOV)

test_adsorption_vol([val for val in range(25,31)], 293.15, 0.018015, WPAIR_DARCTAN1)

test_absorption_con([val for val in range(25,8001)], 343.15, WPAIR_DUEHRING)

test_absorption_act([val for val in range(40000,50001)], 353.15, 0.5, 0.5, WPAIR_WILSON)

test_absorption_mix([val for val in range(33000,33401)], 293.15, WPAIR_MIX)
