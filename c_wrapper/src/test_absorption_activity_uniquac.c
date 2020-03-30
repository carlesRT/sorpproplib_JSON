////////////////////////////////////////
// test_absorption_activity_uniquac.c //
////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "absorption_activity_uniquac.h"
#include "refrigerant_vaporPressure.h"


int main() {
	// Define parameter record for executing UNIQUAC equation. Data for working
	// pair "[BMIM]+[(CF3SO2)2N]-(2) / H2O" is taken from:
	//
	// 	Döker M. and J. Gmehling Measurement and prediction of vapor–liquid
	//	equilibria of ternary systems containing ionic liquids. Fluid phase
	//	equilibria 2005. 227(2): p. 255-266.
	//
	double isotherm_par_fdu[] = {345.30552, 3057.12328, 1.4, 10.2, 0.92, 11.16,
		10};

	// Define parameter record for executing NRTL-DGT equation. Data for working
	// pair "[BMIM]+[(CF3SO2)2N]-(2) / Benzene" is taken from:
	//
	// 	Kato R. and J. Gmehling Measurement and correlation of vapor–liquid
	//	equilibria of binary systems containing the ionic liquids
	//	[EMIM][(CF 3 SO 2) 2 N][BMIM][(CF 3 SO 2) 2 N][MMIM][(CH 3) 2 PO 4] and
	//	oxygenated organic compounds respectively water. Fluid Phase Equilibria
	//	2005. 231(1): p. 38-43..
	//
	double isotherm_par_dut[] = {0.0029288, -749.01968, 12.9377648, -3.49891184,
		10.2, 1.824, 11.2, 0.01154, 6};

	// Define parameter record for executing vapor pressure equation. Data for
	// refrigerant "H2O" is taken from:
	//
	// 	W. Wagner and P. Pruß. The IAPWS Formulation 1995 for the Thermodynamic
	//	Properties of Ordinary Water Substance for General and Scientific Use.
	//	Journal of Physical and Chemical Reference Data 2002. 31: p. 387-535.
	//
	double refrigerant_par_water[] = {647.096, 22064000, -7.85951783, 1,
		1.84408259, 1.5, -11.7866497, 3, 22.6807411, 3.5, -15.9618719, 4,
		1.80122502, 7.5};

	// Define parameter record for executing vapour pressure equation. Data for
	// refrigerant "Benzene" is taken from:
	//
	// 	NIST Webbook.
	//
	double refrigerant_par_benzene[] = {4.72583, 1660.652, -1.461};

	// Define some input values for calculating equilibrium data
	//
	double T_K = 353.15;
	double x_water_molmol = 0.1933;
	double x_benzene_molmol = 0.6293;

	double psat_water_Pa = refrigerant_p_sat(T_K, refrigerant_par_water);
	double psat_benzene_Pa = refrigerant_p_sat_antoine(T_K,
		refrigerant_par_benzene);

	// Calculate equilibrium data
	//
	double gamma_water = absorption_activity_uniquac_fdu_g1_Tx(T_K,
		x_water_molmol, isotherm_par_fdu);
	double p_water_Pa = absorption_activity_uniquac_p_Txgpsat(T_K,
		x_water_molmol, absorption_activity_uniquac_fdu_g1_Tx, psat_water_Pa,
		isotherm_par_fdu);
	double x_inv_water_molmol = absorption_activity_uniquac_x_pTgpsat(
		p_water_Pa, T_K, absorption_activity_uniquac_fdu_g1_Tx,
		psat_water_Pa, isotherm_par_fdu);

	double gamma_benzene = absorption_activity_uniquac_duT_g1_Tx(T_K ,
		x_benzene_molmol, isotherm_par_dut);
	double p_benzene_Pa = absorption_activity_uniquac_p_Txgpsat(T_K ,
		x_benzene_molmol, absorption_activity_uniquac_duT_g1_Tx,
		psat_benzene_Pa, isotherm_par_dut);
	double x_inv_benzene_molmol = absorption_activity_uniquac_x_pTgpsat(
		p_benzene_Pa, T_K, absorption_activity_uniquac_duT_g1_Tx,
		psat_benzene_Pa, isotherm_par_dut);


	// Print calculated values
	//
	printf("\n\n##\n##\nSelected working pair is \"[BMIM]+[(CF3SO2)2N]-(2) / "
		"H2O\".");
	printf("\nSelected absorption equation is \"UNIQUAC-FDU\".");

	printf("\n\nFor T = %f K and x = %f mol/mol, activity coefficient of H2O "
		"results in gamma_1 = %f.", T_K, x_water_molmol, gamma_water);
	printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure results in "
		"p = %f Pa.", T_K, x_water_molmol, p_water_Pa);
	printf("\nFor p = %f Pa and T = %f K, equilibrium mole fraction results in "
		"x = %f mol/mol.", p_water_Pa, T_K, x_inv_water_molmol);

	printf("\n\n##\n##\nSelected working pair is \"[BMIM]+[(CF3SO2)2N]-(2) / "
		"Benzene\".");
	printf("\nSelected absorption equation is \"UNIQUAC-DUT\".");

	printf("\n\nFor T = %f K and x = %f mol/mol, activity coefficient of "
		"benzene results in gamma_1 = %f.", T_K, x_benzene_molmol,
		gamma_benzene);
	printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure results in "
		"p = %f Pa.", T_K, x_benzene_molmol, p_benzene_Pa);
	printf("\nFor p = %f Pa and T = %f K, equilibrium mole fraction results in "
		"x = %f mol/mol.", p_benzene_Pa, T_K, x_inv_benzene_molmol);

    return EXIT_SUCCESS;
}