///////////////////////////////////////
// test_refrigerant_vaporPressure.c //
///////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "refrigerant_cubicEoS.h"
#include "refrigerant_vaporPressure.h"


int main() {
	///////////////////////////////////////////////////////////////
	// Check fluid R-134a and thus vapor pressure equation EoS 1 //
	///////////////////////////////////////////////////////////////
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
	double refrigerant_R134a_par[] = {374.18, 4.05629e6, -7.686556, 1, 2.311791,
		1.5, -2.039554, 2, -3.583758, 4, 0, 0, 0, 0, 0, 0};

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
	double refrigerant_R134a_par_cubic_SRKE[] = {-5, 4.0593e6, 374.21, 0.32684,
		-0.0077, 1.0025, 0.50532, -0.04983, 0};
	double refrigerant_R134a_par_cubic_PRE_custom[] = {5, 4.0593e6, 374.21,
		0.32684, -0.0077, 1.0025, 0.50532, -0.04983, 0};
	double refrigerant_R134a_par_cubic_PRE[] = {10, 4.0593e6, 374.21, 0.32684,
		-0.0077, 1.0025, 0.50532, -0.04983, 0};
	double refrigerant_R134a_par_cubic_PRSVE[] = {20, 4.0593e6, 374.21, 0.32684,
		-0.0077, 1.0025, 0.50532, -0.04983, 0};

	// Calculate vapor pressure
	//
	double T_K = 353.15;

	double psat_R134a_eos1_Pa = refrigerant_p_sat_eos1(T_K,
		refrigerant_R134a_par);
	double T_R134a_inv_eos1_K = refrigerant_T_sat_eos1(psat_R134a_eos1_Pa,
		refrigerant_R134a_par);
	double dpsat_dT_R134a_eos1_PaK = refrigerant_dp_sat_dT_eos1(T_K,
		refrigerant_R134a_par);

	double psat_R134a_plus_eos1_Pa = refrigerant_p_sat_eos1(T_K+0.0001,
		refrigerant_R134a_par);
	double psat_R134a_minus_eos1_Pa = refrigerant_p_sat_eos1(T_K-0.0001,
		refrigerant_R134a_par);
	double dpsat_dT_R134a_eos1_num_PaK =
		(psat_R134a_plus_eos1_Pa - psat_R134a_minus_eos1_Pa) / 0.0002;


	double psat_R134a_SRKE_Pa = refrigerant_p_sat_cubic(T_K,
		refrigerant_cubic_SRKE_parameters,
		refrigerant_cubic_SRKE_gen_parameters,
		refrigerant_cubic_SRKE_fugacity_coefficient_pure,
		refrigerant_R134a_par_cubic_SRKE);
	double T_R134a_inv_SRKE_K = refrigerant_T_sat_cubic(psat_R134a_SRKE_Pa,
		refrigerant_cubic_SRKE_parameters,
		refrigerant_cubic_SRKE_gen_parameters,
		refrigerant_cubic_SRKE_fugacity_coefficient_pure,
		refrigerant_R134a_par_cubic_SRKE);
	double dpsat_dT_R134a_SRKE_PaK = refrigerant_dp_sat_dT_cubic(T_K,
		refrigerant_cubic_SRKE_parameters,
		refrigerant_cubic_SRKE_gen_parameters,
		refrigerant_cubic_SRKE_fugacity_coefficient_pure,
		refrigerant_R134a_par_cubic_SRKE);

	double psat_R134a_plus_SRKE_Pa = refrigerant_p_sat_cubic(T_K+1e-8,
		refrigerant_cubic_SRKE_parameters,
		refrigerant_cubic_SRKE_gen_parameters,
		refrigerant_cubic_SRKE_fugacity_coefficient_pure,
		refrigerant_R134a_par_cubic_SRKE);
	double psat_R134a_minus_SRKE_Pa = refrigerant_p_sat_cubic(T_K-1e-8,
		refrigerant_cubic_SRKE_parameters,
		refrigerant_cubic_SRKE_gen_parameters,
		refrigerant_cubic_SRKE_fugacity_coefficient_pure,
		refrigerant_R134a_par_cubic_SRKE);
	double dpsat_dT_R134a_SRKE_num_PaK =
		(psat_R134a_plus_SRKE_Pa - psat_R134a_minus_SRKE_Pa) / (2e-8);


	double psat_R134a_PRE_custom_Pa = refrigerant_p_sat_cubic(T_K,
		refrigerant_cubic_PRE_parameters,
		refrigerant_cubic_PRE_gen_parameters,
		refrigerant_cubic_PRE_fugacity_coefficient_pure,
		refrigerant_R134a_par_cubic_PRE_custom);
	double T_R134a_inv_PRE_custom_K = refrigerant_T_sat_cubic(
		psat_R134a_PRE_custom_Pa,
		refrigerant_cubic_PRE_parameters,
		refrigerant_cubic_PRE_gen_parameters,
		refrigerant_cubic_PRE_fugacity_coefficient_pure,
		refrigerant_R134a_par_cubic_PRE_custom);
	double dpsat_dT_R134a_PRE_custom_PaK = refrigerant_dp_sat_dT_cubic(T_K,
		refrigerant_cubic_PRE_parameters,
		refrigerant_cubic_PRE_gen_parameters,
		refrigerant_cubic_PRE_fugacity_coefficient_pure,
		refrigerant_R134a_par_cubic_PRE_custom);

	double psat_R134a_plus_PRE_custom_Pa = refrigerant_p_sat_cubic(T_K+1e-8,
		refrigerant_cubic_PRE_parameters,
		refrigerant_cubic_PRE_gen_parameters,
		refrigerant_cubic_PRE_fugacity_coefficient_pure,
		refrigerant_R134a_par_cubic_PRE_custom);
	double psat_R134a_minus_PRE_custom_Pa = refrigerant_p_sat_cubic(T_K-1e-8,
		refrigerant_cubic_PRE_parameters,
		refrigerant_cubic_PRE_gen_parameters,
		refrigerant_cubic_PRE_fugacity_coefficient_pure,
		refrigerant_R134a_par_cubic_PRE_custom);
	double dpsat_dT_R134a_PRE_custom_num_PaK =
		(psat_R134a_plus_PRE_custom_Pa - psat_R134a_minus_PRE_custom_Pa) /
		(2e-8);


	double psat_R134a_PRE_Pa = refrigerant_p_sat_cubic(T_K,
		refrigerant_cubic_PRE_parameters,
		refrigerant_cubic_PRE_gen_parameters,
		refrigerant_cubic_PRE_fugacity_coefficient_pure,
		refrigerant_R134a_par_cubic_PRE);
	double T_R134a_inv_PRE_K = refrigerant_T_sat_cubic(psat_R134a_PRE_Pa,
		refrigerant_cubic_PRE_parameters,
		refrigerant_cubic_PRE_gen_parameters,
		refrigerant_cubic_PRE_fugacity_coefficient_pure,
		refrigerant_R134a_par_cubic_PRE);
	double dpsat_dT_R134a_PRE_PaK = refrigerant_dp_sat_dT_cubic(T_K,
		refrigerant_cubic_PRE_parameters,
		refrigerant_cubic_PRE_gen_parameters,
		refrigerant_cubic_PRE_fugacity_coefficient_pure,
		refrigerant_R134a_par_cubic_PRE);

	double psat_R134a_plus_PRE_Pa = refrigerant_p_sat_cubic(T_K+1e-8,
		refrigerant_cubic_PRE_parameters,
		refrigerant_cubic_PRE_gen_parameters,
		refrigerant_cubic_PRE_fugacity_coefficient_pure,
		refrigerant_R134a_par_cubic_PRE);
	double psat_R134a_minus_PRE_Pa = refrigerant_p_sat_cubic(T_K-1e-8,
		refrigerant_cubic_PRE_parameters,
		refrigerant_cubic_PRE_gen_parameters,
		refrigerant_cubic_PRE_fugacity_coefficient_pure,
		refrigerant_R134a_par_cubic_PRE);
	double dpsat_dT_R134a_PRE_num_PaK =
		(psat_R134a_plus_PRE_Pa - psat_R134a_minus_PRE_Pa) / (2e-8);


	double psat_R134a_PRSVE_Pa = refrigerant_p_sat_cubic(T_K,
		refrigerant_cubic_PRSVE_parameters,
		refrigerant_cubic_PRSVE_gen_parameters,
		refrigerant_cubic_PRSVE_fugacity_coefficient_pure,
		refrigerant_R134a_par_cubic_PRSVE);
	double T_R134a_inv_PRSVE_K = refrigerant_T_sat_cubic(psat_R134a_PRSVE_Pa,
		refrigerant_cubic_PRSVE_parameters,
		refrigerant_cubic_PRSVE_gen_parameters,
		refrigerant_cubic_PRSVE_fugacity_coefficient_pure,
		refrigerant_R134a_par_cubic_PRSVE);
	double dpsat_dT_R134a_PRSVE_PaK = refrigerant_dp_sat_dT_cubic(T_K,
		refrigerant_cubic_PRSVE_parameters,
		refrigerant_cubic_PRSVE_gen_parameters,
		refrigerant_cubic_PRSVE_fugacity_coefficient_pure,
		refrigerant_R134a_par_cubic_PRSVE);

	double psat_R134a_plus_PRSVE_Pa = refrigerant_p_sat_cubic(T_K+1e-8,
		refrigerant_cubic_PRSVE_parameters,
		refrigerant_cubic_PRSVE_gen_parameters,
		refrigerant_cubic_PRSVE_fugacity_coefficient_pure,
		refrigerant_R134a_par_cubic_PRSVE);
	double psat_R134a_minus_PRSVE_Pa = refrigerant_p_sat_cubic(T_K-1e-8,
		refrigerant_cubic_PRSVE_parameters,
		refrigerant_cubic_PRSVE_gen_parameters,
		refrigerant_cubic_PRSVE_fugacity_coefficient_pure,
		refrigerant_R134a_par_cubic_PRSVE);
	double dpsat_dT_R134a_PRSVE_num_PaK =
		(psat_R134a_plus_PRSVE_Pa - psat_R134a_minus_PRSVE_Pa) / (2e-8);

	// Print calculated values
	//
	printf("\n\n##\n##\nSelected refrigerant: \"R-134a\".");
	printf("\n##\n##\nApproach: \"EoS - 1\".\n##");
	printf("\nFor T = %f K, vapor pressure results in p = %f Pa.",
		T_K, psat_R134a_eos1_Pa);
	printf("\nFor p = %f Pa, saturation temperature results in T = %f K.",
		psat_R134a_eos1_Pa, T_R134a_inv_eos1_K);
	printf("\nFor T = %f K, analytical derivative of vapor pressure wrt. "
		"temperature results in dp_dT = %f Pa/K.",
		T_K, dpsat_dT_R134a_eos1_PaK);
	printf("\nFor T = %f K, numerical derivative of vapor pressure wrt. "
		"temperature results in dp_dT = %f Pa/K.",
		T_K, dpsat_dT_R134a_eos1_num_PaK);

	printf("\n##\nApproach: \"Cubic - SRKE\".\n##");
	printf("\nFor T = %f K, vapor pressure results in p = %f Pa.",
		T_K, psat_R134a_SRKE_Pa);
	printf("\nFor p = %f Pa, saturation temperature results in T = %f K.",
		psat_R134a_SRKE_Pa, T_R134a_inv_SRKE_K);
	printf("\nFor T = %f K, analytical derivative of vapor pressure wrt. "
		"temperature results in dp_dT = %f Pa/K.",
		T_K, dpsat_dT_R134a_SRKE_PaK);
	printf("\nFor T = %f K, numerical derivative of vapor pressure wrt. "
		"temperature results in dp_dT = %f Pa/K.",
		T_K, dpsat_dT_R134a_SRKE_num_PaK);

	printf("\n##\nApproach: \"Cubic - PRE w. custom alpha equation\".\n##");
	printf("\nFor T = %f K, vapor pressure results in p = %f Pa.",
		T_K, psat_R134a_PRE_custom_Pa);
	printf("\nFor p = %f Pa, saturation temperature results in T = %f K.",
		psat_R134a_PRE_custom_Pa, T_R134a_inv_PRE_custom_K);
	printf("\nFor T = %f K, analytical derivative of vapor pressure wrt. "
		"temperature results in dp_dT = %f Pa/K.",
		T_K, dpsat_dT_R134a_PRE_custom_PaK);
	printf("\nFor T = %f K, numerical derivative of vapor pressure wrt. "
		"temperature results in dp_dT = %f Pa/K.",
		T_K, dpsat_dT_R134a_PRE_custom_num_PaK);

	printf("\n##\nApproach: \"Cubic - PRE\".\n##");
	printf("\nFor T = %f K, vapor pressure results in p = %f Pa.",
		T_K, psat_R134a_PRE_Pa);
	printf("\nFor p = %f Pa, saturation temperature results in T = %f K.",
		psat_R134a_PRE_Pa, T_R134a_inv_PRE_K);
	printf("\nFor T = %f K, analytical derivative of vapor pressure wrt. "
		"temperature results in dp_dT = %f Pa/K.",
		T_K, dpsat_dT_R134a_PRE_PaK);
	printf("\nFor T = %f K, numerical derivative of vapor pressure wrt. "
		"temperature results in dp_dT = %f Pa/K.",
		T_K, dpsat_dT_R134a_PRE_num_PaK);

	printf("\n##\nApproach: \"Cubic - PRSVE\".\n##");
	printf("\nFor T = %f K, vapor pressure results in p = %f Pa.",
		T_K, psat_R134a_PRSVE_Pa);
	printf("\nFor p = %f Pa, saturation temperature results in T = %f K.",
		psat_R134a_PRSVE_Pa, T_R134a_inv_PRSVE_K);
	printf("\nFor T = %f K, analytical derivative of vapor pressure wrt. "
		"temperature results in dp_dT = %f Pa/K.",
		T_K, dpsat_dT_R134a_PRSVE_PaK);
	printf("\nFor T = %f K, numerical derivative of vapor pressure wrt. "
		"temperature results in dp_dT = %f Pa/K.",
		T_K, dpsat_dT_R134a_PRSVE_num_PaK);


	////////////////////////////////////////////////////////////////
	// Check fluid benzene and thus vapor pressure equation EoS 2 //
	////////////////////////////////////////////////////////////////
	/*
	 * Define parameter record for executing vapor pressure equation. Data of
	 * refrigerant "benzene" is taken from:
	 *
	 *	Goodwin, Robert D. (1988): Benzene Thermophysical Properties from 279
	 *	to 900 K at Pressures to 1000 Bar. In: Journal of Physical and Chemical
	 *	Reference Data 17 (4), S. 1541–1636. DOI: 10.1063/1.555813.
	 *
	 */
	double refrigerant_benzene_par[] = {561.75, 100000, -10.655375280,
		23.941912372, 0, -22.388714756, 1, 20.208593271, 2, -7.219556515, 3,
		4.847283265, 1.7};

	/*
	 * Define parameter record for executing vapor pressure equation. Data of
	 * refrigerant "Benzene" is taken from:
	 *
	 * 	NIST Webbook.
	 *
	 */
	double refrigerant_benzene_par_antoine[] = {4.72583, 1660.652, -1.461};

	//
	// Calculate vapor pressure
	//
	double psat_benzene_eos2_Pa = refrigerant_p_sat_eos2(T_K,
		refrigerant_benzene_par);
	double T_benzene_inv_eos2_K = refrigerant_T_sat_eos2(psat_benzene_eos2_Pa,
		refrigerant_benzene_par);
	double dpsat_dT_benzene_eos2_PaK = refrigerant_dp_sat_dT_eos2(T_K,
		refrigerant_benzene_par);

	double psat_benzene_plus_eos2_Pa = refrigerant_p_sat_eos2(T_K+0.0001,
		refrigerant_benzene_par);
	double psat_benzene_minus_eos2_Pa = refrigerant_p_sat_eos2(T_K-0.0001,
		refrigerant_benzene_par);
	double dpsat_dT_benzene_eos2_num_PaK =
		(psat_benzene_plus_eos2_Pa - psat_benzene_minus_eos2_Pa) / 0.0002;


	double psat_benzene_antoine_Pa = refrigerant_p_sat_antoine(T_K,
		refrigerant_benzene_par_antoine);
	double T_benzene_inv_antoine_K = refrigerant_T_sat_antoine(
		psat_benzene_antoine_Pa, refrigerant_benzene_par_antoine);
	double dpsat_dT_benzene_antoine_PaK = refrigerant_dp_sat_dT_antoine(T_K,
		refrigerant_benzene_par_antoine);

	double psat_benzene_plus_antoine_Pa = refrigerant_p_sat_antoine(T_K+0.0001,
		refrigerant_benzene_par_antoine);
	double psat_benzene_minus_antoine_Pa = refrigerant_p_sat_antoine(T_K-0.0001,
		refrigerant_benzene_par_antoine);
	double dpsat_dT_benzene_antoine_num_PaK = (psat_benzene_plus_antoine_Pa -
		psat_benzene_minus_antoine_Pa) / 0.0002;

	// Print calculated values
	//
	printf("\n\n##\n##\nSelected refrigerant: \"Benzene\".");
	printf("\n##\n##\nApproach: \"EoS - 2\".\n##");
	printf("\nFor T = %f K, vapor pressure results in p = %f Pa.",
		T_K, psat_benzene_eos2_Pa);
	printf("\nFor p = %f Pa, saturation temperature results in T = %f K.",
		psat_benzene_eos2_Pa, T_benzene_inv_eos2_K);
	printf("\nFor T = %f K, analytical derivative of vapor pressure wrt. "
		"temperature results in dp_dT = %f Pa/K.",
		T_K, dpsat_dT_benzene_eos2_PaK);
	printf("\nFor T = %f K, numerical derivative of vapor pressure wrt. "
		"temperature results in dp_dT = %f Pa/K.",
		T_K, dpsat_dT_benzene_eos2_num_PaK);

	printf("\n##\nApproach: \"Antoine\".\n##");
	printf("\nFor T = %f K, vapor pressure results in p = %f Pa.",
		T_K, psat_benzene_antoine_Pa);
	printf("\nFor p = %f Pa, saturation temperature results in T = %f K.",
		psat_benzene_antoine_Pa, T_benzene_inv_antoine_K);
	printf("\nFor T = %f K, analytical derivative of vapor pressure wrt. "
		"temperature results in dp_dT = %f Pa/K.", T_K,
		dpsat_dT_benzene_antoine_PaK);
	printf("\nFor T = %f K, numerical derivative of vapor pressure wrt. "
		"temperature results in dp_dT = %f Pa/K.", T_K,
		dpsat_dT_benzene_antoine_num_PaK);


	////////////////////////////////////////////////////////////////
	// Check fluid benzene and thus vapor pressure equation EoS 3 //
	////////////////////////////////////////////////////////////////
	/*
	 * Define parameter record for executing vapor pressure equation. Data of
	 * refrigerant "R-142b" is taken from:
	 *
	 *	McLinden, M. O.: Physical Properties of Alternatives to the Fully
	 *	Halogenated Chlorofluorocarbons.
	 *
	 */
	double refrigerant_R142b_par[] = {410.25, 1000, -3382.422, 17.01384, 0,
		-0.001012149, 1, 3.224924, 1.5};

	//
	// Calculate vapor pressure
	//
	double psat_R142b_eos3_Pa = refrigerant_p_sat_eos3(T_K,
		refrigerant_R142b_par);
	double T_R142b_inv_eos3_K = refrigerant_T_sat_eos3(psat_R142b_eos3_Pa,
		refrigerant_R142b_par);
	double dpsat_dT_R142b_eos3_PaK = refrigerant_dp_sat_dT_eos3(T_K,
		refrigerant_R142b_par);

	double psat_R142b_plus_eos3_Pa = refrigerant_p_sat_eos3(T_K+0.0001,
		refrigerant_R142b_par);
	double psat_R142b_minus_eos3_Pa = refrigerant_p_sat_eos3(T_K-0.0001,
		refrigerant_R142b_par);
	double dpsat_dT_R142b_eos3_num_PaK =
		(psat_R142b_plus_eos3_Pa - psat_R142b_minus_eos3_Pa) / 0.0002;

	// Print calculated values
	//
	printf("\n\n##\n##\nSelected refrigerant: \"R-142b\".");
	printf("\n##\n##\nApproach: \"EoS - 3\".\n##");
	printf("\nFor T = %f K, vapor pressure results in p = %f Pa.",
		T_K, psat_R142b_eos3_Pa);
	printf("\nFor p = %f Pa, saturation temperature results in T = %f K.",
		psat_R142b_eos3_Pa, T_R142b_inv_eos3_K);
	printf("\nFor T = %f K, analytical derivative of vapor pressure wrt. "
		"temperature results in dp_dT = %f Pa/K.",
		T_K, dpsat_dT_R142b_eos3_PaK);
	printf("\nFor T = %f K, numerical derivative of vapor pressure wrt. "
		"temperature results in dp_dT = %f Pa/K.",
		T_K, dpsat_dT_R142b_eos3_num_PaK);

    return EXIT_SUCCESS;
}
