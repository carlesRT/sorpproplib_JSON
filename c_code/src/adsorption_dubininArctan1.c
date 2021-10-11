/////////////////////////////////
// adsorption_dubininArctan1.c //
/////////////////////////////////
#include <math.h>
#include "adsorption_dubininArctan1.h"


/////////////////////////////
// Definition of constants //
/////////////////////////////
#ifndef IDEAL_GAS_CONSTANT
#define IDEAL_GAS_CONSTANT 8.314462618f
#endif

#ifndef PI
#define PI 3.14159265359f
#endif


/////////////////////////////
// Definition of functions //
/////////////////////////////
/*
 * General form of Dubinin-Astakhov equation:
 * ------------------------------------------
 * 	w = W * rho_l(T)
 *
 * Temperature-dependent coefficients:
 * -----------------------------------
 * 	W = a/PI * [arctan((A - b) / c) + PI/2] + d
 *	A = R * T * ln(p_sat(T)/p)
 *
 * Possible inputs required by user:
 * ---------------------------------
 *	p: Pressure in Pa
 *	T: Temperature in K
 *	w: Loading in kg/kg
 *
 *	p_sat(T): Saturation pressure in Pa
 *	rho_l(T): Density of adsorpt in kg/m³
 *	dp_sat_dT(T): Derivative of vapor pressure wrt. temperature in Pa/K
 *	drho_l_dT(T): Derivative of saturated liquid density of adsorpt wrt.
 *		temperature in kg/m³/K
 *
 * Order of coefficients in JSON-file:
 * -----------------------------------
 *	isotherm_par[0]	-> a	-> in m³/kg
 * 	isotherm_par[1] -> b	-> in J/mol
 * 	isotherm_par[2] -> c	-> in J/mol
 * 	isotherm_par[3] -> d	-> in m³/kg
 *
 */


