# -*- coding: utf-8 -*-
"""
Created on Thu Mar  5 17:55:58 2020

@author: Engelpracht
"""

#%% Import relevant libraries
#
from typing import Union

from json import dumps
from os import listdir
from os.path import isfile, join
from pandas import read_csv, DataFrame, Series


#%% Define global constants used to read *.csv-files and to create JSON-file
#
# Definition of paths
#
PATH_COEFFICIENTS = 'data\\JSON\\data_coefficients\\'
PATH_EXPERIMENTS = 'data\\JSON\\data_experiments\\'
PATH_JSON = 'data\\JSON\\'
PATH_PYTHON = 'data\\64bit\\'


# Definition of names of columns of data frames
#
# a) Names that are required for several entries
#
DF_REF_WPAIR_COLUMNS = ['sorbent', 'refrigerant', 'sorbent-subtype', 'type']
DF_EQUATION_COLUMNS = ['literature']
DF_PROP_COLUMNS = ['prop-diameter-crystal', 'prop-diameter-pore',
                   'prop-diameter-pellet', 'prop-diameter-extrudate',
                   'prop-length-extrudate', 'prop-area-surface',
                   'prop-volume-pore', 'props-porosity-pellet',
                   'props-density-bulk', 'props-density-pellet',
                   'props-density-solid']

# b) Names that are required for entries of coefficient
#
DF_VALIDITY_COLUMNS = ['validity-pressure-min', 'validity-pressure-max',
                       'validity-temperature-min', 'validity-temperature-max',
                       'validity-loading-min', 'validity-loading-max']
DF_ERROR_COLUMNS = ['error-are', 'error-rmse']

# c) Names that are required for entries of experimental data
#
DF_MEASUREMENT_COLUMNS = ['measurements-procedure', 'measurements-type']
DF_UNCERTAINTY_COLUMNS = ['uncertainty-pressure-abs',
                          'uncertainty-pressure-rel',
                          'uncertainty-temperature-abs',
                          'uncertainty-temperature-rel',
                          'uncertainty-loading-abs',
                          'uncertainty-loading-rel',
                          'uncertainty-adsorptionPotential-abs',
                          'uncertainty-adsorptionPotential-rel',
                          'uncertainty-volumetricLoading-abs',
                          'uncertainty-volumetricLoading-rel']


# Definition of names of dicts
#
# a) Keys that are more general
#
DICT_BASE_KEYS = ['k', 'v']

DICT_K_KEYS = ['_as_', '_rf_', '_st_', '_tp_']
DICT_V_KEYS = ['_ep_', '_ed_', '_r_', '_s_', '_t_']

DICT_EQUATIONS_KEYS = ['_c_', '_e_', '_pr_', '_va_', '_er_', '_p_']
DICT_ED_KEYS = ['_c_', '_pr_', '_m_', '_u_', '_d_']

DICT_PROP_KEYS = ['diameter-crystal', 'diameter-pore', 'diameter-pellet',
                  'diameter-extrudate', 'length-extrudate', 'area-surface',
                  'volume-pore', 'porosity-pellet', 'density-bulk',
                  'density-pellet', 'density-solid']

# b) Keys that are required for entries of coefficients
#
DICT_VALIDITY_KEYS = ['pressure-min', 'pressure-max',
                      'temperature-min', 'temperature-max',
                      'loading-min', 'loading-max']
DICT_ERROR_KEYS = ['are', 'rmse']

# c) Keys that are required for entries of experimental data
#
DICT_MEASUREMENT_KEYS = ['procedure', 'type']
DICT_UNCERTAINTY_KEYS = ['pressure-abs', 'pressure-rel', 'temperature-abs',
                         'temperature-rel', 'loading-abs', 'loading-rel',
                         'adsorptionPotential-abs', 'adsorptionPotential-rel',
                         'volumetricLoading-abs', 'volumetricLoading-rel']


