////////////////////////
// test_workingPair.c //
////////////////////////
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "workingPair.h"
#include "structDefinitions.c"


//////////////////////////
// Definition of macros //
//////////////////////////
#ifdef __unix
	#define PATH ".//data//sorpproplib_ValidationCInterface.json"
#else
	#define PATH ".\\data\\sorpproplib_ValidationCInterface.json"
#endif


/////////////////////////////
// Definition of functions //
/////////////////////////////
void testWorkingPair_ads(const char *path_db, const char *wp_as,
	const char *wp_st, const char *wp_rf, const char *wp_iso, int no_iso,
	const char *rf_psat, int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Initialize working pair
	//
	WorkingPair *workingPair = newWorkingPair(path_db, wp_as, wp_st, wp_rf,
		wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol);

	if (workingPair != NULL) {
		// Define some random parameters to calculate equilibrium properties
		//
		double p_Pa = 0.1e6;
		double T_K = 303.15;

		// Execute equilibrium functions that are always defined
		//
		double w_kgkg = ads_w_pT(p_Pa, T_K, workingPair);
		double dw_dp_kgkgPa = ads_dw_dp_pT(p_Pa, T_K, workingPair);
		double dw_dT_kgkgK = ads_dw_dT_pT(p_Pa, T_K, workingPair);
		double p_Pa_inv = ads_p_wT(w_kgkg, T_K, workingPair);
		double dp_dw_Pakgkg = ads_dp_dw_wT(w_kgkg, T_K, workingPair);
		double dp_dT_PaK = ads_dp_dT_wT(w_kgkg, T_K, workingPair);
		double T_K_inv = ads_T_pw(p_Pa, w_kgkg, workingPair);
		double piStar_molkg = ads_piStar_pyxgTM(p_Pa, 1, 1, 1, T_K,
			0.04401, workingPair);

		double w_kgkg_direct = direct_ads_w_pT(
			p_Pa, T_K, path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso,
			rf_psat, no_p_sat, rf_rhol, no_rhol);
		double p_Pa_inv_direct = direct_ads_p_wT(
			w_kgkg_direct, T_K, path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso,
			rf_psat, no_p_sat, rf_rhol, no_rhol);
		double T_K_inv_direct = direct_ads_T_pw(
			p_Pa, w_kgkg_direct, path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso,
			rf_psat, no_p_sat, rf_rhol, no_rhol);
		double dw_dp_kgkgPa_direct = direct_ads_dw_dp_pT(
			p_Pa, T_K, path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso,
			rf_psat, no_p_sat, rf_rhol, no_rhol);
		double dw_dT_kgkgK_direct = direct_ads_dw_dT_pT(
			p_Pa, T_K, path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso,
			rf_psat, no_p_sat, rf_rhol, no_rhol);
		double dp_dw_Pakgkg_direct = direct_ads_dp_dw_wT(
			w_kgkg_direct, T_K, path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso,
			rf_psat, no_p_sat, rf_rhol, no_rhol);
		double dp_dT_PaK_direct = direct_ads_dp_dT_wT(
			w_kgkg_direct, T_K, path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso,
			rf_psat, no_p_sat, rf_rhol, no_rhol);
		double piStar_molkg_direct = direct_ads_piStar_pyxgTM(
			p_Pa, 1, 1, 1, T_K, 0.04401, path_db, wp_as, wp_st, wp_rf, wp_iso,
			no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol);

		// Execute equilibrium functions that are only defined for isotherm
		// models based on the surface approach
		//
		double w_kgkg_sur = ads_sur_w_pT(p_Pa, T_K, workingPair);
		double dw_dp_kgkgPa_sur = ads_sur_dw_dp_pT(p_Pa, T_K, workingPair);
		double dw_dT_kgkgK_sur = ads_sur_dw_dT_pT(p_Pa, T_K, workingPair);
		double p_Pa_inv_sur = ads_sur_p_wT(w_kgkg_sur, T_K, workingPair);
		double dp_dw_Pakgkg_sur = ads_sur_dp_dw_wT(w_kgkg_sur, T_K,
			workingPair);
		double dp_dT_PaK_sur = ads_sur_dp_dT_wT(w_kgkg_sur, T_K, workingPair);
		double T_K_inv_sur = ads_sur_T_pw(p_Pa, w_kgkg_sur, workingPair);
		double piStar_molkg_sur = ads_sur_piStar_pyxgTM(p_Pa, 1, 1, 1, T_K,
			0.04401, workingPair);

		double w_kgkg_sur_direct = direct_ads_sur_w_pT(
			p_Pa, T_K, path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso,
			rf_psat, no_p_sat, rf_rhol, no_rhol);
		double p_Pa_inv_sur_direct = direct_ads_sur_p_wT(
			w_kgkg_direct, T_K, path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso,
			rf_psat, no_p_sat, rf_rhol, no_rhol);
		double T_K_inv_sur_direct = direct_ads_sur_T_pw(
			p_Pa, w_kgkg_direct, path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso,
			rf_psat, no_p_sat, rf_rhol, no_rhol);
		double dw_dp_kgkgPa_sur_direct = direct_ads_sur_dw_dp_pT(
			p_Pa, T_K, path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso,
			rf_psat, no_p_sat, rf_rhol, no_rhol);
		double dw_dT_kgkgK_sur_direct = direct_ads_sur_dw_dT_pT(
			p_Pa, T_K, path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso,
			rf_psat, no_p_sat, rf_rhol, no_rhol);
		double dp_dw_Pakgkg_sur_direct = direct_ads_sur_dp_dw_wT(
			w_kgkg_direct, T_K, path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso,
			rf_psat, no_p_sat, rf_rhol, no_rhol);
		double dp_dT_PaK_sur_direct = direct_ads_sur_dp_dT_wT(
			w_kgkg_direct, T_K, path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso,
			rf_psat, no_p_sat, rf_rhol, no_rhol);
		double piStar_molkg_sur_direct = direct_ads_sur_piStar_pyxgTM(
			p_Pa, 1, 1, 1, T_K, 0.04401, path_db, wp_as, wp_st, wp_rf, wp_iso,
			no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol);

		// Execute equilibrium functions that are only defined for isotherm
		// models based on the surface approach and using saturated vapor
		// pressure as additional function argument
		//
		double p_sat_Pa = ref_p_sat_T(T_K, workingPair);
		double dp_sat_dT_PaK = ref_dp_sat_dT_T(T_K, workingPair);

		double w_kgkg_surPsat = ads_sur_w_pTpsat(p_Pa, T_K, p_sat_Pa,
			workingPair);
		double dw_dp_kgkgPa_surPsat = ads_sur_dw_dp_pTpsat(p_Pa, T_K, p_sat_Pa,
			workingPair);
		double dw_dT_kgkgK_surPsat = ads_sur_dw_dT_pTpsat(p_Pa, T_K, p_sat_Pa,
			dp_sat_dT_PaK, workingPair);
		double p_Pa_inv_surPsat = ads_sur_p_wTpsat(w_kgkg_sur, T_K, p_sat_Pa,
			workingPair);
		double dp_dw_Pakgkg_surPsat = ads_sur_dp_dw_wTpsat(w_kgkg_sur, T_K,
			p_sat_Pa, workingPair);
		double dp_dT_PaK_surPsat = ads_sur_dp_dT_wTpsat(w_kgkg_sur, T_K,
			dp_sat_dT_PaK, p_sat_Pa, workingPair);
		double T_K_inv_surPsat = ads_sur_T_pwpsat(p_Pa, w_kgkg_sur, p_sat_Pa,
			workingPair);
		double piStar_molkg_surPsat = ads_sur_piStar_pyxgTpsatM(p_Pa, 1, 1, 1,
			T_K, p_sat_Pa, 0.04401, workingPair);

		double w_kgkg_surPsat_direct = direct_ads_sur_w_pTpsat(
			p_Pa, T_K, p_sat_Pa, path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso,
			rf_psat, no_p_sat, rf_rhol, no_rhol);
		double p_Pa_inv_surPsat_direct = direct_ads_sur_p_wTpsat(
			w_kgkg_direct, T_K, p_sat_Pa, path_db, wp_as, wp_st, wp_rf, wp_iso,
			no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol);
		double T_K_inv_surPsat_direct = direct_ads_sur_T_pwpsat(
			p_Pa, w_kgkg_direct, p_sat_Pa, path_db, wp_as, wp_st, wp_rf, wp_iso,
			no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol);
		double dw_dp_kgkgPa_surPsat_direct = direct_ads_sur_dw_dp_pTpsat(
			p_Pa, T_K, p_sat_Pa, path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso,
			rf_psat, no_p_sat, rf_rhol, no_rhol);
		double dw_dT_kgkgK_surPsat_direct = direct_ads_sur_dw_dT_pTpsat(
			p_Pa, T_K, p_sat_Pa, dp_sat_dT_PaK, path_db, wp_as, wp_st, wp_rf,
			wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol);
		double dp_dw_Pakgkg_surPsat_direct = direct_ads_sur_dp_dw_wTpsat(
			w_kgkg_direct, T_K, p_sat_Pa, path_db, wp_as, wp_st, wp_rf, wp_iso,
			no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol);
		double dp_dT_PaK_surPsat_direct = direct_ads_sur_dp_dT_wTpsat(
			w_kgkg_direct, T_K, p_sat_Pa, dp_sat_dT_PaK, path_db, wp_as, wp_st,
			wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol);
		double piStar_molkg_surPsat_direct = direct_ads_sur_piStar_pyxgTpsatM(
			p_Pa, 1, 1, 1, T_K, p_sat_Pa, 0.04401, path_db, wp_as, wp_st, wp_rf,
			wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol);

		// Calculate equilibrium properties with functions that are only defined
		// for models based on the volumetric approach
		//
		double rho_kgm3 = ref_rho_l_T(T_K, workingPair);
		double drho_dT_kgm3K = ref_drho_l_dT_T(T_K, workingPair);
		double A_Jmol = 8.314462618 * T_K * log(p_sat_Pa/p_Pa);

		double W_m3kg = ads_vol_W_ARho(A_Jmol, rho_kgm3, workingPair);
		double dW_dA_m3molkgJ = ads_vol_dW_dA_ARho(A_Jmol, rho_kgm3,
			workingPair);
		double A_Jmol_inv = ads_vol_A_WRho(W_m3kg, rho_kgm3, workingPair);
		double dA_dW_Jkgkgmol = ads_vol_dA_dW_WRho(W_m3kg, rho_kgm3,
			workingPair);
		double w_kgkg_vol = ads_vol_w_pTpsatRho(p_Pa, T_K, p_sat_Pa,
			rho_kgm3, workingPair);
		double dw_dp_kgkgPa_vol = ads_vol_dw_dp_pTpsatRho(p_Pa, T_K, p_sat_Pa,
			rho_kgm3, workingPair);
		double dw_dT_kgkgK_vol = ads_vol_dw_dT_pTpsatRho(p_Pa, T_K, p_sat_Pa,
			rho_kgm3, dp_sat_dT_PaK, drho_dT_kgm3K, workingPair);
		double p_Pa_vol_inv = ads_vol_p_wTpsatRho(w_kgkg_vol, T_K, p_sat_Pa,
			rho_kgm3, workingPair);
		double dp_dT_PaK_vol = ads_vol_dp_dT_wTpsatRho(w_kgkg_vol, T_K,
			p_sat_Pa, rho_kgm3, dp_sat_dT_PaK, drho_dT_kgm3K, workingPair);
		double dp_dw_Pakgkg_vol = ads_vol_dp_dw_wTpsatRho(w_kgkg_vol, T_K,
			p_sat_Pa, rho_kgm3, workingPair);
		double T_K_vol_inv = ads_vol_T_pwpsatRho(p_Pa, w_kgkg_vol, p_sat_Pa,
			rho_kgm3, workingPair);
		double piStar_molkg_vol = ads_vol_piStar_pyxgTpsatRhoM(p_Pa, 1, 1, 1,
			T_K, p_sat_Pa, rho_kgm3, 0.04401, workingPair);

		double W_m3kg_direct = direct_ads_vol_W_ARho(
			A_Jmol, rho_kgm3, path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso,
			rf_psat, no_p_sat, rf_rhol, no_rhol);
		double dW_dA_m3molkgJ_direct = direct_ads_vol_dW_dA_ARho(
			A_Jmol, rho_kgm3, path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso,
			rf_psat, no_p_sat, rf_rhol, no_rhol);
		double A_Jmol_inv_direct = direct_ads_vol_A_WRho(
			W_m3kg, rho_kgm3, path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso,
			rf_psat, no_p_sat, rf_rhol, no_rhol);
		double dA_dW_Jkgkgmol_direct = direct_ads_vol_dA_dW_WRho(
			W_m3kg, rho_kgm3, path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso,
			rf_psat, no_p_sat, rf_rhol, no_rhol);

		double w_kgkg_vol_direct = direct_ads_vol_w_pTpsatRho(
			p_Pa, T_K, p_sat_Pa, rho_kgm3, path_db, wp_as, wp_st, wp_rf, wp_iso,
			no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol);
		double dw_dp_kgkgPa_vol_direct = direct_ads_vol_dw_dp_pTpsatRho(
			p_Pa, T_K, p_sat_Pa, rho_kgm3, path_db, wp_as, wp_st, wp_rf, wp_iso,
			no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol);
		double dw_dT_kgkgK_vol_direct = direct_ads_vol_dw_dT_pTpsatRho(
			p_Pa, T_K, p_sat_Pa, rho_kgm3, dp_sat_dT_PaK, drho_dT_kgm3K,
			path_db, wp_as, wp_st, wp_rf, wp_iso,	no_iso, rf_psat, no_p_sat,
			rf_rhol, no_rhol);
		double p_Pa_vol_inv_direct = direct_ads_vol_p_wTpsatRho(
			w_kgkg_vol, T_K, p_sat_Pa, rho_kgm3, path_db, wp_as, wp_st, wp_rf,
			wp_iso,	no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol);
		double dp_dw_Pakgkg_vol_direct = direct_ads_vol_dp_dw_wTpsatRho(
			w_kgkg_vol, T_K, p_sat_Pa, rho_kgm3, path_db, wp_as, wp_st, wp_rf,
			wp_iso,	no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol);
		double dp_dT_PaK_vol_direct = direct_ads_vol_dp_dT_wTpsatRho(
			w_kgkg_vol, T_K, p_sat_Pa, rho_kgm3, dp_sat_dT_PaK, drho_dT_kgm3K,
			path_db, wp_as, wp_st, wp_rf, wp_iso,	no_iso, rf_psat, no_p_sat,
			rf_rhol, no_rhol);
		double T_K_vol_inv_direct = direct_ads_vol_T_pwpsatRho(
			p_Pa, w_kgkg_vol, p_sat_Pa, rho_kgm3, path_db, wp_as, wp_st, wp_rf,
			wp_iso,	no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol);
		double piStar_molkg_vol_direct = direct_ads_vol_piStar_pyxgTpsatRhoM(
			p_Pa, 1, 1, 1, T_K, p_sat_Pa, rho_kgm3, 0.04401, path_db, wp_as,
			wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol);

		// Print general information of selected working pair
		//
		printf("\n\n#############################");
		printf("\n#############################");
		printf("\n## Test WorkingPair-struct ##");
		printf("\n#############################");
		printf("\n#############################");

		printf("\n\n\nGeneral information of working pair:");
		printf("\n------------------------------------");
		printf("\nSelected sorbent is: %s.", workingPair->wp_as);
		printf("\nSelected sub-type of sorbent is: %s.", workingPair->wp_st);
		printf("\nSelected refrigerant is: %s.", workingPair->wp_rf);
		printf("\nSelected isotherm is: %s / ID %i.", workingPair->wp_iso,
			workingPair->no_iso);
		printf("\nSelected calculation approach for vapor pressure is: %s / "
			" ID %i.", workingPair->rf_psat, workingPair->no_p_sat);
		printf("\nSelected calculation approach for saturated liquid density "
			"is: %s / ID %i.", workingPair->rf_rhol, workingPair->no_rhol);

		// Print calculated values
		//
		printf("\n\n\nResults of isotherm functions that are always defined:");
		printf("\n------------------------------------------------------");
		printf("\nFor T = %f K and p = %f Pa, loading results in w = %f "
			"kg/kg.", T_K, p_Pa, w_kgkg);
		printf("\nFor T = %f K and w = %f kg/kg, pressure results in p = %f "
			"Pa.", T_K, w_kgkg, p_Pa_inv);
		printf("\nFor p = %f Pa and w = %f kg/kg, temperature results in T = "
			"%f K.", p_Pa, w_kgkg, T_K_inv);
		printf("\n\nFor T = %f K and p = %f Pa, derivative of loading wrt. "
			"pressure results in dw_dp = %f kg/kg/Pa.", T_K, p_Pa,
			dw_dp_kgkgPa);
		printf("\nFor T = %f K and p = %f Pa, derivative of loading wrt. "
			"temperature results in dw_dT = %f kg/kg/K.", T_K, p_Pa,
			dw_dT_kgkgK);
		printf("\nFor T = %f K and w = %f kg/kg, derivative of p with respect "
			"to w results in dp_dw = %f Pakg/kg.", T_K, w_kgkg,
			dp_dw_Pakgkg);
		printf("\nFor T = %f K and w = %f kg/kg, derivative of p with respect "
			"to T results in dp_dT = %f Pa/K.", T_K, w_kgkg, dp_dT_PaK);
		printf("\n\nFor T = %f K and p = %f Pa, reduced spreading pressure "
			"results in piStar = %f mol/kg.", T_K, p_Pa, piStar_molkg);

		printf("\n\n\nResults of isotherm functions that are only defined for "
			"isotherm models based on the surface approach:");
		printf("\n-------------------------------------------------------------"
			"-----------------------------------------");
		printf("\nFor T = %f K and p = %f Pa, loading results in w = %f "
			"kg/kg.", T_K, p_Pa, w_kgkg_sur);
		printf("\nFor T = %f K and w = %f kg/kg, pressure results in p = %f "
			"Pa.", T_K, w_kgkg_sur, p_Pa_inv_sur);
		printf("\nFor p = %f Pa and w = %f kg/kg, temperature results in T = "
			"%f K.", p_Pa, w_kgkg_sur, T_K_inv_sur);
		printf("\n\nFor T = %f K and p = %f Pa, derivative of loading wrt. "
			"pressure results in dw_dp = %f kg/kg/Pa.", T_K, p_Pa,
			dw_dp_kgkgPa_sur);
		printf("\nFor T = %f K and p = %f Pa, derivative of loading wrt. "
			"temperature results in dw_dT = %f kg/kg/K.", T_K, p_Pa,
			dw_dT_kgkgK_sur);
		printf("\nFor T = %f K and w = %f kg/kg, derivative of p with respect "
			"to w results in dp_dw = %f Pakg/kg.", T_K, w_kgkg_sur,
			dp_dw_Pakgkg_sur);
		printf("\nFor T = %f K and w = %f kg/kg, derivative of p with respect "
			"to T results in dp_dT = %f Pa/K.", T_K, w_kgkg_sur, dp_dT_PaK_sur);
		printf("\n\nFor T = %f K and p = %f Pa, reduced spreading pressure "
			"results in piStar = %f mol/kg.", T_K, p_Pa, piStar_molkg_sur);

		printf("\n\n\nResults of isotherm functions that are only defined for "
			"isotherm models based on the surface approach and using saturated "
			"vapor as additional function argument:");
		printf("\n-------------------------------------------------------------"
			"------------------------------------------------------------------"
			"---------------------------------");
		printf("\nFor T = %f K, p = %f Pa, and p_sat = %f Pa, loading results "
			"in w = %f kg/kg.", T_K, p_Pa, p_sat_Pa, w_kgkg_surPsat);
		printf("\nFor T = %f K, w = %f kg/kg, and p_sat = %f Pa, pressure "
			"results in p = %f Pa.", T_K, w_kgkg_sur, p_sat_Pa,
			p_Pa_inv_surPsat);
		printf("\nFor p = %f Pa, w = %f kg/kg, and p_sat = %f Pa, temperature "
			"results in T = %f K.", p_Pa, w_kgkg_surPsat, p_sat_Pa,
			T_K_inv_surPsat);
		printf("\n\nFor T = %f K, p = %f Pa, and p_sat = %f Pa, derivative of "
			"loading wrt. pressure results in dw_dp = %f kg/kg/Pa.", T_K, p_Pa,
			p_sat_Pa, dw_dp_kgkgPa_surPsat);
		printf("\nFor T = %f K, p = %f Pa, and p_sat = %f Pa, derivative of "
			"loading wrt. temperature results in dw_dT = %f kg/kg/K.", T_K,
			p_Pa, p_sat_Pa, dw_dT_kgkgK_surPsat);
		printf("\nFor T = %f K, w = %f kg/kg, and p_sat = %f Pa, derivative of "
			"p with respect to w results in dp_dw = %f Pakg/kg.", T_K,
			w_kgkg_surPsat, p_sat_Pa, dp_dw_Pakgkg_surPsat);
		printf("\nFor T = %f K, w = %f kg/kg, and p_sat = %f Pa, derivative of "
			"p with respect to T results in dp_dT = %f Pa/K.", T_K,
			w_kgkg_surPsat, p_sat_Pa, dp_dT_PaK_surPsat);
		printf("\n\nFor T = %f K, p = %f Pa, and p_sat = %f Pa, reduced "
			"spreading pressure results in piStar = %f mol/kg.", T_K, p_Pa,
			p_sat_Pa, piStar_molkg_surPsat);

		printf("\n\n\nResults of isotherm functions that are only defined for "
			"isotherm models based on the volumetric approach:");
		printf("\n-------------------------------------------------------------"
			"--------------------------------------------");
		printf("\nFor A = %f J/mol, volumetric loading results in W = %f "
			"m3/kg.", A_Jmol, W_m3kg);
		printf("\nFor W = %f m3/kg, adsorption potential results in A = %f "
			"J/mol.", W_m3kg, A_Jmol_inv);
		printf("\n\nFor A = %f J/mol, derivative of volumetric loading wrt. of "
			"adsorption potential results in dW_dA = %f m3mol/kg/J.", A_Jmol,
			dW_dA_m3molkgJ);
		printf("\nFor W = %f m3/kg, derivative of adsorption potential wrt. "
			"volumetric loading results in dA_dW = %f Jkg/mol/kg.", W_m3kg,
			dA_dW_Jkgkgmol);

		printf("\n\nFor p = %f Pa, T = %f K, p_sat = %f Pa and rho_l = %f "
			"kg/m3, loading results in w = %f kg/kg.", p_Pa, T_K, p_sat_Pa,
			rho_kgm3, w_kgkg_vol);
		printf("\nFor w = %f kg/kg, T = %f K, p_sat = %f Pa and rho_l = %f "
			"kg/m3, pressure results in p = %f Pa.", w_kgkg_vol, T_K, p_sat_Pa,
			rho_kgm3, p_Pa_vol_inv);
		printf("\nFor p = %f Pa, w = %f kg/kg, p_sat = %f Pa and rho_l = %f "
			"kg/m3, temperature results in T = %f K.", p_Pa, w_kgkg_vol,
			p_sat_Pa, rho_kgm3, T_K_vol_inv);

		printf("\n\nFor p = %f Pa, T = %f K, p_sat = %f Pa and rho_l = %f "
			"kg/m3, derivative of loading wrt. pressure results in dw_dp = %f "
			"kg/kg/Pa.", p_Pa, T_K, p_sat_Pa, rho_kgm3, dw_dp_kgkgPa_vol);
		printf("\nFor p = %f Pa, T = %f K, p_sat = %f Pa and rho_l = %f kg/m3, "
			"derivative of loading wrt. temperature results in dw_dT = %f "
			"kg/kg/K.", p_Pa, T_K, p_sat_Pa, rho_kgm3, dw_dT_kgkgK_vol);
		printf("\nFor w = %f kg/kg, T = %f K, p_sat = %f Pa and rho_l = %f "
			"kg/m3, derivative of loading wrt. pressure results in dp_dw = %f "
			"Pakg/kg.", w_kgkg_vol, T_K, p_sat_Pa, rho_kgm3, dp_dw_Pakgkg_vol);
		printf("\nFor w = %f kg/kg, T = %f K, p_sat = %f Pa and rho_l = %f "
			"kg/m3, derivative of loading wrt. temperature results in dp_dT = "
			"%f Pa/K.", w_kgkg_vol, T_K, p_sat_Pa, rho_kgm3, dp_dT_PaK_vol);

		printf("\n\nFor p = %f Pa, T = %f K, p_sat = %f Pa and rho_l = %f "
			"kg/m3, reduced spreading pressure results in piStart = %f mol/kg.",
			p_Pa, T_K, p_sat_Pa, rho_kgm3, piStar_molkg_vol);

		printf("\n\n\nResults of isotherm functions that are always defined "
			"-> direct approach:");
		printf("\n-------------------------------------------------------------"
			"------------");
		printf("\nFor T = %f K and p = %f Pa, loading results in w = %f kg/kg.",
			T_K, p_Pa, w_kgkg_direct);
		printf("\nFor T = %f K and w = %f kg/kg, pressure results in p = %f "
			"Pa.", T_K, w_kgkg_direct, p_Pa_inv_direct);
		printf("\nFor p = %f Pa and w = %f kg/kg, temperature results in T = "
			"%f K.", p_Pa, w_kgkg_direct, T_K_inv_direct);

		printf("\n\nFor T = %f K and p = %f Pa, derivative of loading wrt. "
			"pressure results in dwdp = %f kg/kg/Pa.", T_K, p_Pa,
			dw_dp_kgkgPa_direct);
		printf("\nFor T = %f K and p = %f Pa, derivative of loading wrt. "
			"temperature results in dwdT = %f kg/kg/K.", T_K, p_Pa,
			dw_dT_kgkgK_direct);
		printf("\nFor T = %f K and w = %f kg/kg, derivative of p with respect "
			"to w results in dp_dw = %f Pakg/kg.", T_K, w_kgkg_direct,
			dp_dw_Pakgkg_direct);
		printf("\nFor T = %f K and w = %f kg/kg, derivative of p with respect "
			"to T results in dp_dT = %f Pa/K.", T_K, w_kgkg_direct,
			dp_dT_PaK_direct);

		printf("\n\nFor T = %f K and p = %f Pa, reduced spreading pressure "
			"results in piStar = %f mol/kg.", T_K, p_Pa,
			piStar_molkg_direct);

		printf("\n\n\nResults of isotherm functions that are only defined for "
			"isotherm models based on the surface approach -> direct "
			"approach:");
		printf("\n-------------------------------------------------------------"
			"------------------------------------------------------------");
		printf("\nFor T = %f K and p = %f Pa, loading results in w = %f "
			"kg/kg.", T_K, p_Pa, w_kgkg_sur_direct);
		printf("\nFor T = %f K and w = %f kg/kg, pressure results in p = %f "
			"Pa.", T_K, w_kgkg_sur_direct, p_Pa_inv_sur_direct);
		printf("\nFor p = %f Pa and w = %f kg/kg, temperature results in T = "
			"%f K.", p_Pa, w_kgkg_sur_direct, T_K_inv_sur_direct);
		printf("\n\nFor T = %f K and p = %f Pa, derivative of loading wrt. "
			"pressure results in dw_dp = %f kg/kg/Pa.", T_K, p_Pa,
			dw_dp_kgkgPa_sur_direct);
		printf("\nFor T = %f K and p = %f Pa, derivative of loading wrt. "
			"temperature results in dw_dT = %f kg/kg/K.", T_K, p_Pa,
			dw_dT_kgkgK_sur_direct);
		printf("\nFor T = %f K and w = %f kg/kg, derivative of p with respect "
			"to w results in dp_dw = %f Pakg/kg.", T_K, w_kgkg_sur_direct,
			dp_dw_Pakgkg_sur_direct);
		printf("\nFor T = %f K and w = %f kg/kg, derivative of p with respect "
			"to T results in dp_dT = %f Pa/K.", T_K, w_kgkg_sur_direct,
			dp_dT_PaK_sur_direct);
		printf("\n\nFor T = %f K and p = %f Pa, reduced spreading pressure "
			"results in piStar = %f mol/kg.", T_K, p_Pa,
			piStar_molkg_sur_direct);

		printf("\n\n\nResults of isotherm functions that are only defined for "
			"isotherm models based on the surface approach and using saturated "
			"vapor as additional function argument -> direct approach:");
		printf("\n-------------------------------------------------------------"
			"------------------------------------------------------------------"
			"-----------------------------------------------------");
		printf("\nFor T = %f K, p = %f Pa, and p_sat = %f Pa, loading results "
			"in w = %f kg/kg.", T_K, p_Pa, p_sat_Pa, w_kgkg_surPsat_direct);
		printf("\nFor T = %f K, w = %f kg/kg, and p_sat = %f Pa, pressure "
			"results in p = %f Pa.", T_K, w_kgkg_surPsat_direct, p_sat_Pa,
			p_Pa_inv_surPsat_direct);
		printf("\nFor p = %f Pa, w = %f kg/kg, and p_sat = %f Pa, temperature "
			"results in T = %f K.", p_Pa, w_kgkg_surPsat_direct, p_sat_Pa,
			T_K_inv_surPsat_direct);
		printf("\n\nFor T = %f K, p = %f Pa, and p_sat = %f Pa, derivative of "
			"loading wrt. pressure results in dw_dp = %f kg/kg/Pa.", T_K, p_Pa,
			p_sat_Pa, dw_dp_kgkgPa_surPsat_direct);
		printf("\nFor T = %f K, p = %f Pa, and p_sat = %f Pa, derivative of "
			"loading wrt. temperature results in dw_dT = %f kg/kg/K.", T_K,
			p_Pa, p_sat_Pa, dw_dT_kgkgK_surPsat_direct);
		printf("\nFor T = %f K, w = %f kg/kg, and p_sat = %f Pa, derivative of "
			"p with respect to w results in dp_dw = %f Pakg/kg.", T_K,
			w_kgkg_surPsat_direct, p_sat_Pa, dp_dw_Pakgkg_surPsat_direct);
		printf("\nFor T = %f K, w = %f kg/kg, and p_sat = %f Pa, derivative of "
			"p with respect to T results in dp_dT = %f Pa/K.", T_K,
			w_kgkg_surPsat_direct, p_sat_Pa, dp_dT_PaK_surPsat_direct);
		printf("\n\nFor T = %f K, p = %f Pa, and p_sat = %f Pa, reduced "
			"spreading pressure results in piStar = %f mol/kg.", T_K, p_Pa,
			p_sat_Pa, piStar_molkg_surPsat_direct);

		printf("\n\n\nResults of isotherm functions that are only defined for "
			"volumetric approach -> direct approach:");
		printf("\n-------------------------------------------------------------"
			"----------------------------------");
		printf("\nFor A = %f J/mol, volumetric loading results in W = %f "
			"m3/kg.", A_Jmol, W_m3kg_direct);
		printf("\nFor W = %f m3/kg, adsorption potential results in A = %f "
			"J/mol.", W_m3kg, A_Jmol_inv_direct);
		printf("\n\nFor A = %f J/mol, derivative of volumetric loading wrt. of "
			"adsorption potential results in dW_dA = %f m3mol/kg/J.", A_Jmol,
			dW_dA_m3molkgJ_direct);
		printf("\nFor W = %f m3/kg, derivative of adsorption potential wrt. "
			"volumetric loading results in dA_dW = %f Jkg/mol/kg.", W_m3kg,
			dA_dW_Jkgkgmol_direct);

		printf("\n\nFor p = %f Pa, T = %f K, p_sat = %f Pa and rho_l = %f "
			"kg/m3, loading results in w = %f kg/kg.", p_Pa, T_K, p_sat_Pa,
			rho_kgm3, w_kgkg_vol_direct);
		printf("\nFor w = %f kg/kg, T = %f K, p_sat = %f Pa and rho_l = %f "
			"kg/m3, pressure results in p = %f Pa.", w_kgkg_vol, T_K, p_sat_Pa,
			rho_kgm3, p_Pa_vol_inv_direct);
		printf("\nFor p = %f Pa, w = %f kg/kg, p_sat = %f Pa and rho_l = %f "
			"kg/m3, temperature results in T = %f K.", p_Pa, w_kgkg_vol,
			p_sat_Pa, rho_kgm3, T_K_vol_inv_direct);

		printf("\n\nFor p = %f Pa, T = %f K, p_sat = %f Pa and rho_l = %f "
			"kg/m3, derivative of loading wrt. pressure results in dw_dp = %f "
			"kg/kg/Pa.", p_Pa, T_K, p_sat_Pa, rho_kgm3,
			dw_dp_kgkgPa_vol_direct);
		printf("\nFor p = %f Pa, T = %f K, p_sat = %f Pa and rho_l = %f kg/m3, "
			"derivative of loading wrt. temperature results in dw_dT = %f "
			"kg/kg/K.", p_Pa, T_K, p_sat_Pa, rho_kgm3, dw_dT_kgkgK_vol_direct);
		printf("\nFor w = %f kg/kg, T = %f K, p_sat = %f Pa and rho_l = %f "
			"kg/m3, derivative of loading wrt. pressure results in dp_dw = %f "
			"Pakg/kg.", w_kgkg_vol, T_K, p_sat_Pa, rho_kgm3,
			dp_dw_Pakgkg_vol_direct);
		printf("\nFor w = %f kg/kg, T = %f K, p_sat = %f Pa and rho_l = %f "
			"kg/m3, derivative of loading wrt. temperature results in dp_dT = "
			"%f Pa/K.", w_kgkg_vol, T_K, p_sat_Pa, rho_kgm3,
			dp_dT_PaK_vol_direct);

		printf("\n\nFor p = %f Pa, T = %f K, p_sat = %f Pa and rho_l = %f "
			"kg/m3, reduced spreading pressure results in piStart = %f mol/kg.",
			p_Pa, T_K, p_sat_Pa, rho_kgm3, piStar_molkg_vol_direct);

		// Free allocated memory
		//
		delWorkingPair(workingPair);

	}
}


