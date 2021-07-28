"""Package 'SorpPropLib' cotains several sub-packages to use SorpPropLib.

Extended Summary
----------------
This module is an interface to the "Sorption Properties Library" written in C using a JSON
database. Using the SorpPropLib, the user can easily search for different working pairs inserted
into the database and calculate its equilibrium properties. Moreover, various plotting
capabilities are provided.


Module Listings
---------------
data:
    Routines to generate JSON database from *.-csv-files.
documentation:
    Routines to generate documentation file.
plot:
    Routines to plot equilibrium properties.
wpair:
    Classes for main interface to SorpPropLib (i.e., C code)


Revisions
---------
    Version     Date        Author              Changes
    1.0.0       2020-05-22  Mirko Engelpracht   First implementation
"""

# %% Definition of local references.
#
__all__ = []
