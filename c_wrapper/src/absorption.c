//////////////////
// absorption.c //
//////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "absorption.h"
#include "absorption_duehring.h"
#include "absorption_antoine.h"
#include "absorption_activity_wilson.h"
#include "absorption_activity_tsubokakatayama.h"
#include "absorption_activity_heil.h"
#include "absorption_activity_wangchao.h"
#include "absorption_activity_nrtl.h"
#include "absorption_activity_uniquac.h"
#include "absorption_activity_floryhuggins.h"
#include "absorption_mixing_1pvdw.h"
#include "structDefinitions.c"


/////////////////////////////
// Definition of functions //
/////////////////////////////
/*
 * newAbsorption:
 * --------------
 *
 * Initialization function to create a new Absorption-struct. Therefore,
 * function checks first if enough memory is available to allocate absorption
 * struct and then sets function pointers within the struct depending on the
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
 *	struct *Absorption:
 *		Returns malloc-ed Absorption-struct that contains function pointers
 *		referring to functions of a specific isotherm that is chosen by
 *		parameter isotherm_type.
 *
 * History:
 * --------
 *	02/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
Absorption *newAbsorption(const char *isotherm_type) {
    // Try to allocate memory for vector structure
	//
    Absorption *retAbsorption = (Absorption *) malloc(sizeof(Absorption));

    if (retAbsorption == NULL) {
		// Not enough memory available for vector structure
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nCannot allocate memory for Absorption-structure.");
        return NULL;
	}

	// Save general information of isotherm
	//
	retAbsorption->isotherm_type = isotherm_type;

	// Initialize all function pointers with NULL because NULL is used by
	// functions of WorkingPair-struct to check if isotherm function is
	// implemented or not
	//
	retAbsorption->con_X_pT = NULL;
	retAbsorption->con_p_XT = NULL;
	retAbsorption->con_T_pX = NULL;
	retAbsorption->con_dX_dp_pT = NULL;
	retAbsorption->con_dX_dT_pT = NULL;
	retAbsorption->con_dp_dX_XT = NULL;
	retAbsorption->con_dp_dT_XT = NULL;


	retAbsorption->act_g_Tx_wo_v = NULL;
	retAbsorption->act_g_Txv1v2_w_v = NULL;
	retAbsorption->act_p_Txpsat_wo_v = NULL;
	retAbsorption->act_p_Txv1v2gpsat_w_v_gf = NULL;
	retAbsorption->act_p_Txv1v2psat_w_v = NULL;
	retAbsorption->act_p_Txgpsat_w_gf = NULL;
	retAbsorption->act_x_pTpsat_wo_v = NULL;
	retAbsorption->act_x_pTv1v2psat_w_v = NULL;
	retAbsorption->act_x_pTv1v2gpsat_w_v_gf = NULL;
	retAbsorption->act_x_pTgpsat_w_gf = NULL;

	retAbsorption->act_g_Txv1v2 = NULL;
	retAbsorption->act_p_Txgv1v2psat = NULL;
	retAbsorption->act_x_pTgv1v2psat = NULL;


	retAbsorption->mix_p_Tx = NULL;

    // Set function pointers of struct to correct isotherm functions depending
	// on the isotherm type.
	//
	if (strcmp(isotherm_type, "duehring") == 0) {
		// Set function pointers to functions of Duehring isotherm
		//
		retAbsorption->con_X_pT = &absorption_duehring_X_pT;
		retAbsorption->con_p_XT = &absorption_duehring_p_XT;
		retAbsorption->con_T_pX = &absorption_duehring_T_pX;

		retAbsorption->con_dX_dp_pT = &absorption_duehring_dX_dp_pT;
		retAbsorption->con_dX_dT_pT = &absorption_duehring_dX_dT_pT;
		retAbsorption->con_dp_dX_XT = &absorption_duehring_dp_dX_XT;
		retAbsorption->con_dp_dT_XT = &absorption_duehring_dp_dT_XT;

		// Set function pointers that are always defined
		//


		// Return structure
		//
		return retAbsorption;

	} else if (strcmp(isotherm_type, "antoine") == 0) {
		// Set function pointers to functions of Duehring isotherm
		//
		retAbsorption->con_X_pT = &absorption_antoine_X_pT;
		retAbsorption->con_p_XT = &absorption_antoine_p_XT;
		retAbsorption->con_T_pX = &absorption_antoine_T_pX;

		retAbsorption->con_dX_dp_pT = &absorption_antoine_dX_dp_pT;
		retAbsorption->con_dX_dT_pT = &absorption_antoine_dX_dT_pT;
		retAbsorption->con_dp_dX_XT = &absorption_antoine_dp_dX_XT;
		retAbsorption->con_dp_dT_XT = &absorption_antoine_dp_dT_XT;

		// Set function pointers that are always defined
		//


		// Return structure
		//
		return retAbsorption;

	} else if (strcmp(isotherm_type, "wilson-fixeddl") == 0) {
		// Set function pointers to functions of Wilson isotherm with fixed dl
		// (i.e. dl depends not on temperature)
		//
		retAbsorption->act_g_Txv1v2_w_v =
			&absorption_activity_wilson_fdl_g1_Txv1v2;
		retAbsorption->act_p_Txv1v2gpsat_w_v_gf =
			&absorption_activity_wilson_p_Txv1v2psat;
		retAbsorption->act_x_pTv1v2gpsat_w_v_gf =
			&absorption_activity_wilson_x_pTv1v2psat;

		// Set function pointers that are always defined for absorption
		// equations based on activity coefficients
		//
		retAbsorption->act_g_Txv1v2 = &act_g_Txv1v2_w_v;
		retAbsorption->act_p_Txgv1v2psat = &act_p_Txgv1v2psat_w_v_gf;
		retAbsorption->act_x_pTgv1v2psat = &act_x_pTgv1v2psat_w_v_gf;

		// Set function pointers that are always defined
		//

		// Return structure
		//
		return retAbsorption;

	}  else if (strcmp(isotherm_type, "wilson-dl-t") == 0) {
		// Set function pointers to functions of Wilson isotherm with dl that
		// depends on temperature
		//
		retAbsorption->act_g_Txv1v2_w_v =
			&absorption_activity_wilson_tdl_g1_Txv1v2;
		retAbsorption->act_p_Txv1v2gpsat_w_v_gf =
			&absorption_activity_wilson_p_Txv1v2psat;
		retAbsorption->act_x_pTv1v2gpsat_w_v_gf =
			&absorption_activity_wilson_x_pTv1v2psat;

		// Set function pointers that are always defined for absorption
		// equations based on activity coefficients
		//
		retAbsorption->act_g_Txv1v2 = &act_g_Txv1v2_w_v;
		retAbsorption->act_p_Txgv1v2psat = &act_p_Txgv1v2psat_w_v_gf;
		retAbsorption->act_x_pTgv1v2psat = &act_x_pTgv1v2psat_w_v_gf;

		// Set function pointers that are always defined
		//

		// Return structure
		//
		return retAbsorption;

	} else if (strcmp(isotherm_type, "tsuboka-katayama") == 0) {
		// Set function pointers to functions of Tsuboka-Katayama isotherm
		//
		retAbsorption->act_g_Txv1v2_w_v =
			&absorption_activity_tsubokakatayama_g1_Txv1v2;
		retAbsorption->act_p_Txv1v2psat_w_v =
			&absorption_activity_tsubokakatayama_p_Txv1v2psat;
		retAbsorption->act_x_pTv1v2psat_w_v =
			&absorption_activity_tsubokakatayama_x_pTv1v2psat;

		// Set function pointers that are always defined for absorption
		// equations based on activity coefficients
		//
		retAbsorption->act_g_Txv1v2 = &act_g_Txv1v2_w_v;
		retAbsorption->act_p_Txgv1v2psat = &act_p_Txgv1v2psat_w_v;
		retAbsorption->act_x_pTgv1v2psat = &act_x_pTgv1v2psat_w_v;

		// Set function pointers that are always defined
		//

		// Return structure
		//
		return retAbsorption;

	} else if (strcmp(isotherm_type, "heil") == 0) {
		// Set function pointers to functions of Heil isotherm
		//
		retAbsorption->act_g_Txv1v2_w_v =
			&absorption_activity_heil_g1_Txv1v2;
		retAbsorption->act_p_Txv1v2psat_w_v =
			&absorption_activity_heil_p_Txv1v2psat;
		retAbsorption->act_x_pTv1v2psat_w_v =
			&absorption_activity_heil_x_pTv1v2psat;

		// Set function pointers that are always defined for absorption
		// equations based on activity coefficients
		//
		retAbsorption->act_g_Txv1v2 = &act_g_Txv1v2_w_v;
		retAbsorption->act_p_Txgv1v2psat = &act_p_Txgv1v2psat_w_v;
		retAbsorption->act_x_pTgv1v2psat = &act_x_pTgv1v2psat_w_v;

		// Set function pointers that are always defined
		//

		// Return structure
		//
		return retAbsorption;

	} else if (strcmp(isotherm_type, "wang-chao") == 0) {
		// Set function pointers to functions of Wang-Chao isotherm
		//
		retAbsorption->act_g_Txv1v2_w_v =
			&absorption_activity_wangchao_g1_Txv1v2;
		retAbsorption->act_p_Txv1v2psat_w_v =
			&absorption_activity_wangchao_p_Txv1v2psat;
		retAbsorption->act_x_pTv1v2psat_w_v =
			&absorption_activity_wangchao_x_pTv1v2psat;

		// Set function pointers that are always defined for absorption
		// equations based on activity coefficients
		//
		retAbsorption->act_g_Txv1v2 = &act_g_Txv1v2_w_v;
		retAbsorption->act_p_Txgv1v2psat = &act_p_Txgv1v2psat_w_v;
		retAbsorption->act_x_pTgv1v2psat = &act_x_pTgv1v2psat_w_v;

		// Set function pointers that are always defined
		//

		// Return structure
		//
		return retAbsorption;

	} else if (strcmp(isotherm_type, "nrtl-fixeddg") == 0) {
		// Set function pointers to functions of NRTL isotherm with fixed dg
		// (i.e. dg depends not on temperature)
		//
		retAbsorption->act_g_Tx_wo_v =
			&absorption_activity_nrtl_fdg_g1_Tx;
		retAbsorption->act_p_Txgpsat_w_gf =
			&absorption_activity_nrtl_p_Txgpsat;
		retAbsorption->act_x_pTgpsat_w_gf =
			&absorption_activity_nrtl_x_pTgpsat;

		// Set function pointers that are always defined for absorption
		// equations based on activity coefficients
		//
		retAbsorption->act_g_Txv1v2 = &act_g_Txv1v2_wo_v;
		retAbsorption->act_p_Txgv1v2psat = &act_p_Txgv1v2psat_w_gf;
		retAbsorption->act_x_pTgv1v2psat = &act_x_pTgv1v2psat_w_gf;

		// Set function pointers that are always defined
		//

		// Return structure
		//
		return retAbsorption;

	} else if (strcmp(isotherm_type, "nrtl-dg-t") == 0) {
		// Set function pointers to functions of NRTL isotherm with gd that
		// depends on temperature
		//
		retAbsorption->act_g_Tx_wo_v =
			&absorption_activity_nrtl_dgT_g1_Tx;
		retAbsorption->act_p_Txgpsat_w_gf =
			&absorption_activity_nrtl_p_Txgpsat;
		retAbsorption->act_x_pTgpsat_w_gf =
			&absorption_activity_nrtl_x_pTgpsat;

		// Set function pointers that are always defined for absorption
		// equations based on activity coefficients
		//
		retAbsorption->act_g_Txv1v2 = &act_g_Txv1v2_wo_v;
		retAbsorption->act_p_Txgv1v2psat = &act_p_Txgv1v2psat_w_gf;
		retAbsorption->act_x_pTgv1v2psat = &act_x_pTgv1v2psat_w_gf;

		// Set function pointers that are always defined
		//

		// Return structure
		//
		return retAbsorption;

	} else if (strcmp(isotherm_type, "uniquac-fixeddu") == 0) {
		// Set function pointers to functions of UNIQUAC isotherm with fixed du
		// (i.e. du depends not on temperature)
		//
		retAbsorption->act_g_Tx_wo_v =
			&absorption_activity_uniquac_fdu_g1_Tx;
		retAbsorption->act_p_Txgpsat_w_gf =
			&absorption_activity_uniquac_p_Txgpsat;
		retAbsorption->act_x_pTgpsat_w_gf =
			&absorption_activity_uniquac_x_pTgpsat;

		// Set function pointers that are always defined for absorption
		// equations based on activity coefficients
		//
		retAbsorption->act_g_Txv1v2 = &act_g_Txv1v2_wo_v;
		retAbsorption->act_p_Txgv1v2psat = &act_p_Txgv1v2psat_w_gf;
		retAbsorption->act_x_pTgv1v2psat = &act_x_pTgv1v2psat_w_gf;

		// Set function pointers that are always defined
		//

		// Return structure
		//
		return retAbsorption;

	} else if (strcmp(isotherm_type, "uniquac-du-t") == 0) {
		// Set function pointers to functions of UNIQUAC isotherm with du that
		// depends on temperature
		//
		retAbsorption->act_g_Tx_wo_v =
			&absorption_activity_uniquac_duT_g1_Tx;
		retAbsorption->act_p_Txgpsat_w_gf =
			&absorption_activity_uniquac_p_Txgpsat;
		retAbsorption->act_x_pTgpsat_w_gf =
			&absorption_activity_uniquac_x_pTgpsat;

		// Set function pointers that are always defined for absorption
		// equations based on activity coefficients
		//
		retAbsorption->act_g_Txv1v2 = &act_g_Txv1v2_wo_v;
		retAbsorption->act_p_Txgv1v2psat = &act_p_Txgv1v2psat_w_gf;
		retAbsorption->act_x_pTgv1v2psat = &act_x_pTgv1v2psat_w_gf;

		// Set function pointers that are always defined
		//

		// Return structure
		//
		return retAbsorption;

	} else if (strcmp(isotherm_type, "flory-huggins") == 0) {
		// Set function pointers to functions of Flory-Huggins isotherm
		//
		retAbsorption->act_g_Tx_wo_v =
			&absorption_activity_floryhuggins_g1_Tx;
		retAbsorption->act_p_Txpsat_wo_v =
			&absorption_activity_floryhuggins_p_Txpsat;
		retAbsorption->act_x_pTpsat_wo_v =
			&absorption_activity_floryhuggins_x_pTgpsat;

		// Set function pointers that are always defined for absorption
		// equations based on activity coefficients
		//
		retAbsorption->act_g_Txv1v2 = &act_g_Txv1v2_wo_v;
		retAbsorption->act_p_Txgv1v2psat = &act_p_Txgv1v2psat_wo_v;
		retAbsorption->act_x_pTgv1v2psat = &act_x_pTgv1v2psat_wo_v;

		// Set function pointers that are always defined
		//

		// Return structure
		//
		return retAbsorption;

	} else if (strcmp(isotherm_type, "mixingrule-1pvdw") == 0) {
		// Set function pointers to functions of Heil isotherm
		//
		retAbsorption->mix_p_Tx =
			&absorption_mixing_1pvdw_p_Tx;

		// Set function pointers that are always defined
		//


		// Return structure
		//
		return retAbsorption;

	} else {
		// Isotherm does not exist. Therefore, throw warning and free memory.
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nChosen isotherm type \"%s\" does not exist.",
			isotherm_type);
		printf("\nCannot create Absorption-structure.");

		free(retAbsorption);
		return NULL;
	}
}


/*
 * delAbsorption:
 * --------------
 *
 * Destructor function to free allocated memory of a Absorption-struct.
 *
 * Parameters:
 * -----------
 * 	struct *Absorption:
 *		Pointer of Absorption-struct.
 *
 * History:
 * --------
 *	02/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
void delAbsorption(void *absorption) {
	// Typecast void pointer given as input to pointer of Absorption-struct
	//
	Absorption *retAbsorption = (Absorption *) absorption;

    // Because of initialization function, Absorption-struct is NULL or fully
	// built
	//
    if (retAbsorption != NULL) {
		free(retAbsorption);
    }
}


/*
 * act_g_Txv1v2_wo_v:
 * ------------------
 *
 * Calculates activity coefficient of first component depending on temperature
 * T_K in K and mole fraction in liquid phase x_molmol in mol/mol.
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
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm equation.
 *	struct *Adsorption:
 *		Pointer to Absorption-struct that contains pointer of isotherm
 *		functions.
 *
 * Returns:
 * --------
 *	double:
 *		Activity coefficient of first component.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate activity coefficients that may
 *	need molar volumes as input. Here, molar volumes are not required.
 *
 * History:
 * --------
 *	02/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double act_g_Txv1v2_wo_v(double T_K, double x_molmol,
	double v1_m3mol, double v2_m3mol, double isotherm_par[], void *absorption) {
	// Typecast void pointers given as inputs to correct structs
	//
	Absorption *retAbsorption = (Absorption *) absorption;

	// Check if structures do exists and if functions are implemented
	//
	if (retAbsorption == NULL) {
		// Absorption-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nAbsorption-struct does not exist.");
		printf("\nReturn -1 for function call \"act_g_Txv1v2_wo_v\".");
		return -1;
	}
	if (retAbsorption->act_g_Tx_wo_v == NULL || isotherm_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for act_g_Tx_wo_v or its coefficients do not "
			"exist.");
		printf("\nReturn -1 for function call \"act_g_Txv1v2_wo_v\".");
		return -1;
	}

	// Avoid compiler warning C4100 by adding "do nothing" expression
	// "Do nothing" expression will be optimized out by compiler
	//
	(v1_m3mol) = (v1_m3mol);
	(v2_m3mol) = (v2_m3mol);

	// Return directly result of isotherm function
	//
	return retAbsorption->act_g_Tx_wo_v(T_K, x_molmol, isotherm_par);
}


/*
 * act_g_Txv1v2_w_v:
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
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm equation.
 *	struct *Absorption:
 *		Pointer to Absorption-struct that contains pointer of isotherm
 *		functions.
 *
 * Returns:
 * --------
 *	double:
 *		Activity coefficient of first component.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate activity coefficients that may
 *	need molar volumes as input. Here, molar volumes may are required.
 *
 * History:
 * --------
 *	02/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double act_g_Txv1v2_w_v(double T_K, double x_molmol,
	double v1_m3mol, double v2_m3mol, double isotherm_par[], void *absorption) {
	// Typecast void pointers given as inputs to correct structs
	//
	Absorption *retAbsorption = (Absorption *) absorption;

	// Check if structures do exists and if functions are implemented
	//
	if (retAbsorption == NULL) {
		// Absorption-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nAbsorption-struct does not exist.");
		printf("\nReturn -1 for function call \"act_g_Txv1v2_w_v\".");
		return -1;
	}
	if (retAbsorption->act_g_Txv1v2_w_v == NULL || isotherm_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for act_g_Txv1v2_w_v or its coefficients do not "
			"exist.");
		printf("\nReturn -1 for function call \"act_g_Txv1v2_w_v\".");
		return -1;
	}

	// Return directly result of isotherm function
	//
	return retAbsorption->act_g_Txv1v2_w_v(T_K, x_molmol, v1_m3mol, v2_m3mol,
		isotherm_par);
}


/*
 * act_p_Txgv1v2psat_wo_v:
 * -----------------------
 *
 * Calculates equilibrium pressure p_Pa in Pa of first component depending on
 * temperature T_K in K, mole fraction in liquid phase x_molmol in mol/mol, and
 * saturation pressure of first component p_sat_Pa in Pa.
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
 *		Saturation pressure on Pa.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Wilson equation.
 *	struct *Absorption:
 *		Pointer to Absorption-struct that contains pointer of isotherm
 *		functions.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure in Pa.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate activity coefficients that may
 *	need molar volumes as inputs. Here, molar volumes are not required
 *
 * History:
 * --------
 *	02/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double act_p_Txgv1v2psat_wo_v(double T_K, double x_molmol,
	double v1_m3mol, double v2_m3mol, double p_sat_Pa, double isotherm_par[],
	void *absorption) {
	// Typecast void pointers given as inputs to correct structs
	//
	Absorption *retAbsorption = (Absorption *) absorption;

	// Check if structures do exists and if functions are implemented
	//
	if (retAbsorption == NULL) {
		// Absorption-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nAbsorption-struct does not exist.");
		printf("\nReturn -1 for function call \"act_p_Txgv1v2psat_wo_v\".");
		return -1;
	}
	if (retAbsorption->act_p_Txpsat_wo_v == NULL || isotherm_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for act_p_Txpsat_wo_v or its coefficients do not "
			"exist.");
		printf("\nReturn -1 for function call \"act_p_Txgv1v2psat_wo_v\".");
		return -1;
	}

	// Avoid compiler warning C4100 by adding "do nothing" expression
	// "Do nothing" expression will be optimized out by compiler
	//
	(v1_m3mol) = (v1_m3mol);
	(v2_m3mol) = (v2_m3mol);

	// Return directly result of isotherm function
	//
	return retAbsorption->act_p_Txpsat_wo_v(T_K, x_molmol, p_sat_Pa,
		isotherm_par);
}


/*
 * act_p_Txgv1v2psat_w_v:
 * ----------------------
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
 *		Saturation pressure on Pa.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Wilson equation.
 *	struct *Absorption:
 *		Pointer to Absorption-struct that contains pointer of isotherm
 *		functions.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure in Pa.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate activity coefficients that may
 *	need molar volumes as inputs. Here, molar volumes are required.
 *
 * History:
 * --------
 *	02/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double act_p_Txgv1v2psat_w_v(double T_K, double x_molmol,
	double v1_m3mol, double v2_m3mol, double p_sat_Pa, double isotherm_par[],
	void *absorption) {
	// Typecast void pointers given as inputs to correct structs
	//
	Absorption *retAbsorption = (Absorption *) absorption;

	// Check if structures do exists and if functions are implemented
	//
	if (retAbsorption == NULL) {
		// Absorption-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nAbsorption-struct does not exist.");
		printf("\nReturn -1 for function call \"act_p_Txgv1v2psat_w_v\".");
		return -1;
	}
	if (retAbsorption->act_p_Txv1v2psat_w_v == NULL || isotherm_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for act_p_Txv1v2psat_w_v or its coefficients do not "
			"exist.");
		printf("\nReturn -1 for function call \"act_p_Txgv1v2psat_w_v\".");
		return -1;
	}

	// Return directly result of isotherm function
	//
	return retAbsorption->act_p_Txv1v2psat_w_v(T_K, x_molmol, v1_m3mol,
		v2_m3mol, p_sat_Pa,	isotherm_par);
}


/*
 * act_p_Txgv1v2psat_w_v_gf:
 * -------------------------
 *
 * Calculates equilibrium pressure p_Pa in Pa of first component depending on
 * temperature T_K in K, mole fraction in liquid phase x_molmol in mol/mol,
 * molar volume of first component in m³/mol, molar volume of second component,
 * saturation pressure of first component p_sat_Pa in Pa, and function pointer
 * for activity coefficient of first component.
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
 *		Saturation pressure on Pa.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Wilson equation.
 *	struct *Absorption:
 *		Pointer to Absorption-struct that contains pointer of isotherm
 *		functions.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure in Pa.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate activity coefficients that may
 *	need molar volumes. Here,  molar volumes are required. Moreover, a function
 *  pointer is given internally to calculate the activity coefficient of the
 *	first component.
 *
 * History:
 * --------
 *	03/25/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double act_p_Txgv1v2psat_w_v_gf(double T_K, double x_molmol,
	double v1_m3mol, double v2_m3mol, double p_sat_Pa, double isotherm_par[],
	void *absorption) {
	// Typecast void pointers given as inputs to correct structs
	//
	Absorption *retAbsorption = (Absorption *) absorption;

	// Check if structures do exists and if functions are implemented
	//
	if (retAbsorption == NULL) {
		// Absorption-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nAbsorption-struct does not exist.");
		printf("\nReturn -1 for function call \"act_p_Txgv1v2psat_w_v_gf\".");
		return -1;
	}
	if (retAbsorption->act_p_Txv1v2gpsat_w_v_gf == NULL ||
		isotherm_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for act_p_Txv1v2gpsat_w_v_gf or its coefficients do "
			"not exist.");
		printf("\nReturn -1 for function call \"act_p_Txgv1v2psat_w_v_gf\".");
		return -1;
	}
	if (retAbsorption->act_g_Txv1v2_w_v == NULL || isotherm_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for act_g_Txv1v2_w_v or its coefficients do not "
			"exist.");
		printf("\nReturn -1 for function call \"act_p_Txgv1v2psat_w_v_gf\".");
		return -1;
	}

	// Return directly result of isotherm function
	//
	return retAbsorption->act_p_Txv1v2gpsat_w_v_gf(T_K, x_molmol, v1_m3mol,
		v2_m3mol, retAbsorption->act_g_Txv1v2_w_v, p_sat_Pa, isotherm_par);
}


/*
 * act_p_Txgv1v2psat_w_gf:
 * -----------------------
 *
 * Calculates equilibrium pressure p_Pa in Pa of first component depending on
 * temperature T_K in K, mole fraction in liquid phase x_molmol in mol/mol,
 * saturation pressure of first component p_sat_Pa in Pa, and function pointer
 * for activity coefficient of first component.
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
 *		Saturation pressure on Pa.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Wilson equation.
 *	struct *Absorption:
 *		Pointer to Absorption-struct that contains pointer of isotherm
 *		functions.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure in Pa.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate activity coefficients that may
 *	need molar volumes. Here,  molar volumes are not required but a function
 *  pointer is given internally to calculate the activity coefficient of the
 *	first component.
 *
 * History:
 * --------
 *	02/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double act_p_Txgv1v2psat_w_gf(double T_K, double x_molmol,
	double v1_m3mol, double v2_m3mol, double p_sat_Pa, double isotherm_par[],
	void *absorption) {
	// Typecast void pointers given as inputs to correct structs
	//
	Absorption *retAbsorption = (Absorption *) absorption;

	// Check if structures do exists and if functions are implemented
	//
	if (retAbsorption == NULL) {
		// Absorption-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nAbsorption-struct does not exist.");
		printf("\nReturn -1 for function call \"act_p_Txgv1v2psat_w_gf\".");
		return -1;
	}
	if (retAbsorption->act_p_Txgpsat_w_gf == NULL || isotherm_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for act_p_Txgpsat_w_gf or its coefficients do not "
			"exist.");
		printf("\nReturn -1 for function call \"act_p_Txgv1v2psat_w_gf\".");
		return -1;
	}
	if (retAbsorption->act_g_Tx_wo_v == NULL || isotherm_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for act_g_Tx_wo_v or its coefficients do not "
			"exist.");
		printf("\nReturn -1 for function call \"act_p_Txgv1v2psat_w_gf\".");
		return -1;
	}

	// Avoid compiler warning C4100 by adding "do nothing" expression
	// "Do nothing" expression will be optimized out by compiler
	//
	(v1_m3mol) = (v1_m3mol);
	(v2_m3mol) = (v2_m3mol);

	// Return directly result of isotherm function
	//
	return retAbsorption->act_p_Txgpsat_w_gf(T_K, x_molmol,
		retAbsorption->act_g_Tx_wo_v, p_sat_Pa, isotherm_par);
}


/*
 * act_x_pTgv1v2psat_wo_v:
 * -----------------------
 *
 * Calculates mole fraction of first component in liquid phase x_molmol in
 * mol/mol depending on equilibrium pressure p_Pa in Pa of first component,
 * temperature T_K in K, and saturation pressure of first component p_sat_Pa in
 * Pa.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure of first component in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double v1_m3mol:
 *		Equilibrium molar volume of first component in m³/mol.
 *	double v2_m3mol:
 *		Equilibrium molar volume of second component in m³/mol.
 *	double p_sat_Pa:
 *		Saturation pressure on Pa.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Wilson equation.
 *	struct *Absorption:
 *		Pointer to Absorption-struct that contains pointer of isotherm
 *		functions.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium mole fraction of first component in liquid phase in mol/mol.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate activity coefficients that may
 *	need molar volumes as inputs. Here, molar volumes are not required
 *
 * History:
 * --------
 *	03/26/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double act_x_pTgv1v2psat_wo_v(double p_Pa, double T_K, double v1_m3mol,
	double v2_m3mol, double p_sat_Pa, double isotherm_par[], void *absorption) {
	// Typecast void pointers given as inputs to correct structs
	//
	Absorption *retAbsorption = (Absorption *) absorption;

	// Check if structures do exists and if functions are implemented
	//
	if (retAbsorption == NULL) {
		// Absorption-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nAbsorption-struct does not exist.");
		printf("\nReturn -1 for function call \"act_x_pTgv1v2psat_wo_v\".");
		return -1;
	}
	if (retAbsorption->act_x_pTpsat_wo_v == NULL || isotherm_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for act_x_pTpsat_wo_v or its coefficients do not "
			"exist.");
		printf("\nReturn -1 for function call \"act_x_pTgv1v2psat_wo_v\".");
		return -1;
	}

	// Avoid compiler warning C4100 by adding "do nothing" expression
	// "Do nothing" expression will be optimized out by compiler
	//
	(v1_m3mol) = (v1_m3mol);
	(v2_m3mol) = (v2_m3mol);

	// Return directly result of isotherm function
	//
	return retAbsorption->act_x_pTpsat_wo_v(p_Pa, T_K, p_sat_Pa,
		isotherm_par);
}


/*
 * act_x_pTgv1v2psat_w_v:
 * ----------------------
 *
 * Calculates mole fraction of first component in liquid phase x_molmol in
 * mol/mol depending on equilibrium pressure p_Pa in Pa of first component,
 * temperature T_K in K, molar volume of first component in m³/mol, molar volume
 * of second component in m³/mol,and saturation pressure of first component
 * p_sat_Pa in Pa.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure of first component in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double v1_m3mol:
 *		Equilibrium molar volume of first component in m³/mol.
 *	double v2_m3mol:
 *		Equilibrium molar volume of second component in m³/mol.
 *	double p_sat_Pa:
 *		Saturation pressure on Pa.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Wilson equation.
 *	struct *Absorption:
 *		Pointer to Absorption-struct that contains pointer of isotherm
 *		functions.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium mole fraction of first component in liquid phase in mol/mol.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate activity coefficients that may
 *	need molar volumes as inputs. Here, molar volumes are required.
 *
 * History:
 * --------
 *	03/26/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double act_x_pTgv1v2psat_w_v(double p_Pa, double T_K, double v1_m3mol,
	double v2_m3mol, double p_sat_Pa, double isotherm_par[], void *absorption) {
	// Typecast void pointers given as inputs to correct structs
	//
	Absorption *retAbsorption = (Absorption *) absorption;

	// Check if structures do exists and if functions are implemented
	//
	if (retAbsorption == NULL) {
		// Absorption-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nAbsorption-struct does not exist.");
		printf("\nReturn -1 for function call \"act_x_pTgv1v2psat_w_v\".");
		return -1;
	}
	if (retAbsorption->act_x_pTv1v2psat_w_v == NULL || isotherm_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for act_x_pTv1v2psat_w_v or its coefficients do not "
			"exist.");
		printf("\nReturn -1 for function call \"act_x_pTgv1v2psat_w_v\".");
		return -1;
	}

	// Return directly result of isotherm function
	//
	return retAbsorption->act_x_pTv1v2psat_w_v(p_Pa, T_K, v1_m3mol, v2_m3mol,
		p_sat_Pa, isotherm_par);
}


/*
 * act_x_pTgv1v2psat_w_v_gf:
 * -------------------------
 *
 * Calculates mole fraction of first component in liquid phase x_molmol in
 * mol/mol depending on equilibrium pressure p_Pa in Pa of first component,
 * temperature T_K in K, molar volume of first component in m³/mol, molar volume
 * of second component in m³/mol,and saturation pressure of first component
 * p_sat_Pa in Pa, and function pointer for activity coefficient of first
 * component.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure of first component in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double v1_m3mol:
 *		Equilibrium molar volume of first component in m³/mol.
 *	double v2_m3mol:
 *		Equilibrium molar volume of second component in m³/mol.
 *	double p_sat_Pa:
 *		Saturation pressure on Pa.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Wilson equation.
 *	struct *Absorption:
 *		Pointer to Absorption-struct that contains pointer of isotherm
 *		functions.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium mole fraction of first component in liquid phase in mol/mol.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate activity coefficients that may
 *	need molar volumes as inputs. Here, molar volumes are required. Moreover, a
 *  function pointer is given internally to calculate the activity coefficient
 * 	of the first component.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double act_x_pTgv1v2psat_w_v_gf(double p_Pa, double T_K, double v1_m3mol,
	double v2_m3mol, double p_sat_Pa, double isotherm_par[], void *absorption) {
	// Typecast void pointers given as inputs to correct structs
	//
	Absorption *retAbsorption = (Absorption *) absorption;

	// Check if structures do exists and if functions are implemented
	//
	if (retAbsorption == NULL) {
		// Absorption-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nAbsorption-struct does not exist.");
		printf("\nReturn -1 for function call \"act_x_pTgv1v2psat_w_v_gf\".");
		return -1;
	}
	if (retAbsorption->act_x_pTv1v2gpsat_w_v_gf == NULL ||
		isotherm_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for act_x_pTv1v2gpsat_w_v_gf or its coefficients do "
			"not exist.");
		printf("\nReturn -1 for function call \"act_x_pTgv1v2psat_w_v_gf\".");
		return -1;
	}
	if (retAbsorption->act_g_Txv1v2_w_v == NULL || isotherm_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for act_g_Txv1v2_w_v or its coefficients do not "
			"exist.");
		printf("\nReturn -1 for function call \"act_x_pTgv1v2psat_w_v_gf\".");
		return -1;
	}

	// Return directly result of isotherm function
	//
	return retAbsorption->act_x_pTv1v2gpsat_w_v_gf(p_Pa, T_K, v1_m3mol,
		v2_m3mol, retAbsorption->act_g_Txv1v2_w_v, p_sat_Pa, isotherm_par);
}


/*
 * act_x_pTgv1v2psat_w_gf:
 * -----------------------
 *
 * Calculates mole fraction of first component in liquid phase x_molmol in
 * mol/mol depending on equilibrium pressure p_Pa in Pa of first component,
 * temperature T_K in K, and saturation pressure of first component p_sat_Pa in
 * Pa, and function pointer for activity coefficient of first component.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure of first component in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double v1_m3mol:
 *		Equilibrium molar volume of first component in m³/mol.
 *	double v2_m3mol:
 *		Equilibrium molar volume of second component in m³/mol.
 *	double p_sat_Pa:
 *		Saturation pressure on Pa.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Wilson equation.
 *	struct *Absorption:
 *		Pointer to Absorption-struct that contains pointer of isotherm
 *		functions.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium mole fraction of first component in liquid phase in mol/mol.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate activity coefficients that may
 *	need molar volumes as inputs. Here, molar volumes are not required. However,
 *  a function pointer is given internally to calculate the activity coefficient
 * 	of the first component.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double act_x_pTgv1v2psat_w_gf(double p_Pa, double T_K, double v1_m3mol,
	double v2_m3mol, double p_sat_Pa, double isotherm_par[], void *absorption) {
	// Typecast void pointers given as inputs to correct structs
	//
	Absorption *retAbsorption = (Absorption *) absorption;

	// Check if structures do exists and if functions are implemented
	//
	if (retAbsorption == NULL) {
		// Absorption-struct does not exists
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nAbsorption-struct does not exist.");
		printf("\nReturn -1 for function call \"act_x_pTgv1v2psat_w_gf\".");
		return -1;
	}
	if (retAbsorption->act_x_pTgpsat_w_gf == NULL || isotherm_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for act_x_pTgpsat_w_gf or its coefficients do not "
			"exist.");
		printf("\nReturn -1 for function call \"act_x_pTgv1v2psat_w_gf\".");
		return -1;
	}
	if (retAbsorption->act_g_Tx_wo_v == NULL || isotherm_par == NULL) {
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nFunction for act_g_Tx_wo_v or its coefficients do not "
			"exist.");
		printf("\nReturn -1 for function call \"act_x_pTgv1v2psat_w_gf\".");
		return -1;
	}

	// Avoid compiler warning C4100 by adding "do nothing" expression
	// "Do nothing" expression will be optimized out by compiler
	//
	(v1_m3mol) = (v1_m3mol);
	(v2_m3mol) = (v2_m3mol);

	// Return directly result of isotherm function
	//
	return retAbsorption->act_x_pTgpsat_w_gf(p_Pa, T_K,
		retAbsorption->act_g_Tx_wo_v, p_sat_Pa, isotherm_par);
}