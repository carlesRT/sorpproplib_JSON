/////////////////////////////
// adsorption_freundlich.c //
/////////////////////////////
#include <math.h>
#include "adsorption_freundlich.h"


/////////////////////////////
// Definition of functions //
/////////////////////////////
/*
 * General form of Freundlich equation:
 * ------------------------------------
 * 	w = A(T) * (p / p_sat(T))^(B(T))
 *
 * Temperature-dependent coefficients:
 * -----------------------------------
 * 	A(T) = A0 + A1*T + A2*T^2 + A3*T^3
 * 	B(T) = B0 + B1*T + B2*T^2 + B3*T^3
 *
 * Possible inputs required by user:
 * ---------------------------------
 *	p: Pressure in Pa
 *	T: Temperature in K
 *	w: Loading in kg/kg
 *	p_sat(T): Saturation pressure in Pa
 *	rho_l(T): Density of adsorpt in kg/m³
 *	dp_sat_dT(T): Derivative of vapor pressure wrt. temperature in Pa/K
 *	drho_l_dT(T): Derivative of saturated liquid density of adsorpt wrt.
 *		temperature in kg/m³/K
 *
 * Order of coefficients in JSON-file:
 * -----------------------------------
 *	isotherm_par[0]	-> A0	-> in kg/kg
 * 	isotherm_par[1] -> A1	-> in kg/kg/K
 * 	isotherm_par[2] -> A2	-> in kg/kg/K^2
 * 	isotherm_par[3] -> A3	-> in kg/kg/K^3
 * 	isotherm_par[4] -> B0	-> in -
 * 	isotherm_par[5] -> B1	-> in 1/K
 * 	isotherm_par[6] -> B2	-> in 1/K^2
 * 	isotherm_par[7] -> B3	-> in 1/K^3
 *
 */


/*
 * adsorption_freundlich_w_pTpsat:
 * -------------------------------
 *
 * Calculates equilibrium loading w in kg/kg depending on equilibrium pressure
 * p in Pa, equilibrium temperature T in K, and saturation pressure p_sat in Pa.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Freundlich equation.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium loading in kg/kg.
 *
 * History:
 * --------
 *	03/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_freundlich_w_pTpsat(double p_Pa, double T_K, double p_sat_Pa,
	double isotherm_par[]) {
	// Calculate temperature-dependent coefficients
	//
	double A = isotherm_par[0] + isotherm_par[1] * T_K + isotherm_par[2] *
		pow(T_K, 2) + isotherm_par[3] * pow(T_K, 3);
	double B = isotherm_par[4] + isotherm_par[5] * T_K + isotherm_par[6] *
		pow(T_K, 2) + isotherm_par[7] * pow(T_K, 3);

	// Calculate loading
	//
	return A * pow(p_Pa / p_sat_Pa, B);
}


/*
 * adsorption_freundlich_p_wTpsat:
 * -------------------------------
 *
 * Calculates equilibrium pressure p in Pa depending on equilibrium loading w
 * in kg/kg, equilibrium temperature T in K, and saturation pressure p_sat in
 * Pa.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Freundlich equation.
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
double adsorption_freundlich_p_wTpsat(double w_kgkg, double T_K,
	double p_sat_Pa, double isotherm_par[]) {
	// Calculate temperature-dependent coefficients
	//
	double A = isotherm_par[0] + isotherm_par[1] * T_K + isotherm_par[2] *
		pow(T_K, 2) + isotherm_par[3] * pow(T_K, 3);
	double B = isotherm_par[4] + isotherm_par[5] * T_K + isotherm_par[6] *
		pow(T_K, 2) + isotherm_par[7] * pow(T_K, 3);

	// Calculate pressure
	//
	return pow(w_kgkg / A, 1 / B) * p_sat_Pa;
}


/*
 * adsorption_freundlich_T_pwpsat:
 * -------------------------------
 *
 * Calculates equilibrium temperature T in K depending on equilibrium pressure p
 * in Pa, equilibrium loading w in kg/kg, and saturation pressure in Pa.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double (*p_sat_T_PaK)(double T_K, double p_sat_par[], void *refrigerant):
 *		Function pointer for vapor pressure wrt. temperature.
 *	double (*dp_sat_dT_PaK)(double T_K, double p_sat_par[], void *refrigerant):
 *		Function pointer for derivative of vapor pressure wrt. temperature.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Freundlich equation.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients vapor pressure.
 *	void *refrigerant:
 *		Pointer to Refrigerant-struct required for refrigerant functions.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium temperature in K.
 *
 * Remarks:
 * --------
 *	No error handling because pointers are checked at highest level (i.e.
 *	functions for workingPair-Struct).
 *	Uses Newton-Raphson method for calculating equilibrium temperature.
 *
 * History:
 * --------
 *	03/17/2020, by Mirko Engelpracht:
 *		First implementation.
 *	04/08/2020, by Mirko Engelpracht:
 *		Added proper refrigerant functions.
 *
 */
