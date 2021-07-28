
//////////////////////////////////////////
// refrigerant_saturatedLiquidDensity.c //
//////////////////////////////////////////
#include <math.h>
#include "refrigerant_saturatedLiquidDensity.h"


/////////////////////////////
// Definition of constants //
/////////////////////////////
#ifndef THRESHOLD_ZERO
#define THRESHOLD_ZERO 1e-10f
#endif


///////////////////////////////////////////////
// Definition of functions: EoS - Approach 1 //
///////////////////////////////////////////////
/*
 * General form of equation for saturated liquid density:
 * ------------------------------------------------------
 * 	X = ln(rho_l / rho_ref) if flag<0 else rho_l / rho_ref
 *	X = [fac1*THETA^exp1 + fac2*THETA^exp2 + fac3*THETA^exp3 +
 *		 fac4*THETA^exp4 + fac5*THETA^exp5 + fac6*THETA^exp6 +
 *		 fac7*THETA^exp7 + fac8*THETA^exp8]
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
 *	refrigerant_par[0]	-> T_crit	-> in K
 *	refrigerant_par[1]	-> rho_ref	-> in kg/m3
 *	refrigerant_par[2]	-> flag		-> in -
 * 	refrigerant_par[3] 	-> fac1		-> in -
 * 	refrigerant_par[4] 	-> exp1		-> in -
 * 	refrigerant_par[5] 	-> fac2		-> in -
 * 	refrigerant_par[6] 	-> exp2		-> in -
 * 	refrigerant_par[7] 	-> fac3		-> in -
 * 	refrigerant_par[8] 	-> exp3		-> in -
 * 	refrigerant_par[9] 	-> fac4		-> in -
 * 	refrigerant_par[10] -> exp4		-> in -
 * 	refrigerant_par[11] -> fac5		-> in -
 * 	refrigerant_par[12] -> exp5		-> in -
 * 	refrigerant_par[13] -> fac6		-> in -
 * 	refrigerant_par[14] -> exp6		-> in -
 * 	refrigerant_par[15] -> fac7		-> in -
 * 	refrigerant_par[16] -> exp7		-> in -
 * 	refrigerant_par[17] -> fac8		-> in -
 * 	refrigerant_par[18] -> exp8		-> in -
 *
 */


/*
 * refrigerant_rho_l_eos1:
 * -----------------------
 *
 * Calculates saturated liquid density in kg/m³ depending on saturation
 * temperature T in K.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Saturation temperature in K.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for equation of saturated
 *		liquid density.
 *
 * Returns:
 * --------
 *	double:
 *		Saturated liquid density in kg/m³.
 *
 * History:
 * --------
 *	01/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_rho_l_eos1(double T_K,
	double refrigerant_par[]) {
	// Calculate temperature-dependent coefficients
	//
	const double theta = T_K/refrigerant_par[0];
	const double THETA = 1 - theta;

	const double X = refrigerant_par[3] * pow(THETA, refrigerant_par[4]) +
		refrigerant_par[5] * pow(THETA, refrigerant_par[6]) +
		refrigerant_par[7] * pow(THETA, refrigerant_par[8]) +
		refrigerant_par[9] * pow(THETA, refrigerant_par[10]) +
		refrigerant_par[11] * pow(THETA, refrigerant_par[12]) +
		refrigerant_par[13] * pow(THETA, refrigerant_par[14]) +
		refrigerant_par[15] * pow(THETA, refrigerant_par[16]) +
		refrigerant_par[17] * pow(THETA, refrigerant_par[18]);

	// Calculate saturated liquid density
	//
	return (refrigerant_par[2] < 0 ? exp(X)*refrigerant_par[1] :
		X*refrigerant_par[1]);
}


/*
 * refrigerant_drho_l_dT_eos1:
 * ---------------------------
 *
 * Calculates derivative of saturated liquid density wrt. temperature in kg/m³/K
 * depending on saturation temperature T in K.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Saturation temperature in K.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for equation of saturated
 *		liquid density.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of saturated liquid density wrt. temperature in kg/m³/K.
 *
 * History:
 * --------
 *	01/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_drho_l_dT_eos1(double T_K, double refrigerant_par[]) {
	// Calculate temperature-dependent coefficients
	//
	const double theta = T_K/refrigerant_par[0];
	const double THETA = 1 - theta;

	// Check if exponent is zero and, thus, derivative is not necessary:
	// Watch out! Checks for equality are not applicable when using type
	// double. Instead, compare for small threshold.
	//
	const double summand1 = (fabs(refrigerant_par[4]) < THRESHOLD_ZERO ? 0 :
		refrigerant_par[4] * refrigerant_par[3] *
		pow(THETA, refrigerant_par[4] -1));

	const double summand2 = (fabs(refrigerant_par[6]) < THRESHOLD_ZERO ? 0 :
		refrigerant_par[6] * refrigerant_par[5] *
		pow(THETA, refrigerant_par[6] - 1));

	const double summand3 = (fabs(refrigerant_par[8]) < THRESHOLD_ZERO ? 0 :
		refrigerant_par[8] * refrigerant_par[7] *
		pow(THETA, refrigerant_par[8] - 1));

	const double summand4 = (fabs(refrigerant_par[10]) < THRESHOLD_ZERO ? 0 :
		refrigerant_par[10] * refrigerant_par[9] *
		pow(THETA, refrigerant_par[10] - 1));

	const double summand5 = (fabs(refrigerant_par[12]) < THRESHOLD_ZERO ? 0 :
		refrigerant_par[12] * refrigerant_par[11] *
		pow(THETA, refrigerant_par[12] - 1));

	const double summand6 = (fabs(refrigerant_par[14]) < THRESHOLD_ZERO ? 0 :
		refrigerant_par[14] * refrigerant_par[13] *
		pow(THETA, refrigerant_par[14] - 1));

	const double summand7 = (fabs(refrigerant_par[16]) < THRESHOLD_ZERO ? 0 :
		refrigerant_par[16] * refrigerant_par[15] *
		pow(THETA, refrigerant_par[16] - 1));

	const double summand8 = (fabs(refrigerant_par[18]) < THRESHOLD_ZERO ? 0 :
		refrigerant_par[18] * refrigerant_par[17] *
		pow(THETA, refrigerant_par[18] - 1));

	const double dX_dTHETA = summand1 + summand2 + summand3 + summand4 +
		summand5 + summand6 + summand7 + summand8;

	const double dTHETA_dT = -1 / refrigerant_par[0];

	// Calculate derivative of saturated liquid density wrt. temperature
	//
	if (refrigerant_par[2] < 0) {
		// Approach 1
		//
		const double rho_l = refrigerant_rho_l_eos1(T_K, refrigerant_par);

		return (rho_l * dX_dTHETA * dTHETA_dT);

	} else {
		// Approach 2
		//
		return (refrigerant_par[1] * dX_dTHETA * dTHETA_dT);

	}
}
