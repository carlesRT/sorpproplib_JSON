"""Module containing function to load the SorpPropLib.DLL.

Extended Summary
----------------
This module cotains functions to load the SorpPropLib.DLL into Python. Thereby, C-structures
are mapped to equivalent Python-classes. Moreover, the return types and argument types of
all functions in the DLL are specified in order to allow type checks in Python.

Routine Listings
----------------
wrap_dll_function:
    Set up functions of the DLL, i.e., set up argument and return types.
load_dll:
    Load SorpPropLib.DLL and set up its functions.

Examples
--------
An example script is given below.

Revisions
---------
    Version     Date        Author              Changes
    1.0.0       2020-04-15  Mirko Engelpracht   First implementation
"""

# %% Import libraries.
#
import os
import struct
import sys

from ctypes import byref, c_int, c_double, c_char_p, cdll, Structure, POINTER, CFUNCTYPE
from typing import Union

from numpy import log


# %% Map C-structures to Python classes.
#
# Define function prototypes using the following notation:
#
#   1) Number of inputs of type double
#   2) Number of inputs of type double*
#   3) Number of inputs of type void*
#
GENFUNC_1_1_0 = CFUNCTYPE(POINTER(c_double),
                          c_double,
                          POINTER(c_double))
GENFUNC_1_1_1 = CFUNCTYPE(POINTER(c_double),
                          c_double,
                          POINTER(c_double),
                          POINTER(None))
GENFUNC_2_1_0 = CFUNCTYPE(POINTER(c_double),
                          c_double, c_double,
                          POINTER(c_double))
GENFUNC_2_3_2 = CFUNCTYPE(POINTER(c_double),
                          c_double, c_double,
                          POINTER(c_double), POINTER(c_double), POINTER(c_double),
                          POINTER(None), POINTER(None))
GENFUNC_3_1_0 = CFUNCTYPE(POINTER(c_double),
                          c_double, c_double, c_double,
                          POINTER(c_double))
GENFUNC_4_1_0 = CFUNCTYPE(POINTER(c_double),
                          c_double, c_double, c_double, c_double,
                          POINTER(c_double))
GENFUNC_4_1_1 = CFUNCTYPE(POINTER(c_double),
                          c_double, c_double, c_double, c_double,
                          POINTER(c_double),
                          POINTER(None))
GENFUNC_5_1_0 = CFUNCTYPE(POINTER(c_double),
                          c_double, c_double, c_double, c_double, c_double,
                          POINTER(c_double))
GENFUNC_6_1_0 = CFUNCTYPE(POINTER(c_double),
                          c_double, c_double, c_double, c_double, c_double, c_double,
                          POINTER(c_double))

GENMIXFUNC_1 = CFUNCTYPE(POINTER(None),
                         POINTER(c_double),
                         c_double,
                         POINTER(c_double))
GENMIXFUNC_2 = CFUNCTYPE(POINTER(None),
                         POINTER(c_double),
                         c_double, c_double, c_double, c_double, c_double, c_double,
                         POINTER(c_double))
GENMIXFUNC_3 = CFUNCTYPE(POINTER(None),
                         POINTER(c_double),
                         c_double, c_double, c_double, c_double)
GENMIXFUNC_4 = CFUNCTYPE(POINTER(c_double),
                         POINTER(c_int),
                         c_int, c_double, c_double, c_double, c_double, c_double,
                         c_double, c_double, c_double, c_double, c_double)


# Map C-structures
#
class Adsorption(Structure):
    r"""Map attributes of Adsorption-struct to Python class.

    The Adsorption-struct contains the following attributes:

    - String defining isotherm type
    - Function pointers of adsorption equations that are always defined
    - Function pointers of surface-based adsorption equations
    - Function pointers of surface-based adsorption equations that also use saturation
      pressure as additional function argument
    - Function pointers of volume-based adsorption equations
    """

    _fields_ = [('isotherm_type', c_char_p),


                ('w_pT', GENFUNC_2_3_2),
                ('p_wT', GENFUNC_2_3_2),
                ('T_pw', GENFUNC_2_3_2),

                ('dw_dp_pT', GENFUNC_2_3_2),
                ('dw_dT_pT', GENFUNC_2_3_2),
                ('dp_dw_wT', GENFUNC_2_3_2),
                ('dp_dT_wT', GENFUNC_2_3_2),

                ('piStar_pyxgTM', CFUNCTYPE(POINTER(c_double),
                                            c_double,
                                            c_double,
                                            c_double,
                                            c_double,
                                            c_double,
                                            c_double,
                                            POINTER(c_double),
                                            POINTER(c_double),
                                            POINTER(c_double),
                                            POINTER(None),
                                            POINTER(None))),


                ('sur_w_pT', GENFUNC_2_1_0),
                ('sur_p_wT', GENFUNC_2_1_0),
                ('sur_T_pw', GENFUNC_2_1_0),

                ('sur_dw_dp_pT', GENFUNC_2_1_0),
                ('sur_dw_dT_pT', GENFUNC_2_1_0),
                ('sur_dp_dw_wT', GENFUNC_2_1_0),
                ('sur_dp_dT_wT', GENFUNC_2_1_0),

                ('sur_piStar_pyxgTM', CFUNCTYPE(POINTER(c_double),
                                                c_double,
                                                c_double,
                                                c_double,
                                                c_double,
                                                c_double,
                                                c_double,
                                                POINTER(c_double))),


                ('sur_w_pTpsat', GENFUNC_3_1_0),
                ('sur_p_wTpsat', GENFUNC_3_1_0),
                ('sur_T_pwpsat', CFUNCTYPE(POINTER(c_double),
                                           c_double,
                                           c_double,
                                           GENFUNC_1_1_1,
                                           GENFUNC_1_1_1,
                                           POINTER(c_double),
                                           POINTER(c_double),
                                           POINTER(None))),

                ('sur_dw_dp_pTpsat', GENFUNC_3_1_0),
                ('sur_dw_dT_pTpsat', GENFUNC_4_1_0),
                ('sur_dp_dw_wTpsat', GENFUNC_3_1_0),
                ('sur_dp_dT_wTpsat', GENFUNC_4_1_0),

                ('sur_piStar_pyxgTpsatM', CFUNCTYPE(POINTER(c_double),
                                                    c_double,
                                                    c_double,
                                                    c_double,
                                                    c_double,
                                                    c_double,
                                                    c_double,
                                                    c_double,
                                                    POINTER(c_double))),


                ('vol_W_ARho', GENFUNC_2_1_0),
                ('vol_A_WRho', GENFUNC_2_1_0),
                ('vol_dW_dA_ARho', GENFUNC_2_1_0),
                ('vol_dA_dW_WRho', GENFUNC_2_1_0),

                ('vol_w_pTpsatRho', GENFUNC_4_1_0),
                ('vol_p_wTpsatRho', GENFUNC_4_1_0),
                ('vol_T_pwpsatRho', CFUNCTYPE(POINTER(c_double),
                                              c_double,
                                              c_double,
                                              GENFUNC_1_1_1,
                                              GENFUNC_1_1_0,
                                              GENFUNC_1_1_1,
                                              GENFUNC_1_1_0,
                                              POINTER(c_double),
                                              POINTER(c_double),
                                              POINTER(c_double),
                                              POINTER(None))),

                ('vol_dw_dp_pTpsatRho', GENFUNC_4_1_0),
                ('vol_dw_dT_pTpsatRho', GENFUNC_6_1_0),
                ('vol_dp_dw_wTpsatRho', GENFUNC_4_1_0),
                ('vol_dp_dT_wTpsatRho', GENFUNC_6_1_0),

                ('vol_piStar_pyxgTpsatRhoM', CFUNCTYPE(POINTER(c_double),
                                                       c_double,
                                                       c_double,
                                                       c_double,
                                                       c_double,
                                                       c_double,
                                                       c_double,
                                                       c_double,
                                                       c_double,
                                                       POINTER(c_double)))]


