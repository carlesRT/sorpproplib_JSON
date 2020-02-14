////////////////////////
// test_workingPair.c //
////////////////////////
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "workingPair.h"

#include "structDefinitions.c"


/////////////////////////////
// Definition of functions //
/////////////////////////////
void testWorkingPair_ads(const char *path_db, const char *wp_as, 
	const char *wp_st, const char *wp_rf, const char *wp_iso, int no_iso,
	const char *rf_psat, int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Initiate working pair
	//
	WorkingPair *workingPair = newWorkingPair(
		path_db,
		wp_as,
		wp_st,
		wp_rf,
		wp_iso,
		no_iso,
		rf_psat,
		no_p_sat,
		rf_rhol,
		no_rhol);

	if (workingPair != NULL) {
		// Define some parameters to calculate equilibrium properties
		//		
		double p_Pa = 0.1e6;
		double T_K = 303.15;	


		// Check if refrigerant functions exist to avoid errors
		//
		double p_sat_Pa = ref_p_sat_T(T_K, workingPair);
		double rho_kgm3 = ref_rho_l_T(T_K, workingPair);

		// Calculate equilibrium properties with functions that are always defined
		//
		double w_kgkg_sur = ads_w_pT(p_Pa, T_K, workingPair);
		double w_kgkg_sur_direct = direct_ads_w_pT_workingPair(p_Pa, T_K, 
			path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
			rf_rhol, no_rhol);
		double p_Pa_sur_inv = ads_p_wT(w_kgkg_sur, T_K, workingPair);
		double p_Pa_sur_inv_direct = direct_ads_p_wT_workingPair(w_kgkg_sur, 
			T_K, path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, 
			no_p_sat, rf_rhol, no_rhol);
		double T_K_sur_inv = ads_T_pw(p_Pa, w_kgkg_sur, workingPair);	
		double T_K_sur_inv_direct = direct_ads_T_pw_workingPair(p_Pa, 
			w_kgkg_sur, path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, 
			no_p_sat, rf_rhol, no_rhol);
		double dw_dp_kgkgPa_sur = ads_dw_dp_pT(p_Pa, T_K, workingPair);
		double dw_dT_kgkgK_sur = ads_dw_dT_pT(p_Pa, T_K, workingPair);
		double dp_dw_Pakgkg_sur = ads_dp_dw_wT(w_kgkg_sur, T_K, workingPair);
		double dp_dT_PaK_sur = ads_dp_dT_wT(w_kgkg_sur, T_K, workingPair);
		double piStar_molkg_sur = ads_piStar_pyxgTM(p_Pa, 1, 1, 1, T_K, 
			0.04401, workingPair);


		// Calculate equilibrium properties with functions that are only defined for
		// isotherms based on the volumetric appoach
		//
		double A_Jmol = 8.314462618 * T_K * log(p_sat_Pa/p_Pa);
		
		double W_m3kg = ads_W_ARho(A_Jmol, rho_kgm3, workingPair);
		double A_Jmol_inv = ads_A_WRho(W_m3kg, rho_kgm3, workingPair);
		double dW_dA_m3molkgJ = ads_dW_dA_ARho(A_Jmol, rho_kgm3, workingPair);
		double dA_dW_Jkgkgmol = ads_dA_dW_WRho(W_m3kg, rho_kgm3, workingPair);
		
		double w_kgkg_vol = ads_w_pTpsatRho(p_Pa, T_K, p_sat_Pa, rho_kgm3, 
			workingPair);
		double p_Pa_vol_inv = ads_p_wTpsatRho(w_kgkg_vol, T_K, p_sat_Pa, rho_kgm3, 
			workingPair);
		double T_K_vol_inv = ads_T_pwpsatRho(p_Pa, w_kgkg_vol, p_sat_Pa, rho_kgm3, 
			workingPair);
		double dw_dp_kgkgPa_vol = ads_dw_dp_pTpsatRho(p_Pa, T_K, p_sat_Pa, rho_kgm3, 
			workingPair);			
		double dw_dT_kgkgK_vol = ads_dw_dT_pTpsatRho(p_Pa, T_K, p_sat_Pa, 
			rho_kgm3, ref_dp_sat_dT_T(T_K, workingPair), 
			ref_drho_l_dT_T(T_K, workingPair), workingPair);			
		double dp_dw_Pakgkg_vol = ads_dp_dw_wTpsatRho(w_kgkg_vol, T_K, p_sat_Pa,
			rho_kgm3, workingPair);			
		double dp_dT_PaK_vol = ads_dp_dT_wTpsatRho(w_kgkg_vol, T_K, p_sat_Pa, 
			rho_kgm3, ref_dp_sat_dT_T(T_K, workingPair), 
			ref_drho_l_dT_T(T_K, workingPair), workingPair);
		double piStar_molkg_vol = ads_piStar_pyxgTpsatRhoM(p_Pa, 1, 1, 1, T_K, 
			p_sat_Pa, rho_kgm3, 0.04401, workingPair);
		
		// Print general information of selected working pair
		//
		printf("\n\n#############################");
		printf("\n#############################");
		printf("\n## Test WorkingPair-struct ##");
		printf("\n#############################");
		printf("\n#############################");

		printf("\n\nGeneral information of working pair:");
		printf("\n------------------------------------");
		printf("\nSelected sorbent is: %s.", workingPair->wp_as);
		printf("\nSelected sub-type of sorbent is: %s.", workingPair->wp_st);
		printf("\nSelected refrigerant is: %s.", workingPair->wp_rf);
		printf("\nSelected isotherm is: %s - %i.", workingPair->wp_iso,
			workingPair->no_iso);
		printf("\nSelected calculation approach for vapour pressure is: %s - %i.",
			workingPair->rf_psat, workingPair->no_p_sat);
		printf("\nSelected calculation approach for vsaturated liquid density is: %s - %i.",
			workingPair->rf_rhol, workingPair->no_rhol);
			
		// Print calculated values
		//
		printf("\n\nResults of isotherm functions that are always defined:");
		printf("\n------------------------------------------------------");
		printf("\nFor T = %f K and p = %f Pa, loading results in w = %f kg/kg.",
			T_K, p_Pa, w_kgkg_sur);
		printf("\nFor T = %f K and w = %f kg/kg, pressure results in p = %f Pa.",
			T_K, w_kgkg_sur, p_Pa_sur_inv);
		printf("\nFor p = %f Pa and w = %f kg/kg, temperature results in T = %f K.",
			p_Pa, w_kgkg_sur, T_K_sur_inv);
		
		printf("\n\nFor T = %f K and p = %f Pa, derivative of loading wrt. pressure results in dwdp = %f kg/kg/Pa.",
			T_K, p_Pa, dw_dp_kgkgPa_sur);
		printf("\nFor T = %f K and p = %f Pa, derivative of loading wrt. temperature results in dwdT = %f kg/kg/K.",
			T_K, p_Pa, dw_dT_kgkgK_sur);	
		printf("\nFor T = %f K and w = %f kg/kg, derivative of p with respect to w results in dp_dw = %f Pakg/kg.", 
			T_K, w_kgkg_sur, dp_dw_Pakgkg_sur);
		printf("\nFor T = %f K and w = %f kg/kg, derivative of p with respect to T results in dp_dT = %f Pa/K.", 
			T_K, w_kgkg_sur, dp_dT_PaK_sur);
			
		printf("\n\nFor T = %f K and p = %f Pa, reduced spreading pressure results in piStar = %f mol/kg.",
			T_K, p_Pa, piStar_molkg_sur);

		printf("\n\nResults of isotherm functions that are only defined for volumetric approach:");
		printf("\n----------------------------------------------------------------------------");
		printf("\nFor A = %f J/mol, volumetric loading results in W = %f m3/kg.",
			A_Jmol, W_m3kg);
		printf("\nFor W = %f m3/kg, adsorption potential results in A = %f J/mol.",
			W_m3kg, A_Jmol_inv);
		printf("\n\nFor A = %f J/mol, derivative of volumetric loading wrt. of adsorption potential results in dW_dA = %f m3mol/kg/J.",
			A_Jmol, dW_dA_m3molkgJ);
		printf("\nFor W = %f m3/kg, derivative of adsorption potential wrt. volumetric loading results in dA_dW = %f Jkg/mol/kg.",
			W_m3kg, dA_dW_Jkgkgmol);
			
		printf("\n\nFor p = %f Pa, T = %f K, p_sat = %f Pa and rho_l = %f kg/m3, loading results in w = %f kg/kg.",
			p_Pa, T_K, p_sat_Pa, rho_kgm3, w_kgkg_vol);
		printf("\nFor w = %f kg/kg, T = %f K, p_sat = %f Pa and rho_l = %f kg/m3, pressure results in p = %f Pa.",
			w_kgkg_vol, T_K, p_sat_Pa, rho_kgm3, p_Pa_vol_inv);
		printf("\nFor p = %f Pa, w = %f kg/kg, p_sat = %f Pa and rho_l = %f kg/m3, temperature results in T = %f K.",
			p_Pa, w_kgkg_vol, p_sat_Pa, rho_kgm3, T_K_vol_inv);
		
		printf("\n\nFor p = %f Pa, T = %f K, p_sat = %f Pa and rho_l = %f kg/m3, derivative of loading wrt. pressure results in dw_dp = %f kg/kg/Pa.",
			p_Pa, T_K, p_sat_Pa, rho_kgm3, dw_dp_kgkgPa_vol);
		printf("\nFor p = %f Pa, T = %f K, p_sat = %f Pa and rho_l = %f kg/m3, derivative of loading wrt. temperature results in dw_dT = %f kg/kg/K.",
			p_Pa, T_K, p_sat_Pa, rho_kgm3, dw_dT_kgkgK_vol);			
		printf("\nFor w = %f kg/kg, T = %f K, p_sat = %f Pa and rho_l = %f kg/m3, derivative of loading wrt. pressure results in dp_dw = %f Pakg/kg.",
			w_kgkg_vol, T_K, p_sat_Pa, rho_kgm3, dp_dw_Pakgkg_vol);
		printf("\nFor w = %f kg/kg, T = %f K, p_sat = %f Pa and rho_l = %f kg/m3, derivative of loading wrt. temperature results in dp_dT = %f Pa/K.",
			w_kgkg_vol, T_K, p_sat_Pa, rho_kgm3, dp_dT_PaK_vol);
		
		printf("\n\nFor p = %f Pa, T = %f K, p_sat = %f Pa and rho_l = %f kg/m3, reduced spreading pressure results in piStart = %f mol/kg.",
			p_Pa, T_K, p_sat_Pa, rho_kgm3, piStar_molkg_vol);

		printf("\n\nResults of isotherm functions that are always defined using direct approach:");
		printf("\n----------------------------------------------------------------------------");
		printf("\nFor T = %f K and p = %f Pa, loading results in w = %f kg/kg.",
			T_K, p_Pa, w_kgkg_sur_direct);
		printf("\nFor T = %f K and w = %f kg/kg, pressure results in p = %f Pa.",
			T_K, w_kgkg_sur, p_Pa_sur_inv_direct);
		printf("\nFor p = %f Pa and w = %f kg/kg, temperature results in T = %f K.",
			p_Pa, w_kgkg_sur, T_K_sur_inv_direct);
			
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
	WorkingPair *workingPair = newWorkingPair(
		path_db,
		wp_as,
		wp_st,
		wp_rf,
		wp_iso,
		no_iso,
		rf_psat,
		no_p_sat,
		rf_rhol,
		no_rhol);

	if (workingPair != NULL) {
		// Define some parameters to calculate equilibrium properties
		//		
		double p_Pa = 724.659957;
		double T_K = 323.15;

		// Calculate equilibrium properties
		//
		double X_kgkg = abs_X_pT(p_Pa, T_K, workingPair);
		double p_Pa_inv = abs_p_XT(X_kgkg, T_K, workingPair);
		double T_K_inv = abs_T_pX(p_Pa, X_kgkg, workingPair);
		double dX_dp_kgkgPa = abs_dX_dp_pT(p_Pa, T_K, workingPair);	
		double dX_dT_kgkgK = abs_dX_dT_pT(p_Pa, T_K, workingPair);
		double dp_dX_Pakgkg = abs_dp_dX_XT(X_kgkg, T_K, workingPair);
		double dp_dT_Pakgkg = abs_dp_dT_XT(X_kgkg, T_K, workingPair);
			
		
		// Print general information of selected working pair
		//
		printf("\n\n#############################");
		printf("\n#############################");
		printf("\n## Test WorkingPair-struct ##");
		printf("\n#############################");
		printf("\n#############################");

		printf("\n\nGeneral information of working pair:");
		printf("\n------------------------------------");
		printf("\nSelected sorbent is: %s.", workingPair->wp_as);
		printf("\nSelected sub-type of sorbent is: %s.", workingPair->wp_st);
		printf("\nSelected refrigerant is: %s.", workingPair->wp_rf);
		printf("\nSelected isotherm is: %s - %i.", workingPair->wp_iso,
			workingPair->no_iso);
		printf("\nSelected calculation approach for vapour pressure is: %s - %i.",
			workingPair->rf_psat, workingPair->no_p_sat);
		printf("\nSelected calculation approach for saturated liquid density is: %s - %i.",
			workingPair->rf_rhol, workingPair->no_rhol);
			
		// Print calculated values
		//
		printf("\n\nResults of conventional absorption functions:");
		printf("\n---------------------------------------------");
		printf("\nFor T = %f K and p = %f Pa, equilibrium concentration results in X = %f kg/kg.", 
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
	WorkingPair *workingPair = newWorkingPair(
		path_db,
		wp_as,
		wp_st,
		wp_rf,
		wp_iso,
		no_iso,
		rf_psat,
		no_p_sat,
		rf_rhol,
		no_rhol);

	if (workingPair != NULL) {
		// Define some parameters to calculate equilibrium properties
		//		
		double psat_Pa = ref_p_sat_T(T_K, workingPair);

		// Calculate equilibrium properties
		//
		double gamma_1 = abs_g_Txv1v2(T_K , x_molmol, -1, -1, workingPair);
		double p_Pa = abs_p_Txv1v2psat(T_K , x_molmol, -1, -1, psat_Pa, 
			workingPair);
		double p_Pa_ref = abs_p_Txv1v2(T_K , x_molmol, -1, -1, workingPair);
			
		
		// Print general information of selected working pair
		//
		printf("\n\n#############################");
		printf("\n#############################");
		printf("\n## Test WorkingPair-struct ##");
		printf("\n#############################");
		printf("\n#############################");

		printf("\n\nGeneral information of working pair:");
		printf("\n------------------------------------");
		printf("\nSelected sorbent is: %s.", workingPair->wp_as);
		printf("\nSelected sub-type of sorbent is: %s.", workingPair->wp_st);
		printf("\nSelected refrigerant is: %s.", workingPair->wp_rf);
		printf("\nSelected isotherm is: %s - %i.", workingPair->wp_iso,
			workingPair->no_iso);
		printf("\nSelected calculation approach for vapour pressure is: %s - %i.",
			workingPair->rf_psat, workingPair->no_p_sat);
		printf("\nSelected calculation approach for saturated liquid density is: %s - %i.",
			workingPair->rf_rhol, workingPair->no_rhol);
			
		// Print calculated values
		//
		printf("\n\nResults of activity-based absorption functions:");
		printf("\n-----------------------------------------------");
		printf("\nFor T = %f K and x = %f mol/mol, activity coefficient results in gamma_1 = %f.", 
			T_K, x_molmol, gamma_1);
		printf("\nFor T = %f K, saturation pressure results in p = %f Pa.", 
			T_K, psat_Pa);
		printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure results in p = %f Pa.", 
			T_K, x_molmol, p_Pa);
		printf("\nFor T = %f K and x = %f mol/mol, equilibrium pressure results in p = %f Pa.", 
			T_K, x_molmol, p_Pa_ref);
			
			
		// Free allocated memory
		//	
		delWorkingPair(workingPair);
	
	}
}


void testWorkingPair_abs_mix(const char *path_db, const char *wp_as, 
	const char *wp_st, const char *wp_rf, const char *wp_iso, int no_iso,
	const char *rf_psat, int no_p_sat, const char *rf_rhol, int no_rhol) {
	// Initiate working pair
	//
	WorkingPair *workingPair = newWorkingPair(
		path_db,
		wp_as,
		wp_st,
		wp_rf,
		wp_iso,
		no_iso,
		rf_psat,
		no_p_sat,
		rf_rhol,
		no_rhol);

	if (workingPair != NULL) {
		// Define some parameters to calculate equilibrium properties
		//		
		double T_K = 313.2;
		double x_molmol = 0.769;	

		// Calculate equilibrium properties
		//
		double p_Pa = abs_p_Tvx(T_K, 0.5/1237.1, x_molmol, workingPair);
			
		
		// Print general information of selected working pair
		//
		printf("\n\n#############################");
		printf("\n#############################");
		printf("\n## Test WorkingPair-struct ##");
		printf("\n#############################");
		printf("\n#############################");

		printf("\n\nGeneral information of working pair:");
		printf("\n------------------------------------");
		printf("\nSelected sorbent is: %s.", workingPair->wp_as);
		printf("\nSelected sub-type of sorbent is: %s.", workingPair->wp_st);
		printf("\nSelected refrigerant is: %s.", workingPair->wp_rf);
		printf("\nSelected isotherm is: %s - %i.", workingPair->wp_iso,
			workingPair->no_iso);
		printf("\nSelected calculation approach for vapour pressure is: %s - %i.",
			workingPair->rf_psat, workingPair->no_p_sat);
		printf("\nSelected calculation approach for saturated liquid density is: %s - %i.",
			workingPair->rf_rhol, workingPair->no_rhol);
			
		// Print calculated values
		//
		printf("\n\nResults of mixing-based absorption functions:");
		printf("\n---------------------------------------------");
		printf("\nFor T = %f K,  x = %f mol/mol and PRE, equilibrium pressure results in p = %f Pa.", 
		T_K, x_molmol, p_Pa);
			
			
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
		".\\data\\sorpproplib_ValidationCInterface.json",
		"carbon",
		"maxsorb-iii",
		"r-134a",
		"dubinin-astakov",
		1,
		"EoS_vapourPressure",
		1,
		"EoS_saturatedLiquidDensity",
		1);
		
	// Test working pair: Carbon Acf-a-20 / R-134a
	//	
	testWorkingPair_ads(
		".\\data\\sorpproplib_ValidationCInterface.json",
		"carbon",
		"acf-a-20",
		"r-134a",
		"dubinin-astakov",
		1,
		"EoS_vapourPressure",
		1,
		"EoS_saturatedLiquidDensity",
		1);
		
	// Test working pair: Carbon Acf-a-20 / R507a
	//	
	testWorkingPair_ads(
		".\\data\\sorpproplib_ValidationCInterface.json",
		"carbon",
		"acf-a-20",
		"r507a",
		"dubinin-astakov",
		1,
		"EoS_vapourPressure",
		1,
		"EoS_saturatedLiquidDensity",
		1);
		
	// Test working pair: Carbon Maxsorb-III / R-410a
	//	
	testWorkingPair_ads(
		".\\data\\sorpproplib_ValidationCInterface.json",
		"carbon",
		"maxsorb-iii",
		"r-410a",
		"dubinin-astakov",
		1,
		"EoS_vapourPressure",
		1,
		"EoS_saturatedLiquidDensity",
		1);

	// Test working pair: Carbon Maxsorb-III / Propane
	//	
	testWorkingPair_ads(
		".\\data\\sorpproplib_ValidationCInterface.json",
		"carbon",
		"maxsorb-iii",
		"propane",
		"dubinin-astakov",
		1,
		"EoS_vapourPressure",
		1,
		"EoS_saturatedLiquidDensity",
		1);

	// Test working pair: Carbon Norit-rb1 / CO2
	//	
	testWorkingPair_ads(
		".\\data\\sorpproplib_ValidationCInterface.json",
		"carbon",
		"norit-rb1",
		"co2",
		"langmuir",
		1,
		"EoS_vapourPressure",
		1,
		"EoS_saturatedLiquidDensity",
		1);

	// Test working pair: Carbon Norit-rb1 / Methane
	//	
	testWorkingPair_ads(
		".\\data\\sorpproplib_ValidationCInterface.json",
		"carbon",
		"norit-rb1",
		"methane",
		"langmuir",
		1,
		"EoS_vapourPressure",
		1,
		"EoS_saturatedLiquidDensity",
		1);

	// Test working pair: Zeolite 5A / Water
	//	
	testWorkingPair_ads(
		".\\data\\sorpproplib_ValidationCInterface.json",
		"zeolite",
		"5a",
		"water",
		"toth",
		1,
		"EoS_vapourPressure",
		1,
		"EoS_saturatedLiquidDensity",
		1);

	// Test working pair: Zeolite 13X / Water
	//	
	testWorkingPair_ads(
		".\\data\\sorpproplib_ValidationCInterface.json",
		"zeolite",
		"13x",
		"water",
		"toth",
		1,
		"EoS_vapourPressure",
		1,
		"EoS_saturatedLiquidDensity",
		1);

	// Test working pair: Silicagel / Water
	//	
	testWorkingPair_ads(
		".\\data\\sorpproplib_ValidationCInterface.json",
		"silicagel",
		"",
		"water",
		"toth",
		1,
		"EoS_vapourPressure",
		1,
		"EoS_saturatedLiquidDensity",
		1);

	// Test working pair: Zeolite 5A / CO2
	//	
	testWorkingPair_ads(
		".\\data\\sorpproplib_ValidationCInterface.json",
		"zeolite",
		"5a",
		"co2",
		"toth",
		1,
		"EoS_vapourPressure",
		1,
		"EoS_saturatedLiquidDensity",
		1);

	// Test working pair: Zeolite 13X / CO2
	//	
	testWorkingPair_ads(
		".\\data\\sorpproplib_ValidationCInterface.json",
		"zeolite",
		"13x",
		"co2",
		"toth",
		1,
		"EoS_vapourPressure",
		1,
		"EoS_saturatedLiquidDensity",
		1);

	// Test working pair: Silicagel / CO2
	//	
	testWorkingPair_ads(
		".\\data\\sorpproplib_ValidationCInterface.json",
		"silicagel",
		"",
		"co2",
		"toth",
		1,
		"EoS_vapourPressure",
		1,
		"EoS_saturatedLiquidDensity",
		1);

	// Test working pair: MOF CuBtC / Propane
	//	
	testWorkingPair_ads(
		".\\data\\sorpproplib_ValidationCInterface.json",
		"mof",
		"cubtc",
		"propane",
		"dss",
		1,
		"EoS_vapourPressure",
		1,
		"EoS_saturatedLiquidDensity",
		1);

	// Test working pair: MOF CuBtC / Isobutane
	//	
	testWorkingPair_ads(
		".\\data\\sorpproplib_ValidationCInterface.json",
		"mof",
		"cubtc",
		"isobutane",
		"dss",
		1,
		"EoS_vapourPressure",
		1,
		"EoS_saturatedLiquidDensity",
		1);

	// Test working pair: MOF CuBtC / Propylene
	//	
	testWorkingPair_ads(
		".\\data\\sorpproplib_ValidationCInterface.json",
		"mof",
		"cubtc",
		"propylene",
		"dss",
		1,
		"Antoine",
		1,
		"EoS_saturatedLiquidDensity",
		1);
	
	// Test working pair: NaOH-KOH-CsOH / H2O
	//	
	testWorkingPair_abs_con(
		".\\data\\sorpproplib_ValidationCInterface.json",
		"naoh-koh-csoh",
		"",
		"water",
		"duhring",
		1,
		"EoS_vapourPressure",
		1,
		"EoS_saturatedLiquidDensity",
		1);
		
	// Test working pair: [BMIM]+[(CF3SO2)2N]-(2)  / H2O
	//	
	testWorkingPair_abs_act(
		353.15,
		0.9386,
		".\\data\\sorpproplib_ValidationCInterface.json",
		"il",
		"[bmim][(cf3so2)2n]",
		"water",
		"wilson",
		1,
		"EoS_vapourPressure",
		1,
		"EoS_saturatedLiquidDensity",
		1);
				
	// Test working pair: [BMIM]+[(CF3SO2)2N]-(2)  / H2O
	//	
	testWorkingPair_abs_act(
		353.15,
		0.9386,
		".\\data\\sorpproplib_ValidationCInterface.json",
		"il",
		"[bmim][(cf3so2)2n]",
		"water",
		"nrtl-fixeddg",
		1,
		"EoS_vapourPressure",
		1,
		"EoS_saturatedLiquidDensity",
		1);
		
	// Test working pair: [BMIM]+[(CF3SO2)2N]-(2)  / Benzene
	//	
	testWorkingPair_abs_act(
		353.15,
		0.289300,
		".\\data\\sorpproplib_ValidationCInterface.json",
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
		".\\data\\sorpproplib_ValidationCInterface.json",
		"il",
		"[bmim][(cf3so2)2n]",
		"water",
		"uniquac-fixeddu",
		1,
		"EoS_vapourPressure",
		1,
		"EoS_saturatedLiquidDensity",
		1);
		
	// Test working pair: [BMIM]+[(CF3SO2)2N]-(2)  / Benzene
	//	
	testWorkingPair_abs_act(
		353.15,
		0.3475,
		".\\data\\sorpproplib_ValidationCInterface.json",
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
		".\\data\\sorpproplib_ValidationCInterface.json",
		"lubricant",
		"pec-9",
		"r-134a",
		"flory-huggins",
		1,
		"EoS_vapourPressure",
		1,
		"EoS_saturatedLiquidDensity",
		1);
		
	// Test working pair: PAG / R-134a
	//	
	testWorkingPair_abs_act(
		323.07,
		0.5864,
		".\\data\\sorpproplib_ValidationCInterface.json",
		"lubricant",
		"pag",
		"r-134a",
		"heil",
		1,
		"EoS_vapourPressure",
		1,
		"EoS_saturatedLiquidDensity",
		1);
		
	// Test working pair: IL [c10mim][ntf2] / CO2
	//	
	testWorkingPair_abs_mix(
		".\\data\\sorpproplib_ValidationCInterface.json",
		"il",
		"[c10mim][ntf2]",
		"co2",
		"mixingrule-1pvdw",
		1,
		"EoS_vapourPressure",
		1,
		"EoS_saturatedLiquidDensity",
		1);
		
	return EXIT_SUCCESS;	
}