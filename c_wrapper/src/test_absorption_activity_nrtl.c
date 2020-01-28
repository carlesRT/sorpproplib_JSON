/////////////////////////////////////
// test_absorption_activity_nrtl.c //
/////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "absorption_activity_nrtl.h"
#include "refrigerant_vapourPressure.h"


int main() {
	// Define parameter record for executing NRTL-FDG equation. Data for working
	// pair "[BMIM]+[(CF3SO2)2N]-(2) / H2O" is taken from:
	//
	// 	Döker M. and J. Gmehling Measurement and prediction of vapor–liquid 
	//	equilibria of ternary systems containing ionic liquids. Fluid phase 
	//	equilibria 2005. 227(2): p. 255-266.
	//
	double isotherm_par_fdg[] = {19435.5168, -348.983256, 0.3};
	
	// Define parameter record for executing NRTL-DGT equation. Data for working
	// pair "[BMIM]+[(CF3SO2)2N]-(2) / Benzene" is taken from:
	//
	// 	Kato R. and J. Gmehling Measurement and correlation of vapor–liquid 
	//	equilibria of binary systems containing the ionic liquids 
	//	[EMIM][(CF 3 SO 2) 2 N][BMIM][(CF 3 SO 2) 2 N][MMIM][(CH 3) 2 PO 4] and
	//	oxygenated organic compounds respectively water. Fluid Phase Equilibria
	//	2005. 231(1): p. 38-43..
	//
	double isotherm_par_dgt[] = {156912.552, 741.57216, -251.860064, 
		-3.96228984, 0.2};
	
	// Define parameter record for executing vapour pressure equation. Data for
	// refrigerant "H2O" is taken from:
	//
	// 	W. Wagner and P. Pruß. The IAPWS Formulation 1995 for the Thermodynamic
	//	Properties of Ordinary Water Substance for General and Scientific Use.
	//	Journal of Physical and Chemical Reference Data 2002. 31: p. 387-535.
	//
	double refrigerant_par_H2O[] = {647.096, 22064000, -7.85951783, 1, 1.84408259,
		1.5, -11.7866497, 3, 22.6807411, 3.5, -15.9618719, 4, 1.80122502, 7.5};
	
	// Define parameter record for executing vapour pressure equation. Data for
	// refrigerant "Benzene" is taken from:
	//
	// 	NIST Webbook.
	//
	double refrigerant_par_benzene[] = {4.72583, 1660.652, -1.461};


	// Define some input values for calculating equilibrium data
	//
	double T_K = 353.15;
	
	double x_H2O_molmol = 0.9386;
	double x_benzene_molmol = 0.2893;
	
	double psat_H2O_Pa = refrigerant_p_sat(T_K, refrigerant_par_H2O);
	double psat_benzene_Pa = refrigerant_p_sat_antoine(T_K, 
		refrigerant_par_benzene);
	
	
	// Calculate equilibrium data
	//
	double gamma_H2O = absorption_activity_nrtl_fdg_g1_Tx(T_K , x_H2O_molmol,
		isotherm_par_fdg);	
	double p_H2O_Pa = absorption_activity_nrtl_p_Txgpsat(T_K , x_H2O_molmol,
		psat_H2O_Pa, absorption_activity_nrtl_fdg_g1_Tx, isotherm_par_fdg);
	
	double gamma_benzene = absorption_activity_nrtl_dgT_g1_Tx(T_K , 
		x_benzene_molmol, isotherm_par_dgt);	
	double p_benzene_Pa = absorption_activity_nrtl_p_Txgpsat(T_K , x_benzene_molmol,
		psat_benzene_Pa, absorption_activity_nrtl_dgT_g1_Tx, isotherm_par_dgt);
		
	
	// Print calculated values
	//
	printf("\n\n##\n##\nSelected working pair is \"[BMIM]+[(CF3SO2)2N]-(2) / H2O\".");	
	printf("\nSelected absorption equation is \"NRTL-FDG\".");
	
	printf("\n\nFor T = %f K and x = %f mol/mol, activity coefficient of H2O results in gamma_1 = %f.", 
		T_K, x_H2O_molmol, gamma_H2O);
	printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure results in p = %f Pa.", 
		T_K, x_H2O_molmol, p_H2O_Pa);
		

	printf("\n\n##\n##\nSelected working pair is \"[BMIM]+[(CF3SO2)2N]-(2) / Benzene\".");	
	printf("\nSelected absorption equation is \"NRTL-DGT\".");
	
	printf("\n\nFor T = %f K and x = %f mol/mol, activity coefficient of benzene results in gamma_1 = %f.", 
		T_K, x_benzene_molmol, gamma_benzene);
	printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure results in p = %f Pa.", 
		T_K, x_benzene_molmol, p_benzene_Pa);
		
    return EXIT_SUCCESS;	
}