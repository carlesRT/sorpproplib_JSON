////////////////////////
// test_refrigerant.c //
////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "refrigerant.h"
#include "structDefinitions.c"


int main() {
	// Define parameter record for executing vapor pressure equation and
	// saturated liquid density equation. Data for refrigerant "R-134a" is taken
	// from:
	//
	// 	R. Tillner-Roth and H.D. Baehr. An International Standard Formulation
	//	for the Thermodynamic Properties of 1,1,1,2-Tetrafluoroethane (HFC-134a)
	//	for Temperatures from 170 K to 455 K and Pressures up to 70 MPa. Journal
	//	of Physical and Chemical Reference Data 1994. 23: p. 657-729.
	//
	double vapPre_par[] = {374.18, 4.05629e6, -7.686556, 1, 2.311791,
		1.5, -2.039554, 2, -3.583758, 4, 0, 0, 0, 0};
	double satLiqDen_par[] = {374.18, 1, 1, 518.20, 0, 884.13,
		0.333333333333333, 485.84, 0.666666666666666, 193.29,
		3.333333333333333, 0, 0, 0, 0, 0, 0};

	// Define parameter record for executing cubic equation of state. Data for
	// refrigerant "R-134a" is taken from:
	//
	// 	Takigawa et al. Solubility and viscosity of refrigerant/lubricant
	//	mixtures: hydrofluorocarbon/alkylbenzene systems. International Journal
	//	of Refrigeration 2002. 25: p. 1014-1024.
	//
	double refrigerant_R134a_par_cubic[] = {10, 4.0593e6, 374.21, 0.32684,
		-0.0077, 1.0025, 0.50532, -0.04983, 0};

	// Define parameter record for executing vapor pressure equation. Data for
	// refrigerant "Benzene" is taken from:
	//
	// 	NIST Webbook.
	//
	double refrigerant_par_benzene[] = {4.72583, 1660.652, -1.461};


	// Define some input values for calculation of fluid properties
	//
	double T_K = 353.15;


	// Calculate state values
	//
	Refrigerant *refrigerant_r134a = newRefrigerant(
		"EoS_vaporPressure",
		"EoS_saturatedLiquidDensity",
		vapPre_par);

	double psat_r134a_Pa = refrigerant_r134a->psat_T(T_K, vapPre_par,
		refrigerant_r134a);
	double T_r134a_inv_K = refrigerant_r134a->Tsat_p(psat_r134a_Pa,
		vapPre_par, refrigerant_r134a);
	double psat_r134a_Pa_plus = refrigerant_r134a->psat_T(T_K+0.0001,
		vapPre_par, refrigerant_r134a);
	double psat_r134a_Pa_minus = refrigerant_r134a->psat_T(T_K-0.0001,
		vapPre_par, refrigerant_r134a);
	double dpsat_dT_r134a_PaK = refrigerant_r134a->dpsat_dT(T_K, vapPre_par,
		refrigerant_r134a);
	double dpsat_dT_r134a_PaK_num = (psat_r134a_Pa_plus -
		psat_r134a_Pa_minus) / 0.0002;

	double rho_l_r134a_kgm3 = refrigerant_r134a->rho_l_T(T_K, satLiqDen_par);
	double rho_l_r134a_kgm3_plus = refrigerant_r134a->rho_l_T(T_K+0.0001,
		satLiqDen_par);
	double rho_l_r134a_kgm3_minus = refrigerant_r134a->rho_l_T(T_K-0.0001,
		satLiqDen_par);
	double drho_l_dT_r134a_kgm3K = refrigerant_r134a->drho_l_dT(T_K,
		satLiqDen_par);
	double drho_l_dT_r134a_kgm3K_num = (rho_l_r134a_kgm3_plus -
		rho_l_r134a_kgm3_minus) / 0.0002;


	Refrigerant *refrigerant_r134a_1 = newRefrigerant(
		"EoS_cubic",
		"EoS_saturatedLiquidDensity",
		refrigerant_R134a_par_cubic);

	double psat_r134a_Pa_1 = refrigerant_r134a_1->psat_T(T_K,
		refrigerant_R134a_par_cubic, refrigerant_r134a_1);
	double T_r134a_inv_K_1 = refrigerant_r134a_1->Tsat_p(psat_r134a_Pa_1,
		refrigerant_R134a_par_cubic, refrigerant_r134a_1);
	double psat_r134a_Pa_plus_1 = refrigerant_r134a_1->psat_T(T_K+0.00000001,
		refrigerant_R134a_par_cubic, refrigerant_r134a_1);
	double psat_r134a_Pa_minus_1 = refrigerant_r134a_1->psat_T(T_K-0.00000001,
		refrigerant_R134a_par_cubic, refrigerant_r134a_1);
	double dpsat_dT_r134a_PaK_1 = refrigerant_r134a_1->dpsat_dT(T_K,
		refrigerant_R134a_par_cubic, refrigerant_r134a_1);
	double dpsat_dT_r134a_PaK_num_1 = (psat_r134a_Pa_plus_1 -
		psat_r134a_Pa_minus_1) / 0.00000002;


	Refrigerant *refrigerant_benzene = newRefrigerant(
		"Antoine",
		"NoSaturatedLiquidDensity",
		refrigerant_par_benzene);

	double psat_benzene_Pa = refrigerant_benzene->psat_T(T_K,
		refrigerant_par_benzene, refrigerant_benzene);
	double T_benzene_inv_K = refrigerant_benzene->Tsat_p(psat_benzene_Pa,
		refrigerant_par_benzene, refrigerant_benzene);
	double psat_benzene_Pa_plus = refrigerant_benzene->psat_T(T_K+0.0001,
		refrigerant_par_benzene, refrigerant_benzene);
	double psat_benzene_Pa_minus = refrigerant_benzene->psat_T(T_K-0.0001,
		refrigerant_par_benzene, refrigerant_benzene);
	double dpsat_dT_benzene_PaK = refrigerant_benzene->dpsat_dT(T_K,
		refrigerant_par_benzene, refrigerant_benzene);
	double dpsat_dT_benzene_PaK_num = (psat_benzene_Pa_plus -
		psat_benzene_Pa_minus) / 0.0002;


	// Print results of calculations
	//
	printf("\n\n##\n##\nCreated structure for refrigerant.");
	printf("\nSelected refrigerant is \"R-134a\".");

	printf("\n\nFor T = %f K, vapor pressure results in p = %f Pa.",
		T_K, psat_r134a_Pa);
	printf("\nFor p = %f Pa, saturation temperature results in T = %f K.",
		psat_r134a_Pa, T_r134a_inv_K);
	printf("\nFor T = %f K, analytical derivative of vapor pressure wrt. "
		"temperature results in dp_dT = %f Pa/K.", T_K, dpsat_dT_r134a_PaK);
	printf("\nFor T = %f K, numerical derivative of vapor pressure wrt. "
		"temperature results in dp_dT = %f Pa/K.", T_K, dpsat_dT_r134a_PaK_num);

	printf("\nFor T = %f K, saturated liquid density results in rho_l = %f "
		"kg/m3.", T_K, rho_l_r134a_kgm3);
	printf("\nFor T = %f K, analytical derivative of saturated liquid density "
		"wrt. temperature results in drho_l_dT = %f kg/m3/K.", T_K,
		drho_l_dT_r134a_kgm3K);
	printf("\nFor T = %f K, numerical derivative of saturated liquid density "
		"wrt. temperature results in drho_l_dT = %f kg/m3/K.", T_K,
		drho_l_dT_r134a_kgm3K_num);


	printf("\n\n##\n##\nCreated structure for refrigerant.");
	printf("\nSelected refrigerant is \"R-134a\".");

	printf("\n\nFor T = %f K, vapor pressure results in p = %f Pa.",
		T_K, psat_r134a_Pa_1);
	printf("\nFor p = %f Pa, saturation temperature results in T = %f K.",
		psat_r134a_Pa_1, T_r134a_inv_K_1);
	printf("\nFor T = %f K, analytical derivative of vapor pressure wrt. "
		"temperature results in dp_dT = %f Pa/K.", T_K, dpsat_dT_r134a_PaK_1);
	printf("\nFor T = %f K, numerical derivative of vapor pressure wrt. "
		"temperature results in dp_dT = %f Pa/K.", T_K,
		dpsat_dT_r134a_PaK_num_1);


	printf("\n\n##\n##\nCreated structure for refrigerant.");
	printf("\nSelected refrigerant is \"Benzene\".");

	printf("\n\nFor T = %f K, vapor pressure results in p = %f Pa.",
		T_K, psat_benzene_Pa);
	printf("\nFor p = %f Pa, saturation temperature results in T = %f K.",
		psat_benzene_Pa, T_benzene_inv_K);
	printf("\nFor T = %f K, analytical derivative of vapor pressure wrt. "
		"temperature results in dp_dT = %f Pa/K.", T_K, dpsat_dT_benzene_PaK);
	printf("\nFor T = %f K, numerical derivative of vapor pressure wrt. "
		"temperature results in dp_dT = %f Pa/K.", T_K,
		dpsat_dT_benzene_PaK_num);


	// Free memory that is allocate for structures
	//
	delRefrigerant(refrigerant_r134a);
	delRefrigerant(refrigerant_benzene);

    return EXIT_SUCCESS;
}