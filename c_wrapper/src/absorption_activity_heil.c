////////////////////////////////
// absorption_activity_heil.c //
////////////////////////////////
#include <math.h>
#include "absorption_activity_heil.h"


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
 * General form of Heil equation:
 * ------------------------------
 * 	gamma_1 = exp(-ln(x_1 + x_2 * Lambda_21) + x_2 * (Lambda_21 / (x_1 + x_2 * 
 *		Lambda_21) - Lambda_12 / (x_1 * Lambda_12 + x_2)) + x_2^2 * (tau_12 *
 *		(Lambda_21 / (x_1 + x_2 * Lambda_21))^2 + tau_12 * Lambda_12 / (x_2 +
 *		x_1 * Lambda_12)^2))
 *
 * Temperature-dependent coefficients:
 * -----------------------------------
 * 	Lambda_12 = vm_1 / vm_2 * exp(-tau_12)
 *	Lambda_21 = vm_2 / vm_1 * exp(-tau_21)
 *	tau_12 = dlambda_12 / (R * T)
 *	tau_21 = dlambda_21 / (R * T)
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
 * absorption_activity_heil_g1_Txv1v2:
 * -----------------------------------
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
 *		Array of doubles that contains coefficients of Heil equation.
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
double absorption_activity_heil_g1_Txv1v2(double T_K, double x_molmol,
	double v1_m3mol, double v2_m3mol, double isotherm_par[]) {
	// Calculate mole fractions
	//
	double x_1 = x_molmol;
	double x_2 = 1 - x_1;
	
	// Calculate temperature-dependent coefficients:
	//
	double tau_12 = isotherm_par[0] / (IDEAL_GAS_CONSTANT * T_K);
	double tau_21 = isotherm_par[1] / (IDEAL_GAS_CONSTANT * T_K);	

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
	
	double Lambda_12 = rho_21 * exp(-tau_12);
	double Lambda_21 = rho_12 * exp(-tau_21);
	
	// Return activity coefficient of first component
	//
	double aux1 = x_1 + x_2 * Lambda_21;
	double aux2 = x_2 + x_1 * Lambda_12;
	double aux3 = Lambda_21 / aux1;
	double aux4 = Lambda_12 / aux2;
	
	return exp(-log(aux1) + x_2 * (aux3 - aux4) + tau_12 * pow(x_2, 2) * (
		pow(aux3, 2) + aux4 / aux2));
}


/*
 * absorption_activity_heil_p_Txv1v2psat:
 * --------------------------------------
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
 *		Array of doubles that contains coefficients of Heil equation.
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
double absorption_activity_heil_p_Txv1v2psat(double T_K, double x_molmol,
	double v1_m3mol, double v2_m3mol, double p_sat_Pa, double isotherm_par[]) {
	// Calculate activity coefficient of first component
	//
	double gamma = absorption_activity_heil_g1_Txv1v2(T_K, x_molmol, v1_m3mol,
		v2_m3mol, isotherm_par);
	
	// Return equilibrium pressure
	//
	return gamma * x_molmol * p_sat_Pa;
}
