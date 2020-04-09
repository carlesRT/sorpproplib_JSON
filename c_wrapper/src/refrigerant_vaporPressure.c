/////////////////////////////////
// refrigerant_vaporPressure.c //
/////////////////////////////////
#include <math.h>
#include <stdio.h>
#include "refrigerant_cubicEoS.h"
#include "refrigerant_vaporPressure.h"


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


/*
 * Approach to calculate saturation properties from cubic equation of state:
 * -------------------------------------------------------------------------
 *
 *	a) Calculate pure component parameters
 *	b) Start iteration for vapor pressure until liquid fugacity equals vapor
 *	   fugacity:
 *
 *		i) Solve cubic equation of state (i.e. calculate Z_l and Z_v)
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
 * refrigerant_p_sat_cubic:
 * ------------------------
 *
 * Calculates vapor pressure in Pa depending on saturation temperature T in K
 * using cubic equation of state.
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
 * refrigerant_p_sat_cubic_internal:
 * ---------------------------------
 *
 * Calculates vapor pressure in Pa depending on saturation temperature T in K
 * using cubic equation of state.
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

	// Initialize variables required for iteration
	//
	int finished = -1;
	int counter = 0;
	const double tolerance = 1e-6;

	double delta = 0;
	double delta_old = 0;
	double p_change = 25000;

	double p = p_guess_Pa;

	// Iterate vapor pressure
	//
	while(finished == -1 && counter < 250) {
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

			if (fabs(delta) <= fabs(tolerance * log(phi_v))) {
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
	return (counter == 250 ? -1 : p);
}


/*
 * refrigerant_T_sat_cubic:
 * ------------------------
 *
 * Calculates saturation temperature in K depending on vapor pressure in Pa
 * using cubic equation of state.
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
 *		Array of doubles that contains coefficients for Antoine-equation.
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
		// VLE exists: Calculate guess value for temperature pressure and
		// execute Newton-Raphson method
		//
		double T_guess_K = refrigerant_par[2] / (1 - (log(p_Pa) -
			log(refrigerant_par[1])) / (log(10) * 7 / 3 * (1 +
			refrigerant_par[3])));
		double p_guess_Pa = p_Pa;
		double dp_guess_dT_PaK = p_guess_Pa / T_guess_K;

		int counter_NRM = 0;
		const double tolerance = 1e-6;

		for (p_guess_Pa = refrigerant_p_sat_cubic(T_guess_K,
				calc_eos_parameters, calc_gen_parameters, calc_fug_coefficient,
				refrigerant_par);
			 fabs(p_guess_Pa - p_Pa)>tolerance && counter_NRM<50;
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
			T_guess_K = T_guess_K < 0 ? 1 : T_guess_K;
		}

		// Return -1 when number of iterations exceeds 50
		//
		return (counter_NRM == 50 ? -1 : T_guess_K);

	}
}


/*
 * refrigerant_dp_sat_dT_cubic:
 * ----------------------------
 *
 * Calculates derivative of vapor pressure wrt. temperature in Pa/K depending
 * on saturation temperature T in K using cubic equation of state.
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
 *	Derivative is calculated numerically by the symmetric derivative using
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
	return (p_plus_Pa - p_minus_Pa) / (2 * dT_K);
}


/*
 * refrigerant_dp_sat_dT_cubic_internal:
 * -------------------------------------
 *
 * Calculates derivative of vapor pressure wrt. temperature in Pa/K depending
 * on saturation temperature T in K using cubic equation of state.
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
 *	Derivative is calculated numerically by the symmetric derivative using
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
	return (p_plus_Pa - p_minus_Pa) / (2 * dT_K);
}