void testWorkingPair_abs_con(const char *path_db, const char *wp_as,
	const char *wp_st, const char *wp_rf, const char *wp_iso, int no_iso,
	const char *rf_psat, int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Initiate working pair
	//
	WorkingPair *workingPair = newWorkingPair(path_db, wp_as, wp_st, wp_rf,
		wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol);

	if (workingPair != NULL) {
		// Define some parameters to calculate equilibrium properties
		//
		double p_Pa = 724.659957;
		double T_K = 323.15;

		// Calculate equilibrium properties
		//
		double X_kgkg = abs_con_X_pT(p_Pa, T_K, workingPair);
		double dX_dp_kgkgPa = abs_con_dX_dp_pT(p_Pa, T_K, workingPair);
		double dX_dT_kgkgK = abs_con_dX_dT_pT(p_Pa, T_K, workingPair);
		double p_Pa_inv = abs_con_p_XT(X_kgkg, T_K, workingPair);
		double dp_dX_Pakgkg = abs_con_dp_dX_XT(X_kgkg, T_K, workingPair);
		double dp_dT_Pakgkg = abs_con_dp_dT_XT(X_kgkg, T_K, workingPair);
		double T_K_inv = abs_con_T_pX(p_Pa, X_kgkg, workingPair);

		double X_kgkg_direct = direct_abs_con_X_pT_workingPair(
			p_Pa, T_K, path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat,
			no_p_sat, rf_rhol, no_rhol);
		double dX_dp_kgkgPa_direct = direct_abs_con_dX_dp_pT_workingPair(
			p_Pa, T_K, path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat,
			no_p_sat, rf_rhol, no_rhol);
		double dX_dT_kgkgK_direct = direct_abs_con_dX_dT_pT_workingPair(
			p_Pa, T_K, path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat,
			no_p_sat, rf_rhol, no_rhol);
		double p_Pa_inv_direct = direct_abs_con_p_XT_workingPair(
			X_kgkg, T_K, path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat,
			no_p_sat, rf_rhol, no_rhol);
		double dp_dX_Pakgkg_direct = direct_abs_con_dp_dX_XT_workingPair(
			X_kgkg, T_K, path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat,
			no_p_sat, rf_rhol, no_rhol);
		double dp_dT_Pakgkg_direct = direct_abs_con_dp_dT_XT_workingPair(
			X_kgkg, T_K, path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat,
			no_p_sat, rf_rhol, no_rhol);
		double T_K_inv_direct = direct_abs_con_T_pX_workingPair(
			p_Pa, X_kgkg, path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat,
			no_p_sat, rf_rhol, no_rhol);

		// Print general information of selected working pair
		//
		printf("\n\n#############################");
		printf("\n#############################");
		printf("\n## Test WorkingPair-struct ##");
		printf("\n#############################");
		printf("\n#############################");

		printf("\n\n\nGeneral information of working pair:");
		printf("\n------------------------------------");
		printf("\nSelected sorbent is: %s.", workingPair->wp_as);
		printf("\nSelected sub-type of sorbent is: %s.", workingPair->wp_st);
		printf("\nSelected refrigerant is: %s.", workingPair->wp_rf);
		printf("\nSelected isotherm is: %s / ID %i.", workingPair->wp_iso,
			workingPair->no_iso);
		printf("\nSelected calculation approach for vapor pressure is: %s / "
			" ID %i.", workingPair->rf_psat, workingPair->no_p_sat);
		printf("\nSelected calculation approach for saturated liquid density "
			"is: %s / ID %i.", workingPair->rf_rhol, workingPair->no_rhol);

		// Print calculated values
		//
		printf("\n\n\nResults of conventional absorption functions:");
		printf("\n---------------------------------------------");
		printf("\nFor T = %f K and p = %f Pa, equilibrium concentration "
			"results in X = %f kg/kg.", T_K, p_Pa, X_kgkg);
		printf("\nFor T = %f K and X = %f kg/kg, equilibrium pressure results "
			"in p = %f Pa.", T_K, X_kgkg, p_Pa_inv);
		printf("\nFor p = %f Pa and X = %f kg/kg equilibrium temperature "
			"results in T = %f K.", p_Pa, X_kgkg, T_K_inv);

		printf("\n\nFor T = %f K and p = %f Pa, analytical derivative of X "
			"with respect to p results in dX_dp = %f kg/kg/Pa.", T_K, p_Pa,
			dX_dp_kgkgPa);
		printf("\nFor T = %f K and p = %f Pa, analytical derivative of X "
			"with respect to T results in dX_dT = %f kg/kg/K.", T_K, p_Pa,
			dX_dT_kgkgK);
		printf("\n\nFor T = %f K and X = %f kg/kg, analytical derivative of p "
			"with respect to X results in dp_dX = %f Pakg/kg.", T_K, X_kgkg,
			dp_dX_Pakgkg);
		printf("\nFor T = %f K and X = %f kg/kg, analytical derivative of p "
			"with respect to T results in dp_dT = %f Pa/K.", T_K, X_kgkg,
			dp_dT_Pakgkg);

		printf("\n\n\nResults of conventional absorption functions using -> "
			"direct approach:");
		printf("\n-------------------------------------------------------------"
			"---------");
		printf("\nFor T = %f K and p = %f Pa, equilibrium concentration "
			"results in X = %f kg/kg.", T_K, p_Pa, X_kgkg_direct);
		printf("\nFor T = %f K and X = %f kg/kg, equilibrium pressure results "
			"in p = %f Pa.", T_K, X_kgkg, p_Pa_inv_direct);
		printf("\nFor p = %f Pa and X = %f kg/kg equilibrium temperature "
			"results in T = %f K.", p_Pa, X_kgkg, T_K_inv_direct);

		printf("\n\nFor T = %f K and p = %f Pa, analytical derivative of X "
			"with respect to p results in dX_dp = %f kg/kg/Pa.", T_K, p_Pa,
			dX_dp_kgkgPa_direct);
		printf("\nFor T = %f K and p = %f Pa, analytical derivative of X with "
			"respect to T results in dX_dT = %f kg/kg/K.", T_K, p_Pa,
			dX_dT_kgkgK_direct);
		printf("\n\nFor T = %f K and X = %f kg/kg, analytical derivative of p "
			"with respect to X results in dp_dX = %f Pakg/kg.", T_K, X_kgkg,
			dp_dX_Pakgkg_direct);
		printf("\nFor T = %f K and X = %f kg/kg, analytical derivative of p "
			"with respect to T results in dp_dT = %f Pa/K.", T_K, X_kgkg,
			dp_dT_Pakgkg_direct);

		// Free allocated memory
		//
		delWorkingPair(workingPair);

	}
}


