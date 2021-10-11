/////////////////////////////////
// refrigerant_vaporPressure.c //
/////////////////////////////////
#include <math.h>
#include <stdio.h>
#include "refrigerant_cubicEoS.h"
#include "refrigerant_vaporPressure.h"


/////////////////////////////
// Definition of constants //
/////////////////////////////
#ifndef THRESHOLD_ZERO
#define THRESHOLD_ZERO 1e-10f
#endif

#ifndef ACCURACY_NEWTON
#define ACCURACY_NEWTON 1e-8f
#endif

#ifndef MAX_ITERATIONS_NEWTON
#define MAX_ITERATIONS_NEWTON 50
#endif

#ifndef ACCURACY_CUBIC
#define ACCURACY_CUBIC 1e-6f
#endif

#ifndef MAX_ITERATIONS_CUBIC
#define MAX_ITERATIONS_CUBIC 50
#endif


///////////////////////////////////////////////
// Definition of functions: EoS - Approach 1 //
///////////////////////////////////////////////
/*
 * General form of equation for vapor pressure:
 * --------------------------------------------
 * 	p_sat = p_crit * exp(1/theta *
 *		[fac1*THETA^exp1 + fac2*THETA^exp2 + fac3*THETA^exp3 + fac4*THETA^exp4 +
 *		 fac5*THETA^exp5 + fac6*THETA^exp6 + fac7*THETA^exp7])
 *
 * Temperature-dependent coefficients:
 * -----------------------------------
 * 	theta = T/T_crit
 * 	THETA = 1 - theta
 *
 * Inputs required by user:
 * ------------------------
 *	T: Temperature in K
 *
 * Order of coefficients in JSON-file:
 * -----------------------------------
 *	refrigerant_par[0]	-> T_crit	-> in K
 * 	refrigerant_par[1] 	-> p_crit	-> in Pa
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
 * 	refrigerant_par[14] -> fac7		-> in -
 * 	refrigerant_par[15] -> exp7		-> in -
 *
 */


/*
 * refrigerant_p_sat_eos1:
 * -----------------------
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
 *	09/02/2020, by Mirko Engelpracht:
 *		Improved code.
 *
 */
double refrigerant_p_sat_eos1(const double T_K,
	const double refrigerant_par[]) {
	// Calculate temperature-dependent coefficients
	//
	const double theta = T_K/refrigerant_par[0];
	const double THETA = 1 - theta;

	// Calculate vapor pressure
	//
	return (refrigerant_par[1] * exp(1/theta * (
		refrigerant_par[2] * pow(THETA, refrigerant_par[3]) +
		refrigerant_par[4] * pow(THETA, refrigerant_par[5]) +
		refrigerant_par[6] * pow(THETA, refrigerant_par[7]) +
		refrigerant_par[8] * pow(THETA, refrigerant_par[9]) +
		refrigerant_par[10] * pow(THETA, refrigerant_par[11]) +
		refrigerant_par[12] * pow(THETA, refrigerant_par[13]) +
		refrigerant_par[14] * pow(THETA, refrigerant_par[15]))));
}


/*
 * refrigerant_dp_sat_dT_eos1:
 * ---------------------------
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
 *	09/02/2020, by Mirko Engelpracht:
 *		Improved code.
 *
 */
