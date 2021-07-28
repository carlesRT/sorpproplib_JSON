"""Module containing class 'WpairStruct'.

Extended Summary
----------------
This module cotains the class 'wpair_struct' that contains both isotherm and refrigerant
functions based on C-structs.

Revisions
---------
    Version     Date        Author              Changes
    1.0.0       2020-05-22  Mirko Engelpracht   First implementation
"""

# %% Import libraries.
#
from .wpair_struct_absorption import WPairStructAbsorption
from .wpair_struct_adsorption import WPairStructAdsorption
from .wpair_struct_refrigerant import WPairStructRefrigerant
from .wpair_partial import WPair


# %% Ignore pylint messages that are not reasonable.
#
# pylint: disable-msg=C0103
# pylint: disable-msg=R0903
# pylint: disable-msg=W0221


# %% Definition of class WPairStruct
#
class WPairStruct(WPair):
    r"""Class allowing equilibrium calculations of a working pair.

    This class cotains base information of a working pair. These information is the sorbent, the
    subtype of the sorbent, and the refrigerant; and the calculation approaches of the
    isotherm, the vapor pressure, and the saturated liquid density. Furthermore, this class
    contains the sub-classes 'Ads', 'Abs', and 'Eef' that contain function to calculate equilibrium
    properties of adsorption working pairs, equilibrium properties of absorption working pairs, and
    saturation properties of refrigerants.

    Attributes
    ----------
    wpair: dict
        Dict containing keys 'sorbent', 'subtype', and 'refrigerant'.
    wpair_approach: dict
        Dict containing keys 'isotherm', 'id_isotherm', 'vapor_pressure', 'id_vapor_pressure',
        'sat_liq_density', and 'id_sat_liq_density'.

    Ads: class WPairStructAdsorption
        Class containing functions to calculate equilibrium properties of adsorption isotherms.
    Abs: class WPairStructAbsorption
        Class containing functions to calculate equilibrium properties of absorption isotherms.
    Ref: class WPairStructRefrigerant
        Class containing functions to calculate saturation properties of refrigerants.

    Methods
    -------
    update_wpair:
        Allows to re-initialize the working pair.
    show_wpair:
        Return dict containing base information of the working pair.

    Raises
    ------
    UserWarning:
        If DLL given by parameter 'path_dll' does not exist, its functions cannot be setted up, or
        the given working pair is not saved into JSON data base.
    FileNotFoundError:
        If JSON data base given by parameter 'path_json' does not exist.

    History
    -------
    05/22/2020, by Mirko Engelpracht:
        First implementation.
    """

    def __init__(self, wpair: dict, wpair_approach: dict, path_dll: str, path_json: str):
        r"""Initialize working pair.

        This function initializes the working pair class and sets up basic of information of the
        working pair such as the name of the sorbent or the isotherm type. Moreover, it loads the
        DLL of the SorpPropLib, sets up all functions of the DLL, and initializes the
        'working pair'-struct required to execute functions of the DLL.

        Parameters
        ----------
        wpair : dict
            Dict containing keys 'sorbent', 'subtype', and 'refrigerant'.
        wpair_approach: dict
            Dict containing keys 'isotherm', 'id_isotherm', 'vapor_pressure', 'id_vapor_pressure',
            'sat_liq_density', and 'id_sat_liq_density'.
        path_dll: str
            Path indicting location of the SorpPropLib.DLL.
        path_json: str
            Path indicting location of the JSON database.

        Notes
        -----
        For the parameter 'wpair_approach', the keys describing the IDs of the calculation
        approaches are optional. If an ID is not given, the function assumes 1 for the ID.

        Both parameters are shallowly copied.

        Raises
        ------
        UserWarning:
            If SorpPropLib.DLL given by parameter 'path_dll' does not exist, some functions of it
            does not exist, or the given working pair is not saved into JSON data base.
        FileNotFoundError:
            If JSON data base given by parameter 'path_json' does not exist.

        History
        -------
        05/22/2020, by Mirko Engelpracht:
            First implementation.
        """
        # Definition of attributes defining the working pair
        #
        super().__init__(wpair, wpair_approach, path_dll, path_json)

        # Initializing a 'working pair'-struct
        #
        self.__wpair_c_struct = \
            self._WPair__sorp_prop_lib.newWorkingPair(self._WPair__wpair_c_dict["path_json"],
                                                      self._WPair__wpair_c_dict["as"],
                                                      self._WPair__wpair_c_dict["st"],
                                                      self._WPair__wpair_c_dict["rf"],
                                                      self._WPair__wpair_c_dict["iso"],
                                                      self._WPair__wpair_c_dict["id_iso"],
                                                      self._WPair__wpair_c_dict["p_sat"],
                                                      self._WPair__wpair_c_dict["id_p_sat"],
                                                      self._WPair__wpair_c_dict["rho_l"],
                                                      self._WPair__wpair_c_dict["id_rho_l"])

        if not bool(self.__wpair_c_struct):
            raise UserWarning("Specified working pair does not exists in the JSON data base." +
                              "Thus, creation of class 'WPairStruct' is not reasonable!")

        # Initialize sub-classes required for any calculation
        #
        self.Ads = WPairStructAdsorption(self._WPair__sorp_prop_lib, self.__wpair_c_struct)
        self.Abs = WPairStructAbsorption(self._WPair__sorp_prop_lib, self.__wpair_c_struct)
        self.Ref = WPairStructRefrigerant(self._WPair__sorp_prop_lib, self.__wpair_c_struct)

    def __del__(self):
        r"""Destroys class.

        This function is mandantory to free allocated memory of 'working pair'-struct if it exists.

        History
        -------
        05/22/2020, by Mirko Engelpracht:
            First implementation.
        """
        # Check if 'working pair'-struct exists and need to be freed
        #
        if not self._WPair__error_library:
            self._WPair__sorp_prop_lib.delWorkingPair(self.__wpair_c_struct)

    def update_wpair(self, wpair: dict, wpair_approach: dict, path_dll: str, path_json: str):
        r"""Re-initialize working pair.

        This function re-initializes the working pair class and sets up basic of information of the
        working pair such as the name of the sorbent or the isotherm type. Moreover, it loads the
        DLL of the SorpPropLib, sets up all functions of the DLL, and initializes the
        'working pair'-struct required to execute functions of the DLL.

        Parameters
        ----------
        wpair : dict
            Dict containing keys 'sorbent', 'subtype', and 'refrigerant'.
        wpair_approach: dict
            Dict containing keys 'isotherm', 'id_isotherm', 'vapor_pressure', 'id_vapor_pressure',
            'sat_liq_density', and 'id_sat_liq_density'.
        path_dll: str
            Path indicting location of the SorpPropLib.DLL.
        path_json: str
            Path indicting location of the JSON database.

        Notes
        -----
        For the parameter 'wpair_approach', the keys describing the IDs of the calculation
        approaches are optional. If an ID is not given, the function assumes 1 for the ID.

        Both parameters are shallowly copied.

        Raises
        ------
        UserWarning:
            If SorpPropLib.DLL given by parameter 'path_dll' does not exist, some functions of it
            does not exist, or the given working pair is not saved into JSON data base.
        FileNotFoundError:
            If JSON data base given by parameter 'path_json' does not exist.

        History
        -------
        05/22/2020, by Mirko Engelpracht:
            First implementation.
        """
        self.__init__(wpair, wpair_approach, path_dll, path_json)
