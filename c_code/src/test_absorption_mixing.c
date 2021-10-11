////////////////////////////////////
// test_absorption_mixing_1pvdw.c //
////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "absorption_mixing.h"
#include "refrigerant_cubicEoS.h"


int main() {
	// Define parameter record for executing Wilson equation. Data for working
	// pair "IL [c10mim][ntf2] / CO2" is taken from:
	//
	// 	Manic M.S. et al. High-pressure solubilities of carbon dioxide in ionic
	//	liquids based on bis (trifluoromethylsulfonyl) imide and chloride. The
	//	Journal of Supercritical Fluids 2012. 65: p. 1-10.
	//
	double isotherm_par[] =
		{10, -5, 3.6290e6, 407.81, 0.183531783208, 0, 0, 0, 0, 0,
		1.127e6, 743.05, 0.7915, 0, 0, 0, 0, 0,
		0.01749, 0, 0, 0, 0};

	// Define some inputs
	//
	double T_K = 313.15;
	double w_1_kgkg = 0.2;
	double x_1_molmol =  (w_1_kgkg / 0.0581222) / (w_1_kgkg / 0.0581222 +
		(1- w_1_kgkg) / 0.350);

	// Calculate values
	//
	double y_1_molmol, y_1_molmol_1, y_1_molmol_2,
		y_2_molmol, y_2_molmol_1, y_2_molmol_2;
	double p_Pa = absorption_mixing_p_Tx(&y_1_molmol, &y_2_molmol, T_K,
		x_1_molmol,
		refrigerant_cubic_PRE_parameters,
		refrigerant_cubic_1pvdw_parameters,
		refrigerant_cubic_PRE_gen_parameters,
		refrigerant_cubic_PRE_fugacity_coefficient_1pvdw,
		isotherm_par);
	double T_K_inv = absorption_mixing_T_px(&y_1_molmol_1, &y_2_molmol_1, p_Pa,
		x_1_molmol,
		refrigerant_cubic_PRE_parameters,
		refrigerant_cubic_1pvdw_parameters,
		refrigerant_cubic_PRE_gen_parameters,
		refrigerant_cubic_PRE_fugacity_coefficient_1pvdw,
		isotherm_par);
	double x_1_molmol_inv = absorption_mixing_x_pT(&y_1_molmol_2, &y_2_molmol_2,
		p_Pa, T_K,
		refrigerant_cubic_PRE_parameters,
		refrigerant_cubic_1pvdw_parameters,
		refrigerant_cubic_PRE_gen_parameters,
		refrigerant_cubic_PRE_fugacity_coefficient_1pvdw,
		isotherm_par);

	double dp_dT_Pa = absorption_mixing_dp_dT_Tx(T_K, x_1_molmol,
		refrigerant_cubic_PRE_parameters,
		refrigerant_cubic_1pvdw_parameters,
		refrigerant_cubic_PRE_gen_parameters,
		refrigerant_cubic_PRE_fugacity_coefficient_1pvdw,
		isotherm_par);
	double dp_dx_Pa = absorption_mixing_dp_dx_Tx(T_K, x_1_molmol,
		refrigerant_cubic_PRE_parameters,
		refrigerant_cubic_1pvdw_parameters,
		refrigerant_cubic_PRE_gen_parameters,
		refrigerant_cubic_PRE_fugacity_coefficient_1pvdw,
		isotherm_par);

	// Print calculated values
	//
	printf("\n\n##\n##\nSelected working pair is \"R-600a / Oil\".");
	printf("\nSelected absorption equation is \"PRE + Mixing-1PVDW\".");

	printf("\n\nFor T = %f K and x = %f mol/mol, equilibrium pressure results "
		"in p = %f Pa and vapor mole fraction results in y = %f mol/mol.",
		T_K, x_1_molmol, p_Pa, y_1_molmol);
	printf("\nFor p = %f Pa and x = %f mol/mol, equilibrium temperature "
		"results in T = %f K and vapor mole fraction results in y = %f "
		"mol/mol.", p_Pa, x_1_molmol, T_K_inv, y_1_molmol_1);
	printf("\nFor p = %f Pa and T = %f K, equilibrium liquid mole fraction "
		"results in x = %f mol/mol and vapor mole fraction results in y = %f "
		"mol/mol.", p_Pa, T_K, x_1_molmol_inv, y_1_molmol_2);

	printf("\n\nFor T = %f K and x = %f mol/mol, derivative of equilibrium "
		"p with respect to temperature results in dp_dT = %f Pa/K.",
		T_K, x_1_molmol, dp_dT_Pa);
	printf("\nFor T = %f K and x = %f mol/mol, derivative of equilibrium "
		"p with respect to molar fraction results in dp_dx = %f Pa.",
		T_K, x_1_molmol, dp_dx_Pa);

    return EXIT_SUCCESS;
}
