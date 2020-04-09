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
 *		Name of calculation approach for vapor pressure.
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * 	double *iso_par:
 *		Array containing coefficients of isotherm.
 * 	double *psat_par:
 *		Array containing coefficients of vapor pressure equation.
 * 	double *rhol_par:
 *		Array containing coefficients of saturated liquid density equation.
 *
 *	Adsorption *adsorption:
 *		Struct containing function pointers for isotherm functions.
 *	Absorption *absorption:
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
 * Initialization function to create a new WorkingPair-struct. Therefore,
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	struct *WorkingPair:
 *		Returns malloc-ed WorkingPair-struct containing all information required
 *		to calculate the resorption equilibrium of the working working.
 *
 * History:
 * --------
 *	01/06/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API WorkingPair *newWorkingPair(const char *path_db, const char *wp_as,
	const char *wp_st, const char *wp_rf, const char *wp_iso, int no_iso,
	const char *rf_psat, int no_p_sat, const char *rf_rhol, int no_rhol) ;


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


/*
 * warning_struct:
 * ---------------
 *
 * Auxiliary function to throw warning if some struct (e.g. workingPair or
 * Adsorption) does not exist.
 *
 * Parameters:
 * -----------
 * 	const char *name_struct:
 *		Name of struct that does not exist.
 * 	const char *name_function:
 *		Name of function that does not exist.
 *
 * Returns:
 * --------
 *	double:
 *		Returns -1 to indicate error.
 *
 * History:
 * --------
 *	27/03/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double warning_struct(const char *name_struct, const char *name_function);


/*
 * warning_function:
 * -----------------
 *
 * Auxiliary function to throw warning if some parameter is missing.
 *
 * Parameters:
 * -----------
 * 	const char *name_parameter:
 *		Name of parameter that is missing.
 * 	const char *name_wpair_function:
 *		Name of working pair function that does not exist.
 *
 * Returns:
 * --------
 *	double:
 *		Returns -1 to indicate error.
 *
 * History:
 * --------
 *	27/03/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double warning_parameter(const char *name_parameter,
	const char *name_wpair_function);


/*
 * warning_function:
 * -----------------
 *
 * Auxiliary function to throw warning if some function does not exist.
 *
 * Parameters:
 * -----------
 * 	const char *name_iso_function:
 *		Name of isotherm function that does not exist.
 * 	const char *name_wpair_function:
 *		Name of working pair function that does not exist.
 *
 * Returns:
 * --------
 *	double:
 *		Returns -1 to indicate error.
 *
 * History:
 * --------
 *	27/03/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double warning_function(const char *name_function,
	const char *name_wpair_function);


////////////////////////////////////////////////////////////////////////////
// Definition of function prototypes regarding adsorption working w struct//
////////////////////////////////////////////////////////////////////////////
/*
 * ads_w_pT:
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
DLL_API double ads_w_pT(double p_Pa, double T_K, void *workingPair);


/*
 * ads_p_wT:
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
DLL_API double ads_p_wT(double w_kgkg, double T_K, void *workingPair);


/*
 * ads_T_pw:
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
 *	Function uses equation of states implemented for calculation of vapor
 *	pressure, saturated liquid density, derivative of vapor pressure wrt.
 *	temperature, and derivative of saturated liquid density wrt. temperature
 *	when isotherm of working pair is based on volumetric approach (e.g.
 *	Dubinin theory).
 *
 * History:
 * --------
 *	01/06/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_T_pw(double p_Pa, double w_kgkg, void *workingPair);


/*
 * ads_dw_dp_pT:
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
DLL_API double ads_dw_dp_pT(double p_Pa, double T_K, void *workingPair);


/*
 * ads_dw_dT_pT:
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
DLL_API double ads_dw_dT_pT(double p_Pa, double T_K, void *workingPair);


/*
 * ads_dp_dw_wT:
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
DLL_API double ads_dp_dw_wT(double w_kgkg, double T_K, void *workingPair);


/*
 * ads_dp_dT_wT:
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
DLL_API double ads_dp_dT_wT(double w_kgkg, double T_K, void *workingPair);


/*
 * ads_piStar_pyxgTM:
 * ------------------
 *
 * Calculates reduced spreading pressure piStar in kg/mol depending on
 * equilibrium pressure p in Pa, molar fraction of refrigerant in vapor phase
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
 *		Total pressure of vapor phase in Pa.
 *	double y_molmol:
 *		Molar fraction of refrigerant in vapor phase in mol/mol.
 *	double x_molmol:
 *		Molar fraction of refrigerant in adsorbed phase in mol/mol.
 *	double gamma:
 *		Activity coefficient of refrigerant in adsorbed phase.
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
 * History:
 * --------
 *	01/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_piStar_pyxgTM(double p_total_Pa, double y_molmol,
	double x_molmol, double gamma, double T_K, double M_kgmol,
	void *workingPair);


/*
 * ads_sur_w_pT:
 * -------------
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
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_sur_w_pT(double p_Pa, double T_K, void *workingPair);


/*
 * ads_sur_p_wT:
 * -------------
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
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_sur_p_wT(double w_kgkg, double T_K, void *workingPair);


/*
 * ads_sur_T_pw:
 * -------------
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
 *	This function is only valid for isotherm models based on the surface
 *	approach.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_sur_T_pw(double p_Pa, double w_kgkg, void *workingPair);


/*
 * ads_sur_dw_dp_pT:
 * -----------------
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
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_sur_dw_dp_pT(double p_Pa, double T_K, void *workingPair);


/*
 * ads_sur_dw_dT_pT:
 * -----------------
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
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_sur_dw_dT_pT(double p_Pa, double T_K, void *workingPair);


/*
 * ads_sur_dp_dw_wT:
 * -----------------
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
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_sur_dp_dw_wT(double w_kgkg, double T_K, void *workingPair);


/*
 * ads_sur_dp_dT_wT:
 * -----------------
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
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_sur_dp_dT_wT(double w_kgkg, double T_K, void *workingPair);


/*
 * ads_sur_piStar_pyxgTM:
 * ----------------------
 *
 * Calculates reduced spreading pressure piStar in kg/mol depending on
 * equilibrium pressure p in Pa, molar fraction of refrigerant in vapor phase
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
 *		Total pressure of vapor phase in Pa.
 *	double y_molmol:
 *		Molar fraction of refrigerant in vapor phase in mol/mol.
 *	double x_molmol:
 *		Molar fraction of refrigerant in adsorbed phase in mol/mol.
 *	double gamma:
 *		Activity coefficient of refrigerant in adsorbed phase.
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
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_sur_piStar_pyxgTM(double p_total_Pa, double y_molmol,
	double x_molmol, double gamma, double T_K, double M_kgmol,
	void *workingPair);


/*
 * ads_sur_w_pTpsat:
 * -----------------
 *
 * Calculates equilibrium loading w in kg/kg depending on equilibrium pressure
 * p in Pa, equilibrium temperature T in K, and saturation pressure p_sat in Pa.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation vapor pressure in Pa.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium loading in kg/kg.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach using the saturated vapor pressure as additional argument.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_sur_w_pTpsat(double p_Pa, double T_K, double p_sat_Pa,
	void *workingPair);


/*
 * ads_sur_p_wTpsat:
 * -----------------
 *
 * Calculates equilibrium pressure p in Pa depending on equilibrium loading w
 * in kg/kg, equilibrium temperature T in K, and saturation pressure p_sat in
 * Pa.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation vapor pressure in Pa.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium pressure in Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach using the saturated vapor pressure as additional argument.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_sur_p_wTpsat(double w_kgkg, double T_K, double p_sat_Pa,
	void *workingPair);


/*
 * ads_sur_T_pwpsat:
 * -----------------
 *
 * Calculates equilibrium temperature in K depending on equilibrium pressure p
 * in Pa, equilibrium loading w in kg/kg, and saturation pressure p_sat in Pa.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double p_sat_Pa:
 *		Saturation vapor pressure in Pa.
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
 *	This function is only valid for isotherm models based on the surface
 *	approach using the saturated vapor pressure as additional argument.
 *	Uses internal equation of states to calculate vapor pressure and derivative
 *	of vapor pressure wrt. temperature. If equation of state is not implemented
 *	for refrigerant, function returns -1 and throws warning. If user want to use
 *	his own equation of state, this function cannot be used and determination of
 *	root to calculate T must be implemented by user.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_sur_T_pwpsat(double p_Pa, double w_kgkg, double p_sat_Pa,
	void *workingPair);


/*
 * ads_sur_dw_dp_pTpsat:
 * ---------------------
 *
 * Calculates derivative of equilibrium loading w with respect to pressure
 * p in kg/kg/Pa depending on equilibrium pressure p in Pa, equilibrium
 * temperature T in K, and saturation pressure p_sat in Pa.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation vapor pressure in Pa.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. pressure in kg/kg/Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach using the saturated vapor pressure as additional argument.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_sur_dw_dp_pTpsat(double p_Pa, double T_K, double p_sat_Pa,
	void *workingPair);


/*
 * ads_sur_dw_dT_pTpsat:
 * ---------------------
 *
 * Calculates derivative of equilibrium loading w with respect to temperature
 * T in kg/kg/K depending on equilibrium pressure p in Pa, equilibrium
 * temperature T in K, saturation pressure p_sat in Pa, and derivative of
 * saturation pressure wrt. temperature dp_sat_dT in Pa/K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation vapor pressure in Pa.
 * 	double dp_sat_dT_PaK:
 *		Derivative of saturation pressure wrt. temperature in Pa/K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. temperature in kg/kg/K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach using the saturated vapor pressure as additional argument.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_sur_dw_dT_pTpsat(double p_Pa, double T_K, double p_sat_Pa,
	double dp_sat_dT_PaK, void *workingPair);


/*
 * ads_sur_dp_dw_wTpsat:
 * ---------------------
 *
 * Calculates derivative of equilibrium pressure p with respect to loading
 * w in kgPa/kg depending on equilibrium loading w in kg/kg, equilibrium
 * temperature T in K, and saturation pressure p_sat in Pa.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation vapor pressure in Pa.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. loading in Pakg/kg.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach using the saturated vapor pressure as additional argument.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_sur_dp_dw_wTpsat(double w_kgkg, double T_K, double p_sat_Pa,
	void *workingPair);


/*
 * ads_sur_dp_dT_wTpsat:
 * ---------------------
 *
 * Calculates derivative of equilibrium pressure p with respect to temperature
 * T in kg/kg/K depending on equilibrium loading w in kg/kg, equilibrium
 * temperature T in K, saturation pressure p_sat in Pa, and derivative of
 * saturation pressure wrt. temperature dp_sat_dT in Pa/K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation vapor pressure in Pa.
 * 	double dp_sat_dT_PaK:
 *		Derivative of saturation pressure wrt. temperature in Pa/K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. temperature in Pa/K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach using the saturated vapor pressure as additional argument.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_sur_dp_dT_wTpsat(double w_kgkg, double T_K, double p_sat_Pa,
	double dp_sat_dT_PaK, void *workingPair);


/*
 * ads_sur_piStar_pyxgTpsatM:
 * --------------------------
 *
 * Calculates reduced spreading pressure piStar in kg/mol depending on
 * equilibrium pressure p in Pa, molar fraction of refrigerant in vapor phase
 * in mol/mol, molar fraction of refrigerant in adsorbed phase in mol/mol,
 * activity coefficient of refrigerant in adsorbed phase, equilibrium
 * temperature T in K, saturation pressure p_sat in Pa, and molar mass of
 * refrigerant M in kg/mol. The reduced spreading pressure is defined as
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
 *		Total pressure of vapor phase in Pa.
 *	double y_molmol:
 *		Molar fraction of refrigerant in vapor phase in mol/mol.
 *	double x_molmol:
 *		Molar fraction of refrigerant in adsorbed phase in mol/mol.
 *	double gamma:
 *		Activity coefficient of refrigerant in adsorbed phase.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation vapor pressure in Pa.
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
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach using the saturated vapor pressure as additional argument.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_sur_piStar_pyxgTpsatM(double p_total_Pa, double y_molmol,
	double x_molmol, double gamma, double T_K, double p_sat_Pa, double M_kgmol,
	void *workingPair);


/*
 * ads_vol_W_ARho:
 * ---------------
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
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *	Saturated liquid density of adsorpt is required to calculate volumetric
 *	loading from characteristic curve in m3/kg when coefficients would directly
 *	return loading w in kg/kg. Therefore, function checks unit of coefficients
 *	by checking isotherm coefficients.
 *
 * History:
 * --------
 *	01/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_vol_W_ARho(double A_Jmol, double rho_l_kgm3,
	void *workingPair);


/*
 * ads_vol_A_WRho:
 * ---------------
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
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *	Saturated liquid density of adsorpt is required to calculate volumetric
 *	loading from characteristic curve in m3/kg when coefficients would directly
 *	return loading w in kg/kg.
 *
 * History:
 * --------
 *	01/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_vol_A_WRho(double W_m3kg, double rho_l_kgm3,
	void *workingPair);


/*
 * ads_vol_w_pTpsatRho:
 * --------------------
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
DLL_API double ads_vol_w_pTpsatRho(double p_Pa, double T_K, double p_sat_Pa,
	double rho_kgm3, void *workingPair);


/*
 * ads_vol_p_wTpsatRho:
 * --------------------
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
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *
 * History:
 * --------
 *	01/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_vol_p_wTpsatRho(double w_kgkg, double T_K, double p_sat_Pa,
	double rho_kgm3, void *workingPair);


/*
 * ads_vol_T_pwpsatRho:
 * --------------------
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
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *	Uses internal equations of states to calculate vapor pressure, saturated
 *	liquid density of adsorpt, derivative of vapor pressure wrt. temperature
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
DLL_API double ads_vol_T_pwpsatRho(double p_Pa, double w_kgkg, double p_sat_Pa,
	double rho_kgm3, void *workingPair);


/*
 * ads_vol_dW_dA_ARho:
 * -------------------
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
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *	Saturated liquid density of adsorpt is required to calculate volumetric
 *	loading from characteristic curve in m3/kg when coefficients would directly
 *	return loading w in kg/kg.
 *
 * History:
 * --------
 *	01/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_vol_dW_dA_ARho(double A_Jmol, double rho_l_kgm3,
	void *workingPair);


/*
 * ads_vol_dA_dW_WRho:
 * -------------------
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
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *	Saturated liquid density of adsorpt is required to calculate volumetric
 *	loading from characteristic curve in m3/kg when coefficients would directly
 *	return loading w in kg/kg.
 *
 * History:
 * --------
 *	01/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_vol_dA_dW_WRho(double W_m3kg, double rho_l_kgm3,
	void *workingPair);


/*
 * ads_vol_dw_dp_pTpsatRho:
 * ------------------------
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
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *
 * History:
 * --------
 *	01/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_vol_dw_dp_pTpsatRho(double p_Pa, double T_K, double p_sat_Pa,
	double rho_kgm3, void *workingPair);


/*
 * ads_vol_dw_dT_pTpsatRho:
 * ------------------------
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
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *
 * History:
 * --------
 *	01/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_vol_dw_dT_pTpsatRho(double p_Pa, double T_K, double p_sat_Pa,
	double rho_kgm3, double dp_sat_dT_PaK, double drho_dT_kgm3K,
	void *workingPair);


/*
 * ads_vol_dp_dw_wTpsatRho:
 * ------------------------
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
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *
 * History:
 * --------
 *	01/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_vol_dp_dw_wTpsatRho(double w_kgkg, double T_K,
	double p_sat_Pa, double rho_kgm3, void *workingPair);


/*
 * ads_vol_dp_dT_wTpsatRho:
 * ------------------------
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
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *
 * History:
 * --------
 *	01/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_vol_dp_dT_wTpsatRho(double w_kgkg, double T_K,
	double p_sat_Pa, double rho_kgm3, double dp_sat_dT_PaK,
	double drho_dT_kgm3K, void *workingPair);


/*
 * ads_vol_piStar_pyxgTpsatRhoM:
 * -----------------------------
 *
 * Calculates reduced spreading pressure in kg/mol depending on equilibrium
 * pressure p in Pa, molar fraction of refrigerant in vapor phase in mol/mol,
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
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *
 * History:
 * --------
 *	01/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ads_vol_piStar_pyxgTpsatRhoM(double p_total_Pa, double y_molmol,
	double x_molmol, double gamma, double T_K, double p_sat_Pa,
	double rho_kgm3, double M_kgmol, void *workingPair);


////////////////////////////////////////////////////////////////////////////
// Definition of function prototypes regarding absorption working w struct//
////////////////////////////////////////////////////////////////////////////
/*
 * abs_con_X_pT:
 * -------------
 *
 * Calculates equilibrium concentration X in kg/kg depending on equilibrium
 * pressure p in Pa and equilibrium temperature T in K.
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
 *		Equilibrium concentration in kg/kg.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the conventional
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double abs_con_X_pT(double p_Pa, double T_K, void *workingPair);


/*
 * abs_con_p_XT:
 * -------------
 *
 * Calculates equilibrium pressure p in Pa depending on equilibrium
 * concentration X in kg/kg and equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 *	double X_kgkg:
 *		Equilibrium concentration in kg/kg.
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
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the conventional
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double abs_con_p_XT(double X_kgkg, double T_K, void *workingPair);


/*
 * abs_con_T_pX:
 * -------------
 *
 * Calculates equilibrium temperature in K depending on equilibrium pressure p
 * in Pa and equilibrium concentration X in kg/kg.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double X_kgkg:
 *		Equilibrium concentration in kg/kg.
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
 *	This function is only valid for isotherm models based on the conventional
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double abs_con_T_pX(double p_Pa, double X_kgkg, void *workingPair);


/*
 * abs_con_dX_dp_pT:
 * -----------------
 *
 * Calculates derivative of equilibrium concentration X with respect to pressure
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
 *		Derivative of equilibrium concentration wrt. pressure in kg/kg/Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the conventional
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double abs_con_dX_dp_pT(double p_Pa, double T_K, void *workingPair);


/*
 * abs_con_dX_dT_pT:
 * -----------------
 *
 * Calculates derivative of equilibrium concentration X with respect to
 * temperature T in kg/kg/K depending on equilibrium pressure p in Pa and
 * equilibrium temperature T in K.
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
 *		Derivative of equilibrium concentration wrt. temperature in kg/kg/K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the conventional
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double abs_con_dX_dT_pT(double p_Pa, double T_K, void *workingPair);


/*
 * abs_con_dp_dX_XT:
 * -----------------
 *
 * Calculates derivative of equilibrium pressure p with respect to concentration
 * w in kgPa/kg depending on equilibrium concentration X in kg/kg and
 * equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 *	double X_kgkg:
 *		Equilibrium concentration in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. concentration in Pakg/kg.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the conventional
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double abs_con_dp_dX_XT(double X_kgkg, double T_K, void *workingPair);


/*
 * abs_con_dp_dT_XT:
 * -----------------
 *
 * Calculates derivative of equilibrium pressure p with respect to temperature
 * T in kg/kg/K depending on equilibrium concentration X in kg/kg and
 * equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 *	double X_kgkg:
 *		Equilibrium concentration in kg/kg.
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
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the conventional
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double abs_con_dp_dT_XT(double X_kgkg, double T_K, void *workingPair);


/*
 * abs_act_g_Txv1v2:
 * -----------------
 *
 * Calculates activity coefficient of first component depending on temperature
 * T_K in K, mole fraction in liquid phase x_molmol in mol/mol, molar volume of
 * first component in m³/mol, and molar volume of second component in m³/mol.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double x_molmol:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
 *	double v1_m3mol:
 *		Equilibrium molar volume of first component in m³/mol.
 *	double v2_m3mol:
 *		Equilibrium molar volume of second component in m³/mol.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Activity coefficient of first component.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on activity
 *	coefficients.
 *	Molar volumes may are not required and ignored. When molar volumes are
 * 	required, function uses molar volumes stored in JSON file when input
 *	v1_m3mol or v2_m3mol is -1. Otherwise, function uses molar volumes given by
 *	inputs.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double abs_act_g_Txv1v2(double T_K, double x_molmol, double v1_m3mol,
	double v2_m3mol, void *workingPair);


/*
 * abs_act_p_Txv1v2psat:
 * ---------------------
 *
 * Calculates equilibrium pressure p_Pa in Pa of first component depending on
 * temperature T_K in K, mole fraction in liquid phase x_molmol in mol/mol,
 * molar volume of first component in m³/mol, molar volume of second component
 * in m³/mol,and saturation pressure of first component p_sat_Pa in Pa.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double x_molmol:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
 *	double v1_m3mol:
 *		Equilibrium molar volume of first component in m³/mol.
 *	double v2_m3mol:
 *		Equilibrium molar volume of second component in m³/mol.
 *	double p_sat_Pa:
 *		Saturation pressure of first component in Pa.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure p_Pa in Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on activity
 *	coefficients.
 *	Molar volumes may are not required and ignored. When molar volumes are
 * 	required, function uses molar volumes stored in JSON file when input
 *	v1_m3mol or v2_m3mol is -1. Otherwise, function uses molar volumes given by
 *	inputs.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double abs_act_p_Txv1v2psat(double T_K, double x_molmol,
	double v1_m3mol, double v2_m3mol, double p_sat_Pa, void *workingPair);


/*
 * abs_act_x_pTv1v2psat:
 * ---------------------
 *
 * Calculates mole fraction of first component in liquid phase x_molmol in
 * mol/mol depending on equilibrium pressure p_Pa in Pa of first component,
 * temperature T_K in K, molar volume of first component in m³/mol, molar volume
 * of second component in m³/mol and saturation pressure of first component
 * p_sat_Pa in Pa
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure p_Pa in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double v1_m3mol:
 *		Equilibrium molar volume of first component in m³/mol.
 *	double v2_m3mol:
 *		Equilibrium molar volume of second component in m³/mol.
 *	double p_sat_Pa:
 *		Saturation pressure of first component in Pa.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on activity
 *	coefficients.
 *	Molar volumes may are not required and ignored. When molar volumes are
 * 	required, function uses molar volumes stored in JSON file when input
 *	v1_m3mol or v2_m3mol is -1. Otherwise, function uses molar volumes given by
 *	inputs.
 *
 * History:
 * --------
 *	30/03/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double abs_act_x_pTv1v2psat(double p_Pa, double T_K, double v1_m3mol,
	double v2_m3mol, double p_sat_Pa, void *workingPair);


/*
 * abs_act_p_Txv1v2:
 * -----------------
 *
 * Calculates equilibrium pressure p_Pa in Pa of first component depending on
 * temperature T_K in K, mole fraction in liquid phase x_molmol in mol/mol,
 * molar volume of first component in m³/mol, and molar volume of second
 * component in m³/mol.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double x_molmol:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
 *	double v1_m3mol:
 *		Equilibrium molar volume of first component in m³/mol.
 *	double v2_m3mol:
 *		Equilibrium molar volume of second component in m³/mol.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure p_Pa in Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on activity
 *	coefficients.
 *	Molar volumes may are not required and ignored. When molar volumes are
 * 	required, function uses molar volumes stored in JSON file when input
 *	v1_m3mol or v2_m3mol is -1. Otherwise, function uses molar volumes given by
 *	inputs.
 *	This function uses internal model for vapor pressure of refrigerant.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double abs_act_p_Txv1v2(double T_K, double x_molmol,
	double v1_m3mol, double v2_m3mol, void *workingPair);


/*
 * abs_act_x_pTv1v2:
 * -----------------
 *
 * Calculates mole fraction of first component in liquid phase x_molmol in
 * mol/mol depending on equilibrium pressure p_Pa in Pa of first component,
 * temperature T_K in K, molar volume of first component in m³/mol, and molar
 * volume of second component in m³/mol.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure p_Pa in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double v1_m3mol:
 *		Equilibrium molar volume of first component in m³/mol.
 *	double v2_m3mol:
 *		Equilibrium molar volume of second component in m³/mol.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on activity
 *	coefficients.
 *	Molar volumes may are not required and ignored. When molar volumes are
 * 	required, function uses molar volumes stored in JSON file when input
 *	v1_m3mol or v2_m3mol is -1. Otherwise, function uses molar volumes given by
 *	inputs.
 *	This function uses internal model for vapor pressure of refrigerant.
 *
 * History:
 * --------
 *	30/03/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double abs_act_x_pTv1v2(double p_Pa, double T_K, double v1_m3mol,
	double v2_m3mol, void *workingPair);


/*
 * abs_mix_x_pT:
 * -------------
 *
 * Calculates equilibrium liquid mole fraction of first component in mol/mol
 * depending on pressure in Pa and temperature T_K in K.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure p_Pa in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium mole fraction in of first component liquid phase in mol/mol.
 *	double *ret_y_1_molmol:
 *		Equilibrium mole fraction of first component.
 *	double *ret_y_2_molmol:
 *		Equilibrium mole fraction of second component.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on mixing rules.
 *
 * History:
 * --------
 *	04/09/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double abs_mix_x_pT(double *ret_y_1_molmol, double *ret_y_2_molmol,
	double p_Pa, double T_K, void *workingPair);


/*
 * abs_mix_p_Tx:
 * -------------
 *
 * Calculates equilibrium pressure p_Pa in Pa depending on temperature T_K in K
 * and mole fraction in liquid phase x_molmol in mol/mol.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double x_1_molmol:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure p_Pa in Pa.
 *	double *ret_y_1_molmol:
 *		Equilibrium mole fraction of first component.
 *	double *ret_y_2_molmol:
 *		Equilibrium mole fraction of second component.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on mixing rules.
 *
 * History:
 * --------
 *	04/09/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double abs_mix_p_Tx(double *ret_y_1_molmol, double *ret_y_2_molmol,
	double T_K, double x_1_molmol, void *workingPair);


/*
 * abs_mix_T_px:
 * -------------
 *
 * Calculates equilibrium temperature T_K in K depending on pressure p_Pa in Pa
 * and mole fraction in liquid phase x_molmol in mol/mol.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure p_Pa in Pa.
 *	double x_1_molmol:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
 *	double *ret_y_1_molmol:
 *		Equilibrium mole fraction of first component.
 *	double *ret_y_2_molmol:
 *		Equilibrium mole fraction of second component.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on mixing rules.
 *	Uses Newton-Raphson method for calculating equilibrium temperature.
 *
 * History:
 * --------
 *	04/09/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double abs_mix_T_px(double *ret_y_1_molmol, double *ret_y_2_molmol,
	double p_Pa, double x_1_molmol, void *workingPair);


/*
 * abs_mix_dp_dx_Tx:
 * -----------------
 *
 * Calculates derivative of equilibrium pressure with respect to liquid mole
 * fraction in Pa depending on temperature T_K in K and mole fraction in liquid
 * phase x_molmol in mol/mol.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double x_1_molmol:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt liquid mole fraction in Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on mixing rules.
 *	Derivative is calculated numerically by the symmetric derivative using
 *  h = 0.00000001 mol/mol as small change.
 *
 * History:
 * --------
 *	04/09/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double abs_mix_dp_dx_Tx(double T_K, double x_1_molmol,
	void *workingPair);


/*
 * abs_mix_dp_dT_Tx:
 * -----------------
 *
 * Calculates derivative of equilibrium pressure with respect to temperature in
 * Pa/K depending on temperature T_K in K and mole fraction in liquid phase
 * x_molmol in mol/mol.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double x_1_molmol:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt temperature in Pa/K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on mixing rules.
 *	Derivative is calculated numerically by the symmetric derivative using
 *  h = 0.0001 K as small change.
 *
 * History:
 * --------
 *	04/09/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double abs_mix_dp_dT_Tx(double T_K, double x_1_molmol,
	void *workingPair);


/////////////////////////////////////////////////////////////////////////////
// Definition of function prototypes regarding refrigerant working w struct//
/////////////////////////////////////////////////////////////////////////////
/*
 * ref_p_sat_T:
 * ------------
 *
 * Calculates saturation pressure in Pa depending on equilibrium temperature T
 * in K.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Saturation pressure in Pa.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ref_p_sat_T(double T_K, void *workingPair);


/*
 * ref_dp_sat_dT_T:
 * ----------------
 *
 * Calculates derivative of saturation pressure in Pa/K wrt to temperature
 * depending on equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of saturation pressure wrt temperature in Pa/K.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ref_dp_sat_dT_T(double T_K, void *workingPair);


/*
 * ref_rho_l_T:
 * ------------
 *
 * Calculates saturated liquid density in kg/m³ depending on equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Saturated liquid density in kg/m³.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ref_rho_l_T(double T_K, void *workingPair);


/*
 * ref_drho_l_dT_T:
 * ----------------
 *
 * Calculates derivative of saturated liquid density wrt temperature in kg/m³/K
 * depending on equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of saturated liquid density wrt temperature in kg/m³/K.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ref_drho_l_dT_T(double T_K, void *workingPair);


/*
 * ref_T_sat_p:
 * ------------
 *
 * Calculates saturation temperature in K depending on equilibrium pressure p
 * in Pa.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Saturation pressure in Pa.
 * 	struct *WorkingPair:
 *		Pointer of WorkingPair-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium temperature in K.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double ref_T_sat_p(double p_Pa, void *workingPair);


/////////////////////////////////////////////////////////////////////////////
// Definition of function prototypes regarding adsorption working wo struct//
/////////////////////////////////////////////////////////////////////////////
/*
 * direct_ads_w_pT:
 * ----------------
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
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
DLL_API double direct_ads_w_pT(double p_Pa, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ads_p_wT:
 * ----------------
 *
 * Calculates equilibrium pressure p in Pa depending on equilibrium temperature
 * T in K, equilibrium loading w in kg/kg, equilibrium temperature T in K, and
 * specified working pair.
 *
 * Parameters:
 * -----------
 * 	double w_kgkg:
 *		Equilibrium loading in kg/kg.
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *		Name of calculation approach for liquid density.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure in Pa.
 *
 * History:
 * --------
 *	02/11/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_p_wT(double w_kgkg, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ads_T_pw:
 * ----------------
 *
 * Calculates equilibrium temperature in K depending on equilibrium pressure p
 * in Pa, equilibrium loading w in kg/kg, and specified working pair.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *		Name of calculation approach for liquid density.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium temperature in K.
 *
 * Remarks:
 * --------
 *	Function uses equation of states implemented for calculation of vapor
 *	pressure, saturated liquid density, derivative of vapor pressure wrt.
 *	temperature, and derivative of saturated liquid density wrt. temperature
 *	when isotherm of working pair is based on volumetric approach (e.g.
 *	Dubinin theory).
 *
 * History:
 * --------
 *	02/11/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_T_pw(double p_Pa, double w_kgkg,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ads_dw_dp_pT:
 * --------------------
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. pressure in kg/kg/Pa.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_dw_dp_pT(double p_Pa, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ads_dw_dT_pT:
 * --------------------
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. temperature in kg/kg/K.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_dw_dT_pT(double p_Pa, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ads_dp_dw_wT:
 * --------------------
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. loading in Pakg/kg.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_dp_dw_wT(double w_kgkg, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ads_dp_dT_wT:
 * --------------------
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. temperature in Pa/K.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_dp_dT_wT(double w_kgkg, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ads_piStar_pyxgTM:
 * -------------------------
 *
 * Calculates reduced spreading pressure piStar in kg/mol depending on
 * equilibrium pressure p in Pa, molar fraction of refrigerant in vapor phase
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
 *		Total pressure of vapor phase in Pa.
 *	double y_molmol:
 *		Molar fraction of refrigerant in vapor phase in mol/mol.
 *	double x_molmol:
 *		Molar fraction of refrigerant in adsorbed phase in mol/mol.
 *	double gamma:
 *		Activity coefficient of refrigerant in adsorbed phase.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double M_kgmol:
 *		Molar mass of refrigerant M in kg/mol.
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Reduced spreading pressure in kg/mol.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_piStar_pyxgTM(double p_total_Pa,
	double y_molmol, double x_molmol, double gamma, double T_K, double M_kgmol,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ads_sur_w_pT:
 * --------------------
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium loading in kg/kg.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach.
 *
 * History:
 * --------
 *	03/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_sur_w_pT(double p_Pa, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ads_sur_p_wT:
 * --------------------
 *
 * Calculates equilibrium pressure p in Pa depending on equilibrium temperature
 * T in K, equilibrium loading w in kg/kg, equilibrium temperature T in K, and
 * specified working pair.
 *
 * Parameters:
 * -----------
 * 	double w_kgkg:
 *		Equilibrium loading in kg/kg.
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *		Name of calculation approach for liquid density.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure in Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach.
 *
 * History:
 * --------
 *	03/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_sur_p_wT(double w_kgkg, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ads_sur_T_pw:
 * --------------------
 *
 * Calculates equilibrium temperature in K depending on equilibrium pressure p
 * in Pa, equilibrium loading w in kg/kg, and specified working pair.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *		Name of calculation approach for liquid density.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium temperature in K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach.
 *
 * History:
 * --------
 *	03/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_sur_T_pw(double p_Pa, double w_kgkg,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ads_sur_dw_dp_pT:
 * ------------------------
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. pressure in kg/kg/Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach.
 *
 * History:
 * --------
 *	03/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_sur_dw_dp_pT(double p_Pa, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ads_sur_dw_dT_pT:
 * ------------------------
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. temperature in kg/kg/K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach.
 *
 * History:
 * --------
 *	03/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_sur_dw_dT_pT(double p_Pa, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ads_sur_dp_dw_wT:
 * ------------------------
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. loading in Pakg/kg.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach.
 *
 * History:
 * --------
 *	03/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_sur_dp_dw_wT(double w_kgkg, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ads_sur_dp_dT_wT:
 * ------------------------
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. temperature in Pa/K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach.
 *
 * History:
 * --------
 *	03/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_sur_dp_dT_wT(double w_kgkg, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ads_sur_piStar_pyxgTM:
 * -----------------------------
 *
 * Calculates reduced spreading pressure piStar in kg/mol depending on
 * equilibrium pressure p in Pa, molar fraction of refrigerant in vapor phase
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
 *		Total pressure of vapor phase in Pa.
 *	double y_molmol:
 *		Molar fraction of refrigerant in vapor phase in mol/mol.
 *	double x_molmol:
 *		Molar fraction of refrigerant in adsorbed phase in mol/mol.
 *	double gamma:
 *		Activity coefficient of refrigerant in adsorbed phase.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double M_kgmol:
 *		Molar mass of refrigerant M in kg/mol.
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Reduced spreading pressure in kg/mol.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach.
 *
 * History:
 * --------
 *	03/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_sur_piStar_pyxgTM(double p_total_Pa,
	double y_molmol, double x_molmol, double gamma, double T_K, double M_kgmol,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ads_sur_w_pTpsat:
 * ------------------------
 *
 * Calculates equilibrium loading w in kg/kg depending on equilibrium pressure
 * p in Pa, equilibrium temperature T in K, and saturation pressure p_sat in Pa.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation vapor pressure in Pa.
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium loading in kg/kg.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach using the saturated vapor pressure as additional argument.
 *
 * History:
 * --------
 *	03/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_sur_w_pTpsat(double p_Pa, double T_K,
	double p_sat_Pa, const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ads_sur_p_wTpsat:
 * ------------------------
 *
 * Calculates equilibrium pressure p in Pa depending on equilibrium loading w
 * in kg/kg, equilibrium temperature T in K, and saturation pressure p_sat in
 * Pa.
 *
 * Parameters:
 * -----------
 * 	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation vapor pressure in Pa.
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *		Name of calculation approach for liquid density.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure in Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach using the saturated vapor pressure as additional argument.
 *
 * History:
 * --------
 *	03/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_sur_p_wTpsat(double w_kgkg, double T_K,
	double p_sat_Pa, const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ads_sur_T_pwpsat:
 * ------------------------
 *
 * Calculates equilibrium temperature in K depending on equilibrium pressure p
 * in Pa, equilibrium loading w in kg/kg, and saturation pressure p_sat in Pa.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double p_sat_Pa:
 *		Saturation vapor pressure in Pa.
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *		Name of calculation approach for liquid density.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium temperature in K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach using the saturated vapor pressure as additional argument.
 *	Uses internal equation of states to calculate vapor pressure and derivative
 *	of vapor pressure wrt. temperature. If equation of state is not implemented
 *	for refrigerant, function returns -1 and throws warning. If user want to use
 *	his own equation of state, this function cannot be used and determination of
 *	root to calculate T must be implemented by user.
 *
 * History:
 * --------
 *	03/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_sur_T_pwpsat(double p_Pa, double w_kgkg,
	double p_sat_Pa, const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ads_sur_dw_dp_pTpsat:
 * ----------------------------
 *
 * Calculates derivative of equilibrium loading w with respect to pressure
 * p in kg/kg/Pa depending on equilibrium pressure p in Pa, equilibrium
 * temperature T in K, and saturation pressure p_sat in Pa.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation vapor pressure in Pa.
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. pressure in kg/kg/Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach using the saturated vapor pressure as additional argument.
 *
 * History:
 * --------
 *	03/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_sur_dw_dp_pTpsat(double p_Pa, double T_K,
	double p_sat_Pa, const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ads_sur_dw_dT_pTpsat:
 * ----------------------------
 *
 * Calculates derivative of equilibrium loading w with respect to temperature
 * T in kg/kg/K depending on equilibrium pressure p in Pa, equilibrium
 * temperature T in K, saturation pressure p_sat in Pa, and derivative of
 * saturation pressure wrt. temperature dp_sat_dT in Pa/K.
 *
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation vapor pressure in Pa.
 * 	double dp_sat_dT_PaK:
 *		Derivative of saturation pressure wrt. temperature in Pa/K.
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. temperature in kg/kg/K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach using the saturated vapor pressure as additional argument.
 *
 * History:
 * --------
 *	03/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_sur_dw_dT_pTpsat(double p_Pa, double T_K,
	double p_sat_Pa, double dp_sat_dT_PaK, const char *path_db,
	const char *wp_as, const char *wp_st, const char *wp_rf, const char *wp_iso,
	int no_iso, const char *rf_psat, int no_p_sat, const char *rf_rhol,
	int no_rhol);


/*
 * direct_ads_sur_dp_dw_wTpsat:
 * ----------------------------
 *
 * Calculates derivative of equilibrium pressure p with respect to loading
 * w in kgPa/kg depending on equilibrium loading w in kg/kg, equilibrium
 * temperature T in K, and saturation pressure p_sat in Pa.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation vapor pressure in Pa.
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. loading in Pakg/kg.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach using the saturated vapor pressure as additional argument.
 *
 * History:
 * --------
 *	03/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_sur_dp_dw_wTpsat(double w_kgkg, double T_K,
	double p_sat_Pa, const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ads_sur_dp_dT_wTpsat:
 * ----------------------------
 *
 * Calculates derivative of equilibrium pressure p with respect to temperature
 * T in kg/kg/K depending on equilibrium loading w in kg/kg, equilibrium
 * temperature T in K, saturation pressure p_sat in Pa, and derivative of
 * saturation pressure wrt. temperature dp_sat_dT in Pa/K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation vapor pressure in Pa.
 * 	double dp_sat_dT_PaK:
 *		Derivative of saturation pressure wrt. temperature in Pa/K.
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. temperature in Pa/K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach using the saturated vapor pressure as additional argument.
 *
 * History:
 * --------
 *	03/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_sur_dp_dT_wTpsat(double w_kgkg, double T_K,
	double p_sat_Pa, double dp_sat_dT_PaK, const char *path_db,
	const char *wp_as, const char *wp_st, const char *wp_rf, const char *wp_iso,
	int no_iso, const char *rf_psat, int no_p_sat, const char *rf_rhol,
	int no_rhol);


/*
 * direct_ads_sur_piStar_pyxgTpsatM:
 * ---------------------------------
 *
 * Calculates reduced spreading pressure piStar in kg/mol depending on
 * equilibrium pressure p in Pa, molar fraction of refrigerant in vapor phase
 * in mol/mol, molar fraction of refrigerant in adsorbed phase in mol/mol,
 * activity coefficient of refrigerant in adsorbed phase, equilibrium
 * temperature T in K, saturation pressure p_sat in Pa, and molar mass of
 * refrigerant M in kg/mol. The reduced spreading pressure is defined as
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
 *		Total pressure of vapor phase in Pa.
 *	double y_molmol:
 *		Molar fraction of refrigerant in vapor phase in mol/mol.
 *	double x_molmol:
 *		Molar fraction of refrigerant in adsorbed phase in mol/mol.
 *	double gamma:
 *		Activity coefficient of refrigerant in adsorbed phase.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double p_sat_Pa:
 *		Saturation vapor pressure in Pa.
 *	double M_kgmol:
 *		Molar mass of refrigerant M in kg/mol.
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Reduced spreading pressure in kg/mol.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the surface
 *	approach using the saturated vapor pressure as additional argument.
 *
 * History:
 * --------
 *	03/30/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_sur_piStar_pyxgTpsatM(double p_total_Pa,
	double y_molmol, double x_molmol, double gamma, double T_K, double p_sat_Pa,
	double M_kgmol, const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ads_vol_W_ARho:
 * ----------------------
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium volumetric loading W in m³/kg.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *	Saturated liquid density of adsorpt is required to calculate volumetric
 *	loading from characteristic curve in m3/kg when coefficients would directly
 *	return loading w in kg/kg. Therefore, function checks unit of coefficients
 *	by checking isotherm coefficients.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_vol_W_ARho(double A_Jmol, double rho_l_kgm3,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ads_vol_A_WRho:
 * ----------------------
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium adsorption potential A in J/mol.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *	Saturated liquid density of adsorpt is required to calculate volumetric
 *	loading from characteristic curve in m3/kg when coefficients would directly
 *	return loading w in kg/kg.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_vol_A_WRho(double W_m3kg, double rho_l_kgm3,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ads_vol_w_pTpsatRho:
 * ---------------------------
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium loading in kg/kg.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_vol_w_pTpsatRho(double p_Pa, double T_K,
	double p_sat_Pa, double rho_kgm3, const char *path_db, const char *wp_as,
	const char *wp_st, const char *wp_rf, const char *wp_iso, int no_iso,
	const char *rf_psat, int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ads_vol_p_wTpsatRho:
 * ---------------------------
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure in Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_vol_p_wTpsatRho(double w_kgkg, double T_K,
	double p_sat_Pa, double rho_kgm3, const char *path_db, const char *wp_as,
	const char *wp_st, const char *wp_rf, const char *wp_iso, int no_iso,
	const char *rf_psat, int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ads_vol_T_pwpsatRho:
 * ---------------------------
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium temperature in K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *	Uses internal equation of states to calculate vapor pressure, saturated
 *	liquid density of adsorpt, derivative of vapor pressure wrt. temperature
 *	and derivative of saturated liquid density of adsorpt wrt. temperature. If
 *	equation of states are not implemented for refrigerant, function returns -1
 *  and throws warning. If user want to use his own equation of states, this
 *  function cannot be used and determination of root to calculate T must
 *  be implemented by user.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_vol_T_pwpsatRho(double p_Pa, double w_kgkg,
	double p_sat_Pa, double rho_kgm3, const char *path_db, const char *wp_as,
	const char *wp_st, const char *wp_rf, const char *wp_iso, int no_iso,
	const char *rf_psat, int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ads_vol_dW_dA_ARho:
 * --------------------------
 *
 * Calculates derivative of equilibrium volumetric loading dW_dA in m³mol/kg/J
 * depending on equilibrium adsorption potential A in J/mol and saturated liquid
 * density of adsorpt rho_l in kg/m3.
 *
 * Parameters:
 * -----------
 * 	double A_Jmol:
 *		Equilibrium adsorption potential A in J/mol.
 * 	double rho_l_kgm3:
 *		Saturated liquid density of adsorpt rho_l in kg/m3.
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of volumetric loading wrt. adsorption potential in
 *		m³mol/kg/J.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *	Saturated liquid density of adsorpt is required to calculate volumetric
 *	loading from characteristic curve in m3/kg when coefficients would directly
 *	return loading w in kg/kg.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_vol_dW_dA_ARho(double A_Jmol, double rho_l_kgm3,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ads_vol_dA_dW_WRho:
 * --------------------------
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of adsorption potential wrt. volumetric loading in
 *		kgJ/mol/m³.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *	Saturated liquid density of adsorpt is required to calculate volumetric
 *	loading from characteristic curve in m3/kg when coefficients would directly
 *	return loading w in kg/kg.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_vol_dA_dW_WRho(double W_m3kg, double rho_l_kgm3,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ads_vol_dw_dp_pTpsatRho:
 * -------------------------------
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. pressure in kg/kg/Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_vol_dw_dp_pTpsatRho(double p_Pa, double T_K,
	double p_sat_Pa, double rho_kgm3, const char *path_db, const char *wp_as,
	const char *wp_st, const char *wp_rf, const char *wp_iso, int no_iso,
	const char *rf_psat, int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ads_vol_dw_dT_pTpsatRho:
 * -------------------------------
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. temperature in kg/kg/K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_vol_dw_dT_pTpsatRho(double p_Pa, double T_K,
	double p_sat_Pa, double rho_kgm3, double dp_sat_dT_PaK,
	double drho_dT_kgm3K, const char *path_db, const char *wp_as,
	const char *wp_st, const char *wp_rf, const char *wp_iso, int no_iso,
	const char *rf_psat, int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ads_vol_dp_dw_wTpsatRho:
 * -------------------------------
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. loading in Pakg/kg.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_vol_dp_dw_wTpsatRho(double w_kgkg, double T_K,
	double p_sat_Pa, double rho_kgm3, const char *path_db, const char *wp_as,
	const char *wp_st, const char *wp_rf, const char *wp_iso, int no_iso,
	const char *rf_psat, int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ads_vol_dp_dT_wTpsatRho:
 * -------------------------------
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. temperature in Pa/K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_vol_dp_dT_wTpsatRho(double w_kgkg, double T_K,
	double p_sat_Pa, double rho_kgm3, double dp_sat_dT_PaK,
	double drho_dT_kgm3K, const char *path_db, const char *wp_as,
	const char *wp_st, const char *wp_rf, const char *wp_iso, int no_iso,
	const char *rf_psat, int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ads_vol_piStar_pyxgTpsatRhoM:
 * ------------------------------------
 *
 * Calculates reduced spreading pressure in kg/mol depending on equilibrium
 * pressure p in Pa, molar fraction of refrigerant in vapor phase in mol/mol,
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
 *	double rho_kgm3:
 *		Density of adsorpt in kg/m³.
 *	double M_kgmol:
 *		Molar mass of refrigerant M in kg/mol.
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Reduced spreading pressure in kg/mol.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the volumetric
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ads_vol_piStar_pyxgTpsatRhoM(double p_total_Pa,
	double y_molmol, double x_molmol, double gamma, double T_K, double p_sat_Pa,
	double rho_kgm3, double M_kgmol, const char *path_db, const char *wp_as,
	const char *wp_st, const char *wp_rf, const char *wp_iso, int no_iso,
	const char *rf_psat, int no_p_sat, const char *rf_rhol, int no_rhol);


/////////////////////////////////////////////////////////////////////////////
// Definition of function prototypes regarding absorption working wo struct//
/////////////////////////////////////////////////////////////////////////////
/*
 * direct_abs_con_X_pT:
 * --------------------
 *
 * Calculates equilibrium concentration X in kg/kg depending on equilibrium
 * pressure p in Pa and equilibrium temperature T in K.
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium concentration in kg/kg.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the conventional
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_abs_con_X_pT_workingPair(double p_Pa, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_abs_con_p_XT:
 * --------------------
 *
 * Calculates equilibrium pressure p in Pa depending on equilibrium
 * concentration X in kg/kg and equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 *	double X_kgkg:
 *		Equilibrium concentration in kg/kg.
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium pressure in Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the conventional
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_abs_con_p_XT_workingPair(double X_kgkg, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_abs_con_T_pX:
 * --------------------
 *
 * Calculates equilibrium temperature in K depending on equilibrium pressure p
 * in Pa and equilibrium concentration X in kg/kg.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double X_kgkg:
 *		Equilibrium concentration in kg/kg.
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium temperature in K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the conventional
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_abs_con_T_pX_workingPair(double p_Pa, double X_kgkg,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_abs_con_dX_dp_pT:
 * ------------------------
 *
 * Calculates derivative of equilibrium concentration X with respect to pressure
 * p in kg/kg/Pa depending on equilibrium pressure p in Pa and equilibrium
 * temperature T in K.
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium concentration wrt. pressure in kg/kg/Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the conventional
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_abs_con_dX_dp_pT_workingPair(double p_Pa, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_abs_con_dX_dT_pT:
 * ------------------------
 *
 * Calculates derivative of equilibrium concentration X with respect to
 * temperature T in kg/kg/K depending on equilibrium pressure p in Pa and
 * equilibrium temperature T in K.
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium concentration wrt. temperature in kg/kg/K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the conventional
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_abs_con_dX_dT_pT_workingPair(double p_Pa, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_abs_con_dp_dX_XT:
 * ------------------------
 *
 * Calculates derivative of equilibrium pressure p with respect to concentration
 * w in kgPa/kg depending on equilibrium concentration X in kg/kg and
 * equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 *	double X_kgkg:
 *		Equilibrium concentration in kg/kg.
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. concentration in Pakg/kg.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the conventional
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_abs_con_dp_dX_XT_workingPair(double X_kgkg, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_abs_con_dp_dT_XT:
 * ------------------------
 *
 * Calculates derivative of equilibrium pressure p with respect to temperature
 * T in kg/kg/K depending on equilibrium concentration X in kg/kg and
 * equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 *	double X_kgkg:
 *		Equilibrium concentration in kg/kg.
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. temperature in Pa/K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on the conventional
 *	approach.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_abs_con_dp_dT_XT_workingPair(double X_kgkg, double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_abs_act_g_Txv1v2:
 * ------------------------
 *
 * Calculates activity coefficient of first component depending on temperature
 * T_K in K, mole fraction in liquid phase x_molmol in mol/mol, molar volume of
 * first component in m³/mol, and molar volume of second component in m³/mol.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double x_molmol:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
 *	double v1_m3mol:
 *		Equilibrium molar volume of first component in m³/mol.
 *	double v2_m3mol:
 *		Equilibrium molar volume of second component in m³/mol.
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Activity coefficient of first component.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on activity
 *	coefficients.
 *	Molar volumes may are not required and ignored. When molar volumes are
 * 	required, function uses molar volumes stored in JSON file when input
 *	v1_m3mol or v2_m3mol is -1. Otherwise, function uses molar volumes given by
 *	inputs.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_abs_act_g_Txv1v2(double T_K, double x_molmol,
	double v1_m3mol, double v2_m3mol, const char *path_db, const char *wp_as,
	const char *wp_st, const char *wp_rf, const char *wp_iso, int no_iso,
	const char *rf_psat, int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_abs_act_p_Txv1v2psat:
 * ----------------------------
 *
 * Calculates equilibrium pressure p_Pa in Pa of first component depending on
 * temperature T_K in K, mole fraction in liquid phase x_molmol in mol/mol,
 * molar volume of first component in m³/mol, molar volume of second component
 * in m³/mol,and saturation pressure of first component p_sat_Pa in Pa.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double x_molmol:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
 *	double v1_m3mol:
 *		Equilibrium molar volume of first component in m³/mol.
 *	double v2_m3mol:
 *		Equilibrium molar volume of second component in m³/mol.
 *	double p_sat_Pa:
 *		Saturation pressure of first component in Pa.
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure p_Pa in Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on activity
 *	coefficients.
 *	Molar volumes may are not required and ignored. When molar volumes are
 * 	required, function uses molar volumes stored in JSON file when input
 *	v1_m3mol or v2_m3mol is -1. Otherwise, function uses molar volumes given by
 *	inputs.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_abs_act_p_Txv1v2psat(double T_K, double x_molmol,
	double v1_m3mol, double v2_m3mol, double p_sat_Pa, const char *path_db,
	const char *wp_as, const char *wp_st, const char *wp_rf, const char *wp_iso,
	int no_iso, const char *rf_psat, int no_p_sat, const char *rf_rhol,
	int no_rhol);


/*
 * direct_abs_act_x_pTv1v2psat:
 * ----------------------------
 *
 * Calculates mole fraction of first component in liquid phase x_molmol in
 * mol/mol depending on equilibrium pressure p_Pa in Pa of first component,
 * temperature T_K in K, molar volume of first component in m³/mol, molar volume
 * of second component in m³/mol and saturation pressure of first component
 * p_sat_Pa in Pa
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure p_Pa in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double v1_m3mol:
 *		Equilibrium molar volume of first component in m³/mol.
 *	double v2_m3mol:
 *		Equilibrium molar volume of second component in m³/mol.
 *	double p_sat_Pa:
 *		Saturation pressure of first component in Pa.
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on activity
 *	coefficients.
 *	Molar volumes may are not required and ignored. When molar volumes are
 * 	required, function uses molar volumes stored in JSON file when input
 *	v1_m3mol or v2_m3mol is -1. Otherwise, function uses molar volumes given by
 *	inputs.
 *
 * History:
 * --------
 *	30/03/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_abs_act_x_pTv1v2psat(double p_Pa, double T_K,
	double v1_m3mol, double v2_m3mol, double p_sat_Pa, const char *path_db,
	const char *wp_as, const char *wp_st, const char *wp_rf, const char *wp_iso,
	int no_iso, const char *rf_psat, int no_p_sat, const char *rf_rhol,
	int no_rhol);


/*
 * direct_abs_act_p_Txv1v2:
 * ------------------------
 *
 * Calculates equilibrium pressure p_Pa in Pa of first component depending on
 * temperature T_K in K, mole fraction in liquid phase x_molmol in mol/mol,
 * molar volume of first component in m³/mol and molar volume of second
 * component in m³/mol.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double x_molmol:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
 *	double v1_m3mol:
 *		Equilibrium molar volume of first component in m³/mol.
 *	double v2_m3mol:
 *		Equilibrium molar volume of second component in m³/mol.
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure p_Pa in Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on activity
 *	coefficients.
 *	Molar volumes may are not required and ignored. When molar volumes are
 * 	required, function uses molar volumes stored in JSON file when input
 *	v1_m3mol or v2_m3mol is -1. Otherwise, function uses molar volumes given by
 *	inputs.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_abs_act_p_Txv1v2(double T_K, double x_molmol,
	double v1_m3mol, double v2_m3mol, const char *path_db, const char *wp_as,
	const char *wp_st, const char *wp_rf, const char *wp_iso, int no_iso,
	const char *rf_psat, int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_abs_act_x_pTv1v2:
 * ------------------------
 *
 * Calculates mole fraction of first component in liquid phase x_molmol in
 * mol/mol depending on equilibrium pressure p_Pa in Pa of first component,
 * temperature T_K in K, molar volume of first component in m³/mol and molar
 * volume of second component in m³/mol.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure p_Pa in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double v1_m3mol:
 *		Equilibrium molar volume of first component in m³/mol.
 *	double v2_m3mol:
 *		Equilibrium molar volume of second component in m³/mol.
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on activity
 *	coefficients.
 *	Molar volumes may are not required and ignored. When molar volumes are
 * 	required, function uses molar volumes stored in JSON file when input
 *	v1_m3mol or v2_m3mol is -1. Otherwise, function uses molar volumes given by
 *	inputs.
 *
 * History:
 * --------
 *	30/03/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_abs_act_x_pTv1v2(double p_Pa, double T_K,
	double v1_m3mol, double v2_m3mol, const char *path_db, const char *wp_as,
	const char *wp_st, const char *wp_rf, const char *wp_iso, int no_iso,
	const char *rf_psat, int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_abs_mix_x_pT:
 * --------------------
 *
 * Calculates equilibrium liquid mole fraction of first component in mol/mol
 * depending on pressure in Pa and temperature T_K in K.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure p_Pa in Pa.
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium mole fraction in of first component liquid phase in mol/mol.
 *	double *ret_y_1_molmol:
 *		Equilibrium mole fraction of first component.
 *	double *ret_y_2_molmol:
 *		Equilibrium mole fraction of second component.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on mixing rules.
 *
 * History:
 * --------
 *	04/09/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_abs_mix_x_pT(double *ret_y_1_molmol,
	double *ret_y_2_molmol, double p_Pa, double T_K, const char *path_db,
	const char *wp_as, const char *wp_st, const char *wp_rf, const char *wp_iso,
	int no_iso, const char *rf_psat, int no_p_sat, const char *rf_rhol,
	int no_rhol);


/*
 * direct_abs_mix_p_Tx:
 * --------------------
 *
 * Calculates equilibrium pressure p_Pa in Pa depending on temperature T_K in K
 * and mole fraction in liquid phase x_molmol in mol/mol.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double x_1_molmol:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure p_Pa in Pa.
 *	double *ret_y_1_molmol:
 *		Equilibrium mole fraction of first component.
 *	double *ret_y_2_molmol:
 *		Equilibrium mole fraction of second component.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on mixing rules.
 *
 * History:
 * --------
 *	04/09/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_abs_mix_p_Tx(double *ret_y_1_molmol,
	double *ret_y_2_molmol, double T_K, double x_1_molmol, const char *path_db,
	const char *wp_as, const char *wp_st, const char *wp_rf, const char *wp_iso,
	int no_iso, const char *rf_psat, int no_p_sat, const char *rf_rhol,
	int no_rhol);


/*
 * direct_abs_mix_T_px:
 * --------------------
 *
 * Calculates equilibrium temperature T_K in K depending on pressure p_Pa in Pa
 * and mole fraction in liquid phase x_molmol in mol/mol.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure p_Pa in Pa.
 *	double x_1_molmol:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
 *	double *ret_y_1_molmol:
 *		Equilibrium mole fraction of first component.
 *	double *ret_y_2_molmol:
 *		Equilibrium mole fraction of second component.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on mixing rules.
 *	Uses Newton-Raphson method for calculating equilibrium temperature.
 *
 * History:
 * --------
 *	04/09/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_abs_mix_T_px(double *ret_y_1_molmol,
	double *ret_y_2_molmol, double p_Pa, double x_1_molmol, const char *path_db,
	const char *wp_as, const char *wp_st, const char *wp_rf, const char *wp_iso,
	int no_iso, const char *rf_psat, int no_p_sat, const char *rf_rhol,
	int no_rhol);


/*
 * direct_abs_mix_dp_dx_Tx:
 * ------------------------
 *
 * Calculates derivative of equilibrium pressure with respect to liquid mole
 * fraction in Pa depending on temperature T_K in K and mole fraction in liquid
 * phase x_molmol in mol/mol.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double x_1_molmol:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt liquid mole fraction in Pa.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on mixing rules.
 *	Derivative is calculated numerically by the symmetric derivative using
 *  h = 0.00000001 mol/mol as small change.
 *
 * History:
 * --------
 *	04/09/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_abs_mix_dp_dx_Tx(double T_K, double x_1_molmol,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_abs_mix_dp_dT_Tx:
 * ------------------------
 *
 * Calculates derivative of equilibrium pressure with respect to temperature in
 * Pa/K depending on temperature T_K in K and mole fraction in liquid phase
 * x_molmol in mol/mol.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double x_1_molmol:
 *		Equilibrium mole fraction in liquid phase in mol/mol.
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt temperature in Pa/K.
 *
 * Remarks:
 * --------
 *	This function is only valid for isotherm models based on mixing rules.
 *	Derivative is calculated numerically by the symmetric derivative using
 *  h = 0.0001 K as small change.
 *
 * History:
 * --------
 *	04/09/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_abs_mix_dp_dT_Tx(double T_K, double x_1_molmol,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/////////////////////////////////////////////////////////////////////////////
// Definition of function prototypes regarding refrigerant working w struct//
/////////////////////////////////////////////////////////////////////////////
/*
 * direct_ref_p_sat_T:
 * -------------------
 *
 * Calculates saturation pressure in Pa depending on equilibrium temperature T
 * in K.
 *
 * Parameters:
 * -----------
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Saturation pressure in Pa.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ref_p_sat_T(double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ref_dp_sat_dT_T:
 * -----------------------
 *
 * Calculates derivative of saturation pressure in Pa/K wrt to temperature
 * depending on equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of saturation pressure wrt temperature in Pa/K.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ref_dp_sat_dT_T(double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ref_rho_l_T:
 * -------------------
 *
 * Calculates saturated liquid density in kg/m³ depending on equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Saturated liquid density in kg/m³.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ref_rho_l_T(double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ref_drho_l_dT_T:
 * -----------------------
 *
 * Calculates derivative of saturated liquid density wrt temperature in kg/m³/K
 * depending on equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of saturated liquid density wrt temperature in kg/m³/K.
 *
 * History:
 * --------
 *	02/14/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ref_drho_l_dT_T(double T_K,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);


/*
 * direct_ref_T_sat_p:
 * -------------------
 *
 * Calculates saturation temperature in K depending on equilibrium pressure p
 * in Pa.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Saturation pressure in Pa.
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
 *	int no_iso:
 *		ID of isotherm (i.e. when more than one isotherm is available)
 * 	const char *rf_psat:
 *		Name of calculation approach for vapor pressure.
 *	int no_p_sat:
 *		ID of vapor pressure equation (i.e. when more than one equation is
 *		available)
 * 	const char *rf_rhol:
 *		Name of calculation approach for liquid density.
 *	int no_rhol:
 *		ID of liquid density equation (i.e. when more than one equation is
 *		available)
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium temperature in K.
 *
 * History:
 * --------
 *	30/03/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
DLL_API double direct_ref_p_sat_T_workingPair(double p_Pa,
	const char *path_db, const char *wp_as, const char *wp_st,
	const char *wp_rf, const char *wp_iso, int no_iso, const char *rf_psat,
	int no_p_sat, const char *rf_rhol, int no_rhol);

#endif