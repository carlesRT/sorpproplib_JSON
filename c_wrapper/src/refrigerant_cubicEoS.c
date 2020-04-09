////////////////////////////
// refrigerant_cubicEoS.c //
////////////////////////////
#include <stdlib.h>
#include <math.h>
#include "refrigerant_cubicEoS.h"


/////////////////////////////
// Definition of constants //
/////////////////////////////
#ifndef IDEAL_GAS_CONSTANT
#define IDEAL_GAS_CONSTANT 8.314462618f
#endif
#ifndef PI
#define PI 3.1415926535897932384626433f
#endif


/////////////////////////////
// Definition of functions //
/////////////////////////////
/*
 * General form of Soave-Redlich-Kwong equation of state (SRKE):
 * -------------------------------------------------------------
 * 	p = R * T / (v - b) - a / (v * (v + b))
 *
 * Temperature-dependent coefficients:
 * -----------------------------------
 * 	a = 1 / (9 * (2^(1 / 3) - 1)) * R^2 * T_crit^2 / p_crit * alpha
 *	b = 0.08664 * R * T_crit / p_crit
 *
 *		with: alpha = (1 + kappa * (1 - sqrt(T / T_crit)))^2
 *		----- kappa = 0.480 + 1.574 * omega - 0.176 * omega^2
 *
 *
 * General form of Peng-Robinson equation of state (PRE):
 * ------------------------------------------------------
 * 	p = R * T / (v - b) - a / (v * (v + b) + b * (v - b))
 *
 * Temperature-dependent coefficients:
 * -----------------------------------
 * 	a = 0.45724 * R^2 * T_crit^2 / p_crit * alpha
 *	b = 0.077796 * R * T_crit / p_crit
 *
 *		with: alpha = (1 + kappa * (1 - sqrt(T / T_crit)))^2
 *		----- kappa = 0.37464 + 1.54226 * omega - 0.269992 * omega^2
 *
 *
 * General form of Peng-Robinson-Stryjek-Vera equation of state (PRSVE):
 * ---------------------------------------------------------------------
 * 	p = R * T / (v - b) - a / (v * (v + b) + b * (v - b))
 *
 * Temperature-dependent coefficients:
 * -----------------------------------
 * 	a = 0.457235 * R^2 * T_crit^2 / p_crit * alpha
 *	b = 0.077796 * R * T_crit / p_crit
 *
 *		with: alpha = (1 + kappa * (1 - sqrt(T / T_crit)))^2
 *		----- kappa = kappa_0 + kappa_1 * (1 + sqrt(T / T_crit)) *
 *			  	(0.7 - T / T_crit)
 *			  kappa_0 = 0.378893 + 1.4897153 * omega - 0.17131848 * omega^2 +
 *			  	0.0196554 * omega^3
 *
 *
 * Inputs required by user:
 * ------------------------
 *	p: Pressure in Pa
 *	T: Temperature in K
 *	v: Molar volume in m³/mol
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
 *
 *
 * One-parameter van der Waals mixing rule:
 * ----------------------------------------
 *
 *	a_m = z_1^2 * a_1 + 2 * z_1 * z_2 * a_12 + z_2^2 * a_2
 *	b_m = z_1 * b_1 + z_2 * b_2
 *
 *		with: a_12 = sqrt(a_1 * a_2) * (1 - delta_12)
 *		----- b_12 = (b_1 + b_2) / 2
 *
 *
 * One-parameter van der Waals mixing rule:
 * ----------------------------------------
 *
 *	a_m = z_1^2 * a_1 + 2 * z_1 * z_2 * a_12 + z_2^2 * a_2
 *	b_m = z_1^2 * b_1 + 2 * z_1 * z_2 * b_12 + z_2^2 * b_2
 *
 *		with: a_12 = sqrt(a_1 * a_2) * (1 - delta_12)
 *		----- b_12 = (b_1 + b_2) / 2 * (1 - m)
 *
 *
 * Modified van der Waals and Berthelot mixing rule:
 * -------------------------------------------------
 *
 *	a_m = z_1^2 * a_1 + 2 * z_1 * z_2 * a_12 + z_2^2 * a_2
 *	b_m = z_1^2 * b_1 + 2 * z_1 * z_2 * b_12 + z_2^2 * b_2
 *
 *		with: a_12 = sqrt(a_1 * a_2) * (1 + t / T) * (1 - delta_12)
 *		----- b_12 = (b_1 + b_2) / 2 * (1 - m)
 *			  delta_12 = l_12 * l_21 * (z_1 + z_2) / (l_21 * z_1 + l_12 * z_2)
 *
 * Order of coefficients in JSON-file:
 * -----------------------------------
 *	refrigerant_par[0]	-> delta_12	-> in -
 * 	refrigerant_par[1] 	-> m		-> in Pa
 *	refrigerant_par[2]	-> l_12		-> in K
 * 	refrigerant_par[3] 	-> l_21		-> in -
 * 	refrigerant_par[4] 	-> t		-> in -
 */


