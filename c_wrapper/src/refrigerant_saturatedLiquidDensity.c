//////////////////////////////////////////
// refrigerant_saturatedLiquidDensity.c //
//////////////////////////////////////////
#include <math.h>
#include "refrigerant_saturatedLiquidDensity.h"


///////////////////////////////////////
// Definition of function prototypes //
///////////////////////////////////////
/*
 * General form of equation for saturated liquid density:
 * ------------------------------------------------------
 * 	X = ln(rho_l / rho_crit) if flag<0 else rho_l / rho_crit
 *	X = [fac1*THETA^exp1 + fac2*THETA^exp2 + fac3*THETA^exp3 +
 *		 fac4*THETA^exp4 + fac5*THETA^exp5 + fac6*THETA^exp6 +
 *		 fac7*THETA^exp7]
 *
 * Temperature-dependent coefficients:
 * -----------------------------------
 * 	theta = T/T_crit
 * 	THETA = 1 - T/T_crit
 *
 * Inputs required by user:
 * ------------------------
 *	T: Temperature in K
 *
 * Order of coefficients in JSON-file:
 * -----------------------------------
 *	isotherm_par[0]		-> Tcrit	-> in K
 *	isotherm_par[1]		-> rhocrit	-> in kg/m3
 *	isotherm_par[2]		-> flag		-> in -
 * 	isotherm_par[3] 	-> fac1		-> in -
 * 	isotherm_par[4] 	-> exp1		-> in -
 * 	isotherm_par[5] 	-> fac2		-> in -
 * 	isotherm_par[6] 	-> exp2		-> in -
 * 	isotherm_par[7] 	-> fac3		-> in -
 * 	isotherm_par[8] 	-> exp3		-> in -
 * 	isotherm_par[9] 	-> fac4		-> in -
 * 	isotherm_par[10] 	-> exp4		-> in -
 * 	isotherm_par[11] 	-> fac5		-> in -
 * 	isotherm_par[12] 	-> exp5		-> in -
 * 	isotherm_par[13] 	-> fac6		-> in -
 * 	isotherm_par[14] 	-> exp6		-> in -
 * 	isotherm_par[15] 	-> fac7		-> in -
 * 	isotherm_par[16] 	-> exp7		-> in -
 *
 */
 

/*
 * refrigerant_rho_l:
 * ------------------
 *
 * Calculates saturated liquid density in kg/m³ depending on saturation 
 * temperature T in K.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Saturation temperature in K.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for equation of saturated
 *		liquid density.
 *
 * Returns:
 * --------
 *	double:
 *		Saturated liquid density in kg/m³.
 *
 * History:
 * --------
 *	01/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_rho_l(double T_K,
	double refrigerant_par[]) {
	// Calculate temperature-dependent coefficients
	//
	double theta = T_K/refrigerant_par[0];
	double THETA = 1 - theta;
	
	double X = refrigerant_par[3] * pow(THETA, refrigerant_par[4]) + 
		refrigerant_par[5] * pow(THETA, refrigerant_par[6]) + 
		refrigerant_par[7] * pow(THETA, refrigerant_par[8]) + 
		refrigerant_par[9] * pow(THETA, refrigerant_par[10]) + 
		refrigerant_par[11] * pow(THETA, refrigerant_par[12]) + 
		refrigerant_par[13] * pow(THETA, refrigerant_par[14]) + 
		refrigerant_par[15] * pow(THETA, refrigerant_par[16]);
 
	// Calculate vapour pressure
	//
	return (refrigerant_par[2] < 0 ? exp(X)*refrigerant_par[1] :
		X*refrigerant_par[1]);
}


/*
 * refrigerant_drho_l_dT:
 * ----------------------
 *
 * Calculates derivative of saturated liquid density wrt. temperature in kg/m³/K
 * depending on saturation temperature T in K.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Saturation temperature in K.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for equation of saturated
 *		liquid density.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of saturated liquid density wrt. temperature in kg/m³/K.
 *
 * History:
 * --------
 *	01/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_drho_l_dT(double T_K, double refrigerant_par[]) {
	// Calculate temperature-dependent coefficients
	//
	double theta = T_K/refrigerant_par[0];
	double THETA = 1 - theta;
	
	double dX_dTHETA = refrigerant_par[4] * refrigerant_par[3] * 
			pow(THETA, refrigerant_par[4] - 1) + 
		refrigerant_par[6] * refrigerant_par[5] * 
			pow(THETA, refrigerant_par[6] - 1) + 
		refrigerant_par[8] * refrigerant_par[7] * 
			pow(THETA, refrigerant_par[8] - 1) + 
		refrigerant_par[10] * refrigerant_par[9] * 
			pow(THETA, refrigerant_par[10] - 1) + 
		refrigerant_par[12] * refrigerant_par[11] * 
			pow(THETA, refrigerant_par[12] - 1) + 
		refrigerant_par[14] * refrigerant_par[13] * 
			pow(THETA, refrigerant_par[14] - 1) + 
		refrigerant_par[16] * refrigerant_par[15] * 
			pow(THETA, refrigerant_par[16] - 1);
	double dTHETA_dT = -1 / refrigerant_par[0];	
 
	// Calculate vapour pressure 
	//
	if (refrigerant_par[2] < 0) {
		// Calculate temperature-dependent coefficients
		//
		double rho_l = refrigerant_rho_l(T_K, refrigerant_par);		
		return rho_l * dX_dTHETA * dTHETA_dT;
		
	} else {
		// Calculate temperature-dependent coefficients
		//	
		return refrigerant_par[1]*dX_dTHETA*dTHETA_dT;
			
	}
}
