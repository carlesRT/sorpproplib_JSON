////////////////////////////////
// test_adsorption_langmuir.c //
////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "adsorption_langmuir.h"


int main() {
	// Define parameter record for executing Langmuir equation. Data for working
	// pair "Carbon Norit-RB1 / CO2" is taken from:
	//
	// 	Rick van der Vaar et al. Single and Mixed Gas Adsorption Equilibria of 
	//	Carbon Dioxide/Methane on Activated Carbon. Adsorption 2000. 6: p. 
	//	311-323.
	//
	double isotherm_par[] = {23500, 0.000000000322, 0.34943146};		
	

	// Define some input values for calculating equilibrium data
	//
	double p_Pa = 3e5;
	double T_K = 303.1;
	
	
	// Calculate equilibrium data
	//
	double w_kgkg = adsorption_langmuir_w_pT(p_Pa, T_K, isotherm_par);
	double p_Pa_inv = adsorption_langmuir_p_wT(w_kgkg, T_K, isotherm_par);
	double T_K_inv = adsorption_langmuir_T_pw(p_Pa, w_kgkg, isotherm_par);


	// Calculate derivatives
	//
	double dw_dp_func_kgkgPa = adsorption_langmuir_dw_dp_pT(p_Pa, T_K,
		isotherm_par);
	double dw_dp_plus_kgkgPa = adsorption_langmuir_w_pT(p_Pa+0.1, T_K,
		isotherm_par);
	double dw_dp_minus_kgkgPa = adsorption_langmuir_w_pT(p_Pa-0.1, T_K,
		isotherm_par);
	double dw_dp_num_kgkgPa = (dw_dp_plus_kgkgPa - dw_dp_minus_kgkgPa)/0.2;
	
	double dw_dT_func_kgkgK = adsorption_langmuir_dw_dT_pT(p_Pa, T_K,
		isotherm_par);
	double dw_dT_plus_kgkgK = adsorption_langmuir_w_pT(p_Pa, T_K+0.01,
		isotherm_par);
	double dw_dT_minus_kgkgK = adsorption_langmuir_w_pT(p_Pa, T_K-0.01,
		isotherm_par);
	double dw_dT_num_kgkgK = (dw_dT_plus_kgkgK - dw_dT_minus_kgkgK)/0.02;
	
	double dp_dw_func_Pakgkg= adsorption_langmuir_dp_dw_wT(w_kgkg, T_K,
		isotherm_par);
	double dp_dw_plus_Pakgkg = adsorption_langmuir_p_wT(w_kgkg+0.00001, T_K,
		isotherm_par);
	double dp_dw_minus_Pakgkg = adsorption_langmuir_p_wT(w_kgkg-0.00001, T_K,
		isotherm_par);
	double dp_dw_num_Pakgkg = (dp_dw_plus_Pakgkg - dp_dw_minus_Pakgkg)/0.00002;
	
	double dp_dT_func_Pakgkg= adsorption_langmuir_dp_dT_wT(w_kgkg, T_K,
		isotherm_par);
	double dp_dT_plus_Pakgkg = adsorption_langmuir_p_wT(w_kgkg, T_K+0.01,
		isotherm_par);
	double dp_dT_minus_Pakgkg = adsorption_langmuir_p_wT(w_kgkg, T_K-0.01,
		isotherm_par);
	double dp_dT_num_Pakgkg = (dp_dT_plus_Pakgkg - dp_dT_minus_Pakgkg)/0.02;
	
	// Calculate reduced spreading pressure
	//
	double piStar_molkg_func = adsorption_langmuir_piStar_pyxgTM(p_Pa, 1, 1, 1,
		T_K, 0.04401, isotherm_par);
	
	int n_counter;
	int n_steps = 10000000;
	double h = (p_Pa - 0) / n_steps;
	double p_tmp;
	double piStar_molkg_num = 0;
	
	for(n_counter=1; n_counter<n_steps; n_counter++) {
		// Calculate reduced spreading pressure by numerical integration
		// using the trapezoidal rule
		//
		p_tmp = 0 - h/2 + n_counter * h;
		piStar_molkg_num += (h / 0.04401) *
			(adsorption_langmuir_w_pT(p_tmp, T_K, isotherm_par) / p_tmp);
	}
	
	
	// Print calculated values
	//
	printf("\n\n##\n##\nSelected working pair is \"Carbon Norit-RB1 / CO2\".");	
	printf("\nSelected isotherm is \"Langmuir\".");
	
	printf("\n\nFor T = %f K and p = %f Pa, equilibrium loading results in x = %f kg/kg.", 
		T_K, p_Pa, w_kgkg);
	printf("\nFor T = %f K and w = %f kg/kg, equilibrium pressure results in p = %f Pa.", 
		T_K, w_kgkg, p_Pa_inv);
	printf("\nFor p = %f Pa and w = %f kg/kg equilibrium temperature results in T = %f K.", 
		p_Pa, w_kgkg, T_K_inv);
		
	printf("\n\nFor T = %f K and p = %f Pa, analytical derivative of w with respect to p results in dw_dp = %f kg/kg/Pa.",
		T_K, p_Pa, dw_dp_func_kgkgPa);
	printf("\nFor T = %f K and p = %f Pa, numerical derivative of w with respect to p results in dw_dp = %f kg/kg/Pa.",
		T_K, p_Pa, dw_dp_num_kgkgPa);
	printf("\nFor T = %f K and p = %f Pa, analytical derivative of w with respect to T results in dw_dT = %f kg/kg/K.",
		T_K, p_Pa, dw_dT_func_kgkgK);
	printf("\nFor T = %f K and p = %f Pa, numerical derivative of w with respect to T results in dw_dT = %f kg/kg/K.",
		T_K, p_Pa, dw_dT_num_kgkgK);
		
	printf("\n\nFor T = %f K and w = %f kg/kg, analytical derivative of p with respect to w results in dp_dw = %f Pakg/kg.", 
		T_K, w_kgkg, dp_dw_func_Pakgkg);
	printf("\nFor T = %f K and w = %f kg/kg, numerical derivative of p with respect to w results in dp_dw = %f Pakg/kg.", 
		T_K, w_kgkg, dp_dw_num_Pakgkg);
	printf("\nFor T = %f K and w = %f kg/kg, analytical derivative of p with respect to T results in dp_dT = %f Pa/K.", 
		T_K, w_kgkg, dp_dT_func_Pakgkg);
	printf("\nFor T = %f K and w = %f kg/kg, numerical derivative of p with respect to T results in dp_dT = %f Pa/K.", 
		T_K, w_kgkg, dp_dT_num_Pakgkg);
		
	printf("\n\nFor T = %f K and p = %f Pa, analytical reduced spreading pressure results in piStar = %f mol/kg.",
		T_K, p_Pa, piStar_molkg_func);
	printf("\nFor T = %f K and p = %f Pa, numerical reduced spreading pressure results in piStar = %f mol/kg.",
		T_K, p_Pa, piStar_molkg_num);
		
    return EXIT_SUCCESS;	
}