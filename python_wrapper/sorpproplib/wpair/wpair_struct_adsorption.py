"""Module containing classes with calculation routines for adsorption isotherms based on C-struct.

Extended Summary
----------------
This module cotains the classes 'WPairStructAdsorption', 'WPairStructAdsorptionSurface',
'WPairStructAdsorptionSurfaceRefrigerant', and 'WPairStructAdsorptionVolumetric'. All classes
contain function for adsorption isotherms based on C-structs.

Revisions
---------
    Version     Date        Author              Changes
    1.0.0       2020-05-22  Mirko Engelpracht   First implementation
"""

# %% Import libraries.
#
from ctypes import c_double, cdll
from typing import Callable

from .interface_cdll import WorkingPair
from .wpair_partial import  WPairCStruct


# %% Ignore pylint messages that are not reasonable.
#
# pylint: disable-msg=C0103
# pylint: disable-msg=R0903
# pylint: disable-msg=W0221


# %% Definition of class WPairStructAdsorptionSurface
#
class WPairStructAdsorptionSurface(WPairCStruct):
    r"""Class containing calculation routines for adsorption working pairs.

    This class contains calculations routines to calculate equilibrium properties of an adsorption
    working pair. In this class, the provided methods a available for adsorption isotherms based on
    the surface approach.

    Methods
    -------
    w_pT:
        Calculates loading depending on pressure and temperature.
    p_wT:
        Calculates pressure depending on loading and temperature.
    T_pw:
        Calculates temperature depending on pressure and loading.
    dw_dp_pT:
        Calculates derivative of loading wrt. pressure depending on pressure and temperature.
    dw_dT_pT:
        Calculates derivative of loading wrt. temperature depending on pressure and temperature.
    dp_dw_wT:
        Calculates derivative of pressure wrt. loading depending on loading and temperature.
    dp_dT_wT:
        Calculates derivative of pressure wrt. temperature depending on loading and temperature.
    piStar_pyxgTM:
        Calculates reduced spreading pressure in depending on pressure, molar fraction of
        refrigerant in vapor phase, molar fraction of refrigerant in adsorbed phase, activity
        coefficient of refrigerant in adsorbed phase,  temperature, and molar mass of refrigerant.

    History
    -------
    06/08/2021, by Mirko Engelpracht:
        First implementation.
    """

    def __init__(self, library: cdll, c_struct: WorkingPair):
        r"""Initialize class containing all functions of adsorption working pairs.

        This function initializes the working pair class and sets references to the library
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

    def w_pT(self, p: float, T: float) -> float:
        r"""Cacluate loading.

        This function calculates the loading w depending on pressure p and temperature T for the
        adsorption isotherm that was initialized in the 'working pair'-struct.

        Parameters
        ----------
        p: float
            Pressure in Pa.
        T: float
            Temperature in K.

        Returns
        -------
        w: float
            Loading in kg/kg.

        Notes
        -----
        This function is defined for adsorption isotherms based on the surface approach. If
        function is not defined for selected isotherm, this function returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ads_sur_w_pT(c_double(p), c_double(T),
                                                self._wpair_c_struct)

    def p_wT(self, w: float, T: float) -> float:
        r"""Cacluate pressure.

        This function calculates the pressure p depending on loading w and temperature T for the
        adsorption isotherm that was initialized in the 'working pair'-struct.

        Parameters
        ----------
        w: float
            Loading in kg/kg.
        T: float
            Temperature in K.

        Returns
        -------
        p: float
            Pressure in Pa.

        Notes
        -----
        This function is defined for adsorption isotherms based on the surface approach. If
        function is not defined for selected isotherm, this function returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ads_sur_p_wT(c_double(w), c_double(T),
                                                self._wpair_c_struct)

    def T_pw(self, p: float, w: float) -> float:
        r"""Cacluate temperature.

        This function calculates the temperature T depending on pressure p and loading w for the
        adsorption isotherm that was initialized in the 'working pair'-struct.

        Parameters
        ----------
        p: float
            Pressure in Pa.
        T: float
            Temperature in K.

        Returns
        -------
        T: float
            Temperature in K.

        Notes
        -----
        This function is defined for adsorption isotherms based on the surface approach. If
        function is not defined for selected isotherm, this function returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ads_sur_T_pw(c_double(p), c_double(w),
                                                self._wpair_c_struct)

    def dw_dp_pT(self, p: float, T: float) -> float:
        r"""Cacluate derivative of loading wrt. pressure.

        This function calculates the derivative of loading w with respect to pressure depending on
        pressure p and temperature T for the adsorption isotherm that was initialized in the
        'working pair'-struct.

        Parameters
        ----------
        p: float
            Pressure in Pa.
        T: float
            Temperature in K.

        Returns
        -------
        dw_dp: float
            Derivative of loading wrt. pressure in kg/kg/Pa.

        Notes
        -----
        This function is defined for adsorption isotherms based on the surface approach. If
        function is not defined for selected isotherm, this function returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ads_sur_dw_dp_pT(c_double(p), c_double(T),
                                                    self._wpair_c_struct)

    def dw_dT_pT(self, p: float, T: float) -> float:
        r"""Cacluate derivative of loading wrt. temperature.

        This function calculates the derivative of loading w with respect to temperature depending
        on pressure p and temperature T for the adsorption isotherm that was initialized in the
        'working pair'-struct.

        Parameters
        ----------
        p: float
            Pressure in Pa.
        T: float
            Temperature in K.

        Returns
        -------
        dw_dT: float
            Derivative of loading wrt. temperature in kg/kg/K.

        Notes
        -----
        This function is defined for adsorption isotherms based on the surface approach. If
        function is not defined for selected isotherm, this function returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ads_sur_dw_dT_pT(c_double(p), c_double(T),
                                                    self._wpair_c_struct)

    def dp_dw_wT(self, w: float, T: float) -> float:
        r"""Cacluate derivative of pressure wrt. loading.

        This function calculates the derivative of pressure p with respect to loading depending on
        loading w and temperature T for the adsorption isotherm that was initialized in the
        'working pair'-struct.

        Parameters
        ----------
        w: float
            Loading in kg/kg.
        T: float
            Temperature in K.

        Returns
        -------
        dp_dw: float
            Derivative of pressure wrt. loading in Pa.kg/kg.

        Notes
        -----
        This function is defined for adsorption isotherms based on the surface approach. If
        function is not defined for selected isotherm, this function returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ads_sur_dp_dw_wT(c_double(w), c_double(T),
                                                    self._wpair_c_struct)

    def dp_dT_wT(self, w: float, T: float) -> float:
        r"""Cacluate derivative of pressure wrt. temperature.

        This function calculates the derivative of pressure p with respect to temperature depending
        on loading w and temperature T for the adsorption isotherm that was initialized in the
        'working pair'-struct.

        Parameters
        ----------
        w: float
            Loading in kg/kg.
        T: float
            Temperature in K.

        Returns
        -------
        dp_dw: float
            Derivative of pressure wrt. temperature in Pa/K.

        Notes
        -----
        This function is defined for adsorption isotherms based on the surface approach. If
        function is not defined for selected isotherm, this function returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ads_sur_dp_dT_wT(c_double(w), c_double(T),
                                                    self._wpair_c_struct)

    def piStar_pyxgTM(self, p: float, y: float, x: float,
                      gamma: float, T: float, M: float) -> float:
        r"""Cacluate reduced spreading pressure.

        This function calculates the reduced spreading pressure piStar depending on pressure p,
        molar fraction of refrigerant in vapor phase y, molar fraction of refrigerant in adsorbed
        phase x, activity coefficient of refrigerant in adsorbed phase gamma, temperature T, and
        molar mass of refrigerant M. The reduced spreading pressure is defined as follows:

            piStar = A * pi / (R * T * m_sorbent) = 1 / M * Integral_0^p0{w(p,T) / p * dp}

            where p0 = p_total*y / (gamma*x).

        Reduced spreading pressure may be needed to calculate adsorption equilibrium of mixtures,
        e.g., using the Ideal Adsorbed Solution Theory (IAST).

        Parameters
        ----------
        p: float
            Total pressure in Pa.
        y: float
            Molar fraction of refrigerant in vapor phase in mol/mol.
        x: float
            Molar fraction of refrigerant in adsorbed phase in mol/mol.
        gamma: float
            Activity coefficient of refrigerant in adsorbed phase.
        T: float
            Temperature in K.
        M: float
            Molar mass of refrigerant in kg/mol.

        Returns
        -------
        piStart: float
            Reduced spreading pressure in kg/mol.

        Notes
        -----
        This function often solves the integral using a numerical approach. Hence, accuracy may
        be to low for user.

        This function is defined for adsorption isotherms based on the surface approach. If
        function is not defined for selected isotherm, this function returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ads_sur_piStar_pyxgTM(c_double(p), c_double(y), c_double(x),
                                                         c_double(gamma), c_double(T),
                                                         c_double(M), self._wpair_c_struct)


# %% Definition of class WPairStructAdsorptionSurfaceRefrigerant
#
class WPairStructAdsorptionSurfaceRefrigerant(WPairCStruct):
    r"""Class containing calculation routines for adsorption working pairs.

    This class contains calculations routines to calculate equilibrium properties of an adsorption
    working pair. In this class, the provided methods a available for adsorption isotherms based on
    the surface approach and using refrigerant properties (e.g., vapor pressure).

    Methods
    -------
    w_pTpsat:
        Calculates loading depending on pressure, temperature, and vapor pressure.
    p_wTpsat:
        Calculates pressure depending on loading, temperature, and vapor pressure.
    T_pwpsat:
        Calculates temperature depending on pressure, loading, function for vapor pressure, and
        function for derivative of vapor pressure.
    dw_dp_pTpsat:
        Calculates derivative of loading wrt. pressure depending on pressure, temperature, and
        vapor pressure.
    dw_dT_pTpsat:
        Calculates derivative of loading wrt. temperature depending on pressure, temperature, vapor
        pressure, and derivative of vapor pressure wrt. temperature.
    dp_dw_wTpsat:
        Calculates derivative of pressure wrt. loading depending on loading, temperature, and
        vapor pressure.
    dp_dT_wTpsat:
        Calculates derivative of pressure wrt. temperature depending on loading, temperature, vapor
        pressure, and derivative of vapor pressure wrt. temperature.
    piStar_pyxgTpsatM:
        Calculates reduced spreading pressure in depending on pressure, molar fraction of
        refrigerant in vapor phase, molar fraction of refrigerant in adsorbed phase, activity
        coefficient of refrigerant in adsorbed phase,  temperature, vapor pressure, and molar mass
        of refrigerant.

    History
    -------
    06/08/2021, by Mirko Engelpracht:
        First implementation.
    """

    def __init__(self, library: cdll, c_struct: WorkingPair):
        r"""Initialize class containing all functions of adsorption working pairs.

        This function initializes the working pair class and sets references to the library
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

    def w_pTpsat(self, p: float, T: float, p_sat: float) -> float:
        r"""Cacluate loading.

        This function calculates the loading w depending on pressure p, temperature T, and vapor
        pressure p_sat for the adsorption isotherm that was initialized in the 'working pair'-
        struct.

        Parameters
        ----------
        p: float
            Pressure in Pa.
        T: float
            Temperature in K.
        p_sat: float
            Vapor pressure in Pa.

        Returns
        -------
        w: float
            Loading in kg/kg.

        Notes
        -----
        This function is defined for adsorption isotherms based on the surface approach and using
        refrigerant properties (e.g., vapor pressure). If function is not defined for selected
        isotherm, this function returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ads_sur_w_pTpsat(c_double(p), c_double(T), c_double(p_sat),
                                                    self._wpair_c_struct)

    def p_wTpsat(self, w: float, T: float, p_sat: float) -> float:
        r"""Cacluate pressure.

        This function calculates the pressure p depending on loading w, temperature T, and vapor
        pressure p_sat for the adsorption isotherm that was initialized in the 'working pair'-
        struct.

        Parameters
        ----------
        w: float
            Loading in kg/kg.
        T: float
            Temperature in K.
        p_sat: float
            Vapor pressure in Pa.

        Returns
        -------
        p: float
            Pressure in Pa.

        Notes
        -----
        This function is defined for adsorption isotherms based on the surface approach and using
        refrigerant properties (e.g., vapor pressure). If function is not defined for selected
        isotherm, this function returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ads_sur_p_wTpsat(c_double(w), c_double(T), c_double(p_sat),
                                                    self._wpair_c_struct)

    def T_pwpsat(self, p: float, w: float,
                 p_sat: Callable[[float], float], dp_sat_dT: Callable[[float], float],
                 T_guess: float = 313.15) -> float:
        r"""Cacluate temperature.

        This function calculates the temperature T depending on pressure p, loading w, and vapor
        pressure p_sat for the adsorption isotherm that was initialized in the 'working pair'-
        struct.

        Parameters
        ----------
        p: float
            Pressure in Pa.
        T: float
            Temperature in K.
        p_sat: function with float as input
            Function to calculate vapor pressure in Pa depending on temperature in K.
        dp_sat_dT: function with float as input
            Function to calculate derivative of vapor pressure wrt. temperature in Pa/K depending
            on temperature in K.
        T_guess: float, optional
            Guess value of temperature in K

        Returns
        -------
        T: float
            Temperature in K.

        Notes
        -----
        This function is defined for adsorption isotherms based on the surface approach and using
        refrigerant properties (e.g., vapor pressure). If function is not defined for selected
        isotherm, this function returns -1.

        Uses Newton-Raphson method for calculating equilibrium temperature.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        # Calculate start values
        #
        T = T_guess
        w_guess = self.w_pTpsat(p, T, p_sat(T))

        # Perform Newton-Raphson
        #
        counter = 0

        while counter < 50 and abs(w_guess-w) > 1e-8:
            # Calculate loading depending on pressure and guess value for temperature
            #
            w_guess = self.w_pTpsat(p, T, p_sat(T))

            # Calculate derivative of loading wrt. temperature depending on pressure and guess
            # value for temperature
            #
            dw_guess_dT = self.dw_dT_pTpsat( p, T, p_sat(T), dp_sat_dT(T))

            # Update guess value: Negative temperature are not allowed
            #
            T = T - (w_guess - w) / dw_guess_dT
            T = 100 if T < 0 else T

            counter = counter + 1

        # Return result
        #
        return T if counter < 50 else -1

    def dw_dp_pTpsat(self, p: float, T: float, p_sat: float) -> float:
        r"""Cacluate derivative of loading wrt. pressure.

        This function calculates the derivative of loading w with respect to pressure depending on
        pressure p, temperature T, and vapor pressure p_sat for the adsorption isotherm that was
        initialized in the 'working pair'-struct.

        Parameters
        ----------
        p: float
            Pressure in Pa.
        T: float
            Temperature in K.
        p_sat: float
            Vapor pressure in Pa.

        Returns
        -------
        dw_dp: float
            Derivative of loading wrt. pressure in kg/kg/Pa.

        Notes
        -----
        This function is defined for adsorption isotherms based on the surface approach and using
        refrigerant properties (e.g., vapor pressure). If function is not defined for selected
        isotherm, this function returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ads_sur_dw_dp_pTpsat(c_double(p), c_double(T), c_double(p_sat),
                                                        self._wpair_c_struct)

    def dw_dT_pTpsat(self, p: float, T: float, p_sat: float, dp_sat_dT: float) -> float:
        r"""Cacluate derivative of loading wrt. temperature.

        This function calculates the derivative of loading w with respect to temperature depending
        on pressure p, temperature T, vapor pressure p_sat, and derivative of vapor pressure wrt.
        temperature dp_sat_dT for the adsorption isotherm that was initialized in the
        'working pair'-struct.

        Parameters
        ----------
        p: float
            Pressure in Pa.
        T: float
            Temperature in K.
        p_sat: float
            Vapor pressure in Pa.
        dp_sat_dT: float
            Derivative of vapor pressure wrt. temperature in Pa/K.

        Returns
        -------
        dw_dT: float
            Derivative of loading wrt. temperature in kg/kg/K.

        Notes
        -----
        This function is defined for adsorption isotherms based on the surface approach and using
        refrigerant properties (e.g., vapor pressure). If function is not defined for selected
        isotherm, this function returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ads_sur_dw_dT_pTpsat(c_double(p), c_double(T),
                                                        c_double(p_sat), c_double(dp_sat_dT),
                                                        self._wpair_c_struct)

    def dp_dw_wTpsat(self, w: float, T: float, p_sat: float) -> float:
        r"""Cacluate derivative of pressure wrt. loading.

        This function calculates the derivative of pressure p with respect to loading depending on
        loading w, temperature T, and vapor pressure p_sat for the adsorption isotherm that was
        initialized in the 'working pair'-struct.

        Parameters
        ----------
        w: float
            Loading in kg/kg.
        T: float
            Temperature in K.
        p_sat: float
            Vapor pressure in Pa.

        Returns
        -------
        dp_dw: float
            Derivative of pressure wrt. loading in Pa.kg/kg.

        Notes
        -----
        This function is defined for adsorption isotherms based on the surface approach and using
        refrigerant properties (e.g., vapor pressure). If function is not defined for selected
        isotherm, this function returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ads_sur_dp_dw_wTpsat(c_double(w), c_double(T), c_double(p_sat),
                                                        self._wpair_c_struct)

    def dp_dT_wTpsat(self, w: float, T: float, p_sat: float, dp_sat_dT: float) -> float:
        r"""Cacluate derivative of pressure wrt. temperature.

        This function calculates the derivative of pressure p with respect to temperature depending
        on loading w, temperature T, vapor pressure p_sat, and derivative of vapor pressure wrt.
        temperature dp_sat_dT for the adsorption isotherm that was initialized in the
        'working pair'-struct.

        Parameters
        ----------
        w: float
            Loading in kg/kg.
        T: float
            Temperature in K.
        p_sat: float
            Vapor pressure in Pa.
        dp_sat_dT: float
            Derivative of vapor pressure wrt. temperature in Pa/K.

        Returns
        -------
        dp_dw: float
            Derivative of pressure wrt. temperature in Pa/K.

        Notes
        -----
        This function is defined for adsorption isotherms based on the surface approach and using
        refrigerant properties (e.g., vapor pressure). If function is not defined for selected
        isotherm, this function returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ads_sur_dp_dT_wTpsat(c_double(w), c_double(T),
                                                        c_double(p_sat), c_double(dp_sat_dT),
                                                        self._wpair_c_struct)

    def piStar_pyxgTpsatM(self, p: float, y: float, x: float,
                      gamma: float, T: float, p_sat: float, M: float) -> float:
        r"""Cacluate reduced spreading pressure.

        This function calculates the reduced spreading pressure piStar depending on pressure p,
        molar fraction of refrigerant in vapor phase y, molar fraction of refrigerant in adsorbed
        phase x, activity coefficient of refrigerant in adsorbed phase gamma, temperature T, vapor
        pressure p_sat, and molar mass of refrigerant M. The reduced spreading pressure is defined
        as follows:

            piStar = A * pi / (R * T * m_sorbent) = 1 / M * Integral_0^p0{w(p,T) / p * dp}

            where p0 = p_total*y / (gamma*x).

        Reduced spreading pressure may be needed to calculate adsorption equilibrium of mixtures,
        e.g., using the Ideal Adsorbed Solution Theory (IAST).

        Parameters
        ----------
        p: float
            Total pressure in Pa.
        y: float
            Molar fraction of refrigerant in vapor phase in mol/mol.
        x: float
            Molar fraction of refrigerant in adsorbed phase in mol/mol.
        gamma: float
            Activity coefficient of refrigerant in adsorbed phase.
        T: float
            Temperature in K.
        p_sat: float
            Vapor pressure in Pa.
        M: float
            Molar mass of refrigerant in kg/mol.

        Returns
        -------
        piStart: float
            Reduced spreading pressure in kg/mol.

        Notes
        -----
        This function often solves the integral using a numerical approach. Hence, accuracy may
        be to low for user.

        This function is defined for adsorption isotherms based on the surface approach and using
        refrigerant properties (e.g., vapor pressure). If function is not defined for selected
        isotherm, this function returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ads_sur_piStar_pyxgTpsatM(c_double(p), c_double(y),
                                                             c_double(x), c_double(gamma),
                                                             c_double(T),  c_double(p_sat),
                                                             c_double(M), self._wpair_c_struct)


