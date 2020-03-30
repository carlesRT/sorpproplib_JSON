///////////////////
// workingPair.c //
///////////////////
#include <stdio.h>
#include <stdlib.h>
#include "absorption.h"
#include "adsorption.h"
#include "cJSON.h"
#include "json_interface.h"
#include "refrigerant.h"
#include "workingPair.h"
#include "structDefinitions.c"


///////////////////////////
// Definition of structs //
///////////////////////////
/*
 * WorkingPair:
 * ------------
 *
 * Contains strings defining working pair and calculations approaches,
 * coefficients for calculation approaches, and structures that contain function
 * pointers for the isotherm and the refrigerant functions of the working pair.
 *
 * Attributes:
 * -----------
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 *
 * 	double *iso_par:
 *		Array containing coefficients of isotherm.
 * 	double *psat_par:
 *		Array containing coefficients of vapor pressure equation.
 * 	double *rhol_par:
 *		Array containing coefficients of saturated liquid density equation.
 * 	double *ac_par:
 *		Array containing coefficients of activity coefficient equation.
 *
 *	Adsorption *adsorption:
 *		Struct containing function pointers for isotherm functions.
 *	Refrigerant *refrigerant:
 *		Struct containing function pointers for refrigerant functions.
 *
 * History:
 * --------
 *	01/06/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */


