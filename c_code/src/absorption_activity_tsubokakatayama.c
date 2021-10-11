///////////////////////////////////////////
// absorption_activity_tsubokakatayama.c //
///////////////////////////////////////////
#include <math.h>
#include "absorption_activity_tsubokakatayama.h"


/////////////////////////////
// Definition of constants //
/////////////////////////////
#ifndef IDEAL_GAS_CONSTANT
#define IDEAL_GAS_CONSTANT 8.314462618f
#endif


/////////////////////////////
// Definition of functions //
/////////////////////////////
/*
 * General form of Tsuboka & Katayama equation:
 * --------------------------------------------
 * 	gamma_1 = exp(-ln(x_1 + x_2 * Lambda_21) + x_2 * (Lambda_21 / (x_1 + x_2 *
 *		Lambda_21) - Lambda_12 / (x_1 * Lambda_12 + x_2)) + ln(x_1 + x_2 *
 *		rho_21) - x_2 * (rho_21 / (x_1 + x_2 * rho_21) - rho_12 / (x_1 *
 *		rho_12 + x_2)))
 *
 * Temperature-dependent coefficients:
 * -----------------------------------
 * 	Lambda_12 = rho_21 * exp(-dlambda_12 / (R * T))
 *	Lambda_21 = rho_12 * exp(-dlambda_21 / (R * T))
 *	rho_12 = vm_1 / vm_2
 *	rho_21 = vm_2 / vm_1
 *
 * Possible inputs required by user:
 * ---------------------------------
 *	T: Temperature in K
 *	x: Mole fraction in liquid phase in mol/mol
 *  vm_1: Molar volume of first component in m³/mol
 *  vm_2: Molar volume of second component in m³/mol
 *
 * Order of coefficients in JSON-file:
 * -----------------------------------
 *	isotherm_par[0]	-> dlambda_12	-> in J/mol
 * 	isotherm_par[1] -> dlambda_21	-> in J/mol
 * 	isotherm_par[2] -> vm_1			-> in m³/mol
 * 	isotherm_par[3] -> vm_2			-> in m³/mol
 *
 */


/*
 * absorption_activity_tsubokakatayama_g1_Txv1v2:
 * ----------------------------------------------
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
 *		Array of doubles that contains coefficients of Tsuboka & Katayama
 *		equation.
 *
 * Returns:
 * --------
 *	double:
 *		Activity coefficient of first component.
 *
 * Remarks:
 * --------
 *	Uses molar volumes stored in JSON file when input v1_m3mol or v2_m3mol is
 * 	-1.
 *
 * History:
 * --------
 *	01/29/2020, by Mirko Engelpracht:
 *		First implementation.
 *	02/13/2020, by Mirko Engelpracht:
 *		Added possibility to use molar volumes as inputs.
 *
 */
double absorption_activity_tsubokakatayama_g1_Txv1v2(double T_K,
	double x_molmol, double v1_m3mol, double v2_m3mol, double isotherm_par[]) {
	// Calculate mole fractions
	//
	double x_1 = x_molmol;
	double x_2 = 1 - x_1;

	// Check, if molar volumes given by inputs need to be used
	//
	double rho_21;
	double rho_12;

	if (v1_m3mol < 0 || v2_m3mol < 0) {
		// Use molar volumes stored in JSON file
		//
		rho_21 = isotherm_par[3] / isotherm_par[2];
		rho_12 = isotherm_par[2] / isotherm_par[3];

	} else {
		// Use molar volumes given by inputs
		//
		rho_21 = v2_m3mol / v1_m3mol;
		rho_12 = v1_m3mol / v2_m3mol;

	}

	// Calculate temperature-dependent coefficients:
	//
	double Lambda_12 = rho_21 * exp(-isotherm_par[0] / (IDEAL_GAS_CONSTANT *
		T_K));
	double Lambda_21 = rho_12 * exp(-isotherm_par[1] / (IDEAL_GAS_CONSTANT *
		T_K));

	// Return activity coefficient of first component
	//
	double aux1 = x_1 + x_2 * Lambda_21;
	double aux2 = x_2 + x_1 * Lambda_12;
	double aux3 = x_1 + x_2 * rho_21;
	double aux4 = x_2 + x_1 * rho_12;

	return exp(-log(aux1) + x_2 * (Lambda_21 / aux1 - Lambda_12 / aux2) +
		log(aux3) - x_2 * (rho_21 / aux3 - rho_12 / aux4));
}


