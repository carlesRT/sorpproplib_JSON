"""Package 'equation' cotains functions to generate equations and cofficient tables of the SorpPropLib.

Routine Listings
----------------
write_ref_equation_density_eos1:
    Write equation and coefficient table of refrigerant function 'SaturatedLiquidDensity_EoS1'.
write_ref_equation_pressure_eos1:
    Write equation and coefficient table of refrigerant function 'VaporPressure_EoS1'.
write_ref_equation_pressure_eos2:
    Write equation and coefficient table of refrigerant function 'VaporPressure_EoS2'.
write_ref_equation_pressure_eos3:
    Write equation and coefficient table of refrigerant function 'VaporPressure_EoS3'.
write_ref_equation_pressure_antoine:
    Write equation and coefficient table of refrigerant function 'VaporPressure_Antoine'.
write_ref_equation_pressure_mixing:
    Write equation and coefficient table of refrigerant function 'VaporPressure_EoSCubic'.
write_ref_equation:
    Select formatted LaTeX string depending on refrigerant function.
write_ads_langmuir:
	Generate equation and overview table of adsorption isotherm function 'Langmuir'.
write_ads_toth:
	Generate equation and overview table of adsorption isotherm function 'Toth'.
write_ads_dss:
	Generate equation and overview table of adsorption isotherm function 'DualSiteSips'.
write_ads_dubinin_astakhov:
	Generate equation and overview table of adsorption isotherm function 'DubininAstakhov'.
write_ads_dubinin_arctan1:
	Generate equation and overview table of adsorption isotherm function 'DubininArctan1'.
write_ads_equation:
    Select formatted LaTeX string depending on adsorption isotherm function.
write_abs_antoine:
	Generate equation and overview table of absorption isotherm function 'Antoine'.
write_abs_duehring:
	Generate equation and overview table of absorption isotherm function 'Duehring'.
write_abs_wilson_fdl:
	Generate equation and overview table of absorption isotherm function 'WilsonFixedDl'.
write_abs_wilson_tdl:
	Generate equation and overview table of absorption isotherm function 'WilsonTemperatureDl'.
write_abs_tsuboka_katayama:
	Generate equation and overview table of absorption isotherm function 'TsubokaKatayama'.
write_abs_wang_chao:
	Generate equation and overview table of absorption isotherm function 'WangChao'.
write_abs_heil:
	Generate equation and overview table of absorption isotherm function 'Heil'.
write_abs_flory_huggins:
	Generate equation and overview table of absorption isotherm function 'FloryHuggins'.
write_abs_nrtl_fdg:
	Generate equation and overview table of absorption isotherm function 'NrtlFixedDg'.
write_abs_nrtl_tdg:
	Generate equation and overview table of absorption isotherm function 'NrtlTemperatureDg'.
write_abs_uniquac_fdu:
	Generate equation and overview table of absorption isotherm function 'UniquacFixedDu'.
write_abs_uniquac_tdu:
	Generate equation and overview table of absorption isotherm function 'UniquacTemperatureDu'.
write_abs_mixing:
	Generate equation and overview table of absorption isotherm function 'MixingRule'.
write_abs_equation:
    Select formatted LaTeX string depending on absorption isotherm function.

Revisions
---------
    Version     Date        Author              Changes
    1.0.0       2020-07-20  Mirko Engelpracht   First implementation
"""


# %% Definition of constants.
#
# Constant strings defining start and end of equation and parameter section
#
STR_EQU_START = '\\textbf{Equation and parameters:}'    + '\n'  + \
                '\\newline'                             + '\n'  + \
                '%'                                     + '\n'

STR_EQU_DENS_START = STR_EQU_START                                                              + \
                     'Saturated liquid density $\\rho_\\mathrm{sat}^\\mathrm{liq}$ in '         + \
                     '$\\si{\\kilogram\\per\\cubic\\meter}$ is calculated depending on '        + \
                     'temperature $T$ in $\\si{\\kelvin}$ by:'                           + '\n' + \
                     '%'                                                                 + '\n'

STR_EQU_PRES_START = STR_EQU_START                                                              + \
                     'Vapor pressure $p_\\mathrm{sat}$ in $\\si{\\pascal}$ is calculated '      + \
                     'depending on temperature $T$ in $\\si{\\kelvin}$ by:'              + '\n' + \
                     '%'                                                                 + '\n'

STR_EQU_ADS_SUR_START = STR_EQU_START                                                           + \
                        'Loading $w$ in $\\si{\\kilogram\\per\\kilogram}$ is calculated '       + \
                        'depending on pressure $p$ in $\\si{\\pascal}$ and temperature $T$ '    + \
                        'in $\\si{\\kelvin}$ by:'                                        + '\n' + \
                        '%'                                                              + '\n'

STR_EQU_ADS_SUR_VAP_START = STR_EQU_START                                                       + \
                            'Loading $w$ in $\\si{\\kilogram\\per\\kilogram}$ is calculated '   + \
                            'depending on pressure $p$ in $\\si{\\pascal}$, temperature $T$ in '+ \
                            '$\\si{\\kelvin}$, and vapor pressure $p_\\mathrm{sat}$ in '        + \
                            '$\\si{\\pascal}$ by:'                                       + '\n' + \
                            '%'                                                          + '\n'

STR_EQU_ADS_VOL_START = STR_EQU_START                                                           + \
                        'Loading $w$ in $\\si{\\kilogram\\per\\kilogram}$ is calculated '       + \
                        'depending on pressure $p$ in $\\si{\\pascal}$, temperature $T$ in '    + \
                        '$\\si{\\kelvin}, vapor pressure $p_\\mathrm{sat}$ in '                 + \
                        '$\\si{\\pascal}$, and saturated liquid density '                       + \
                        '$\\rho_\\mathrm{sat}^\\mathrm{liq}$ in '                               + \
                        '$\\si{\\kilogram\\per\\cubic\\meter}$ by:'                     + '\n' + \
                        '%'                                                             + '\n'

STR_EQU_ABS_CON_START = STR_EQU_START                                                           + \
                        'Pressure $p$ in $\\si{\\pascal}$ is calculated depending on '          + \
                        'concentration $X$ in $\\si{\\kilogram\\per\\kilogram}$ and  '          + \
                        'temperature $T$ in $\\si{\\kelvin}$ by:'                        + '\n' + \
                        '%'                                                              + '\n'

STR_EQU_ABS_ACT_START = STR_EQU_START                                                           + \
                        'Pressure $p$ in $\\si{\\pascal}$ is calculated depending on '          + \
                        'molar fraction of refrigerant in the liquid phase $x_1$ in '           + \
                        '$\\si{\\mole\\per\\mole}$, temperature $T$ in $\\si{\\kelvin}$, '      + \
                        'molar volumes of both components ($v_1$ and $v_2$) in '                + \
                        '$\\si{\\cubic\\meter\\per\\mole}$, and vapor pressure '                + \
                        '$p_\\mathrm{sat,1}$ in $\\si{\\pascal}$. If molar volumes less than '  + \
                        'zero are used as function arguements, constant molar volumes given by '+ \
                        'parameter record are used. Equilibrium equation is given by:'   + '\n' + \
                        '%'                                                              + '\n'

STR_EQU_ABS_ACT2_START = STR_EQU_START                                                          + \
                         'Pressure $p$ in $\\si{\\pascal}$ is calculated depending on '         + \
                         'molar fraction of refrigerant in the liquid phase $x_1$ in '          + \
                         '$\\si{\\mole\\per\\mole}$, temperature $T$ in $\\si{\\kelvin}$, '     + \
                         'and vapor pressure $p_\\mathrm{sat,1}$ in $\\si{\\pascal}$ by:'+ '\n' + \
                         '%'                                                             + '\n'

STR_EQU_TABLE_START = '%'                                                               + '\n'  + \
                      'The parameters of the equation are:'                             + '\n'  + \
                      '%'                                                               + '\n'  + \
                      '\\begin{longtable}[l]{lll|lll}'                                  + '\n'  + \
                      '\\toprule'                                                       + '\n'  + \
                      '\\addlinespace'                                                  + '\n'  + \
                      '\\textbf{Par.} & \\textbf{Unit} & \\textbf{Value} &	'                   + \
                      '\\textbf{Par.} & \\textbf{Unit} & \\textbf{Value} \\\\'          + '\n'  + \
                      '\\addlinespace'                                                  + '\n'  + \
                      '\\midrule'                                                       + '\n'  + \
                      '\\endhead'                                                       + '\n\n'+ \
                      '\\bottomrule'                                                    + '\n'  + \
                      '\\endfoot'                                                       + '\n'  + \
                      '\\bottomrule'                                                    + '\n'  + \
                      '\\endlastfoot'                                                   + '\n'  + \
                      '\\addlinespace'                                                  + '\n\n'

STR_EQU_TABLE_END = '\n' + '\\addlinespace' + '\\end{longtable}' + '\n\n'


# %% Define functions.
#
def write_ref_equation_density_eos1(REF: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Generate equation and coefficient table of refrigerant function 'SaturatedLiquidDensity_EoS1'.

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
        Formatted LaTeX string describing equation and coefficient table of refrigerant function.

    """
    # Get correct contents from dict
    #
    STR_FLAG = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['flag'])
    STR_T_CRIT = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['T_crit'])
    STR_RHO_CRIT = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['rho_crit'])
    STR_A1 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['fac1'])
    STR_A2 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['fac2'])
    STR_A3 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['fac3'])
    STR_A4 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['fac4'])
    STR_A5 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['fac5'])
    STR_A6 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['fac6'])
    STR_A7 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['fac7'])
    STR_A8 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['fac8'])
    STR_B1 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['exp1'])
    STR_B2 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['exp2'])
    STR_B3 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['exp3'])
    STR_B4 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['exp4'])
    STR_B5 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['exp5'])
    STR_B6 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['exp6'])
    STR_B7 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['exp7'])
    STR_B8 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['exp8'])

    # Write formatted LaTeX string
    #
    STR_LATEX = STR_EQU_DENS_START                                                              + \
                '\\begin{equation*}'                                                + '\n'      + \
                '\\begin{split}'                                                    + '\n'      + \
                '\\rho_\\mathrm{sat}^\\mathrm{liq} &=& \\begin{cases} \\rho_\\mathrm{ref} '     + \
                '\\exp(\\Omega) & \\quad \\text{if flag } < 0 \\\\ \\rho_\\mathrm{ref} \\Omega '+ \
                '& \\quad \\text{else} \\end{cases} & \\quad\\text{, with} \\\\'    + '\n'      + \
                '\\Omega &=& \\sum_{i=1}^{8} a_i \\xi^{b_i} & \\quad\\text{, and} '             + \
                '\\\\'                                                              + '\n'      + \
                '\\xi &=& 1 - \\theta & \\quad\\text{, and} \\\\'                   + '\n'      + \
                '\\theta &=& \\nicefrac{T}{T_\\mathrm{crit}} & \\quad\\text{.}'     + '\n'      + \
                '\\end{split}'                                                      + '\n'      + \
                '\\end{equation*}'                                                  + '\n'      + \
                STR_EQU_TABLE_START                                                             + \
                'flag & - & '                                                      + STR_FLAG     + ' & $b_4$ & - & ' + STR_B4 + ' \\\\'  + '\n' + \
                '$T_\\mathrm{crit}$ & $\\si{\\kelvin}$ & '                         + STR_T_CRIT   + ' & $a_5$ & - & ' + STR_A5 + ' \\\\'  + '\n' + \
                '$\\rho_\\mathrm{ref}$ & $\\si{\\kilogram\\per\\cubic\\meter}$ & ' + STR_RHO_CRIT + ' & $b_5$ & - & ' + STR_B5 + ' \\\\'  + '\n' + \
                '$a_1$ & - & '                                                     + STR_A1       + ' & $a_6$ & - & ' + STR_A6 + ' \\\\'  + '\n' + \
                '$b_1$ & - & '                                                     + STR_B1       + ' & $b_6$ & - & ' + STR_B6 + ' \\\\'  + '\n' + \
                '$a_2$ & - & '                                                     + STR_A2       + ' & $a_7$ & - & ' + STR_A7 + ' \\\\'  + '\n' + \
                '$b_2$ & - & '                                                     + STR_B2       + ' & $b_7$ & - & ' + STR_B7 + ' \\\\'  + '\n' + \
                '$a_3$ & - & '                                                     + STR_A3       + ' & $a_8$ & - & ' + STR_A8 + ' \\\\'  + '\n' + \
                '$b_3$ & - & '                                                     + STR_B3       + ' & $b_8$ & - & ' + STR_B8 + ' \\\\'  + '\n' + \
                '$a_4$ & - & '                                                     + STR_A4       + ' & & & \\\\'                          + '\n' + \
                STR_EQU_TABLE_END

    # Return formatted LaTeX string
    #
    return STR_LATEX

def write_ref_equation_pressure_eos1(REF: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Generate equation and overview table of refrigerant function 'VaporPressure_EoS1'.

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
        Formatted LaTeX string describing equation and coefficient table of refrigerant function.

    """
    # Get correct contents from dict
    #
    STR_T_CRIT = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['T_crit'])
    STR_P_CRIT = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['p_crit'])
    STR_A1 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['fac1'])
    STR_A2 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['fac2'])
    STR_A3 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['fac3'])
    STR_A4 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['fac4'])
    STR_A5 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['fac5'])
    STR_A6 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['fac6'])
    STR_A7 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['fac7'])
    STR_B1 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['exp1'])
    STR_B2 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['exp2'])
    STR_B3 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['exp3'])
    STR_B4 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['exp4'])
    STR_B5 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['exp5'])
    STR_B6 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['exp6'])
    STR_B7 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['exp7'])

    # Write formatted LaTeX string
    #
    STR_LATEX = STR_EQU_PRES_START                                                              + \
                '\\begin{equation*}'                                                + '\n'      + \
                '\\begin{split}'                                                    + '\n'      + \
                'p_\\mathrm{sat} &=& p_\\mathrm{crit} \\exp \\left( \\nicefrac{1}{\\theta} '    + \
                '\\sum_{i=1}^{7} a_i \\xi^{b_i} \\right) & \\quad\\text{, and} \\\\'+ '\n'      + \
                '\\xi &=& 1 - \\theta & \\quad\\text{, and} \\\\'                   + '\n'      + \
                '\\theta &=& \\nicefrac{T}{T_\\mathrm{crit}} & \\quad\\text{.}'     + '\n'      + \
                '\\end{split}'                                                      + '\n'      + \
                '\\end{equation*}'                                                  + '\n'      + \
                STR_EQU_TABLE_START                                                             + \
                '$T_\\mathrm{crit}$ & $\\si{\\kelvin}$ & '  + STR_T_CRIT   + ' & $a_4$ & - & ' + STR_A4 + ' \\\\'  + '\n' + \
                '$p_\\mathrm{crit}$ & $\\si{\\pascal}$ & '  + STR_P_CRIT   + ' & $b_4$ & - & ' + STR_B4 + ' \\\\'  + '\n' + \
                '$a_1$ & - & '                              + STR_A1       + ' & $a_5$ & - & ' + STR_A5 + ' \\\\'  + '\n' + \
                '$b_1$ & - & '                              + STR_B1       + ' & $b_5$ & - & ' + STR_B5 + ' \\\\'  + '\n' + \
                '$a_2$ & - & '                              + STR_A2       + ' & $a_6$ & - & ' + STR_A6 + ' \\\\'  + '\n' + \
                '$b_2$ & - & '                              + STR_B2       + ' & $b_6$ & - & ' + STR_B6 + ' \\\\'  + '\n' + \
                '$a_3$ & - & '                              + STR_A3       + ' & $a_7$ & - & ' + STR_A7 + ' \\\\'  + '\n' + \
                '$b_3$ & - & '                              + STR_B3       + ' & $b_7$ & - & ' + STR_B7 + ' \\\\'  + '\n' + \
                STR_EQU_TABLE_END

    # Return formatted LaTeX string
    #
    return STR_LATEX

