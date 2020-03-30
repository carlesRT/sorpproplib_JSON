///////////////////////////////////
// absorption_activity_uniquac.h //
///////////////////////////////////
#ifndef absorption_activity_uniquac_h
#define absorption_activity_uniquac_h


///////////////////////////////////////
// Definition of function prototypes //
///////////////////////////////////////
/*
 * General form of UNIQUAC equation:
 * ---------------------------------
 *	gamma_1 = exp(ln(gamma_1_C) + ln(gamma_1_R))
 *
 *	with: ln(gamma_1_C) = (1 - V_1 + ln(V_1)) - z/2 * q_1 * (1 - V_1/F_1 +
 *	-----  	ln(V_1/F_1))
 *		  ln(gamma_1_R) = q_1 * (1 - ln((q_1 * x_1 + q_2 * x_2 * tau_21) /
 *		  	(q_1 * x_1 + q_2 * x_2)) - (q_1 * x_1 / (q_1 * x_1 + q_2 * x_2 *
 *		  	tau_21) + q_2 * x_2 * tau_12 / (q_1 * x_1 * tau_12 + q_2 * x_2)))
 *
 *	with: V_1 = r_1 / (x_1 * r_1 + x_2 * r_2)
 *	----- F_1 = q_1 / (x_1 * q_1 + x_2 * q_2)
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
	double isotherm_par[]);


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
	double isotherm_par[]);


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
 *		Array of doubles that contains coefficients of UNIQUAC equation.
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
	double (*func_gamma)(double, double, double[]), double p_sat_Pa,
	double isotherm_par[]);


/*
 * absorption_activity_uniquac_x_pTgpsat:
 * --------------------------------------
 *
 * Calculates equilibrium mole fraction x_molmol in mol/mol of first component
 * depending on equilibrium pressure p_Pa in Pa of first component, temperature
 * T_K in K, saturation pressure of first component p_sat_Pa in Pa, and function
 * pointer for activity coefficient of first component.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure p_Pa in Pa of first component.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double (*func_gamma)(double, double, double[]):
 *		Function pointer for activity coefficient of first component.
 *	double p_sat_Pa:
 *		Saturation pressure of first component in Pa.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of UNIQUAC equation.
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
 *	03/23/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_activity_uniquac_x_pTgpsat(double p_Pa, double T_K,
	double (*func_gamma)(double, double, double[]), double p_sat_Pa,
	double isotherm_par[]);

#endif