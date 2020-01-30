///////////////////////////////////////////
// absorption_activity_tsubokakatayama.h //
///////////////////////////////////////////
#ifndef absorption_activity_tsubokakatayama_h
#define absorption_activity_tsubokakatayama_h


///////////////////////////////////////
// Definition of function prototypes //
///////////////////////////////////////
/*
 * General form of Tsubo & Kakatayama equation:
 * --------------------------------------------
 * 	gamma_1 = exp(-ln(x_1 + x_2 * Lambda_21) + x_2 * (Lambda_21 / (x_1 + x_2 *
 *		Lambda_21) - Lambda_12 / (x_1 * Lambda_12 + x_2)) + ln(x_1 + x_2 * 
 *		rho_21) + x_2 * (rho_21 / (x_1 + x_2 * rho_21) - rho_12 / (x_1 * 
 *		rho_12 + x_2)))
 *
 * Temperature-dependent coefficients:
 * -----------------------------------
 * 	Lambda_12 = rho_12 * exp(-dlambda_12 / (R * T))
 *	Lambda_21 = rho_21 * exp(-dlambda_21 / (R * T))
 *	rho_12 = vm_1 / vm_2
 *	rho_21 = vm_2 / vm_1
 *
 * Possible inputs required by user:
 * ---------------------------------
 *	T: Temperature in K
 *	x: Mole fraction in liquid phase in mol/mol
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
 * absorption_activity_tsubokakatayama_g1_Tx:
 * ------------------------------------------
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
 *		Array of doubles that contains coefficients of Tsubo & Kakatayama
 *		equation.
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
double absorption_activity_tsubokakatayama_g1_Tx(double T_K, double x_molmol,
	double isotherm_par[]);


/*
 * absorption_activity_tsubokakatayama_p_Txpsat:
 * ---------------------------------------------
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
 *		Array of doubles that contains coefficients of Tsubo & Kakatayama
 *		equation.
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
double absorption_activity_tsubokakatayama_p_Txpsat(double T_K, double x_molmol,
	double p_sat_Pa, double isotherm_par[]);

#endif