def write_ref_equation_pressure_eos2(REF: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Generate equation and overview table of refrigerant function 'VaporPressure_EoS2'.

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
        Formatted LaTeX string describing equation and coefficient table of refrigerant function.

    """
    # Get correct contents from dict
    #
    STR_T_CRIT = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['T_crit'])
    STR_P_REF = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['p_ref'])
    STR_A0 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['fac0'])
    STR_A1 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['fac1'])
    STR_A2 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['fac2'])
    STR_A3 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['fac3'])
    STR_A4 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['fac4'])
    STR_A5 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['fac5'])
    STR_B1 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['exp1'])
    STR_B2 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['exp2'])
    STR_B3 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['exp3'])
    STR_B4 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['exp4'])
    STR_B5 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['exp5'])

    # Write formatted LaTeX string
    #
    STR_LATEX = STR_EQU_PRES_START                                                              + \
                '\\begin{equation*}'                                                + '\n'      + \
                '\\begin{split}'                                                    + '\n'      + \
                'p_\\mathrm{sat} &=& p_\\mathrm{ref} \\exp \\left( \\nicefrac{a_0}{\\theta} + ' + \
                '\\sum_{i=1}^{4} a_i \\theta^{b_i} + a_5 \\xi^{b_5} \\right) & \\quad\\text{, ' + \
                'and} \\\\'                                                         + '\n'      + \
                '\\xi &=& 1 - \\theta & \\quad\\text{, and} \\\\'                   + '\n'      + \
                '\\theta &=& \\nicefrac{T}{T_\\mathrm{crit}} & \\quad\\text{.}'     + '\n'      + \
                '\\end{split}'                                                      + '\n'      + \
                '\\end{equation*}'                                                  + '\n'      + \
                STR_EQU_TABLE_START                                                             + \
                '$T_\\mathrm{crit}$ & $\\si{\\kelvin}$ & '  + STR_T_CRIT   + ' & $a_3$ & - & ' + STR_A3 + ' \\\\'  + '\n' + \
                '$p_\\mathrm{ref}$ & $\\si{\\pascal}$ & '   + STR_P_REF    + ' & $b_3$ & - & ' + STR_B3 + ' \\\\'  + '\n' + \
                '$a_0$ & - & '                              + STR_A0       + ' & $a_4$ & - & ' + STR_A4 + ' \\\\'  + '\n' + \
                '$a_1$ & - & '                              + STR_A1       + ' & $b_4$ & - & ' + STR_B4 + ' \\\\'  + '\n' + \
                '$b_1$ & - & '                              + STR_B1       + ' & $a_5$ & - & ' + STR_A5 + ' \\\\'  + '\n' + \
                '$a_2$ & - & '                              + STR_A2       + ' & $b_5$ & - & ' + STR_B5 + ' \\\\'  + '\n' + \
                '$b_2$ & - & '                              + STR_B2       + ' & & & \\\\'                         + '\n' + \
                STR_EQU_TABLE_END

    # Return formatted LaTeX string
    #
    return STR_LATEX

def write_ref_equation_pressure_eos3(REF: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Generate equation and overview table of refrigerant function 'VaporPressure_EoS3'.

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
        Formatted LaTeX string describing equation and coefficient table of refrigerant function.

    """
    # Get correct contents from dict
    #
    STR_T_CRIT = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['T_crit'])
    STR_P_REF = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['p_ref'])
    STR_A0 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['fac0'])
    STR_A1 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['fac1'])
    STR_A2 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['fac2'])
    STR_A3 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['fac3'])
    STR_B1 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['exp1'])
    STR_B2 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['exp2'])
    STR_B3 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['exp3'])

    # Write formatted LaTeX string
    #
    STR_LATEX = STR_EQU_PRES_START                                                              + \
                '\\begin{equation*}'                                                + '\n'      + \
                '\\begin{split}'                                                    + '\n'      + \
                'p_\\mathrm{sat} &=& p_\\mathrm{ref} \\exp \\left( \\nicefrac{a_0}{T} + '       + \
                '\\sum_{i=1}^{2} a_i T^{b_i} + a_3 \\xi^{b_3} \\right) & \\quad\\text{, '       + \
                'and} \\\\'                                                         + '\n'      + \
                '\\xi &=& 1 - \\theta & \\quad\\text{, and} \\\\'                   + '\n'      + \
                '\\theta &=& \\nicefrac{T}{T_\\mathrm{crit}} & \\quad\\text{.}'     + '\n'      + \
                '\\end{split}'                                                      + '\n'      + \
                '\\end{equation*}'                                                  + '\n'      + \
                STR_EQU_TABLE_START                                                             + \
                '$T_\\mathrm{crit}$ & $\\si{\\kelvin}$ & '  + STR_T_CRIT   + ' & $a_2$ & - & ' + STR_A2 + ' \\\\'  + '\n' + \
                '$p_\\mathrm{ref}$ & $\\si{\\pascal}$ & '   + STR_P_REF    + ' & $b_2$ & - & ' + STR_B2 + ' \\\\'  + '\n' + \
                '$a_0$ & - & '                              + STR_A0       + ' & $a_3$ & - & ' + STR_A3 + ' \\\\'  + '\n' + \
                '$a_1$ & - & '                              + STR_A1       + ' & $b_3$ & - & ' + STR_B3 + ' \\\\'  + '\n' + \
                '$b_1$ & - & '                              + STR_B1       + ' & & & \\\\'                         + '\n' + \
                STR_EQU_TABLE_END

    # Return formatted LaTeX string
    #
    return STR_LATEX

