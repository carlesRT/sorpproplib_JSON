////////////////////////////////////
// test_adsorption_dualSiteSips.c //
////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "adsorption_dualSiteSips.h"


int main() {
	// Define parameter record for executing Dual-Site-Sips equation. Data for
	// working pair "MOF CuBTC / Propane" is taken from:
	//
	// 	Lamia N. et al. Adsorption of propane propylene and isobutane on a
	//	metal–organic framework: Molecular simulation and experiment. Chemical
	//	Engineering Science 2009. 64(14): p. 3246-3259.
	//
	double isotherm_par[] =
		{0.00007,0.00016,28700,34100,0.82,0.32,0.27384237,0.05027058,323};


	// Define some input values for calculating equilibrium data
	//
	double p_Pa = 40000;
	double T_K = 373;


	// Calculate equilibrium data
	//
	double w_kgkg = adsorption_dualSiteSips_w_pT(p_Pa, T_K, isotherm_par);
	double p_Pa_inv = adsorption_dualSiteSips_p_wT(w_kgkg, T_K, isotherm_par);
	double T_K_inv = adsorption_dualSiteSips_T_pw(p_Pa, w_kgkg, isotherm_par);


	// Calculate derivatives
	//
	double dw_dp_func_kgkgPa = adsorption_dualSiteSips_dw_dp_pT(p_Pa, T_K,
		isotherm_par);
	double dw_dp_plus_kgkgPa = adsorption_dualSiteSips_w_pT(p_Pa+0.1, T_K,
		isotherm_par);
	double dw_dp_minus_kgkgPa = adsorption_dualSiteSips_w_pT(p_Pa-0.1, T_K,
		isotherm_par);
	double dw_dp_num_kgkgPa = (dw_dp_plus_kgkgPa - dw_dp_minus_kgkgPa)/0.2;

	double dw_dT_func_kgkgK = adsorption_dualSiteSips_dw_dT_pT(p_Pa, T_K,
		isotherm_par);
	double dw_dT_plus_kgkgK = adsorption_dualSiteSips_w_pT(p_Pa, T_K+0.1,
		isotherm_par);
	double dw_dT_minus_kgkgK = adsorption_dualSiteSips_w_pT(p_Pa, T_K-0.1,
		isotherm_par);
	double dw_dT_num_kgkgK = (dw_dT_plus_kgkgK - dw_dT_minus_kgkgK)/0.2;

	double dp_dw_func_Pakgkg= adsorption_dualSiteSips_dp_dw_wT(w_kgkg, T_K,
		isotherm_par);
	double dp_dw_plus_Pakgkg = adsorption_dualSiteSips_p_wT(w_kgkg+0.00001, T_K,
		isotherm_par);
	double dp_dw_minus_Pakgkg = adsorption_dualSiteSips_p_wT(w_kgkg-0.00001,
		T_K, isotherm_par);
	double dp_dw_num_Pakgkg = (dp_dw_plus_Pakgkg - dp_dw_minus_Pakgkg)/0.00002;

	double dp_dT_func_Pakgkg= adsorption_dualSiteSips_dp_dT_wT(w_kgkg, T_K,
		isotherm_par);
	double dp_dT_plus_Pakgkg = adsorption_dualSiteSips_p_wT(w_kgkg, T_K+0.01,
		isotherm_par);
	double dp_dT_minus_Pakgkg = adsorption_dualSiteSips_p_wT(w_kgkg, T_K-0.01,
		isotherm_par);
	double dp_dT_num_Pakgkg = (dp_dT_plus_Pakgkg - dp_dT_minus_Pakgkg)/0.02;

	// Calculate reduced spreading pressure
	//
	double piStar_molkg = adsorption_dualSiteSips_piStar_pyxgTM(p_Pa, 1, 1, 1,
		T_K, 0.0441, isotherm_par);

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
		piStar_molkg_num += (h / 0.0441) *
			(adsorption_dualSiteSips_w_pT(p_tmp, T_K, isotherm_par) / p_tmp);
	}


	// Print calculated values
	//
	printf("\n\n##\n##\nSelected working pair is \"MOF CuBTC / Propane\".");
	printf("\nSelected isotherm is \"Dual-Site-Sips\".");

	printf("\n\nFor T = %f K and p = %f Pa, equilibrium loading results in x = "
		"%f kg/kg.", T_K, p_Pa, w_kgkg);
	printf("\nFor T = %f K and w = %f kg/kg, equilibrium pressure results in p "
		"= %f Pa.", T_K, w_kgkg, p_Pa_inv);
	printf("\nFor p = %f Pa and w = %f kg/kg equilibrium temperature results "
		"in T = %f K.", p_Pa, w_kgkg, T_K_inv);

	printf("\n\nFor T = %f K and p = %f Pa, analytical derivative of w with "
		"respect to p results in dw_dp = %f kg/kg/Pa.", T_K, p_Pa,
		dw_dp_func_kgkgPa);
	printf("\nFor T = %f K and p = %f Pa, numerical derivative of w with "
		"respect to p results in dw_dp = %f kg/kg/Pa.", T_K, p_Pa,
		dw_dp_num_kgkgPa);
	printf("\nFor T = %f K and p = %f Pa, analytical derivative of w with "
		"respect to T results in dw_dT = %f kg/kg/K.", T_K, p_Pa,
		dw_dT_func_kgkgK);
	printf("\nFor T = %f K and p = %f Pa, numerical derivative of w with "
		"respect to T results in dw_dT = %f kg/kg/K.", T_K, p_Pa,
		dw_dT_num_kgkgK);

	printf("\n\nFor T = %f K and w = %f kg/kg, analytical derivative of p with "
		"respect to w results in dp_dw = %f Pakg/kg.", T_K, w_kgkg,
		dp_dw_func_Pakgkg);
	printf("\nFor T = %f K and w = %f kg/kg, numerical derivative of p with "
		"respect to w results in dp_dw = %f Pakg/kg.", T_K, w_kgkg,
		dp_dw_num_Pakgkg);
	printf("\nFor T = %f K and w = %f kg/kg, analytical derivative of p with "
		"respect to T results in dp_dT = %f Pa/K.", T_K, w_kgkg,
		dp_dT_func_Pakgkg);
	printf("\nFor T = %f K and w = %f kg/kg, numerical derivative of p with "
		"respect to T results in dp_dT = %f Pa/K.", T_K, w_kgkg,
		dp_dT_num_Pakgkg);

	printf("\n\nFor T = %f K and p = %f Pa, analytical reduced spreading "
		"pressure results in piStar = %f mol/kg.", T_K, p_Pa, piStar_molkg);
	printf("\nFor T = %f K and p = %f Pa, numerical reduced spreading pressure "
		"results in piStar = %f mol/kg.", T_K, p_Pa, piStar_molkg_num);

    return EXIT_SUCCESS;
}