#%% Definition of sub-functions
#
def read_csv_to_list(path: str) -> Union[list, list]:
    r"""Reads *.csv-files.

    This function reads all *.csv-files that are located in the directory
    specified by the variable 'path'. This function returns a list containing
    all names of the *.csv-files and another list containing all *.csv-files
    saved as data frames.

    Parameters
    ----------
    path : str
        Path that specifies folder in which *.csv-files are located.

    Returns
    -------
    names : list
        Contains all names of files without their type extensions (i.e. .csv).
    files : list
        Contains all files saved as type 'pandas.core.frame.DataFrame'.

    Notes
    -----
    This function does not check if files have typ .csv.

    Examples
    --------
    >>> names, files = read_csv_to_list('C:\\example\\')
    >>> print(names)
    ['dss', 'langmuir']
    >>> print(type(files[0]))
    <class 'pandas.core.frame.DataFrame'>

    History
    -------
    03/07/2020, by Mirko Engelpracht:
        First implementation.
    """

    # Read names of files
    #
    names = [file for file in listdir(path) if isfile(join(path, file))]

    # Import all files as 'pandas.core.frame.DataFrame'
    # Drop second line of all files since it only contains units of values
    #
    files = []
    for _, name_file in enumerate(names):
        tmp_df = read_csv(join(path, name_file),
                          sep=';',
                          encoding='utf_8',
                          keep_default_na=False)
        files.append(tmp_df.drop(0).reset_index(drop=True))


    # Cut file-extensions of file names and return lists
    #
    return [name_file.split('.')[0] for name_file in names], files


def search_df(df_search: DataFrame, columns: list, values: list) -> DataFrame:
    r"""Searches data frame for values in some columns.

    This function searches the data frame 'df_search' for the specified 'values'
    in the specified 'columns' and returns the maching data frame.

    Parameters
    ----------
    df_search : pandas.core.frame.DataFrame
        Data frame that that is being searched for.
    columns : list
        List containing names of the columns to be searched.
    values : list
        List containing values to be searched for.

    Returns
    -------
    pandas.core.frame.DataFrame
        Data frame that only contains searched values.

    Notes
    -----
    Parameters 'columns' and 'values' need to have the same length.

    Examples
    --------
    >>> search_result = search_df(df, ['Column 1'], ['Random value'])
    >>> print(type(search_result))
    <class 'pandas.core.frame.DataFrame'>

    History
    -------
    03/07/2020, by Mirko Engelpracht:
        First implementation.
    """

    # Search for specified values by sequential application of the search
    # criteria
    #
    for no_criterion, value_criterion in enumerate(values):
        df_search = df_search.loc[df_search[columns[no_criterion]] == \
                                  value_criterion]

    # Return search result
    #
    return df_search


def search_wpair(dfs: list, columns: list) -> DataFrame:
    r"""Searches for all available working pais.

    This function counts the number of unique working pairs, which are
    specfied by the 'columns' of the data frame, and returns a data frame that
    only contains the unique working pairs.

    Parameters
    ----------
    dfs : list
        List containing data frames that are searched for.
    columns : list
        List containing names of the columns to be searched.

    Returns
    -------
    pandas.core.frame.DataFrame
        Data frame that only unique working pairs according to 'columns'.

    History
    -------
    03/07/2020, by Mirko Engelpracht:
        First implementation.
    """

    # Create empty data frame and add all working pairs to it
    #
    df_unique = DataFrame(columns=columns)
    for _, val in enumerate(dfs):
        df_unique = df_unique.append(val[columns], ignore_index=True)

    # Drop duplicates and return data frame cointaing all working pairs
    #
    return df_unique.drop_duplicates().reset_index(drop=True)


def create_dict(series: Series, keys: list, convert: bool = False) -> dict:
    r"""Creates a dict for a JSON file.

    This function creates a dict. Therefore, it uses the given 'keys' and takes
    values from the given 'series'. If necessary, the funtion convert values to
    floats when a conversion is possible.

    Parameters
    ----------
    series : pandas.core.series.Series
        Series containing values for the dict.
    keys : list
        Lists containing names of the keys of the dict.
    convert : bool
        Boolean indicating if values shall be converted to type float.

    Returns
    -------
    dict
        Data containing values from series.

    History
    -------
    03/07/2020, by Mirko Engelpracht:
        First implementation.
    """

    # Get series, read values of indices, and convert values if necessary
    #
    names_series = series.index
    values_series = []
    for _, val in enumerate(names_series):
        if convert:
            # Convert values to type float
            #
            if series[val] == '':
                values_series.append(series[val])
            else:
                values_series.append(float(series[val]))

        else:
            # Do not convert values to type float
            #
            values_series.append(series[val])

    # Create dict and return it
    #
    return dict(zip(keys, values_series))