void testWorkingPair_abs_act(double T_K, double x_molmol,
	const char *path_db, const char *wp_as,
	const char *wp_st, const char *wp_rf, const char *wp_iso, int no_iso,
	const char *rf_psat, int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Initiate working pair
	//
	WorkingPair *workingPair = newWorkingPair(path_db, wp_as, wp_st, wp_rf,
		wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol);

	if (workingPair != NULL) {
		// Define some parameters to calculate equilibrium properties
		//
		double psat_Pa = ref_p_sat_T(T_K, workingPair);

		// Calculate equilibrium properties
		//
		double gamma_1 = abs_act_g_Txv1v2(
			T_K , x_molmol, -1, -1, workingPair);
		double p_Pa = abs_act_p_Txv1v2psat(
			T_K , x_molmol, -1, -1, psat_Pa, workingPair);
		double x_molmol_inv = abs_act_x_pTv1v2psat(
			p_Pa, T_K, -1, -1, psat_Pa, workingPair);

		double gamma_1_direct = direct_abs_act_g_Txv1v2(
			T_K , x_molmol, -1, -1, path_db, wp_as, wp_st, wp_rf, wp_iso,
			no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol);
		double p_Pa_direct = direct_abs_act_p_Txv1v2psat(
			T_K , x_molmol, -1, -1, psat_Pa, path_db, wp_as, wp_st, wp_rf,
			wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol);
		double x_molmol_inv_direct = direct_abs_act_x_pTv1v2psat(
			p_Pa_direct, T_K, -1, -1, psat_Pa, path_db, wp_as, wp_st, wp_rf,
			wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol);

		// Print general information of selected working pair
		//
		printf("\n\n#############################");
		printf("\n#############################");
		printf("\n## Test WorkingPair-struct ##");
		printf("\n#############################");
		printf("\n#############################");

		printf("\n\n\nGeneral information of working pair:");
		printf("\n------------------------------------");
		printf("\nSelected sorbent is: %s.", workingPair->wp_as);
		printf("\nSelected sub-type of sorbent is: %s.", workingPair->wp_st);
		printf("\nSelected refrigerant is: %s.", workingPair->wp_rf);
		printf("\nSelected isotherm is: %s / ID %i.", workingPair->wp_iso,
			workingPair->no_iso);
		printf("\nSelected calculation approach for vapor pressure is: %s / "
			" ID %i.", workingPair->rf_psat, workingPair->no_p_sat);
		printf("\nSelected calculation approach for saturated liquid density "
			"is: %s / ID %i.", workingPair->rf_rhol, workingPair->no_rhol);

		// Print calculated values
		//
		printf("\n\n\nResults of activity-based absorption functions that are "
			"always defined:");
		printf("\n-------------------------------------------------------------"
			"----------");
		printf("\nFor T = %f K and x = %f mol/mol, activity coefficient of "
			"first component results in gamma_1 = %f.", T_K, x_molmol, gamma_1);
		printf("\n\nFor T = %f K and x = %f mol/mol, equilibrium pressure of "
			"first component results in p = %f Pa.", T_K, x_molmol, p_Pa);
		printf("\nFor p = %f Pa and T = %f K, equilibrium molar fraction of "
			"first component results in x = %f mol/mol.", p_Pa, T_K,
			x_molmol_inv);

		printf("\n\n\nResults of activity-based absorption functions using "
			"that are always defined -> direct approach:");
		printf("\n-------------------------------------------------------------"
			"-----------------------------------");
		printf("\nFor T = %f K and x = %f mol/mol, activity coefficient of "
			"first component results in gamma_1 = %f.", T_K, x_molmol,
			gamma_1_direct);
		printf("\n\nFor T = %f K and x = %f mol/mol, equilibrium pressure of "
			"first component results in p = %f Pa.", T_K, x_molmol,
			p_Pa_direct);
		printf("\nFor p = %f Pa and T = %f K, equilibrium molar fraction of "
			"first component results in x = %f mol/mol.", p_Pa_direct, T_K,
			x_molmol_inv_direct);

		// Free allocated memory
		//
		delWorkingPair(workingPair);

	}
}