double refrigerant_dp_sat_dT_eos1(const double T_K,
	const double refrigerant_par[]) {
	// Calculate temperature-dependent coefficients
	//
	const double p_sat_Pa = refrigerant_p_sat_eos1(T_K, refrigerant_par);

	const double theta = T_K/refrigerant_par[0];
	const double THETA = 1 - theta;

	// Check if exponent is zero and, thus, derivative is not necessary:
	// Watch out! Checks for equality are not applicable when using type
	// double. Instead, compare for small threshold.
	//
	const double summand1 = (fabs(refrigerant_par[3]) < THRESHOLD_ZERO ? 0 :
		refrigerant_par[3] * refrigerant_par[2] *
		pow(THETA, refrigerant_par[3] -1));

	const double summand2 = (fabs(refrigerant_par[5]) < THRESHOLD_ZERO ? 0 :
		refrigerant_par[5] * refrigerant_par[4] *
		pow(THETA, refrigerant_par[5] - 1));

	const double summand3 = (fabs(refrigerant_par[7]) < THRESHOLD_ZERO ? 0 :
		refrigerant_par[7] * refrigerant_par[6] *
		pow(THETA, refrigerant_par[7] - 1));

	const double summand4 = (fabs(refrigerant_par[9]) < THRESHOLD_ZERO ? 0 :
		refrigerant_par[9] * refrigerant_par[8] *
		pow(THETA, refrigerant_par[9] - 1));

	const double summand5 = (fabs(refrigerant_par[11]) < THRESHOLD_ZERO ? 0 :
		refrigerant_par[11] * refrigerant_par[10] *
		pow(THETA, refrigerant_par[11] - 1));

	const double summand6 = (fabs(refrigerant_par[13]) < THRESHOLD_ZERO ? 0 :
		refrigerant_par[13] * refrigerant_par[12] *
		pow(THETA, refrigerant_par[13] - 1));

	const double summand7 = (fabs(refrigerant_par[15]) < THRESHOLD_ZERO ? 0 :
		refrigerant_par[15] * refrigerant_par[14] *
		pow(THETA, refrigerant_par[15] - 1));

	// Calculate derivative of vapor pressure wrt. temperature
	//
	return (-p_sat_Pa / T_K * (log(p_sat_Pa / refrigerant_par[1]) +
		summand1 + summand2 + summand3 + summand4 + summand5 +
		summand6 + summand7));
}


/*
 * refrigerant_T_sat_eos1:
 * -----------------------
 *
 * Calculates saturation temperature in K depending on vapor pressure p in Pa.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Vapor pressure in Pa.
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
 *	09/02/2020, by Mirko Engelpracht:
 *		Improved code.
 *
 */
double refrigerant_T_sat_eos1(const double p_Pa,
	const double refrigerant_par[]) {
	// Initialize variables for using Newton-Raphson method
	//
	double T_guess_K = 253.15;
	double p_guess_Pa = p_Pa;
	double dp_guess_dT_PaK = p_guess_Pa / T_guess_K;

	int counter_NRM = 0;

	// Calculate temperature using Newton-Raphson method
	//
	for (p_guess_Pa = refrigerant_p_sat_eos1(T_guess_K, refrigerant_par);
	     fabs(p_guess_Pa - p_Pa) > ACCURACY_NEWTON &&
			counter_NRM < MAX_ITERATIONS_NEWTON;
		 counter_NRM++) {
		// Calculate pressure depending on guess value for temperature
		//
		p_guess_Pa = refrigerant_p_sat_eos1(T_guess_K, refrigerant_par);

		// Calculate derivative of vapor pressure with respect to temperature
		//
		dp_guess_dT_PaK = refrigerant_dp_sat_dT_eos1(T_guess_K,
			refrigerant_par);

		// Update guess value for temperature:
		// Only positive values or values smaller than critical temperature are
		// allowed
		//
		T_guess_K -= (p_guess_Pa - p_Pa) / dp_guess_dT_PaK;

		if (T_guess_K < 0) {
			T_guess_K = 5;
		} else if (T_guess_K > refrigerant_par[0]) {
			T_guess_K = refrigerant_par[0] - 5;
		}
	}

	// Return -1 when number of iterations exceeds MAX_ITERATIONS_NEWTON
	//
	return (counter_NRM == MAX_ITERATIONS_NEWTON ? -1 : T_guess_K);
}


///////////////////////////////////////////////
// Definition of functions: EoS - Approach 2 //
///////////////////////////////////////////////
/*
 * General form of equation for vapor pressure:
 * --------------------------------------------
 * 	p_sat = p_ref * exp(fac0/theta + fac1*theta^exp1 + fac2*theta^exp2 +
 *		fac3*theta^exp3 + fac4*theta^exp4 + fac5*(1-theta)^exp5)
 *
 * Temperature-dependent coefficients:
 * -----------------------------------
 * 	theta = T/T_crit
 *
 * Inputs required by user:
 * ------------------------
 *	T: Temperature in K
 *
 * Order of coefficients in JSON-file:
 * -----------------------------------
 *	refrigerant_par[0]	-> T_crit	-> in K
 * 	refrigerant_par[1] 	-> p_ref	-> in Pa
 * 	refrigerant_par[2] 	-> fac0		-> in -
 * 	refrigerant_par[3] 	-> fac1		-> in -
 * 	refrigerant_par[4] 	-> exp1		-> in -
 * 	refrigerant_par[5] 	-> fac2		-> in -
 * 	refrigerant_par[6] 	-> exp2		-> in -
 * 	refrigerant_par[7] 	-> fac3		-> in -
 * 	refrigerant_par[8] 	-> exp3		-> in -
 * 	refrigerant_par[9] 	-> fac4		-> in -
 * 	refrigerant_par[10]	-> exp4		-> in -
 * 	refrigerant_par[11] -> fac5		-> in -
 * 	refrigerant_par[12] -> exp5		-> in -
 *
 */


