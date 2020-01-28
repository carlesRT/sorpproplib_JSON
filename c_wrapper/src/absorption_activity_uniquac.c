///////////////////////////////////
// absorption_activity_uniquac.c //
///////////////////////////////////
#include <math.h>
#include "absorption_activity_uniquac.h"


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
 * General form of UNIQUAC equation:
 * ---------------------------------
 * 	gamma_1 = exp(ln(phi_1 / x_1) + z / 2 * q_1 * ln(theta_1 / phi_1) + phi_2 *
 *		(l_1 - r_1 / r_2 * l_2) - q_1 * ln(theta_1 + theta_2 * tau_21) +
 *		phi_2 * q_1 * (tau_21 / (phi_1 + phi_2 * tau_21) - tau_12 / 
 *		(phi_2 + phi_1 * tau_12)))
 *
 *	with: l_1 = z / 2 * (r_1 - q_1) - (r_1 - 1)
 *	----- l_2 = z / 2 * (r_2 - q_2) - (r_2 - 1)
 *		  phi_1 = r_1 * x_1 / (r_1 * x_1 + r_2 * x_2)
 *		  phi_2 = r_2 * x_2 / (r_1 * x_1 + r_2 * x_2)
 *		  theta_1 = q_1 * x_1 / (q_1 * x_1 + q_2 * x_2)
 *		  theta_2 = q_2 * x_1 / (q_1 * x_1 + q_2 * x_2)
 *		  
 * Temperature-dependent coefficients:
 * -----------------------------------
 *	tau_12 = -du_12 / (R * T)
 *	tau_21 = -du_21 / (R * T)
 *	du_12 = a_12 + b_12 * T   or   constant, depending on function
 *	du_21 = a_21 + b_21 * T   or   constant, depending on function
 *
 *		with: For du_12 and du_21, it is not necessary to chose an approach that
 *		----- depends on temperature. Instead, du_12 and du_21 can be modeled
 *			  as constants when choosing the function
 *			  'absorption_activity_uniquac_fdu_g1_Tx'.
 *
 * Possible inputs required by user:
 * ---------------------------------
 *	T: Temperature in K
 *	x: Mole fraction in liquid phase in mol/mol
 *
 * Order of coefficients in JSON-file when modeling du_12 & du_21 as constants:
 * ----------------------------------------------------------------------------
 *	isotherm_par[0]	-> du_12	-> in J/mol
 * 	isotherm_par[1] -> du_21	-> in J/mol
 *	isotherm_par[2]	-> q_1		-> in -
 * 	isotherm_par[3] -> q_2		-> in -
 *	isotherm_par[4]	-> r_1		-> in -
 * 	isotherm_par[5] -> r_2		-> in -
 * 	isotherm_par[6] -> z		-> in -
 *
 * Order of coefficients in JSON-file when modeling du_12 & du_21 
 * temperature-dependent:
 * ----------------------
 *	isotherm_par[0]	-> a_12		-> in J/mol
 * 	isotherm_par[1] -> a_21		-> in J/mol
 *	isotherm_par[2]	-> b_12		-> in J/mol/K
 * 	isotherm_par[3] -> b_21		-> in J/mol/K
 *	isotherm_par[4]	-> q_1		-> in -
 * 	isotherm_par[5] -> q_2		-> in -
 *	isotherm_par[6]	-> r_1		-> in -
 * 	isotherm_par[7] -> r_2		-> in -
 * 	isotherm_par[8] -> z		-> in -
 *
 */