def write_ref_equation_pressure_antoine(REF: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Generate equation and overview table of refrigerant function 'VaporPressure_Antoine'.

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
        Formatted LaTeX string describing equation and coefficient table of refrigerant function.

    """
    # Get correct contents from dict
    #
    STR_A = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['a'])
    STR_B = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['b'])
    STR_C = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['c'])

    # Write formatted LaTeX string
    #
    STR_LATEX = STR_EQU_PRES_START                                                              + \
                '\\begin{equation*}'                                                  + '\n'    + \
                '\\nicefrac{p_\\mathrm{sat}}{100000} = 10^{a - \\nicefrac{b}{T + c}}' + '\n'    + \
                '\\end{equation*}'                                                    + '\n'    + \
                STR_EQU_TABLE_START                                                             + \
                '$a$ & - & '                + STR_A     + ' & $c$ & $\\si{\\kelvin}$  & ' + STR_C + ' \\\\'  + '\n' + \
                '$b$ & $\\si{\\kelvin}$ & ' + STR_B     + ' & & & \\\\'                                      + '\n' + \
                STR_EQU_TABLE_END

    # Return formatted LaTeX string
    #
    return STR_LATEX

def write_ref_equation_pressure_mixing(REF: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Generate equation and overview table of refrigerant function 'VaporPressure_EoSCubic'.

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
        Formatted LaTeX string describing equation and coefficient table of refrigerant function.

    """
    # Get correct contents from dict
    #
    STR_EOS = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['EoS'])
    STR_T_CRIT = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['T_crit'])
    STR_P_CRIT = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['p_crit'])
    STR_OMEGA = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['omega'])
    STR_KAPPA1 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['kappa_1'])
    STR_BETA0 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['beta_0'])
    STR_BETA1 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['beta_1'])
    STR_BETA2 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['beta_2'])
    STR_BETA3 = '{:.9e}'.format(REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['beta_3'])

    # Write formatted LaTeX string
    #
    STR_LATEX = STR_EQU_START                                                                   + \
                'Vapor pressure $p_\\mathrm{sat}$ in $\\si{\\pascal}$ is calculated '           + \
                'depending on temperature $T$ in $\\si{\\kelvin}$ and molar volume v in '       + \
                '$\\si{\\mole\\per\\cubic\\meter}$ by using cubic equation of state. For this ' + \
                'purpose, molar volumes of liquid and vapor phase are changed iteratively '     + \
                'until fugacity coefficients of vapor and liquid phase are equal. Cubic '       + \
                'equation of state is given by:'                                          + '\n'

    if REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['EoS'] <= -7.5:
        # SRKE with custom alpha equation
        #
        STR_LATEX += '\\begin{equation*}'                                                + '\n'      + \
                     '\\begin{split}'                                                    + '\n'      + \
                     'p &=& R \\frac{T}{v - b} - \\frac{a}{v \\left(v + b\\right)} & \\quad\\text{, and} \\\\' + '\n' + \
                     'a &=& \\frac{1}{9 \\left(2^{\\nicefrac{1}{3}} - 1\\right)} \\frac{\\left(R T_\\mathrm{crit} \\right)^2}{p_\\mathrm{crit}} \\alpha & \\quad\\text{, and} \\\\' + '\n' + \
                     'b &=& 0.08664 R \\frac{T_\\mathrm{crit}}{p_\\mathrm{crit}} & \\quad\\text{, and} \\\\' + '\n' + \
                     '\\alpha &=& \\beta_0 + \\sum_{i=1}^{3} \\beta_i \\theta^i & \\quad\\text{, and} \\\\' + '\n' + \
                     '\\theta &=& \\nicefrac{T_\\mathrm{crit}}{T} - \\nicefrac{T}{T_\\mathrm{crit}} & \\quad\\text{.}' + '\n' + \
                     '\\end{split}'                                                      + '\n' + \
                     '\\end{equation*}'

    elif REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['EoS'] <= -2.5:
        # SRKE
        #
        STR_LATEX += '\\begin{equation*}'                                                + '\n'      + \
                     '\\begin{split}'                                                    + '\n'      + \
                     'p &=& R \\frac{T}{v - b} - \\frac{a}{v \\left(v + b\\right)} & \\quad\\text{, and} \\\\' + '\n' + \
                     'a &=& \\frac{1}{9 \\left(2^{\\nicefrac{1}{3}} - 1\\right)} \\frac{\\left(R T_\\mathrm{crit} \\right)^2}{p_\\mathrm{crit}} \\alpha & \\quad\\text{, and} \\\\' + '\n' + \
                     'b &=& 0.08664 R \\frac{T_\\mathrm{crit}}{p_\\mathrm{crit}} & \\quad\\text{, and} \\\\' + '\n' + \
                     '\\alpha &=& \\left(1 + \\kappa \\left(1 - \\sqrt(\\nicefrac{T}{T_\\mathrm{crit}}) \\right) \\right)^2 & \\quad\\text{, and} \\\\' + '\n' + \
                     '\\kappa &=& 0.48508 + 1.55171 \\omega - 0.15613 \\omega^2 & \\quad\\text{.}' + '\n' + \
                     '\\end{split}'                                                      + '\n' + \
                     '\\end{equation*}'                                                  + '\n'

    elif REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['EoS'] <= 7.5:
        # PRE with custom alpha equation
        #
        STR_LATEX += '\\begin{equation*}'                                                + '\n'      + \
                     '\\begin{split}'                                                    + '\n'      + \
                     'p &=& R \\frac{T}{v - b} - \\frac{a}{v \\left(v + b\\right) + b \\left(v - b\\right)} & \\quad\\text{, and} \\\\' + '\n' + \
                     'a &=& 0.45724 \\frac{\\left(R T_\\mathrm{crit} \\right)^2}{p_\\mathrm{crit}} \\alpha & \\quad\\text{, and} \\\\' + '\n' + \
                     'b &=& 0.07780 R \\frac{T_\\mathrm{crit}}{p_\\mathrm{crit}} & \\quad\\text{, and} \\\\' + '\n' + \
                     '\\alpha &=& \\beta_0 + \\sum_{i=1}^{3} \\beta_i \\theta^i & \\quad\\text{, and} \\\\' + '\n' + \
                     '\\theta &=& \\nicefrac{T_\\mathrm{crit}}{T} - \\nicefrac{T}{T_\\mathrm{crit}} & \\quad\\text{.}' + '\n' + \
                     '\\end{split}'                                                      + '\n' + \
                     '\\end{equation*}'

    elif REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['EoS'] <= 12.5:
        # PRE
        #
        STR_LATEX += '\\begin{equation*}'                                                + '\n'      + \
                     '\\begin{split}'                                                    + '\n'      + \
                     'p &=& R \\frac{T}{v - b} - \\frac{a}{v \\left(v + b\\right) + b \\left(v - b\\right)} & \\quad\\text{, and} \\\\' + '\n' + \
                     'a &=& 0.45724 \\frac{\\left(R T_\\mathrm{crit} \\right)^2}{p_\\mathrm{crit}} \\alpha & \\quad\\text{, and} \\\\' + '\n' + \
                     'b &=& 0.07780 R \\frac{T_\\mathrm{crit}}{p_\\mathrm{crit}} & \\quad\\text{, and} \\\\' + '\n' + \
                     '\\alpha &=& \\left(1 + \\kappa \\left(1 - \\sqrt(\\nicefrac{T}{T_\\mathrm{crit}}) \\right) \\right)^2 & \\quad\\text{, and} \\\\' + '\n' + \
                     '\\kappa &=& 0.37464 + 1.54226 \\omega - 0.26992 \\omega^2 & \\quad\\text{.}' + '\n' + \
                     '\\end{split}'                                                      + '\n' + \
                     '\\end{equation*}'                                                  + '\n'

    elif REF['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['EoS'] <= 22.5:
        # PRSVE
        #
        STR_LATEX += '\\begin{equation*}'                                                + '\n'      + \
                     '\\begin{split}'                                                    + '\n'      + \
                     'p &=& R \\frac{T}{v - b} - \\frac{a}{v \\left(v + b\\right) + b \\left(v - b\\right)} & \\quad\\text{, and} \\\\' + '\n' + \
                     'a &=& 0.45724 \\frac{\\left(R T_\\mathrm{crit} \\right)^2}{p_\\mathrm{crit}} \\alpha & \\quad\\text{, and} \\\\' + '\n' + \
                     'b &=& 0.07780 R \\frac{T_\\mathrm{crit}}{p_\\mathrm{crit}} & \\quad\\text{, and} \\\\' + '\n' + \
                     '\\alpha &=& \\left(1 + \\kappa \\left(1 - \\sqrt(\\nicefrac{T}{T_\\mathrm{crit}}) \\right) \\right)^2 & \\quad\\text{, and} \\\\' + '\n' + \
                     '\\kappa &=& \\kappa_0 + \\kappa_1 \\left(1 + \\sqrt(\\nicefrac{T}{T_\\mathrm{crit}}) \\right) \\left(0.7 - \\nicefrac{T}{T_\\mathrm{crit}}\\right)  & \\quad\\text{, and} \\\\' + '\n' + \
                     '\\kappa_0 &=& 0.378893 + 1.4897153 \\omega - 0.17131848 \\omega^2 + 0.0196554 \\omega^3 & \\quad\\text{.}' + '\n' + \
                     '\\end{split}'                                                      + '\n' + \
                     '\\end{equation*}'                                                  + '\n'

    STR_LATEX += STR_EQU_TABLE_START                                                                                             + \
                 'EoS & - & '                               + STR_EOS     + ' & $\\beta_0$ & - & ' + STR_BETA0 + ' \\\\'  + '\n' + \
                 '$T_\\mathrm{crit}$ & $\\si{\\kelvin}$ & ' + STR_T_CRIT  + ' & $\\beta_1$ & - & ' + STR_BETA1 + ' \\\\'  + '\n' + \
                 '$p_\\mathrm{crit}$ & $\\si{\\pascal}$ & ' + STR_P_CRIT  + ' & $\\beta_2$ & - & ' + STR_BETA2 + ' \\\\'  + '\n' + \
                 '$\\omega$ & - & '                         + STR_OMEGA   + ' & $\\beta_3$ & - & ' + STR_BETA3 + ' \\\\'  + '\n' + \
                 '$\\kappa_1$ & - & '                       + STR_KAPPA1  + ' & & & \\\\'                                 + '\n' + \
                 STR_EQU_TABLE_END

    # Return formatted LaTeX string
    #
    return STR_LATEX

def write_ref_equation(ADS: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Select formatted LaTeX string depending on refrigerant function.

    Parameters
    ----------
    ADS : dict
        Dict containing properties of current refrigerant extrated from data base (i.e., JSON).
    EQU_NAME : str
        Name of refrigerant functions for which overview table shall be generated.
    EQU_ID : int
        ID of refrigerant functions for which overview table shall be generated.

    Returns
    -------
    str
        Formatted LaTeX string describing equation and overview table of refrigerant function.

    """
    # Switch equations
    #
    if EQU_NAME == 'SaturatedLiquidDensity_EoS1':
        return write_ref_equation_density_eos1(ADS, EQU_NAME, EQU_ID)

    elif EQU_NAME == 'VaporPressure_EoS1':
        return write_ref_equation_pressure_eos1(ADS, EQU_NAME, EQU_ID)

    elif EQU_NAME == 'VaporPressure_EoS2':
        return write_ref_equation_pressure_eos2(ADS, EQU_NAME, EQU_ID)

    elif EQU_NAME == 'VaporPressure_EoS3':
        return write_ref_equation_pressure_eos3(ADS, EQU_NAME, EQU_ID)

    elif EQU_NAME == 'VaporPressure_Antoine':
        return write_ref_equation_pressure_antoine(ADS, EQU_NAME, EQU_ID)

    elif EQU_NAME == 'VaporPressure_EoSCubic':
        return write_ref_equation_pressure_mixing(ADS, EQU_NAME, EQU_ID)

    else:
        # No formatted LaTeX string implemented for selected equation
        #
        return STR_EQU_START + 'For equation \'' + EQU_NAME.replace('_', '\_') + \
            '\', formatted LaTeX string is not implemented in manual!' + '\n\n'

def write_ads_langmuir(ADS: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Generate equation and overview table of adsorption isotherm function 'Langmuir'.

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
        Formatted LaTeX string describing equation and coefficient table of adsorption isotherm function.

    """
    # Get correct contents from dict
    #
    STR_DH = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['delta_H'])
    STR_K0 = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['K0'])
    STR_WS = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['w_s'])

    # Write formatted LaTeX string
    #
    STR_LATEX = STR_EQU_ADS_SUR_START                                                             + \
                '\\begin{equation*}'                                                       + '\n' + \
                '\\begin{split}'                                                           + '\n' + \
                'w &=& \\frac{w_\\mathrm{sat} K p}{1 + K p} & \\quad\\text{, and} \\\\'    + '\n' + \
                'K &=& K_0 \\exp \\left( \\frac{\\Delta H}{R T} \\right) & \\quad\\text{.}'+ '\n' + \
                '\\end{split}'                                                             + '\n' + \
                '\\end{equation*}'                                                         + '\n' + \
                STR_EQU_TABLE_START                                                               + \
                '$\\Delta H$ & $\\si{\\joule\\per\\mole}$ & ' + STR_DH + ' & $w_\\mathrm{sat}$ & $\\si{\\kilogram\\per\\kilogram}$ & ' + STR_WS + ' \\\\'  + '\n' + \
                '$K_0$ & $\\si{\\per\\pascal}$ & '            + STR_K0 + ' & & & \\\\'                                                                     + '\n' + \
                STR_EQU_TABLE_END

    # Return formatted LaTeX string
    #
    return STR_LATEX

def write_ads_toth(ADS: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Generate equation and overview table of adsorption isotherm function 'Toth'.

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
        Formatted LaTeX string describing equation and coefficient table of adsorption isotherm function.

    """
    # Get correct contents from dict
    #
    STR_B0 = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['bo'])
    STR_C = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['c'])
    STR_M = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['m'])
    STR_N0 = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['n0'])
    STR_Q_STAR = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['q_star_R'])
    STR_R = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['r'])
    STR_WS = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['w_s'])

    # Write formatted LaTeX string
    #
    STR_LATEX = STR_EQU_ADS_SUR_START                                                             + \
                '\\begin{equation*}'                                                       + '\n' + \
                '\\begin{split}'                                                           + '\n' + \
                'w &=& \\frac{w_\\mathrm{sat} b^{m} p}{\\left( 1 + b^{r} p^{n} \\right)^{\\nicefrac{1}{n}}} & \\quad\\text{, and} \\\\'     + '\n' + \
                'b &=& b_0 \\exp\\left( \\frac{Q^{*}}{T} \\right) & \\quad\\text{, and} \\\\'                                               + '\n' + \
                'n &=& n_0 + \\nicefrac{c}{T} & \\quad\\text{, and} \\\\'                                                                   + '\n' + \
                'r &=& \\begin{cases} n & \\quad \\text{if } r^{*} < 0 \\\\ r^{*}  & \\quad \\text{else} \\end{cases} & \\quad\\text{.}'    + '\n' + \
                '\\end{split}'                                                             + '\n' + \
                '\\end{equation*}'                                                         + '\n' + \
                STR_EQU_TABLE_START                                                               + \
                '$b_0$ & $\\si{\\per\\pascal}$ & '  + STR_B0 + ' & $Q^{*}$ & $\\si{\\kelvin}$ & '                             + STR_Q_STAR  + ' \\\\'  + '\n' + \
                '$c$ & $\\si{\\kelvin}$ & '         + STR_C  + ' & $r^{*}$ & - & '                                            + STR_R       + ' \\\\'  + '\n' + \
                '$m$ & - & '                        + STR_M  + ' & $w_\\mathrm{sat}$ & $\\si{\\kilogram\\per\\kilogram}$ & '  + STR_WS      + ' \\\\'  + '\n' + \
                '$n_0$ & - & '                      + STR_N0 + ' & & & \\\\'                                                                           + '\n' + \
                STR_EQU_TABLE_END

    # Return formatted LaTeX string
    #
    return STR_LATEX

def write_ads_dss(ADS: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Generate equation and overview table of adsorption isotherm function 'DualSiteSips'.

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
        Formatted LaTeX string describing equation and coefficient table of adsorption isotherm function.

    """
    # Get correct contents from dict
    #
    STR_BA0 = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['b_A_0'])
    STR_BB0 = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['b_B_0'])
    STR_QA = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['Q_A_st'])
    STR_QB = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['Q_B_st'])
    STR_ETAA = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['eta_A'])
    STR_ETAB = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['eta_B'])
    STR_WA = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['w_A'])
    STR_WB = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['w_B'])
    STR_T0 = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['T_0'])

    # Write formatted LaTeX string
    #
    STR_LATEX = STR_EQU_ADS_SUR_START                                                             + \
                '\\begin{equation*}'                                                       + '\n' + \
                '\\begin{split}'                                                           + '\n' + \
                'w &=& \\sum_{i=A}^{B} w_\\mathrm{i} \\frac{\\left( b_i p \\right) ^ {\\nicefrac{1}{\\eta_i}} }{1 + \\left( b_i p \\right) ^ {\\nicefrac{1}{\\eta_i}}} & \\quad\\text{, and} \\\\' + '\n' + \
                'b_i &=& b_{i,0} \\exp \\left( \\frac{Q_i}{R T} \\left( 1 - \\frac{T}{T_0} \\right) \\right) & \\quad\\text{.}'                                                                    + '\n' + \
                '\\end{split}'                                                             + '\n' + \
                '\\end{equation*}'                                                         + '\n' + \
                STR_EQU_TABLE_START                                                               + \
                '$b_\\mathrm{A,0}$ & $\\si{\\per\\pascal}$ & '           + STR_BA0  + ' & $b_\\mathrm{B,0}$ & $\\si{\\per\\pascal}$ & '           + STR_BB0  + ' \\\\' + '\n' + \
                '$Q_\\mathrm{A}$ & $\\si{\\joule\\per\\mole}$ & '        + STR_QA   + ' & $Q_\\mathrm{B}$ & $\\si{\\joule\\per\\mole}$ & '        + STR_QB   + ' \\\\' + '\n' + \
                '$\\eta_\\mathrm{A}$ & - & '                             + STR_ETAA + ' & $\\eta_\\mathrm{B}$ & - & '                             + STR_ETAB + ' \\\\' + '\n' + \
                '$w_\\mathrm{A}$ & $\\si{\\kilogram\\per\\kilogram}$ & ' + STR_WA   + ' & $w_\\mathrm{B}$ & $\\si{\\kilogram\\per\\kilogram}$ & ' + STR_WB   + ' \\\\' + '\n' + \
                '$T_0$ & $\\si{\\kelvin}$ & '                            + STR_T0   + ' & & & \\\\'                                                                    + '\n' + \
                STR_EQU_TABLE_END

    # Return formatted LaTeX string
    #
    return STR_LATEX

