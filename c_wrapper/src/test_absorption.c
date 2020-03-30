///////////////////////
// test_adsorption.c //
///////////////////////
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "absorption.h"
#include "refrigerant_vaporPressure.h"

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
	double p_Pa = 2321;
	double T_K = 323.15;

	// Calculate equilibrium data
	//
	Absorption *isotherm_duehring = newAbsorption("duehring");

	double X_kgkg = isotherm_duehring->con_X_pT(p_Pa, T_K, par_duehring);
	double p_Pa_inv = isotherm_duehring->con_p_XT(X_kgkg, T_K, par_duehring);
	double T_K_inv = isotherm_duehring->con_T_pX(p_Pa, X_kgkg, par_duehring);
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

	printf("\n\nFor T = %f K and p = %f Pa, equilibrium concentration results "
		"in X = %f kg/kg.", T_K, p_Pa, X_kgkg);
	printf("\nFor T = %f K and X = %f kg/kg, equilibrium pressure results in p "
		"= %f Pa.", T_K, X_kgkg, p_Pa_inv);
	printf("\nFor p = %f Pa and X = %f kg/kg equilibrium temperature results "
		"in T = %f K.", p_Pa, X_kgkg, T_K_inv);

	printf("\n\nFor T = %f K and p = %f Pa, derivative of X with respect to p "
		"results in dX_dp = %f kg/kg/Pa.", T_K, p_Pa, dX_dp_kgkgPa);
	printf("\nFor T = %f K and p = %f Pa, derivative of X with respect to T "
		"results in dX_dT = %f kg/kg/K.", T_K, p_Pa, dX_dT_kgkgK);

	printf("\n\nFor T = %f K and X = %f kg/kg, derivative of p with respect to "
		"X results in dp_dX = %f Pakg/kg.", T_K, X_kgkg, dp_dX_Pakgkg);
	printf("\nFor T = %f K and X = %f kg/kg, derivative of p with respect to T "
		"results in dp_dT = %f Pa/K.", T_K, X_kgkg, dp_dT_Pakgkg);


	// Define parameter record for executing Antoine equation. Data for working
	// pair "LiBr + CH3COOK (2:1) / H2O" is taken from:
	//
	// 	Vapour pressures, densities, and viscosities of the (water + lithium
	//	bromide + potassium acetate) system and (water + lithium bromide +
	//	sodium lactate) system. Jounarl of Chemical Thermodynamics 2006. 38):
	//	p. 123-129.
	//
	double par_antoine[] =
		{6.95, -1.33e-2, -9.02e-6, 0, 0, -1.64, 1.83e-3, -2.52e-6, 0, 0,
		 43.15, 1000};

	// Define some input values for calculating equilibrium data
	//
	p_Pa = 2480;
	T_K = 298.15;

	// Calculate equilibrium data
	//
	Absorption *isotherm_antoine = newAbsorption("antoine");

	X_kgkg = isotherm_antoine->con_X_pT(p_Pa, T_K, par_antoine);
	p_Pa_inv = isotherm_antoine->con_p_XT(X_kgkg, T_K, par_antoine);
	T_K_inv = isotherm_antoine->con_T_pX(p_Pa, X_kgkg, par_antoine);
	dX_dp_kgkgPa = isotherm_antoine->con_dX_dp_pT(p_Pa, T_K, par_antoine);
	dX_dT_kgkgK = isotherm_antoine->con_dX_dT_pT(p_Pa, T_K, par_antoine);
	dp_dX_Pakgkg = isotherm_antoine->con_dp_dX_XT(X_kgkg, T_K, par_antoine);
	dp_dT_Pakgkg = isotherm_antoine->con_dp_dT_XT(X_kgkg, T_K, par_antoine);

	// Print results of calculations
	//
	printf("\n\n##\n##\nCreated structure for Antoine isotherm.");
	printf("\nSelected working pair is \"LiBr + CH3COOK (2:1) / H2O\".");

	printf("\n\nFor T = %f K and p = %f Pa, equilibrium concentration results "
		"in X = %f kg/kg.", T_K, p_Pa, X_kgkg);
	printf("\nFor T = %f K and X = %f kg/kg, equilibrium pressure results in p "
		"= %f Pa.", T_K, X_kgkg, p_Pa_inv);
	printf("\nFor p = %f Pa and X = %f kg/kg equilibrium temperature results "
		"in T = %f K.", p_Pa, X_kgkg, T_K_inv);

	printf("\n\nFor T = %f K and p = %f Pa, derivative of X with respect to p "
		"results in dX_dp = %f kg/kg/Pa.", T_K, p_Pa, dX_dp_kgkgPa);
	printf("\nFor T = %f K and p = %f Pa, derivative of X with respect to T "
		"results in dX_dT = %f kg/kg/K.", T_K, p_Pa, dX_dT_kgkgK);

	printf("\n\nFor T = %f K and X = %f kg/kg, derivative of p with respect to "
		"X results in dp_dX = %f Pakg/kg.", T_K, X_kgkg, dp_dX_Pakgkg);
	printf("\nFor T = %f K and X = %f kg/kg, derivative of p with respect to T "
		"results in dp_dT = %f Pa/K.", T_K, X_kgkg, dp_dT_Pakgkg);


	// Define parameter record for executing Wilson equation. Data for working
	// pair "[BMIM]+[(CF3SO2)2N]-(2) / H2O" is taken from:
	//
	// 	Döker M. and J. Gmehling Measurement and prediction of vapor–liquid
	//	equilibria of ternary systems containing ionic liquids. Fluid phase
	//	equilibria 2005. 227(2): p. 255-266.
	//
	double par_wilson_fdl[] = {0, 0, 7985.164, 16736, 1, 1};

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
	Absorption *isotherm_wilson_fdl = newAbsorption("wilson-fixeddl");

	double gamma_1 = isotherm_wilson_fdl->act_g_Txv1v2_w_v(T_K , x_molmol, -1,
		-1, par_wilson_fdl);
	p_Pa = isotherm_wilson_fdl->act_p_Txv1v2gpsat_w_v_gf(T_K , x_molmol, -1,
		-1, isotherm_wilson_fdl->act_g_Txv1v2_w_v, psat_Pa, par_wilson_fdl);
	double x_inv_molmol = isotherm_wilson_fdl->act_x_pTv1v2gpsat_w_v_gf(
		p_Pa, T_K, -1, -1, isotherm_wilson_fdl->act_g_Txv1v2_w_v, psat_Pa,
		par_wilson_fdl);

	// Print calculated values
	//
	printf("\n\n##\n##\nCreated structure for Wilson-FDL isotherm.");
	printf("\nSelected working pair is \"[BMIM]+[(CF3SO2)2N]-(2) / H2O\".");

	printf("\n\nFor T = %f K and x = %f mol/mol, activity coefficient of H2O "
		"results in gamma_1 = %f.",	T_K, x_molmol, gamma_1);
	printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure results in "
		"p = %f Pa.", T_K, x_molmol, p_Pa);
	printf("\nFor p = %f Pa and T = %f K, equilibrium mole fraction results in "
		"x = %f mol/mol.", p_Pa, T_K, x_inv_molmol);


	// Define parameter record for executing Wilson equation. Data for working
	// pair "[BMIM]+[(CF3SO2)2N]-(2) / H2O" is taken from:
	//
	// 	Döker M. and J. Gmehling Measurement and prediction of vapor–liquid
	//	equilibria of ternary systems containing ionic liquids. Fluid phase
	//	equilibria 2005. 227(2): p. 255-266.
	//
	double par_wilson_tdl[] = {-149.128 * 8.314462618, 0.959291 *
		8.314462618, 368.189 * 8.314462618, 0.929126 * 8.314462618, 273.15,
		0.0014, 0.0014};

	// Define parameter record for executing vapor pressure equation and
	// saturated liquid density equation. Data for refrigerant "R-134a" is taken
	// from:
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
	T_K = 303.24;
	x_molmol = 0.9203;
	psat_Pa = refrigerant_p_sat(T_K, r134a_par);

	// Calculate equilibrium data
	//
	Absorption *isotherm_wilson_tdl = newAbsorption("wilson-dl-t");

	gamma_1 = isotherm_wilson_tdl->act_g_Txv1v2_w_v(T_K , x_molmol, -1,
		-1, par_wilson_tdl);
	p_Pa = isotherm_wilson_tdl->act_p_Txv1v2gpsat_w_v_gf(T_K , x_molmol, -1,
		-1, isotherm_wilson_tdl->act_g_Txv1v2_w_v, psat_Pa, par_wilson_tdl);
	x_inv_molmol = isotherm_wilson_tdl->act_x_pTv1v2gpsat_w_v_gf(
		p_Pa, T_K, -1, -1, isotherm_wilson_tdl->act_g_Txv1v2_w_v, psat_Pa,
		par_wilson_tdl);

	// Print calculated values
	//
	printf("\n\n##\n##\nCreated structure for Wilson-TDL isotherm.");
	printf("\nSelected working pair is \"Triethylene glycol dimethyl "
		"ether / R-134a.");

	printf("\n\nFor T = %f K and x = %f mol/mol, activity coefficient of "
		"R-134a results in gamma_1 = %f.",	T_K, x_molmol, gamma_1);
	printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure results in "
		"p = %f Pa.", T_K, x_molmol, p_Pa);
	printf("\nFor p = %f Pa and T = %f K, equilibrium mole fraction results in "
		"x = %f mol/mol.", p_Pa, T_K, x_inv_molmol);


	// Define parameter record for executing Wilson equation. Data for working
	// pair "PAG / R-134a" is taken from:
	//
	// 	Martz W.L. and A. Jacobi Refrigerant-oil mixtures and local composition
	//	modeling. 1994 Air Conditioning and Refrigeration Center. College of
	//	Engineering. University of Illinois at Urbana-Champaign.
	//
	double par_tsuboka_katayama[] = {247, 15259, 1, 1};

	// Define some input values for calculating equilibrium data
	//
	T_K = 323.15;
	x_molmol =  0.93946701;
	psat_Pa = refrigerant_p_sat(T_K, r134a_par);

	// Calculate equilibrium data
	//
	Absorption *isotherm_tsuboka_katayama = newAbsorption("tsuboka-katayama");

	gamma_1 = isotherm_tsuboka_katayama->act_g_Txv1v2_w_v(T_K , x_molmol, -1,
		-1, par_tsuboka_katayama);
	p_Pa = isotherm_tsuboka_katayama->act_p_Txv1v2psat_w_v(T_K , x_molmol, -1,
		-1, psat_Pa, par_tsuboka_katayama);
	x_inv_molmol = isotherm_tsuboka_katayama->act_x_pTv1v2psat_w_v(
		p_Pa, T_K, -1, -1, psat_Pa, par_tsuboka_katayama);

	// Print calculated values
	//
	printf("\n\n##\n##\nCreated structure for Tsubo & Kakatayama isotherm.");
	printf("\nSelected working pair is \"PAG / R-134a\".");

	printf("\n\nFor T = %f K and x = %f mol/mol, activity coefficient of "
		"R-134a  results in gamma_1 = %f.", T_K, x_molmol, gamma_1);
	printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure results in "
		"p = %f Pa.", T_K, x_molmol, p_Pa);
	printf("\nFor p = %f Pa and T = %f K, equilibrium mole fraction results in "
		"x = %f mol/mol.", p_Pa, T_K, x_inv_molmol);


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
	T_K = 323.15;
	x_molmol =  0.93946701;
	psat_Pa = refrigerant_p_sat(T_K, r134a_par);

	// Calculate equilibrium data
	//
	Absorption *isotherm_heil = newAbsorption("heil");

	gamma_1 = isotherm_heil->act_g_Txv1v2_w_v(T_K , x_molmol, -1, -1,
		par_heil);
	p_Pa = isotherm_heil->act_p_Txv1v2psat_w_v(T_K , x_molmol, -1, -1, psat_Pa,
		par_heil);
	x_inv_molmol = isotherm_heil->act_x_pTv1v2psat_w_v(
		p_Pa, T_K, -1, -1, psat_Pa, par_heil);

	// Print calculated values
	//
	printf("\n\n##\n##\nCreated structure for Heil isotherm.");
	printf("\nSelected working pair is \"PAG / R-134a\".");

	printf("\n\nFor T = %f K and x = %f mol/mol, activity coefficient of "
		"R-134a  results in gamma_1 = %f.", T_K, x_molmol, gamma_1);
	printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure results in "
		"p = %f Pa.", T_K, x_molmol, p_Pa);
	printf("\nFor p = %f Pa and T = %f K, equilibrium mole fraction results in "
		"x = %f mol/mol.", p_Pa, T_K, x_inv_molmol);


	// Define parameter record for executing Wilson equation. Data for working
	// pair "PAG / R-134a" is taken from:
	//
	// 	Martz W.L. and A. Jacobi Refrigerant-oil mixtures and local composition
	//	modeling. 1994 Air Conditioning and Refrigeration Center. College of
	//	Engineering. University of Illinois at Urbana-Champaign.
	//
	double par_wangchao[] = {2715, -1609, 6, 1, 1};

	// Define some input values for calculating equilibrium data
	//
	T_K = 323.15;
	x_molmol =  0.93946701;
	psat_Pa = refrigerant_p_sat(T_K, r134a_par);

	// Calculate equilibrium data
	//
	Absorption *isotherm_wangchao = newAbsorption("wang-chao");

	gamma_1 = isotherm_wangchao->act_g_Txv1v2_w_v(T_K , x_molmol, -1, -1,
		par_wangchao);
	p_Pa = isotherm_wangchao->act_p_Txv1v2psat_w_v(T_K , x_molmol, -1, -1,
		psat_Pa, par_wangchao);
	x_inv_molmol = isotherm_wangchao->act_x_pTv1v2psat_w_v(
		p_Pa, T_K, -1, -1, psat_Pa, par_wangchao);

	// Print calculated values
	//
	printf("\n\n##\n##\nCreated structure for Wang-Chao isotherm.");
	printf("\nSelected working pair is \"PAG / R-134a\".");

	printf("\n\nFor T = %f K and x = %f mol/mol, activity coefficient of "
		"R-134a  results in gamma_1 = %f.", T_K, x_molmol, gamma_1);
	printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure results in "
		"p = %f Pa.", T_K, x_molmol, p_Pa);
	printf("\nFor p = %f Pa and T = %f K, equilibrium mole fraction results in "
		"x = %f mol/mol.", p_Pa, T_K, x_inv_molmol);


	// Define parameter record for executing UNIQUAC equation. Data for working
	// pair "PAG / R-134a" is taken from:
	//
	// 	Martz W.L. and A. Jacobi Refrigerant-oil mixtures and local composition
	//	modeling. 1994 Air Conditioning and Refrigeration Center. College of
	//	Engineering. University of Illinois at Urbana-Champaign.
	//
	double par_nrtl_fdg[] = {-4788, -359, 0.5};

	// Define some input values for calculating equilibrium data
	//
	T_K = 323.15;
	x_molmol =  0.93946701;
	psat_Pa = refrigerant_p_sat(T_K, r134a_par);

	// Calculate equilibrium data
	//
	Absorption *isotherm_nrtl_fdg = newAbsorption("nrtl-fixeddg");

	gamma_1 = isotherm_nrtl_fdg->act_g_Tx_wo_v(T_K , x_molmol, par_nrtl_fdg);
	p_Pa = isotherm_nrtl_fdg->act_p_Txgpsat_w_gf(T_K , x_molmol,
		isotherm_nrtl_fdg->act_g_Tx_wo_v , psat_Pa, par_nrtl_fdg);
	x_inv_molmol = isotherm_nrtl_fdg->act_x_pTgpsat_w_gf(
		p_Pa, T_K, isotherm_nrtl_fdg->act_g_Tx_wo_v, psat_Pa, par_nrtl_fdg);

	// Print calculated values
	//
	printf("\n\n##\n##\nCreated structure for NRTL-FDG isotherm.");
	printf("\nSelected working pair is \"PAG / R-134a\".");

	printf("\n\nFor T = %f K and x = %f mol/mol, activity coefficient of "
		"R-134a  results in gamma_1 = %f.", T_K, x_molmol, gamma_1);
	printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure results in "
		"p = %f Pa.", T_K, x_molmol, p_Pa);
	printf("\nFor p = %f Pa and T = %f K, equilibrium mole fraction results in "
		"x = %f mol/mol.", p_Pa, T_K, x_inv_molmol);


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

	// Define parameter record for executing vapor pressure equation. Data for
	// refrigerant "Benzene" is taken from:
	//
	// 	NIST Webbook.
	//
	double benzene_par[] = {4.72583, 1660.652, -1.461};

	// Define some input values for calculating equilibrium data
	//
	T_K = 353.15;
	x_molmol = 0.6293;
	psat_Pa = refrigerant_p_sat_antoine(T_K, benzene_par);

	// Calculate equilibrium data
	//
	Absorption *isotherm_nrtl_dgt = newAbsorption("nrtl-dg-t");

	gamma_1 = isotherm_nrtl_dgt->act_g_Tx_wo_v(T_K , x_molmol, par_nrtl_dgt);
	p_Pa = isotherm_nrtl_dgt->act_p_Txgpsat_w_gf(T_K , x_molmol,
		isotherm_nrtl_dgt->act_g_Tx_wo_v , psat_Pa, par_nrtl_dgt);
	x_inv_molmol = isotherm_nrtl_dgt->act_x_pTgpsat_w_gf(
		p_Pa, T_K, isotherm_nrtl_dgt->act_g_Tx_wo_v, psat_Pa, par_nrtl_dgt);

	// Print calculated values
	//
	printf("\n\n##\n##\nCreated structure for NRTL-DGT isotherm.");
	printf("\nSelected working pair is \"[BMIM]+[(CF3SO2)2N]-(2) / Benzene\".");

	printf("\n\nFor T = %f K and x = %f mol/mol, activity coefficient of "
		"benzene  results in gamma_1 = %f.", T_K, x_molmol, gamma_1);
	printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure results in "
		"p = %f Pa.", T_K, x_molmol, p_Pa);
	printf("\nFor p = %f Pa and T = %f K, equilibrium mole fraction results in "
		"x = %f mol/mol.", p_Pa, T_K, x_inv_molmol);


	// Define parameter record for executing UNIQUAC equation. Data for working
	// pair "[BMIM]+[(CF3SO2)2N]-(2) / H2O" is taken from:
	//
	// 	Döker M. and J. Gmehling Measurement and prediction of vapor–liquid
	//	equilibria of ternary systems containing ionic liquids. Fluid phase
	//	equilibria 2005. 227(2): p. 255-266.
	//
	double par_uniquac_fdu[] = {345.30552, 3057.12328, 1.4, 10.2, 0.92, 11.16,
		10};

	// Define some input values for calculating equilibrium data
	//
	T_K = 353.15;
	x_molmol = 0.1933;
	psat_Pa = refrigerant_p_sat(T_K, H2O_par);

	// Calculate equilibrium data
	//
	Absorption *isotherm_uniquac_fdu = newAbsorption("uniquac-fixeddu");

	gamma_1 = isotherm_uniquac_fdu->act_g_Tx_wo_v(T_K, x_molmol,
		par_uniquac_fdu);
	p_Pa = isotherm_uniquac_fdu->act_p_Txgpsat_w_gf(T_K, x_molmol,
		isotherm_uniquac_fdu->act_g_Tx_wo_v, psat_Pa, par_uniquac_fdu);
	x_inv_molmol = isotherm_uniquac_fdu->act_x_pTgpsat_w_gf(p_Pa, T_K,
		isotherm_uniquac_fdu->act_g_Tx_wo_v, psat_Pa, par_uniquac_fdu);

	// Print calculated values
	//
	printf("\n\n##\n##\nCreated structure for UNIQUAC-FDU isotherm.");
	printf("\nSelected working pair is \"[BMIM]+[(CF3SO2)2N]-(2) / H2O\".");

	printf("\n\nFor T = %f K and x = %f mol/mol, activity coefficient of "
		"H2O  results in gamma_1 = %f.", T_K, x_molmol, gamma_1);
	printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure results in "
		"p = %f Pa.", T_K, x_molmol, p_Pa);
	printf("\nFor p = %f Pa and T = %f K, equilibrium mole fraction results in "
		"x = %f mol/mol.", p_Pa, T_K, x_inv_molmol);


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
		10.2, 1.824, 11.2, 0.01154, 6};

	// Define some input values for calculating equilibrium data
	//
	T_K = 353.15;
	x_molmol =  0.6293;
	psat_Pa = refrigerant_p_sat_antoine(T_K, benzene_par);

	// Calculate equilibrium data
	//
	Absorption *isotherm_uniquac_dut = newAbsorption("uniquac-du-t");

	gamma_1 = isotherm_uniquac_dut->act_g_Tx_wo_v(T_K, x_molmol,
		par_uniquac_dut);
	p_Pa = isotherm_uniquac_dut->act_p_Txgpsat_w_gf(T_K, x_molmol,
		isotherm_uniquac_dut->act_g_Tx_wo_v, psat_Pa, par_uniquac_dut);
	x_inv_molmol = isotherm_uniquac_dut->act_x_pTgpsat_w_gf(p_Pa, T_K,
		isotherm_uniquac_dut->act_g_Tx_wo_v, psat_Pa, par_uniquac_dut);

	// Print calculated values
	//
	printf("\n\n##\n##\nCreated structure for NRTL-DGT isotherm.");
	printf("\nSelected working pair is \"[BMIM]+[(CF3SO2)2N]-(2) / Benzene\".");

	printf("\n\nFor T = %f K and x = %f mol/mol, activity coefficient of "
		"benzene  results in gamma_1 = %f.", T_K, x_molmol, gamma_1);
	printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure results in "
		"p = %f Pa.", T_K, x_molmol, p_Pa);
	printf("\nFor p = %f Pa and T = %f K, equilibrium mole fraction results in "
		"x = %f mol/mol.", p_Pa, T_K, x_inv_molmol);


	// Define parameter record for executing Wilson equation. Data for working
	// pair "Lubricant PEB9 / HFC134a" is taken from:
	//
	// 	Wahlström Å. and L. Vamling Solubility of HFCs in pentaerythritol
	//	tetraalkyl esters. Journal of Chemical & Engineering Data 2000. 45(1):
	//	p. 97-103.
	//
	double par_flory_huggins[] = {10.41, 938, -176};

	// Define some input values for calculating equilibrium data
	//
	T_K = 303.15;
	x_molmol =  0.1622;
	psat_Pa = refrigerant_p_sat(T_K, r134a_par);

	// Calculate equilibrium data
	//
	Absorption *isotherm_flory_huggins = newAbsorption("flory-huggins");

	gamma_1 = isotherm_flory_huggins->act_g_Tx_wo_v(T_K, x_molmol,
		par_flory_huggins);
	p_Pa = isotherm_flory_huggins->act_p_Txpsat_wo_v(T_K, x_molmol, psat_Pa,
		par_flory_huggins);
	x_inv_molmol = isotherm_flory_huggins->act_x_pTpsat_wo_v(p_Pa, T_K,
		psat_Pa, par_flory_huggins);

	// Print calculated values
	//
	printf("\n\n##\n##\nCreated structure for Flory-Huggins isotherm.");
	printf("\nSelected working pair is \"Lubricant PEB9 / R-134a\".");

	printf("\n\nFor T = %f K and x = %f mol/mol, activity coefficient of "
		"R-134a  results in gamma_1 = %f.", T_K, x_molmol, gamma_1);
	printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure results in "
		"p = %f Pa.", T_K, x_molmol, p_Pa);
	printf("\nFor p = %f Pa and T = %f K, equilibrium mole fraction results in "
		"x = %f mol/mol.", p_Pa, T_K, x_inv_molmol);


	// Free memory that is allocated for structures
	//
	delAbsorption(isotherm_flory_huggins);
	delAbsorption(isotherm_uniquac_dut);
	delAbsorption(isotherm_uniquac_fdu);
	delAbsorption(isotherm_nrtl_dgt);
	delAbsorption(isotherm_nrtl_fdg);
	delAbsorption(isotherm_wangchao);
	delAbsorption(isotherm_heil);
	delAbsorption(isotherm_tsuboka_katayama);
	delAbsorption(isotherm_wilson_tdl);
	delAbsorption(isotherm_wilson_fdl);
	delAbsorption(isotherm_antoine);
	delAbsorption(isotherm_duehring);

    return EXIT_SUCCESS;
}