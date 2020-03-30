///////////////////////////////////////
// test_absorption_activity_wilson.c //
///////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "absorption_activity_wilson.h"
#include "refrigerant_vaporPressure.h"


int main() {
	// Define parameter record for executing Wilson equation. Data for working
	// pair "[BMIM]+[(CF3SO2)2N]-(2) / H2O" is taken from:
	//
	// 	Döker M. and J. Gmehling Measurement and prediction of vapor–liquid
	//	equilibria of ternary systems containing ionic liquids. Fluid phase
	//	equilibria 2005. 227(2): p. 255-266.
	//
	double isotherm_water_par[] = {0, 0, 7985.164, 16736, 1, 1};

	// Define parameter record for executing Wilson equation. Data for working
	// pair "Triethylene glycol dimethyl ether / R-134a" is taken from:
	//
	// 	DMarchi et al. Bubble pressure measurements for the (1,1,1,2-
	//	tetrafluoroethane + triethylene glycol dimethyl ether) system. Journal
	//	of chemical thermodynamics 2006. 38: p. 1247-1253.
	//
	double isotherm_r134a_par[] = {-149.128 * 8.314462618, 0.959291 *
		8.314462618, 368.189 * 8.314462618, 0.929126 * 8.314462618, 273.15,
		0.0014, 0.0014};

	// Define parameter record for executing Wilson equation. Data for working
	// pair "PAG / R-134a" is taken from:
	//
	// 	Martz W.L. and A. Jacobi Refrigerant-oil mixtures and local composition
	//	modeling. 1994 Air Conditioning and Refrigeration Center. College of
	//	Engineering. University of Illinois at Urbana-Champaign.
	//
	double isotherm_r134a_2_par[] = {0, 0, 3441, -8128, 1, 1};

	// Define parameter record for executing vapor pressure equation. Data for
	// refrigerant "H2O" is taken from:
	//
	// 	W. Wagner and P. Pruß. The IAPWS Formulation 1995 for the Thermodynamic
	//	Properties of Ordinary Water Substance for General and Scientific Use.
	//	Journal of Physical and Chemical Reference Data 2002. 31: p. 387-535.
	//
	double refrigerant_water_par[] = {647.096, 22064000, -7.85951783, 1,
		1.84408259, 1.5, -11.7866497, 3, 22.6807411, 3.5, -15.9618719, 4,
		1.80122502, 7.5};

	// Define parameter record for executing vapor pressure equation and
	// saturated liquid density equation. Data for refrigerant "R-134a" is taken
	// from:
	//
	// 	R. Tillner-Roth and H.D. Baehr. An International Standard Formulation
	//	for the Thermodynamic Properties of 1,1,1,2-Tetrafluoroethane (HFC-134a)
	//	for Temperatures from 170 K to 455 K and Pressures up to 70 MPa. Journal
	//	of Physical and Chemical Reference Data 1994. 23: p. 657-729.
	//
	double refrigerant_r134a_par[] = {374.18, 4.05629e6, -7.686556, 1, 2.311791,
		1.5, -2.039554, 2, -3.583758, 4, 0, 0, 0, 0};

	// Define some input values for calculating equilibrium data
	//
	double T_water_K = 353.15;
	double x_water_molmol = 0.9229;
	double psat_water_Pa = refrigerant_p_sat(T_water_K, refrigerant_water_par);

	double T_r134a_K = 303.24;
	double x_r134a_molmol = 0.9203;
	double psat_r134a_Pa = refrigerant_p_sat(T_r134a_K, refrigerant_r134a_par);

	double T_r134a_2_K = 323.15;
	double x_r134a_2_molmol = 0.93946701;
	double psat_r134a_2_Pa = refrigerant_p_sat(T_r134a_2_K,
		refrigerant_r134a_par);

	// Calculate equilibrium data
	//
	double gamma_water_1 = absorption_activity_wilson_fdl_g1_Txv1v2(T_water_K,
		x_water_molmol, -1, -1, isotherm_water_par);
	double p_water_Pa = absorption_activity_wilson_p_Txv1v2psat(T_water_K,
		x_water_molmol, -1, -1, absorption_activity_wilson_fdl_g1_Txv1v2,
		psat_water_Pa, isotherm_water_par);
	double x_inv_water_molmol = absorption_activity_wilson_x_pTv1v2psat(
		p_water_Pa, T_water_K, -1, -1, absorption_activity_wilson_fdl_g1_Txv1v2,
		psat_water_Pa, isotherm_water_par);

	double gamma_r134a_1 = absorption_activity_wilson_tdl_g1_Txv1v2(T_r134a_K,
		x_r134a_molmol, -1, -1, isotherm_r134a_par);
	double p_r134a_Pa = absorption_activity_wilson_p_Txv1v2psat(T_r134a_K,
		x_r134a_molmol, -1, -1, absorption_activity_wilson_tdl_g1_Txv1v2,
		psat_r134a_Pa, isotherm_r134a_par);
	double x_inv_r134a_molmol = absorption_activity_wilson_x_pTv1v2psat(
		p_r134a_Pa, T_r134a_K, -1, -1, absorption_activity_wilson_tdl_g1_Txv1v2,
		psat_r134a_Pa, isotherm_r134a_par);

	double gamma_r134a_2_1 = absorption_activity_wilson_fdl_g1_Txv1v2(
		T_r134a_2_K, x_r134a_2_molmol, -1, -1, isotherm_r134a_2_par);
	double p_r134a_2_Pa = absorption_activity_wilson_p_Txv1v2psat(T_r134a_2_K,
		x_r134a_2_molmol, -1, -1, absorption_activity_wilson_fdl_g1_Txv1v2,
		psat_r134a_2_Pa, isotherm_r134a_2_par);
	double x_inv_r134a_2_molmol = absorption_activity_wilson_x_pTv1v2psat(
		p_r134a_2_Pa, T_r134a_2_K, -1, -1,
		absorption_activity_wilson_fdl_g1_Txv1v2, psat_r134a_2_Pa,
		isotherm_r134a_2_par);

	// Print calculated values
	//
	printf("\n\n##\n##\nSelected working pair is \"[BMIM]+[(CF3SO2)2N]-(2) / "
		"H2O\".");
	printf("\nSelected absorption equation is \"Wilson-FDL\".");

	printf("\n\nFor T = %f K and x = %f mol/mol, activity coefficient of H2O "
		"results in gamma_1 = %f.", T_water_K, x_water_molmol, gamma_water_1);
	printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure results in "
		"p = %f Pa.", T_water_K, x_water_molmol, p_water_Pa);
	printf("\nFor p = %f Pa and T = %f K, equilibrium mole fraction results in "
		"x = %f mol/mol.", p_water_Pa, T_water_K, x_inv_water_molmol);

	printf("\n\n##\n##\nSelected working pair is \"Triethylene glycol dimethyl "
		"ether / R-134a.");
	printf("\nSelected absorption equation is \"Wilson-TDL\".");

	printf("\n\nFor T = %f K and x = %f mol/mol, activity coefficient of "
		"R-134a results in gamma_1 = %f.", T_r134a_K, x_water_molmol,
		gamma_r134a_1);
	printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure results in "
		"p = %f Pa.", T_r134a_K, x_r134a_molmol, p_r134a_Pa);
	printf("\nFor p = %f Pa and T = %f K, equilibrium mole fraction results in "
		"x = %f mol/mol.", p_r134a_Pa, T_r134a_K, x_inv_r134a_molmol);

	printf("\n\n##\n##\nSelected working pair is \"PAG / R-134a\".");
	printf("\nSelected absorption equation is \"Wilson-FDL\".");

	printf("\n\nFor T = %f K and x = %f mol/mol, activity coefficient of R134a "
		"results in gamma_1 = %f.", T_r134a_2_K, x_r134a_2_molmol,
		gamma_r134a_2_1);
	printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure results in "
		"p = %f Pa.", T_r134a_2_K, x_r134a_2_molmol, p_r134a_2_Pa);
	printf("\nFor p = %f Pa and T = %f K, equilibrium mole fraction results in "
		"x = %f mol/mol.", p_r134a_2_Pa, T_r134a_2_K, x_inv_r134a_2_molmol);
    return EXIT_SUCCESS;
}