void testWorkingPair_abs_mix(double T_K, double x_molmol,
	const char *path_db, const char *wp_as,
	const char *wp_st, const char *wp_rf, const char *wp_iso, int no_iso,
	const char *rf_psat, int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Initiate working pair
	//
	WorkingPair *workingPair = newWorkingPair(path_db, wp_as, wp_st, wp_rf,
		wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol);

	if (workingPair != NULL) {
		// Calculate equilibrium properties
		//
		double y_1_molmol, y_1_molmol_1, y_1_molmol_2,
			y_2_molmol, y_2_molmol_1, y_2_molmol_2,
			y_1_molmol_direct, y_1_molmol_1_direct, y_1_molmol_2_direct,
			y_2_molmol_direct, y_2_molmol_1_direct, y_2_molmol_2_direct;

		double p_Pa = abs_mix_p_Tx(&y_1_molmol, &y_2_molmol, T_K, x_molmol,
			workingPair);
		double x_inv_molmol = abs_mix_x_pT(&y_1_molmol_1, &y_2_molmol_1,
			p_Pa, T_K, workingPair);
		double T_K_inv = abs_mix_T_px(&y_1_molmol_2, &y_2_molmol_2, p_Pa,
			x_molmol, workingPair);

		double dp_dx_Pa = abs_mix_dp_dx_Tx(T_K, x_molmol,
			workingPair);
		double dp_dT_PaK = abs_mix_dp_dT_Tx(T_K, x_molmol,
			workingPair);

		double p_Pa_direct = direct_abs_mix_p_Tx(&y_1_molmol_direct,
			&y_2_molmol_direct, T_K, x_molmol, path_db, wp_as, wp_st, wp_rf,
			wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol);
		double x_inv_molmol_direct = direct_abs_mix_x_pT(&y_1_molmol_1_direct,
			&y_2_molmol_1_direct, p_Pa, T_K, path_db, wp_as, wp_st, wp_rf,
			wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol);
		double T_K_inv_direct = direct_abs_mix_T_px(&y_1_molmol_2_direct,
			&y_2_molmol_2_direct, p_Pa, x_molmol, path_db, wp_as, wp_st, wp_rf,
			wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol);

		double dp_dx_Pa_direct = direct_abs_mix_dp_dx_Tx(T_K, x_molmol,
			path_db, wp_as, wp_st, wp_rf, wp_iso,
			no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol);
		double dp_dT_PaK_direct = direct_abs_mix_dp_dT_Tx(T_K, x_molmol,
			path_db, wp_as, wp_st, wp_rf, wp_iso,
			no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol);

		// Print general information of selected working pair
		//
		printf("\n\n#############################");
		printf("\n#############################");
		printf("\n## Test WorkingPair-struct ##");
		printf("\n#############################");
		printf("\n#############################");

		printf("\n\n\nGeneral information of working pair:");
		printf("\n------------------------------------");
		printf("\nSelected sorbent is: %s.", workingPair->wp_as);
		printf("\nSelected sub-type of sorbent is: %s.", workingPair->wp_st);
		printf("\nSelected refrigerant is: %s.", workingPair->wp_rf);
		printf("\nSelected isotherm is: %s / ID %i.", workingPair->wp_iso,
			workingPair->no_iso);
		printf("\nSelected calculation approach for vapor pressure is: %s / "
			" ID %i.", workingPair->rf_psat, workingPair->no_p_sat);
		printf("\nSelected calculation approach for saturated liquid density "
			"is: %s / ID %i.", workingPair->rf_rhol, workingPair->no_rhol);

		// Print calculated values
		//
		printf("\n\n\nResults of mixing-based absorption functions that are "
			"always defined:");
		printf("\n-------------------------------------------------------------"
			"--------");
		printf("\nFor p = %f Pa and T = %f K, equilibrium liquid mole fraction "
			"results in x = %f mol/mol and vapor mole fraction results in y = "
			"%f mol/mol.", p_Pa, T_K, x_inv_molmol, y_1_molmol_1);
		printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure "
			"results in p = %f Pa and vapor mole fraction results in y = %f "
			"mol/mol.", T_K, x_molmol, p_Pa, y_1_molmol);
		printf("\nFor p = %f Pa and x = %f mol/mol, equilibrium temperature "
			"results in T = %f K and vapor mole fraction results in y = %f "
			"mol/mol.", p_Pa, x_molmol, T_K_inv, y_1_molmol_2);

		printf("\n\nFor T = %f K and x = %f mol/mol, derivative of equilibrium "
			"p with respect to molar fraction results in dp_dx = %f Pa.",
			T_K, x_molmol, dp_dx_Pa);
		printf("\nFor T = %f K and x = %f mol/mol, derivative of equilibrium "
			"p with respect to temperature results in dp_dT = %f Pa/K.",
			T_K, x_molmol, dp_dT_PaK);

		printf("\n\n\nResults of mixing-based absorption functions using "
			"that are always defined -> direct approach:");
		printf("\n-------------------------------------------------------------"
			"---------------------------------");
		printf("\nFor p = %f Pa and T = %f K, equilibrium liquid mole fraction "
			"results in x = %f mol/mol and vapor mole fraction results in y = "
			"%f mol/mol.", p_Pa_direct, T_K, x_inv_molmol_direct,
			y_1_molmol_1_direct);
		printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure "
			"results in p = %f Pa and vapor mole fraction results in y = %f "
			"mol/mol.", T_K, x_molmol, p_Pa_direct, y_1_molmol_direct);
		printf("\nFor p = %f Pa and x = %f mol/mol, equilibrium temperature "
			"results in T = %f K and vapor mole fraction results in y = %f "
			"mol/mol.", p_Pa_direct, x_molmol, T_K_inv_direct,
			y_1_molmol_2_direct);

		printf("\n\nFor T = %f K and x = %f mol/mol, derivative of equilibrium "
			"p with respect to molar fraction results in dp_dx = %f Pa.",
			T_K, x_molmol, dp_dx_Pa_direct);
		printf("\nFor T = %f K and x = %f mol/mol, derivative of equilibrium "
			"p with respect to temperature results in dp_dT = %f Pa/K.",
			T_K, x_molmol, dp_dT_PaK_direct);

		// Free allocated memory
		//
		delWorkingPair(workingPair);

	}
}