class Absorption(Structure):
    r"""Map attributes of Absorption-struct to Python class.

    The Absorption-struct contains the following attributes:

    - String defining isotherm type
    - Function pointers of conventional absorption equations
    - Function pointers of activity-based absorption equations
    - Function pointers of mixing-based absorption equations
    """

    _fields_ = [('isotherm_type', c_char_p),


                ('con_X_pT', GENFUNC_2_1_0),
                ('con_p_XT', GENFUNC_2_1_0),
                ('con_T_pX', GENFUNC_2_1_0),

                ('con_dX_dp_pT', GENFUNC_2_1_0),
                ('con_dX_dT_pT', GENFUNC_2_1_0),
                ('con_dp_dX_XT', GENFUNC_2_1_0),
                ('con_dp_dT_XT', GENFUNC_2_1_0),


                ('act_g_Tx_wo_v', GENFUNC_2_1_0),
                ('act_g_Txv1v2_w_v', GENFUNC_4_1_0),

                ('act_p_Txpsat_wo_v', GENFUNC_3_1_0),
                ('act_p_Txv1v2psat_w_v', GENFUNC_5_1_0),
                ('act_p_Txv1v2gpsat_w_v_gf', CFUNCTYPE(POINTER(c_double),
                                                       c_double,
                                                       c_double,
                                                       c_double,
                                                       c_double,
                                                       GENFUNC_4_1_0,
                                                       c_double,
                                                       POINTER(c_double))),
                ('act_p_Txgpsat_w_gf', CFUNCTYPE(POINTER(c_double),
                                                 c_double,
                                                 c_double,
                                                 GENFUNC_2_1_0,
                                                 c_double,
                                                 POINTER(c_double))),

                ('act_x_pTpsat_wo_v', GENFUNC_3_1_0),
                ('act_x_pTv1v2psat_w_v', GENFUNC_5_1_0),
                ('act_x_pTv1v2gpsat_w_v_gf', CFUNCTYPE(POINTER(c_double),
                                                       c_double,
                                                       c_double,
                                                       c_double,
                                                       c_double,
                                                       GENFUNC_4_1_0,
                                                       c_double,
                                                       POINTER(c_double))),
                ('act_x_pTgpsat_w_gf', CFUNCTYPE(POINTER(c_double),
                                                 c_double,
                                                 c_double,
                                                 GENFUNC_2_1_0,
                                                 c_double,
                                                 POINTER(c_double))),

                ('act_g_Txv1v2', GENFUNC_4_1_1),
                ('act_p_Txgv1v2psat', CFUNCTYPE(POINTER(c_double),
                                                c_double,
                                                c_double,
                                                c_double,
                                                c_double,
                                                c_double,
                                                POINTER(c_double),
                                                POINTER(None))),
                ('act_x_pTgv1v2psat', CFUNCTYPE(POINTER(c_double),
                                                c_double,
                                                c_double,
                                                c_double,
                                                c_double,
                                                c_double,
                                                POINTER(c_double),
                                                POINTER(None))),


                ('mix_x_pT', CFUNCTYPE(POINTER(c_double),
                                       POINTER(c_double),
                                       POINTER(c_double),
                                       c_double,
                                       c_double,
                                       POINTER(c_double),
                                       POINTER(None))),
                ('mix_p_Tx', CFUNCTYPE(POINTER(c_double),
                                       POINTER(c_double),
                                       POINTER(c_double),
                                       c_double,
                                       c_double,
                                       POINTER(c_double),
                                       POINTER(None))),
                ('mix_T_px', CFUNCTYPE(POINTER(c_double),
                                       POINTER(c_double),
                                       POINTER(c_double),
                                       c_double,
                                       c_double,
                                       POINTER(c_double),
                                       POINTER(None))),

                ('mix_dp_dx_Tx', CFUNCTYPE(POINTER(c_double),
                                           c_double,
                                           c_double,
                                           POINTER(c_double),
                                           POINTER(None))),
                ('mix_dp_dT_Tx', CFUNCTYPE(POINTER(c_double),
                                           c_double,
                                           c_double,
                                           POINTER(c_double),
                                           POINTER(None))),

                ('mix_pure_parameters', GENMIXFUNC_1),
                ('mix_mixture_parameters', GENMIXFUNC_2),
                ('mix_gen_parameters', GENMIXFUNC_3),
                ('mix_mixture_fugacity_coefficient', GENMIXFUNC_4)]


class Refrigerant(Structure):
    """Map attributes of Refrigerant-struct to Python class.

    The Refrigerant-struct contains only function pointers of refrigerant functions.
    The refrigerant function may be based on emperical correlations or cubic
    equations of states.
    """

    _fields_ = [('p_sat_T', GENFUNC_1_1_1),
                ('T_sat_p', GENFUNC_1_1_1),
                ('rho_l_T', GENFUNC_1_1_0),
                ('dp_sat_dT_T', GENFUNC_1_1_1),
                ('drho_l_dT_T', GENFUNC_1_1_0),

                ('other_p_sat_T', GENFUNC_1_1_0),
                ('other_T_sat_p', GENFUNC_1_1_0),
                ('other_dp_sat_dT_T', GENFUNC_1_1_0),

                ('cubic_eos_parameters', CFUNCTYPE(POINTER(None),
                                                   POINTER(c_double),
                                                   c_double,
                                                   POINTER(c_double))),
                ('cubic_gen_eos_parameters', CFUNCTYPE(POINTER(None),
                                                       POINTER(c_double),
                                                       c_double,
                                                       c_double,
                                                       c_double,
                                                       c_double)),
                ('cubic_fug_coefficient', CFUNCTYPE(POINTER(c_double),
                                                    POINTER(c_int),
                                                    c_double,
                                                    c_double,
                                                    c_double))]