/////////////////////////////
// Definition of functions //
/////////////////////////////
/*
 * newWorkingPair:
 * ---------------
 *
 * Initialization function to create a new WorkingPair-struct. Therefore,
 * function checks first if enough memory available to allocate WorkingPair-
 * struct and then reads coefficients of functions from database and sets
 * pointers within the struct depending on working pair given as input.
 * If memory allocation fails, functions returns NULL and prints a warning.
 *
 * Parameters:
 * -----------
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	struct *WorkingPair:
 *		Returns malloc-ed WorkingPair-struct containing all information required
 *		to calculate the resorption equilibrium of the working working.
 *
 * History:
 * --------
 *	01/06/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API WorkingPair *newWorkingPair(const char *path_db, const char *wp_as,
	const char *wp_st, const char *wp_rf, const char *wp_iso, int no_iso,
	const char *rf_psat, int no_p_sat, const char *rf_rhol, int no_rhol) {
    // Try to allocate memory for vector structure
	//
    WorkingPair *retWorkingPair = (WorkingPair *) malloc(sizeof(WorkingPair));

    if (retWorkingPair == NULL) {
		// Not enough memory available for vector structure
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nCannot allocate memory for WorkingPair-struct.");
        return NULL;
	}

    // Set general information of working pair
	//
	retWorkingPair->wp_as = wp_as;
	retWorkingPair->wp_st = wp_st;
	retWorkingPair->wp_rf = wp_rf;
	retWorkingPair->wp_iso = wp_iso;
	retWorkingPair->rf_psat = rf_psat;
	retWorkingPair->rf_rhol = rf_rhol;
	retWorkingPair->no_iso = no_iso;
	retWorkingPair->no_p_sat = no_p_sat;
	retWorkingPair->no_rhol = no_rhol;

	// Connect to JSON database and parse database's content to JSON-structs
	//
	char *json_content = json_read_file(path_db);
    if (json_content == NULL) {
		// If connection to database fails, free memory
		//
		printf("\n\n###########\n# Error #\n###########");
		printf("\nCannot read JSON database! See warnings printed before!");
		free(retWorkingPair);
        return NULL;
	}

	cJSON *json_file = json_parse_file(json_content);
    if (json_file == NULL) {
		// If content of database cannot be parsed to JSON-struct, free memory
		//
		printf("\n\n###########\n# Error #\n###########");
		printf("\nCannot work with JSON-structs! See warnings printed before!");
		free(json_content);
		free(retWorkingPair);
        return NULL;
	}

	// First, read coefficients for isotherm equation
	//
	int wp_tp;
	cJSON *json_isotherm = json_search_equation(wp_as, wp_st, wp_rf, &wp_tp,
		json_file);

    if (json_isotherm == NULL) {
		// Cannot select isotherm, free memory
		//
		printf("\n\n###########\n# Error #\n###########");
		printf("\nCannot get coefficients of isotherm! See warnings printed "
			"before!");
		free(json_file);
		free(json_content);
		free(retWorkingPair);
        return NULL;
	}

	retWorkingPair->iso_par = json_search_parameters(wp_iso, no_iso,
		json_isotherm);
    if (retWorkingPair->iso_par == NULL) {
		// Cannot select coefficients of isotherm, free memory
		//
		printf("\n\n###########\n# Error #\n###########");
		printf("\nCannot save coefficients of isotherm! See warnings printed "
			"before!");
		free(json_file);
		free(json_content);
		free(retWorkingPair);
        return NULL;
	}

	// Second, read coefficients for refrigerant equations
	// If equations are not implemented, throw warning but do not fail
	//
	int rf_tp;
	cJSON *json_refrigerant = json_search_equation("dum_sorb", "dum_subtype",
		wp_rf, &rf_tp, json_file);

    if (json_refrigerant == NULL) {
		// Cannot select refrigerant, free memory
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nCannot select refrigerant \"%s\" required for calculating "
			"fluid properties.", wp_rf);
		retWorkingPair->psat_par = NULL;
		retWorkingPair->rhol_par = NULL;

	} else {
		// Select coefficients for vapor pressure and saturated liquid density:
		// If function and thus coefficients are not implemented, pointer is
		// set to NULL
		//
		retWorkingPair->psat_par = json_search_parameters(rf_psat, no_p_sat,
			json_refrigerant);
		retWorkingPair->rhol_par = json_search_parameters(rf_rhol, no_rhol,
			json_refrigerant);
	}

	// Free memory that is not needed anymore
	//
	free(json_file);
	free(json_content);

	// Check which equations of states exists for initialization of
	// "Refrigerant"-struct
	//
	const char *rf_psat_int = (retWorkingPair->psat_par == NULL) ?
		"NoVaporPressure" : rf_psat;
	const char *rf_rhol_int= (retWorkingPair->rhol_par == NULL) ?
		"NoSaturatedLiquidDensity" : rf_rhol;

	// Initialize structs containing isotherm functions
	//
	if (wp_tp == 1) {
		// Adsorption equations
		//
		retWorkingPair->adsorption = newAdsorption(wp_iso);
		retWorkingPair->absorption = NULL;
		if (retWorkingPair->adsorption == NULL) {
			// Cannot create Adsorption-struct, free memory
			//
			printf("\n\n###########\n# Error #\n###########");
			printf("\nCannot create Adsorption-struct within "
				"WorkingPair-struct!");
			printf("\nTherefore, cannot execute isotherm functions!");
			free(retWorkingPair);
			return NULL;
		}

	} else if (wp_tp == 2) {
		// Absorption equations
		//
		retWorkingPair->absorption = newAbsorption(wp_iso);
		retWorkingPair->adsorption = NULL;
		if (retWorkingPair->absorption == NULL) {
			// Cannot create Absorption-struct, free memory
			//
			printf("\n\n###########\n# Error #\n###########");
			printf("\nCannot create Absorption-struct within "
				"WorkingPair-struct!");
			printf("\nTherefore, cannot execute isotherm functions!");
			free(retWorkingPair);
			return NULL;
		}

	} else {
		// Neither adsorption nor absorption: Something went wrong!
		//
		printf("\n\n###########\n# Error #\n###########");
		printf("\nDo not know whether to create Adsorption- or "
			"Absorption-struct! This should not happen: Most likely, the "
			"column 'type' is wrong within the JSON database.");
		printf("\nTherefore, cannot execute isotherm functions!");
		free(retWorkingPair);
		return NULL;

	}

	retWorkingPair->refrigerant = newRefrigerant(rf_psat_int, rf_rhol_int);
    if (retWorkingPair->refrigerant == NULL) {
		// Cannot create "Refrigerant"-struct, free memory
		//
		printf("\n\n###########\n# Error #\n###########");
		printf("\nCannot create Refrigerant-struct within WorkingPair-struct!");
		printf("\nTherefore, cannot execute refrigerant functions!");
		free(retWorkingPair);
        return NULL;
	}

	// Avoid compiler warning C4100 by adding "do nothing" expression
	// "Do nothing" expression will be optimized out by compiler
	//
	(rf_tp) = (rf_tp);

	// Return structure
	//
	return retWorkingPair;
}


/*
 * delWorkingPair:
 * ---------------
 *
 * Destructor function to free allocated memory of a WorkingPair-struct.
 *
 * Parameters:
 * -----------
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * History:
 * --------
 *	01/06/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API void delWorkingPair(void *workingPair) {
	// Typecast void pointer given as input to pointer of Refrigerant-struct
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

    // Because of initialization function, workingPair is NULL or fully built
	//
    if (retWorkingPair != NULL) {
		// Free allocated memory of structs containing function pointers
		//
		delRefrigerant(retWorkingPair->refrigerant);

		if (retWorkingPair->absorption != NULL) {
			delAdsorption(retWorkingPair->absorption);
		}
		if (retWorkingPair->adsorption != NULL) {
			delAdsorption(retWorkingPair->adsorption);
		}

		// Free allocated memory of coefficients of functions if memory is
		// allocated
		//
		if (retWorkingPair->rhol_par != NULL) {
			free(retWorkingPair->rhol_par);
		}
		if (retWorkingPair->psat_par != NULL) {
			free(retWorkingPair->psat_par);
		}

		free(retWorkingPair->iso_par);

		// Free allocated memory of WorkingPair-struct
		//
		free(retWorkingPair);
    }
}


/*
 * warning_struct:
 * ---------------
 *
 * Auxiliary function to throw warning if some struct (e.g. workingPair or
 * Adsorption) does not exist.
 *
 * Parameters:
 * -----------
 * 	const char *name_struct:
 *		Name of struct that does not exist.
 * 	const char *name_function:
 *		Name of function that does not exist.
 *
 * History:
 * --------
 *	27/03/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
void warning_struct(const char *name_struct, const char *name_function) {
	printf("\n\n###########\n# Warning #\n###########");
	printf("\n%s-struct does not exist.", name_struct);
	printf("\nReturn -1 for function call \"%s\".", name_function);
 }


/*
 * warning_function:
 * -----------------
 *
 * Auxiliary function to throw warning if some parameter is missing.
 *
 * Parameters:
 * -----------
 * 	const char *name_parameter:
 *		Name of parameter that is missing.
 * 	const char *name_wpair_function:
 *		Name of working pair function that does not exist.
 *
 * History:
 * --------
 *	27/03/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
void warning_parameter(const char *name_parameter,
	const char *name_wpair_function) {
	printf("\n\n###########\n# Warning #\n###########");
	printf("\nParameters required to calculate \"%s\" are not stored within "
		"the workingPair-struct.", name_parameter);
	printf("\nReturn -1 for function call \"%s\".", name_wpair_function);
}


/*
 * warning_function:
 * -----------------
 *
 * Auxiliary function to throw warning if some function does not exist.
 *
 * Parameters:
 * -----------
 * 	const char *name_iso_function:
 *		Name of isotherm function that does not exist.
 * 	const char *name_wpair_function:
 *		Name of working pair function that does not exist.
 *
 * History:
 * --------
 *	27/03/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
void warning_function(const char *name_function,
	const char *name_wpair_function) {
	printf("\n\n###########\n# Warning #\n###########");
	printf("\nChosen isotherm function \"%s\" is not implemented.",
		name_function);
	printf("\nReturn -1 for function call \"%s\".", name_wpair_function);
}


////////////////////////////////////////////////////////////////////////////
// Definition of function prototypes regarding adsorption working w struct//
////////////////////////////////////////////////////////////////////////////
/*
 * ads_w_pT:
 * ---------
 *
 * Calculates equilibrium loading w in kg/kg depending on equilibrium pressure
 * p in Pa and equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium loading in kg/kg.
 *
 * History:
 * --------
 *	01/06/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_w_pT(double p_Pa, double T_K, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_w_pT");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->w_pT==NULL) {
		warning_function("w_pT", "ads_w_pT");
		return -1;

	} else {
		return retWorkingPair->adsorption->w_pT(p_Pa, T_K,
			retWorkingPair->iso_par, retWorkingPair->psat_par,
			retWorkingPair->rhol_par, retWorkingPair->adsorption,
			retWorkingPair->refrigerant);

	}
}


/*
 * ads_p_wT:
 * ---------
 *
 * Calculates equilibrium pressure p in Pa depending on equilibrium loading w
 * in kg/kg and equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium pressure in Pa.
 *
 * History:
 * --------
 *	01/06/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_p_wT(double w_kgkg, double T_K, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_p_wT");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->p_wT==NULL) {
		warning_function("p_wT", "ads_p_wT");
		return -1;

	} else {
		return retWorkingPair->adsorption->p_wT(w_kgkg, T_K,
			retWorkingPair->iso_par, retWorkingPair->psat_par,
			retWorkingPair->rhol_par, retWorkingPair->adsorption,
			retWorkingPair->refrigerant);

	}
}


/*
 * ads_T_pw:
 * ---------
 *
 * Calculates equilibrium temperature in K depending on equilibrium pressure p
 * in Pa and equilibrium loading w in kg/kg.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium temperature in K.
 *
 * Remarks:
 * --------
 *	Function uses equation of states implemented for calculation of vapor
 *	pressure, saturated liquid density, derivative of vapor pressure wrt.
 *	temperature, and derivative of saturated liquid density wrt. temperature
 *	when isotherm of working pair is based on volumetric approach (e.g.
 *	Dubinin theory).
 *
 * History:
 * --------
 *	01/06/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_T_pw(double p_Pa, double w_kgkg, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_T_pw");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->T_pw==NULL) {
		warning_function("T_pw", "ads_T_pw");
		return -1;

	} else {
		return retWorkingPair->adsorption->T_pw(p_Pa, w_kgkg,
			retWorkingPair->iso_par, retWorkingPair->psat_par,
			retWorkingPair->rhol_par, retWorkingPair->adsorption,
			retWorkingPair->refrigerant);

	}
}


/*
 * ads_dw_dp_pT:
 * -------------
 *
 * Calculates derivative of equilibrium loading w with respect to pressure
 * p in kg/kg/Pa depending on equilibrium pressure p in Pa and equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. pressure in kg/kg/Pa.
 *
 * History:
 * --------
 *	01/06/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_dw_dp_pT(double p_Pa, double T_K, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_dw_dp_pT");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->dw_dp_pT==NULL) {
		warning_function("dw_dp_pT", "ads_dw_dp_pT");
		return -1;

	} else {
		return retWorkingPair->adsorption->dw_dp_pT(p_Pa, T_K,
			retWorkingPair->iso_par, retWorkingPair->psat_par,
			retWorkingPair->rhol_par, retWorkingPair->adsorption,
			retWorkingPair->refrigerant);

	}
}


/*
 * ads_dw_dT_pT:
 * -------------
 *
 * Calculates derivative of equilibrium loading w with respect to temperature
 * T in kg/kg/K depending on equilibrium pressure p in Pa and equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. temperature in kg/kg/K.
 *
 * History:
 * --------
 *	01/06/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_dw_dT_pT(double p_Pa, double T_K, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_dw_dT_pT");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->dw_dT_pT==NULL) {
		warning_function("dw_dT_pT", "ads_dw_dT_pT");
		return -1;

	} else {
		return retWorkingPair->adsorption->dw_dT_pT(p_Pa, T_K,
			retWorkingPair->iso_par, retWorkingPair->psat_par,
			retWorkingPair->rhol_par, retWorkingPair->adsorption,
			retWorkingPair->refrigerant);

	}
}


/*
 * ads_dp_dw_wT:
 * -------------
 *
 * Calculates derivative of equilibrium pressure p with respect to loading
 * w in kgPa/kg depending on equilibrium loading w in kg/kg and equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. loading in Pakg/kg.
 *
 * History:
 * --------
 *	01/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_dp_dw_wT(double w_kgkg, double T_K, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_dp_dw_wT");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->dp_dw_wT==NULL) {
		warning_function("dp_dw_wT", "ads_dp_dw_wT");
		return -1;

	} else {
		return retWorkingPair->adsorption->dp_dw_wT(w_kgkg, T_K,
			retWorkingPair->iso_par, retWorkingPair->psat_par,
			retWorkingPair->rhol_par, retWorkingPair->adsorption,
			retWorkingPair->refrigerant);

	}
}


/*
 * ads_dp_dT_wT:
 * -------------
 *
 * Calculates derivative of equilibrium pressure p with respect to temperature
 * T in kg/kg/K depending on equilibrium loading w in kg/kg and equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. temperature in Pa/K.
 *
 * History:
 * --------
 *	01/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_dp_dT_wT(double w_kgkg, double T_K, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_dp_dT_wT");
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->dp_dw_wT==NULL) {
		warning_function("dp_dw_wT", "ads_dp_dT_wT");
		return -1;

	} else {
		return retWorkingPair->adsorption->dp_dT_wT(w_kgkg, T_K,
			retWorkingPair->iso_par, retWorkingPair->psat_par,
			retWorkingPair->rhol_par, retWorkingPair->adsorption,
			retWorkingPair->refrigerant);

	}
}


/*
 * ads_piStar_pyxgTM:
 * ------------------
 *
 * Calculates reduced spreading pressure piStar in kg/mol depending on
 * equilibrium pressure p in Pa, molar fraction of refrigerant in vapor phase
 * in mol/mol, molar fraction of refrigerant in adsorbed phase in mol/mol,
 * activity coefficient of refrigerant in adsorbed phase, equilibrium
 * temperature T in K and molar mass of refrigerant M in kg/mol. The reduced
 * spreading pressure is defined as follows:
 *
 * 	piStar = A * pi / (R * T * m_sorbent) = 1 / M *
 *		Integral_0^p0{w(p,T) / p * dp}
 *
 *	where p0 = p_total*y / (gamma*x)
 *
 * Parameters:
 * -----------
 *	double p_total_Pa:
 *		Total pressure of vapor phase in Pa.
 *	double y_molmol:
 *		Molar fraction of refrigerant in vapor phase in mol/mol.
 *	double x_molmol:
 *		Molar fraction of refrigerant in adsorbed phase in mol/mol.
 *	double gamma:
 *		Activity coefficient of refrigerant in adsorbed phase.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double M_kgmol:
 *		Molar mass of refrigerant M in kg/mol.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Reduced spreading pressure in kg/mol.
 *
 * History:
 * --------
 *	01/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_piStar_pyxgTM(double p_total_Pa, double y_molmol,
	double x_molmol, double gamma, double T_K, double M_kgmol,
	void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_piStar_pyxgTM");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->piStar_pyxgTM==NULL) {
		warning_function("piStar_pyxgTM", "ads_piStar_pyxgTM");
		return -1;

	} else {
		return retWorkingPair->adsorption->piStar_pyxgTM(p_total_Pa, y_molmol,
			x_molmol, gamma, T_K, M_kgmol, retWorkingPair->iso_par,
			retWorkingPair->psat_par, retWorkingPair->rhol_par,
			retWorkingPair->adsorption, retWorkingPair->refrigerant);

	}
}


/*
 * ads_sur_w_pT:
 * -------------
 *
 * Calculates equilibrium loading w in kg/kg depending on equilibrium pressure
 * p in Pa and equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium loading in kg/kg.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_sur_w_pT(double p_Pa, double T_K, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_sur_w_pT");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->sur_w_pT==NULL) {
		warning_function("sur_w_pT", "ads_sur_w_pT");
		return -1;

	} else {
		return retWorkingPair->adsorption->sur_w_pT(p_Pa, T_K,
			retWorkingPair->iso_par);

	}
}


/*
 * ads_sur_p_wT:
 * -------------
 *
 * Calculates equilibrium pressure p in Pa depending on equilibrium loading w
 * in kg/kg and equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium pressure in Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_sur_p_wT(double w_kgkg, double T_K, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_sur_p_wT");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->sur_p_wT==NULL) {
		warning_function("sur_p_wT", "ads_sur_p_wT");
		return -1;

	} else {
		return retWorkingPair->adsorption->sur_p_wT(w_kgkg, T_K,
			retWorkingPair->iso_par);

	}
}


/*
 * ads_sur_T_pw:
 * -------------
 *
 * Calculates equilibrium temperature in K depending on equilibrium pressure p
 * in Pa and equilibrium loading w in kg/kg.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium temperature in K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_sur_T_pw(double p_Pa, double w_kgkg, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_sur_T_pw");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->sur_T_pw==NULL) {
		warning_function("sur_T_pw", "ads_sur_T_pw");
		return -1;

	} else {
		return retWorkingPair->adsorption->sur_T_pw(p_Pa, w_kgkg,
			retWorkingPair->iso_par);

	}
}


/*
 * ads_sur_dw_dp_pT:
 * -----------------
 *
 * Calculates derivative of equilibrium loading w with respect to pressure
 * p in kg/kg/Pa depending on equilibrium pressure p in Pa and equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. pressure in kg/kg/Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_sur_dw_dp_pT(double p_Pa, double T_K, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_sur_dw_dp_pT");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->sur_dw_dp_pT==NULL) {
		warning_function("sur_dw_dp_pT", "ads_sur_dw_dp_pT");
		return -1;

	} else {
		return retWorkingPair->adsorption->sur_dw_dp_pT(p_Pa, T_K,
			retWorkingPair->iso_par);

	}
}


/*
 * ads_sur_dw_dT_pT:
 * -----------------
 *
 * Calculates derivative of equilibrium loading w with respect to temperature
 * T in kg/kg/K depending on equilibrium pressure p in Pa and equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. temperature in kg/kg/K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_sur_dw_dT_pT(double p_Pa, double T_K, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_sur_dw_dT_pT");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->sur_dw_dT_pT==NULL) {
		warning_function("sur_dw_dT_pT", "ads_sur_dw_dT_pT");
		return -1;

	} else {
		return retWorkingPair->adsorption->sur_dw_dT_pT(p_Pa, T_K,
			retWorkingPair->iso_par);

	}
}


/*
 * ads_sur_dp_dw_wT:
 * -----------------
 *
 * Calculates derivative of equilibrium pressure p with respect to loading
 * w in kgPa/kg depending on equilibrium loading w in kg/kg and equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. loading in Pakg/kg.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_sur_dp_dw_wT(double w_kgkg, double T_K, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_sur_dp_dw_wT");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->sur_dp_dw_wT==NULL) {
		warning_function("sur_dp_dw_wT", "ads_sur_dp_dw_wT");
		return -1;

	} else {
		return retWorkingPair->adsorption->sur_dp_dw_wT(w_kgkg, T_K,
			retWorkingPair->iso_par);

	}
}


/*
 * ads_sur_dp_dT_wT:
 * -----------------
 *
 * Calculates derivative of equilibrium pressure p with respect to temperature
 * T in kg/kg/K depending on equilibrium loading w in kg/kg and equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. temperature in Pa/K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_sur_dp_dT_wT(double w_kgkg, double T_K, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_sur_dp_dT_wT");
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->sur_dp_dT_wT==NULL) {
		warning_function("sur_dp_dT_wT", "ads_sur_dp_dT_wT");
		return -1;

	} else {
		return retWorkingPair->adsorption->sur_dp_dT_wT(w_kgkg, T_K,
			retWorkingPair->iso_par);

	}
}


/*
 * ads_sur_piStar_pyxgTM:
 * ----------------------
 *
 * Calculates reduced spreading pressure piStar in kg/mol depending on
 * equilibrium pressure p in Pa, molar fraction of refrigerant in vapor phase
 * in mol/mol, molar fraction of refrigerant in adsorbed phase in mol/mol,
 * activity coefficient of refrigerant in adsorbed phase, equilibrium
 * temperature T in K and molar mass of refrigerant M in kg/mol. The reduced
 * spreading pressure is defined as follows:
 *
 * 	piStar = A * pi / (R * T * m_sorbent) = 1 / M *
 *		Integral_0^p0{w(p,T) / p * dp}
 *
 *	where p0 = p_total*y / (gamma*x)
 *
 * Parameters:
 * -----------
 *	double p_total_Pa:
 *		Total pressure of vapor phase in Pa.
 *	double y_molmol:
 *		Molar fraction of refrigerant in vapor phase in mol/mol.
 *	double x_molmol:
 *		Molar fraction of refrigerant in adsorbed phase in mol/mol.
 *	double gamma:
 *		Activity coefficient of refrigerant in adsorbed phase.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double M_kgmol:
 *		Molar mass of refrigerant M in kg/mol.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Reduced spreading pressure in kg/mol.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_sur_piStar_pyxgTM(double p_total_Pa, double y_molmol,
	double x_molmol, double gamma, double T_K, double M_kgmol,
	void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_sur_piStar_pyxgTM");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->sur_piStar_pyxgTM==NULL) {
		warning_function("sur_piStar_pyxgTM", "ads_sur_piStar_pyxgTM");
		return -1;

	} else {
		return retWorkingPair->adsorption->sur_piStar_pyxgTM(p_total_Pa,
			y_molmol, x_molmol, gamma, T_K, M_kgmol, retWorkingPair->iso_par);

	}
}


/*
 * ads_sur_w_pTpsat:
 * -----------------
 *
 * Calculates equilibrium loading w in kg/kg depending on equilibrium pressure
 * p in Pa, equilibrium temperature T in K, and saturation pressure p_sat in Pa.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation vapor pressure in Pa.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium loading in kg/kg.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach using the saturated vapor pressure as additional argument.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_sur_w_pTpsat(double p_Pa, double T_K, double p_sat_Pa,
	void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_sur_w_pTpsat");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->sur_w_pTpsat==NULL) {
		warning_function("sur_w_pTpsat", "ads_sur_w_pTpsat");
		return -1;

	} else {
		return retWorkingPair->adsorption->sur_w_pTpsat(p_Pa, T_K, p_sat_Pa,
			retWorkingPair->iso_par);

	}
}


/*
 * ads_sur_p_wTpsat:
 * -----------------
 *
 * Calculates equilibrium pressure p in Pa depending on equilibrium loading w
 * in kg/kg, equilibrium temperature T in K, and saturation pressure p_sat in
 * Pa.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation vapor pressure in Pa.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium pressure in Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach using the saturated vapor pressure as additional argument.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_sur_p_wTpsat(double w_kgkg, double T_K, double p_sat_Pa,
	void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_sur_p_wTpsat");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->sur_p_wTpsat==NULL) {
		warning_function("sur_p_wTpsat", "ads_sur_p_wTpsat");
		return -1;

	} else {
		return retWorkingPair->adsorption->sur_p_wTpsat(w_kgkg, T_K, p_sat_Pa,
			retWorkingPair->iso_par);

	}
}


/*
 * ads_sur_T_pwpsat:
 * -----------------
 *
 * Calculates equilibrium temperature in K depending on equilibrium pressure p
 * in Pa, equilibrium loading w in kg/kg, and saturation pressure p_sat in Pa.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double p_sat_Pa:
 *		Saturation vapor pressure in Pa.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium temperature in K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach using the saturated vapor pressure as additional argument.
 *	Uses internal equation of states to calculate vapor pressure and derivative
 *	of vapor pressure wrt. temperature. If equation of state is not implemented
 *	for refrigerant, function returns -1 and throws warning. If user want to use
 *	his own equation of state, this function cannot be used and determination of
 *	root to calculate T must be implemented by user.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_sur_T_pwpsat(double p_Pa, double w_kgkg, double p_sat_Pa,
	void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_sur_T_pwpsat");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->sur_T_pwpsat==NULL) {
		warning_function("sur_T_pwpsat", "ads_sur_T_pwpsat");
		return -1;

	} else if (retWorkingPair->refrigerant->psat_T==NULL) {
		warning_function("psat_T", "ads_sur_T_pwpsat");
		return -1;

	} else if (retWorkingPair->refrigerant->dpsat_dT==NULL) {
		warning_function("dpsat_dT", "ads_sur_T_pwpsat");
		return -1;

	} else if (retWorkingPair->psat_par==NULL) {
		warning_parameter("saturation pressure and its derivative with respect "
		"to temperature", "ads_sur_T_pwpsat");
		return -1;

	} else {
		// Avoid compiler warning C4100 by adding "do nothing" expression
		// "Do nothing" expression will be optimized out by compiler
		//
		(p_sat_Pa) = (p_sat_Pa);

		return retWorkingPair->adsorption->sur_T_pwpsat(p_Pa, w_kgkg,
			retWorkingPair->refrigerant->psat_T,
			retWorkingPair->refrigerant->dpsat_dT,
			retWorkingPair->iso_par, retWorkingPair->psat_par);

	}
}


/*
 * ads_sur_dw_dp_pTpsat:
 * ---------------------
 *
 * Calculates derivative of equilibrium loading w with respect to pressure
 * p in kg/kg/Pa depending on equilibrium pressure p in Pa, equilibrium
 * temperature T in K, and saturation pressure p_sat in Pa.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation vapor pressure in Pa.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. pressure in kg/kg/Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach using the saturated vapor pressure as additional argument.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_sur_dw_dp_pTpsat(double p_Pa, double T_K, double p_sat_Pa,
	void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_sur_dw_dp_pTpsat");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->sur_dw_dp_pTpsat==NULL) {
		warning_function("sur_dw_dp_pTpsat", "ads_sur_dw_dp_pTpsat");
		return -1;

	} else {
		return retWorkingPair->adsorption->sur_dw_dp_pTpsat(p_Pa, T_K, p_sat_Pa,
			retWorkingPair->iso_par);

	}
}


/*
 * ads_sur_dw_dT_pTpsat:
 * ---------------------
 *
 * Calculates derivative of equilibrium loading w with respect to temperature
 * T in kg/kg/K depending on equilibrium pressure p in Pa, equilibrium
 * temperature T in K, saturation pressure p_sat in Pa, and derivative of
 * saturation pressure wrt. temperature dp_sat_dT in Pa/K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation vapor pressure in Pa.
 * 	double dp_sat_dT_PaK:
 *		Derivative of saturation pressure wrt. temperature in Pa/K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. temperature in kg/kg/K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach using the saturated vapor pressure as additional argument.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_sur_dw_dT_pTpsat(double p_Pa, double T_K, double p_sat_Pa,
	double dp_sat_dT_PaK, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_sur_dw_dT_pTpsat");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->sur_dw_dT_pTpsat==NULL) {
		warning_function("sur_dw_dT_pTpsat", "ads_sur_dw_dT_pTpsat");
		return -1;

	} else {
		return retWorkingPair->adsorption->sur_dw_dT_pTpsat(p_Pa, T_K, p_sat_Pa,
			dp_sat_dT_PaK, retWorkingPair->iso_par);

	}
}


/*
 * ads_sur_dp_dw_wTpsat:
 * ---------------------
 *
 * Calculates derivative of equilibrium pressure p with respect to loading
 * w in kgPa/kg depending on equilibrium loading w in kg/kg, equilibrium
 * temperature T in K, and saturation pressure p_sat in Pa.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation vapor pressure in Pa.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. loading in Pakg/kg.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach using the saturated vapor pressure as additional argument.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_sur_dp_dw_wTpsat(double w_kgkg, double T_K, double p_sat_Pa,
	void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_sur_dp_dw_wTpsat");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->sur_dp_dw_wTpsat==NULL) {
		warning_function("sur_dp_dw_wTpsat", "ads_sur_dp_dw_wTpsat");
		return -1;

	} else {
		return retWorkingPair->adsorption->sur_dp_dw_wTpsat(w_kgkg, T_K,
			 p_sat_Pa, retWorkingPair->iso_par);

	}
}


/*
 * ads_sur_dp_dT_wTpsat:
 * ---------------------
 *
 * Calculates derivative of equilibrium pressure p with respect to temperature
 * T in kg/kg/K depending on equilibrium loading w in kg/kg, equilibrium
 * temperature T in K, saturation pressure p_sat in Pa, and derivative of
 * saturation pressure wrt. temperature dp_sat_dT in Pa/K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation vapor pressure in Pa.
 * 	double dp_sat_dT_PaK:
 *		Derivative of saturation pressure wrt. temperature in Pa/K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. temperature in Pa/K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach using the saturated vapor pressure as additional argument.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_sur_dp_dT_wTpsat(double w_kgkg, double T_K, double p_sat_Pa,
	double dp_sat_dT_PaK, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_sur_dp_dT_wTpsat");
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->sur_dp_dT_wTpsat==NULL) {
		warning_function("sur_dp_dT_wTpsat", "ads_sur_dp_dT_wTpsat");
		return -1;

	} else {
		return retWorkingPair->adsorption->sur_dp_dT_wTpsat(w_kgkg, T_K,
			p_sat_Pa, dp_sat_dT_PaK, retWorkingPair->iso_par);

	}
}


/*
 * ads_sur_piStar_pyxgTpsatM:
 * --------------------------
 *
 * Calculates reduced spreading pressure piStar in kg/mol depending on
 * equilibrium pressure p in Pa, molar fraction of refrigerant in vapor phase
 * in mol/mol, molar fraction of refrigerant in adsorbed phase in mol/mol,
 * activity coefficient of refrigerant in adsorbed phase, equilibrium
 * temperature T in K, saturation pressure p_sat in Pa, and molar mass of
 * refrigerant M in kg/mol. The reduced spreading pressure is defined as
 * follows:
 *
 * 	piStar = A * pi / (R * T * m_sorbent) = 1 / M *
 *		Integral_0^p0{w(p,T) / p * dp}
 *
 *	where p0 = p_total*y / (gamma*x)
 *
 * Parameters:
 * -----------
 *	double p_total_Pa:
 *		Total pressure of vapor phase in Pa.
 *	double y_molmol:
 *		Molar fraction of refrigerant in vapor phase in mol/mol.
 *	double x_molmol:
 *		Molar fraction of refrigerant in adsorbed phase in mol/mol.
 *	double gamma:
 *		Activity coefficient of refrigerant in adsorbed phase.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation vapor pressure in Pa.
 *	double M_kgmol:
 *		Molar mass of refrigerant M in kg/mol.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Reduced spreading pressure in kg/mol.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach using the saturated vapor pressure as additional argument.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_sur_piStar_pyxgTpsatM(double p_total_Pa, double y_molmol,
	double x_molmol, double gamma, double T_K, double p_sat_Pa, double M_kgmol,
	void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_sur_piStar_pyxgTpsatM");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->sur_piStar_pyxgTpsatM==NULL) {
		warning_function("sur_piStar_pyxgTpsatM", "ads_sur_piStar_pyxgTpsatM");
		return -1;

	} else {
		return retWorkingPair->adsorption->sur_piStar_pyxgTpsatM(p_total_Pa,
			y_molmol, x_molmol, gamma, T_K, p_sat_Pa, M_kgmol,
			retWorkingPair->iso_par);

	}
}


/*
 * ads_vol_W_ARho:
 * ---------------
 *
 * Calculates equilibrium volumetric loading W in m³/kg depending on equilibrium
 * adsorption potential A in J/mol and saturated liquid density of adsorpt rho_l
 * in kg/m3.
 *
 * Parameters:
 * -----------
 * 	double A_Jmol:
 *		Equilibrium adsorption potential A in J/mol.
 * 	double rho_l_kgm3:
 *		Saturated liquid density of adsorpt rho_l in kg/m3.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium volumetric loading W in m³/kg.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *	Saturated liquid density of adsorpt is required to calculate volumetric
 *	loading from characteristic curve in m3/kg when coefficients would directly
 *	return loading w in kg/kg. Therefore, function checks unit of coefficients
 *	by checking isotherm coefficients.
 *
 * History:
 * --------
 *	01/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_vol_W_ARho(double A_Jmol, double rho_l_kgm3,
	void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_vol_W_ARho");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->vol_W_ARho==NULL) {
		warning_function("vol_W_ARho", "ads_vol_W_ARho");
		return -1;

	} else {
		return retWorkingPair->adsorption->vol_W_ARho(A_Jmol, rho_l_kgm3,
			retWorkingPair->iso_par);

	}
}


/*
 * ads_vol_A_WRho:
 * ---------------
 *
 * Calculates equilibrium adsorption potential A in J/mol depending on
 * equilibrium volumetric loading W in m³/kg and saturated liquid density of
 * adsorpt rho_l in kg/m3.
 *
 * Parameters:
 * -----------
 * 	double W_m3kg:
 *		Equilibrium volumetric loading W in m³/kg.
 * 	double rho_l_kgm3:
 *		Saturated liquid density of adsorpt rho_l in kg/m3.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium adsorption potential A in J/mol.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *	Saturated liquid density of adsorpt is required to calculate volumetric
 *	loading from characteristic curve in m3/kg when coefficients would directly
 *	return loading w in kg/kg.
 *
 * History:
 * --------
 *	01/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_vol_A_WRho(double W_m3kg, double rho_l_kgm3,
	void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_vol_A_WRho");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->vol_A_WRho==NULL) {
		warning_function("vol_A_WRho", "ads_vol_A_WRho");
		return -1;

	} else {
		return retWorkingPair->adsorption->vol_A_WRho(W_m3kg, rho_l_kgm3,
			retWorkingPair->iso_par);

	}
}


/*
 * ads_vol_w_pTpsatRho:
 * --------------------
 *
 * Calculates equilibrium loading w in kg/kg depending on equilibrium pressure
 * p in Pa, equilibrium temperature T in K, saturation pressure p_Sat in Pa and
 * saturated liquid density of adsorpt rho in kg/m³.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_l_kgm3:
 *		Saturated liquid density of adsorpt in kg/m³.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium loading in kg/kg.
 *
 * History:
 * --------
 *	01/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_vol_w_pTpsatRho(double p_Pa, double T_K, double p_sat_Pa,
	double rho_kgm3, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_vol_w_pTpsatRho");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->vol_w_pTpsatRho==NULL) {
		warning_function("vol_w_pTpsatRho", "ads_vol_w_pTpsatRho");
		return -1;

	} else {
		return retWorkingPair->adsorption->vol_w_pTpsatRho(p_Pa, T_K, p_sat_Pa,
			rho_kgm3, retWorkingPair->iso_par);

	}
}


/*
 * ads_vol_p_wTpsatRho:
 * --------------------
 *
 * Calculates equilibrium pressure p in Pa depending on equilibrium loading w in
 * kg/kg, equilibrium temperature T in K, saturation pressure p_Sat in Pa and
 * density of adsorpt rho in kg/m³.
 *
 * Parameters:
 * -----------
 * 	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_kgm3:
 *		Density of adsorpt in kg/m³.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure in Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *
 * History:
 * --------
 *	01/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_vol_p_wTpsatRho(double w_kgkg, double T_K, double p_sat_Pa,
	double rho_kgm3, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_vol_p_wTpsatRho");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->vol_p_wTpsatRho==NULL) {
		warning_function("vol_p_wTpsatRho", "ads_vol_p_wTpsatRho");
		return -1;

	} else {
		return retWorkingPair->adsorption->vol_p_wTpsatRho(w_kgkg, T_K,
			p_sat_Pa, rho_kgm3,retWorkingPair->iso_par);

	}
}


/*
 * ads_vol_T_pwpsatRho:
 * --------------------
 *
 * Calculates equilibrium temperature T in K depending on equilibrium pressure
 * in Pa, equilibrium loading w in kg/kg, saturation pressure p_Sat in Pa and
 * density of adsorpt rho in kg/m³.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure in Pa.
 * 	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_kgm3:
 *		Density of adsorpt in kg/m³.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium temperature in K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *	Uses internal equations of states to calculate vapor pressure, saturated
 *	liquid density of adsorpt, derivative of vapor pressure wrt. temperature
 *	and derivative of saturated liquid density of adsorpt wrt. temperature. If
 *	equation of states are not implemented for refrigerant, function returns -1
 *  and throws warning. If user want to use his own equation of states, this
 *  function cannot be used and determination of root to calculate T must
 *  be implemented by user.
 *
 * History:
 * --------
 *	01/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_vol_T_pwpsatRho(double p_Pa, double w_kgkg, double p_sat_Pa,
	double rho_kgm3, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_vol_T_pwpsatRho");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->vol_T_pwpsatRho==NULL) {
		warning_function("vol_T_pwpsatRho", "ads_vol_T_pwpsatRho");
		return -1;

	} else if (retWorkingPair->refrigerant->psat_T==NULL) {
		warning_function("psat_T", "ads_vol_T_pwpsatRho");
		return -1;

	} else if (retWorkingPair->refrigerant->rho_l_T==NULL) {
		warning_function("rho_l_T", "ads_vol_T_pwpsatRho");
		return -1;

	} else if (retWorkingPair->refrigerant->dpsat_dT==NULL) {
		warning_function("dpsat_dT", "ads_vol_T_pwpsatRho");
		return -1;

	} else if (retWorkingPair->refrigerant->drho_l_dT==NULL) {
		warning_function("drho_l_dT", "ads_vol_T_pwpsatRho");
		return -1;

	} else if (retWorkingPair->psat_par==NULL) {
		warning_parameter("saturated vapor pressure and its derivative with "
			"respect to temperature", "ads_vol_T_pwpsatRho");
		return -1;

	} else if (retWorkingPair->rhol_par==NULL) {
		warning_parameter("saturated liquid density and its derivative with "
			"respect to temperature", "ads_vol_T_pwpsatRho");
		return -1;

	} else {
		// Avoid compiler warning C4100 by adding "do nothing" expression
		// "Do nothing" expression will be optimized out by compiler
		//
		(p_sat_Pa) = (p_sat_Pa);
		(rho_kgm3) = (rho_kgm3);

		return retWorkingPair->adsorption->vol_T_pwpsatRho(p_Pa, w_kgkg,
			retWorkingPair->refrigerant->psat_T,
			retWorkingPair->refrigerant->rho_l_T,
			retWorkingPair->refrigerant->dpsat_dT,
			retWorkingPair->refrigerant->drho_l_dT,
			retWorkingPair->iso_par, retWorkingPair->psat_par,
			retWorkingPair->rhol_par);

	}
}


/*
 * ads_vol_dW_dA_ARho:
 * -------------------
 *
 * Calculates derivative of equilibrium volumetric loading dW_dA in m³mol/kg/J
 * depending on equilibrium adsorption potential A in J/mol and saturated liquid
 * density of adsorpt rho_l in kg/m3.
 *
 * Parameters:
 * -----------
 * 	double A_Jmol:
 *		Equilibrium adsorption potential A in mol/J.
 * 	double rho_l_kgm3:
 *		Saturated liquid density of adsorpt rho_l in kg/m3.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of volumetric loading wrt. adsorption potential in
 *		m³mol/kg/J.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *	Saturated liquid density of adsorpt is required to calculate volumetric
 *	loading from characteristic curve in m3/kg when coefficients would directly
 *	return loading w in kg/kg.
 *
 * History:
 * --------
 *	01/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_vol_dW_dA_ARho(double A_Jmol, double rho_l_kgm3,
	void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_vol_dW_dA_ARho");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->vol_dW_dA_ARho==NULL) {
		warning_function("vol_dW_dA_ARho", "ads_vol_dW_dA_ARho");
		return -1;

	} else {
		return retWorkingPair->adsorption->vol_dW_dA_ARho(A_Jmol, rho_l_kgm3,
			retWorkingPair->iso_par);

	}
}


/*
 * ads_vol_dA_dW_WRho:
 * -------------------
 *
 * Calculates derivative of equilibrium adsorption potential dA_dW in kgJ/mol/m³
 * depending on equilibrium volumetric loading W in m³/kg and saturated liquid
 * density of adsorpt rho_l in kg/m3.
 *
 * Parameters:
 * -----------
 * 	double W_m3kg:
 *		Equilibrium volumetric loading W in m³/kg.
 * 	double rho_l_kgm3:
 *		Saturated liquid density of adsorpt rho_l in kg/m3.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of adsorption potential wrt. volumetric loading in
 *		kgJ/mol/m³.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *	Saturated liquid density of adsorpt is required to calculate volumetric
 *	loading from characteristic curve in m3/kg when coefficients would directly
 *	return loading w in kg/kg.
 *
 * History:
 * --------
 *	01/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_vol_dA_dW_WRho(double W_m3kg, double rho_l_kgm3,
	void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_vol_dA_dW_WRho");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->vol_dA_dW_WRho==NULL) {
		warning_function("vol_dA_dW_WRho", "ads_vol_dA_dW_WRho");
		return -1;

	} else {
		return retWorkingPair->adsorption->vol_dA_dW_WRho(W_m3kg, rho_l_kgm3,
			retWorkingPair->iso_par);

	}
}


/*
 * ads_vol_dw_dp_pTpsatRho:
 * ------------------------
 *
 * Calculates derivative of equilibrium loading dw_dp with respect to pressure
 * in kg/kg/Pa depending on equilibrium pressure p in Pa, equilibrium
 * temperature T in K, saturation pressure p_Sat in Pa and density of adsorpt
 * rho in kg/m³.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_kgm3:
 *		Density of adsorpt in kg/m³.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. pressure in kg/kg/Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *
 * History:
 * --------
 *	01/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_vol_dw_dp_pTpsatRho(double p_Pa, double T_K, double p_sat_Pa,
	double rho_kgm3, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_vol_dw_dp_pTpsatRho");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->vol_dw_dp_pTpsatRho==NULL) {
		warning_function("vol_dw_dp_pTpsatRho", "ads_vol_dw_dp_pTpsatRho");
		return -1;

	} else {
		return retWorkingPair->adsorption->vol_dw_dp_pTpsatRho(p_Pa, T_K,
			p_sat_Pa, rho_kgm3, retWorkingPair->iso_par);

	}
}


/*
 * ads_vol_dw_dT_pTpsatRho:
 * ------------------------
 *
 * Calculates derivative of equilibrium loading dw_dp with respect to
 * temperature in kg/kg/K depending on equilibrium pressure p in Pa, equilibrium
 * temperature T in K, saturation pressure p_Sat in Pa, density of adsorpt
 * rho in kg/m³, derivative of saturation pressure with respect to temperature
 * dp_sat_dT in Pa/K and derivative of density of adsorpt with respect to
 * temperature drho_dT in kg/m³/K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_kgm3:
 *		Density of adsorpt in kg/m³.
 * 	double dp_sat_dT_PaK:
 *		Derivative of saturation pressure wrt. temperature in Pa/K.
 *	double drho_dT_kgm3K:
 *		Derivative of density of adsorpt wrt. temperature in kg/m³/K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. temperature in kg/kg/K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *
 * History:
 * --------
 *	01/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_vol_dw_dT_pTpsatRho(double p_Pa, double T_K, double p_sat_Pa,
	double rho_kgm3, double dp_sat_dT_PaK, double drho_dT_kgm3K,
	void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_vol_dw_dT_pTpsatRho");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->vol_dw_dT_pTpsatRho==NULL) {
		warning_function("vol_dw_dT_pTpsatRho", "ads_vol_dw_dT_pTpsatRho");
		return -1;

	} else {
		return retWorkingPair->adsorption->vol_dw_dT_pTpsatRho(p_Pa, T_K,
			p_sat_Pa, rho_kgm3, dp_sat_dT_PaK, drho_dT_kgm3K,
			retWorkingPair->iso_par);

	}
}


/*
 * ads_vol_dp_dw_wTpsatRho:
 * ------------------------
 *
 * Calculates derivative of equilibrium pressure p with respect to loading
 * w in kgPa/kg depending on equilibrium loading w in kg/kg, equilibrium
 * temperature T in K, saturation pressure p_Sat in Pa and density of adsorpt
 * rho in kg/m³.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_kgm3:
 *		Density of adsorpt in kg/m³.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. loading in Pakg/kg.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *
 * History:
 * --------
 *	01/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_vol_dp_dw_wTpsatRho(double w_kgkg, double T_K,
	double p_sat_Pa, double rho_kgm3, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_vol_dp_dw_wTpsatRho");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->vol_dp_dw_wTpsatRho==NULL) {
		warning_function("vol_dp_dw_wTpsatRho", "ads_vol_dp_dw_wTpsatRho");
		return -1;

	} else {
		return retWorkingPair->adsorption->vol_dp_dw_wTpsatRho(w_kgkg, T_K,
			p_sat_Pa, rho_kgm3, retWorkingPair->iso_par);

	}
}


/*
 * ads_vol_dp_dT_wTpsatRho:
 * ------------------------
 *
 * Calculates derivative of equilibrium pressure p with respect to temperature
 * T in kg/kg/K depending on equilibrium loading w in kg/kg, equilibrium
 * temperature T in K, saturation pressure p_Sat in Pa and density of adsorpt
 * rho in kg/m³, derivative of saturation pressure with respect to temperature
 * dp_sat_dT in Pa/K and derivative of density of adsorpt with respect to
 * temperature drho_dT in kg/m³/K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_kgm3:
 *		Density of adsorpt in kg/m³.
 * 	double dp_sat_dT_PaK:
 *		Derivative of saturation pressure wrt. temperature in Pa/K.
 *	double drho_dT_kgm3K:
 *		Derivative of density of adsorpt wrt. temperature in kg/m³/K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. temperature in Pa/K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *
 * History:
 * --------
 *	01/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_vol_dp_dT_wTpsatRho(double w_kgkg, double T_K,
	double p_sat_Pa, double rho_kgm3, double dp_sat_dT_PaK,
	double drho_dT_kgm3K, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_vol_dp_dT_wTpsatRho");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->vol_dp_dT_wTpsatRho==NULL) {
		warning_function("vol_dp_dT_wTpsatRho", "ads_vol_dp_dT_wTpsatRho");
		return -1;

	} else {
		return retWorkingPair->adsorption->vol_dp_dT_wTpsatRho(w_kgkg, T_K,
			p_sat_Pa, rho_kgm3, dp_sat_dT_PaK, drho_dT_kgm3K,
			retWorkingPair->iso_par);

	}
}


/*
 * ads_vol_piStar_pyxgTpsatRhoM:
 * -----------------------------
 *
 * Calculates reduced spreading pressure in kg/mol depending on equilibrium
 * pressure p in Pa, molar fraction of refrigerant in vapor phase in mol/mol,
 * molar fraction of refrigerant in adsorbed phase in mol/mol, activity
 * coefficient of refrigerant in adsorbed phase, equilibrium temperature T in K,
 * saturation pressure p_Sat in Pa and density of adsorpt rho in kg/m³ and molar
 * mass of refrigerant M in kg/mol. The reduced spreading pressure is defined as
 * follows:
 *
 * 	piStar = A * pi / (R * T * m_sorbent) = 1 / M *
 *		Integral_0^p0{w(p,T) / p * dp}
 *
 *	where p0 = p_total*y / (gamma*x)
 *
 * Parameters:
 * -----------
 *	double p_total_Pa:
 *		Total pressure of vapor phase in Pa.
 *	double y_molmol:
 *		Molar fraction of refrigerant in vapor phase in mol/mol.
 *	double x_molmol:
 *		Molar fraction of refrigerant in adsorbed phase in mol/mol.
 *	double gamma:
 *		Activity coefficient of refrigerant in adsorbed phase.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_kgm3:
 *		Density of adsorpt in kg/m³.
 *	double M_kgmol:
 *		Molar mass of refrigerant M in kg/mol.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Reduced spreading pressure in kg/mol.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *
 * History:
 * --------
 *	01/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_vol_piStar_pyxgTpsatRhoM(double p_total_Pa, double y_molmol,
	double x_molmol, double gamma, double T_K, double p_sat_Pa,
	double rho_kgm3, double M_kgmol, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ads_vol_piStar_pyxgTpsatRhoM");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->adsorption->vol_piStar_pyxgTpsatRhoM==NULL) {
		warning_function("vol_piStar_pyxgTpsatRhoM",
			"ads_vol_piStar_pyxgTpsatRhoM");
		return -1;

	} else {
		return retWorkingPair->adsorption->vol_piStar_pyxgTpsatRhoM(p_total_Pa,
			y_molmol, x_molmol, gamma, T_K, p_sat_Pa, rho_kgm3, M_kgmol,
			retWorkingPair->iso_par);

	}
}


////////////////////////////////////////////////////////////////////////////
// Definition of function prototypes regarding absorption working w struct//
////////////////////////////////////////////////////////////////////////////
/*
 * abs_con_X_pT:
 * -------------
 *
 * Calculates equilibrium concentration X in kg/kg depending on equilibrium
 * pressure p in Pa and equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium concentration in kg/kg.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the conventional
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double abs_con_X_pT(double p_Pa, double T_K, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "abs_X_pT");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->absorption->con_X_pT==NULL) {
		warning_function("con_X_pT", "abs_X_pT");
		return -1;

	} else {
		return retWorkingPair->absorption->con_X_pT(p_Pa, T_K,
			retWorkingPair->iso_par);

	}
}


/*
 * abs_con_p_XT:
 * -------------
 *
 * Calculates equilibrium pressure p in Pa depending on equilibrium
 * concentration X in kg/kg and equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 *	double X_kgkg:
 *		Equilibrium concentration in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium pressure in Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the conventional
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double abs_con_p_XT(double X_kgkg, double T_K, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "abs_p_XT");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->absorption->con_p_XT==NULL) {
		warning_function("con_p_XT", "abs_p_XT");
		return -1;

	} else {
		return retWorkingPair->absorption->con_p_XT(X_kgkg, T_K,
			retWorkingPair->iso_par);

	}
}


/*
 * abs_con_T_pX:
 * -------------
 *
 * Calculates equilibrium temperature in K depending on equilibrium pressure p
 * in Pa and equilibrium concentration X in kg/kg.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double X_kgkg:
 *		Equilibrium concentration in kg/kg.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium temperature in K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the conventional
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double abs_con_T_pX(double p_Pa, double X_kgkg, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "abs_T_pX");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->absorption->con_T_pX==NULL) {
		warning_function("con_T_pX", "abs_T_pX");
		return -1;

	} else {
		return retWorkingPair->absorption->con_T_pX(p_Pa, X_kgkg,
			retWorkingPair->iso_par);

	}
}


/*
 * abs_con_dX_dp_pT:
 * -----------------
 *
 * Calculates derivative of equilibrium concentration X with respect to pressure
 * p in kg/kg/Pa depending on equilibrium pressure p in Pa and equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium concentration wrt. pressure in kg/kg/Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the conventional
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double abs_con_dX_dp_pT(double p_Pa, double T_K, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "abs_dX_dp_pT");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->absorption->con_dX_dp_pT==NULL) {
		warning_function("con_dX_dp_pT", "abs_dX_dp_pT");
		return -1;

	} else {
		return retWorkingPair->absorption->con_dX_dp_pT(p_Pa, T_K,
			retWorkingPair->iso_par);

	}
}


/*
 * abs_con_dX_dT_pT:
 * -----------------
 *
 * Calculates derivative of equilibrium concentration X with respect to
 * temperature T in kg/kg/K depending on equilibrium pressure p in Pa and
 * equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium concentration wrt. temperature in kg/kg/K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the conventional
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double abs_con_dX_dT_pT(double p_Pa, double T_K, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "abs_dX_dT_pT");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->absorption->con_dX_dT_pT==NULL) {
		warning_function("con_dX_dT_pT", "abs_dX_dT_pT");
		return -1;

	} else {
		return retWorkingPair->absorption->con_dX_dT_pT(p_Pa, T_K,
			retWorkingPair->iso_par);

	}
}


/*
 * abs_con_dp_dX_XT:
 * -----------------
 *
 * Calculates derivative of equilibrium pressure p with respect to concentration
 * w in kgPa/kg depending on equilibrium concentration X in kg/kg and
 * equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 *	double X_kgkg:
 *		Equilibrium concentration in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. concentration in Pakg/kg.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the conventional
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double abs_con_dp_dX_XT(double X_kgkg, double T_K, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "abs_dp_dX_XT");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->absorption->con_dp_dX_XT==NULL) {
		warning_function("con_dp_dX_XT", "abs_dp_dX_XT");
		return -1;

	} else {
		return retWorkingPair->absorption->con_dp_dX_XT(X_kgkg, T_K,
			retWorkingPair->iso_par);

	}
}


/*
 * abs_con_dp_dT_XT:
 * -----------------
 *
 * Calculates derivative of equilibrium pressure p with respect to temperature
 * T in kg/kg/K depending on equilibrium concentration X in kg/kg and
 * equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 *	double X_kgkg:
 *		Equilibrium concentration in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. temperature in Pa/K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the conventional
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double abs_con_dp_dT_XT(double X_kgkg, double T_K, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "abs_dp_dT_XT");
		return -1;
	}

	// Check if isotherm function exists for chosen isotherm type
	//
	if (retWorkingPair->absorption->con_dp_dT_XT==NULL) {
		warning_function("con_dp_dT_XT", "abs_dp_dT_XT");
		return -1;

	} else {
		return retWorkingPair->absorption->con_dp_dT_XT(X_kgkg, T_K,
			retWorkingPair->iso_par);

	}
}


/*
 * abs_act_g_Txv1v2:
 * -----------------
 *
 * Calculates activity coefficient of first component depending on temperature
 * T_K in K, mole fraction in liquid phase x_molmol in mol/mol, molar volume of
 * first component in m³/mol, and molar volume of second component in m³/mol.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double x_molmol:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
 *	double v1_m3mol:
 *		Equilibrium molar volume of first component in m³/mol.
 *	double v2_m3mol:
 *		Equilibrium molar volume of second component in m³/mol.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Activity coefficient of first component.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on activity
 *	coefficients.
 *	Molar volumes may are not required and ignored. When molar volumes are
 * 	required, function uses molar volumes stored in JSON file when input
 *	v1_m3mol or v2_m3mol is -1. Otherwise, function uses molar volumes given by
 *	inputs.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double abs_act_g_Txv1v2(double T_K, double x_molmol, double v1_m3mol,
	double v2_m3mol, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "abs_act_g_Txv1v2");
		return -1;
	}

	// Check if absorption function exists for chosen working pair
	//
	if (retWorkingPair->absorption->act_g_Txv1v2==NULL) {
		warning_function("act_g_Txv1v2", "abs_act_g_Txv1v2");
		return -1;

	} else {
		return retWorkingPair->absorption->act_g_Txv1v2(T_K, x_molmol, v1_m3mol,
			v2_m3mol, retWorkingPair->iso_par, retWorkingPair->absorption);

	}
}


/*
 * abs_act_p_Txv1v2psat:
 * ---------------------
 *
 * Calculates equilibrium pressure p_Pa in Pa of first component depending on
 * temperature T_K in K, mole fraction in liquid phase x_molmol in mol/mol,
 * molar volume of first component in m³/mol, molar volume of second component
 * in m³/mol,and saturation pressure of first component p_sat_Pa in Pa.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double x_molmol:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
 *	double v1_m3mol:
 *		Equilibrium molar volume of first component in m³/mol.
 *	double v2_m3mol:
 *		Equilibrium molar volume of second component in m³/mol.
 *	double p_sat_Pa:
 *		Saturation pressure of first component in Pa.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure p_Pa in Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on activity
 *	coefficients.
 *	Molar volumes may are not required and ignored. When molar volumes are
 * 	required, function uses molar volumes stored in JSON file when input
 *	v1_m3mol or v2_m3mol is -1. Otherwise, function uses molar volumes given by
 *	inputs.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double abs_act_p_Txv1v2psat(double T_K, double x_molmol,
	double v1_m3mol, double v2_m3mol, double p_sat_Pa, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "abs_act_p_Txv1v2psat");
		return -1;
	}

	// Check if absorption function exists for chosen working pair
	//
	if (retWorkingPair->absorption->act_p_Txgv1v2psat==NULL) {
		warning_function("act_p_Txgv1v2psat", "abs_act_p_Txv1v2psat");
		return -1;

	} else {
		return retWorkingPair->absorption->act_p_Txgv1v2psat(T_K, x_molmol,
			v1_m3mol, v2_m3mol, p_sat_Pa, retWorkingPair->iso_par,
			retWorkingPair->absorption);

	}
}


/*
 * abs_act_x_pTv1v2psat:
 * ---------------------
 *
 * Calculates mole fraction of first component in liquid phase x_molmol in
 * mol/mol depending on equilibrium pressure p_Pa in Pa of first component,
 * temperature T_K in K, molar volume of first component in m³/mol, molar volume
 * of second component in m³/mol and saturation pressure of first component
 * p_sat_Pa in Pa
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure p_Pa in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double v1_m3mol:
 *		Equilibrium molar volume of first component in m³/mol.
 *	double v2_m3mol:
 *		Equilibrium molar volume of second component in m³/mol.
 *	double p_sat_Pa:
 *		Saturation pressure of first component in Pa.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on activity
 *	coefficients.
 *	Molar volumes may are not required and ignored. When molar volumes are
 * 	required, function uses molar volumes stored in JSON file when input
 *	v1_m3mol or v2_m3mol is -1. Otherwise, function uses molar volumes given by
 *	inputs.
 *
 * History:
 * --------
 *	30/03/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double abs_act_x_pTv1v2psat(double p_Pa, double T_K, double v1_m3mol,
	double v2_m3mol, double p_sat_Pa, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "abs_act_x_pTv1v2psat");
		return -1;
	}

	// Check if absorption function exists for chosen working pair
	//
	if (retWorkingPair->absorption->act_x_pTgv1v2psat==NULL) {
		warning_function("act_x_pTgv1v2psat", "abs_act_x_pTv1v2psat");
		return -1;

	} else {
		return retWorkingPair->absorption->act_x_pTgv1v2psat(p_Pa, T_K,
			v1_m3mol, v2_m3mol, p_sat_Pa, retWorkingPair->iso_par,
			retWorkingPair->absorption);

	}
}


/////////////////////////////////////////////////////////////////////////////
// Definition of function prototypes regarding refrigerant working w struct//
/////////////////////////////////////////////////////////////////////////////
/*
 * ref_p_sat_T:
 * ------------
 *
 * Calculates saturation pressure in Pa depending on equilibrium temperature T
 * in K.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Saturation pressure in Pa.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ref_p_sat_T(double T_K, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ref_p_sat_T");
		return -1;
	}

	// Check if refrigerant function exists for chosen working pair
	//
	if (retWorkingPair->refrigerant->psat_T==NULL) {
		warning_function("psat_T", "ref_p_sat_T");
		return -1;

	} else if (retWorkingPair->psat_par==NULL) {
		warning_parameter("the saturated vapor pressure", "ref_p_sat_T");
		return -1;

	} else {
		return retWorkingPair->refrigerant->psat_T(T_K,
			retWorkingPair->psat_par);

	}
}


/*
 * ref_dp_sat_dT_T:
 * ----------------
 *
 * Calculates derivative of saturation pressure in Pa/K wrt to temperature
 * depending on equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of saturation pressure wrt temperature in Pa/K.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ref_dp_sat_dT_T(double T_K, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ref_dp_sat_dT_T");
		return -1;
	}

	// Check if refrigerant function exists for chosen working pair
	//
	if (retWorkingPair->refrigerant->dpsat_dT==NULL) {
		warning_function("psat_T", "ref_dp_sat_dT_T");
		return -1;

	} else if (retWorkingPair->psat_par==NULL) {
		warning_parameter("the derivative of the saturated vapor pressure with "
			"respect to temperature", "ref_dp_sat_dT_T");
		return -1;

	} else {
		return retWorkingPair->refrigerant->dpsat_dT(T_K,
			retWorkingPair->psat_par);

	}
}


/*
 * ref_rho_l_T:
 * ------------
 *
 * Calculates saturated liquid density in kg/m³ depending on equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Saturated liquid density in kg/m³.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ref_rho_l_T(double T_K, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ref_rho_l_T");
		return -1;
	}

	// Check if refrigerant function exists for chosen working pair
	//
	if (retWorkingPair->refrigerant->rho_l_T==NULL) {
		warning_function("psat_T", "ref_rho_l_T");
		return -1;

	} else if (retWorkingPair->psat_par==NULL) {
		warning_parameter("the saturated liquid density", "ref_rho_l_T");
		return -1;

	} else {
		return retWorkingPair->refrigerant->rho_l_T(T_K,
			retWorkingPair->rhol_par);

	}
}


/*
 * ref_drho_l_dT_T:
 * ----------------
 *
 * Calculates derivative of saturated liquid density wrt temperature in kg/m³/K
 * depending on equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of saturated liquid density wrt temperature in kg/m³/K.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ref_drho_l_dT_T(double T_K, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ref_drho_l_dT_T");
		return -1;
	}

	// Check if refrigerant function exists for chosen working pair
	//
	if (retWorkingPair->refrigerant->drho_l_dT==NULL) {
		warning_function("psat_T", "ref_drho_l_dT_T");
		return -1;

	} else if (retWorkingPair->psat_par==NULL) {
		warning_parameter("the derivative of the saturated liquid density with "
			"respect to temperature", "ref_drho_l_dT_T");
		return -1;

	} else {
		return retWorkingPair->refrigerant->drho_l_dT(T_K,
			retWorkingPair->rhol_par);

	}
}


/*
 * ref_T_sat_p:
 * ------------
 *
 * Calculates saturation temperature in K depending on equilibrium pressure p
 * in Pa.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Saturation pressure in Pa.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium temperature in K.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ref_T_sat_p(double p_Pa, void *workingPair) {
	// Typecast void pointers given as inputs to correct structs
	//
	WorkingPair *retWorkingPair = (WorkingPair *) workingPair;

	if (retWorkingPair == NULL) {
		warning_struct("workingPair", "ref_T_sat_p");
		return -1;
	}

	// Check if refrigerant function exists for chosen working pair
	//
	if (retWorkingPair->refrigerant->Tsat_p==NULL) {
		warning_function("Tsat_p", "ref_T_sat_p");
		return -1;

	} else if (retWorkingPair->psat_par==NULL) {
		warning_parameter("the saturated vapor pressure", "ref_T_sat_p");
		return -1;

	} else {
		return retWorkingPair->refrigerant->Tsat_p(p_Pa,
			retWorkingPair->psat_par);

	}
}


/////////////////////////////////////////////////////////////////////////////
// Definition of function prototypes regarding adsorption working wo struct//
/////////////////////////////////////////////////////////////////////////////
/*
 * direct_ads_w_pT:
 * ----------------
 *
 * Calculates equilibrium loading w in kg/kg depending on equilibrium pressure
 * p in Pa, equilibrium temperature T in K, and specified working pair.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium loading in kg/kg.
 *
 * History:
 * --------
 *	02/11/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_w_pT(double p_Pa, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_w_pT(p_Pa, T_K, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_p_wT:
 * ----------------
 *
 * Calculates equilibrium pressure p in Pa depending on equilibrium temperature
 * T in K, equilibrium loading w in kg/kg, equilibrium temperature T in K, and
 * specified working pair.
 *
 * Parameters:
 * -----------
 * 	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *		Name of calculation approach for liquid density.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure in Pa.
 *
 * History:
 * --------
 *	02/11/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_p_wT(double w_kgkg, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_p_wT(w_kgkg, T_K, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_T_pw:
 * ----------------
 *
 * Calculates equilibrium temperature in K depending on equilibrium pressure p
 * in Pa, equilibrium loading w in kg/kg, and specified working pair.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *		Name of calculation approach for liquid density.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium temperature in K.
 *
 * Remarks:
 * --------
 *	Function uses equation of states implemented for calculation of vapor
 *	pressure, saturated liquid density, derivative of vapor pressure wrt.
 *	temperature, and derivative of saturated liquid density wrt. temperature
 *	when isotherm of working pair is based on volumetric approach (e.g.
 *	Dubinin theory).
 *
 * History:
 * --------
 *	02/11/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_T_pw(double p_Pa, double w_kgkg,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_T_pw(p_Pa, w_kgkg, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_dw_dp_pT:
 * --------------------
 *
 * Calculates derivative of equilibrium loading w with respect to pressure
 * p in kg/kg/Pa depending on equilibrium pressure p in Pa and equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. pressure in kg/kg/Pa.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_dw_dp_pT(double p_Pa, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_dw_dp_pT(p_Pa, T_K, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_dw_dT_pT:
 * --------------------
 *
 * Calculates derivative of equilibrium loading w with respect to temperature
 * T in kg/kg/K depending on equilibrium pressure p in Pa and equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. temperature in kg/kg/K.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_dw_dT_pT(double p_Pa, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_dw_dT_pT(p_Pa, T_K, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_dp_dw_wT:
 * --------------------
 *
 * Calculates derivative of equilibrium pressure p with respect to loading
 * w in kgPa/kg depending on equilibrium loading w in kg/kg and equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. loading in Pakg/kg.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_dp_dw_wT(double w_kgkg, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_dp_dw_wT(w_kgkg, T_K, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_dp_dT_wT:
 * --------------------
 *
 * Calculates derivative of equilibrium pressure p with respect to temperature
 * T in kg/kg/K depending on equilibrium loading w in kg/kg and equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. temperature in Pa/K.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_dp_dT_wT(double w_kgkg, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_dp_dT_wT(w_kgkg, T_K, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_piStar_pyxgTM:
 * -------------------------
 *
 * Calculates reduced spreading pressure piStar in kg/mol depending on
 * equilibrium pressure p in Pa, molar fraction of refrigerant in vapor phase
 * in mol/mol, molar fraction of refrigerant in adsorbed phase in mol/mol,
 * activity coefficient of refrigerant in adsorbed phase, equilibrium
 * temperature T in K and molar mass of refrigerant M in kg/mol. The reduced
 * spreading pressure is defined as follows:
 *
 * 	piStar = A * pi / (R * T * m_sorbent) = 1 / M *
 *		Integral_0^p0{w(p,T) / p * dp}
 *
 *	where p0 = p_total*y / (gamma*x)
 *
 * Parameters:
 * -----------
 *	double p_total_Pa:
 *		Total pressure of vapor phase in Pa.
 *	double y_molmol:
 *		Molar fraction of refrigerant in vapor phase in mol/mol.
 *	double x_molmol:
 *		Molar fraction of refrigerant in adsorbed phase in mol/mol.
 *	double gamma:
 *		Activity coefficient of refrigerant in adsorbed phase.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double M_kgmol:
 *		Molar mass of refrigerant M in kg/mol.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Reduced spreading pressure in kg/mol.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_piStar_pyxgTM(double p_total_Pa,
	double y_molmol, double x_molmol, double gamma, double T_K, double M_kgmol,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_piStar_pyxgTM(p_total_Pa, y_molmol, x_molmol, gamma,
		T_K, M_kgmol, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_sur_w_pT:
 * --------------------
 *
 * Calculates equilibrium loading w in kg/kg depending on equilibrium pressure
 * p in Pa, equilibrium temperature T in K, and specified working pair.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium loading in kg/kg.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach.
 *
 * History:
 * --------
 *	03/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_sur_w_pT(double p_Pa, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_sur_w_pT(p_Pa, T_K, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_sur_p_wT:
 * --------------------
 *
 * Calculates equilibrium pressure p in Pa depending on equilibrium temperature
 * T in K, equilibrium loading w in kg/kg, equilibrium temperature T in K, and
 * specified working pair.
 *
 * Parameters:
 * -----------
 * 	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *		Name of calculation approach for liquid density.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure in Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach.
 *
 * History:
 * --------
 *	03/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_sur_p_wT(double w_kgkg, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_sur_p_wT(w_kgkg, T_K, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_sur_T_pw:
 * --------------------
 *
 * Calculates equilibrium temperature in K depending on equilibrium pressure p
 * in Pa, equilibrium loading w in kg/kg, and specified working pair.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *		Name of calculation approach for liquid density.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium temperature in K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach.
 *
 * History:
 * --------
 *	03/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_sur_T_pw(double p_Pa, double w_kgkg,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_sur_T_pw(p_Pa, w_kgkg, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_sur_dw_dp_pT:
 * ------------------------
 *
 * Calculates derivative of equilibrium loading w with respect to pressure
 * p in kg/kg/Pa depending on equilibrium pressure p in Pa and equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. pressure in kg/kg/Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach.
 *
 * History:
 * --------
 *	03/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_sur_dw_dp_pT(double p_Pa, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_sur_dw_dp_pT(p_Pa, T_K, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_sur_dw_dT_pT:
 * ------------------------
 *
 * Calculates derivative of equilibrium loading w with respect to temperature
 * T in kg/kg/K depending on equilibrium pressure p in Pa and equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. temperature in kg/kg/K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach.
 *
 * History:
 * --------
 *	03/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_sur_dw_dT_pT(double p_Pa, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_sur_dw_dT_pT(p_Pa, T_K, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_sur_dp_dw_wT:
 * ------------------------
 *
 * Calculates derivative of equilibrium pressure p with respect to loading
 * w in kgPa/kg depending on equilibrium loading w in kg/kg and equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. loading in Pakg/kg.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach.
 *
 * History:
 * --------
 *	03/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_sur_dp_dw_wT(double w_kgkg, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_sur_dp_dw_wT(w_kgkg, T_K, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_sur_dp_dT_wT:
 * ------------------------
 *
 * Calculates derivative of equilibrium pressure p with respect to temperature
 * T in kg/kg/K depending on equilibrium loading w in kg/kg and equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. temperature in Pa/K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach.
 *
 * History:
 * --------
 *	03/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_sur_dp_dT_wT(double w_kgkg, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_sur_dp_dT_wT(w_kgkg, T_K, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_sur_piStar_pyxgTM:
 * -----------------------------
 *
 * Calculates reduced spreading pressure piStar in kg/mol depending on
 * equilibrium pressure p in Pa, molar fraction of refrigerant in vapor phase
 * in mol/mol, molar fraction of refrigerant in adsorbed phase in mol/mol,
 * activity coefficient of refrigerant in adsorbed phase, equilibrium
 * temperature T in K and molar mass of refrigerant M in kg/mol. The reduced
 * spreading pressure is defined as follows:
 *
 * 	piStar = A * pi / (R * T * m_sorbent) = 1 / M *
 *		Integral_0^p0{w(p,T) / p * dp}
 *
 *	where p0 = p_total*y / (gamma*x)
 *
 * Parameters:
 * -----------
 *	double p_total_Pa:
 *		Total pressure of vapor phase in Pa.
 *	double y_molmol:
 *		Molar fraction of refrigerant in vapor phase in mol/mol.
 *	double x_molmol:
 *		Molar fraction of refrigerant in adsorbed phase in mol/mol.
 *	double gamma:
 *		Activity coefficient of refrigerant in adsorbed phase.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double M_kgmol:
 *		Molar mass of refrigerant M in kg/mol.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Reduced spreading pressure in kg/mol.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach.
 *
 * History:
 * --------
 *	03/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_sur_piStar_pyxgTM(double p_total_Pa,
	double y_molmol, double x_molmol, double gamma, double T_K, double M_kgmol,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_sur_piStar_pyxgTM(p_total_Pa, y_molmol, x_molmol, gamma,
		T_K, M_kgmol, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_sur_w_pTpsat:
 * ------------------------
 *
 * Calculates equilibrium loading w in kg/kg depending on equilibrium pressure
 * p in Pa, equilibrium temperature T in K, and saturation pressure p_sat in Pa.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation vapor pressure in Pa.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium loading in kg/kg.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach using the saturated vapor pressure as additional argument.
 *
 * History:
 * --------
 *	03/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_sur_w_pTpsat(double p_Pa, double T_K,
	double p_sat_Pa, const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_sur_w_pTpsat(p_Pa, T_K, p_sat_Pa, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_sur_p_wTpsat:
 * ------------------------
 *
 * Calculates equilibrium pressure p in Pa depending on equilibrium loading w
 * in kg/kg, equilibrium temperature T in K, and saturation pressure p_sat in
 * Pa.
 *
 * Parameters:
 * -----------
 * 	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation vapor pressure in Pa.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *		Name of calculation approach for liquid density.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure in Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach using the saturated vapor pressure as additional argument.
 *
 * History:
 * --------
 *	03/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_sur_p_wTpsat(double w_kgkg, double T_K,
	double p_sat_Pa, const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_sur_p_wTpsat(w_kgkg, T_K, p_sat_Pa, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_sur_T_pwpsat:
 * ------------------------
 *
 * Calculates equilibrium temperature in K depending on equilibrium pressure p
 * in Pa, equilibrium loading w in kg/kg, and saturation pressure p_sat in Pa.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double p_sat_Pa:
 *		Saturation vapor pressure in Pa.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *		Name of calculation approach for liquid density.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium temperature in K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach using the saturated vapor pressure as additional argument.
 *	Uses internal equation of states to calculate vapor pressure and derivative
 *	of vapor pressure wrt. temperature. If equation of state is not implemented
 *	for refrigerant, function returns -1 and throws warning. If user want to use
 *	his own equation of state, this function cannot be used and determination of
 *	root to calculate T must be implemented by user.
 *
 * History:
 * --------
 *	03/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_sur_T_pwpsat(double p_Pa, double w_kgkg,
	double p_sat_Pa, const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_sur_T_pwpsat(p_Pa, w_kgkg, p_sat_Pa, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_sur_dw_dp_pTpsat:
 * ----------------------------
 *
 * Calculates derivative of equilibrium loading w with respect to pressure
 * p in kg/kg/Pa depending on equilibrium pressure p in Pa, equilibrium
 * temperature T in K, and saturation pressure p_sat in Pa.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation vapor pressure in Pa.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. pressure in kg/kg/Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach using the saturated vapor pressure as additional argument.
 *
 * History:
 * --------
 *	03/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_sur_dw_dp_pTpsat(double p_Pa, double T_K,
	double p_sat_Pa, const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_sur_dw_dp_pTpsat(p_Pa, T_K, p_sat_Pa, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_sur_dw_dT_pTpsat:
 * ----------------------------
 *
 * Calculates derivative of equilibrium loading w with respect to temperature
 * T in kg/kg/K depending on equilibrium pressure p in Pa, equilibrium
 * temperature T in K, saturation pressure p_sat in Pa, and derivative of
 * saturation pressure wrt. temperature dp_sat_dT in Pa/K.
 *
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation vapor pressure in Pa.
 * 	double dp_sat_dT_PaK:
 *		Derivative of saturation pressure wrt. temperature in Pa/K.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. temperature in kg/kg/K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach using the saturated vapor pressure as additional argument.
 *
 * History:
 * --------
 *	03/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_sur_dw_dT_pTpsat(double p_Pa, double T_K,
	double p_sat_Pa, double dp_sat_dT_PaK, const char *path_db,
	const char *wp_as, const char *wp_st, const char *wp_rf, const char *wp_iso,
	int no_iso, const char *rf_psat, int no_p_sat, const char *rf_rhol,
	int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_sur_dw_dT_pTpsat(p_Pa, T_K, p_sat_Pa, dp_sat_dT_PaK,
		retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_sur_dp_dw_wTpsat:
 * ----------------------------
 *
 * Calculates derivative of equilibrium pressure p with respect to loading
 * w in kgPa/kg depending on equilibrium loading w in kg/kg, equilibrium
 * temperature T in K, and saturation pressure p_sat in Pa.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation vapor pressure in Pa.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. loading in Pakg/kg.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach using the saturated vapor pressure as additional argument.
 *
 * History:
 * --------
 *	03/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_sur_dp_dw_wTpsat(double w_kgkg, double T_K,
	double p_sat_Pa, const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_sur_dp_dw_wTpsat(w_kgkg, T_K, p_sat_Pa, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_sur_dp_dT_wTpsat:
 * ----------------------------
 *
 * Calculates derivative of equilibrium pressure p with respect to temperature
 * T in kg/kg/K depending on equilibrium loading w in kg/kg, equilibrium
 * temperature T in K, saturation pressure p_sat in Pa, and derivative of
 * saturation pressure wrt. temperature dp_sat_dT in Pa/K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation vapor pressure in Pa.
 * 	double dp_sat_dT_PaK:
 *		Derivative of saturation pressure wrt. temperature in Pa/K.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. temperature in Pa/K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach using the saturated vapor pressure as additional argument.
 *
 * History:
 * --------
 *	03/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_sur_dp_dT_wTpsat(double w_kgkg, double T_K,
	double p_sat_Pa, double dp_sat_dT_PaK, const char *path_db,
	const char *wp_as, const char *wp_st, const char *wp_rf, const char *wp_iso,
	int no_iso, const char *rf_psat, int no_p_sat, const char *rf_rhol,
	int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_sur_dp_dT_wTpsat(w_kgkg, T_K, p_sat_Pa, dp_sat_dT_PaK,
		retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_sur_piStar_pyxgTpsatM:
 * ---------------------------------
 *
 * Calculates reduced spreading pressure piStar in kg/mol depending on
 * equilibrium pressure p in Pa, molar fraction of refrigerant in vapor phase
 * in mol/mol, molar fraction of refrigerant in adsorbed phase in mol/mol,
 * activity coefficient of refrigerant in adsorbed phase, equilibrium
 * temperature T in K, saturation pressure p_sat in Pa, and molar mass of
 * refrigerant M in kg/mol. The reduced spreading pressure is defined as
 * follows:
 *
 * 	piStar = A * pi / (R * T * m_sorbent) = 1 / M *
 *		Integral_0^p0{w(p,T) / p * dp}
 *
 *	where p0 = p_total*y / (gamma*x)
 *
 * Parameters:
 * -----------
 *	double p_total_Pa:
 *		Total pressure of vapor phase in Pa.
 *	double y_molmol:
 *		Molar fraction of refrigerant in vapor phase in mol/mol.
 *	double x_molmol:
 *		Molar fraction of refrigerant in adsorbed phase in mol/mol.
 *	double gamma:
 *		Activity coefficient of refrigerant in adsorbed phase.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation vapor pressure in Pa.
 *	double M_kgmol:
 *		Molar mass of refrigerant M in kg/mol.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Reduced spreading pressure in kg/mol.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach using the saturated vapor pressure as additional argument.
 *
 * History:
 * --------
 *	03/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_sur_piStar_pyxgTpsatM(double p_total_Pa,
	double y_molmol, double x_molmol, double gamma, double T_K, double p_sat_Pa,
	double M_kgmol, const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_sur_piStar_pyxgTpsatM(p_total_Pa, y_molmol, x_molmol,
		gamma, T_K, p_sat_Pa, M_kgmol, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_vol_W_ARho:
 * ----------------------
 *
 * Calculates equilibrium volumetric loading W in m³/kg depending on equilibrium
 * adsorption potential A in J/mol and saturated liquid density of adsorpt rho_l
 * in kg/m3.
 *
 * Parameters:
 * -----------
 * 	double A_Jmol:
 *		Equilibrium adsorption potential A in J/mol.
 * 	double rho_l_kgm3:
 *		Saturated liquid density of adsorpt rho_l in kg/m3.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium volumetric loading W in m³/kg.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *	Saturated liquid density of adsorpt is required to calculate volumetric
 *	loading from characteristic curve in m3/kg when coefficients would directly
 *	return loading w in kg/kg. Therefore, function checks unit of coefficients
 *	by checking isotherm coefficients.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_vol_W_ARho(double A_Jmol, double rho_l_kgm3,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_vol_W_ARho(A_Jmol, rho_l_kgm3, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_vol_A_WRho:
 * ----------------------
 *
 * Calculates equilibrium adsorption potential A in J/mol depending on
 * equilibrium volumetric loading W in m³/kg and saturated liquid density of
 * adsorpt rho_l in kg/m3.
 *
 * Parameters:
 * -----------
 * 	double W_m3kg:
 *		Equilibrium volumetric loading W in m³/kg.
 * 	double rho_l_kgm3:
 *		Saturated liquid density of adsorpt rho_l in kg/m3.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium adsorption potential A in J/mol.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *	Saturated liquid density of adsorpt is required to calculate volumetric
 *	loading from characteristic curve in m3/kg when coefficients would directly
 *	return loading w in kg/kg.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_vol_A_WRho(double W_m3kg, double rho_l_kgm3,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_vol_A_WRho(W_m3kg, rho_l_kgm3, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_vol_w_pTpsatRho:
 * ---------------------------
 *
 * Calculates equilibrium loading w in kg/kg depending on equilibrium pressure
 * p in Pa, equilibrium temperature T in K, saturation pressure p_Sat in Pa and
 * saturated liquid density of adsorpt rho in kg/m³.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_l_kgm3:
 *		Saturated liquid density of adsorpt in kg/m³.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium loading in kg/kg.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_vol_w_pTpsatRho(double p_Pa, double T_K,
	double p_sat_Pa, double rho_kgm3, const char *path_db, const char *wp_as,
	const char *wp_st, const char *wp_rf, const char *wp_iso, int no_iso,
	const char *rf_psat, int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_vol_w_pTpsatRho(p_Pa, T_K, p_sat_Pa, rho_kgm3,
		retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_vol_p_wTpsatRho:
 * ---------------------------
 *
 * Calculates equilibrium pressure p in Pa depending on equilibrium loading w in
 * kg/kg, equilibrium temperature T in K, saturation pressure p_Sat in Pa and
 * density of adsorpt rho in kg/m³.
 *
 * Parameters:
 * -----------
 * 	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_kgm3:
 *		Density of adsorpt in kg/m³.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure in Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_vol_p_wTpsatRho(double w_kgkg, double T_K,
	double p_sat_Pa, double rho_kgm3, const char *path_db, const char *wp_as,
	const char *wp_st, const char *wp_rf, const char *wp_iso, int no_iso,
	const char *rf_psat, int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_vol_p_wTpsatRho(w_kgkg, T_K, p_sat_Pa, rho_kgm3,
		retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_vol_T_pwpsatRho:
 * ---------------------------
 *
 * Calculates equilibrium temperature T in K depending on equilibrium pressure
 * in Pa, equilibrium loading w in kg/kg, saturation pressure p_Sat in Pa and
 * density of adsorpt rho in kg/m³.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure in Pa.
 * 	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_kgm3:
 *		Density of adsorpt in kg/m³.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium temperature in K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *	Uses internal equation of states to calculate vapor pressure, saturated
 *	liquid density of adsorpt, derivative of vapor pressure wrt. temperature
 *	and derivative of saturated liquid density of adsorpt wrt. temperature. If
 *	equation of states are not implemented for refrigerant, function returns -1
 *  and throws warning. If user want to use his own equation of states, this
 *  function cannot be used and determination of root to calculate T must
 *  be implemented by user.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_vol_T_pwpsatRho(double p_Pa, double w_kgkg,
	double p_sat_Pa, double rho_kgm3, const char *path_db, const char *wp_as,
	const char *wp_st, const char *wp_rf, const char *wp_iso, int no_iso,
	const char *rf_psat, int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_vol_T_pwpsatRho(p_Pa, w_kgkg, p_sat_Pa, rho_kgm3,
		retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_vol_dW_dA_ARho:
 * --------------------------
 *
 * Calculates derivative of equilibrium volumetric loading dW_dA in m³mol/kg/J
 * depending on equilibrium adsorption potential A in J/mol and saturated liquid
 * density of adsorpt rho_l in kg/m3.
 *
 * Parameters:
 * -----------
 * 	double A_Jmol:
 *		Equilibrium adsorption potential A in J/mol.
 * 	double rho_l_kgm3:
 *		Saturated liquid density of adsorpt rho_l in kg/m3.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of volumetric loading wrt. adsorption potential in
 *		m³mol/kg/J.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *	Saturated liquid density of adsorpt is required to calculate volumetric
 *	loading from characteristic curve in m3/kg when coefficients would directly
 *	return loading w in kg/kg.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_vol_dW_dA_ARho(double A_Jmol, double rho_l_kgm3,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_vol_dW_dA_ARho(A_Jmol, rho_l_kgm3, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_vol_dA_dW_WRho:
 * --------------------------
 *
 * Calculates derivative of equilibrium adsorption potential dA_dW in kgJ/mol/m³
 * depending on equilibrium volumetric loading W in m³/kg and saturated liquid
 * density of adsorpt rho_l in kg/m3.
 *
 * Parameters:
 * -----------
 * 	double W_m3kg:
 *		Equilibrium volumetric loading W in m³/kg.
 * 	double rho_l_kgm3:
 *		Saturated liquid density of adsorpt rho_l in kg/m3.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of adsorption potential wrt. volumetric loading in
 *		kgJ/mol/m³.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *	Saturated liquid density of adsorpt is required to calculate volumetric
 *	loading from characteristic curve in m3/kg when coefficients would directly
 *	return loading w in kg/kg.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_vol_dA_dW_WRho(double W_m3kg, double rho_l_kgm3,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_vol_dA_dW_WRho(W_m3kg, rho_l_kgm3, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_vol_dw_dp_pTpsatRho:
 * -------------------------------
 *
 * Calculates derivative of equilibrium loading dw_dp with respect to pressure
 * in kg/kg/Pa depending on equilibrium pressure p in Pa, equilibrium
 * temperature T in K, saturation pressure p_Sat in Pa and density of adsorpt
 * rho in kg/m³.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_kgm3:
 *		Density of adsorpt in kg/m³.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. pressure in kg/kg/Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_vol_dw_dp_pTpsatRho(double p_Pa, double T_K,
	double p_sat_Pa, double rho_kgm3, const char *path_db, const char *wp_as,
	const char *wp_st, const char *wp_rf, const char *wp_iso, int no_iso,
	const char *rf_psat, int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_vol_dw_dp_pTpsatRho(p_Pa, T_K, p_sat_Pa, rho_kgm3,
		retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_vol_dw_dT_pTpsatRho:
 * -------------------------------
 *
 * Calculates derivative of equilibrium loading dw_dp with respect to
 * temperature in kg/kg/K depending on equilibrium pressure p in Pa, equilibrium
 * temperature T in K, saturation pressure p_Sat in Pa, density of adsorpt
 * rho in kg/m³, derivative of saturation pressure with respect to temperature
 * dp_sat_dT in Pa/K and derivative of density of adsorpt with respect to
 * temperature drho_dT in kg/m³/K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_kgm3:
 *		Density of adsorpt in kg/m³.
 * 	double dp_sat_dT_PaK:
 *		Derivative of saturation pressure wrt. temperature in Pa/K.
 *	double drho_dT_kgm3K:
 *		Derivative of density of adsorpt wrt. temperature in kg/m³/K.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. temperature in kg/kg/K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_vol_dw_dT_pTpsatRho(double p_Pa, double T_K,
	double p_sat_Pa, double rho_kgm3, double dp_sat_dT_PaK,
	double drho_dT_kgm3K, const char *path_db, const char *wp_as,
	const char *wp_st, const char *wp_rf, const char *wp_iso, int no_iso,
	const char *rf_psat, int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_vol_dw_dT_pTpsatRho(p_Pa, T_K, p_sat_Pa, rho_kgm3,
		dp_sat_dT_PaK, drho_dT_kgm3K, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_vol_dp_dw_wTpsatRho:
 * -------------------------------
 *
 * Calculates derivative of equilibrium pressure p with respect to loading
 * w in kgPa/kg depending on equilibrium loading w in kg/kg, equilibrium
 * temperature T in K, saturation pressure p_Sat in Pa and density of adsorpt
 * rho in kg/m³.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_kgm3:
 *		Density of adsorpt in kg/m³.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. loading in Pakg/kg.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_vol_dp_dw_wTpsatRho(double w_kgkg, double T_K,
	double p_sat_Pa, double rho_kgm3, const char *path_db, const char *wp_as,
	const char *wp_st, const char *wp_rf, const char *wp_iso, int no_iso,
	const char *rf_psat, int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_vol_dp_dw_wTpsatRho(w_kgkg, T_K, p_sat_Pa, rho_kgm3,
		retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_vol_dp_dT_wTpsatRho:
 * -------------------------------
 *
 * Calculates derivative of equilibrium pressure p with respect to temperature
 * T in kg/kg/K depending on equilibrium loading w in kg/kg, equilibrium
 * temperature T in K, saturation pressure p_Sat in Pa and density of adsorpt
 * rho in kg/m³, derivative of saturation pressure with respect to temperature
 * dp_sat_dT in Pa/K and derivative of density of adsorpt with respect to
 * temperature drho_dT in kg/m³/K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_kgm3:
 *		Density of adsorpt in kg/m³.
 * 	double dp_sat_dT_PaK:
 *		Derivative of saturation pressure wrt. temperature in Pa/K.
 *	double drho_dT_kgm3K:
 *		Derivative of density of adsorpt wrt. temperature in kg/m³/K.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. temperature in Pa/K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_vol_dp_dT_wTpsatRho(double w_kgkg, double T_K,
	double p_sat_Pa, double rho_kgm3, double dp_sat_dT_PaK,
	double drho_dT_kgm3K, const char *path_db, const char *wp_as,
	const char *wp_st, const char *wp_rf, const char *wp_iso, int no_iso,
	const char *rf_psat, int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_vol_dp_dT_wTpsatRho(w_kgkg, T_K, p_sat_Pa, rho_kgm3,
		dp_sat_dT_PaK, drho_dT_kgm3K, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ads_vol_piStar_pyxgTpsatRhoM:
 * ------------------------------------
 *
 * Calculates reduced spreading pressure in kg/mol depending on equilibrium
 * pressure p in Pa, molar fraction of refrigerant in vapor phase in mol/mol,
 * molar fraction of refrigerant in adsorbed phase in mol/mol, activity
 * coefficient of refrigerant in adsorbed phase, equilibrium temperature T in K,
 * saturation pressure p_Sat in Pa and density of adsorpt rho in kg/m³ and molar
 * mass of refrigerant M in kg/mol. The reduced spreading pressure is defined as
 * follows:
 *
 * 	piStar = A * pi / (R * T * m_sorbent) = 1 / M *
 *		Integral_0^p0{w(p,T) / p * dp}
 *
 *	where p0 = p_total*y / (gamma*x)
 *
 * Parameters:
 * -----------
 *	double p_total_Pa:
 *		Total pressure of vapor phase in Pa.
 *	double y_molmol:
 *		Molar fraction of refrigerant in vapor phase in mol/mol.
 *	double x_molmol:
 *		Molar fraction of refrigerant in adsorbed phase in mol/mol.
 *	double gamma:
 *		Activity coefficient of refrigerant in adsorbed phase.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_kgm3:
 *		Density of adsorpt in kg/m³.
 *	double M_kgmol:
 *		Molar mass of refrigerant M in kg/mol.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Reduced spreading pressure in kg/mol.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_vol_piStar_pyxgTpsatRhoM(double p_total_Pa,
	double y_molmol, double x_molmol, double gamma, double T_K, double p_sat_Pa,
	double rho_kgm3, double M_kgmol, const char *path_db, const char *wp_as,
	const char *wp_st, const char *wp_rf, const char *wp_iso, int no_iso,
	const char *rf_psat, int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ads_vol_piStar_pyxgTpsatRhoM(p_total_Pa, y_molmol, x_molmol,
		gamma, T_K, p_sat_Pa, rho_kgm3, M_kgmol, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/////////////////////////////////////////////////////////////////////////////
// Definition of function prototypes regarding absorption working wo struct//
/////////////////////////////////////////////////////////////////////////////
/*
 * direct_abs_con_X_pT:
 * --------------------
 *
 * Calculates equilibrium concentration X in kg/kg depending on equilibrium
 * pressure p in Pa and equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium concentration in kg/kg.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the conventional
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_abs_con_X_pT_workingPair(double p_Pa, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = abs_con_X_pT(p_Pa, T_K, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_abs_con_p_XT:
 * --------------------
 *
 * Calculates equilibrium pressure p in Pa depending on equilibrium
 * concentration X in kg/kg and equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 *	double X_kgkg:
 *		Equilibrium concentration in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium pressure in Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the conventional
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_abs_con_p_XT_workingPair(double X_kgkg, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = abs_con_p_XT(X_kgkg, T_K, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_abs_con_T_pX:
 * --------------------
 *
 * Calculates equilibrium temperature in K depending on equilibrium pressure p
 * in Pa and equilibrium concentration X in kg/kg.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double X_kgkg:
 *		Equilibrium concentration in kg/kg.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium temperature in K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the conventional
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_abs_con_T_pX_workingPair(double p_Pa, double X_kgkg,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = abs_con_T_pX(p_Pa, X_kgkg, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_abs_con_dX_dp_pT:
 * ------------------------
 *
 * Calculates derivative of equilibrium concentration X with respect to pressure
 * p in kg/kg/Pa depending on equilibrium pressure p in Pa and equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium concentration wrt. pressure in kg/kg/Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the conventional
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_abs_con_dX_dp_pT_workingPair(double p_Pa, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = abs_con_dX_dp_pT(p_Pa, T_K, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_abs_con_dX_dT_pT:
 * ------------------------
 *
 * Calculates derivative of equilibrium concentration X with respect to
 * temperature T in kg/kg/K depending on equilibrium pressure p in Pa and
 * equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium concentration wrt. temperature in kg/kg/K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the conventional
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_abs_con_dX_dT_pT_workingPair(double p_Pa, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = abs_con_dX_dT_pT(p_Pa, T_K, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_abs_con_dp_dX_XT:
 * ------------------------
 *
 * Calculates derivative of equilibrium pressure p with respect to concentration
 * w in kgPa/kg depending on equilibrium concentration X in kg/kg and
 * equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 *	double X_kgkg:
 *		Equilibrium concentration in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. concentration in Pakg/kg.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the conventional
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_abs_con_dp_dX_XT_workingPair(double X_kgkg, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = abs_con_dp_dX_XT(X_kgkg, T_K, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_abs_con_dp_dT_XT:
 * ------------------------
 *
 * Calculates derivative of equilibrium pressure p with respect to temperature
 * T in kg/kg/K depending on equilibrium concentration X in kg/kg and
 * equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 *	double X_kgkg:
 *		Equilibrium concentration in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. temperature in Pa/K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the conventional
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_abs_con_dp_dT_XT_workingPair(double X_kgkg, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = abs_con_dp_dT_XT(X_kgkg, T_K, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_abs_act_g_Txv1v2:
 * ------------------------
 *
 * Calculates activity coefficient of first component depending on temperature
 * T_K in K, mole fraction in liquid phase x_molmol in mol/mol, molar volume of
 * first component in m³/mol, and molar volume of second component in m³/mol.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double x_molmol:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
 *	double v1_m3mol:
 *		Equilibrium molar volume of first component in m³/mol.
 *	double v2_m3mol:
 *		Equilibrium molar volume of second component in m³/mol.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Activity coefficient of first component.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on activity
 *	coefficients.
 *	Molar volumes may are not required and ignored. When molar volumes are
 * 	required, function uses molar volumes stored in JSON file when input
 *	v1_m3mol or v2_m3mol is -1. Otherwise, function uses molar volumes given by
 *	inputs.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_abs_act_g_Txv1v2(double T_K, double x_molmol,
	double v1_m3mol, double v2_m3mol, const char *path_db, const char *wp_as,
	const char *wp_st, const char *wp_rf, const char *wp_iso, int no_iso,
	const char *rf_psat, int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = abs_act_g_Txv1v2(T_K, x_molmol, v1_m3mol, v2_m3mol,
		retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_abs_act_p_Txv1v2psat:
 * ----------------------------
 *
 * Calculates equilibrium pressure p_Pa in Pa of first component depending on
 * temperature T_K in K, mole fraction in liquid phase x_molmol in mol/mol,
 * molar volume of first component in m³/mol, molar volume of second component
 * in m³/mol,and saturation pressure of first component p_sat_Pa in Pa.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double x_molmol:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
 *	double v1_m3mol:
 *		Equilibrium molar volume of first component in m³/mol.
 *	double v2_m3mol:
 *		Equilibrium molar volume of second component in m³/mol.
 *	double p_sat_Pa:
 *		Saturation pressure of first component in Pa.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure p_Pa in Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on activity
 *	coefficients.
 *	Molar volumes may are not required and ignored. When molar volumes are
 * 	required, function uses molar volumes stored in JSON file when input
 *	v1_m3mol or v2_m3mol is -1. Otherwise, function uses molar volumes given by
 *	inputs.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_abs_act_p_Txv1v2psat(double T_K, double x_molmol,
	double v1_m3mol, double v2_m3mol, double p_sat_Pa, const char *path_db,
	const char *wp_as, const char *wp_st, const char *wp_rf, const char *wp_iso,
	int no_iso, const char *rf_psat, int no_p_sat, const char *rf_rhol,
	int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = abs_act_p_Txv1v2psat(T_K, x_molmol, v1_m3mol, v2_m3mol,
		p_sat_Pa, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_abs_act_x_pTv1v2psat:
 * ----------------------------
 *
 * Calculates mole fraction of first component in liquid phase x_molmol in
 * mol/mol depending on equilibrium pressure p_Pa in Pa of first component,
 * temperature T_K in K, molar volume of first component in m³/mol, molar volume
 * of second component in m³/mol and saturation pressure of first component
 * p_sat_Pa in Pa
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure p_Pa in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double v1_m3mol:
 *		Equilibrium molar volume of first component in m³/mol.
 *	double v2_m3mol:
 *		Equilibrium molar volume of second component in m³/mol.
 *	double p_sat_Pa:
 *		Saturation pressure of first component in Pa.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on activity
 *	coefficients.
 *	Molar volumes may are not required and ignored. When molar volumes are
 * 	required, function uses molar volumes stored in JSON file when input
 *	v1_m3mol or v2_m3mol is -1. Otherwise, function uses molar volumes given by
 *	inputs.
 *
 * History:
 * --------
 *	30/03/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_abs_act_x_pTv1v2psat(double p_Pa, double T_K,
	double v1_m3mol, double v2_m3mol, double p_sat_Pa, const char *path_db,
	const char *wp_as, const char *wp_st, const char *wp_rf, const char *wp_iso,
	int no_iso, const char *rf_psat, int no_p_sat, const char *rf_rhol,
	int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = abs_act_x_pTv1v2psat(p_Pa, T_K, v1_m3mol, v2_m3mol,
		p_sat_Pa, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/////////////////////////////////////////////////////////////////////////////
// Definition of function prototypes regarding refrigerant working w struct//
/////////////////////////////////////////////////////////////////////////////
/*
 * direct_ref_p_sat_T:
 * -------------------
 *
 * Calculates saturation pressure in Pa depending on equilibrium temperature T
 * in K.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Saturation pressure in Pa.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ref_p_sat_T(double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ref_p_sat_T(T_K, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ref_dp_sat_dT_T:
 * -----------------------
 *
 * Calculates derivative of saturation pressure in Pa/K wrt to temperature
 * depending on equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of saturation pressure wrt temperature in Pa/K.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ref_dp_sat_dT_T(double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ref_dp_sat_dT_T(T_K, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ref_rho_l_T:
 * -------------------
 *
 * Calculates saturated liquid density in kg/m³ depending on equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Saturated liquid density in kg/m³.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ref_rho_l_T(double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ref_rho_l_T(T_K, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ref_drho_l_dT_T:
 * -----------------------
 *
 * Calculates derivative of saturated liquid density wrt temperature in kg/m³/K
 * depending on equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of saturated liquid density wrt temperature in kg/m³/K.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ref_drho_l_dT_T(double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ref_drho_l_dT_T(T_K, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}


/*
 * direct_ref_T_sat_p:
 * -------------------
 *
 * Calculates saturation temperature in K depending on equilibrium pressure p
 * in Pa.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Saturation pressure in Pa.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium temperature in K.
 *
 * History:
 * --------
 *	30/03/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ref_p_sat_T_workingPair(double p_Pa,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Create workingPair-struct to execute function
	//
	WorkingPair *retWorkingPair = newWorkingPair(
		path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
		rf_rhol, no_rhol);

	// Execute requested function
	//
	double result = ref_T_sat_p(p_Pa, retWorkingPair);

	if (retWorkingPair != NULL) {
		delWorkingPair(retWorkingPair);
	}

	return result;
}
