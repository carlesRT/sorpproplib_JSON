//////////////////
// adsorption.h //
//////////////////
#ifndef adsorption_h
#define adsorption_h


////////////////////////////
// Definition of typedefs //
////////////////////////////
/*
 * Adsorption:
 * -----------
 *
 * Contains function pointers for functions of a specific isotherm type.
 * Function pointers are set during initialization of Adsorption-struct.
 *
 * General attributes:
 * -------------------
 *	const char *isotherm_type:
 *		String indicating name of isotherm.
 *
 * 	function w_pT:
 *		Returns equilibrium loading w in kg/kg depending on pressure p in Pa,
 *		temperature T in K, and coefficients of isotherm type isotherm_par.
 * 	function p_wT:
 *		Returns equilibrium pressure p in Pa depending on pressure p in Pa,
 *		temperature T in K, and coefficients of isotherm type isotherm_par.
 * 	function T_pw:
 *		Returns equilibrium temperature T in K depending on pressure p in Pa,
 *		loading w in kg/kg, and coefficients of isotherm type isotherm_par.
 *
 * 	function dw_dp_pT:
 *		Returns derivative of equilibrium loading dw_dp wrt pressure in kg/kg/Pa
 *		depending on pressure p in Pa, temperature T in K, and coefficients of
 *		isotherm type isotherm_par.
 * 	function dw_dT_pT:
 *		Returns derivative of equilibrium loading dw_dT wrt temperature in
 *		kg/kg/K depending on pressure p in Pa, temperature T in K, and
 *		coefficients of isotherm type isotherm_par.
 * 	function dp_dw_wT:
 *		Returns derivative of equilibrium pressure dp_dw wrt loading in Pakg/kg
 *		depending on loading w in kg/kg, temperature T in K, and coefficients of
 *		isotherm type isotherm_par.
 * 	function dp_dT_wT:
 *		Returns derivative of equilibrium pressure dp_dT wrt temperature in Pa/K
 *		depending on loading w in kg/kg, temperature T in K, and coefficients of
 *		isotherm type isotherm_par.
 *
 * 	function piStar_pyxgTM:
 *		Returns reduced spreading pressure piStar in kg/mol depending on
 *		equilibrium pressure p in Pa, molar fraction of refrigerant in vapor
 *		phase in mol/mol, molar fraction of refrigerant in adsorbed phase in
 *		mol/mol, activity coefficient of refrigerant in adsorbed phase,
 *		equilibrium temperature T in K, molar mass of refrigerant M in
 *		kg/mol and coefficients of isotherm type isotherm_par.
 *
 * Attributes for classical isotherms:
 * -----------------------------------
 * 	function sur_w_pT:
 *		Returns equilibrium loading w in kg/kg depending on pressure p in Pa,
 *		temperature T in K, and coefficients of isotherm type isotherm_par.
 * 	function sur_p_wT:
 *		Returns equilibrium pressure p in Pa depending on pressure p in Pa,
 *		temperature T in K, and coefficients of isotherm type isotherm_par.
 * 	function sur_T_pw:
 *		Returns equilibrium temperature T in K depending on pressure p in Pa,
 *		loading w in kg/kg, and coefficients of isotherm type isotherm_par.
 *
 * 	function sur_dw_dp_pT:
 *		Returns derivative of equilibrium loading dw_dp wrt pressure in kg/kg/Pa
 *		depending on pressure p in Pa, temperature T in K, and coefficients of
 *		isotherm type isotherm_par.
 * 	function sur_dw_dT_pT:
 *		Returns derivative of equilibrium loading dw_dT wrt temperature in
 *		kg/kg/K depending on pressure p in Pa, temperature T in K, and
 *		coefficients of isotherm type isotherm_par.
 * 	function sur_dp_dw_wT:
 *		Returns derivative of equilibrium pressure dp_dw wrt loading in Pakg/kg
 *		depending on loading w in kg/kg, temperature T in K, and coefficients of
 *		isotherm type isotherm_par.
 * 	function sur_dp_dT_wT:
 *		Returns derivative of equilibrium pressure dp_dT wrt temperature in Pa/K
 *		depending on loading w in kg/kg, temperature T in K, and coefficients of
 *		isotherm type isotherm_par.
 *
 * 	function sur_piStar_pyxgTM:
 *		Returns reduced spreading pressure piStar in kg/mol depending on
 *		equilibrium pressure p in Pa, molar fraction of refrigerant in vapor
 *		phase in mol/mol, molar fraction of refrigerant in adsorbed phase in
 *		mol/mol, activity coefficient of refrigerant in adsorbed phase,
 *		equilibrium temperature T in K, molar mass of refrigerant M in
 *		kg/mol and coefficients of isotherm type isotherm_par.
 *
 * Attributes for classical isotherms using also saturation pressure:
 * ------------------------------------------------------------------
 * 	function sur_w_pTpsat:
 *		Returns equilibrium loading w in kg/kg depending on pressure p in Pa,
 *		temperature T in K, saturation pressure p_sat in K, and coefficients of
 *		isotherm type isotherm_par.
 * 	function sur_p_wTpsat:
 *		Returns equilibrium pressure p in Pa depending on pressure p in Pa,
 *		temperature T in K, saturation pressure p_sat in K, and coefficients of
 *		isotherm type isotherm_par.
 * 	function sur_T_pwpsat:
 *		Returns equilibrium temperature T in K depending on pressure p in Pa,
 *		loading w in kg/kg, function pointer for saturation pressure p_sat in
 *		Pa, function pointer for derivative of saturation pressure wrt.
 *		temperature dp_sat_dT in Pa/K, coefficients of isotherm type
 *		isotherm_par, and coefficients of saturation pressure p_sat_par.
 *
 * 	function sur_dw_dp_pTpsat:
 *		Returns derivative of equilibrium loading dw_dp wrt pressure in kg/kg/Pa
 *		depending on pressure p in Pa, temperature T in K, saturation pressure
 *		p_sat in K, and coefficients of isotherm type isotherm_par.
 * 	function sur_dw_dT_pTpsat:
 *		Returns derivative of equilibrium loading dw_dT wrt temperature in
 *		kg/kg/K depending on pressure p in Pa, temperature T in K, saturation
 *		pressure p_sat in K, derivative of saturation pressure wrt. temperature
 *		dp_sat_dT in Pa/K, and coefficients of isotherm type isotherm_par.
 * 	function sur_dp_dw_wTpsat:
 *		Returns derivative of equilibrium pressure dp_dw wrt loading in Pakg/kg
 *		depending on loading w in kg/kg, temperature T in K, saturation pressure
 *		p_sat in K, and coefficients of isotherm type isotherm_par.
 * 	function sur_dp_dT_wTpsat:
 *		Returns derivative of equilibrium pressure dp_dT wrt temperature in Pa/K
 *		depending on loading w in kg/kg, temperature T in K, saturation pressure
 *		p_sat in K, derivative of saturation pressure wrt. temperature
 *		dp_sat_dT in Pa/K, and coefficients of isotherm type isotherm_par.
 *
 * 	function sur_piStar_pyxgTpsatM:
 *		Returns reduced spreading pressure piStar in kg/mol depending on
 *		equilibrium pressure p in Pa, molar fraction of refrigerant in vapour
 *		phase in mol/mol, molar fraction of refrigerant in adsorbed phase in
 *		mol/mol, activity coefficient of refrigerant in adsorbed phase,
 *		equilibrium temperature T in K, saturation pressure p_sat in K, molar
 *		mass of refrigerant M in kg/mol and coefficients of isotherm type
 *		isotherm_par.
 *
 * Attributes for Dubinin isotherms:
 * ---------------------------------
 * 	function vol_W_ARho:
 *		Returns equilibrium volumetric loading W in m3/kg depending on
 *		adsorption potential A in J/mol, saturated liquid density rho_l in
 *		kg/m3, and coefficients of isotherm type isotherm_par.
 * 	function vol_A_WRho:
 *		Returns equilibrium adsorption potential A in J/mol depending on
 *		volumetric loading W in m3/kg, saturated liquid density rho_l in kg/m3,
 *		and coefficients of isotherm type isotherm_par.
 * 	function vol_dW_dA_ARho:
 *		Returns derivative of equilibrium volumetric loading dW_dA wrt
 *		adsorption potential in m3mol/kg/J depending on adsorption potential A
 *		in J/mol, saturated liquid density rho_l in kg/m3, and coefficients of
 *		isotherm type isotherm_par.
 * 	function vol_dA_dW_WRho:
 *		Returns derivative of adsorption potential dA_dW wrt. volumetric loading
 *		in Jkg/mol/m3 depending on volumetric loading W in m3/kg, saturated
 *		liquid density rho_l in kg/m3, and coefficients of isotherm type
 *		isotherm_par.
 *
 * 	function vol_w_pTpsatRho:
 *		Returns equilibrium loading w in kg/kg depending on pressure p in Pa,
 *		temperature T in K, saturation pressure p_Sat in Pa, density of adsorpt
 *		rho in kg/m³ and coefficients of isotherm type isotherm_par.
 * 	function vol_p_wTpsatRho:
 *		Returns equilibrium pressure p in Pa depending on loading w in kg/kg,
 *		temperature T in K, saturation pressure p_Sat in Pa, density of adsorpt
 *		rho in kg/m³ and coefficients of isotherm type isotherm_par.
 * 	function vol_T_pwpsatRho:
 *		Returns equilibrium temperature T in K depending on pressure p in Pa,
 *		loading w in kg/kg, function pointers for function of saturation
 *		pressure p_Sat in Pa, for function of saturated liquid density of
 *		adsorpt rho_l in kg/m³, for function of derivative of saturation
 *		pressure wrt. temperature dp_sat_dT in Pa/K, for function of derivative
 *		of saturated liquid density of adsorpt wrt. temperature drho_l_dT in
 *		kg/m3/K, as well as coefficients of isotherm type isotherm_par, of
 *		saturation pressure p_sat_par and of saturated liquid density rho_l_par.
 *
 * 	function vol_dw_dp_pTpsatRho:
 *		Returns derivative of equilibrium loading dw_dp wrt pressure in kg/kg/Pa
 *		depending on pressure p in Pa, temperature T in K, saturation pressure
 *		p_Sat in Pa, density of adsorpt rho in kg/m³ and coefficients of
 *		isotherm type isotherm_par.
 * 	function vol_dw_dT_pTpsatRho:
 *		Returns derivative of equilibrium loading dw_dT wrt temperature in
 *		kg/kg/K depending on pressure p in Pa, temperature T in K, saturation
 *		pressure p_Sat in Pa, density of adsorpt rho in kg/m³, derivative of
 *		saturation pressure with respect to temperature dp_sat_dT in Pa/K,
 *		derivative of density of adsorpt with respect to temperature drho_dT in
 *		kg/m³/K and coefficients of isotherm type isotherm_par.
 * 	function vol_dp_dw_wTpsatRho:
 *		Returns derivative of equilibrium pressure dp_dw wrt loading in Pakg/kg
 *		depending on loading w in kg/kg, temperature T in K, saturation pressure
 *		p_Sat in Pa, density of adsorpt rho in kg/m³ and coefficients of
 *		isotherm type isotherm_par.
 * 	function vol_dp_dT_wTpsatRho:
 *		Returns derivative of equilibrium pressure dp_dT wrt temperature in Pa/K
 *		depending on loading w in kg/kg, temperature T in K, saturation pressure
 *		p_Sat in Pa, density of adsorpt rho in kg/m³, derivative of saturation
 *		pressure with respect to temperature dp_sat_dT in Pa/K, derivative of
 *		density of adsorpt with respect to temperature drho_dT in kg/m³/K and
 *		coefficients of isotherm type isotherm_par.
 *
 * 	function vol_piStar_pyxgTpsatRhoM:
 *		Returns reduced spreading pressure piStar in kg/mol depending on
 * 		equilibrium pressure p in Pa, molar fraction of refrigerant in vapor
 *		phase in mol/mol, molar fraction of refrigerant in adsorbed phase in
 *		mol/mol,activity coefficient of refrigerant in adsorbed phase,
 *		equilibrium temperature T in K, saturation pressure p_Sat in Pa, density
 *		of adsorpt rho in kg/m³ and molar mass of refrigerant M in kg/mol.
 *
 * Remarks:
 * --------
 *	Function returns NULL when function does not exist for a specific isotherm
 *	type.
 *
 * History:
 * --------
 *	01/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *	01/13/2020, by Mirko Engelpracht:
 *		Added functions for dp_dw, dp_dT and piStar.
 *	04/08/2020, by Mirko Engelpracht:
 *		Added function pointers to Refrigerant-struct for isotherm types based
 *		on both, the surface approach using saturated vapor pressure and the
 *		volumetric approach.
 *
 */
