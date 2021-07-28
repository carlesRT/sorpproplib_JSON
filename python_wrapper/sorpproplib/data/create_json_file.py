"""
Created on Thu Mar  5 17:55:58 2020.

@author: Engelpracht
"""

# %% Import relevant libraries.
#
from json import dumps
from os import listdir
from os.path import isfile, join
from typing import Union

from pandas import read_csv, DataFrame, Series


# %% Define global constants used to read *.csv-files and to create JSON-file.
#
# Definition of names of columns of data frames: Columns are different for refrigerants, adsorption
# or absorption isotherms; and for equations or experimental data
#
# a) Names that are required for several entries
#
COLUMNS_DF_WPAIR = ['sorbent', 'refrigerant', 'sorbent-subtype', 'type']
COLUMNS_DF_LITERATURE = ['comment', 'literature']

COLUMNS_DF_ADS_PROPERTIES = ['prop-diameter-crystal', 'prop-diameter-pore',
                             'prop-diameter-pellet', 'prop-diameter-extrudate',
                             'prop-length-extrudate', 'prop-area-surface',
                             'prop-volume-pore', 'props-porosity-pellet',
                             'props-density-bulk', 'props-density-pellet',
                             'props-density-solid']

# b) Names that are required for entries of equation coefficients
#
COLUMNS_DF_ERROR = ['error-are', 'error-rmse']

COLUMNS_DF_ABS_VALIDITY = ['validity-temperature-min', 'validity-temperature-max']
COLUMNS_DF_ADS_VALIDITY = ['validity-pressure-min', 'validity-pressure-max',
                           'validity-temperature-min', 'validity-temperature-max',
                           'validity-loading-min', 'validity-loading-max']
COLUMNS_DF_REF_VALIDITY = ['validity-temperature-min', 'validity-temperature-max']

# c) Names that are required for entries of experimental data
#
COLUMNS_DF_MEASUREMENT = ['measurements-procedure', 'measurements-type']

COLUMNS_DF_ADS_UNCERTAINITY = ['uncertainty-data-source',
                               'uncertainty-pressure-abs',
                               'uncertainty-pressure-rel',
                               'uncertainty-temperature-abs',
                               'uncertainty-temperature-rel',
                               'uncertainty-loading-abs',
                               'uncertainty-loading-rel',
                               'uncertainty-adsorptionPotential-abs',
                               'uncertainty-adsorptionPotential-rel',
                               'uncertainty-volumetricLoading-abs',
                               'uncertainty-volumetricLoading-rel']

COLUMNS_DF_ABS_UNCERTAINITY = ['uncertainty-data-source',
                               'uncertainty-pressure-abs',
                               'uncertainty-pressure-rel',
                               'uncertainty-temperature-abs',
                               'uncertainty-temperature-rel',
                               'uncertainty-loading-abs',
                               'uncertainty-loading-rel']


# Definition of names of keys of dicts
#
# a) Keys that are more general
#
KEYS_DICT_BASE = ['k', 'v']

KEYS_DICT_K = ['_as_', '_rf_', '_st_', '_tp_']
KEYS_DICT_V = ['_ep_', '_ed_', '_r_', '_s_', '_t_']

KEYS_DICT_ABS_EQUATIONS = ['_c_', '_n_', '_e_', '_va_', '_p_']
KEYS_DICT_ADS_EQUATIONS = ['_c_', '_n_', '_e_', '_pr_', '_va_', '_er_', '_p_']
KEYS_DICT_REF_EQUATIONS = ['_c_', '_n_', '_e_', '_va_', '_p_']

KEYS_DICT_ADS_EXPDATA = ['_c_', '_n_', '_pr_', '_m_', '_u_', '_d_']
KEYS_DICT_ABS_EXPDATA = ['_c_', '_n_', '_m_', '_u_', '_d_']

KEYS_DICT_ADS_PROPERTIES = ['diameter-crystal', 'diameter-pore', 'diameter-pellet',
                            'diameter-extrudate', 'length-extrudate', 'area-surface',
                            'volume-pore', 'porosity-pellet', 'density-bulk',
                            'density-pellet', 'density-solid']

# b) Keys that are required for entries of equation coefficients
#
KEYS_DICT_ERROR = ['are', 'rmse']

