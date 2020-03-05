///////////////////
// workingPair.h //
///////////////////
#ifndef workingPair_h
#define workingPair_h


//////////////////////////
// Definition of macros //
//////////////////////////
#ifdef DLL_EXPORTS
#define DLL_API __declspec(dllexport) 
#else
#define DLL_API
#endif


////////////////////////////
// Definition of typedefs //
////////////////////////////
/*
 * WorkingPair:
 * ------------
 *
 * Contains strings defining working pair and calculations approaches,
 * coefficients for calculation approaches, and structures that contain function
 * pointers for the isotherm and the refrigerant functions of the working pair.
 *
 * Attributes:
 * -----------
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 * 	const char *rf_psat:
 *		Name of calculation approach for vapour pressure.
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 * 	const char *rf_ac:
 *		Name of calculation approach for activity coefficients.
 *
 * 	double *iso_par:
 *		Array containing coefficients of isotherm.
 * 	double *psat_par:
 *		Array containing coefficients of vapour pressure equation.
 * 	double *rhol_par:
 *		Array containing coefficients of saturated liquid density equation.
 * 	double *ac_par:
 *		Array containing coefficients of activity coefficent equation.
 *
 *	Adsorption *adsorption:
 *		Struct containing function pointers for isotherm functions.
 *	Refrigerant *refrigerant:
 *		Struct containing function pointers for refrigerant functions.
 *
 * History:
 * --------
 *	01/06/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API typedef struct WorkingPair WorkingPair;


///////////////////////////////////////
// Definition of function prototypes //
///////////////////////////////////////
/*
 * newWorkingPair:
 * ---------------
 *
 * Initialisation function to create a new WorkingPair-struct. Therefore,
 * function checks first if enough memory available to allocate WorkingPair-
 * struct and then reads coefficients of functions from database and sets
 * pointers within the struct depending on working pair given as input.
 * If memory allocation fails, functions returns NULL and prints a warning.
 *
 * Parameters:
 * -----------
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 * 	const char *rf_psat:
 *		Name of calculation approach for vapour pressure.
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 * 	const char *rf_ac:
 *		Name of calculation approach for activity coefficients.
 *
 * Returns:
 * --------
 *	struct *WorkingPair:
 *		Returns malloc-ed WorkingPair-struct containg all information requird
 *		to calculate the sorption equilibrium of the working working.
 *
 * History:
 * --------
 *	01/06/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API WorkingPair *newWorkingPair(const char *path_db, const char *wp_as, 
	const char *wp_st, const char *wp_rf, const char *wp_iso, 
	const char *rf_psat, const char *rf_rhol, const char *rf_ac);


/*
 * delWorkingPair:
 * ---------------
 *
 * Destructor function to free allocated memory of a WorkingPair-struct.
 *
 * Parameters:
 * -----------
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * History:
 * --------
 *	01/06/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API void delWorkingPair(void *workingPair);


////////////////////////////////////////////////////////////
// Definition of function prototypes regarding adsorption //
////////////////////////////////////////////////////////////


/*
 * iso_w_pT:
 * ---------
 *
 * Calculates equilibrium loading w in kg/kg depending on equilibrium pressure
 * p in Pa and equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium loading in kg/kg.
 *
 * History:
 * --------
 *	01/06/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double iso_w_pT(double p_Pa, double T_K, void *workingPair);


/*
 * iso_p_wT:
 * ---------
 *
 * Calculates equilibrium pressure p in Pa depending on equilibrium loading w
 * in kg/kg and equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium pressure in Pa.
 *
 * History:
 * --------
 *	01/06/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double iso_p_wT(double w_kgkg, double T_K, void *workingPair);


/*
 * iso_T_pw:
 * ---------
 *
 * Calculates equilibrium temperature in K depending on equilibrium pressure p
 * in Pa and equilibrium loading w in kg/kg.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium temperature in K.
 *
 * Remarks:
 * --------
 *	Function uses equation of states implemented for calculation of vapour
 *	pressure, saturated liquid density, derivative of vapour pressure wrt.
 *	temperature, and derivative of saturated liquid density wrt. temperature
 *	when isothem of working pair is based on volumetric approach (e.g.
 *	Dubinin theory). 
 *
 * History:
 * --------
 *	01/06/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double iso_T_pw(double p_Pa, double w_kgkg, void *workingPair);


/*
 * iso_dw_dp_pT:
 * -------------
 *
 * Calculates derivative of equilibrium loading w with respect to pressure 
 * p in kg/kg/Pa depending on equilibrium pressure p in Pa and equilibrium 
 * temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. pressure in kg/kg/Pa.
 *
 * History:
 * --------
 *	01/06/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double iso_dw_dp_pT(double p_Pa, double T_K, void *workingPair);


/*
 * iso_dw_dT_pT:
 * -------------
 *
 * Calculates derivative of equilibrium loading w with respect to temperature 
 * T in kg/kg/K depending on equilibrium pressure p in Pa and equilibrium 
 * temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. temperature in kg/kg/K.
 *
 * History:
 * --------
 *	01/06/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double iso_dw_dT_pT(double p_Pa, double T_K, void *workingPair);


/*
 * iso_dp_dw_wT:
 * -------------
 *
 * Calculates derivative of equilibrium pressure p with respect to loading 
 * w in kgPa/kg depending on equilibrium loading w in kg/kg and equilibrium 
 * temperature T in K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. loading in Pakg/kg.
 *
 * History:
 * --------
 *	01/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double iso_dp_dw_wT(double w_kgkg, double T_K, void *workingPair);


/*
 * iso_dp_dT_wT:
 * -------------
 *
 * Calculates derivative of equilibrium pressure p with respect to temperature 
 * T in kg/kg/K depending on equilibrium loading w in kg/kg and equilibrium 
 * temperature T in K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. temperature in Pa/K.
 *
 * History:
 * --------
 *	01/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double iso_dp_dT_wT(double w_kgkg, double T_K, void *workingPair);


/*
 * iso_piStar_pyxgTM:
 * ------------------
 *
 * Calculates reduced spreading pressure piStar in kg/mol depending on 
 * equilibrium pressure p in Pa, molar fraction of refrigerant in vapour phase 
 * in mol/mol, molar fraction of refrigerant in adsorbed phase in mol/mol, 
 * activity coefficient of refrigerant in adsorbed phase, equilibrium 
 * temperature T in K and molar mass of refrigerant M in kg/mol. The reduced 
 * spreading pressure is defined as follows:
 *
 * 	piStar = A * pi / (R * T * m_sorbent) = 1 / M *
 *		Integral_0^p0{w(p,T) / p * dp}
 *
 *	where p0 = p_total*y / (gamma*x)
 *
 * Parameters:
 * -----------
 *	double p_total_Pa:
 *		Total pressure of vapour phase in Pa.
 *	double y_molmol:
 *		Molar fraction of refrigerant in vapour phase in mol/mol.
 *	double x_molmol:
 *		Molar fraction of refrigerant in adsorbed phase in mol/mol.
 *	double gamma:
 *		Activity coefficent of refrigerant in adsorbed phase.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double M_kgmol:
 *		Molar mass of refrigerant M in kg/mol.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Reduced spreading pressure in kg/mol.
 *
 */
