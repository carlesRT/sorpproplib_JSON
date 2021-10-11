"""Package 'equilibrium' cotains functions to plot equilibrium properties.

Extended Summary
----------------
This module containes functions to plot equilibrium properties of working pairs in various
diagrams. The following diagrams are implemented: 1) Isosteric, 2) isotherms, 3) isobars,
and 4) Dühring. Moreover, a function generating a characteristic curve is implemented
for adsorption working pairs based on the volumetric approach.


Routine Listings
----------------
plot_isosteres:
    Creates and returns a diagrams with isosteres.
plot_isotherms:
    Creates and returns a diagrams with isotherms.
plot_isobars:
    Creates and returns a diagrams with isobars.
plot_char_curve:
    Creates and returns a diagram with the characteristic curve.
plot_duehring:
    Creates and returns a Dühring diagram.


Revisions
---------
    Version     Date        Author              Changes
    1.0.0       2020-07-09  Mirko Engelpracht   First implementation
"""

# %% Import relevant modules.
#
import matplotlib.pyplot as plt
import numpy as np

from typing import Callable


# %% Definition of functions
#
def plot_isosteres(func_p_sat_T: Callable[[float], float],
                   func_p_wxXT: Callable[[float, float], float],
                   func_wxX_pT: Callable[[float, float], float],
                   T_calc: list, wxX_calc: list,
                   T_axis: list, p_axis: list) -> dict:
    r"""Plot isosters.

    This functions calculates vapor pressures and equilibrium pressures for the temperatures
    and loadings / molar fractions / concentrations given as inputs. The input 'func_p_wxXT'
    defines what will be needed for calculations: loadings / molar fractions / concentrations.

    Parameters
    ----------
    func_p_sat_T : Callable[[float], float]
        Method to calculate vapor pressure depending on temperature.
    func_p_wxXT : Callable[[float, float], float]
        Method to calculate pressure depending on loading / molar fraction / concentration and
        temperature.
    func_wxX_pT : Callable[[float, float], float]
        Method to calculate loading / molar fraction / concentration depending on pressure and
        temperature.
    T_calc : list
        List containing temperatures (i.e., floats) required to calculate equilibrium pressure.
    wxX_calc : list
        List containing loadings / molar fractions / concentrations (i.e., floats) required to
        equilibrium pressure.
    T_axis : list
        List containing temperatures (i.e., floats) defining the ticks of the x axis.
    p_axis : list
        List containing pressures (i.e., floats) defining the ticks of the y axis.

    Returns
    -------
    dict
        Dict containing a) handles for figure, axis, and plots; and b) calculated vapor pressures
        and equilibrium pressures.

    """
    # Calculate saturation properties and get reasonable results (i.e., p_sat <= p_crit)
    #
    p_sat = [func_p_sat_T(val) for val in T_calc]
    id_limit = np.logical_and(np.greater_equal(p_sat, 0), np.isfinite(p_sat))

    saturationPropeties = {'p_sat':   np.extract(id_limit, p_sat),
                           'T_sat':   np.extract(id_limit, T_calc)}

    # Calculate maximum loading / molar fraction / concentration to adapt input wxX_calc
    #
    wxX_max = func_wxX_pT(min(saturationPropeties['p_sat']),
                          min(saturationPropeties['T_sat']))

    wxX_calc = np.extract(np.less_equal(wxX_calc, wxX_max), wxX_calc)

    # Get critical pressure and temperature or maximum pressure and temperature if both are
    # lower than critical pressure and temperature
    #
    p_crit = max(saturationPropeties['p_sat'])
    T_crit = max(saturationPropeties['T_sat'])

    # Calculate equilibrium properties and get reasonable results (i.e., p_wxXT <= p_sat)
    #
    sorptionEquilibrium = [None] * len(wxX_calc)

    for id_wxX, val_wxX in enumerate(wxX_calc):
        p_wxXT = [func_p_wxXT(val_wxX, val) for val in T_calc]

        if not np.less_equal(p_wxXT[:len(saturationPropeties['p_sat'])],
                             saturationPropeties['p_sat']).all():
            # Cut at p_sat
            #
            id_finite = np.where(np.less_equal(p_wxXT, p_sat) == False)[0][0]

            sorptionEquilibrium[id_wxX] = {'p_wxXT':    np.array(p_wxXT[:id_finite]),
                                           'T_equ':    np.array(T_calc[:id_finite])}

        else:
            # Cut at p_crit
            #
            id_limit = np.less_equal(np.array(p_wxXT), p_crit)

            sorptionEquilibrium[id_wxX] = {'p_wxXT':    np.extract(id_limit, np.array(p_wxXT)),
                                           'T_equ':     np.extract(id_limit, np.array(T_calc))}

    # Plot data
    #
    h_fig, h_ax = plt.subplots(1, 1, figsize=(20/2.54, 10/2.54))
    h_plots = [None] * (len(wxX_calc) + 1)

    h_plots[0] = h_ax.plot(-1/saturationPropeties['T_sat'],
                           saturationPropeties['p_sat'],
                           linestyle = '-',
                           color = '#00459F',
                           label = 'Vapor pressure')

    for id_wxX, val_wxX in enumerate(wxX_calc):
        h_plots[id_wxX+1] = h_ax.plot(-1/sorptionEquilibrium[id_wxX]['T_equ'],
                                      sorptionEquilibrium[id_wxX]['p_wxXT'],
                                      linestyle = '--',
                                      # color = '#646567',
                                      label = str(val_wxX*100) + ' %')

    # Set up axis
    #
    h_ax.spines['top'].set_visible(False)
    h_ax.spines['right'].set_visible(False)
    h_ax.spines['left'].set_linewidth(0.5)
    h_ax.spines['bottom'].set_linewidth(0.5)

    h_ax.set_xlim(-1/min(T_axis), -1/max(T_axis))
    h_ax.set_ylim(min(p_axis), max(p_axis))

    h_ax.set_xticks([-1/val for val in T_axis])
    h_ax.set_yticks([val for val in p_axis])

    h_ax.set_xticklabels([str(int(val - 273.15)) for val in T_axis])

    h_ax.set_yscale('log')

    h_ax.grid(linestyle = ':', color = '#9C9E9F')

    h_ax.set_xlabel(r'Temperature $t$ / °C $\longrightarrow$')
    h_ax.set_ylabel(r'Pressure $p$ / Pa $\longrightarrow$')

    h_ax.legend(bbox_to_anchor=(1.05, 1),
                loc='upper left',
                labelspacing=0.05,
                frameon=False)

    # Tight layout
    #
    h_fig.align_ylabels()
    h_fig.tight_layout()

    # Return results allowing for custom made moedifications
    #
    return {'handles':  (h_fig, h_ax, h_plots),
            'data':     (saturationPropeties, sorptionEquilibrium)}


