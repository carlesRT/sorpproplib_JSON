///////////////////////////
// adsorption_langmuir.c //
///////////////////////////
#include <math.h>
#include "adsorption_langmuir.h"


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
 * General form of Langmuir equation:
 * ----------------------------------
 * 	w = [w_s * K * p] / [1 + K * p]
 *
 * Temperature-dependent coefficients:
 * -----------------------------------
 * 	K = K0 * exp(delta_H / (R * T))
 *
 * Possible inputs required by user:
 * ---------------------------------
 *	p: Pressure in Pa
 *	T: Temperature in K
 *	w: Loading in kg/kg
 *
 * Order of coefficients in JSON-file:
 * -----------------------------------
 * 	isotherm_par[1] -> delta_H	-> in J/mol
 * 	isotherm_par[2] -> K0		-> in 1/Pa
 * 	isotherm_par[3] -> w_s		-> in kg/kg
 *
 */


/*
 * adsorption_langmuir_w_pT:
 * -------------------------
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
 *		Array of doubles that contains coefficients of Langmuir equation.
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
 *	11/23/2019, by Mirko Engelpracht:
 *		Updated units of coefficients to SI-units.
 *
 */
double adsorption_langmuir_w_pT(double p_Pa, double T_K,
	double isotherm_par[]) {
	// Calculate temperature-dependent coefficients
	//
	double K = isotherm_par[1] * exp(isotherm_par[0] /
		(IDEAL_GAS_CONSTANT * T_K));

	// Calculate loading
	//
	return isotherm_par[2] * K * p_Pa / (1 + K * p_Pa);
}


/*
 * adsorption_langmuir_p_wT:
 * -------------------------
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
 *		Array of doubles that contains coefficients of Langmuir equation.
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
 *	11/23/2019, by Mirko Engelpracht:
 *		Updated units of coefficients to SI-units.
 *
 */
double adsorption_langmuir_p_wT(double w_kgkg, double T_K,
	double isotherm_par[]) {
	// Calculate temperature-dependent coefficients
	//
	double K = isotherm_par[1] * exp(isotherm_par[0] /
		(IDEAL_GAS_CONSTANT * T_K));

	// Calculate pressure
	//
	return w_kgkg / ((isotherm_par[2] - w_kgkg) * K);
}


/*
 * adsorption_langmuir_T_pw:
 * -----------------------
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
 *		Array of doubles that contains coefficients of Langmuir equation.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium temperature in K.
 *
 * History:
 * --------
 *	10/20/2019, by Mirko Engelpracht:
 *		First implementation.
 *	11/23/2019, by Mirko Engelpracht:
 *		Updated units of coefficients to SI-units.
 *
 */
double adsorption_langmuir_T_pw(double p_Pa, double w_kgkg,
	double isotherm_par[]) {
	// Calculate temperature-dependent coefficients from Langmuir equation
	//
	double K = w_kgkg / ((isotherm_par[2] - w_kgkg) * p_Pa);

	// Calculate temperature
	//
	return isotherm_par[0] / (IDEAL_GAS_CONSTANT * log(K / isotherm_par[1]));
}


/*
 * adsorption_langmuir_dw_dp_pT:
 * -----------------------------
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
 *		Array of doubles that contains coefficients of Langmuir equation.
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
double adsorption_langmuir_dw_dp_pT(double p_Pa, double T_K,
	double isotherm_par[]) {
	// Calculate temperature-dependent coefficients
	//
	double K = isotherm_par[1] * exp(isotherm_par[0] /
		(IDEAL_GAS_CONSTANT * T_K));

	// Calculate derivative of loading with respect to pressure
	//
	return isotherm_par[2] * K / pow(1 + K * p_Pa, 2);
}


/*
 * adsorption_langmuir_dw_dT_pT:
 * -----------------------------
 *
 * Calculates derivative of equilibrium loading w with respect to temperature
 * T in kg/kg/K depending on equilibrium pressure p in Pa and equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Langmuir equation.
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
 *	11/23/2019, by Mirko Engelpracht:
 *		Updated units of coefficients to SI-units.
 *
 */
double adsorption_langmuir_dw_dT_pT(double p_Pa, double T_K,
	double isotherm_par[]) {
	// Calculate temperature-dependent coefficients
	//
	double K = isotherm_par[1] * exp(isotherm_par[0] / (IDEAL_GAS_CONSTANT *
		T_K));
	double dK_dT = -isotherm_par[0] / (IDEAL_GAS_CONSTANT * pow(T_K, 2)) * K;

	// Calculate derivative of loading with respect to temperature
	//
	double dw_dK = isotherm_par[2] * p_Pa / pow(1 + K * p_Pa, 2);

	return dw_dK * dK_dT;
}


/*
 * adsorption_langmuir_dp_dw_wT:
 * -----------------------------
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
 *		Array of doubles that contains coefficients of Langmuir equation.
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
double adsorption_langmuir_dp_dw_wT(double w_kgkg, double T_K,
	double isotherm_par[]) {
	// Calculate temperature-dependent coefficients
	//
	double K = isotherm_par[1] * exp(isotherm_par[0] / (IDEAL_GAS_CONSTANT *
		T_K));

	// Calculate derivative of pressure wrt loading
	//
	return isotherm_par[2] / (K * pow(w_kgkg - isotherm_par[2], 2));
}


/*
 * adsorption_langmuir_dp_dT_wT:
 * -----------------------------
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
 *		Array of doubles that contains coefficients of Langmuir equation.
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
double adsorption_langmuir_dp_dT_wT(double w_kgkg, double T_K,
	double isotherm_par[]) {
	// Calculate temperature-dependent coefficients
	//
	double K = isotherm_par[1] * exp(isotherm_par[0] / (IDEAL_GAS_CONSTANT *
		T_K));
	double dK_dT = -isotherm_par[0] / (IDEAL_GAS_CONSTANT * pow(T_K, 2)) * K;

	// Calculate derivative of pressure wrt temperature
	//
	return -w_kgkg / ((isotherm_par[2] - w_kgkg) * pow(K, 2)) * dK_dT;
}


/*
 * adsorption_langmuir_piStar_pyxgTM:
 * ----------------------------------
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
 *		Array of doubles that contains coefficients of Langmuir equation.
 *
 * Returns:
 * --------
 *	double:
 *		Reduced spreading pressure in kg/mol.
 *
 * Remarks:
 * --------
 *	Reduced spreading pressure is needed to solve, for example, the Ideal
 *	Adsorbed Solution (IAST) theory. However, implemented solution is only
 *	valid for m=1 and r=n.
 *
 * History:
 * --------
 *	01/11/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_langmuir_piStar_pyxgTM(double p_total_Pa, double y_molmol,
	double x_molmol, double gamma, double T_K, double M_kgmol,
	double isotherm_par[]) {
	// Calculate temperature-dependent coefficients
	//
	double K = isotherm_par[1] * exp(isotherm_par[0] / (IDEAL_GAS_CONSTANT *
		T_K));

	// Calculate analytical solution of reduced spreading pressure
	//
	return isotherm_par[2] / M_kgmol * log(1 + K * p_total_Pa * y_molmol /
		x_molmol / gamma);
}
