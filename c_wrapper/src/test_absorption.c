///////////////////////
// test_adsorption.c //
///////////////////////
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "absorption.h"
#include "refrigerant_vapourPressure.h"

#include "structDefinitions.c"


int main() {
	// Define parameter record for executing Duehring equation. Data for working
	// pair "NaOH-KOH-CsOH / H2O" is taken from:
	//
	// 	Herold K.E. et al. Development of an absorption heat pump water heater
	//  using an aqueous ternary hydroxide working fluid. International journal 
	//  of refrigeration 1991. 14(3): p. 156-167.
	//
	double par_duehring[] = 
		{6.164233723, -0.2746665026, 0.004916023734, -0.00002859098259, 
		 -53.80343163, 5.004848451, -0.1228273028, 0.0010961422341,
		 6.427154896, -1208.919437, -166159.963, 0, 1, 273.15, 0.001};	


	// Define some input values for calculating equilibrium data
	//
	double p_Pa = 724.659957;
	double T_K = 323.15;
	
	
	// Calculate equilibrium data
	//
	Absorption *isotherm_duehring = newAbsorption("duhring");
	
	double X_kgkg = isotherm_duehring->con_X_pT(p_Pa, T_K,
		par_duehring);
	double p_Pa_inv = isotherm_duehring->con_p_XT(X_kgkg, T_K,
		par_duehring);
	double T_K_inv = isotherm_duehring->con_T_pX(p_Pa, X_kgkg,
		par_duehring);
	double dX_dp_kgkgPa = isotherm_duehring->con_dX_dp_pT(p_Pa, T_K,
		par_duehring);	
	double dX_dT_kgkgK = isotherm_duehring->con_dX_dT_pT(p_Pa, T_K,
		par_duehring);
	double dp_dX_Pakgkg = isotherm_duehring->con_dp_dX_XT(X_kgkg, T_K,
		par_duehring);
	double dp_dT_Pakgkg = isotherm_duehring->con_dp_dT_XT(X_kgkg, T_K,
		par_duehring);


	// Print results of calculations
	//
	printf("\n\n##\n##\nCreated structure for Duehring isotherm.");	
	printf("\nSelected working pair is \"NaOH-KOH-CsOH / H2O\".");	
	
	printf("\n\nFor T = %f K and p = %f Pa, equilibrium concentration results in X = %f kg/kg.", 
		T_K, p_Pa, X_kgkg);
	printf("\nFor T = %f K and X = %f kg/kg, equilibrium pressure results in p = %f Pa.", 
		T_K, X_kgkg, p_Pa_inv);
	printf("\nFor p = %f Pa and X = %f kg/kg equilibrium temperature results in T = %f K.", 
		p_Pa, X_kgkg, T_K_inv);
		
	printf("\n\nFor T = %f K and p = %f Pa, analytical derivative of X with respect to p results in dX_dp = %f kg/kg/Pa.",
		T_K, p_Pa, dX_dp_kgkgPa);
	printf("\nFor T = %f K and p = %f Pa, analytical derivative of X with respect to T results in dX_dT = %f kg/kg/K.",
		T_K, p_Pa, dX_dT_kgkgK);		
	printf("\n\nFor T = %f K and X = %f kg/kg, analytical derivative of p with respect to X results in dp_dX = %f Pakg/kg.", 
		T_K, X_kgkg, dp_dX_Pakgkg);
	printf("\nFor T = %f K and X = %f kg/kg, analytical derivative of p with respect to T results in dp_dT = %f Pa/K.", 
		T_K, X_kgkg, dp_dT_Pakgkg);
		
		
	// Define parameter record for executing Wilson equation. Data for working
	// pair "[BMIM]+[(CF3SO2)2N]-(2) / H2O" is taken from:
	//
	// 	Döker M. and J. Gmehling Measurement and prediction of vapor–liquid 
	//	equilibria of ternary systems containing ionic liquids. Fluid phase 
	//	equilibria 2005. 227(2): p. 255-266.
	//
	double par_wilson[] = {7985.164, 16736, 0, 0, 0, 0};
	
	// Define parameter record for executing vapour pressure equation. Data for
	// refrigerant "H2O" is taken from:
	//
	// 	W. Wagner and P. Pruß. The IAPWS Formulation 1995 for the Thermodynamic
	//	Properties of Ordinary Water Substance for General and Scientific Use.
	//	Journal of Physical and Chemical Reference Data 2002. 31: p. 387-535.
	//
	double H2O_par[] = {647.096, 22064000, -7.85951783, 1, 1.84408259,
		1.5, -11.7866497, 3, 22.6807411, 3.5, -15.9618719, 4, 1.80122502, 7.5};


	// Define some input values for calculating equilibrium data
	//
	T_K = 353.15;
	double x_molmol = 0.9386;	
	double psat_Pa = refrigerant_p_sat(T_K, H2O_par);
		
	
	// Calculate equilibrium data
	//
	Absorption *isotherm_wilson = newAbsorption("wilson");
	
	double gamma_1 = isotherm_wilson->act_g_Txv1v2(T_K , x_molmol, -1,
		-1, par_wilson);
	p_Pa = isotherm_wilson->act_p_Txv1v2psat(T_K , x_molmol, -1, 
		-1, psat_Pa, par_wilson);
		
	
	// Print calculated values
	//
	printf("\n\n##\n##\nCreated structure for Wilson isotherm.");	
	printf("\nSelected working pair is \"[BMIM]+[(CF3SO2)2N]-(2) / H2O\".");	
	
	printf("\n\nFor T = %f K and x = %f mol/mol, activity coefficient of H2O results in gamma_1 = %f.", 
		T_K, x_molmol, gamma_1);
	printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure results in p = %f Pa.", 
		T_K, x_molmol, p_Pa);
	
	
	
	// Define parameter record for executing NRTL-FDG equation. Data for working
	// pair "[BMIM]+[(CF3SO2)2N]-(2) / H2O" is taken from:
	//
	// 	Döker M. and J. Gmehling Measurement and prediction of vapor–liquid 
	//	equilibria of ternary systems containing ionic liquids. Fluid phase 
	//	equilibria 2005. 227(2): p. 255-266.
	//
	double par_nrtl_fdg[] = {19435.5168, -348.983256, 0.3};
	

	// Define some input values for calculating equilibrium data
	//
	T_K = 353.15;
	x_molmol = 0.9386;	
	psat_Pa = refrigerant_p_sat(T_K, H2O_par);
	
	// Calculate equilibrium data
	//
	Absorption *isotherm_nrtl_fdg = newAbsorption("nrtl-fixeddg");
	
	gamma_1 = isotherm_nrtl_fdg->act_g_Tx(T_K , x_molmol, par_nrtl_fdg);
	p_Pa = isotherm_nrtl_fdg->act_p_Txgpsat(T_K , x_molmol, 
		isotherm_nrtl_fdg->act_g_Tx , psat_Pa, par_nrtl_fdg);
	
	
	// Print calculated values
	//
	printf("\n\n##\n##\nCreated structure for NRTL-FDG isotherm.");	
	printf("\nSelected working pair is \"[BMIM]+[(CF3SO2)2N]-(2) / H2O\".");	
	
	printf("\n\nFor T = %f K and x = %f mol/mol, activity coefficient of H2O results in gamma_1 = %f.", 
		T_K, x_molmol, gamma_1);
	printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure results in p = %f Pa.", 
		T_K, x_molmol, p_Pa);
	
	
	// Define parameter record for executing NRTL-DGT equation. Data for working
	// pair "[BMIM]+[(CF3SO2)2N]-(2) / Benzene" is taken from:
	//
	// 	Kato R. and J. Gmehling Measurement and correlation of vapor–liquid 
	//	equilibria of binary systems containing the ionic liquids 
	//	[EMIM][(CF 3 SO 2) 2 N][BMIM][(CF 3 SO 2) 2 N][MMIM][(CH 3) 2 PO 4] and
	//	oxygenated organic compounds respectively water. Fluid Phase Equilibria
	//	2005. 231(1): p. 38-43..
	//
	double par_nrtl_dgt[] = {156912.552, 741.57216, -251.860064, 
		-3.96228984, 0.2};
		
	// Define parameter record for executing vapour pressure equation. Data for
	// refrigerant "Benzene" is taken from:
	//
	// 	NIST Webbook.
	//
	double benzene_par[] = {4.72583, 1660.652, -1.461};
	

	// Define some input values for calculating equilibrium data
	//
	T_K = 353.15;
	x_molmol =  0.2893;	
	psat_Pa = refrigerant_p_sat_antoine(T_K, benzene_par);
	
	// Calculate equilibrium data
	//
	Absorption *isotherm_nrtl_dgt = newAbsorption("nrtl-dg-t");
	
	gamma_1 = isotherm_nrtl_dgt->act_g_Tx(T_K , x_molmol, par_nrtl_dgt);
	p_Pa = isotherm_nrtl_dgt->act_p_Txgpsat(T_K , x_molmol, 
		isotherm_nrtl_dgt->act_g_Tx , psat_Pa, par_nrtl_dgt);
	
	
	// Print calculated values
	//
	printf("\n\n##\n##\nCreated structure for NRTL-DGT isotherm.");	
	printf("\nSelected working pair is \"[BMIM]+[(CF3SO2)2N]-(2) / Benzene\".");	
	
	printf("\n\nFor T = %f K and x = %f mol/mol, activity coefficient of benzene results in gamma_1 = %f.", 
		T_K, x_molmol, gamma_1);
	printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure results in p = %f Pa.", 
		T_K, x_molmol, p_Pa);
	
		
	// Define parameter record for executing UNIQUAC equation. Data for working
	// pair "[BMIM]+[(CF3SO2)2N]-(2) / H2O" is taken from:
	//
	// 	Döker M. and J. Gmehling Measurement and prediction of vapor–liquid 
	//	equilibria of ternary systems containing ionic liquids. Fluid phase 
	//	equilibria 2005. 227(2): p. 255-266.
	//
	double par_uniquac_fdu[] = {345.30552, 3057.12328, 1.4, 10.2, 0.92, 11.16,
		0.5};
	

	// Define some input values for calculating equilibrium data
	//
	T_K = 353.15;
	x_molmol = 0.9386;	
	psat_Pa = refrigerant_p_sat(T_K, H2O_par);
	
	// Calculate equilibrium data
	//
	Absorption *isotherm_uniquac_fdu = newAbsorption("uniquac-fixeddu");
	
	gamma_1 = isotherm_uniquac_fdu->act_g_Tx(T_K , x_molmol, par_uniquac_fdu);
	p_Pa = isotherm_uniquac_fdu->act_p_Txgpsat(T_K , x_molmol, 
		isotherm_uniquac_fdu->act_g_Tx , psat_Pa, par_uniquac_fdu);
	
	
	// Print calculated values
	//
	printf("\n\n##\n##\nCreated structure for UNIQUAC-FDU isotherm.");	
	printf("\nSelected working pair is \"[BMIM]+[(CF3SO2)2N]-(2) / H2O\".");	
	
	printf("\n\nFor T = %f K and x = %f mol/mol, activity coefficient of H2O results in gamma_1 = %f.", 
		T_K, x_molmol, gamma_1);
	printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure results in p = %f Pa.", 
		T_K, x_molmol, p_Pa);
		
	
	// Define parameter record for executing NRTL-DGT equation. Data for working
	// pair "[BMIM]+[(CF3SO2)2N]-(2) / Benzene" is taken from:
	//
	// 	Kato R. and J. Gmehling Measurement and correlation of vapor–liquid 
	//	equilibria of binary systems containing the ionic liquids 
	//	[EMIM][(CF 3 SO 2) 2 N][BMIM][(CF 3 SO 2) 2 N][MMIM][(CH 3) 2 PO 4] and
	//	oxygenated organic compounds respectively water. Fluid Phase Equilibria
	//	2005. 231(1): p. 38-43..
	//
	double par_uniquac_dut[] = {0.0029288, -749.01968, 12.9377648, -3.49891184,
		10.2, 1.824, 11.2, 0.01154, 0.5};
	

	// Define some input values for calculating equilibrium data
	//
	T_K = 353.15;
	x_molmol =  0.3475;	
	psat_Pa = refrigerant_p_sat_antoine(T_K, benzene_par);
	
	// Calculate equilibrium data
	//
	Absorption *isotherm_uniquac_dut = newAbsorption("uniquac-du-t");
	
	gamma_1 = isotherm_uniquac_dut->act_g_Tx(T_K , x_molmol, par_uniquac_dut);
	p_Pa = isotherm_uniquac_dut->act_p_Txgpsat(T_K , x_molmol, 
		isotherm_uniquac_dut->act_g_Tx , psat_Pa, par_uniquac_dut);
	
	
	// Print calculated values
	//
	printf("\n\n##\n##\nCreated structure for NRTL-DGT isotherm.");	
	printf("\nSelected working pair is \"[BMIM]+[(CF3SO2)2N]-(2) / Benzene\".");	
	
	printf("\n\nFor T = %f K and x = %f mol/mol, activity coefficient of benzene results in gamma_1 = %f.", 
		T_K, x_molmol, gamma_1);
	printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure results in p = %f Pa.", 
		T_K, x_molmol, p_Pa);
		
		
	// Define parameter record for executing Wilson equation. Data for working
	// pair "Lubricant PEB9 / HFC134a" is taken from:
	//
	// 	Wahlström Å. and L. Vamling Solubility of HFCs in pentaerythritol 
	//	tetraalkyl esters. Journal of Chemical & Engineering Data 2000. 45(1): 
	//	p. 97-103.
	//
	double par_flory_huggins[] = {10.41, 938, -176};

	// Define parameter record for executing vapour pressure equation. Data for
	// refrigerant "R-134a" is taken from:
	//
	// 	R. Tillner-Roth and H.D. Baehr. An International Standard Formulation
	//	for the Thermodynamic Properties of 1,1,1,2-Tetrafluoroethane (HFC-134a)
	//	for Temperatures from 170 K to 455 K and Pressures up to 70 MPa. Journal
	//	of Physical and Chemical Reference Data 1994. 23: p. 657-729.
	//
	double r134a_par[] = {374.18, 4.05629e6, -7.686556, 1, 2.311791,
		1.5, -2.039554, 2, -3.583758, 4, 0, 0, 0, 0};


	// Define some input values for calculating equilibrium data
	//
	T_K = 323.07;
	x_molmol =  0.5864;	
	psat_Pa = refrigerant_p_sat(T_K, r134a_par);
	
	// Calculate equilibrium data
	//
	Absorption *isotherm_flory_huggins = newAbsorption("flory-huggins");
	
	gamma_1 = isotherm_flory_huggins->act_g_Tx(T_K , x_molmol, par_flory_huggins);
	p_Pa = isotherm_flory_huggins->act_p_Txpsat(T_K , x_molmol, psat_Pa, 
		par_flory_huggins);
	
	
	// Print calculated values
	//
	printf("\n\n##\n##\nCreated structure for Flory-Huggins isotherm.");	
	printf("\nSelected working pair is \"Lubricant PEB9 / HFC134a\".");	
	
	printf("\n\nFor T = %f K and x = %f mol/mol, activity coefficient of benzene results in gamma_1 = %f.", 
		T_K, x_molmol, gamma_1);
	printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure results in p = %f Pa.", 
		T_K, x_molmol, p_Pa);
		
		
	// Define parameter record for executing Wilson equation. Data for working
	// pair "PAG / R-134a" is taken from:
	//
	// 	Martz W.L. and A. Jacobi Refrigerant-oil mixtures and local composition
	//	modeling. 1994 Air Conditioning and Refrigeration Center. College of 
	//	Engineering. University of Illinois at Urbana-Champaign.
	//
	double par_heil[] = {1667, -4626, 1, 1};


	// Define some input values for calculating equilibrium data
	//
	T_K = 353.15;
	x_molmol =  0.5250;	
	psat_Pa = refrigerant_p_sat(T_K, r134a_par);
	
	// Calculate equilibrium data
	//
	Absorption *isotherm_heil = newAbsorption("heil");
	
	gamma_1 = isotherm_heil->act_g_Txv1v2(T_K , x_molmol, -1, -1,
		par_heil);
	p_Pa = isotherm_heil->act_p_Txv1v2psat(T_K , x_molmol, -1, -1, psat_Pa, 
		par_heil);
	
	
	// Print calculated values
	//
	printf("\n\n##\n##\nCreated structure for Heil isotherm.");	
	printf("\nSelected working pair is \"PAG / R-134a\".");	
	
	printf("\n\nFor T = %f K and x = %f mol/mol, activity coefficient of benzene results in gamma_1 = %f.", 
		T_K, x_molmol, gamma_1);
	printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure results in p = %f Pa.", 
		T_K, x_molmol, p_Pa);
		
		
	// Define parameter record for executing Wilson equation. Data for working
	// pair "PAG / R-134a" is taken from:
	//
	// 	Martz W.L. and A. Jacobi Refrigerant-oil mixtures and local composition
	//	modeling. 1994 Air Conditioning and Refrigeration Center. College of 
	//	Engineering. University of Illinois at Urbana-Champaign.
	//
	double par_tsuboka_katayama[] = {-247, -15259, 1, 1};


	// Define some input values for calculating equilibrium data
	//
	T_K = 353.15;
	x_molmol =  0.5250;	
	psat_Pa = refrigerant_p_sat(T_K, r134a_par);
	
	
	// Calculate equilibrium data
	//
	Absorption *isotherm_tsuboka_katayama = newAbsorption("tsuboka-katayama");
	
	gamma_1 = isotherm_tsuboka_katayama->act_g_Txv1v2(T_K , x_molmol, -1, -1,
		par_tsuboka_katayama);
	p_Pa = isotherm_tsuboka_katayama->act_p_Txv1v2psat(T_K , x_molmol, -1, -1,
		psat_Pa, par_tsuboka_katayama);
	
	
	// Print calculated values
	//
	printf("\n\n##\n##\nCreated structure for Tsubo & Kakatayama isotherm.");	
	printf("\nSelected working pair is \"PAG / R-134a\".");	
	
	printf("\n\nFor T = %f K and x = %f mol/mol, activity coefficient of benzene results in gamma_1 = %f.", 
		T_K, x_molmol, gamma_1);
	printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure results in p = %f Pa.", 
		T_K, x_molmol, p_Pa);
		
		
	// Define parameter record for executing Wilson equation. Data for working
	// pair "IL [c10mim][ntf2] / CO2" is taken from:
	//
	// 	Manic M.S. et al. High-pressure solubilities of carbon dioxide in ionic
	//	liquids based on bis (trifluoromethylsulfonyl) imide and chloride. The 
	//	Journal of Supercritical Fluids 2012. 65: p. 1-10.
	//
	double par_mixing_1pvdw_PR[] = {10, 0.2236, 0.5741, 0.0056, 304.2, 800, 
		7380000, 1867000};
	double par_mixing_1pvdw_SRK[] = {-10, 0.2236, 0.0625, 0.0056, 304.2, 800, 
		7380000, 1867000};


	// Define some input values for calculating equilibrium data
	//
	T_K = 313.2;
	x_molmol = 0.769;	
	
	
	// Calculate equilibrium data
	//
	Absorption *isotherm_mixing_1pvdw = newAbsorption("mixingrule-1pvdw");
	
	p_Pa = isotherm_mixing_1pvdw->mix_p_Tvx(T_K , x_molmol, 0.5/1237.1,
		par_mixing_1pvdw_PR);
	double p_Pa_1 = isotherm_mixing_1pvdw->mix_p_Tvx(T_K , x_molmol, 0.5/1237.1,
		par_mixing_1pvdw_SRK);
	
	
	// Print calculated values
	//
	printf("\n\n##\n##\nCreated structure for mixingrule-1pvdw isotherm.");	
	printf("\nSelected working pair is \"IL [c10mim][ntf2] / CO2\".");	
	
	printf("\n\nFor T = %f K,  x = %f mol/mol and PRW, equilibrium pressure results in p = %f Pa.", 
		T_K, x_molmol, p_Pa);
	printf("\nFor T = %f K,  x = %f mol/mol and SRK, equilibrium pressure results in p = %f Pa.", 
		T_K, x_molmol, p_Pa_1);
			
				
	// Free memory that is allocated for structures
	//
	delAbsorption(isotherm_mixing_1pvdw);
	delAbsorption(isotherm_tsuboka_katayama);
	delAbsorption(isotherm_heil);
	delAbsorption(isotherm_flory_huggins);
	delAbsorption(isotherm_uniquac_dut);
	delAbsorption(isotherm_uniquac_fdu);
	delAbsorption(isotherm_nrtl_dgt);
	delAbsorption(isotherm_nrtl_fdg);
	delAbsorption(isotherm_wilson);
	delAbsorption(isotherm_duehring);
	
    return EXIT_SUCCESS;	
}