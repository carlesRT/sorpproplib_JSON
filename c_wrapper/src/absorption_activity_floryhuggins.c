////////////////////////////////////////
// absorption_activity_floryhuggins.c //
////////////////////////////////////////
#include <math.h>
#include "absorption_activity_floryhuggins.h"


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
 * General form of Flory-Huggins equation:
 * ---------------------------------------
 * 	gamma_1 = exp(ln(1 - (1 - 1 / r) * Phi_2) + (1 / r) * Phi_2 + Chi * Phi_2^2)
 *
 * Temperature-dependent coefficients:
 * -----------------------------------
 * 	Phi_2 = r * x_2 / (x_1 + r * x_2)
 * 	Chi = w_0 / (k_Boltzmann * T) * (1 + w_1 / T)
 *
 * Possible inputs required by user:
 * ---------------------------------
 *	T: Temperature in K
 *	x: Mole fraction in liquid phase in mol/mol
 *
 * Order of coefficients in JSON-file:
 * -----------------------------------
 *	isotherm_par[0]	-> r	-> in -
 * 	isotherm_par[1] -> w_0	-> in J/mol
 * 	isotherm_par[2] -> w_1	-> in K
 *
 */


/*
 * absorption_activity_floryhuggins_g1_Tx:
 * ---------------------------------------
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
 *		Array of doubles that contains coefficients of Flory-Huggins equation.
 *
 * Returns:
 * --------
 *	double:
 *		Activity coefficient of first component.
 *
 * History:
 * --------
 *	01/29/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_activity_floryhuggins_g1_Tx(double T_K, double x_molmol,
	double isotherm_par[]) {
	// Calculate mole fractions
	//
	double x_1 = x_molmol;
	double x_2 = 1 - x_1;
	
	// Calculate temperature-dependent coefficients:
	// Boltzmann constant is already included in coefficient w_0.
	//
	double Phi_2 = isotherm_par[0] * x_2 / (x_1 + isotherm_par[0] * x_2);
	double Chi = isotherm_par[1] / T_K * (1 + isotherm_par[2] / T_K);
	
	// Return activity coefficient of first component
	//
	return exp(log(1 - (1 - 1 / isotherm_par[0]) * Phi_2) + (1 - 1 / 
		isotherm_par[0]) * Phi_2 + Chi * pow(Phi_2, 2));
}


/*
 * absorption_activity_floryhuggins_p_Txpsat:
 * ------------------------------------------
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
 *	double p_sat_Pa:
 *		Saturation pressure of first component in Pa.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Flory-Huggins equation.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure p_Pa in Pa.
 *
 * History:
 * --------
 *	01/29/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_activity_floryhuggins_p_Txpsat(double T_K, double x_molmol,
	double p_sat_Pa, double isotherm_par[]) {
	// Calculate activity coefficient of first component
	//
	double gamma = absorption_activity_floryhuggins_g1_Tx(T_K, x_molmol,
		isotherm_par);
	
	// Return equilibrium pressure
	//
	return gamma * x_molmol * p_sat_Pa;
}
