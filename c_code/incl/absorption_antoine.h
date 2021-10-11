//////////////////////////
// absorption_antoine.h //
//////////////////////////
#ifndef absorption_antoine_h
#define absorption_antoine_h


///////////////////////////////////////
// Definition of function prototypes //
///////////////////////////////////////
/*
 * General form of Antoine equation:
 * ---------------------------------
 * 	p = d* 10^(SUM_i=0^k [(A_i + 1000 * B_i / (T - c)) * (100 * X)^i])
 *
 *		with: k <= 4
 *
 * Possible inputs required by user:
 * ---------------------------------
 *	p: Pressure in Pa
 *	T: Temperature in K
 *	X: Concentration in kg/kg
 *
 * Order of coefficients in JSON-file:
 * -----------------------------------
 *	isotherm_par[0]	-> A0	-> in -
 * 	isotherm_par[1] -> A1	-> in -
 * 	isotherm_par[2] -> A2	-> in -
 * 	isotherm_par[3] -> A3	-> in -
 * 	isotherm_par[4] -> A4	-> in -
 * 	isotherm_par[5] -> B0	-> in K
 * 	isotherm_par[6] -> B1	-> in K
 * 	isotherm_par[7] -> B2	-> in K
 * 	isotherm_par[8] -> B3	-> in K
 * 	isotherm_par[9] -> B4	-> in K
 * 	isotherm_par[10]-> c	-> in K
 * 	isotherm_par[11]-> d	-> in Pa
 *
 */


/*
 * absorption_antoine_X_pT:
 * ------------------------
 *
 * Calculates equilibrium concentration X in kg/kg depending on equilibrium
 * pressure p in Pa and equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Antoine equation.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium concentration in kg/kg.
 *
 * Remarks:
 * --------
 *	Uses Newton-Raphson method for calculating equilibrium concentration.
 *
 * History:
 * --------
 *	03/17/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_antoine_X_pT(double p_Pa, double T_K,
	double isotherm_par[]);


/*
 * absorption_antoine_p_XT:
 * -------------------------
 *
 * Calculates equilibrium pressure p in Pa depending on equilibrium
 * concentration X in kg/kg, and equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 *	double X_kgkg:
 *		Equilibrium concentration in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Antoine equation.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium pressure in Pa.
 *
 * History:
 * --------
 *	03/17/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_antoine_p_XT(double X_kgkg, double T_K,
	double isotherm_par[]);


/*
 * absorption_antoine_T_pX:
 * ------------------------
 *
 * Calculates equilibrium temperature in K depending on equilibrium pressure p
 * in Pa and equilibrium concentration X in kg/kg.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double X_kgkg:
 *		Equilibrium concentration in kg/kg.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Antoine equation.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium temperature in K.
 *
 * Remarks:
 * --------
 *	Uses Newton-Raphson method for calculating equilibrium concentration.
 *
 * History:
 * --------
 *	03/18/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_antoine_T_pX(double p_Pa, double X_kgkg,
	double isotherm_par[]);


/*
 * absorption_antoine_dX_dp_pT:
 * ----------------------------
 *
 * Calculates derivative of equilibrium concentration X with respect to pressure
 * p in kg/kg/Pa depending on equilibrium pressure p in Pa and equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Antoine equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium concentration wrt. pressure in kg/kg/Pa.
 *
 * Remarks:
 * --------
 *	Derivative is calculated numerically by the symmetric derivative approach
 *  using h = 0.001 Pa as small change.
 *
 * History:
 * --------
 *	03/18/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_antoine_dX_dp_pT(double p_Pa, double T_K,
	double isotherm_par[]);


/*
 * absorption_antoine_dX_dT_pT:
 * ----------------------------
 *
 * Calculates derivative of equilibrium concentration X with respect to
 * temperature T in kg/kg/K depending on equilibrium pressure p in Pa and
 * equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Antoine equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium concentration wrt. temperature in kg/kg/K.
 *
 * Remarks:
 * --------
 *	Derivative is calculated numerically by the symmetric derivative approach
 *  using h = 0.001 K as small change.
 *
 * History:
 * --------
 *	03/18/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_antoine_dX_dT_pT(double p_Pa, double T_K,
	double isotherm_par[]);


/*
 * absorption_antoine_dp_dX_XT:
 * ----------------------------
 *
 * Calculates derivative of equilibrium pressure p with respect to concentration
 * X in kgPa/kg depending on equilibrium concentration X in kg/kg and
 * equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 *	double X_kgkg:
 *		Equilibrium concentration in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Antoine equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. concentration in kgPa/kg.
 *
 * History:
 * --------
 *	03/18/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_antoine_dp_dX_XT(double X_kgkg, double T_K,
	double isotherm_par[]);


/*
 * absorption_antoine_dp_dT_XT:
 * ----------------------------
 *
 * Calculates derivative of equilibrium pressure p with respect to temperature
 * T in Pa/K depending on equilibrium concentration X in kg/kg and equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 *	double X_kgkg:
 *		Equilibrium concentration in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Antoine equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. temperature in Pa/K.
 *
 * History:
 * --------
 *	03/18/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_antoine_dp_dT_XT(double X_kgkg, double T_K,
	double isotherm_par[]);

#endif
