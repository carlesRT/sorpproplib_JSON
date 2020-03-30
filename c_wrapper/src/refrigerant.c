///////////////////
// refrigerant.c //
///////////////////
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "refrigerant.h"
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
 *
 */
Refrigerant *newRefrigerant(const char *vaporPressure_approach,
	const char *saturatedLiquidDensity_approach) {
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

	// Set function pointers for vapor pressure
	//
	if (strcmp(vaporPressure_approach, "EoS_vaporPressure") == 0) {
		retRefrigerant->psat_T = &refrigerant_p_sat;
		retRefrigerant->Tsat_p = &refrigerant_T_sat;
		retRefrigerant->dpsat_dT = &refrigerant_dp_sat_dT;

	} else if (strcmp(vaporPressure_approach, "Antoine") == 0) {
		retRefrigerant->psat_T = &refrigerant_p_sat_antoine;
		retRefrigerant->Tsat_p = &refrigerant_T_sat_antoine;
		retRefrigerant->dpsat_dT = &refrigerant_dp_sat_dT_antoine;

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
