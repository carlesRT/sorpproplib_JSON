"""Module containing class 'WPair' and 'WPairCStruct'.

Extended Summary
----------------
This module cotains the class 'WPair' that contains basic information of a working pair. This
class is used as base class to derive more detailed classes that implement functions to calculate
sorption equilibrium properties.

This module also contains class 'WPairCStruct' that contains basic information of a working pair
based on a C-struct.  This class is used as base class to derive more detailed classes that
implement functions to calculate sorption equilibrium properties using a C-struct.

Revisions
---------
    Version     Date        Author              Changes
    1.0.0       2020-05-22  Mirko Engelpracht   First implementation
"""

# %% Import libraries.
#
import os

from ctypes import c_int, cdll
from .interface_cdll import load_dll, WorkingPair


# %% Definition of class 'WPair'.
#
class WPair:
    r"""Class containing base information of a working pair.

    This class cotains base information of a working pair. These information is the sorbent, the
    subtype of the sorbent, and the refrigerant; and the calculation approaches of the
    isotherm, the vapor pressure, and the saturated liquid density.

    Attributes
    ----------
    wpair : dict
        Dict containing keys 'sorbent', 'subtype', and 'refrigerant'.
    wpair_approach: dict
        Dict containing keys 'isotherm', 'id_isotherm', 'vapor_pressure', 'id_vapor_pressure',
        'sat_liq_density', and 'id_sat_liq_density'.

    Methods
    -------
    update_wpair:
        Allows to re-initialize the working pair.
    show_wpair:
        Return dict containing base information of the working pair.

    See Also
    --------
    WPairStruct:
        Derived class based on C-structs that can be used for fast equilibrium calculations.
    WPairDirect:
        Derived class that is slower than class 'WPairStruct' and should only be used to perform
        a few calculations, e.g., during intilization routines of complex problems.

    Notes
    -----
    Since this class only contains base information of a working pair, further classes should be
    derived from WPair and implementing routines to calculate, for example, sorption
    equilibrium properties.

    History
    -------
    05/22/2020, by Mirko Engelpracht:
        First implementation.
    """

    def __init__(self, wpair: dict, wpair_approach: dict, path_dll: str, path_json: str):
        r"""Initialize working pair.

        This function initializes the working pair class and sets up basic of information of the
        working pair such as the name of the sorbent or the isotherm type.  Moreover, it loads the
        DLL of the SorpPropLib and sets up all functions of the DLL.

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

        Both parameters wpair and wpair_approach are shallowly copied.

        History
        -------
        05/22/2020, by Mirko Engelpracht:
            First implementation.
        """
        # Definition of attributes defining the working pair
        #
        self.wpair = wpair.copy()
        self.wpair_approach = wpair_approach.copy()

        # Check if IDs of calculation approaches are given by input
        # If not, append default value
        #
        if "id_isotherm" not in self.wpair_approach:
            self.wpair_approach.update({"id_isotherm": 1})
        if "id_vapor_pressure" not in self.wpair_approach:
            self.wpair_approach.update({"id_vapor_pressure": 1})
        if "id_sat_liq_density" not in self.wpair_approach:
            self.wpair_approach.update({"id_sat_liq_density": 1})

        # Loading and setting up the DLL of the SorpPropLib
        #
        try:
            self.__error_library = False
            self.__sorp_prop_lib = load_dll(path_dll)

        except (AttributeError, OSError):
            self.__error_library = True
            raise UserWarning("Either DLL of SorpPropLib does not exist or DLL does not contain " +
                              "specified functions. Thus, cannot create class 'WPairStruct'!")

        # Checking if JSON data base exists
        #
        if os.path.exists(path_json):
            self.__error_library = False

        else:
            self.__error_library = True
            raise FileNotFoundError("JSON database specified by path '{}'".format(path_json) +
                                    " does not exis. Thus, cannot create class 'WPairStruct'!")

        # Set C-dict required to call functions from DLL
        #
        self.__wpair_c_dict = {"path_json": path_json,
                               "as": self.wpair["sorbent"].encode("UTF-8"),
                               "st": self.wpair["subtype"].encode("UTF-8"),
                               "rf": self.wpair["refrigerant"].encode("UTF-8"),
                               "iso": self.wpair_approach["isotherm"].encode("UTF-8"),
                               "p_sat": self.wpair_approach["vapor_pressure"].encode("UTF-8"),
                               "rho_l": self.wpair_approach["sat_liq_density"].encode("UTF-8"),
                               "id_iso": c_int(self.wpair_approach["id_isotherm"]),
                               "id_p_sat": c_int(self.wpair_approach["id_vapor_pressure"]),
                               "id_rho_l": c_int(self.wpair_approach["id_sat_liq_density"])}

    def show_wpair(self, flag_print: bool = False) -> dict:
        r"""Return dict containing base information of the working pair.

        This function loads the SorpPropLib.DLL from the specified path and
        sets up the DLL's functions (i.e., argument types and return types).

        Parameters
        ----------
        flag_print : bool
            If true, base information of working pair is printed.

        Returns
        -------
        wpair : dict
            Dict containing all base information of the working pair.

        History
        -------
        04/14/2020, by Mirko Engelpracht:
            First implementation.
        """
        # Combine dicts containing base information of working pair
        #
        tmp_dict = self.wpair.copy()
        tmp_dict.update(self.wpair_approach)

        # Check if base information of working pair shall be printed
        #
        if flag_print:
            # Identify maximum lengths of keys and values
            #
            len_key = max(max([len(val) for val in tmp_dict.keys()]), len("Category"))
            len_val = max(max([len(str(val)) for val in tmp_dict.values()]), len("Value"))
            width = max(len_key + len_val + 3, len("Specified Working Pair"))

            # Format and print data
            #
            s_f = "| {:<{w1}} | {:<{w2}} |"
            s_fnl = "{:-^{width}}".format("-", width=width+4)

            print("\n" + s_fnl)
            print("| {:^{width}} |".format("Specified Working Pair", width=width))
            print(s_fnl)
            print(s_f.format("Category", "Value", w1=len_key, w2=len_val))
            print(s_fnl)
            for val in tmp_dict.keys():
                print(s_f.format(val.capitalize(),
                                 str(tmp_dict.get(val)),
                                 w1=len_key, w2=len_val))
                print(s_fnl)

        return tmp_dict


# %% Definition of class WPairCStruct
#
class WPairCStruct:
    r"""Class containing base information of a working pair based on a C-struct.

    This class cotains base information of a working pair based on a C-struct. These information
    is a reference to the library containing functions of the SorpPropLib.DLL and a pointer to
    a C-struct containing all data required by the functions of the SorpPropLib.DLL

    Notes
    -----
    This class serves as base class for all classes of working pairs based on a C-struct. Hence,
    this class should not be used as standalone class. Instead, sub-classes derived from this
    base class should be used for any calculations.

    See Also
    --------
    interface_cdll:
        File containing all classes based on C-structs.

    History
    -------
    05/23/2020, by Mirko Engelpracht:
        First implementation.
    """

    def __init__(self, library: cdll, c_struct: WorkingPair):
        r"""Initialize working pair based on a C-struct.

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
        05/23/2020, by Mirko Engelpracht:
            First implementation.
        """
        # Save reference to C-struct and to library
        #
        self._sorp_prop_lib = library
        self._wpair_c_struct = c_struct
