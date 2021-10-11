///////////////////////////////////////
// test_adsorption_dubininAstakhov.c //
///////////////////////////////////////
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "adsorption_freundlich.h"
#include "refrigerant.h"
#include "structDefinitions.c"


int main() {
	/*
	 * Define parameter record for executing vapor pressure equation. Data for
	 * refrigerant "H2O" is taken from:
	 *
	 * 	W. Wagner and P. Pruß. The IAPWS Formulation 1995 for the Thermodynamic
	 *	Properties of Ordinary Water Substance for General and Scientific Use.
	 *	Journal of Physical and Chemical Reference Data 2002. 31: p. 387-535.
	 *
	 */
	double refrigerant_par_H2O[] = {647.096, 22064000, -7.85951783, 1,
		1.84408259, 1.5, -11.7866497, 3, 22.6807411, 3.5, -15.9618719, 4,
		1.80122502, 7.5, 0, 0};

	/*
	 * Define parameter record for executing Freundlich equation. Data for
	 * working pair "Silica gel Fuji A Type / Water" is taken from:
	 *
	 * 	A. Sakoda, M. Suzuki. FUNDAMENTAL STUDY ON SOLAR POWERED ADSORPTION
	 *	COOLING SYSTEM. Journal of Chemical Engineering of Japan 1984: p.
	 *	17-52.
	 *
	 */
	double isotherm_par[] = {0.346, 0, 0, 0, 0.625, 0, 0, 0};


	// Define some input values for calculating equilibrium data
	//
	Refrigerant *refrigerant_water = newRefrigerant(
		"VaporPressure_EoS1",
		"SaturatedLiquidDensity_EoS1",
		isotherm_par);

	double p_Pa = refrigerant_water->p_sat_T(
		283, refrigerant_par_H2O, refrigerant_water);
	double T_K = 373;

	double p_sat_Pa = refrigerant_water->p_sat_T(
		T_K, refrigerant_par_H2O, refrigerant_water);
	double dp_sat_dT_PaK = refrigerant_water->dp_sat_dT(T_K,
		refrigerant_par_H2O, refrigerant_water);


	// Calculate equilibrium data
	//
	double w_kgkg = adsorption_freundlich_w_pTpsat(p_Pa, T_K, p_sat_Pa,
		isotherm_par);
	double p_Pa_inv = adsorption_freundlich_p_wTpsat(w_kgkg, T_K, p_sat_Pa,
		isotherm_par);
	double T_K_inv = adsorption_freundlich_T_pwpsat(p_Pa, w_kgkg,
		refrigerant_water->p_sat_T, refrigerant_water->dp_sat_dT,
		isotherm_par, refrigerant_par_H2O, refrigerant_water);


	// Calculate derivatives
	//
	double dw_dp_func_kgkgPa = adsorption_freundlich_dw_dp_pTpsat(p_Pa, T_K,
		p_sat_Pa, isotherm_par);
	double dw_dp_plus_kgkgPa = adsorption_freundlich_w_pTpsat(p_Pa+0.1, T_K,
		p_sat_Pa, isotherm_par);
	double dw_dp_minus_kgkgPa = adsorption_freundlich_w_pTpsat(p_Pa-0.1, T_K,
		p_sat_Pa, isotherm_par);
	double dw_dp_num_kgkgPa = (dw_dp_plus_kgkgPa - dw_dp_minus_kgkgPa)/0.2;

	double dw_dT_func_kgkgK = adsorption_freundlich_dw_dT_pTpsat(p_Pa, T_K,
		p_sat_Pa, dp_sat_dT_PaK, isotherm_par);
	double dw_dT_plus_kgkgK = adsorption_freundlich_w_pTpsat(p_Pa, T_K+0.01,
		refrigerant_water->p_sat_T(T_K+0.01, refrigerant_par_H2O,
		refrigerant_water), isotherm_par);
	double dw_dT_minus_kgkgK = adsorption_freundlich_w_pTpsat(p_Pa, T_K-0.01,
		refrigerant_water->p_sat_T(T_K-0.01, refrigerant_par_H2O,
		refrigerant_water), isotherm_par);
	double dw_dT_num_kgkgK = (dw_dT_plus_kgkgK - dw_dT_minus_kgkgK)/0.02;

	double dp_dw_func_Pakgkg= adsorption_freundlich_dp_dw_wTpsat(w_kgkg, T_K,
		p_sat_Pa, isotherm_par);
	double dp_dw_plus_Pakgkg = adsorption_freundlich_p_wTpsat(w_kgkg+0.00001,
		T_K, p_sat_Pa, isotherm_par);
	double dp_dw_minus_Pakgkg = adsorption_freundlich_p_wTpsat(w_kgkg-0.00001,
		T_K, p_sat_Pa, isotherm_par);
	double dp_dw_num_Pakgkg = (dp_dw_plus_Pakgkg - dp_dw_minus_Pakgkg)/0.00002;

	double dp_dT_func_Pakgkg= adsorption_freundlich_dp_dT_wTpsat(w_kgkg, T_K,
		p_sat_Pa, dp_sat_dT_PaK, isotherm_par);
	double dp_dT_plus_Pakgkg = adsorption_freundlich_p_wTpsat(w_kgkg, T_K+0.01,
		refrigerant_water->p_sat_T(T_K+0.01, refrigerant_par_H2O,
		refrigerant_water), isotherm_par);
	double dp_dT_minus_Pakgkg = adsorption_freundlich_p_wTpsat(w_kgkg, T_K-0.01,
		refrigerant_water->p_sat_T(T_K-0.01, refrigerant_par_H2O,
		refrigerant_water), isotherm_par);
	double dp_dT_num_Pakgkg = (dp_dT_plus_Pakgkg - dp_dT_minus_Pakgkg)/0.02;

	// Calculate reduced spreading pressure
	//
	double piStar_molkg = adsorption_freundlich_piStar_pyxgTpsatM(p_Pa, 1, 1,
		1, T_K, p_sat_Pa, 0.01801528, isotherm_par);

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
		piStar_molkg_num += (h / 0.01801528) *
			(adsorption_freundlich_w_pTpsat(p_tmp, T_K, p_sat_Pa,
			isotherm_par) / p_tmp);
	}


	// Print calculated values
	//
	printf("\n\n##\n##\nSelected working pair is \"Silica gel Fuji A Type / "
		"H2O\".");
	printf("\nSelected isotherm is \"Freundlich\".");

	printf("\n##\nFor T = %f K and p = %f Pa, equilibrium loading results in "
		"x = %f kg/kg.", T_K, p_Pa, w_kgkg);
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

	// Free memory that is allocate for structures
	//
	delRefrigerant(refrigerant_water);

    return EXIT_SUCCESS;
}
