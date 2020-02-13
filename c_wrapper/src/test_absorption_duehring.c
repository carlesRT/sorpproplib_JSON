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
	double X_kgkg = absorption_duehring_X_pT(p_Pa, T_K, isotherm_par);
	double p_Pa_inv = absorption_duehring_p_XT(X_kgkg, T_K, isotherm_par);
	double T_K_inv = absorption_duehring_T_pX(p_Pa, X_kgkg, isotherm_par);


	// Calculate derivatives
	//
	double dX_dp_func_kgkgPa = absorption_duehring_dX_dp_pT(p_Pa, T_K,
		isotherm_par);
	double dX_dp_plus_kgkgPa = absorption_duehring_X_pT(p_Pa+0.1, T_K,
		isotherm_par);
	double dX_dp_minus_kgkgPa = absorption_duehring_X_pT(p_Pa-0.1, T_K,
		isotherm_par);
	double dX_dp_num_kgkgPa = (dX_dp_plus_kgkgPa - dX_dp_minus_kgkgPa)/0.2;
	
	double dX_dT_func_kgkgK = absorption_duehring_dX_dT_pT(p_Pa, T_K,
		isotherm_par);
	double dX_dT_plus_kgkgK = absorption_duehring_X_pT(p_Pa, T_K+0.1,
		isotherm_par);
	double dX_dT_minus_kgkgK = absorption_duehring_X_pT(p_Pa, T_K-0.1,
		isotherm_par);
	double dX_dT_num_kgkgK = (dX_dT_plus_kgkgK - dX_dT_minus_kgkgK)/0.2;
	
	double dp_dX_func_Pakgkg= absorption_duehring_dp_dX_XT(X_kgkg, T_K,
		isotherm_par);
	double dp_dX_plus_Pakgkg = absorption_duehring_p_XT(X_kgkg+0.00001, T_K,
		isotherm_par);
	double dp_dX_minus_Pakgkg = absorption_duehring_p_XT(X_kgkg-0.00001, T_K,
		isotherm_par);
	double dp_dX_num_Pakgkg = (dp_dX_plus_Pakgkg - dp_dX_minus_Pakgkg)/0.00002;
	
	double dp_dT_func_Pakgkg= absorption_duehring_dp_dT_XT(X_kgkg, T_K,
		isotherm_par);
	double dp_dT_plus_Pakgkg = absorption_duehring_p_XT(X_kgkg, T_K+0.01,
		isotherm_par);
	double dp_dT_minus_Pakgkg = absorption_duehring_p_XT(X_kgkg, T_K-0.01,
		isotherm_par);
	double dp_dT_num_Pakgkg = (dp_dT_plus_Pakgkg - dp_dT_minus_Pakgkg)/0.02;
	
	
	// Print calculated values
	//
	printf("\n\n##\n##\nSelected working pair is \"NaOH-KOH-CsOH / H2O\".");	
	printf("\nSelected absorption equation is \"Duehring\".");
	
	printf("\n\nFor T = %f K and p = %f Pa, equilibrium concentration results in X = %f kg/kg.", 
		T_K, p_Pa, X_kgkg);
	printf("\nFor T = %f K and X = %f kg/kg, equilibrium pressure results in p = %f Pa.", 
		T_K, X_kgkg, p_Pa_inv);
	printf("\nFor p = %f Pa and X = %f kg/kg equilibrium temperature results in T = %f K.", 
		p_Pa, X_kgkg, T_K_inv);
		
	printf("\n\nFor T = %f K and p = %f Pa, analytical derivative of X with respect to p results in dX_dp = %f kg/kg/Pa.",
		T_K, p_Pa, dX_dp_func_kgkgPa);
	printf("\nFor T = %f K and p = %f Pa, numerical derivative of X with respect to p results in dX_dp = %f kg/kg/Pa.",
		T_K, p_Pa, dX_dp_num_kgkgPa);
	printf("\nFor T = %f K and p = %f Pa, analytical derivative of X with respect to T results in dX_dT = %f kg/kg/K.",
		T_K, p_Pa, dX_dT_func_kgkgK);
	printf("\nFor T = %f K and p = %f Pa, numerical derivative of X with respect to T results in dX_dT = %f kg/kg/K.",
		T_K, p_Pa, dX_dT_num_kgkgK);
		
	printf("\n\nFor T = %f K and X = %f kg/kg, analytical derivative of p with respect to X results in dp_dX = %f Pakg/kg.", 
		T_K, X_kgkg, dp_dX_func_Pakgkg);
	printf("\nFor T = %f K and X = %f kg/kg, numerical derivative of p with respect to X results in dp_dX = %f Pakg/kg.", 
		T_K, X_kgkg, dp_dX_num_Pakgkg);
	printf("\nFor T = %f K and X = %f kg/kg, analytical derivative of p with respect to T results in dp_dT = %f Pa/K.", 
		T_K, X_kgkg, dp_dT_func_Pakgkg);
	printf("\nFor T = %f K and X = %f kg/kg, numerical derivative of p with respect to T results in dp_dT = %f Pa/K.", 
		T_K, X_kgkg, dp_dT_num_Pakgkg);
		
    return EXIT_SUCCESS;	
}