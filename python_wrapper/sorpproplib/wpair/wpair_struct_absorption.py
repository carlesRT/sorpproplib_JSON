"""Module containing classes with calculation routines for absorption isotherms based on C-struct.

Extended Summary
----------------
This module cotains the classes 'WPairStructAbsorption', 'WPairStructAbsorptionConventional',
'WPairStructAbsorptionActivity', and 'WPairStructAbsorptionMixing'. All classes
contain function for absorption isotherms based on C-structs.

Revisions
---------
    Version     Date        Author              Changes
    1.0.0       2021-06-08  Mirko Engelpracht   First implementation
"""

# %% Import libraries.
#
from ctypes import byref, c_double, cdll

from .interface_cdll import WorkingPair
from .wpair_partial import  WPairCStruct


# %% Ignore pylint messages that are not reasonable.
#
# pylint: disable-msg=C0103
# pylint: disable-msg=R0903
# pylint: disable-msg=W0221


# %% Definition of class WPairStructAbsorptionConventional
#
class WPairStructAbsorptionConventional(WPairCStruct):
    r"""Class containing calculation routines for absorption working pairs.

    This class contains calculations routines to calculate equilibrium properties of an absorption
    working pair. In this class, the provided methods a available for absorption isotherms based on
    conventional approaches (e.g., Dühring or Antoine).

    Methods
    -------
    X_pT:
        Calculates concentration depending on pressure and temperature.
    p_XT:
        Calculates pressure depending on concentration and temperature.
    T_pX:
        Calculates temperature depending on pressure and concentration.
    dX_dp_pT:
        Calculates derivative of concentration wrt. pressure depending on pressure and temperature.
    dX_dT_pT:
        Calculates derivative of concentration wrt. temperature depending on pressure and
        temperature.
    dp_dX_XT:
        Calculates derivative of pressure wrt. concentration depending on concentration and
        temperature.
    dp_dT_XT:
        Calculates derivative of pressure wrt. temperature depending on concentration and
        temperature.

    History
    -------
    06/08/2021, by Mirko Engelpracht:
        First implementation.
    """

    def __init__(self, library: cdll, c_struct: WorkingPair):
        r"""Initialize class containing all functions of absorption working pairs.

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

    def X_pT(self, p: float, T: float) -> float:
        r"""Cacluate concentration.

        This function calculates the concentration X depending on pressure p and temperature T
        for the absorption isotherm that was initialized in the 'working pair'-struct.

        Parameters
        ----------
        p: float
            Pressure in Pa.
        T: float
            Temperature in K.

        Returns
        -------
        X: float
            Concentration in kg/kg.

        Notes
        -----
        This function is defined for absorption isotherms based on conventional approaches (e.g.,
        Dühring or Antoine). If function is not defined for selected isotherm, this function
        returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.abs_con_X_pT(c_double(p), c_double(T),
                                                self._wpair_c_struct)

    def p_XT(self, X: float, T: float) -> float:
        r"""Cacluate pressure.

        This function calculates the pressure p depending on concentration X and temperature T
        for the absorption isotherm that was initialized in the 'working pair'-struct.

        Parameters
        ----------
        X: float
            Concentration in kg/kg.
        T: float
            Temperature in K.

        Returns
        -------
        p: float
            Pressure in Pa.

        Notes
        -----
        This function is defined for absorption isotherms based on conventional approaches (e.g.,
        Dühring or Antoine). If function is not defined for selected isotherm, this function
        returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.abs_con_p_XT(c_double(X), c_double(T),
                                                self._wpair_c_struct)

    def T_pX(self, p: float, X: float) -> float:
        r"""Cacluate temperature.

        This function calculates the temperature T depending on pressure p and concentration X
        for the absorption isotherm that was initialized in the 'working pair'-struct.

        Parameters
        ----------
        p: float
            Pressure in Pa.
        X: float
            Concentration in kg/kg.

        Returns
        -------
        T: float
            Temperature in K.

        Notes
        -----
        This function is defined for absorption isotherms based on conventional approaches (e.g.,
        Dühring or Antoine). If function is not defined for selected isotherm, this function
        returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.abs_con_T_pX(c_double(p), c_double(X),
                                                self._wpair_c_struct)

    def dX_dp_pT(self, p: float, T: float) -> float:
        r"""Cacluate derivative of loading wrt. pressure.

        This function calculates the derivative of concentration X with respect to pressure
        depending on pressure p and temperature T for the absorption isotherm that was initialized
        in the 'working pair'-struct.

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
        This function is defined for absorption isotherms based on conventional approaches (e.g.,
        Dühring or Antoine). If function is not defined for selected isotherm, this function
        returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.abs_con_dX_dp_pT(c_double(p), c_double(T),
                                                    self._wpair_c_struct)

    def dX_dT_pT(self, p: float, T: float) -> float:
        r"""Cacluate derivative of loading wrt. temperature.

        This function calculates the derivative of concentration X with respect to temperature
        depending on pressure p and temperature T for the absorption isotherm that was initialized
        in the 'working pair'-struct.

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
        This function is defined for absorption isotherms based on conventional approaches (e.g.,
        Dühring or Antoine). If function is not defined for selected isotherm, this function
        returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.abs_con_dX_dT_pT(c_double(p), c_double(T),
                                                    self._wpair_c_struct)

    def dp_dX_XT(self, X: float, T: float) -> float:
        r"""Cacluate derivative of pressure wrt. loading.

        This function calculates the derivative of pressure p with respect to concentration
        depending on concentration X and temperature T for the absorption isotherm that was
        initialized in the 'working pair'-struct.

        Parameters
        ----------
        X: float
            Concentration in kg/kg.
        T: float
            Temperature in K.

        Returns
        -------
        dp_dw: float
            Derivative of pressure wrt. loading in Pa.kg/kg.

        Notes
        -----
        This function is defined for absorption isotherms based on conventional approaches (e.g.,
        Dühring or Antoine). If function is not defined for selected isotherm, this function
        returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.abs_con_dp_dX_XT(c_double(X), c_double(T),
                                                    self._wpair_c_struct)

    def dp_dT_XT(self, X: float, T: float) -> float:
        r"""Cacluate derivative of pressure wrt. temperature.

        This function calculates the derivative of pressure p with respect to temperature depending
        on concentration Xand temperature T for the absorption isotherm that was initialized in the
        'working pair'-struct.

        Parameters
        ----------
        X: float
            Concentration in kg/kg.
        T: float
            Temperature in K.

        Returns
        -------
        dp_dw: float
            Derivative of pressure wrt. temperature in Pa/K.

        Notes
        -----
        This function is defined for absorption isotherms based on conventional approaches (e.g.,
        Dühring or Antoine). If function is not defined for selected isotherm, this function
        returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.abs_con_dp_dT_XT(c_double(X), c_double(T),
                                                    self._wpair_c_struct)


