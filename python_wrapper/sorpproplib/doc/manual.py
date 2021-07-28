"""Package 'manual' cotains functions to generate manual of the SorpPropLib.

Routine Listings
----------------
get_typedependent_content_lists:
    Generate dict containing content of SorpPropLib divided by type (e.g., refrigerant).
write_ref_single:
    Generate formatted LaTeX string of one refrigerant function.
write_ref_all:
    Generate and save formatted LaTeX strings of all refrigerant functions implemented in data base.
write_ads_single:
    Generate formatted LaTeX string of one adsorption isotherm function.
write_ads_all:
    Generate and save formatted LaTeX strings of all adsorption isotherm functions implemented in data base.

Revisions
---------
    Version     Date        Author              Changes
    1.0.0       2020-07-17  Mirko Engelpracht   First implementation
"""


# %% Import relevant modules.
#
import os
import pandas as pd

from .equation import write_ref_equation, write_ads_equation, write_abs_equation
from json import load
from .overview import write_ref_overview, write_ads_overview, write_abs_overview
from .properties import write_ads_property
from .utilities import str_file
from .validity import write_ref_validity, write_ads_validity, write_abs_validity
from .visualization import write_ref_viszualization, write_ads_viszualization, write_abs_viszualization


# %% Define functions.
#
def get_typedependent_content_lists(PATH_DB: str) -> dict:
    r"""Generate dict containing content of SorpPropLib divided by type (e.g., refrigerant).

    Parameters
    ----------
    PATH_DB : str
        Path to JSON data base of SorpPropLib.

    Returns
    -------
    dict
        Dict containing content of SorpPropLib divided by type (e.g., refrigerant).

    """
    # Load data base
    #
    with open(PATH_DB, "r") as read_file:
        JSON_FILE = load(read_file)

    # Get lists that contain content of data base separated by refrigerant, adsorption, or
    # absorption
    #
    list_refrigerants = []
    list_ads_working_pairs = []
    list_abs_working_pairs = []

    for _, entry in enumerate(JSON_FILE):
        if entry['k']['_tp_'] == 'refrig':
            list_refrigerants.append(entry)

        elif entry['k']['_tp_'] == 'ads':
            list_ads_working_pairs.append(entry)

        elif entry['k']['_tp_'] == 'abs':
            list_abs_working_pairs.append(entry)

    # Return contents
    #
    return {'ref':  list_refrigerants,
            'ads':  list_ads_working_pairs,
            'abs':  list_abs_working_pairs,
            'wpair':list_ads_working_pairs+list_abs_working_pairs}

def write_ref_single(REF: dict, EQU_NAME: str, EQU_ID: int,
                     WPAIRS: list, PATH_WRAPPER: str,
                     PATH_SORPPROPLIB: str, PATH_DB: str) -> str:
    r"""Generate formatted LaTeX string of one refrigerant function.

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
        Formatted LaTeX string of one refrigerant function.

    """
    # Get formatted LaTeX strings of sub-sections
    #
    STR_OVERVIEW = write_ref_overview(REF, EQU_NAME, EQU_ID)
    STR_EQUATION = write_ref_equation(REF, EQU_NAME, EQU_ID)
    STR_VALIDITY = write_ref_validity(REF, EQU_NAME, EQU_ID)
    STR_VISUALIZATION = write_ref_viszualization(REF, EQU_NAME, EQU_ID, WPAIRS,
                                                 PATH_WRAPPER, PATH_SORPPROPLIB, PATH_DB)

    # Get label and subsection
    #
    STR_NAME = EQU_NAME.split('_')[0]
    IND_UPPER = [i for i,e in enumerate(STR_NAME + 'A') if e.isupper()]
    LIST_NAME_PARTS = [STR_NAME[IND_UPPER[j]:IND_UPPER[j+1]] for j in range(len(IND_UPPER)-1)]

    str_subsection = ''
    for val in LIST_NAME_PARTS:
        str_subsection += val + ' '
    str_subsection += '- ' + EQU_NAME.split('_')[1] + ' - ID ' + str(EQU_ID+1)

    # Write formatted LaTeX string
    #
    STR_LATEX = '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%' + '\n'  + \
                '\\subsection{' + str_subsection + '}'                                            + '\n'  + \
                '%'                                                                               + '\n'  + \
                STR_OVERVIEW + STR_EQUATION + STR_VALIDITY + STR_VISUALIZATION                            + \
                '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%' + '\n'

    # Return formatted LaTeX string
    #
    return STR_LATEX

