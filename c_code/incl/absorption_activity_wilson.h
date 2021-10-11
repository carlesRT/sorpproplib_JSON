//////////////////////////////////
// absorption_activity_wilson.h //
//////////////////////////////////
#ifndef absorption_activity_wilson_h
#define absorption_activity_wilson_h


///////////////////////////////////////
// Definition of function prototypes //
///////////////////////////////////////
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
 *		with: A_12 and A_21 are parameters given by JSON-file
 *		----- if not A_12 * A_21 == 0.
 *
 *	d_lambda_12 = (d_lambda_12_c + d_lambda_12_t * (T - c))
 *	d_lambda_21 = (d_lambda_21_c + d_lambda_21_t * (T - c))
 *
 *		with: For d_lambda_12 and d_lambda_21, it is not necessary to chose an
 *		----- approach that depends on temperature. Instead, both parameters can
 *			  be modeled as constants when choosing the function
 *			  'absorption_activity_wilson__fdl_g1_Txv1v2'.
 *
 * Possible inputs required by user:
 * ---------------------------------
 *	T: Temperature in K
 *	x: Mole fraction in liquid phase in mol/mol
 *  v_1: Molar volume of first component in m³/mol
 *  v_2: Molar volume of second component in m³/mol
 *
 * Order of coefficients in JSON-file when modeling d_lambda_ij as constants:
 * --------------------------------------------------------------------------
 *	isotherm_par[0]	-> A_12			-> in J/mol
 * 	isotherm_par[1] -> A_21			-> in J/mol
 * 	isotherm_par[2] -> d_lambda_12	-> in J/mol
 * 	isotherm_par[3] -> d_lambda_21	-> in J/mol
 * 	isotherm_par[4] -> v_1			-> in m3/mol
 * 	isotherm_par[5] -> v_2			-> in m3/mol
 *
 * Order of coefficients in JSON-file when modeling d_lambda_ij
 * temperature-dependent:
 * ----------------------
 * 	isotherm_par[0] -> d_lambda_12_c-> in K
 * 	isotherm_par[1] -> d_lambda_12_T-> in -
 * 	isotherm_par[2] -> d_lambda_21_c-> in K
 * 	isotherm_par[3] -> d_lambda_21_T-> in -
 * 	isotherm_par[4] -> c			-> in -
 * 	isotherm_par[5] -> v_1			-> in m3/mol
 * 	isotherm_par[6] -> v_2			-> in m3/mol
 *
 */


/*
 * absorption_activity_wilson_fdl_g1_Txv1v2:
 * -----------------------------------------
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
 *		Equilibrium mole fraction in liquid phase of first component in mol/mol.
 *	double v1_m3mol:
 *		Equilibrium molar volume of first component in m³/mol.
 *	double v2_m3mol:
 *		Equilibrium molar volume of second component in m³/mol.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Wilson equation.
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
 *	01/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *	02/13/2020, by Mirko Engelpracht:
 *		Added possibility to use molar volumes as inputs.
 *
 */
double absorption_activity_wilson_fdl_g1_Txv1v2(double T_K, double x_molmol,
	double v1_m3mol, double v2_m3mol, double isotherm_par[]);


/*
 * absorption_activity_wilson_tdl_g1_Txv1v2:
 * -----------------------------------------
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
 *		Equilibrium mole fraction in liquid phase of first component in mol/mol.
 *	double v1_m3mol:
 *		Equilibrium molar volume of first component in m³/mol.
 *	double v2_m3mol:
 *		Equilibrium molar volume of second component in m³/mol.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Wilson equation.
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
 *	03/23/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_activity_wilson_tdl_g1_Txv1v2(double T_K, double x_molmol,
	double v1_m3mol, double v2_m3mol, double isotherm_par[]);


/*
 * absorption_activity_wilson_p_Txv1v2psat:
 * ----------------------------------------
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
 *	double (*func_gamma)(double, double, double, double, double[]):
 *		Function pointer for activity coefficient of first component.
 *	double p_sat_Pa:
 *		Saturation pressure of first component in Pa.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Wilson equation.
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
 *	01/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *	02/13/2020, by Mirko Engelpracht:
 *		Added possibility to use molar volumes as inputs.
 *	03/23/2020, by Mirko Engelpracht:
 *		Added function pointer for activity coefficient as function argument.
 *
 */
double absorption_activity_wilson_p_Txv1v2psat(double T_K, double x_molmol,
	double v1_m3mol, double v2_m3mol, double (*func_gamma)(double, double,
	double, double, double[]), double p_sat_Pa, double isotherm_par[]);


/*
 * absorption_activity_wilson_x_pTv1v2psat:
 * ----------------------------------------
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
 *	double (*func_gamma)(double, double, double, double, double[]):
 *		Function pointer for activity coefficient of first component.
 *	double p_sat_Pa:
 *		Saturation pressure of first component in Pa.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Wilson equation.
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
 *	03/19/2020, by Mirko Engelpracht:
 *		First implementation.
 *	03/23/2020, by Mirko Engelpracht:
 *		Added function pointer for activity coefficient as function argument.
 *
 */
double absorption_activity_wilson_x_pTv1v2psat(double p_Pa, double T_K,
	double v1_m3mol, double v2_m3mol, double (*func_gamma)(double, double,
	double, double, double[]), double p_sat_Pa, double isotherm_par[]);

#endif
