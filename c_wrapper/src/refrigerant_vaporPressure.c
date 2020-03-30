/////////////////////////////////
// refrigerant_vaporPressure.c //
/////////////////////////////////
#include "refrigerant_vaporPressure.h"
#include <math.h>


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
double refrigerant_p_sat(double T_K, double refrigerant_par[]) {
	// Calculate temperature-dependent coefficients
	//
	double theta = T_K/refrigerant_par[0];
	double THETA = 1 - theta;

	// Calculate vapor pressure
	//
	return (refrigerant_par[1] * exp(1/theta * (
		refrigerant_par[2] * pow(THETA, refrigerant_par[3]) +
		refrigerant_par[4] * pow(THETA, refrigerant_par[5]) +
		refrigerant_par[6] * pow(THETA, refrigerant_par[7]) +
		refrigerant_par[8] * pow(THETA, refrigerant_par[9]) +
		refrigerant_par[10] * pow(THETA, refrigerant_par[11]) +
		refrigerant_par[12] * pow(THETA, refrigerant_par[13]))));
}


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
double refrigerant_T_sat(double p_Pa, double refrigerant_par[]) {
	// Initialize variables for using Newton-Raphson method
	//
	double T_guess_K = 253.15;
	double p_guess_Pa = p_Pa;
	double dp_guess_dT_PaK = T_guess_K/p_guess_Pa;

	int counter_NRM = 0;
	const double tolerance = 1e-8;

	// Calculate temperature using Newton-Raphson method
	//
	for (p_guess_Pa = refrigerant_p_sat(T_guess_K, refrigerant_par);
	     fabs(p_guess_Pa - p_Pa)>tolerance && counter_NRM<50;
		 counter_NRM++) {
		// Calculate pressure depending on guess value for temperature
		//
		p_guess_Pa = refrigerant_p_sat(T_guess_K, refrigerant_par);

		// Calculate derivative of the pressure with respect to temperature
		//
		dp_guess_dT_PaK = refrigerant_dp_sat_dT(T_guess_K, refrigerant_par);

		// Update guess value for temperature
		// Only positive values or values smaller than critical temperature are
		// allowed
		//
		T_guess_K -= (p_guess_Pa - p_Pa) / dp_guess_dT_PaK;
		T_guess_K = (T_guess_K < 0 || T_guess_K > refrigerant_par[0]) ?
			refrigerant_par[0] - 5 : T_guess_K;
	}

	// Return -1 when number of iterations exceeds 50
	//
	return (counter_NRM == 50 ? -1 : T_guess_K);
}


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
double refrigerant_dp_sat_dT(double T_K, double refrigerant_par[]) {
	// Calculate temperature-dependent coefficients
	//
	double p_sat_Pa = refrigerant_p_sat(T_K, refrigerant_par);

	double theta = T_K/refrigerant_par[0];
	double THETA = 1 - theta;

	// Calculate derivative of vapor pressure wrt. temperature
	//
	return -p_sat_Pa / T_K * (log(p_sat_Pa / refrigerant_par[1]) +
		refrigerant_par[3] * refrigerant_par[2] *
			pow(THETA, refrigerant_par[3] -1 ) +
		refrigerant_par[5] * refrigerant_par[4] *
			pow(THETA, refrigerant_par[5] - 1) +
		refrigerant_par[7] * refrigerant_par[6] *
			pow(THETA, refrigerant_par[7] - 1) +
		refrigerant_par[9] * refrigerant_par[8] *
			pow(THETA, refrigerant_par[9] - 1) +
		refrigerant_par[11] * refrigerant_par[10] *
			pow(THETA, refrigerant_par[11] - 1) +
		refrigerant_par[13] * refrigerant_par[12] *
			pow(THETA, refrigerant_par[13] - 1));
}


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
double refrigerant_p_sat_antoine(double T_K, double refrigerant_par[]) {
	// Calculate vapor pressure
	//
	return pow(10, refrigerant_par[0] - (refrigerant_par[1] /
		(T_K + refrigerant_par[2]))) * 100000;
}


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
double refrigerant_T_sat_antoine(double p_Pa, double refrigerant_par[]) {
	// Convert units
	//
	double p_bar = p_Pa / 100000;

	// Calculate vapor pressure
	//
	return - refrigerant_par[1] / (log10(p_bar) - refrigerant_par[0]) -
		refrigerant_par[2];
}


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
double refrigerant_dp_sat_dT_antoine(double T_K, double refrigerant_par[]) {
	// Calculate vapor pressure
	//
	double p_Pa = refrigerant_p_sat_antoine(T_K, refrigerant_par);

	// Calculate derivative of vapor pressure wrt. temperature
	//
	return refrigerant_par[1] * log(10) / pow(refrigerant_par[2] + T_K, 2) *
		p_Pa;
}