KEYS_DICT_ABS_VALIDITY = ['temperature-min', 'temperature-max']
KEYS_DICT_ADS_VALIDITY = ['pressure-min', 'pressure-max',
                          'temperature-min', 'temperature-max',
                          'loading-min', 'loading-max']
KEYS_DICT_REF_VALIDITY = ['temperature-min', 'temperature-max']

# c) Keys that are required for entries of experimental data
#
KEYS_DICT_MEASUREMENT = ['procedure', 'type']

KEYS_DICT_ADS_UNCERTAINITY = ['data-source',
                              'pressure-abs', 'pressure-rel', 'temperature-abs',
                              'temperature-rel', 'loading-abs', 'loading-rel',
                              'adsorptionPotential-abs', 'adsorptionPotential-rel',
                              'volumetricLoading-abs', 'volumetricLoading-rel']

KEYS_DICT_ABS_UNCERTAINITY = ['data-source',
                              'pressure-abs', 'pressure-rel', 'temperature-abs',
                              'temperature-rel', 'loading-abs', 'loading-rel',
                              'adsorptionPotential-abs', 'adsorptionPotential-rel',
                              'volumetricLoading-abs', 'volumetricLoading-rel']


# %% Definition of sub-functions.
#
def read_csv_to_list(path: str) -> Union[list, list]:
    r"""Input a path to folder with *.csv-files to read files into panda data frames.

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
        Contains all names of files without their type extensions (i.e. *.csv).
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
    r"""Search data frame for values in some columns.

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
        df_search = df_search.loc[df_search[columns[no_criterion]] == value_criterion]

    # Return search result
    #
    return df_search


def search_wpair(dfs: list, columns: list) -> DataFrame:
    r"""Search for all available working pais.

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
    r"""Create a dict for a JSON file.

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