class WorkingPair(Structure):
    """Map attributes of WorkingPair-struct to Python class.

    The WorkingPair-struct contains the following attributes:

    - Strings and integers defining the working pair and equilibrium equations
    - Arrays defining coefficients of the equilibrium equations
    - Structs containing functions pointers of the equilibrium equations
    """

    _fields_ = [('wp_as', c_char_p),
                ('wp_st', c_char_p),
                ('wp_rf', c_char_p),
                ('wp_iso', c_char_p),
                ('rf_p_sat', c_char_p),
                ('rf_rho_l', c_char_p),
                ('no_iso', c_int),
                ('no_p_sat', c_int),
                ('no_rho_l', c_int),

                ('iso_par', POINTER(c_double)),
                ('p_sat_par', POINTER(c_double)),
                ('rho_l_par', POINTER(c_double)),

                ('adsorption', POINTER(Adsorption)),
                ('absorption', POINTER(Absorption)),
                ('refrigerant', POINTER(Refrigerant))]


# %% Set up DLL functions.
#
def wrap_dll_function(library: cdll, name_func: list,
                      restype: Union[None, c_double, POINTER(WorkingPair)],
                      argtypes: list) -> cdll:
    r"""Wrap function of a DLL.

    This function gets a list 'name_func' containing names of functions in the DLL. For the given
    functions in the DLL, the types of the arguments and return value are set according to the
    user inputs. Note, the the given functions need to have the same arguments and return values.

    Parameters
    ----------
    library : ctypes.CDLL
        Handle of SorpPropLib.DLL.
    name_func: list
        List containing names of the function in the DLL.
    restype: None, ctypes.c_double, ctypes.POINTER(WorkingPair)
        Return type of the function of the DLL.
    argtypes: list
        List containing argument types of the function of the DLL.

    Returns
    -------
    sorpPropLib : ctypes.CDLL
        SorpPropLib.DLL that has parametrized functions.

    Raises
    ------
    AttributeError:
        If function given by 'name_func' does not exist in the given library.

    Examples
    --------
    >>> sorpPropLib = wrap_dll_function(sorpPropLib,
    >>>                                 ['ads_p_wT'],
    >>>                                 c_double,
    >>>                                 [c_double, c_double, POINTER(WorkingPair)])

    History
    -------
    04/15/2020, by Mirko Engelpracht:
        First implementation.
    """
    try:
        for _, val in enumerate(name_func):
            # Get function of DLL specified by user
            #
            func = library.__getattr__(val)

            # Set types of inputs and outputsy
            #
            func.restype = restype
            func.argtypes = argtypes

        return library

    except AttributeError as error:
        # Print error and throw error
        #
        raise AttributeError("The error '{}'".format(error.args[0]) +
                             " raised when wrapping the function '{}'".format(val) +
                             "of the library '{}'".format(library._name))


