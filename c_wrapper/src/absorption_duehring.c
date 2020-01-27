///////////////////////////
// absorption_duehring.c //
///////////////////////////
#include <math.h>
#include "absorption_duehring.h"


/////////////////////////////
// Definition of functions //
/////////////////////////////
/*
 * General form of Duehring equation:
 * ----------------------------------
 * 	p = 1/r * exp(C + D/T_ref + E/T_ref^2)
 *
 * Temperature-dependent coefficients:
 * -----------------------------------
 * 	T_ref = (n*T_cor + m - B) / A + q
 *	T_cor = T - 273.15
 *
 *		with: A = a0 + a1*w_cor + a2*w_cor^2 + a3*w_cor^3
 *		----- B = b0 + b1*w_cor + b2*w_cor^2 + b3*w_cor^3
 *			  w_cor = w * 100
 *
 * Possible inputs required by user:
 * ---------------------------------
 *	p: Pressure in Pa
 *	T: Temperature in K
 *	w: Loading in kg/kg
 *
 * Order of coefficients in JSON-file:
 * -----------------------------------
 *	isotherm_par[0]	-> a0	-> in -
 * 	isotherm_par[1] -> a1	-> in -
 * 	isotherm_par[2] -> a2	-> in -
 * 	isotherm_par[3] -> a3	-> in -
 * 	isotherm_par[4] -> b0	-> in -
 * 	isotherm_par[5] -> b1	-> in -
 * 	isotherm_par[6] -> b2	-> in -
 * 	isotherm_par[7] -> b3	-> in -
 * 	isotherm_par[8] -> C	-> in -
 * 	isotherm_par[9] -> D	-> in K
 * 	isotherm_par[10] -> E	-> in K^2
 * 	isotherm_par[11] -> m	-> in -
 * 	isotherm_par[12] -> n	-> in -
 * 	isotherm_par[13] -> q	-> in - 
 * 	isotherm_par[14] -> r	-> in 1/Pa
 *
 */


