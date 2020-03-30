///////////////////////////////
// absorption_mixing_1pvdw.c //
///////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "absorption_mixing_1pvdw.h"


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
 * General form of Peng-Robinson or Soave-Redlich-Kwong equation when using
 * one parameter van der Waals mixing rule:
 * ----------------------------------------
 * 	p = R * T / (V_m - b) - a / (V_m^2 + m * b * V_m + n * b^2)
 *
 *		with: For PR equation, m = 2 and n = -1
 *		----- For SRK equation, m = 1 and n = 0
 *
 * Temperature-dependent coefficients:
 * -----------------------------------
 * 	a = x_1^2 * a_1 + x_2^2 * a_2 + 2 * x_1 * x_2 * a_12
 *	b = x_1 * b_1 + x_2 * b_2
 *
 *		with: a_12 = sqrt(a_1 * a_2) * (1- k_12)
 *		----- a_1 = alpha_0 * R^2 * T_crit,1^2 / p_crit,1 * alpha_1
 *			  a_2 = alpha_0 * R^2 * T_crit,2^2 / p_crit,2 * alpha_2
 *			  alpha_1 = (1 + m_1 * (1 - sqrt(T / T_crit,1)))^2
 *			  alpha_2 = (1 + m_2 * (1 - sqrt(T / T_crit,2)))^2
 *			  m_1 = c_0 + c_1 * w_1 + c_2 * w_1^2
 *			  m_2 = c_0 + c_1 * w_2 + c_2 * w_2^2
 *			  b_1 = b_0 * R * T_crit,1 / p_crit,1
 *			  b_2 = b_0 * R * T_crit,2 / p_crit,2
 *
 *				with: For PR equation, alpha_0 = 0.45724, c_0 = 0.37464,
 *				----- c_1 = 1.54226, c_2 = -0.26992 and b_0 = 0.077796
 *					  For SRK equation, alpha_0 = 1/(9*(2^(1/3)-1)),
 *					  c_0 = 0.480, c_1 = 1.574, c_2 = -0.176 and b_0 = 0.08664
 *
 * Possible inputs required by user:
 * ---------------------------------
 *	T: Temperature in K
 *	x: Mole fraction in liquid phase in mol/mol
 *	v: Molar volume in m³/mol
 *
 * Order of coefficients in JSON-file:
 * -----------------------------------
 *	isotherm_par[0]	-> EOS		-> in -
 * 	isotherm_par[1] -> w_1		-> in -
 * 	isotherm_par[2] -> w_2		-> in -
 * 	isotherm_par[3] -> k_12		-> in -
 * 	isotherm_par[4] -> T_crit1	-> in K
 * 	isotherm_par[5] -> T_crit2	-> in K
 * 	isotherm_par[6] -> p_crit1	-> in Pa
 * 	isotherm_par[7] -> p_crit2	-> in Pa
 *
 */


