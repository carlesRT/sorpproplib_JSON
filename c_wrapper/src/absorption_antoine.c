//////////////////////////
// absorption_antoine.c //
//////////////////////////
#include <math.h>
#include "absorption_antoine.h"


/////////////////////////////
// Definition of functions //
/////////////////////////////
/*
 * General form of Antoine equation:
 * ---------------------------------
 * 	p = d * 10^(SUM_i=0^k [(A_i + 1000 * B_i / (T - c)) * (100 * X)^i])
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
	double isotherm_par[]) {
	// Initialize variables for using Newton-Raphson method
	//
	double X_guess_kgkg = 0.5;
	double p_guess_Pa  = p_Pa;
	double dp_guess_dX_Pakgkg = p_Pa/X_guess_kgkg;

	int counter_NRM = 0;
	const double tolerance = 1e-8;

	// Calculate concentration using Newton-Raphson method
	//
	for (p_guess_Pa = absorption_antoine_p_XT(X_guess_kgkg, T_K, isotherm_par);
	     fabs(p_guess_Pa - p_Pa)>tolerance && counter_NRM<50;
		 counter_NRM++) {
		// Calculate pressure depending on guess value for concentration and
		// temperature
		//
		p_guess_Pa = absorption_antoine_p_XT(X_guess_kgkg, T_K, isotherm_par);

		// Calculate derivative of the pressure with respect to concentration
		//
		dp_guess_dX_Pakgkg = absorption_antoine_dp_dX_XT(X_guess_kgkg, T_K,
			isotherm_par);

		// Update guess value for concentration
		// Only positive values are allowed
		//
		X_guess_kgkg -= (p_guess_Pa - p_Pa) / dp_guess_dX_Pakgkg;

		if (X_guess_kgkg < 0) {
			X_guess_kgkg = 0;
		} else if (X_guess_kgkg > 1) {
			X_guess_kgkg = 1;
		}
	}

	// Return -1 when number of iterations exceeds 50
	//
	return (counter_NRM == 50 ? -1 : X_guess_kgkg);
}


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
	double isotherm_par[]) {
	// Calculate summands of Antoine equation
	//
	int i = 0;
	double summand = 0;
	for (i=0; i<5; i++) {
		summand += (isotherm_par[i] + 1000 * isotherm_par[i+5] / (T_K -
		isotherm_par[10])) * pow(100 * X_kgkg, i);
	}

	// Calculate equilibrium pressure
	//
	return isotherm_par[11] * pow(10, summand);
}


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
	double isotherm_par[]) {
	// Initialize variables for using Newton-Raphson method
	//
	double T_guess_K = 353.15;
	double p_guess_Pa = p_Pa;
	double dp_guess_dT_PaK = p_Pa/T_guess_K;

	int counter_NRM = 0;
	const double tolerance = 1e-8;

	// Calculate temperature using Newton-Raphson method
	//
	for (p_guess_Pa = absorption_antoine_p_XT(X_kgkg, T_guess_K, isotherm_par);
	     fabs(p_guess_Pa - p_Pa)>tolerance && counter_NRM<50;
		 counter_NRM++) {
		// Calculate pressure depending on concentration and guess values for
		// temperature
		//
		p_guess_Pa = absorption_antoine_p_XT(X_kgkg, T_guess_K, isotherm_par);

		// Calculate the first derivative of the pressure with respect to
		// temperature
		//
		dp_guess_dT_PaK = absorption_antoine_dp_dT_XT(X_kgkg, T_guess_K,
			isotherm_par);

		// Update guess value for temperature
		// Negative temperatures are not allowed
		//
		T_guess_K -= (p_guess_Pa - p_Pa) / dp_guess_dT_PaK;
		T_guess_K = T_guess_K < 0 ? 253.15 : T_guess_K;
	}

	// Return -1 when number of iterations exceeds 50
	//
	return (counter_NRM == 50 ? -1 : T_guess_K);
}


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
	double isotherm_par[]) {
	// Calculate concentrations
	//
	const double dp_Pa = 0.001;

	double X_plus_kgkg = absorption_antoine_X_pT(p_Pa + dp_Pa, T_K,
		isotherm_par);
	double X_minus_kgkg = absorption_antoine_X_pT(p_Pa - dp_Pa, T_K,
		isotherm_par);

	// Calculate derivative of concentration wrt temperature
	//
	return (X_plus_kgkg - X_minus_kgkg) / (2 * dp_Pa);
}


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
	double isotherm_par[]) {
	// Calculate concentrations
	//
	const double dT_K = 0.001;

	double X_plus_kgkg = absorption_antoine_X_pT(p_Pa, T_K + dT_K,
		isotherm_par);
	double X_minus_kgkg = absorption_antoine_X_pT(p_Pa, T_K - dT_K,
		isotherm_par);

	// Calculate derivative of concentration wrt temperature
	//
	return (X_plus_kgkg - X_minus_kgkg) / (2 * dT_K);
}


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
	double isotherm_par[]) {
	// Calculate pressure
	//
	double p_Pa = absorption_antoine_p_XT(X_kgkg, T_K, isotherm_par);

	// Calculate factor
	//
	int i = 0;
	double factor = 0;
	for (i=1; i<5; i++) {
		factor += (isotherm_par[i] + 1000 * isotherm_par[i+5] / (T_K -
		isotherm_par[10])) * pow(X_kgkg, i-1) * i * pow(100, i);
	}

	// Calculate derivative of pressure wrt concentration
	//
	return log(10) * p_Pa * factor;
}


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
	double isotherm_par[]) {
	// Calculate pressure
	//
	double p_Pa = absorption_antoine_p_XT(X_kgkg, T_K, isotherm_par);

	// Calculate factor
	//
	int i = 0;
	double factor = 0;
	for (i=0; i<5; i++) {
		factor += -1000 * isotherm_par[i+5] * pow(100 * X_kgkg, i) / pow(
			T_K - isotherm_par[10], 2);
	}

	// Calculate derivative of pressure wrt temperature
	//
	return log(10) * p_Pa * factor;
}