def write_ref_all(CONTENT: dict, PATH_WRAPPER: str, PATH_SORPPROPLIB: str, PATH_DB: str):
    r"""Generate and save formatted LaTeX strings of all refrigerant functions implemented in data base.

    Parameters
    ----------
    CONTENT : dict
        Dict containing content of SorpPropLib divided by type (e.g., refrigerant).
    PATH_WRAPPER : str
        Path to Python wrapper.
    PATH_SORPPROPLIB : str
        Path to DLL of SorpPropLib.
    PATH_DB : str
        Path to JSON data base of SorpPropLib.

    """
    # Sort refrigerents by names, equation approaches, and equation IDs using pandas data frame
    #
    df_ref = pd.DataFrame(columns=['refrigerant',
                                   'name of equation',
                                   'ID in approaches list',
                                   'ID in original list'])

    for ref_id, dict_ref in enumerate(CONTENT['ref']):
        # Get all refrigerant equations implemented in the data base for current refrigerant
        # Remove entry that contains some general data
        #
        dict_approaches = dict_ref['v']['_ep_']

        list_approaches_names = list(dict_approaches.keys())
        if 'Refrigerants' in list_approaches_names:
            list_approaches_names.remove('Refrigerants')

        # First, iterate over appraoches
        # Second, iterate over all entries per approach and add to data frame
        #
        for approach_name in list_approaches_names:
            for approach_name_id, approach_name_i in enumerate(dict_approaches[approach_name]):
                tmp_dict = {'refrigerant':          dict_ref['k']['_rf_'],
                            'name of equation':     approach_name,
                            'ID in approaches list':approach_name_id,
                            'ID in original list':  ref_id}

                df_ref = df_ref.append(tmp_dict, ignore_index = True)

    df_ref = df_ref.sort_values(['refrigerant', 'name of equation', 'ID in approaches list'])

    # Iterate over all refrigerants implemented in data base to generate formatted LaTeX strings
    # for each refrigerant
    #
    entry_id = 0
    entry_ref = ''
    STR_LATEX = ''

    list_paths = []

    for entry in df_ref.iterrows():
        if entry_ref != entry[1]['refrigerant']:
            if entry_id != 0:
                # Save old refrigerant chapter
                #
                str_dir = os.path.join(PATH_WRAPPER, 'sorpproplib', 'doc', 'chapter', 'ref')
                str_name = str_file(entry_ref) + '.tex'
                str_path = os.path.join(str_dir, str_name)

                if not os.path.exists(str_dir):
                    os.mkdir(str_dir)

                with open(str_path, "w", encoding="utf-8") as text_file:
                    text_file.write(STR_LATEX)

                list_paths.append(os.path.join('chapter', 'ref', str_name))

            # Write new chapter for refrigerant
            #
            STR_LATEX = '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%' + '\n'  + \
                        '% Chapter \'Refrigerants - ' + entry[1]['refrigerant'] + '\''                    + '\n'  + \
                        '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%' + '\n'  + \
                        '\section{' + entry[1]['refrigerant'] + '}'                                       + '\n'  + \
                        '%'                                                                               + '\n'  + \
                        '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%' + '\n'

            # Add content of first equation
            #
            STR_LATEX += write_ref_single(CONTENT['ref'][entry[1]['ID in original list']],
                                          entry[1]['name of equation'],
                                          entry[1]['ID in approaches list'],
                                          CONTENT['wpair'],
                                          PATH_WRAPPER,
                                          PATH_SORPPROPLIB,
                                          PATH_DB)

        else:
            # If refrigerant is still the same, add content of further equations
            #
            STR_LATEX += write_ref_single(CONTENT['ref'][entry[1]['ID in original list']],
                                          entry[1]['name of equation'],
                                          entry[1]['ID in approaches list'],
                                          CONTENT['wpair'],
                                          PATH_WRAPPER,
                                          PATH_SORPPROPLIB,
                                          PATH_DB)

        # Update values for comparison
        #
        entry_id += 1
        entry_ref = entry[1]['refrigerant']

    # Save last refrigerant chapter
    #
    str_dir = os.path.join(PATH_WRAPPER, 'sorpproplib', 'doc', 'chapter', 'ref')
    str_name = str_file(entry_ref) + '.tex'
    str_path = os.path.join(str_dir, str_name)

    with open(str_path, "w", encoding="utf-8") as text_file:
        text_file.write(STR_LATEX)

    list_paths.append(os.path.join('chapter', 'ref', str_name))

    # Generate and save refrigerant chapter
    #
    STR_LATEX = '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%' + '\n' + \
                '% Chapter \' Refrigerants \''                                                    + '\n' + \
                '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%' + '\n' + \
                '\\chapter{Refrigerants}'                                                         + '\n' + \
                '\\label{cha:refrigerants}'                                                       + '\n' + \
                '%'                                                                               + '\n'

    for val in list_paths:
        STR_LATEX += '\\input{' + val.replace('\\', '/') + '}' + '\n'

    STR_LATEX += '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%' + '\n'

    str_dir = os.path.join(PATH_WRAPPER, 'sorpproplib', 'doc', 'chapter')
    str_path = os.path.join(str_dir, 'refrigerants.tex')

    with open(str_path, "w", encoding="utf-8") as text_file:
        text_file.write(STR_LATEX)

