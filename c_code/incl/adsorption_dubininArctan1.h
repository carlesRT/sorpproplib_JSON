/////////////////////////////////
// adsorption_dubininArctan1.h //
/////////////////////////////////
#ifndef adsorption_dubininArctan1_h
#define adsorption_dubininArctan1_h


///////////////////////////////////////
// Definition of function prototypes //
///////////////////////////////////////
/*
 * General form of Dubinin-Astakhov equation:
 * ------------------------------------------
 * 	w = W * rho_l(T)
 *
 * Temperature-dependent coefficients:
 * -----------------------------------
 * 	W = a/PI * [arctan((A - b) / c) + PI/2] + d
 *	A = R * T * ln(p_sat(T)/p)
 *
 * Possible inputs required by user:
 * ---------------------------------
 *	p: Pressure in Pa
 *	T: Temperature in K
 *	w: Loading in kg/kg
 *
 *	p_sat(T): Saturation pressure in Pa
 *	rho_l(T): Density of adsorpt in kg/m³
 *	dp_sat_dT(T): Derivative of vapor pressure wrt. temperature in Pa/K
 *	drho_l_dT(T): Derivative of saturated liquid density of adsorpt wrt.
 *		temperature in kg/m³/K
 *
 * Order of coefficients in JSON-file:
 * -----------------------------------
 *	isotherm_par[0]	-> a	-> in m³/kg
 * 	isotherm_par[1] -> b	-> in J/mol
 * 	isotherm_par[2] -> c	-> in J/mol
 * 	isotherm_par[3] -> d	-> in m³/kg
 *
 */


