///////////////////
// refrigerant.c //
///////////////////
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "refrigerant.h"
#include "refrigerant_cubicEoS.h"
#include "refrigerant_saturatedLiquidDensity.h"
#include "refrigerant_vaporPressure.h"
#include "structDefinitions.c"


/////////////////////////////
// Definition of functions //
/////////////////////////////
/*
 * newRefrigerant:
 * ---------------
 *
 * Initialization function to create a new Refrigerant-struct. Therefore,
 * function checks first if enough memory available to allocate refrigerant
 * struct and then set function pointers within the struct depending on the
 * calculation approach that is given as input. If memory allocation fails,
 * functions returns NULL and prints a warning.
 *
 * Parameters:
 * -----------
 * 	const char *vaporPressure_approach:
 *		Calculation approach for vapor pressure.
 * 	const char *saturatedLiquidDensity_approach:
 *		Calculation approach for saturated liquid density.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for vapor refrigerant
 *		functions.
 *
 * Returns:
 * --------
 *	struct *Refrigerant:
 *		Returns malloc-ed Refrigerant-struct that contains function pointers
 *		referring to functions of refrigerant.
 *
 * History:
 * --------
 *	01/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *	04/07/2020, by Mirko Engelpracht:
 *		Added function pointers for cubic equation of states.
 *
 */
Refrigerant *newRefrigerant(const char *vaporPressure_approach,
	const char *saturatedLiquidDensity_approach, double refrigerant_par[]) {
    // Try to allocate memory for vector structure
	//
    Refrigerant *retRefrigerant = (Refrigerant *) malloc(sizeof(Refrigerant));

    if (retRefrigerant == NULL) {
		// Not enough memory available for vector structure
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nCannot allocate memory for Refrigerant-struct.");
        return NULL;
	}

	// Initialize all function pointers with NULL because NULL is used by
	// functions of Adsorption-struct and WorkingPair-struct to check if
	// refrigerant function is implemented or not
	//
	retRefrigerant->psat_T = NULL;
	retRefrigerant->Tsat_p = NULL;
	retRefrigerant->dpsat_dT = NULL;
	retRefrigerant->rho_l_T = NULL;
	retRefrigerant->drho_l_dT = NULL;

	retRefrigerant->other_psat_T = NULL;
	retRefrigerant->other_Tsat_p = NULL;
	retRefrigerant->other_dpsat_dT = NULL;
	retRefrigerant->cubic_eos_parameters = NULL;
	retRefrigerant->cubic_gen_eos_parameters = NULL;
	retRefrigerant->cubic_fug_coefficient = NULL;

	// Set function pointers for vapor pressure
	//
	if (strcmp(vaporPressure_approach, "EoS_vaporPressure") == 0) {
		retRefrigerant->other_psat_T = &refrigerant_p_sat;
		retRefrigerant->other_Tsat_p = &refrigerant_T_sat;
		retRefrigerant->other_dpsat_dT = &refrigerant_dp_sat_dT;

		retRefrigerant->psat_T = &refrigerant_other_p_sat;
		retRefrigerant->Tsat_p = &refrigerant_other_T_sat;
		retRefrigerant->dpsat_dT = &refrigerant_other_dp_sat_dT;

	} else if (strcmp(vaporPressure_approach, "Antoine") == 0) {
		retRefrigerant->other_psat_T = &refrigerant_p_sat_antoine;
		retRefrigerant->other_Tsat_p = &refrigerant_T_sat_antoine;
		retRefrigerant->other_dpsat_dT = &refrigerant_dp_sat_dT_antoine;

		retRefrigerant->psat_T = &refrigerant_other_p_sat;
		retRefrigerant->Tsat_p = &refrigerant_other_T_sat;
		retRefrigerant->dpsat_dT = &refrigerant_other_dp_sat_dT;

	} else if (strcmp(vaporPressure_approach, "EoS_cubic") == 0) {
		// Check if parameters are given
		//		
		if (refrigerant_par == NULL) {
			// Parameters are not given
			//
			printf("\n\n###########\n# Warning #\n###########");
			printf("\nParameters for saturated vapor pressure are not given. "
				"Therefore, cannot select and execute cubic equation of "
				"state!");
			free(retRefrigerant);
			return NULL;
		}
		
		// Switch parameter 'EoS' to identify kind of cubic equation of state:
		// Set internal functions according to EoS
		//
		if (refrigerant_par[0] < -7) {
			// Soave-Redlich-Kwong equation of state with custom alpha equation
			//
			retRefrigerant->cubic_eos_parameters =
				&refrigerant_cubic_SRKE_custom_parameters;
			retRefrigerant->cubic_gen_eos_parameters =
				&refrigerant_cubic_SRKE_gen_parameters;
			retRefrigerant->cubic_fug_coefficient =
				&refrigerant_cubic_SRKE_fugacity_coefficient_pure;

		} else if (refrigerant_par[0] < -2) {
			// Soave-Redlich-Kwong equation of state
			//
			retRefrigerant->cubic_eos_parameters =
				&refrigerant_cubic_SRKE_parameters;
			retRefrigerant->cubic_gen_eos_parameters =
				&refrigerant_cubic_SRKE_gen_parameters;
			retRefrigerant->cubic_fug_coefficient =
				&refrigerant_cubic_SRKE_fugacity_coefficient_pure;

		} else if (refrigerant_par[0] < 7) {
			// Peng-Robinson equation of state with custom alpha equation
			//
			retRefrigerant->cubic_eos_parameters =
				&refrigerant_cubic_PRE_custom_parameters;
			retRefrigerant->cubic_gen_eos_parameters =
				&refrigerant_cubic_PRE_gen_parameters;
			retRefrigerant->cubic_fug_coefficient =
				&refrigerant_cubic_PRE_fugacity_coefficient_pure;

		} else if (refrigerant_par[0] < 12) {
			// Peng-Robinson equation of state
			//
			retRefrigerant->cubic_eos_parameters =
				&refrigerant_cubic_PRE_parameters;
			retRefrigerant->cubic_gen_eos_parameters =
				&refrigerant_cubic_PRE_gen_parameters;
			retRefrigerant->cubic_fug_coefficient =
				&refrigerant_cubic_PRE_fugacity_coefficient_pure;

		} else {
			// Peng-Robinson-Stryjek-Vera equation of state
			//
			retRefrigerant->cubic_eos_parameters =
				&refrigerant_cubic_PRSVE_parameters;
			retRefrigerant->cubic_gen_eos_parameters =
				&refrigerant_cubic_PRSVE_gen_parameters;
			retRefrigerant->cubic_fug_coefficient =
				&refrigerant_cubic_PRSVE_fugacity_coefficient_pure;

		}

		retRefrigerant->psat_T = &refrigerant_cubic_p_sat;
		retRefrigerant->Tsat_p = &refrigerant_cubic_T_sat;
		retRefrigerant->dpsat_dT = &refrigerant_cubic_dp_sat_dT;

	} else {
		// Calculation approach does not exist. Therefore, throw warning.
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nChosen approach \"%s\" does not exist for vapor pressure.",
			vaporPressure_approach);

	}

	// Set function pointers for saturated liquid density
	//
	if (strcmp(saturatedLiquidDensity_approach,
		"EoS_saturatedLiquidDensity") == 0) {
		retRefrigerant->rho_l_T = &refrigerant_rho_l;
		retRefrigerant->drho_l_dT = &refrigerant_drho_l_dT;

	} else {
		// Calculation approach does not exist. Therefore, throw warning.
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nChosen approach \"%s\" does not exist for saturated liquid "
			"density.", saturatedLiquidDensity_approach);

	}

	// Return structure
	//
	return retRefrigerant;
}