def write_ads_single(ADS: dict, EQU_NAME: str, EQU_ID: int, REFS: list,
                     PATH_WRAPPER: str, PATH_SORPPROPLIB: str, PATH_DB: str) -> str:
    r"""Generate formatted LaTeX string of one adsorption isotherm.

    Parameters
    ----------
    ADS : dict
        Dict containing properties of current adsorption working pair extrated from data base
        (i.e., JSON).
    EQU_NAME : str
        Name of adsorption isotherm functions for which overview table shall be generated.
    EQU_ID : int
        ID of adsorption isotherm functions for which overview table shall be generated.
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
        Formatted LaTeX string of one adsorption isotherm function.

    """
    # Get formatted LaTeX strings of sub-sections
    #
    STR_OVERVIEW = write_ads_overview(ADS, EQU_NAME, EQU_ID)
    STR_PROPERTIES = write_ads_property(ADS, EQU_NAME, EQU_ID)
    STR_EQUATION = write_ads_equation(ADS, EQU_NAME, EQU_ID)
    STR_VALIDITY = write_ads_validity(ADS, EQU_NAME, EQU_ID)
    STR_VISUALIZATION = write_ads_viszualization(ADS, EQU_NAME, EQU_ID, REFS,
                                                 PATH_WRAPPER, PATH_SORPPROPLIB, PATH_DB)

    # Write formatted LaTeX string
    #
    STR_LATEX = '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%' + '\n'  + \
                '\\subsubsection{' + EQU_NAME + ' - ID ' + str(EQU_ID+1) + '}'                    + '\n'  + \
                '%'                                                                               + '\n'  + \
                STR_OVERVIEW + STR_PROPERTIES + STR_EQUATION + STR_VALIDITY + STR_VISUALIZATION           + \
                '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%' + '\n'

    # Return formatted LaTeX string
    #
    return STR_LATEX