DLL_API double iso_piStar_pyxgTM(double p_total_Pa, double y_molmol,
	double x_molmol, double gamma, double T_K, double M_kgmol, 
	void *workingPair);


/*
 * iso_W_ARho:
 * -----------
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
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium volumetric loading W in m³/kg.
 *
 * Remarks:
 * --------
 *	Saturated liquid density of adsorpt is requird to calculate volumetric
 *	loading from characteristic curve in m3/kg when coefficients would directly
 *	return loading w in kg/kg. Therefore, function checks unit of coefficents
 *	by checking isotherm coefficients.
 *
 * History:
 * --------
 *	01/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double iso_W_ARho(double A_Jmol, double rho_l_kgm3, void *workingPair);


/*
 * iso_A_WRho:
 * -----------
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
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium adsorption potential A in J/mol.
 *
 * Remarks:
 * --------
 *	Saturated liquid density of adsorpt is requird to calculate volumetric
 *	loading from characteristic curve in m3/kg when coefficients would directly
 *	return loading w in kg/kg.
 *
 * History:
 * --------
 *	01/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double iso_A_WRho(double W_m3kg, double rho_l_kgm3, void *workingPair);


/*
 * iso_w_pTpsatRho:
 * ----------------
 *
 * Calculates equilibrium loading w in kg/kg depending on equilibrium pressure
 * p in Pa, equilibrium temperature T in K, saturation pressure p_Sat in Pa and 
 * saturated liquid density of adsorpt rho in kg/m³.
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
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium loading in kg/kg.
 *
 * History:
 * --------
 *	01/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double iso_w_pTpsatRho(double p_Pa, double T_K, double p_sat_Pa,
	double rho_kgm3, void *workingPair);


/*
 * iso_p_wTpsatRho:
 * ----------------
 *
 * Calculates equilibrium pressure p in Pa depending on equilibrium loading w in
 * kg/kg, equilibrium temperature T in K, saturation pressure p_Sat in Pa and 
 * density of adsorpt rho in kg/m³.
 *
 * Parameters:
 * -----------
 * 	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_kgm3:
 *		Density of adsorpt in kg/m³.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure in Pa.
 *
 * History:
 * --------
 *	01/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double iso_p_wTpsatRho(double w_kgkg, double T_K, double p_sat_Pa,
	double rho_kgm3, void *workingPair);


/*
 * iso_T_pwpsatRho:
 * ----------------
 *
 * Calculates equilibrium temperature T in K depending on equilibrium pressure
 * in Pa, equilibrium loading w in kg/kg, saturation pressure p_Sat in Pa and 
 * density of adsorpt rho in kg/m³.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure in Pa.
 * 	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_kgm3:
 *		Density of adsorpt in kg/m³.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium temperature in K.
 *
 * Remarks:
 * --------
 *	Uses internal euqation of states to calculate vapour pressure, saturated
 *	liquid density of adsorpt, derivative of vapour pressure wrt. temperature
 *	and derivative of saturated liquid density of adsorpt wrt. temperature. If
 *	equation of states are not implemented for refrigerant, function returns -1
 *  and throws warning. If user want to use his own equation of states, this
 *  function cannot be used and determination of root to calculate T must
 *  be implemented by user.
 *
 * History:
 * --------
 *	01/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double iso_T_pwpsatRho(double p_Pa, double w_kgkg, double p_sat_Pa,
	double rho_kgm3, void *workingPair);
 

/*
 * iso_dW_dA_ARho:
 * ---------------
 *
 * Calculates derivative of equilibrium volumetric loading dW_dA in m³mol/kg/J
 * depending on equilibrium adsorption potential A in J/mol and saturated liquid
 * density of adsorpt rho_l in kg/m3.
 *
 * Parameters:
 * -----------
 * 	double A_Jmol:
 *		Equilibrium adsorption potential A in mol/J.
 * 	double rho_l_kgm3:
 *		Saturated liquid density of adsorpt rho_l in kg/m3.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of volumetric loading wrt. adsorption potential in
 *		m³mol/kg/J.
 *
 * Remarks:
 * --------
 *	Saturated liquid density of adsorpt is requird to calculate volumetric
 *	loading from characteristic curve in m3/kg when coefficients would directly
 *	return loading w in kg/kg.
 *
 * History:
 * --------
 *	01/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double iso_dW_dA_ARho(double A_Jmol, double rho_l_kgm3, 
	void *workingPair);


/*
 * iso_dA_dW_WRho:
 * ---------------
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
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of adsorption potential wrt. volumetric loading in
 *		kgJ/mol/m³.
 *
 * Remarks:
 * --------
 *	Saturated liquid density of adsorpt is requird to calculate volumetric
 *	loading from characteristic curve in m3/kg when coefficients would directly
 *	return loading w in kg/kg.
 *
 * History:
 * --------
 *	01/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double iso_dA_dW_WRho(double W_m3kg, double rho_l_kgm3, 
	void *workingPair);


/*
 * iso_dw_dp_pTpsatRho:
 * --------------------
 *
 * Calculates derivative of equilibrium loading dw_dp with respect to pressure
 * in kg/kg/Pa depending on equilibrium pressure p in Pa, equilibrium 
 * temperature T in K, saturation pressure p_Sat in Pa and density of adsorpt 
 * rho in kg/m³.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_kgm3:
 *		Density of adsorpt in kg/m³.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. pressure in kg/kg/Pa.
 *
 * History:
 * --------
 *	01/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double iso_dw_dp_pTpsatRho(double p_Pa, double T_K, double p_sat_Pa, 
	double rho_kgm3, void *workingPair);


/*
 * iso_dw_dT_pTpsatRho:
 * --------------------
 *
 * Calculates derivative of equilibrium loading dw_dp with respect to
 * temperature in kg/kg/K depending on equilibrium pressure p in Pa, equilibrium 
 * temperature T in K, saturation pressure p_Sat in Pa, density of adsorpt 
 * rho in kg/m³, derivative of saturation pressure with respect to temperature
 * dp_sat_dT in Pa/K and derivative of density of adsorpt with respect to
 * temperature drho_dT in kg/m³/K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_kgm3:
 *		Density of adsorpt in kg/m³.
 * 	double dp_sat_dT_PaK:
 *		Derivative of saturation pressure wrt. temperature in Pa/K.
 *	double drho_dT_kgm3K:
 *		Derivative of density of adsorpt wrt. temperature in kg/m³/K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. temperature in kg/kg/K.
 *
 * History:
 * --------
 *	01/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double iso_dw_dT_pTpsatRho(double p_Pa, double T_K, double p_sat_Pa, 
	double rho_kgm3, double dp_sat_dT_PaK, double drho_dT_kgm3K,
	void *workingPair);


/*
 * iso_dp_dw_wTpsatRho:
 * --------------------
 *
 * Calculates derivative of equilibrium pressure p with respect to loading 
 * w in kgPa/kg depending on equilibrium loading w in kg/kg, equilibrium 
 * temperature T in K, saturation pressure p_Sat in Pa and density of adsorpt 
 * rho in kg/m³.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_kgm3:
 *		Density of adsorpt in kg/m³.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. loading in Pakg/kg.
 *
 * History:
 * --------
 *	01/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double iso_dp_dw_wTpsatRho(double w_kgkg, double T_K, double p_sat_Pa, 
	double rho_kgm3, void *workingPair);


/*
 * iso_dp_dT_wTpsatRho:
 * --------------------
 *
 * Calculates derivative of equilibrium pressure p with respect to temperature 
 * T in kg/kg/K depending on equilibrium loading w in kg/kg, equilibrium 
 * temperature T in K, saturation pressure p_Sat in Pa and density of adsorpt 
 * rho in kg/m³, derivative of saturation pressure with respect to temperature
 * dp_sat_dT in Pa/K and derivative of density of adsorpt with respect to
 * temperature drho_dT in kg/m³/K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_kgm3:
 *		Density of adsorpt in kg/m³.
 * 	double dp_sat_dT_PaK:
 *		Derivative of saturation pressure wrt. temperature in Pa/K.
 *	double drho_dT_kgm3K:
 *		Derivative of density of adsorpt wrt. temperature in kg/m³/K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. temperature in Pa/K.
 *
 * History:
 * --------
 *	01/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double iso_dp_dT_wTpsatRho(double w_kgkg, double T_K, double p_sat_Pa, 
	double rho_kgm3, double dp_sat_dT_PaK, double drho_dT_kgm3K,
	void *workingPair);


/*
 * iso_piStar_pyxgTpsatRhoM:
 * -------------------------
 *
 * Calculates reduced spreading pressure in kg/mol depending on equilibrium
 * pressure p in Pa, molar fraction of refrigerant in vapour phase in mol/mol,
 * molar fraction of refrigerant in adsorbed phase in mol/mol, activity
 * coefficient of refrigerant in adsorbed phase, equilibrium temperature T in K,
 * saturation pressure p_Sat in Pa and density of adsorpt rho in kg/m³ and molar
 * mass of refrigerant M in kg/mol. The reduced spreading pressure is defined as 
 * follows:
 *
 * 	piStar = A * pi / (R * T * m_sorbent) = 1 / M *
 *		Integral_0^p0{w(p,T) / p * dp}
 *
 *	where p0 = p_total*y / (gamma*x)
 *
 * Parameters:
 * -----------
 *	double p_total_Pa:
 *		Total pressure of vapour phase in Pa.
 *	double y_molmol:
 *		Molar fraction of refrigerant in vapour phase in mol/mol.
 *	double x_molmol:
 *		Molar fraction of refrigerant in adsorbed phase in mol/mol.
 *	double gamma:
 *		Activity coefficent of refrigerant in adsorbed phase.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	double p_sat_Pa:
 *		Saturation pressure in Pa.
 *	double rho_kgm3:
 *		Density of adsorpt in kg/m³.
 *	double M_kgmol:
 *		Molar mass of refrigerant M in kg/mol.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Reduced spreading pressure in kg/mol.
 *
 * History:
 * --------
 *	01/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double iso_piStar_pyxgTpsatRhoM(double p_total_Pa, double y_molmol,
	double x_molmol, double gamma, double T_K, double p_sat_Pa, 
	double rho_kgm3, double M_kgmol, void *workingPair);


/*
 * direct_iso_w_pT_workingPair:
 * ----------------------------
 *
 * Calculates equilibrium loading w in kg/kg depending on equilibrium pressure
 * p in Pa, equilibrium temperature T in K, and specified working pair.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *
 *	const char *path_db:
 *		Path to database.
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	const char *wp_iso:
 *		Name of isotherm.
 * 	const char *rf_psat:
 *		Name of calculation approach for vapour pressure.
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 * 	const char *rf_ac:
 *		Name of calculation approach for activity coefficients.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium loading in kg/kg.
 *
 * History:
 * --------
 *	02/11/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_iso_w_pT_workingPair(double p_Pa, double T_K, 
	const char *path_db, const char *wp_as, const char *wp_st, 
	const char *wp_rf, const char *wp_iso, const char *rf_psat, 
	const char *rf_rhol, const char *rf_ac);

#endif