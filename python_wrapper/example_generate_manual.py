# -*- coding: utf-8 -*-
"""Script 'example_generate_manual'.

This script is an example showing how to generate the manual of the SorpPropLib. Here, LaTeX files
are automatically created and saved at '.\sorpproplib\doc\*'. Then, the main doxument 'manual.tex'
stored at '.\sorpproplib\doc\' must be executed with a LaTeX editor to create the manual.

Revisions:
----------
    Version     Date        Author              Changes
    1.0.0       2021-07-17  Mirko Engelpracht   First implementation
"""


# %% Import relevant modules.
#
import os
import struct
import sys

import sorpproplib.doc.manual as manual


# %% Setup.
#
# Set up paths depending on plattform and architecture
#
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

PATH_WRAPPER = os.path.join(os.getcwd().split("sorpproplib_JSON")[0],
                            'sorpproplib_JSON', 'python_wrapper')
PATH_DB = os.path.join(PATH_WRAPPER, 'sorpproplib',
                       'data', 'JSON', 'sorpproplib.json').encode()
PATH_SORPPROPLIB = os.path.join(PATH_WRAPPER,  'sorpproplib',
                                'data', REQUIRED_DLL, 'libsorpPropLib.dll')


# %% Generate manual.
#
CONTENT = manual.get_typedependent_content_lists(PATH_DB)

manual.write_ref_all(CONTENT, PATH_WRAPPER, PATH_SORPPROPLIB,PATH_DB)
manual.write_ads_all(CONTENT, PATH_WRAPPER, PATH_SORPPROPLIB,PATH_DB)
manual.write_abs_all(CONTENT, PATH_WRAPPER, PATH_SORPPROPLIB,PATH_DB)