def plot_isotherms(func_p_sat_T: Callable[[float], float],
                   func_wxX_pT: Callable[[float, float], float],
                   p_calc: list, T_calc: list,
                   p_axis: list, wxX_axis: list,
                   flag_p_rel: bool = False) -> dict:
    r"""Plot isotherms.

    This functions calculates vapor pressures and loadings / molar fractions / concentrations for
    the pressures and temperatures given as inputs. The input 'func_wxX_pT' defines what will be
    calculated: loadings / molar fractions / concentrations.

    Parameters
    ----------
    func_p_sat_T : Callable[[float], float]
        Method to calculate vapor pressure depending on temperature.
    func_wxX_pT : Callable[[float, float], float]
        Method to calculate loading / molar fraction / concentration depending on pressure and
        temperature.
    p_calc : list
        List containing pressures (i.e., floats) required to calculate loadings / molar fractions
        / concentrations.
    T_calc : list
        List containing temperatures (i.e., floats) required to calculate loadings / molar
        fractions / concentrations.
    p_axis : list
        List containing pressures (i.e., floats) defining the ticks of the x axis.
    wxX_axis : list
        List containing loadings / molar fractions / concentrations (i.e., floats) defining the
        ticks of the y axis.
    flag_p_rel : bool, optional
        Flag defining if x axis shall be pressure or relative pressure (i.e., p/p_sat). The
        default is False.

    Returns
    -------
    dict
        Dict containing a) handles for figure, axis, and plots; and b) calculated vapor pressures
        and equilibrium loadings / molar fractions / concentrations.

    """
    # Calculate saturation properties and get finite results
    #
    p_sat = [func_p_sat_T(val) for val in T_calc]
    id_limit = np.logical_and(np.greater_equal(p_sat, 0), np.isfinite(p_sat))

    saturationPropeties = {'p_sat': np.extract(id_limit, p_sat),
                           'T_sat': np.extract(id_limit, T_calc)}

    # Get critical pressure and temperature or maximum pressure and temperature if both are
    # lower than critical pressure and temperature
    #
    p_crit = max(saturationPropeties['p_sat'])
    T_crit = max(saturationPropeties['T_sat'])

    # Calculate saturation capacity
    #
    T_sat_boundary = [T_calc[0]] + [val for val in range(int(np.ceil(T_calc[0])),
                                                         int(np.floor(T_crit)))] + [T_crit]
    p_sat_boundary = [func_p_sat_T(val) for val in T_sat_boundary]

    # Calculate equilibrium properties and get reasonable results (i.e., p_calc <= p_sat(T_calc))
    #
    sorptionEquilibrium = [None] * len(T_calc)

    for id_T, val_T in enumerate(T_calc):
        wxXT_pT = [func_wxX_pT(val, val_T) for val in p_calc]

        if val_T <= T_crit:
            # Cut at p_sat
            #
            if not np.less_equal(p_calc, p_sat[id_T]).all():
                # Cut necessary
                #
                id_limit = np.less_equal(p_calc, p_sat[id_T])

                sorptionEquilibrium[id_T] = {'wxXT_pT': np.extract(id_limit, np.array(wxXT_pT)),
                                             'p_equ':   np.extract(id_limit, np.array(p_calc))}

            else:
                # No cut necessary
                #
                sorptionEquilibrium[id_T] = {'wxXT_pT': np.array(wxXT_pT),
                                             'p_equ':   np.array(p_calc)}

        else:
            # Cut at p_crit
            #
            id_limit = np.less_equal(p_calc, p_crit)

            sorptionEquilibrium[id_T] = {'wxXT_pT': np.extract(id_limit, np.array(wxXT_pT)),
                                         'p_equ':   np.extract(id_limit, np.array(p_calc))}

    # Plot data
    #
    h_fig, h_ax = plt.subplots(1, 1, figsize=(20/2.54, 10/2.54))
    h_plots = [None] * (len(T_calc)  + 1)

    h_plots[0] = h_ax.plot(p_sat_boundary if not flag_p_rel else \
                           np.array(p_sat_boundary) / np.array(p_sat_boundary),
                           [func_wxX_pT(p_sat_boundary[ind], val) for ind, val in \
                            enumerate(T_sat_boundary)],
                           linestyle = '-',
                           color = '#00459F',
                           label = 'Saturation capacity')

    for id_T, val_T in enumerate(T_calc):
        # Convert x axis if necessary
        #
        if val_T <= T_crit:
            # Normalize by p_sat(T)
            #
            p_plot = sorptionEquilibrium[id_T]['p_equ'] if not flag_p_rel else \
                sorptionEquilibrium[id_T]['p_equ'] / p_sat[id_T]

        else:
            # Normalize by p_crit
            #
            p_plot = sorptionEquilibrium[id_T]['p_equ'] if not flag_p_rel else \
                sorptionEquilibrium[id_T]['p_equ'] / p_crit


        h_plots[id_T+1] = h_ax.plot(p_plot,
                                    sorptionEquilibrium[id_T]['wxXT_pT'],
                                    linestyle = '--',
                                    # color = '#646567',
                                    label = str(val_T - 273.15) + ' °C')

    # Set up axis
    #
    h_ax.spines['top'].set_visible(False)
    h_ax.spines['right'].set_visible(False)
    h_ax.spines['left'].set_linewidth(0.5)
    h_ax.spines['bottom'].set_linewidth(0.5)

    if not flag_p_rel:
        h_ax.set_xlim(min(p_axis), max(p_axis))
    else:
        h_ax.set_xlim(0, 1)


    h_ax.set_ylim(min(wxX_axis), max(wxX_axis))

    h_ax.set_xticks([val for val in p_axis] if not flag_p_rel else \
                    [0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1])
    h_ax.set_yticks([val for val in wxX_axis])

    h_ax.grid(linestyle = ':', color = '#9C9E9F')

    h_ax.set_xlabel(r'Pressure $p$ / Pa $\longrightarrow$' if not flag_p_rel else \
                    r'Relative pressure $p$ $p_\mathrm{sat}^{-1}$ / Pa Pa$^{-1}$ $\longrightarrow$')
    h_ax.set_ylabel(r'Sorption capacity $w$ or $x$ or $X$ / various $\longrightarrow$')

    h_ax.legend(bbox_to_anchor=(1.05, 1),
                loc='upper left',
                labelspacing=0.05,
                frameon=False)

    # Tight layout
    #
    h_fig.align_ylabels()
    h_fig.tight_layout()

    # Return results allowing for custom made moedifications
    #
    return {'handles':  (h_fig, h_ax, h_plots),
            'data':     (saturationPropeties, sorptionEquilibrium)}