/*
 * absorption_activity_uniquac_fdu_g1_Tx:
 * --------------------------------------
 *
 * Calculates activity coefficient of first component depending on temperature 
 * T_K in K and mole fraction in liquid phase x_molmol in mol/mol. Variables
 * du_12 and du_21 are modeled as constants.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double x_molmol:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of NRTL-FDG equation.
 *
 * Returns:
 * --------
 *	double:
 *		Activity coefficient of first component.
 *
 * History:
 * --------
 *	01/28/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_activity_uniquac_fdu_g1_Tx(double T_K, double x_molmol,
	double isotherm_par[]) {
	// Calculate mole fractions
	//
	double x_1 = x_molmol;
	double x_2 = 1 - x_1;

	// Calculate segment fractions
	//
	double V_1 = isotherm_par[4] / (isotherm_par[4] * x_1 + 
		isotherm_par[5] * x_2);
	
	// Calculate area fractions
	//
	double F_1 = isotherm_par[2] / (isotherm_par[2] * x_1 +
		isotherm_par[3] * x_2);
		
	// Calculate UNIQUAC binary interaction parameters
	//
	double tau_12 = exp(-isotherm_par[0] / (IDEAL_GAS_CONSTANT * T_K));
	double tau_21 = exp(-isotherm_par[1] / (IDEAL_GAS_CONSTANT * T_K));
	
	// Return activity coefficient of first component
	//
	double combCont = (1 - V_1 + log(V_1)) - (isotherm_par[6] / 2) * 
		isotherm_par[2] * (1 - V_1 / F_1 + log(V_1 / F_1));
	double resiCont = isotherm_par[2] * (1 - log((isotherm_par[2] * x_1 + 
		isotherm_par[3] * x_2 * tau_21) / (isotherm_par[2] * x_1 + 
		isotherm_par[3] * x_2)) - ((isotherm_par[2] * x_1) / (isotherm_par[2] * 
		x_1 + isotherm_par[3] * x_2 * tau_21) + (isotherm_par[3] * x_2 * 
		tau_12) / (isotherm_par[2] * x_1 * tau_12 + isotherm_par[3] * x_2)));

	return exp(combCont + resiCont);
}


/*
 * absorption_activity_uniquac_duT_g1_Tx:
 * --------------------------------------
 *
 * Calculates activity coefficient of first component depending on temperature 
 * T_K in K and mole fraction in liquid phase x_molmol in mol/mol. Variables
 * du_12 and du_21 are modeled temperature-dependent.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double x_molmol:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of NRTL-FDG equation.
 *
 * Returns:
 * --------
 *	double:
 *		Activity coefficient of first component.
 *
 * History:
 * --------
 *	01/28/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_activity_uniquac_duT_g1_Tx(double T_K, double x_molmol,
	double isotherm_par[]) {
	// Calculate mole fractions
	//
	double x_1 = x_molmol;
	double x_2 = 1 - x_1;

	// Calculate segment fractions
	//
	double V_1 = isotherm_par[6] / (isotherm_par[6] * x_1 + 
		isotherm_par[7] * x_2);
	
	// Calculate area fractions
	//
	double F_1 = isotherm_par[4] / (isotherm_par[4] * x_1 +
		isotherm_par[5] * x_2);
	
	// Calculate temperature-dependent du_12 and du_21
	//
	double du_12 = isotherm_par[0] + isotherm_par[2] * T_K;
	double du_21 = isotherm_par[1] + isotherm_par[3] * T_K;
		
	// Calculate UNIQUAC binary interaction parameters
	//
	double tau_12 = exp(-du_12 / (IDEAL_GAS_CONSTANT * T_K));
	double tau_21 = exp(-du_21 / (IDEAL_GAS_CONSTANT * T_K));
	
	// Return activity coefficient of first component
	//
	double combCont = (1 - V_1 + log(V_1)) - (isotherm_par[8] / 2) * 
		isotherm_par[4] * (1 - V_1 / F_1 + log(V_1 / F_1));
	double resiCont = isotherm_par[4] * (1 - log((isotherm_par[4] * x_1 + 
		isotherm_par[5] * x_2 * tau_21) / (isotherm_par[4] * x_1 + 
		isotherm_par[5] * x_2)) - ((isotherm_par[4] * x_1) / (isotherm_par[4] * 
		x_1 + isotherm_par[5] * x_2 * tau_21) + (isotherm_par[5] * x_2 * 
		tau_12) / (isotherm_par[4] * x_1 * tau_12 + isotherm_par[5] * x_2)));

	return exp(combCont + resiCont);
}


/*
 * absorption_activity_uniquac_p_Txgpsat:
 * --------------------------------------
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
 *	double (*func_gamma)(double, double, double[]):
 *		Function pointer for activity coefficient of first component.
 *	double p_sat_Pa:
 *		Saturation pressure of first component in Pa. *
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Wilson equation.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure p_Pa in Pa.
 *
 * History:
 * --------
 *	01/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_activity_uniquac_p_Txgpsat(double T_K, double x_molmol,
	double p_sat_Pa, double (*func_gamma)(double, double, double[]),
	double isotherm_par[]) {
	// Calculate activity coefficient of first component
	//
	double gamma = func_gamma(T_K, x_molmol, isotherm_par);
	
	// Return equilibrium pressure
	//
	return gamma * x_molmol * p_sat_Pa;
}