double adsorption_freundlich_T_pwpsat(double p_Pa, double w_kgkg,
	double (*p_sat_T_Pa)(double T_K, double p_sat_par[], void *refrigerant),
	double (*dp_sat_dT_PaK)(double T_K, double p_sat_par[], void *refrigerant),
	double isotherm_par[], double p_sat_par[], void *refrigerant) {
	// Initialize variables for using Newton-Raphson method
	//
	double T_guess_K = 353.15;
	double w_guess_kgkg = w_kgkg;
	double dw_guess_dT_kgkgK = w_kgkg/T_guess_K;

	int counter_NRM = 0;
	const double tolerance = 1e-8;

	// Calculate temperature using Newton-Raphson method
	//
	for (w_guess_kgkg = adsorption_freundlich_w_pTpsat(p_Pa, T_guess_K,
			p_sat_T_Pa(T_guess_K, p_sat_par, refrigerant), isotherm_par);
	     fabs(w_guess_kgkg - w_kgkg)>tolerance && counter_NRM<50;
		 counter_NRM++) {
		// Calculate loading depending on pressure and guess values for
		// temperature
		//
		w_guess_kgkg = adsorption_freundlich_w_pTpsat(p_Pa, T_guess_K,
			p_sat_T_Pa(T_guess_K, p_sat_par, refrigerant), isotherm_par);

		// Calculate the first derivative of the loading with respect to
		// temperature
		//
		dw_guess_dT_kgkgK = adsorption_freundlich_dw_dT_pTpsat(p_Pa, T_guess_K,
			p_sat_T_Pa(T_guess_K, p_sat_par, refrigerant),
			dp_sat_dT_PaK(T_guess_K, p_sat_par, refrigerant), isotherm_par);

		// Update guess value for temperature
		// Negative temperatures are not allowed
		//
		T_guess_K -= (w_guess_kgkg - w_kgkg) / dw_guess_dT_kgkgK;
		T_guess_K = T_guess_K < 0 ? 323.15 : T_guess_K;
	}

	// Return -1 when number of iterations exceeds 50
	//
	return (counter_NRM == 50 ? -1 : T_guess_K);
}