/*
 * refrigerant_p_sat_eos2:
 * -----------------------
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
 *	06/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_p_sat_eos2(const double T_K,
	const double refrigerant_par[]) {
	// Calculate temperature-dependent coefficients
	//
	const double theta = T_K/refrigerant_par[0];

	// Calculate vapor pressure
	//
	return (refrigerant_par[1] * exp(refrigerant_par[2]/theta +
		refrigerant_par[3] * pow(theta, refrigerant_par[4]) +
		refrigerant_par[5] * pow(theta, refrigerant_par[6]) +
		refrigerant_par[7] * pow(theta, refrigerant_par[8]) +
		refrigerant_par[9] * pow(theta, refrigerant_par[10]) +
		refrigerant_par[11] * pow(1-theta, refrigerant_par[12])));
}


/*
 * refrigerant_dp_sat_dT_eos2:
 * ---------------------------
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
 *	06/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_dp_sat_dT_eos2(const double T_K,
	const double refrigerant_par[]) {
	// Calculate temperature-dependent coefficients
	//
	const double p_sat_Pa = refrigerant_p_sat_eos2(T_K, refrigerant_par);

	const double theta = T_K/refrigerant_par[0];

	// Check if exponent is zero and, thus, derivative is not necessary:
	// Watch out! Checks for equality are not applicable when using type
	// double. Instead, compare for small threshold.
	//
	const double summand1 = (fabs(refrigerant_par[4]) < THRESHOLD_ZERO ? 0 :
		refrigerant_par[4] * refrigerant_par[3] *
		pow(theta, refrigerant_par[4] - 1) / refrigerant_par[0]);

	const double summand2 = (fabs(refrigerant_par[6]) < THRESHOLD_ZERO ? 0 :
		refrigerant_par[6] * refrigerant_par[5] *
		pow(theta, refrigerant_par[6] - 1) / refrigerant_par[0]);

	const double summand3 = (fabs(refrigerant_par[8]) < THRESHOLD_ZERO ? 0 :
		refrigerant_par[8] * refrigerant_par[7] *
		pow(theta, refrigerant_par[8] - 1) / refrigerant_par[0]);

	const double summand4 = (fabs(refrigerant_par[10]) < THRESHOLD_ZERO ? 0 :
		refrigerant_par[10] * refrigerant_par[9] *
		pow(theta, refrigerant_par[10] - 1) / refrigerant_par[0]);

	const double summand5 = (fabs(refrigerant_par[12]) < THRESHOLD_ZERO ? 0 :
		-refrigerant_par[12] * refrigerant_par[11] / refrigerant_par[0] *
		pow(1-theta, refrigerant_par[12] - 1));

	// Calculate derivative of vapor pressure wrt. temperature
	//
	return (p_sat_Pa  * (
		-refrigerant_par[2] * refrigerant_par[0] / pow(T_K, 2) +
		summand1 + summand2 + summand3 + summand4 + summand5));
}


/*
 * refrigerant_T_sat_eos2:
 * -----------------------
 *
 * Calculates saturation temperature in K depending on vapor pressure p in Pa.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Vapor pressure in Pa.
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
 *	06/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_T_sat_eos2(const double p_Pa,
	const double refrigerant_par[]) {
	// Initialize variables for using Newton-Raphson method
	//
	double T_guess_K = 313.15;
	double p_guess_Pa = p_Pa;
	double dp_guess_dT_PaK = p_guess_Pa / T_guess_K;

	int counter_NRM = 0;

	// Calculate temperature using Newton-Raphson method
	//
	for (p_guess_Pa = refrigerant_p_sat_eos2(T_guess_K, refrigerant_par);
	     fabs(p_guess_Pa - p_Pa) > ACCURACY_NEWTON &&
			counter_NRM < MAX_ITERATIONS_NEWTON;
		 counter_NRM++) {
		// Calculate pressure depending on guess value for temperature
		//
		p_guess_Pa = refrigerant_p_sat_eos2(T_guess_K, refrigerant_par);

		// Calculate derivative of vapor pressure with respect to temperature
		//
		dp_guess_dT_PaK = refrigerant_dp_sat_dT_eos2(T_guess_K,
			refrigerant_par);

		// Update guess value for temperature:
		// Only positive values or values smaller than critical temperature are
		// allowed
		//
		T_guess_K -= (p_guess_Pa - p_Pa) / dp_guess_dT_PaK;

		if (T_guess_K < 0) {
			T_guess_K = 5;
		} else if (T_guess_K > refrigerant_par[0]) {
			T_guess_K = refrigerant_par[0] - 5;
		}
	}

	// Return -1 when number of iterations exceeds MAX_ITERATIONS_NEWTON
	//
	return (counter_NRM == MAX_ITERATIONS_NEWTON ? -1 : T_guess_K);
}


///////////////////////////////////////////////
// Definition of functions: EoS - Approach 3 //
///////////////////////////////////////////////
/*
 * General form of equation for vapor pressure:
 * --------------------------------------------
 * 	p_sat = p_ref * exp(fac0/T + fac1*T^exp1 + fac2*T^exp2 + fac3*(1-theta)^exp3)
 *
 * Temperature-dependent coefficients:
 * -----------------------------------
 * 	theta = T/T_crit
 *
 * Inputs required by user:
 * ------------------------
 *	T: Temperature in K
 *
 * Order of coefficients in JSON-file:
 * -----------------------------------
 *	refrigerant_par[0]	-> T_crit	-> in K
 * 	refrigerant_par[1] 	-> p_ref	-> in Pa
 * 	refrigerant_par[2] 	-> fac0		-> in -
 * 	refrigerant_par[3] 	-> fac1		-> in -
 * 	refrigerant_par[4] 	-> exp1		-> in -
 * 	refrigerant_par[5] 	-> fac2		-> in -
 * 	refrigerant_par[6] 	-> exp2		-> in -
 * 	refrigerant_par[7] 	-> fac3		-> in -
 * 	refrigerant_par[8] 	-> exp3		-> in -
 *
 */


