///////////////////////////////
// absorption_mixing_1pvdw.c //
///////////////////////////////
#include <math.h>
#include "absorption_mixing_1pvdw.h"


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
 * General form of Peng-Robinson or Soave-Redlich-Kwong equation when using
 * one parameter van der Waals mixing rule:
 * ----------------------------------------
 * 	p = R * T / (V_m - b) - a / (V_m^2 + m *b + n * b^2)
 *
 *		with: For PR equation, m = 2 and n = -1
 *		----- For SRK equation, m = 1 and n = 0
 *
 * Temperature-dependent coefficients:
 * -----------------------------------
 * 	a = x_1^2 * a_1 + x_2^2 * a_2 + 2 * x_1 * x_2 * a_12
 *	b = x_1 * b_1 + x_2 * b_2
 *
 *		with: a_12 = sqrt(a_1 * a_2) * (1- k_12)
 *		----- a_1 = alpha_0 * R^2 * T_crit,1^2 / p_crit,1 * alpha_1
 *			  a_2 = alpha_0 * R^2 * T_crit,2^2 / p_crit,2 * alpha_2
 *			  alpha_1 = (1 + m_1 * (1 - sqrt(T / T_crit,1)))^2
 *			  alpha_2 = (1 + m_2 * (1 - sqrt(T / T_crit,2)))^2
 *			  m_1 = c_0 + c_1 * w_1^2 + c_2 * w_1^2
 *			  m_2 = c_0 + c_1 * w_2^2 + c_2 * w_2^2
 *			  b_1 = b_0 * R * T_crit,1 / p_crit,1
 *			  b_2 = b_0 * R * T_crit,2 / p_crit,2
 *
 * Possible inputs required by user:
 * ---------------------------------
 *	T: Temperature in K
 *	x: Mole fraction in liquid phase in mol/mol
 *	V_m: Molar volume in m³/mol
 *
 * Order of coefficients in JSON-file:
 * -----------------------------------
 *	isotherm_par[0]	-> dlambda_12	-> in J/mol
 * 	isotherm_par[1] -> dlambda_21	-> in J/mol
 * 	isotherm_par[2] -> vm_1			-> in m³/mol
 * 	isotherm_par[3] -> vm_2			-> in m³/mol
 *
 */


/*
 * absorption_mixing_1pvdw_p_Tvx:
 * ------------------------------
 *
 * Calculates equilibrium pressure p_Pa in Pa of first component depending on 
 * temperature T_K in K, mole fraction in liquid phase x_molmol in mol/mol, and
 * saturation pressure of first component p_sat_Pa in Pa.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double x_molmol:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
 *	double p_sat_Pa:
 *		Saturation pressure of first component in Pa.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Tsubo & Kakatayama
 *		equation.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure p_Pa in Pa.
 *
 * History:
 * --------
 *	01/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double absorption_mixing_1pvdw_p_Tvx(double T_K, double v,
	double x_molmol, double isotherm_par[]) {
	// Check type of equation of state
	//
	double m, n, alpha_0, c_0, c_1, c_2, b_0;
	
	if (EOS > 5) {
		// Peng-Robinson equation
		//
		m = 2;
		n = -1;
		alpha_0 = 0.45724;
		c_0 = 0.37464;
		c_1 = 1.54226;
		c_2 = -0.26992;
		b_0 = 0.077796;
		
	} else if (EOS < -5) {
		// Soave-Redlich-Kwong equation
		//
		m = 1;
		n = 0;
		alpha_0 = 1 / (9 * (pow(2, 1/3) - 1));
		c_0 = 0.480;
		c_1 = 1.574;
		c_2 = -0.176;
		b_0 = 0.08664;		
		
	} else {
		// Nothing implemented yet
		//
		m = 0;
		n = 0;
		alpha_0 = 0;
		c_0 = 0;
		c_1 = 0;
		c_2 = 0;
		b_0 = 0;
				
	}
	
	// Calculate mole fractions
	//
	double x_1 = x_molmol;
	double x_2 = 1 - x_1;
	
	// Calculate a
	//
	double m_1 = c_0 + c_1 * w_1 + c_2 * pow(w_1, 2);
	double m_2 = c_0 + c_1 * w_2 + c_2 * pow(w_2, 2);	
	double alpha_1 = pow(1 + m_1 * (1 - sqrt(T_K / T_crit1)), 2);
	double alpha_2 = pow(1 + m_2 * (1 - sqrt(T_K / T_crit2)), 2);
	
	double a_1 = alpha_0 * pow(IDEAL_GAS_CONSTANT * T_crit1, 2) / 
		p_crit1 * alpha_1;
	double a_2 = alpha_0 * pow(IDEAL_GAS_CONSTANT * T_crit2, 2) / 
		p_crit2 * alpha_2;
	double a_12 = sqrt(a_1 * a_2) * (1- k_12);
	
	double a = pow(x_1, 2) * a_1 + pow(x_2, 2) * a_2 + 2 * x_1 * x_2 * a_12;
	
	// Calculate b
	//
	double b_1 = b_0 * IDEAL_GAS_CONSTANT * T_crit1 / p_crit1;
	double b_2 = b_0 * IDEAL_GAS_CONSTANT * T_crit2 / p_crit2;	
	
	double b = x_1 * b_1 + x_2 * b_2;
	
	// Calculate pressure
	//
	return IDEAL_GAS_CONSTANT * T_K / (v - b) - a / (pow(v, 2) + m * v * b +
		n * pow(b, 2));
}
