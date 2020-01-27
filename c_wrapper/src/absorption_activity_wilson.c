//////////////////////////////////
// absorption_activity_wilson.c //
//////////////////////////////////
#include <math.h>
#include "absorption_activity_wilson.h"


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
 * General form of Wilson equation:
 * --------------------------------
 * 	gamma_1 = exp(-ln(x_1 + A_12 * x_2) + x_2 * [A_12 / (x_1 + A_12 * x_2) - 
 *		A_21 / (x_2 + A_21 * x_1)])
 *
 * Temperature-dependent coefficients:
 * -----------------------------------
 * 	A_12 = v_2 / v_1 * exp(-d_lambda_12 / (R * T))
 * 	A_21 = v_1 / v_2 * exp(-d_lambda_21 / (R * T))
 *
 *		with: A_12 and A_21 are parameters given by array containing
 *		----- coefficients if A_12 * A_21 == 0.
 *
 * Possible inputs required by user:
 * ---------------------------------
 *	T: Temperature in K
 *	x: Mole fraction in liquid phase
 *
 * Order of coefficients in JSON-file:
 * -----------------------------------
 *	isotherm_par[0]	-> A_12			-> in J/mol
 * 	isotherm_par[1] -> A_21			-> in J/mol
 * 	isotherm_par[2] -> d_lambda_12	-> in J/mol
 * 	isotherm_par[3] -> d_lambda_21	-> in J/mol
 * 	isotherm_par[4] -> v_1			-> in m3/kg
 * 	isotherm_par[5] -> v_2			-> in m3/kg
 *
 */


/*
 * absorption_activity_wilson_g1_Tx:
 * ---------------------------------
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
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Wilson equation.
 *
 * Returns:
 * --------
 *	double:
 *		Activity coefficient of first component.
 *
 * History:
 * --------
 *	01/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_activity_wilson_g1_Tx(double T_K, double x_molmol,
	double isotherm_par[]) {
	// Calculate mole fractions
	//
	double x_1 = x_molmol;
	double x_2 = 1 - x_1;
	
	// Calculate Lambdas depending on coefficients of Wilson equation
	//
	double A_12;
	double A_21;
	
	if (isotherm_par[0] * isotherm_par[0] == 0) {
		// Lambdas depend on temperature
		//
		A_12 = isotherm_par[5] / isotherm_par[4] * exp(isotherm_par[2] /
			(IDEAL_GAS_CONSTANT * T_K));
		A_21 = isotherm_par[4] / isotherm_par[5] * exp(isotherm_par[3] /
			(IDEAL_GAS_CONSTANT * T_K));
		
	} else {
		// Lambdas do not depend on temperature
		//
		A_12 = isotherm_par[0];
		A_21 = isotherm_par[1];
		
	}
	
	// Return activity coefficient of first component
	//
	return exp(-log(x_1 + A_12 * x_2) + x_2 * (A_12 / (x_1 + A_12 * x_2) - 
		A_21 / (x_2 + A_21 * x_1)));
}
	