def write_ads_freundlich(ADS: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Generate equation and overview table of adsorption isotherm function 'DualSiteSips'.

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
        Formatted LaTeX string describing equation and coefficient table of adsorption isotherm function.

    """
    # Get correct contents from dict
    #
    STR_A0 = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['A0'])
    STR_A1 = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['A1'])
    STR_A2 = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['A2'])
    STR_A3 = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['A3'])
    STR_B0 = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['B0'])
    STR_B1 = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['B1'])
    STR_B2 = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['B2'])
    STR_B3 = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['B3'])

    # Write formatted LaTeX string
    #
    STR_LATEX = STR_EQU_ADS_SUR_VAP_START                                                         + \
                '\\begin{equation*}'                                                       + '\n' + \
                '\\begin{split}'                                                           + '\n' + \
                'w &=& A \\left( \\nicefrac{p}{p_\\mathrm{sat}} \\right) ^{B} & \\quad\\text{, and} \\\\'   + '\n' + \
                'A &=& A_0 + A_1 T + A_2 T^2 + A_3 T^3 & \\quad\\text{, and} \\\\'                          + '\n' + \
                'B &=& B_0 + B_1 T + B_2 T^2 + B_3 T^3 & \\quad\\text{.} \\\\'                              + '\n' + \
                '\\end{split}'                                                             + '\n' + \
                '\\end{equation*}'                                                         + '\n' + \
                STR_EQU_TABLE_START                                                               + \
                '$A_0$ & $\\si{\\kilogram\\per\\kilogram}$ & '                      + STR_A0 + ' & $B_0$ & - & '                            + STR_B0 + ' \\\\' + '\n' + \
                '$A_1$ & $\\si{\\kilogram\\per\\kilogram\\per\\kelvin}$ & '         + STR_A1 + ' & $B_1$ & $\\si{\\per\\kelvin}$ & '        + STR_B1 + ' \\\\' + '\n' + \
                '$A_2$ & $\\si{\\kilogram\\per\\kilogram\\per\\square\\kelvin}$ & ' + STR_A2 + ' & $B_2$ & $\\si{\\per\\square\\kelvin}$ & '+ STR_B2 + ' \\\\' + '\n' + \
                '$A_3$ & $\\si{\\kilogram\\per\\kilogram\\per\\cubic\\kelvin}$ & '  + STR_A3 + ' & $B_3$ & $\\si{\\per\\cubic\\kelvin}$ & ' + STR_B3 + ' \\\\' + '\n' + \
                STR_EQU_TABLE_END

    # Return formatted LaTeX string
    #
    return STR_LATEX

def write_ads_dubinin_astakhov(ADS: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Generate equation and overview table of adsorption isotherm function 'DubininAstakhov'.

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
        Formatted LaTeX string describing equation and coefficient table of adsorption isotherm function.

    """
    # Get correct contents from dict
    #
    STR_E = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['E'])
    STR_N = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['n'])
    STR_W0 = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['W0'])
    STR_FLAG = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['flag'])

    # Write formatted LaTeX string
    #
    STR_UNIT_W_0 = '$\\si{\\cubic\\meter\\per\\kilogram}$' if \
        ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['flag'] >= 0 else \
        '$\\si{\\kilogram\\per\\kilogram}$'

    STR_LATEX = STR_EQU_ADS_SUR_VAP_START                                                         + \
                '\\begin{equation*}'                                                       + '\n' + \
                '\\begin{split}'                                                           + '\n' + \
                'w &=& \\begin{cases} W \\rho_\\mathrm{sat}^{\\mathrm{liq}} & \\quad \\text{if flag} \\geq 0 \\\\ W & \\quad \\text{else} \\end{cases} & \\quad\\text{, and} \\\\' + '\n' + \
                'W &=& W_0 \\exp \\left( - \\left( \\nicefrac{A}{E} \\right) ^{n} \\right) & \\quad\\text{, and} \\\\'                                                             + '\n' + \
                'A &=& R T \\ln \\left( \\nicefrac{p_\\mathrm{sat}}{p} \\right) & \\quad\\text{.} \\\\'                                                                            + '\n' + \
                '\\end{split}'                                                             + '\n' + \
                '\\end{equation*}'                                                         + '\n' + \
                STR_EQU_TABLE_START                                                               + \
                'flag & - & '                        + STR_FLAG + ' & $n$ & - & '                      + STR_N  + ' \\\\' + '\n' + \
                '$E$ & $\\si{\\joule\\per\\mole}$ & '+ STR_E    + ' & $W_0$ & ' + STR_UNIT_W_0 + ' & ' + STR_W0 + ' \\\\' + '\n' + \
                STR_EQU_TABLE_END

    # Return formatted LaTeX string
    #
    return STR_LATEX

def write_ads_dubinin_arctan1(ADS: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Generate equation and overview table of adsorption isotherm function 'DubininArctan1'.

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
        Formatted LaTeX string describing equation and coefficient table of adsorption isotherm function.

    """
    # Get correct contents from dict
    #
    STR_A = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['a'])
    STR_B = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['b'])
    STR_C = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['c'])
    STR_D = '{:.9e}'.format(ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['d'])

    # Write formatted LaTeX string
    #
    STR_LATEX = STR_EQU_ADS_SUR_VAP_START                                                         + \
                '\\begin{equation*}'                                                       + '\n' + \
                '\\begin{split}'                                                           + '\n' + \
                'w &=& W \\rho_\\mathrm{sat}^{\\mathrm{liq}} & \\quad\\text{, and} \\\\'                                                            + '\n' + \
                'W &=& \\frac{a}{\\Pi} \\left( \\arctan \\left( \\frac{A - b}{c} \\right) + \\frac{\\Pi}{2}\\right) + d & \\quad\\text{, and} \\\\' + '\n' + \
                'A &=& R T \\ln \\left( \\nicefrac{p_\\mathrm{sat}}{p} \\right) & \\quad\\text{.} \\\\'                                             + '\n' + \
                '\\end{split}'                                                             + '\n' + \
                '\\end{equation*}'                                                         + '\n' + \
                STR_EQU_TABLE_START                                                               + \
                '$a$ & $\\si{\\cubic\\meter\\per\\kilogram}$ & '    + STR_A + ' & $c$ & $\\si{\\joule\\per\\mole}$ & '            + STR_C + ' \\\\' + '\n' + \
                '$b$ & $\\si{\\joule\\per\\mole}$ & '               + STR_B + ' & $d$ & $\\si{\\cubic\\meter\\per\\kilogram}$ & ' + STR_D + ' \\\\' + '\n' + \
                STR_EQU_TABLE_END

    # Return formatted LaTeX string
    #
    return STR_LATEX

def write_ads_equation(ADS: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Select formatted LaTeX string depending on adsorption isotherm function.

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
        Formatted LaTeX string describing equation and overview table of adsorption isotherm
        function.

    """
    # Switch equations
    #
    if EQU_NAME == 'DualSiteSips':
        return write_ads_dss(ADS, EQU_NAME, EQU_ID)

    elif EQU_NAME == 'DubininArctan1':
        return write_ads_dubinin_arctan1(ADS, EQU_NAME, EQU_ID)

    elif EQU_NAME == 'DubininAstakhov':
        return write_ads_dubinin_astakhov(ADS, EQU_NAME, EQU_ID)

    elif EQU_NAME == 'Freundlich':
        return write_ads_freundlich(ADS, EQU_NAME, EQU_ID)

    elif EQU_NAME == 'Langmuir':
        return write_ads_langmuir(ADS, EQU_NAME, EQU_ID)

    elif EQU_NAME == 'Toth':
        return write_ads_toth(ADS, EQU_NAME, EQU_ID)

    else:
        # No formatted LaTeX string implemented for selected adsorption isotherm equation
        #
        return STR_EQU_START + 'For adsorption isotherm \'' + EQU_NAME.replace('_', '\_') + \
            '\', formatted LaTeX string is not implemented in manual!' + '\n\n'

def write_abs_antoine(ABS: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Generate equation and overview table of absorption isotherm function 'Antoine'.

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
        Formatted LaTeX string describing equation and coefficient table of absorption isotherm
        function.

    """
    # Get correct contents from dict
    #
    STR_A0 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['A0'])
    STR_A1 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['A1'])
    STR_A2 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['A2'])
    STR_A3 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['A3'])
    STR_A4 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['A4'])
    STR_B0 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['B0'])
    STR_B1 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['B1'])
    STR_B2 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['B2'])
    STR_B3 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['B3'])
    STR_B4 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['B4'])
    STR_C = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['c'])
    STR_D = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['d'])

    # Write formatted LaTeX string
    #
    STR_LATEX = STR_EQU_ABS_CON_START                                                             + \
                '\\begin{equation*}'                                                                                                    + '\n' + \
                '\\nicefrac{p}{d} = 10 ^ { \\sum_{i=0}^{4} \\left( A_i + \\frac{1000 B_i}{T - c} \\right) \\left( 100 X \\right) ^{i}}' + '\n' + \
                '\\end{equation*}'                                                                                                      + '\n' + \
                STR_EQU_TABLE_START                                                               + \
                '$c$ & $\\si{\\kelvin}$ & ' + STR_C  + ' & $d$ & $\\si{\\pascal}$ & '   + STR_D + ' \\\\'  + '\n' + \
                '$A_0$ & - & '              + STR_A0 + ' & $B_0$ & $\\si{\\kelvin}$ & ' + STR_B0 + ' \\\\' + '\n' + \
                '$A_1$ & - & '              + STR_A1 + ' & $B_1$ & $\\si{\\kelvin}$ & ' + STR_B1 + ' \\\\' + '\n' + \
                '$A_2$ & - & '              + STR_A2 + ' & $B_2$ & $\\si{\\kelvin}$ & ' + STR_B2 + ' \\\\' + '\n' + \
                '$A_3$ & - & '              + STR_A3 + ' & $B_3$ & $\\si{\\kelvin}$ & ' + STR_B3 + ' \\\\' + '\n' + \
                '$A_4$ & - & '              + STR_A4 + ' & $B_4$ & $\\si{\\kelvin}$ & ' + STR_B4 + ' \\\\' + '\n' + \
                STR_EQU_TABLE_END

    # Return formatted LaTeX string
    #
    return STR_LATEX

def write_abs_duehring(ABS: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Generate equation and overview table of absorption isotherm function 'Duehring'.

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
        Formatted LaTeX string describing equation and coefficient table of absorption isotherm
        function.

    """
    # Get correct contents from dict
    #
    STR_A0 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['a0'])
    STR_A1 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['a1'])
    STR_A2 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['a2'])
    STR_A3 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['a3'])
    STR_B0 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['b0'])
    STR_B1 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['b1'])
    STR_B2 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['b2'])
    STR_B3 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['b3'])
    STR_C = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['C'])
    STR_D = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['D'])
    STR_E = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['E'])
    STR_M = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['m'])
    STR_N = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['n'])
    STR_Q = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['q'])
    STR_R = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['r'])

    # Write formatted LaTeX string
    #
    STR_LATEX = STR_EQU_ABS_CON_START                                                           + \
                '\\begin{equation*}'                                                     + '\n' + \
                '\\begin{split}'                                                         + '\n' + \
                'p &=& \\nicefrac{1}{r} 10 ^ { C + \\frac{D}{T_\\mathrm{ref}} + \\frac{E}{T_\\mathrm{ref}^{2}} } & \\quad\\text{, and} \\\\' + '\n' + \
                'T_\\mathrm{ref} &=& q + \\frac{\\left( n T + m - B \\right)}{A}  & \\quad\\text{, and} \\\\'                                + '\n' + \
                'A &=& \\sum_{i=0}^{3} a_0 X_\\mathrm{cor} ^{i}  & \\quad\\text{, and} \\\\'                                                 + '\n' + \
                'B &=& \\sum_{i=0}^{3} b_0 X_\\mathrm{cor} ^{i}  & \\quad\\text{, and} \\\\'                                                 + '\n' + \
                'X_\\mathrm{cor} &=& 100 X  & \\quad\\text{.} \\\\'                                                                          + '\n' + \
                '\\end{split}'                                                           + '\n' + \
                '\\end{equation*}'                                                       + '\n' + \
                STR_EQU_TABLE_START                                                             + \
                '$a_0$ & - & '                   + STR_A0  + ' & $b_0$ & - & '                      + STR_B0 + ' \\\\'  + '\n' + \
                '$a_1$ & - & '                   + STR_A1  + ' & $b_1$ & - & '                      + STR_B1 + ' \\\\'  + '\n' + \
                '$a_2$ & - & '                   + STR_A2  + ' & $b_2$ & - & '                      + STR_B2 + ' \\\\'  + '\n' + \
                '$a_3$ & - & '                   + STR_A3  + ' & $b_3$ & - & '                      + STR_B3 + ' \\\\'  + '\n' + \
                '$C$ & - & '                     + STR_C   + ' & $D$ & $\\si{\\kelvin}$ & '         + STR_D  + ' \\\\'  + '\n' + \
                '$m$ & - & '                     + STR_M   + ' & $E$ & $\\si{\\square\\kelvin}$ & ' + STR_E  + ' \\\\'  + '\n' + \
                '$n$ & - & '                     + STR_N   + ' & $q$ & - & '                        + STR_Q  + ' \\\\'  + '\n' + \
                '$r$ & $\\si{\\per\\pascal}$ & ' + STR_R   + ' & & & \\\\'                                              + '\n' + \
                STR_EQU_TABLE_END

    # Return formatted LaTeX string
    #
    return STR_LATEX

