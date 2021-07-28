"""Package 'visualization' cotains functions to visualize approaches implemented in the SorpPropLib.

Routine Listings
----------------
write_ref_validity:
    Generate formatted LaTeX string for refrigerant functions.
write_ads_viszualization:
    Generate formatted LaTeX string describing visualization of adsorption isotherm equation.
write_abs_viszualization:
    Generate formatted LaTeX string describing visualization of absorption isotherm equation.
    
Revisions
---------
    Version     Date        Author              Changes
    1.0.0       2020-07-20  Mirko Engelpracht   First implementation
"""


# %% Import relevant modules.
#
import os
import matplotlib.pyplot as plt
import numpy as np

from .utilities import str_file, fuct_w_pT___ads_isotherm, fuct_p_wT___abs_isosteres, fuct_w_pT___abs_isotherms
from ..wpair.wpair_struct import WPairStruct


# %% Definition of constants.
#
# Constant strings defining start and end of visualization section
#
STR_VISUALIZATION_START = '\\textbf{Visualization:}' + '\n' + '%'

STR_VISUALIZATION_END = '\n' + '\\FloatBarrier' + '\n' + '\\newpage' + '\n'


# %% Define functions.
#
def write_ref_viszualization(REF: dict, EQU_NAME: str, EQU_ID: int,
                             WPAIRS: list, PATH_WRAPPER: str,
                             PATH_SORPPROPLIB: str, PATH_DB: str) -> str:
    r"""Generate formatted LaTeX string describing visualization of refrigerant equation.

    Parameters
    ----------
    REF : dict
        Dict containing properties of current refrigerant extrated from data base (i.e., JSON).
    EQU_NAME : str
        Name of refrigerant functions for which overview table shall be generated.
    EQU_ID : int
        ID of refrigerant functions for which overview table shall be generated.
    WPAIRS : list
        List containing all ab- and adsorption working pairs implemented in data base.
    PATH_WRAPPER : str
        Path to Python wrapper.
    PATH_SORPPROPLIB : str
        Path to DLL of SorpPropLib.
    PATH_DB : str
        Path to JSON data base of SorpPropLib.

    Returns
    -------
    str
        Formatted LaTeX string describing visualization of refrigerant equation.

    """
    # Create figure if possible: Creation only possible, if ad- or absorption working pair exists
    # that uses current refrigerant
    #
    flag_wpair_exist = False

    for _, wpair_i in enumerate(WPAIRS):
        if REF['k']['_rf_'] == wpair_i['k']['_rf_']:
            # Create working pair
            #
            wpair = {"sorbent": wpair_i['k']['_as_'],
                     "subtype": wpair_i['k']['_st_'],
                     "refrigerant": wpair_i['k']['_rf_']}
            wpair_approach = {"isotherm": list(wpair_i['v']['_ep_'].keys())[0],
                              "id_isotherm": 1,
                              "vapor_pressure": EQU_NAME,
                              "id_vapor_pressure": EQU_ID+1,
                              "sat_liq_density": EQU_NAME,
                              "id_sat_liq_density": EQU_ID+1}
            wpair_class = WPairStruct(wpair, wpair_approach, PATH_SORPPROPLIB, PATH_DB)

            # Get equation-dependent data and calculate properties
            #
            T_min = REF['v']['_ep_'][EQU_NAME][EQU_ID]['_va_']['temperature-min'] if \
                REF['v']['_ep_'][EQU_NAME][EQU_ID]['_va_']['temperature-min'] > 0 else \
                    0.75 * REF['v']['_ep_'][EQU_NAME][EQU_ID]['_va_']['temperature-max']
            T_max = REF['v']['_ep_'][EQU_NAME][EQU_ID]['_va_']['temperature-max']

            data_x = [val for val in range(int(np.ceil(T_min)), int(np.floor(T_max)))]

            if EQU_NAME.split('_')[0] == 'SaturatedLiquidDensity':
                data_y = [wpair_class.Ref.rho_sat_l_T(val) for val in data_x]
                str_y_label = r'Saturated liquid density $\rho_\mathrm{sat}^{\mathrm{liq}}$ / ' + \
                               'kg m$^{\mathrm{-3}}$ $\longrightarrow$'

            else:
                data_y = [wpair_class.Ref.p_sat_T(val) / 1e5 for val in data_x]
                str_y_label = r'Pressure $p$ / bar $\longrightarrow$'

            # Create plot
            #
            fig, ax = plt.subplots(1, 1, figsize=(14.9/2.54, 10/2.54))

            ax.plot(data_x, data_y, linestyle = '-', color = '#00459F')

            ax.spines['top'].set_visible(False)
            ax.spines['right'].set_visible(False)
            ax.spines['left'].set_linewidth(0.5)
            ax.spines['bottom'].set_linewidth(0.5)

            ax.grid(linestyle = ':', color = '#9C9E9F')

            ax.set_xlabel(r'Temperature $T$ / K $\longrightarrow$')
            ax.set_ylabel(str_y_label)

            fig.align_ylabels()
            fig.tight_layout()

            # Save plot
            #
            str_dir = os.path.join(PATH_WRAPPER, 'sorpproplib', 'doc', 'figs', 'ref')
            str_name = 'ref_' + str_file(wpair_i['k']['_rf_']) + '_' + EQU_NAME + \
                '_' + str(EQU_ID+1) + '.pdf'
            str_path = os.path.join(str_dir, str_name)

            if not os.path.exists(str_dir):
                os.mkdir(str_dir)

            fig.savefig(str_path)
            plt.close()

            # Delete working pair
            #
            del(wpair_class)
            flag_wpair_exist = True
            break

    # Write formatted LaTeX string
    #
    if flag_wpair_exist:
        STR_LATEX = STR_VISUALIZATION_START                                             + '\n'  + \
                    '\\begin{figure}[!htp]'                                             + '\n'  + \
                    '{\\noindent\\includegraphics[height=10cm, keepaspectratio]{figs/ref/'      + \
                    str_name + '}}'                                                     + '\n'  + \
                    '\\end{figure}'                                                     + '\n'  + \
                    '%'                                                                 + '\n'  + \
                    STR_VISUALIZATION_END

    else:
        STR_LATEX = STR_VISUALIZATION_START                                             + '\n'  + \
                    '\\newline'                                                         + '\n'  + \
                    'No adsorption or absorption working pair exists, which uses refrigerant '  + \
                    '\'' + REF['k']['_rf_'] + '\'. Thus, data cannot be visualized!'    + '\n'  + \
                    '%'                                                                 + '\n'  + \
                    STR_VISUALIZATION_END

    # Return formatted LaTeX string
    #
    return STR_LATEX

