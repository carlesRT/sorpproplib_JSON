/////////////////////////
// structDefinitions.c //
/////////////////////////
#ifndef structDefinitions_c
#define structDefinitions_c


////////////////////////////
// Definition of typedefs //
////////////////////////////
/*
 * These prototypes are used to create the Adsorption-struct in a more cleaner
 * way. Thus, function pointers can be expressed by type definition. The numbers
 * indicate the following:
 *
 *	1) Number of inputs of type double
 *	2) Number of inputs of type double*
 *	3) Number of inputs of type void*
 *
 */
typedef double (*genFunc_1_1_0)(double, double[]);
typedef double (*genFunc_2_1_0)(double, double, double[]);
typedef double (*genFunc_3_1_0)(double, double, double, double[]);
typedef double (*genFunc_4_1_0)(double, double, double, double, double[]);
typedef double (*genFunc_5_1_0)(double, double, double, double, double,
	double[]);
typedef double (*genFunc_6_1_0)(double, double, double, double, double, double, 
	double[]);
typedef double (*genFunc_2_3_2)(double, double, double[], double[], double[], 
	void*, void*);
	

////////////////////////
// Definition structs //
////////////////////////
/*
 * Adsorption:
 * -----------
 *
 * Contains function pointers for functions of a specific isotherm type.
 * Function pointers are set during initialisation of Adsorption-struct.
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
 *		equilibrium pressure p in Pa, molar fraction of refrigerant in vapour 
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
 *		equilibrium pressure p in Pa, molar fraction of refrigerant in vapour 
 *		phase in mol/mol, molar fraction of refrigerant in adsorbed phase in 
 *		mol/mol, activity coefficient of refrigerant in adsorbed phase, 
 *		equilibrium temperature T in K, molar mass of refrigerant M in 
 *		kg/mol and coefficients of isotherm type isotherm_par.
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
 *		Returns derivative of adsorption potential dA_dW wr. volumetric loading
 *		in Jkg/mol/m3 depending on volumetric loading W in m3/kg, saturated 
 *		liquiddensity rho_l in kg/m3, and coefficients of isotherm type 
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
 * 		equilibrium pressure p in Pa, molar fraction of refrigerant in vapour 
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
 *
 */
struct Adsorption {
	// General information of isotherm
	//
	const char *isotherm_type;
	
	// Pointers for isotherm functions that are always defined
	//
    genFunc_2_3_2 w_pT;
    genFunc_2_3_2 p_wT;
    genFunc_2_3_2 T_pw;
	
    genFunc_2_3_2 dw_dp_pT;
    genFunc_2_3_2 dw_dT_pT;
    genFunc_2_3_2 dp_dw_wT;
    genFunc_2_3_2 dp_dT_wT;
			
    double (*piStar_pyxgTM)(double, double, double, double, double, double, 
		double[], double[], double[], void*, void*);
	
	// Pointers for isotherm functions that are only defined for isotherm types
	// based on the surface approach (e.g. Langmuir, Toth, Sips, ...)
	//
    genFunc_2_1_0 sur_w_pT;
    genFunc_2_1_0 sur_p_wT;
    genFunc_2_1_0 sur_T_pw;
	
    genFunc_2_1_0 sur_dw_dp_pT;
    genFunc_2_1_0 sur_dw_dT_pT;
    genFunc_2_1_0 sur_dp_dw_wT;
    genFunc_2_1_0 sur_dp_dT_wT;	
	
	double (*sur_piStar_pyxgTM)(double p_Pa, double y_molmol, double x_molmol, 
		double gamma, double T_K, double M_kgmol, double isotherm_par[]);
	
	// Pointers for isotherm functions that are only defined for isotherm types
	// based on the volumetric approach (e.g. Dubinin theory)
	//
    genFunc_2_1_0 vol_W_ARho;
    genFunc_2_1_0 vol_A_WRho;
    genFunc_2_1_0 vol_dW_dA_ARho;
    genFunc_2_1_0 vol_dA_dW_WRho;
	
	genFunc_4_1_0 vol_w_pTpsatRho;
	genFunc_4_1_0 vol_p_wTpsatRho;	
    double (*vol_T_pwpsatRho)(double, double,
		genFunc_1_1_0, genFunc_1_1_0, genFunc_1_1_0, genFunc_1_1_0,
		double[], double[], double[]);
		
