//////////////////
// absorption.h //
//////////////////
#ifndef absorption_h
#define absorption_h


////////////////////////////
// Definition of typedefs //
////////////////////////////
/*
 * Absorption:
 * -----------
 *
 * Contains function pointers for functions of a specific isotherm type.
 * Function pointers are set during initialization of Absorption-struct.
 *
 * General attributes:
 * -------------------
 *	const char *isotherm_type:
 *		String indicating name of isotherm.
 *
 * Attributes for conventional isotherms:
 * --------------------------------------
 * 	function con_X_pT:
 *		Returns equilibrium concentration X in kg/kg depending on equilibrium
 * 		pressure p in Pa, and equilibrium temperature T in K.
 * 	function con_p_XT:
 *		Returns equilibrium pressure p in Pa depending on equilibrium
 * 		concentration X in kg/kg, and equilibrium temperature T in K.
 * 	function con_T_pX:
 *		Returns equilibrium temperature in K depending on equilibrium pressure p
 * 		in Pa, and equilibrium concentration X in kg/kg.
 *
 * 	function con_dX_dp_pT:
 *		Returns derivative of equilibrium concentration X with respect to
 * 		pressure p in kg/kg/Pa depending on equilibrium pressure p in Pa and
 * 		equilibrium temperature T in K.
 * 	function con_dX_dT_pT:
 *		Returns derivative of equilibrium concentration X with respect to
 * 		temperature T in kg/kg/K depending on equilibrium pressure p in Pa and
 * 		equilibrium temperature T in K.
 * 	function con_dp_dX_XT:
 *		Returns derivative of equilibrium pressure p with respect to
 * 		concentration X in kgPa/kg depending on equilibrium concentration X in
 * 		kg/kg and  equilibrium temperature T in K.
 * 	function con_dp_dT_XT:
 *		Returns derivative of equilibrium pressure p with respect to temperature
 * 		T in Pa/K depending on equilibrium concentration X in kg/kg and
 * 		equilibrium temperature T in K.
 *
 * Attributes for isotherms based on activity coefficients:
 * --------------------------------------------------------
 * 	function act_g_Tx_wo_v:
 *		Returns activity coefficient of first component depending on temperature
 * 		T_K in K, and mole fraction in liquid phase x_molmol in mol/mol.
 * 	function act_g_Txv1v2_w_v:
 *		Returns activity coefficient of first component depending on temperature
 * 		T_K in K, mole fraction in liquid phase x_molmol in mol/mol, molar
 *  	volume of first component in m³/mol, and molar volume of second
 *		component in m³/mol.
 * 	function act_p_Txpsat_wo_v:
 *		Returns equilibrium pressure p_Pa in Pa of first component depending on
 * 		temperature T_K in K, mole fraction in liquid phase x_molmol in mol/mol,
 * 		and saturation pressure of first component p_sat_Pa in Pa.
 * 	function act_p_Txv1v2psat_w_v:
 *		Returns equilibrium pressure p_Pa in Pa of first component depending on
 * 		temperature T_K in K, mole fraction in liquid phase x_molmol in mol/mol,
 * 		molar  volume of first component in m³/mol, molar volume of second
 *		component in m³/mol, and saturation pressure of first component p_sat_Pa
 *		in Pa.
 * 	function act_p_Txv1v2gpsat_w_v_gf:
 *		Returns equilibrium pressure p_Pa in Pa of first component depending on
 * 		temperature T_K in K, mole fraction in liquid phase x_molmol in mol/mol,
 * 		molar  volume of first component in m³/mol, molar volume of second
 *		component in m³/mol, function pointer for activity coefficient of first
 *		component, and saturation pressure of first component p_sat_Pa in Pa.
 * 	function act_p_Txgpsat_w_gf:
 *		Returns equilibrium pressure p_Pa in Pa of first component depending on
 * 		temperature T_K in K, mole fraction in liquid phase x_molmol in mol/mol,
 * 		function pointer for activity coefficient of first component, and
 *		saturation pressure of first component p_sat_Pa in Pa.
 * 	function act_x_pTpsat_wo_v:
 *		Returns mole fraction of first component in liquid phase x_molmol in
 * 		mol/mol depending on equilibrium pressure p_Pa in Pa of first component,
 * 		temperature T_K in K, and saturation pressure of first component
 *		p_sat_Pa in Pa.
 * 	function act_x_pTv1v2psat_w_v:
 *		Returns mole fraction of first component in liquid phase x_molmol in
 * 		mol/mol depending on equilibrium pressure p_Pa in Pa of first component,
 * 		temperature T_K in K, molar volume of first component in m³/mol, molar
 *		volume of second component in m³/mol, and saturation pressure of first
 *		component p_sat_Pa in Pa.
 * 	function act_x_pTv1v2gpsat_w_v_gf:
 *		Returns mole fraction of first component in liquid phase x_molmol in
 * 		mol/mol depending on equilibrium pressure p_Pa in Pa of first component,
 * 		temperature T_K in K, molar volume of first component in m³/mol, molar
 *		volume of second component in m³/mol, function pointer for activity
 *		coefficient of first component, and saturation pressure of first
 *		component p_sat_Pa in Pa.
 * 	function act_x_pTgpsat_w_gf:
 *		Returns mole fraction of first component in liquid phase x_molmol in
 * 		mol/mol depending on equilibrium pressure p_Pa in Pa of first component,
 * 		temperature T_K in K, function pointer for activity coefficient of first
 *		component, and saturation pressure of first component p_sat_Pa in Pa.
 *
 * 	function act_g_Txv1v2:
 *		Returns activity coefficient of first component depending on temperature
 * 		T_K in K, mole fraction in liquid phase x_molmol in mol/mol, molar
 *  	volume of first component in m³/mol, molar volume of second component
 *		in m³/mol, and pointer to Absorption-struct.
 * 	function act_p_Txgv1v2psat:
 *		Returns equilibrium pressure p_Pa in Pa of first component depending on
 * 		temperature T_K in K, mole fraction in liquid phase x_molmol in mol/mol,
 * 		,molar volume of first component in m³/mol, molar volume of second
 *		component in m³/mol, saturation pressure of first component p_sat_Pa in
 *		Pa, and pointer to Absorption-struct.
 * 	function act_x_pTgv1v2psat:
 *		Returns mole fraction of first component in liquid phase x_molmol in
 * 		mol/mol depending on equilibrium pressure p_Pa in Pa of first component,
 * 		temperature T_K in K, molar volume of first component in m³/mol, molar
 *		volume of second component in m³/mol, saturation pressure of first
 *		component p_sat_Pa in Pa, and pointer to Absorption-struct.
 *
 * Attributes for isotherms based on mixing rules:
 * -----------------------------------------------
 * 	function mix_x_pT:
 *		Returns equilibrium mole fraction in liquid phase x_molmol in mol/mol
 *		depending on pressure p_Pa in Pa and temperature T_K in K.
 * 	function mix_p_Tx:
 *		Returns equilibrium pressure p_Pa in Pa of first component depending on
 * 		temperature T_K in K and mole fraction in liquid phase x_molmol in
 *		mol/mol.
 * 	function mix_T_px:
 *		Returns equilibrium temperature in K depending on equilibrium pressure p
 * 		in Pa and equilibrium mole fraction X in mol/mol.
 *
 * 	function mix_dp_dx_Tx:
 *		Returns derivative of equilibrium pressure p with respect to mole
 * 		fraction x in Pa depending on equilibrium mole fraction x in mol/mol
 * 		and equilibrium temperature T in K.
 * 	function mix_dp_dT_Tx:
 *		Returns derivative of equilibrium pressure p with respect to temperature
 * 		T in Pa/K depending on equilibrium mole fraction x in mol/mol and
 * 		equilibrium temperature T in K.
 *
 *	function mix_pure_parameters:
 *		Returns pure component parameters of cubic equation of state.
 *	function mix_mixture_parameters:
 *		Returns mixture parameters of cubic equation of state depending on pure
 *		component parameters.
 *	function mix_gen_parameters:
 *		Returns generalized parameters of cubic equation of state.
 *	function mix_mixture_fugacity_coefficient:
 *		Returns mixture fugacity coefficient of cubic equation of state.
 *
 * Remarks:
 * --------
 *	Function returns NULL when function does not exist for a specific isotherm
 *	type.
 *
 * History:
 * --------
 *	02/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
typedef struct Absorption Absorption;


///////////////////////////////////////
// Definition of function prototypes //
///////////////////////////////////////
/*
 * newAbsorption:
 * --------------
 *
 * Initialization function to create a new Absorption-struct. Therefore,
 * function checks first if enough memory is available to allocate absorption
 * struct and then sets function pointers within the struct depending on the
 * isotherm type that is given as input. If memory allocation fails, functions
 * returns NULL and prints a warning.
 *
 * Parameters:
 * -----------
 * 	const char *isotherm_type:
 *		Name of isotherm type.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients for isotherm functions.
 *
 * Returns:
 * --------
 *	struct *Absorption:
 *		Returns malloc-ed Absorption-struct that contains function pointers
 *		referring to functions of a specific isotherm that is chosen by
 *		parameter isotherm_type.
 *
 * History:
 * --------
 *	02/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *	04/09/2020, by Mirko Engelpracht:
 *		Added functions based on mixing approach.
 *
 */
