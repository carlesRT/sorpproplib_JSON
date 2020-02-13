////////////////////////////////////
// test_absorption_mixing_1pvdw.c //
////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "absorption_mixing_1pvdw.h"


int main() {
	// Define parameter record for executing Wilson equation. Data for working
	// pair "IL [c10mim][ntf2] / CO2" is taken from:
	//
	// 	Manic M.S. et al. High-pressure solubilities of carbon dioxide in ionic
	//	liquids based on bis (trifluoromethylsulfonyl) imide and chloride. The 
	//	Journal of Supercritical Fluids 2012. 65: p. 1-10.
	//
	double isotherm_par_PR[] = {10, 0.2236, 0.5741, 0.0056, 304.2, 800, 
		7380000, 1867000};

	double isotherm_par_SRK[] = {-10, 0.2236, 0.0625, 0.0056, 304.2, 800, 
		7380000, 1867000};


	// Define some input values for calculating equilibrium data
	//
	double T_K = 313.2;
	double x_molmol = 0.769;		


	// Calculate equilibrium data
	//
	double p_Pa_PR = absorption_mixing_1pvdw_p_Tvx(T_K , x_molmol, 0.5/1237.1,
		isotherm_par_PR);
	double p_Pa_SRK = absorption_mixing_1pvdw_p_Tvx(T_K , x_molmol, 0.5/1237.1,
		isotherm_par_SRK);
		
	
	// Print calculated values
	//
	printf("\n\n##\n##\nSelected working pair is \"IL [c10mim][ntf2] / CO2\".");	
	printf("\nSelected absorption equation is \"1PVDW\".");
	
	printf("\n\nFor T = %f K, x = %f mol/mol and PRE, equilibrium pressure results in p = %f Pa.", 
		T_K, x_molmol, p_Pa_PR);
	printf("\nFor T = %f K, x = %f mol/mol and SRK, equilibrium pressure results in p = %f Pa.", 
		T_K, x_molmol, p_Pa_SRK);
		
    return EXIT_SUCCESS;	
}