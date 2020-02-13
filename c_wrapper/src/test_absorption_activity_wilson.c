///////////////////////////////////////
// test_absorption_activity_wilson.c //
///////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "absorption_activity_wilson.h"
#include "refrigerant_vapourPressure.h"


int main() {
	// Define parameter record for executing Wilson equation. Data for working
	// pair "[BMIM]+[(CF3SO2)2N]-(2) / H2O" is taken from:
	//
	// 	Döker M. and J. Gmehling Measurement and prediction of vapor–liquid 
	//	equilibria of ternary systems containing ionic liquids. Fluid phase 
	//	equilibria 2005. 227(2): p. 255-266.
	//
	double isotherm_par[] = {7985.164, 16736, 0, 0, 0, 0};
	
	// Define parameter record for executing vapour pressure equation. Data for
	// refrigerant "H2O" is taken from:
	//
	// 	W. Wagner and P. Pruß. The IAPWS Formulation 1995 for the Thermodynamic
	//	Properties of Ordinary Water Substance for General and Scientific Use.
	//	Journal of Physical and Chemical Reference Data 2002. 31: p. 387-535.
	//
	double refrigerant_par[] = {647.096, 22064000, -7.85951783, 1, 1.84408259,
		1.5, -11.7866497, 3, 22.6807411, 3.5, -15.9618719, 4, 1.80122502, 7.5};


	// Define some input values for calculating equilibrium data
	//
	double T_K = 353.15;
	double x_molmol = 0.9386;
	
	double psat_Pa = refrigerant_p_sat(T_K, refrigerant_par);
		
	
	// Calculate equilibrium data
	//
	double gamma_1 = absorption_activity_wilson_g1_Txv1v2(T_K , x_molmol, -1,
		-1, isotherm_par);
	double p_Pa = absorption_activity_wilson_p_Txv1v2psat(T_K , x_molmol, -1, 
		-1, psat_Pa, isotherm_par);
		
	
	// Print calculated values
	//
	printf("\n\n##\n##\nSelected working pair is \"[BMIM]+[(CF3SO2)2N]-(2) / H2O\".");	
	printf("\nSelected absorption equation is \"Wilson\".");
	
	printf("\n\nFor T = %f K and x = %f mol/mol, activity coefficient of H2O results in gamma_1 = %f.", 
		T_K, x_molmol, gamma_1);
	printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure results in p = %f Pa.", 
		T_K, x_molmol, p_Pa);
		
    return EXIT_SUCCESS;	
}