/*
 * refrigerant_p_sat_eos3:
 * -----------------------
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
 *	06/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_p_sat_eos3(const double T_K,
	const double refrigerant_par[]) {
	// Calculate temperature-dependent coefficients
	//
	const double theta = T_K/refrigerant_par[0];

	// Calculate vapor pressure
	//
	return (refrigerant_par[1] * exp(refrigerant_par[2]/T_K +
		refrigerant_par[3] * pow(T_K, refrigerant_par[4]) +
		refrigerant_par[5] * pow(T_K, refrigerant_par[6]) +
		refrigerant_par[7] * pow(1-theta, refrigerant_par[8])));
}


/*
 * refrigerant_dp_sat_dT_eos3:
 * ---------------------------
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
 *	06/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_dp_sat_dT_eos3(const double T_K,
	const double refrigerant_par[]) {
	// Calculate temperature-dependent coefficients
	//
	const double p_sat_Pa = refrigerant_p_sat_eos3(T_K, refrigerant_par);

	const double theta = T_K/refrigerant_par[0];

	// Check if exponent is zero and, thus, derivative is not necessary:
	// Watch out! Checks for equality are not applicable when using type
	// double. Instead, compare for small threshold.
	//
	const double summand1 = (fabs(refrigerant_par[4]) < THRESHOLD_ZERO ? 0 :
		refrigerant_par[4] * refrigerant_par[3] *
		pow(T_K, refrigerant_par[4] - 1));

	const double summand2 = (fabs(refrigerant_par[6]) < THRESHOLD_ZERO ? 0 :
		refrigerant_par[6] * refrigerant_par[5] *
		pow(T_K, refrigerant_par[6] - 1));

	const double summand3 = (fabs(refrigerant_par[8]) < THRESHOLD_ZERO ? 0 :
		-refrigerant_par[8] * refrigerant_par[7] / refrigerant_par[0] *
		pow(1-theta, refrigerant_par[8] - 1));

	// Calculate derivative of vapor pressure wrt. temperature
	//
	return (p_sat_Pa  * (-refrigerant_par[2] / pow(T_K, 2) +
		summand1 + summand2 + summand3));
}


/*
 * refrigerant_T_sat_eos3:
 * -----------------------
 *
 * Calculates saturation temperature in K depending on vapor pressure p in Pa.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Vapor pressure in Pa.
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
 *	06/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_T_sat_eos3(const double p_Pa,
	const double refrigerant_par[]) {
	// Initialize variables for using Newton-Raphson method
	//
	double T_guess_K = 253.15;
	double p_guess_Pa = p_Pa;
	double dp_guess_dT_PaK = p_guess_Pa / T_guess_K;

	int counter_NRM = 0;

	// Calculate temperature using Newton-Raphson method
	//
	for (p_guess_Pa = refrigerant_p_sat_eos3(T_guess_K, refrigerant_par);
	     fabs(p_guess_Pa - p_Pa) > ACCURACY_NEWTON &&
			counter_NRM < MAX_ITERATIONS_NEWTON;
		 counter_NRM++) {
		// Calculate pressure depending on guess value for temperature
		//
		p_guess_Pa = refrigerant_p_sat_eos3(T_guess_K, refrigerant_par);

		// Calculate derivative of vapor pressure with respect to temperature
		//
		dp_guess_dT_PaK = refrigerant_dp_sat_dT_eos3(T_guess_K,
			refrigerant_par);

		// Update guess value for temperature:
		// Only positive values or values smaller than critical temperature are
		// allowed
		//
		T_guess_K -= (p_guess_Pa - p_Pa) / dp_guess_dT_PaK;

		if (T_guess_K < 0) {
			T_guess_K = 5;
		} else if (T_guess_K > refrigerant_par[0]) {
			T_guess_K = refrigerant_par[0] - 5;
		}
	}

	// Return -1 when number of iterations exceeds MAX_ITERATIONS_NEWTON
	//
	return (counter_NRM == MAX_ITERATIONS_NEWTON ? -1 : T_guess_K);
}


///////////////////////////////////////////////
// Definition of functions: Antoine equation //
///////////////////////////////////////////////
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
 *	01/28/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_p_sat_antoine(double T_K, double refrigerant_par[]) {
	// Calculate vapor pressure and change unit from bar into Pa
	//
	return (pow(10, refrigerant_par[0] - (refrigerant_par[1] /
		(T_K + refrigerant_par[2]))) * 100000);
}


/*
 * refrigerant_dp_sat_dT_antoine:
 * ------------------------------
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
	return (refrigerant_par[1] * log(10) / pow(refrigerant_par[2] + T_K, 2) *
		p_Pa);
}


/*
 * refrigerant_T_sat_antoine:
 * --------------------------
 *
 * Calculates saturation temperature in K depending on vapor pressure in Pa.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Vapor pressure in Pa.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for equation of vapor
 *		pressure.
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
	// Convert unit from Pa into bar
	//
	double p_bar = p_Pa / 100000;

	// Calculate vapor pressure
	//
	return (-refrigerant_par[1] / (log10(p_bar) - refrigerant_par[0]) -
		refrigerant_par[2]);
}


/////////////////////////////////////////
// Definition of functions: Cubic EoS  //
/////////////////////////////////////////
/*
 * Approach to calculate saturation properties from cubic equation of state:
 * -------------------------------------------------------------------------
 *
 *	a) Calculate pure component parameters
 *	b) Start iteration for vapor pressure until liquid fugacity equals vapor
 *	   fugacity:
 *
 *		i) Solve cubic equation of state (i.e., calculate Z_l and Z_v)
 *		ii) Calculate fugacity coefficients of both phases
 *		iii) Check if fugacity coefficients are equal
 *
 *			--> If yes: Convergence achieved
 *			--> If not: Adapt pressure according to fugacity coefficients
 *
 * Inputs required by user:
 * ------------------------
 *	T: Temperature in K
 *
 * Remarks:
 * --------
 *	Parameter 'EoS' defines equation of state:
 *		EoS = -10: SRKE with custom alpha equation
 *		EoS = -5: SRKE
 *		EoS = 5: PRE with custom alpha equation
 *		EoS = 10: PRE
 *		EoS = 20: PRSVE
 *
 * Order of coefficients in JSON-file:
 * -----------------------------------
 *	refrigerant_par[0]	-> EoS		-> in -
 * 	refrigerant_par[1] 	-> p_crit	-> in Pa
 *	refrigerant_par[2]	-> T_crit	-> in K
 * 	refrigerant_par[3] 	-> omega	-> in -
 * 	refrigerant_par[4] 	-> kappa_1	-> in -
 * 	refrigerant_par[5] 	-> beta_0	-> in -
 * 	refrigerant_par[6] 	-> beta_1	-> in -
 * 	refrigerant_par[7] 	-> beta_2	-> in -
 * 	refrigerant_par[8] 	-> beta_3	-> in -
 *
 */