def write_abs_wilson_fdl(ABS: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Generate equation and overview table of absorption isotherm function 'WilsonFixedDl'.

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
        Formatted LaTeX string describing equation and coefficient table of absorption isotherm
        function.

    """
    # Get correct contents from dict
    #
    STR_GL12 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['A_12'])
    STR_GL21 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['A_21'])
    STR_L12 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['d_lambda_12'])
    STR_L21 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['d_lambda_21'])
    STR_V1 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['v_1'])
    STR_V2 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['v_2'])

    # Write formatted LaTeX string
    #
    STR_LATEX = STR_EQU_ABS_ACT_START                                                           + \
                '\\begin{equation*}'                                                     + '\n' + \
                '\\begin{split}'                                                         + '\n' + \
                'p &=& \\gamma_1 x_1 p_\\mathrm{sat,1} & \\quad\\text{, and} \\\\'                                                                                                                                                                                       + '\n' + \
                '\\gamma_1 &=& \\exp \\left( - \\ln \\left( x_1 + \\Lambda_{12} x_2 \\right) + x_2 \\left( \\frac{\\Lambda_{12}}{x_1 + \\Lambda_{12} x_2} - \\frac{\\Lambda_{21}}{x_2 + \\Lambda_{21} x_1} \\right) \\right) & \\quad\\text{, and} \\\\'                 + '\n' + \
                '\\Lambda_{12} &=& \\begin{cases} \\nicefrac{v_2}{v_1} \\exp \\left( - \\nicefrac{\\Delta\\lambda_{12}}{R T} \\right) & \\quad \\text{if } \\Lambda_{12}^{*} = 0 \\\\ \\Lambda_{12}^{*}  & \\quad \\text{else} \\end{cases}  & \\quad\\text{, and} \\\\' + '\n' + \
                '\\Lambda_{21} &=& \\begin{cases} \\nicefrac{v_1}{v_2} \\exp \\left( - \\nicefrac{\\Delta\\lambda_{21}}{R T} \\right) & \\quad \\text{if } \\Lambda_{21}^{*} = 0 \\\\ \\Lambda_{21}^{*}  & \\quad \\text{else} \\end{cases}  & \\quad\\text{, and} \\\\' + '\n' + \
                'x_2 &=& 1 - x_1  & \\quad\\text{.} \\\\'                                                                                                                                                                                                                + '\n' + \
                '\\end{split}'                                                           + '\n' + \
                '\\end{equation*}'                                                       + '\n' + \
                STR_EQU_TABLE_START                                                             + \
                '$\\Lambda_{12}^{*}$ & - & '                             + STR_GL12 + ' & $\\Lambda_{21}^{*}$ & - & '                             + STR_GL21 + ' \\\\'  + '\n' + \
                '$\\Delta\\lambda_{12}$ & $\\si{\\joule\\per\\mole}$ & ' + STR_L12  + ' & $\\Delta\\lambda_{21}$ & $\\si{\\joule\\per\\mole}$ & ' + STR_L21  + ' \\\\'  + '\n' + \
                '$v_1$ & $\\si{\\cubic\\meter\\per\\mole}$ & '           + STR_V1   + ' & $v_2$ & $\\si{\\cubic\\meter\\per\\mole}$ & '           + STR_V2   + ' \\\\'  + '\n' + \
                STR_EQU_TABLE_END

    # Return formatted LaTeX string
    #
    return STR_LATEX

def write_abs_wilson_tdl(ABS: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Generate equation and overview table of absorption isotherm function 'WilsonTemperatureDl'.

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
        Formatted LaTeX string describing equation and coefficient table of absorption isotherm
        function.

    """
    # Get correct contents from dict
    #
    STR_L12C = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['d_lambda_12_c'])
    STR_L21C = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['d_lambda_21_c'])
    STR_L12T = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['d_lambda_12_T'])
    STR_L21T = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['d_lambda_21_T'])
    STR_V1 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['v_1'])
    STR_V2 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['v_2'])
    STR_C = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['c'])

    # Write formatted LaTeX string
    #
    STR_LATEX = STR_EQU_ABS_ACT_START                                                           + \
                '\\begin{equation*}'                                                     + '\n' + \
                '\\begin{split}'                                                         + '\n' + \
                'p &=& \\gamma_1 x_1 p_\\mathrm{sat,1} & \\quad\\text{, and} \\\\'                                                                                                                                                                      + '\n' + \
                '\\gamma_1 &=& \\exp \\left( - \\ln \\left( x_1 + \\Lambda_{12} x_2 \\right) + x_2 \\left( \\frac{\\Lambda_{12}}{x_1 + \\Lambda_{12} x_2} - \\frac{\\Lambda_{21}}{x_2 + \\Lambda_{21} x_1} \\right) \\right) & \\quad\\text{, and} \\\\'+ '\n' + \
                '\\Lambda_{12} &=& \\nicefrac{v_2}{v_1} \\exp \\left( - \\frac{\\Delta\\lambda_{12}}{R T} \\right) & \\quad\\text{, and} \\\\'                                                                                                          + '\n' + \
                '\\Lambda_{21} &=& \\nicefrac{v_1}{v_2} \\exp \\left( - \\frac{\\Delta\\lambda_{21}}{R T} \\right) & \\quad\\text{, and} \\\\'                                                                                                          + '\n' + \
                '\\Delta\\lambda_{12} &=& R \\left( \\Delta\\lambda_\\mathrm{12,c} + \\Delta\\lambda_\\mathrm{12,t} \\left( T - c \\right) \\right) & \\quad\\text{, and} \\\\'                                                                         + '\n' + \
                '\\Delta\\lambda_{21} &=& R \\left( \\Delta\\lambda_\\mathrm{21,c} + \\Delta\\lambda_\\mathrm{21,t} \\left( T - c \\right) \\right) & \\quad\\text{, and} \\\\'                                                                         + '\n' + \
                'x_2 &=& 1 - x_1  & \\quad\\text{.} \\\\'                                                                                                                                                                                               + '\n' + \
                '\\end{split}'                                                           + '\n' + \
                '\\end{equation*}'                                                       + '\n' + \
                STR_EQU_TABLE_START                                                             + \
                '$\\Delta\\lambda_\\mathrm{12,c}$ & $\\si{\\kelvin}$ & '+ STR_L12C + ' & $\\Delta\\lambda_\\mathrm{21,c}$ & $\\si{\\kelvin}$ & ' + STR_L21C + ' \\\\'  + '\n' + \
                '$\\Delta\\lambda_\\mathrm{12,t}$ & - & '               + STR_L12T + ' & $\\Delta\\lambda_\\mathrm{12,t}$ & - & '                + STR_L21T + ' \\\\'  + '\n' + \
                '$v_1$ & $\\si{\\cubic\\meter\\per\\mole}$ & '          + STR_V1   + ' & $v_2$ & $\\si{\\cubic\\meter\\per\\mole}$ & '           + STR_V2   + ' \\\\'  + '\n' + \
                '$c$ & $\\si{\\kelvin}$ & '                             + STR_C    + ' & & & \\\\'                                                                     + '\n' + \
                STR_EQU_TABLE_END

    # Return formatted LaTeX string
    #
    return STR_LATEX

def write_abs_tsuboka_katayama(ABS: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Generate equation and overview table of absorption isotherm function 'TsubokaKatayama'.

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
        Formatted LaTeX string describing equation and coefficient table of absorption isotherm
        function.

    """
    # Get correct contents from dict
    #
    STR_L12 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['d_lambda_12'])
    STR_L21 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['d_lambda_21'])
    STR_V1 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['v_1'])
    STR_V2 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['v_2'])

    # Write formatted LaTeX string
    #
    STR_LATEX = STR_EQU_ABS_ACT_START                                                           + \
                '\\begin{equation*}'                                                     + '\n' + \
                '\\begin{split}'                                                         + '\n' + \
                'p &=& \\gamma_1 x_1 p_\\mathrm{sat,1} & \\quad\\text{, and} \\\\'                                                                                                                                                                               + '\n' + \
                '\\gamma_1 &=& \\exp \\left( - \\ln \\left( x_1 + \\Lambda_{21} x_2 \\right) + x_2 \\left( \\frac{\\Lambda_{21}}{x_1 + \\Lambda_{21} x_2} - \\frac{\\Lambda_{12}}{x_1 \\Lambda_{12} + x_2} \\right) + \\Phi \\right) & \\quad\\text{, and} \\\\' + '\n' + \
                '\\Phi &=& \\ln \\left( x_1 + x_2 \\rho_{21} \\right) - x_2 \\left( \\frac{\\rho_{21}}{x_1 + x_2 \\rho_{21}} - \\frac{\\rho_{12}}{x_1 \\rho_{12} + x_2} \\right) & \\quad\\text{, and} \\\\'                                                     + '\n' + \
                '\\Lambda_{12} &=& \\rho_{21} \\exp \\left( - \\nicefrac{\\Delta\\lambda_{12}}{R T} \\right) & \\quad\\text{, and} \\\\'                                                                                                                         + '\n' + \
                '\\Lambda_{21} &=& \\rho_{12} \\exp \\left( - \\nicefrac{\\Delta\\lambda_{21}}{R T} \\right) & \\quad\\text{, and} \\\\'                                                                                                                         + '\n' + \
                '\\rho_{12} &=& \\nicefrac{v_1}{v_2} & \\quad\\text{, and} \\\\'                                                                                                                                                                                 + '\n' + \
                '\\rho_{21} &=& \\nicefrac{v_2}{v_1} & \\quad\\text{, and} \\\\'                                                                                                                                                                                 + '\n' + \
                'x_2 &=& 1 - x_1  & \\quad\\text{.} \\\\'                                                                                                                                                                                                        + '\n' + \
                '\\end{split}'                                                           + '\n' + \
                '\\end{equation*}'                                                       + '\n' + \
                STR_EQU_TABLE_START                                                             + \
                '$\\Delta\\lambda_{12}$ & $\\si{\\joule\\per\\mole}$ & ' + STR_L12  + ' & $\\Delta\\lambda_{21}$ & $\\si{\\joule\\per\\mole}$ & ' + STR_L21  + ' \\\\'  + '\n' + \
                '$v_1$ & $\\si{\\cubic\\meter\\per\\mole}$ & '           + STR_V1   + ' & $v_2$ & $\\si{\\cubic\\meter\\per\\mole}$ & '           + STR_V2   + ' \\\\'  + '\n' + \
                STR_EQU_TABLE_END

    # Return formatted LaTeX string
    #
    return STR_LATEX

def write_abs_wang_chao(ABS: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Generate equation and overview table of absorption isotherm function 'WangChao'.

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
        Formatted LaTeX string describing equation and coefficient table of absorption isotherm
        function.

    """
    # Get correct contents from dict
    #
    STR_L12 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['d_lambda_12'])
    STR_L21 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['d_lambda_21'])
    STR_V1 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['v_1'])
    STR_V2 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['v_2'])
    STR_Z = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['z'])

    # Write formatted LaTeX string
    #
    STR_LATEX = STR_EQU_ABS_ACT_START                                                           + \
                '\\begin{equation*}'                                                     + '\n' + \
                '\\begin{split}'                                                         + '\n' + \
                'p &=& \\gamma_1 x_1 p_\\mathrm{sat,1} & \\quad\\text{, and} \\\\'                                                                                                                                                                               + '\n' + \
                '\\gamma_1 &=& \\exp \\left( - \\ln \\left( x_1 + \\Lambda_{21} x_2 \\right) + x_2 \\left( \\frac{\\Lambda_{21}}{x_1 + \\Lambda_{21} x_2} - \\frac{\\Lambda_{12}}{x_1 \\Lambda_{12} + x_2} \\right) + \\Phi \\right) & \\quad\\text{, and} \\\\' + '\n' + \
                '\\Phi &=& \\frac{1}{R T} \\frac{z}{2} \\left( x_{21}^2 \\Delta\\lambda_{21} + x_2 x_{22} \\frac{x_{12}}{x_1} \\Delta\\lambda_{12} \\right) & \\quad\\text{, and} \\\\'                                                                          + '\n' + \
                'x_{11} &=& \\left( 1 + \\nicefrac{x_2}{x_1} \\exp \\left( - \\nicefrac{\\Delta\\lambda_{21}}{R T} \\right) \\right) ^ {-1} & \\quad\\text{, and} \\\\'                                                                                          + '\n' + \
                'x_{22} &=& \\left( 1 + \\nicefrac{x_1}{x_2} \\exp \\left( - \\nicefrac{\\Delta\\lambda_{12}}{R T} \\right) \\right) ^ {-1} & \\quad\\text{, and} \\\\'                                                                                          + '\n' + \
                '\\Lambda_{12} &=& \\rho_{21} \\exp \\left( - \\frac{\\Delta\\lambda_{12}}{R T} \\right) & \\quad\\text{, and} \\\\'                                                                                                                             + '\n' + \
                '\\Lambda_{21} &=& \\rho_{12} \\exp \\left( - \\frac{\\Delta\\lambda_{21}}{R T} \\right) & \\quad\\text{, and} \\\\'                                                                                                                             + '\n' + \
                '\\rho_{12} &=& \\nicefrac{v_1}{v_2} & \\quad\\text{, and} \\\\'                                                                                                                                                                                 + '\n' + \
                '\\rho_{21} &=& \\nicefrac{v_2}{v_1} & \\quad\\text{, and} \\\\'                                                                                                                                                                                 + '\n' + \
                'x_{12} &=& 1 - x_{22} & \\quad\\text{, and} \\\\'                                                                                                                                                                                               + '\n' + \
                'x_{21} &=& 1 - x_{11} & \\quad\\text{, and} \\\\'                                                                                                                                                                                               + '\n' + \
                'x_2 &=& 1 - x_1  & \\quad\\text{.} \\\\'                                                                                                                                                                                                        + '\n' + \
                '\\end{split}'                                                           + '\n' + \
                '\\end{equation*}'                                                       + '\n' + \
                STR_EQU_TABLE_START                                                             + \
                '$\\Delta\\lambda_{12}$ & $\\si{\\joule\\per\\mole}$ & ' + STR_L12 + ' & $\\Delta\\lambda_{21}$ & $\\si{\\joule\\per\\mole}$ & ' + STR_L21  + ' \\\\'  + '\n' + \
                '$v_1$ & $\\si{\\cubic\\meter\\per\\mole}$ & '           + STR_V1  + ' & $v_2$ & $\\si{\\cubic\\meter\\per\\mole}$ & '           + STR_V2   + ' \\\\'  + '\n' + \
                '$z$ & - & '                                             + STR_Z   + ' & & & \\\\'                                                                     + '\n' + \
                STR_EQU_TABLE_END

    # Return formatted LaTeX string
    #
    return STR_LATEX

