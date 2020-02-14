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
void testWorkingPair(const char *path_db, const char *wp_as, 
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
		double p_sat_Pa = (workingPair->refrigerant->psat_T==NULL) ? -1 :
			workingPair->refrigerant->psat_T(T_K, workingPair->psat_par);
		double rho_kgm3 = (workingPair->refrigerant->rho_l_T==NULL) ? -1 :
			workingPair->refrigerant->rho_l_T(T_K, workingPair->rhol_par);

		// Calculate equilibrium properties with functions that are always defined
		//
		double w_kgkg_sur = iso_w_pT(p_Pa, T_K, workingPair);
		double w_kgkg_sur_direct = direct_iso_w_pT_workingPair(p_Pa, T_K, 
			path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat,
			rf_rhol, no_rhol);
		double p_Pa_sur_inv = iso_p_wT(w_kgkg_sur, T_K, workingPair);
		double p_Pa_sur_inv_direct = direct_iso_p_wT_workingPair(w_kgkg_sur, 
			T_K, path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, 
			no_p_sat, rf_rhol, no_rhol);
		double T_K_sur_inv = iso_T_pw(p_Pa, w_kgkg_sur, workingPair);	
		double T_K_sur_inv_direct = direct_iso_T_pw_workingPair(p_Pa, 
			w_kgkg_sur, path_db, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, 
			no_p_sat, rf_rhol, no_rhol);
		double dw_dp_kgkgPa_sur = iso_dw_dp_pT(p_Pa, T_K, workingPair);
		double dw_dT_kgkgK_sur = iso_dw_dT_pT(p_Pa, T_K, workingPair);
		double dp_dw_Pakgkg_sur = iso_dp_dw_wT(w_kgkg_sur, T_K, workingPair);
		double dp_dT_PaK_sur = iso_dp_dT_wT(w_kgkg_sur, T_K, workingPair);
		double piStar_molkg_sur = iso_piStar_pyxgTM(p_Pa, 1, 1, 1, T_K, 
			0.04401, workingPair);


		// Calculate equilibrium properties with functions that are only defined for
		// isotherms based on the volumetric appoach
		//
		double A_Jmol = 8.314462618 * T_K * log(p_sat_Pa/p_Pa);
		
		double W_m3kg = iso_W_ARho(A_Jmol, rho_kgm3, workingPair);
		double A_Jmol_inv = iso_A_WRho(W_m3kg, rho_kgm3, workingPair);
		double dW_dA_m3molkgJ = iso_dW_dA_ARho(A_Jmol, rho_kgm3, workingPair);
		double dA_dW_Jkgkgmol = iso_dA_dW_WRho(W_m3kg, rho_kgm3, workingPair);
		
		double w_kgkg_vol = iso_w_pTpsatRho(p_Pa, T_K, p_sat_Pa, rho_kgm3, 
			workingPair);
		double p_Pa_vol_inv = iso_p_wTpsatRho(w_kgkg_vol, T_K, p_sat_Pa, rho_kgm3, 
			workingPair);
		double T_K_vol_inv = iso_T_pwpsatRho(p_Pa, w_kgkg_vol, p_sat_Pa, rho_kgm3, 
			workingPair);
		double dw_dp_kgkgPa_vol = iso_dw_dp_pTpsatRho(p_Pa, T_K, p_sat_Pa, rho_kgm3, 
			workingPair);			
		double dw_dT_kgkgK_vol = (workingPair->refrigerant->dpsat_dT == NULL ||
			workingPair->refrigerant->drho_l_dT == NULL) ? -1 : 
			iso_dw_dT_pTpsatRho(p_Pa, T_K, p_sat_Pa, rho_kgm3, 
			workingPair->refrigerant->dpsat_dT(T_K, workingPair->psat_par), 
			workingPair->refrigerant->drho_l_dT(T_K, workingPair->rhol_par), 
			workingPair);			
		double dp_dw_Pakgkg_vol = iso_dp_dw_wTpsatRho(w_kgkg_vol, T_K, p_sat_Pa,
			rho_kgm3, workingPair);			
		double dp_dT_PaK_vol = (workingPair->refrigerant->dpsat_dT == NULL ||
			workingPair->refrigerant->drho_l_dT == NULL) ? -1 : 
			iso_dp_dT_wTpsatRho(w_kgkg_vol, T_K, p_sat_Pa, rho_kgm3, 
			workingPair->refrigerant->dpsat_dT(T_K, workingPair->psat_par), 
			workingPair->refrigerant->drho_l_dT(T_K, workingPair->rhol_par), 
			workingPair);
		double piStar_molkg_vol = iso_piStar_pyxgTpsatRhoM(p_Pa, 1, 1, 1, T_K, 
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


/////////////////////////////////
// Definition of main function //
/////////////////////////////////
int main() {
	// Test working pair: Carbon Maxsorb-III / R-134a
	//	
	testWorkingPair(
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
	
	return EXIT_SUCCESS;	
}