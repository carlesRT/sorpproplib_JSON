////////////////////////////////////////
// absorption_activity_floryhuggins.h //
////////////////////////////////////////
#ifndef absorption_activity_floryhuggins_h
#define absorption_activity_floryhuggins_h


///////////////////////////////////////
// Definition of function prototypes //
///////////////////////////////////////
/*
 * General form of Flory-Huggins equation:
 * ---------------------------------------
 * 	gamma_1 = exp(ln(1 - (1 - 1 / r) * Phi_2) + (1 / r) * Phi_2 + Chi * Phi_2^2)
 *
 * Temperature-dependent coefficients:
 * -----------------------------------
 * 	Phi_2 = r * x_2 / (x_1 + r * x_2)
 * 	Chi = w_0 / T * (1 + w_1 / T)
 *
 * Possible inputs required by user:
 * ---------------------------------
 *	T: Temperature in K
 *	x: Mole fraction in liquid phase in mol/mol
 *
 * Order of coefficients in JSON-file:
 * -----------------------------------
 *	isotherm_par[0]	-> r				-> in -
 * 	isotherm_par[1] -> w_0/k_Boltzmann	-> in J/mol
 * 	isotherm_par[2] -> w_1				-> in K
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
	double isotherm_par[]);


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
	double p_sat_Pa, double isotherm_par[]);


/*
 * absorption_activity_floryhuggins_x_pTpsat:
 * ------------------------------------------
 *
 * Calculates equilibrium mole fraction x_molmol in mol/mol of first component
 * depending on equilibrium pressure p_Pa in Pa of first component, temperature
 * T_K in K, and saturation pressure of first component p_sat_Pa in Pa.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure p_Pa in Pa of first component.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation pressure of first component in Pa.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Flory-Huggins equation.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium mole fraction x_molmol in mol/mol of first component.
 *
 * Remarks:
 * --------
 *	Uses Newton-Raphson method for calculating equilibrium molar fraction.
 *
 * History:
 * --------
 *	03/24/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_activity_floryhuggins_x_pTgpsat(double p_Pa, double T_K,
	double p_sat_Pa, double isotherm_par[]);

#endif