"""Package 'calc' contains function to calculate equilibrium properties of a working pair.

Extended Summary
----------------
This module is the main interface to the "Sorption Properties Library". It cotains functions to a)
load the SorpPropLib.DLL depending on the platform and to b) map all functions of the DLL to
coresponding Python functions. Moreover, one class is provided to interact with
the SorpPropLib.DLL: The class 'wpair_struct' contains functions that exploit a C-struct of the
working pair and allows very fast calculations of equilibrium properties.


Class Listings
--------------
WPair:
    Base class for all working pairs
WPairCStruct:
    Base class for all working pairs based on a C-struct.
WPairStruct:
    Class allowing to execute isotherm and refrigerant functions based on a C-struct. These
    classes are: WPairStructAbsorptionConventional, WPairStructAbsorptionActivity, 
    WPairStructAbsorptionMixing, WPairStructAdsorptionSurface, 
    WPairStructAdsorptionSurfaceRefrigerant, WPairStructAdsorptionVolumetric,
    WPairStructAdsorption, and WPairStructRefrigerant.


Routine Listings
----------------
wrap_dll_function:
    Set up functions of the DLL, i.e., set up argument and return types.
load_dll:
    Load SorpPropLib.DLL and set up its functions.


Revisions
---------
    Version     Date        Author              Changes
    1.0.0       2020-05-22  Mirko Engelpracht   First implementation
"""

# %% Definition of local references.
#
__all__ = []