def plot_isobars(func_p_sat_T: Callable[[float], float],
                 func_T_sat_p: Callable[[float], float],
                 func_wxX_pT: Callable[[float, float], float],
                 p_calc: list, T_calc: list,
                 T_axis: list, wxX_axis: list) -> dict:
    r"""Plot isobars.

    This functions calculates vapor pressures and loadings / molar fractions / concentrations for
    the pressures and temperatures given as inputs. The input 'func_wxX_pT' defines what will be
    calculated: loadings / molar fractions / concentrations.

    Parameters
    ----------
    func_p_sat_T : Callable[[float], float]
        Method to calculate vapor pressure depending on temperature.
    func_T_sat_p : Callable[[float], float]
        Method to calculate vapor temperature depending on pressure.
    func_wxX_pT : Callable[[float, float], float]
        Method to calculate loading / molar fraction / concentration depending on pressure and
        temperature.
    p_calc : list
        List containing pressures (i.e., floats) required to calculate loadings / molar fractions
        / concentrations.
    T_calc : list
        List containing temperatures (i.e., floats) required to calculate loadings / molar
        fractions / concentrations.
    T_axis : list
        List containing temperatures (i.e., floats) defining the ticks of the x axis.
    wxX_axis : list
        List containing loadings / molar fractions / concentrations (i.e., floats) defining the
        ticks of the y axis.

    Returns
    -------
    dict
        Dict containing a) handles for figure, axis, and plots; and b) calculated vapor pressures
        and equilibrium loadings / molar fractions / concentrations.

    """
    # Calculate saturation properties and get finite results
    #
    p_sat = [func_p_sat_T(val) for val in T_calc]
    id_limit = np.logical_and(np.greater_equal(p_sat, 0), np.isfinite(p_sat))

    saturationPropeties = {'p_sat': np.extract(id_limit, p_sat),
                           'T_sat': np.extract(id_limit, T_calc)}

    # Get critical pressure and temperature or maximum pressure and temperature if both are
    # lower than critical pressure and temperature: Cut p_calc if necessary
    #
    p_crit = max(saturationPropeties['p_sat'])
    T_crit = max(saturationPropeties['T_sat'])

    p_calc = np.extract(np.less_equal(p_calc, p_crit), p_calc)

    # Calculate saturation capacity
    #
    T_sat_boundary = [T_calc[0]] + [val for val in range(int(np.ceil(T_calc[0])),
                                                         int(np.floor(T_crit)))] + [T_crit]
    p_sat_boundary = [func_p_sat_T(val) for val in T_sat_boundary]

    # Calculate equilibrium properties and get reasonable results (i.e., p_calc <= p_sat(T_calc))
    #
    sorptionEquilibrium = [None] * len(p_calc)

    for id_p, val_p in enumerate(p_calc):
        wxXT_pT = [func_wxX_pT(val_p, val) for val in T_calc]
        id_limit = np.less_equal(wxXT_pT, func_wxX_pT(val_p, func_T_sat_p(val_p)))

        sorptionEquilibrium[id_p] = {'wxXT_pT': np.extract(id_limit, np.array(wxXT_pT)),
                                     'T_equ':   np.extract(id_limit, np.array(T_calc))}

    # Plot data
    #
    h_fig, h_ax = plt.subplots(1, 1, figsize=(20/2.54, 10/2.54))
    h_plots = [None] * (len(p_calc) + 1)

    h_plots[0] = h_ax.plot(T_sat_boundary,
                           [func_wxX_pT(p_sat_boundary[ind], val) for ind, val in \
                            enumerate(T_sat_boundary)],
                           linestyle = '-',
                           color = '#00459F',
                           label = 'Saturation capacity')

    for id_p, val_p in enumerate(p_calc):
        h_plots[id_p+1] = h_ax.plot(sorptionEquilibrium[id_p]['T_equ'],
                                    sorptionEquilibrium[id_p]['wxXT_pT'],
                                    linestyle = '--',
                                    # color = '#646567',
                                    label = str(val_p/1000) + ' kPa')

    # Set up axis
    #
    h_ax.spines['top'].set_visible(False)
    h_ax.spines['right'].set_visible(False)
    h_ax.spines['left'].set_linewidth(0.5)
    h_ax.spines['bottom'].set_linewidth(0.5)

    h_ax.set_xlim(min(T_axis), max(T_axis))
    h_ax.set_ylim(min(wxX_axis), max(wxX_axis))

    h_ax.set_xticks([val for val in T_axis])
    h_ax.set_yticks([val for val in wxX_axis])

    h_ax.set_xticklabels([str(int(val - 273.15)) for val in T_axis])

    h_ax.grid(linestyle = ':', color = '#9C9E9F')

    h_ax.set_xlabel(r'Temperature $t$ / °C $\longrightarrow$')
    h_ax.set_ylabel(r'Sorption capacity $w$ or $x$ or $X$ / various $\longrightarrow$')

    h_ax.legend(bbox_to_anchor=(1.05, 1),
                loc='upper left',
                labelspacing=0.05,
                frameon=False)

    # Tight layout
    #
    h_fig.align_ylabels()
    h_fig.tight_layout()

    # Return results allowing for custom made moedifications
    #
    return {'handles':  (h_fig, h_ax, h_plots),
            'data':     (saturationPropeties, sorptionEquilibrium)}