/*
 * absorption_duehring_w_pT:
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
 *		Array of doubles that contains coefficients of Duehring equation.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium loading in kg/kg.
 *
 * Remarks:
 * --------
 *	Uses Newton-Raphson method for calculating equlibrium loading.
 *
 * History:
 * --------
 *	01/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_duehring_w_pT(double p_Pa, double T_K,
	double isotherm_par[]) {
	// Initialise variables for using Newton-Raphson method
	//
	double w_guess_kgkg = 0.5;
	double p_guess_Pa  = p_Pa;
	double dp_guess_dw_Pakgkg = p_Pa/w_guess_kgkg;

	int counter_NRM = 0;
	const double tolerance = 1e-8;

	// Calculate loading using Newton-Raphson method
	//
	for (p_guess_Pa = absorption_duehring_p_wT(w_guess_kgkg, T_K,
		 isotherm_par);
	     fabs(p_guess_Pa - p_Pa)>tolerance && counter_NRM<50; 
		 counter_NRM++) {
		// Calculate pressure depending on guess value for loading and 
		// temperature
		//
		p_guess_Pa = absorption_duehring_p_wT(w_guess_kgkg, T_K,
			isotherm_par);

		// Calculate derivative of the pressure with respect to loading
		//
		dp_guess_dw_Pakgkg = absorption_duehring_dp_dw_wT(w_guess_kgkg, T_K,
		 isotherm_par);

		// Update guess value for loading
		// Only positive values are allowed
		//
		w_guess_kgkg -= (p_guess_Pa - p_Pa) / dp_guess_dw_Pakgkg;
		w_guess_kgkg = w_guess_kgkg < 0 ? 0.001 : w_guess_kgkg;
	}

	// Return -1 when number of iteratoons exceeds 50
	//
	return (counter_NRM == 50 ? -1 : w_guess_kgkg);

}		

 
/*
 * absorption_duehring_p_wT:
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
 *		Array of doubles that contains coefficients of Duehring equation.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium pressure in Pa.
 *
 * History:
 * --------
 *	01/26/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_duehring_p_wT(double w_kgkg, double T_K,
	double isotherm_par[]) {
	// Calculate loading-dependent coefficients
	//
	double w_cor = w_kgkg * 100;	
	double A = isotherm_par[0] + isotherm_par[1] * w_cor +
		isotherm_par[2] * pow(w_cor, 2) + isotherm_par[3] * pow(w_cor, 3);
	double B = isotherm_par[4] + isotherm_par[5] * w_cor +
		isotherm_par[6] * pow(w_cor, 2) + isotherm_par[7] * pow(w_cor, 3);

	// Calculate temperature-dependent coefficients
	//		
	double T_cor = T_K - 273.15;	
	double T_ref = (isotherm_par[12] * T_cor + isotherm_par[11] - B) / A +
		isotherm_par[13];
	
	// Calculate pressure
	//
	return 1 / isotherm_par[14] * exp(isotherm_par[8] + 
		isotherm_par[9] / T_ref + isotherm_par[10] / pow(T_ref, 2));
}


/*
 * absorption_duehring_T_pw:
 * -------------------------
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
 *		Array of doubles that contains coefficients of Duehring equation.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium temperature in K.
 *
 * Remarks:
 * --------
 *	Uses reduced quadratic equation for calculating equlibrium temperature.
 *
 * History:
 * --------
 *	01/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_duehring_T_pw(double p_Pa, double w_kgkg,
	double isotherm_par[]) {
	// Calculate loading-dependent coefficients
	//
	double w_cor = w_kgkg * 100;	
	double A = isotherm_par[0] + isotherm_par[1] * w_cor +
		isotherm_par[2] * pow(w_cor, 2) + isotherm_par[3] * pow(w_cor, 3);
	double B = isotherm_par[4] + isotherm_par[5] * w_cor +
		isotherm_par[6] * pow(w_cor, 2) + isotherm_par[7] * pow(w_cor, 3);

	// Calculate pressure-dependent coefficients
	//
	double aux1 = log(p_Pa * isotherm_par[14]) - isotherm_par[8];
	double aux2 = isotherm_par[9] / aux1;	
	double T_ref = aux2 / 2 + sqrt(pow(-aux2 / 2, 2) + isotherm_par[10] / aux1);
	
	// Calculate temperature
	//
	return  ((T_ref - isotherm_par[13]) * A + B - isotherm_par[11]) /
		isotherm_par[12] + 273.15;
}


/*
 * absorption_duehring_dw_dp_pT:
 * -----------------------------
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
 *		Array of doubles that contains coefficients of Duehring equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. pressure in kg/kg/Pa.
 *
 * Remarks:
 * --------
 *	Derivative is calculated numerically by the symmetric derivative using
 *  h = 0.001 Pa as small change.
 *
 * History:
 * --------
 *	01/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_duehring_dw_dp_pT(double p_Pa, double T_K,
	double isotherm_par[]) {
	// Calculate loadings
	//
	const double dp_Pa = 0.001;
	
	double w_plus_kgkg = absorption_duehring_w_pT(p_Pa + dp_Pa, T_K,
		isotherm_par);
	double w_minus_kgkg = absorption_duehring_w_pT(p_Pa - dp_Pa, T_K,
		isotherm_par);
	
	// Calculate derivative of loading wrt temperature
	//
	return (w_plus_kgkg - w_minus_kgkg) / (2 * dp_Pa);
}


/*
 * absorption_duehring_dw_dT_pT:
 * -----------------------------
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
 *		Array of doubles that contains coefficients of Duehring equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. temperature in kg/kg/K.
 *
 * Remarks:
 * --------
 *	Derivative is calculated numerically by the symmetric derivative using
 *  h = 0.001 K as small change.
 *
 * History:
 * --------
 *	01/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_duehring_dw_dT_pT(double p_Pa, double T_K,
	double isotherm_par[]) {
	// Calculate loadings
	//
	const double dT_K = 0.001;
	
	double w_plus_kgkg = absorption_duehring_w_pT(p_Pa, T_K + dT_K,
		isotherm_par);
	double w_minus_kgkg = absorption_duehring_w_pT(p_Pa, T_K - dT_K,
		isotherm_par);
	
	// Calculate derivative of loading wrt temperature
	//
	return (w_plus_kgkg - w_minus_kgkg) / (2 * dT_K);
}


/*
 * absorption_duehring_dp_dw_wT:
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
 *		Array of doubles that contains coefficients of Duehring equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. loading in kgPa/kg.
 *
 * History:
 * --------
 *	01/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_duehring_dp_dw_wT(double w_kgkg, double T_K,
	double isotherm_par[]) {
	// Calculate pressure:
	// We do not use function absorption_duehring_p_wT because we need T_ref and
	// A for furhter calculations
	//
	double w_cor = w_kgkg * 100;	
	double A = isotherm_par[0] + isotherm_par[1] * w_cor +
		isotherm_par[2] * pow(w_cor, 2) + isotherm_par[3] * pow(w_cor, 3);
	double B = isotherm_par[4] + isotherm_par[5] * w_cor +
		isotherm_par[6] * pow(w_cor, 2) + isotherm_par[7] * pow(w_cor, 3);
		
	double T_cor = T_K - 273.15;	
	double T_ref = (isotherm_par[12] * T_cor + isotherm_par[11] - B) / A +
		isotherm_par[13];
		
	double p_Pa = 1 / isotherm_par[14] * exp(isotherm_par[8] + 
		isotherm_par[9] / T_ref + isotherm_par[10] / pow(T_ref, 2));

	// Calculate partial derivatives
	//
	double dA_dw_cor = isotherm_par[1] + 2 * isotherm_par[2] * w_cor + 
		3 * isotherm_par[3] * pow(w_cor, 2);
	double dB_dw_cor = isotherm_par[5] + 2 * isotherm_par[6] * w_cor + 
		3 * isotherm_par[7] * pow(w_cor, 2);
	double dw_cor_dw = 100;
	
	double dp_dT_ref = -p_Pa * (isotherm_par[9] * T_ref + 2 * 
		isotherm_par[10]) / pow(T_ref, 3);
	double dT_ref_dw = -dw_cor_dw * (dA_dw_cor * (isotherm_par[12] * T_cor + 
		isotherm_par[11] - B) + A * dB_dw_cor) / pow(A, 2);
	
	// Calculate derivative of pressure wrt loading
	//
	return dp_dT_ref * dT_ref_dw;
}


/*
 * absorption_duehring_dp_dT_wT:
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
 *		Array of doubles that contains coefficients of Duehring equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. temperature in Pa/K.
 *
 * History:
 * --------
 *	01/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_duehring_dp_dT_wT(double w_kgkg, double T_K,
	double isotherm_par[]) {
	// Calculate pressure:
	// We do not use function absorption_duehring_p_wT because we need T_ref and
	// A for furhter calculations
	//
	double w_cor = w_kgkg * 100;	
	double A = isotherm_par[0] + isotherm_par[1] * w_cor +
		isotherm_par[2] * pow(w_cor, 2) + isotherm_par[3] * pow(w_cor, 3);
	double B = isotherm_par[4] + isotherm_par[5] * w_cor +
		isotherm_par[6] * pow(w_cor, 2) + isotherm_par[7] * pow(w_cor, 3);
		
	double T_cor = T_K - 273.15;	
	double T_ref = (isotherm_par[12] * T_cor + isotherm_par[11] - B) / A +
		isotherm_par[13];
		
	double p_Pa = 1 / isotherm_par[14] * exp(isotherm_par[8] + 
		isotherm_par[9] / T_ref + isotherm_par[10] / pow(T_ref, 2));

	// Calculate partial derivatives
	//
	double dp_dT_ref = -p_Pa * (isotherm_par[9] * T_ref + 2 * 
		isotherm_par[10]) / pow(T_ref, 3);
	double dT_ref_dT_cor = isotherm_par[12] / A;
	double dT_cor_dT = 1;
	
	// Calculate derivative of pressure wrt temperature
	//
	return dp_dT_ref * dT_ref_dT_cor * dT_cor_dT;
}