/*
 * adsorption_dubininArctan1_W_ARho:
 * ---------------------------------
 *
 * Calculates equilibrium volumetric loading W in m³/kg depending on equilibrium
 * adsorption potential A in J/mol and saturated liquid density of adsorpt rho_l
 * in kg/m3.
 *
 * Parameters:
 * -----------
 * 	double A_Jmol:
 *		Equilibrium adsorption potential A in J/mol.
 * 	double rho_l_kgm3:
 *		Saturated liquid density of adsorpt rho_l in kg/m3.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Dubinin-Arctan1
 *		equation.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium volumetric loading W in m³/kg.
 *
 * History:
 * --------
 *	06/04/2021, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_dubininArctan1_W_ARho(double A_Jmol, double rho_l_kgm3,
	double isotherm_par[]);


/*
 * adsorption_dubininArctan1_A_WRho:
 * ---------------------------------
 *
 * Calculates equilibrium adsorption potential A in J/mol depending on
 * equilibrium volumetric loading W in m³/kg and saturated liquid density of
 * adsorpt rho_l in kg/m3.
 *
 * Parameters:
 * -----------
 * 	double W_m3kg:
 *		Equilibrium volumetric loading W in m³/kg.
 * 	double rho_l_kgm3:
 *		Saturated liquid density of adsorpt rho_l in kg/m3.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Dubinin-Arctan1
 *		equation.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium adsorption potential A in J/mol.
 *
 * History:
 * --------
 *	06/04/2021, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_dubininArctan1_A_WRho(double W_m3kg, double rho_l_kgm3,
	double isotherm_par[]);


/*
 * adsorption_dubininArctan1_w_pTpsatRho:
 * --------------------------------------
 *
 * Calculates equilibrium loading w in kg/kg depending on equilibrium pressure
 * p in Pa, equilibrium temperature T in K, saturation pressure p_Sat in Pa and
 * saturated liquid density of adsorpt rho_l in kg/m³.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_l_kgm3:
 *		Saturated liquid density of adsorpt in kg/m³.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Dubinin-Arctan1
 *		equation.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium loading in kg/kg.
 *
 * History:
 * --------
 *	06/04/2021, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_dubininArctan1_w_pTpsatRho(double p_Pa, double T_K,
	double p_sat_Pa, double rho_l_kgm3, double isotherm_par[]);


/*
 * adsorption_dubininArctan1_p_wTpsatRho:
 * --------------------------------------
 *
 * Calculates equilibrium pressure p in Pa depending on equilibrium loading w in
 * kg/kg, equilibrium temperature T in K, saturation pressure p_Sat in Pa and
 * saturated liquid density of adsorpt rho_l in kg/m³.
 *
 * Parameters:
 * -----------
 * 	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_l_kgm3:
 *		Saturated liquid density of adsorpt in kg/m³.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Dubinin-Arctan1
 *		equation.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure in Pa.
 *
 * History:
 * --------
 *	06/04/2021, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_dubininArctan1_p_wTpsatRho(double w_kgkg, double T_K,
	double p_sat_Pa, double rho_kgm3, double isotherm_par[]);


/*
 * adsorption_dubininArctan1_T_pwpsatRho:
 * --------------------------------------
 *
 * Calculates equilibrium temperature T in K depending on equilibrium pressure
 * in Pa, equilibrium loading w in kg/kg and function pointers for saturation
 * pressure p_Sat in Pa, saturated liquid density of adsorpt rho_l in kg/m³,
 * derivative of vapor pressure wrt. temperature and derivative of saturated
 * liquid density of adsorpt wrt. temperature.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure in Pa.
 * 	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double (*p_sat_T_PaK)(double T_K, double p_sat_par[], void *refrigerant):
 *		Function pointer for vapor pressure wrt. temperature.
 *	double (*rho_l_T_kgm3K)(double T_K, double rho_l_par[]):
 *		Function pointer for saturated liquid density of adsorpt.
 *	double (*dp_sat_dT_PaK)(double T_K, double p_sat_par[], void *refrigerant):
 *		Function pointer for derivative of vapor pressure wrt. temperature.
 *	double (*drho_l_dT_kgm3K)(double T_K, double rho_l_par[]):
 *		Function pointer for derivative of saturated liquid density of adsorpt
 *		wrt. temperature.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Dubinin-Arctan1
 *		equation.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients vapor pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients for saturated liquid
 *		density of adsorpt.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium temperature in K.
 *
 * Remarks:
 * --------
 *	No error handling because pointers are checked at highest level (i.e.,
 *	functions for workingPair-Struct).
 *
 * History:
 * --------
 *	06/04/2021, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_dubininArctan1_T_pwpsatRho(double p_Pa, double w_kgkg,
	double (*p_sat_T_Pa)(double T_K, double p_sat_par[], void *refrigerant),
	double (*rho_l_T_kgm3)(double T_K, double rho_l_par[]),
	double (*dp_sat_dT_PaK)(double T_K, double p_sat_par[], void *refrigerant),
	double (*drho_l_dT_kgm3K)(double T_K, double rho_l_par[]),
	double isotherm_par[], double p_sat_par[], double rho_l_par[],
	void *refrigerant);


/*
 * adsorption_dubininArctan1_dW_dA_ARho:
 * -------------------------------------
 *
 * Calculates derivative of equilibrium volumetric loading dW_dA in m³mol/kg/J
 * depending on equilibrium adsorption potential A in J/mol and saturated liquid
 * density of adsorpt in kg/m3.
 *
 * Parameters:
 * -----------
 * 	double A_Jmol:
 *		Equilibrium adsorption potential A in mol/J.
 * 	double rho_l_kgm3:
 *		Saturated liquid density of adsorpt rho_l in kg/m3.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Dubinin-Arctan1
 *		equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of volumetric loading wrt. adsorption potential in
 *		m³mol/kg/J.
 *
 * History:
 * --------
 *	06/04/2021, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_dubininArctan1_dW_dA_ARho(double A_Jmol, double rho_l_kgm3,
	double isotherm_par[]);


/*
 * adsorption_dubininArctan1_dA_dW_WRho:
 * -------------------------------------
 *
 * Calculates derivative of equilibrium adsorption potential dA_dW in kgJ/mol/m³
 * depending on equilibrium volumetric loading W in m³/kg and saturated liquid
 * density of adsorpt rho_l in kg/m3.
 *
 * Parameters:
 * -----------
 * 	double W_m3kg:
 *		Equilibrium volumetric loading W in m³/kg.
 * 	double rho_l_kgm3:
 *		Saturated liquid density of adsorpt rho_l in kg/m3.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Dubinin-Arctan1
 *		equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of adsorption potential wrt. volumetric loading in
 *		kgJ/mol/m³.
 *
 * History:
 * --------
 *	06/04/2021, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_dubininArctan1_dA_dW_WRho(double W_m3kg, double rho_l_kgm3,
	double isotherm_par[]);


/*
 * adsorption_dubininArctan1_dw_dp_pTpsatRho:
 * ------------------------------------------
 *
 * Calculates derivative of equilibrium loading dw_dp with respect to pressure
 * in kg/kg/Pa depending on equilibrium pressure p in Pa, equilibrium
 * temperature T in K, saturation pressure p_Sat in Pa and saturated liquid
 * density of adsorpt rho_l in kg/m³.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_l_kgm3:
 *		Saturated liquid density of adsorpt in kg/m³.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Dubinin-Arctan1
 *		equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. pressure in kg/kg/Pa.
 *
 * History:
 * --------
 *	06/04/2021, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_dubininArctan1_dw_dp_pTpsatRho(double p_Pa, double T_K,
	double p_sat_Pa, double rho_l_kgm3, double isotherm_par[]);


/*
 * adsorption_dubininArctan1_dw_dT_pTpsatRho:
 * ------------------------------------------
 *
 * Calculates derivative of equilibrium loading dw_dp with respect to
 * temperature in kg/kg/K depending on equilibrium pressure p in Pa, equilibrium
 * temperature T in K, saturation pressure p_Sat in Pa, saturated liquid density
 * of adsorpt rho_l in kg/m³, derivative of saturation pressure with respect to
 * temperature dp_sat_dT in Pa/K and derivative of saturated liquid density of
 * adsorpt with respect to temperature drho_l_dT in kg/m³/K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_l_kgm3:
 *		Saturated liquid density of adsorpt in kg/m³.
 * 	double dp_sat_dT_PaK:
 *		Derivative of saturation pressure wrt. temperature in Pa/K.
 *	double drho_l_dT_kgm3K:
 *		Derivative of saturated liquid density of adsorpt wrt. temperature in
 *		kg/m³/K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Dubinin-Arctan1
 *		equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. temperature in kg/kg/K.
 *
 * History:
 * --------
 *	06/04/2021, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_dubininArctan1_dw_dT_pTpsatRho(double p_Pa, double T_K,
	double p_sat_Pa, double rho_l_kgm3, double dp_sat_dT_PaK,
	double drho_l_dT_kgm3K, double isotherm_par[]);


/*
 * adsorption_dubininArctan1_dp_dw_wTpsatRho:
 * ------------------------------------------
 *
 * Calculates derivative of equilibrium pressure p with respect to loading
 * w in kgPa/kg depending on equilibrium loading w in kg/kg, equilibrium
 * temperature T in K, saturation pressure p_Sat in Pa and saturated liquid
 * density of adsorpt rho_l in kg/m³.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double rho_l_kgm3:
 *		Saturated liquid density of adsorpt in kg/m³.
 * 	double dp_sat_dT_PaK:
 *		Derivative of saturation pressure wrt. temperature in Pa/K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Dubinin-Arctan1
 *		equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. loading in kgPa/kg.
 *
 * History:
 * --------
 *	06/04/2021, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_dubininArctan1_dp_dw_wTpsatRho(double w_kgkg, double T_K,
	double p_sat_Pa, double rho_l_kgm3, double isotherm_par[]);


/*
 * adsorption_dubininArctan1_dp_dT_wTpsatRho:
 * ------------------------------------------
 *
 * Calculates derivative of equilibrium pressure p with respect to temperature
 * w in Pa/K depending on equilibrium loading w in kg/kg and equilibrium
 * temperature T in K, saturation pressure p_Sat in Pa, saturated liquid density
 * of adsorpt rho_l in kg/m³, derivative of saturation pressure with respect to
 * temperature dp_sat_dT in Pa/K and derivative of saturated liquid density of
 * adsorpt with respect to temperature drho_l_dT in kg/m³/K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_l_kgm3:
 *		Saturated liquid density of adsorpt in kg/m³.
 * 	double dp_sat_dT_PaK:
 *		Derivative of saturation pressure wrt. temperature in Pa/K.
 *	double drho_l_dT_kgm3K:
 *		Derivative of saturated liquid density of adsorpt wrt. temperature in
 *		kg/m³/K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Dubinin-Arctan1
 *		equation.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. temperature in Pa/K.
 *
 * History:
 * --------
 *	06/04/2021, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_dubininArctan1_dp_dT_wTpsatRho(double w_kgkg, double T_K,
	double p_sat_Pa, double rho_l_kgm3, double dp_sat_dT_PaK,
	double drho_l_dT_kgm3K, double isotherm_par[]);


/*
 * adsorption_dubininArctan1_piStar_pyxgTpsatRhoM:
 * -----------------------------------------------
 *
 * Calculates reduced spreading pressure in kg/mol depending on total pressure
 * of vapor phase p_total in Pa, molar fraction of refrigerant in vapor phase
 * y in mol/mol, molar fraction of refrigerant in adsorbed phase in mol/mol,
 * activity coefficient of refrigerant in adsorbed phase, equilibrium
 * temperature T in K, molar mass of refrigerant M in kg/mol, saturation
 * pressure p_Sat in  Pa, saturated liquid density of adsorpt rho_l in kg/m³.
 * The reduced spreading pressure is defined as follows:
 *
 * 	piStar = A * pi / (R * T * m_sorbent) = 1 / M *
 *		Integral_0^p0{w(p,T) / p * dp}
 *
 *	where p0 = p_total*y / (gamma*x)
 *
 * Parameters:
 * -----------
 *	double p_total_Pa:
 *		Total pressure of vapor phase in Pa.
 *	double y_molmol:
 *		Molar fraction of refrigerant in vapor phase in mol/mol.
 *	double x_molmol:
 *		Molar fraction of refrigerant in adsorbed phase in mol/mol.
 *	double gamma:
 *		Activity coefficient of refrigerant in adsorbed phase.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_l_kgm3:
 *		Saturated liquid density of adsorpt in kg/m³.
 *	double M_kgmol:
 *		Molar mass of refrigerant M in kg/mol.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Dubinin-Arctan1
 *		equation.
 *
 * Returns:
 * --------
 *	double:
 *		Reduced spreading pressure in kg/mol.
 *
 * Remarks:
 * --------
 *	Reduced spreading pressure is needed to solve, for example, the Ideal
 *	Adsorbed Solution Theory (IAST). Currently, integral is solved numerically;
 *	however, an analytical solution of the integral exists that is based on
 *	the upper incomplete gamma function.
 *
 * History:
 * --------
 *	06/04/2021, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double adsorption_dubininArctan1_piStar_pyxgTpsatRhoM(double p_total_Pa,
	double y_molmol, double x_molmol, double gamma, double T_K,
	double p_sat_Pa, double rho_l_kgm3, double M_kgmol, double isotherm_par[]);

#endif
