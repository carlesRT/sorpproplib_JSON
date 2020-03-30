//////////////////////////////////////////
// refrigerant_saturatedLiquidDensity.h //
//////////////////////////////////////////
#ifndef refrigerant_saturatedLiquidDensity_h
#define refrigerant_saturatedLiquidDensity_h


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
 *	refrigerant_par[0]	-> Tcrit	-> in K
 *	refrigerant_par[1]	-> rhocrit	-> in kg/m3
 *	refrigerant_par[2]	-> flag		-> in -
 * 	refrigerant_par[3] 	-> fac1		-> in -
 * 	refrigerant_par[4] 	-> exp1		-> in -
 * 	refrigerant_par[5] 	-> fac2		-> in -
 * 	refrigerant_par[6] 	-> exp2		-> in -
 * 	refrigerant_par[7] 	-> fac3		-> in -
 * 	refrigerant_par[8] 	-> exp3		-> in -
 * 	refrigerant_par[9] 	-> fac4		-> in -
 * 	refrigerant_par[10] -> exp4		-> in -
 * 	refrigerant_par[11] -> fac5		-> in -
 * 	refrigerant_par[12] -> exp5		-> in -
 * 	refrigerant_par[13] -> fac6		-> in -
 * 	refrigerant_par[14] -> exp6		-> in -
 * 	refrigerant_par[15] -> fac7		-> in -
 * 	refrigerant_par[16] -> exp7		-> in -
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
double refrigerant_rho_l(double T_K, double refrigerant_par[]);


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
double refrigerant_drho_l_dT(double T_K, double refrigerant_par[]);

#endif
