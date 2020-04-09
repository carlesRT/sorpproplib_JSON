///////////////////////
// test_adsorption.c //
///////////////////////
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "adsorption.h"
#include "refrigerant.h"
#include "structDefinitions.c"


int main() {
	// Define parameter record for executing Dual-Site-Sips equation. Data for
	// working pair "MOF CuBTC / Propane" is taken from:
	//
	// 	Lamia N. et al. Adsorption of propane propylene and isobutane on a
	//	metal–organic framework: Molecular simulation and experiment. Chemical
	//	Engineering Science 2009. 64(14): p. 3246-3259.
	//
	double par_dss[] =
		{0.00007,0.00016,28700,34100,0.82,0.32,0.27384237,0.05027058,323};

	// Define some input values for calculating equilibrium data
	//
	double p_Pa = 40000;
	double T_K = 373;

	// Calculate equilibrium data
	//
	Adsorption *isotherm_dss = newAdsorption("dss");

	double w_kgkg = isotherm_dss->sur_w_pT(p_Pa, T_K, par_dss);
	double p_Pa_inv = isotherm_dss->sur_p_wT(w_kgkg, T_K, par_dss);
	double T_K_inv = isotherm_dss->sur_T_pw(p_Pa, w_kgkg, par_dss);
	double dw_dp_kgkgPa = isotherm_dss->sur_dw_dp_pT(p_Pa, T_K, par_dss);
	double dw_dT_kgkgK = isotherm_dss->sur_dw_dT_pT(p_Pa, T_K, par_dss);
	double dp_dw_Pakgkg = isotherm_dss->sur_dp_dw_wT(w_kgkg, T_K, par_dss);
	double dp_dT_Pakgkg = isotherm_dss->sur_dp_dT_wT(w_kgkg, T_K, par_dss);
	double piStar_kgmol = isotherm_dss->sur_piStar_pyxgTM(p_Pa, 1, 1, 1,
		T_K, 0.0441, par_dss);

	// Print results of calculations
	//
	printf("\n\n##\n##\nCreated structure for Dual-Site-Sips isotherm.");
	printf("\nSelected working pair is \"MOF CuBTC / Propane\".");

	printf("\n\nFor T = %f K and p = %f Pa, equilibrium loading results in x = "
		"%f kg/kg.", T_K, p_Pa, w_kgkg);
	printf("\nFor T = %f K and w = %f kg/kg, equilibrium pressure results in p "
		"= %f Pa.", T_K, w_kgkg, p_Pa_inv);
	printf("\nFor p = %f Pa and w = %f kg/kg equilibrium temperature results "
		"in T = %f K.", p_Pa, w_kgkg, T_K_inv);

	printf("\n\nFor T = %f K and p = %f Pa, derivative of w with respect to p "
		"results in dw_dp = %f kg/kg/Pa.", T_K, p_Pa, dw_dp_kgkgPa);
	printf("\nFor T = %f K and p = %f Pa, derivative of w with respect to T "
		"results in dw_dT = %f kg/kg/K.", T_K, p_Pa, dw_dT_kgkgK);
	printf("\nFor T = %f K and w = %f kg/kg, derivative of p with respect to w "
		"results in dp_dw = %f Pakg/kg.", T_K, w_kgkg, dp_dw_Pakgkg);
	printf("\nFor T = %f K and w = %f kg/kg, derivative of p with respect to T "
		"results in dp_dT = %f Pa/K.", T_K, w_kgkg, dp_dT_Pakgkg);

	printf("\n\nFor T = %f K and p = %f Pa, reduced spreading pressure results "
		"in piStar = %f mol/kg.", T_K, p_Pa, piStar_kgmol);


	// Define parameter record for executing Langmuir equation. Data for working
	// pair "Carbon Norit-RB1 / CO2" is taken from:
	//
	// 	Rick van der Vaar et al. Single and Mixed Gas Adsorption Equilibria of
	//	Carbon Dioxide/Methane on Activated Carbon. Adsorption 2000. 6: p.
	//	311-323.
	//
	double par_langmuir[] = {23500, 0.000000000322, 0.34943146};

	// Define some input values for calculating equilibrium data
	//
	p_Pa = 3e5;
	T_K = 303.1;

	// Calculate equilibrium data
	//
	Adsorption *isotherm_langmuir = newAdsorption("langmuir");

	w_kgkg = isotherm_langmuir->sur_w_pT(p_Pa, T_K, par_langmuir);
	p_Pa_inv = isotherm_langmuir->sur_p_wT(w_kgkg, T_K, par_langmuir);
	T_K_inv = isotherm_langmuir->sur_T_pw(p_Pa, w_kgkg, par_langmuir);
	dw_dp_kgkgPa = isotherm_langmuir->sur_dw_dp_pT(p_Pa, T_K, par_langmuir);
	dw_dT_kgkgK = isotherm_langmuir->sur_dw_dT_pT(p_Pa, T_K, par_langmuir);
	dp_dw_Pakgkg = isotherm_langmuir->sur_dp_dw_wT(w_kgkg, T_K, par_langmuir);
	dp_dT_Pakgkg = isotherm_langmuir->sur_dp_dT_wT(w_kgkg, T_K, par_langmuir);
	piStar_kgmol = isotherm_langmuir->sur_piStar_pyxgTM(p_Pa, 1, 1, 1,
		T_K, 0.04401, par_langmuir);

	// Print results of calculations
	//
	printf("\n\n##\n##\nCreated structure for Langmuir isotherm.");
	printf("\nSelected working pair is \"Carbon Norit-RB1 / CO2\".");

	printf("\n\nFor T = %f K and p = %f Pa, equilibrium loading results in x = "
		"%f kg/kg.", T_K, p_Pa, w_kgkg);
	printf("\nFor T = %f K and w = %f kg/kg, equilibrium pressure results in p "
		"= %f Pa.", T_K, w_kgkg, p_Pa_inv);
	printf("\nFor p = %f Pa and w = %f kg/kg equilibrium temperature results "
		"in T = %f K.", p_Pa, w_kgkg, T_K_inv);

	printf("\n\nFor T = %f K and p = %f Pa, derivative of w with respect to p "
		"results in dw_dp = %f kg/kg/Pa.", T_K, p_Pa, dw_dp_kgkgPa);
	printf("\nFor T = %f K and p = %f Pa, derivative of w with respect to T "
		"results in dw_dT = %f kg/kg/K.", T_K, p_Pa, dw_dT_kgkgK);
	printf("\nFor T = %f K and w = %f kg/kg, derivative of p with respect to w "
		"results in dp_dw = %f Pakg/kg.", T_K, w_kgkg, dp_dw_Pakgkg);
	printf("\nFor T = %f K and w = %f kg/kg, derivative of p with respect to T "
		"results in dp_dT = %f Pa/K.", T_K, w_kgkg, dp_dT_Pakgkg);

	printf("\n\nFor T = %f K and p = %f Pa, reduced spreading pressure results "
		"in piStar = %f mol/kg.", T_K, p_Pa, piStar_kgmol);


	// Define parameter record for executing Toth equation. Data for working
	// pair "Zeolite 5A / H2O" is taken from:
	//
	// 	Wang Y. and M.D. LeVan Adsorption equilibrium of carbon dioxide and
	//	water vapor on zeolites 5A and 13X and silica gel: pure components.
	//	Journal of Chemical & Engineering Data 2009. 54(10): p. 2839-2844.
	//
	double par_toth[] =
		{4.714e-13, -51.114, 1, 0.3548, 9955, -1, 0.422675081243106};

	// Define some input values for calculating equilibrium data
	//
	p_Pa = 1270;
	T_K = 373.15;

	// Calculate equilibrium data
	//
	Adsorption *isotherm_toth = newAdsorption("toth");

	w_kgkg = isotherm_toth->sur_w_pT(p_Pa, T_K, par_toth);
	p_Pa_inv = isotherm_toth->sur_p_wT(w_kgkg, T_K, par_toth);
	T_K_inv = isotherm_toth->sur_T_pw(p_Pa, w_kgkg, par_toth);
	dw_dp_kgkgPa = isotherm_toth->sur_dw_dp_pT(p_Pa, T_K, par_toth);
	dw_dT_kgkgK = isotherm_toth->sur_dw_dT_pT(p_Pa, T_K, par_toth);
	dp_dw_Pakgkg = isotherm_toth->sur_dp_dw_wT(w_kgkg, T_K, par_toth);
	dp_dT_Pakgkg = isotherm_toth->sur_dp_dT_wT(w_kgkg, T_K, par_toth);
	piStar_kgmol = isotherm_toth->sur_piStar_pyxgTM(p_Pa, 1, 1, 1,
		T_K, 0.01801528, par_toth);

	// Print results of calculations
	//
	printf("\n\n##\n##\nCreated structure for Toth isotherm.");
	printf("\nSelected working pair is \"Zeolite 5A / H2O\".");

	printf("\n\nFor T = %f K and p = %f Pa, equilibrium loading results in x = "
		"%f kg/kg.", T_K, p_Pa, w_kgkg);
	printf("\nFor T = %f K and w = %f kg/kg, equilibrium pressure results in p "
		"= %f Pa.", T_K, w_kgkg, p_Pa_inv);
	printf("\nFor p = %f Pa and w = %f kg/kg equilibrium temperature results "
		"in T = %f K.", p_Pa, w_kgkg, T_K_inv);

	printf("\n\nFor T = %f K and p = %f Pa, derivative of w with respect to p "
		"results in dw_dp = %f kg/kg/Pa.", T_K, p_Pa, dw_dp_kgkgPa);
	printf("\nFor T = %f K and p = %f Pa, derivative of w with respect to T "
		"results in dw_dT = %f kg/kg/K.", T_K, p_Pa, dw_dT_kgkgK);
	printf("\nFor T = %f K and w = %f kg/kg, derivative of p with respect to w "
		"results in dp_dw = %f Pakg/kg.", T_K, w_kgkg, dp_dw_Pakgkg);
	printf("\nFor T = %f K and w = %f kg/kg, derivative of p with respect to T "
		"results in dp_dT = %f Pa/K.", T_K, w_kgkg, dp_dT_Pakgkg);

	printf("\n\nFor T = %f K and p = %f Pa, reduced spreading pressure results "
		"in piStar = %f mol/kg.", T_K, p_Pa, piStar_kgmol);


	// Define parameter record for executing Freundlich equation. Data for
	// working pair "Silica gel Fuji A Type / Water" is taken from:
	//
	// 	A. Sakoda, M. Suzuki. FUNDAMENTAL STUDY ON SOLAR POWERED ADSORPTION
	//	COOLING SYSTEM. Journal of Chemical Engineering of Japan 1984: p.
	//	17-52.
	//
	// Define parameter record for executing vapor pressure equation. Data for
	// refrigerant "H2O" is taken from:
	//
	// 	W. Wagner and P. Pruß. The IAPWS Formulation 1995 for the Thermodynamic
	//	Properties of Ordinary Water Substance for General and Scientific Use.
	//	Journal of Physical and Chemical Reference Data 2002. 31: p. 387-535.
	//
	double freundlich_par[] = {0.346, 0, 0, 0, 0.625, 0, 0, 0};
	double refrigerant_par_H2O[] = {647.096, 22064000, -7.85951783, 1,
		1.84408259, 1.5, -11.7866497, 3, 22.6807411, 3.5, -15.9618719, 4,
		1.80122502, 7.5};

	// Define some input values for calculating equilibrium data
	//
	Refrigerant *refrigerant_water = newRefrigerant("EoS_vaporPressure",
		"EoS_saturatedLiquidDensity", refrigerant_par_H2O);

	p_Pa = refrigerant_water->psat_T(283, refrigerant_par_H2O,
		refrigerant_water);
	T_K = 373;
	double p_sat_Pa = refrigerant_water->psat_T(T_K, refrigerant_par_H2O,
		refrigerant_water);
	double dp_sat_dT_PaK = refrigerant_water->dpsat_dT(T_K,
		refrigerant_par_H2O, refrigerant_water);

	// Calculate equilibrium data
	//
	Adsorption *isotherm_freundlich = newAdsorption("freundlich");

	w_kgkg = isotherm_freundlich->sur_w_pTpsat(p_Pa, T_K, p_sat_Pa,
		freundlich_par);
	p_Pa_inv = isotherm_freundlich->sur_p_wTpsat(w_kgkg, T_K, p_sat_Pa,
		freundlich_par);
	T_K_inv = isotherm_freundlich->sur_T_pwpsat(p_Pa, w_kgkg,
		refrigerant_water->psat_T, refrigerant_water->dpsat_dT,
		freundlich_par, refrigerant_par_H2O, refrigerant_water);
	dw_dp_kgkgPa = isotherm_freundlich->sur_dw_dp_pTpsat(p_Pa, T_K, p_sat_Pa,
		freundlich_par);
	dw_dT_kgkgK = isotherm_freundlich->sur_dw_dT_pTpsat(p_Pa, T_K, p_sat_Pa,
		dp_sat_dT_PaK, freundlich_par);
	dp_dw_Pakgkg = isotherm_freundlich->sur_dp_dw_wTpsat(w_kgkg, T_K, p_sat_Pa,
		freundlich_par);
	dp_dT_Pakgkg = isotherm_freundlich->sur_dp_dT_wTpsat(w_kgkg, T_K, p_sat_Pa,
		dp_sat_dT_PaK, freundlich_par);
	piStar_kgmol = isotherm_freundlich->sur_piStar_pyxgTpsatM(p_Pa, 1, 1, 1,
		T_K, p_sat_Pa, 0.01801528, freundlich_par);

	// Print results of calculations
	//
	printf("\n\n##\n##\nCreated structure for Freundlich isotherm.");
	printf("\nSelected working pair is \"Silica gel Fuji A Type / H2O\".");

	printf("\n\nFor T = %f K and p = %f Pa, equilibrium loading results in x = "
		"%f kg/kg.", T_K, p_Pa, w_kgkg);
	printf("\nFor T = %f K and w = %f kg/kg, equilibrium pressure results in p "
		"= %f Pa.", T_K, w_kgkg, p_Pa_inv);
	printf("\nFor p = %f Pa and w = %f kg/kg equilibrium temperature results "
		"in T = %f K.", p_Pa, w_kgkg, T_K_inv);

	printf("\n\nFor T = %f K and p = %f Pa, derivative of w with respect to p "
		"results in dw_dp = %f kg/kg/Pa.", T_K, p_Pa, dw_dp_kgkgPa);
	printf("\nFor T = %f K and p = %f Pa, derivative of w with respect to T "
		"results in dw_dT = %f kg/kg/K.", T_K, p_Pa, dw_dT_kgkgK);
	printf("\nFor T = %f K and w = %f kg/kg, derivative of p with respect to w "
		"results in dp_dw = %f Pakg/kg.", T_K, w_kgkg, dp_dw_Pakgkg);
	printf("\nFor T = %f K and w = %f kg/kg, derivative of p with respect to T "
		"results in dp_dT = %f Pa/K.", T_K, w_kgkg, dp_dT_Pakgkg);

	printf("\n\nFor T = %f K and p = %f Pa, reduced spreading pressure results "
		"in piStar = %f mol/kg.", T_K, p_Pa, piStar_kgmol);


	// Define parameter record for executing Dubinin-Astakhov equation. Data for
	// working pair "Maxsorb III / R-134a" is taken from:
	//
	// 	Loh W.S. et al. Adsorption isotherms and isosteric enthalpy of
	//	adsorption for assorted refrigerants on activated carbons. Journal of
	//	Chemical & Engineering Data 2012. 57(10): p. 2766-2773.
	//
	double par_dubininAstakhov[] = {7332.69, 1.29, 2.22, -1};
	double vapPre_par[] = {374.18, 4.05629e6, -7.686556, 1, 2.311791,
		1.5, -2.039554, 2, -3.583758, 4, 0, 0, 0, 0};
	double satLiqDen_par[] = {374.18, 1, 1, 518.20, 0, 884.13,
		0.333333333333333, 485.84, 0.666666666666666, 193.29,
		3.333333333333333, 0, 0, 0, 0, 0, 0};

	// Define some input values for calculating equilibrium data
	//
	p_Pa = 0.1e6;
	T_K = 258.15;

	// Calculate equilibrium data
	//
	Adsorption *isotherm_dubininAstakhov = newAdsorption("dubinin-astakhov");
	Refrigerant *refrigerant_r134a = newRefrigerant("EoS_vaporPressure",
		"EoS_saturatedLiquidDensity", vapPre_par);

	p_sat_Pa = refrigerant_r134a->psat_T(T_K, vapPre_par,
		refrigerant_r134a);
	double rho_kgm3 = refrigerant_r134a->rho_l_T(T_K, satLiqDen_par);
	double A_Jmol = 8.314462618f * T_K * log(p_sat_Pa/p_Pa);

	double W = isotherm_dubininAstakhov->vol_W_ARho(A_Jmol, rho_kgm3,
		par_dubininAstakhov);
	double A_Jmol_inv = isotherm_dubininAstakhov->vol_A_WRho(W, rho_kgm3,
		par_dubininAstakhov);

	w_kgkg = isotherm_dubininAstakhov->vol_w_pTpsatRho(p_Pa, T_K, p_sat_Pa,
		rho_kgm3, par_dubininAstakhov);
	p_Pa_inv = isotherm_dubininAstakhov->vol_p_wTpsatRho(w_kgkg, T_K,
		p_sat_Pa, rho_kgm3, par_dubininAstakhov);
	T_K_inv = isotherm_dubininAstakhov->vol_T_pwpsatRho(p_Pa, w_kgkg,
		refrigerant_r134a->psat_T,
		refrigerant_r134a->rho_l_T,
		refrigerant_r134a->dpsat_dT,
		refrigerant_r134a->drho_l_dT,
		par_dubininAstakhov, vapPre_par, satLiqDen_par, refrigerant_r134a);

	double dW_dA = isotherm_dubininAstakhov->vol_dW_dA_ARho(A_Jmol, rho_kgm3,
		par_dubininAstakhov);
	double dA_dW = isotherm_dubininAstakhov->vol_dA_dW_WRho(W, rho_kgm3,
		par_dubininAstakhov);
	dw_dp_kgkgPa = isotherm_dubininAstakhov->vol_dw_dp_pTpsatRho(p_Pa,
		T_K, p_sat_Pa, rho_kgm3, par_dubininAstakhov);
	dw_dT_kgkgK = isotherm_dubininAstakhov->vol_dw_dT_pTpsatRho(p_Pa,
		T_K, p_sat_Pa, rho_kgm3, refrigerant_r134a->dpsat_dT(T_K, vapPre_par,
		refrigerant_r134a), refrigerant_r134a->drho_l_dT(T_K, satLiqDen_par),
		par_dubininAstakhov);
	dp_dw_Pakgkg = isotherm_dubininAstakhov->vol_dp_dw_wTpsatRho(w_kgkg, T_K,
		p_sat_Pa, rho_kgm3, par_dubininAstakhov);
	dp_dT_Pakgkg = isotherm_dubininAstakhov->vol_dp_dT_wTpsatRho(w_kgkg, T_K,
		p_sat_Pa, rho_kgm3, refrigerant_r134a->dpsat_dT(T_K, vapPre_par,
		refrigerant_r134a), refrigerant_r134a->drho_l_dT(T_K, satLiqDen_par),
		par_dubininAstakhov);
	piStar_kgmol = isotherm_dubininAstakhov->vol_piStar_pyxgTpsatRhoM(p_Pa, 1,
		1, 1, T_K, p_sat_Pa, rho_kgm3, 0.0441, par_dubininAstakhov);

	// Print results of calculations
	//
	printf("\n\n##\n##\nCreated structure for Dubinin-Astakhov isotherm.");
	printf("\nSelected working pair is \"Maxsorb III / R-134a\".");

	printf("\n\nFor A = %f J/mol, equilibrium volumetric loading results in W "
		"= %f m3/kg.", A_Jmol, W);
	printf("\nFor W = %f m3/kg , equilibrium adsorption potential results in A "
		"= %f J/mol.", W, A_Jmol_inv);
	printf("\n\nFor T = %f K and p = %f Pa, equilibrium loading results in x = "
		"%f kg/kg.", T_K, p_Pa, w_kgkg);
	printf("\nFor T = %f K and w = %f kg/kg , equilibrium pressure results in "
		"p = %f Pa.", T_K, w_kgkg, p_Pa_inv);
	printf("\nFor p = %f Pa and w = %f kg/kg , equilibrium temperature results "
		"in T = %f K.", p_Pa, w_kgkg, T_K_inv);

	printf("\n\nFor A = %f J/mol, derivative of W with respect to A results in "
		"dW_dA = %f m3mol/J/kg.", A_Jmol, dW_dA);
	printf("\nFor W = %f m3/kg, derivative of A with respect to W results in "
		"dA_dW = %.3f Jkg/mol/m3.", W, dA_dW);

	printf("\n\nFor T = %f K and p = %f Pa, derivative of w with respect to p "
		"results in dw_dp = %f kg/kg/Pa.", T_K, p_Pa, dw_dp_kgkgPa);
	printf("\nFor T = %f K and p = %f Pa, derivative of w with respect to T "
		"results in dw_dT = %.4f kg/kg/K.", T_K, p_Pa, dw_dT_kgkgK);
	printf("\nFor T = %f K and w = %f kg/kg, derivative of p with respect to "
		"w results in dp_dw = %f Pakg/kg.", T_K, w_kgkg, dp_dw_Pakgkg);
	printf("\nFor T = %f K and w = %f kg/kg, derivative of p with respect to T "
		"results in dp_dT = %f Pa/K.", T_K, w_kgkg, dp_dT_Pakgkg);

	printf("\n\nFor T = %f K and p = %f Pa, reduced spreading pressure results "
		"in piStar = %f mol/kg.", T_K, p_Pa, piStar_kgmol);


	// Free memory that is allocated for structures
	//
	delRefrigerant(refrigerant_r134a);
	delAdsorption(isotherm_dubininAstakhov);
	delAdsorption(isotherm_freundlich);
	delRefrigerant(refrigerant_water);
	delAdsorption(isotherm_toth);
	delAdsorption(isotherm_langmuir);
	delAdsorption(isotherm_dss);

    return EXIT_SUCCESS;
}