/*
 * adsorption_freundlich_dw_dp_pTpsat:
 * -----------------------------------
 *
 * Calculates derivative of equilibrium loading w with respect to pressure
 * p in kg/kg/Pa depending on equilibrium pressure p in Pa, equilibrium
 * temperature T in K, and saturation pressure p_sat in Pa.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Freundlich equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. pressure in kg/kg/Pa.
 *
 * History:
 * --------
 *	03/17/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_freundlich_dw_dp_pTpsat(double p_Pa, double T_K,
	double p_sat_Pa, double isotherm_par[]) {
	// Calculate temperature-dependent coefficients
	//
	double B = isotherm_par[4] + isotherm_par[5] * T_K + isotherm_par[6] *
		pow(T_K, 2) + isotherm_par[7] * pow(T_K, 3);

	double w_kgkg = adsorption_freundlich_w_pTpsat(p_Pa, T_K, p_sat_Pa,
		isotherm_par);

	// Calculate derivative of loading with respect to pressure
	//
	return B / p_Pa * w_kgkg;
}


/*
 * adsorption_freundlich_dw_dT_pTpsat:
 * -----------------------------------
 *
 * Calculates derivative of equilibrium loading w with respect to pressure
 * p in kg/kg/Pa depending on equilibrium pressure p in Pa, equilibrium
 * temperature T in K, saturation pressure p_sat in Pa, and derivative of
 * saturation pressure wrt. temperature dp_sat_dT in Pa/K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation pressure in Pa.
 * 	double dp_sat_dT_PaK:
 *		Derivative of saturation pressure wrt. temperature in Pa/K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Freundlich equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. temperature in kg/kg/K.
 *
 * History:
 * --------
 *	03/17/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_freundlich_dw_dT_pTpsat(double p_Pa, double T_K,
	double p_sat_Pa, double dp_sat_dT_PaK, double isotherm_par[]) {
	// Calculate temperature-dependent coefficients
	//
	double A = isotherm_par[0] + isotherm_par[1] * T_K + isotherm_par[2] *
		pow(T_K, 2) + isotherm_par[3] * pow(T_K, 3);
	double B = isotherm_par[4] + isotherm_par[5] * T_K + isotherm_par[6] *
		pow(T_K, 2) + isotherm_par[7] * pow(T_K, 3);

	// Calculate partial derivatives
	//
	double dA_dT = isotherm_par[1] + 2 * isotherm_par[2] * T_K +
		3 * isotherm_par[3] * pow(T_K, 2);
	double dB_dT = isotherm_par[5] + 2 * isotherm_par[6] * T_K +
		3 * isotherm_par[7] * pow(T_K, 2);

	// Calculate derivative of loading with respect to temperature:
	//
	return pow(p_Pa / p_sat_Pa, B) * (p_sat_Pa * (dA_dT + A * dB_dT * log(p_Pa /
		p_sat_Pa)) - A * B * dp_sat_dT_PaK) / p_sat_Pa;
}


/*
 * adsorption_freundlich_dp_dw_wTpsat:
 * -----------------------------------
 *
 * Calculates derivative of equilibrium pressure p with respect to loading
 * w in kgPa/kg depending on equilibrium loading w in kg/kg, and saturation
 * pressure p_sat in Pa.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Freundlich equation.
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
double adsorption_freundlich_dp_dw_wTpsat(double w_kgkg, double T_K,
	double p_sat_Pa, double isotherm_par[]) {
	// Calculate temperature-dependent coefficients
	//
	double B = isotherm_par[4] + isotherm_par[5] * T_K + isotherm_par[6] *
		pow(T_K, 2) + isotherm_par[7] * pow(T_K, 3);

	double p_Pa = adsorption_freundlich_p_wTpsat(w_kgkg, T_K, p_sat_Pa,
		isotherm_par);

	// Calculate derivative of pressure wrt loading
	//
	return p_Pa / (w_kgkg * B);


}


/*
 * adsorption_freundlich_dp_dT_wTpsat:
 * -----------------------------------
 *
 * Calculates derivative of equilibrium pressure p with respect to temperature
 * T in Pa/K depending on equilibrium loading w in kg/kg, equilibrium
 * temperature T in K, saturation pressure p_sat in Pa, and derivative of
 * saturation pressure wrt. temperature dp_sat_dT in Pa/K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation pressure in Pa.
 * 	double dp_sat_dT_PaK:
 *		Derivative of saturation pressure wrt. temperature in Pa/K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Freundlich equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. temperature in Pa/K.
 *
 * History:
 * --------
 *	03/17/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_freundlich_dp_dT_wTpsat(double w_kgkg, double T_K,
	double p_sat_Pa, double dp_sat_dT_PaK, double isotherm_par[]) {
	// Calculate temperature-dependent coefficients
	//
	double A = isotherm_par[0] + isotherm_par[1] * T_K + isotherm_par[2] *
		pow(T_K, 2) + isotherm_par[3] * pow(T_K, 3);
	double B = isotherm_par[4] + isotherm_par[5] * T_K + isotherm_par[6] *
		pow(T_K, 2) + isotherm_par[7] * pow(T_K, 3);

	// Calculate partial derivatives
	//
	double dA_dT = isotherm_par[1] + 2 * isotherm_par[2] * T_K +
		3 * isotherm_par[3] * pow(T_K, 2);
	double dB_dT = isotherm_par[5] + 2 * isotherm_par[6] * T_K +
		3 * isotherm_par[7] * pow(T_K, 2);


	// Calculate derivative of pressure wrt temperature
	//
	return pow(w_kgkg / A, 1/B) * (-B * p_sat_Pa * dA_dT - A * p_sat_Pa *
		dB_dT * log(w_kgkg / A) + A * pow(B, 2) *dp_sat_dT_PaK) / (A *
		pow(B, 2));
}


/*
 * adsorption_freundlich_piStar_pyxgTpsatM:
 * ----------------------------------------
 *
 * Calculates reduced spreading pressure in kg/mol depending on total pressure
 * of vapor phase p_total in Pa, molar fraction of refrigerant in vapor phase
 * y in mol/mol, molar fraction of refrigerant in adsorbed phase in mol/mol,
 * activity coefficient of refrigerant in adsorbed phase, equilibrium
 * temperature T in K, saturation pressure p_sat in Pa, and molar mass of
 * refrigerant M in kg/mol. The reduced spreading pressure is defined as
 * follows:
 *
 * 	piStar = A * pi / (R * T * m_sorbent) = 1 / M *
 *		Integral_0^p0{w(p,T) / p * dp}
 *
 *	where p0 = p_total*y / (gamma*x)
 *
 * Parameters:
 * -----------
 *	double p_total_Pa:
 *		Total pressure of vapor phase in Pa.
 *	double y_molmol:
 *		Molar fraction of refrigerant in vapor phase in mol/mol.
 *	double x_molmol:
 *		Molar fraction of refrigerant in adsorbed phase in mol/mol.
 *	double gamma:
 *		Activity coefficient of refrigerant in adsorbed phase.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double M_kgmol:
 *		Molar mass of refrigerant M in kg/mol.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Freundlich equation.
 *
 * Returns:
 * --------
 *	double:
 *		Reduced spreading pressure in kg/mol.
 *
 * Remarks:
 * --------
 *	Reduced spreading pressure is needed to solve, for example, the Ideal
 *	Adsorbed Solution (IAST) theory. Implemented solution is solved
 *	numerically.
 *
 * History:
 * --------
 *	03/17/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_freundlich_piStar_pyxgTpsatM(double p_total_Pa,
	double y_molmol, double x_molmol, double gamma, double T_K,
	double p_sat_Pa, double M_kgmol, double isotherm_par[]) {
	// Calculate numerically reduced spreading pressure
	//
	int n_counter;
	int n_steps = 1000000;
	double h = ((p_total_Pa * y_molmol / x_molmol / gamma) - 0) / n_steps;
	double p_tmp;
	double piStar_molkg_num = 0;

	for(n_counter=1; n_counter<n_steps; n_counter++) {
		// Calculate reduced spreading pressure by numerical integration
		// using the trapezoidal rule
		//
		p_tmp = 0 - h/2 + n_counter * h;
		piStar_molkg_num += (h / M_kgmol) *
			(adsorption_freundlich_w_pTpsat(p_tmp, T_K, p_sat_Pa,
			isotherm_par) / p_tmp);
	}

	return piStar_molkg_num;
}
