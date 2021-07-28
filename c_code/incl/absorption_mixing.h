/////////////////////////
// absorption_mixing.h //
/////////////////////////
#ifndef absorption_mixing_h
#define absorption_mixing_h


///////////////////////////////////////
// Definition of function prototypes //
///////////////////////////////////////
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
double ret_max(double a, double b);


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
double ret_min(double a, double b);


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
	double isotherm_par[]);


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
	double isotherm_par[]);


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
	double isotherm_par[]);


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
	double isotherm_par[]);


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
	double isotherm_par[]);


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
	double isotherm_par[]);


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
	double isotherm_par[]);

#endif