/*
 * absorption_activity_tsubokakatayama_p_Txv1v2psat:
 * -------------------------------------------------
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
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Tsuboka & Katayama
 *		equation.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure p_Pa in Pa.
 *
 * Remarks:
 * --------
 *	Uses molar volumes stored in JSON file when input v1_m3mol or v2_m3mol is
 * 	-1.
 *
 * History:
 * --------
 *	01/29/2020, by Mirko Engelpracht:
 *		First implementation.
 *	02/13/2020, by Mirko Engelpracht:
 *		Added possibility to use molar volumes as inputs.
 *
 */
double absorption_activity_tsubokakatayama_p_Txv1v2psat(double T_K,
	double x_molmol, double v1_m3mol, double v2_m3mol, double p_sat_Pa,
	double isotherm_par[]){
	// Calculate activity coefficient of first component
	//
	double gamma = absorption_activity_tsubokakatayama_g1_Txv1v2(T_K, x_molmol,
		v1_m3mol, v2_m3mol, isotherm_par);

	// Return equilibrium pressure
	//
	return gamma * x_molmol * p_sat_Pa;
}


/*
 * absorption_activity_tsubokakatayama_x_pTv1v2psat:
 * -------------------------------------------------
 *
 * Calculates equilibrium mole fraction x_molmol in mol/mol of first component
 * depending on equilibrium pressure p_Pa in Pa of first component, temperature
 * T_K in K, molar volume of first component in m³/mol, molar volume of second
 * component in m³/mol,and saturation pressure of first component p_sat_Pa in
 * Pa.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure p_Pa in Pa of first component.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double v1_m3mol:
 *		Equilibrium molar volume of first component in m³/mol.
 *	double v2_m3mol:
 *		Equilibrium molar volume of second component in m³/mol.
 *	double p_sat_Pa:
 *		Saturation pressure of first component in Pa.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Tsuboka & Katayama 
 *		equation.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium mole fraction x_molmol in mol/mol of first component.
 *
 * Remarks:
 * --------
 *	Uses molar volumes stored in JSON file when input v1_m3mol or v2_m3mol is
 * 	-1.
 *	Uses Newton-Raphson method for calculating equilibrium molar fraction.
 *
 * History:
 * --------
 *	03/23/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_activity_tsubokakatayama_x_pTv1v2psat(double p_Pa, double T_K,
	double v1_m3mol, double v2_m3mol, double p_sat_Pa, double isotherm_par[]) {
	// Initialize variables for using Newton-Raphson method
	//
	double x_guess_molmol = 0.5;
	double p_guess_Pa = p_Pa;
	double dp_guess_dx_Pamolmol = p_guess_Pa/x_guess_molmol;

	int counter_NRM = 0;
	const double tolerance = 1e-8;

	// Calculate temperature using Newton-Raphson method
	//
	for (p_guess_Pa = absorption_activity_tsubokakatayama_p_Txv1v2psat(T_K,
			x_guess_molmol, v1_m3mol, v2_m3mol, p_sat_Pa, isotherm_par);
	     fabs(p_guess_Pa - p_Pa)>tolerance && counter_NRM<50;
		 counter_NRM++) {
		// Calculate pressure depending on guess value for molar fraction and
		// temperature
		//
		p_guess_Pa = absorption_activity_tsubokakatayama_p_Txv1v2psat(T_K,
			x_guess_molmol, v1_m3mol, v2_m3mol, p_sat_Pa, isotherm_par);

		// Calculate derivative of the pressure with respect to molar fraction
		//
		dp_guess_dx_Pamolmol =
			(absorption_activity_tsubokakatayama_p_Txv1v2psat(T_K,
			x_guess_molmol+0.00001, v1_m3mol, v2_m3mol, p_sat_Pa, isotherm_par)
			- absorption_activity_tsubokakatayama_p_Txv1v2psat(T_K,
			x_guess_molmol-0.00001, v1_m3mol, v2_m3mol, p_sat_Pa, isotherm_par))
			/ 0.00002;

		// Update guess value for molar fraction
		// Only positive values are allowed
		//
		x_guess_molmol -= (p_guess_Pa - p_Pa) / dp_guess_dx_Pamolmol;

		if (x_guess_molmol < 0) {
			x_guess_molmol = 0;
		} else if (x_guess_molmol > 1) {
			x_guess_molmol = 1;
		}
	}

	// Return -1 when number of iterations exceeds 50
	//
	return (counter_NRM == 50 ? -1 : x_guess_molmol);
}