/*
 * refrigerant_p_sat_cubic_internal:
 * ---------------------------------
 *
 * Calculates vapor pressure in Pa depending on saturation temperature T in K.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Saturation temperature in K.
 *	double p_guess_Pa:
 *		Guess value for vapor pressure in Pa.
 *	void (*calc_eos_parameters)(double[], double, double[]):
 *		Function to calculate parameters of cubic equation of state.
 *	void (*calc_gen_parameters)(double[], double, double, double, double):
 *		Function to calculate generalized parameters of cubic equation of state.
 *	double (*calc_fug_coefficient)(int*, double, double, double):
 *		Function to calculate fugacity coefficient.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for cubic equation of state.
 *
 * Returns:
 * --------
 *	double:
 *		Vapor pressure in Pa.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_p_sat_cubic_internal(double T_K, double p_guess_Pa,
	void (*calc_eos_parameters)(double[], double, double[]),
	void (*calc_gen_parameters)(double[], double, double, double, double),
	double (*calc_fug_coefficient)(int*, double, double, double),
	double refrigerant_par[]) {
	// Initialize variables
	//
	int err_phi_l, err_phi_v;

	double par_eos[2];
	double gen_par_eos[5];
	double sol_eos[2];
	double phi_l, phi_v;

	// Calculate pure component parameters
	//
	calc_eos_parameters(par_eos, T_K, refrigerant_par);

	// Initialize variables required for iteration specifically
	//
	int finished = -1;
	int counter = 0;

	double delta = 0;
	double delta_old = 0;
	double p_change = 25000;

	double p = p_guess_Pa;

	// Iterate vapor pressure
	//
	while(finished == -1 && counter < MAX_ITERATIONS_CUBIC) {
		// Solve equation of state
		//
		calc_gen_parameters(gen_par_eos, p, T_K, par_eos[0], par_eos[1]);
		refrigerant_cubic_solve(sol_eos, gen_par_eos[2], gen_par_eos[3],
			gen_par_eos[4]);

		// Calculate fugacity coefficients
		//
		phi_l = calc_fug_coefficient(&err_phi_l, sol_eos[0], gen_par_eos[0],
			gen_par_eos[1]);
		phi_v = calc_fug_coefficient(&err_phi_v, sol_eos[1], gen_par_eos[0],
			gen_par_eos[1]);

		// Check if equations for fugacity coefficients were solvable
		//
		if (err_phi_l == 0 && err_phi_v == 0) {
			// Could calculate fugacity coefficient: Check for convergence
			//
			delta_old = delta;
			delta = log(phi_v) - log(phi_l);

			if (fabs(delta) <= fabs(ACCURACY_CUBIC * log(phi_v))) {
				// Convergence achieved
				//
				finished = 1;
				counter++;

			} else {
				// No convergence: Update pressure
				//
				p_change = (delta_old * delta <= 0) ? 0.5 * p_change : p_change;
				p = (delta <= 0) ? p + p_change : p - p_change;
				counter++;

			}

		} else {
			// Could not calculate fugacity coefficient: Reduce pressure
			//
			p *= 0.5;
			counter++;

		}
	}

	// Return result
	//
	return (counter == MAX_ITERATIONS_CUBIC ? -1 : p);
}


/*
 * refrigerant_dp_sat_dT_cubic_internal:
 * -------------------------------------
 *
 * Calculates derivative of vapor pressure wrt. temperature in Pa/K depending
 * on saturation temperature T in K.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Saturation temperature in K.
 *	double p_guess_Pa:
 *		Guess value for vapor pressure in Pa.
 *	void (*calc_eos_parameters)(double[], double, double[]):
 *		Function to calculate parameters of cubic equation of state.
 *	void (*calc_gen_parameters)(double[], double, double, double, double):
 *		Function to calculate generalized parameters of cubic equation of state.
 *	double (*calc_fug_coefficient)(int*, double, double, double):
 *		Function to calculate fugacity coefficient.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for cubic equation of state.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of vapor pressure wrt. temperature in Pa/K.
 *
 * Remarks:
 * --------
 *	Derivative is calculated numerically by using a symmetric derivative with
 *  h = 0.0001 K as small change.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_dp_sat_dT_cubic_internal(double T_K, double p_guess_Pa,
	void (*calc_eos_parameters)(double[], double, double[]),
	void (*calc_gen_parameters)(double[], double, double, double, double),
	double (*calc_fug_coefficient)(int*, double, double, double),
	double refrigerant_par[]) {
	// Calculate vapor pressures
	//
	const double dT_K = 0.0001;

	double p_plus_Pa = refrigerant_p_sat_cubic_internal(T_K + dT_K,
		p_guess_Pa, calc_eos_parameters, calc_gen_parameters,
		calc_fug_coefficient, refrigerant_par);
	double p_minus_Pa = refrigerant_p_sat_cubic_internal(T_K - dT_K,
		p_guess_Pa, calc_eos_parameters, calc_gen_parameters,
		calc_fug_coefficient, refrigerant_par);

	// Calculate derivative of vapor pressure wrt. temperature
	//
	return ((p_plus_Pa - p_minus_Pa) / (2 * dT_K));
}


/*
 * refrigerant_p_sat_cubic:
 * ------------------------
 *
 * Calculates vapor pressure in Pa depending on saturation temperature T in K.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Saturation temperature in K.
 *	void (*calc_eos_parameters)(double[], double, double[]):
 *		Function to calculate parameters of cubic equation of state.
 *	void (*calc_gen_parameters)(double[], double, double, double, double):
 *		Function to calculate generalized parameters of cubic equation of state.
 *	double (*calc_fug_coefficient)(int*, double, double, double):
 *		Function to calculate fugacity coefficient.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for cubic equation of state.
 *
 * Returns:
 * --------
 *	double:
 *		Vapor pressure in Pa.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_p_sat_cubic(double T_K,
	void (*calc_eos_parameters)(double[], double, double[]),
	void (*calc_gen_parameters)(double[], double, double, double, double),
	double (*calc_fug_coefficient)(int*, double, double, double),
	double refrigerant_par[]) {
	// Check if VLE exists
	//
	if (T_K >= refrigerant_par[2]) {
		// VLE does not exist: Print warning and return -1
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nVapor-Liquid-Equilibrium does not exist because temperature "
			" is greater than critical temperature.");

		return -1;

	} else {
		// VLE exists: Calculate guess value for vapor pressure and execute main
		// function
		//
		double p_guess_Pa = exp(log(refrigerant_par[1]) + log(10) * 7 / 3 * (1 +
			refrigerant_par[3]) * (1 - refrigerant_par[2] / T_K));

		return refrigerant_p_sat_cubic_internal(T_K, p_guess_Pa,
			calc_eos_parameters, calc_gen_parameters, calc_fug_coefficient,
			refrigerant_par);

	}
}


/*
 * refrigerant_dp_sat_dT_cubic:
 * ----------------------------
 *
 * Calculates derivative of vapor pressure wrt. temperature in Pa/K depending
 * on saturation temperature T in K.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Saturation temperature in K.
 *	void (*calc_eos_parameters)(double[], double, double[]):
 *		Function to calculate parameters of cubic equation of state.
 *	void (*calc_gen_parameters)(double[], double, double, double, double):
 *		Function to calculate generalized parameters of cubic equation of state.
 *	double (*calc_fug_coefficient)(int*, double, double, double):
 *		Function to calculate fugacity coefficient.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for cubic equation of state.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of vapor pressure wrt. temperature in Pa/K.
 *
 * Remarks:
 * --------
 *	Derivative is calculated numerically by using a symmetric derivative with
 *  h = 0.0001 K as small change.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_dp_sat_dT_cubic(double T_K,
	void (*calc_eos_parameters)(double[], double, double[]),
	void (*calc_gen_parameters)(double[], double, double, double, double),
	double (*calc_fug_coefficient)(int*, double, double, double),
	double refrigerant_par[]) {
	// Calculate vapor pressures
	//
	const double dT_K = 0.0001;

	double p_plus_Pa = refrigerant_p_sat_cubic(T_K + dT_K, calc_eos_parameters,
		calc_gen_parameters, calc_fug_coefficient, refrigerant_par);
	double p_minus_Pa = refrigerant_p_sat_cubic(T_K - dT_K, calc_eos_parameters,
		calc_gen_parameters, calc_fug_coefficient, refrigerant_par);

	// Calculate derivative of vapor pressure wrt. temperature
	//
	return ((p_plus_Pa - p_minus_Pa) / (2 * dT_K));
}


/*
 * refrigerant_T_sat_cubic:
 * ------------------------
 *
 * Calculates saturation temperature in K depending on vapor pressure in Pa.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Vapor pressure in Pa.
 *	void (*calc_eos_parameters)(double[], double, double[]):
 *		Function to calculate parameters of cubic equation of state.
 *	void (*calc_gen_parameters)(double[], double, double, double, double):
 *		Function to calculate generalized parameters of cubic equation of state.
 *	double (*calc_fug_coefficient)(double*, double, double, double):
 *		Function to calculate fugacity coefficient.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for cubic equation of state.
 *
 * Returns:
 * --------
 *	double:
 *		Saturation temperature in K.
 *
 * Remarks:
 * --------
 *	Uses Newton-Raphson method for calculating equilibrium temperature.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_T_sat_cubic(double p_Pa,
	void (*calc_eos_parameters)(double[], double, double[]),
	void (*calc_gen_parameters)(double[], double, double, double, double),
	double (*calc_fug_coefficient)(int*, double, double, double),
	double refrigerant_par[]) {
	// Check if VLE exists
	//
	if (p_Pa >= refrigerant_par[1]) {
		// VLE does not exist: Print warning and return -1
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nVapor-Liquid-Equilibrium does not exist because pressure "
			" is greater than critical pressure.");

		return -1;

	} else {
		// VLE exists: Calculate guess value for temperature  and
		// execute Newton-Raphson method
		//
		double T_guess_K = refrigerant_par[2] / (1 - (log(p_Pa) -
			log(refrigerant_par[1])) / (log(10) * 7 / 3 * (1 +
			refrigerant_par[3])));
		double p_guess_Pa = p_Pa;
		double dp_guess_dT_PaK = p_guess_Pa / T_guess_K;

		int counter_NRM = 0;

		for (p_guess_Pa = refrigerant_p_sat_cubic(T_guess_K,
				calc_eos_parameters, calc_gen_parameters, calc_fug_coefficient,
				refrigerant_par);
			 fabs(p_guess_Pa - p_Pa)>ACCURACY_NEWTON &&
				counter_NRM<MAX_ITERATIONS_NEWTON;
			 counter_NRM++) {
			// Calculate pressure depending on guess value for temperature
			//
			p_guess_Pa = refrigerant_p_sat_cubic(T_guess_K, calc_eos_parameters,
				calc_gen_parameters, calc_fug_coefficient, refrigerant_par);

			// Calculate derivative of the loading with respect to pressure
			//
			dp_guess_dT_PaK = refrigerant_dp_sat_dT_cubic_internal(T_guess_K,
				p_guess_Pa, calc_eos_parameters, calc_gen_parameters,
				calc_fug_coefficient, refrigerant_par);

			// Update guess value for temperature
			// Only positive values are allowed
			//
			T_guess_K -= (p_guess_Pa - p_Pa) / dp_guess_dT_PaK;
			T_guess_K = T_guess_K < 0 ? 5 : T_guess_K;
		}

		// Return -1 when number of iterations exceeds 50
		//
		return (counter_NRM == MAX_ITERATIONS_NEWTON ? -1 : T_guess_K);

	}
}
