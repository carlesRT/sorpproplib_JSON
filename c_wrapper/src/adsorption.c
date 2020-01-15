//////////////////
// adsorption.c //
//////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "adsorption.h"
#include "adsorption_dualSiteSips.h"
#include "adsorption_dubininAstakhov.h"	
#include "adsorption_langmuir.h"
#include "adsorption_toth.h"
#include "refrigerant.h"

#include "structDefinitions.c"


/////////////////////////////
// Definition of functions //
/////////////////////////////
/*
 * newAdsorption:
 * --------------
 *
 * Initialisation function to create a new Adsorption-struct. Therefore,
 * function checks first if enough memory available to allocate adsorption 
 * struct and then set function pointers within the struct depending on the 
 * isotherm type that is given as input. If memory allocation fails, functions 
 * returns NULL and prints a warning.
 *
 * Parameters:
 * -----------
 * 	const char *isotherm_type:
 *		Name of isotherm type.
 *
 * Returns:
 * --------
 *	struct *Adsorption:
 *		Returns malloc-ed Adsorption-struct that contains function pointers
 *		reffering to functions of a specific isotherm that is chosen by
 *		parameter isotherm_type.
 *
 * History:
 * --------
 *	01/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
Adsorption *newAdsorption(const char *isotherm_type) {
    // Try to allocate memory for vector structure
	//
    Adsorption *retAdsorption = (Adsorption *) malloc(sizeof(Adsorption));
	
    if (retAdsorption == NULL) {
		// Not enough memory available for vector structure
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nCannot allocate memory for Adsorption-structure.");
        return NULL;
	}
	
	// Save general information of isotherm
	//
	retAdsorption->isotherm_type = isotherm_type;
		
	// Initialize all function pointers with NULL because NULL is used by
	// functions of WorkingPair-struct to check if isotherm function is
	// implemented or not
	//
	retAdsorption->w_pT = NULL;
	retAdsorption->p_wT = NULL;
	retAdsorption->T_pw = NULL;	
	retAdsorption->dw_dp_pT = NULL;
	retAdsorption->dw_dT_pT = NULL;
	retAdsorption->dp_dw_wT = NULL;
	retAdsorption->dp_dT_wT = NULL;
	retAdsorption->piStar_pyxgTM = NULL;
	
	retAdsorption->sur_w_pT = NULL;
	retAdsorption->sur_p_wT = NULL;
	retAdsorption->sur_T_pw = NULL;	
	retAdsorption->sur_dw_dp_pT = NULL;
	retAdsorption->sur_dw_dT_pT = NULL;
	retAdsorption->sur_dp_dw_wT = NULL;
	retAdsorption->sur_dp_dT_wT = NULL;
	retAdsorption->sur_piStar_pyxgTM = NULL;
	
	retAdsorption->vol_W_ARho = NULL;
	retAdsorption->vol_dW_dA_ARho = NULL;
	retAdsorption->vol_A_WRho = NULL;	
	retAdsorption->vol_dA_dW_WRho = NULL;	
	retAdsorption->vol_w_pTpsatRho = NULL;
	retAdsorption->vol_dw_dp_pTpsatRho = NULL;
	retAdsorption->vol_dw_dT_pTpsatRho = NULL;
	retAdsorption->vol_p_wTpsatRho = NULL;
	retAdsorption->vol_dp_dw_wTpsatRho = NULL;
	retAdsorption->vol_dp_dT_wTpsatRho = NULL;
	retAdsorption->vol_T_pwpsatRho = NULL;
	retAdsorption->vol_piStar_pyxgTpsatRhoM = NULL;
		
    // Set function pointers of struct to correct isotherm functions depending
	// on the isotherm type.
	//
	if (strcmp(isotherm_type, "dss") == 0) {
		// Set function pointers to functions of Dual-Site-Sips isotherm
		//
		retAdsorption->sur_w_pT = &adsorption_dualSiteSips_w_pT;
		retAdsorption->sur_p_wT = &adsorption_dualSiteSips_p_wT;
		retAdsorption->sur_T_pw = &adsorption_dualSiteSips_T_pw;
		
		retAdsorption->sur_dw_dp_pT = &adsorption_dualSiteSips_dw_dp_pT;
		retAdsorption->sur_dw_dT_pT = &adsorption_dualSiteSips_dw_dT_pT;		
		retAdsorption->sur_dp_dw_wT = &adsorption_dualSiteSips_dp_dw_wT;
		retAdsorption->sur_dp_dT_wT = &adsorption_dualSiteSips_dp_dT_wT;	
		
		retAdsorption->sur_piStar_pyxgTM =
			&adsorption_dualSiteSips_piStar_pyxgTM;	
		
		// Set function pointers that are always defined
		//
		init_surFunctions(retAdsorption);
		
		// Return structure
		//
		return retAdsorption;
	  
	} else if (strcmp(isotherm_type, "langmuir") == 0) {
		// Set function pointers to functions of Langmuir isotherm
		//
		retAdsorption->sur_w_pT = &adsorption_langmuir_w_pT;
		retAdsorption->sur_p_wT = &adsorption_langmuir_p_wT;
		retAdsorption->sur_T_pw = &adsorption_langmuir_T_pw;
		
		retAdsorption->sur_dw_dp_pT = &adsorption_langmuir_dw_dp_pT;
		retAdsorption->sur_dw_dT_pT = &adsorption_langmuir_dw_dT_pT;		
		retAdsorption->sur_dp_dw_wT = &adsorption_langmuir_dp_dw_wT;
		retAdsorption->sur_dp_dT_wT = &adsorption_langmuir_dp_dT_wT;	
		
		retAdsorption->sur_piStar_pyxgTM = &adsorption_langmuir_piStar_pyxgTM;
		
		// Set function pointers that are always defined
		//
		init_surFunctions(retAdsorption);
				
		// Return structure
		//
		return retAdsorption;
	  
	} else if (strcmp(isotherm_type, "toth") == 0) {
		// Set function pointers to functions of Toth isotherm
		//
		retAdsorption->sur_w_pT = &adsorption_toth_w_pT;
		retAdsorption->sur_p_wT = &adsorption_toth_p_wT;
		retAdsorption->sur_T_pw = &adsorption_toth_T_pw;
		
		retAdsorption->sur_dw_dp_pT = &adsorption_toth_dw_dp_pT;
		retAdsorption->sur_dw_dT_pT = &adsorption_toth_dw_dT_pT;		
		retAdsorption->sur_dp_dw_wT = &adsorption_toth_dp_dw_wT;
		retAdsorption->sur_dp_dT_wT = &adsorption_toth_dp_dT_wT;	
		
		retAdsorption->sur_piStar_pyxgTM = &adsorption_toth_piStar_pyxgTM;	
		
		// Set function pointers that are always defined
		//
		init_surFunctions(retAdsorption);
				
		// Return structure
		//
		return retAdsorption;
	  
	} else if (strcmp(isotherm_type, "dubinin-astakov") == 0) {
		// Set function pointers to functions of Dubinin-Astakhov isotherm
		//
		retAdsorption->vol_W_ARho = &adsorption_dubininAstakhov_W_ARho;
		retAdsorption->vol_A_WRho = &adsorption_dubininAstakhov_A_WRho;		
		retAdsorption->vol_dW_dA_ARho = &adsorption_dubininAstakhov_dW_dA_ARho;
		retAdsorption->vol_dA_dW_WRho = &adsorption_dubininAstakhov_dA_dW_WRho;
			
		retAdsorption->vol_w_pTpsatRho =
			&adsorption_dubininAstakhov_w_pTpsatRho;
		retAdsorption->vol_p_wTpsatRho =
			&adsorption_dubininAstakhov_p_wTpsatRho;
		retAdsorption->vol_T_pwpsatRho =
			&adsorption_dubininAstakhov_T_pwpsatRho;		
		retAdsorption->vol_dw_dp_pTpsatRho =
			&adsorption_dubininAstakhov_dw_dp_pTpsatRho;
		retAdsorption->vol_dw_dT_pTpsatRho =
			&adsorption_dubininAstakhov_dw_dT_pTpsatRho;		
		retAdsorption->vol_dp_dw_wTpsatRho =
			&adsorption_dubininAstakhov_dp_dw_wTpsatRho;
		retAdsorption->vol_dp_dT_wTpsatRho =
			&adsorption_dubininAstakhov_dp_dT_wTpsatRho;	
		
		retAdsorption->vol_piStar_pyxgTpsatRhoM =
			&adsorption_dubininAstakhov_piStar_pyxgTpsatRhoM;	
		
		// Set function pointers that are always defined
		//
		init_volFunctions(retAdsorption);
				
		// Return structure
		//
		return retAdsorption;
		
	} else {
		// Isotherm does not exist. Therefore, throw warning and free memory.
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nChosen isotherm type \"%s\" does not exist.",
			isotherm_type);
		printf("\nCannot create Adsorption-structure.");
		
		free(retAdsorption);			
		return NULL;	
	}
}


/*
 * delAdsorption:
 * --------------
 *
 * Destructor function to free allocated memory of a Adsorption-struct.
 *
 * Parameters:
 * -----------
 * 	struct *Adsorption:
 *		Pointer of Adsorption-struct.
 *
 * History:
 * --------
 *	01/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
void delAdsorption(void *adsorption) {
	// Typecast void pointer given as input to pointer of Adsorption-struct
	//
	Adsorption *retAdsorption = (Adsorption *) adsorption;
	
    // Because of initialization function, Adsorption-struct is NULL or fully 
	// built
	//
    if (retAdsorption != NULL) {
		free(retAdsorption);
    }
}


/*
 * init_surFunctions:
 * ------------------
 *
 * Sets function pointers to isotherm functions that are always defined for
 * isotherms based on the surface approach. Function is only used during
 * initialisation.
 *
 * Parameters:
 * -----------
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer of isotherm
 *		functions.
 *
 * History:
 * --------
 *	09/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
void init_surFunctions(void *adsorption) {
	// Typecast void pointer given as input to pointer of Adsorption-struct
	//
	Adsorption *retAdsorption = (Adsorption *) adsorption;
	
	if (retAdsorption == NULL) {
		// Adsorption-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nAdsorption-struct does not exist.");
		printf("\nCannot run function \"init_surFunctions\".");
		
	} else {
		// Set function pointers
		//
		retAdsorption->w_pT = w_pT_sur;
		retAdsorption->p_wT = p_wT_sur;
		retAdsorption->T_pw = T_pw_sur;		
		retAdsorption->dw_dp_pT = dw_dp_pT_sur;
		retAdsorption->dw_dT_pT = dw_dT_pT_sur;
		retAdsorption->dp_dw_wT = dp_dw_wT_sur;
		retAdsorption->dp_dT_wT = dp_dT_wT_sur;
		retAdsorption->piStar_pyxgTM = piStar_pyxgTM_sur;
	}
}


/*
 * init_volFunctions:
 * ------------------
 *
 * Sets function pointers to isotherm functions that are always defined for
 * isotherms based on the volume approach. Function is only used during
 * initialisation.
 *
 * Parameters:
 * -----------
 *	struct *Adsorption:
 *		Pointer to "Adsorption-struct that contains pointer of isotherm
 *		functions.
 *
 * History:
 * --------
 *	09/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
void init_volFunctions(void *adsorption) {
	// Typecast void pointer given as input to pointer of Adsorption-struct
	//
	Adsorption *retAdsorption = (Adsorption *) adsorption;
	
	if (retAdsorption == NULL) {
		// Adsorption-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nAdsorption-struct does not exist.");
		printf("\nCannot run function \"init_volFunctions\".");
		
	} else {
		// Set function pointers
		//
		retAdsorption->w_pT = w_pT_vol;
		retAdsorption->p_wT = p_wT_vol;
		retAdsorption->T_pw = T_pw_vol;		
		retAdsorption->dw_dp_pT = dw_dp_pT_vol;
		retAdsorption->dw_dT_pT = dw_dT_pT_vol;
		retAdsorption->dp_dw_wT = dp_dw_wT_vol;
		retAdsorption->dp_dT_wT = dp_dT_wT_vol;
		retAdsorption->piStar_pyxgTM = piStar_pyxgTM_vol;	
	}
}


/*
 * w_pT_sur:
 * ---------
 *
 * Calculates equilibrium loading in w kg/kg depending on equilibrium pressure p
 * in Pa and equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapour pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer of isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid porperties at saturated state.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium loading in kg/kg.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate loading depending on pressure and
 *	temperature when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for surface approach, fluid properties at saturated state
 *	are not required and thus ignored.
 *
 * History:
 * --------
 *	08/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double w_pT_sur(double p_Pa, double T_K, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant) {
	// Typecast void pointers given as inputs to correct structs
	//
	Adsorption *retAdsorption = (Adsorption *) adsorption;
	Refrigerant *retRefrigerant = (Refrigerant *) refrigerant;
	
	// Check if structures do exists and if functions are implemented
	//	
	if (retAdsorption == NULL) {
		// Adsorption-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nAdsorption-struct does not exist.");
		printf("\nReturn -1 for function call \"w_pT_sur\".");
		return -1;
	}
	if (retRefrigerant == NULL) {
		// Refrigerant-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nRefrigerant-struct does not exist.");
		printf("\nReturn -1 for function call \"w_pT_sur\".");
		return -1;
	}		
	if (retAdsorption->sur_w_pT == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for sur_w_pT or its coefficients do not exist.");
		printf("\nReturn -1 for function call \"w_pT_sur\".");
		return -1;
	}
	
	// Avoid compiler warning C4100 by adding "do nothing" expression
	// "Do nothing" expression will be optimised out by compiler
	//
	(p_sat_par) = (p_sat_par);
	(rho_l_par) = (rho_l_par);
	
	// Return directly result of isotherm function
	//
	return retAdsorption->sur_w_pT(p_Pa, T_K, isotherm_par);
}


/*
 * w_pT_vol:
 * ---------
 *
 * Calculates equilibrium loading w in kg/kg depending on equilibrium pressure p
 * in Pa and equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapour pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer of isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid porperties at saturated state.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium loading in kg/kg.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate loading depending on pressure and
 *	temperature when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for volumetric approach, fluid properties at saturated state
 *	are required.
 *
 * History:
 * --------
 *	08/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double w_pT_vol(double p_Pa, double T_K, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant) {
	// Typecast void pointers given as inputs to correct structs
	//
	Adsorption *retAdsorption = (Adsorption *) adsorption;
	Refrigerant *retRefrigerant = (Refrigerant *) refrigerant;
	
	// Check if structures do exists and if functions are implemented
	//	
	if (retAdsorption == NULL) {
		// Adsorption-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nAdsorption-struct does not exist.");
		printf("\nReturn -1 for function call \"w_pT_vol\".");
		return -1;
	}
	if (retRefrigerant == NULL) {
		// Refrigerant-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nRefrigerant-struct does not exist.");
		printf("\nReturn -1 for function call \"w_pT_vol\".");
		return -1;
	}	
	if (retRefrigerant->psat_T == NULL || p_sat_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for p_sat_T or its coefficients do not exist.");
		printf("\nReturn -1 for function call \"w_pT_vol\".");
		return -1;
	}
	if (retRefrigerant->rho_l_T == NULL || rho_l_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for rho_l_T or its coefficients do not exist.");
		printf("\nReturn -1 for function call \"w_pT_vol\".");
		return -1;
	}
	if (retAdsorption->vol_w_pTpsatRho == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for vol_w_pTpsatRho or its coefficients do not exist.");
		printf("\nReturn -1 for function call \"w_pT_vol\".");
		return -1;
	}
	
	// Calculate vapour pressure and saturated liquid of adsorpt
	//
	double p_sat_Pa = retRefrigerant->psat_T(T_K, p_sat_par);
	double rho_l_kgm3 = retRefrigerant->rho_l_T(T_K, rho_l_par);
	
	// Return result of isotherm function
	//
	return retAdsorption->vol_w_pTpsatRho(p_Pa, T_K, p_sat_Pa,
		rho_l_kgm3, isotherm_par);
}


/*
 * p_wT_sur:
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
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapour pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer of isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid porperties at saturated state.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium pressure in Pa.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate pressure depending on loading and
 *	temperature when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for surface approach, fluid properties at saturated state
 *	are not required and thus ignored.
 *
 * History:
 * --------
 *	08/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double p_wT_sur(double w_kgkg, double T_K, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant) {
	// Typecast void pointers given as inputs to correct structs
	//
	Adsorption *retAdsorption = (Adsorption *) adsorption;
	Refrigerant *retRefrigerant = (Refrigerant *) refrigerant;
	
	// Check if structures do exists and if functions are implemented
	//	
	if (retAdsorption == NULL) {
		// Adsorption-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nAdsorption-struct does not exist.");
		printf("\nReturn -1 for function call \"p_wT_sur\".");
		return -1;
	}
	if (retRefrigerant == NULL) {
		// Refrigerant-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nRefrigerant-struct does not exist.");
		printf("\nReturn -1 for function call \"p_wT_sur\".");
		return -1;
	}	
	if (retAdsorption->sur_p_wT == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for sur_p_wT or its coefficients do not exist.");
		printf("\nReturn -1 for function call \"p_wT_sur\".");
		return -1;
	}
	
	// Avoid compiler warning C4100 by adding "do nothing" expression
	// "Do nothing" expression will be optimised out by compiler
	//
	(p_sat_par) = (p_sat_par);
	(rho_l_par) = (rho_l_par);
	
	// Return directly result of isotherm function
	//
	return retAdsorption->sur_p_wT(w_kgkg, T_K, isotherm_par);
}


/*
 * p_wT_vol:
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
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapour pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer to isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid porperties at saturated state.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium pressure in Pa.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate pressure depending on loading and
 *	temperature when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for volumetric approach, fluid properties at saturated state
 *	are required.
 *
 * History:
 * --------
 *	08/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double p_wT_vol(double w_kgkg, double T_K, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant) {
	// Typecast void pointers given as inputs to correct structs
	//
	Adsorption *retAdsorption = (Adsorption *) adsorption;
	Refrigerant *retRefrigerant = (Refrigerant *) refrigerant;
	
	// Check if structures do exists and if functions are implemented
	//	
	if (retAdsorption == NULL) {
		// Adsorption-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nAdsorption-struct does not exist.");
		printf("\nReturn -1 for function call \"p_wT_vol\".");
		return -1;
	}
	if (retRefrigerant == NULL) {
		// Refrigerant-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nRefrigerant-struct does not exist.");
		printf("\nReturn -1 for function call \"p_wT_vol\".");
		return -1;
	}	
	if (retRefrigerant->psat_T == NULL || p_sat_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for p_sat_T or its coefficients do not exist.");
		printf("\nReturn -1 for function call \"p_wT_vol\".");
		return -1;
	}
	if (retRefrigerant->rho_l_T == NULL || rho_l_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for rho_l_T or its coefficients do not exist.");
		printf("\nReturn -1 for function call \"p_wT_vol\".");
		return -1;
	}
	if (retAdsorption->vol_p_wTpsatRho == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for vol_p_wTpsatRho do not exist.");
		printf("\nReturn -1 for function call \"p_wT_vol\".");
		return -1;
	}
	
	// Calculate vapour pressure and saturated liquid of adsorpt
	//
	double p_sat_Pa = retRefrigerant->psat_T(T_K, p_sat_par);
	double rho_l_kgm3 = retRefrigerant->rho_l_T(T_K, rho_l_par);
	
	// Return result of isotherm function
	//
	return retAdsorption->vol_p_wTpsatRho(w_kgkg, T_K, p_sat_Pa,
		rho_l_kgm3, isotherm_par);
}


/*
 * T_pw_sur:
 * ---------
 *
 * Calculates equilibrium temperature T in K depending on equilibrium pressure p
 * in Pa and equilibrium loading w in kg/kg.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapour pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer to isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid porperties at saturated state.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium pressure in Pa.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate temperature depending on pressure
 *	and loading when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for surface approach, fluid properties at saturated state
 *	are not required and thus ignored.
 *
 * History:
 * --------
 *	08/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double T_pw_sur(double p_Pa, double w_kgkg, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant) {
	// Typecast void pointers given as inputs to correct structs
	//
	Adsorption *retAdsorption = (Adsorption *) adsorption;
	Refrigerant *retRefrigerant = (Refrigerant *) refrigerant;
	
	// Check if structures do exists and if functions are implemented
	//	
	if (retAdsorption == NULL) {
		// Adsorption-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nAdsorption-struct does not exist.");
		printf("\nReturn -1 for function call \"T_pw_sur\".");
		return -1;
	}
	if (retRefrigerant == NULL) {
		// Refrigerant-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nRefrigerant-struct does not exist.");
		printf("\nReturn -1 for function call \"T_pw_sur\".");
		return -1;
	}	
	if (retAdsorption->sur_T_pw == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for sur_T_pw or its coefficients do not exist.");
		printf("\nReturn -1 for function call \"T_pw_sur\".");
		return -1;
	}
	
	// Avoid compiler warning C4100 by adding "do nothing" expression
	// "Do nothing" expression will be optimised out by compiler
	//
	(p_sat_par) = (p_sat_par);
	(rho_l_par) = (rho_l_par);
	
	// Return directly result of isotherm function
	//
	return retAdsorption->sur_T_pw(p_Pa, w_kgkg, isotherm_par);
}


/*
 * T_pw_vol:
 * ---------
 *
 * Calculates equilibrium temperature T in K depending on equilibrium pressure p
 * in Pa and equilibrium loading w in kg/kg.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapour pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer to isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid porperties at saturated state.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium pressure in Pa.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate temperature depending on pressure
 *	and loading when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for volumetric approach, fluid properties at saturated state
 *	are required.
 *	Function uses equation of states implemented for calculation of vapour
 *	pressure, saturated liquid density, derivative of vapour pressure wrt.
 *	temperature, and derivative of saturated liquid density wrt. temperature
 *	when isothem of chosen working pair is based on volumetric approach (e.g.
 *	Dubinin theory). 
 *
 * History:
 * --------
 *	08/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double T_pw_vol(double p_Pa, double w_kgkg, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant) {
	// Typecast void pointers given as inputs to correct structs
	//
	Adsorption *retAdsorption = (Adsorption *) adsorption;
	Refrigerant *retRefrigerant = (Refrigerant *) refrigerant;
	
	// Check if structures do exists and if functions are implemented
	//	
	if (retAdsorption == NULL) {
		// Adsorption-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nAdsorption-struct does not exist.");
		printf("\nReturn -1 for function call \"T_pw_vol\".");
		return -1;
	}
	if (retRefrigerant == NULL) {
		// Refrigerant-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nRefrigerant-struct does not exist.");
		printf("\nReturn -1 for function call \"T_pw_vol\".");
		return -1;
	}	
	if (retRefrigerant->psat_T == NULL || p_sat_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for p_sat_T or its coefficients do not exist.");
		printf("\nReturn -1 for function call \"T_pw_vol\".");
		return -1;
	}
	if (retRefrigerant->rho_l_T == NULL || rho_l_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for rho_l_T or its coefficients do not exist.");
		printf("\nReturn -1 for function call \"T_pw_vol\".");
		return -1;
	}
	if (retRefrigerant->dpsat_dT == NULL || p_sat_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for dpsat_dT or its coefficients do not exist.");
		printf("\nReturn -1 for function call \"T_pw_vol\".");
		return -1;
	}
	if (retRefrigerant->drho_l_dT == NULL || rho_l_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for drho_l_dT or its coefficients do not exist.");
		printf("\nReturn -1 for function call \"T_pw_vol\".");
		return -1;
	}
	if (retAdsorption->vol_T_pwpsatRho == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for vol_T_pwpsatRho do not exist.");
		printf("\nReturn -1 for function call \"T_pw_vol\".");
		return -1;
	}
	
	// Return result of isotherm function
	//
	return retAdsorption->vol_T_pwpsatRho(p_Pa, w_kgkg,
		retRefrigerant->psat_T,
		retRefrigerant->rho_l_T,
		retRefrigerant->dpsat_dT,
		retRefrigerant->drho_l_dT,
		isotherm_par, p_sat_par, rho_l_par);
}
	
	
/*
 * dw_dp_pT_sur:
 * -------------
 *
 * Calculates derivative of equilibrium loading with respect to pressure 
 * p in kg/kg/Pa depending on equilibrium pressure p in Pa and equilibrium 
 * temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapour pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer to isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid porperties at saturated state.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. pressure in kg/kg/Pa.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate loading depending on pressure and
 *	temperature when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for surface approach, fluid properties at saturated state
 *	are not required and thus ignored.
 *
 * History:
 * --------
 *	08/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double dw_dp_pT_sur(double p_Pa, double T_K, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant) {
	// Typecast void pointers given as inputs to correct structs
	//
	Adsorption *retAdsorption = (Adsorption *) adsorption;
	Refrigerant *retRefrigerant = (Refrigerant *) refrigerant;
	
	// Check if structures do exists and if functions are implemented
	//	
	if (retAdsorption == NULL) {
		// Adsorption-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nAdsorption-struct does not exist.");
		printf("\nReturn -1 for function call \"dw_dp_pT_sur\".");
		return -1;
	}
	if (retRefrigerant == NULL) {
		// Refrigerant-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nRefrigerant-struct does not exist.");
		printf("\nReturn -1 for function call \"dw_dp_pT_sur\".");
		return -1;
	}	
	if (retAdsorption->sur_dw_dp_pT == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for sur_dw_dp_pT or its coefficients do not exist.");
		printf("\nReturn -1 for function call \"dw_dp_pT_sur\".");
		return -1;
	}
	
	// Avoid compiler warning C4100 by adding "do nothing" expression
	// "Do nothing" expression will be optimised out by compiler
	//
	(p_sat_par) = (p_sat_par);
	(rho_l_par) = (rho_l_par);
	
	// Return directly result of isotherm function
	//
	return retAdsorption->sur_dw_dp_pT(p_Pa, T_K, isotherm_par);
}
	
	
/*
 * dw_dp_pT_vol:
 * -------------
 *
 * Calculates derivative of equilibrium loading with respect to pressure 
 * p in kg/kg/Pa depending on equilibrium pressure p in Pa and equilibrium 
 * temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapour pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer to isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid porperties at saturated state.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. pressure in kg/kg/Pa.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate loading depending on pressure and
 *	temperature when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for volumetric approach, fluid properties at saturated state
 *	are required.
 *
 * History:
 * --------
 *	08/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double dw_dp_pT_vol(double p_Pa, double T_K, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant) {
	// Typecast void pointers given as inputs to correct structs
	//
	Adsorption *retAdsorption = (Adsorption *) adsorption;
	Refrigerant *retRefrigerant = (Refrigerant *) refrigerant;
	
	// Check if structures do exists and if functions are implemented
	//	
	if (retAdsorption == NULL) {
		// Adsorption-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nAdsorption-struct does not exist.");
		printf("\nReturn -1 for function call \"dw_dp_pT_vol\".");
		return -1;
	}
	if (retRefrigerant == NULL) {
		// Refrigerant-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nRefrigerant-struct does not exist.");
		printf("\nReturn -1 for function call \"dw_dp_pT_vol\".");
		return -1;
	}	
	if (retRefrigerant->psat_T == NULL || p_sat_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for p_sat_T or its coefficients do not exist.");
		printf("\nReturn -1 for function call \"dw_dp_pT_vol\".");
		return -1;
	}
	if (retRefrigerant->rho_l_T == NULL || rho_l_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for rho_l_T or its coefficients do not exist.");
		printf("\nReturn -1 for function call \"dw_dp_pT_vol\".");
		return -1;
	}
	if (retAdsorption->vol_dw_dp_pTpsatRho == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for vol_dw_dp_pTpsatRho do not exist.");
		printf("\nReturn -1 for function call \"dw_dp_pT_vol\".");
		return -1;
	}
	
	// Calculate vapour pressure and saturated liquid of adsorpt
	//
	double p_sat_Pa = retRefrigerant->psat_T(T_K, p_sat_par);
	double rho_l_kgm3 = retRefrigerant->rho_l_T(T_K, rho_l_par);
	
	// Return result of isotherm function
	//
	return retAdsorption->vol_dw_dp_pTpsatRho(p_Pa, T_K,
		p_sat_Pa, rho_l_kgm3,isotherm_par);
}
	
	
/*
 * dw_dT_pT_sur:
 * -------------
 *
 * Calculates derivative of equilibrium loading with respect to temperature 
 * T in kg/kg/K depending on equilibrium pressure p in Pa and equilibrium 
 * temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapour pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer to isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid porperties at saturated state.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. temperature in kg/kg/K.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate loading depending on pressure and
 *	temperature when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for surface approach, fluid properties at saturated state
 *	are not required and thus ignored.
 *
 * History:
 * --------
 *	08/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double dw_dT_pT_sur(double p_Pa, double T_K, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant) {
	// Typecast void pointers given as inputs to correct structs
	//
	Adsorption *retAdsorption = (Adsorption *) adsorption;
	Refrigerant *retRefrigerant = (Refrigerant *) refrigerant;
	
	// Check if structures do exists and if functions are implemented
	//	
	if (retAdsorption == NULL) {
		// Adsorption-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nAdsorption-struct does not exist.");
		printf("\nReturn -1 for function call \"dw_dT_pT_sur\".");
		return -1;
	}
	if (retRefrigerant == NULL) {
		// Refrigerant-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nRefrigerant-struct does not exist.");
		printf("\nReturn -1 for function call \"dw_dT_pT_sur\".");
		return -1;
	}	
	if (retAdsorption->sur_dw_dT_pT == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for sur_dw_dT_pT or its coefficients do not exist.");
		printf("\nReturn -1 for function call \"dw_dT_pT_sur\".");
		return -1;
	}
	
	// Avoid compiler warning C4100 by adding "do nothing" expression
	// "Do nothing" expression will be optimised out by compiler
	//
	(p_sat_par) = (p_sat_par);
	(rho_l_par) = (rho_l_par);
	
	// Return directly result of isotherm function
	//
	return retAdsorption->sur_dw_dT_pT(p_Pa, T_K, isotherm_par);
}
	
	
/*
 * dw_dT_pT_vol:
 * -------------
 *
 * Calculates derivative of equilibrium loading with respect to temperature 
 * T in kg/kg/K depending on equilibrium pressure p in Pa and equilibrium 
 * temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapour pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer to isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid porperties at saturated state.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. temperature in kg/kg/K.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate loading depending on pressure and
 *	temperature when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for volumetric approach, fluid properties at saturated state
 *	are required.
 *
 * History:
 * --------
 *	08/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double dw_dT_pT_vol(double p_Pa, double T_K, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant) {
	// Typecast void pointers given as inputs to correct structs
	//
	Adsorption *retAdsorption = (Adsorption *) adsorption;
	Refrigerant *retRefrigerant = (Refrigerant *) refrigerant;
	
	// Check if structures do exists and if functions are implemented
	//	
	if (retAdsorption == NULL) {
		// Adsorption-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nAdsorption-struct does not exist.");
		printf("\nReturn -1 for function call \"dw_dT_pT_vol\".");
		return -1;
	}
	if (retRefrigerant == NULL) {
		// Refrigerant-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nRefrigerant-struct does not exist.");
		printf("\nReturn -1 for function call \"dw_dT_pT_vol\".");
		return -1;
	}	
	if (retRefrigerant->psat_T == NULL || p_sat_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for p_sat_T or its coefficients do not exist.");
		printf("\nReturn -1 for function call \"dw_dT_pT_vol\".");
		return -1;
	}
	if (retRefrigerant->dpsat_dT == NULL || p_sat_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for dpsat_dT or its coefficients do not exist.");
		printf("\nReturn -1 for function call \"dw_dT_pT_vol\".");
		return -1;
	}
	if (retRefrigerant->rho_l_T == NULL || rho_l_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for rho_l_T or its coefficients do not exist.");
		printf("\nReturn -1 for function call \"dw_dT_pT_vol\".");
		return -1;
	}
	if (retRefrigerant->drho_l_dT == NULL || rho_l_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for drho_l_dT or its coefficients do not exist.");
		printf("\nReturn -1 for function call \"dw_dT_pT_vol\".");
		return -1;
	}
	if (retAdsorption->vol_dw_dT_pTpsatRho == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for vol_dw_dT_pTpsatRho do not exist.");
		printf("\nReturn -1 for function call \"dw_dT_pT_vol\".");
		return -1;
	}
	
	// Calculate vapour pressure and saturated liquid of adsorpt
	//
	double p_sat_Pa = retRefrigerant->psat_T(T_K, p_sat_par);
	double dp_sat_dT_PaK = retRefrigerant->dpsat_dT(T_K, p_sat_par);
	double rho_l_kgm3 = retRefrigerant->rho_l_T(T_K, rho_l_par);
	double drho_l_dT_kgm3K = retRefrigerant->drho_l_dT(T_K, rho_l_par);
	
	// Return result of isotherm function
	//
	return retAdsorption->vol_dw_dT_pTpsatRho(p_Pa, T_K,
		p_sat_Pa, rho_l_kgm3, dp_sat_dT_PaK, drho_l_dT_kgm3K, isotherm_par);
}
	
	
/*
 * dp_dw_wT_sur:
 * -------------
 *
 * Calculates derivative of equilibrium pressure with respect to loading 
 * w in kgPa/kg depending on equilibrium loading w in kg/kg and equilibrium 
 * temperature T in K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapour pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer to isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid porperties at saturated state.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. loading in kgPa/kg.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate loading depending on pressure and
 *	temperature when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for volumetric approach, fluid properties at saturated state
 *	are required.
 *
 * History:
 * --------
 *	13/01/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double dp_dw_wT_sur(double w_kgkg, double T_K, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant) {
	// Typecast void pointers given as inputs to correct structs
	//
	Adsorption *retAdsorption = (Adsorption *) adsorption;
	Refrigerant *retRefrigerant = (Refrigerant *) refrigerant;
	
	// Check if structures do exists and if functions are implemented
	//	
	if (retAdsorption == NULL) {
		// Adsorption-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nAdsorption-struct does not exist.");
		printf("\nReturn -1 for function call \"dp_dw_wT_sur\".");
		return -1;
	}
	if (retRefrigerant == NULL) {
		// Refrigerant-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nRefrigerant-struct does not exist.");
		printf("\nReturn -1 for function call \"dp_dw_wT_sur\".");
		return -1;
	}	
	if (retAdsorption->sur_dp_dw_wT == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for sur_dp_dw_wT or its coefficients do not exist.");
		printf("\nReturn -1 for function call \"dp_dw_wT_sur\".");
		return -1;
	}
	
	// Avoid compiler warning C4100 by adding "do nothing" expression
	// "Do nothing" expression will be optimised out by compiler
	//
	(p_sat_par) = (p_sat_par);
	(rho_l_par) = (rho_l_par);
	
	// Return directly result of isotherm function
	//
	return retAdsorption->sur_dp_dw_wT(w_kgkg, T_K, isotherm_par);
}
	
	
/*
 * dp_dw_wT_vol:
 * -------------
 *
 * Calculates derivative of equilibrium pressure with respect to loading 
 * w in kgPa/kg depending on equilibrium loading w in kg/kg and equilibrium 
 * temperature T in K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapour pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer to isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid porperties at saturated state.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. loading in kgPa/kg.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate loading depending on pressure and
 *	temperature when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for surface approach, fluid properties at saturated state
 *	are not required and thus ignored.
 *
 * History:
 * --------
 *	13/01/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double dp_dw_wT_vol(double w_kgkg, double T_K, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant) {
	// Typecast void pointers given as inputs to correct structs
	//
	Adsorption *retAdsorption = (Adsorption *) adsorption;
	Refrigerant *retRefrigerant = (Refrigerant *) refrigerant;
	
	// Check if structures do exists and if functions are implemented
	//	
	if (retAdsorption == NULL) {
		// Adsorption-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nAdsorption-struct does not exist.");
		printf("\nReturn -1 for function call \"dp_dw_wT_vol\".");
		return -1;
	}
	if (retRefrigerant == NULL) {
		// Refrigerant-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nRefrigerant-struct does not exist.");
		printf("\nReturn -1 for function call \"dp_dw_wT_vol\".");
		return -1;
	}	
	if (retRefrigerant->psat_T == NULL || p_sat_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for p_sat_T or its coefficients do not exist.");
		printf("\nReturn -1 for function call \"dp_dw_wT_vol\".");
		return -1;
	}
	if (retRefrigerant->rho_l_T == NULL || rho_l_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for rho_l_T or its coefficients do not exist.");
		printf("\nReturn -1 for function call \"dp_dw_wT_vol\".");
		return -1;
	}
	if (retAdsorption->vol_dp_dw_wTpsatRho == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for vol_dp_dw_wTpsatRho do not exist.");
		printf("\nReturn -1 for function call \"dp_dw_wT_vol\".");
		return -1;
	}
	
	// Calculate vapour pressure and saturated liquid of adsorpt
	//
	double p_sat_Pa = retRefrigerant->psat_T(T_K, p_sat_par);
	double rho_l_kgm3 = retRefrigerant->rho_l_T(T_K, rho_l_par);
	
	// Return directly result of isotherm function
	//
	return retAdsorption->vol_dp_dw_wTpsatRho(w_kgkg, T_K, p_sat_Pa,
		rho_l_kgm3, isotherm_par);
}


/*
 * dp_dT_wT_sur:
 * -------------
 *
 * Calculates derivative of equilibrium pressure with respect to temperature 
 * T in Pa/K depending on equilibrium loading w in kg/kg and equilibrium 
 * temperature T in K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapour pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer to isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid porperties at saturated state.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. temperature in Pa/K.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate loading depending on pressure and
 *	temperature when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for surface approach, fluid properties at saturated state
 *	are not required and thus ignored.
 *
 * History:
 * --------
 *	13/01/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double dp_dT_wT_sur(double w_kgkg, double T_K, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant) {
	// Typecast void pointers given as inputs to correct structs
	//
	Adsorption *retAdsorption = (Adsorption *) adsorption;
	Refrigerant *retRefrigerant = (Refrigerant *) refrigerant;
	
	// Check if structures do exists and if functions are implemented
	//	
	if (retAdsorption == NULL) {
		// Adsorption-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nAdsorption-struct does not exist.");
		printf("\nReturn -1 for function call \"dp_dT_wT_sur\".");
		return -1;
	}
	if (retRefrigerant == NULL) {
		// Refrigerant-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nRefrigerant-struct does not exist.");
		printf("\nReturn -1 for function call \"dp_dT_wT_sur\".");
		return -1;
	}	
	if (retAdsorption->sur_dp_dT_wT == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for sur_dp_dT_wT or its coefficients do not exist.");
		printf("\nReturn -1 for function call \"dp_dT_wT_sur\".");
		return -1;
	}
	
	// Avoid compiler warning C4100 by adding "do nothing" expression
	// "Do nothing" expression will be optimised out by compiler
	//
	(p_sat_par) = (p_sat_par);
	(rho_l_par) = (rho_l_par);
	
	// Return directly result of isotherm function
	//
	return retAdsorption->sur_dp_dT_wT(w_kgkg, T_K, isotherm_par);
}


/*
 * dp_dT_wT_vol:
 * -------------
 *
 * Calculates derivative of equilibrium pressure with respect to temperature 
 * T in Pa/K depending on equilibrium loading w in kg/kg and equilibrium 
 * temperature T in K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapour pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer to isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid porperties at saturated state.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. pressure in Pa/K.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate loading depending on pressure and
 *	temperature when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for volumetric approach, fluid properties at saturated state
 *	are required.
 *
 * History:
 * --------
 *	13/01/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double dp_dT_wT_vol(double w_kgkg, double T_K, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant) {
	// Typecast void pointers given as inputs to correct structs
	//
	Adsorption *retAdsorption = (Adsorption *) adsorption;
	Refrigerant *retRefrigerant = (Refrigerant *) refrigerant;
	
	// Check if structures do exists and if functions are implemented
	//	
	if (retAdsorption == NULL) {
		// Adsorption-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nAdsorption-struct does not exist.");
		printf("\nReturn -1 for function call \"dp_dT_wT_vol\".");
		return -1;
	}
	if (retRefrigerant == NULL) {
		// Refrigerant-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nRefrigerant-struct does not exist.");
		printf("\nReturn -1 for function call \"dp_dT_wT_vol\".");
		return -1;
	}	
	if (retRefrigerant->psat_T == NULL || p_sat_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for p_sat_T or its coefficients do not exist.");
		printf("\nReturn -1 for function call \"dp_dT_wT_vol\".");
		return -1;
	}
	if (retRefrigerant->dpsat_dT == NULL || p_sat_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for dpsat_dT or its coefficients do not exist.");
		printf("\nReturn -1 for function call \"dp_dT_wT_vol\".");
		return -1;
	}
	if (retRefrigerant->rho_l_T == NULL || rho_l_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for rho_l_T or its coefficients do not exist.");
		printf("\nReturn -1 for function call \"dp_dT_wT_vol\".");
		return -1;
	}
	if (retRefrigerant->drho_l_dT == NULL || rho_l_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for drho_l_dT or its coefficients do not exist.");
		printf("\nReturn -1 for function call \"dp_dT_wT_vol\".");
		return -1;
	}
	if (retAdsorption->vol_dp_dT_wTpsatRho == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for vol_dp_dT_wTpsatRho do not exist.");
		printf("\nReturn -1 for function call \"dp_dT_wT_vol\".");
		return -1;
	}
	
	// Calculate vapour pressure and saturated liquid of adsorpt
	//
	double p_sat_Pa = retRefrigerant->psat_T(T_K, p_sat_par);
	double dp_sat_dT_PaK = retRefrigerant->dpsat_dT(T_K, p_sat_par);
	double rho_l_kgm3 = retRefrigerant->rho_l_T(T_K, rho_l_par);
	double drho_l_dT_kgm3K = retRefrigerant->drho_l_dT(T_K, rho_l_par);
	
	// Return directly result of isotherm function
	//
	return retAdsorption->vol_dp_dT_wTpsatRho(w_kgkg, T_K, p_sat_Pa,
		rho_l_kgm3, dp_sat_dT_PaK, drho_l_dT_kgm3K, isotherm_par);
}


/*
 * piStar_pyxgTM_sur:
 * ------------------
 *
 * Calculates reduced spreading pressure in kg/mol depending on total pressure
 * of vapour phase p_total in Pa, molar fraction of refrigerant in vapour phase
 * y in mol/mol, molar fraction of refrigerant in adsorbed phase in mol/mol,
 * activity coefficent of refrigerant in adsorbed phase, equilibrium temperature
 * T in K and molar mass of refrigerant M in kg/mol. The reduced spreading 
 * pressure is defined as follows:
 *
 * 	piStar = A * pi / (R * T * m_sorbent) = 1 / M *
 *		Integral_0^p0{w(p,T) / p * dp}
 *
 *	where p0 = p_total*y / (gamma*x)
 *
 * Parameters:
 * -----------
 *	double p_total_Pa:
 *		Total pressure of vapour phase in Pa.
 *	double y_molmol:
 *		Molar fraction of refrigerant in vapour phase in mol/mol.
 *	double x_molmol:
 *		Molar fraction of refrigerant in adsorbed phase in mol/mol.
 *	double gamma:
 *		Activity coefficent of refrigerant in adsorbed phase.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double M_kgmol:
 *		Molar mass of refrigerant M in kg/mol.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapour pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer to isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid porperties at saturated state.
 *
 * Returns:
 * --------
 *	double:
 *		Reduced spreading pressure in kg/mol.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate reduced spreading pressure
 *	depending on pressure and temperature when volumetric approach (e.g. Dubinin 
 *	theory) is chosen as isotherm. Here, for surface approach, fluid properties
 *	at saturated state are not required and thus ignored.
 *
 * History:
 * --------
 *	13/01/2020, by Mirko Engelpracht:
 *		First implementation.
 *
*/
double piStar_pyxgTM_sur(double p_total_Pa, double y_molmol, double x_molmol,
	double gamma, double T_K, double M_kgmol, double isotherm_par[], 
	double p_sat_par[], double rho_l_par[], void *adsorption, 
	void *refrigerant) {
	// Typecast void pointers given as inputs to correct structs
	//
	Adsorption *retAdsorption = (Adsorption *) adsorption;
	Refrigerant *retRefrigerant = (Refrigerant *) refrigerant;
	
	// Check if structures do exists and if functions are implemented
	//	
	if (retAdsorption == NULL) {
		// Adsorption-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nAdsorption-struct does not exist.");
		printf("\nReturn -1 for function call \"piStar_pyxgTM_sur\".");
		return -1;
	}
	if (retRefrigerant == NULL) {
		// Refrigerant-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nRefrigerant-struct does not exist.");
		printf("\nReturn -1 for function call \"piStar_pyxgTM_sur\".");
		return -1;
	}	
	if (retAdsorption->sur_piStar_pyxgTM == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for sur_piStar_pyxgTM or its coefficients do not exist.");
		printf("\nReturn -1 for function call \"piStar_pyxgTM_sur\".");
		return -1;
	}
	
	// Avoid compiler warning C4100 by adding "do nothing" expression
	// "Do nothing" expression will be optimised out by compiler
	//
	(p_sat_par) = (p_sat_par);
	(rho_l_par) = (rho_l_par);
	
	// Return directly result of isotherm function
	//
	return retAdsorption->sur_piStar_pyxgTM(p_total_Pa, y_molmol, x_molmol, gamma,
		T_K, M_kgmol, isotherm_par);		
}