def write_abs_heil(ABS: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Generate equation and overview table of absorption isotherm function 'Heil'.

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
        Formatted LaTeX string describing equation and coefficient table of absorption isotherm
        function.

    """
    # Get correct contents from dict
    #
    STR_L12 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['d_lambda_12'])
    STR_L21 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['d_lambda_21'])
    STR_V1 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['v_1'])
    STR_V2 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['v_2'])

    # Write formatted LaTeX string
    #
    STR_LATEX = STR_EQU_ABS_ACT_START                                                           + \
                '\\begin{equation*}'                                                     + '\n' + \
                '\\begin{split}'                                                         + '\n' + \
                'p &=& \\gamma_1 x_1 p_\\mathrm{sat,1} & \\quad\\text{, and} \\\\'                                                                                                                                                                                + '\n' + \
                '\\gamma_1 &=& \\exp \\left( - \\ln \\left( x_1 + \\Lambda_{21} x_2 \\right) + x_2 \\left( \\frac{\\Lambda_{21}}{x_1 + \\Lambda_{21} x_2} - \\frac{\\Lambda_{12}}{x_1 \\Lambda_{12} + x_2} \\right) + \\Phi \\right) & \\quad\\text{, and} \\\\'  + '\n' + \
                '\\Phi &=& x_2^2 \\left( \\tau_{12} \\left( \\frac{\\Lambda_{21}}{x_1 + \\Lambda_{21} x_2} \\right) ^2 + \\tau_{12} \\frac{\\Lambda_{12}}{\\left( x_1 \\Lambda_{12} + x_2 \\right) ^2}  \\right) & \\quad\\text{, and} \\\\'                      + '\n' + \
                '\\Delta\\Lambda_{12} &=& \\nicefrac{v_2}{v_1} \\exp \\left( - \\tau_{12} \\right) & \\quad\\text{, and} \\\\'                                                                                                                                    + '\n' + \
                '\\Delta\\Lambda_{21} &=& \\nicefrac{v_1}{v_2} \\exp \\left( - \\tau_{21} \\right) & \\quad\\text{, and} \\\\'                                                                                                                                    + '\n' + \
                '\\tau_{12} &=& \\nicefrac{\\lambda_{12}}{R T} & \\quad\\text{, and} \\\\'                                                                                                                                                                        + '\n' + \
                '\\tau_{21} &=& \\nicefrac{\\lambda_{21}}{R T} & \\quad\\text{, and} \\\\'                                                                                                                                                                        + '\n' + \
                'x_2 &=& 1 - x_1  & \\quad\\text{.} \\\\'                                                                                                                                                                                                         + '\n' + \
                '\\end{split}'                                                           + '\n' + \
                '\\end{equation*}'                                                       + '\n' + \
                STR_EQU_TABLE_START                                                             + \
                '$\\Delta\\lambda_{12}$ & $\\si{\\joule\\per\\mole}$ & ' + STR_L12  + ' & $\\Delta\\lambda_{21}$ & $\\si{\\joule\\per\\mole}$ & ' + STR_L21  + ' \\\\'  + '\n' + \
                '$v_1$ & $\\si{\\cubic\\meter\\per\\mole}$ & '           + STR_V1   + ' & $v_2$ & $\\si{\\cubic\\meter\\per\\mole}$ & '           + STR_V2   + ' \\\\'  + '\n' + \
                STR_EQU_TABLE_END

    # Return formatted LaTeX string
    #
    return STR_LATEX

def write_abs_flory_huggins(ABS: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Generate equation and overview table of absorption isotherm function 'FloryHuggins'.

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
        Formatted LaTeX string describing equation and coefficient table of absorption isotherm
        function.

    """
    # Get correct contents from dict
    #
    STR_R = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['r'])
    STR_WK = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['w_0 / k_Boltzmann'])
    STR_W1 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['w_1'])

    # Write formatted LaTeX string
    #
    STR_LATEX = STR_EQU_ABS_ACT_START                                                           + \
                '\\begin{equation*}'                                                     + '\n' + \
                '\\begin{split}'                                                         + '\n' + \
                'p &=& \\gamma_1 x_1 p_\\mathrm{sat,1} & \\quad\\text{, and} \\\\'                                                                                                                                            + '\n' + \
                '\\gamma_1 &=& \\exp \\left( \\ln \\left( 1 - \\left( 1 - \\nicefrac{1}{r} \\right) \\Phi_2  \\right) + \\left(1 - \\nicefrac{1}{r}\\right) \\Phi_2 + \\Gamma \\Phi_2 ^2 \\right) & \\quad\\text{, and} \\\\' + '\n' + \
                '\\Phi_2 &=& r \\frac{x_2}{x_1 + r x_2} & \\quad\\text{, and} \\\\'                                                                                                                                           + '\n' + \
                '\\Gamma &=& \\nicefrac{w_0^{*}}{T} \\left( 1 + \\nicefrac{w_1}{T} \\right) & \\quad\\text{, and} \\\\'                                                                                                       + '\n' + \
                'x_2 &=& 1 - x_1  & \\quad\\text{.} \\\\'                                                                                                                                                                     + '\n' + \
                '\\end{split}'                                                           + '\n' + \
                '\\end{equation*}'                                                       + '\n' + \
                STR_EQU_TABLE_START                                                             + \
                '$r$ & - & '                  + STR_R  + ' & $w_0^{*}$ & $\\si{\\kelvin}$ & ' + STR_WK  + ' \\\\'  + '\n' + \
                '$w_1$ & $\\si{\\kelvin}$ & ' + STR_W1 + ' & & & \\\\'                                             + '\n' + \
                STR_EQU_TABLE_END

    # Return formatted LaTeX string
    #
    return STR_LATEX

def write_abs_nrtl_fdg(ABS: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Generate equation and overview table of absorption isotherm function 'NrtlFixedDg'.

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
        Formatted LaTeX string describing equation and coefficient table of absorption isotherm
        function.

    """
    # Get correct contents from dict
    #
    STR_DG12 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['d_g_12'])
    STR_DG21 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['d_g_21'])
    STR_ALPHA12 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['alpha_12'])

    # Write formatted LaTeX string
    #
    STR_LATEX = STR_EQU_ABS_ACT2_START                                                          + \
                '\\begin{equation*}'                                                     + '\n' + \
                '\\begin{split}'                                                         + '\n' + \
                'p &=& \\gamma_1 x_1 p_\\mathrm{sat,1} & \\quad\\text{, and} \\\\'                                                                                                                                                        + '\n' + \
                '\\gamma_1 &=& \\exp \\left( x_2^2 \\left( \\tau_{21} \\left( \\frac{G_{21}}{x_1 + x_2 G_{21}} \\right) ^2 + \\tau_{12} \\frac{G_{12}}{\\left( x_2 + x_1 G_{12} \\right) ^2}\\right) \\right) & \\quad\\text{, and} \\\\' + '\n' + \
                'G_{12} &=& \\exp \\left( -\\alpha_{12} \\tau_{12} \\right) & \\quad\\text{, and} \\\\'                                                                                                                                   + '\n' + \
                'G_{21} &=& \\exp \\left( -\\alpha_{21} \\tau_{21} \\right) & \\quad\\text{, and} \\\\'                                                                                                                                   + '\n' + \
                '\\tau_{12} &=& \\nicefrac{\\Delta g_{12}}{R T} & \\quad\\text{, and} \\\\'                                                                                                                                               + '\n' + \
                '\\tau_{21} &=& \\nicefrac{\\Delta g_{21}}{R T} & \\quad\\text{, and} \\\\'                                                                                                                                               + '\n' + \
                'x_2 &=& 1 - x_1  & \\quad\\text{.} \\\\'                                                                                                                                                                                 + '\n' + \
                '\\end{split}'                                                           + '\n' + \
                '\\end{equation*}'                                                       + '\n' + \
                STR_EQU_TABLE_START                                                             + \
                '$\\alpha_{12}$ & - & '                            + STR_ALPHA12 + ' & $\\alpha_{21}$ & - & '                            + STR_ALPHA12  + ' \\\\'  + '\n' + \
                '$\\Delta g_{12}$ & $\\si{\\joule\\per\\mole}$ & ' + STR_DG12 +    ' & $\\Delta g_{21}$ & $\\si{\\joule\\per\\mole}$ & ' + STR_DG21     + ' \\\\'  + '\n' + \
                STR_EQU_TABLE_END

    # Return formatted LaTeX string
    #
    return STR_LATEX

def write_abs_nrtl_tdg(ABS: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Generate equation and overview table of absorption isotherm function 'NrtlTemperatureDg'.

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
        Formatted LaTeX string describing equation and coefficient table of absorption isotherm
        function.

    """
    # Get correct contents from dict
    #
    STR_A12 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['a_12'])
    STR_A21 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['a_21'])
    STR_B12 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['b_12'])
    STR_B21 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['b_21'])
    STR_ALPHA12 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['alpha_12'])

    # Write formatted LaTeX string
    #
    STR_LATEX = STR_EQU_ABS_ACT2_START                                                          + \
                '\\begin{equation*}'                                                     + '\n' + \
                '\\begin{split}'                                                         + '\n' + \
                'p &=& \\gamma_1 x_1 p_\\mathrm{sat,1} & \\quad\\text{, and} \\\\'                                                                                                                                                        + '\n' + \
                '\\gamma_1 &=& \\exp \\left( x_2^2 \\left( \\tau_{21} \\left( \\frac{G_{21}}{x_1 + x_2 G_{21}} \\right) ^2 + \\tau_{12} \\frac{G_{12}}{\\left( x_2 + x_1 G_{12} \\right) ^2}\\right) \\right) & \\quad\\text{, and} \\\\' + '\n' + \
                'G_{12} &=& \\exp \\left( -\\alpha_{12} \\tau_{12} \\right) & \\quad\\text{, and} \\\\'                                                                                                                                   + '\n' + \
                'G_{21} &=& \\exp \\left( -\\alpha_{21} \\tau_{21} \\right) & \\quad\\text{, and} \\\\'                                                                                                                                   + '\n' + \
                '\\tau_{12} &=& \\nicefrac{\\Delta g_{12}}{R T} & \\quad\\text{, and} \\\\'                                                                                                                                               + '\n' + \
                '\\tau_{21} &=& \\nicefrac{\\Delta g_{21}}{R T} & \\quad\\text{, and} \\\\'                                                                                                                                               + '\n' + \
                '\\Delta g_{12} &=& a_{12} + b_{12} T & \\quad\\text{, and} \\\\'                                                                                                                                                         + '\n' + \
                '\\Delta g_{21} &=& a_{21} + b_{21} T & \\quad\\text{, and} \\\\'                                                                                                                                                         + '\n' + \
                'x_2 &=& 1 - x_1  & \\quad\\text{.} \\\\'                                                                                                                                                                                 + '\n' + \
                '\\end{split}'                                                           + '\n' + \
                '\\end{equation*}'                                                       + '\n' + \
                STR_EQU_TABLE_START                                                             + \
                '$\\alpha_{12}$ & - & '                                 + STR_ALPHA12 + ' & $\\alpha_{21}$ & - & '                                + STR_ALPHA12 + ' \\\\'  + '\n' + \
                '$a_{12}$ & $\\si{\\joule\\per\\mole}$ & '              + STR_A12 +    ' & $a_{21}$ & $\\si{\\joule\\per\\mole}$ & '              + STR_A21     + ' \\\\'  + '\n' + \
                '$b_{12}$ & $\\si{\\joule\\per\\mole\\per\\kelvin}$ & ' + STR_B12 +    ' & $b_{21}$ & $\\si{\\joule\\per\\mole\\per\\kelvin}$ & ' + STR_B21     + ' \\\\'  + '\n' + \
                STR_EQU_TABLE_END

    # Return formatted LaTeX string
    #
    return STR_LATEX

