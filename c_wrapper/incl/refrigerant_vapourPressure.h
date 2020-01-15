//////////////////////////////////
// refrigerant_vapourPressure.h //
//////////////////////////////////
#ifndef refrigerant_vapourPressure_h
#define refrigerant_vapourPressure_h


///////////////////////////////////////
// Definition of function prototypes //
///////////////////////////////////////
/*
 * General form of equation for vapour pressure:
 * ---------------------------------------------
 * 	p_sat = p_crit * exp(1/theta *
 *		[fac1*THETA^exp1 + fac2*THETA^exp2 + fac3*THETA^exp3 + fac4*THETA^exp4 +
 *		 fac5*THETA^exp5 + fac6*THETA^exp6])
 *
 * Temperature-dependent coefficients:
 * -----------------------------------
 * 	theta = T/T_crit
 * 	THETA = 1 - T/T_crit
 *
 * Inputs required by user:
 * ------------------------
 *	T: Temperature in K
 *
 * Order of coefficients in JSON-file:
 * -----------------------------------
 *	isotherm_par[0]		-> Tcrit	-> in K
 * 	isotherm_par[1] 	-> pcrit	-> in Pa
 * 	isotherm_par[2] 	-> fac1		-> in -
 * 	isotherm_par[3] 	-> exp1		-> in -
 * 	isotherm_par[4] 	-> fac2		-> in -
 * 	isotherm_par[5] 	-> exp2		-> in -
 * 	isotherm_par[6] 	-> fac3		-> in -
 * 	isotherm_par[7] 	-> exp3		-> in -
 * 	isotherm_par[8] 	-> fac4		-> in -
 * 	isotherm_par[9] 	-> exp4		-> in -
 * 	isotherm_par[10] 	-> fac5		-> in -
 * 	isotherm_par[11] 	-> exp5		-> in -
 * 	isotherm_par[12] 	-> fac6		-> in -
 * 	isotherm_par[13] 	-> exp6		-> in -
 *
 */
 

/*
 * refrigerant_p_sat:
 * ------------------
 *
 * Calculates vapour pressure in Pa depending on saturation temperature T in K.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Saturation temperature in K.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for equation of vapour
 *		pressure.
 *
 * Returns:
 * --------
 *	double:
 *		Vapour pressure in Pa.
 *
 * History:
 * --------
 *	01/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_p_sat(double T_K, double refrigerant_par[]);
 

/*
 * refrigerant_dp_sat_dT:
 * ----------------------
 *
 * Calculates derivative of vapour pressure wrt. temperature in Pa/K depending
 * on saturation temperature T in K.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Saturation temperature in K.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for equation of vapour
 *		pressure.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of vapour pressure wrt. temperature in Pa/K.
 *
 * History:
 * --------
 *	01/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_dp_sat_dT(double T_K, double refrigerant_par[]);

#endif
