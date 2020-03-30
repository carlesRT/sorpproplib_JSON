///////////////////////////////////////
// test_refrigerant_vaporPressure.c //
///////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "refrigerant_vaporPressure.h"


int main() {
	// Define parameter record for executing vapor pressure equation. Data for
	// refrigerant "R-134a" is taken from:
	//
	// 	R. Tillner-Roth and H.D. Baehr. An International Standard Formulation
	//	for the Thermodynamic Properties of 1,1,1,2-Tetrafluoroethane (HFC-134a)
	//	for Temperatures from 170 K to 455 K and Pressures up to 70 MPa. Journal
	//	of Physical and Chemical Reference Data 1994. 23: p. 657-729.
	//
	double refrigerant_R134a_par[] = {374.18, 4.05629e6, -7.686556, 1, 2.311791,
		1.5, -2.039554, 2, -3.583758, 4, 0, 0, 0, 0};

	// Define parameter record for executing vapor pressure equation. Data for
	// refrigerant "Benzene" is taken from:
	//
	// 	NIST Webbook.
	//
	double refrigerant_par_benzene[] = {4.72583, 1660.652, -1.461};


	// Define some input values for calculating vapor pressure
	//
	double T_K = 353.15;


	// Calculate vapor pressure
	//
	double psat_R134a_Pa = refrigerant_p_sat(T_K, refrigerant_R134a_par);
	double T_R134a_inv_K = refrigerant_T_sat(psat_R134a_Pa,
		refrigerant_R134a_par);
	double psat_R134a_Pa_plus = refrigerant_p_sat(T_K+0.0001,
		refrigerant_R134a_par);
	double psat_R134a_Pa_minus = refrigerant_p_sat(T_K-0.0001,
		refrigerant_R134a_par);
	double dpsat_dT_R134a_PaK = refrigerant_dp_sat_dT(T_K,
		refrigerant_R134a_par);
	double dpsat_dT_R134a_PaK_num = (psat_R134a_Pa_plus - psat_R134a_Pa_minus) /
		0.0002;

	double psat_benzene_Pa = refrigerant_p_sat_antoine(T_K,
		refrigerant_par_benzene);
	double T_benzene_inv_K = refrigerant_T_sat_antoine(psat_benzene_Pa,
		refrigerant_par_benzene);
	double psat_benzene_Pa_plus = refrigerant_p_sat_antoine(T_K+0.0001,
		refrigerant_par_benzene);
	double psat_benzene_Pa_minus = refrigerant_p_sat_antoine(T_K-0.0001,
		refrigerant_par_benzene);
	double dpsat_dT_benzene_PaK = refrigerant_dp_sat_dT_antoine(T_K,
		refrigerant_par_benzene);
	double dpsat_dT_benzene_PaK_num = (psat_benzene_Pa_plus -
		psat_benzene_Pa_minus) / 0.0002;


	// Print calculated values
	//
	printf("\n\n##\n##\nSelected refrigerant: \"R-134a\".");
	printf("\n\nFor T = %f K, vapor pressure results in p = %f Pa.",
		T_K, psat_R134a_Pa);
	printf("\nFor p = %f Pa, saturation temperature results in T = %f K.",
		psat_R134a_Pa, T_R134a_inv_K);
	printf("\nFor T = %f K, analytical derivative of vapor pressure wrt. "
		"temperature results in dp_dT = %f Pa/K.", T_K, dpsat_dT_R134a_PaK);
	printf("\nFor T = %f K, numerical derivative of vapor pressure wrt. "
		"temperature results in dp_dT = %f Pa/K.", T_K, dpsat_dT_R134a_PaK_num);

	printf("\n\n##\n##\nSelected refrigerant: \"Benzene\".");
	printf("\n\nFor T = %f K, vapor pressure results in p = %f Pa.",
		T_K, psat_benzene_Pa);
	printf("\nFor p = %f Pa, saturation temperature results in T = %f K.",
		psat_benzene_Pa, T_benzene_inv_K);
	printf("\nFor T = %f K, analytical derivative of vapor pressure wrt. "
		"temperature results in dp_dT = %f Pa/K.", T_K, dpsat_dT_benzene_PaK);
	printf("\nFor T = %f K, numerical derivative of vapor pressure wrt. "
		"temperature results in dp_dT = %f Pa/K.", T_K,
		dpsat_dT_benzene_PaK_num);

    return EXIT_SUCCESS;
}