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
 * Function pointers are set during initialisation of Absorption-struct.
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
 * 	function act_p_Txgpsat_w_gf:
 *		Returns equilibrium pressure p_Pa in Pa of first component depending on 
 * 		temperature T_K in K, mole fraction in liquid phase x_molmol in mol/mol, 
 * 		function pointer for activity coefficient of first component, and 
 *		saturation pressure of first component p_sat_Pa in Pa.
 *
 * 	function act_g_Txv1v2:
 *		Returns activity coefficient of first component depending on temperature 
 * 		T_K in K, mole fraction in liquid phase x_molmol in mol/mol, molar
 *  	volume of first component in m³/mol, molar volume of second component
 *		in m³/mol, and pointer to Absorption-struct.
 * 	function act_p_Txgv1v2psat:
 *		Returns equilibrium pressure p_Pa in Pa of first component depending on 
 * 		temperature T_K in K, mole fraction in liquid phase x_molmol in mol/mol, 
 * 		function pointer for activity coefficient of first component, molar 
 *		olume of first component in m³/mol, molar volume of second component in
 *		m³/mol, saturation pressure of first component p_sat_Pa in Pa, and 
 *		pointer to Absorption-struct.
 * 	function act_p_Txgv1v2:
 *		Returns equilibrium pressure p_Pa in Pa of first component depending on 
 * 		temperature T_K in K, mole fraction in liquid phase x_molmol in mol/mol, 
 * 		function pointer for activity coefficient of first component, molar 
 *		olume of first component in m³/mol, molar volume of second component in
 *		m³/mol, pointer to Refrigerant-struct, and pointer to Absorption-struct.
 *
 * Attributes for isotherms based on mixing rules:
 * -----------------------------------------------
 * 	function mix_p_Tvx:
 *		Returns equilibrium pressure p_Pa in Pa of first component depending on 
 * 		temperature T_K in K, molar mixing volume v_m3mol in m³/mol, and mole 
 * 		fraction in liquid phase x_molmol in mol/mol
 *
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
 * Initialisation function to create a new Absorption-struct. Therefore,
 * function checks first if enough memory available to allocate absorption 
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
 *	struct *Absorption:
 *		Returns malloc-ed Absorption-struct that contains function pointers
 *		referring to functions of a specific isotherm that is chosen by
 *		parameter isotherm_type.
 *
 * History:
 * --------
 *	02/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
Absorption *newAbsorption(const char *isotherm_type);


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
 *	double (*gamma)(double T_K, double x_molmol, double isotherm_par[]):
 *		Function pointer for calculation approach of activity coefficient
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
 *	need molar volumes or a function pointer for the activity coefficient as 
 *	inputs. Here, neither molar volumes nor a function pointer are required.
 *
 * History:
 * --------
 *	02/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double act_p_Txgv1v2psat_wo_v(double T_K, double x_molmol,
	double (*gamma)(double, double, double[]), double v1_m3mol, double v2_m3mol, 
	double p_sat_Pa, double isotherm_par[], void *absorption);


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
 *	double (*gamma)(double T_K, double x_molmol, double isotherm_par[]):
 *		Function pointer for calculation approach of activity coefficient
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
 *	need molar volumes or a function pointer for the activity coefficient as 
 *	inputs. Here, molar volumes are required but no function pointer is 
 *	required.
 *
 * History:
 * --------
 *	02/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double act_p_Txgv1v2psat_w_v(double T_K, double x_molmol,
	double (*gamma)(double, double, double[]), double v1_m3mol, double v2_m3mol, 
	double p_sat_Pa, double isotherm_par[], void *absorption);


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
 *	double (*gamma)(double T_K, double x_molmol, double isotherm_par[]):
 *		Function pointer for calculation approach of activity coefficient
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
 *	need molar volumes or a function pointer for the activity coefficient as 
 *	inputs. Here, no molar volumes are required but a function pointer is 
 *	required.
 *
 * History:
 * --------
 *	02/13/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double act_p_Txgv1v2psat_w_gf(double T_K, double x_molmol,
	double (*gamma)(double, double, double[]), double v1_m3mol, double v2_m3mol, 
	double p_sat_Pa, double isotherm_par[], void *absorption);
	
#endif