	genFunc_4_1_0 vol_dw_dp_pTpsatRho;
	genFunc_6_1_0 vol_dw_dT_pTpsatRho;
	genFunc_4_1_0 vol_dp_dw_wTpsatRho;
	genFunc_6_1_0 vol_dp_dT_wTpsatRho;
			
	double (*vol_piStar_pyxgTpsatRhoM)(double, double, double, double, double, 
		double, double, double, double[]);		
};


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
 * 	function act_g_Txv1v2:
 *		Returns activity coefficient of first component depending on temperature 
 * 		T_K in K, mole fraction in liquid phase x_molmol in mol/mol, molar
 *  	volume of first component in m³/mol, and molar volume of second 
 *		component in m³/mol.
 * 	function act_p_Txgv1v2psat:
 *		Returns equilibrium pressure p_Pa in Pa of first component depending on 
 * 		temperature T_K in K, mole fraction in liquid phase x_molmol in mol/mol, 
 * 		function pointer for activity coefficient of first component, molar 
 *		volume of first component in m³/mol, molar volume of second component in
 *		m³/mol, and saturation pressure of first component p_sat_Pa in Pa.
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
struct Absorption {
	// General information of isotherm
	//
	const char *isotherm_type;
	
	// Pointers for isotherm functions that are always defined
	//
	
	
	// Pointers for isotherm functions that are only defined for isotherm types
	// that are conventional (e.g. Duehring, ...)
	//
    genFunc_2_1_0 con_X_pT;
    genFunc_2_1_0 con_p_XT;
    genFunc_2_1_0 con_T_pX;
	
    genFunc_2_1_0 con_dX_dp_pT;
    genFunc_2_1_0 con_dX_dT_pT;
    genFunc_2_1_0 con_dp_dX_XT;
    genFunc_2_1_0 con_dp_dT_XT;	
	
	// Pointers for isotherm functions that are only defined for isotherm types
	// based on activity coefficients (e.g. Wilson, NRTL, UNIQUAC, ...)
	//
	genFunc_2_1_0 act_g_Tx;
	genFunc_4_1_0 act_g_Txv1v2;
	
	genFunc_3_1_0 act_p_Txpsat;
	genFunc_5_1_0 act_p_Txv1v2psat;
    double (*act_p_Txgpsat)(double, double, genFunc_2_1_0, double, double[]);
			
	// Pointers for isotherm functions that are only defined for isotherm types
	// based on mixing rules (e.g. 1PVDW, ...)
	//
	genFunc_3_1_0 mix_p_Tvx;	
};


/*
 * Refrigerant:
 * ------------
 *
 * Contains function pointers for functions that calculate fluid properties of
 * a refrigerant. Included functions are listed and described in attributed
 * section.
 *
 * Attributes:
 * -----------
 * 	function psat_T:
 *		Returns vapour pressure in Pa depending on temperature T in K and
 *		coefficients for refrigerant refrigerant_par.
 * 	function dpsat_dT:
 *		Returns derivative of vapour pressure wrt. temperature in Pa/K depending
 *		on temperature T in K and coefficients for refrigerant refrigerant_par.
 * 	function rho_l_T:
 *		Returns saturated liquid density in kg/m3 depending on temperature T in
 *		K and coefficients for refrigerant refrigerant_par.
 * 	function drho_l_dT:
 *		Returns derivative of saturated liquid density wrt. temperature in in
 *		kg/m3/K depending on temperature T in K and coefficients for refrigerant
 *		refrigerant_par.
 *
 * Remarks:
 * --------
 *	Function pointer is NULL-pointer if refrigerant function does not exist.
 *
 * History:
 * --------
 *	01/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
struct Refrigerant {	
	// Pointers for functions describing state of refrigerant
	//
    double (*psat_T)(double, double[]);
    double (*rho_l_T)(double, double[]);
    double (*dpsat_dT)(double, double[]);
    double (*drho_l_dT)(double, double[]);
};


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
struct WorkingPair {
	// General information of working pair
	//
	const char *wp_as;
	const char *wp_st;
	const char *wp_rf;
	const char *wp_iso;	
	const char *rf_psat;	
	const char *rf_rhol;	
	const char *rf_ac;	
	
	// Arraies containing coefficients of functions
	//
	double *iso_par;
	double *psat_par;	
	double *rhol_par;	
	double *ac_par;
	
	// Structs containing function pointers of isotherm and refrigerant
	// functions
	//
	struct Adsorption *adsorption;
	struct Refrigerant *refrigerant;	
};
#endif