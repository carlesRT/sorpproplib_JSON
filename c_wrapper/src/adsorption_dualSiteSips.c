///////////////////////////////
// adsorption_dualSiteSips.c //
///////////////////////////////
#include <math.h>
#include "adsorption_dualSiteSips.h"


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
 * General form of Dual-Site-Sips equation:
 * ----------------------------------------
 * 	w = w_A * [(b_A * p)^(1 / eta_A)] / [1 + (b_A * p)^(1 / eta_A)] +
 *      w_B * [(b_B * p)^(1 / eta_B)] / [1 + (b_B * p)^(1 / eta_B)]
 *
 * Temperature-dependent coefficients:
 * -----------------------------------
 * 	b_i = b_i_0 * exp(Q_i_st / (R * T) * (1 - T / T_0)) with i = A or B
 *
 * Possible inputs required by user:
 * ---------------------------------
 *	p: Pressure in Pa
 *	T: Temperature in K
 *	w: Loading in kg/kg
 *
 * Order of coefficients in JSON-file:
 * -----------------------------------
 *	isotherm_par[0]	-> b_A_0	-> in 1/Pa
 * 	isotherm_par[1] -> b_B_0	-> in 1/Pa
 * 	isotherm_par[2]	-> Q_A_st	-> in J/mol
 * 	isotherm_par[3]	-> Q_B_st	-> in J/mol
 * 	isotherm_par[4]	-> eta_A	-> in -
 * 	isotherm_par[5]	-> eta_B	-> in -
 * 	isotherm_par[6]	-> w_A		-> in kg/kg
 * 	isotherm_par[7]	-> w_B		-> in kg/kg
 * 	isotherm_par[8]	-> T_0		-> in K
 *
 */
 