def write_ads_viszualization(ADS: dict, EQU_NAME: str, EQU_ID: int,
                             REFS: list, PATH_WRAPPER: str,
                             PATH_SORPPROPLIB: str, PATH_DB: str) -> str:
    r"""Generate formatted LaTeX string describing visualization of adsorption isotherm equation.

    Parameters
    ----------
    ADS : dict
        Dict containing properties of current adsorption working pair extrated from data
        base (i.e., JSON).
    EQU_NAME : str
        Name of refrigerant functions for which overview table shall be generated.
    EQU_ID : int
        ID of refrigerant functions for which overview table shall be generated.
    REFS : list
        List containing all refrigerants implemented in data base.
    PATH_WRAPPER : str
        Path to Python wrapper.
    PATH_SORPPROPLIB : str
        Path to DLL of SorpPropLib.
    PATH_DB : str
        Path to JSON data base of SorpPropLib.

    Returns
    -------
    str
        Formatted LaTeX string describing visualization of refrigerant equation.

    """
    # Create figure if possible: Creation only possible, if experimental data exisits.
    #
    flag_data_exist = False

    STR_SORBENT = ADS['k']['_as_']
    STR_SUBTYPE = ADS['k']['_st_']
    STR_REFRIGERANT =ADS['k']['_rf_']
    STR_REFERRENCE = ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_c_']
    
    # Existance of experimental data is checked by checking for same reference
    #
    for int_id_ed, val in enumerate(ADS['v']['_ed_']):
        if val['_c_'] == STR_REFERRENCE:
            flag_data_exist = True
            break

    if flag_data_exist:
        # Check for available refrigerant functions and select them to create a working pair object
        #
        for _, ref_i in enumerate(REFS):
            if ref_i['k']['_rf_'] == STR_REFRIGERANT:
                if 'VaporPressure_EoS1' in ref_i['v']['_ep_']:
                    STR_VAPOR_PRESSURE = 'VaporPressure_EoS1'
                elif 'VaporPressure_EoS2' in ref_i['v']['_ep_']:
                    STR_VAPOR_PRESSURE = 'VaporPressure_EoS2'
                elif 'VaporPressure_EoS3' in ref_i['v']['_ep_']:
                    STR_VAPOR_PRESSURE = 'VaporPressure_EoS3'
                elif 'VaporPressure_EoSCubic' in ref_i['v']['_ep_']:
                    STR_VAPOR_PRESSURE = 'VaporPressure_EoSCubic'
                elif 'VaporPressure_Antoine' in ref_i['v']['_ep_']:
                    STR_VAPOR_PRESSURE = 'VaporPressure_Antoine'
                else:
                    STR_VAPOR_PRESSURE = 'NoVaporPressure'
    
                if 'SaturatedLiquidDensity_EoS1' in ref_i['v']['_ep_']:
                    STR_DENSITY = 'SaturatedLiquidDensity_EoS1'
                else:
                    STR_DENSITY = 'NoSaturatedLiquidDensity'
    
                break
                
        # Create working pair
        #
        WPAIR = {"sorbent": STR_SORBENT,
                 "subtype": STR_SUBTYPE,
                 "refrigerant": STR_REFRIGERANT}
        WPAIR_APPROACH = {"isotherm": EQU_NAME,
                          "id_isotherm": EQU_ID+1,
                          "vapor_pressure": STR_VAPOR_PRESSURE,
                          "id_vapor_pressure": 1,
                          "sat_liq_density": STR_DENSITY,
                          "id_sat_liq_density": 1}
        WPAIR_CLASS = WPairStruct(WPAIR, WPAIR_APPROACH, PATH_SORPPROPLIB, PATH_DB)
    
        # Get raw data and sort it
        #
        p_raw = ADS['v']['_ed_'][int_id_ed]['_d_']['pressures']
        T_raw = ADS['v']['_ed_'][int_id_ed]['_d_']['temperatures']
        w_raw = ADS['v']['_ed_'][int_id_ed]['_d_']['loadings']
        A_raw = ADS['v']['_ed_'][int_id_ed]['_d_']['adsorptionPotentials']
        W_raw = ADS['v']['_ed_'][int_id_ed]['_d_']['volumetricLoadings']

        # Check for type of experimental data (i.e., isotherms, isobars, isosteres, 
        # adsorption potentials)
        #
        if ADS['v']['_ed_'][int_id_ed]['_m_']['type'] == 'isotherms':
            # Sort row data
            #
            T_unique = sorted(list(set(T_raw)))
            
            # Get correct function w(p,T): Check, if special form is required for vapor pressure or
            # saturated liquid density --> Exceptions are hard coded!
            #
            func_w_pT = fuct_w_pT___ads_isotherm(ADS, EQU_NAME, EQU_ID, WPAIR_CLASS)
            
            # Create figure: Iterate over unique temperatures to calculate and plot data
            #
            min_x = 1e12
            min_y = 1e12
            max_x = 0
            max_y = 0
            
            mape = 0
            no_mape = 0
            
            fig, ax = plt.subplots(1, 1, figsize=(14.9/2.54, 10/2.54))                   
            
            for _, T_unique_i in enumerate(T_unique):
                # Get and sort data
                #
                p_plot = []
                T_plot = []
                w_plot = []

                for tmp_ind, tmp_val in enumerate(T_raw):
                    # Get data sorted by temperatures
                    #
                    if T_unique_i == tmp_val:
                        p_plot.append(p_raw[tmp_ind]/1000)
                        T_plot.append(tmp_val)
                        w_plot.append(w_raw[tmp_ind])
                        
                        w_calc = func_w_pT(p_raw[tmp_ind], tmp_val)

                        min_x = min_x if 0.9*p_raw[tmp_ind]/1000 > min_x else 0.9*p_raw[tmp_ind]/1000
                        min_y = min_y if w_raw[tmp_ind] > min_y else w_raw[tmp_ind]
                        
                        max_x = max_x if 1.1*p_raw[tmp_ind]/1000 < max_x else 1.1*p_raw[tmp_ind]/1000
                        max_y = max_y if w_raw[tmp_ind] < max_y else w_raw[tmp_ind]
                        if np.isfinite(w_calc):
                            max_y = max_y if w_calc < max_y else w_calc
                        
                            mape += np.abs(w_raw[tmp_ind] - w_calc) / max(w_raw[tmp_ind], 1e-12)
                            no_mape += 1

                w_calc = [func_w_pT(tmp_val*1000, T_unique_i) for tmp_val in sorted(p_plot)]

                # Plot data
                #
                ax.scatter(p_plot, w_plot, label='Exp.: ' + str(T_unique_i) + ' K')
                ax.plot(sorted(p_plot),  w_calc, label='Calc.: ' + str(T_unique_i) + ' K')

                # Set axis
                #
                ax.set_xlabel(r'Pressure $p$ / kPa $\longrightarrow$')
                ax.set_ylabel(r'Loading $w$ / kg kg$^{-1}$ $\longrightarrow$')

        elif ADS['v']['_ed_'][int_id_ed]['_m_']['type'] == 'isobars':
            # Sort row data
            #
            p_unique = sorted(list(set(p_raw)))
            
            # Get correct function w(p,T): Check, if special form is required for vapor pressure or
            # saturated liquid density --> Exceptions are hard coded!
            #
            func_w_pT = fuct_w_pT___ads_isotherm(ADS, EQU_NAME, EQU_ID, WPAIR_CLASS)
            
            # Create figure: Iterate over unique temperatures to calculate and plot data
            #
            min_x = 1e12
            min_y = 1e12
            max_x = 0
            max_y = 0
            
            mape = 0
            no_mape = 0
            
            fig, ax = plt.subplots(1, 1, figsize=(14.9/2.54, 10/2.54))                   
            
            for _, p_unique_i in enumerate(p_unique):
                # Get and sort data
                #
                p_plot = []
                T_plot = []
                w_plot = []

                for tmp_ind, tmp_val in enumerate(p_raw):
                    # Get data sorted by pressures
                    #
                    if p_unique_i == tmp_val:
                        p_plot.append(tmp_val/1000)
                        T_plot.append(T_raw[tmp_ind])
                        w_plot.append(w_raw[tmp_ind])
                        
                        w_calc = func_w_pT(tmp_val, T_raw[tmp_ind])

                        min_x = min_x if 0.9*T_raw[tmp_ind] > min_x else 0.9*T_raw[tmp_ind]
                        min_y = min_y if w_raw[tmp_ind] > min_y else w_raw[tmp_ind]
                        
                        max_x = max_x if 1.1*T_raw[tmp_ind] < max_x else 1.1*T_raw[tmp_ind]
                        max_y = max_y if w_raw[tmp_ind] < max_y else w_raw[tmp_ind]
                        if np.isfinite(w_calc):
                            max_y = max_y if w_calc < max_y else w_calc
                        
                            mape += np.abs(w_raw[tmp_ind] - w_calc) / max(w_raw[tmp_ind], 1e-12)
                            no_mape += 1

                w_calc = [func_w_pT(p_unique_i, tmp_val) for tmp_val in sorted(T_plot)]

                # Plot data
                #
                ax.scatter(T_plot, w_plot, label='Exp.: ' + str(p_unique_i) + ' Pa')
                ax.plot(sorted(T_plot),  w_calc, label='Calc.: ' + str(p_unique_i) + ' Pa')

                # Set axis
                #
                ax.set_xlabel(r'Temperature $T$ / K $\longrightarrow$')
                ax.set_ylabel(r'Loading $w$ / kg kg$^{-1}$ $\longrightarrow$')

        elif ADS['v']['_ed_'][int_id_ed]['_m_']['type'] == 'isosteres':
            # Sort row data
            #
            w_unique = sorted(list(set(w_raw)))
            
            # Create figure: Iterate over unique temperatures to calculate and plot data
            #
            min_x = 1e12
            min_y = 1e12
            max_x = -1e12
            max_y = -1e12
            
            mape = 0
            no_mape = 0
            
            fig, ax = plt.subplots(1, 1, figsize=(14.9/2.54, 10/2.54))                   
            
            for _, w_unique_i in enumerate(w_unique):
                # Get and sort data
                #
                p_plot = []
                T_plot = []
                w_plot = []

                for tmp_ind, tmp_val in enumerate(w_raw):
                    # Get data sorted by loadings
                    #
                    if w_unique_i == tmp_val:
                        p_plot.append(p_raw[tmp_ind]/1000)
                        T_plot.append(T_raw[tmp_ind])
                        w_plot.append(tmp_val)
                        
                        p_calc = WPAIR_CLASS.Ads.p_wT(tmp_val, T_raw[tmp_ind])

                        min_x = min_x if -1/T_raw[tmp_ind]*1.1 > min_x else -1/T_raw[tmp_ind]*1.1
                        min_y = min_y if p_raw[tmp_ind]/1000 > min_y else p_raw[tmp_ind]/1000
                        
                        max_x = max_x if -1/T_raw[tmp_ind]*0.9 < max_x else -1/T_raw[tmp_ind]*0.9
                        max_y = max_y if p_raw[tmp_ind]/1000 < max_y else p_raw[tmp_ind]/1000
                        if np.isfinite(p_calc):
                            max_y = max_y if p_calc/1000 < max_y else p_calc/1000
                        
                            mape += np.abs(p_raw[tmp_ind] - p_calc) / max(p_raw[tmp_ind], 1e-12)
                            no_mape += 1

                p_calc = [WPAIR_CLASS.Ads.p_wT(w_unique_i, tmp_val)/1000 for tmp_val in sorted(T_plot)]

                # Plot data
                #
                ax.scatter(-1/np.array(T_plot), p_plot, 
                           label='Exp.: ' + str(w_unique_i*100) + ' %')
                ax.plot(-1/np.array(sorted(T_plot)),  p_calc, 
                        label='Calc.: ' + str(w_unique_i*100) + ' %')

                # Set axis
                #
                ax.set_yscale('log')
                
                ax.set_xlabel(r'Temperature $-1/T$ / -K$^{-1}$ $\longrightarrow$')
                ax.set_ylabel(r'Pressure $p$ / kPa $\longrightarrow$')

        else:
            # Create figure: Iterate over unique temperatures to calculate and plot data
            #
            min_x = 1e12
            min_y = 1e12
            max_x = 0
            max_y = 0
            
            mape = 0
            no_mape = 0
            
            fig, ax = plt.subplots(1, 1, figsize=(14.9/2.54, 10/2.54))       
            
            for tmp_ind, tmp_val in enumerate(A_raw):
                W_calc = WPAIR_CLASS.Ads.Vol.W_ARho(tmp_val, 1)

                min_x = min_x if 0.9*A_raw[tmp_ind] > min_x else 0.9*A_raw[tmp_ind]
                min_y = min_y if W_raw[tmp_ind] > min_y else W_raw[tmp_ind]
                
                max_x = max_x if 1.1*A_raw[tmp_ind] < max_x else 1.1*A_raw[tmp_ind]
                max_y = max_y if W_raw[tmp_ind] < max_y else W_raw[tmp_ind]
                if np.isfinite(W_calc):
                    max_y = max_y if W_calc < max_y else W_calc
                
                    mape += np.abs(W_raw[tmp_ind] - W_calc) / max(W_raw[tmp_ind], 1e-12)
                    no_mape += 1

                W_calc = [WPAIR_CLASS.Ads.Vol.W_ARho(tmp_val, 1) for tmp_val in sorted(A_raw)]

            # Plot data
            #
            ax.scatter(A_raw, W_raw,  label='Experiment')
            ax.plot(sorted(A_raw), W_calc, label='Calculated')

            # Set axis
            #
            ax.set_xlabel(r'Adsorption potential $A$ / J mol$^{-1}$ $\longrightarrow$')
            ax.set_ylabel(r'Volumetric loading $W$ / m$^{3}$ kg$^{-1}$ $\longrightarrow$')

        # Set axis
        #
        ax.spines['top'].set_visible(False)
        ax.spines['right'].set_visible(False)
        ax.spines['left'].set_linewidth(0.5)
        ax.spines['bottom'].set_linewidth(0.5)
        
        ax.set_xlim(min_x, max_x)
        ax.set_ylim(max(0, 0.9*min_y), 1.1*max_y)

        ax.grid(linestyle = ':', color = '#9C9E9F')

        ax.legend(loc='best', labelspacing=0.05, frameon=True)
                            
        fig.align_ylabels()
        fig.tight_layout()

        # Save plot
        #
        str_dir = os.path.join(PATH_WRAPPER, 'sorpproplib', 'doc', 'figs', 'ads')
        str_name = 'ads_' + str_file(STR_REFRIGERANT) + '_' \
                   + str_file(STR_SORBENT) + '_'  \
                   + str_file(STR_SUBTYPE) + '_' + EQU_NAME +  '_' + str(EQU_ID+1) + '.pdf'
        str_path = os.path.join(str_dir, str_name)

        if not os.path.exists(str_dir):
            os.mkdir(str_dir)

        fig.savefig(str_path)
        plt.close()
            
        # Delete working pair
        #
        del(WPAIR_CLASS)
                          
        # Write formatted LaTeX string containing information about selected refrigerant functions
        #
        STR_SPECIAL_REF_FUNC = '\\item Special refrigerant functions as described by comment and CoolProp'      + '\n' if \
            ADS['v']['_ep_'][EQU_NAME][EQU_ID]['_n_'] else ''
        STR_REF_FUNC = 'To generate the figure, the following refrigerant functions were selected:'    + '\n'   + \
                       '\\begin{itemize}'                                                              + '\n'   + \
                       '\\item Vapor pressure: ' + STR_VAPOR_PRESSURE.replace('_','\_') + ' - ID 1'    + '\n'   + \
                       '\\item Saturated liquid density: ' + STR_DENSITY.replace('_','\_') + ' - ID 1' + '\n'   + \
                       STR_SPECIAL_REF_FUNC                                                                     + \
                       '\\end{itemize}'                                                                + '\n\n'

        # Write formatted LaTeX string containing information about uncertainity
        #
        STR_UNC_DS = str(ADS['v']['_ed_'][int_id_ed]['_u_']['data-source'])
        STR_UNC_P_ABS = str(ADS['v']['_ed_'][int_id_ed]['_u_']['pressure-abs'])
        STR_UNC_P_REL = str(ADS['v']['_ed_'][int_id_ed]['_u_']['pressure-rel'])
        STR_UNC_T_ABS = str(ADS['v']['_ed_'][int_id_ed]['_u_']['temperature-abs'])
        STR_UNC_T_REL = str(ADS['v']['_ed_'][int_id_ed]['_u_']['temperature-rel'])
        STR_UNC_W_ABS = str(ADS['v']['_ed_'][int_id_ed]['_u_']['loading-abs'])
        STR_UNC_W_REL = str(ADS['v']['_ed_'][int_id_ed]['_u_']['loading-rel'])
        STR_UNC_A_ABS = str(ADS['v']['_ed_'][int_id_ed]['_u_']['adsorptionPotential-abs'])
        STR_UNC_A_REL = str(ADS['v']['_ed_'][int_id_ed]['_u_']['adsorptionPotential-rel'])
        STR_UNC_VW_ABS = str(ADS['v']['_ed_'][int_id_ed]['_u_']['volumetricLoading-abs'])
        STR_UNC_VW_REL = str(ADS['v']['_ed_'][int_id_ed]['_u_']['volumetricLoading-rel'])
    
        LIST_PROPERTIES = ['Data source $\\,\\to\\,$ Data was taken from ', 
                           'Pressure, absolute, in $\\si{\\pascal}$ $\\,\\to\\,$ ',
                           'Pressure, relative, in \% $\\,\\to\\,$ ',
                           'Temperature, absolute, in $\\si{\\kelvin}$ $\\,\\to\\,$ ',
                           'Temperature, relative, in \% $\\,\\to\\,$ ',
                           'Loading, absolute, in $\\si{\\kilogram\\per\\kilogram}$ $\\,\\to\\,$ ',
                           'Loading, relative, in \% $\\,\\to\\,$ ',
                           'Adsorption potential, absolute, in $\\si{\\joule\\per\\mole}$ $\\,\\to\\,$ ',
                           'Adsorption potential, relative, in \% $\\,\\to\\,$ ',
                           'Volumetric loading, absolute, in $\\si{\\cuibe\\meter\\per\\kilogram}$ $\\,\\to\\,$ ',
                           'Volumetric loading, relative, in \% $\\,\\to\\,$ ']
    
        LIST_ENTRIES = [STR_UNC_DS,
                        STR_UNC_P_ABS,
                        STR_UNC_P_REL,
                        STR_UNC_T_ABS,
                        STR_UNC_T_REL,
                        STR_UNC_W_ABS,
                        STR_UNC_W_REL,
                        STR_UNC_A_ABS,
                        STR_UNC_A_REL,
                        STR_UNC_VW_ABS,
                        STR_UNC_VW_REL]
    
        # Check if content exists
        #
        flag_entry_unc = False
    
        for val in LIST_ENTRIES:
            if val:
                flag_entry_unc = True
                break
    
        if flag_entry_unc:
            STR_UNC = 'The uncertainity of the experimental data is:' + '\n' + \
                      '\\begin{itemize}'                              + '\n'
    
            for ind, val in enumerate(LIST_ENTRIES):
                if val:
                    STR_UNC += '\\item ' + LIST_PROPERTIES[ind] + val + '\n'
    
            STR_UNC += '\\end{itemize}' + '\n\n'
    
        else:
            STR_UNC = ''
                 
        # Write formatted LaTeX string containing information about error
        #
        STR_ERROR = 'The mean absolute percentage error (MAPE) between the experimental ' + \
            'and calculated data results in ' + str(round(mape/max(1,no_mape) * 100, 2)) + '\%.'
            
        # Write formatted LaTeX string containing all information
        #
        STR_LATEX = STR_VISUALIZATION_START                                             + '\n'  + \
                    '\\begin{figure}[!htp]'                                             + '\n'  + \
                    '{\\noindent\\includegraphics[height=10cm, keepaspectratio]{figs/ads/'      + \
                    str_name + '}}'                                                     + '\n'  + \
                    '\\end{figure}'                                                     + '\n'  + \
                    '%'                                                                 + '\n\n'+ \
                    STR_REF_FUNC                                                                + \
                    STR_UNC                                                                     + \
                    STR_ERROR                                                                   + \
                    STR_VISUALIZATION_END

    else:
        STR_LATEX = STR_VISUALIZATION_START                                             + '\n'  + \
                    '\\newline'                                                         + '\n'  + \
                    'No experimental data exists. Thus, isotherm is not visualized!'    + '\n'  + \
                    '%'                                                                 + '\n'  + \
                    STR_VISUALIZATION_END

    # Return formatted LaTeX string
    #
    return STR_LATEX