def write_abs_uniquac_fdu(ABS: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Generate equation and overview table of absorption isotherm function 'UniquacFixedDu'.

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
        Formatted LaTeX string describing equation and coefficient table of absorption isotherm
        function.

    """
    # Get correct contents from dict
    #
    STR_DU12 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['d_u_12'])
    STR_DU21 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['d_u_21'])
    STR_R1 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['r_1'])
    STR_R2 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['r_2'])
    STR_Q1 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['q_1'])
    STR_Q2 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['q_2'])
    STR_Z = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['z'])

    # Write formatted LaTeX string
    #
    STR_LATEX = STR_EQU_ABS_ACT2_START                                                          + \
                '\\begin{equation*}'                                                     + '\n' + \
                '\\begin{split}'                                                         + '\n' + \
                'p &=& \\gamma_1 x_1 p_\\mathrm{sat,1} & \\quad\\text{, and} \\\\'                                                                                                                                                                                                         + '\n' + \
                '\\gamma_1 &=& \\exp \\left( \\ln \\left( \\nicefrac{\\Phi_1}{x_1} \\right) + q_1 \\nicefrac{z}{2} \\ln \\left( \\nicefrac{\\Theta_1}{\\Phi_1} \\right) + l_1 - \\nicefrac{\\Phi_1}{x_2} \\left( x_1 l_1 + x_2 l_2 \\right) + \\Gamma \\right) & \\quad\\text{, and} \\\\' + '\n' + \
                '\\Gamma &=& - q_1 \\ln \\left( \\Theta_1 + \\Theta_2 \\tau_{21} \\right) + q_1 - q_1 \\left( \\frac{\\Theta_1}{\\Theta_1 + \\Theta_2 \\tau_{21}} + \\frac{\\Theta_2 \\tau_{12}}{\\Theta_1 \\tau_{12} + \\Theta_2} \\right) & \\quad\\text{, and} \\\\'                    + '\n' + \
                '\\tau_{ij} &=& \\exp \\left( \\nicefrac{\\Delta u_{ij}}{R T} \\right) & \\quad\\text{, and} \\\\'                                                                                                                                                                         + '\n' + \
                'l_i &=& \\nicefrac{z}{2} \\left( r_i - q_i \\right) \\left(r_i - 1 \\right) & \\quad\\text{, and} \\\\'                                                                                                                                                                   + '\n' + \
                '\\Theta_i &=& \\frac{q_i x_i}{\\sum_{j=1}^{2} q_j x_j} & \\quad\\text{, and} \\\\'                                                                                                                                                                                        + '\n' + \
                '\\Phi_i &=& \\frac{r_i x_i}{\\sum_{j=1}^{2} r_j x_j} & \\quad\\text{, and} \\\\'                                                                                                                                                                                          + '\n' + \
                'x_2 &=& 1 - x_1  & \\quad\\text{.} \\\\'                                                                                                                                                                                                                                  + '\n' + \
                '\\end{split}'                                                           + '\n' + \
                '\\end{equation*}'                                                       + '\n' + \
                STR_EQU_TABLE_START                                                             + \
                '$\\Delta u_{12}$ & $\\si{\\joule\\per\\mole}$ & ' + STR_DU12 + ' & $\\Delta u_{21}$ & $\\si{\\joule\\per\\mole}$ & ' + STR_DU21 + ' \\\\'  + '\n' + \
                '$r_{1}$ & - & '                                   + STR_R1 +   ' & $r_{2}$ & - & '                                   + STR_R2   + ' \\\\'  + '\n' + \
                '$q_{1}$ & - & '                                   + STR_Q1 +   ' & $q_{2}$ & - & '                                   + STR_Q2   + ' \\\\'  + '\n' + \
                '$z$ & - & '                                       + STR_Z +    ' & & &  \\\\'                                                   + '\n' + \
                STR_EQU_TABLE_END

    # Return formatted LaTeX string
    #
    return STR_LATEX

def write_abs_uniquac_tdu(ABS: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Generate equation and overview table of absorption isotherm function 'UniquacTemperatureDu'.

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
        Formatted LaTeX string describing equation and coefficient table of absorption isotherm
        function.

    """
    # Get correct contents from dict
    #
    STR_A12 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['a_12'])
    STR_A21 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['a_21'])
    STR_B12 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['b_12'])
    STR_B21 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['b_21'])
    STR_R1 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['r_1'])
    STR_R2 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['r_2'])
    STR_Q1 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['q_1'])
    STR_Q2 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['q_2'])
    STR_Z = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['z'])

    # Write formatted LaTeX string
    #
    STR_LATEX = STR_EQU_ABS_ACT2_START                                                          + \
                '\\begin{equation*}'                                                     + '\n' + \
                '\\begin{split}'                                                         + '\n' + \
                'p &=& \\gamma_1 x_1 p_\\mathrm{sat,1} & \\quad\\text{, and} \\\\'                                                                                                                                                                                                         + '\n' + \
                '\\gamma_1 &=& \\exp \\left( \\ln \\left( \\nicefrac{\\Phi_1}{x_1} \\right) + q_1 \\nicefrac{z}{2} \\ln \\left( \\nicefrac{\\Theta_1}{\\Phi_1} \\right) + l_1 - \\nicefrac{\\Phi_1}{x_2} \\left( x_1 l_1 + x_2 l_2 \\right) + \\Gamma \\right) & \\quad\\text{, and} \\\\' + '\n' + \
                '\\Gamma &=& - q_1 \\ln \\left( \\Theta_1 + \\Theta_2 \\tau_{21} \\right) + q_1 - q_1 \\left( \\frac{\\Theta_1}{\\Theta_1 + \\Theta_2 \\tau_{21}} + \\frac{\\Theta_2 \\tau_{12}}{\\Theta_1 \\tau_{12} + \\Theta_2} \\right) & \\quad\\text{, and} \\\\'                    + '\n' + \
                '\\tau_{ij} &=& \\exp \\left( \\nicefrac{\\Delta u_{ij}}{R T} \\right) & \\quad\\text{, and} \\\\'                                                                                                                                                                         + '\n' + \
                '\\Delta u_{ij} &=& a_{ij} + b_{ij} T & \\quad\\text{, and} \\\\'                                                                                                                                                                                                          + '\n' + \
                'l_i &=& \\nicefrac{z}{2} \\left( r_i - q_i \\right) \\left(r_i - 1 \\right) & \\quad\\text{, and} \\\\'                                                                                                                                                                   + '\n' + \
                '\\Theta_i &=& \\frac{q_i x_i}{\\sum_{j=1}^{2} q_j x_j} & \\quad\\text{, and} \\\\'                                                                                                                                                                                        + '\n' + \
                '\\Phi_i &=& \\frac{r_i x_i}{\\sum_{j=1}^{2} r_j x_j} & \\quad\\text{, and} \\\\'                                                                                                                                                                                          + '\n' + \
                'x_2 &=& 1 - x_1  & \\quad\\text{.} \\\\'                                                                                                                                                                                                                                  + '\n' + \
                '\\end{split}'                                                           + '\n' + \
                '\\end{equation*}'                                                       + '\n' + \
                STR_EQU_TABLE_START                                                             + \
                '$a_{12}$ & $\\si{\\joule\\per\\mole}$ & '              + STR_A12 + ' & $a_{21}$ & $\\si{\\joule\\per\\mole}$ & '              + STR_A21 + ' \\\\'  + '\n' + \
                '$b_{12}$ & $\\si{\\joule\\per\\mole\\per\\kelvin}$ & ' + STR_B12 + ' & $b_{21}$ & $\\si{\\joule\\per\\mole\\per\\kelvin}$ & ' + STR_B21 + ' \\\\'  + '\n' + \
                '$r_{1}$ & - & '                                        + STR_R1  +   ' & $r_{2}$ & - & '                                      + STR_R2   + ' \\\\' + '\n' + \
                '$q_{1}$ & - & '                                        + STR_Q1  +   ' & $q_{2}$ & - & '                                      + STR_Q2   + ' \\\\' + '\n' + \
                '$z$ & - & '                                            + STR_Z   +    ' & & &  \\\\'                                                               + '\n' + \
                STR_EQU_TABLE_END

    # Return formatted LaTeX string
    #
    return STR_LATEX