/*
 * piStar_pyxgTM_vol:
 * ------------------
 *
 * Calculates reduced spreading pressure in kg/mol depending on total pressure
 * of vapour phase p_total in Pa, molar fraction of refrigerant in vapour phase
 * y in mol/mol, molar fraction of refrigerant in adsorbed phase in mol/mol,
 * activity coefficent of refrigerant in adsorbed phase, equilibrium temperature
 * T in K and molar mass of refrigerant M in kg/mol. The reduced spreading 
 * pressure is defined as follows:
 *
 * 	piStar = A * pi / (R * T * m_sorbent) = 1 / M *
 *		Integral_0^p0{w(p,T) / p * dp}
 *
 *	where p0 = p_total*y / (gamma*x)
 *
 * Parameters:
 * -----------
 *	double p_total_Pa:
 *		Total pressure of vapour phase in Pa.
 *	double y_molmol:
 *		Molar fraction of refrigerant in vapour phase in mol/mol.
 *	double x_molmol:
 *		Molar fraction of refrigerant in adsorbed phase in mol/mol.
 *	double gamma:
 *		Activity coefficent of refrigerant in adsorbed phase.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double M_kgmol:
 *		Molar mass of refrigerant M in kg/mol.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapour pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer to isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid porperties at saturated state.
 *
 * Returns:
 * --------
 *	double:
 *		Reduced spreading pressure in kg/mol.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate reduced spreading pressure
 *	depending on pressure and temperature when volumetric approach (e.g. Dubinin 
 *	theory) is chosen as isotherm. Here, for surface approach, fluid properties
 *	at saturated state are not required and thus ignored.
 *
 * History:
 * --------
 *	13/01/2020, by Mirko Engelpracht:
 *		First implementation.
 *
*/
double piStar_pyxgTM_vol(double p_total_Pa, double y_molmol, double x_molmol,
	double gamma, double T_K, double M_kgmol, double isotherm_par[], 
	double p_sat_par[], double rho_l_par[], void *adsorption, 
	void *refrigerant) {
	// Typecast void pointers given as inputs to correct structs
	//
	Adsorption *retAdsorption = (Adsorption *) adsorption;
	Refrigerant *retRefrigerant = (Refrigerant *) refrigerant;
	
	// Check if structures do exists and if functions are implemented
	//	
	if (retAdsorption == NULL) {
		// Adsorption-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nAdsorption-struct does not exist.");
		printf("\nReturn -1 for function call \"piStar_pyxgTM_vol\".");
		return -1;
	}
	if (retRefrigerant == NULL) {
		// Refrigerant-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nRefrigerant-struct does not exist.");
		printf("\nReturn -1 for function call \"piStar_pyxgTM_vol\".");
		return -1;
	}	
	if (retRefrigerant->psat_T == NULL || p_sat_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for p_sat_T or its coefficients do not exist.");
		printf("\nReturn -1 for function call \"piStar_pyxgTM_vol\".");
		return -1;
	}
	if (retRefrigerant->rho_l_T == NULL || rho_l_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for rho_l_T or its coefficients do not exist.");
		printf("\nReturn -1 for function call \"piStar_pyxgTM_vol\".");
		return -1;
	}
	if (retAdsorption->vol_piStar_pyxgTpsatRhoM == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for vol_piStar_pyxgTpsatRhoM or its coefficients do not exist.");
		printf("\nReturn -1 for function call \"piStar_pyxgTM_vol\".");
		return -1;
	}
	
	// Calculate vapour pressure and saturated liquid of adsorpt
	//
	double p_sat_Pa = retRefrigerant->psat_T(T_K, p_sat_par);
	double rho_l_kgm3 = retRefrigerant->rho_l_T(T_K, rho_l_par);
	
	// Return directly result of isotherm function
	//
	return retAdsorption->vol_piStar_pyxgTpsatRhoM(p_total_Pa, y_molmol, x_molmol,
		gamma, T_K, p_sat_Pa, rho_l_kgm3, M_kgmol, isotherm_par);		
}
