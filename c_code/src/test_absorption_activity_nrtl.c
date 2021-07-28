/////////////////////////////////////
// test_absorption_activity_nrtl.c //
/////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "absorption_activity_nrtl.h"
#include "refrigerant_vaporPressure.h"


int main() {
	// Define parameter record for executing UNIQUAC equation. Data for working
	// pair "PAG / R-134a" is taken from:
	//
	// 	Martz W.L. and A. Jacobi Refrigerant-oil mixtures and local composition
	//	modeling. 1994 Air Conditioning and Refrigeration Center. College of
	//	Engineering. University of Illinois at Urbana-Champaign.
	//
	double isotherm_par_fdg_r134a[] = {-4788, -359, 0.5};

	// Define parameter record for executing NRTL-FDG equation. Data for working
	// pair "[BMIM]+[(CF3SO2)2N]-(2) / H2O" is taken from:
	//
	// 	Döker M. and J. Gmehling Measurement and prediction of vapor–liquid
	//	equilibria of ternary systems containing ionic liquids. Fluid phase
	//	equilibria 2005. 227(2): p. 255-266.
	//
	double isotherm_par_fdg_water[] = {19435.5168, -348.983256, 0.3};

	// Define parameter record for executing NRTL-DGT equation. Data for working
	// pair "[BMIM]+[(CF3SO2)2N]-(2) / H2O" is taken from:
	//
	// 	Kato R. and J. Gmehling Measurement and correlation of vapor–liquid
	//	equilibria of binary systems containing the ionic liquids
	//	[EMIM][(CF 3 SO 2) 2 N][BMIM][(CF 3 SO 2) 2 N][MMIM][(CH 3) 2 PO 4] and
	//	oxygenated organic compounds respectively water. Fluid Phase Equilibria
	//	2005. 231(1): p. 38-43..
	//
	double isotherm_par_dgt[] = {156912.552, 741.57216, -251.860064,
		-3.96228984, 0.2};

	// Define parameter record for executing vapor pressure equation. Data for
	// refrigerant "R-134a" is taken from:
	//
	// 	R. Tillner-Roth and H.D. Baehr. An International Standard Formulation
	//	for the Thermodynamic Properties of 1,1,1,2-Tetrafluoroethane (HFC-134a)
	//	for Temperatures from 170 K to 455 K and Pressures up to 70 MPa. Journal
	//	of Physical and Chemical Reference Data 1994. 23: p. 657-729.
	//
	double refrigerant_par_r134a[] = {374.18, 4.05629e6, -7.686556, 1, 2.311791,
		1.5, -2.039554, 2, -3.583758, 4, 0, 0, 0, 0, 0, 0};

	// Define parameter record for executing vapor pressure equation. Data for
	// refrigerant "H2O" is taken from:
	//
	// 	W. Wagner and P. Pruß. The IAPWS Formulation 1995 for the Thermodynamic
	//	Properties of Ordinary Water Substance for General and Scientific Use.
	//	Journal of Physical and Chemical Reference Data 2002. 31: p. 387-535.
	//
	double refrigerant_par_water[] = {647.096, 22064000, -7.85951783, 1,
		1.84408259, 1.5, -11.7866497, 3, 22.6807411, 3.5, -15.9618719, 4,
		1.80122502, 7.5, 0, 0};

	// Define parameter record for executing vapor pressure equation. Data for
	// refrigerant "Benzene" is taken from:
	//
	// 	NIST Webbook.
	//
	double refrigerant_par_benzene[] = {4.72583, 1660.652, -1.461};

	// Define some input values for calculating equilibrium data
	//
	double T_r134a_K = 323.15;
	double T_K = 353.15;
	double x_r134a_molmol = 0.93946701;
	double x_water_molmol = 0.1933;
	double x_benzene_molmol = 0.6293;

	double psat_r134a_Pa = refrigerant_p_sat_eos1(T_r134a_K,
		refrigerant_par_r134a);
	double psat_water_Pa = refrigerant_p_sat_eos1(T_K,
		refrigerant_par_water);
	double psat_benzene_Pa = refrigerant_p_sat_antoine(T_K,
		refrigerant_par_benzene);

	// Calculate equilibrium data
	//
	double gamma_r134a = absorption_activity_nrtl_fdg_g1_Tx(T_r134a_K,
		x_r134a_molmol, isotherm_par_fdg_r134a);
	double p_r134a_Pa = absorption_activity_nrtl_p_Txgpsat(T_r134a_K,
		x_r134a_molmol, absorption_activity_nrtl_fdg_g1_Tx, psat_r134a_Pa,
		isotherm_par_fdg_r134a);
	double x_inv_r134a_molmol = absorption_activity_nrtl_x_pTgpsat(
		p_r134a_Pa, T_r134a_K, absorption_activity_nrtl_fdg_g1_Tx,
		psat_r134a_Pa, isotherm_par_fdg_r134a);

	double gamma_water = absorption_activity_nrtl_fdg_g1_Tx(T_K, x_water_molmol,
		isotherm_par_fdg_water);
	double p_water_Pa = absorption_activity_nrtl_p_Txgpsat(T_K, x_water_molmol,
		absorption_activity_nrtl_fdg_g1_Tx, psat_water_Pa,
		isotherm_par_fdg_water);
	double x_inv_water_molmol = absorption_activity_nrtl_x_pTgpsat(
		p_water_Pa, T_K, absorption_activity_nrtl_fdg_g1_Tx,
		psat_water_Pa, isotherm_par_fdg_water);

	double gamma_benzene = absorption_activity_nrtl_dgT_g1_Tx(T_K,
		x_benzene_molmol, isotherm_par_dgt);
	double p_benzene_Pa = absorption_activity_nrtl_p_Txgpsat(T_K,
		x_benzene_molmol, absorption_activity_nrtl_dgT_g1_Tx, psat_benzene_Pa,
		isotherm_par_dgt);
	double x_inv_benzene_molmol = absorption_activity_nrtl_x_pTgpsat(
		p_benzene_Pa, T_K, absorption_activity_nrtl_dgT_g1_Tx,
		psat_benzene_Pa, isotherm_par_dgt);

	// Print calculated values
	//
	printf("\n\n##\n##\nSelected working pair is \"PAG / R-134a\".");
	printf("\nSelected absorption equation is \"NRTL-FDG\".");

	printf("\n\nFor T = %f K and x = %f mol/mol, activity coefficient of R134a "
		"results in gamma_1 = %f.", T_r134a_K, x_r134a_molmol, gamma_r134a);
	printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure results in "
		"p = %f Pa.", T_r134a_K, x_r134a_molmol, p_r134a_Pa);
	printf("\nFor p = %f Pa and T = %f K, equilibrium mole fraction results in "
		"x = %f mol/mol.", p_r134a_Pa, T_r134a_K, x_inv_r134a_molmol);


	printf("\n\n##\n##\nSelected working pair is \"[BMIM]+[(CF3SO2)2N]-(2) / "
		"H2O\".");
	printf("\nSelected absorption equation is \"NRTL-FDG\".");

	printf("\n\nFor T = %f K and x = %f mol/mol, activity coefficient of H2O "
		"results in gamma_1 = %f.", T_K, x_water_molmol, gamma_water);
	printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure results in "
		"p = %f Pa.", T_K, x_water_molmol, p_water_Pa);
	printf("\nFor p = %f Pa and T = %f K, equilibrium mole fraction results in "
		"x = %f mol/mol.", p_water_Pa, T_K, x_inv_water_molmol);


	printf("\n\n##\n##\nSelected working pair is \"[BMIM]+[(CF3SO2)2N]-(2) / "
		"Benzene\".");
	printf("\nSelected absorption equation is \"NRTL-DGT\".");

	printf("\n\nFor T = %f K and x = %f mol/mol, activity coefficient of "
		"benzene results in gamma_1 = %f.", T_K, x_benzene_molmol,
		gamma_benzene);
	printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure results in "
		"p = %f Pa.", T_K, x_benzene_molmol, p_benzene_Pa);
	printf("\nFor p = %f Pa and T = %f K, equilibrium mole fraction results in "
		"x = %f mol/mol.", p_benzene_Pa, T_K, x_inv_benzene_molmol);

    return EXIT_SUCCESS;
}
