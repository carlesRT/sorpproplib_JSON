/////////////////////////
// absorption_mixing.c //
/////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "absorption_mixing.h"
#include "refrigerant_cubicEoS.h"


/////////////////////////////
// Definition of constants //
/////////////////////////////
#ifndef IDEAL_GAS_CONSTANT
#define IDEAL_GAS_CONSTANT 8.314462618f
#endif


/////////////////////////////
// Definition of functions //
/////////////////////////////
/*
 * Approach to calculate saturation properties from cubic equation of state:
 * -------------------------------------------------------------------------
 *
 *	a) Calculate all pure component parameters
 *	b) Start outer iteration for vapor pressure until vapor compositions sums
 *	   up to one:
 *
 *		i) Calculate liquid mixing parameter (i.e. use x_i)
 *		ii) Solve cubic equation of state (i.e. calculate Z_l_liq and Z_v_liq)
 *		iii) Calculate liquid fugacity coefficients for all components
 *		iv) Start inner iteration for vapor compositions until vapor
 *			do not change anymore:
 *
 *			1) Calculate vapor mixing parameter (i.e. use y_i)
 *			2) Solve cubic equation of state (i.e. calculate Z_l_vap and
 *			   Z_v_vap)
 *			3) Calculate vapor fugacity coefficients for all components
 *			4) Check if vapor composition do not change anymore:
 *
 *				--> If yes: Convergence achieved
 *				--> If not: Adapt pressure according to fugacity coefficients
 *
 *		v) Check if vapor composition sums up to one:
 *
 *			--> If yes: Convergence achieved
 *			--> If not: Adapt pressure according to vapor composition
 *
 *	c) Check if convergence is achieved:
 *
 *		--> If yes: Return result
 *		--> If no: Special treatment to find solution:
 *
 *			i) Use bisection method to find both, nearest slighter and greater
 *			   liquid composition that have converged solutions
 *			ii) Use linear interpolation wrt. liquid composition to find
 *			    equilibrium pressure and vapor composition
 *
 * Inputs required by user:
 * ------------------------
 *	p: Pressure in p
 *	T: Temperature in K
 *	x: Molar fraction of components in liquid phase
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
 *	Parameter 'MIXING' defines mixing rule:
 *		MIXING = -5: One-parameter van der Waals mixing rule
 *		MIXING = 5: Two-parameter van der Waals mixing rule
 *		MIXING = 10: modified van der Waals and Berthelot mixing rule
 *
 * Order of coefficients in JSON-file:
 * -----------------------------------
 *	isotherm_par[0]		-> EoS		-> in -
 *	isotherm_par[1]		-> MIXING	-> in -
 * 	isotherm_par[2] 	-> p_crit_1	-> in Pa
 *	isotherm_par[3]		-> T_crit_1	-> in K
 * 	isotherm_par[4] 	-> omega_1	-> in -
 * 	isotherm_par[5] 	-> kappa_1_1-> in -
 * 	isotherm_par[6] 	-> beta_0_1	-> in -
 * 	isotherm_par[7] 	-> beta_1_1	-> in -
 * 	isotherm_par[8] 	-> beta_2_1	-> in -
 * 	isotherm_par[9] 	-> beta_3_1	-> in -
 * 	isotherm_par[10]	-> p_crit_2	-> in Pa
 *	isotherm_par[11]	-> T_crit_2	-> in K
 * 	isotherm_par[12]	-> omega_2	-> in -
 * 	isotherm_par[13]	-> kappa_1_2-> in -
 * 	isotherm_par[14]	-> beta_0_2	-> in -
 * 	isotherm_par[15]	-> beta_1_2	-> in -
 * 	isotherm_par[16]	-> beta_2_2	-> in -
 * 	isotherm_par[17]	-> beta_3_2	-> in -
 *	isotherm_par[18]	-> delta_12	-> in -
 * 	isotherm_par[19]	-> m		-> in Pa
 *	isotherm_par[20]	-> l_12		-> in K
 * 	isotherm_par[21]	-> l_21		-> in -
 * 	isotherm_par[22]	-> t		-> in -
 *
 */


