/////////////////////////////////
// refrigerant_vaporPressure.h //
/////////////////////////////////
#ifndef refrigerant_vaporPressure_h
#define refrigerant_vaporPressure_h


///////////////////////////////////////
// Definition of function prototypes //
///////////////////////////////////////
/*
 * General form of equation for vapor pressure:
 * --------------------------------------------
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
 *	refrigerant_par[0]	-> Tcrit	-> in K
 * 	refrigerant_par[1] 	-> pcrit	-> in Pa
 * 	refrigerant_par[2] 	-> fac1		-> in -
 * 	refrigerant_par[3] 	-> exp1		-> in -
 * 	refrigerant_par[4] 	-> fac2		-> in -
 * 	refrigerant_par[5] 	-> exp2		-> in -
 * 	refrigerant_par[6] 	-> fac3		-> in -
 * 	refrigerant_par[7] 	-> exp3		-> in -
 * 	refrigerant_par[8] 	-> fac4		-> in -
 * 	refrigerant_par[9] 	-> exp4		-> in -
 * 	refrigerant_par[10] -> fac5		-> in -
 * 	refrigerant_par[11] -> exp5		-> in -
 * 	refrigerant_par[12] -> fac6		-> in -
 * 	refrigerant_par[13] -> exp6		-> in -
 *
 */


/*
 * refrigerant_p_sat:
 * ------------------
 *
 * Calculates vapor pressure in Pa depending on saturation temperature T in K.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Saturation temperature in K.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for equation of vapor
 *		pressure.
 *
 * Returns:
 * --------
 *	double:
 *		Vapor pressure in Pa.
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
 * Calculates derivative of vapor pressure wrt. temperature in Pa/K depending
 * on saturation temperature T in K.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Saturation temperature in K.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for equation of vapor
 *		pressure.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of vapor pressure wrt. temperature in Pa/K.
 *
 * History:
 * --------
 *	01/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_dp_sat_dT(double T_K, double refrigerant_par[]);


/*
 * refrigerant_T_sat:
 * ------------------
 *
 * Calculates saturation temperature in K depending on saturation vapor
 * pressure p in Pa.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Saturation vapor pressure in Pa.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for equation of vapor
 *		pressure.
 *
 * Returns:
 * --------
 *	double:
 *		Saturation temperature in K.
 *
 * Remarks:
 * --------
 *	Uses Newton-Raphson method for calculating saturation temperature.
 *
 * History:
 * --------
 *	03/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_T_sat(double p_Pa, double refrigerant_par[]);


/*
 * General form of equation for vapor pressure:
 * --------------------------------------------
 * 	p_sat = 10 ^ (a - b / (T + c))
 *
 * Inputs required by user:
 * ------------------------
 *	T: Temperature in K
 *
 * Order of coefficients in JSON-file:
 * -----------------------------------
 *	refrigerant_par[0]	-> a	-> in -
 * 	refrigerant_par[1] 	-> b	-> in K
 * 	refrigerant_par[2] 	-> c	-> in K
 *
 */


/*
 * refrigerant_p_sat_antoine:
 * --------------------------
 *
 * Calculates vapor pressure in Pa depending on saturation temperature T in K
 * using Antoine-equation.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Saturation temperature in K.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for Antoine-equation.
 *
 * Returns:
 * --------
 *	double:
 *		Vapor pressure in Pa.
 *
 * History:
 * --------
 *	01/28/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_p_sat_antoine(double T_K, double refrigerant_par[]);


/*
 * refrigerant_T_sat_antoine:
 * --------------------------
 *
 * Calculates saturation temperature in K depending on vapor pressure in Pa
 * using Antoine-equation.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Vapor pressure in Pa.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for Antoine-equation.
 *
 * Returns:
 * --------
 *	double:
 *		Saturation temperature in K.
 *
 * History:
 * --------
 *	03/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_T_sat_antoine(double p_Pa, double refrigerant_par[]);


/*
 * refrigerant_dp_sat_dT_antoine:
 * ------------------------------
 *
 * Calculates derivative of vapor pressure wrt. temperature in Pa/K depending
 * on saturation temperature T in K using Antoine-equation.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Saturation temperature in K.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for Antoine-equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of vapor pressure wrt. temperature in Pa/K.
 *
 * History:
 * --------
 *	01/28/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_dp_sat_dT_antoine(double T_K, double refrigerant_par[]);

#endif
