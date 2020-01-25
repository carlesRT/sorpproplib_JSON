///////////////////////////////////////
// test_refrigerant_vapourPressure.c //
///////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "refrigerant_vapourPressure.h"


int main() {
	// Define parameter record for executing vapour pressure equation. Data for
	// refrigerant "R-134a" is taken from:
	//
	// 	R. Tillner-Roth and H.D. Baehr. An International Standard Formulation
	//	for the Thermodynamic Properties of 1,1,1,2-Tetrafluoroethane (HFC-134a)
	//	for Temperatures from 170 K to 455 K and Pressures up to 70 MPa. Journal
	//	of Physical and Chemical Reference Data 1994. 23: p. 657-729.
	//
	double refrigerant_par[] = {374.18, 4.05629e6, -7.686556, 1, 2.311791,
		1.5, -2.039554, 2, -3.583758, 4, 0, 0, 0, 0};


	// Define some input values for calculating vapour pressure
	//
	double T_K = 246.78;
	
	
	// Calculate vapour pressure
	//
	double psat_Pa = refrigerant_p_sat(T_K, refrigerant_par);
	double psat_Pa_plus = refrigerant_p_sat(T_K+0.0001, refrigerant_par);
	double psat_Pa_minus = refrigerant_p_sat(T_K-0.0001, refrigerant_par);
	double dpsat_dT_PaK = refrigerant_dp_sat_dT(T_K, refrigerant_par);
	double dpsat_dT_PaK_num = (psat_Pa_plus - psat_Pa_minus) / 0.0002;


	// Print calculated values
	//
	printf("\n\n##\n##\nSelected refrigerant: \"R-134a\".");
	printf("\n\nFor T = %f K, vapour pressure results in p = %f Pa.", 
		T_K, psat_Pa);
	printf("\nFor T = %f K, analytical derivative of vapour pressure wrt. temperature results in dp_dT = %f Pa/K.", 
		T_K, dpsat_dT_PaK);
	printf("\nFor T = %f K, numerical derivative of vapour pressure wrt. temperature results in dp_dT = %f Pa/K.", 
		T_K, dpsat_dT_PaK_num);

    return EXIT_SUCCESS;	
}