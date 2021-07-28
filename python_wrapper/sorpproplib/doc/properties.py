"""Package 'properties' cotains functions to generate property tables of the SorpPropLib.

Routine Listings
----------------
write_ads_property:
    Generates property table for equation approaches of adsorption working pairs.

Revisions
---------
    Version     Date        Author              Changes
    1.0.0       2020-07-21  Mirko Engelpracht   First implementation
"""


# %% Definition of constants.
#
STR_PROPERTY_START = '\\textbf{Properties of sorbent:}'                                 + '\n'  + \
                     '\\newline'                                                        + '\n'  + \
                     '%'                                                                + '\n'

STR_PROPERTY_TABLE_START = '\\begin{longtable}[l]{lll}'                                 + '\n'  + \
                           '\\toprule'                                                  + '\n'  + \
                           '\\addlinespace'                                             + '\n'  + \
                           '\\textbf{Property} & \\textbf{Unit} & \\textbf{Value} \\\\' + '\n'  + \
                           '\\addlinespace'                                             + '\n'  + \
                           '\\midrule'                                                  + '\n'  + \
                           '\\endhead'                                                  + '\n'  + \
                           '\\bottomrule'                                               + '\n'  + \
                           '\\endfoot'                                                  + '\n'  + \
                           '\\bottomrule'                                               + '\n'  + \
                           '\\endlastfoot'                                              + '\n'  + \
                           '\\addlinespace'                                             + '\n\n'

STR_PROPERTY_TABLE_END = '\n' + '\\addlinespace' + '\\end{longtable}' + '\n\n'


# %% Definition of functions.
#
def write_ads_property(ADS: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Generate property table for equation approaches of adsorption working pairs.

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
    STR_DM_CRYSTAL = str(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_pr_']['diameter-crystal'])
    STR_DM_PORE = str(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_pr_']['diameter-pore'])
    STR_DM_PELLET = str(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_pr_']['diameter-pellet'])
    STR_DM_EXTRUDATE = str(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_pr_']['diameter-extrudate'])
    STR_L_EXTRUDATE = str(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_pr_']['length-extrudate'])

    STR_AREA_SURFACE = str(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_pr_']['area-surface'])
    STR_VOL_PORE = str(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_pr_']['volume-pore'])
    STR_POR_PELLET = str(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_pr_']['porosity-pellet'])

    STR_DENS_BULK = str(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_pr_']['density-bulk'])
    STR_DENS_PELLET = str(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_pr_']['density-pellet'])
    STR_DENS_SOLID = str(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_pr_']['density-solid'])

    LIST_PROPERTIES = ['Diameter of crystal', 'Diameter of pore',
                       'Diameter of pellet', 'Diameter of extrudate',
                       'Length of extrudate',
                       'Surface area',
                       'Pore volume',
                       'Porosity of pellet',
                       'Bulk density',
                       'Pellet density',
                       'Solid density']

    LIST_UNITS = ['\\si{\\milli\\meter}', '\\si{\\milli\\meter}',
                  '\\si{\\milli\\meter}', '\\si{\\milli\\meter}',
                  '\\si{\\milli\\meter}',
                  '\\si{\\square\\meter\\per\\gram}',
                  '\\si{\\milli\\cubic\\meter\\per\\gram}',
                  '-',
                  '\\si{\\kilogram\\per\\cubic\\meter}',
                  '\\si{\\kilogram\\per\\cubic\\meter}',
                  '\\si{\\kilogram\\per\\cubic\\meter}']

    LIST_ENTRIES = [STR_DM_CRYSTAL, STR_DM_PORE,
                    STR_DM_PELLET, STR_DM_EXTRUDATE,
                    STR_L_EXTRUDATE,
                    STR_AREA_SURFACE,
                    STR_VOL_PORE,
                    STR_POR_PELLET,
                    STR_DENS_BULK,
                    STR_DENS_PELLET,
                    STR_DENS_SOLID]

    # Check if content exists
    #
    flag_entry = False

    for val in LIST_ENTRIES:
        if val:
            flag_entry = True
            break

    # Write formatted LaTeX table
    #
    STR_LATEX = STR_PROPERTY_START

    if flag_entry:
        # Start table
        #
        STR_LATEX += STR_PROPERTY_TABLE_START

        # Add entries if exist
        #
        for ind, val in enumerate(LIST_ENTRIES):
            if val:
                STR_LATEX += LIST_PROPERTIES[ind] + ' & ' + LIST_UNITS[ind] + ' & ' + \
                              val + '\\\\' + '\n'

        # End table
        #
        STR_LATEX += STR_PROPERTY_TABLE_END

    else:
        STR_LATEX += 'Property data of sorbent and subtype does not exist.' + '\n\n'

    # Return formatted LaTeX table
    #
    return STR_LATEX
