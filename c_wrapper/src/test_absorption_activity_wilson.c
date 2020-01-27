///////////////////////////////////////
// test_absorption_activity_wilson.c //
///////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "absorption_activity_wilson.h"


int main() {
	// Define parameter record for executing Wilson equation. Data for working
	// pair "[BMIM]+[(CF3SO2)2N]-(2) / H2O" is taken from:
	//
	// 	Döker M. and J. Gmehling Measurement and prediction of vapor–liquid 
	//	equilibria of ternary systems containing ionic liquids. Fluid phase 
	//	equilibria 2005. 227(2): p. 255-266.
	//
	double isotherm_par[] = {7985.164, 16736};


	// Define some input values for calculating equilibrium data
	//
	double T_K = 353.15;
	double x_molmol = 0.0562;
	
	
	// Calculate equilibrium data
	//
	double gamma_1 = absorption_activity_wilson_g1_Tx(T_K , x_molmol,
		isotherm_par);
		
	
	// Print calculated values
	//
	printf("\n\n##\n##\nSelected working pair is \"[BMIM]+[(CF3SO2)2N]-(2) / H2O\".");	
	printf("\nSelected absorption equation is \"Wilson\".");
	
	printf("\n\nFor T = %f K and x = %f mol/mol, activity coefficient of H2O results in gamma_1 = %f.", 
		T_K, x_molmol, gamma_1*1000);
		
    return EXIT_SUCCESS;	
}