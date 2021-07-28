"""Package 'validity' cotains functions to get validity of function from the SorpPropLib.

Routine Listings
----------------
write_ref_validity:
    Generate formatted LaTeX string describing validity of adsorption isotherm functions.
write_ads_validity:
    Generate formatted LaTeX string describing validity of adsorption isotherm functions.
write_abs_validity:
    Generate formatted LaTeX string describing validity of absorption isotherm functions.

Revisions
---------
    Version     Date        Author              Changes
    1.0.0       2020-07-20  Mirko Engelpracht   First implementation
"""


# %% Definition of constants.
#
# Constant strings defining start and end of validity section
#
STR_VALIDITY_START = '\\textbf{Validity:}'                      + '\n'  + \
                     '\\newline'                                + '\n'  + \
                     'Equation is approximately valid for'

STR_VALIDITY_END = '\n' + '\\newline' + '\n\n'


# %% Define functions.
#
def write_ref_validity(REF: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Generate formatted LaTeX string describing validity of refrigerant functions.

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
        Formatted LaTeX string describing validity of refrigerant functions.

    """
    # Get correct contents from dict
    #
    STR_T_MIN = str(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_va_']['temperature-min'])
    STR_T_MAX = str(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_va_']['temperature-max'])

    # Write formatted LaTeX string
    #
    STR_LATEX = STR_VALIDITY_START                                                                     + \
                ' $' + STR_T_MIN + ' \\si{\\kelvin} \\leq T \\leq ' + STR_T_MAX  + ' \\si{\\kelvin}$.' + \
                STR_VALIDITY_END

    # Return formatted LaTeX string
    #
    return STR_LATEX

def write_ads_validity(ADS: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Generate formatted LaTeX string describing validity of adsorption isotherm functions.

    Parameters
    ----------
    ADS : dict
        Dict containing properties of current adsorption working pair extrated from data base 
        (i.e., JSON).
    EQU_NAME : str
        Name of refrigerant functions for which overview table shall be generated.
    EQU_ID : int
        ID of refrigerant functions for which overview table shall be generated.

    Returns
    -------
    str
        Formatted LaTeX string describing validity of refrigerant functions.

    """
    # Get correct contents from dict
    #
    STR_P_MIN = str(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_va_']['pressure-min'])
    STR_P_MAX = str(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_va_']['pressure-max'])
    STR_T_MIN = str(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_va_']['temperature-min'])
    STR_T_MAX = str(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_va_']['temperature-max'])
    STR_W_MIN = str(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_va_']['loading-min'])
    STR_W_MAX = str(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_va_']['loading-max'])

    # Write formatted LaTeX string
    #
    list_variable = []
    list_min = []
    list_max = []
    list_unit = []
    
    if STR_P_MIN:
        list_variable.append('p')
        list_min.append(STR_P_MIN)
        list_max.append(STR_P_MAX)
        list_unit.append('\\si{\\pascal}')
        
    if STR_T_MIN:
        list_variable.append('T')
        list_min.append(STR_T_MIN)
        list_max.append(STR_T_MAX)
        list_unit.append('\\si{\\kelvin}')
        
    if STR_W_MIN:
        list_variable.append('w')
        list_min.append(STR_W_MIN)
        list_max.append(STR_W_MAX)
        list_unit.append('\\si{\\kilogram\\per\\kilogram}')

    if len(list_min) > 0:
        STR_LATEX = STR_VALIDITY_START
        
        for ind, val in enumerate(list_min):
            if len(list_min) == 1:
                STR_LATEX += ' $' + val + ' ' + list_unit[ind] + ' \\leq ' + list_variable[ind]    + \
                             ' \\leq ' + list_max[ind] + ' ' + list_unit[ind] + '$.'  
                
            elif ind < len(list_min)-1:
                STR_LATEX += ' $' + val + ' ' + list_unit[ind] + ' \\leq ' + list_variable[ind]    + \
                             ' \\leq ' + list_max[ind] + ' ' + list_unit[ind] + '$, '                
            else:
                STR_LATEX += 'and $' + val + ' ' + list_unit[ind] + ' \\leq ' + list_variable[ind] + \
                             ' \\leq ' + list_max[ind] + ' ' + list_unit[ind] + '$.'  
                      
        STR_LATEX += STR_VALIDITY_END

    else:
        STR_LATEX = '\\textbf{Validity:}'                      + '\n'  + \
                    '\\newline'                                + '\n'  + \
                    'No data on validity available!'                   + \
                    STR_VALIDITY_END

    # Return formatted LaTeX string
    #
    return STR_LATEX

def write_abs_validity(ABS: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Generate formatted LaTeX string describing validity of absorption isotherm functions.

    Parameters
    ----------
    ABS : dict
        Dict containing properties of current absorption working pair extrated from data base 
        (i.e., JSON).
    EQU_NAME : str
        Name of refrigerant functions for which overview table shall be generated.
    EQU_ID : int
        ID of refrigerant functions for which overview table shall be generated.

    Returns
    -------
    str
        Formatted LaTeX string describing validity of refrigerant functions.

    """
    # Get correct contents from dict
    #
    STR_T_MIN = str(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_va_']['temperature-min'])
    STR_T_MAX = str(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_va_']['temperature-max'])

    # Write formatted LaTeX string
    #
    list_variable = []
    list_min = []
    list_max = []
    list_unit = []
        
    if STR_T_MIN:
        list_variable.append('T')
        list_min.append(STR_T_MIN)
        list_max.append(STR_T_MAX)
        list_unit.append('\\si{\\kelvin}')

    if len(list_min) > 0:
        STR_LATEX = STR_VALIDITY_START
        
        for ind, val in enumerate(list_min):
            if len(list_min) == 1:
                STR_LATEX += ' $' + val + ' ' + list_unit[ind] + ' \\leq ' + list_variable[ind]    + \
                             ' \\leq ' + list_max[ind] + ' ' + list_unit[ind] + '$.'  
                
            elif ind < len(list_min)-1:
                STR_LATEX += ' $' + val + ' ' + list_unit[ind] + ' \\leq ' + list_variable[ind]    + \
                             ' \\leq ' + list_max[ind] + ' ' + list_unit[ind] + '$, '                
            else:
                STR_LATEX += 'and $' + val + ' ' + list_unit[ind] + ' \\leq ' + list_variable[ind] + \
                             ' \\leq ' + list_max[ind] + ' ' + list_unit[ind] + '$.'  
                      
        STR_LATEX += STR_VALIDITY_END

    else:
        STR_LATEX = '\\textbf{Validity:}'                      + '\n'  + \
                    '\\newline'                                + '\n'  + \
                    'No data on validity available!'                   + \
                    STR_VALIDITY_END

    # Return formatted LaTeX string
    #
    return STR_LATEX