def load_dll(path: str) -> cdll:
    r"""Load SorpPropLib.DLL and set up its functions.

    This function loads the SorpPropLib.DLL from the specified path and
    sets up the DLL's functions (i.e., argument types and return types).

    Parameters
    ----------
    path : str
        Path that specifies SorpPropLib.DLL.

    Returns
    -------
    library : ctypes.CDLL
        SorpPropLib.DLL that has parametrized functions.

    Raises
    ------
    OSError:
        If DLL given by parameter path does not exist.
    AttributeError:
        If function given by 'name_func' does not exist in the given library.

    See Also
    --------
    wrap_dll_function:
        Function to set up a function contained in the library specified by 'path'.

    Examples
    --------
    >>> library = load_dll('libsorpPropLib.dll')
    >>> print(type(library))
    <class 'ctypes.CDLL'>

    History
    -------
    04/14/2020, by Mirko Engelpracht:
        First implementation.
    """
    try:
        # Load DLL
        #
        library = cdll.LoadLibrary(path)

        # Set up general functions
        #
        wrap_dll_function(library, ['newWorkingPair'], POINTER(WorkingPair),
                          [c_char_p, c_char_p, c_char_p, c_char_p, c_char_p, c_int,
                           c_char_p, c_int, c_char_p, c_int])
        wrap_dll_function(library, ['delWorkingPair'], None, [POINTER(WorkingPair)])

        # Set up adsorption functions based on the WorkingPair-struct
        #
        wrap_dll_function(library, ['ads_w_pT', 'ads_p_wT', 'ads_T_pw', 'ads_dw_dp_pT',
                                    'ads_dw_dT_pT', 'ads_dp_dw_wT', 'ads_dp_dT_wT'],
                          c_double, [c_double, c_double, POINTER(WorkingPair)])

        wrap_dll_function(library, ['ads_piStar_pyxgTM'], c_double,
                          [c_double, c_double, c_double, c_double, c_double, c_double,
                           POINTER(WorkingPair)])

        wrap_dll_function(library, ['ads_sur_w_pT', 'ads_sur_p_wT', 'ads_sur_T_pw',
                                    'ads_sur_dw_dp_pT',
                                    'ads_sur_dw_dT_pT', 'ads_sur_dp_dw_wT', 'ads_sur_dp_dT_wT'],
                          c_double, [c_double, c_double, POINTER(WorkingPair)])

        wrap_dll_function(library, ['ads_sur_piStar_pyxgTM'], c_double,
                          [c_double, c_double, c_double, c_double, c_double, c_double,
                           POINTER(WorkingPair)])

        wrap_dll_function(library, ['ads_sur_w_pTpsat', 'ads_sur_p_wTpsat', 'ads_sur_T_pwpsat',
                                    'ads_sur_dw_dp_pTpsat', 'ads_sur_dp_dw_wTpsat'],
                          c_double, [c_double, c_double, c_double, POINTER(WorkingPair)])

        wrap_dll_function(library, ['ads_sur_dw_dT_pTpsat'], c_double,
                          [c_double, c_double, c_double, c_double, POINTER(WorkingPair)])
        wrap_dll_function(library, ['ads_sur_dp_dT_wTpsat'], c_double,
                          [c_double, c_double, c_double, c_double, POINTER(WorkingPair)])
        wrap_dll_function(library, ['ads_sur_piStar_pyxgTpsatM'], c_double,
                          [c_double, c_double, c_double, c_double, c_double, c_double, c_double,
                           POINTER(WorkingPair)])

        wrap_dll_function(library, ['ads_vol_W_ARho', 'ads_vol_A_WRho', 'ads_vol_dW_dA_ARho',
                                    'ads_vol_dA_dW_WRho'],
                          c_double, [c_double, c_double, POINTER(WorkingPair)])

        wrap_dll_function(library, ['ads_vol_w_pTpsatRho', 'ads_vol_p_wTpsatRho',
                                    'ads_vol_T_pwpsatRho', 'ads_vol_dw_dp_pTpsatRho',
                                    'ads_vol_dp_dw_wTpsatRho'],
                          c_double, [c_double, c_double, c_double, c_double, POINTER(WorkingPair)])

        wrap_dll_function(library, ['ads_vol_dw_dT_pTpsatRho'], c_double,
                          [c_double, c_double, c_double, c_double, c_double, c_double,
                           POINTER(WorkingPair)])
        wrap_dll_function(library, ['ads_vol_dp_dT_wTpsatRho'], c_double,
                          [c_double, c_double, c_double, c_double, c_double, c_double,
                           POINTER(WorkingPair)])
        wrap_dll_function(library, ['ads_vol_piStar_pyxgTpsatRhoM'], c_double,
                          [c_double, c_double, c_double, c_double, c_double, c_double, c_double,
                           c_double, POINTER(WorkingPair)])

        # Set up absorption functions based on the WorkingPair-struct
        #
        wrap_dll_function(library, ['abs_con_X_pT', 'abs_con_p_XT', 'abs_con_T_pX',
                                    'abs_con_dX_dp_pT', 'abs_con_dX_dT_pT', 'abs_con_dp_dX_XT',
                                    'abs_con_dp_dT_XT'],
                          c_double, [c_double, c_double, POINTER(WorkingPair)])

        wrap_dll_function(library, ['abs_act_g_Txv1v2', 'abs_act_p_Txv1v2', 'abs_act_x_pTv1v2'],
                          c_double, [c_double, c_double, c_double, c_double, POINTER(WorkingPair)])

        wrap_dll_function(library, ['abs_act_p_Txv1v2psat'], c_double,
                          [c_double, c_double, c_double, c_double, c_double, POINTER(WorkingPair)])
        wrap_dll_function(library, ['abs_act_x_pTv1v2psat'], c_double,
                          [c_double, c_double, c_double, c_double, c_double, POINTER(WorkingPair)])

        wrap_dll_function(library, ['abs_mix_x_pT', 'abs_mix_p_Tx', 'abs_mix_T_px'],
                          c_double, [POINTER(c_double), POINTER(c_double), c_double, c_double,
                                     POINTER(WorkingPair)])

        wrap_dll_function(library, ['abs_mix_dp_dx_Tx'], c_double,
                          [c_double, c_double, POINTER(WorkingPair)])
        wrap_dll_function(library, ['abs_mix_dp_dT_Tx'], c_double,
                          [c_double, c_double, POINTER(WorkingPair)])

        # Set up refrigerant functions based on the WorkingPair-struct
        #
        wrap_dll_function(library, ['ref_p_sat_T', 'ref_dp_sat_dT_T', 'ref_rho_l_T',
                                    'ref_drho_l_dT_T', 'ref_T_sat_p'],
                          c_double, [c_double, POINTER(WorkingPair)])

        # Set up adsorption functions based on direct function call
        #
        wrap_dll_function(library, ['direct_ads_w_pT', 'direct_ads_p_wT', 'direct_ads_T_pw',
                                    'direct_ads_dw_dp_pT', 'direct_ads_dw_dT_pT',
                                    'direct_ads_dp_dw_wT', 'direct_ads_dp_dT_wT'],
                          c_double, [c_double, c_double, c_char_p, c_char_p, c_char_p, c_char_p,
                                     c_char_p, c_int, c_char_p, c_int, c_char_p, c_int])

        wrap_dll_function(library, ['direct_ads_piStar_pyxgTM'], c_double,
                          [c_double, c_double, c_double, c_double, c_double, c_double, c_char_p,
                           c_char_p, c_char_p, c_char_p, c_char_p, c_int, c_char_p, c_int,
                           c_char_p, c_int])

        wrap_dll_function(library, ['direct_ads_sur_w_pT', 'direct_ads_sur_p_wT',
                                    'direct_ads_sur_T_pw', 'direct_ads_sur_dw_dp_pT',
                                    'direct_ads_sur_dw_dT_pT', 'direct_ads_sur_dp_dw_wT',
                                    'direct_ads_sur_dp_dT_wT'],
                          c_double, [c_double, c_double, c_char_p, c_char_p, c_char_p, c_char_p,
                                     c_char_p, c_int, c_char_p, c_int, c_char_p, c_int])

        wrap_dll_function(library, ['direct_ads_sur_piStar_pyxgTM'], c_double,
                          [c_double, c_double, c_double, c_double, c_double, c_double, c_char_p,
                           c_char_p, c_char_p, c_char_p, c_char_p, c_int, c_char_p, c_int,
                           c_char_p, c_int])

        wrap_dll_function(library, ['direct_ads_sur_w_pTpsat', 'direct_ads_sur_p_wTpsat',
                                    'direct_ads_sur_T_pwpsat', 'direct_ads_sur_dw_dp_pTpsat',
                                    'direct_ads_sur_dp_dw_wTpsat'],
                          c_double, [c_double, c_double, c_double, c_char_p, c_char_p, c_char_p,
                                     c_char_p, c_char_p, c_int, c_char_p, c_int, c_char_p, c_int])

        wrap_dll_function(library, ['direct_ads_sur_dw_dT_pTpsat'], c_double,
                          [c_double, c_double, c_double, c_double, c_char_p, c_char_p, c_char_p,
                           c_char_p, c_char_p, c_int, c_char_p, c_int, c_char_p, c_int])
        wrap_dll_function(library, ['direct_ads_sur_dp_dT_wTpsat'], c_double,
                          [c_double, c_double, c_double, c_double, c_char_p, c_char_p, c_char_p,
                           c_char_p, c_char_p, c_int, c_char_p, c_int, c_char_p, c_int])
        wrap_dll_function(library, ['direct_ads_sur_piStar_pyxgTpsatM'], c_double,
                          [c_double, c_double, c_double, c_double, c_double, c_double, c_double,
                           c_char_p, c_char_p, c_char_p, c_char_p, c_char_p, c_int, c_char_p,
                           c_int, c_char_p, c_int])

        wrap_dll_function(library, ['direct_ads_vol_W_ARho', 'direct_ads_vol_A_WRho',
                                    'direct_ads_vol_dW_dA_ARho', 'direct_ads_vol_dA_dW_WRho'],
                          c_double, [c_double, c_double, c_char_p, c_char_p, c_char_p, c_char_p,
                                     c_char_p, c_int, c_char_p, c_int, c_char_p, c_int])

        wrap_dll_function(library, ['direct_ads_vol_w_pTpsatRho', 'direct_ads_vol_p_wTpsatRho',
                                    'direct_ads_vol_T_pwpsatRho', 'direct_ads_vol_dw_dp_pTpsatRho',
                                    'direct_ads_vol_dp_dw_wTpsatRho'],
                          c_double, [c_double, c_double, c_double, c_double, c_char_p, c_char_p,
                                     c_char_p, c_char_p, c_char_p, c_int, c_char_p, c_int,
                                     c_char_p, c_int])

        wrap_dll_function(library, ['direct_ads_vol_dw_dT_pTpsatRho'], c_double,
                          [c_double, c_double, c_double, c_double, c_double, c_double, c_char_p,
                           c_char_p, c_char_p, c_char_p, c_char_p, c_int, c_char_p, c_int,
                           c_char_p, c_int])
        wrap_dll_function(library, ['direct_ads_vol_dp_dT_wTpsatRho'], c_double,
                          [c_double, c_double, c_double, c_double, c_double, c_double, c_char_p,
                           c_char_p, c_char_p, c_char_p, c_char_p, c_int, c_char_p, c_int,
                           c_char_p, c_int])
        wrap_dll_function(library, ['direct_ads_vol_piStar_pyxgTpsatRhoM'], c_double,
                          [c_double, c_double, c_double, c_double, c_double, c_double, c_double,
                           c_double, c_char_p, c_char_p, c_char_p, c_char_p, c_char_p, c_int,
                           c_char_p, c_int, c_char_p, c_int])

        # Set up absorption functions based on direct function call
        #
        wrap_dll_function(library, ['direct_abs_con_X_pT', 'direct_abs_con_p_XT',
                                    'direct_abs_con_T_pX', 'direct_abs_con_dX_dp_pT',
                                    'direct_abs_con_dX_dT_pT', 'direct_abs_con_dp_dX_XT',
                                    'direct_abs_con_dp_dT_XT'],
                          c_double, [c_double, c_double, c_char_p, c_char_p, c_char_p, c_char_p,
                                     c_char_p, c_int, c_char_p, c_int, c_char_p, c_int])

        wrap_dll_function(library, ['direct_abs_act_g_Txv1v2', 'direct_abs_act_p_Txv1v2',
                                    'direct_abs_act_x_pTv1v2'],
                          c_double, [c_double, c_double, c_double, c_double, c_char_p, c_char_p,
                                     c_char_p, c_char_p, c_char_p, c_int, c_char_p, c_int,
                                     c_char_p, c_int])

        wrap_dll_function(library, ['direct_abs_act_p_Txv1v2psat'], c_double,
                          [c_double, c_double, c_double, c_double, c_double, c_char_p, c_char_p,
                           c_char_p, c_char_p, c_char_p, c_int, c_char_p, c_int, c_char_p, c_int])
        wrap_dll_function(library, ['direct_abs_act_x_pTv1v2psat'], c_double,
                          [c_double, c_double, c_double, c_double, c_double, c_char_p, c_char_p,
                           c_char_p, c_char_p, c_char_p, c_int, c_char_p, c_int, c_char_p, c_int])

        wrap_dll_function(library, ['direct_abs_mix_x_pT', 'direct_abs_mix_p_Tx',
                                    'direct_abs_mix_T_px'],
                          c_double, [POINTER(c_double), POINTER(c_double), c_double, c_double,
                                     c_char_p, c_char_p, c_char_p, c_char_p, c_char_p, c_int,
                                     c_char_p, c_int, c_char_p, c_int])

        wrap_dll_function(library, ['direct_abs_mix_dp_dx_Tx'], c_double,
                          [c_double, c_double, c_char_p, c_char_p, c_char_p, c_char_p, c_char_p,
                           c_int, c_char_p, c_int, c_char_p, c_int])
        wrap_dll_function(library, ['direct_abs_mix_dp_dT_Tx'], c_double,
                          [c_double, c_double, c_char_p, c_char_p, c_char_p, c_char_p, c_char_p,
                           c_int, c_char_p, c_int, c_char_p, c_int])

        # Set up refrigerant functions based on direct function call
        #
        wrap_dll_function(library, ['direct_ref_p_sat_T', 'direct_ref_dp_sat_dT_T',
                                    'direct_ref_rho_l_T', 'direct_ref_drho_l_dT_T',
                                    'direct_ref_T_sat_p'],
                          c_double, [c_double, c_char_p, c_char_p, c_char_p, c_char_p, c_char_p,
                                     c_int, c_char_p, c_int, c_char_p, c_int])

        # Return DLL
        #
        return library

    except OSError as error:
        raise OSError("The error '{}'".format(error.args[1]) +
                      " raised when trying to load the library '{}'.".format(path))