def write_ads_all(CONTENT: dict, PATH_WRAPPER: str, PATH_SORPPROPLIB: str, PATH_DB: str):
    r"""Generate and save formatted LaTeX strings of all adsorption isotherm functions implemented in data base.

    Parameters
    ----------
    CONTENT : dict
        Dict containing content of SorpPropLib divided by type (e.g., refrigerant).
    PATH_WRAPPER : str
        Path to Python wrapper.
    PATH_SORPPROPLIB : str
        Path to DLL of SorpPropLib.
    PATH_DB : str
        Path to JSON data base of SorpPropLib.

    """
    # Sort adsorption working pairs by refrigerant, sorbent and substype, equation approaches,
    # and equation IDs using pandas data frame
    #
    df_ads = pd.DataFrame(columns=['refrigerant',
                                   'sorbent',
                                   'subtype',
                                   'name of equation',
                                   'ID in approaches list',
                                   'ID in original list'])

    for ads_id, dict_ads in enumerate(CONTENT['ads']):
        # Get all adsorption isotherm equations implemented in the data base for current adsorption
        # working pair
        #
        dict_approaches = dict_ads['v']['_ep_']
        list_approaches_names = list(dict_approaches.keys())

        # First, iterate over appraoches
        # Second, iterate over all entries per approach and add to data frame
        #
        for approach_name in list_approaches_names:
            for approach_name_id, approach_name_i in enumerate(dict_approaches[approach_name]):
                tmp_dict = {'refrigerant':          dict_ads['k']['_rf_'],
                            'sorbent':              dict_ads['k']['_as_'],
                            'subtype':              dict_ads['k']['_st_'],
                            'name of equation':     approach_name,
                            'ID in approaches list':approach_name_id,
                            'ID in original list':  ads_id}

                df_ads = df_ads.append(tmp_dict, ignore_index = True)

    df_ads = df_ads.sort_values(['refrigerant', 'sorbent', 'subtype',
                                 'name of equation', 'ID in approaches list'])

    # Iterate over all adsorption working pairs implemented in data base to generate formatted
    # LaTeX strings for each indivudal combination of refrigerant, sorbent, and subtype
    #
    entry_id = 0
    entry_ref = ''
    entry_sor = ''
    entry_typ = ''
    STR_LATEX = ''

    list_paths_ref = []
    list_paths_sor_typ = []

    for entry in df_ads.iterrows():
        # Check if refrigerant have changed and, thus, new section is needed
        #
        if entry_ref != entry[1]['refrigerant'] and entry_id != 0:
            # Save old sorbent and subtype sub-section
            #
            str_dir = os.path.join(PATH_WRAPPER, 'sorpproplib', 'doc', 'chapter', 'ads')
            str_name = str_file(entry_ref) + '_' + str_file(entry_sor) + \
                '_' + str_file(entry_typ) + '.tex'
            str_path = os.path.join(str_dir, str_name)

            with open(str_path, "w", encoding="utf-8") as text_file:
                text_file.write(STR_LATEX)

            list_paths_sor_typ.append(os.path.join('chapter', 'ads', str_name))

            # Save old refrigerant section
            #
            STR_LATEX = '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%' + '\n' + \
                        '% Chapter \'Adsorption - ' + entry_ref + '\''                                    + '\n' + \
                        '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%' + '\n' + \
                        '\\section{' + entry_ref + '}'                                                    + '\n' + \
                        '%'                                                                               + '\n'

            for val in list_paths_sor_typ:
                STR_LATEX += '\\input{' + val.replace('\\', '/') + '}' + '\n'

            STR_LATEX += '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%' + '\n'

            str_dir = os.path.join(PATH_WRAPPER, 'sorpproplib', 'doc', 'chapter', 'ads')
            str_name = str_file(entry_ref) + '.tex'
            str_path = os.path.join(str_dir, str_name)

            with open(str_path, "w", encoding="utf-8") as text_file:
                text_file.write(STR_LATEX)

            list_paths_ref.append(os.path.join('chapter', 'ads', str_name))
            list_paths_sor_typ = []

            # Write new sorbent and subtype sub-section
            #
            str_subsection = entry[1]['refrigerant'] + ' - ' + entry[1]['sorbent'] + ' ' + \
                entry[1]['subtype']

            STR_LATEX = '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%'  + '\n'  + \
                        '% Chapter \'Adsorption - ' + str_subsection + '\''                                + '\n'  + \
                        '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%'  + '\n'  + \
                        '\subsection{' + entry[1]['sorbent'].capitalize() + ' ' + entry[1]['subtype'].replace('%', '\%') + '}'+ '\n'  + \
                        '%'                                                                                + '\n'  + \
                        '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%'  + '\n'

            # Add content of first adsorption isotherm equation of first sorbent and subtype
            #
            STR_LATEX += write_ads_single(CONTENT['ads'][entry[1]['ID in original list']],
                                          entry[1]['name of equation'],
                                          entry[1]['ID in approaches list'],
                                          CONTENT['ref'],
                                          PATH_WRAPPER,
                                          PATH_SORPPROPLIB,
                                          PATH_DB)

        else:
            # Check if sorbent and subtype have changed and, thus, new sub-section is needed
            #
            if entry_sor != entry[1]['sorbent'] or entry_typ != entry[1]['subtype']:
                if entry_id != 0:
                    # Save old sorbent and subtype sub-section
                    #
                    str_dir = os.path.join(PATH_WRAPPER, 'sorpproplib', 'doc', 'chapter', 'ads')
                    str_name = str_file(entry_ref) + '_' + str_file(entry_sor) + \
                        '_' + str_file(entry_typ) + '.tex'
                    str_path = os.path.join(str_dir, str_name)

                    if not os.path.exists(str_dir):
                        os.mkdir(str_dir)

                    with open(str_path, "w", encoding="utf-8") as text_file:
                        text_file.write(STR_LATEX)

                    list_paths_sor_typ.append(os.path.join('chapter', 'ads', str_name))

                # Write new sorbent and subtype sub-section
                #
                str_subsection = entry[1]['refrigerant'] + ' - ' + entry[1]['sorbent'] + ' ' + \
                    entry[1]['subtype']

                STR_LATEX = '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%'  + '\n'  + \
                            '% Chapter \'Adsorption - ' + str_subsection + '\''                                + '\n'  + \
                            '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%'  + '\n'  + \
                            '\subsection{' + entry[1]['sorbent'].capitalize() + ' ' + entry[1]['subtype'].replace('%', '\%') + '}'+ '\n'  + \
                            '%'                                                                                + '\n'  + \
                            '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%'  + '\n'

                # Add content of first adsorption isotherm equation
                #
                STR_LATEX += write_ads_single(CONTENT['ads'][entry[1]['ID in original list']],
                                              entry[1]['name of equation'],
                                              entry[1]['ID in approaches list'],
                                              CONTENT['ref'],
                                              PATH_WRAPPER,
                                              PATH_SORPPROPLIB,
                                              PATH_DB)

            else:
                # Add content of furhter adsorption isotherm equation
                #
                STR_LATEX += write_ads_single(CONTENT['ads'][entry[1]['ID in original list']],
                                              entry[1]['name of equation'],
                                              entry[1]['ID in approaches list'],
                                              CONTENT['ref'],
                                              PATH_WRAPPER,
                                              PATH_SORPPROPLIB,
                                              PATH_DB)

        # Update values for comparison
        #
        entry_id += 1
        entry_ref = entry[1]['refrigerant']
        entry_sor = entry[1]['sorbent']
        entry_typ = entry[1]['subtype']


    # Save last sorbent and subtype sub-section
    #
    str_dir = os.path.join(PATH_WRAPPER, 'sorpproplib', 'doc', 'chapter', 'ads')
    str_name = str_file(entry_ref) + '_' + str_file(entry_sor) + \
        '_' + str_file(entry_typ) + '.tex'
    str_path = os.path.join(str_dir, str_name)

    with open(str_path, "w", encoding="utf-8") as text_file:
        text_file.write(STR_LATEX)

    list_paths_sor_typ.append(os.path.join('chapter', 'ads', str_name))

    # Save last refrigerant section
    #
    STR_LATEX = '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%' + '\n' + \
                '% Chapter \'Adsorption - ' + entry_ref + '\''                                    + '\n' + \
                '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%' + '\n' + \
                '\\section{' + entry_ref + '}'                                                    + '\n' + \
                '%'                                                                               + '\n'

    for val in list_paths_sor_typ:
        STR_LATEX += '\\input{' + val.replace('\\', '/') + '}' + '\n'

    STR_LATEX += '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%' + '\n'

    str_dir = os.path.join(PATH_WRAPPER, 'sorpproplib', 'doc', 'chapter', 'ads')
    str_name = str_file(entry_ref) + '.tex'
    str_path = os.path.join(str_dir, str_name)

    with open(str_path, "w", encoding="utf-8") as text_file:
        text_file.write(STR_LATEX)

    list_paths_ref.append(os.path.join('chapter', 'ads', str_name))

    # Generate and save adsorption chapter
    #
    STR_LATEX = '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%' + '\n' + \
                '% Chapter \' Adsorption \''                                                      + '\n' + \
                '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%' + '\n' + \
                '\\chapter{Adsorption}'                                                           + '\n' + \
                '\\label{cha:adsorption}'                                                         + '\n' + \
                '%'                                                                               + '\n'

    for val in list_paths_ref:
        STR_LATEX += '\\input{' + val.replace('\\', '/') + '}' + '\n'

    STR_LATEX += '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%' + '\n'

    str_dir = os.path.join(PATH_WRAPPER, 'sorpproplib', 'doc', 'chapter')
    str_path = os.path.join(str_dir, 'adsorption.tex')

    with open(str_path, "w", encoding="utf-8") as text_file:
        text_file.write(STR_LATEX)