/*
 * absorption_mixing_1pvdw_p_Tx:
 * -----------------------------
 *
 * Calculates equilibrium pressure p_Pa in Pa of first component depending on
 * temperature T_K in K and mole fraction in liquid phase x_molmol in mol/mol.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double x_molmol:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of "one parameter van der
 *		Waals mixing" equation.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure p_Pa in Pa.
 *
 * History:
 * --------
 *	01/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_mixing_1pvdw_p_Tx(double T_K, double x_molmol,
	double isotherm_par[]) {
	// Check type of equation of state
	//
	/*
	double m, n, alpha_0, c_0, c_1, c_2, b_0;

	if (isotherm_par[0] > 5) {
		// Peng-Robinson equation
		//
		m = 2;
		n = -1;
		alpha_0 = 0.45724;
		c_0 = 0.37464;
		c_1 = 1.54226;
		c_2 = -0.26992;
		b_0 = 0.077796;

	} else if (isotherm_par[0] < -5) {
		// Soave-Redlich-Kwong equation
		//
		m = 1;
		n = 0;
		alpha_0 = 1 / (9 * (pow(2, 1/3) - 1));
		c_0 = 0.480;
		c_1 = 1.574;
		c_2 = -0.176;
		b_0 = 0.08664;

	} else {
		// Nothing implemented yet
		//
		m = 0;
		n = 0;
		alpha_0 = 0;
		c_0 = 0;
		c_1 = 0;
		c_2 = 0;
		b_0 = 0;

	}

	// Calculate mole fractions
	//
	double x_1 = x_molmol;
	double x_2 = 1 - x_1;

	// Calculate a
	//
	double m_1 = c_0 + c_1 * isotherm_par[1] + c_2 * pow(isotherm_par[1], 2);
	double m_2 = c_0 + c_1 * isotherm_par[2] + c_2 * pow(isotherm_par[2], 2);
	double alpha_1 = pow(1 + m_1 * (1 - sqrt(T_K / isotherm_par[4])), 2);
	double alpha_2 = pow(1 + m_2 * (1 - sqrt(T_K / isotherm_par[5])), 2);

	double a_1 = alpha_0 * pow(IDEAL_GAS_CONSTANT * isotherm_par[4], 2) /
		isotherm_par[6] * alpha_1;
	double a_2 = alpha_0 * pow(IDEAL_GAS_CONSTANT * isotherm_par[5], 2) /
		isotherm_par[7] * alpha_2;
	double a_12 = sqrt(a_1 * a_2) * (1- isotherm_par[3]);

	double a = pow(x_1, 2) * a_1 + pow(x_2, 2) * a_2 + 2 * x_1 * x_2 * a_12;

	// Calculate b
	//
	double b_1 = b_0 * IDEAL_GAS_CONSTANT * isotherm_par[4] / isotherm_par[6];
	double b_2 = b_0 * IDEAL_GAS_CONSTANT * isotherm_par[5] / isotherm_par[7];

	double b = x_1 * b_1 + x_2 * b_2;
	
	
	double p = IDEAL_GAS_CONSTANT * T_K / (v_m3mol - b) - a / (pow(v_m3mol, 2) +
		m * v_m3mol * b + n * pow(b, 2));
	p = p;
	*/
	/*
	// Calculate mole fractions
	//
	double x_1 = x_molmol;
	double x_2 = 1 - x_1;	
	
	// Soave-Redlich-Kwong equation (SRK equation)
	//
	// Calculate coefficients of SRK equation, which has the following
	// functional form:
	//
	//	p = R * T / (V - b) - a / (V^2 + b * V)
	//
	//		with coefficients according to van der Waals one-fluid mixing rule:
	//		-------------------------------------------------------------------
	//			a = x_1^2 * a_1 + 2 * x_1 * x_2 * a_12 + x_2^2 * a_2
	//			a_12 = sqrt(a_1 * a_2) * (1 - k_12)
	//			a_1 = alpha_0 * R^2 * T_crit_1^2 / p_crit_1 * alpha_1
	//			a_2 = alpha_0 * R^2 * T_crit_2^2 / p_crit_2 * alpha_2
	//			alpha_1 = (1 + m_1 * (1 - sqrt(T / T_crit_1)))^2
	//			alpha_2 = (1 + m_2 * (1 - sqrt(T / T_crit_2)))^2
	//			m_1 = c_0 + c_1 * omega_1 - c_2 * omega_1^2
	//			m_2 = c_0 + c_1 * omega_2 - c_2 * omega_2^2
	//
	//			b = x_1 * b_1 + x_2 * b_2
	//			b_1 = b_0 * R * T_crit_1 / p_crit_1
	//			b_2 = b_0 * R * T_crit_2 / p_crit_2
	//
	isotherm_par = isotherm_par;
	double k_12 = -0.0018;
	double w_1 = 0.2236;
	double w_2 = 0.3334;
	double T_crit_1 = 304.2;
	double T_crit_2 = 1079.6;
	double p_crit_1 = 7380000;
	double p_crit_2 = 2588000;
	
	double alpha_0 = 1 / (9 * (pow(2, 0.333333333f) - 1));
	double c_0 = 0.480;
	double c_1 = 1.574;
	double c_2 = -0.176;
	double b_0 = 0.08664;
	
	double m_1 = c_0 + c_1 * w_1 - c_2 * pow(w_1, 2);
	double m_2 = c_0 + c_1 * w_2 - c_2 * pow(w_2, 2);
	double alpha_1 = pow(1 + m_1 * (1 - sqrt(T_K / T_crit_1)), 2);
	double alpha_2 = pow(1 + m_2 * (1 - sqrt(T_K / T_crit_2)), 2);
	double a_1 = alpha_0 * pow(IDEAL_GAS_CONSTANT * T_crit_1, 2) /
		p_crit_1 * alpha_1;
	double a_2 = alpha_0 * pow(IDEAL_GAS_CONSTANT * T_crit_2, 2) /
		p_crit_2 * alpha_2;
	double a_12 = sqrt(a_1 * a_2) * (1 - k_12);
	double a = pow(x_1, 2) * a_1 + 2 * x_1 * x_2 * a_12 + pow(x_2, 2) * a_2;
	
	double b_1 = b_0 * IDEAL_GAS_CONSTANT * T_crit_1 / p_crit_1;
	double b_2 = b_0 * IDEAL_GAS_CONSTANT * T_crit_2 / p_crit_2;
	double b = x_1 * b_1 + x_2 * b_2;
	
	// Algorithm for calculating equilibrium vapor pressure
	//
	double p_Pa = 10000;
	double phi_v_1 = 1;
	double phi_v_2 = 1;
	
	// Calculate coefficients of reduced cubic equation, which has the following
	// functional form:
	//
	//	0 = Z^3 - Z^2 + Z * (A - B - B^2) - A * B
	//
	//		with: Z = V * p / (R * T)
	//		----- A = a * p / (R * T)^2
	//			  B = b * p / (R * T)
	//
	double A = a * p_Pa / (pow(IDEAL_GAS_CONSTANT * T_K, 2));
	double B = b * p_Pa / (IDEAL_GAS_CONSTANT * T_K);
	
	// Rearrange reduced cubic equation to apply numerical solving algorithm:
	//
	//	f(Z) = Z^3 + y_2 * Z^2 + y_1 * Z + y_0
	//	f'(Z) = 3 * Z^2 + 2 * y_2 * Z + y_1
	//	f''(Z) = 6 * Z + 2 * y_2
	//
	//		with: y_0 = -A * B
	//		----- y_1 = (A - B - B^2)
	//			  y_2 = -1
	//
	double y_0 = -A * B;
	double y_1 = A - B - pow(B, 2);
	double y_2 = -1;
	
	// Solve rearranged reduced cubic equation using numeric algorithm presented
	// by Deiters and Macias-Salinas
	//
	// First solution:
	//
	double x_infl = -y_2 / 3;
	double d = pow(y_2, 2) - 3 * y_1;
	double x_lower = x_infl - 2 / 3 * sqrt(d);
	double x_upper = x_infl + 2 / 3 * sqrt(d);
	
	double Z_sol_1;
	double f_x_infl = pow(x_infl, 3) + y_2 * pow(x_infl, 2) + y_1 * x_infl + y_0;
	if (f_x_infl == 0) {
		Z_sol_1 = x_infl;
		
	} else if (d == 0) {
		Z_sol_1 = x_infl - cbrt(f_x_infl);
		
	} else {
		// Use Halley's method: Choose initial value
		//
		if (d > 0 && f_x_infl > 0) {
			Z_sol_1 = x_lower;
		} else if (d < 0) {
			Z_sol_1 = x_infl;
		} else {
			Z_sol_1 = x_upper;
		}
		
		// Use Halley's method: Algorithm
		//
		double Z_sol_1_new, df_x_infl, ddf_x_infl, delta;
		
		int counter_while = 0;
		const double tolerance = 1e-12;
		do {
			// Evaluate rearranged reduced cubic equation
			//			
			f_x_infl = pow(Z_sol_1, 3) + y_2 * pow(Z_sol_1, 2) + y_1 * Z_sol_1 + y_0;
			df_x_infl = 3 * pow(Z_sol_1, 2) + 2 * y_2 * Z_sol_1 + y_1;
			ddf_x_infl = 6 * Z_sol_1 + 2 * y_2;
			
			// Update first solution
			//
			Z_sol_1_new = Z_sol_1 - 2 * f_x_infl * df_x_infl /
				(2 * pow(fabs(df_x_infl), 2) - f_x_infl * ddf_x_infl);		
				
			counter_while += 1;
			delta = fabs(Z_sol_1_new-Z_sol_1);
			
			Z_sol_1 = Z_sol_1_new;			
			printf("\n Counter: %i | Z_sol_1: %f | delta: %f", counter_while, Z_sol_1, delta);
		} while (delta > tolerance && counter_while < 50);		
	}
	
	// Second and third solution:
	//
	double v_1 = Z_sol_1 + y_2;
	double v_2 = v_1 * Z_sol_1 + y_1;
	double Z_sol_2 = -v_1 / 2 + sqrt(pow(v_1 / 2, 2) - v_2);
	double Z_sol_3 = -v_1 / 2 - sqrt(pow(v_1 / 2, 2) - v_2);
	
	// Calculate molar volumes
	//
	double v_sol_1 = Z_sol_1 * IDEAL_GAS_CONSTANT * T_K / p_Pa;
	double v_sol_2 = Z_sol_2 * IDEAL_GAS_CONSTANT * T_K / p_Pa;
	double v_sol_3 = Z_sol_3 * IDEAL_GAS_CONSTANT * T_K / p_Pa;	
	double Z_l = min(Z_sol_1, min(Z_sol_2, Z_sol_3));
	double Z_v = max(Z_sol_1, max(Z_sol_2, Z_sol_3));
	double v_l = min(v_sol_1, min(v_sol_2, v_sol_3));
	double v_v = max(v_sol_1, max(v_sol_2, v_sol_3));
	
	// For liquid phase, calculate fugacity coefficients of both components
	//
	double Z_l_new = v_l / (v_l - b) - a / (IDEAL_GAS_CONSTANT * T_K * (v_l + b));
	
	double phi_l_m = exp(Z_l - 1 - log(Z_l * (1 - b / v_l)) - a / (b * IDEAL_GAS_CONSTANT * T_K) * log(1 + b / v_l));
	double phi_l_1 = exp(b_1 / b * (Z_l - 1) - log(Z_l * (1 - b / v_l)) + 1 / (b * IDEAL_GAS_CONSTANT * T_K) * (a * b_1 / b - 2 * sqrt(a * a_1)) * log(1 + b / v_l));
	double phi_l_2 = exp(b_2 / b * (Z_l - 1) - log(Z_l * (1 - b / v_l)) + 1 / (b * IDEAL_GAS_CONSTANT * T_K) * (a * b_2 / b - 2 * sqrt(a * a_2)) * log(1 + b / v_l));
	
	// Update pressure
	//
	double fugacity_1 = phi_l_1 * x_1 * p_Pa;
	double fugacity_2 = phi_l_2 * x_2 * p_Pa;
	
	double y_star_1 = fugacity_1 / (phi_v_1 * p_Pa);
	double y_star_2 = fugacity_2 / (phi_v_2 * p_Pa);
	
	p_Pa = (y_star_1 + y_star_2) * p_Pa;
	
	// Normalize molar fractions of gas phase
	//
	double y_comp_1 = y_star_1 / (y_star_1 + y_star_2);
	double y_comp_2 = y_star_2 / (y_star_1 + y_star_2);
	
	printf("\n\n m_1: %f", m_1);
	printf("\n m_2: %f", m_2);
	printf("\n alpha_1: %f", alpha_1);
	printf("\n alpha_2: %f", alpha_2);
	printf("\n a_1: %f", a_1);
	printf("\n a_2: %f", a_2);
	printf("\n a_12: %f", a_12);
	printf("\n a: %f", a);	
	printf("\n b_1: %f", b_1);
	printf("\n b_2: %f", b_2);
	printf("\n b: %f", b);	
	printf("\n A: %f", A);
	printf("\n B: %f", B);
	
	
	printf("\n\n y_0: %f", y_0);
	printf("\n y_1: %f", y_1);
	printf("\n y_2: %f", y_2);
	printf("\n x_infl: %f", x_infl);
	printf("\n d: %f", d);
	printf("\n x_lower: %f", x_lower);
	printf("\n x_upper: %f", x_upper);
	printf("\n v_1: %f", v_1);
	printf("\n v_2: %f", v_2);
	printf("\n Z_sol_1: %f", Z_sol_1);
	printf("\n Z_sol_2: %f", Z_sol_2);
	printf("\n Z_sol_3: %f", Z_sol_3);
	printf("\n fZ_sol_1: %2.32f", pow(Z_sol_1, 3) + y_2 * pow(Z_sol_1, 2) + y_1 * Z_sol_1 + y_0);
	printf("\n fZ_sol_2: %2.32f", pow(Z_sol_2, 3) + y_2 * pow(Z_sol_2, 2) + y_1 * Z_sol_2 + y_0);
	printf("\n fZ_sol_3: %2.32f", pow(Z_sol_3, 3) + y_2 * pow(Z_sol_3, 2) + y_1 * Z_sol_3 + y_0);
	
	printf("\n\n v_sol_1: %f", 1/v_sol_1);
	printf("\n v_sol_2: %f", 1/v_sol_2);
	printf("\n v_sol_3: %f", 1/v_sol_3);
	printf("\n Z_l_new: %f", Z_l_new);
	printf("\n Z_l: %f", Z_l);
	printf("\n Z_v: %f", Z_v);
	printf("\n v_l: %f", v_l);
	printf("\n v_v: %f", v_v);
	
	printf("\n\n phi_l_m: %f", phi_l_m);
	printf("\n phi_l_1: %f", phi_l_1);
	printf("\n phi_l_2: %f", phi_l_2);
	printf("\n fugacity_1: %f", fugacity_1);
	printf("\n fugacity_2: %f", fugacity_2);
	printf("\n y_star_1: %f", y_star_1);
	printf("\n y_star_2: %f", y_star_2);
	printf("\n p_Pa: %f", p_Pa);
	printf("\n y_comp_1: %f", y_comp_1);
	printf("\n y_comp_2: %f\n", y_comp_2);
	
	// For gas phase, calculate density and fugacity coefficients of both components
	//
	A = a * p_Pa / (pow(IDEAL_GAS_CONSTANT * T_K, 2));
	B = b * p_Pa / (IDEAL_GAS_CONSTANT * T_K);
	y_0 = -A * B;
	y_1 = A - B - pow(B, 2);
	y_2 = -1;
	x_infl = -y_2 / 3;
	d = pow(y_2, 2) - 3 * y_1;
	x_lower = x_infl - 2 / 3 * sqrt(d);
	x_upper = x_infl + 2 / 3 * sqrt(d);	
	f_x_infl = pow(x_infl, 3) + y_2 * pow(x_infl, 2) + y_1 * x_infl + y_0;
	if (f_x_infl == 0) {
		Z_sol_1 = x_infl;
		
	} else if (d == 0) {
		Z_sol_1 = x_infl - cbrt(f_x_infl);
		
	} else {
		// Use Halley's method: Choose initial value
		//
		if (d > 0 && f_x_infl > 0) {
			Z_sol_1 = x_lower;
		} else if (d < 0) {
			Z_sol_1 = x_infl;
		} else {
			Z_sol_1 = x_upper;
		}
		
		// Use Halley's method: Algorithm
		//		
		double Z_sol_1_new, df_x_infl, ddf_x_infl, delta;
		
		int counter_while = 0;
		const double tolerance = 1e-12;
		do {
			// Evaluate rearranged reduced cubic equation
			//			
			f_x_infl = pow(Z_sol_1, 3) + y_2 * pow(Z_sol_1, 2) + y_1 * Z_sol_1 + y_0;
			df_x_infl = 3 * pow(Z_sol_1, 2) + 2 * y_2 * Z_sol_1 + y_1;
			ddf_x_infl = 6 * Z_sol_1 + 2 * y_2;
			
			// Update first solution
			//
			Z_sol_1_new = Z_sol_1 - 2 * f_x_infl * df_x_infl /
				(2 * pow(fabs(df_x_infl), 2) - f_x_infl * ddf_x_infl);		
				
			counter_while += 1;
			delta = fabs(Z_sol_1_new-Z_sol_1);
			
			Z_sol_1 = Z_sol_1_new;			
			printf("\n Counter: %i | Z_sol_1: %f | delta: %f", counter_while, Z_sol_1, delta);
		} while (delta > tolerance && counter_while < 50);		
	}
	v_1 = Z_sol_1 + y_2;
	v_2 = v_1 * Z_sol_1 + y_1;
	Z_sol_2 = -v_1 / 2 + sqrt(pow(v_1 / 2, 2) - v_2);
	Z_sol_3 = -v_1 / 2 - sqrt(pow(v_1 / 2, 2) - v_2);
	
	// Calculate molar volumes
	//
	v_sol_1 = Z_sol_1 * IDEAL_GAS_CONSTANT * T_K / p_Pa;
	v_sol_2 = Z_sol_2 * IDEAL_GAS_CONSTANT * T_K / p_Pa;
	v_sol_3 = Z_sol_3 * IDEAL_GAS_CONSTANT * T_K / p_Pa;	
	Z_l = min(Z_sol_1, min(Z_sol_2, Z_sol_3));
	Z_v = max(Z_sol_1, max(Z_sol_2, Z_sol_3));
	v_l = min(v_sol_1, min(v_sol_2, v_sol_3));
	v_v = max(v_sol_1, max(v_sol_2, v_sol_3));
	
	// For liquid phase, calculate fugacity coefficients of both components
	//
	Z_l_new = v_l / (v_l - b) - a / (IDEAL_GAS_CONSTANT * T_K * (v_l + b));
	
	phi_l_m = exp(Z_l - 1 - log(Z_l * (1 - b / v_l)) - a / (b * IDEAL_GAS_CONSTANT * T_K) * log(1 + b / v_l));
	phi_l_1 = exp(b_1 / b * (Z_l - 1) - log(Z_l * (1 - b / v_l)) + 1 / (b * IDEAL_GAS_CONSTANT * T_K) * (a * b_1 / b - 2 * sqrt(a * a_1)) * log(1 + b / v_l));
	phi_l_2 = exp(b_2 / b * (Z_l - 1) - log(Z_l * (1 - b / v_l)) + 1 / (b * IDEAL_GAS_CONSTANT * T_K) * (a * b_2 / b - 2 * sqrt(a * a_2)) * log(1 + b / v_l));
	
	// Update pressure
	//
	fugacity_1 = phi_l_1 * x_1 * p_Pa;
	fugacity_2 = phi_l_2 * x_2 * p_Pa;
	
	y_star_1 = fugacity_1 / (phi_v_1 * p_Pa);
	y_star_2 = fugacity_2 / (phi_v_2 * p_Pa);
	
	p_Pa = (y_star_1 + y_star_2) * p_Pa;
	
	// Normalize molar fractions of gas phase
	//
	y_comp_1 = y_star_1 / (y_star_1 + y_star_2);
	y_comp_2 = y_star_2 / (y_star_1 + y_star_2);
	
	printf("\n\n A: %f", A);
	printf("\n B: %f", B);	
	
	printf("\n\n y_0: %f", y_0);
	printf("\n y_1: %f", y_1);
	printf("\n y_2: %f", y_2);
	printf("\n x_infl: %f", x_infl);
	printf("\n d: %f", d);
	printf("\n x_lower: %f", x_lower);
	printf("\n x_upper: %f", x_upper);
	printf("\n v_1: %f", v_1);
	printf("\n v_2: %f", v_2);
	printf("\n Z_sol_1: %f", Z_sol_1);
	printf("\n Z_sol_2: %f", Z_sol_2);
	printf("\n Z_sol_3: %f", Z_sol_3);
	printf("\n fZ_sol_1: %2.32f", pow(Z_sol_1, 3) + y_2 * pow(Z_sol_1, 2) + y_1 * Z_sol_1 + y_0);
	printf("\n fZ_sol_2: %2.32f", pow(Z_sol_2, 3) + y_2 * pow(Z_sol_2, 2) + y_1 * Z_sol_2 + y_0);
	printf("\n fZ_sol_3: %2.32f", pow(Z_sol_3, 3) + y_2 * pow(Z_sol_3, 2) + y_1 * Z_sol_3 + y_0);
	
	printf("\n\n v_sol_1: %f", 1/v_sol_1);
	printf("\n v_sol_2: %f", 1/v_sol_2);
	printf("\n v_sol_3: %f", 1/v_sol_3);
	printf("\n Z_l_new: %f", Z_l_new);
	printf("\n Z_l: %f", Z_l);
	printf("\n Z_v: %f", Z_v);
	printf("\n v_l: %f", v_l);
	printf("\n v_v: %f", v_v);
	
	printf("\n\n phi_l_m: %f", phi_l_m);
	printf("\n phi_l_1: %f", phi_l_1);
	printf("\n phi_l_2: %f", phi_l_2);
	printf("\n fugacity_1: %f", fugacity_1);
	printf("\n fugacity_2: %f", fugacity_2);
	printf("\n y_star_1: %f", y_star_1);
	printf("\n y_star_2: %f", y_star_2);
	printf("\n p_Pa: %f", p_Pa);
	printf("\n y_comp_1: %f", y_comp_1);
	printf("\n y_comp_2: %f\n", y_comp_2);
	
	// Check convergence
	//
	
	// Calculate pressure
	//
	*/
	T_K = T_K;
	x_molmol = x_molmol;
	isotherm_par = isotherm_par;
	
	return -1;
}
