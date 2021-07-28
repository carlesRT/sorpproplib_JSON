/////////////////////////////////
// refrigerant_vaporPressure.h //
/////////////////////////////////
#ifndef refrigerant_vaporPressure_h
#define refrigerant_vaporPressure_h


/////////////////////////////////////////////////////////
// Definition of function prototypes: EoS - Approach 1 //
/////////////////////////////////////////////////////////
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
double refrigerant_p_sat_eos1(const double T_K, const double refrigerant_par[]);


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
	const double refrigerant_par[]);


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
	const double refrigerant_par[]);


/////////////////////////////////////////////////////////
// Definition of function prototypes: EoS - Approach 2 //
/////////////////////////////////////////////////////////
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
double refrigerant_p_sat_eos2(const double T_K, const double refrigerant_par[]);


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
	const double refrigerant_par[]);


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
	const double refrigerant_par[]);


/////////////////////////////////////////////////////////
// Definition of function prototypes: EoS - Approach 3 //
/////////////////////////////////////////////////////////
/*
 * General form of equation for vapor pressure:
 * --------------------------------------------
 * 	p_sat = exp(fac0/T + fac1*T^exp1 + fac2*T^exp2 + fac3*(1-theta)^exp3)
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
double refrigerant_p_sat_eos3(const double T_K, const double refrigerant_par[]);


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
	const double refrigerant_par[]);


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
	const double refrigerant_par[]);


/////////////////////////////////////////////////////////
// Definition of function prototypes: Antoine equation //
/////////////////////////////////////////////////////////
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
double refrigerant_p_sat_antoine(double T_K, double refrigerant_par[]);


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
double refrigerant_dp_sat_dT_antoine(double T_K, double refrigerant_par[]);


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
double refrigerant_T_sat_antoine(double p_Pa, double refrigerant_par[]);


///////////////////////////////////////////////////
// Definition of function prototypes: Cubic EoS  //
///////////////////////////////////////////////////
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
	double refrigerant_par[]);


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
	double refrigerant_par[]);


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
	double refrigerant_par[]);


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
	double refrigerant_par[]);


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
double refrigerant_T_sat_cubic(double T_K,
	void (*calc_eos_parameters)(double[], double, double[]),
	void (*calc_gen_parameters)(double[], double, double, double, double),
	double (*calc_fug_coefficient)(int*, double, double, double),
	double refrigerant_par[]);

#endif