# %% Definition of class WPairStructAbsorptionActivity
#
class WPairStructAbsorptionActivity(WPairCStruct):
    r"""Class containing calculation routines for absorption working pairs.

    This class contains calculations routines to calculate equilibrium properties of an absorption
    working pair. In this class, the provided methods a available for absorption isotherms based on
    activity coefficients.

    Methods
    -------
    abs_act_g_Txv1v2:
        Calculates activity coefficient depending on temperature, mole fraction in liquid phase,
        molar volume of first component, and molar volume of second component.
    abs_act_p_Txv1v2:
        Calculates pressure depending on temperature, mole fraction in liquid phase, molar volume
        of first component, and molar volume of second component.
    abs_act_x_pTv1v2:
        Calculates mole fraction in liquid phase depending on pressure, temperature, molar volume
        of first component, and molar volume of second component.
    abs_act_p_Txv1v2psat:
        Calculates pressure depending on temperature, mole fraction in liquid phase, molar volume
        of first component, molar volume of second component, and vapor pressure.
    abs_act_x_pTv1v2psat:
        Calculates mole fraction in liquid phase depending on pressure, temperature, molar volume
        of first component, and molar volume of second component, and vapor pressure.

    History
    -------
    06/08/2021, by Mirko Engelpracht:
        First implementation.
    """

    def __init__(self, library: cdll, c_struct: WorkingPair):
        r"""Initialize class containing all functions of absorption working pairs.

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

    def g_Txv1v2(self, T: float, x: float, v1: float, v2: float) -> float:
        r"""Cacluate activity coefficient.

        This function calculates the activity coefficient of first component g depending on
        temperature T, mole fraction in liquid phase x, molar volume of first component v1, and
        molar volume of second component v2.

        Parameters
        ----------
        T: float
            Temperature in K.
        x: float
            Mole fraction of first component in liquid phase in mol/mol.
        v1: float
            Molar volume of first component in m³/mol.
        v2: float
            Molar volume of second component in m³/mol.

        Returns
        -------
        g: float
            Activity coefficient of first component.

        Notes
        -----
        This function is defined for absorption isotherms based on activity coefficients. If
        function is not defined for selected isotherm, this function returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.abs_act_g_Txv1v2(c_double(T), c_double(x),
                                                    c_double(v1), c_double(v2),
                                                    self._wpair_c_struct)

    def p_Txv1v2(self, T: float, x: float, v1: float, v2: float) -> float:
        r"""Cacluate pressure.

        This function calculates the pressure p depending on temperature T, mole fraction in
        liquid phase x, molar volume of first component v1, and molar volume of second component
        v2.

        Parameters
        ----------
        T: float
            Temperature in K.
        x: float
            Mole fraction of first component in liquid phase in mol/mol.
        v1: float
            Molar volume of first component in m³/mol.
        v2: float
            Molar volume of second component in m³/mol.

        Returns
        -------
        p: float
            Pressure in Pa.

        Notes
        -----
        This function is defined for absorption isotherms based on activity coefficients. If
        function is not defined for selected isotherm, this function returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.abs_act_p_Txv1v2(c_double(T), c_double(x),
                                                    c_double(v1), c_double(v2),
                                                    self._wpair_c_struct)

    def x_pTv1v2(self, p: float, T: float, v1: float, v2: float) -> float:
        r"""Cacluate mole fraction in liquid phase.

        This function calculates the mole fraction in liquid phase x, pressure p, molar volume of
        first component v1, and molar volume of second component v2.

        Parameters
        ----------
        p: float
            Pressure in Pa.
        T: float
            Temperature in K.
        v1: float
            Molar volume of first component in m³/mol.
        v2: float
            Molar volume of second component in m³/mol.

        Returns
        -------
        x: float
            Mole fraction of first component in liquid phase in mol/mol.

        Notes
        -----
        This function is defined for absorption isotherms based on activity coefficients. If
        function is not defined for selected isotherm, this function returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.abs_act_x_pTv1v2(c_double(p), c_double(T),
                                                    c_double(v1), c_double(v2),
                                                    self._wpair_c_struct)

    def p_Txv1v2psat(self, T: float, x: float, v1: float, v2: float, p_sat: float) -> float:
        r"""Cacluate pressure.

        This function calculates the pressure p depending on temperature T, mole fraction in
        liquid phase x, molar volume of first component v1, molar volume of second component v2,
        and vapor pressure p_sat.

        Parameters
        ----------
        T: float
            Temperature in K.
        x: float
            Mole fraction of first component in liquid phase in mol/mol.
        v1: float
            Molar volume of first component in m³/mol.
        v2: float
            Molar volume of second component in m³/mol.
        p_sat: float
            Vapor pressure in Pa.

        Returns
        -------
        p: float
            Pressure in Pa.

        Notes
        -----
        This function is defined for absorption isotherms based on activity coefficients. If
        function is not defined for selected isotherm, this function returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.abs_act_p_Txv1v2psat(c_double(T), c_double(x),
                                                        c_double(v1), c_double(v2),
                                                        c_double(p_sat), self._wpair_c_struct)

    def x_pTv1v2psat(self, p: float, T: float, v1: float, v2: float, p_sat: float) -> float:
        r"""Cacluate mole fraction in liquid phase.

        This function calculates the mole fraction in liquid phase x, pressure p, molar volume of
        first component v1, molar volume of second component v2, and vapor pressure p_sat.

        Parameters
        ----------
        p: float
            Pressure in Pa.
        T: float
            Temperature in K.
        v1: float
            Molar volume of first component in m³/mol.
        v2: float
            Molar volume of second component in m³/mol.
        p_sat: float
            Vapor pressure in Pa.


        Returns
        -------
        x: float
            Mole fraction of first component in liquid phase in mol/mol.

        Notes
        -----
        This function is defined for absorption isotherms based on activity coefficients. If
        function is not defined for selected isotherm, this function returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.abs_act_x_pTv1v2psat(c_double(p), c_double(T),
                                                        c_double(v1), c_double(v2),
                                                        c_double(p_sat), self._wpair_c_struct)


