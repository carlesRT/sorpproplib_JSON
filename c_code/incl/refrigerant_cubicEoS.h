////////////////////////////
// refrigerant_cubicEoS.h //
////////////////////////////
#ifndef refrigerant_cubicEoS_h
#define refrigerant_cubicEoS_h


//////////////////////////////////////////////////////////
// Definition of function prototypes: General functions //
//////////////////////////////////////////////////////////
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
	double EOS_3);


/////////////////////////////////////////////////////////////////
// Definition of function prototypes: Cubic equations of state //
/////////////////////////////////////////////////////////////////
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
 */


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
	double refrigerant_par[]);


/*
 * refrigerant_cubic_SRKE_custom_parameters:
 * -----------------------------------------
 *
 * Calculates coefficients a and b of Soave-Redlich-Kwong equation of state that
 * is based on a custom equation for alpha.
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
	double refrigerant_par[]);


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
	double T_K, double a, double b);


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
	double Z, double A, double B);


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
	double refrigerant_par[]);


/*
 * refrigerant_cubic_PRE_custom_parameters:
 * ----------------------------------------
 *
 * Calculates coefficients a and b of Peng-Robinson equation of state that is
 * based on a custom equation for alpha.
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
	double refrigerant_par[]);


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
	double T_K, double a, double b);


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
	double Z, double A, double B);


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
	double refrigerant_par[]);


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
	double T_K, double a, double b);


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
	double Z, double A, double B);


/////////////////////////////////////////////////////
// Definition of function prototypes: Mixing rules //
/////////////////////////////////////////////////////
/*
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
 * Two-parameter van der Waals mixing rule:
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
 *
 */


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
	double a_1, double a_2, double b_1, double b_2, double refrigerant_par[]);


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
	double a_1, double a_2, double b_1, double b_2, double refrigerant_par[]);


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
	double a_1, double a_2, double b_1, double b_2, double refrigerant_par[]);


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
	double b_m, double a_i, double a_m, double a_m_12, double b_m_12);


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
 * Remarks:
 * --------
 *	Not implemented yet. Calls one-parameter van der Waals mixing rule.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_cubic_SRKE_fugacity_coefficient_2pvdw(int *ret_error,
	int i, double z_i, double Z, double A_m, double B_m, double b_i,
	double b_m, double a_i, double a_m, double a_m_12, double b_m_12);


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
 * Remarks:
 * --------
 *	Not implemented yet. Calls one-parameter van der Waals mixing rule.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_cubic_SRKE_fugacity_coefficient_vdwb(int *ret_error,
	int i, double z_i, double Z, double A_m, double B_m, double b_i,
	double b_m, double a_i, double a_m, double a_m_12, double b_m_12);


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
	double b_m, double a_i, double a_m, double a_m_12, double b_m_12);


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
 * Remarks:
 * --------
 *	Not implemented yet. Calls one-parameter van der Waals mixing rule.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_cubic_PRE_fugacity_coefficient_2pvdw(int *ret_error,
	int i, double z_i, double Z, double A_m, double B_m, double b_i,
	double b_m, double a_i, double a_m, double a_m_12, double b_m_12);


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
 * Remarks:
 * --------
 *	Not implemented yet. Calls one-parameter van der Waals mixing rule.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_cubic_PRE_fugacity_coefficient_vdwb(int *ret_error,
	int i, double z_i, double Z, double A_m, double B_m, double b_i,
	double b_m, double a_i, double a_m, double a_m_12, double b_m_12);


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
 * Remarks:
 * --------
 *	Calls corresponding function of Peng-Robinson equation of state as
 *	calculation approach is identical. Hence, check corresponding function of
 *	Peng-Robinson equation of state for further information.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_cubic_PRSVE_fugacity_coefficient_1pvdw(int *ret_error,
	int i, double z_i, double Z, double A_m, double B_m, double b_i,
	double b_m, double a_i, double a_m, double a_m_12, double b_m_12);


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
 * Remarks:
 * --------
 *	Calls corresponding function of Peng-Robinson equation of state as
 *	calculation approach is identical. Hence, check corresponding function of
 *	Peng-Robinson equation of state for further information.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_cubic_PRSVE_fugacity_coefficient_2pvdw(int *ret_error,
	int i, double z_i, double Z, double A_m, double B_m, double b_i,
	double b_m, double a_i, double a_m, double a_m_12, double b_m_12);


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
 * Remarks:
 * --------
 *	Calls corresponding function of Peng-Robinson equation of state as
 *	calculation approach is identical. Hence, check corresponding function of
 *	Peng-Robinson equation of state for further information.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_cubic_PRSVE_fugacity_coefficient_vdwb(int *ret_error,
	int i, double z_i, double Z, double A_m, double B_m, double b_i,
	double b_m, double a_i, double a_m, double a_m_12, double b_m_12);

#endif