/*
 * refrigerant_cubic_solve:
 * ------------------------
 *
 * Solves cubic equation of state that is written with respect to
 * compressibility factor Z:
 *
 *	F(Z) = 0 = Z^3 + EOS_1 * Z^2 + EOS_2 * Z + EOS_3
 *
 * Parameters:
 * -----------
 *	double EOS_1:
 *		First parameter of generalized cubic equation of state.
 *	double EOS_2:
 *		Second parameter of generalized cubic equation of state.
 *	double EOS_3:
 *		Third parameter of generalized cubic equation of state.
 *
 * Returns:
 * --------
 *	*ret_Z:
 *		Returns liquid and vapor compressibility factor Z_l and Z_v,
 *		respectively.
 *
 * Remarks:
 * --------
 *	Function that calls this functions need to pre-allocate array 'ret_Z' to
 *	ensure correct memory handling.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
void refrigerant_cubic_solve(double *ret_Z, double EOS_1, double EOS_2,
	double EOS_3) {
    // Define additional parameters to determine real roots of cubic equation
    //
    double Q1 = EOS_1 * EOS_2 / 6 - EOS_3 / 2 - pow(EOS_1, 3) / 27;
    double P1 = pow(EOS_1, 2) / 9 - EOS_2 / 3;
    double D = pow(Q1, 2) - pow(P1, 3);

    // Check number of real roots
    //
    if (D >= 0) {
        // One real root
        //
        ret_Z[0] = cbrt(Q1 + sqrt(D)) + cbrt(Q1 - sqrt(D)) - EOS_1 / 3;
        ret_Z[1] = cbrt(Q1 + sqrt(D)) + cbrt(Q1 - sqrt(D)) - EOS_1 / 3;

    } else {
        // Three real roots
        //
        double t1 = pow(Q1, 2) / pow(P1, 3);
        double t2 = sqrt(1 - t1) / sqrt(t1) * Q1 / fabs(Q1);
        double Phi = atan(t2);

        double Z0 = 2 * sqrt(P1) * cos(Phi / 3) - EOS_1 / 3;
        double Z1 = 2 * sqrt(P1) * cos((Phi + 2 * PI) / 3) - EOS_1 / 3;
        double Z2 = 2 * sqrt(P1) * cos((Phi + 4 * PI) / 3) - EOS_1 / 3;

		// Find minimum and maximum among the three solutions
		//
		double Z_min = (Z0 <= Z1) ? Z0 : Z1;
		double Z_max = (Z0 >= Z1) ? Z0 : Z1;

        ret_Z[0] = (Z_min <= Z2) ? Z_min : Z2;
        ret_Z[1] = (Z_max >= Z2) ? Z_max : Z2;

	}
}


/*
 * refrigerant_cubic_SRKE_parameters:
 * ----------------------------------
 *
 * Calculates coefficients a and b of Soave-Redlich-Kwong equation of state.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Temperature in K.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for cubic equations of
 *		state.
 *
 * Returns:
 * --------
 *	*ret_par:
 *		Returns parameters a and b of SRKE.
 *
 * Remarks:
 * --------
 *	Function that calls this functions need pre-allocate array 'ret_par' to
 *	ensure correct memory handling.
 *
 * History:
 * --------
 *	04/06/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
void refrigerant_cubic_SRKE_parameters(double *ret_par, double T_K,
	double refrigerant_par[]) {
	// Calculate auxiliary parameters
	//
	double kappa = 0.480 + 1.574 * refrigerant_par[3] - 0.176 *
		pow(refrigerant_par[3], 2);
	double alpha = pow(1 + kappa * (1 - sqrt(T_K / refrigerant_par[2])), 2);

	// Calculate and return parameters a and b
	//
	ret_par[0] = (1 / (9 * (cbrt(2) - 1))) * pow(IDEAL_GAS_CONSTANT *
		refrigerant_par[2], 2) / refrigerant_par[1] * alpha;
	ret_par[1] = 0.08664 * IDEAL_GAS_CONSTANT * refrigerant_par[2] /
		refrigerant_par[1];
}


/*
 * refrigerant_cubic_SRKE_custom_parameters:
 * -----------------------------------------
 *
 * Calculates coefficients a and b of Soave-Redlich-Kwong equation of state for
 * custom equation for alpha.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Temperature in K.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for cubic equations of
 *		state.
 *
 * Returns:
 * --------
 *	*ret_par:
 *		Returns parameters a and b of SRKE.
 *
 * Remarks:
 * --------
 *	Function that calls this functions need pre-allocate array 'ret_par' to
 *	ensure correct memory handling.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
void refrigerant_cubic_SRKE_custom_parameters(double *ret_par, double T_K,
	double refrigerant_par[]) {
	// Calculate auxiliary parameters
	//
	double T_red = refrigerant_par[2] / T_K - T_K / refrigerant_par[2];
	double alpha = refrigerant_par[5] + refrigerant_par[6] * T_red +
		refrigerant_par[7] * pow(T_red, 2) + refrigerant_par[8] * pow(T_red, 3);

	// Calculate and return parameters a and b
	//
	ret_par[0] = (1 / (9 * (cbrt(2) - 1))) * pow(IDEAL_GAS_CONSTANT *
		refrigerant_par[2], 2) / refrigerant_par[1] * alpha;
	ret_par[1] = 0.08664 * IDEAL_GAS_CONSTANT * refrigerant_par[2] /
		refrigerant_par[1];
}


/*
 * refrigerant_cubic_SRKE_gen_parameters:
 * --------------------------------------
 *
 * Calculates generalized parameters A and B of Soave-Redlich-Kwong equation of
 * state. The parameters A and B are required to generalize the equation of
 * state as follows:
 *
 *	F(Z) = Z^3 - Z^2 + (A - B - B^2) * Z -(A * B)
 *
 *		with: A = a * p / (R * T)^2
 *		----- B = b * p / (R * T)
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Pressure in Pa.
 *	double T_K:
 *		Temperature in K.
 *	double a:
 *		Coefficient a of Soave-Redlich-Kwong equation of state.
 *	double b:
 *		Coefficient b of Soave-Redlich-Kwong equation of state.
 *
 * Returns:
 * --------
 *	*ret_par:
 *		Returns parameters A, B, EOS_1, EOS_2, and EOS_3 of SRKE.
 *
 * Remarks:
 * --------
 *	Function that calls this functions need pre-allocate array 'ret_par' to
 *	ensure correct memory handling.
 *
 * History:
 * --------
 *	04/06/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
void refrigerant_cubic_SRKE_gen_parameters(double *ret_par, double p_Pa,
	double T_K, double a, double b) {
	// Calculate auxiliary parameters
	//
    double A = a * p_Pa / pow(IDEAL_GAS_CONSTANT * T_K, 2);
    double B = b * p_Pa / (IDEAL_GAS_CONSTANT * T_K);

	// Calculate and return parameters a and b
	//
	ret_par[0] = A;
	ret_par[1] = B;
	ret_par[2] = -1;
	ret_par[3] = A - B - pow(B, 2);
	ret_par[4] = -(A * B);
}


/*
 * refrigerant_cubic_SRKE_fugacity_coefficient_pure:
 * -------------------------------------------------
 *
 * Calculates fugacity coefficient of Soave-Redlich-Kwong equation of state.
 *
 * Parameters:
 * -----------
 *	double Z:
 *		Compressibility factor.
 *	double A:
 *		Generalized parameter A of Soave-Redlich-Kwong equation of state.
 *	double B:
 *		Generalized parameter B of Soave-Redlich-Kwong equation of state.
 *
 * Returns:
 * --------
 *	phi:
 *		Fugacity coefficient.
 *	*ret_error:
 *		Error indicator that equals -1 if equation is not solvable.
 *
 * History:
 * --------
 *	04/06/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_cubic_SRKE_fugacity_coefficient_pure(int *ret_error,
	double Z, double A, double B) {
	// Avoid negative argument of logarithm or division by nearly zero
	//
    *ret_error = (Z - B <= 0 || B <= 1E-50) ? -1 : 0;

	// Calculate fugacity coefficient
	//
	if (*ret_error < 0) {
		return 1;

	} else {
		return exp(Z - 1 - log(Z - B) - A / B * log(1 + B / Z));

	}
}


/*
 * refrigerant_cubic_SRKE_fugacity_coefficient_1pvdw:
 * --------------------------------------------------
 *
 * Calculates fugacity coefficient of Soave-Redlich-Kwong equation of state for
 * mixtures when using one-parameter van der Waals mixing rule.
 *
 * Parameters:
 * -----------
 *	int i:
 *		Integer indicating first or second component of mixture.
 *	double z_i:
 *		Mole fraction of component i in liquid or vapor phase.
 *	double Z:
 *		Compressibility factor.
 *	double A_m:
 *		Generalized parameter A_m of Soave-Redlich-Kwong equation of state.
 *	double B_m:
 *		Generalized parameter B_m of Soave-Redlich-Kwong equation of state.
 *	double a_i:
 *		Pure component parameter a_i of Soave-Redlich-Kwong equation of state.
 *	double a_m:
 *		Mixture parameter a_m of Soave-Redlich-Kwong equation of state.
 *	double b_i:
 *		Pure component parameter b_i of Soave-Redlich-Kwong equation of state.
 *	double b_m:
 *		Mixture parameter b_m of Soave-Redlich-Kwong equation of state.
 *	double a_m_12:
 *		Mixture parameter a_m_12 of Soave-Redlich-Kwong equation of state.
 *	double b_m_12:
 *		Mixture parameter a_m_12 of Soave-Redlich-Kwong equation of state.
 *
 * Returns:
 * --------
 *	phi:
 *		Fugacity coefficient of mixture.
 *	*error:
 *		Error indicator that equals -1 if equation is not solvable.
 *
 * History:
 * --------
 *	04/06/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_cubic_SRKE_fugacity_coefficient_1pvdw(int *ret_error,
	int i, double z_i, double Z, double A_m, double B_m, double b_i,
	double b_m, double a_i, double a_m, double a_m_12, double b_m_12) {
	// Avoid negative argument of logarithm or division by nearly zero
	//
    *ret_error = (Z - B_m <= 0 || B_m <= 1E-50) ? -1 : 0;

	// Calculate fugacity coefficient
	//
	if (*ret_error < 0) {
		return 1;

	} else {
		// Calculate auxiliary variables
		//
		double z_1 = (i == 1) ? z_i : 1 - z_i;
		double z_2 = (i == 2) ? z_i : 1 - z_i;
		double sum_a = (i == 1) ? z_1 * a_i + z_2 * a_m_12 :
			z_1 * a_m_12 + z_2 * a_i;
		double sum_b = (i == 2) ? z_1 * b_m_12 + z_2 * b_i :
			z_1 * b_i + z_2 * b_m_12;

		return exp((2 * sum_b / b_m - 1) * (Z - 1) - log(Z - B_m) - A_m / B_m *
			(2 * sum_a / a_m + (1 - 2 * sum_b / b_m)) * log(1 + B_m / Z));

	}
}


/*
 * refrigerant_cubic_SRKE_fugacity_coefficient_2pvdw:
 * --------------------------------------------------
 *
 * Calculates fugacity coefficient of Soave-Redlich-Kwong equation of state for
 * mixtures when using two-parameter van der Waals mixing rule.
 *
 * Parameters:
 * -----------
 *	int i:
 *		Integer indicating first or second component of mixture.
 *	double z_i:
 *		Mole fraction of component i in liquid or vapor phase.
 *	double Z:
 *		Compressibility factor.
 *	double A_m:
 *		Generalized parameter A_m of Soave-Redlich-Kwong equation of state.
 *	double B_m:
 *		Generalized parameter B_m of Soave-Redlich-Kwong equation of state.
 *	double a_i:
 *		Pure component parameter a_i of Soave-Redlich-Kwong equation of state.
 *	double a_m:
 *		Mixture parameter a_m of Soave-Redlich-Kwong equation of state.
 *	double b_i:
 *		Pure component parameter b_i of Soave-Redlich-Kwong equation of state.
 *	double b_m:
 *		Mixture parameter b_m of Soave-Redlich-Kwong equation of state.
 *	double a_m_12:
 *		Mixture parameter a_m_12 of Soave-Redlich-Kwong equation of state.
 *	double b_m_12:
 *		Mixture parameter a_m_12 of Soave-Redlich-Kwong equation of state.
 *
 * Returns:
 * --------
 *	phi:
 *		Fugacity coefficient of mixture.
 *	*error:
 *		Error indicator that equals -1 if equation is not solvable.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_cubic_SRKE_fugacity_coefficient_2pvdw(int *ret_error,
	int i, double z_i, double Z, double A_m, double B_m, double b_i,
	double b_m, double a_i, double a_m, double a_m_12, double b_m_12) {
	// Execute function for one-parameter van der Waals mixing rule
	//
    return refrigerant_cubic_SRKE_fugacity_coefficient_1pvdw(ret_error, i, z_i,
		Z, A_m, B_m, b_i, b_m, a_i, a_m, a_m_12, b_m_12);
}


/*
 * refrigerant_cubic_SRKE_fugacity_coefficient_vdwb:
 * --------------------------------------------------
 *
 * Calculates fugacity coefficient of Soave-Redlich-Kwong equation of state for
 * mixtures when using modified mixing rule from van der Waals and Berthelot.
 *
 * Parameters:
 * -----------
 *	int i:
 *		Integer indicating first or second component of mixture.
 *	double z_i:
 *		Mole fraction of component i in liquid or vapor phase.
 *	double Z:
 *		Compressibility factor.
 *	double A_m:
 *		Generalized parameter A_m of Soave-Redlich-Kwong equation of state.
 *	double B_m:
 *		Generalized parameter B_m of Soave-Redlich-Kwong equation of state.
 *	double a_i:
 *		Pure component parameter a_i of Soave-Redlich-Kwong equation of state.
 *	double a_m:
 *		Mixture parameter a_m of Soave-Redlich-Kwong equation of state.
 *	double b_i:
 *		Pure component parameter b_i of Soave-Redlich-Kwong equation of state.
 *	double b_m:
 *		Mixture parameter b_m of Soave-Redlich-Kwong equation of state.
 *	double a_m_12:
 *		Mixture parameter a_m_12 of Soave-Redlich-Kwong equation of state.
 *	double b_m_12:
 *		Mixture parameter a_m_12 of Soave-Redlich-Kwong equation of state.
 *
 * Returns:
 * --------
 *	phi:
 *		Fugacity coefficient of mixture.
 *	*error:
 *		Error indicator that equals -1 if equation is not solvable.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_cubic_SRKE_fugacity_coefficient_vdwb(int *ret_error,
	int i, double z_i, double Z, double A_m, double B_m, double b_i,
	double b_m, double a_i, double a_m, double a_m_12, double b_m_12) {
	// Execute function for one-parameter van der Waals mixing rule
	//
    return refrigerant_cubic_SRKE_fugacity_coefficient_1pvdw(ret_error, i, z_i,
		Z, A_m, B_m, b_i, b_m, a_i, a_m, a_m_12, b_m_12);
}


/*
 * refrigerant_cubic_PRE_parameters:
 * ---------------------------------
 *
 * Calculates coefficients a and b of Peng-Robinson equation of state.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Temperature in K.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for cubic equations of
 *		state.
 *
 * Returns:
 * --------
 *	*ret_par:
 *		Returns parameters a and b of PRE.
 *
 * Remarks:
 * --------
 *	Function that calls this functions need pre-allocate array 'ret_par' to
 *	ensure correct memory handling.
 *
 * History:
 * --------
 *	04/06/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
void refrigerant_cubic_PRE_parameters(double *ret_par, double T_K,
	double refrigerant_par[]) {
	// Calculate auxiliary parameters
	//
	double kappa = 0.37464 + 1.54226 * refrigerant_par[3] - 0.269992 *
		pow(refrigerant_par[3], 2);
	double alpha = pow(1 + kappa * (1 - sqrt(T_K / refrigerant_par[2])), 2);

	// Calculate and return parameters a and b
	//
	ret_par[0] = 0.45724 * pow(IDEAL_GAS_CONSTANT * refrigerant_par[2], 2) /
		refrigerant_par[1] * alpha;
	ret_par[1] = 0.077796 * IDEAL_GAS_CONSTANT * refrigerant_par[2] /
		refrigerant_par[1];
}


/*
 * refrigerant_cubic_PRE_custom_parameters:
 * ----------------------------------------
 *
 * Calculates coefficients a and b of Peng-Robinson equation of state for
 * custom equation for alpha.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Temperature in K.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for cubic equations of
 *		state.
 *
 * Returns:
 * --------
 *	*ret_par:
 *		Returns parameters a and b of PRE.
 *
 * Remarks:
 * --------
 *	Function that calls this functions need pre-allocate array 'ret_par' to
 *	ensure correct memory handling.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
void refrigerant_cubic_PRE_custom_parameters(double *ret_par, double T_K,
	double refrigerant_par[]) {
	// Calculate auxiliary parameters
	//
	double T_red = refrigerant_par[2] / T_K - T_K / refrigerant_par[2];
	double alpha = refrigerant_par[5] + refrigerant_par[6] * T_red +
		refrigerant_par[7] * pow(T_red, 2) + refrigerant_par[8] * pow(T_red, 3);

	// Calculate and return parameters a and b
	//
	ret_par[0] = 0.45724 * pow(IDEAL_GAS_CONSTANT * refrigerant_par[2], 2) /
		refrigerant_par[1] * alpha;
	ret_par[1] = 0.077796 * IDEAL_GAS_CONSTANT * refrigerant_par[2] /
		refrigerant_par[1];
}


/*
 * refrigerant_cubic_PRE_gen_parameters:
 * -------------------------------------
 *
 * Calculates generalized parameters A and B of Peng-Robinson equation of state.
 * The parameters A and B are required to generalize the equation of state as
 * follows:
 *
 *	F(Z) = Z^3 -(1 - B) * Z^2 + (A - 3 * B^2 - 2 * B) * Z -(A * B - B^2 - B^3)
 *
 *		with: A = a * p / (R * T)^2
 *		----- B = b * p / (R * T)
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Pressure in Pa.
 *	double T_K:
 *		Temperature in K.
 *	double a:
 *		Coefficient a of Peng-Robinson equation of state.
 *	double b:
 *		Coefficient b of Peng-Robinson equation of state.
 *
 * Returns:
 * --------
 *	*ret_par:
 *		Returns parameters A, B, EOS_1, EOS_2, and EOS_3 of PRE.
 *
 * Remarks:
 * --------
 *	Function that calls this functions need pre-allocate array 'ret_par' to
 *	ensure correct memory handling.
 *
 * History:
 * --------
 *	04/06/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
void refrigerant_cubic_PRE_gen_parameters(double *ret_par, double p_Pa,
	double T_K, double a, double b) {
	// Calculate auxiliary parameters
	//
    double A = a * p_Pa / pow(IDEAL_GAS_CONSTANT * T_K, 2);
    double B = b * p_Pa / (IDEAL_GAS_CONSTANT * T_K);

	// Calculate and return parameters a and b
	//
	ret_par[0] = A;
	ret_par[1] = B;
	ret_par[2] = -(1 - B);
	ret_par[3] = A - 3 * pow(B, 2) - 2 * B;
	ret_par[4] = -(A * B - pow(B, 2) - pow(B, 3));
}


/*
 * refrigerant_cubic_PRE_fugacity_coefficient_pure:
 * ------------------------------------------------
 *
 * Calculates fugacity coefficient of Peng-Robinson equation of state.
 *
 * Parameters:
 * -----------
 *	double Z:
 *		Compressibility factor.
 *	double A:
 *		Generalized parameter A of Peng-Robinson equation of state.
 *	double B:
 *		Generalized parameter B of Peng-Robinson equation of state.
 *
 * Returns:
 * --------
 *	phi:
 *		Fugacity coefficient.
 *	*ret_error:
 *		Error indicator that equals -1 if equation is not solvable.
 *
 * History:
 * --------
 *	04/06/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_cubic_PRE_fugacity_coefficient_pure(int *ret_error,
	double Z, double A, double B) {
	// Avoid negative argument of logarithm or division by nearly zero
	//
    *ret_error = (Z - B <= 0 || B <= 1E-50) ? -1 : 0;

	// Calculate fugacity coefficient
	//
	if (*ret_error < 0) {
		return 1;

	} else {
		return exp(Z - 1 - log(Z - B) - A / (2 * sqrt(2) * B) *
			log((Z + (1 + sqrt(2)) * B) / (Z + (1 - sqrt(2)) * B)));

	}
}


/*
 * refrigerant_cubic_PRE_fugacity_coefficient_1pvdw:
 * -------------------------------------------------
 *
 * Calculates fugacity coefficient of Peng-Robinson equation of state for
 * mixtures when using one-parameter van der Waals mixing rule.
 *
 * Parameters:
 * -----------
 *	int i:
 *		Integer indicating first or second component of mixture.
 *	double z_i:
 *		Mole fraction of component i in liquid or vapor phase.
 *	double Z:
 *		Compressibility factor.
 *	double A_m:
 *		Generalized parameter A_m of Peng-Robinson equation of state.
 *	double B_m:
 *		Generalized parameter B_m of Peng-Robinson equation of state.
 *	double a_i:
 *		Pure component parameter a_i of Peng-Robinson equation of state.
 *	double a_m:
 *		Mixture parameter a_m of Peng-Robinson equation of state.
 *	double b_i:
 *		Pure component parameter b_i of Peng-Robinson equation of state.
 *	double b_m:
 *		Mixture parameter b_m of Peng-Robinson equation of state.
 *	double a_m_12:
 *		Mixture parameter a_m_12 of Peng-Robinson equation of state.
 *	double b_m_12:
 *		Mixture parameter b_m_12 of Peng-Robinson equation of state.
 *
 * Returns:
 * --------
 *	phi:
 *		Fugacity coefficient of mixture.
 *	*error:
 *		Error indicator that equals -1 if equation is not solvable.
 *
 * History:
 * --------
 *	04/06/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_cubic_PRE_fugacity_coefficient_1pvdw(int *ret_error,
	int i, double z_i, double Z, double A_m, double B_m, double b_i,
	double b_m, double a_i, double a_m, double a_m_12, double b_m_12) {
	// Avoid negative argument of logarithm or division by nearly zero
	//
    *ret_error = (Z - B_m <= 0 || B_m <= 1E-50) ? -1 : 0;

	// Calculate fugacity coefficient
	//
	if (*ret_error < 0) {
		return 1;

	} else {
		// Calculate auxiliary variables
		//
		double z_1 = (i == 1) ? z_i : 1 - z_i;
		double z_2 = (i == 2) ? z_i : 1 - z_i;
		double sum_a = (i == 1) ? z_1 * a_i + z_2 * a_m_12 :
			z_1 * a_m_12 + z_2 * a_i;
		double sum_b = (i == 1) ? z_1 * b_i + z_2 * b_m_12 :
			z_1 * b_m_12 + z_2 * b_i;

		return exp((2 * sum_b - b_m) / b_m * (Z - 1) - log(Z - B_m) -
			A_m / (2 * sqrt(2) * B_m) * (2 * sum_a / a_m - (2 * sum_b - b_m) /
			b_m) * log((Z + (1 + sqrt(2)) * B_m) / (Z + (1 - sqrt(2)) * B_m)));

	}
}


/*
 * refrigerant_cubic_PRE_fugacity_coefficient_2pvdw:
 * -------------------------------------------------
 *
 * Calculates fugacity coefficient of Peng-Robinson equation of state for
 * mixtures when using two-parameter van der Waals mixing rule.
 *
 * Parameters:
 * -----------
 *	int i:
 *		Integer indicating first or second component of mixture.
 *	double z_i:
 *		Mole fraction of component i in liquid or vapor phase.
 *	double Z:
 *		Compressibility factor.
 *	double A_m:
 *		Generalized parameter A_m of Peng-Robinson equation of state.
 *	double B_m:
 *		Generalized parameter B_m of Peng-Robinson equation of state.
 *	double a_i:
 *		Pure component parameter a_i of Peng-Robinson equation of state.
 *	double a_m:
 *		Mixture parameter a_m of Peng-Robinson equation of state.
 *	double b_i:
 *		Pure component parameter b_i of Peng-Robinson equation of state.
 *	double b_m:
 *		Mixture parameter b_m of Peng-Robinson equation of state.
 *	double a_m_12:
 *		Mixture parameter a_m_12 of Peng-Robinson equation of state.
 *	double b_m_12:
 *		Mixture parameter b_m_12 of Peng-Robinson equation of state.
 *
 * Returns:
 * --------
 *	phi:
 *		Fugacity coefficient of mixture.
 *	*error:
 *		Error indicator that equals -1 if equation is not solvable.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_cubic_PRE_fugacity_coefficient_2pvdw(int *ret_error,
	int i, double z_i, double Z, double A_m, double B_m, double b_i,
	double b_m, double a_i, double a_m, double a_m_12, double b_m_12) {
	// Execute function for one-parameter van der Waals mixing rule
	//
    return refrigerant_cubic_PRE_fugacity_coefficient_1pvdw(ret_error, i, z_i,
		Z, A_m, B_m, b_i, b_m, a_i, a_m, a_m_12, b_m_12);
}


/*
 * refrigerant_cubic_PRE_fugacity_coefficient_vdwb:
 * ------------------------------------------------
 *
 * Calculates fugacity coefficient of Peng-Robinson equation of state for
 * mixtures when using modified mixing rule from van der Waals and Berthelot.
 *
 * Parameters:
 * -----------
 *	int i:
 *		Integer indicating first or second component of mixture.
 *	double z_i:
 *		Mole fraction of component i in liquid or vapor phase.
 *	double Z:
 *		Compressibility factor.
 *	double A_m:
 *		Generalized parameter A_m of Peng-Robinson equation of state.
 *	double B_m:
 *		Generalized parameter B_m of Peng-Robinson equation of state.
 *	double a_i:
 *		Pure component parameter a_i of Peng-Robinson equation of state.
 *	double a_m:
 *		Mixture parameter a_m of Peng-Robinson equation of state.
 *	double b_i:
 *		Pure component parameter b_i of Peng-Robinson equation of state.
 *	double b_m:
 *		Mixture parameter b_m of Peng-Robinson equation of state.
 *	double a_m_12:
 *		Mixture parameter a_m_12 of Peng-Robinson equation of state.
 *	double b_m_12:
 *		Mixture parameter b_m_12 of Peng-Robinson equation of state.
 *
 * Returns:
 * --------
 *	phi:
 *		Fugacity coefficient of mixture.
 *	*error:
 *		Error indicator that equals -1 if equation is not solvable.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_cubic_PRE_fugacity_coefficient_vdwb(int *ret_error,
	int i, double z_i, double Z, double A_m, double B_m, double b_i,
	double b_m, double a_i, double a_m, double a_m_12, double b_m_12) {
	// Execute function for one-parameter van der Waals mixing rule
	//
    return refrigerant_cubic_PRE_fugacity_coefficient_1pvdw(ret_error, i, z_i,
		Z, A_m, B_m, b_i, b_m, a_i, a_m, a_m_12, b_m_12);
}


/*
 * refrigerant_cubic_PRSVE_parameters:
 * -----------------------------------
 *
 * Calculates coefficients a and b of Peng-Robinson-Stryjek-Vera equation of
 * state.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Temperature in K.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for cubic equations of
 *		state.
 *
 * Returns:
 * --------
 *	*ret_par:
 *		Returns parameters a and b of PRSVE.
 *
 * Remarks:
 * --------
 *	Function that calls this functions need pre-allocate array 'ret_par' to
 *	ensure correct memory handling.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
void refrigerant_cubic_PRSVE_parameters(double *ret_par, double T_K,
	double refrigerant_par[]) {
	// Calculate auxiliary parameters
	//
	double kappa_0 = 0.378893 + 1.4897153 * refrigerant_par[3] - 0.17131848 *
		pow(refrigerant_par[3], 2) + 0.0196554 * pow(refrigerant_par[3], 3);
	double kappa = kappa_0 + refrigerant_par[4] *
		(1 + sqrt(T_K / refrigerant_par[2])) * (0.7 - T_K / refrigerant_par[2]);
	double alpha = pow(1 + kappa * (1 - sqrt(T_K / refrigerant_par[2])), 2);

	// Calculate and return parameters a and b
	//
	ret_par[0] = 0.457235 * pow(IDEAL_GAS_CONSTANT * refrigerant_par[2], 2) /
		refrigerant_par[1] * alpha;
	ret_par[1] = 0.077796 * IDEAL_GAS_CONSTANT * refrigerant_par[2] /
		refrigerant_par[1];
}


/*
 * refrigerant_cubic_PRSVE_gen_parameters:
 * ---------------------------------------
 *
 * Calculates generalized parameters A and B of Peng-Robinson-Stryjek-Vera
 * equation of state. The parameters A and B are required to generalize the
 * equation of state as follows:
 *
 *	F(Z) = Z^3 -(1 - B) * Z^2 + (A - 3 * B^2 - 2 * B) * Z -(A * B - B^2 - B^3)
 *
 *		with: A = a * p / (R * T)^2
 *		----- B = b * p / (R * T)
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Pressure in Pa.
 *	double T_K:
 *		Temperature in K.
 *	double a:
 *		Coefficient a of Peng-Robinson-Stryjek-Vera equation of state.
 *	double b:
 *		Coefficient b of Peng-Robinson-Stryjek-Vera equation of state.
 *
 * Returns:
 * --------
 *	*ret_par:
 *		Returns parameters A, B, EOS_1, EOS_2, and EOS_3 of PRSVE.
 *
 * Remarks:
 * --------
 *	Function that calls this functions need pre-allocate array 'ret_par' to
 *	ensure correct memory handling.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
void refrigerant_cubic_PRSVE_gen_parameters(double *ret_par, double p_Pa,
	double T_K, double a, double b) {
	// Execute corresponding function of Peng-Robinson equation of state
	//
    refrigerant_cubic_PRE_gen_parameters(ret_par, p_Pa, T_K, a, b);
}


/*
 * refrigerant_cubic_PRSVE_fugacity_coefficient_pure:
 * --------------------------------------------------
 *
 * Calculates fugacity coefficient of Peng-Robinson-Stryjek-Vera equation of
 * state.
 *
 * Parameters:
 * -----------
 *	double Z:
 *		Compressibility factor.
 *	double A:
 *		Generalized parameter A of Peng-Robinson-Stryjek-Vera equation of state.
 *	double B:
 *		Generalized parameter B of Peng-Robinson-Stryjek-Vera equation of state.
 *
 * Returns:
 * --------
 *	phi:
 *		Fugacity coefficient.
 *	*ret_error:
 *		Error indicator that equals -1 if equation is not solvable.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_cubic_PRSVE_fugacity_coefficient_pure(int *ret_error,
	double Z, double A, double B) {
	// Execute corresponding function of Peng-Robinson equation of state
	//
    return refrigerant_cubic_PRE_fugacity_coefficient_pure(ret_error, Z, A, B);
}


/*
 * refrigerant_cubic_PRSVE_fugacity_coefficient_1pvdw:
 * ---------------------------------------------------
 *
 * Calculates fugacity coefficient of Peng-Robinson-Stryjek-Vera equation of
 * state for mixtures when using one-parameter van der Waals mixing rule.
 *
 * Parameters:
 * -----------
 *	int i:
 *		Integer indicating first or second component of mixture.
 *	double z_i:
 *		Mole fraction of component i in liquid or vapor phase.
 *	double Z:
 *		Compressibility factor.
 *	double A_m:
 *		Generalized parameter A_m of Peng-Robinson-Stryjek-Vera equation of
 *		state.
 *	double B_m:
 *		Generalized parameter B_m of Peng-Robinson-Stryjek-Vera equation of
 *		state.
 *	double a_i:
 *		Pure component parameter a_i of Peng-Robinson-Stryjek-Vera equation of
 *		state.
 *	double a_m:
 *		Mixture parameter a_m of Peng-Robinson-Stryjek-Vera equation of state.
 *	double b_i:
 *		Pure component parameter b_i of Peng-Robinson-Stryjek-Vera equation of
 *		state.
 *	double b_m:
 *		Mixture parameter b_m of Peng-Robinson-Stryjek-Vera equation of state.
 *	double a_m_12:
 *		Mixture parameter a_m_12 of Peng-Robinson-Stryjek-Vera equation of
 *		state.
 *	double b_m_12:
 *		Mixture parameter b_m_12 of Peng-Robinson-Stryjek-Vera equation of
 *		state.
 *
 * Returns:
 * --------
 *	phi:
 *		Fugacity coefficient of mixture.
 *	*error:
 *		Error indicator that equals -1 if equation is not solvable.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_cubic_PRSVE_fugacity_coefficient_1pvdw(int *ret_error,
	int i, double z_i, double Z, double A_m, double B_m, double b_i,
	double b_m, double a_i, double a_m, double a_m_12, double b_m_12) {
	// Execute corresponding function of Peng-Robinson equation of state
	//
    return refrigerant_cubic_PRE_fugacity_coefficient_1pvdw(ret_error, i, z_i,
		Z, A_m, B_m, b_i, b_m, a_i, a_m, a_m_12, b_m_12);
}


/*
 * refrigerant_cubic_PRSVE_fugacity_coefficient_2pvdw:
 * ---------------------------------------------------
 *
 * Calculates fugacity coefficient of Peng-Robinson-Stryjek-Vera equation of
 * state for mixtures when using two-parameter van der Waals mixing rule.
 *
 * Parameters:
 * -----------
 *	int i:
 *		Integer indicating first or second component of mixture.
 *	double z_i:
 *		Mole fraction of component i in liquid or vapor phase.
 *	double Z:
 *		Compressibility factor.
 *	double A_m:
 *		Generalized parameter A_m of Peng-Robinson-Stryjek-Vera equation of
 *		state.
 *	double B_m:
 *		Generalized parameter B_m of Peng-Robinson-Stryjek-Vera equation of
 *		state.
 *	double a_i:
 *		Pure component parameter a_i of Peng-Robinson-Stryjek-Vera equation of
 *		state.
 *	double a_m:
 *		Mixture parameter a_m of Peng-Robinson-Stryjek-Vera equation of state.
 *	double b_i:
 *		Pure component parameter b_i of Peng-Robinson-Stryjek-Vera equation of
 *		state.
 *	double b_m:
 *		Mixture parameter b_m of Peng-Robinson-Stryjek-Vera equation of state.
 *	double a_m_12:
 *		Mixture parameter a_m_12 of Peng-Robinson-Stryjek-Vera equation of
 *		state.
 *	double b_m_12:
 *		Mixture parameter b_m_12 of Peng-Robinson-Stryjek-Vera equation of
 *		state.
 *
 * Returns:
 * --------
 *	phi:
 *		Fugacity coefficient of mixture.
 *	*error:
 *		Error indicator that equals -1 if equation is not solvable.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_cubic_PRSVE_fugacity_coefficient_2pvdw(int *ret_error,
	int i, double z_i, double Z, double A_m, double B_m, double b_i,
	double b_m, double a_i, double a_m, double a_m_12, double b_m_12) {
	// Execute corresponding function of Peng-Robinson equation of state
	//
    return refrigerant_cubic_PRE_fugacity_coefficient_2pvdw(ret_error, i, z_i,
		Z, A_m, B_m, b_i, b_m, a_i, a_m, a_m_12, b_m_12);
}


/*
 * refrigerant_cubic_PRSVE_fugacity_coefficient_vdwb:
 * --------------------------------------------------
 *
 * Calculates fugacity coefficient of Peng-Robinson-Stryjek-Vera equation of
 * state for mixtures when using modified mixing rule from van der Waals and
 * Berthelot.
 *
 * Parameters:
 * -----------
 *	int i:
 *		Integer indicating first or second component of mixture.
 *	double z_i:
 *		Mole fraction of component i in liquid or vapor phase.
 *	double Z:
 *		Compressibility factor.
 *	double A_m:
 *		Generalized parameter A_m of Peng-Robinson-Stryjek-Vera equation of
 *		state.
 *	double B_m:
 *		Generalized parameter B_m of Peng-Robinson-Stryjek-Vera equation of
 *		state.
 *	double a_i:
 *		Pure component parameter a_i of Peng-Robinson-Stryjek-Vera equation of
 *		state.
 *	double a_m:
 *		Mixture parameter a_m of Peng-Robinson-Stryjek-Vera equation of state.
 *	double b_i:
 *		Pure component parameter b_i of Peng-Robinson-Stryjek-Vera equation of
 *		state.
 *	double b_m:
 *		Mixture parameter b_m of Peng-Robinson-Stryjek-Vera equation of state.
 *	double a_m_12:
 *		Mixture parameter a_m_12 of Peng-Robinson-Stryjek-Vera equation of
 *		state.
 *	double b_m_12:
 *		Mixture parameter b_m_12 of Peng-Robinson-Stryjek-Vera equation of
 *		state.
 *
 * Returns:
 * --------
 *	phi:
 *		Fugacity coefficient of mixture.
 *	*error:
 *		Error indicator that equals -1 if equation is not solvable.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_cubic_PRSVE_fugacity_coefficient_vdwb(int *ret_error,
	int i, double z_i, double Z, double A_m, double B_m, double b_i,
	double b_m, double a_i, double a_m, double a_m_12, double b_m_12) {
	// Execute corresponding function of Peng-Robinson equation of state
	//
    return refrigerant_cubic_PRE_fugacity_coefficient_vdwb(ret_error, i, z_i,
		Z, A_m, B_m, b_i, b_m, a_i, a_m, a_m_12, b_m_12);
}


/*
 * refrigerant_cubic_1pvdw_parameters:
 * -----------------------------------
 *
 * Calculates mixing parameters according to one-parameter van der Waals mixing
 * rule:
 *
 *	a_m = z_1^2 * a_1 + 2 * z_1 * z_2 * a_12 + z_2^2 * a_2
 *	b_m = z_1 * b_1 + z_2 * b_2
 *
 *		with: a_12 = sqrt(a_1 * a_2) * (1 - delta_12)
 *		----- b_12 = (b_1 + b_2) / 2
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Temperature in K.
 *	double z_1:
 *		Mole fraction of component 1 in liquid or vapor phase.
 *	double a_1:
 *		Pure component parameter a of component 1.
 *	double a_2:
 *		Pure component parameter a of component 2.
 *	double b_1:
 *		Pure component parameter b of component 1.
 *	double b_2:
 *		Pure component parameter b of component 2.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients of mixing rule.
 *
 * Returns:
 * --------
 *	*ret_par:
 *		Returns mixing parameters a_m, b_m, a_12 and b_12.
 *
 * Remarks:
 * --------
 *	Function that calls this functions need pre-allocate array 'ret_par' to
 *	ensure correct memory handling.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
void refrigerant_cubic_1pvdw_parameters(double *ret_par, double T_K, double z_1,
	double a_1, double a_2, double b_1, double b_2, double refrigerant_par[]) {
	// Calculate auxiliary parameters
	//
    double z_2 = 1 - z_1;

	// Avoid compiler warning C4100 by adding "do nothing" expression
	// "Do nothing" expression will be optimized out by compiler
	//
	(T_K) = (T_K);

	// Calculate and return mixing parameters
	//
    ret_par[2] = (1 - refrigerant_par[0]) * sqrt(a_1 * a_2);
    ret_par[3] = (b_1 + b_2) / 2;
    ret_par[0] = pow(z_1, 2) * a_1 + 2 * z_1 * z_2 * ret_par[2] +
		pow(z_2, 2) * a_2;
    ret_par[1] = z_1 * b_1 + z_2 * b_2;
}


/*
 * refrigerant_cubic_2pvdw_parameters:
 * -----------------------------------
 *
 * Calculates mixing parameters according to two-parameter van der Waals mixing
 * rule:
 *
 *	a_m = z_1^2 * a_1 + 2 * z_1 * z_2 * a_12 + z_2^2 * a_2
 *	b_m = z_1^2 * b_1 + 2 * z_1 * z_2 * b_12 + z_2^2 * b_2
 *
 *		with: a_12 = sqrt(a_1 * a_2) * (1 - delta_12)
 *		----- b_12 = (b_1 + b_2) / 2 * (1 - m)
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Temperature in K.
 *	double z_1:
 *		Mole fraction of component 1 in liquid or vapor phase.
 *	double a_1:
 *		Pure component parameter a of component 1.
 *	double a_2:
 *		Pure component parameter a of component 2.
 *	double b_1:
 *		Pure component parameter b of component 1.
 *	double b_2:
 *		Pure component parameter b of component 2.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients of mixing rule.
 *
 * Returns:
 * --------
 *	*ret_par:
 *		Returns mixing parameters a_m, b_m, a_12 and b_12.
 *
 * Remarks:
 * --------
 *	Function that calls this functions need pre-allocate array 'ret_par' to
 *	ensure correct memory handling.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
void refrigerant_cubic_2pvdw_parameters(double *ret_par, double T_K, double z_1,
	double a_1, double a_2, double b_1, double b_2, double refrigerant_par[]) {
	// Calculate auxiliary parameters
	//
    double z_2 = 1 - z_1;

	// Avoid compiler warning C4100 by adding "do nothing" expression
	// "Do nothing" expression will be optimized out by compiler
	//
	(T_K) = (T_K);

	// Calculate and return mixing parameters
	//
    ret_par[2] = (1 - refrigerant_par[0]) * sqrt(a_1 * a_2);
    ret_par[3] = (1 - refrigerant_par[1]) * (b_1 + b_2) / 2;
    ret_par[0] = pow(z_1, 2) * a_1 + 2 * z_1 * z_2 * ret_par[2] +
		pow(z_2, 2) * a_2;
    ret_par[1] = pow(z_1, 2) * b_1 + 2 * z_1 * z_2 * ret_par[3] +
		pow(z_2, 2) * b_2;
}


/*
 * refrigerant_cubic_vdwb_parameters:
 * ----------------------------------
 *
 * Calculates mixing parameters according to modified van der Waals and
 * Berthelot mixing rule:
 *
 *	a_m = z_1^2 * a_1 + 2 * z_1 * z_2 * a_12 + z_2^2 * a_2
 *	b_m = z_1^2 * b_1 + 2 * z_1 * z_2 * b_12 + z_2^2 * b_2
 *
 *		with: a_12 = sqrt(a_1 * a_2) * (1 + t / T) * (1 - delta_12)
 *		----- b_12 = (b_1 + b_2) / 2 * (1 - m)
 *			  delta_12 = l_12 * l_21 * (z_1 + z_2) / (l_21 * z_1 + l_12 * z_2)
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Temperature in K.
 *	double z_1:
 *		Mole fraction of component 1 in liquid or vapor phase.
 *	double a_1:
 *		Pure component parameter a of component 1.
 *	double a_2:
 *		Pure component parameter a of component 2.
 *	double b_1:
 *		Pure component parameter b of component 1.
 *	double b_2:
 *		Pure component parameter b of component 2.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients of mixing rule.
 *
 * Returns:
 * --------
 *	*ret_par:
 *		Returns mixing parameters a_m, b_m, a_12 and b_12.
 *
 * Remarks:
 * --------
 *	Function that calls this functions need pre-allocate array 'ret_par' to
 *	ensure correct memory handling.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
void refrigerant_cubic_vdwb_parameters(double *ret_par, double T_K, double z_1,
	double a_1, double a_2, double b_1, double b_2, double refrigerant_par[]) {
	// Calculate auxiliary parameters
	//
    double z_2 = 1 - z_1;
	double delta_12 = refrigerant_par[2] * refrigerant_par[3] * (z_1 + z_2) /
		(refrigerant_par[3] * z_1 + refrigerant_par[2] * z_2);

	// Calculate and return mixing parameters
	//
    ret_par[2] = (1 - delta_12) * (1 + refrigerant_par[4] / T_K) *
		sqrt(a_1 * a_2);
    ret_par[3] = (1 - refrigerant_par[1]) * (b_1 + b_2) / 2;
    ret_par[0] = pow(z_1, 2) * a_1 + 2 * z_1 * z_2 * ret_par[2] +
		pow(z_2, 2) * a_2;
    ret_par[1] = pow(z_1, 2) * b_1 + 2 * z_1 * z_2 * ret_par[3] +
		pow(z_2, 2) * b_2;
}