def write_abs_single(ADS: dict, EQU_NAME: str, EQU_ID: int, REFS: list,
                     PATH_WRAPPER: str, PATH_SORPPROPLIB: str, PATH_DB: str) -> str:
    r"""Generate formatted LaTeX string of one absorption isotherm.

    Parameters
    ----------
    ADS : dict
        Dict containing properties of current absorption working pair extrated from data base
        (i.e., JSON).
    EQU_NAME : str
        Name of absorption isotherm functions for which overview table shall be generated.
    EQU_ID : int
        ID of absorption isotherm functions for which overview table shall be generated.
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
        Formatted LaTeX string of one absorption isotherm function.

    """
    # Get formatted LaTeX strings of sub-sections
    #
    STR_OVERVIEW = write_abs_overview(ADS, EQU_NAME, EQU_ID)
    STR_EQUATION = write_abs_equation(ADS, EQU_NAME, EQU_ID)
    STR_VALIDITY = write_abs_validity(ADS, EQU_NAME, EQU_ID)
    STR_VISUALIZATION = write_abs_viszualization(ADS, EQU_NAME, EQU_ID, REFS,
                                                 PATH_WRAPPER, PATH_SORPPROPLIB, PATH_DB)

    # Write formatted LaTeX string
    #
    STR_LATEX = '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%' + '\n'  + \
                '\\subsubsection{' + EQU_NAME + ' - ID ' + str(EQU_ID+1) + '}'                    + '\n'  + \
                '%'                                                                               + '\n'  + \
                STR_OVERVIEW  + STR_EQUATION + STR_VALIDITY + STR_VISUALIZATION                           + \
                '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%' + '\n'

    # Return formatted LaTeX string
    #
    return STR_LATEX