# %% Definition of main-function.
#
def create_json_file(path_coef: str, path_exp: list, path_json: str) -> Union[list, str, str]:
    r"""Create JSON file.

    This function creates the JSON file. Therefore, it needs the path to the
    coefficients of the equations (i.e., 'path_coef'), the paths to the
    experimental data (i.e., 'path_exp'), and the path to the location where
    the JSON files shall be saved (i.e., 'path_json').

    Parameters
    ----------
    path_coef : str
        Path to *.csv-files that contain coefficients of equations.
    path_exp : list
        List containing paths to *.csv-files that contain experimental data points.
    path_exp : str
        Path to to folder where json-files shall be safed.

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
    # 0) Load all *.csv-files that contain equation coefficients or experimental
    #    data of refrigerants, adsorption isotherms, and absorption isotherms
    #
    names_equations, list_df_equations = read_csv_to_list(path_coef)

    tmp_list_df_experimental_data = []
    list_df_experimental_data = [None] * len(path_exp)

    for ind_exp, path_exp_i in enumerate(path_exp):
        _, list_df_experimental_data[ind_exp] = read_csv_to_list(path_exp_i)
        tmp_list_df_experimental_data = list_df_experimental_data[ind_exp] if ind_exp == 0 else \
            tmp_list_df_experimental_data + list_df_experimental_data[ind_exp]

    # 1) Get names of all unique refrigerants and working pairs:
    #
    df_wpair_unique = search_wpair(list_df_equations + tmp_list_df_experimental_data,
                                   COLUMNS_DF_WPAIR)

    # 2) Create dict 'k' that is used to save general information of all
    #    refrigerants and working pairs:
    #
    #       - Iterate over all unique refrigerants and working pairs
    #       - Save general information as dict
    #
    dict_k = [dict(zip(KEYS_DICT_K, row)) for _, row in df_wpair_unique.iterrows()]

    # 3) Create dict 'v' to save coefficients of equations and experimental
    #    data:
    #
    #       - Iterate over all unique refrigerants and working pairs
    #       - Create dicts and save dummy values for keys that will be later
    #         used for coefficients of equations and for experimental data
    #
    dict_v = [dict(zip(KEYS_DICT_V, [[], [],
                                     row[COLUMNS_DF_WPAIR[1]],
                                     row[COLUMNS_DF_WPAIR[0]],
                                     row[COLUMNS_DF_WPAIR[2]]]))
              for _, row in df_wpair_unique.iterrows()]

    # 4) Create dict '_ep_' to save coefficients of equations:
    #
    #       - Create dict that contains names of all equations as keys and empty
    #         lists as valeus
    #       - Create list for all unique refrigerants and working pairs with former
    #         created dict as attribute
    #       - For each refrigerant and working pair, count how often an equation
    #         occurs by iterating over refrigerants and working pairs first and
    #         iterating over equations second
    #       - For each equation, iterate over available sets of coefficients and
    #         save coresponding coefficients as dict
    #
    list_equations = []
    for _ in range(len(df_wpair_unique)):
        # Create empty dicts for each unique refrigerant and working pair, in which
        # the keys corespond the names of all existing equations
        #
        dict_equations = dict()
        for _, val in enumerate(names_equations):
            dict_equations.update(zip([val], [[]]))
        list_equations.append(dict_equations)

    for ind_ref_wpair, ref_wpair in df_wpair_unique.iterrows():
        for ind_equ, name_equ in enumerate(dict_equations):
            # Search current working pair within current equation data frame
            #
            df_equ = list_df_equations[ind_equ]
            cur_ref_wpair = ref_wpair.tolist()
            df_result = search_df(df_equ, COLUMNS_DF_WPAIR, cur_ref_wpair)

            # Save current equation data frame into coresponding position of
            # 'list_equations' when working pair exists
            #
            df_result = df_result.reset_index(drop=True)
            df_result = [] if df_result.empty else df_result
            list_equations[ind_ref_wpair].update(dict(zip([name_equ],
                                                          [df_result])))

    for ind_ref_wpair, _ in enumerate(dict_v):
        # Count and save names of different equation approaches for
        # current working pair
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
                # Create dummy dict for each set of coefficients depending on
                # kind of data (e.g., refrigerant or adsorption isotherm)
                #
                if dict_k[ind_ref_wpair]['_tp_']=='refrig':
                    list_equ_set = [[] for i in range(len(KEYS_DICT_REF_EQUATIONS))]
                    dict_equ_set = dict(zip(KEYS_DICT_REF_EQUATIONS, list_equ_set))

                elif dict_k[ind_ref_wpair]['_tp_']=='ads':
                    list_equ_set = [[] for i in range(len(KEYS_DICT_ADS_EQUATIONS))]
                    dict_equ_set = dict(zip(KEYS_DICT_ADS_EQUATIONS, list_equ_set))

                else:
                    list_equ_set = [[] for i in range(len(KEYS_DICT_ABS_EQUATIONS))]
                    dict_equ_set = dict(zip(KEYS_DICT_ABS_EQUATIONS, list_equ_set))

                # Update dummy dict depending on kind of data (e.g., refrigerant or
                # adsorption isotherm)
                #
                if dict_k[ind_ref_wpair]['_tp_']=='refrig':
                    # Update dummy dict: Literature
                    #
                    dict_equ_set[KEYS_DICT_REF_EQUATIONS[0]] = \
                        equ_set[COLUMNS_DF_LITERATURE[1]]

                    # Update dummy dict: Note
                    #
                    dict_equ_set[KEYS_DICT_REF_EQUATIONS[1]] = \
                        equ_set[COLUMNS_DF_LITERATURE[0]]

                    # Update dummy dict: Name of equation
                    #
                    dict_equ_set[KEYS_DICT_REF_EQUATIONS[2]] = \
                        name_equ.capitalize()

                    # Update dummy dict: Valid range
                    #
                    dict_equ_set[KEYS_DICT_REF_EQUATIONS[3]] = \
                        create_dict(equ_set[COLUMNS_DF_REF_VALIDITY],
                                    KEYS_DICT_REF_VALIDITY, True)


                    # Update dummy dict: Coefficients
                    #
                    equ_set = equ_set.drop(COLUMNS_DF_WPAIR +
                                           COLUMNS_DF_LITERATURE +
                                           COLUMNS_DF_REF_VALIDITY)

                    name_par = equ_set.index
                    list_par_name = []
                    list_par_value = []
                    for _, val in enumerate(name_par):
                        list_par_name.append(val)
                        list_par_value.append(float(equ_set[val]))

                    dict_equ_set[KEYS_DICT_REF_EQUATIONS[4]] = dict(zip(list_par_name,
                                                                    list_par_value))

                elif dict_k[ind_ref_wpair]['_tp_']=='ads':
                    # Update dummy dict: Literature
                    #
                    dict_equ_set[KEYS_DICT_ADS_EQUATIONS[0]] = \
                        equ_set[COLUMNS_DF_LITERATURE[1]]

                    # Update dummy dict: Note
                    #
                    dict_equ_set[KEYS_DICT_ADS_EQUATIONS[1]] = \
                        equ_set[COLUMNS_DF_LITERATURE[0]]

                    # Update dummy dict: Name of equation
                    #
                    dict_equ_set[KEYS_DICT_ADS_EQUATIONS[2]] = \
                        name_equ.capitalize()

                    # Update dummy dict: Further properties of sorbent
                    #
                    dict_equ_set[KEYS_DICT_ADS_EQUATIONS[3]] = \
                        create_dict(equ_set[COLUMNS_DF_ADS_PROPERTIES],
                                    KEYS_DICT_ADS_PROPERTIES)

                    # Update dummy dict: Valid range
                    #
                    dict_equ_set[KEYS_DICT_ADS_EQUATIONS[4]] = \
                        create_dict(equ_set[COLUMNS_DF_ADS_VALIDITY],
                                    KEYS_DICT_ADS_VALIDITY, True)

                    # Update dummy dict: Error
                    #
                    dict_equ_set[KEYS_DICT_ADS_EQUATIONS[5]] = \
                        create_dict(equ_set[COLUMNS_DF_ERROR],
                                    KEYS_DICT_ERROR, True)

                    # Update dummy dict: Coefficients
                    #
                    equ_set = equ_set.drop(COLUMNS_DF_WPAIR +
                                           COLUMNS_DF_LITERATURE +
                                           COLUMNS_DF_ADS_PROPERTIES +
                                           COLUMNS_DF_ADS_VALIDITY +
                                           COLUMNS_DF_ERROR)

                    name_par = equ_set.index
                    list_par_name = []
                    list_par_value = []
                    for _, val in enumerate(name_par):
                        list_par_name.append(val)
                        list_par_value.append(float(equ_set[val]))

                    dict_equ_set[KEYS_DICT_ADS_EQUATIONS[6]] = dict(zip(list_par_name,
                                                                    list_par_value))

                else:
                    # Update dummy dict: Literature
                    #
                    dict_equ_set[KEYS_DICT_ABS_EQUATIONS[0]] = \
                        equ_set[COLUMNS_DF_LITERATURE[1]]

                    # Update dummy dict: Note
                    #
                    dict_equ_set[KEYS_DICT_ABS_EQUATIONS[1]] = \
                        equ_set[COLUMNS_DF_LITERATURE[0]]

                    # Update dummy dict: Name of equation
                    #
                    dict_equ_set[KEYS_DICT_ABS_EQUATIONS[2]] = name_equ.capitalize()

                    # Update dummy dict: Valid range
                    #
                    dict_equ_set[KEYS_DICT_ABS_EQUATIONS[3]] = \
                        create_dict(equ_set[COLUMNS_DF_ABS_VALIDITY],
                                    KEYS_DICT_ABS_VALIDITY, True)

                    # Update dummy dict: Coefficients
                    #
                    equ_set = equ_set.drop(COLUMNS_DF_WPAIR +
                                           COLUMNS_DF_LITERATURE +
                                           COLUMNS_DF_ABS_VALIDITY)

                    name_par = equ_set.index
                    list_par_name = []
                    list_par_value = []
                    for _, val in enumerate(name_par):
                        list_par_name.append(val)
                        list_par_value.append(float(equ_set[val]))

                    dict_equ_set[KEYS_DICT_ABS_EQUATIONS[4]] = dict(zip(list_par_name,
                                                                    list_par_value))

                # Update list that contains all sets of coefficients
                #
                list_equ.append(dict_equ_set)

            # Update dict that contains different equations
            #
            dict_equ[name_equ] = list_equ

        # Update dict 'v' by adding coefficients of equations
        #
        dict_v[ind_ref_wpair][KEYS_DICT_V[0]] = dict_equ

    # 5) Create dict '_ed_' to save experimental data. Thereby, check type of data:
    #
    #       - Iterate over all refrigerants and working pairs
    #       - Check if experimental data exist
    #       - Iterate over all refrigerants and working pairs and save
    #         experimental data as dict
    #
    # Iterate over all experimental data specified by list of paths
    #
    for ind_type_exp, list_df_experimental_data_i in enumerate(list_df_experimental_data):
        df_exp_data = search_wpair(list_df_experimental_data_i, COLUMNS_DF_WPAIR)

        # Iterate over all refrigerants and working pairs
        #
        for ind_ref_wpair, _ in enumerate(dict_v):
            list_exp_data = []

            # Check if experimental data exists for current working pair
            # Thus, save time and skip iterations that are not necessary
            #
            df_cur_ref_wpair = df_wpair_unique.loc[ind_ref_wpair, :]
            name_cur_ref_wpair = df_cur_ref_wpair.tolist()
            df_result = search_df(df_exp_data, COLUMNS_DF_WPAIR,
                                  name_cur_ref_wpair)

            # If experimental data exist, save it into dict '_ed_'
            #
            if not df_result.empty:
                # Iterate over all data frames that contain experimental data
                #
                for _, df_exp in enumerate(list_df_experimental_data_i):
                    # Search experimental data within data frame for current working
                    # pair
                    # Note that experimental data only exists within one data frame!
                    #
                    df_exp = search_df(df_exp, COLUMNS_DF_WPAIR,
                                        name_cur_ref_wpair)

                    if not df_exp.empty:
                        # Check type of experimental data
                        #
                        if df_result['type'].all() == 'ads':
                            # Iterate over all available experimental data for the
                            # current working pair
                            #
                            for _, exp_set in df_exp.iterrows():
                                # Create dummy dict to save current experimental data
                                #
                                list_cur_exp_data = [[] for i in range(len(KEYS_DICT_ADS_EXPDATA))]
                                dict_exp_data = dict(zip(KEYS_DICT_ADS_EXPDATA,
                                                          list_cur_exp_data))

                                # Update dummy dict: Literature
                                #
                                dict_exp_data[KEYS_DICT_ADS_EXPDATA[0]] = \
                                    exp_set[COLUMNS_DF_LITERATURE[1]]

                                # Update dummy dict: Note
                                #
                                dict_exp_data[KEYS_DICT_ADS_EXPDATA[1]] = \
                                    exp_set[COLUMNS_DF_LITERATURE[0]]

                                # Update dummy dict: Sorbent properties
                                #
                                dict_exp_data[KEYS_DICT_ADS_EXPDATA[2]] = \
                                    create_dict(exp_set[COLUMNS_DF_ADS_PROPERTIES],
                                                KEYS_DICT_ADS_PROPERTIES)

                                # Update dummy dict: Measurement information
                                #
                                dict_exp_data[KEYS_DICT_ADS_EXPDATA[3]] = \
                                    create_dict(exp_set[COLUMNS_DF_MEASUREMENT],
                                                KEYS_DICT_MEASUREMENT)

                                # Update dummy dict: Uncertainty information
                                #
                                dict_exp_data[KEYS_DICT_ADS_EXPDATA[4]] = \
                                    create_dict(exp_set[COLUMNS_DF_ADS_UNCERTAINITY],
                                                KEYS_DICT_ADS_UNCERTAINITY)

                                # Update dummy dict: Measurements
                                #
                                mea_set = exp_set.drop(COLUMNS_DF_WPAIR +
                                                        COLUMNS_DF_LITERATURE +
                                                        COLUMNS_DF_ADS_PROPERTIES +
                                                        COLUMNS_DF_MEASUREMENT +
                                                        COLUMNS_DF_ADS_UNCERTAINITY)

                                name_par = mea_set.index
                                list_par_value = []
                                for _, val in enumerate(name_par):
                                    # Split string and ensure empty lists
                                    #
                                    measurments = mea_set[val].split(',')
                                    measurments = [] if (measurments[0] == '') else \
                                        [float(item) for item in measurments]

                                    list_par_value.append(measurments)

                                dict_exp_data[KEYS_DICT_ADS_EXPDATA[5]] = \
                                    dict(zip(name_par.to_list(), list_par_value))

                                # Update list containing all experiment data of current
                                # working pair
                                #
                                list_exp_data.append(dict_exp_data)

                        elif df_result['type'].all() == 'abs':
                            # Iterate over all available experimental data for the
                            # current working pair
                            #
                            for _, exp_set in df_exp.iterrows():
                                # Create dummy dict to save current experimental data
                                #
                                list_cur_exp_data = [[] for i in range(len(KEYS_DICT_ABS_EXPDATA))]
                                dict_exp_data = dict(zip(KEYS_DICT_ABS_EXPDATA,
                                                          list_cur_exp_data))

                                # Update dummy dict: Literature
                                #
                                dict_exp_data[KEYS_DICT_ABS_EXPDATA[0]] = \
                                    exp_set[COLUMNS_DF_LITERATURE[1]]

                                # Update dummy dict: Note
                                #
                                dict_exp_data[KEYS_DICT_ABS_EXPDATA[1]] = \
                                    exp_set[COLUMNS_DF_LITERATURE[0]]

                                # Update dummy dict: Measurement information
                                #
                                dict_exp_data[KEYS_DICT_ABS_EXPDATA[2]] = \
                                    create_dict(exp_set[COLUMNS_DF_MEASUREMENT],
                                                KEYS_DICT_MEASUREMENT)

                                # Update dummy dict: Uncertainty information
                                #
                                dict_exp_data[KEYS_DICT_ABS_EXPDATA[3]] = \
                                    create_dict(exp_set[COLUMNS_DF_ABS_UNCERTAINITY],
                                                KEYS_DICT_ABS_UNCERTAINITY)

                                # Update dummy dict: Measurements
                                #
                                mea_set = exp_set.drop(COLUMNS_DF_WPAIR +
                                                        COLUMNS_DF_LITERATURE +
                                                        COLUMNS_DF_MEASUREMENT +
                                                        COLUMNS_DF_ABS_UNCERTAINITY)

                                name_par = mea_set.index
                                list_par_value = []
                                for _, val in enumerate(name_par):
                                    # Split string and ensure empty lists
                                    #
                                    measurments = mea_set[val].split(',')
                                    measurments = [] if (measurments[0] == '') else \
                                        [float(item) for item in measurments]

                                    list_par_value.append(measurments)

                                dict_exp_data[KEYS_DICT_ABS_EXPDATA[4]] = \
                                    dict(zip(name_par.to_list(), list_par_value))

                                # Update list containing all experiment data of current
                                # working pair
                                #
                                list_exp_data.append(dict_exp_data)

                        # Update dict 'v' by adding experimental data
                        #
                        dict_v[ind_ref_wpair][KEYS_DICT_V[1]] = list_exp_data

    # 6) Create base dict that containts dicts 'k' and 'v':
    #
    #       - Iterate over all unique refrigerants and working pairs
    #       - Create dicts by zip-function
    #
    list_json = [dict(zip(KEYS_DICT_BASE, [val, dict_v[ind]])) for ind, val in
                 enumerate(dict_k)]

    # 7) Save JSON-files
    #
    with open(join(path_json, "sorpproplib.json"), "w") as text_file:
        json_compact = dumps(list_json)
        text_file.write(json_compact)

    with open(join(path_json, "sorpproplib_formatted.json"), "w") as text_file:
        json_formatted = dumps(list_json, indent=4)
        text_file.write(json_formatted)

    # Return data
    #
    return list_json, json_compact, json_formatted


# %% Execute main-function.
#
if __name__ == "__main__":
    # Defines paths
    #
    PATH_COEFFICIENTS = 'JSON\\equation_coefficients\\'
    PATH_EXPERIMENTS = ['JSON\\experimental_data\\ads\\']
    PATH_JSON = 'JSON\\'

    # Create JSON files
    #
    JSON_FILE, JSON_STR_COMPLETE, JSON_STR_FORMATTED = \
        create_json_file(PATH_COEFFICIENTS, PATH_EXPERIMENTS, PATH_JSON)
