"""Module containing class 'WPairStructRefrigerant'.

Extended Summary
----------------
This module cotains the class 'WPairStructRefrigerant' that contains calculation routines based on
a C-struct to calculate refrigerant properties.

Revisions
---------
    Version     Date        Author              Changes
    1.0.0       2021-06-08  Mirko Engelpracht   First implementation
"""

# %% Import libraries.
#
from ctypes import c_double, cdll
from .interface_cdll import WorkingPair
from .wpair_partial import  WPairCStruct


# %% Ignore pylint messages that are not reasonable.
#
# pylint: disable-msg=C0103
# pylint: disable-msg=R0903
# pylint: disable-msg=W0221


# %% Definition of class WPairStructAdsorption
#
class WPairStructRefrigerant(WPairCStruct):
    r"""Class containing calculation routines for refrigerant properties of working pairs.

    This class contains calculations routines to calculate refrigerant properties of the
    refrigerant of an adsorption working pair. If no calculation approach for the vapor pressure
    or saturated liquid density is selected during initialization of the working pair, functions
    return -1.

    Methods
    -------
    p_sat_T:
        Calculates vapor pressure depending on temperature.
    T_sat_p:
        Calculates vapor temperature depending on pressure.
    rho_sat_l_T:
        Calculates saturated liquid density depending on temperature.
    dp_sat_dT_T:
        Calculates derivative of vapor pressure wrt. temperature depending on temperature.
    drho_sat_l_dT_T:
        Calculates derivative of aturated liquid density wrt. temperature depending on temperature.

    History
    -------
    06/08/2021, by Mirko Engelpracht:
        First implementation.
    """

    def __init__(self, library: cdll, c_struct: WorkingPair):
        r"""Initialize class containing calculation routines for refrigerant properties.

        This function initializes the refrigerant class and sets references to the library
        containing all functions of the SorpPropLib.DLL and to the c_struct containing a pointer
        to a specifc working pair.

        Parameters
        ----------
        library : <class 'ctypes.CDLL'>
            Reference to library containing all functions of the SorpPropLib.DLL.
        c_struct: <class 'sorpproplib.calc.interface_cdll.LP_WorkingPair'>
            Pointer to a specific working pair required by the library of the SorpPropLib.DLL.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        super().__init__(library, c_struct)

    def p_sat_T(self, T: float) -> float:
        r"""Calculate vapor pressure.

        This function calculates the vapor pressure p_sat depending on temperature T for the
        refrigerant that was initialized in the 'working pair'-struct.

        Parameters
        ----------
        T: float
            Temperature in K.

        Returns
        -------
        p_sat: float
            Vapor pressure in Pa.

        Notes
        -----
        Function returns -1 if calculation approach for vapor pressure was not selected during the
        initialization of the 'working pair'-struct or if selected calculation approach does not
        exist for refrigerant.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ref_p_sat_T(c_double(T), self._wpair_c_struct)

    def T_sat_p(self, p: float) -> float:
        r"""Calculate temperature.

        This function calculates the vapor temperature T_sat depending on apor pressure p_sat for
        the refrigerant that was initialized in the 'working pair'-struct.

        Parameters
        ----------
        p: float
            Vapor pressure in Pa.

        Returns
        -------
        T_sat: float
            Vapor temperature in K.

        Notes
        -----
        Function returns -1 if calculation approach for vapor pressure was not selected during the
        initialization of the 'working pair'-struct or if selected calculation approach does not
        exist for refrigerant.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ref_T_sat_p(c_double(p), self._wpair_c_struct)

    def rho_sat_l_T(self, T: float) -> float:
        r"""Calculate saturated liquid density.

        This function calculates the saturated liquid density rho_sat_l depending on temperature
        T for the refrigerant that was initialized in the 'working pair'-struct.

        Parameters
        ----------
        T: float
            Temperature in K.

        Returns
        -------
        rho_sat_l: float
            Saturated liquid density in kg/m³.

        Notes
        -----
        Function returns -1 if calculation approach for saturated liquid density was not selected
        during the initialization of the 'working pair'-struct or if selected calculation approach
        does not exist for refrigerant.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ref_rho_l_T(c_double(T), self._wpair_c_struct)

    def dp_sat_dT_T(self, T: float) -> float:
        r"""Calculate derivative of vapor pressure wrt. temperature.

        This function calculates the darivative of vapor pressure wrt. temperature dp_sat_dT
        depending on temperature T for the refrigerant that was initialized in the
        'working pair'-struct.

        Parameters
        ----------
        T: float
            Temperature in K.

        Returns
        -------
        dp_sat_dT: float
            Derivative of vapor pressure wrt. temperature in Pa/K.

        Notes
        -----
        Function returns -1 if calculation approach for vapor pressure was not selected during the
        initialization of the 'working pair'-struct or if selected calculation approach does not
        exist for refrigerant.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ref_dp_sat_dT_T(c_double(T), self._wpair_c_struct)

    def drho_sat_l_dT_T(self, T: float) -> float:
        r"""Calculate derivative of saturated liquid density wrt. temperature.

        This function calculates the derivative of saturated liquid density wrt. temperature
        rho_sat_l depending on temperature T for the refrigerant that was initialized in the
        'working pair'-struct.

        Parameters
        ----------
        T: float
            Temperature in K.

        Returns
        -------
        drho_sat_l_dT: float
            Darivative of saturated liquid density wrt. temperature in kg/m³/K.

        Notes
        -----
        Function returns -1 if calculation approach for saturated liquid density was not selected
        during the initialization of the 'working pair'-struct or if selected calculation approach
        does not exist for refrigerant.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ref_drho_l_dT_T(c_double(T), self._wpair_c_struct)