# %% Execute test script.
#
if __name__ == "__main__":
    # Set up paths depending on plattform and architecture
    #
    PATH_WRAPPER = os.path.join(os.getcwd().split("python_wrapper")[0], 'python_wrapper')
    PATH_DB = os.path.join(PATH_WRAPPER, 'sorpproplib', 'data', 'JSON',
                           'SorpPropLib_MinimalExample.json').encode()

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

    PATH_SORPPROPLIB = os.path.join(PATH_WRAPPER, 'sorpproplib', 'data', REQUIRED_DLL,
                                    'libsorpPropLib.dll')

    # Load DLL and create different working pairs
    #
    try:
        SORP_PROP_LIB = load_dll(PATH_SORPPROPLIB)

    except (AttributeError, OSError):
        print("\n\nERROR: Either DLL does not exist or DLL does not contain specified functions." +
              "\nThus, cannot test DLL-functions and stop test script!")
        sys.exit()

    # Create different working pairs
    #
    WP_AS = b"zeolite"
    WP_ST = b"13x"
    WP_RF = b"water"
    WP_ISO = b"Toth"
    NO_ISO = c_int(1)
    RF_PSAT = b"VaporPressure_EoS1"
    NO_PSAT = c_int(1)
    RF_RHOL = b"SaturatedLiquidDensity_EoS1"
    NO_RHOL = c_int(1)
    WPAIR_TOTH = SORP_PROP_LIB.newWorkingPair(PATH_DB, WP_AS, WP_ST, WP_RF, WP_ISO, NO_ISO,
                                              RF_PSAT, NO_PSAT, RF_RHOL, NO_RHOL)

    WP_AS = b"carbon"
    WP_ST = b"maxsorb-iii"
    WP_RF = b"R-134a"
    WP_ISO = b"DubininAstakhov"
    WPAIR_DUBI = SORP_PROP_LIB.newWorkingPair(PATH_DB, WP_AS, WP_ST, WP_RF, WP_ISO, NO_ISO,
                                              RF_PSAT, NO_PSAT, RF_RHOL, NO_RHOL)

    WP_AS = b"naoh-koh-csoh"
    WP_ST = b""
    WP_RF = b"water"
    WP_ISO = b"Duehring"
    WPAIR_DUEH = SORP_PROP_LIB.newWorkingPair(PATH_DB, WP_AS, WP_ST, WP_RF, WP_ISO, NO_ISO,
                                              RF_PSAT, NO_PSAT, RF_RHOL, NO_RHOL)

    WP_AS = b"il"
    WP_ST = b"[bmim][(cf3so2)2n]"
    WP_RF = b"water"
    WP_ISO = b"WilsonFixedDl"
    WPAIR_ACTI = SORP_PROP_LIB.newWorkingPair(PATH_DB, WP_AS, WP_ST, WP_RF, WP_ISO, NO_ISO,
                                              RF_PSAT, NO_PSAT, RF_RHOL, NO_RHOL)

    WP_AS = b"oil"
    WP_ST = b""
    WP_RF = b"R-600a"
    WP_ISO = b"MixingRule"
    RF_PSAT = b"NoVaporPressure"
    RF_RHOL = b"NoSaturatedLiquidDensity"
    WPAIR_MIXI = SORP_PROP_LIB.newWorkingPair(PATH_DB, WP_AS, WP_ST, WP_RF, WP_ISO, NO_ISO,
                                              RF_PSAT, NO_PSAT, RF_RHOL, NO_RHOL)

    # Execute functions of working pair 'WPAIR_MIXI'
    #
    X1 = (0.2 / 0.0581222) / (0.2 / 0.0581222 + 0.8 / 0.350)
    Y1 = c_double()
    Y2 = c_double()

    P_TX1 = SORP_PROP_LIB.abs_mix_p_Tx(byref(Y1), byref(Y2),
                                       c_double(313.15), c_double(X1), WPAIR_MIXI)
    X1_PT = SORP_PROP_LIB.abs_mix_x_pT(byref(Y1), byref(Y2),
                                       c_double(P_TX1), c_double(313.15), WPAIR_MIXI)
    T_PX1 = SORP_PROP_LIB.abs_mix_T_px(byref(Y1), byref(Y2),
                                       c_double(P_TX1), c_double(X1), WPAIR_MIXI)
    DP_DX1_TX1 = SORP_PROP_LIB.abs_mix_dp_dx_Tx(c_double(313.15), c_double(X1), WPAIR_MIXI)
    DP_DT_TX1 = SORP_PROP_LIB.abs_mix_dp_dT_Tx(c_double(313.15), c_double(X1), WPAIR_MIXI)

    print('#########################################################################')
    print('### Working pair: Oil / R-600a | Mixing isotherm + WorkingPair-struct ###')
    print('#########################################################################')

    print('\nMole fraction x1 = ' + str(X1_PT) + ' mol/mol')
    print('Pressure p = ' + str(P_TX1) + ' Pa')
    print('Temperature T = ' + str(T_PX1) + ' K')

    print('\nDerivative of pressure wrt mole fraction dp/dx1 = ' + str(DP_DX1_TX1) + ' Pa.mol/mol')
    print('Derivative of pressure wrt temperature dp/dT = ' + str(DP_DT_TX1) + ' Pa/K')

    # Execute functions of working pair 'WPAIR_ACTI'
    #
    GAMMA_TX = SORP_PROP_LIB.abs_act_g_Txv1v2(c_double(353.15), c_double(0.9386),
                                              c_double(-1), c_double(-1), WPAIR_ACTI)
    P_TX = SORP_PROP_LIB.abs_act_p_Txv1v2(c_double(353.15), c_double(0.9386),
                                          c_double(-1), c_double(-1), WPAIR_ACTI)
    X_PT = SORP_PROP_LIB.abs_act_x_pTv1v2(c_double(P_TX), c_double(353.15),
                                          c_double(-1), c_double(-1), WPAIR_ACTI)

    PSAT_T = SORP_PROP_LIB.ref_p_sat_T(c_double(353.15), WPAIR_ACTI)

    P_TXPSAT = SORP_PROP_LIB.abs_act_p_Txv1v2psat(c_double(353.15), c_double(0.9386),
                                                  c_double(-1), c_double(-1),
                                                  c_double(PSAT_T), WPAIR_ACTI)
    X_PTPSAT = SORP_PROP_LIB.abs_act_x_pTv1v2psat(c_double(P_TX), c_double(353.15),
                                                  c_double(-1), c_double(-1),
                                                  c_double(PSAT_T), WPAIR_ACTI)

    print('\n\n###############################################################################'
          '#############')
    print('### Working pair: [BMIM]+[(CF3SO2)2N]-(2) / Water | Wilson isotherm + '
          'WorkingPair-struct ###')
    print('###################################################################################'
          '#########')

    print('\nActivity coefficient gamma = ' + str(GAMMA_TX))
    print('Pressure p = ' + str(P_TX) + ' Pa')
    print('Mole fraction x = ' + str(X_PT) + ' mol/mol')

    # Execute functions of working pair 'WPAIR_DUEH'
    #
    X_PT = SORP_PROP_LIB.abs_con_X_pT(c_double(500), c_double(288.15), WPAIR_DUEH)
    P_WT = SORP_PROP_LIB.abs_con_p_XT(c_double(X_PT), c_double(288.15), WPAIR_DUEH)
    T_PW = SORP_PROP_LIB.abs_con_T_pX(c_double(500), c_double(X_PT), WPAIR_DUEH)
    DX_DP_PT = SORP_PROP_LIB.abs_con_dX_dp_pT(c_double(500), c_double(288.15), WPAIR_DUEH)
    DX_DT_PT = SORP_PROP_LIB.abs_con_dX_dT_pT(c_double(500), c_double(288.15), WPAIR_DUEH)
    DP_DX_XT = SORP_PROP_LIB.abs_con_dp_dX_XT(c_double(X_PT), c_double(288.15), WPAIR_DUEH)
    DP_DT_XT = SORP_PROP_LIB.abs_con_dp_dT_XT(c_double(X_PT), c_double(288.15), WPAIR_DUEH)

    print('\n\n###############################################################################'
          '####')
    print('### Working pair: NaOH-KOH-CsOH / Water | Dühring isotherm + WorkingPair-struct ###')
    print('###################################################################################')

    print('\nConcentration X = ' + str(X_PT) + ' kg/kg')
    print('Pressure p = ' + str(P_WT) + ' Pa')
    print('Temperature T = ' + str(T_PW) + ' K')

    print('\nDerivative of concentration wrt pressure dX/dp = ' + str(DX_DP_PT) + ' kg/kg/Pa')
    print('Derivative of concentration wrt temperature dX/dT = ' + str(DX_DT_PT) + ' kg/kg/K')
    print('Derivative of pressure wrt loading dp/dX = ' + str(DP_DX_XT) + ' Pa.kg/kg')
    print('Derivative of pressure wrt temperature dp/dT = ' + str(DP_DT_XT) + ' Pa/K')

    # Execute functions of working pair 'WPAIR_DUBI'
    #
    W_PT = SORP_PROP_LIB.ads_w_pT(c_double(500), c_double(288.15), WPAIR_DUBI)
    P_WT = SORP_PROP_LIB.ads_p_wT(c_double(W_PT), c_double(288.15), WPAIR_DUBI)
    T_PW = SORP_PROP_LIB.ads_T_pw(c_double(500), c_double(W_PT), WPAIR_DUBI)
    DW_DP_PT = SORP_PROP_LIB.ads_dw_dp_pT(c_double(500), c_double(288.15), WPAIR_DUBI)
    DW_DT_PT = SORP_PROP_LIB.ads_dw_dT_pT(c_double(500), c_double(288.15), WPAIR_DUBI)
    DP_DW_WT = SORP_PROP_LIB.ads_dp_dw_wT(c_double(W_PT), c_double(288.15), WPAIR_DUBI)
    DP_DT_WT = SORP_PROP_LIB.ads_dp_dT_wT(c_double(W_PT), c_double(288.15), WPAIR_DUBI)
    PISTAR_PYXGTM = SORP_PROP_LIB.ads_piStar_pyxgTM(c_double(500), c_double(1), c_double(1),
                                                    c_double(1), c_double(288.15),
                                                    c_double(0.10203), WPAIR_DUBI)

    PSAT_T = SORP_PROP_LIB.ref_p_sat_T(c_double(288.15), WPAIR_DUBI)
    DPSAT_DT_T = SORP_PROP_LIB.ref_dp_sat_dT_T(c_double(288.15), WPAIR_DUBI)
    RHOL_T = SORP_PROP_LIB.ref_rho_l_T(c_double(288.15), WPAIR_DUBI)
    DRHOL_DT_T = SORP_PROP_LIB.ref_drho_l_dT_T(c_double(288.15), WPAIR_DUBI)
    A_PT = 8.314462618 * 288.15 * log(PSAT_T / 288.15)

    W_A = SORP_PROP_LIB.ads_vol_W_ARho(c_double(A_PT), c_double(RHOL_T), WPAIR_DUBI)
    A_W = SORP_PROP_LIB.ads_vol_A_WRho(c_double(W_A), c_double(RHOL_T), WPAIR_DUBI)
    DW_DA_A = SORP_PROP_LIB.ads_vol_dW_dA_ARho(c_double(A_PT), c_double(RHOL_T), WPAIR_DUBI)
    DA_DW_W = SORP_PROP_LIB.ads_vol_dA_dW_WRho(c_double(W_A), c_double(RHOL_T), WPAIR_DUBI)

    W_PT_VOL = SORP_PROP_LIB.ads_vol_w_pTpsatRho(c_double(500), c_double(288.15),
                                                 c_double(PSAT_T), c_double(RHOL_T),
                                                 WPAIR_DUBI)
    P_WT_VOL = SORP_PROP_LIB.ads_vol_p_wTpsatRho(c_double(W_PT_VOL), c_double(288.15),
                                                 c_double(PSAT_T), c_double(RHOL_T),
                                                 WPAIR_DUBI)
    T_PW_VOL = SORP_PROP_LIB.ads_vol_T_pwpsatRho(c_double(500), c_double(W_PT_VOL),
                                                 c_double(PSAT_T), c_double(RHOL_T),
                                                 WPAIR_DUBI)
    DW_DP_PT_VOL = SORP_PROP_LIB.ads_vol_dw_dp_pTpsatRho(c_double(500), c_double(288.15),
                                                         c_double(PSAT_T), c_double(RHOL_T),
                                                         WPAIR_DUBI)
    DW_DT_PT_VOL = SORP_PROP_LIB.ads_vol_dw_dT_pTpsatRho(c_double(500), c_double(288.15),
                                                         c_double(PSAT_T), c_double(RHOL_T),
                                                         c_double(DPSAT_DT_T),
                                                         c_double(DRHOL_DT_T), WPAIR_DUBI)
    DP_DW_WT_VOL = SORP_PROP_LIB.ads_vol_dp_dw_wTpsatRho(c_double(W_PT_VOL), c_double(288.15),
                                                         c_double(PSAT_T), c_double(RHOL_T),
                                                         WPAIR_DUBI)
    DP_DT_WT_VOL = SORP_PROP_LIB.ads_vol_dp_dT_wTpsatRho(c_double(W_PT_VOL), c_double(288.15),
                                                         c_double(PSAT_T), c_double(RHOL_T),
                                                         c_double(DPSAT_DT_T),
                                                         c_double(DRHOL_DT_T), WPAIR_DUBI)
    PISTAR_PYXGTM_VOL = SORP_PROP_LIB.ads_vol_piStar_pyxgTpsatRhoM(c_double(500), c_double(1),
                                                                   c_double(1), c_double(1),
                                                                   c_double(288.15),
                                                                   c_double(PSAT_T),
                                                                   c_double(RHOL_T),
                                                                   c_double(0.10203), WPAIR_DUBI)

    print('\n\n#############################################################################'
          '#####################')
    print('### Working pair: Carbon Maxsorb-III / R-134a | Dubinin-Astakhov isotherm + '
          'WorkingPair-struct ###')
    print('#############################################################################'
          '#####################')

    print('\nVolumetric loading W = ' + str(W_A) + ' m³/kg')
    print('Adsorption potential A = ' + str(A_W) + ' J/mol')
    print('Derivative of volumetric loading wrt adsorption potential dW/dA = ' + str(DW_DA_A) +
          ' m³.mol/kg/J')
    print('Derivative of adsorption potential wrt volumetric loading dA/dW = ' + str(DA_DW_W) +
          ' J.kg/mol/m³')

    print('\nLoading w = ' + str(W_PT) + ' kg/kg (' + str(W_PT_VOL) +
          ' kg/kg using volume function)')
    print('Pressure p = ' + str(P_WT) + ' Pa (' + str(P_WT_VOL) +
          ' Pa using volume function)')
    print('Temperature T = ' + str(T_PW) + ' K (' + str(T_PW_VOL) +
          ' K using volume function)')

    print('\nDerivative of loading wrt pressure dw/dp = ' + str(DW_DP_PT) + ' kg/kg/Pa (' +
          str(DW_DP_PT_VOL) + ' kg/kg/Pa using volume function)')
    print('Derivative of loading wrt temperature dw/dT = ' + str(DW_DT_PT) + ' kg/kg/K (' +
          str(DW_DT_PT_VOL) + ' kg/kg/K using volume function)')
    print('Derivative of pressure wrt loading dp/dw = ' + str(DP_DW_WT) + ' Pa.kg/kg (' +
          str(DP_DW_WT_VOL) + ' kg/kg/Pa using volume function)')
    print('Derivative of pressure wrt temperature dp/dT = ' + str(DP_DT_WT) + ' Pa/K (' +
          str(DP_DT_WT_VOL) + ' Pa/K using volume function)')

    print('\nReduced spreading pressure pi_star = ' + str(PISTAR_PYXGTM) + ' kg/mol (' +
          str(PISTAR_PYXGTM_VOL) + ' kg/mol using volume function)')

    # Execute functions of working pair 'WPAIR_TOTH'
    #
    W_PT = SORP_PROP_LIB.ads_w_pT(c_double(500), c_double(288.15), WPAIR_TOTH)
    P_WT = SORP_PROP_LIB.ads_p_wT(c_double(W_PT), c_double(288.15), WPAIR_TOTH)
    T_PW = SORP_PROP_LIB.ads_T_pw(c_double(500), c_double(W_PT), WPAIR_TOTH)
    DW_DP_PT = SORP_PROP_LIB.ads_dw_dp_pT(c_double(500), c_double(288.15), WPAIR_TOTH)
    DW_DT_PT = SORP_PROP_LIB.ads_dw_dT_pT(c_double(500), c_double(288.15), WPAIR_TOTH)
    DP_DW_WT = SORP_PROP_LIB.ads_dp_dw_wT(c_double(W_PT), c_double(288.15), WPAIR_TOTH)
    DP_DT_WT = SORP_PROP_LIB.ads_dp_dT_wT(c_double(W_PT), c_double(288.15), WPAIR_TOTH)
    PISTAR_PYXGTM = SORP_PROP_LIB.ads_piStar_pyxgTM(c_double(500), c_double(1), c_double(1),
                                                    c_double(1), c_double(288.15),
                                                    c_double(0.018), WPAIR_TOTH)

    W_PT_SUR = SORP_PROP_LIB.ads_sur_w_pT(c_double(500), c_double(288.15), WPAIR_TOTH)
    P_WT_SUR = SORP_PROP_LIB.ads_sur_p_wT(c_double(W_PT_SUR), c_double(288.15), WPAIR_TOTH)
    T_PW_SUR = SORP_PROP_LIB.ads_sur_T_pw(c_double(500), c_double(W_PT_SUR), WPAIR_TOTH)
    DW_DP_PT_SUR = SORP_PROP_LIB.ads_sur_dw_dp_pT(c_double(500), c_double(288.15), WPAIR_TOTH)
    DW_DT_PT_SUR = SORP_PROP_LIB.ads_sur_dw_dT_pT(c_double(500), c_double(288.15), WPAIR_TOTH)
    DP_DW_WT_SUR = SORP_PROP_LIB.ads_sur_dp_dw_wT(c_double(W_PT_SUR), c_double(288.15),
                                                  WPAIR_TOTH)
    DP_DT_WT_SUR = SORP_PROP_LIB.ads_sur_dp_dT_wT(c_double(W_PT_SUR), c_double(288.15),
                                                  WPAIR_TOTH)
    PISTAR_PYXGTM_SUR = SORP_PROP_LIB.ads_sur_piStar_pyxgTM(c_double(500), c_double(1),
                                                            c_double(1), c_double(1),
                                                            c_double(288.15), c_double(0.018),
                                                            WPAIR_TOTH)

    print('\n\n##############################################################################')
    print('### Working pair: Zeolite 13X / Water | Toth isotherm + WorkingPair-struct ###')
    print('##############################################################################')

    print('\nLoading w = ' + str(W_PT) + ' kg/kg (' + str(W_PT_SUR) +
          ' kg/kg using surface function)')
    print('Pressure p = ' + str(P_WT) + ' Pa (' + str(P_WT_SUR) +
          ' Pa using surface function)')
    print('Temperature T = ' + str(T_PW) + ' K (' + str(T_PW_SUR) +
          ' K using surface function)')

    print('\nDerivative of loading wrt pressure dw/dp = ' + str(DW_DP_PT) + ' kg/kg/Pa (' +
          str(DW_DP_PT_SUR) + ' kg/kg/Pa using surface function)')
    print('Derivative of loading wrt temperature dw/dT = ' + str(DW_DT_PT) + ' kg/kg/K (' +
          str(DW_DT_PT_SUR) + ' kg/kg/K using surface function)')
    print('Derivative of pressure wrt loading dp/dw = ' + str(DP_DW_WT) + ' Pa.kg/kg (' +
          str(DP_DW_WT_SUR) + ' kg/kg/Pa using surface function)')
    print('Derivative of pressure wrt temperature dp/dT = ' + str(DP_DT_WT) + ' Pa/K (' +
          str(DP_DT_WT_SUR) + ' Pa/K using surface function)')

    print('\nReduced spreading pressure pi_star = ' + str(PISTAR_PYXGTM) + ' kg/mol (' +
          str(PISTAR_PYXGTM_SUR) + ' kg/mol using surface function)')

    # Free memory
    #
    SORP_PROP_LIB.delWorkingPair(WPAIR_MIXI)
    SORP_PROP_LIB.delWorkingPair(WPAIR_ACTI)
    SORP_PROP_LIB.delWorkingPair(WPAIR_DUEH)
    SORP_PROP_LIB.delWorkingPair(WPAIR_DUBI)
    SORP_PROP_LIB.delWorkingPair(WPAIR_TOTH)

    del WPAIR_MIXI
    del WPAIR_ACTI
    del WPAIR_DUEH
    del WPAIR_DUBI
    del WPAIR_TOTH