def write_abs_viszualization(ABS: dict, EQU_NAME: str, EQU_ID: int,
                             REFS: list, PATH_WRAPPER: str,
                             PATH_SORPPROPLIB: str, PATH_DB: str) -> str:
    r"""Generate formatted LaTeX string describing visualization of adsorption isotherm equation.

    Parameters
    ----------
    ABS : dict
        Dict containing properties of current adsorption working pair extrated from data
        base (i.e., JSON).
    EQU_NAME : str
        Name of refrigerant functions for which overview table shall be generated.
    EQU_ID : int
        ID of refrigerant functions for which overview table shall be generated.
    REFS : list
        List containing all refrigerants implemented in data base.
    PATH_WRAPPER : str
        Path to Python wrapper.
    PATH_SORPPROPLIB : str
        Path to DLL of SorpPropLib.
    PATH_DB : str
        Path to JSON data base of SorpPropLib.

    Returns
    -------
    str
        Formatted LaTeX string describing visualization of refrigerant equation.

    """
    # Create figure if possible: Creation only possible, if experimental data exisits.
    #
    flag_data_exist = False

    STR_SORBENT = ABS['k']['_as_']
    STR_SUBTYPE = ABS['k']['_st_']
    STR_REFRIGERANT =ABS['k']['_rf_']
    STR_REFERRENCE = ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_c_']
    
    # Existance of experimental data is checked by checking for same reference
    #
    for int_id_ed, val in enumerate(ABS['v']['_ed_']):
        if val['_c_'] == STR_REFERRENCE:
            flag_data_exist = True
            break

    if flag_data_exist:
        # Check for available refrigerant functions and select them to create a working pair object
        #
        STR_VAPOR_PRESSURE = ''
        STR_DENSITY = ''
        
        for _, ref_i in enumerate(REFS):
            if ref_i['k']['_rf_'] == STR_REFRIGERANT:
                if 'VaporPressure_EoS1' in ref_i['v']['_ep_']:
                    STR_VAPOR_PRESSURE = 'VaporPressure_EoS1'
                elif 'VaporPressure_EoS2' in ref_i['v']['_ep_']:
                    STR_VAPOR_PRESSURE = 'VaporPressure_EoS2'
                elif 'VaporPressure_EoS3' in ref_i['v']['_ep_']:
                    STR_VAPOR_PRESSURE = 'VaporPressure_EoS3'
                elif 'VaporPressure_EoSCubic' in ref_i['v']['_ep_']:
                    STR_VAPOR_PRESSURE = 'VaporPressure_EoSCubic'
                elif 'VaporPressure_Antoine' in ref_i['v']['_ep_']:
                    STR_VAPOR_PRESSURE = 'VaporPressure_Antoine'
                else:
                    STR_VAPOR_PRESSURE = 'NoVaporPressure'
    
                if 'SaturatedLiquidDensity_EoS1' in ref_i['v']['_ep_']:
                    STR_DENSITY = 'SaturatedLiquidDensity_EoS1'
                else:
                    STR_DENSITY = 'NoSaturatedLiquidDensity'
    
                break
            
        if not STR_VAPOR_PRESSURE and (EQU_NAME != 'Antoine' or EQU_NAME != 'Duehring' or EQU_NAME != 'MixingRule'):
            # Vapor pressure is required for calculations
            #
            flag_data_exist = False
            print('\n Absorption working pair for that no equation for vapor pressure exists:')
            print(STR_SORBENT)
            print(STR_SUBTYPE)
            print(STR_REFRIGERANT)
            print(EQU_NAME)
            
    if flag_data_exist:
        # Create working pair
        #
        WPAIR = {"sorbent": STR_SORBENT,
                 "subtype": STR_SUBTYPE,
                 "refrigerant": STR_REFRIGERANT}
        WPAIR_APPROACH = {"isotherm": EQU_NAME,
                          "id_isotherm": EQU_ID+1,
                          "vapor_pressure": STR_VAPOR_PRESSURE,
                          "id_vapor_pressure": 1,
                          "sat_liq_density": STR_DENSITY,
                          "id_sat_liq_density": 1}
        WPAIR_CLASS = WPairStruct(WPAIR, WPAIR_APPROACH, PATH_SORPPROPLIB, PATH_DB)
    
        # Get raw data and sort it
        #
        p_raw = ABS['v']['_ed_'][int_id_ed]['_d_']['pressures']
        T_raw = ABS['v']['_ed_'][int_id_ed]['_d_']['temperatures']
        w_raw = ABS['v']['_ed_'][int_id_ed]['_d_']['loadings']

        # Check for type of experimental data (i.e., isotherms, isobars, isosteres, 
        # adsorption potentials)
        #
        if ABS['v']['_ed_'][int_id_ed]['_m_']['type'] == 'isotherms':
            # Sort row data
            #
            T_unique = sorted(list(set(T_raw)))
            
            # Get correct function w(p,T): Check, if special form is required for vapor pressure or
            # saturated liquid densities --> Exceptions are hard coded!
            #
            func_w_pT = fuct_w_pT___abs_isotherms(ABS, EQU_NAME, EQU_ID, WPAIR_CLASS)
            
            # Create figure: Iterate over unique temperatures to calculate and plot data
            #
            min_x = 1e12
            min_y = 1e12
            max_x = 0
            max_y = 0
            
            mape = 0
            no_mape = 0
            
            fig, ax = plt.subplots(1, 1, figsize=(14.9/2.54, 10/2.54))                   
            
            for _, T_unique_i in enumerate(T_unique):
                # Get and sort data
                #
                p_plot = []
                T_plot = []
                w_plot = []

                for tmp_ind, tmp_val in enumerate(T_raw):
                    # Get data sorted by temperatures
                    #
                    if T_unique_i == tmp_val:
                        p_plot.append(p_raw[tmp_ind]/1000)
                        T_plot.append(tmp_val)
                        w_plot.append(w_raw[tmp_ind])
                        
                        w_calc = func_w_pT(p_raw[tmp_ind], tmp_val)

                        min_x = min_x if 0.9*p_raw[tmp_ind]/1000 > min_x else 0.9*p_raw[tmp_ind]/1000
                        min_y = min_y if w_raw[tmp_ind] > min_y else w_raw[tmp_ind]
                        
                        max_x = max_x if 1.1*p_raw[tmp_ind]/1000 < max_x else 1.1*p_raw[tmp_ind]/1000
                        max_y = max_y if w_raw[tmp_ind] < max_y else w_raw[tmp_ind]
                        if np.isfinite(w_calc):
                            max_y = max_y if w_calc < max_y else w_calc
                        
                            mape += np.abs(w_raw[tmp_ind] - w_calc) / max(w_raw[tmp_ind], 1e-12)
                            no_mape += 1

                w_calc = [func_w_pT(tmp_val*1000, T_unique_i) for tmp_val in sorted(p_plot)]

                # Plot data
                #
                ax.scatter(p_plot, w_plot, label='Exp.: ' + str(T_unique_i) + ' K')
                ax.plot(sorted(p_plot),  w_calc, label='Calc.: ' + str(T_unique_i) + ' K')

                # Set axis
                #
                ax.set_xlabel(r'Pressure $p$ / kPa $\longrightarrow$')
                ax.set_ylabel(r'Conc. $X$ or molar fraction $x$  / kg kg$^{-1}$ or mol mol$^{-1}$ $\longrightarrow$')
                
        elif ABS['v']['_ed_'][int_id_ed]['_m_']['type'] == 'isotherms_p':
            # Sort row data
            #
            T_unique = sorted(list(set(T_raw)))
            
            # Get correct function w(p,T): Check, if special form is required for vapor pressure or
            # saturated liquid densities --> Exceptions are hard coded!
            #
            func_p_wT = fuct_p_wT___abs_isosteres(ABS, EQU_NAME, EQU_ID, WPAIR_CLASS)
            
            # Create figure: Iterate over unique temperatures to calculate and plot data
            #
            min_x = 1e12
            min_y = 1e12
            max_x = 0
            max_y = 0
            
            mape = 0
            no_mape = 0
            
            fig, ax = plt.subplots(1, 1, figsize=(14.9/2.54, 10/2.54))                   
            
            for _, T_unique_i in enumerate(T_unique):
                # Get and sort data
                #
                p_plot = []
                T_plot = []
                w_plot = []

                for tmp_ind, tmp_val in enumerate(T_raw):
                    # Get data sorted by temperatures
                    #
                    if T_unique_i == tmp_val:
                        p_plot.append(p_raw[tmp_ind]/1000)
                        T_plot.append(tmp_val)
                        w_plot.append(w_raw[tmp_ind])
                        
                        p_calc = func_p_wT(w_raw[tmp_ind], tmp_val)

                        min_x = min_x if 0.9*w_raw[tmp_ind] > min_x else 0.9*w_raw[tmp_ind]
                        min_y = min_y if p_raw[tmp_ind]/1000 > min_y else p_raw[tmp_ind]/1000
                        
                        max_x = max_x if 1.1*w_raw[tmp_ind] < max_x else 1.1*w_raw[tmp_ind]
                        max_y = max_y if p_raw[tmp_ind]/1000 < max_y else p_raw[tmp_ind]/1000
                        if np.isfinite(p_calc):
                            max_y = max_y if p_calc/1000 < max_y else p_calc/1000
                        
                            mape += np.abs(p_raw[tmp_ind] - p_calc) / max(p_raw[tmp_ind], 1e-12)
                            no_mape += 1

                p_calc = [func_p_wT(tmp_val, T_unique_i)/1000 for tmp_val in sorted(w_plot)]

                # Plot data
                #
                ax.scatter(w_plot, p_plot, label='Exp.: ' + str(T_unique_i) + ' K')
                ax.plot(sorted(w_plot),  p_calc, label='Calc.: ' + str(T_unique_i) + ' K')

                # Set axis
                #
                ax.set_xlabel(r'Conc. $X$ or molar fraction $x$  / kg kg$^{-1}$ or mol mol$^{-1}$ $\longrightarrow$')
                ax.set_ylabel(r'Pressure $p$ / kPa $\longrightarrow$')

        elif ABS['v']['_ed_'][int_id_ed]['_m_']['type'] == 'isosteres':
            # Sort row data
            #
            w_unique = sorted(list(set(w_raw)))
            
            # Get correct function w(p,T): Check, if special form is required for vapor pressure or
            # saturated liquid densities --> Exceptions are hard coded!
            #
            func_p_wT = fuct_p_wT___abs_isosteres(ABS, EQU_NAME, EQU_ID, WPAIR_CLASS)
            
            # Create figure: Iterate over unique temperatures to calculate and plot data
            #
            min_x = 1e12
            min_y = 1e12
            max_x = -1e12
            max_y = -1e12
            
            mape = 0
            no_mape = 0
            
            fig, ax = plt.subplots(1, 1, figsize=(14.9/2.54, 10/2.54))                   
            
            for _, w_unique_i in enumerate(w_unique):
                # Get and sort data
                #
                p_plot = []
                T_plot = []
                w_plot = []

                for tmp_ind, tmp_val in enumerate(w_raw):
                    # Get data sorted by loadings
                    #
                    if w_unique_i == tmp_val:
                        p_plot.append(p_raw[tmp_ind]/1000)
                        T_plot.append(T_raw[tmp_ind])
                        w_plot.append(tmp_val)
                        
                        p_calc = func_p_wT(tmp_val, T_raw[tmp_ind])

                        min_x = min_x if -1/T_raw[tmp_ind]*1.1 > min_x else -1/T_raw[tmp_ind]*1.1
                        min_y = min_y if p_raw[tmp_ind]/1000 > min_y else p_raw[tmp_ind]/1000
                        
                        max_x = max_x if -1/T_raw[tmp_ind]*0.9 < max_x else -1/T_raw[tmp_ind]*0.9
                        max_y = max_y if p_raw[tmp_ind]/1000 < max_y else p_raw[tmp_ind]/1000
                        if np.isfinite(p_calc):
                            max_y = max_y if p_calc/1000 < max_y else p_calc/1000
                        
                            mape += np.abs(p_raw[tmp_ind] - p_calc) / max(p_raw[tmp_ind], 1e-12)
                            no_mape += 1

                p_calc = [func_p_wT(w_unique_i, tmp_val)/1000 for tmp_val in sorted(T_plot)]

                # Plot data
                #
                ax.scatter(-1/np.array(T_plot), p_plot, 
                           label='Exp.: ' + str(round(w_unique_i*100,2)) + ' %')
                ax.plot(-1/np.array(sorted(T_plot)),  p_calc, 
                        label='Calc.: ' + str(round(w_unique_i*100,2)) + ' %')

                # Set axis
                #
                ax.set_yscale('log')
                
                ax.set_xlabel(r'Temperature $-1/T$ / -K$^{-1}$ $\longrightarrow$')
                ax.set_ylabel(r'Pressure $p$ / kPa $\longrightarrow$')
                
        else:
            print('/n Absorption working pair for that no equation for vapor pressure exists:')
            print(STR_SORBENT)
            print(STR_SUBTYPE)
            print(STR_REFRIGERANT)
            print(EQU_NAME)

        # Set axis
        #
        ax.spines['top'].set_visible(False)
        ax.spines['right'].set_visible(False)
        ax.spines['left'].set_linewidth(0.5)
        ax.spines['bottom'].set_linewidth(0.5)
        
        ax.set_xlim(min_x, max_x)
        ax.set_ylim(max(0, 0.9*min_y), 1.1*max_y)

        ax.grid(linestyle = ':', color = '#9C9E9F')

        ax.legend(loc='best', labelspacing=0.05, frameon=True)
                            
        fig.align_ylabels()
        fig.tight_layout()

        # Save plot
        #
        str_dir = os.path.join(PATH_WRAPPER, 'sorpproplib', 'doc', 'figs', 'abs')
        str_name = 'abs_' + str_file(STR_REFRIGERANT) + '_' \
                   + str_file(STR_SORBENT) + '_'  \
                   + str_file(STR_SUBTYPE) + '_' + EQU_NAME +  '_' + str(EQU_ID+1) + '.pdf'
        str_path = os.path.join(str_dir, str_name)

        if not os.path.exists(str_dir):
            os.mkdir(str_dir)

        fig.savefig(str_path)
        plt.close()
            
        # Delete working pair
        #
        del(WPAIR_CLASS)
                          
        # Write formatted LaTeX string containing information about selected refrigerant functions
        #
        STR_SPECIAL_REF_FUNC = '\\item Special refrigerant functions as described by comment and CoolProp'      + '\n' if \
            ABS['v']['_ep_'][EQU_NAME][EQU_ID]['_n_'] else ''
        STR_REF_FUNC = 'To generate the figure, the following refrigerant functions were selected:'    + '\n'   + \
                       '\\begin{itemize}'                                                              + '\n'   + \
                       '\\item Vapor pressure: ' + STR_VAPOR_PRESSURE.replace('_','\_') + ' - ID 1'    + '\n'   + \
                       '\\item Saturated liquid density: ' + STR_DENSITY.replace('_','\_') + ' - ID 1' + '\n'   + \
                       STR_SPECIAL_REF_FUNC                                                                     + \
                       '\\end{itemize}'                                                                + '\n\n'

        # Write formatted LaTeX string containing information about uncertainity
        #
        STR_UNC_DS = str(ABS['v']['_ed_'][int_id_ed]['_u_']['data-source'])
        STR_UNC_P_ABS = str(ABS['v']['_ed_'][int_id_ed]['_u_']['pressure-abs'])
        STR_UNC_P_REL = str(ABS['v']['_ed_'][int_id_ed]['_u_']['pressure-rel'])
        STR_UNC_T_ABS = str(ABS['v']['_ed_'][int_id_ed]['_u_']['temperature-abs'])
        STR_UNC_T_REL = str(ABS['v']['_ed_'][int_id_ed]['_u_']['temperature-rel'])
        STR_UNC_W_ABS = str(ABS['v']['_ed_'][int_id_ed]['_u_']['loading-abs'])
        STR_UNC_W_REL = str(ABS['v']['_ed_'][int_id_ed]['_u_']['loading-rel'])
    
        LIST_PROPERTIES = ['Data source $\\,\\to\\,$ Data was taken from ', 
                           'Pressure, absolute, in $\\si{\\pascal}$ $\\,\\to\\,$ ',
                           'Pressure, relative, in \% $\\,\\to\\,$ ',
                           'Temperature, absolute, in $\\si{\\kelvin}$ $\\,\\to\\,$ ',
                           'Temperature, relative, in \% $\\,\\to\\,$ ',
                           'Concentration / Molar fraction, absolute, in $\\si{\\kilogram\\per\\kilogram}$ / $\\si{\\mole\\per\\mole}$ $\\,\\to\\,$ ',
                           'Concentration / Molar fraction, relative, in \% $\\,\\to\\,$ ',]
    
        LIST_ENTRIES = [STR_UNC_DS,
                        STR_UNC_P_ABS,
                        STR_UNC_P_REL,
                        STR_UNC_T_ABS,
                        STR_UNC_T_REL,
                        STR_UNC_W_ABS,
                        STR_UNC_W_REL]
    
        # Check if content exists
        #
        flag_entry_unc = False
    
        for val in LIST_ENTRIES:
            if val:
                flag_entry_unc = True
                break
    
        if flag_entry_unc:
            STR_UNC = 'The uncertainity of the experimental data is:' + '\n' + \
                      '\\begin{itemize}'                              + '\n'
    
            for ind, val in enumerate(LIST_ENTRIES):
                if val:
                    STR_UNC += '\\item ' + LIST_PROPERTIES[ind] + val + '\n'
    
            STR_UNC += '\\end{itemize}' + '\n\n'
    
        else:
            STR_UNC = ''
                 
        # Write formatted LaTeX string containing information about error
        #
        STR_ERROR = 'The mean absolute percentage error (MAPE) between the experimental ' + \
            'and calculated data results in ' + str(round(mape/max(1,no_mape) * 100, 2)) + '\%.'
            
        # Write formatted LaTeX string containing all information
        #
        STR_LATEX = STR_VISUALIZATION_START                                             + '\n'  + \
                    '\\begin{figure}[!htp]'                                             + '\n'  + \
                    '{\\noindent\\includegraphics[height=10cm, keepaspectratio]{figs/abs/'      + \
                    str_name + '}}'                                                     + '\n'  + \
                    '\\end{figure}'                                                     + '\n'  + \
                    '%'                                                                 + '\n\n'+ \
                    STR_REF_FUNC                                                                + \
                    STR_UNC                                                                     + \
                    STR_ERROR                                                                   + \
                    STR_VISUALIZATION_END

    else:
        STR_LATEX = STR_VISUALIZATION_START                                             + '\n'  + \
                    '\\newline'                                                         + '\n'  + \
                    'No experimental data exists. Thus, isotherm is not visualized!'    + '\n'  + \
                    '%'                                                                 + '\n'  + \
                    STR_VISUALIZATION_END

    # Return formatted LaTeX string
    #
    return STR_LATEX