/*
 * ret_max:
 * --------
 *
 * Returns the greatest value among the inputs.
 *
 * Parameters:
 * -----------
 *	double a:
 *		First value.
 *	double b:
 *		Second value.
 *
 * Returns:
 * --------
 *	double:
 *		Greatest value.
 *
 * History:
 * --------
 *	04/08/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double ret_max(double a, double b) {
	return (a >= b) ? a : b;
}


/*
 * ret_min:
 * --------
 *
 * Returns the smallest value among the inputs.
 *
 * Parameters:
 * -----------
 *	double a:
 *		First value.
 *	double b:
 *		Second value.
 *
 * Returns:
 * --------
 *	double:
 *		Smallest value.
 *
 * History:
 * --------
 *	04/08/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double ret_min(double a, double b) {
	return (a <= b) ? a : b;
}


/*
 * absorption_mixing_p_Tx:
 * -----------------------
 *
 * Calculates equilibrium pressure p_Pa in Pa depending on temperature T_K in K
 * and mole fraction in liquid phase x_molmol in mol/mol.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double x_1_molmol:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
 *	void (*calc_eos_parameters)(double[], double, double[]):
 *		Function to calculate pure component parameters of cubic equation of
 *		state.
 *	void (*calc_eos_mixing_parameters)(double[], double, double, double, double,
 *		double, double, double[]):
 *		Function to calculate mixing parameters of cubic equation of state.
 *	void (*calc_gen_parameters)(double[], double, double, double, double):
 *		Function to calculate generalized pure component parameters of cubic
 *		equation of state.
 *	double (*calc_fug_coefficient)((int*, int, double, double, double, double,
 *		double, double, double, double, double, double):
 *		Function to calculate fugacity coefficient.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of pure component equations
 *		of state and of mixing rule
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure p_Pa in Pa.
 *	double *ret_y_1_molmol:
 *		Equilibrium mole fraction of first component.
 *	double *ret_y_2_molmol:
 *		Equilibrium mole fraction of second component.
 *
 * History:
 * --------
 *	04/08/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_mixing_p_Tx(double *ret_y_1_molmol, double *ret_y_2_molmol,
	double T_K, double x_1_molmol,
	void (*calc_eos_parameters)(double[], double, double[]),
	void (*calc_eos_mixing_parameters)(double[], double, double, double, double,
		double, double, double[]),
	void (*calc_gen_parameters)(double[], double, double, double, double),
	double (*calc_fug_coefficient)(int*, int, double, double, double, double,
		double, double, double, double, double, double),
	double isotherm_par[]) {
	// Calculate guess value for pressure
	//
	double p_sat_1 = exp(log(isotherm_par[2]) + log(10) * 7 / 3 *
		(1 + isotherm_par[4]) * (1 - isotherm_par[3] / T_K));
	double p_sat_2 = exp(log(isotherm_par[10]) + log(10) * 7 / 3 *
		(1 + isotherm_par[12]) * (1 - isotherm_par[11] / T_K));
	double p_Pa = x_1_molmol * p_sat_1 + (1 - x_1_molmol) * p_sat_2;

	// Execute equilibrium algorithm
	//
	int counter_iterations;

	return absorption_mixing_p_Tx_internal(ret_y_1_molmol, ret_y_2_molmol,
		&counter_iterations, T_K, x_1_molmol, 1, p_Pa, calc_eos_parameters,
		calc_eos_mixing_parameters, calc_gen_parameters, calc_fug_coefficient,
		isotherm_par);
}


/*
 * absorption_mixing_p_Tx_internal:
 * --------------------------------
 *
 * Calculates equilibrium pressure p_Pa in Pa depending on temperature T_K in K
 * and mole fraction in liquid phase x_molmol in mol/mol.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double x_1_molmol:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
 *	int check_conv:
 *		Flag indicating if special treatment is required when solution does not
 *		converge.
 *	double p_guess_Pa:
 *		Guess value for equilibrium pressure in Pa.
 *	void (*calc_eos_parameters)(double[], double, double[]):
 *		Function to calculate pure component parameters of cubic equation of
 *		state.
 *	void (*calc_eos_mixing_parameters)(double[], double, double, double, double,
 *		double, double, double[]):
 *		Function to calculate mixing parameters of cubic equation of state.
 *	void (*calc_gen_parameters)(double[], double, double, double, double):
 *		Function to calculate generalized pure component parameters of cubic
 *		equation of state.
 *	double (*calc_fug_coefficient)((int*, int, double, double, double, double,
 *		double, double, double, double, double, double):
 *		Function to calculate fugacity coefficient.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of pure component equations
 *		of state and of mixing rule
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure p_Pa in Pa.
 *	double *ret_y_1_molmol:
 *		Equilibrium mole fraction of first component.
 *	double *ret_y_2_molmol:
 *		Equilibrium mole fraction of second component.
 *	int *ret_counter_outer:
 *		Counter of outer iterations.
 *
 * History:
 * --------
 *	01/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *	04/08/2020, by Mirko Engelpracht:
 *		Implemented algorithm to calculate VLE properties.
 *
 */
