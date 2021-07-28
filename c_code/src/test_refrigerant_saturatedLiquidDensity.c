///////////////////////////////////////////////
// test_refrigerant_saturatedLiquidDensity.c //
///////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "refrigerant_saturatedLiquidDensity.h"


int main() {
	///////////////////////////////////////////////////////////////
	// Check fluid R-134a and thus liquid density equation EoS 1 //
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
	double refrigerant_R134a_par[] = {374.18, 1, 1, 518.20, 0, 884.13,
		0.333333333333333, 485.84, 0.666666666666666, 193.29,
		3.333333333333333, 0, 0, 0, 0, 0, 0, 0, 0};

	// Calculate saturated liquid density
	//
	double T_K = 246.78;

	double rho_l_R134a_kgm3 = refrigerant_rho_l_eos1(T_K,
		refrigerant_R134a_par);
	double drho_l_dT_R134a_kgm3K = refrigerant_drho_l_dT_eos1(T_K,
		refrigerant_R134a_par);

	double rho_l_R134a_plus_kgm3 = refrigerant_rho_l_eos1(T_K+0.0001,
		refrigerant_R134a_par);
	double rho_l_R134a_minus_kgm3 = refrigerant_rho_l_eos1(T_K-0.0001,
		refrigerant_R134a_par);
	double drho_l_dT_R134a_num_kgm3K = (rho_l_R134a_plus_kgm3 -
		rho_l_R134a_minus_kgm3) / 0.0002;

	// Print calculated values
	//
	printf("\n\n##\n##\nSelected refrigerant: \"R-134a\".");
	printf("\n##\n##\nApproach: \"EoS - 1\".\n##");
	printf("\nFor T = %f K, saturated liquid density results in rho_l = %f "
		"kg/m3.", T_K, rho_l_R134a_kgm3);
	printf("\nFor T = %f K, analytical derivative of saturated liquid density "
		"wrt. temperature results in drho_l_dT = %f kg/m3/K.", T_K,
		drho_l_dT_R134a_kgm3K);
	printf("\nFor T = %f K, numerical derivative of saturated liquid density "
		"wrt. temperature results in drho_l_dT = %f kg/m3/K.", T_K,
		drho_l_dT_R134a_num_kgm3K);


	////////////////////////////////////////////////////////////
	// Check fluid CO2 and thus liquid density equation EoS 1 //
	////////////////////////////////////////////////////////////
	/*
	 * Define parameter record for executing vapor pressure equation. Data of
	 * refrigerant "CO2" is taken from:
	 *
	 *	Span, Roland; Wagner, Wolfgang (1996): A New Equation of State for
	 *	Carbon Dioxide Covering the Fluid Region from the Triple‐Point
	 *	Temperature to 1100 K at Pressures up to 800 MPa. In: Journal of
	 *	Physical and Chemical Reference Data 25 (6), S. 1509–1596.
	 *	DOI: 10.1063/1.555991.
	 *
	 */
	double refrigerant_CO2_par[] = {304.1282, 467.6, -1, 1.9245108, 0.34,
		-0.62385555, 0.5, -0.32731127, 1.666666666666667, 0.39245142,
		1.833333333333333, 0, 0, 0, 0, 0, 0, 0, 0};

	// Calculate saturated liquid density
	//
	double rho_l_CO2_kgm3 = refrigerant_rho_l_eos1(T_K,
		refrigerant_CO2_par);
	double drho_l_dT_CO2_kgm3K = refrigerant_drho_l_dT_eos1(T_K,
		refrigerant_CO2_par);

	double rho_l_CO2_plus_kgm3 = refrigerant_rho_l_eos1(T_K+0.0001,
		refrigerant_CO2_par);
	double rho_l_CO2_minus_kgm3 = refrigerant_rho_l_eos1(T_K-0.0001,
		refrigerant_CO2_par);
	double drho_l_dT_CO2_num_kgm3K = (rho_l_CO2_plus_kgm3 -
		rho_l_CO2_minus_kgm3) / 0.0002;

	// Print calculated values
	//
	printf("\n\n##\n##\nSelected refrigerant: \"CO2\".");
	printf("\n##\n##\nApproach: \"EoS - 1\".\n##");
	printf("\nFor T = %f K, saturated liquid density results in rho_l = %f "
		"kg/m3.", T_K, rho_l_CO2_kgm3);
	printf("\nFor T = %f K, analytical derivative of saturated liquid density "
		"wrt. temperature results in drho_l_dT = %f kg/m3/K.", T_K,
		drho_l_dT_CO2_kgm3K);
	printf("\nFor T = %f K, numerical derivative of saturated liquid density "
		"wrt. temperature results in drho_l_dT = %f kg/m3/K.", T_K,
		drho_l_dT_CO2_num_kgm3K);


	////////////////////////////////////////////////////////////////
	// Check fluid benzene and thus liquid density equation EoS 1 //
	////////////////////////////////////////////////////////////////
	/*
	 * Define parameter record for executing vapor pressure equation. Data of
	 * refrigerant "CO2" is taken from:
	 *
	 *	Span, Roland; Wagner, Wolfgang (1996): A New Equation of State for
	 *	Carbon Dioxide Covering the Fluid Region from the Triple‐Point
	 *	Temperature to 1100 K at Pressures up to 800 MPa. In: Journal of
	 *	Physical and Chemical Reference Data 25 (6), S. 1509–1596.
	 *	DOI: 10.1063/1.555991.
	 *
	 */
	double refrigerant_benzene_par[] = {561.75, 304.629, 1, 1, 0,
		1.9600182, 0.35, 1.0628812, 1, -1.5856640, 2, 2.0926704, 3,
		0, 0, 0, 0, 0, 0};

	// Calculate saturated liquid density
	//
	double rho_l_benzene_kgm3 = refrigerant_rho_l_eos1(T_K,
		refrigerant_benzene_par);
	double drho_l_dT_benzene_kgm3K = refrigerant_drho_l_dT_eos1(T_K,
		refrigerant_benzene_par);

	double rho_l_benzene_plus_kgm3 = refrigerant_rho_l_eos1(T_K+0.0001,
		refrigerant_benzene_par);
	double rho_l_benzene_minus_kgm3 = refrigerant_rho_l_eos1(T_K-0.0001,
		refrigerant_benzene_par);
	double drho_l_dT_benzene_num_kgm3K = (rho_l_benzene_plus_kgm3 -
		rho_l_benzene_minus_kgm3) / 0.0002;

	// Print calculated values
	//
	printf("\n\n##\n##\nSelected refrigerant: \"Benzene\".");
	printf("\n##\n##\nApproach: \"EoS - 1\".\n##");
	printf("\nFor T = %f K, saturated liquid density results in rho_l = %f "
		"kg/m3.", T_K, rho_l_benzene_kgm3);
	printf("\nFor T = %f K, analytical derivative of saturated liquid density "
		"wrt. temperature results in drho_l_dT = %f kg/m3/K.", T_K,
		drho_l_dT_benzene_kgm3K);
	printf("\nFor T = %f K, numerical derivative of saturated liquid density "
		"wrt. temperature results in drho_l_dT = %f kg/m3/K.", T_K,
		drho_l_dT_benzene_num_kgm3K);

    return EXIT_SUCCESS;
}
