"""Package 'overview' cotains functions to generate overview tables of the SorpPropLib.

Routine Listings
----------------
write_ref_overview:
    Generates overview table for refrigerants.
write_ads_overview:
    Generates overview table for equation approaches of adsorption working pairs.
write_abs_overview:
    Generates overview table for equation approaches of absorption working pairs.

Revisions
---------
    Version     Date        Author              Changes
    1.0.0       2020-07-20  Mirko Engelpracht   First implementation
"""


# %% Import relevant modules.
#
from .utilities import str_latex


# %% Definition of constants.
#
STR_OVERVIEW_START = '\\begin{tabular}[l]{|lp{11.5cm}|}'    + '\n'      + \
                     '\\hline'                              + '\n'      + \
                     '\\addlinespace'                       + '\n\n'

STR_OVERVIEW_END = '\\addlinespace'     + '\n'      + \
                   '\\hline'            + '\n'      + \
                   '\\end{tabular}'     + '\n'      + \
                   '\\newline'          + '\n\n'


# %% Define functions.
#
def write_ref_overview(REF: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Generate overview table for equation approaches of refrigerants.

    Parameters
    ----------
    REF : dict
        Dict containing properties of current refrigerant extrated from data base (i.e., JSON).
    EQU_NAME : str
        Name of refrigerant functions for which overview table shall be generated.
    EQU_ID : int
        ID of refrigerant functions for which overview table shall be generated.

    Returns
    -------
    str
        Formatted LaTeX string with overview table.

    """
    # Get correct contents from dict
    #
    STR_REFRIGERANT = REF['k']['_rf_']
    STR_EQUATION = str_latex(EQU_NAME)
    STR_EQU = str(EQU_ID+1)
    STR_REFERENCE = str_latex(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_c_'])
    STR_NOTE = 'None' if not REF['v']['_ep_'][EQU_NAME][EQU_ID]['_n_'] else \
        str_latex(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_n_'])

    # Write formatted LaTeX table
    #
    STR_LATEX = STR_OVERVIEW_START                                                     + \
            	'\\textbf{Name:} & '       + STR_REFRIGERANT   + ' \\\\'   + '\n'      + \
            	'\\textbf{Equation:} & '   + STR_EQUATION      + ' \\\\'   + '\n'      + \
            	'\\textbf{ID:} & '         + STR_EQU           + ' \\\\'   + '\n'      + \
            	'\\textbf{Reference:} & '  + STR_REFERENCE     + ' \\\\'   + '\n'      + \
            	'\\textbf{Comment:} & '    + STR_NOTE          + ' \\\\'   + '\n\n'    + \
                STR_OVERVIEW_END

    # Return formatted LaTeX table
    #
    return STR_LATEX

def write_ads_overview(ADS: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Generate overview table for equation approaches of adsorption working pairs.

    Parameters
    ----------
    ADS : dict
        Dict containing properties of current adsorption working pair extrated from data
        base (i.e., JSON).
    EQU_NAME : str
        Name of isotherm functions for which overview table shall be generated.
    EQU_ID : int
        ID of isotherm functions for which overview table shall be generated.

    Returns
    -------
    str
        Formatted LaTeX string with overview table.

    """
    # Get correct contents from dict
    #
    STR_SORBENT = ADS['k']['_as_']
    STR_SUBTYPE = ADS['k']['_st_'].replace('%', '\%')
    STR_REFRIGERANT = ADS['k']['_rf_']
    STR_EQUATION = str_latex(EQU_NAME)
    STR_EQU = str(EQU_ID+1)
    STR_REFERENCE = str_latex(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_c_'])
    STR_NOTE = 'None' if not ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_n_'] else \
        str_latex(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_n_'])

    # Write formatted LaTeX table
    #
    STR_LATEX = STR_OVERVIEW_START                                                     + \
            	'\\textbf{Sorbent:} & '    + STR_SORBENT       + ' \\\\'   + '\n'      + \
            	'\\textbf{Subtype:} & '    + STR_SUBTYPE       + ' \\\\'   + '\n'      + \
            	'\\textbf{Refrigerant:} & '+ STR_REFRIGERANT   + ' \\\\'   + '\n'      + \
            	'\\textbf{Equation:} & '   + STR_EQUATION      + ' \\\\'   + '\n'      + \
            	'\\textbf{ID:} & '         + STR_EQU           + ' \\\\'   + '\n'      + \
            	'\\textbf{Reference:} & '  + STR_REFERENCE     + ' \\\\'   + '\n'      + \
            	'\\textbf{Comment:} & '    + STR_NOTE          + ' \\\\'   + '\n\n'    + \
                STR_OVERVIEW_END

    # Return formatted LaTeX table
    #
    return STR_LATEX

def write_abs_overview(ABS: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Generate overview table for equation approaches of absorption working pairs.

    Parameters
    ----------
    ABS : dict
        Dict containing properties of current absorption working pair extrated from data
        base (i.e., JSON).
    EQU_NAME : str
        Name of isotherm functions for which overview table shall be generated.
    EQU_ID : int
        ID of isotherm functions for which overview table shall be generated.

    Returns
    -------
    str
        Formatted LaTeX string with overview table.

    """
    return write_ads_overview(ABS, EQU_NAME, EQU_ID)