/*
 * adsorption_dubininArctan1_W_ARho:
 * ---------------------------------
 *
 * Calculates equilibrium volumetric loading W in m³/kg depending on equilibrium
 * adsorption potential A in J/mol and saturated liquid density of adsorpt rho_l
 * in kg/m3.
 *
 * Parameters:
 * -----------
 * 	double A_Jmol:
 *		Equilibrium adsorption potential A in J/mol.
 * 	double rho_l_kgm3:
 *		Saturated liquid density of adsorpt rho_l in kg/m3.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Dubinin-Arctan1
 *		equation.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium volumetric loading W in m³/kg.
 *
 * History:
 * --------
 *	06/04/2021, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_dubininArctan1_W_ARho(double A_Jmol, double rho_l_kgm3,
	double isotherm_par[]) {
	// Calculate volumetric loading
	//
	double W = isotherm_par[0]/PI * (atan((A_Jmol - isotherm_par[1]) /
		isotherm_par[2]) + PI/2) + isotherm_par[3];

	// Avoid compiler warning C4100 by adding "do nothing" expression
	// "Do nothing" expression will be optimized out by compiler
	//
	(rho_l_kgm3) = (rho_l_kgm3);

	return (W);
}


/*
 * adsorption_dubininArctan1_A_WRho:
 * ---------------------------------
 *
 * Calculates equilibrium adsorption potential A in J/mol depending on
 * equilibrium volumetric loading W in m³/kg and saturated liquid density of
 * adsorpt rho_l in kg/m3.
 *
 * Parameters:
 * -----------
 * 	double W_m3kg:
 *		Equilibrium volumetric loading W in m³/kg.
 * 	double rho_l_kgm3:
 *		Saturated liquid density of adsorpt rho_l in kg/m3.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Dubinin-Arctan1
 *		equation.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium adsorption potential A in J/mol.
 *
 * History:
 * --------
 *	06/04/2021, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_dubininArctan1_A_WRho(double W_m3kg, double rho_l_kgm3,
	double isotherm_par[]) {
	// Check unit of volumetric loading
	//
	double A = isotherm_par[1] + isotherm_par[2] * tan(PI / isotherm_par[0] *
		(W_m3kg - isotherm_par[3]) - PI/2);

	// Avoid compiler warning C4100 by adding "do nothing" expression
	// "Do nothing" expression will be optimized out by compiler
	//
	(rho_l_kgm3) = (rho_l_kgm3);

	// Calculate adsorption potential
	//
	return (A);
}


/*
 * adsorption_dubininArctan1_w_pTpsatRho:
 * --------------------------------------
 *
 * Calculates equilibrium loading w in kg/kg depending on equilibrium pressure
 * p in Pa, equilibrium temperature T in K, saturation pressure p_Sat in Pa and
 * saturated liquid density of adsorpt rho_l in kg/m³.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_l_kgm3:
 *		Saturated liquid density of adsorpt in kg/m³.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Dubinin-Arctan1
 *		equation.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium loading in kg/kg.
 *
 * History:
 * --------
 *	06/04/2021, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_dubininArctan1_w_pTpsatRho(double p_Pa, double T_K,
	double p_sat_Pa, double rho_l_kgm3, double isotherm_par[]) {
	// Calculate temperature-dependent coefficients
	//
	double A_Jmol = IDEAL_GAS_CONSTANT * T_K * log(p_sat_Pa / p_Pa);
	double W_m3kg = adsorption_dubininArctan1_W_ARho(A_Jmol, rho_l_kgm3,
		isotherm_par);

	// Calculate loading
	//
	return (W_m3kg*rho_l_kgm3);
}


/*
 * adsorption_dubininArctan1_p_wTpsatRho:
 * --------------------------------------
 *
 * Calculates equilibrium pressure p in Pa depending on equilibrium loading w in
 * kg/kg, equilibrium temperature T in K, saturation pressure p_Sat in Pa and
 * saturated liquid density of adsorpt rho_l in kg/m³.
 *
 * Parameters:
 * -----------
 * 	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_l_kgm3:
 *		Saturated liquid density of adsorpt in kg/m³.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Dubinin-Arctan1
 *		equation.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure in Pa.
 *
 * History:
 * --------
 *	06/04/2021, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_dubininArctan1_p_wTpsatRho(double w_kgkg, double T_K,
	double p_sat_Pa, double rho_l_kgm3, double isotherm_par[]) {
	// Calculate temperature-dependent coefficients
	//
	double W_m3kg = w_kgkg/ rho_l_kgm3;
	double A_Jmol = adsorption_dubininArctan1_A_WRho(W_m3kg, rho_l_kgm3,
		isotherm_par);

	// Calculate pressure
	//
	return (p_sat_Pa / exp(A_Jmol / (IDEAL_GAS_CONSTANT * T_K)));
}


/*
 * adsorption_dubininArctan1_T_pwpsatRho:
 * --------------------------------------
 *
 * Calculates equilibrium temperature T in K depending on equilibrium pressure
 * in Pa, equilibrium loading w in kg/kg and function pointers for saturation
 * pressure p_Sat in Pa, saturated liquid density of adsorpt rho_l in kg/m³,
 * derivative of vapor pressure wrt. temperature and derivative of saturated
 * liquid density of adsorpt wrt. temperature.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure in Pa.
 * 	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double (*p_sat_T_PaK)(double T_K, double p_sat_par[], void *refrigerant):
 *		Function pointer for vapor pressure wrt. temperature.
 *	double (*rho_l_T_kgm3K)(double T_K, double rho_l_par[]):
 *		Function pointer for saturated liquid density of adsorpt.
 *	double (*dp_sat_dT_PaK)(double T_K, double p_sat_par[], void *refrigerant):
 *		Function pointer for derivative of vapor pressure wrt. temperature.
 *	double (*drho_l_dT_kgm3K)(double T_K, double rho_l_par[]):
 *		Function pointer for derivative of saturated liquid density of adsorpt
 *		wrt. temperature.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Dubinin-Arctan1
 *		equation.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients vapor pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients for saturated liquid
 *		density of adsorpt.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium temperature in K.
 *
 * Remarks:
 * --------
 *	No error handling because pointers are checked at highest level (i.e.,
 *	functions for workingPair-Struct).
 *
 * History:
 * --------
 *	06/04/2021, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_dubininArctan1_T_pwpsatRho(double p_Pa, double w_kgkg,
	double (*p_sat_T_Pa)(double T_K, double p_sat_par[], void *refrigerant),
	double (*rho_l_T_kgm3)(double T_K, double rho_l_par[]),
	double (*dp_sat_dT_PaK)(double T_K, double p_sat_par[], void *refrigerant),
	double (*drho_l_dT_kgm3K)(double T_K, double rho_l_par[]),
	double isotherm_par[], double p_sat_par[], double rho_l_par[],
	void *refrigerant) {
	// Initialize variables for using Newton-Raphson method
	//
	double T_guess_K = 313.15;
	double w_guess_kgkg = w_kgkg;
	double dw_guess_dT_kgkgK = w_kgkg/T_guess_K;

	int counter_NRM = 0;
	const double tolerance = 1e-8;

	// Calculate temperature using Newton-Raphson method
	//
	for (w_guess_kgkg = adsorption_dubininArctan1_w_pTpsatRho(p_Pa, T_guess_K,
			p_sat_T_Pa(T_guess_K, p_sat_par, refrigerant),
			rho_l_T_kgm3(T_guess_K, rho_l_par), isotherm_par);
	     fabs(w_guess_kgkg - w_kgkg)>tolerance && counter_NRM<50;
		 counter_NRM++) {
		// Calculate loading depending on pressure and guess values for
		// temperature
		//
		w_guess_kgkg = adsorption_dubininArctan1_w_pTpsatRho(p_Pa, T_guess_K,
			p_sat_T_Pa(T_guess_K, p_sat_par, refrigerant),
			rho_l_T_kgm3(T_guess_K, rho_l_par), isotherm_par);

		// Calculate the first derivative of the loading with respect to
		// temperature
		//
		dw_guess_dT_kgkgK = adsorption_dubininArctan1_dw_dT_pTpsatRho(p_Pa,
			T_guess_K,
			p_sat_T_Pa(T_guess_K, p_sat_par, refrigerant),
			rho_l_T_kgm3(T_guess_K, rho_l_par),
			dp_sat_dT_PaK(T_guess_K, p_sat_par, refrigerant),
			drho_l_dT_kgm3K(T_guess_K, rho_l_par), isotherm_par);

		// Update guess value for temperature
		// Negative values are not allowed
		//
		T_guess_K -= (w_guess_kgkg - w_kgkg) / dw_guess_dT_kgkgK;
		T_guess_K = T_guess_K < 0 ? 323.15 : T_guess_K;

	}

	return (counter_NRM == 50 ? -1 : T_guess_K);
}


/*
 * adsorption_dubininArctan1_dW_dA_ARho:
 * -------------------------------------
 *
 * Calculates derivative of equilibrium volumetric loading dW_dA in m³mol/kg/J
 * depending on equilibrium adsorption potential A in J/mol and saturated liquid
 * density of adsorpt in kg/m3.
 *
 * Parameters:
 * -----------
 * 	double A_Jmol:
 *		Equilibrium adsorption potential A in mol/J.
 * 	double rho_l_kgm3:
 *		Saturated liquid density of adsorpt rho_l in kg/m3.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Dubinin-Arctan1
 *		equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of volumetric loading wrt. adsorption potential in
 *		m³mol/kg/J.
 *
 * History:
 * --------
 *	06/04/2021, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_dubininArctan1_dW_dA_ARho(double A_Jmol, double rho_l_kgm3,
	double isotherm_par[]) {
	// Calculate volumetric loading
	//
	double dW_dA_m3molkgJ = isotherm_par[0] / (PI *  isotherm_par[2] * (1 +
		pow((A_Jmol - isotherm_par[1]) / isotherm_par[2], 2)));

	// Avoid compiler warning C4100 by adding "do nothing" expression
	// "Do nothing" expression will be optimized out by compiler
	//
	(rho_l_kgm3) = (rho_l_kgm3);

	// Calculate derivative of volumetric loading wrt. adsorption potential
	//
	return (dW_dA_m3molkgJ);
}


/*
 * adsorption_dubininArctan1_dA_dW_WRho:
 * -------------------------------------
 *
 * Calculates derivative of equilibrium adsorption potential dA_dW in kgJ/mol/m³
 * depending on equilibrium volumetric loading W in m³/kg and saturated liquid
 * density of adsorpt rho_l in kg/m3.
 *
 * Parameters:
 * -----------
 * 	double W_m3kg:
 *		Equilibrium volumetric loading W in m³/kg.
 * 	double rho_l_kgm3:
 *		Saturated liquid density of adsorpt rho_l in kg/m3.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Dubinin-Arctan1
 *		equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of adsorption potential wrt. volumetric loading in
 *		kgJ/mol/m³.
 *
 * History:
 * --------
 *	06/04/2021, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_dubininArctan1_dA_dW_WRho(double W_m3kg, double rho_l_kgm3,
	double isotherm_par[]) {
	// Calculate derivative of adsorption potential wrt. volumetric loading
	//
	double dA_dW_Jkgmolm3 = isotherm_par[2] * PI / (isotherm_par[0] *
		pow(cos((W_m3kg - isotherm_par[3]) / isotherm_par[0] * PI -
		PI / 2), 2));

	// Avoid compiler warning C4100 by adding "do nothing" expression
	// "Do nothing" expression will be optimized out by compiler
	//
	(rho_l_kgm3) = (rho_l_kgm3);

	return (dA_dW_Jkgmolm3);
}


/*
 * adsorption_dubininArctan1_dw_dp_pTpsatRho:
 * ------------------------------------------
 *
 * Calculates derivative of equilibrium loading dw_dp with respect to pressure
 * in kg/kg/Pa depending on equilibrium pressure p in Pa, equilibrium
 * temperature T in K, saturation pressure p_Sat in Pa and saturated liquid
 * density of adsorpt rho_l in kg/m³.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_l_kgm3:
 *		Saturated liquid density of adsorpt in kg/m³.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Dubinin-Arctan1
 *		equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. pressure in kg/kg/Pa.
 *
 * History:
 * --------
 *	06/04/2021, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_dubininArctan1_dw_dp_pTpsatRho(double p_Pa, double T_K,
	double p_sat_Pa, double rho_l_kgm3, double isotherm_par[]) {
	// Calculate temperature-dependent coefficients
	//
	double A_Jmol = IDEAL_GAS_CONSTANT * T_K * log(p_sat_Pa / p_Pa);

	// Calculate derivatives of temperature-dependent coefficients
	//
	double dW_dA_m3molkgJ = adsorption_dubininArctan1_dW_dA_ARho(A_Jmol,
		rho_l_kgm3, isotherm_par);
	double dA_dp_JmolPa = -IDEAL_GAS_CONSTANT * T_K / p_Pa;

	// Calculate derivative of loading wrt. pressure
	//
	return (rho_l_kgm3 * dW_dA_m3molkgJ * dA_dp_JmolPa);
}


/*
 * adsorption_dubininArctan1_dw_dT_pTpsatRho:
 * ------------------------------------------
 *
 * Calculates derivative of equilibrium loading dw_dp with respect to
 * temperature in kg/kg/K depending on equilibrium pressure p in Pa, equilibrium
 * temperature T in K, saturation pressure p_Sat in Pa, saturated liquid density
 * of adsorpt rho_l in kg/m³, derivative of saturation pressure with respect to
 * temperature dp_sat_dT in Pa/K and derivative of saturated liquid density of
 * adsorpt with respect to temperature drho_l_dT in kg/m³/K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_l_kgm3:
 *		Saturated liquid density of adsorpt in kg/m³.
 * 	double dp_sat_dT_PaK:
 *		Derivative of saturation pressure wrt. temperature in Pa/K.
 *	double drho_l_dT_kgm3K:
 *		Derivative of saturated liquid density of adsorpt wrt. temperature in
 *		kg/m³/K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Dubinin-Arctan1
 *		equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. temperature in kg/kg/K.
 *
 * History:
 * --------
 *	06/04/2021, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_dubininArctan1_dw_dT_pTpsatRho(double p_Pa, double T_K,
	double p_sat_Pa, double rho_l_kgm3, double dp_sat_dT_PaK,
	double drho_l_dT_kgm3K, double isotherm_par[]) {
	// Calculate temperature-dependent coefficients
	//
	double A_Jmol = IDEAL_GAS_CONSTANT * T_K * log(p_sat_Pa / p_Pa);
	double W_m3kg = adsorption_dubininArctan1_W_ARho(A_Jmol, rho_l_kgm3,
		isotherm_par);

	// Calculate derivatives of temperature-dependent coefficients
	//
	double dW_dA_m3molkgJ = adsorption_dubininArctan1_dW_dA_ARho(A_Jmol,
		rho_l_kgm3, isotherm_par);
	double dA_dT_JmolK = IDEAL_GAS_CONSTANT * (log(p_sat_Pa / p_Pa) +
		T_K / p_sat_Pa * dp_sat_dT_PaK);

	// Calculate derivative of loading wrt. temperature depending on flag
	//
	return (W_m3kg * drho_l_dT_kgm3K +
		rho_l_kgm3 * dW_dA_m3molkgJ * dA_dT_JmolK);
}


/*
 * adsorption_dubininArctan1_dp_dw_wTpsatRho:
 * ------------------------------------------
 *
 * Calculates derivative of equilibrium pressure p with respect to loading
 * w in kgPa/kg depending on equilibrium loading w in kg/kg, equilibrium
 * temperature T in K, saturation pressure p_Sat in Pa and saturated liquid
 * density of adsorpt rho_l in kg/m³.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double rho_l_kgm3:
 *		Saturated liquid density of adsorpt in kg/m³.
 * 	double dp_sat_dT_PaK:
 *		Derivative of saturation pressure wrt. temperature in Pa/K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Dubinin-Arctan1
 *		equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. loading in kgPa/kg.
 *
 * History:
 * --------
 *	06/04/2021, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_dubininArctan1_dp_dw_wTpsatRho(double w_kgkg, double T_K,
	double p_sat_Pa, double rho_l_kgm3, double isotherm_par[]) {
	// Calculate temperature-dependent coefficients
	//
	double p_Pa = adsorption_dubininArctan1_p_wTpsatRho(w_kgkg, T_K, p_sat_Pa,
		rho_l_kgm3, isotherm_par);
	double W_m3kg = w_kgkg / rho_l_kgm3;

	// Calculate partial derivatives
	//
	double dp_dA_PamolJ = -1 / (IDEAL_GAS_CONSTANT * T_K) * p_Pa;
	double dA_dW_Jkgmolm3 = adsorption_dubininArctan1_dA_dW_WRho(W_m3kg,
		rho_l_kgm3, isotherm_par);
	double dW_dw_m3kg = 1 / rho_l_kgm3;

	// Calculate derivative of pressure wrt loading
	//
	return (dp_dA_PamolJ * dA_dW_Jkgmolm3 * dW_dw_m3kg);
}


/*
 * adsorption_dubininArctan1_dp_dT_wTpsatRho:
 * ------------------------------------------
 *
 * Calculates derivative of equilibrium pressure p with respect to temperature
 * w in Pa/K depending on equilibrium loading w in kg/kg and equilibrium
 * temperature T in K, saturation pressure p_Sat in Pa, saturated liquid density
 * of adsorpt rho_l in kg/m³, derivative of saturation pressure with respect to
 * temperature dp_sat_dT in Pa/K and derivative of saturated liquid density of
 * adsorpt with respect to temperature drho_l_dT in kg/m³/K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_l_kgm3:
 *		Saturated liquid density of adsorpt in kg/m³.
 * 	double dp_sat_dT_PaK:
 *		Derivative of saturation pressure wrt. temperature in Pa/K.
 *	double drho_l_dT_kgm3K:
 *		Derivative of saturated liquid density of adsorpt wrt. temperature in
 *		kg/m³/K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Dubinin-Arctan1
 *		equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. temperature in Pa/K.
 *
 * History:
 * --------
 *	06/04/2021, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_dubininArctan1_dp_dT_wTpsatRho(double w_kgkg, double T_K,
	double p_sat_Pa, double rho_l_kgm3, double dp_sat_dT_PaK,
	double drho_l_dT_kgm3K, double isotherm_par[]) {
	// Calculate temperature-dependent coefficients
	//
	double W_m3kg = w_kgkg / rho_l_kgm3;
	double A_Jmol = adsorption_dubininArctan1_A_WRho(W_m3kg, rho_l_kgm3,
		isotherm_par);
	double p_Pa = adsorption_dubininArctan1_p_wTpsatRho(w_kgkg, T_K, p_sat_Pa,
		rho_l_kgm3, isotherm_par);

	// Calculate partial derivatives
	//
	double dp_dT_PaK = A_Jmol / (IDEAL_GAS_CONSTANT * pow(T_K, 2)) * p_Pa;
	double dp_dp_sat = 1 / exp(A_Jmol / (IDEAL_GAS_CONSTANT * T_K));
	double dp_dA_PamolJ = -1 / (IDEAL_GAS_CONSTANT * T_K) * p_Pa;

	double dA_dW_Jkgmolm3 = adsorption_dubininArctan1_dA_dW_WRho(W_m3kg,
		rho_l_kgm3, isotherm_par);
	double dW_drho_l_m6kg2 = -w_kgkg / pow(rho_l_kgm3, 2);

	// Calculate derivative of pressure wrt loading
	//
	return (dp_dT_PaK + dp_dp_sat * dp_sat_dT_PaK +
		dp_dA_PamolJ * dA_dW_Jkgmolm3 * dW_drho_l_m6kg2 * drho_l_dT_kgm3K);
}


/*
 * adsorption_dubininArctan1_piStar_pyxgTpsatRhoM:
 * -----------------------------------------------
 *
 * Calculates reduced spreading pressure in kg/mol depending on total pressure
 * of vapor phase p_total in Pa, molar fraction of refrigerant in vapor phase
 * y in mol/mol, molar fraction of refrigerant in adsorbed phase in mol/mol,
 * activity coefficient of refrigerant in adsorbed phase, equilibrium
 * temperature T in K, molar mass of refrigerant M in kg/mol, saturation
 * pressure p_Sat in  Pa, saturated liquid density of adsorpt rho_l in kg/m³.
 * The reduced spreading pressure is defined as follows:
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
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_l_kgm3:
 *		Saturated liquid density of adsorpt in kg/m³.
 *	double M_kgmol:
 *		Molar mass of refrigerant M in kg/mol.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Dubinin-Arctan1
 *		equation.
 *
 * Returns:
 * --------
 *	double:
 *		Reduced spreading pressure in kg/mol.
 *
 * Remarks:
 * --------
 *	Reduced spreading pressure is needed to solve, for example, the Ideal
 *	Adsorbed Solution Theory (IAST). Currently, integral is solved numerically;
 *	however, an analytical solution of the integral exists that is based on
 *	the upper incomplete gamma function.
 *
 * History:
 * --------
 *	06/04/2021, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_dubininArctan1_piStar_pyxgTpsatRhoM(double p_total_Pa,
	double y_molmol, double x_molmol, double gamma, double T_K,
	double p_sat_Pa, double rho_l_kgm3, double M_kgmol, double isotherm_par[]) {
	// Calculate numerically reduced spreading pressure
	//
	int n_counter;
	int n_steps = 100000000;
	double h = ((p_total_Pa * y_molmol / x_molmol / gamma) - 0) / n_steps;
	double p_tmp;
	double piStar_molkg_num = 0;

	for(n_counter=1; n_counter<n_steps; n_counter++) {
		// Calculate reduced spreading pressure by numerical integration
		// using the trapezoidal rule
		//
		p_tmp = 0 - h/2 + n_counter * h;
		piStar_molkg_num += (h / M_kgmol) *
			(adsorption_dubininArctan1_w_pTpsatRho(p_tmp, T_K, p_sat_Pa,
			 rho_l_kgm3, isotherm_par) / p_tmp);
	}

	return (piStar_molkg_num);
}
