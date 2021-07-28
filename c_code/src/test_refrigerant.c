////////////////////////
// test_refrigerant.c //
////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "refrigerant.h"
#include "structDefinitions.c"


int main() {
	/*
	 * Define parameter record for executing vapor pressure equation. Data of
	 * refrigerant "R-134a" is taken from:
	 *
	 *	R. Tillner-Roth and H.D. Baehr. An International Standard Formulation
	 *	for the Thermodynamic Properties of 1,1,1,2-Tetrafluoroethane (HFC-134a)
	 *	for Temperatures from 170 K to 455 K and Pressures up to 70 MPa. Journal
	 *	of Physical and Chemical Reference Data 1994. 23: p. 657-729.
	 *
	 */
	double refrigerant_R134a_vaporPressure_EoS1[] = {374.18, 4.05629e6,
		-7.686556, 1, 2.311791, 1.5, -2.039554, 2, -3.583758, 4, 0, 0, 0, 0,
		0, 0};
	double refrigerant_R134a_liquidDensity_EoS1[] = {374.18, 1, 1, 518.20, 0,
		884.13, 0.333333333333333, 485.84, 0.666666666666666, 193.29,
		3.333333333333333, 0, 0, 0, 0, 0, 0, 0, 0};

	/*
	 * Define parameter record for executing cubic equation of state. Data of
	 * refrigerant "R-134a" allows checking PRE with custom alpha equation. Data
	 * is taken from:
	 *
	 * 	Takigawa et al. Solubility and viscosity of refrigerant/lubricant
	 *	mixtures: hydrofluorocarbon/alkylbenzene systems. International Journal
	 *	of Refrigeration 2002. 25: p. 1014-1024.
	 *
	 */
	double refrigerant_R134a_vaporPressure_cubic_SRKE[] = {-5, 4.0593e6,
		374.21, 0.32684, -0.0077, 1.0025, 0.50532, -0.04983, 0};
	double refrigerant_R134a_vaporPressure_cubic_PRE_custom[] = {5, 4.0593e6,
		374.21, 0.32684, -0.0077, 1.0025, 0.50532, -0.04983, 0};
	double refrigerant_R134a_vaporPressure_cubic_PRE[] = {10, 4.0593e6, 374.21,
		0.32684, -0.0077, 1.0025, 0.50532, -0.04983, 0};
	double refrigerant_R134a_vaporPressure_cubic_PRSVE[] = {20, 4.0593e6,
		374.21, 0.32684, -0.0077, 1.0025, 0.50532, -0.04983, 0};

	/*
	 * Define parameter record for executing vapor pressure equation. Data of
	 * refrigerant "benzene" is taken from:
	 *
	 *	Goodwin, Robert D. (1988): Benzene Thermophysical Properties from 279
	 *	to 900 K at Pressures to 1000 Bar. In: Journal of Physical and Chemical
	 *	Reference Data 17 (4), S. 1541–1636. DOI: 10.1063/1.555813.
	 *
	 */
	double refrigerant_benzene_vaporPressure_EoS2[] = {561.75, 100000,
		-10.655375280, 23.941912372, 0, -22.388714756, 1, 20.208593271, 2,
		-7.219556515, 3, 4.847283265, 1.7};

	/*
	 * Define parameter record for executing vapor pressure equation. Data of
	 * refrigerant "Benzene" is taken from:
	 *
	 * 	NIST Webbook.
	 *
	 */
	double refrigerant_benzene_vaporPressure_antoine[] = {4.72583, 1660.652,
		-1.461};

	/*
	 * Define parameter record for executing vapor pressure equation. Data of
	 * refrigerant "R-142b" is taken from:
	 *
	 *	McLinden, M. O.: Physical Properties of Alternatives to the Fully
	 *	Halogenated Chlorofluorocarbons.
	 *
	 */
	double refrigerant_R142b_vaporPressure_EoS3[] = {410.25, 1000, -3382.422,
		17.01384, 0, -0.001012149, 1, 3.224924, 1.5};

	// Create structs
	//
	Refrigerant *refrigerant_R134a_EoS1 = newRefrigerant(
		"VaporPressure_EoS1",
		"SaturatedLiquidDensity_EoS1",
		refrigerant_R134a_vaporPressure_EoS1);

	Refrigerant *refrigerant_R134a_cubic_SRKE = newRefrigerant(
		"VaporPressure_EoSCubic",
		"SaturatedLiquidDensity_None",
		refrigerant_R134a_vaporPressure_cubic_SRKE);

	Refrigerant *refrigerant_R134a_cubic_PRE_custom = newRefrigerant(
		"VaporPressure_EoSCubic",
		"SaturatedLiquidDensity_None",
		refrigerant_R134a_vaporPressure_cubic_PRE_custom);

	Refrigerant *refrigerant_R134a_cubic_PRE = newRefrigerant(
		"VaporPressure_EoSCubic",
		"SaturatedLiquidDensity_None",
		refrigerant_R134a_vaporPressure_cubic_PRE);

	Refrigerant *refrigerant_R134a_cubic_PRSVE = newRefrigerant(
		"VaporPressure_EoSCubic",
		"SaturatedLiquidDensity_None",
		refrigerant_R134a_vaporPressure_cubic_PRSVE);

	Refrigerant *refrigerant_benzene_EoS2 = newRefrigerant(
		"VaporPressure_EoS2",
		"SaturatedLiquidDensity_None",
		refrigerant_benzene_vaporPressure_EoS2);

	Refrigerant *refrigerant_benzene_antoine = newRefrigerant(
		"VaporPressure_Antoine",
		"SaturatedLiquidDensity_None",
		refrigerant_benzene_vaporPressure_antoine);

	Refrigerant *refrigerant_R142b_EoS3 = newRefrigerant(
		"VaporPressure_EoS3",
		"SaturatedLiquidDensity_None",
		refrigerant_R142b_vaporPressure_EoS3);

	// Calculate state values
	//
	double T_K = 353.15;


	double psat_R134a_EoS1_Pa = refrigerant_R134a_EoS1->p_sat_T(T_K,
		refrigerant_R134a_vaporPressure_EoS1,
		refrigerant_R134a_EoS1);
	double T_R134a_inv_EoS1_K = refrigerant_R134a_EoS1->T_sat_p(
		psat_R134a_EoS1_Pa,
		refrigerant_R134a_vaporPressure_EoS1,
		refrigerant_R134a_EoS1);
	double dpsat_dT_R134a_EoS1_PaK = refrigerant_R134a_EoS1->dp_sat_dT(T_K,
		refrigerant_R134a_vaporPressure_EoS1,
		refrigerant_R134a_EoS1);


	double psat_R134a_cubic_SRKE_Pa = refrigerant_R134a_cubic_SRKE->p_sat_T(T_K,
		refrigerant_R134a_vaporPressure_cubic_SRKE,
		refrigerant_R134a_cubic_SRKE);
	double T_R134a_inv_cubic_SRKE_K = refrigerant_R134a_cubic_SRKE->T_sat_p(
		psat_R134a_EoS1_Pa,
		refrigerant_R134a_vaporPressure_cubic_SRKE,
		refrigerant_R134a_cubic_SRKE);
	double dpsat_dT_R134a_cubic_SRKE_PaK =
		refrigerant_R134a_cubic_SRKE->dp_sat_dT(
		T_K,
		refrigerant_R134a_vaporPressure_cubic_SRKE,
		refrigerant_R134a_cubic_SRKE);


	double psat_R134a_cubic_PRE_custom_Pa =
		refrigerant_R134a_cubic_PRE_custom->p_sat_T(T_K,
		refrigerant_R134a_vaporPressure_cubic_PRE_custom,
		refrigerant_R134a_cubic_PRE_custom);
	double T_R134a_inv_cubic_PRE_custom_K =
		refrigerant_R134a_cubic_PRE_custom->T_sat_p(
		psat_R134a_EoS1_Pa,
		refrigerant_R134a_vaporPressure_cubic_PRE_custom,
		refrigerant_R134a_cubic_PRE_custom);
	double dpsat_dT_R134a_cubic_PRE_custom_PaK =
		refrigerant_R134a_cubic_PRE_custom->dp_sat_dT(
		T_K,
		refrigerant_R134a_vaporPressure_cubic_PRE_custom,
		refrigerant_R134a_cubic_PRE_custom);


	double psat_R134a_cubic_PRE_Pa =
		refrigerant_R134a_cubic_PRE->p_sat_T(T_K,
		refrigerant_R134a_vaporPressure_cubic_PRE,
		refrigerant_R134a_cubic_PRE);
	double T_R134a_inv_cubic_PRE_K =
		refrigerant_R134a_cubic_PRE->T_sat_p(
		psat_R134a_EoS1_Pa,
		refrigerant_R134a_vaporPressure_cubic_PRE,
		refrigerant_R134a_cubic_PRE);
	double dpsat_dT_R134a_cubic_PRE_PaK =
		refrigerant_R134a_cubic_PRE->dp_sat_dT(
		T_K,
		refrigerant_R134a_vaporPressure_cubic_PRE,
		refrigerant_R134a_cubic_PRE);


	double psat_R134a_cubic_PRSVE_Pa =
		refrigerant_R134a_cubic_PRSVE->p_sat_T(T_K,
		refrigerant_R134a_vaporPressure_cubic_PRSVE,
		refrigerant_R134a_cubic_PRSVE);
	double T_R134a_inv_cubic_PRSVE_K =
		refrigerant_R134a_cubic_PRSVE->T_sat_p(
		psat_R134a_EoS1_Pa,
		refrigerant_R134a_vaporPressure_cubic_PRSVE,
		refrigerant_R134a_cubic_PRSVE);
	double dpsat_dT_R134a_cubic_PRSVE_PaK =
		refrigerant_R134a_cubic_PRSVE->dp_sat_dT(
		T_K,
		refrigerant_R134a_vaporPressure_cubic_PRSVE,
		refrigerant_R134a_cubic_PRSVE);


	double psat_benzene_EoS2_Pa = refrigerant_benzene_EoS2->p_sat_T(T_K,
		refrigerant_benzene_vaporPressure_EoS2,
		refrigerant_benzene_EoS2);
	double T_benzene_inv_EoS2_K = refrigerant_benzene_EoS2->T_sat_p(
		psat_benzene_EoS2_Pa,
		refrigerant_benzene_vaporPressure_EoS2,
		refrigerant_benzene_EoS2);
	double dpsat_dT_benzene_EoS2_PaK = refrigerant_benzene_EoS2->dp_sat_dT(T_K,
		refrigerant_benzene_vaporPressure_EoS2,
		refrigerant_benzene_EoS2);


	double psat_benzene_antoine_Pa = refrigerant_benzene_antoine->p_sat_T(T_K,
		refrigerant_benzene_vaporPressure_antoine,
		refrigerant_benzene_antoine);
	double T_benzene_inv_antoine_K = refrigerant_benzene_antoine->T_sat_p(
		psat_benzene_antoine_Pa,
		refrigerant_benzene_vaporPressure_antoine,
		refrigerant_benzene_antoine);
	double dpsat_dT_benzene_antoine_PaK =
		refrigerant_benzene_antoine->dp_sat_dT(T_K,
		refrigerant_benzene_vaporPressure_antoine,
		refrigerant_benzene_antoine);


	double psat_R142b_EoS3_Pa = refrigerant_R142b_EoS3->p_sat_T(T_K,
		refrigerant_R142b_vaporPressure_EoS3,
		refrigerant_R142b_EoS3);
	double T_R142b_inv_EoS3_K = refrigerant_R142b_EoS3->T_sat_p(
		psat_R142b_EoS3_Pa,
		refrigerant_R142b_vaporPressure_EoS3,
		refrigerant_R142b_EoS3);
	double dpsat_dT_R142b_EoS3_PaK = refrigerant_R142b_EoS3->dp_sat_dT(T_K,
		refrigerant_R142b_vaporPressure_EoS3,
		refrigerant_R142b_EoS3);


	double rho_l_R134a_EoS1_kgm3 = refrigerant_R134a_EoS1->rho_l_T(T_K,
		refrigerant_R134a_liquidDensity_EoS1);
	double drho_l_dT_R134a_EoS1_kgm3K = refrigerant_R134a_EoS1->drho_l_dT(T_K,
		refrigerant_R134a_liquidDensity_EoS1);

	// Print calculated values
	//
	printf("\n\n##\n##\nCreated struct for: \"R-134a\".");
	printf("\n##\n##\nApproach vapor: \"EoS - 1\".\n##");
	printf("\nFor T = %f K, vapor pressure results in p = %f Pa.",
		T_K, psat_R134a_EoS1_Pa);
	printf("\nFor p = %f Pa, saturation temperature results in T = %f K.",
		psat_R134a_EoS1_Pa, T_R134a_inv_EoS1_K);
	printf("\nFor T = %f K, derivative of vapor pressure wrt. "
		"temperature results in dp_dT = %f Pa/K.",
		T_K, dpsat_dT_R134a_EoS1_PaK);

	printf("\n##\n##\nApproach vapor: \"Cubic - SRKE\".\n##");
	printf("\nFor T = %f K, vapor pressure results in p = %f Pa.",
		T_K, psat_R134a_cubic_SRKE_Pa);
	printf("\nFor p = %f Pa, saturation temperature results in T = %f K.",
		psat_R134a_cubic_SRKE_Pa, T_R134a_inv_cubic_SRKE_K);
	printf("\nFor T = %f K, derivative of vapor pressure wrt. "
		"temperature results in dp_dT = %f Pa/K.",
		T_K, dpsat_dT_R134a_cubic_SRKE_PaK);

	printf("\n##\n##\nApproach vapor: \"Cubic - PRE custom\".\n##");
	printf("\nFor T = %f K, vapor pressure results in p = %f Pa.",
		T_K, psat_R134a_cubic_PRE_custom_Pa);
	printf("\nFor p = %f Pa, saturation temperature results in T = %f K.",
		psat_R134a_cubic_PRE_custom_Pa, T_R134a_inv_cubic_PRE_custom_K);
	printf("\nFor T = %f K, derivative of vapor pressure wrt. "
		"temperature results in dp_dT = %f Pa/K.",
		T_K, dpsat_dT_R134a_cubic_PRE_custom_PaK);

	printf("\n##\n##\nApproach vapor: \"Cubic - PRE\".\n##");
	printf("\nFor T = %f K, vapor pressure results in p = %f Pa.",
		T_K, psat_R134a_cubic_PRE_Pa);
	printf("\nFor p = %f Pa, saturation temperature results in T = %f K.",
		psat_R134a_cubic_PRE_Pa, T_R134a_inv_cubic_PRE_K);
	printf("\nFor T = %f K, derivative of vapor pressure wrt. "
		"temperature results in dp_dT = %f Pa/K.",
		T_K, dpsat_dT_R134a_cubic_PRE_PaK);

	printf("\n##\n##\nApproach vapor: \"Cubic - PRSVE\".\n##");
	printf("\nFor T = %f K, vapor pressure results in p = %f Pa.",
		T_K, psat_R134a_cubic_PRSVE_Pa);
	printf("\nFor p = %f Pa, saturation temperature results in T = %f K.",
		psat_R134a_cubic_PRSVE_Pa, T_R134a_inv_cubic_PRSVE_K);
	printf("\nFor T = %f K, derivative of vapor pressure wrt. "
		"temperature results in dp_dT = %f Pa/K.",
		T_K, dpsat_dT_R134a_cubic_PRSVE_PaK);

	printf("\n##\n##\nApproach liquid density: \"EoS - 1\".\n##");
	printf("\nFor T = %f K, saturated liquid density results in rho_l = %f "
		"kg/m3.", T_K, rho_l_R134a_EoS1_kgm3);
	printf("\nFor T = %f K, derivative of saturated liquid density "
		"wrt. temperature results in drho_l_dT = %f kg/m3/K.", T_K,
		drho_l_dT_R134a_EoS1_kgm3K);


	printf("\n\n##\n##\nCreated struct for: \"Benzene\".");
	printf("\n##\n##\nApproach vapor: \"EoS - 2\".\n##");
	printf("\nFor T = %f K, vapor pressure results in p = %f Pa.",
		T_K, psat_benzene_EoS2_Pa);
	printf("\nFor p = %f Pa, saturation temperature results in T = %f K.",
		psat_benzene_EoS2_Pa, T_benzene_inv_EoS2_K);
	printf("\nFor T = %f K, derivative of vapor pressure wrt. "
		"temperature results in dp_dT = %f Pa/K.",
		T_K, dpsat_dT_benzene_EoS2_PaK);

	printf("\n##\n##\nApproach vapor: \"Antoine\".\n##");
	printf("\nFor T = %f K, vapor pressure results in p = %f Pa.",
		T_K, psat_benzene_antoine_Pa);
	printf("\nFor p = %f Pa, saturation temperature results in T = %f K.",
		psat_benzene_antoine_Pa, T_benzene_inv_antoine_K);
	printf("\nFor T = %f K, derivative of vapor pressure wrt. "
		"temperature results in dp_dT = %f Pa/K.",
		T_K, dpsat_dT_benzene_antoine_PaK);


	printf("\n\n##\n##\nCreated struct for: \"R-142b\".");
	printf("\n##\n##\nApproach vapor: \"EoS - 3\".\n##");
	printf("\nFor T = %f K, vapor pressure results in p = %f Pa.",
		T_K, psat_R142b_EoS3_Pa);
	printf("\nFor p = %f Pa, saturation temperature results in T = %f K.",
		psat_R142b_EoS3_Pa, T_R142b_inv_EoS3_K);
	printf("\nFor T = %f K, derivative of vapor pressure wrt. "
		"temperature results in dp_dT = %f Pa/K.",
		T_K, dpsat_dT_R142b_EoS3_PaK);

	// Free memory that is allocate for structures
	//
	delRefrigerant(refrigerant_R134a_EoS1);
	delRefrigerant(refrigerant_R134a_cubic_SRKE);
	delRefrigerant(refrigerant_R134a_cubic_PRE_custom);
	delRefrigerant(refrigerant_R134a_cubic_PRE);
	delRefrigerant(refrigerant_R134a_cubic_PRSVE);
	delRefrigerant(refrigerant_benzene_EoS2);
	delRefrigerant(refrigerant_benzene_antoine);
	delRefrigerant(refrigerant_R142b_EoS3);

    return EXIT_SUCCESS;
}