def write_abs_mixing(ABS: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Generate equation and overview table of absorption isotherm function 'MixingRule'.

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
        Formatted LaTeX string describing equation and coefficient table of absorption isotherm
        function.

    """
    # Get correct contents from dict
    #
    STR_EOS = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['EoS'])
    STR_MIXING = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['MIXING'])
    STR_T_CRIT1 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['T_crit_1'])
    STR_P_CRIT1 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['p_crit_1'])
    STR_OMEGA1 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['omega_1'])
    STR_KAPPA11 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['kappa_1_1'])
    STR_BETA01 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['beta_0_1'])
    STR_BETA11 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['beta_1_1'])
    STR_BETA21 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['beta_2_1'])
    STR_BETA31 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['beta_3_1'])
    STR_T_CRIT2 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['T_crit_2'])
    STR_P_CRIT2 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['p_crit_2'])
    STR_OMEGA2 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['omega_2'])
    STR_KAPPA12 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['kappa_1_2'])
    STR_BETA02 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['beta_0_2'])
    STR_BETA12 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['beta_1_2'])
    STR_BETA22 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['beta_2_2'])
    STR_BETA32 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['beta_3_2'])
    STR_K12 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['delta_12'])
    STR_M = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['m'])
    STR_L12 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['l_12'])
    STR_L21 = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['l_21'])
    STR_T = '{:.9e}'.format(ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['t'])

    # Write formatted LaTeX string
    #
    STR_LATEX = STR_EQU_START                                                                   + \
                'Vapor pressure $p_\\mathrm{sat}$ in $\\si{\\pascal}$ is calculated '           + \
                'depending on temperature $T$ in $\\si{\\kelvin}$ and molar volume v in '       + \
                '$\\si{\\mole\\per\\cubic\\meter}$ by using cubic equation of state. For this ' + \
                'purpose, molar volumes of liquid and vapor phase are changed iteratively '     + \
                'until fugacity coefficients of vapor and liquid phase are equal. Cubic '       + \
                'equation of state and mixing rules are given by:'                              + '\n'

    if ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['MIXING'] <= -2.5:
        STR_MIX = 'a &=& z_1^2 a_1 + 2 z_1 z_2 a_{12} + 2_2^2 a_2 & \\quad\\text{, and} \\\\' + '\n' + \
                  'b &=& z_1 b_1 + z_2 b_2 & \\quad\\text{, and} \\\\' + '\n' + \
                  'a_{12} &=& \\left( a_1 a_2 \\right) ^{0.5} \\left(1 - k_{12} \\right) & \\quad\\text{, and} \\\\' + '\n' + \
                  'z_j &=& x_j \\textrm { or } y_j \\textrm{ depending on phase} & \\quad\\text{, and} \\\\' + '\n'
        
    elif ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['MIXING'] <= 7.5:
        STR_MIX = 'a &=& z_1^2 a_1 + 2 z_1 z_2 a_{12} + 2_2^2 a_2 & \\quad\\text{, and} \\\\' + '\n' + \
                  'b &=& z_1^2 b_1 + 2 z_1 z_2 b_{12} + 2_2^2 b_2 & \\quad\\text{, and} \\\\' + '\n' + \
                  'a_{12} &=& \\left( a_1 a_2 \\right) ^{0.5} \\left(1 - k_{12} \\right) & \\quad\\text{, and} \\\\' + '\n' + \
                  'b_{12} &=& \\frac{ b_1 + b_2 }{2} \\left(1 - m \\right) & \\quad\\text{, and} \\\\' + '\n' + \
                  'z_j &=& x_j \\textrm { or } y_j \\textrm{ depending on phase} & \\quad\\text{, and} \\\\' + '\n'
                  
    elif ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['MIXING'] <= 12.5:
        STR_MIX = 'a &=& z_1^2 a_1 + 2 z_1 z_2 a_{12} + 2_2^2 a_2 & \\quad\\text{, and} \\\\' + '\n' + \
                  'b &=& z_1^2 b_1 + 2 z_1 z_2 b_{12} + 2_2^2 b_2 & \\quad\\text{, and} \\\\' + '\n' + \
                  'a_{12} &=& \\left( a_1 a_2 \\right) ^{0.5} \\left( 1 + \\nicefrac{t}{T} \\right) \\left(1 - k_{12} \\right) & \\quad\\text{, and} \\\\' + '\n' + \
                  'b_{12} &=& \\frac{ b_1 + b_2 }{2} \\left(1 - m \\right) & \\quad\\text{, and} \\\\' + '\n' + \
                  'k_{12} &=& \\frac{l_{12} l_{21} \\left(z_1 + z_2 \\right)}{l_{21} z_1 + l_{12} z_2} & \\quad\\text{, and} \\\\' + '\n' + \
                  'z_j &=& x_j \\textrm { or } y_j \\textrm{ depending on phase} & \\quad\\text{, and} \\\\' + '\n'

    if ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['EoS'] <= -7.5:
        # SRKE with custom alpha equation
        #
        STR_LATEX += '\\begin{equation*}'                                                + '\n'      + \
                     '\\begin{split}'                                                    + '\n'      + \
                     'p &=& R \\frac{T}{v - b} - \\frac{a}{v \\left(v + b\\right)} & \\quad\\text{, and} \\\\' + '\n' + \
                     STR_MIX + \
                     'a_j &=& \\frac{1}{9 \\left(2^{\\nicefrac{1}{3}} - 1\\right)} \\frac{\\left(R T_{\\mathrm{crit},j} \\right)^2}{p_{\\mathrm{crit},j}} \\alpha_j & \\quad\\text{, and} \\\\' + '\n' + \
                     'b_j &=& 0.08664 R \\frac{T_{\\mathrm{crit},j}}{p_{\\mathrm{crit},j}} & \\quad\\text{, and} \\\\' + '\n' + \
                     '\\alpha_j &=& \\beta_{0,j} + \\sum_{i=1}^{3} \\beta_{i,j} \\theta_j^i & \\quad\\text{, and} \\\\' + '\n' + \
                     '\\theta_j &=& \\nicefrac{T_{\\mathrm{crit},j}}{T} - \\nicefrac{T}{T_{\\mathrm{crit},j}} & \\quad\\text{.}' + '\n' + \
                     '\\end{split}'                                                      + '\n' + \
                     '\\end{equation*}'

    elif ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['EoS'] <= -2.5:
        # SRKE
        #
        STR_LATEX += '\\begin{equation*}'                                                + '\n'      + \
                     '\\begin{split}'                                                    + '\n'      + \
                     'p &=& R \\frac{T}{v - b} - \\frac{a}{v \\left(v + b\\right)} & \\quad\\text{, and} \\\\' + '\n' + \
                     STR_MIX + \
                     'a_j &=& \\frac{1}{9 \\left(2^{\\nicefrac{1}{3}} - 1\\right)} \\frac{\\left(R T_{\\mathrm{crit},j} \\right)^2}{p_{\\mathrm{crit},j}} \\alpha_j & \\quad\\text{, and} \\\\' + '\n' + \
                     'b_j &=& 0.08664 R \\frac{T_{\\mathrm{crit},j}}{p_{\\mathrm{crit},j}} & \\quad\\text{, and} \\\\' + '\n' + \
                     '\\alpha_j &=& \\left(1 + \\kappa_j \\left(1 - \\sqrt(\\nicefrac{T}{T_{\\mathrm{crit},j}}) \\right) \\right)^2 & \\quad\\text{, and} \\\\' + '\n' + \
                     '\\kappa_j &=& 0.48508 + 1.55171 \\omega_j - 0.15613 \\omega_j^2 & \\quad\\text{.}' + '\n' + \
                     '\\end{split}'                                                      + '\n' + \
                     '\\end{equation*}'                                                  + '\n'

    elif ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['EoS'] <= 7.5:
        # PRE with custom alpha equation
        #
        STR_LATEX += '\\begin{equation*}'                                                + '\n'      + \
                     '\\begin{split}'                                                    + '\n'      + \
                     'p &=& R \\frac{T}{v - b} - \\frac{a}{v \\left(v + b\\right) + b \\left(v - b\\right)} & \\quad\\text{, and} \\\\' + '\n' + \
                     STR_MIX + \
                     'a_j &=& 0.45724 \\frac{\\left(R T_{\\mathrm{crit},j} \\right)^2}{p_{\\mathrm{crit},j}} \\alpha_j & \\quad\\text{, and} \\\\' + '\n' + \
                     'b_j &=& 0.07780 R \\frac{T_{\\mathrm{crit},j}}{p_{\\mathrm{crit},j}} & \\quad\\text{, and} \\\\' + '\n' + \
                     '\\alpha_j &=& \\beta_{0,j} + \\sum_{i=1}^{3} \\beta_{i,j} \\theta_j^i & \\quad\\text{, and} \\\\' + '\n' + \
                     '\\theta_j &=& \\nicefrac{T_{\\mathrm{crit},j}}{T} - \\nicefrac{T}{T_{\\mathrm{crit},j}} & \\quad\\text{.}' + '\n' + \
                     '\\end{split}'                                                      + '\n' + \
                     '\\end{equation*}'

    elif ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['EoS'] <= 12.5:
        # PRE
        #
        STR_LATEX += '\\begin{equation*}'                                                + '\n'      + \
                     '\\begin{split}'                                                    + '\n'      + \
                     'p &=& R \\frac{T}{v - b} - \\frac{a}{v \\left(v + b\\right) + b \\left(v - b\\right)} & \\quad\\text{, and} \\\\' + '\n' + \
                     STR_MIX + \
                     'a_j &=& 0.45724 \\frac{\\left(R T_{\\mathrm{crit},j} \\right)^2}{p_{\\mathrm{crit},j}} \\alpha_j & \\quad\\text{, and} \\\\' + '\n' + \
                     'b_j &=& 0.07780 R \\frac{T_{\\mathrm{crit},j}}{p_{\\mathrm{crit},j}} & \\quad\\text{, and} \\\\' + '\n' + \
                     '\\alpha_j &=& \\left(1 + \\kappa_j \\left(1 - \\sqrt(\\nicefrac{T}{T_{\\mathrm{crit},j}}) \\right) \\right)^2 & \\quad\\text{, and} \\\\' + '\n' + \
                     '\\kappa_j &=& 0.37464 + 1.54226 \\omega_j - 0.26992 \\omega_j^2 & \\quad\\text{.}' + '\n' + \
                     '\\end{split}'                                                      + '\n' + \
                     '\\end{equation*}'                                                  + '\n'

    elif ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_p_']['EoS'] <= 22.5:
        # PRSVE
        #
        STR_LATEX += '\\begin{equation*}'                                                + '\n'      + \
                     '\\begin{split}'                                                    + '\n'      + \
                     'p &=& R \\frac{T}{v - b} - \\frac{a}{v \\left(v + b\\right) + b \\left(v - b\\right)} & \\quad\\text{, and} \\\\' + '\n' + \
                     STR_MIX + \
                     'a_j &=& 0.45724 \\frac{\\left(R T_{\\mathrm{crit},j} \\right)^2}{p_{\\mathrm{crit},j}} \\alpha_j & \\quad\\text{, and} \\\\' + '\n' + \
                     'b_j &=& 0.07780 R \\frac{T_{\\mathrm{crit},j}}{p_{\\mathrm{crit},j}} & \\quad\\text{, and} \\\\' + '\n' + \
                     '\\alpha_j &=& \\left(1 + \\kappa_j \\left(1 - \\sqrt(\\nicefrac{T}{T_{\\mathrm{crit},j}}) \\right) \\right)^2 & \\quad\\text{, and} \\\\' + '\n' + \
                     '\\kappa_j &=& \\kappa_{0,j} + \\kappa_{1,j} \\left(1 + \\sqrt(\\nicefrac{T}{T_{\\mathrm{crit},j}}) \\right) \\left(0.7 - \\nicefrac{T}{T_{\\mathrm{crit},j}}\\right)  & \\quad\\text{, and} \\\\' + '\n' + \
                     '\\kappa_{0,j} &=& 0.378893 + 1.4897153 \\omega_j - 0.17131848 \\omega_j^2 + 0.0196554 \\omega_j^3 & \\quad\\text{.}' + '\n' + \
                     '\\end{split}'                                                      + '\n' + \
                     '\\end{equation*}'                                                  + '\n'

    STR_LATEX += STR_EQU_TABLE_START                                                                                                                          + \
                 'EoS & - & '                                 + STR_EOS     + ' & Mix & - & ' + STR_MIXING                                  + ' \\\\'  + '\n' + \
                 '$T_\\mathrm{crit,1}$ & $\\si{\\kelvin}$ & ' + STR_T_CRIT1 + ' & $T_\\mathrm{crit,2}$ & $\\si{\\kelvin}$ & ' + STR_T_CRIT2 + ' \\\\'  + '\n' + \
                 '$p_\\mathrm{crit,1}$ & $\\si{\\pascal}$ & ' + STR_P_CRIT1 + ' & $p_\\mathrm{crit,2}$ & $\\si{\\pascal}$ & ' + STR_P_CRIT2 + ' \\\\'  + '\n' + \
                 '$\\omega_{1}$ & - & '                       + STR_OMEGA1  + ' & $\\omega_{2}$ & - & '                       + STR_OMEGA2  + ' \\\\'  + '\n' + \
                 '$\\kappa_{1,1}$ & - & '                     + STR_KAPPA11 + ' & $\\kappa_{1,2}$ & - & '                     + STR_KAPPA12 + ' \\\\'  + '\n' + \
                 '$\\beta_{0,1}$ & - & '                      + STR_BETA01  + ' & $\\beta_{0,2}$ & - & '                      + STR_BETA02  + ' \\\\'  + '\n' + \
                 '$\\beta_{1,1}$ & - & '                      + STR_BETA11  + ' & $\\beta_{1,2}$ & - & '                      + STR_BETA12  + ' \\\\'  + '\n' + \
                 '$\\beta_{2,1}$ & - & '                      + STR_BETA21  + ' & $\\beta_{2,2}$ & - & '                      + STR_BETA22  + ' \\\\'  + '\n' + \
                 '$\\beta_{3,1}$ & - & '                      + STR_BETA31  + ' & $\\beta_{3,2}$ & - & '                      + STR_BETA32  + ' \\\\'  + '\n' + \
                 '$k_{12}$ & - & '                            + STR_K12     + ' & $m$ & - & '                                 + STR_M       + ' \\\\'  + '\n' + \
                 '$l_{12}$ & - & '                            + STR_L12     + ' & $l_{21}$ & - & '                            + STR_L21     + ' \\\\'  + '\n' + \
                 '$t$ & - & '                                 + STR_T       + ' & & & \\\\'                                                            + '\n' + \
                 STR_EQU_TABLE_END
                 
    # Return formatted LaTeX string
    #
    return STR_LATEX

def write_abs_equation(ABS: dict, EQU_NAME: str, EQU_ID: int) -> str:
    r"""Select formatted LaTeX string depending on absorption isotherm function.

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
        Formatted LaTeX string describing equation and overview table of absorption isotherm
        function.

    """
    # Switch equations
    #
    if EQU_NAME == 'Antoine':
        return write_abs_antoine(ABS, EQU_NAME, EQU_ID)

    elif EQU_NAME == 'Duehring':
        return write_abs_duehring(ABS, EQU_NAME, EQU_ID)

    elif EQU_NAME == 'FloryHuggins':
        return write_abs_flory_huggins(ABS, EQU_NAME, EQU_ID)

    elif EQU_NAME == 'Heil':
        return write_abs_heil(ABS, EQU_NAME, EQU_ID)

    elif EQU_NAME == 'MixingRule':
        return write_abs_mixing(ABS, EQU_NAME, EQU_ID)

    elif EQU_NAME == 'NrtlFixedDg':
        return write_abs_nrtl_fdg(ABS, EQU_NAME, EQU_ID)

    elif EQU_NAME == 'NrtlTemperatureDg':
        return write_abs_nrtl_tdg(ABS, EQU_NAME, EQU_ID)

    elif EQU_NAME == 'TsubokaKatayama':
        return write_abs_tsuboka_katayama(ABS, EQU_NAME, EQU_ID)

    elif EQU_NAME == 'UniquacFixedDu':
        return write_abs_uniquac_fdu(ABS, EQU_NAME, EQU_ID)

    elif EQU_NAME == 'UniquacTemperatureDu':
        return write_abs_uniquac_tdu(ABS, EQU_NAME, EQU_ID)

    elif EQU_NAME == 'WangChao':
        return write_abs_wang_chao(ABS, EQU_NAME, EQU_ID)

    elif EQU_NAME == 'WilsonFixedDl':
        return write_abs_wilson_fdl(ABS, EQU_NAME, EQU_ID)

    elif EQU_NAME == 'WilsonTemperatureDl':
        return write_abs_wilson_tdl(ABS, EQU_NAME, EQU_ID)

    else:
        # No formatted LaTeX string implemented for selected adsorption isotherm equation
        #
        return STR_EQU_START + 'For absorption isotherm \'' + EQU_NAME.replace('_', '\_') + \
            '\', formatted LaTeX string is not implemented in manual!' + '\n\n'
