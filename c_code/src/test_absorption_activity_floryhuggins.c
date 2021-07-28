/////////////////////////////////////////////
// test_absorption_activity_floryhuggins.c //
/////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "absorption_activity_floryhuggins.h"
#include "refrigerant_vaporPressure.h"


int main() {
	// Define parameter record for executing UNIQUAC equation. Data for working
	// pair "Lubricant PEB9 / HFC134a" is taken from:
	//
	// 	Wahlström Å. and L. Vamling Solubility of HFCs in pentaerythritol
	//	tetraalkyl esters. Journal of Chemical & Engineering Data 2000. 45(1):
	//	p. 97-103.
	//
	double isotherm_par[] = {10.41, 938, -176};

	// Define parameter record for executing vapor pressure equation. Data for
	// refrigerant "R-134a" is taken from:
	//
	// 	R. Tillner-Roth and H.D. Baehr. An International Standard Formulation
	//	for the Thermodynamic Properties of 1,1,1,2-Tetrafluoroethane (HFC-134a)
	//	for Temperatures from 170 K to 455 K and Pressures up to 70 MPa. Journal
	//	of Physical and Chemical Reference Data 1994. 23: p. 657-729.
	//
	double refrigerant_par[] = {374.18, 4.05629e6, -7.686556, 1, 2.311791,
		1.5, -2.039554, 2, -3.583758, 4, 0, 0, 0, 0, 0, 0};

	// Define some input values for calculating equilibrium data
	//
	double T_K = 303.15;
	double x_molmol = 0.1622;

	double psat_Pa = refrigerant_p_sat_eos1(T_K, refrigerant_par);

	// Calculate equilibrium data
	//
	double gamma_1 = absorption_activity_floryhuggins_g1_Tx(T_K, x_molmol,
		isotherm_par);
	double p_Pa = absorption_activity_floryhuggins_p_Txpsat(T_K, x_molmol,
		psat_Pa, isotherm_par);
	double x_inv_molmol = absorption_activity_floryhuggins_x_pTgpsat(
		p_Pa, T_K, psat_Pa, isotherm_par);

	// Print calculated values
	//
	printf("\n\n##\n##\nSelected working pair is \"Lubricant PEC9 / R-134a\".");
	printf("\nSelected absorption equation is \"Flory-Huggins\".");

	printf("\n\nFor T = %f K and x = %f mol/mol, activity coefficient of "
		"R-134a results in gamma_1 = %f.", T_K, x_molmol, gamma_1);
	printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure results in "
		"p = %f Pa.", T_K, x_molmol, p_Pa);
	printf("\nFor p = %f Pa and T = %f K, equilibrium mole fraction results in "
		"x = %f mol/mol.", p_Pa, T_K, x_inv_molmol);

    return EXIT_SUCCESS;
}