/////////////////////////////////
// Definition of main function //
/////////////////////////////////
int main() {
	// Test working pair: Carbon Maxsorb-III / R-134a
	//
	testWorkingPair_ads(
		PATH,
		"carbon",
		"maxsorb-iii",
		"r-134a",
		"dubinin-astakhov",
		1,
		"EoS_vaporPressure",
		1,
		"EoS_saturatedLiquidDensity",
		1);

	// Test working pair: Carbon Norit-rb1 / CO2
	//
	testWorkingPair_ads(
		PATH,
		"carbon",
		"norit-rb1",
		"co2",
		"langmuir",
		1,
		"EoS_vaporPressure",
		1,
		"EoS_saturatedLiquidDensity",
		1);

	// Test working pair: Zeolite 5A / Water
	//
	testWorkingPair_ads(
		PATH,
		"zeolite",
		"5a",
		"water",
		"toth",
		1,
		"EoS_vaporPressure",
		1,
		"EoS_saturatedLiquidDensity",
		1);

	// Test working pair: MOF CuBtC / Propane
	//
	testWorkingPair_ads(
		PATH,
		"mof",
		"cubtc",
		"propane",
		"dss",
		1,
		"EoS_vaporPressure",
		1,
		"EoS_saturatedLiquidDensity",
		1);

	// Test working pair: NaOH-KOH-CsOH / H2O
	//
	testWorkingPair_abs_con(
		PATH,
		"naoh-koh-csoh",
		"",
		"water",
		"duehring",
		1,
		"EoS_vaporPressure",
		1,
		"EoS_saturatedLiquidDensity",
		1);

	// Test working pair: [BMIM]+[(CF3SO2)2N]-(2)  / H2O
	//
	testWorkingPair_abs_act(
		353.15,
		0.9386,
		PATH,
		"il",
		"[bmim][(cf3so2)2n]",
		"water",
		"wilson-fixeddl",
		1,
		"EoS_vaporPressure",
		1,
		"EoS_saturatedLiquidDensity",
		1);

	// Test working pair: [BMIM]+[(CF3SO2)2N]-(2)  / H2O
	//
	testWorkingPair_abs_act(
		353.15,
		0.9386,
		PATH,
		"il",
		"[bmim][(cf3so2)2n]",
		"water",
		"nrtl-fixeddg",
		1,
		"EoS_vaporPressure",
		1,
		"EoS_saturatedLiquidDensity",
		1);

	// Test working pair: [BMIM]+[(CF3SO2)2N]-(2)  / Benzene
	//
	testWorkingPair_abs_act(
		353.15,
		0.289300,
		PATH,
		"il",
		"[bmim][(cf3so2)2n]",
		"benzene",
		"nrtl-dg-t",
		1,
		"Antoine",
		1,
		"EoS_saturatedLiquidDensity",
		1);

	// Test working pair: [BMIM]+[(CF3SO2)2N]-(2)  / H2O
	//
	testWorkingPair_abs_act(
		353.15,
		0.9386,
		PATH,
		"il",
		"[bmim][(cf3so2)2n]",
		"water",
		"uniquac-fixeddu",
		1,
		"EoS_vaporPressure",
		1,
		"EoS_saturatedLiquidDensity",
		1);

	// Test working pair: [BMIM]+[(CF3SO2)2N]-(2)  / Benzene
	//
	testWorkingPair_abs_act(
		353.15,
		0.3475,
		PATH,
		"il",
		"[bmim][(cf3so2)2n]",
		"benzene",
		"uniquac-du-t",
		1,
		"Antoine",
		1,
		"EoS_saturatedLiquidDensity",
		1);

	// Test working pair: Lubricant PEB9 / HFC134a
	//
	testWorkingPair_abs_act(
		323.07,
		0.5864,
		PATH,
		"lubricant",
		"pec-9",
		"r-134a",
		"flory-huggins",
		1,
		"EoS_vaporPressure",
		1,
		"EoS_saturatedLiquidDensity",
		1);

	// Test working pair: PAG / R-134a
	//
	testWorkingPair_abs_act(
		323.07,
		0.5864,
		PATH,
		"lubricant",
		"pag",
		"r-134a",
		"heil",
		1,
		"EoS_vaporPressure",
		1,
		"EoS_saturatedLiquidDensity",
		1);

	// Test working pair: "R-600a / Oil
	//
	double w_1_kgkg = 0.2;
	double x_molmol =  (w_1_kgkg / 0.0581222) / (w_1_kgkg / 0.0581222 +
		(1- w_1_kgkg) / 0.350);

	testWorkingPair_abs_mix(
		313.15,
		x_molmol,
		PATH,
		"oil",
		"",
		"r-600a",
		"mixingrule",
		1,
		"EoS_noVaporPressure",
		1,
		"EoS_voSaturatedLiquidDensity",
		1);

	return EXIT_SUCCESS;
}
