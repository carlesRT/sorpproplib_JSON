////////////////////////////////
// test_absorption_duehring.c //
////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "absorption_duehring.h"


int main() {
	// Define parameter record for executing Duehring equation. Data for working
	// pair "NaOH-KOH-CsOH / H2O" is taken from:
	//
	// 	Herold K.E. et al. Development of an absorption heat pump water heater
	//  using an aqueous ternary hydroxide working fluid. International journal 
	//  of refrigeration 1991. 14(3): p. 156-167.
	//
	double isotherm_par[] = 
		{6.164233723, -0.2746665026, 0.004916023734, -0.00002859098259, 
		 -53.80343163, 5.004848451, -0.1228273028, 0.0010961422341,
		 6.427154896, -1208.919437, -166159.963, 0, 1, 273.15, 0.001};


	// Define some input values for calculating equilibrium data
	//
	double p_Pa = 724.659957;
	double T_K = 323.15;
	
	
	// Calculate equilibrium data
	//
	double w_kgkg = absorption_duehring_w_pT(p_Pa, T_K, isotherm_par);
	double p_Pa_inv = absorption_duehring_p_wT(w_kgkg, T_K, isotherm_par);
	double T_K_inv = absorption_duehring_T_pw(p_Pa, w_kgkg, isotherm_par);


	// Calculate derivatives
	//
	double dw_dp_func_kgkgPa = absorption_duehring_dw_dp_pT(p_Pa, T_K,
		isotherm_par);
	double dw_dp_plus_kgkgPa = absorption_duehring_w_pT(p_Pa+0.1, T_K,
		isotherm_par);
	double dw_dp_minus_kgkgPa = absorption_duehring_w_pT(p_Pa-0.1, T_K,
		isotherm_par);
	double dw_dp_num_kgkgPa = (dw_dp_plus_kgkgPa - dw_dp_minus_kgkgPa)/0.2;
	
	double dw_dT_func_kgkgK = absorption_duehring_dw_dT_pT(p_Pa, T_K,
		isotherm_par);
	double dw_dT_plus_kgkgK = absorption_duehring_w_pT(p_Pa, T_K+0.1,
		isotherm_par);
	double dw_dT_minus_kgkgK = absorption_duehring_w_pT(p_Pa, T_K-0.1,
		isotherm_par);
	double dw_dT_num_kgkgK = (dw_dT_plus_kgkgK - dw_dT_minus_kgkgK)/0.2;
	
	double dp_dw_func_Pakgkg= absorption_duehring_dp_dw_wT(w_kgkg, T_K,
		isotherm_par);
	double dp_dw_plus_Pakgkg = absorption_duehring_p_wT(w_kgkg+0.00001, T_K,
		isotherm_par);
	double dp_dw_minus_Pakgkg = absorption_duehring_p_wT(w_kgkg-0.00001, T_K,
		isotherm_par);
	double dp_dw_num_Pakgkg = (dp_dw_plus_Pakgkg - dp_dw_minus_Pakgkg)/0.00002;
	
	double dp_dT_func_Pakgkg= absorption_duehring_dp_dT_wT(w_kgkg, T_K,
		isotherm_par);
	double dp_dT_plus_Pakgkg = absorption_duehring_p_wT(w_kgkg, T_K+0.01,
		isotherm_par);
	double dp_dT_minus_Pakgkg = absorption_duehring_p_wT(w_kgkg, T_K-0.01,
		isotherm_par);
	double dp_dT_num_Pakgkg = (dp_dT_plus_Pakgkg - dp_dT_minus_Pakgkg)/0.02;
	
	
	// Print calculated values
	//
	printf("\n\n##\n##\nSelected working pair is \"NaOH-KOH-CsOH / H2O\".");	
	printf("\nSelected absorption equation is \"Duehring\".");
	
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
		
    return EXIT_SUCCESS;	
}