Absorption *newAbsorption(const char *isotherm_type, double isotherm_par[]);


/*
 * delAbsorption:
 * --------------
 *
 * Destructor function to free allocated memory of a Absorption-struct.
 *
 * Parameters:
 * -----------
 * 	struct *Absorption:
 *		Pointer of Absorption-struct.
 *
 * History:
 * --------
 *	02/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
void delAbsorption(void *absorption);


/*
 * warning_par_func:
 * -----------------
 *
 * Auxiliary function to throw warning if some function or parameter is missing.
 *
 * Parameters:
 * -----------
 * 	const char *name_function:
 *		Name of parameter that is missing.
 * 	const char *name_abs_function:
 *		Name of absorption function that is executed.
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
double warning_par_func(const char *name_function,
	const char *name_abs_function);


/*
 * act_g_Txv1v2_wo_v:
 * ------------------
 *
 * Calculates activity coefficient of first component depending on temperature
 * T_K in K and mole fraction in liquid phase x_molmol in mol/mol.
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
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm equation.
 *	struct *Absorption:
 *		Pointer to Absorption-struct that contains pointer of isotherm
 *		functions.
 *
 * Returns:
 * --------
 *	double:
 *		Activity coefficient of first component.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate activity coefficients that may
 *	need molar volumes as input. Here, molar volumes are not required.
 *
 * History:
 * --------
 *	02/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double act_g_Txv1v2_wo_v(double T_K, double x_molmol,
	double v1_m3mol, double v2_m3mol, double isotherm_par[], void *absorption);


/*
 * act_g_Txv1v2_w_v:
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
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of isotherm equation.
 *	struct *Absorption:
 *		Pointer to Absorption-struct that contains pointer of isotherm
 *		functions.
 *
 * Returns:
 * --------
 *	double:
 *		Activity coefficient of first component.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate activity coefficients that may
 *	need molar volumes as input. Here, molar volumes may are required.
 *
 * History:
 * --------
 *	02/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double act_g_Txv1v2_w_v(double T_K, double x_molmol,
	double v1_m3mol, double v2_m3mol, double isotherm_par[], void *absorption);


/*
 * act_p_Txgv1v2psat_wo_v:
 * -----------------------
 *
 * Calculates equilibrium pressure p_Pa in Pa of first component depending on
 * temperature T_K in K, mole fraction in liquid phase x_molmol in mol/mol, and
 * saturation pressure of first component p_sat_Pa in Pa.
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
 *		Saturation pressure on Pa.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Wilson equation.
 *	struct *Absorption:
 *		Pointer to Absorption-struct that contains pointer of isotherm
 *		functions.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure in Pa.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate activity coefficients that may
 *	need molar volumes as inputs. Here, molar volumes are not required
 *
 * History:
 * --------
 *	02/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double act_p_Txgv1v2psat_wo_v(double T_K, double x_molmol,
	double v1_m3mol, double v2_m3mol, double p_sat_Pa, double isotherm_par[],
	void *absorption);


/*
 * act_p_Txgv1v2psat_w_v:
 * ----------------------
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
 *		Saturation pressure on Pa.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Wilson equation.
 *	struct *Absorption:
 *		Pointer to Absorption-struct that contains pointer of isotherm
 *		functions.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure in Pa.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate activity coefficients that may
 *	need molar volumes as inputs. Here, molar volumes are required.
 *
 * History:
 * --------
 *	02/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double act_p_Txgv1v2psat_w_v(double T_K, double x_molmol,
	double v1_m3mol, double v2_m3mol, double p_sat_Pa, double isotherm_par[],
	void *absorption);


/*
 * act_p_Txgv1v2psat_w_v_gf:
 * -------------------------
 *
 * Calculates equilibrium pressure p_Pa in Pa of first component depending on
 * temperature T_K in K, mole fraction in liquid phase x_molmol in mol/mol,
 * molar volume of first component in m³/mol, molar volume of second component,
 * saturation pressure of first component p_sat_Pa in Pa, and function pointer
 * for activity coefficient of first component.
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
 *		Saturation pressure on Pa.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Wilson equation.
 *	struct *Absorption:
 *		Pointer to Absorption-struct that contains pointer of isotherm
 *		functions.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure in Pa.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate activity coefficients that may
 *	need molar volumes. Here,  molar volumes are required. Moreover, a function
 *  pointer is given internally to calculate the activity coefficient of the
 *	first component.
 *
 * History:
 * --------
 *	03/25/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double act_p_Txgv1v2psat_w_v_gf(double T_K, double x_molmol,
	double v1_m3mol, double v2_m3mol, double p_sat_Pa, double isotherm_par[],
	void *absorption);


/*
 * act_p_Txgv1v2psat_w_gf:
 * -----------------------
 *
 * Calculates equilibrium pressure p_Pa in Pa of first component depending on
 * temperature T_K in K, mole fraction in liquid phase x_molmol in mol/mol,
 * saturation pressure of first component p_sat_Pa in Pa, and function pointer
 * for activity coefficient of first component.
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
 *		Saturation pressure on Pa.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Wilson equation.
 *	struct *Absorption:
 *		Pointer to Absorption-struct that contains pointer of isotherm
 *		functions.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium pressure in Pa.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate activity coefficients that may
 *	need molar volumes. Here,  molar volumes are not required but a function
 *  pointer is given internally to calculate the activity coefficient of the
 *	first component.
 *
 * History:
 * --------
 *	02/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double act_p_Txgv1v2psat_w_gf(double T_K, double x_molmol,
	double v1_m3mol, double v2_m3mol, double p_sat_Pa, double isotherm_par[],
	void *absorption);


/*
 * act_x_pTgv1v2psat_wo_v:
 * -----------------------
 *
 * Calculates mole fraction of first component in liquid phase x_molmol in
 * mol/mol depending on equilibrium pressure p_Pa in Pa of first component,
 * temperature T_K in K, and saturation pressure of first component p_sat_Pa in
 * Pa.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure of first component in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double v1_m3mol:
 *		Equilibrium molar volume of first component in m³/mol.
 *	double v2_m3mol:
 *		Equilibrium molar volume of second component in m³/mol.
 *	double p_sat_Pa:
 *		Saturation pressure on Pa.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Wilson equation.
 *	struct *Absorption:
 *		Pointer to Absorption-struct that contains pointer of isotherm
 *		functions.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium mole fraction of first component in liquid phase in mol/mol.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate activity coefficients that may
 *	need molar volumes as inputs. Here, molar volumes are not required
 *
 * History:
 * --------
 *	03/26/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double act_x_pTgv1v2psat_wo_v(double p_Pa, double T_K, double v1_m3mol,
	double v2_m3mol, double p_sat_Pa, double isotherm_par[], void *absorption);


/*
 * act_x_pTgv1v2psat_w_v:
 * ----------------------
 *
 * Calculates mole fraction of first component in liquid phase x_molmol in
 * mol/mol depending on equilibrium pressure p_Pa in Pa of first component,
 * temperature T_K in K, molar volume of first component in m³/mol, molar volume
 * of second component in m³/mol,and saturation pressure of first component
 * p_sat_Pa in Pa.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure of first component in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double v1_m3mol:
 *		Equilibrium molar volume of first component in m³/mol.
 *	double v2_m3mol:
 *		Equilibrium molar volume of second component in m³/mol.
 *	double p_sat_Pa:
 *		Saturation pressure on Pa.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Wilson equation.
 *	struct *Absorption:
 *		Pointer to Absorption-struct that contains pointer of isotherm
 *		functions.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium mole fraction of first component in liquid phase in mol/mol.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate activity coefficients that may
 *	need molar volumes as inputs. Here, molar volumes are required.
 *
 * History:
 * --------
 *	03/26/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double act_x_pTgv1v2psat_w_v(double p_Pa, double T_K, double v1_m3mol,
	double v2_m3mol, double p_sat_Pa, double isotherm_par[], void *absorption);


/*
 * act_x_pTgv1v2psat_w_v_gf:
 * -------------------------
 *
 * Calculates mole fraction of first component in liquid phase x_molmol in
 * mol/mol depending on equilibrium pressure p_Pa in Pa of first component,
 * temperature T_K in K, molar volume of first component in m³/mol, molar volume
 * of second component in m³/mol,and saturation pressure of first component
 * p_sat_Pa in Pa, and function pointer for activity coefficient of first
 * component.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure of first component in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double v1_m3mol:
 *		Equilibrium molar volume of first component in m³/mol.
 *	double v2_m3mol:
 *		Equilibrium molar volume of second component in m³/mol.
 *	double p_sat_Pa:
 *		Saturation pressure on Pa.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Wilson equation.
 *	struct *Absorption:
 *		Pointer to Absorption-struct that contains pointer of isotherm
 *		functions.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium mole fraction of first component in liquid phase in mol/mol.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate activity coefficients that may
 *	need molar volumes as inputs. Here, molar volumes are required. Moreover, a
 *  function pointer is given internally to calculate the activity coefficient
 * 	of the first component.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double act_x_pTgv1v2psat_w_v_gf(double p_Pa, double T_K, double v1_m3mol,
	double v2_m3mol, double p_sat_Pa, double isotherm_par[], void *absorption);


/*
 * act_x_pTgv1v2psat_w_gf:
 * -----------------------
 *
 * Calculates mole fraction of first component in liquid phase x_molmol in
 * mol/mol depending on equilibrium pressure p_Pa in Pa of first component,
 * temperature T_K in K, and saturation pressure of first component p_sat_Pa in
 * Pa, and function pointer for activity coefficient of first component.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Equilibrium pressure of first component in Pa.
 *	double T_K:
 *		Equilibrium temperature in K.
 *	double v1_m3mol:
 *		Equilibrium molar volume of first component in m³/mol.
 *	double v2_m3mol:
 *		Equilibrium molar volume of second component in m³/mol.
 *	double p_sat_Pa:
 *		Saturation pressure on Pa.
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of Wilson equation.
 *	struct *Absorption:
 *		Pointer to Absorption-struct that contains pointer of isotherm
 *		functions.
 *
 * Returns:
 * --------
 *	double:
 *		Equilibrium mole fraction of first component in liquid phase in mol/mol.
 *
 * Remarks:
 * --------
 *	Wrapper function is required to calculate activity coefficients that may
 *	need molar volumes as inputs. Here, molar volumes are not required. However,
 *  a function pointer is given internally to calculate the activity coefficient
 * 	of the first component.
 *
 * History:
 * --------
 *	03/27/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double act_x_pTgv1v2psat_w_gf(double p_Pa, double T_K, double v1_m3mol,
	double v2_m3mol, double p_sat_Pa, double isotherm_par[], void *absorption);


/*
 * mix_x_pT:
 * ---------
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
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of pure component equations
 *		of state and of mixing rule
 *	struct *Absorption:
 *		Pointer to Absorption-struct that contains pointer of isotherm
 *		functions.
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
 * History:
 * --------
 *	04/08/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double mix_x_pT(double *ret_y_1_molmol, double *ret_y_2_molmol, double p_Pa,
	double T_K, double isotherm_par[], void *absorption);


/*
 * mix_p_Tx:
 * ---------
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
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of pure component equations
 *		of state and of mixing rule
 *	struct *Absorption:
 *		Pointer to Absorption-struct that contains pointer of isotherm
 *		functions.
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
 * History:
 * --------
 *	04/08/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double mix_p_Tx(double *ret_y_1_molmol, double *ret_y_2_molmol, double T_K,
	double x_1_molmol, double isotherm_par[], void *absorption);


/*
 * mix_T_px:
 * ---------
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
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of pure component equations
 *		of state and of mixing rule
 *	struct *Absorption:
 *		Pointer to Absorption-struct that contains pointer of isotherm
 *		functions.
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
 *	Uses Newton-Raphson method for calculating equilibrium temperature.
 *
 * History:
 * --------
 *	04/08/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double mix_T_px(double *ret_y_1_molmol, double *ret_y_2_molmol, double p_Pa,
	double x_1_molmol, double isotherm_par[], void *absorption);


/*
 * mix_dp_dx_Tx:
 * -------------
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
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of pure component equations
 *		of state and of mixing rule
 *	struct *Absorption:
 *		Pointer to Absorption-struct that contains pointer of isotherm
 *		functions.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt liquid mole fraction in Pa.
 *
 * Remarks:
 * --------
 *	Derivative is calculated numerically by the symmetric derivative using
 *  h = 0.00000001 K as small change.
 *
 * History:
 * --------
 *	04/08/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double mix_dp_dx_Tx(double T_K, double x_1_molmol, double isotherm_par[],
	void *absorption);


/*
 * mix_dp_dT_Tx:
 * -------------
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
 *	double isotherm_par[]:
 *		Array of doubles that contains coefficients of pure component equations
 *		of state and of mixing rule
 *	struct *Absorption:
 *		Pointer to Absorption-struct that contains pointer of isotherm
 *		functions.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of equilibrium pressure wrt temperature in Pa/K.
 *
 * Remarks:
 * --------
 *	Derivative is calculated numerically by the symmetric derivative using
 *  h = 0.0001 K as small change.
 *
 * History:
 * --------
 *	04/08/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double mix_dp_dT_Tx(double T_K, double x_1_molmol, double isotherm_par[],
	void *absorption);


#endif