typedef struct Adsorption Adsorption;


///////////////////////////////////////
// Definition of function prototypes //
///////////////////////////////////////
/*
 * newAdsorption:
 * --------------
 *
 * Initialization function to create a new Adsorption-struct. Therefore,
 * function checks first if enough memory available to allocate adsorption
 * struct and then set function pointers within the struct depending on the
 * isotherm type that is given as input. If memory allocation fails, functions
 * returns NULL and prints a warning.
 *
 * Parameters:
 * -----------
 * 	const char *isotherm_type:
 *		Name of isotherm type.
 *
 * Returns:
 * --------
 *	struct *Adsorption:
 *		Returns malloc-ed Adsorption-struct that contains function pointers
 *		referring to functions of a specific isotherm that is chosen by
 *		parameter isotherm_type.
 *
 * History:
 * --------
 *	01/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
Adsorption *newAdsorption(const char *isotherm_type);


/*
 * delAdsorption:
 * --------------
 *
 * Destructor function to free allocated memory of a Adsorption-struct.
 *
 * Parameters:
 * -----------
 * 	struct *Adsorption:
 *		Pointer of Adsorption-struct.
 *
 * History:
 * --------
 *	01/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
void delAdsorption(void *adsorption);


/*
 * init_surFunctions:
 * ------------------
 *
 * Sets function pointers to isotherm functions that are always defined for
 * isotherms based on the surface approach. Function is only used during
 * initialization.
 *
 * Parameters:
 * -----------
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer of isotherm
 *		functions.
 *
 * Remarks:
 * --------
 *	Due to initialization procedure of Adsorption-struct, this function is only
 *	callable when Adsorption-struct and sub-functions exist. Thus, no further
 *	checks are necessary.
 *
 * History:
 * --------
 *	01/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
void init_surFunctions(void *adsorption);


/*
 * init_surPsatFunctions:
 * ----------------------
 *
 * Sets function pointers to isotherm functions that are always defined for
 * isotherms based on the surface approach and using saturation pressure as
 * additional function argument. Function is only used during initialization.
 *
 * Parameters:
 * -----------
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer of isotherm
 *		functions.
 *
 * Remarks:
 * --------
 *	Due to initialization procedure of Adsorption-struct, this function is only
 *	callable when Adsorption-struct and sub-functions exist. Thus, no further
 *	checks are necessary.
 *
 * History:
 * --------
 *	03/19/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
void init_surPsatFunctions(void *adsorption);


/*
 * init_volFunctions:
 * ------------------
 *
 * Sets function pointers to isotherm functions that are always defined for
 * isotherms based on the volume approach. Function is only used during
 * initialization.
 *
 * Parameters:
 * -----------
 *	struct *Adsorption:
 *		Pointer to "Adsorption-struct that contains pointer of isotherm
 *		functions.
 *
 * Remarks:
 * --------
 *	Due to initialization procedure of Adsorption-struct, this function is only
 *	callable when Adsorption-struct and sub-functions exist. Thus, no further
 *	checks are necessary.
 *
 * History:
 * --------
 *	01/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
void init_volFunctions(void *adsorption);


/*
 * warning_func_par:
 * -----------------
 *
 * Auxiliary function to throw warning if some function or parameter is missing.
 *
 * Parameters:
 * -----------
 * 	const char *name_function:
 *		Name of parameter that is missing.
 * 	const char *name_ads_function:
 *		Name of adsorption function that is executed.
 *
 * Returns:
 * --------
 *	double:
 *		Returns -1 to indicate error.
 *
 * History:
 * --------
 *	08/04/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double warning_func_par(const char *name_function,
	const char *name_ads_function);


/*
 * w_pT_sur:
 * ---------
 *
 * Calculates equilibrium loading in w kg/kg depending on equilibrium pressure p
 * in Pa and equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapor pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer of isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid properties at saturated state.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium loading in kg/kg.
 *
 * Remarks:
 * --------
 *	No error handling because pointers are checked at highest level (i.e.
 *	functions for workingPair-Struct).
 *	Wrapper function is required to calculate loading depending on pressure and
 *	temperature when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for surface approach, fluid properties at saturated state
 *	are not required and thus ignored.
 *
 * History:
 * --------
 *	01/08/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double w_pT_sur(double p_Pa, double T_K, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant);


/*
 * w_pT_surPsat:
 * -------------
 *
 * Calculates equilibrium loading in w kg/kg depending on equilibrium pressure p
 * in Pa and equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapor pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer of isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid properties at saturated state.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium loading in kg/kg.
 *
 * Remarks:
 * --------
 *	No error handling because pointers are checked at highest level (i.e.
 *	functions for workingPair-Struct).
 *	Wrapper function is required to calculate loading depending on pressure and
 *	temperature when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for surface approach using saturation pressure, only
 *	saturation pressure is needed.
 *
 * History:
 * --------
 *	03/20/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double w_pT_surPsat(double p_Pa, double T_K, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant);


/*
 * w_pT_vol:
 * ---------
 *
 * Calculates equilibrium loading w in kg/kg depending on equilibrium pressure p
 * in Pa and equilibrium temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapor pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer of isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid properties at saturated state.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium loading in kg/kg.
 *
 * Remarks:
 * --------
 *	No error handling because pointers are checked at highest level (i.e.
 *	functions for workingPair-Struct).
 *	Wrapper function is required to calculate loading depending on pressure and
 *	temperature when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for volumetric approach, fluid properties at saturated state
 *	are required.
 *
 * History:
 * --------
 *	01/08/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double w_pT_vol(double p_Pa, double T_K, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant);


/*
 * p_wT_sur:
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
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapor pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer of isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid properties at saturated state.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium pressure in Pa.
 *
 * Remarks:
 * --------
 *	No error handling because pointers are checked at highest level (i.e.
 *	functions for workingPair-Struct).
 *	Wrapper function is required to calculate pressure depending on loading and
 *	temperature when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for surface approach, fluid properties at saturated state
 *	are not required and thus ignored.
 *
 * History:
 * --------
 *	01/08/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double p_wT_sur(double w_kgkg, double T_K, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant);


/*
 * p_wT_surPsat:
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
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapor pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer of isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid properties at saturated state.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium pressure in Pa.
 *
 * Remarks:
 * --------
 *	No error handling because pointers are checked at highest level (i.e.
 *	functions for workingPair-Struct).
 *	Wrapper function is required to calculate pressure depending on loading and
 *	temperature when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for surface approach using saturation pressure, only
 *	saturation pressure is needed.
 *
 * History:
 * --------
 *	03/20/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double p_wT_surPsat(double w_kgkg, double T_K, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant);


/*
 * p_wT_vol:
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
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapor pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer to isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid properties at saturated state.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium pressure in Pa.
 *
 * Remarks:
 * --------
 *	No error handling because pointers are checked at highest level (i.e.
 *	functions for workingPair-Struct).
 *	Wrapper function is required to calculate pressure depending on loading and
 *	temperature when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for volumetric approach, fluid properties at saturated state
 *	are required.
 *
 * History:
 * --------
 *	01/08/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double p_wT_vol(double w_kgkg, double T_K, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant);


/*
 * T_pw_sur:
 * ---------
 *
 * Calculates equilibrium temperature T in K depending on equilibrium pressure p
 * in Pa and equilibrium loading w in kg/kg.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapor pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer to isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid properties at saturated state.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium pressure in Pa.
 *
 * Remarks:
 * --------
 *	No error handling because pointers are checked at highest level (i.e.
 *	functions for workingPair-Struct).
 *	Wrapper function is required to calculate temperature depending on pressure
 *	and loading when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for surface approach, fluid properties at saturated state
 *	are not required and thus ignored.
 *
 * History:
 * --------
 *	01/08/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double T_pw_sur(double p_Pa, double w_kgkg, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant);


/*
 * T_pw_surPsat:
 * -------------
 *
 * Calculates equilibrium temperature T in K depending on equilibrium pressure p
 * in Pa and equilibrium loading w in kg/kg.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapor pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer to isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid properties at saturated state.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium pressure in Pa.
 *
 * Remarks:
 * --------
 *	No error handling because pointers are checked at highest level (i.e.
 *	functions for workingPair-Struct).
 *	Wrapper function is required to calculate temperature depending on pressure
 *	and loading when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for surface approach using saturation pressure, only
 *	saturation pressure is needed.
 *	Function uses equation of states implemented for calculation of vapor
 *	pressure and derivative of vapor pressure wrt. temperature.
 *
 * History:
 * --------
 *	03/20/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double T_pw_surPsat(double p_Pa, double w_kgkg, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant);


/*
 * T_pw_vol:
 * ---------
 *
 * Calculates equilibrium temperature T in K depending on equilibrium pressure p
 * in Pa and equilibrium loading w in kg/kg.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapor pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer to isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid properties at saturated state.
 *
 * Returns:
 * --------
 * 	double:
 *		Equilibrium pressure in Pa.
 *
 * Remarks:
 * --------
 *	No error handling because pointers are checked at highest level (i.e.
 *	functions for workingPair-Struct).
 *	Wrapper function is required to calculate temperature depending on pressure
 *	and loading when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for volumetric approach, fluid properties at saturated state
 *	are required.
 *	Function uses equation of states implemented for calculation of vapor
 *	pressure, saturated liquid density, derivative of vapor pressure wrt.
 *	temperature, and derivative of saturated liquid density wrt. temperature
 *	when isotherm of chosen working pair is based on volumetric approach (e.g.
 *	Dubinin theory).
 *
 * History:
 * --------
 *	01/08/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double T_pw_vol(double p_Pa, double w_kgkg, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant);


/*
 * dw_dp_pT_sur:
 * -------------
 *
 * Calculates derivative of equilibrium loading with respect to pressure
 * p in kg/kg/Pa depending on equilibrium pressure p in Pa and equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapor pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer to isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid properties at saturated state.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. pressure in kg/kg/Pa.
 *
 * Remarks:
 * --------
 *	No error handling because pointers are checked at highest level (i.e.
 *	functions for workingPair-Struct).
 *	Wrapper function is required to calculate loading depending on pressure and
 *	temperature when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for surface approach, fluid properties at saturated state
 *	are not required and thus ignored.
 *
 * History:
 * --------
 *	01/08/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double dw_dp_pT_sur(double p_Pa, double T_K, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant);


/*
 * dw_dp_pT_surPsat:
 * -----------------
 *
 * Calculates derivative of equilibrium loading with respect to pressure
 * p in kg/kg/Pa depending on equilibrium pressure p in Pa and equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapor pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer to isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid properties at saturated state.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. pressure in kg/kg/Pa.
 *
 * Remarks:
 * --------
 *	No error handling because pointers are checked at highest level (i.e.
 *	functions for workingPair-Struct).
 *	Wrapper function is required to calculate loading depending on pressure and
 *	temperature when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for surface approach using saturation pressure, only
 *	saturation pressure is needed.
 *
 * History:
 * --------
 *	03/20/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double dw_dp_pT_surPsat(double p_Pa, double T_K, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant);


/*
 * dw_dp_pT_vol:
 * -------------
 *
 * Calculates derivative of equilibrium loading with respect to pressure
 * p in kg/kg/Pa depending on equilibrium pressure p in Pa and equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapor pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer to isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid properties at saturated state.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. pressure in kg/kg/Pa.
 *
 * Remarks:
 * --------
 *	No error handling because pointers are checked at highest level (i.e.
 *	functions for workingPair-Struct).
 *	Wrapper function is required to calculate loading depending on pressure and
 *	temperature when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for volumetric approach, fluid properties at saturated state
 *	are required.
 *
 * History:
 * --------
 *	01/08/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double dw_dp_pT_vol(double p_Pa, double T_K, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant);


/*
 * dw_dT_pT_sur:
 * -------------
 *
 * Calculates derivative of equilibrium loading with respect to temperature
 * T in kg/kg/K depending on equilibrium pressure p in Pa and equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapor pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer to isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid properties at saturated state.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. temperature in kg/kg/K.
 *
 * Remarks:
 * --------
 *	No error handling because pointers are checked at highest level (i.e.
 *	functions for workingPair-Struct).
 *	Wrapper function is required to calculate loading depending on pressure and
 *	temperature when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for surface approach, fluid properties at saturated state
 *	are not required and thus ignored.
 *
 * History:
 * --------
 *	01/08/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double dw_dT_pT_sur(double p_Pa, double T_K, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant);


/*
 * dw_dT_pT_surPsat:
 * -----------------
 *
 * Calculates derivative of equilibrium loading with respect to temperature
 * T in kg/kg/K depending on equilibrium pressure p in Pa and equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapor pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer to isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid properties at saturated state.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. temperature in kg/kg/K.
 *
 * Remarks:
 * --------
 *	No error handling because pointers are checked at highest level (i.e.
 *	functions for workingPair-Struct).
 *	Wrapper function is required to calculate loading depending on pressure and
 *	temperature when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for surface approach using saturation pressure, only
 *	saturation pressure is needed.
 *	Function uses equation of states implemented for calculation of vapor
 *	pressure and derivative of vapor pressure wrt. temperature.
 *
 * History:
 * --------
 *	03/20/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double dw_dT_pT_surPsat(double p_Pa, double T_K, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant);


/*
 * dw_dT_pT_vol:
 * -------------
 *
 * Calculates derivative of equilibrium loading with respect to temperature
 * T in kg/kg/K depending on equilibrium pressure p in Pa and equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 * 	double p_Pa:
 *		Equilibrium pressure in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapor pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer to isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid properties at saturated state.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. temperature in kg/kg/K.
 *
 * Remarks:
 * --------
 *	No error handling because pointers are checked at highest level (i.e.
 *	functions for workingPair-Struct).
 *	Wrapper function is required to calculate loading depending on pressure and
 *	temperature when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for volumetric approach, fluid properties at saturated state
 *	are required.
 *
 * History:
 * --------
 *	01/08/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double dw_dT_pT_vol(double p_Pa, double T_K, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant);


/*
 * dp_dw_wT_sur:
 * -------------
 *
 * Calculates derivative of equilibrium pressure with respect to loading
 * w in kgPa/kg depending on equilibrium loading w in kg/kg and equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapor pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer to isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid properties at saturated state.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. loading in kgPa/kg.
 *
 * Remarks:
 * --------
 *	No error handling because pointers are checked at highest level (i.e.
 *	functions for workingPair-Struct).
 *	Wrapper function is required to calculate loading depending on pressure and
 *	temperature when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for volumetric approach, fluid properties at saturated state
 *	are required.
 *
 * History:
 * --------
 *	01/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double dp_dw_wT_sur(double w_kgkg, double T_K, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant);


/*
 * dp_dw_wT_surPsat:
 * -----------------
 *
 * Calculates derivative of equilibrium pressure with respect to loading
 * w in kgPa/kg depending on equilibrium loading w in kg/kg and equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapor pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer to isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid properties at saturated state.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. loading in kgPa/kg.
 *
 * Remarks:
 * --------
 *	No error handling because pointers are checked at highest level (i.e.
 *	functions for workingPair-Struct).
 *	Wrapper function is required to calculate loading depending on pressure and
 *	temperature when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for surface approach using saturation pressure, only
 *	saturation pressure is needed.
 *
 * History:
 * --------
 *	03/20/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double dp_dw_wT_surPsat(double w_kgkg, double T_K, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant);


/*
 * dp_dw_wT_vol:
 * -------------
 *
 * Calculates derivative of equilibrium pressure with respect to loading
 * w in kgPa/kg depending on equilibrium loading w in kg/kg and equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapor pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer to isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid properties at saturated state.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. loading in kgPa/kg.
 *
 * Remarks:
 * --------
 *	No error handling because pointers are checked at highest level (i.e.
 *	functions for workingPair-Struct).
 *	Wrapper function is required to calculate loading depending on pressure and
 *	temperature when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for surface approach, fluid properties at saturated state
 *	are not required and thus ignored.
 *
 * History:
 * --------
 *	01/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double dp_dw_wT_vol(double w_kgkg, double T_K, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant);


/*
 * dp_dT_wT_sur:
 * -------------
 *
 * Calculates derivative of equilibrium pressure with respect to temperature
 * T in Pa/K depending on equilibrium loading w in kg/kg and equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapor pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer to isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid properties at saturated state.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. temperature in Pa/K.
 *
 * Remarks:
 * --------
 *	No error handling because pointers are checked at highest level (i.e.
 *	functions for workingPair-Struct).
 *	Wrapper function is required to calculate loading depending on pressure and
 *	temperature when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for surface approach, fluid properties at saturated state
 *	are not required and thus ignored.
 *
 * History:
 * --------
 *	01/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double dp_dT_wT_sur(double w_kgkg, double T_K, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant);


/*
 * dp_dT_wT_surPsat:
 * -----------------
 *
 * Calculates derivative of equilibrium pressure with respect to temperature
 * T in Pa/K depending on equilibrium loading w in kg/kg and equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapor pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer to isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid properties at saturated state.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt. temperature in Pa/K.
 *
 * Remarks:
 * --------
 *	No error handling because pointers are checked at highest level (i.e.
 *	functions for workingPair-Struct).
 *	Wrapper function is required to calculate loading depending on pressure and
 *	temperature when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for surface approach using saturation pressure, only
 *	saturation pressure is needed.
 *	Function uses equation of states implemented for calculation of vapor
 *	pressure and derivative of vapor pressure wrt. temperature.
 *
 * History:
 * --------
 *	03/20/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double dp_dT_wT_surPsat(double w_kgkg, double T_K, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant);


/*
 * dp_dT_wT_vol:
 * -------------
 *
 * Calculates derivative of equilibrium pressure with respect to temperature
 * T in Pa/K depending on equilibrium loading w in kg/kg and equilibrium
 * temperature T in K.
 *
 * Parameters:
 * -----------
 *	double w_kgkg:
 *		Equilibrium loading in kg/kg.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapor pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer to isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid properties at saturated state.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium loading wrt. pressure in Pa/K.
 *
 * Remarks:
 * --------
 *	No error handling because pointers are checked at highest level (i.e.
 *	functions for workingPair-Struct).
 *	Wrapper function is required to calculate loading depending on pressure and
 *	temperature when volumetric approach (e.g. Dubinin theory) is chosen as
 *	isotherm. Here, for volumetric approach, fluid properties at saturated state
 *	are required.
 *
 * History:
 * --------
 *	01/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double dp_dT_wT_vol(double w_kgkg, double T_K, double isotherm_par[],
	double p_sat_par[], double rho_l_par[],
	void *adsorption, void *refrigerant);


/*
 * piStar_pyxgTM_sur:
 * ------------------
 *
 * Calculates reduced spreading pressure in kg/mol depending on total pressure
 * of vapor phase p_total in Pa, molar fraction of refrigerant in vapor phase
 * y in mol/mol, molar fraction of refrigerant in adsorbed phase in mol/mol,
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
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double M_kgmol:
 *		Molar mass of refrigerant M in kg/mol.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapor pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer to isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid properties at saturated state.
 *
 * Returns:
 * --------
 *	double:
 *		Reduced spreading pressure in kg/mol.
 *
 * Remarks:
 * --------
 *	No error handling because pointers are checked at highest level (i.e.
 *	functions for workingPair-Struct).
 *	Wrapper function is required to calculate reduced spreading pressure
 *	depending on pressure and temperature when volumetric approach (e.g. Dubinin
 *	theory) is chosen as isotherm. Here, for surface approach, fluid properties
 *	at saturated state are not required and thus ignored.
 *
 * History:
 * --------
 *	01/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double piStar_pyxgTM_sur(double p_total_Pa, double y_molmol, double x_molmol,
	double gamma, double T_K, double M_kgmol, double isotherm_par[],
	double p_sat_par[], double rho_l_par[], void *adsorption,
	void *refrigerant);


/*
 * piStar_pyxgTM_surPsat:
 * ----------------------
 *
 * Calculates reduced spreading pressure in kg/mol depending on total pressure
 * of vapor phase p_total in Pa, molar fraction of refrigerant in vapor phase
 * y in mol/mol, molar fraction of refrigerant in adsorbed phase in mol/mol,
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
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double M_kgmol:
 *		Molar mass of refrigerant M in kg/mol.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapor pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer to isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid properties at saturated state.
 *
 * Returns:
 * --------
 *	double:
 *		Reduced spreading pressure in kg/mol.
 *
 * Remarks:
 * --------
 *	No error handling because pointers are checked at highest level (i.e.
 *	functions for workingPair-Struct).
 *	Wrapper function is required to calculate reduced spreading pressure
 *	depending on pressure and temperature when volumetric approach (e.g. Dubinin
 *	theory) is chosen as isotherm. Here, for surface approach using saturation
 *	pressure, only saturation pressure is needed.
 *
 * History:
 * --------
 *	03/20/2020, by Mirko Engelpracht:
 *		First implementation.
 *
*/
double piStar_pyxgTM_surPsat(double p_total_Pa, double y_molmol, double x_molmol,
	double gamma, double T_K, double M_kgmol, double isotherm_par[],
	double p_sat_par[], double rho_l_par[], void *adsorption,
	void *refrigerant);