/*
 * delRefrigerant:
 * ---------------
 *
 * Destructor function to free allocated memory of a Refrigerant-struct.
 *
 * Parameters:
 * -----------
 * 	struct *Refrigerant
 *		Pointer of Refrigerant-struct.
 *
 * History:
 * --------
 *	01/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
void delRefrigerant(void *refrigerant) {
	// Typecast void pointer given as input to pointer of Refrigerant-struct
	//
	Refrigerant *retRefrigerant = (Refrigerant *) refrigerant;

    // Because of initialization function, refrigerant is NULL or fully built
	//
    if (retRefrigerant != NULL) {
		free(retRefrigerant);
    }
}


/*
 * refrigerant_other_p_sat:
 * ------------------------
 *
 * Calculates vapor pressure in Pa depending on saturation temperature T in K
 * not using cubic equation of state.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Saturation temperature in K.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for refrigerant function.
 * 	struct *Refrigerant
 *		Pointer of Refrigerant-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Vapor pressure in Pa.
 *
 * Remarks:
 * --------
 *	Due to initialization procedure of Refrigerant-struct, this function is only
 *	callable when Refrigerant-struct and sub-functions exist. Thus, no further
 *	checks are necessary.
 *	This function is valid for all approaches except for cubic equation of
 *	states.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_other_p_sat(double T_K, double refrigerant_par[],
	void *refrigerant) {
	// Typecast void pointer given as input to pointer of Refrigerant-struct
	//
	Refrigerant *retRefrigerant = (Refrigerant *) refrigerant;

    // Because of initialization function, refrigerant is fully built
	//
	return retRefrigerant->other_psat_T(T_K, refrigerant_par);
}


/*
 * refrigerant_other_T_sat:
 * ------------------------
 *
 * Calculates saturation temperature in K depending on vapor pressure in Pa
 * not using cubic equation of state.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Vapor pressure in Pa.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for refrigerant function.
 * 	struct *Refrigerant
 *		Pointer of Refrigerant-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Saturation temperature in K.
 *
 * Remarks:
 * --------
 *	Due to initialization procedure of Refrigerant-struct, this function is only
 *	callable when Refrigerant-struct and sub-functions exist. Thus, no further
 *	checks are necessary.
 *	This function is valid for all approaches except for cubic equation of
 *	states.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_other_T_sat(double p_Pa, double refrigerant_par[],
	void *refrigerant) {
	// Typecast void pointer given as input to pointer of Refrigerant-struct
	//
	Refrigerant *retRefrigerant = (Refrigerant *) refrigerant;

    // Because of initialization function, refrigerant is fully built
	//
	return retRefrigerant->other_Tsat_p(p_Pa, refrigerant_par);
}


/*
 * refrigerant_other_dp_sat_dT:
 * ----------------------------
 *
 * Calculates derivative of vapor pressure wrt. temperature in Pa/K depending
 * on saturation temperature T in K not using cubic equation of state.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Saturation temperature in K.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for refrigerant function.
 * 	struct *Refrigerant
 *		Pointer of Refrigerant-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of vapor pressure wrt. temperature in Pa/K.
 *
 * Remarks:
 * --------
 *	Due to initialization procedure of Refrigerant-struct, this function is only
 *	callable when Refrigerant-struct and sub-functions exist. Thus, no further
 *	checks are necessary.
 *	This function is valid for all approaches except for cubic equation of
 *	states.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_other_dp_sat_dT(double T_K, double refrigerant_par[],
	void *refrigerant) {
	// Typecast void pointer given as input to pointer of Refrigerant-struct
	//
	Refrigerant *retRefrigerant = (Refrigerant *) refrigerant;

    // Because of initialization function, refrigerant is fully built
	//
	return retRefrigerant->other_dpsat_dT(T_K, refrigerant_par);
}


/*
 * refrigerant_cubic_p_sat:
 * ------------------------
 *
 * Calculates vapor pressure in Pa depending on saturation temperature T in K
 * using cubic equation of state.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Saturation temperature in K.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for cubic equation of state.
 * 	struct *Refrigerant
 *		Pointer of Refrigerant-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Vapor pressure in Pa.
 *
 * Remarks:
 * --------
 *	Due to initialization procedure of Refrigerant-struct, this function is only
 *	callable when Refrigerant-struct and sub-functions exist. Thus, no further
 *	checks are necessary.
 *	This function is only valid for cubic equation of states.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_cubic_p_sat(double T_K, double refrigerant_par[],
	void *refrigerant) {
	// Typecast void pointer given as input to pointer of Refrigerant-struct
	//
	Refrigerant *retRefrigerant = (Refrigerant *) refrigerant;

    // Because of initialization function, refrigerant is fully built
	//
	return refrigerant_p_sat_cubic(T_K, retRefrigerant->cubic_eos_parameters,
	retRefrigerant->cubic_gen_eos_parameters,
	retRefrigerant->cubic_fug_coefficient, refrigerant_par);
}


/*
 * refrigerant_cubic_T_sat:
 * ------------------------
 *
 * Calculates saturation temperature in K depending on vapor pressure in Pa
 * using cubic equation of state.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Vapor pressure in Pa.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for cubic equation of state.
 * 	struct *Refrigerant
 *		Pointer of Refrigerant-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Saturation temperature in K.
 *
 * Remarks:
 * --------
 *	Due to initialization procedure of Refrigerant-struct, this function is only
 *	callable when Refrigerant-struct and sub-functions exist. Thus, no further
 *	checks are necessary.
 *	This function is only valid for cubic equation of states.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_cubic_T_sat(double p_Pa, double refrigerant_par[],
	void *refrigerant) {
	// Typecast void pointer given as input to pointer of Refrigerant-struct
	//
	Refrigerant *retRefrigerant = (Refrigerant *) refrigerant;

    // Because of initialization function, refrigerant is fully built
	//
	return refrigerant_T_sat_cubic(p_Pa, retRefrigerant->cubic_eos_parameters,
	retRefrigerant->cubic_gen_eos_parameters,
	retRefrigerant->cubic_fug_coefficient, refrigerant_par);
}


/*
 * refrigerant_cubic_dp_sat_dT:
 * ----------------------------
 *
 * Calculates derivative of vapor pressure wrt. temperature in Pa/K depending
 * on saturation temperature T in K using cubic equation of state.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Saturation temperature in K.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for cubic equation of state.
 * 	struct *Refrigerant
 *		Pointer of Refrigerant-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of vapor pressure wrt. temperature in Pa/K.
 *
 * Remarks:
 * --------
 *	Due to initialization procedure of Refrigerant-struct, this function is only
 *	callable when Refrigerant-struct and sub-functions exist. Thus, no further
 *	checks are necessary.
 *	This function is only valid for cubic equation of states.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_cubic_dp_sat_dT(double T_K, double refrigerant_par[],
	void *refrigerant) {
	// Typecast void pointer given as input to pointer of Refrigerant-struct
	//
	Refrigerant *retRefrigerant = (Refrigerant *) refrigerant;

    // Because of initialization function, refrigerant is fully built
	//
	return refrigerant_dp_sat_dT_cubic(T_K, retRefrigerant->cubic_eos_parameters,
	retRefrigerant->cubic_gen_eos_parameters,
	retRefrigerant->cubic_fug_coefficient, refrigerant_par);
}