# %% Definition of class WPairStructAdsorptionSurfaceRefrigerant
#
class WPairStructAdsorptionVolumetric(WPairCStruct):
    r"""Class containing calculation routines for adsorption working pairs.

    This class contains calculations routines to calculate equilibrium properties of an adsorption
    working pair. In this class, the provided methods a available for adsorption isotherms based on
    the volumetric approach (e.g., Dubinin).

    Methods
    -------
    W_ARho:
        Calculates volumetric loading depending on adsorption potential and saturated liquid
        density.
    A_WRho:
        Calculates adsorption potential depending on volumetric loading and saturated liquid
        density.
    dW_dA_ARho:
        Calculates derivative of volumetric loading wrt. adsorption potential depending on
        adsorption potential and saturated liquid density.
    dA_dW_WRho:
        Calculates derivative of adsorption potential wrt. volumetric loading depending on
        volumetric loading and saturated liquid density.
    w_pTpsatrho:
        Calculates loading depending on pressure, temperature, vapor pressure, and saturated
        liquid density.
    p_wTpsatrho:
        Calculates pressure depending on loading, temperature, vapor pressure, and saturated
        liquid density.
    T_pwpsatrho:
        Calculates temperature depending on pressure, loading, function for vapor pressure,
        function for derivative of vapor pressure, function for saturated liquid density, and
        function for derivative of saturated liquid density.
    dw_dp_pTpsatrho:
        Calculates derivative of loading wrt. pressure depending on pressure, temperature, vapor
        pressure, and saturated liquid density
    dw_dT_pTpsatrho:
        Calculates derivative of loading wrt. temperature depending on pressure, temperature, vapor
        pressure, saturated liquid density, derivative of vapor pressure, and derivative of
        saturated liquid density.
    dp_dw_wTpsatrho:
        Calculates derivative of pressure wrt. loading depending on loading, temperature, apor
        pressure, and saturated liquid density
    dp_dT_wTpsatrho:
        Calculates derivative of pressure wrt. temperature depending on loading, temperature, vapor
        pressure, saturated liquid density, derivative of vapor pressure, and derivative of
        saturated liquid density.
    piStar_pyxgTpsatrhoM:
        Calculates reduced spreading pressure in depending on pressure, molar fraction of
        refrigerant in vapor phase, molar fraction of refrigerant in adsorbed phase, activity
        coefficient of refrigerant in adsorbed phase,  temperature, vapor pressure, saturated
        liquid density, and molar mass of refrigerant.

    History
    -------
    06/08/2021, by Mirko Engelpracht:
        First implementation.
    """

    def __init__(self, library: cdll, c_struct: WorkingPair):
        r"""Initialize class containing all functions of adsorption working pairs.

        This function initializes the working pair class and sets references to the library
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

    def W_ARho(self, A: float, rho_sat_l: float) -> float:
        r"""Cacluate volumetric loading.

        This function calculates the volumetric loading W depending on adsorption potential A and
        saturated liquid density rho_sat_l for the adsorption isotherm that was initialized in the
        'working pair'-struct.

        Parameters
        ----------
        A: float
            Adsorption potential in J/mol.
        rho_sat_l: float
            Saturated liquid density in kg/m³.

        Returns
        -------
        W: float
            Volumetric loading in m³/kg.

        Notes
        -----
        This function is defined for adsorption isotherms based on the volumetric approach (e.g.,
        Dubinin). If saturated liquid density is not required by approach of characteristic curve
        W(A), it is ignored. If function is not defined for selected isotherm, this function
        returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ads_vol_W_ARho(c_double(A), c_double(rho_sat_l),
                                                  self._wpair_c_struct)

    def A_WRho(self, W: float, rho_sat_l: float) -> float:
        r"""Cacluate adsorption potential.

        This function calculates the adsorption potential A depending on volumetric loading W and
        saturated liquid density rho_sat_l for the adsorption isotherm that was initialized in the
        'working pair'-struct.

        Parameters
        ----------
        W: float
            Volumetric loading in m³/kg.
        rho_sat_l: float
            Saturated liquid density in kg/m³.

        Returns
        -------
        A: float
            Adsorption potential in J/mol.

        Notes
        -----
        This function is defined for adsorption isotherms based on the volumetric approach (e.g.,
        Dubinin). If saturated liquid density is not required by approach of characteristic curve
        W(A), it is ignored. If function is not defined for selected isotherm, this function
        returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ads_vol_A_WRho(c_double(W), c_double(rho_sat_l),
                                                  self._wpair_c_struct)

    def dW_dA_ARho(self, A: float, rho_sat_l: float) -> float:
        r"""Cacluate derivative of volumetric loading wrt. adsorption potential.

        This function calculates the derivative of volumetric loading wrt. adsorption potential
        dW_dA depending on adsorption potential A and saturated liquid density rho_sat_l for the
        adsorption isotherm that was initialized in the 'working pair'-struct.

        Parameters
        ----------
        A: float
            Adsorption potential in J/mol.
        rho_sat_l: float
            Saturated liquid density in kg/m³.

        Returns
        -------
        dW_dA: float
            Derivative of volumetric loading wrt. adsorption potential in m³mol/kg/J.

        Notes
        -----
        This function is defined for adsorption isotherms based on the volumetric approach (e.g.,
        Dubinin). If saturated liquid density is not required by approach of characteristic curve
        W(A), it is ignored. If function is not defined for selected isotherm, this function
        returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ads_vol_dW_dA_ARho(c_double(A), c_double(rho_sat_l),
                                                      self._wpair_c_struct)

    def dA_dW_WRho(self, W: float, rho_sat_l: float) -> float:
        r"""Cacluate derivative of adsorption potential wrt. volumetric loading.

        This function calculates the derivative of adsorption potential wrt. volumetric loading
        dA_dW depending on volumetric loading W and saturated liquid density rho_sat_l for the
        adsorption isotherm that was initialized in the 'working pair'-struct.

        Parameters
        ----------
        W: float
            Volumetric loading in m³/kg.
        rho_sat_l: float
            Saturated liquid density in kg/m³.

        Returns
        -------
        dA_dW: float
            Derivative of adsorption potential wrt. volumetric loading in kgJ/mol/m³.

        Notes
        -----
        This function is defined for adsorption isotherms based on the volumetric approach (e.g.,
        Dubinin). If saturated liquid density is not required by approach of characteristic curve
        W(A), it is ignored. If function is not defined for selected isotherm, this function
        returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ads_vol_dA_dW_WRho(c_double(W), c_double(rho_sat_l),
                                                      self._wpair_c_struct)

    def w_pTpsatrho(self, p: float, T: float, p_sat: float, rho_sat_l: float) -> float:
        r"""Cacluate loading.

        This function calculates the loading w depending on pressure p, temperature T, vapor
        pressure p_sat, and saturated liquid density rho_sat_l for the adsorption isotherm that
        was initialized in the 'working pair'-struct.

        Parameters
        ----------
        p: float
            Pressure in Pa.
        T: float
            Temperature in K.
        p_sat: float
            Vapor pressure in Pa.
        rho_sat_l: float
            Saturated liquid density in kg/m³.

        Returns
        -------
        w: float
            Loading in kg/kg.

        Notes
        -----
        This function is defined for adsorption isotherms based on the volumetric approach (e.g.,
        Dubinin). If function is not defined for selected isotherm, this function returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ads_vol_w_pTpsatRho(c_double(p), c_double(T),
                                                       c_double(p_sat), c_double(rho_sat_l),
                                                       self._wpair_c_struct)

    def p_wTpsatrho(self, w: float, T: float, p_sat: float, rho_sat_l: float) -> float:
        r"""Cacluate pressure.

        This function calculates the pressure p depending on loading w, temperature T, vapor
        pressure p_sat, and saturated liquid density rho_sat_l for the adsorption isotherm that
        was initialized in the 'working pair'-struct.

        Parameters
        ----------
        w: float
            Loading in kg/kg.
        T: float
            Temperature in K.
        p_sat: float
            Vapor pressure in Pa.
        rho_sat_l: float
            Saturated liquid density in kg/m³.

        Returns
        -------
        p: float
            Pressure in Pa.

        Notes
        -----
        This function is defined for adsorption isotherms based on the volumetric approach (e.g.,
        Dubinin). If function is not defined for selected isotherm, this function returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ads_vol_p_wTpsatRho(c_double(w), c_double(T),
                                                       c_double(p_sat), c_double(rho_sat_l),
                                                       self._wpair_c_struct)

    def T_pwpsatrho(self, p: float, w: float,
                 p_sat: Callable[[float], float], dp_sat_dT: Callable[[float], float],
                 rho_sat_l: Callable[[float], float], drho_sat_l_dT: Callable[[float], float],
                 T_guess: float = 313.15) -> float:
        r"""Cacluate temperature.

        This function calculates the temperature T depending on pressure p, loading w, and vapor
        pressure p_sat for the adsorption isotherm that was initialized in the 'working pair'-
        struct.

        Parameters
        ----------
        p: float
            Pressure in Pa.
        T: float
            Temperature in K.
        p_sat: function with float as input
            Function to calculate vapor pressure in Pa depending on temperature in K.
        dp_sat_dT: function with float as input
            Function to calculate derivative of vapor pressure wrt. temperature in Pa/K depending
            on temperature in K.
        rho_sat_l: function with float as input
            Function to calculate saturated liquid density in kg/m³ depending on temperature in K.
        drho_sat_l_dT: function with float as input
            Function to calculate derivative of saturated liquid density wrt. temperature in
            Pa/K depending on temperature in kg/m³/K.
        T_guess: float, optional
            Guess value of temperature in K

        Returns
        -------
        T: float
            Temperature in K.

        Notes
        -----
        This function is defined for adsorption isotherms based on the volumetric approach (e.g.,
        Dubinin). If function is not defined for selected isotherm, this function returns -1.

        Uses Newton-Raphson method for calculating equilibrium temperature.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        # Calculate start values
        #
        T = T_guess
        w_guess = self.w_pTpsatrho(p, T, p_sat(T), rho_sat_l(T))

        # Perform Newton-Raphson
        #
        counter = 0

        while counter < 50 and abs(w_guess-w) > 1e-8:
            # Calculate loading depending on pressure and guess value for temperature
            #
            w_guess = self.w_pTpsatrho(p, T, p_sat(T), rho_sat_l(T))

            # Calculate derivative of loading wrt. temperature depending on pressure and guess
            # value for temperature
            #
            dw_guess_dT = self.dw_dT_pTpsatrho( p, T, p_sat(T), dp_sat_dT(T),
                                               rho_sat_l(T), drho_sat_l_dT(T))

            # Update guess value: Negative temperature are not allowed
            #
            T = T - (w_guess - w) / dw_guess_dT
            T = 100 if T < 0 else T

            counter = counter + 1

        # Return result
        #
        return T if counter < 50 else -1

    def dw_dp_pTpsatrho(self, p: float, T: float, p_sat: float, rho_sat_l: float) -> float:
        r"""Cacluate derivative of loading wrt. pressure.

        This function calculates the derivative of loading w with respect to pressure depending on
        pressure p, temperature T, vapor pressure p_sat, and saturated liquid density rho_sat_l
        for the adsorption isotherm that was initialized in the 'working pair'-struct.

        Parameters
        ----------
        p: float
            Pressure in Pa.
        T: float
            Temperature in K.
        p_sat: float
            Vapor pressure in Pa.
        rho_sat_l: float
            Saturated liquid density in kg/m³.

        Returns
        -------
        dw_dp: float
            Derivative of loading wrt. pressure in kg/kg/Pa.

        Notes
        -----
        This function is defined for adsorption isotherms based on the volumetric approach (e.g.,
        Dubinin). If function is not defined for selected isotherm, this function returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ads_vol_dw_dp_pTpsatRho(c_double(p), c_double(T),
                                                           c_double(p_sat), c_double(rho_sat_l),
                                                           self._wpair_c_struct)

    def dw_dT_pTpsatrho(self, p: float, T: float,
                        p_sat: float, dp_sat_dT: float,
                        rho_sat_l: float, drho_sat_l_dT: float) -> float:
        r"""Cacluate derivative of loading wrt. temperature.

        This function calculates the derivative of loading w with respect to temperature depending
        on pressure p, temperature T, vapor pressure p_sat,  derivative of vapor pressure wrt.
        temperature dp_sat_dT, saturated liquid density rho_sat_l, and derivative of saturated
        liquid density wrt. temperature drho_sat_l_dT for the adsorption isotherm that was
        initialized in the 'working pair'-struct.

        Parameters
        ----------
        p: float
            Pressure in Pa.
        T: float
            Temperature in K.
        p_sat: float
            Vapor pressure in Pa.
        dp_sat_dT: float
            Derivative of vapor pressure wrt. temperature in Pa/K.
        rho_sat_l: float
            Saturated liquid density in kg/m³.
        drho_sat_l_dT: float
            Derivative of saturated liquid density wrt. temperature in kg/m³/K.

        Returns
        -------
        dw_dT: float
            Derivative of loading wrt. temperature in kg/kg/K.

        Notes
        -----
        This function is defined for adsorption isotherms based on the volumetric approach (e.g.,
        Dubinin). If function is not defined for selected isotherm, this function returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ads_vol_dw_dT_pTpsatRho(c_double(p), c_double(T),
                                                           c_double(p_sat), c_double(rho_sat_l),
                                                           c_double(dp_sat_dT),
                                                           c_double(drho_sat_l_dT),
                                                           self._wpair_c_struct)

    def dp_dw_wTpsatrho(self, w: float, T: float, p_sat: float, rho_sat_l: float) -> float:
        r"""Cacluate derivative of pressure wrt. loading.

        This function calculates the derivative of pressure p with respect to loading depending on
        loading w, temperature T, vapor pressure p_sat, and saturated liquid density rho_sat_l for
        the adsorption isotherm that was initialized in the 'working pair'-struct.

        Parameters
        ----------
        w: float
            Loading in kg/kg.
        T: float
            Temperature in K.
        p_sat: float
            Vapor pressure in Pa.
        rho_sat_l: float
            Saturated liquid density in kg/m³.

        Returns
        -------
        dp_dw: float
            Derivative of pressure wrt. loading in Pa.kg/kg.

        Notes
        -----
        This function is defined for adsorption isotherms based on the volumetric approach (e.g.,
        Dubinin). If function is not defined for selected isotherm, this function returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ads_vol_dp_dw_wTpsatRho(c_double(w), c_double(T),
                                                           c_double(p_sat), c_double(rho_sat_l),
                                                           self._wpair_c_struct)

    def dp_dT_wTpsatrho(self, w: float, T: float,
                        p_sat: float, dp_sat_dT: float,
                        rho_sat_l: float, drho_sat_l_dT: float) -> float:
        r"""Cacluate derivative of pressure wrt. temperature.

        This function calculates the derivative of pressure p with respect to temperature depending
        on loading w, temperature T, vapor pressure p_sat, derivative of vapor pressure wrt.
        temperature dp_sat_dT, saturated liquid density rho_sat_l, and derivative of saturated
        liquid density wrt. temperature drho_sat_l_dT for the adsorption isotherm that was
        initialized in the 'working pair'-struct.

        Parameters
        ----------
        w: float
            Loading in kg/kg.
        T: float
            Temperature in K.
        p_sat: float
            Vapor pressure in Pa.
        dp_sat_dT: float
            Derivative of vapor pressure wrt. temperature in Pa/K.
        rho_sat_l: float
            Saturated liquid density in kg/m³.
        drho_sat_l_dT: float
            Derivative of saturated liquid density wrt. temperature in kg/m³/K.

        Returns
        -------
        dp_dw: float
            Derivative of pressure wrt. temperature in Pa/K.

        Notes
        -----
        This function is defined for adsorption isotherms based on the volumetric approach (e.g.,
        Dubinin). If function is not defined for selected isotherm, this function returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ads_vol_dp_dT_wTpsatRho(c_double(w), c_double(T),
                                                           c_double(p_sat), c_double(rho_sat_l),
                                                           c_double(dp_sat_dT),
                                                           c_double(drho_sat_l_dT),
                                                           self._wpair_c_struct)

    def piStar_pyxgTpsatrhoM(self, p: float, y: float, x: float,
                      gamma: float, T: float, p_sat: float, rho_sat_l: float, M: float) -> float:
        r"""Cacluate reduced spreading pressure.

        This function calculates the reduced spreading pressure piStar depending on pressure p,
        molar fraction of refrigerant in vapor phase y, molar fraction of refrigerant in adsorbed
        phase x, activity coefficient of refrigerant in adsorbed phase gamma, temperature T, vapor
        pressure p_sat, saturated liquid density rho_sat_l, and molar mass of refrigerant M. The
        reduced spreading pressure is defined as follows:

            piStar = A * pi / (R * T * m_sorbent) = 1 / M * Integral_0^p0{w(p,T) / p * dp}

            where p0 = p_total*y / (gamma*x).

        Reduced spreading pressure may be needed to calculate adsorption equilibrium of mixtures,
        e.g., using the Ideal Adsorbed Solution Theory (IAST).

        Parameters
        ----------
        p: float
            Total pressure in Pa.
        y: float
            Molar fraction of refrigerant in vapor phase in mol/mol.
        x: float
            Molar fraction of refrigerant in adsorbed phase in mol/mol.
        gamma: float
            Activity coefficient of refrigerant in adsorbed phase.
        T: float
            Temperature in K.
        p_sat: float
            Vapor pressure in Pa.
        rho_sat_l: float
            Saturated liquid density in kg/m³.
        M: float
            Molar mass of refrigerant in kg/mol.

        Returns
        -------
        piStart: float
            Reduced spreading pressure in kg/mol.

        Notes
        -----
        This function often solves the integral using a numerical approach. Hence, accuracy may
        be to low for user.

        This function is defined for adsorption isotherms based on the volumetric approach (e.g.,
        Dubinin). If function is not defined for selected isotherm, this function returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ads_vol_piStar_pyxgTpsatRhoM(c_double(p), c_double(y),
                                                                c_double(x), c_double(gamma),
                                                                c_double(T),  c_double(p_sat),
                                                                c_double(rho_sat_l), c_double(M),
                                                                self._wpair_c_struct)


# %% Definition of class WPairStructAdsorption
#
class WPairStructAdsorption(WPairCStruct):
    r"""Class containing calculation routines for adsorption working pairs.

    This class contains calculations routines to calculate equilibrium properties of an adsorption
    working pair. In this class, the provided methods a available for all adsorption isotherms. If
    an adsorption isotherm requires properties of the refrigerant such as the vapor pressure, this
    function uses refrigerant functions provided by the SorpPropLib and selected during the
    initialization of this class.

    If other refrigerant functions are required, please check out the specical functions provided
    by the sub-classes 'Sur', 'SurRef' and 'Vol' for isotherms based on the surface approach, the
    surface approach and refrigerant properties, or the volumetric approach, respectively.

    Attributes
    ----------
    Sur: class WPairStructAdsorptionSurface
        Class containing functions to calculate equilibrium properties of adsorption isotherms
        based on the surface approach.
    SurRef: class WPairStructAdsorptionSurfaceRefrigerant
        Class containing functions to calculate equilibrium properties of adsorption isotherms
        based on the surface approach using refrigerant properties (e.g., vapor pressure).
    Vol: class WPairStructAdsorptionVolumetric
        Class containing functions to calculate equilibrium properties of absorption isotherms
        based on the volumetric approach.

    Methods
    -------
    w_pT:
        Calculates loading depending on pressure and temperature.
    p_wT:
        Calculates pressure depending on loading and temperature.
    T_pw:
        Calculates temperature depending on pressure and loading.
    dw_dp_pT:
        Calculates derivative of loading wrt. pressure depending on pressure and temperature.
    dw_dT_pT:
        Calculates derivative of loading wrt. temperature depending on pressure and temperature.
    dp_dw_wT:
        Calculates derivative of pressure wrt. loading depending on loading and temperature.
    dp_dT_wT:
        Calculates derivative of pressure wrt. temperature depending on loading and temperature.
    piStar_pyxgTM:
        Calculates reduced spreading pressure in depending on pressure, molar fraction of
        refrigerant in vapor phase, molar fraction of refrigerant in adsorbed phase, activity
        coefficient of refrigerant in adsorbed phase,  temperature, and molar mass of refrigerant.

    History
    -------
    05/23/2020, by Mirko Engelpracht:
        First implementation.
    """

    def __init__(self, library: cdll, c_struct: WorkingPair):
        r"""Initialize class containing all functions of adsorption working pairs.

        This function initializes the working pair class and sets references to the library
        containing all functions of the SorpPropLib.DLL and to the c_struct containing a pointer
        to a specifc working pair. Furthermore, it initializes sub-classes containing special
        functions for adsoption isotherms based on the surface approach, the surface approach using
        refrigerant properties, or the volumetric approach.

        Parameters
        ----------
        library : <class 'ctypes.CDLL'>
            Reference to library containing all functions of the SorpPropLib.DLL.
        c_struct: <class 'sorpproplib.calc.interface_cdll.LP_WorkingPair'>
            Pointer to a specific working pair required by the library of the SorpPropLib.DLL.

        History
        -------
        05/23/2020, by Mirko Engelpracht:
            First implementation.
        """
        # Initialize sub-classes containing specialised classes to calculate equilibrium
        # properties
        #
        super().__init__(library, c_struct)

        self.Sur = WPairStructAdsorptionSurface(library, c_struct)
        self.SurRef = WPairStructAdsorptionSurfaceRefrigerant(library, c_struct)
        self.Vol = WPairStructAdsorptionVolumetric(library, c_struct)

    def w_pT(self, p: float, T: float) -> float:
        r"""Cacluate loading.

        This function calculates the loading w depending on pressure p and temperature T for the
        adsorption isotherm that was initialized in the 'working pair'-struct.

        Parameters
        ----------
        p: float
            Pressure in Pa.
        T: float
            Temperature in K.

        See Also
        --------
        Sur.w_pT:
            Same function but only valid for adsorption isotherms based on the suraface approach.
        SurRef.w_pTpsat:
            Same function but only valid for adsorption isotherms based on the suraface approach
            and also requiring the vapor pressure.
        Vol.w_pTpsatRho:
            Same function but only valid for adsorption isotherms based on the volumetric approach.

        Returns
        -------
        w: float
            Loading in kg/kg.

        Notes
        -----
        This function is defined for all adsorption isotherms. If an adsorption isotherm requires
        properties of the refrigerant such as the vapor pressure, this function uses refrigerant
        functions provided by the SorpPropLib and selected during initialization of this class.

        If other refrigerant functions are required, please check out the specical functions that
        are not valid for all adsorption isotherms but allow to specify the refrigerant functions.

        History
        -------
        05/23/2020, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ads_w_pT(c_double(p), c_double(T), self._wpair_c_struct)

    def p_wT(self, w: float, T: float) -> float:
        r"""Cacluate pressure.

        This function calculates the pressure p depending on loading w and temperature T for the
        adsorption isotherm that was initialized in the 'working pair'-struct.

        Parameters
        ----------
        w: float
            Loading in kg/kg.
        T: float
            Temperature in K.

        See Also
        --------
        Sur.p_wT:
            Same function but only valid for adsorption isotherms based on the suraface approach.
        SurRef.p_wTpsat:
            Same function but only valid for adsorption isotherms based on the suraface approach
            and also requiring the vapor pressure.
        Vol.p_wTpsatRho:
            Same function but only valid for adsorption isotherms based on the volumetric approach.

        Returns
        -------
        p: float
            Pressure in Pa.

        Notes
        -----
        This function is defined for all adsorption isotherms. If an adsorption isotherm requires
        properties of the refrigerant such as the vapor pressure, this function uses refrigerant
        functions provided by the SorpPropLib and selected during initialization of this class.

        If other refrigerant functions are required, please check out the specical functions that
        are not valid for all adsorption isotherms but allow to specify the refrigerant functions.

        History
        -------
        05/23/2020, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ads_p_wT(c_double(w), c_double(T), self._wpair_c_struct)

    def T_pw(self, p: float, w: float) -> float:
        r"""Cacluate temperature.

        This function calculates the temperature T depending on pressure p and loading w for the
        adsorption isotherm that was initialized in the 'working pair'-struct.

        Parameters
        ----------
        p: float
            Pressure in Pa.
        T: float
            Temperature in K.

        See Also
        --------
        Sur.T_pT:
            Same function but only valid for adsorption isotherms based on the suraface approach.
        SurRef.T_pTpsat:
            Same function but only valid for adsorption isotherms based on the suraface approach
            and also requiring the vapor pressure.
        Vol.T_pTpsatRho:
            Same function but only valid for adsorption isotherms based on the volumetric approach.

        Returns
        -------
        T: float
            Temperature in K.

        Notes
        -----
        This function is defined for all adsorption isotherms. If an adsorption isotherm requires
        properties of the refrigerant such as the vapor pressure, this function uses refrigerant
        functions provided by the SorpPropLib and selected during initialization of this class.

        If other refrigerant functions are required, please check out the specical functions that
        are not valid for all adsorption isotherms but allow to specify the refrigerant functions.

        History
        -------
        05/23/2020, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ads_T_pw(c_double(p), c_double(w), self._wpair_c_struct)

    def dw_dp_pT(self, p: float, T: float) -> float:
        r"""Cacluate derivative of loading wrt. pressure.

        This function calculates the derivative of loading w with respect to pressure depending on
        pressure p and temperature T for the adsorption isotherm that was initialized in the
        'working pair'-struct.

        Parameters
        ----------
        p: float
            Pressure in Pa.
        T: float
            Temperature in K.

        See Also
        --------
        Sur.dw_dp_pT:
            Same function but only valid for adsorption isotherms based on the suraface approach.
        SurRef.dw_dp_pTpsat:
            Same function but only valid for adsorption isotherms based on the suraface approach
            and also requiring the vapor pressure.
        Vol.dw_dp_pTpsatRho:
            Same function but only valid for adsorption isotherms based on the volumetric approach.

        Returns
        -------
        dw_dp: float
            Derivative of loading wrt. pressure in kg/kg/Pa.

        Notes
        -----
        This function is defined for all adsorption isotherms. If an adsorption isotherm requires
        properties of the refrigerant such as the vapor pressure, this function uses refrigerant
        functions provided by the SorpPropLib and selected during initialization of this class.

        If other refrigerant functions are required, please check out the specical functions that
        are not valid for all adsorption isotherms but allow to specify the refrigerant functions.

        History
        -------
        06/07/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ads_dw_dp_pT(c_double(p), c_double(T), self._wpair_c_struct)

    def dw_dT_pT(self, p: float, T: float) -> float:
        r"""Cacluate derivative of loading wrt. temperature.

        This function calculates the derivative of loading w with respect to temperature depending
        on pressure p and temperature T for the adsorption isotherm that was initialized in the
        'working pair'-struct.

        Parameters
        ----------
        p: float
            Pressure in Pa.
        T: float
            Temperature in K.

        See Also
        --------
        Sur.dw_dT_pT:
            Same function but only valid for adsorption isotherms based on the suraface approach.
        SurRef.dw_dT_pTpsat:
            Same function but only valid for adsorption isotherms based on the suraface approach
            and also requiring the vapor pressure.
        Vol.dw_dT_pTpsatRho:
            Same function but only valid for adsorption isotherms based on the volumetric approach.

        Returns
        -------
        dw_dT: float
            Derivative of loading wrt. temperature in kg/kg/K.

        Notes
        -----
        This function is defined for all adsorption isotherms. If an adsorption isotherm requires
        properties of the refrigerant such as the vapor pressure, this function uses refrigerant
        functions provided by the SorpPropLib and selected during initialization of this class.

        If other refrigerant functions are required, please check out the specical functions that
        are not valid for all adsorption isotherms but allow to specify the refrigerant functions.

        History
        -------
        06/07/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ads_dw_dT_pT(c_double(p), c_double(T), self._wpair_c_struct)

    def dp_dw_wT(self, w: float, T: float) -> float:
        r"""Cacluate derivative of pressure wrt. loading.

        This function calculates the derivative of pressure p with respect to loading depending on
        loading w and temperature T for the adsorption isotherm that was initialized in the
        'working pair'-struct.

        Parameters
        ----------
        w: float
            Loading in kg/kg.
        T: float
            Temperature in K.

        See Also
        --------
        Sur.dp_dw_wT:
            Same function but only valid for adsorption isotherms based on the suraface approach.
        SurRef.dp_dw_wTpsat:
            Same function but only valid for adsorption isotherms based on the suraface approach
            and also requiring the vapor pressure.
        Vol.dp_dw_wTpsatRho:
            Same function but only valid for adsorption isotherms based on the volumetric approach.

        Returns
        -------
        dp_dw: float
            Derivative of pressure wrt. loading in Pa.kg/kg.

        Notes
        -----
        This function is defined for all adsorption isotherms. If an adsorption isotherm requires
        properties of the refrigerant such as the vapor pressure, this function uses refrigerant
        functions provided by the SorpPropLib and selected during initialization of this class.

        If other refrigerant functions are required, please check out the specical functions that
        are not valid for all adsorption isotherms but allow to specify the refrigerant functions.

        History
        -------
        06/07/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ads_dp_dw_wT(c_double(w), c_double(T), self._wpair_c_struct)

    def dp_dT_wT(self, w: float, T: float) -> float:
        r"""Cacluate derivative of pressure wrt. temperature.

        This function calculates the derivative of pressure p with respect to temperature depending
        on loading w and temperature T for the adsorption isotherm that was initialized in the
        'working pair'-struct.

        Parameters
        ----------
        w: float
            Loading in kg/kg.
        T: float
            Temperature in K.

        See Also
        --------
        Sur.dp_dw_wT:
            Same function but only valid for adsorption isotherms based on the suraface approach.
        SurRef.dp_dw_wTpsat:
            Same function but only valid for adsorption isotherms based on the suraface approach
            and also requiring the vapor pressure.
        Vol.dp_dw_wTpsatRho:
            Same function but only valid for adsorption isotherms based on the volumetric approach.

        Returns
        -------
        dp_dw: float
            Derivative of pressure wrt. temperature in Pa/K.

        Notes
        -----
        This function is defined for all adsorption isotherms. If an adsorption isotherm requires
        properties of the refrigerant such as the vapor pressure, this function uses refrigerant
        functions provided by the SorpPropLib and selected during initialization of this class.

        If other refrigerant functions are required, please check out the specical functions that
        are not valid for all adsorption isotherms but allow to specify the refrigerant functions.

        History
        -------
        06/07/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ads_dp_dT_wT(c_double(w), c_double(T), self._wpair_c_struct)

    def piStar_pyxgTM(self, p: float, y: float, x: float,
                      gamma: float, T: float, M: float) -> float:
        r"""Cacluate reduced spreading pressure.

        This function calculates the reduced spreading pressure piStar depending on pressure p,
        molar fraction of refrigerant in vapor phase y, molar fraction of refrigerant in adsorbed
        phase x, activity coefficient of refrigerant in adsorbed phase gamma, temperature T, and
        molar mass of refrigerant M. The reduced spreading pressure is defined as follows:

            piStar = A * pi / (R * T * m_sorbent) = 1 / M * Integral_0^p0{w(p,T) / p * dp}

            where p0 = p_total*y / (gamma*x).

        Reduced spreading pressure may be needed to calculate adsorption equilibrium of mixtures,
        e.g., using the Ideal Adsorbed Solution Theory (IAST).

        Parameters
        ----------
        p: float
            Total pressure in Pa.
        y: float
            Molar fraction of refrigerant in vapor phase in mol/mol.
        x: float
            Molar fraction of refrigerant in adsorbed phase in mol/mol.
        gamma: float
            Activity coefficient of refrigerant in adsorbed phase.
        T: float
            Temperature in K.
        M: float
            Molar mass of refrigerant in kg/mol.

        See Also
        --------
        Sur.piStar_pyxgTM:
            Same function but only valid for adsorption isotherms based on the suraface approach.
        SurRef.piStar_pyxgTpsatM:
            Same function but only valid for adsorption isotherms based on the suraface approach
            and also requiring the vapor pressure.
        Vol.piStar_pyxgTpsatRhoM:
            Same function but only valid for adsorption isotherms based on the volumetric approach.

        Returns
        -------
        piStart: float
            Reduced spreading pressure in kg/mol.

        Notes
        -----
        This function often solves the integral using a numerical approach. Hence, accuracy may
        be to low for user.

        This function is defined for all adsorption isotherms. If an adsorption isotherm requires
        properties of the refrigerant such as the vapor pressure, this function uses refrigerant
        functions provided by the SorpPropLib and selected during initialization of this class.

        If other refrigerant functions are required, please check out the specical functions that
        are not valid for all adsorption isotherms but allow to specify the refrigerant functions.

        History
        -------
        06/07/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.ads_piStar_pyxgTM(c_double(p), c_double(y), c_double(x),
                                                     c_double(gamma), c_double(T),
                                                     c_double(M), self._wpair_c_struct)
