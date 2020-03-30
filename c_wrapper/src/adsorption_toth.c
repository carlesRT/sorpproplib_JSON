///////////////////////
// adsorption_toth.c //
///////////////////////
#include <math.h>
#include "adsorption_toth.h"


/////////////////////////////
// Definition of functions //
/////////////////////////////
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
double adsorption_toth_w_pT(double p_Pa, double T_K, double isotherm_par[]) {
	// Calculate temperature-dependent coefficients
	//
	double b = isotherm_par[0] * exp(isotherm_par[4] / T_K);
	double n = isotherm_par[3] + isotherm_par[1] / T_K;
	double r = isotherm_par[5]<0 ? n : isotherm_par[5];

	// Calculate loading
	//
	return isotherm_par[6] * pow(b, isotherm_par[2]) * p_Pa /
		pow(1 + pow(b, r) * pow(p_Pa, n), 1/n);
}


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
double adsorption_toth_p_wT(double w_kgkg, double T_K, double isotherm_par[]) {
	// Calculate temperature-dependent coefficients
	//
	double b = isotherm_par[0] * exp(isotherm_par[4] / T_K);
	double n = isotherm_par[3] + isotherm_par[1] / T_K;
	double r = isotherm_par[5]<0 ? n : isotherm_par[5];

	// Calculate pressure
	//
	return w_kgkg / pow(pow(isotherm_par[6], n) * pow(b, isotherm_par[2] * n) -
		pow(w_kgkg, n) * pow(b, r), 1 /n);
}


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
double adsorption_toth_T_pw(double p_Pa, double w_kgkg, double isotherm_par[]) {
	// Initialize variables for using Newton-Raphson method
	//
	double T_guess_K = 353.15;
	double w_guess_kgkg = w_kgkg;
	double dw_guess_dT_kgkgK = w_kgkg/T_guess_K;

	int counter_NRM = 0;
	const double tolerance = 1e-8;

	// Calculate temperature using Newton-Raphson method
	//
	for (w_guess_kgkg = adsorption_toth_w_pT(p_Pa, T_guess_K, isotherm_par);
	     fabs(w_guess_kgkg - w_kgkg)>tolerance && counter_NRM<50;
		 counter_NRM++) {
		// Calculate loading depending on pressure and guess values for
		// temperature
		//
		w_guess_kgkg = adsorption_toth_w_pT(p_Pa, T_guess_K, isotherm_par);

		// Calculate the first derivative of the loading with respect to
		// temperature
		//
		dw_guess_dT_kgkgK = adsorption_toth_dw_dT_pT(p_Pa, T_guess_K,
			isotherm_par);

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
double adsorption_toth_dw_dp_pT(double p_Pa, double T_K,
	double isotherm_par[]) {
	// Calculate temperature-dependent coefficients
	//
	double b = isotherm_par[0] * exp(isotherm_par[4] / T_K);
	double n = isotherm_par[3] + isotherm_par[1] / T_K;
	double r = isotherm_par[5]<0 ? n : isotherm_par[5];

	// Calculate derivative of loading with respect to pressure
	//
	return isotherm_par[6] * pow(b, isotherm_par[2]) *
		pow(1 + pow(b, r) * pow(p_Pa, n), -1/n - 1);
}


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
double adsorption_toth_dw_dT_pT(double p_Pa, double T_K,
	double isotherm_par[]) {
	// Calculate temperature-dependent coefficients
	//
	double b = isotherm_par[0] * exp(isotherm_par[4] / T_K);
	double n = isotherm_par[3] + isotherm_par[1] / T_K;
	double r = isotherm_par[5]<0 ? n : isotherm_par[5];

	double db_dT = -isotherm_par[4] / pow(T_K, 2) * b;
	double dn_dT = -isotherm_par[1] / pow(T_K, 2);

	// Calculate derivative of loading with respect to temperature:
	//
	double aux1 = 1 + pow(b, r) * pow(p_Pa, n);
	double aux2 = 1/n;
	double aux3 = isotherm_par[6] * pow(b, isotherm_par[2]) * p_Pa;
	double aux4 = pow(aux1, aux2);

	double daux1_dT = 1;

	if (isotherm_par[5]<0) {
		// Coefficient b of denominator has same exponent as pressure p_Pa
		//
		double aux5 = b * p_Pa;
		double daux5_dT = p_Pa * db_dT;

		daux1_dT = pow(aux5, n) * (dn_dT * log(aux5) + n * daux5_dT / aux5);

	} else {
		// Coefficient b of denominator has own exponent r
		//
		double aux5 = pow(b, r);
		double aux6 = pow(p_Pa, n);
		double daux5_dT = r * pow(b, r - 1) * db_dT;
		double daux6_dT = pow(p_Pa, n) * log(p_Pa) * dn_dT;

		daux1_dT = daux5_dT * aux6 + aux5 * daux6_dT;
	}

	double daux2_dT = -1/pow(n, 2) * dn_dT;
	double daux3_dT = isotherm_par[6] * p_Pa * isotherm_par[2] *
		pow(b, isotherm_par[2] - 1) * db_dT;
	double daux4_dT = aux4 * (daux2_dT * log(aux1) + aux2 * daux1_dT / aux1);

	return (daux3_dT * aux4 - aux3 * daux4_dT) / pow(aux4, 2);
}


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
	double isotherm_par[]) {
	// Calculate temperature-dependent coefficients
	//
	double b = isotherm_par[0] * exp(isotherm_par[4] / T_K);
	double n = isotherm_par[3] + isotherm_par[1] / T_K;
	double r = isotherm_par[5]<0 ? n : isotherm_par[5];

	// Calculate auxiliary variables to short expressions of equation
	//
	double aux1 = pow(b, isotherm_par[2] * n) * pow(isotherm_par[6], n);
	double aux2 = pow(b, r) * pow(w_kgkg, n);

	// Calculate derivative of pressure wrt loading
	//
	return -aux1 / (pow(aux1 - aux2, 1 / n) * (aux2 - aux1));


}


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
	double isotherm_par[]) {
	// Calculate temperature-dependent coefficients
	//
	double b = isotherm_par[0] * exp(isotherm_par[4] / T_K);
	double n = isotherm_par[3] + isotherm_par[1] / T_K;
	double r = isotherm_par[5]<0 ? n : isotherm_par[5];

	// Calculate auxiliary variables to short expressions of equation
	//
	double aux1 = pow(b, isotherm_par[2] * n) * pow(isotherm_par[6], n);
	double aux2 = pow(b, r) * pow(w_kgkg, n);

	// Calculate partial derivatives
	//
	double db_dT = -isotherm_par[4] / pow(T_K, 2) * b;
	double dn_dT = -isotherm_par[1] / pow(T_K, 2);
	double dr_dT = isotherm_par[5]<0 ? dn_dT : 0;

	double dp_db = -w_kgkg * (isotherm_par[2] * n * pow(b, isotherm_par[2] *
		n - 1) * pow(isotherm_par[6], n) - r * pow(b, r - 1) *pow(w_kgkg, n)) *
		pow(aux1 - aux2, -1 / n - 1) / n;
	double dp_dn = w_kgkg * (log(aux1 - aux2) / pow(n, 2) - (aux1 * log(b) *
		isotherm_par[2] + aux1 * log(isotherm_par[6]) - aux2 * log(w_kgkg)) /
		((aux1 - aux2) * n)) / pow(aux1 - aux2, 1 / n);
	double dp_dr = pow(b, r) * log(b) * pow(w_kgkg, n + 1) *
		pow(aux1 - aux2, -1 / n -1) / n;


	// Calculate derivative of pressure wrt temperature
	//
	return dp_db * db_dT + dp_dn * dn_dT + dp_dr * dr_dT;
}


/*
 * adsorption_toth_piStar_pyxgTM:
 * ------------------------------
 *
 * Calculates reduced spreading pressure in kg/mol depending on total pressure
 * of vapor phase p_total in Pa, molar fraction of refrigerant in vapor phase
 * y in mol/mol, molar fraction of refrigerant in adsorbed phase in mol/mol,
 * activity coefficient of refrigerant in adsorbed phase, equilibrium
 * temperature T in K and molar mass of refrigerant M in kg/mol. The reduced
 * spreading pressure is defined as follows:
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
 *	Adsorbed Solution (IAST) theory. Implemented solution is solved
 *	numerically; however, an analytical solutions exists for m=1 and r<0 that is
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
	double isotherm_par[]) {
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
			(adsorption_toth_w_pT(p_tmp, T_K, isotherm_par) / p_tmp);
	}

	return piStar_molkg_num;
}