def write_abs_all(CONTENT: dict, PATH_WRAPPER: str, PATH_SORPPROPLIB: str, PATH_DB: str):
    r"""Generate and save formatted LaTeX strings of all absorption isotherm functions implemented in data base.

    Parameters
    ----------
    CONTENT : dict
        Dict containing content of SorpPropLib divided by type (e.g., refrigerant).
    PATH_WRAPPER : str
        Path to Python wrapper.
    PATH_SORPPROPLIB : str
        Path to DLL of SorpPropLib.
    PATH_DB : str
        Path to JSON data base of SorpPropLib.

    """
    # Sort absorption working pairs by refrigerant, sorbent and substype, equation approaches,
    # and equation IDs using pandas data frame
    #
    df_abs = pd.DataFrame(columns=['refrigerant',
                                   'sorbent',
                                   'subtype',
                                   'name of equation',
                                   'ID in approaches list',
                                   'ID in original list'])

    for abs_id, dict_abs in enumerate(CONTENT['abs']):
        # Get all absorption isotherm equations implemented in the data base for current absorption
        # working pair
        #
        dict_approaches = dict_abs['v']['_ep_']
        list_approaches_names = list(dict_approaches.keys())

        # First, iterate over appraoches
        # Second, iterate over all entries per approach and add to data frame
        #
        for approach_name in list_approaches_names:
            for approach_name_id, approach_name_i in enumerate(dict_approaches[approach_name]):
                tmp_dict = {'refrigerant':          dict_abs['k']['_rf_'],
                            'sorbent':              dict_abs['k']['_as_'],
                            'subtype':              dict_abs['k']['_st_'],
                            'name of equation':     approach_name,
                            'ID in approaches list':approach_name_id,
                            'ID in original list':  abs_id}

                df_abs = df_abs.append(tmp_dict, ignore_index = True)

    df_abs = df_abs.sort_values(['refrigerant', 'sorbent', 'subtype',
                                 'name of equation', 'ID in approaches list'])

    # Iterate over all absorption working pairs implemented in data base to generate formatted
    # LaTeX strings for each indivudal combination of refrigerant, sorbent, and subtype
    #
    entry_id = 0
    entry_ref = ''
    entry_sor = ''
    entry_typ = ''
    STR_LATEX = ''

    list_paths_ref = []
    list_paths_sor_typ = []

    for entry in df_abs.iterrows():
        # Check if refrigerant have changed and, thus, new section is needed
        #
        if entry_ref != entry[1]['refrigerant'] and entry_id != 0:
            # Save old sorbent and subtype sub-section
            #
            str_dir = os.path.join(PATH_WRAPPER, 'sorpproplib', 'doc', 'chapter', 'abs')
            str_name = str_file(entry_ref) + '_' + str_file(entry_sor) + \
                '_' + str_file(entry_typ) + '.tex'
            str_path = os.path.join(str_dir, str_name)

            with open(str_path, "w", encoding="utf-8") as text_file:
                text_file.write(STR_LATEX)

            list_paths_sor_typ.append(os.path.join('chapter', 'abs', str_name))

            # Save old refrigerant section
            #
            STR_LATEX = '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%' + '\n' + \
                        '% Chapter \'Absorption - ' + entry_ref + '\''                                    + '\n' + \
                        '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%' + '\n' + \
                        '\\section{' + entry_ref + '}'                                                    + '\n' + \
                        '%'                                                                               + '\n'

            for val in list_paths_sor_typ:
                STR_LATEX += '\\input{' + val.replace('\\', '/') + '}' + '\n'

            STR_LATEX += '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%' + '\n'

            str_dir = os.path.join(PATH_WRAPPER, 'sorpproplib', 'doc', 'chapter', 'abs')
            str_name = str_file(entry_ref) + '.tex'
            str_path = os.path.join(str_dir, str_name)

            with open(str_path, "w", encoding="utf-8") as text_file:
                text_file.write(STR_LATEX)

            list_paths_ref.append(os.path.join('chapter', 'abs', str_name))
            list_paths_sor_typ = []

            # Write new sorbent and subtype sub-section
            #
            str_subsection = entry[1]['refrigerant'] + ' - ' + entry[1]['sorbent'] + ' ' + \
                entry[1]['subtype']

            STR_LATEX = '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%'  + '\n'  + \
                        '% Chapter \'Absorption - ' + str_subsection + '\''                                + '\n'  + \
                        '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%'  + '\n'  + \
                        '\subsection{' + entry[1]['sorbent'].capitalize() + ' ' + entry[1]['subtype'].replace('%', '\%') + '}'+ '\n'  + \
                        '%'                                                                                + '\n'  + \
                        '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%'  + '\n'

            # Add content of first absorption isotherm equation of first sorbent and subtype
            #
            STR_LATEX += write_abs_single(CONTENT['abs'][entry[1]['ID in original list']],
                                          entry[1]['name of equation'],
                                          entry[1]['ID in approaches list'],
                                          CONTENT['ref'],
                                          PATH_WRAPPER,
                                          PATH_SORPPROPLIB,
                                          PATH_DB)

        else:
            # Check if sorbent and subtype have changed and, thus, new sub-section is needed
            #
            if entry_sor != entry[1]['sorbent'] or entry_typ != entry[1]['subtype']:
                if entry_id != 0:
                    # Save old sorbent and subtype sub-section
                    #
                    str_dir = os.path.join(PATH_WRAPPER, 'sorpproplib', 'doc', 'chapter', 'abs')
                    str_name = str_file(entry_ref) + '_' + str_file(entry_sor) + \
                        '_' + str_file(entry_typ) + '.tex'
                    str_path = os.path.join(str_dir, str_name)

                    if not os.path.exists(str_dir):
                        os.mkdir(str_dir)

                    with open(str_path, "w", encoding="utf-8") as text_file:
                        text_file.write(STR_LATEX)

                    list_paths_sor_typ.append(os.path.join('chapter', 'abs', str_name))

                # Write new sorbent and subtype sub-section
                #
                str_subsection = entry[1]['refrigerant'] + ' - ' + entry[1]['sorbent'] + ' ' + \
                    entry[1]['subtype']

                STR_LATEX = '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%'  + '\n'  + \
                            '% Chapter \'Absorption - ' + str_subsection + '\''                                + '\n'  + \
                            '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%'  + '\n'  + \
                            '\subsection{' + entry[1]['sorbent'].capitalize() + ' ' + entry[1]['subtype'].replace('%', '\%') + '}'+ '\n'  + \
                            '%'                                                                                + '\n'  + \
                            '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%'  + '\n'

                # Add content of first absorption isotherm equation
                #
                STR_LATEX += write_abs_single(CONTENT['abs'][entry[1]['ID in original list']],
                                              entry[1]['name of equation'],
                                              entry[1]['ID in approaches list'],
                                              CONTENT['ref'],
                                              PATH_WRAPPER,
                                              PATH_SORPPROPLIB,
                                              PATH_DB)

            else:
                # Add content of furhter absorption isotherm equation
                #
                STR_LATEX += write_abs_single(CONTENT['abs'][entry[1]['ID in original list']],
                                              entry[1]['name of equation'],
                                              entry[1]['ID in approaches list'],
                                              CONTENT['ref'],
                                              PATH_WRAPPER,
                                              PATH_SORPPROPLIB,
                                              PATH_DB)

        # Update values for comparison
        #
        entry_id += 1
        entry_ref = entry[1]['refrigerant']
        entry_sor = entry[1]['sorbent']
        entry_typ = entry[1]['subtype']


    # Save last sorbent and subtype sub-section
    #
    str_dir = os.path.join(PATH_WRAPPER, 'sorpproplib', 'doc', 'chapter', 'abs')
    str_name = str_file(entry_ref) + '_' + str_file(entry_sor) + \
        '_' + str_file(entry_typ) + '.tex'
    str_path = os.path.join(str_dir, str_name)

    with open(str_path, "w", encoding="utf-8") as text_file:
        text_file.write(STR_LATEX)

    list_paths_sor_typ.append(os.path.join('chapter', 'abs', str_name))

    # Save last refrigerant section
    #
    STR_LATEX = '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%' + '\n' + \
                '% Chapter \'Absorption - ' + entry_ref + '\''                                    + '\n' + \
                '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%' + '\n' + \
                '\\section{' + entry_ref + '}'                                                    + '\n' + \
                '%'                                                                               + '\n'

    for val in list_paths_sor_typ:
        STR_LATEX += '\\input{' + val.replace('\\', '/') + '}' + '\n'

    STR_LATEX += '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%' + '\n'

    str_dir = os.path.join(PATH_WRAPPER, 'sorpproplib', 'doc', 'chapter', 'abs')
    str_name = str_file(entry_ref) + '.tex'
    str_path = os.path.join(str_dir, str_name)

    with open(str_path, "w", encoding="utf-8") as text_file:
        text_file.write(STR_LATEX)

    list_paths_ref.append(os.path.join('chapter', 'abs', str_name))

    # Generate and save absorption chapter
    #
    STR_LATEX = '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%' + '\n' + \
                '% Chapter \' Absorption \''                                                      + '\n' + \
                '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%' + '\n' + \
                '\\chapter{Absorption}'                                                           + '\n' + \
                '\\label{cha:absorption}'                                                         + '\n' + \
                '%'                                                                               + '\n'

    for val in list_paths_ref:
        STR_LATEX += '\\input{' + val.replace('\\', '/') + '}' + '\n'

    STR_LATEX += '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%' + '\n'

    str_dir = os.path.join(PATH_WRAPPER, 'sorpproplib', 'doc', 'chapter')
    str_path = os.path.join(str_dir, 'absorption.tex')

    with open(str_path, "w", encoding="utf-8") as text_file:
        text_file.write(STR_LATEX)