/*
 * piStar_pyxgTM_vol:
 * ------------------
 *
 * Calculates reduced spreading pressure in kg/mol depending on total pressure
 * of vapor phase p_total in Pa, molar fraction of refrigerant in vapor phase
 * y in mol/mol, molar fraction of refrigerant in adsorbed phase in mol/mol,
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
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm.
 *	double p_sat_par[]:
 *		Array of doubles that contains coefficients of vapor pressure.
 *	double rho_l_par[]:
 *		Array of doubles that contains coefficients of saturated liquid density.
 *	struct *Adsorption:
 *		Pointer to Adsorption-struct that contains pointer to isotherm
 *		functions.
 *	struct *Refrigerant:
 *		Pointer to Refrigerant-struct that contains function to calculate
 *		fluid properties at saturated state.
 *
 * Returns:
 * --------
 *	double:
 *		Reduced spreading pressure in kg/mol.
 *
 * Remarks:
 * --------
 *	No error handling because pointers are checked at highest level (i.e.
 *	functions for workingPair-Struct).
 *	Wrapper function is required to calculate reduced spreading pressure
 *	depending on pressure and temperature when volumetric approach (e.g. Dubinin
 *	theory) is chosen as isotherm. Here, for surface approach, fluid properties
 *	at saturated state are not required and thus ignored.
 *
 * History:
 * --------
 *	01/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double piStar_pyxgTM_vol(double p_total_Pa, double y_molmol, double x_molmol,
	double gamma, double T_K, double M_kgmol, double isotherm_par[],
	double p_sat_par[], double rho_l_par[], void *adsorption,
	void *refrigerant);

#endif