def plot_char_curve(func_W_A: Callable[[float], float],
                    A_calc: list, A_axis: list, W_axis: list) -> dict:
    r"""Plot characteristic curve.

    This functions calculates the characteristic curve (i.e., W(A)).

    Parameters
    ----------
    func_W_A : Callable[[float], float]
        Method to calculate volumetric loading depending on adsorption potential.
    A_calc : list
        List containing adsorption potentials (i.e., floats) required to volumetric loadings.
    A_axis : list
        List containing adsorption potentials (i.e., floats) defining the ticks of the x axis.
    W_axis : list
        List volumetric loadings (i.e., floats) defining the ticks of the y axis.

    Returns
    -------
    dict
        Dict containing a) handles for figure, axis, and plots; and b) calculated volumetric
        loadings.

    """
    # Calculate volumetric loadings
    #
    W = [func_W_A(val) for val in A_calc]

    sorptionEquilibrium = {'A':     np.array(A_calc),
                           'W':     np.array(W)}

    # Plot data
    #
    h_fig, h_ax = plt.subplots(1, 1, figsize=(20/2.54, 10/2.54))

    h_plots = h_ax.plot(A_calc,
                        W,
                        linestyle = '-',
                        color = '#00459F',
                        label = 'Char. curve')

    # Set up axis
    #
    h_ax.spines['top'].set_visible(False)
    h_ax.spines['right'].set_visible(False)
    h_ax.spines['left'].set_linewidth(0.5)
    h_ax.spines['bottom'].set_linewidth(0.5)

    h_ax.set_xlim(min(A_axis), max(A_axis))
    h_ax.set_ylim(min(W_axis), max(W_axis))

    h_ax.set_xticks([val for val in A_axis])
    h_ax.set_yticks([val for val in W_axis])

    h_ax.grid(linestyle = ':', color = '#9C9E9F')

    h_ax.set_xlabel(r'Adsorption potential $A$ / J mol$^{-1}$ $\longrightarrow$')
    h_ax.set_ylabel(r'Volumetric loading $W$ / m$^{3}$ kg$^{-1}$ $\longrightarrow$')

    # Tight layout
    #
    h_fig.align_ylabels()
    h_fig.tight_layout()

    # Return results allowing for custom made moedifications
    #
    return {'handles':  (h_fig, h_ax, h_plots),
            'data':     sorptionEquilibrium}