#%% Definition of main-function
#
def create_json_file(path_coef: str, path_exp: str) -> Union[list, str, str]:
    r"""Creates JSON file.

    This function creates the JSON file. Therefore, it needs the path to the
    coefficients of the equations (i.e. 'path_coef') and the path to the
    experimental data (i.e. 'path_exp').

    Parameters
    ----------
    path_coef : str
        Path to *.csv-files that contain coefficients of equations.
    path_exp : str
        Path to *.csv-files that contain experimental data points.

    Returns
    -------
    list_json : list
        List with content of JSON file.
    json_compact : str
        String of JSON file as one line.
    json_formatted : str
        String of JSON file formatted in multiple lines.

    History
    -------
    03/07/2020, by Mirko Engelpracht:
        First implementation.
    """

    # 0) Load all *.csv-files that contain coefficients or experimental data of
    #    refrigerants, adsorption isotherms, and absorption isotherms
    #
    files_coefficients, list_coefficients = read_csv_to_list(path_coef)
    _, list_experiments = read_csv_to_list(path_exp)

    # 1) Get names of all working pairs:
    #
    df_ref_wpair = search_wpair(list_coefficients + list_experiments,
                                DF_REF_WPAIR_COLUMNS)

    # 2) Create dict 'k' that is used to save general information of all
    #    refrigerants and working pairs:
    #
    #       - Iterate over all refrigerants and working pairs
    #       - Save general information as dict
    #
    dict_k = [dict(zip(DICT_K_KEYS, row)) for _, row in df_ref_wpair.iterrows()]

    # 3) Create dict 'v' to save coefficients of equations and experimental
    #    data:
    #
    #       - Iterate over all refrigerants and working pairs
    #       - Create dicts and save dummy values for keys that will be later
    #         used for coefficients of equations and for experimental data
    #
    dict_v = [dict(zip(DICT_V_KEYS, [[], [],
                                     row[DF_REF_WPAIR_COLUMNS[1]],
                                     row[DF_REF_WPAIR_COLUMNS[0]],
                                     row[DF_REF_WPAIR_COLUMNS[2]]]))
              for _, row in df_ref_wpair.iterrows()]

    # 4) Create dict '_ep_' to save coefficients of equations:
    #
    #       - Create dict that contains names of all equations as keys and empty
    #         lists as valeus
    #       - Create list for all refrigerants and working pairs wither former
    #         created dict as attribute
    #       - For each refrigerant and working pair, count how often an equation
    #         occurs by iterating over refrigerants and working pairs first and
    #         iterating over equations second
    #       - For each equation, iterate over available sets of coefficients and
    #         save coresponding coefficients as dict
    #
    list_equations = []
    for _ in range(len(df_ref_wpair)):
        # Create empty dictionaries in which the keys corespond the order of the
        # equations
        #
        dict_equations = dict()
        for _, val in enumerate(files_coefficients):
            dict_equations.update(zip([val], [[]]))
        list_equations.append(dict_equations)

    for ind_ref_wpair, ref_wpair in df_ref_wpair.iterrows():
        for ind_equ, name_equ in enumerate(dict_equations):
            # Search current working pair within current equation data frame
            #
            df_equ = list_coefficients[ind_equ]
            cur_ref_wpair = ref_wpair.tolist()
            df_result = search_df(df_equ, DF_REF_WPAIR_COLUMNS, cur_ref_wpair)

            # Save current equation data frame into coresponding position of
            # 'list_equation' when working pair exists
            #
            df_result = df_result.reset_index(drop=True)
            df_result = [] if df_result.empty else df_result
            list_equations[ind_ref_wpair].update(dict(zip([name_equ],
                                                          [df_result])))

    for ind_ref_wpair, _ in enumerate(dict_v):
        # Count number of different equation approaches for current working pair
        #
        list_name_equ = []
        for _, name_equ in enumerate(list_equations[ind_ref_wpair]):
            if not isinstance(list_equations[ind_ref_wpair][name_equ], list):
                list_name_equ.append(name_equ)

        # Create dummy dict to save coefficient sets for each identified
        # equations of current working pair
        #
        list_equ = [[] for i in range(len(list_name_equ))]
        dict_equ = dict(zip(list_name_equ, list_equ))

        # For each equation, iterate over available sets of coefficients
        # For each set of coefficient, save coefficients as dict
        #
        for _, name_equ in enumerate(dict_equ):
            list_equ = []
            for _, equ_set in list_equations[ind_ref_wpair][name_equ].iterrows():
                # Create dummy dict for each set of coefficients
                #
                list_equ_set = [[] for i in range(len(DICT_EQUATIONS_KEYS))]
                dict_equ_set = dict(zip(DICT_EQUATIONS_KEYS, list_equ_set))

                # Update dummy dict: Literature
                #
                dict_equ_set[DICT_EQUATIONS_KEYS[0]] = \
                    equ_set[DF_EQUATION_COLUMNS[0]]

                # Update dummy dict: Name of equation
                #
                dict_equ_set[DICT_EQUATIONS_KEYS[1]] = name_equ.capitalize()

                # Update dummy dict: Further properties of sorbent
                #
                dict_equ_set[DICT_EQUATIONS_KEYS[2]] = \
                    create_dict(equ_set[DF_PROP_COLUMNS], DICT_PROP_KEYS)

                # Update dummy dict: Valid range
                #
                dict_equ_set[DICT_EQUATIONS_KEYS[3]] = \
                    create_dict(equ_set[DF_VALIDITY_COLUMNS],
                                DICT_VALIDITY_KEYS, True)

                # Update dummy dict: Error
                #
                dict_equ_set[DICT_EQUATIONS_KEYS[4]] = \
                    create_dict(equ_set[DF_ERROR_COLUMNS],
                                DICT_ERROR_KEYS, True)

                # Update dummy dict: Coefficients
                #
                equ_set = equ_set.drop(DF_REF_WPAIR_COLUMNS +
                                       DF_EQUATION_COLUMNS +
                                       DF_PROP_COLUMNS + DF_VALIDITY_COLUMNS +
                                       DF_ERROR_COLUMNS)

                name_par = equ_set.index
                list_par_name = []
                list_par_value = []
                for _, val in enumerate(name_par):
                    list_par_name.append(val)
                    list_par_value.append(float(equ_set[val]))

                dict_equ_set[DICT_EQUATIONS_KEYS[5]] = dict(zip(list_par_name,
                                                                list_par_value))

                # Update list that contains all sets of coefficients
                #
                list_equ.append(dict_equ_set)

            # Update dict that contains different equations
            #
            dict_equ[name_equ] = list_equ

        # Update dict 'v' by adding coefficients of equations
        #
        dict_v[ind_ref_wpair][DICT_V_KEYS[0]] = dict_equ

    # g) Create dict '_ed_' to save experimental data:
    #
    #       - Iterate over all refrigerants and working pairs
    #       - Check if experimental data exist
    #       - Iterate over all refrigerants and working pairs and save
    #         experimental data as dict
    #
    df_exp_data = search_wpair(list_experiments, DF_REF_WPAIR_COLUMNS)

    for ind_ref_wpair, _ in enumerate(dict_v):
        list_exp_data = []

        # Check if experimental data exists for current working pair
        # Thus, save time and skip iterations that are not necessary
        #
        df_cur_ref_wpair = df_ref_wpair.loc[ind_ref_wpair, :]
        name_cur_ref_wpair = df_cur_ref_wpair.tolist()
        df_result = search_df(df_exp_data, DF_REF_WPAIR_COLUMNS,
                              name_cur_ref_wpair)

        # If experimental data exist, save it into dict '_ed_'
        #
        if not df_result.empty:
            # Iterate over all data frames that contain experimental data
            #
            for _, df_exp in enumerate(list_experiments):
                # Search experimental data within data frame for current working
                # pair
                # Note that experimental data only exists within one data frame!
                #
                df_exp = search_df(df_exp, DF_REF_WPAIR_COLUMNS,
                                   name_cur_ref_wpair)

                if not df_exp.empty:
                    # Iterate over all available experimental data for the
                    # current working pair
                    #
                    for _, exp_set in df_exp.iterrows():
                        # Create dummy dict to save current experimental data
                        #
                        list_cur_exp_data = [[] for i in \
                                             range(len(DICT_ED_KEYS))]
                        dict_exp_data = dict(zip(DICT_ED_KEYS,
                                                 list_cur_exp_data))

                        # Update dummy dict: Literature
                        #
                        dict_exp_data[DICT_ED_KEYS[0]] = \
                            exp_set[DF_EQUATION_COLUMNS[0]]

                        # Update dummy dict: Sorbent properties
                        #
                        dict_exp_data[DICT_ED_KEYS[1]] = \
                            create_dict(exp_set[DF_PROP_COLUMNS],
                                        DICT_PROP_KEYS)

                        # Update dummy dict: Measurement information
                        #
                        dict_exp_data[DICT_ED_KEYS[2]] = \
                            create_dict(exp_set[DF_MEASUREMENT_COLUMNS],
                                        DICT_MEASUREMENT_KEYS)

                        # Update dummy dict: Uncertainty information
                        #
                        dict_exp_data[DICT_ED_KEYS[3]] = \
                            create_dict(exp_set[DF_UNCERTAINTY_COLUMNS],
                                        DICT_UNCERTAINTY_KEYS)

                        # Update dummy dict: Measurements
                        #
                        mea_set = exp_set.drop(DF_REF_WPAIR_COLUMNS +
                                               DF_EQUATION_COLUMNS +
                                               DF_PROP_COLUMNS +
                                               DF_MEASUREMENT_COLUMNS +
                                               DF_UNCERTAINTY_COLUMNS)

                        name_par = mea_set.index
                        list_par_value = []
                        for _, val in enumerate(name_par):
                            # Split string and ensure empty lists
                            #
                            measurments = mea_set[val].split(',')
                            measurments = [] if (measurments[0] == '') else \
                                [float(item) for item in measurments]

                            list_par_value.append(measurments)

                        dict_exp_data[DICT_ED_KEYS[4]] = \
                            dict(zip(name_par.to_list(), list_par_value))

                        # Update list containing all experiment data of current
                        # working pair
                        #
                        list_exp_data.append(dict_exp_data)

                    # Update dict 'v' by adding experimental data
                    #
                    dict_v[ind_ref_wpair][DICT_V_KEYS[1]] = list_exp_data

    # 6) Create base dict that containts dicts 'k' and 'v':
    #
    #       - Iterate over all refrigerants and working pairs
    #       - Create dicts by zip-function
    #
    list_json = [dict(zip(DICT_BASE_KEYS, [val, dict_v[ind]])) for ind, val in \
                 enumerate(dict_k)]

    # 7) Save JSON-files
    #
    with open(join(PATH_JSON, "sorpproplib.json"), "w") as text_file:
        json_compact = dumps(list_json)
        text_file.write(json_compact)

    with open(join(PATH_PYTHON, "sorpproplib.json"), "w") as text_file:
        json_compact = dumps(list_json)
        text_file.write(json_compact)

    with open(join(PATH_JSON, "sorpproplib_formatted.json"), "w") as text_file:
        json_formatted = dumps(list_json, indent=4)
        text_file.write(json_formatted)

    # Return data
    #
    return list_json, json_compact, json_formatted


#%% Execute main-function
#
if __name__ == "__main__":
    json_file, str_comp, str_form = \
        create_json_file(PATH_COEFFICIENTS, PATH_EXPERIMENTS)
