////////////////////////
// test_refrigerant.c //
////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "refrigerant.h"

#include "structDefinitions.c"


int main() {
	// Define parameter record for executing vapour pressure equation and
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


	// Define some input values for calculation of fluid properties
	//
	double T_K = 246.78;
	
	
	// Calculate state values
	//
	Refrigerant *refrigerant_r134a = newRefrigerant(
		"EoS_vapourPressure",
		"EoS_saturatedLiquidDensity");
	
	double psat_Pa = refrigerant_r134a->psat_T(T_K, vapPre_par);
	double psat_Pa_plus = refrigerant_r134a->psat_T(T_K+0.0001, vapPre_par);
	double psat_Pa_minus = refrigerant_r134a->psat_T(T_K-0.0001, vapPre_par);
	double dpsat_dT_PaK = refrigerant_r134a->dpsat_dT(T_K, vapPre_par);
	double dpsat_dT_PaK_num = (psat_Pa_plus - psat_Pa_minus) / 0.0002;
	
	double rho_l_kgm3 = refrigerant_r134a->rho_l_T(T_K, satLiqDen_par);
	double rho_l_kgm3_plus = refrigerant_r134a->rho_l_T(T_K+0.0001, satLiqDen_par);
	double rho_l_kgm3_minus = refrigerant_r134a->rho_l_T(T_K-0.0001, satLiqDen_par);
	double drho_l_dT_kgm3K = refrigerant_r134a->drho_l_dT(T_K, satLiqDen_par);
	double drho_l_dT_kgm3K_num = (rho_l_kgm3_plus - rho_l_kgm3_minus) / 0.0002;


	// Print results of calculations
	//
	printf("\n\n##\n##\nCreated structure for refrigerant.");	
	printf("\nSelected refrigerant is \"R-134a\".");	
	
	printf("\n\nFor T = %f K, vapour pressure results in p = %f Pa.", 
		T_K, psat_Pa);
	printf("\nFor T = %f K, analytical derivative of vapour pressure wrt. temperature results in dp_dT = %f Pa/K.", 
		T_K, dpsat_dT_PaK);
	printf("\nFor T = %f K, numerical derivative of vapour pressure wrt. temperature results in dp_dT = %f Pa/K.", 
		T_K, dpsat_dT_PaK_num);	
		
	printf("\nFor T = %f K, saturated liquid density results in rho_l = %f kg/m3.", 
		T_K, rho_l_kgm3);
	printf("\nFor T = %f K, analytical derivative of saturated liquid density wrt. temperature results in drho_l_dT = %f kg/m3/K.", 
		T_K, drho_l_dT_kgm3K);
	printf("\nFor T = %f K, numerical derivative of saturated liquid density wrt. temperature results in drho_l_dT = %f kg/m3/K.", 
		T_K, drho_l_dT_kgm3K_num);
		
		
	// Free memory that is allocate for structures
	//
	delRefrigerant(refrigerant_r134a);
	
    return EXIT_SUCCESS;	
}