////////////////////////////////
// absorption_activity_nrtl.c //
////////////////////////////////
#include <math.h>
#include "absorption_activity_nrtl.h"


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
 * General form of NRTL equation:
 * --------------------------------
 * 	gamma_1 = exp(x_2^2 * [tau_21 * (G_21 / (x_1 + x_2 * G_21))^2 + 
 *		tau_12 * G_12 / (x_2 + x_1 * G_12)^2])
 *
 * Temperature-dependent coefficients:
 * -----------------------------------
 * 	G_12 = exp(-alpha_12 * tau_12)
 * 	G_21 = exp(-alpha_21 * tau_21)
 *	tau_12 = dg_12 / (R * T)
 *	tau_21 = dg_21 / (R * T)
 *	dg_12 = a_12 + b_12 * T   or   constant, depending on function
 *	dg_21 = a_21 + b_21 * T   or   constant, depending on function
 *	alpha_21 = alpha_12
 *
 *		with: For dg_12 and dg_21, it is not necessary to chose an approach that
 *		----- depends on temperature. Instead, dg_12 and dg_21 can be modeled
 *			  as constants when choosing the function
 *			  'absorption_activity_nrtl_fdg_g1_Tx'.
 *
 * Possible inputs required by user:
 * ---------------------------------
 *	T: Temperature in K
 *	x: Mole fraction in liquid phase in mol/mol
 *
 * Order of coefficients in JSON-file when modeling dg_12 & dg_21 as constants:
 * ----------------------------------------------------------------------------
 *	isotherm_par[0]	-> dg_12			-> in J/mol
 * 	isotherm_par[1] -> dg_21			-> in J/mol
 * 	isotherm_par[2] -> alpha_12			-> in -
 *
 * Order of coefficients in JSON-file when modeling dg_12 & dg_21 
 * temperature-dependent:
 * ----------------------
 *	isotherm_par[0]	-> a_12		-> in J/mol
 * 	isotherm_par[1] -> a_21		-> in J/mol/K
 * 	isotherm_par[2] -> b_12		-> in J/mol
 * 	isotherm_par[3] -> b_21		-> in J/mol/K
 * 	isotherm_par[4] -> alpha_12	-> -
 *
 */
 

/*
 * absorption_activity_nrtl_fdg_g1_Tx:
 * -----------------------------------
 *
 * Calculates activity coefficient of first component depending on temperature 
 * T_K in K and mole fraction in liquid phase x_molmol in mol/mol. Variables
 * dg_12 and dg_21 are modeled as constants.
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
double absorption_activity_nrtl_fdg_g1_Tx(double T_K, double x_molmol,
	double isotherm_par[]) {
	// Calculate mole fractions
	//
	double x_1 = x_molmol;
	double x_2 = 1 - x_1;
	
	// Calculate taus
	//
	double tau_12 = isotherm_par[0] / (IDEAL_GAS_CONSTANT * T_K);
	double tau_21 = isotherm_par[1] / (IDEAL_GAS_CONSTANT * T_K);
		
	// Calculate Gibbs free energies
	//
	double G_12 = exp(-isotherm_par[2] * tau_12);
	double G_21 = exp(-isotherm_par[2] * tau_21);
	
	// Return activity coefficient of first component
	//
	return exp(pow(x_2, 2) * (tau_21 * pow(G_21 / (x_1 + x_2 * G_21) , 2) +
		tau_12 * G_12 / pow(x_2 + x_1 * G_12, 2)));
}


/*
 * absorption_activity_nrtl_dgT_g1_Tx:
 * -----------------------------------
 *
 * Calculates activity coefficient of first component depending on temperature 
 * T_K in K and mole fraction in liquid phase x_molmol in mol/mol. Variables
 * dg_12 and dg_21 are modeled temperature-dependent.
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
double absorption_activity_nrtl_dgT_g1_Tx(double T_K, double x_molmol,
	double isotherm_par[]) {
	// Calculate mole fractions
	//
	double x_1 = x_molmol;
	double x_2 = 1 - x_1;
	
	// Calculate temperature-dependent dg_12 and dg_21
	//
	double dg_12 = isotherm_par[0] + isotherm_par[2] * T_K;
	double dg_21 = isotherm_par[1] + isotherm_par[3] * T_K;
	
	// Calculate taus
	//
	double tau_12 = dg_12 / (IDEAL_GAS_CONSTANT * T_K);
	double tau_21 = dg_21 / (IDEAL_GAS_CONSTANT * T_K);
	
	// Calculate Gibbs free energies
	//
	double G_12 = exp(-isotherm_par[4] * tau_12);
	double G_21 = exp(-isotherm_par[4] * tau_21);
	
	// Return activity coefficient of first component
	//
	return exp(pow(x_2, 2) * (tau_21 * pow(G_21 / (x_1 + x_2 * G_21) , 2) +
		tau_12 * G_12 / pow(x_2 + x_1 * G_12, 2)));
}


/*
 * absorption_activity_nrtl_p_Txgpsat:
 * -----------------------------------
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
double absorption_activity_nrtl_p_Txgpsat(double T_K, double x_molmol,
	double p_sat_Pa, double (*func_gamma)(double, double, double[]),
	double isotherm_par[]) {
	// Calculate activity coefficient of first component
	//
	double gamma = func_gamma(T_K, x_molmol, isotherm_par);
	
	// Return equilibrium pressure
	//
	return gamma * x_molmol * p_sat_Pa;
}
