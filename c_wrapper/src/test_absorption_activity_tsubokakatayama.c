////////////////////////////////////////////////
// test_absorption_activity_tsubokakatayama.c //
////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "absorption_activity_tsubokakatayama.h"
#include "refrigerant_vapourPressure.h"


int main() {
	// Define parameter record for executing Wilson equation. Data for working
	// pair "PAG / R-134a" is taken from:
	//
	// 	Martz W.L. and A. Jacobi Refrigerant-oil mixtures and local composition
	//	modeling. 1994 Air Conditioning and Refrigeration Center. College of 
	//	Engineering. University of Illinois at Urbana-Champaign.
	//
	double isotherm_par[] = {-247, -15259, 1, 1};

	// Define parameter record for executing vapour pressure equation. Data for
	// refrigerant "R-134a" is taken from:
	//
	// 	R. Tillner-Roth and H.D. Baehr. An International Standard Formulation
	//	for the Thermodynamic Properties of 1,1,1,2-Tetrafluoroethane (HFC-134a)
	//	for Temperatures from 170 K to 455 K and Pressures up to 70 MPa. Journal
	//	of Physical and Chemical Reference Data 1994. 23: p. 657-729.
	//
	double refrigerant_par[] = {374.18, 4.05629e6, -7.686556, 1, 2.311791,
		1.5, -2.039554, 2, -3.583758, 4, 0, 0, 0, 0};


	// Define some input values for calculating equilibrium data
	//
	double T_K = 353.15;
	double x_molmol = 0.5250;
	
	double psat_Pa = refrigerant_p_sat(T_K, refrigerant_par);
		
	
	// Calculate equilibrium data
	//
	double gamma_1 = absorption_activity_tsubokakatayama_g1_Txv1v2(T_K, 
		x_molmol, -1, -1, isotherm_par);
	double p_Pa = absorption_activity_tsubokakatayama_p_Txv1v2psat(T_K, 
		x_molmol, -1, -1, psat_Pa, isotherm_par);
		
	
	// Print calculated values
	//
	printf("\n\n##\n##\nSelected working pair is \"PAG / R-134a\".");	
	printf("\nSelected absorption equation is \"Tsubo & Kakatayama\".");
	
	printf("\n\nFor T = %f K and x = %f mol/mol, activity coefficient of H2O results in gamma_1 = %f.", 
		T_K, x_molmol, gamma_1);
	printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure results in p = %f Pa.", 
		T_K, x_molmol, p_Pa);
		
    return EXIT_SUCCESS;	
}