/*
 * adsorption_dualSiteSips_w_pT:
 * -----------------------------
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
 *		Array of doubles that contains coefficients of Dual-Site-Sips equation.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium loading in kg/kg.
 *
 * History:
 * --------
 *	11/23/2019, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_dualSiteSips_w_pT(double p_Pa, double T_K, 
	double isotherm_par[]) {
	// Calculate temperature-dependent coefficients
	//
	double b_A = isotherm_par[0] * exp(isotherm_par[2] / (IDEAL_GAS_CONSTANT *
		T_K) * (1 - T_K / isotherm_par[8]));
	double b_B = isotherm_par[1] * exp(isotherm_par[3] / (IDEAL_GAS_CONSTANT * 
		T_K) * (1 - T_K / isotherm_par[8]));
	
	// Calculate loading
	//
	double w_A = isotherm_par[6] * pow((b_A * p_Pa), 1/isotherm_par[4]) /
		(1 + pow((b_A * p_Pa), 1/isotherm_par[4]));
	double w_B = isotherm_par[7] * pow((b_B * p_Pa), 1/isotherm_par[5]) /
		(1 + pow((b_B * p_Pa), 1/isotherm_par[5]));

	return w_A + w_B;
}
 
 
/*
 * adsorption_dualSiteSips_p_wT:
 * -----------------------------
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
 *		Array of doubles that contains coefficients of Dual-Site-Sips equation.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium pressure in Pa.
 *
 * Remarks:
 * --------
 *	Uses Newton-Raphson method for calculating equlibrium pressure.
 *
 * History:
 * --------
 *	11/23/2019, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_dualSiteSips_p_wT(double w_kgkg, double T_K,
	double isotherm_par[]) {
	// Initialise variables for using Newton-Raphson method
	//
	double p_guess_Pa = 10000;
	double w_guess_kgkg = w_kgkg;
	double dw_guess_dp_kgkgPa = w_kgkg/p_guess_Pa;

	int counter_NRM = 0;
	const double tolerance = 1e-8;

	// Calculate temperature using Newton-Raphson method
	//
	for (w_guess_kgkg = adsorption_dualSiteSips_w_pT(p_guess_Pa, T_K,
		 isotherm_par);
	     fabs(w_guess_kgkg - w_kgkg)>tolerance && counter_NRM<50; 
		 counter_NRM++) {
		// Calculate loading depending on guess value for pressure and 
		// temperature
		//
		w_guess_kgkg = adsorption_dualSiteSips_w_pT(p_guess_Pa, T_K,
			isotherm_par);

		// Calculate derivative of the loading with respect to pressure
		//
		dw_guess_dp_kgkgPa = 
			adsorption_dualSiteSips_dw_dp_pT(p_guess_Pa, T_K, isotherm_par);

		// Update guess value for pressure
		// Only positive values are allowed
		//
		p_guess_Pa -= (w_guess_kgkg - w_kgkg) / dw_guess_dp_kgkgPa;
		p_guess_Pa = p_guess_Pa < 0 ? 1000 : p_guess_Pa;
	}

	// Return -1 when number of iteratoons exceeds 50
	//
	return (counter_NRM == 50 ? -1 : p_guess_Pa);
}


/*
 * adsorption_dualSiteSips_T_pw:
 * -----------------------------
 *
 * Calculates equilibrium temperature in K depending on equilibrium pressure p
 * in Pa and equilibrium loading w in kg/kg.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Dual-Site-Sips equation.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium temperature in K.
 *
 * Remarks:
 * --------
 *	Uses Newton-Raphson method for calculating equlibrium temperature.
 *
 * History:
 * --------
 *	11/23/2019, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_dualSiteSips_T_pw(double p_Pa, double w_kgkg,
	double isotherm_par[]) {
	// Initialise variables for using Newton-Raphson method
	//
	double T_guess_K = 303.15;
	double w_guess_kgkg = w_kgkg;
	double dw_guess_dT_kgkgK = w_kgkg/T_guess_K;

	int counter_NRM = 0;
	const double tolerance = 1e-8;

	// Calculate temperature using Newton-Raphson method
	//
	for (w_guess_kgkg = adsorption_dualSiteSips_w_pT(p_Pa, T_guess_K,
		 isotherm_par);
	     fabs(w_guess_kgkg - w_kgkg)>tolerance && counter_NRM<50; 
		 counter_NRM++) {
		// Calculate loading depending on pressure and guess values for 
		// temperature
		//
		w_guess_kgkg = adsorption_dualSiteSips_w_pT(p_Pa, T_guess_K,
			isotherm_par);

		// Calculate the first derivative of the loading with respect to 
		// temperature
		//
		dw_guess_dT_kgkgK =
			adsorption_dualSiteSips_dw_dT_pT(p_Pa, T_guess_K, isotherm_par);

		// Update guess value for temperature
		// Only positive values are allowed
		//
		T_guess_K -= (w_guess_kgkg - w_kgkg) / dw_guess_dT_kgkgK;
		T_guess_K = T_guess_K < 0 ? 323.15 : T_guess_K;
	}

	// Returns -1 when number of iterations exceeds 50
	//
	return (counter_NRM == 50 ? -1 : T_guess_K);
}
 

/*
 * adsorption_dualSiteSips_dw_dp_pT:
 * ---------------------------------
 *
 * Calculates derivative of equilibrium loading w with respect to pressure p 
 * in kg/kg/Pa depending on equilibrium pressure p in Pa and equilibrium 
 * temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Dual-Site-Sips equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. pressure in kg/kg/Pa.
 *
 * History:
 * --------
 *	11/23/2019, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_dualSiteSips_dw_dp_pT(double p_Pa, double T_K, 
	double isotherm_par[]) {
	// Calculate temperature-dependent coefficients
	//
	double b_A = isotherm_par[0] * exp(isotherm_par[2] / (IDEAL_GAS_CONSTANT *
		T_K) * (1 - T_K / isotherm_par[8]));
	double b_B = isotherm_par[1] * exp(isotherm_par[3] / (IDEAL_GAS_CONSTANT * 
		T_K) * (1 - T_K / isotherm_par[8]));
	
	// Calculate derivative of loading wrt. to pressure
	//
	double dw_A_dp = isotherm_par[6] * pow((b_A * p_Pa), 1/isotherm_par[4]) /
		(isotherm_par[4]* p_Pa * pow(1 + pow((b_A * p_Pa), 1/isotherm_par[4]),
		2));
	double dw_B_dp = isotherm_par[7] * pow((b_B * p_Pa), 1/isotherm_par[5]) /
		(isotherm_par[5]* p_Pa * pow(1 + pow((b_B * p_Pa), 1/isotherm_par[5]),
		2));

	return dw_A_dp + dw_B_dp;
}


/*
 * adsorption_dualSiteSips_dw_dT_pT:
 * ---------------------------------
 *
 * Calculates derivative of equilibrium loading w with respect to temperature T
 * in kg/kg/K depending on equilibrium pressure p in Pa and equilibrium 
 * temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Dual-Site-Sips equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. temperature in kg/kg/K.
 *
 * History:
 * --------
 *	11/23/2019, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_dualSiteSips_dw_dT_pT(double p_Pa, double T_K,
	double isotherm_par[]) {
	// Calculate temperature-dependent coefficients
	//
	double b_A = isotherm_par[0] * exp(isotherm_par[2] / (IDEAL_GAS_CONSTANT *
		T_K) * (1 - T_K / isotherm_par[8]));
	double b_B = isotherm_par[1] * exp(isotherm_par[3] / (IDEAL_GAS_CONSTANT *
		T_K) * (1 - T_K / isotherm_par[8]));
		
	double db_A_dT = -(isotherm_par[2] / (IDEAL_GAS_CONSTANT * pow(T_K, 2))) *
		b_A;
	double db_B_dT = -(isotherm_par[3] / (IDEAL_GAS_CONSTANT * pow(T_K, 2))) *
		b_B;
	
	// Calculate derivative of loading wrt. to temperature
	//
	double dw_A_db_A = isotherm_par[6] * pow((b_A * p_Pa), 1/isotherm_par[4]) /
		(isotherm_par[4] * b_A * pow(1 + pow((b_A * p_Pa), 1/isotherm_par[4]),
		2));
	double dw_B_db_B = isotherm_par[7] * pow((b_B * p_Pa), 1/isotherm_par[5]) /
		(isotherm_par[5] * b_B * pow(1 + pow((b_B * p_Pa), 1/isotherm_par[5]),
		2));
	
	return dw_A_db_A*db_A_dT + dw_B_db_B*db_B_dT;
}
	
	
/*
 * adsorption_dualSiteSips_dp_dw_wT:
 * ---------------------------------
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
 *		Array of doubles that contains coefficients of Dual-Site-Sips equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. loading in kgPa/kg.
 *
 * Remarks:
 * --------
 *	Derivative is calculated numerically by the symmetric derivative using
 *  h = 0.00001 kg/kg as small change.
 *
 * History:
 * --------
 *	01/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_dualSiteSips_dp_dw_wT(double w_kgkg, double T_K,
	double isotherm_par[]) {
	// Calculate temperature-dependent variables
	//
	const double dw_kgkg = 0.00001;
	
	double w_plus_kgkg = adsorption_dualSiteSips_p_wT(w_kgkg + dw_kgkg, T_K,
		isotherm_par);
	double w_minus_kgkg = adsorption_dualSiteSips_p_wT(w_kgkg - dw_kgkg, T_K,
		isotherm_par);
	
	// Calculate derivative of pressure wrt loading
	//
	return (w_plus_kgkg - w_minus_kgkg) / (2*dw_kgkg);
}
 

/*
 * adsorption_dualSiteSips_dp_dT_wT:
 * ---------------------------------
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
 *		Array of doubles that contains coefficients of Dual-Site-Sips equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. temperature in Pa/K.
 *
 * Remarks:
 * --------
 *	Derivative is calculated numerically by the symmetric derivative using
 *  h = 0.001 K as small change.
 *
 * History:
 * --------
 *	01/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_dualSiteSips_dp_dT_wT(double w_kgkg, double T_K,
	double isotherm_par[]) {
	// Calculate temperature-dependent variables
	//
	const double dT_K = 0.001;
	
	double w_plus_kgkg = adsorption_dualSiteSips_p_wT(w_kgkg, T_K + dT_K,
		isotherm_par);
	double w_minus_kgkg = adsorption_dualSiteSips_p_wT(w_kgkg, T_K - dT_K,
		isotherm_par);
	
	// Calculate derivative of pressure wrt loading
	//
	return (w_plus_kgkg - w_minus_kgkg) / (2*dT_K);
}
 

/*
 * adsorption_dualSiteSips_piStar_pyxgTM:
 * --------------------------------------
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
 *		Array of doubles that contains coefficients of Dual-Site-Sips equation.
 *
 * Returns:
 * --------
 *	double:
 *		Reduced spreading pressure in kg/mol.
 *
 * Remarks:
 * --------
 *	Reduced spreading pressure is needed to solve, for example, the Ideal
 *	Adsorbed Solution (IAST) theory.
 *
 * History:
 * --------
 *	01/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_dualSiteSips_piStar_pyxgTM(double p_total_Pa, double y_molmol, 
	double x_molmol, double gamma, double T_K, double M_kgmol,
	double isotherm_par[]) {
	// Calculate temperature-dependent coefficients
	//
	double b_A = isotherm_par[0] * exp(isotherm_par[2] / (IDEAL_GAS_CONSTANT *
		T_K) * (1 - T_K / isotherm_par[8]));
	double b_B = isotherm_par[1] * exp(isotherm_par[3] / (IDEAL_GAS_CONSTANT * 
		T_K) * (1 - T_K / isotherm_par[8]));
		
	// Calculate reduced spreading pressure
	//
	double p0_Pa = p_total_Pa * y_molmol / x_molmol / gamma;
	
	return 1/ M_kgmol * (isotherm_par[4] * isotherm_par[6] * log(1 + 
		pow(b_A * p0_Pa, 1 / isotherm_par[4])) + isotherm_par[5] * 
		isotherm_par[7] * log(1 + pow(b_B * p0_Pa, 1 / isotherm_par[5])));
}
