///////////////////////////////////////////////
// test_refrigerant_saturatedLiquidDensity.c //
///////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "refrigerant_saturatedLiquidDensity.h"


int main() {
	// Define parameter record for executing saturated liquid density equation.
	// Data for refrigerant "R-134a" is taken from:
	//
	// 	R. Tillner-Roth and H.D. Baehr. An International Standard Formulation
	//	for the Thermodynamic Properties of 1,1,1,2-Tetrafluoroethane (HFC-134a)
	//	for Temperatures from 170 K to 455 K and Pressures up to 70 MPa. Journal
	//	of Physical and Chemical Reference Data 1994. 23: p. 657-729.
	//
	double refrigerant_par[] = {374.18, 1, 1, 518.20, 0, 884.13,
		0.333333333333333, 485.84, 0.666666666666666, 193.29,
		3.333333333333333, 0, 0, 0, 0, 0, 0};


	// Define some input values for calculating saturated liquid density
	//
	double T_K = 246.78;


	// Calculate saturated liquid density
	//
	double rho_l_kgm3 = refrigerant_rho_l(T_K, refrigerant_par);
	double rho_l_kgm3_plus = refrigerant_rho_l(T_K+0.0001, refrigerant_par);
	double rho_l_kgm3_minus = refrigerant_rho_l(T_K-0.0001, refrigerant_par);
	double drho_l_dT_kgm3K = refrigerant_drho_l_dT(T_K, refrigerant_par);
	double drho_l_dT_kgm3K_num = (rho_l_kgm3_plus - rho_l_kgm3_minus) / 0.0002;


	// Print calculated values
	//
	printf("\n\n##\n##\nSelected refrigerant: \"R-134a\".");
	printf("\n\nFor T = %f K, saturated liquid density results in rho_l = %f "
		"kg/m3.", T_K, rho_l_kgm3);
	printf("\nFor T = %f K, analytical derivative of saturated liquid density "
		"wrt. temperature results in drho_l_dT = %f kg/m3/K.", T_K,
		drho_l_dT_kgm3K);
	printf("\nFor T = %f K, numerical derivative of saturated liquid density "
		"wrt. temperature results in drho_l_dT = %f kg/m3/K.", T_K,
		drho_l_dT_kgm3K_num);

    return EXIT_SUCCESS;
}