///////////////////////
// adsorption_toth.h //
///////////////////////
#ifndef adsorption_toth_h
#define adsorption_toth_h


///////////////////////////////////////
// Definition of function prototypes //
///////////////////////////////////////
/*
 * General form of Toth equation:
 * ------------------------------
 * 	w = [w_s * b^m * p] / [1 + b^r * p^n]^(1/n)
 *
 * Temperature-dependent coefficients:
 * -----------------------------------
 * 	b = b0 * exp(q_star_R / T)
 * 	n = n0 + c/T
 *	r = if r<0 then n else r
 *
 * Possible inputs required by user:
 * ---------------------------------
 *	p: Pressure in Pa
 *	T: Temperature in K
 *	w: Loading in kg/kg
 *
 * Order of coefficients in JSON-file:
 * -----------------------------------
 *	isotherm_par[0]	-> b0		-> in 1/Pa
 * 	isotherm_par[1] -> c		-> in K
 * 	isotherm_par[2] -> m		-> in -
 * 	isotherm_par[3] -> n0		-> in -
 * 	isotherm_par[4] -> q_star_R	-> in K
 * 	isotherm_par[5] -> r		-> in -
 * 	isotherm_par[6] -> w_s		-> in kg/kg
 *
 */


/*
 * adsorption_toth_w_pT:
 * ---------------------
 *
 * Calculates equilibrium loading w in kg/kg depending on equilibrium pressure
 * p in Pa and equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Toth equation.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium loading in kg/kg.
 *
 * History:
 * --------
 *	10/18/2019, by Mirko Engelpracht:
 *		First implementation.
 *	11/24/2019, by Mirko Engelpracht:
 *		Updated units of coefficients to SI-units.
 *
 */
double adsorption_toth_w_pT(double p_Pa, double T_K, double isotherm_par[]);


/*
 * adsorption_toth_p_wT:
 * ---------------------
 *
 * Calculates equilibrium pressure p in Pa depending on equilibrium loading w
 * in kg/kg and equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Toth equation.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium pressure in Pa.
 *
 * History:
 * --------
 *	10/18/2019, by Mirko Engelpracht:
 *		First implementation.
 *	11/24/2019, by Mirko Engelpracht:
 *		Updated units of coefficients to SI-units.
 *
 */
double adsorption_toth_p_wT(double w_kgkg, double T_K, double isotherm_par[]);


/*
 * adsorption_toth_T_pw:
 * ---------------------
 *
 * Calculates equilibrium temperature T in K depending on equilibrium pressure p
 * in Pa and equilibrium loading w in kg/kg.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Toth equation.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium temperature in K.
 *
 * Remarks:
 * --------
 *	Uses Newton-Raphson method for calculating equilibrium temperature.
 *
 * History:
 * --------
 *	10/20/2019, by Mirko Engelpracht:
 *		First implementation.
 *	11/24/2019, by Mirko Engelpracht:
 *		Updated units of coefficients to SI-units.
 *
 */
double adsorption_toth_T_pw(double p_Pa, double w_kgkg, double isotherm_par[]);


/*
 * adsorption_toth_dw_dp_pT:
 * -------------------------
 *
 * Calculates derivative of equilibrium loading w with respect to pressure 
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
 *		Array of doubles that contains coefficients of Toth equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. pressure in kg/kg/Pa.
 *
 * History:
 * --------
 *	11/24/2019, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_toth_dw_dp_pT(double p_Pa, double T_K, double isotherm_par[]);


/*
 * adsorption_toth_dw_dT_pT:
 * -------------------------
 *
 * Calculates derivative of equilibrium loading w with respect to temperature 
 * T in kg/kg/K depending on equilibrium pressure p in Pa and equilibrium 
 * temperature in T K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Toth equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. temperature in kg/kg/K.
 *
 * History:
 * --------
 *	10/20/2019, by Mirko Engelpracht:
 *		First implementation.
 *	11/24/2019, by Mirko Engelpracht:
 *		Updated units of coefficients to SI-units.
 *
 */
double adsorption_toth_dw_dT_pT(double p_Pa, double T_K, double isotherm_par[]);
 

/*
 * adsorption_toth_dp_dw_wT:
 * -------------------------
 *
 * Calculates derivative of equilibrium pressure p with respect to loading 
 * w in kgPa/kg depending on equilibrium loading w in kg/kg and equilibrium 
 * temperature T in K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Toth equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. loading in kgPa/kg.
 *
 * History:
 * --------
 *	01/11/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_toth_dp_dw_wT(double w_kgkg, double T_K,
	double isotherm_par[]);
 

/*
 * adsorption_toth_dp_dT_wT:
 * -------------------------
 *
 * Calculates derivative of equilibrium pressure p with respect to temperature 
 * T in Pa/K depending on equilibrium loading w in kg/kg and equilibrium 
 * temperature T in K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Toth equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. temperature in Pa/K.
 *
 * History:
 * --------
 *	01/11/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_toth_dp_dT_wT(double w_kgkg, double T_K,
	double isotherm_par[]);
 

/*
 * adsorption_toth_piStar_pyxgTM:
 * ------------------------------
 *
 * Calculates reduced spreading pressure in kg/mol depending on total pressure
 * of vapour phase p_total in Pa, molar fraction of refrigerant in vapour phase
 * y in mol/mol, molar fraction of refrigerant in adsorbed phase in mol/mol,
 * activity coefficent of refrigerant in adsorbed phase, equilibrium temperature
 * T in K and molar mass of refrigerant M in kg/mol. The reduced spreading 
 * pressure is defined as follows:
 *
 * 	piStar = A * pi / (R * T * m_sorbent) = 1 / M *
 *		Integral_0^p0{w(p,T) / p * dp}
 *
 *	where p0 = p_total*y / (gamma*x)
 *
 * Parameters:
 * -----------
 *	double p_total_Pa:
 *		Total pressure of vapour phase in Pa.
 *	double y_molmol:
 *		Molar fraction of refrigerant in vapour phase in mol/mol.
 *	double x_molmol:
 *		Molar fraction of refrigerant in adsorbed phase in mol/mol.
 *	double gamma:
 *		Activity coefficent of refrigerant in adsorbed phase.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double M_kgmol:
 *		Molar mass of refrigerant M in kg/mol.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Toth equation.
 *
 * Returns:
 * --------
 *	double:
 *		Reduced spreading pressure in kg/mol.
 *
 * Remarks:
 * --------
 *	Reduced spreading pressure is needed to solve, for example, the Ideal
 *	Adsorbed Solution (IAST) theory. Implementeed solution is solved
 *	numerically; however, an analyitcal solutions exists for m=1 and r<0 that is
 *	based on an infinite sum.
 *
 * History:
 * --------
 *	01/11/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_toth_piStar_pyxgTM(double p_total_Pa, double y_molmol, 
	double x_molmol, double gamma, double T_K, double M_kgmol,
	double isotherm_par[]);

#endif