# %% Definition of class WPairStructAbsorptionMixing
#
class WPairStructAbsorptionMixing(WPairCStruct):
    r"""Class containing calculation routines for absorption working pairs.

    This class contains calculations routines to calculate equilibrium properties of an absorption
    working pair. In this class, the provided methods a available for absorption isotherms based on
    mixing rules of cubic equations of state.

    Methods
    -------
    x_pT:
        Calculates mole fraction in liquid phase depending on pressure and temperature.
    p_Tx:
        Calculates pressure depending on temperature and mole fraction in liquid phase.
    T_px:
        Calculates temperature depending on pressure and mole fraction in liquid phase.
    dp_dx_Tx:
        Calculates derivative of pressure wrt. mole fraction in liquid phase depending on
        temperature and mole fraction in liquid phase.
    dp_dT_Tx:
        Calculates derivative of pressure wrt. temperature depending on temperature and mole
        fraction in liquid phase.

    History
    -------
    06/08/2021, by Mirko Engelpracht:
        First implementation.
    """

    def __init__(self, library: cdll, c_struct: WorkingPair):
        r"""Initialize class containing all functions of absorption working pairs.

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

    def x_pT(self, p: float, T: float) -> float:
        r"""Cacluate mole fraction in liquid phase.

        This function calculates the mole fraction in liquid phase x depending on pressure p and
        temperature T.

        Parameters
        ----------
        p: float
            Pressure in Pa.
        T: float
            Temperature in K.

        Returns
        -------
        x: float
            Mole fraction of first component in liquid phase in mol/mol.
        y1: float
            Mole fraction of first component in vapor phase in mol/mol.
        y2: float
            Mole fraction of second component in vapor phase in mol/mol.

        Notes
        -----
        This function is defined for absorption isotherms based on mixing rules of cubic
        equations of state. If function is not defined for selected isotherm, this function
        returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        # Calculate equilibrium
        #
        y1 = c_double()
        y2 = c_double()
        x = self._sorp_prop_lib.abs_mix_x_pT(byref(y1), byref(y2),
                                             c_double(p), c_double(T), self._wpair_c_struct)

        # Return results
        #
        return (x, y1, y2)

    def p_Tx(self, T: float, x: float) -> float:
        r"""Cacluate pressure.

        This function calculates the pressure p depending on temperature T and mole fraction in
        liquid phase x.

        Parameters
        ----------
        T: float
            Temperature in K.
        x: float
            Mole fraction of first component in liquid phase in mol/mol.

        Returns
        -------
        p: float
            Pressure in Pa.
        y1: float
            Mole fraction of first component in vapor phase in mol/mol.
        y2: float
            Mole fraction of second component in vapor phase in mol/mol.

        Notes
        -----
        This function is defined for absorption isotherms based on mixing rules of cubic
        equations of state. If function is not defined for selected isotherm, this function
        returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        # Calculate equilibrium
        #
        y1 = c_double()
        y2 = c_double()
        p = self._sorp_prop_lib.abs_mix_p_Tx(byref(y1), byref(y2),
                                             c_double(T), c_double(x), self._wpair_c_struct)

        # Return results
        #
        return (p, y1, y2)

    def T_px(self, p: float, x: float) -> float:
        r"""Cacluate temperature.

        This function calculates the temperature T depending on on pressure p and mole fraction
        in liquid phase x.

        Parameters
        ----------
        p: float
            Pressure in Pa.
        x: float
            Mole fraction of first component in liquid phase in mol/mol.

        Returns
        -------
        T: float
            Temperature in K.
        y1: float
            Mole fraction of first component in vapor phase in mol/mol.
        y2: float
            Mole fraction of second component in vapor phase in mol/mol.

        Notes
        -----
        This function is defined for absorption isotherms based on mixing rules of cubic
        equations of state. If function is not defined for selected isotherm, this function
        returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        # Calculate equilibrium
        #
        y1 = c_double()
        y2 = c_double()
        T = self._sorp_prop_lib.abs_mix_T_px(byref(y1), byref(y2),
                                             c_double(p), c_double(x), self._wpair_c_struct)

        # Return results
        #
        return (T, y1, y2)

    def dp_dx_Tx(self, T: float, x: float) -> float:
        r"""Cacluate derivative of pressure wrt. mole fraction in liquid phase.

        This function calculates the derivative of pressure p wrt. mole fraction in liquid phase
        depending on temperature T and mole fraction in liquid phase x.

        Parameters
        ----------
        T: float
            Temperature in K.
        x: float
            Mole fraction of first component in liquid phase in mol/mol.

        Returns
        -------
        dp_dx: float
            Pressure in Pa.mol/mol.

        Notes
        -----
        This function is defined for absorption isotherms based on mixing rules of cubic
        equations of state. If function is not defined for selected isotherm, this function
        returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.abs_mix_dp_dx_Tx(c_double(T), c_double(x), self._wpair_c_struct)

    def dp_dT_Tx(self, T: float, x: float) -> float:
        r"""Cacluate derivative of pressure wrt. temperature.

        This function calculates the derivative of pressure p wrt. temperature depending on
        temperature T and mole fraction in liquid phase x.

        Parameters
        ----------
        T: float
            Temperature in K.
        x: float
            Mole fraction of first component in liquid phase in mol/mol.

        Returns
        -------
        dp_dT: float
            Pressure in Pa/K.

        Notes
        -----
        This function is defined for absorption isotherms based on mixing rules of cubic
        equations of state. If function is not defined for selected isotherm, this function
        returns -1.

        History
        -------
        06/08/2021, by Mirko Engelpracht:
            First implementation.
        """
        return self._sorp_prop_lib.abs_mix_dp_dT_Tx(c_double(T), c_double(x), self._wpair_c_struct)


# %% Definition of class WPairStructAbsorption
#
class WPairStructAbsorption(WPairCStruct):
    r"""Class containing calculation routines for absorption working pairs.

    This class contains calculations routines to calculate equilibrium properties of an absorption
    working pair. Three different types of absorption isotherms are implemented in SorpPropLib,
    which coressponding functions can be executed via the sub-classes 'Con', 'Act' and 'Mix'.

    Attributes
    ----------
    Con: class WPairStructAbsorptionConventional
        Class containing functions to calculate equilibrium properties of ansorption isotherms
        based on conventional approaches like Dühring or Antoine.
    Act: class WPairStructAbsorptionActivity
        Class containing functions to calculate equilibrium properties of ansorption isotherms
        based on activity coefficients.
    Mix: class WPairStructAbsorptionMixing
        Class containing functions to calculate equilibrium properties of absorption isotherms
        based on mixing rules of cubic equation of states.

    History
    -------
    06/08/2021, by Mirko Engelpracht:
        First implementation.
    """

    def __init__(self, library: cdll, c_struct: WorkingPair):
        r"""Initialize class containing all functions of absorption working pairs.

        This function initializes the working pair class and sets references to the library
        containing all functions of the SorpPropLib.DLL and to the c_struct containing a pointer
        to a specifc working pair. Furthermore, it initializes sub-classes containing special
        functions for absoption isotherms based on conventional approaches, activity coefficients,
        and mixing rules.

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
        # Initialize sub-classes containing specialised classes to calculate equilibrium
        # properties
        #
        super().__init__(library, c_struct)

        self.Con = WPairStructAbsorptionConventional(library, c_struct)
        self.Act = WPairStructAbsorptionActivity(library, c_struct)
        self.Mix = WPairStructAbsorptionMixing(library, c_struct)
