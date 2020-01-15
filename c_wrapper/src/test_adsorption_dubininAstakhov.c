///////////////////////////////////////
// test_adsorption_dubininAstakhov.c //
///////////////////////////////////////
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "adsorption_dubininAstakhov.h"
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
		
		
	// Define parameter record for executing Dubinin-Astakhov equation. Data for
	// working pair "Maxsorb III / R-134a" is taken from:
	//
	// 	Loh W.S. et al. Adsorption isotherms and isosteric enthalpy of 
	//	adsorption for assorted refrigerants on activated carbons. Journal of 
	//	Chemical & Engineering Data 2012. 57(10): p. 2766-2773.
	//
	double isotherm_par[] = {7332.69, 1.29, 2.22, -1};


	// Define some input values for calculating equilibrium data
	//
	Refrigerant *refrigerant_r134a = newRefrigerant(
		"EoS_vapourPressure",
		"EoS_saturatedLiquidDensity");
		
	double p_Pa = 0.1e6;
	double T_K = 258.15;	
	
	double p_sat_Pa = refrigerant_r134a->psat_T(T_K, vapPre_par);
	double dp_sat_dT_PaK = refrigerant_r134a->dpsat_dT(T_K, vapPre_par);	
	double rho_kgm3 = refrigerant_r134a->rho_l_T(T_K, satLiqDen_par);
	double drho_dT_kgm3K = refrigerant_r134a->drho_l_dT(T_K, satLiqDen_par);
	
	double A_Jmol = 8.314462618f * T_K * log(p_sat_Pa/p_Pa);
	
	
	// Calculate equilibrium data
	//
	double W = adsorption_dubininAstakhov_W_ARho(A_Jmol, rho_kgm3, isotherm_par);
	double A_Jmol_inv = adsorption_dubininAstakhov_A_WRho(W, rho_kgm3,
		isotherm_par);
	
	double w_kgkg = adsorption_dubininAstakhov_w_pTpsatRho(p_Pa, T_K, p_sat_Pa, 
		rho_kgm3, isotherm_par);
	double p_Pa_inv = adsorption_dubininAstakhov_p_wTpsatRho(w_kgkg, T_K,
		p_sat_Pa, rho_kgm3, isotherm_par);
	double T_K_inv = adsorption_dubininAstakhov_T_pwpsatRho(p_Pa, w_kgkg,
		refrigerant_r134a->psat_T,
		refrigerant_r134a->rho_l_T, 
		refrigerant_r134a->dpsat_dT,
		refrigerant_r134a->drho_l_dT, 
		isotherm_par, vapPre_par, satLiqDen_par);

	// Calculate derivatives
	//
	double dW_dA_func = adsorption_dubininAstakhov_dW_dA_ARho(A_Jmol, 
		rho_kgm3, isotherm_par);
	double dW_dA_plus = adsorption_dubininAstakhov_W_ARho(A_Jmol+0.1, 
		rho_kgm3, isotherm_par);
	double dW_dA_minus = adsorption_dubininAstakhov_W_ARho(A_Jmol-0.1, 
		rho_kgm3, isotherm_par);
	double dW_dA_num = (dW_dA_plus - dW_dA_minus) / 0.2;
	
	double dA_dW_func = adsorption_dubininAstakhov_dA_dW_WRho(W, rho_kgm3, 
		isotherm_par);
	double dA_dW_plus = adsorption_dubininAstakhov_A_WRho(W+1e-8, rho_kgm3, 
		isotherm_par);
	double dA_dW_minus = adsorption_dubininAstakhov_A_WRho(W-1e-8, rho_kgm3, 
		isotherm_par);
	double dA_dW_num = (dA_dW_plus - dA_dW_minus)/(2e-8);
	
	double dw_dp_func_kgkgPa = adsorption_dubininAstakhov_dw_dp_pTpsatRho(p_Pa,
		T_K, p_sat_Pa, rho_kgm3, isotherm_par);
	double dw_dp_plus_kgkgPa = adsorption_dubininAstakhov_w_pTpsatRho(p_Pa+0.001,
		T_K, p_sat_Pa, rho_kgm3, isotherm_par);
	double dw_dp_minus_kgkgPa = adsorption_dubininAstakhov_w_pTpsatRho(p_Pa-0.001,
		T_K, p_sat_Pa, rho_kgm3, isotherm_par);
	double dw_dp_num_kgkgPa = (dw_dp_plus_kgkgPa - dw_dp_minus_kgkgPa) / 0.002;
	
	double dw_dT_func_kgkgK = adsorption_dubininAstakhov_dw_dT_pTpsatRho(p_Pa,
		T_K, p_sat_Pa, rho_kgm3, dp_sat_dT_PaK, drho_dT_kgm3K, isotherm_par);
	double dw_dT_plus_kgkgK = adsorption_dubininAstakhov_w_pTpsatRho(p_Pa,
		T_K+1e-6, p_sat_Pa+1e-6*dp_sat_dT_PaK, rho_kgm3+1e-6*drho_dT_kgm3K,
		isotherm_par);
	double dw_dT_minus_kgkgK = adsorption_dubininAstakhov_w_pTpsatRho(p_Pa,
		T_K-1e-6, p_sat_Pa-1e-6*dp_sat_dT_PaK, rho_kgm3-1e-6*drho_dT_kgm3K,
		isotherm_par);
	double dw_dT_num_kgkgK = (dw_dT_plus_kgkgK - dw_dT_minus_kgkgK)/(2e-6);
	
	double dp_dw_func_Pakgkg= adsorption_dubininAstakhov_dp_dw_wTpsatRho(w_kgkg,
		T_K, p_sat_Pa, rho_kgm3, isotherm_par);
	double dp_dw_plus_Pakgkg = adsorption_dubininAstakhov_p_wTpsatRho(
		w_kgkg+0.00001, T_K, p_sat_Pa, rho_kgm3, isotherm_par);
	double dp_dw_minus_Pakgkg = adsorption_dubininAstakhov_p_wTpsatRho(
		w_kgkg-0.00001, T_K, p_sat_Pa, rho_kgm3, isotherm_par);
	double dp_dw_num_Pakgkg = (dp_dw_plus_Pakgkg - dp_dw_minus_Pakgkg)/0.00002;
	
	double dp_dT_func_Pakgkg= adsorption_dubininAstakhov_dp_dT_wTpsatRho(w_kgkg,
		T_K, p_sat_Pa, rho_kgm3, dp_sat_dT_PaK, drho_dT_kgm3K, isotherm_par);
	double dp_dT_plus_Pakgkg = adsorption_dubininAstakhov_p_wTpsatRho(w_kgkg,
		T_K+1e-6, p_sat_Pa+1e-6*dp_sat_dT_PaK, rho_kgm3+1e-6*drho_dT_kgm3K,
		isotherm_par);
	double dp_dT_minus_Pakgkg = adsorption_dubininAstakhov_p_wTpsatRho(w_kgkg,
		T_K-1e-6, p_sat_Pa-1e-6*dp_sat_dT_PaK, rho_kgm3-1e-6*drho_dT_kgm3K,
		isotherm_par);
	double dp_dT_num_Pakgkg = (dp_dT_plus_Pakgkg - dp_dT_minus_Pakgkg)/(2e-6);
	
	// Calculate reduced spreading pressure
	//
	double piStar_molkg = adsorption_dubininAstakhov_piStar_pyxgTpsatRhoM(p_Pa, 1, 
		1, 1, T_K, p_sat_Pa, rho_kgm3, 0.0441, isotherm_par);
	
	int n_counter;
	int n_steps = 10000000;
	double h = (p_Pa - 0) / n_steps;
	double p_tmp;
	double piStar_molkg_num = 0;
	
	for(n_counter=1; n_counter<n_steps; n_counter++) {
		// Calculate reduced spreading pressure by numerical integration
		// using the trapezoidal rule
		//
		p_tmp = 0 - h/2 + n_counter * h;
		piStar_molkg_num += (h / 0.0441) *
			(adsorption_dubininAstakhov_w_pTpsatRho(p_tmp, T_K, p_sat_Pa, 
			 rho_kgm3, isotherm_par) / p_tmp);
	}

	// Print calculated values
	//
	printf("\n\n##\n##\nSelected working pair is \"Maxsorb III / R-134a\".");	
	printf("\nSelected isotherm is \"Dubinin-Astakhov\".");
	
	printf("\n\nFor A = %f J/mol, equilibrium volumetric loading results in W = %f m3/kg.", 
		A_Jmol, W);
	printf("\nFor W = %f m3/kg , equilibrium adsorption potential results in A = %f J/mol.", 
		W, A_Jmol_inv);
	printf("\n\nFor T = %f K and p = %f Pa, equilibrium loading results in x = %f kg/kg.", 
		T_K, p_Pa, w_kgkg);
	printf("\nFor T = %f K and w = %f kg/kg , equilibrium pressure results in p = %f Pa.", 
		T_K, w_kgkg, p_Pa_inv);
	printf("\nFor p = %f Pa and w = %f kg/kg , equilibrium temperature results in T = %f K.", 
		p_Pa, w_kgkg, T_K_inv);
	
	printf("\n\nFor A = %f J/mol, analytical derivative of W with respect to A results in dW_dA = %f m3mol/J/kg.",
		A_Jmol, dW_dA_func*10000);
	printf("\nFor A = %f J/mol, numerical derivative of W with respect to A results in dW_dA = %f m3mol/J/kg.",
		A_Jmol, dW_dA_num*10000);	
	printf("\nFor W = %f m3/kg, analytical derivative of A with respect to W results in dA_dW = %.3f Jkg/mol/m3.",
		W, dA_dW_func);
	printf("\nFor W = %f m3/kg, numerical derivative of A with respect to W results in dA_dW = %.3f Jkg/mol/m3.",
		W, dA_dW_num);		
	printf("\n\nFor T = %f K and p = %f Pa, analytical derivative of w with respect to p results in dw_dp = %f kg/kg/Pa.",
		T_K, p_Pa, dw_dp_func_kgkgPa);
	printf("\nFor T = %f K and p = %f Pa, numerical derivative of w with respect to p results in dw_dp = %f kg/kg/Pa.",
		T_K, p_Pa, dw_dp_num_kgkgPa);
	printf("\nFor T = %f K and p = %f Pa, analytical derivative of w with respect to T results in dw_dT = %.4f kg/kg/K.",
		T_K, p_Pa, dw_dT_func_kgkgK);
	printf("\nFor T = %f K and p = %f Pa, numerical derivative of w with respect to T results in dw_dT = %.4f kg/kg/K.",
		T_K, p_Pa, dw_dT_num_kgkgK);	
		
	printf("\n\nFor T = %f K and w = %f kg/kg, analytical derivative of p with respect to w results in dp_dw = %f Pakg/kg.", 
		T_K, w_kgkg, dp_dw_func_Pakgkg);
	printf("\nFor T = %f K and w = %f kg/kg, numerical derivative of p with respect to w results in dp_dw = %f Pakg/kg.", 
		T_K, w_kgkg, dp_dw_num_Pakgkg);
	printf("\nFor T = %f K and w = %f kg/kg, analytical derivative of p with respect to T results in dp_dT = %f Pa/K.", 
		T_K, w_kgkg, dp_dT_func_Pakgkg);
	printf("\nFor T = %f K and w = %f kg/kg, numerical derivative of p with respect to T results in dp_dT = %f Pa/K.", 
		T_K, w_kgkg, dp_dT_num_Pakgkg);

	printf("\n\nFor T = %f K and p = %f Pa, analytical reduced spreading pressure results in piStar = %f mol/kg.",
		T_K, p_Pa, piStar_molkg);
	printf("\nFor T = %f K and p = %f Pa, numerical reduced spreading pressure results in piStar = %f mol/kg.",
		T_K, p_Pa, piStar_molkg_num);

	// Free memory that is allocate for structures
	//
	delRefrigerant(refrigerant_r134a);
	
    return EXIT_SUCCESS;	
}