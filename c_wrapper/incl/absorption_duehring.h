///////////////////////////
// absorption_duehring.h //
///////////////////////////
#ifndef absorption_duehring_h
#define absorption_duehring_h


///////////////////////////////////////
// Definition of function prototypes //
///////////////////////////////////////
/*
 * General form of Duehring equation:
 * ----------------------------------
 * 	p = 1/r * exp(C + D/T_ref + E/T_ref^2)
 *
 * Temperature-dependent coefficients:
 * -----------------------------------
 * 	T_ref = (n*T_cor + m - B) / A + q
 *	T_cor = T - 273.15
 *
 *		with: A = a0 + a1*w_cor + a2*w_cor^2 + a3*w_cor^3
 *		----- B = b0 + b1*w_cor + b2*w_cor^2 + b3*w_cor^3
 *			  w_cor = w * 100
 *
 * Possible inputs required by user:
 * ---------------------------------
 *	p: Pressure in Pa
 *	T: Temperature in K
 *	w: Loading in kg/kg
 *
 * Order of coefficients in JSON-file:
 * -----------------------------------
 *	isotherm_par[0]	-> a0	-> in -
 * 	isotherm_par[1] -> a1	-> in -
 * 	isotherm_par[2] -> a2	-> in -
 * 	isotherm_par[3] -> a3	-> in -
 * 	isotherm_par[4] -> b0	-> in -
 * 	isotherm_par[5] -> b1	-> in -
 * 	isotherm_par[6] -> b2	-> in -
 * 	isotherm_par[7] -> b3	-> in -
 * 	isotherm_par[8] -> C	-> in -
 * 	isotherm_par[9] -> D	-> in K
 * 	isotherm_par[10] -> E	-> in K^2
 * 	isotherm_par[11] -> m	-> in -
 * 	isotherm_par[12] -> n	-> in -
 * 	isotherm_par[13] -> q	-> in - 
 * 	isotherm_par[14] -> r	-> in 1/Pa
 *
 */


/*
 * absorption_duehring_w_pT:
 * -------------------------
 *
 * Calculates equilibrium loading w in kg/kg depending on equilibrium pressure
 * p in Pa and equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Duehring equation.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium loading in kg/kg.
 *
 * Remarks:
 * --------
 *	Uses Newton-Raphson method for calculating equlibrium loading.
 *
 * History:
 * --------
 *	01/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_duehring_w_pT(double p_Pa, double T_K,
	double isotherm_par[]);


/*
 * absorption_duehring_p_wT:
 * -------------------------
 *
 * Calculates equilibrium pressure p in Pa depending on equilibrium loading w
 * in kg/kg and equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Duehring equation.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium pressure in Pa.
 *
 * History:
 * --------
 *	01/26/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_duehring_p_wT(double w_kgkg, double T_K,
	double isotherm_par[]);


/*
 * absorption_duehring_T_pw:
 * -------------------------
 *
 * Calculates equilibrium temperature in K depending on equilibrium pressure p
 * in Pa and equilibrium loading w in kg/kg.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Duehring equation.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium temperature in K.
 *
 * Remarks:
 * --------
 *	Uses reduced quadratic equation for calculating equlibrium temperature.
 *
 * History:
 * --------
 *	01/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_duehring_T_pw(double p_Pa, double w_kgkg,
	double isotherm_par[]);


/*
 * absorption_duehring_dw_dp_pT:
 * -----------------------------
 *
 * Calculates derivative of equilibrium loading w with respect to pressure p 
 * in kg/kg/Pa depending on equilibrium pressure p in Pa and equilibrium 
 * temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Duehring equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. pressure in kg/kg/Pa.
 *
 * Remarks:
 * --------
 *	Derivative is calculated numerically by the symmetric derivative using
 *  h = 0.001 Pa as small change.
 *
 * History:
 * --------
 *	01/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_duehring_dw_dp_pT(double p_Pa, double T_K,
	double isotherm_par[]);


/*
 * absorption_duehring_dw_dT_pT:
 * -----------------------------
 *
 * Calculates derivative of equilibrium loading w with respect to temperature T
 * in kg/kg/K depending on equilibrium pressure p in Pa and equilibrium 
 * temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Duehring equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. temperature in kg/kg/K.
 *
 * Remarks:
 * --------
 *	Derivative is calculated numerically by the symmetric derivative using
 *  h = 0.001 K as small change.
 *
 * History:
 * --------
 *	01/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_duehring_dw_dT_pT(double p_Pa, double T_K,
	double isotherm_par[]);

	
/*
 * absorption_duehring_dp_dw_wT:
 * -----------------------------
 *
 * Calculates derivative of equilibrium pressure p with respect to loading 
 * w in kgPa/kg depending on equilibrium loading w in kg/kg and equilibrium 
 * temperature T in K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Duehring equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. loading in kgPa/kg.
 *
 * History:
 * --------
 *	01/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_duehring_dp_dw_wT(double w_kgkg, double T_K,
	double isotherm_par[]);


/*
 * absorption_duehring_dp_dT_wT:
 * -----------------------------
 *
 * Calculates derivative of equilibrium pressure p with respect to temperature 
 * T in Pa/K depending on equilibrium loading w in kg/kg and equilibrium 
 * temperature T in K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Duehring equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. temperature in Pa/K.
 *
 * History:
 * --------
 *	01/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_duehring_dp_dT_wT(double w_kgkg, double T_K,
	double isotherm_par[]);

#endif