def plot_duehring(func_p_sat_T: Callable[[float], float],
                  func_T_sat_p: Callable[[float], float],
                  func_p_wxXT: Callable[[float, float], float],
                  func_wxX_pT: Callable[[float, float], float],
                  T_calc: list, wxX_calc: list,
                  T_axis_x: list, T_axis_y: list) -> dict:
    r"""Plot Dühring diagram.

    This functions calculates vapor temperatures and equilibrium pressures for the temperatures
    and loadings / molar fractions / concentrations given as inputs. The input 'func_p_wxXT'
    defines what will be needed for calculations: loadings / molar fractions / concentrations.

    Parameters
    ----------
    func_p_sat_T : Callable[[float], float]
        Method to calculate vapor pressure depending on temperature.
    func_T_sat_p : Callable[[float], float]
        Method to calculate vapor temperature depending on pressure.
    func_p_wxXT : Callable[[float, float], float]
        Method to calculate pressure depending on loading / molar fraction / concentration and
        temperature.
    func_wxX_pT : Callable[[float, float], float]
        Method to calculate loading / molar fraction / concentration depending on pressure and
        temperature.
    T_calc : list
        List containing temperatures (i.e., floats) required to calculate equilibrium pressure.
    wxX_calc : list
        List containing loadings / molar fractions / concentrations (i.e., floats) required to
        equilibrium pressure.
    T_axis_x : list
        List containing temperatures (i.e., floats) defining the ticks of the x axis.
    T_axis_y : list
        List containing vapor temperatures (i.e., floats) defining the ticks of the y axis.

    Returns
    -------
    dict
        Dict containing a) handles for figure, axis, and plots; and b) calculated vapor pressures
        and equilibrium pressures.

    """
    # Calculate saturation properties and get reasonable results (i.e., p_sat <= p_crit)
    #
    p_sat = [func_p_sat_T(val) for val in T_calc]
    id_limit = np.logical_and(np.greater_equal(p_sat, 0), np.isfinite(p_sat))

    saturationPropeties = {'p_sat':   np.extract(id_limit, p_sat),
                           'T_sat':   np.extract(id_limit, T_calc)}

    # Calculate maximum loading / molar fraction / concentration to adapt input wxX_calc
    #
    wxX_max = func_wxX_pT(min(saturationPropeties['p_sat']),
                          min(saturationPropeties['T_sat']))

    wxX_calc = np.extract(np.less_equal(wxX_calc, wxX_max), wxX_calc)

    # Get critical pressure and temperature or maximum pressure and temperature if both are
    # lower than critical pressure and temperature
    #
    p_crit = max(saturationPropeties['p_sat'])
    T_crit = max(saturationPropeties['T_sat'])

    # Calculate equilibrium properties and get reasonable results (i.e., p_wxXT <= p_sat)
    #
    sorptionEquilibrium = [None] * len(wxX_calc)

    for id_wxX, val_wxX in enumerate(wxX_calc):
        p_wxXT = [func_p_wxXT(val_wxX, val) for val in T_calc]

        if not np.less_equal(p_wxXT[:len(saturationPropeties['p_sat'])],
                             saturationPropeties['p_sat']).all():
            # Cut at p_sat
            #
            id_finite = np.where(np.less_equal(p_wxXT, p_sat) == False)[0][0]

            sorptionEquilibrium[id_wxX] = {'p_wxXT':    np.array(p_wxXT[:id_finite]),
                                           'T_equ':     np.array(T_calc[:id_finite]),
                                           'T_sat':     np.array([func_T_sat_p(val) for val in \
                                                                  np.array(p_wxXT[:id_finite])])}

        else:
            # Cut at p_crit
            #
            id_limit = np.less_equal(np.array(p_wxXT), p_crit)

            sorptionEquilibrium[id_wxX] = {'p_wxXT':    np.extract(id_limit, np.array(p_wxXT)),
                                           'T_equ':     np.extract(id_limit, np.array(T_calc)),
                                           'T_sat':     np.array([func_T_sat_p(val) for val in \
                                                                  np.extract(id_limit,
                                                                             np.array(p_wxXT))])}

    # Plot data
    #
    h_fig, h_ax = plt.subplots(1, 1, figsize=(20/2.54, 10/2.54))
    h_plots = [None] * (len(wxX_calc) + 1)

    h_plots[0] = h_ax.plot(saturationPropeties['T_sat'],
                           saturationPropeties['T_sat'],
                           linestyle = '-',
                           color = '#00459F',
                           label = 'Vapor temperature')

    for id_wxX, val_wxX in enumerate(wxX_calc):
        id_valid = np.greater_equal(sorptionEquilibrium[id_wxX]['T_sat'], 0)

        h_plots[id_wxX+1] = h_ax.plot(np.extract(id_valid, sorptionEquilibrium[id_wxX]['T_equ']),
                                      np.extract(id_valid, sorptionEquilibrium[id_wxX]['T_sat']),
                                      linestyle = '--',
                                      # color = '#646567',
                                      label = str(val_wxX*100) + ' %')

    # Set up axis
    #
    h_ax.spines['top'].set_visible(False)
    h_ax.spines['right'].set_visible(False)
    h_ax.spines['left'].set_linewidth(0.5)
    h_ax.spines['bottom'].set_linewidth(0.5)

    h_ax.set_xlim(min(T_axis_x), max(T_axis_x))
    h_ax.set_ylim(min(T_axis_y), max(T_axis_y))

    h_ax.set_xticks([val for val in T_axis_x])
    h_ax.set_yticks([val for val in T_axis_y])

    h_ax.set_xticklabels([str(int(val - 273.15)) for val in T_axis_x])
    h_ax.set_yticklabels([str(int(val - 273.15)) for val in T_axis_y])

    h_ax.grid(linestyle = ':', color = '#9C9E9F')

    h_ax.set_xlabel(r'Temperature $t$ / °C $\longrightarrow$')
    h_ax.set_ylabel(r'Temperature $t_\mathrm{sat}$ / °C $\longrightarrow$')

    h_ax.legend(bbox_to_anchor=(1.05, 1),
                loc='upper left',
                labelspacing=0.05,
                frameon=False)

    # Tight layout
    #
    h_fig.align_ylabels()
    h_fig.tight_layout()

    # Return results allowing for custom made moedifications
    #
    return {'handles':  (h_fig, h_ax, h_plots),
            'data':     (saturationPropeties, sorptionEquilibrium)}