double absorption_mixing_p_Tx_internal(double *ret_y_1_molmol,
	double *ret_y_2_molmol, int *ret_counter_outer, double T_K,
	double x_1_molmol, int check_conv, double p_guess_Pa,
	void (*calc_eos_parameters)(double[], double, double[]),
	void (*calc_eos_mixing_parameters)(double[], double, double, double, double,
		double, double, double[]),
	void (*calc_gen_parameters)(double[], double, double, double, double),
	double (*calc_fug_coefficient)(int*, int, double, double, double, double,
		double, double, double, double, double, double),
	double isotherm_par[]) {
    // Get necessary parameters from function argument
    //
    double x_2_molmol = 1 - x_1_molmol;

	double parameters_1[9];
	parameters_1[0] = isotherm_par[0];
	parameters_1[1] = isotherm_par[2];
	parameters_1[2] = isotherm_par[3];
	parameters_1[3] = isotherm_par[4];
	parameters_1[4] = isotherm_par[5];
	parameters_1[5] = isotherm_par[6];
	parameters_1[6] = isotherm_par[7];
	parameters_1[7] = isotherm_par[8];
	parameters_1[8] = isotherm_par[9];

	double parameters_2[9];
	parameters_2[0] = isotherm_par[0];
	parameters_2[1] = isotherm_par[10];
	parameters_2[2] = isotherm_par[11];
	parameters_2[3] = isotherm_par[12];
	parameters_2[4] = isotherm_par[13];
	parameters_2[5] = isotherm_par[14];
	parameters_2[6] = isotherm_par[15];
	parameters_2[7] = isotherm_par[16];
	parameters_2[8] = isotherm_par[17];

	double mixing_parameter[5];
	mixing_parameter[0] = isotherm_par[18];
	mixing_parameter[1] = isotherm_par[19];
	mixing_parameter[2] = isotherm_par[20];
	mixing_parameter[3] = isotherm_par[21];
	mixing_parameter[4] = isotherm_par[22];

	// Initialize variables
	//
	int err_phi_1_liq, err_phi_2_liq, err_phi_1_vap, err_phi_2_vap;
	double par_eos_1[2], par_eos_2[2];
	double mix_par_eos_liq[4], mix_par_eos_vap[4];
	double gen_par_eos_liq[5], gen_par_eos_vap[5];
	double sol_eos_liq[2], sol_eos_vap[2];
	double phi_1_liq, phi_2_liq, phi_1_vap, phi_2_vap;
	double K_1, K_2, y_1_calc, y_2_calc;

	// Calculate pure component parameters from equation of state
	//
	calc_eos_parameters(par_eos_1, T_K, parameters_1);
	calc_eos_parameters(par_eos_2, T_K, parameters_2);

	// Calculate mixture parameters of liquid phase
	//
	calc_eos_mixing_parameters(mix_par_eos_liq, T_K, x_1_molmol,
		par_eos_1[0], par_eos_2[0], par_eos_1[1], par_eos_2[1],
		mixing_parameter);

	// Calculate guess values for pressure
	//
	double p_Pa = p_guess_Pa;

	// Start outer iteration for bubble pressure calculation
	//
	int finished_outer = -1;
	int finished_inner = -1;
	int counter_outer = 0;
	int counter_inner = 0;
	const double tolerance = 1e-6;

	double delta = 1e6;

	while (finished_outer == -1 && counter_outer < 100) {
		// Calculate solutions of equation of states of liquid phase
		//
		calc_gen_parameters(gen_par_eos_liq, p_Pa, T_K, mix_par_eos_liq[0],
			mix_par_eos_liq[1]);
		refrigerant_cubic_solve(sol_eos_liq, gen_par_eos_liq[2],
			gen_par_eos_liq[3], gen_par_eos_liq[4]);

		// Calculate liquid fugacity coefficients
		//
		phi_1_liq = calc_fug_coefficient(&err_phi_1_liq, 1, x_1_molmol,
			sol_eos_liq[0], gen_par_eos_liq[0], gen_par_eos_liq[1],
			par_eos_1[1], mix_par_eos_liq[1], par_eos_1[0], mix_par_eos_liq[0],
			mix_par_eos_liq[2], mix_par_eos_liq[3]);
		phi_2_liq = calc_fug_coefficient(&err_phi_2_liq, 2, x_2_molmol,
			sol_eos_liq[0], gen_par_eos_liq[0], gen_par_eos_liq[1],
			par_eos_2[1], mix_par_eos_liq[1], par_eos_2[0], mix_par_eos_liq[0],
			mix_par_eos_liq[2], mix_par_eos_liq[3]);

		// Check if equations for liquid fugacity coefficients were solvable
		//
		if (err_phi_1_liq == 0 && err_phi_2_liq == 0) {
			// Could calculate fugacity coefficient: Calculate guess values for
			// vapor composition
			//
			K_1 = exp(log(isotherm_par[2] / p_Pa) + log(10) * 7 / 3 *
				(1 + isotherm_par[4]) * (1 - isotherm_par[3] / T_K));
			K_2 = exp(log(isotherm_par[10] / p_Pa) + log(10) * 7 / 3 *
				(1 + isotherm_par[12]) * (1 - isotherm_par[11] / T_K));

			*ret_y_1_molmol = x_1_molmol * K_1;
			*ret_y_2_molmol = x_2_molmol * K_2;

			// Start inner iteration for vapor composition
			//
			finished_inner = -1;
			counter_inner = 0;

			while (finished_inner == -1 && counter_inner < 50) {
				// Normalize vapor composition and calculate mixture parameters
				// of vapor phase
				//
				y_1_calc = *ret_y_1_molmol /
					(*ret_y_1_molmol + *ret_y_2_molmol);
				y_2_calc = 1 - y_1_calc;

				calc_eos_mixing_parameters(mix_par_eos_vap, T_K, y_1_calc,
					par_eos_1[0], par_eos_2[0], par_eos_1[1], par_eos_2[1],
					mixing_parameter);

				// Calculate solutions of equation of states of vapor phase
				//
				calc_gen_parameters(gen_par_eos_vap, p_Pa, T_K,
					mix_par_eos_vap[0], mix_par_eos_vap[1]);
				refrigerant_cubic_solve(sol_eos_vap, gen_par_eos_vap[2],
					gen_par_eos_vap[3], gen_par_eos_vap[4]);

				// Calculate vapor fugacity coefficients
				//
				phi_1_vap = calc_fug_coefficient(&err_phi_1_vap, 1, y_1_calc,
					sol_eos_vap[1], gen_par_eos_vap[0], gen_par_eos_vap[1],
					par_eos_1[1], mix_par_eos_vap[1], par_eos_1[0],
					mix_par_eos_vap[0], mix_par_eos_vap[2], mix_par_eos_vap[3]);

				phi_2_vap = calc_fug_coefficient(&err_phi_2_vap, 2, y_2_calc,
					sol_eos_vap[1], gen_par_eos_vap[0], gen_par_eos_vap[1],
					par_eos_2[1], mix_par_eos_vap[1], par_eos_2[0],
					mix_par_eos_vap[0], mix_par_eos_vap[2], mix_par_eos_vap[3]);

				// Check if equations for vapor fugacity coefficients were
				// solvable
				//
				if (err_phi_1_vap == 0 && err_phi_2_vap == 0) {
					// Could calculate vapor fugacity coefficient: Check for
					// convergence
					//
					counter_inner++;

					if (fabs(*ret_y_1_molmol - x_1_molmol *
						phi_1_liq / phi_1_vap) <= tolerance) {
						finished_inner = 0;

					} else{
						*ret_y_1_molmol = x_1_molmol * phi_1_liq / phi_1_vap;

					}

					if (fabs(*ret_y_2_molmol - x_2_molmol *
						phi_2_liq / phi_2_vap) <= tolerance) {
						finished_inner = 0;

					} else {
						*ret_y_2_molmol = x_2_molmol * phi_2_liq / phi_2_vap;

					}

				} else {
					// Could not calculate vapor fugacity coefficient: Update
					// vapor composition
					//
					counter_inner++;
					*ret_y_1_molmol *= 0.95;
					*ret_y_2_molmol *= 0.5;

				}
			}

			// Check if vapor compositions sums up to unity
			//
			delta = 1 - *ret_y_1_molmol - *ret_y_2_molmol;

			if (fabs(delta) <= tolerance) {
				// Check for trivial solution
				//
				if (fabs(x_1_molmol - *ret_y_1_molmol) <= pow(tolerance, 2) &&
					fabs(x_2_molmol - *ret_y_2_molmol) <= pow(tolerance, 2)) {
					// Trivial solution: Reduce pressure
					//
					counter_outer++;
					p_Pa *= 0.85;

				} else {
					// Convergence achieved
					//
					finished_outer = 1;

				}

			} else {
				// Update pressure according to number of iterations
				//
				counter_outer++;

				if (counter_outer <= 5) {
					p_Pa *= (*ret_y_1_molmol + *ret_y_2_molmol);

				} else if (counter_outer <= 10) {
					p_Pa *= ret_max(ret_min(*ret_y_1_molmol + *ret_y_2_molmol,
						1.35), 0.65);

				} else if (counter_outer <= 15) {
					p_Pa *= ret_max(ret_min(*ret_y_1_molmol + *ret_y_2_molmol,
						1.25), 0.75);

				} else if (counter_outer <= 20) {
					p_Pa *= ret_max(ret_min(*ret_y_1_molmol + *ret_y_2_molmol,
						1.15), 0.85);

				} else if (counter_outer <= 25) {
					p_Pa *= ret_max(ret_min(*ret_y_1_molmol + *ret_y_2_molmol,
						1.1), 0.9);

				} else if (counter_outer <= 50) {
					p_Pa *= ret_max(ret_min(*ret_y_1_molmol + *ret_y_2_molmol,
						1.05), 0.95);

				} else {
					p_Pa *= ret_max(ret_min(*ret_y_1_molmol + *ret_y_2_molmol,
						1.025), 0.975);

				}
			}

		} else {
			// Calculation of liquid fugacity coefficients is not possible:
			// Reduce pressure
			//
			counter_outer++;
			p_Pa = 0.85 * fabs(p_Pa);

		}
	}

    // Check if linear interpolation shall be used when convergence is not
	// achieved
    //
	*ret_counter_outer = counter_outer;

    if (check_conv > 0) {
        // Use linear interpolation to interpolate pressure between nearest
		// lower and higher liquid compositions that have achieved convergence

        if (counter_outer == 100) {
            // Convergence is not achieved: Interpolation is necessary
            //
			printf("\n\n###########\n# Warning #\n###########");
			printf("\nNO CONVERGENCE -> USE INTERPOLATION TO FIND SOLUTION.");

            // Use bisection method to find nearest lower and higher liquid
			// compositions that have solution that converged
            //
			double x_low, y_1_low, y_2_low, p_low, p_sat_1, p_sat_2, p_guess;
            double x_upper = x_1_molmol;
            double x_lower = 0.99 * x_upper;
            double x_calc = x_lower;
			double x_calc_old = x_upper;
			double ret_y_1_molmol_bsm, ret_y_2_molmol_bsm;

			int ret_counter_bsm = 0;
			int cou_bsm = 0;
			const double tol_bsm = 1e-12;

			 while(fabs(x_calc_old - x_calc) >= tol_bsm && cou_bsm < 100) {
				// Update values required to check for convergence
				//
				cou_bsm++;
				x_calc_old = x_calc;

				// Try to calculate vapor pressure
				//
				p_sat_1 = exp(log(isotherm_par[2]) + log(10) * 7 / 3 *
					(1 + isotherm_par[4]) * (1 - isotherm_par[3] / T_K));
				p_sat_2 = exp(log(isotherm_par[10]) + log(10) * 7 / 3 *
					(1 + isotherm_par[12]) * (1 - isotherm_par[11] / T_K));
				p_guess = x_calc * p_sat_1 + (1 - x_calc) * p_sat_2;

				absorption_mixing_p_Tx_internal(&ret_y_1_molmol_bsm,
					&ret_y_2_molmol_bsm, &ret_counter_bsm, T_K, x_calc, -1,
					p_guess, calc_eos_parameters, calc_eos_mixing_parameters,
					calc_gen_parameters, calc_fug_coefficient, isotherm_par);

				// Adapt liquid composition
				//
                if (ret_counter_bsm == 100)  {
                    // Solution did not converge: Reduce lower boundary
                    //
                    x_calc *= 0.99;
                    x_lower = x_calc;

                } else {
                    // Solution did converge: Increase upper boundary
                    //
                    x_calc = (x_calc + x_upper) / 2;
                    x_upper = x_calc;

				}
			}

			// Calculate final solution
			//
            if (ret_counter_bsm == 100) {
				printf("\n\n###########\n# Warning #\n###########");
				printf("\nCOULD NOT FIND SOLUTION -> RETURN OLD SOLTION.");
                return p_Pa;

            } else {
                // Calculate lower boundary required for linear interpolation
                //
				x_low = x_lower;

				p_sat_1 = exp(log(isotherm_par[2]) + log(10) * 7 / 3 *
					(1 + isotherm_par[4]) * (1 - isotherm_par[3] / T_K));
				p_sat_2 = exp(log(isotherm_par[10]) + log(10) * 7 / 3 *
					(1 + isotherm_par[12]) * (1 - isotherm_par[11] / T_K));
				p_guess = x_low * p_sat_1 + (1 - x_low) * p_sat_2;

				p_low = absorption_mixing_p_Tx_internal(&y_1_low, &y_2_low,
					&ret_counter_bsm, T_K, x_low, -1, p_guess,
					calc_eos_parameters, calc_eos_mixing_parameters,
					calc_gen_parameters, calc_fug_coefficient, isotherm_par);

			}


			double x_high, y_1_high, y_2_high, p_high;
            x_lower = x_1_molmol;
            x_upper = 1.01 * x_lower;
            x_calc = x_upper;
			x_calc_old = x_lower;

			cou_bsm = 0;

			 while(fabs(x_calc_old - x_calc) >= tol_bsm && cou_bsm < 100) {
				// Update values required to check for convergence
				//
				cou_bsm++;
				x_calc_old = x_calc;

				// Try to calculate vapor pressure
				//
				p_sat_1 = exp(log(isotherm_par[2]) + log(10) * 7 / 3 *
					(1 + isotherm_par[4]) * (1 - isotherm_par[3] / T_K));
				p_sat_2 = exp(log(isotherm_par[10]) + log(10) * 7 / 3 *
					(1 + isotherm_par[12]) * (1 - isotherm_par[11] / T_K));
				p_guess = x_calc * p_sat_1 + (1 - x_calc) * p_sat_2;

				absorption_mixing_p_Tx_internal(&ret_y_1_molmol_bsm,
					&ret_y_2_molmol_bsm, &ret_counter_bsm, T_K, x_calc, -1,
					p_guess, calc_eos_parameters, calc_eos_mixing_parameters,
					calc_gen_parameters, calc_fug_coefficient, isotherm_par);

				// Adapt liquid composition
				//
                if (ret_counter_bsm == 100)  {
                    // Solution did not converge: Reduce lower boundary
                    //
                    x_calc *= 1.01;
                    x_upper = x_calc;

                } else {
                    // Solution did converge: Increase upper boundary
                    //
                    x_calc = (x_calc + x_lower) / 2;
                    x_lower = x_calc;

				}
			}

			// Calculate final solution
			//
            if (ret_counter_bsm == 100) {
				printf("\n\n###########\n# Warning #\n###########");
				printf("\nCOULD NOT FIND SOLUTION -> RETURN OLD SOLTION.");
                return p_Pa;

            } else {
                // Calculate lower boundary required for linear interpolation
                //
				x_high = x_upper;

				p_sat_1 = exp(log(isotherm_par[2]) + log(10) * 7 / 3 *
					(1 + isotherm_par[4]) * (1 - isotherm_par[3] / T_K));
				p_sat_2 = exp(log(isotherm_par[10]) + log(10) * 7 / 3 *
					(1 + isotherm_par[12]) * (1 - isotherm_par[11] / T_K));
				p_guess = x_high * p_sat_1 + (1 - x_high) * p_sat_2;

				p_high = absorption_mixing_p_Tx_internal(&y_1_high, &y_2_high,
					&ret_counter_bsm, T_K, x_high, -1, p_guess_Pa,
					calc_eos_parameters, calc_eos_mixing_parameters,
					calc_gen_parameters, calc_fug_coefficient, isotherm_par);

			}

			// Interpolate pressure and vapor composition
			//
            double m = (p_high-p_low) / (x_high - x_low);
            double b = p_high - x_high * m;
            p_Pa = b + m * x_1_molmol;

            m = (y_1_high-y_1_low) / (x_high - x_low);
            b = y_1_high - x_high * m;
            *ret_y_1_molmol = b + m * x_1_molmol;
            *ret_y_2_molmol = fabs(1 - *ret_y_1_molmol);

            return p_Pa;

        } else {
			// Convergence achieved: No special treatment necessary
			//
            return p_Pa;

		}

    } else {
        // No special treatment if convergence is not achieved
        //
        return p_Pa;

	}
}


/*
 * absorption_mixing_dp_dT_Tx:
 * ---------------------------
 *
 * Calculates derivative of equilibrium pressure with respect to temperature in
 * Pa/K depending on temperature T_K in K and mole fraction in liquid phase
 * x_molmol in mol/mol.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double x_1_molmol:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
 *	void (*calc_eos_parameters)(double[], double, double[]):
 *		Function to calculate pure component parameters of cubic equation of
 *		state.
 *	void (*calc_eos_mixing_parameters)(double[], double, double, double, double,
 *		double, double, double[]):
 *		Function to calculate mixing parameters of cubic equation of state.
 *	void (*calc_gen_parameters)(double[], double, double, double, double):
 *		Function to calculate generalized pure component parameters of cubic
 *		equation of state.
 *	double (*calc_fug_coefficient)((int*, int, double, double, double, double,
 *		double, double, double, double, double, double):
 *		Function to calculate fugacity coefficient.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of pure component equations
 *		of state and of mixing rule
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt temperature in Pa/K.
 *
 * Remarks:
 * --------
 *	Derivative is calculated numerically by the symmetric derivative using
 *  h = 0.0001 K as small change.
 *
 * History:
 * --------
 *	04/08/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_mixing_dp_dT_Tx(double T_K, double x_1_molmol,
	void (*calc_eos_parameters)(double[], double, double[]),
	void (*calc_eos_mixing_parameters)(double[], double, double, double, double,
		double, double, double[]),
	void (*calc_gen_parameters)(double[], double, double, double, double),
	double (*calc_fug_coefficient)(int*, int, double, double, double, double,
		double, double, double, double, double, double),
	double isotherm_par[]) {
	// Calculate vapor pressures
	//
	double ret_y_1_molmol, ret_y_2_molmol;
	const double dT_K = 0.0001;

	double p_plus_Pa = absorption_mixing_p_Tx(&ret_y_1_molmol,
		&ret_y_2_molmol, T_K + dT_K, x_1_molmol,
		calc_eos_parameters, calc_eos_mixing_parameters,
		calc_gen_parameters, calc_fug_coefficient, isotherm_par);
	double p_minus_Pa = absorption_mixing_p_Tx(&ret_y_1_molmol,
		&ret_y_2_molmol, T_K - dT_K, x_1_molmol,
		calc_eos_parameters, calc_eos_mixing_parameters,
		calc_gen_parameters, calc_fug_coefficient, isotherm_par);

	// Calculate derivative of vapor pressure wrt. temperature
	//
	return (p_plus_Pa - p_minus_Pa) / (2 * dT_K);
}


/*
 * absorption_mixing_dp_dT_Tx_internal:
 * ------------------------------------
 *
 * Calculates derivative of equilibrium pressure with respect to temperature in
 * Pa/K depending on temperature T_K in K and mole fraction in liquid phase
 * x_molmol in mol/mol.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double x_1_molmol:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
 *	double p_guess_Pa:
 *		Guess value for equilibrium pressure in Pa.
 *	void (*calc_eos_parameters)(double[], double, double[]):
 *		Function to calculate pure component parameters of cubic equation of
 *		state.
 *	void (*calc_eos_mixing_parameters)(double[], double, double, double, double,
 *		double, double, double[]):
 *		Function to calculate mixing parameters of cubic equation of state.
 *	void (*calc_gen_parameters)(double[], double, double, double, double):
 *		Function to calculate generalized pure component parameters of cubic
 *		equation of state.
 *	double (*calc_fug_coefficient)((int*, int, double, double, double, double,
 *		double, double, double, double, double, double):
 *		Function to calculate fugacity coefficient.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of pure component equations
 *		of state and of mixing rule
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt temperature in Pa/K.
 *
 * Remarks:
 * --------
 *	Derivative is calculated numerically by the symmetric derivative using
 *  h = 0.0001 K as small change.
 *
 * History:
 * --------
 *	04/08/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_mixing_dp_dT_Tx_internal(double T_K, double x_1_molmol,
	double p_guess_Pa, void (*calc_eos_parameters)(double[], double, double[]),
	void (*calc_eos_mixing_parameters)(double[], double, double, double, double,
		double, double, double[]),
	void (*calc_gen_parameters)(double[], double, double, double, double),
	double (*calc_fug_coefficient)(int*, int, double, double, double, double,
		double, double, double, double, double, double),
	double isotherm_par[]) {
	// Calculate vapor pressures
	//
	int counter_iterations;
	double ret_y_1_molmol, ret_y_2_molmol;
	const double dT_K = 0.0001;

	double p_plus_Pa = absorption_mixing_p_Tx_internal(&ret_y_1_molmol,
		&ret_y_2_molmol, &counter_iterations, T_K + dT_K, x_1_molmol, 1,
		p_guess_Pa, calc_eos_parameters, calc_eos_mixing_parameters,
		calc_gen_parameters, calc_fug_coefficient, isotherm_par);
	double p_minus_Pa = absorption_mixing_p_Tx_internal(&ret_y_1_molmol,
		&ret_y_2_molmol, &counter_iterations, T_K - dT_K, x_1_molmol, 1,
		p_guess_Pa, calc_eos_parameters, calc_eos_mixing_parameters,
		calc_gen_parameters, calc_fug_coefficient, isotherm_par);

	// Calculate derivative of vapor pressure wrt. temperature
	//
	return (p_plus_Pa - p_minus_Pa) / (2 * dT_K);
}


/*
 * absorption_mixing_dp_dx_Tx:
 * ---------------------------
 *
 * Calculates derivative of equilibrium pressure with respect to liquid mole
 * fraction in Pa depending on temperature T_K in K and mole fraction in liquid
 * phase x_molmol in mol/mol.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double x_1_molmol:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
 *	void (*calc_eos_parameters)(double[], double, double[]):
 *		Function to calculate pure component parameters of cubic equation of
 *		state.
 *	void (*calc_eos_mixing_parameters)(double[], double, double, double, double,
 *		double, double, double[]):
 *		Function to calculate mixing parameters of cubic equation of state.
 *	void (*calc_gen_parameters)(double[], double, double, double, double):
 *		Function to calculate generalized pure component parameters of cubic
 *		equation of state.
 *	double (*calc_fug_coefficient)((int*, int, double, double, double, double,
 *		double, double, double, double, double, double):
 *		Function to calculate fugacity coefficient.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of pure component equations
 *		of state and of mixing rule
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt liquid mole fraction in Pa.
 *
 * Remarks:
 * --------
 *	Derivative is calculated numerically by the symmetric derivative using
 *  h = 0.00000001 K as small change.
 *
 * History:
 * --------
 *	04/08/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_mixing_dp_dx_Tx(double T_K, double x_1_molmol,
	void (*calc_eos_parameters)(double[], double, double[]),
	void (*calc_eos_mixing_parameters)(double[], double, double, double, double,
		double, double, double[]),
	void (*calc_gen_parameters)(double[], double, double, double, double),
	double (*calc_fug_coefficient)(int*, int, double, double, double, double,
		double, double, double, double, double, double),
	double isotherm_par[]) {
	// Calculate vapor pressures
	//
	double ret_y_1_molmol, ret_y_2_molmol;
	const double dx_molmol = 1e-8;

	double p_plus_Pa = absorption_mixing_p_Tx(&ret_y_1_molmol,
		&ret_y_2_molmol, T_K, x_1_molmol + dx_molmol,
		calc_eos_parameters, calc_eos_mixing_parameters,
		calc_gen_parameters, calc_fug_coefficient, isotherm_par);
	double p_minus_Pa = absorption_mixing_p_Tx(&ret_y_1_molmol,
		&ret_y_2_molmol, T_K, x_1_molmol - dx_molmol,
		calc_eos_parameters, calc_eos_mixing_parameters,
		calc_gen_parameters, calc_fug_coefficient, isotherm_par);

	// Calculate derivative of vapor pressure wrt. temperature
	//
	return (p_plus_Pa - p_minus_Pa) / (2 * dx_molmol);
}


/*
 * absorption_mixing_T_px:
 * -----------------------
 *
 * Calculates equilibrium temperature T_K in K depending on pressure p_Pa in Pa
 * and mole fraction in liquid phase x_molmol in mol/mol.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure p_Pa in Pa.
 *	double x_1_molmol:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
 *	void (*calc_eos_parameters)(double[], double, double[]):
 *		Function to calculate pure component parameters of cubic equation of
 *		state.
 *	void (*calc_eos_mixing_parameters)(double[], double, double, double, double,
 *		double, double, double[]):
 *		Function to calculate mixing parameters of cubic equation of state.
 *	void (*calc_gen_parameters)(double[], double, double, double, double):
 *		Function to calculate generalized pure component parameters of cubic
 *		equation of state.
 *	double (*calc_fug_coefficient)((int*, int, double, double, double, double,
 *		double, double, double, double, double, double):
 *		Function to calculate fugacity coefficient.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of pure component equations
 *		of state and of mixing rule
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
 *	double *ret_y_1_molmol:
 *		Equilibrium mole fraction of first component.
 *	double *ret_y_2_molmol:
 *		Equilibrium mole fraction of second component.
 *
 * Remarks:
 * --------
 *	Uses Newton-Raphson method for calculating equilibrium temperature.
 *
 * History:
 * --------
 *	04/08/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_mixing_T_px(double *ret_y_1_molmol, double *ret_y_2_molmol,
	double p_Pa, double x_1_molmol,
	void (*calc_eos_parameters)(double[], double, double[]),
	void (*calc_eos_mixing_parameters)(double[], double, double, double, double,
		double, double, double[]),
	void (*calc_gen_parameters)(double[], double, double, double, double),
	double (*calc_fug_coefficient)(int*, int, double, double, double, double,
		double, double, double, double, double, double),
	double isotherm_par[]) {
		// Execute Newton-Raphson method
		//
		double T_guess_K = isotherm_par[3] / (1 - (log(p_Pa) -
			log(isotherm_par[2])) / (log(10) * 7 / 3 * (1 + isotherm_par[4])));
		double p_guess_Pa = p_Pa;
		double dp_guess_dT_PaK = p_guess_Pa / T_guess_K;

		int counter_NRM = 0;
		const double tolerance = 1e-6;

		for (p_guess_Pa = absorption_mixing_p_Tx(ret_y_1_molmol,
				ret_y_2_molmol, T_guess_K, x_1_molmol, calc_eos_parameters,
				calc_eos_mixing_parameters, calc_gen_parameters,
				calc_fug_coefficient, isotherm_par);
			 fabs(p_guess_Pa - p_Pa) > tolerance && counter_NRM < 50;
			 counter_NRM++) {
			// Calculate pressure depending on guess value for temperature
			//
			p_guess_Pa = absorption_mixing_p_Tx(ret_y_1_molmol,
				ret_y_2_molmol, T_guess_K, x_1_molmol, calc_eos_parameters,
				calc_eos_mixing_parameters, calc_gen_parameters,
				calc_fug_coefficient, isotherm_par);

			// Calculate derivative of the loading with respect to pressure
			//
			dp_guess_dT_PaK = absorption_mixing_dp_dT_Tx_internal(T_guess_K,
				x_1_molmol, p_guess_Pa, calc_eos_parameters,
				calc_eos_mixing_parameters, calc_gen_parameters,
				calc_fug_coefficient, isotherm_par);

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


/*
 * absorption_mixing_x_pT:
 * -----------------------
 *
 * Calculates equilibrium liquid mole fraction of first component in mol/mol
 * depending on pressure in Pa and temperature T_K in K.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure p_Pa in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	void (*calc_eos_parameters)(double[], double, double[]):
 *		Function to calculate pure component parameters of cubic equation of
 *		state.
 *	void (*calc_eos_mixing_parameters)(double[], double, double, double, double,
 *		double, double, double[]):
 *		Function to calculate mixing parameters of cubic equation of state.
 *	void (*calc_gen_parameters)(double[], double, double, double, double):
 *		Function to calculate generalized pure component parameters of cubic
 *		equation of state.
 *	double (*calc_fug_coefficient)((int*, int, double, double, double, double,
 *		double, double, double, double, double, double):
 *		Function to calculate fugacity coefficient.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of pure component equations
 *		of state and of mixing rule
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
 *	double *ret_y_1_molmol:
 *		Equilibrium mole fraction of first component.
 *	double *ret_y_2_molmol:
 *		Equilibrium mole fraction of second component.
 *
 * History:
 * --------
 *	04/08/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_mixing_x_pT(double *ret_y_1_molmol, double *ret_y_2_molmol,
	double p_Pa, double T_K,
	void (*calc_eos_parameters)(double[], double, double[]),
	void (*calc_eos_mixing_parameters)(double[], double, double, double, double,
		double, double, double[]),
	void (*calc_gen_parameters)(double[], double, double, double, double),
	double (*calc_fug_coefficient)(int*, int, double, double, double, double,
		double, double, double, double, double, double),
	double isotherm_par[]) {
    // Get necessary parameters from function argument
    //
	double parameters_1[9];
	parameters_1[0] = isotherm_par[0];
	parameters_1[1] = isotherm_par[2];
	parameters_1[2] = isotherm_par[3];
	parameters_1[3] = isotherm_par[4];
	parameters_1[4] = isotherm_par[5];
	parameters_1[5] = isotherm_par[6];
	parameters_1[6] = isotherm_par[7];
	parameters_1[7] = isotherm_par[8];
	parameters_1[8] = isotherm_par[9];

	double parameters_2[9];
	parameters_2[0] = isotherm_par[0];
	parameters_2[1] = isotherm_par[10];
	parameters_2[2] = isotherm_par[11];
	parameters_2[3] = isotherm_par[12];
	parameters_2[4] = isotherm_par[13];
	parameters_2[5] = isotherm_par[14];
	parameters_2[6] = isotherm_par[15];
	parameters_2[7] = isotherm_par[16];
	parameters_2[8] = isotherm_par[17];

	double mixing_parameter[5];
	mixing_parameter[0] = isotherm_par[18];
	mixing_parameter[1] = isotherm_par[19];
	mixing_parameter[2] = isotherm_par[20];
	mixing_parameter[3] = isotherm_par[21];
	mixing_parameter[4] = isotherm_par[22];

	// Initialize variables
	//
	int err_phi_1_liq, err_phi_2_liq, err_phi_1_vap, err_phi_2_vap;
	double par_eos_1[2], par_eos_2[2];
	double mix_par_eos_liq[4], mix_par_eos_vap[4];
	double gen_par_eos_liq[5], gen_par_eos_vap[5];
	double sol_eos_liq[2], sol_eos_vap[2];
	double phi_1_liq, phi_2_liq, phi_1_vap, phi_2_vap;
	double x_1_calc, x_2_calc, y_1_calc, y_2_calc;

	// Calculate pure component parameters from equation of state
	//
	calc_eos_parameters(par_eos_1, T_K, parameters_1);
	calc_eos_parameters(par_eos_2, T_K, parameters_2);

	// Set initial values for iteration
	//
	double x_1_molmol = 0.25;
	double x_2_molmol = 1 - x_1_molmol;
	*ret_y_1_molmol = 0.99;
	*ret_y_2_molmol = 1 - x_1_molmol;

	// Start outer iteration for bubble pressure calculation
	//
	int finished = -1;
	int counter = 0;
	const double tolerance = 1e-6;

	double delta = 1e6;

	while (finished == -1 && counter < 250) {
		// Normalize liquid and vapor composition and calculate mixture
		// parameters of liquid and vapor phase
		//
		x_1_calc = x_1_molmol;
		x_2_calc = 1 - x_1_calc;
		y_1_calc = *ret_y_1_molmol;
		y_2_calc = 1 - y_1_calc;

		// Calculate mixture parameters of liquid and vapor phase
		//
		calc_eos_mixing_parameters(mix_par_eos_liq, T_K, x_1_calc,
			par_eos_1[0], par_eos_2[0], par_eos_1[1], par_eos_2[1],
			mixing_parameter);

		calc_eos_mixing_parameters(mix_par_eos_vap, T_K, y_1_calc,
			par_eos_1[0], par_eos_2[0], par_eos_1[1], par_eos_2[1],
			mixing_parameter);

		// Calculate solutions of equation of states of liquid phase
		//
		calc_gen_parameters(gen_par_eos_liq, p_Pa, T_K, mix_par_eos_liq[0],
			mix_par_eos_liq[1]);
		refrigerant_cubic_solve(sol_eos_liq, gen_par_eos_liq[2],
			gen_par_eos_liq[3], gen_par_eos_liq[4]);

		// Calculate solutions of equation of states of vapor phase
		//
		calc_gen_parameters(gen_par_eos_vap, p_Pa, T_K, mix_par_eos_vap[0],
			mix_par_eos_vap[1]);
		refrigerant_cubic_solve(sol_eos_vap, gen_par_eos_vap[2],
			gen_par_eos_vap[3], gen_par_eos_vap[4]);

		// Calculate liquid fugacity coefficients
		//
		phi_1_liq = calc_fug_coefficient(&err_phi_1_liq, 1, x_1_calc,
			sol_eos_liq[0], gen_par_eos_liq[0], gen_par_eos_liq[1],
			par_eos_1[1], mix_par_eos_liq[1], par_eos_1[0], mix_par_eos_liq[0],
			mix_par_eos_liq[2], mix_par_eos_liq[3]);
		phi_2_liq = calc_fug_coefficient(&err_phi_2_liq, 2, x_2_calc,
			sol_eos_liq[0], gen_par_eos_liq[0], gen_par_eos_liq[1],
			par_eos_2[1], mix_par_eos_liq[1], par_eos_2[0], mix_par_eos_liq[0],
			mix_par_eos_liq[2], mix_par_eos_liq[3]);

		// Calculate vapor fugacity coefficients
		//
		phi_1_vap = calc_fug_coefficient(&err_phi_1_vap, 1, y_1_calc,
			sol_eos_vap[1], gen_par_eos_vap[0], gen_par_eos_vap[1],
			par_eos_1[1], mix_par_eos_vap[1], par_eos_1[0], mix_par_eos_vap[0],
			mix_par_eos_vap[2], mix_par_eos_vap[3]);

		phi_2_vap = calc_fug_coefficient(&err_phi_2_vap, 2, y_2_calc,
			sol_eos_vap[1], gen_par_eos_vap[0], gen_par_eos_vap[1],
			par_eos_2[1], mix_par_eos_vap[1], par_eos_2[0], mix_par_eos_vap[0],
			mix_par_eos_vap[2], mix_par_eos_vap[3]);

		// Check if equations for fugacity coefficients were solvable
		//
		if (err_phi_1_liq == 0 && err_phi_2_liq == 0 &&
			err_phi_1_vap == 0 && err_phi_2_vap == 0) {
			// Could solve equations for fugacity coefficients: Check for
			// convergence
			counter++;
			delta = fabs(x_1_molmol * phi_1_liq - *ret_y_1_molmol * phi_1_vap) +
				fabs(x_2_molmol * phi_2_liq - *ret_y_2_molmol * phi_2_vap);

			if (delta <= tolerance) {
				// Convergence achieved
				//
				finished = 1;

			} else {
				// Convergence not achieved: Update compositions
				//
				x_1_molmol = phi_1_vap * (phi_2_vap - phi_2_liq) /
					(phi_1_liq * phi_2_vap - phi_1_vap * phi_2_liq);
				x_2_molmol = 1 - x_1_molmol;


				*ret_y_1_molmol = x_1_molmol * phi_1_liq / phi_1_vap;
				*ret_y_2_molmol = 1 - *ret_y_1_molmol;

			}

		} else {
			// Could solve equations for fugacity coefficients: Update guess
			// values for compositions
			//
			counter++;

			if (err_phi_1_liq != 0 || err_phi_2_liq != 0) {
				// Update guess values for liquid composition
				//
				x_1_molmol *= 1.05;
				x_2_molmol = 1 - x_1_molmol;

			}
			if (err_phi_1_vap != 0 || err_phi_2_vap != 0) {
				// Update guess values for liquid composition
				//
				*ret_y_1_molmol *= 0.95;
				*ret_y_2_molmol = 1 - *ret_y_1_molmol;

			}
		}
	}

	return x_1_molmol;
}
