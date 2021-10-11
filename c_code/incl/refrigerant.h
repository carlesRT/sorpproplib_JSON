///////////////////
// refrigerant.h //
///////////////////
#ifndef refrigerant_h
#define refrigerant_h


////////////////////////////
// Definition of typedefs //
////////////////////////////
/*
 * Refrigerant:
 * ------------
 *
 * Contains function pointers for functions that calculate fluid properties of
 * a refrigerant. Included functions are listed and described in the attributes
 * section.
 *
 * Attributes:
 * -----------
 * 	function p_sat_T:
 *		Returns vapor pressure in Pa depending on temperature T in K,
 *		coefficients for refrigerant refrigerant_par, and Refrigerant-struct.
 * 	function T_sat_p:
 *		Returns saturation temperature in K depending on pressure p in Pa,
 *		coefficients for refrigerant refrigerant_par, and Refrigerant-struct.
 * 	function dp_sat_dT:
 *		Returns derivative of vapor pressure wrt. temperature in Pa/K depending
 *		on temperature T in K, coefficients for refrigerant refrigerant_par, and
 *		Refrigerant-struct.

 * 	function rho_l_T:
 *		Returns saturated liquid density in kg/m3 depending on temperature T in
 *		K and coefficients for refrigerant refrigerant_par.
 * 	function drho_l_dT:
 *		Returns derivative of saturated liquid density wrt. temperature in in
 *		kg/m3/K depending on temperature T in K and coefficients for refrigerant
 *		refrigerant_par.
 *
 * 	function other_p_sat_T:
 *		Returns vapor pressure in Pa depending on temperature T in K and
 *		coefficients for refrigerant refrigerant_par.
 * 	function other_T_sat_p:
 *		Returns saturation temperature in K depending on pressure p in Pa and
 *		coefficients for refrigerant refrigerant_par.
 * 	function other_dp_sat_dT:
 *		Returns derivative of vapor pressure wrt. temperature in Pa/K depending
 *		on temperature T in K and coefficients for refrigerant refrigerant_par.

 * 	function cubic_eos_parameters:
 *		Calculates pure component parameters of cubic equation of state.
 * 	function cubic_gen_eos_parameters:
 *		Calculates generalized pure component parameters of cubic equation of
 *		state.
 * 	function cubic_fug_coefficient:
 *		Calculates fugacity coefficient of cubic equation of state.
 *
 * Remarks:
 * --------
 *	Function pointer is NULL-pointer if refrigerant function does not exist.
 *
 * History:
 * --------
 *	01/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *	04/07/2020, by Mirko Engelpracht:
 *		Added function pointers for cubic equations of state.
 *
 */
typedef struct Refrigerant Refrigerant;


///////////////////////////////////////
// Definition of function prototypes //
///////////////////////////////////////
/*
 * newRefrigerant:
 * ---------------
 *
 * Initialization function to create a new Refrigerant-struct. Therefore,
 * function checks first if enough memory is available to allocate Refrigerant-
 * struct and then sets function pointers within the struct depending on the
 * calculation approach that is given as input. If memory allocation fails,
 * functions returns NULL and prints a warning.
 *
 * Parameters:
 * -----------
 * 	const char *vaporPressure_approach:
 *		Calculation approach for vapor pressure.
 * 	const char *saturatedLiquidDensity_approach:
 *		Calculation approach for saturated liquid density.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for vapor refrigerant
 *		functions (i.e., required if cubic equation of state is selected).
 *
 * Returns:
 * --------
 *	struct *Refrigerant:
 *		Returns malloc-ed Refrigerant-struct that contains function pointers
 *		referring to functions of refrigerant.
 *
 * History:
 * --------
 *	01/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *	04/07/2020, by Mirko Engelpracht:
 *		Added function pointers for cubic equation of states.
 *
 */
Refrigerant *newRefrigerant(const char *vaporPressure_approach,
	const char *saturatedLiquidDensity_approach, double refrigerant_par[]);


/*
 * delRefrigerant:
 * ---------------
 *
 * Destructor function to free allocated memory of a Refrigerant-struct.
 *
 * Parameters:
 * -----------
 * 	struct *Refrigerant
 *		Pointer of Refrigerant-struct.
 *
 * History:
 * --------
 *	01/05/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
void delRefrigerant(void *refrigerant);


/*
 * refrigerant_other_p_sat:
 * ------------------------
 *
 * Calculates vapor pressure in Pa depending on saturation temperature T in K
 * if not using cubic equation of state.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Saturation temperature in K.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for refrigerant function.
 * 	struct *Refrigerant
 *		Pointer of Refrigerant-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Vapor pressure in Pa.
 *
 * Remarks:
 * --------
 *	Due to initialization procedure of Refrigerant-struct, this function is only
 *	callable when Refrigerant-struct and sub-functions exist. Thus, no further
 *	checks are necessary.
 *	This function is valid for all approaches except for cubic equations of
 *	state.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_other_p_sat(double T_K, double refrigerant_par[],
	void *refrigerant);


/*
 * refrigerant_other_T_sat:
 * ------------------------
 *
 * Calculates saturation temperature in K depending on vapor pressure in Pa
 * if not using cubic equation of state.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Vapor pressure in Pa.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for refrigerant function.
 * 	struct *Refrigerant
 *		Pointer of Refrigerant-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Saturation temperature in K.
 *
 * Remarks:
 * --------
 *	Due to initialization procedure of Refrigerant-struct, this function is only
 *	callable when Refrigerant-struct and sub-functions exist. Thus, no further
 *	checks are necessary.
 *	This function is valid for all approaches except for cubic equations of
 *	state.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_other_T_sat(double p_Pa, double refrigerant_par[],
	void *refrigerant);


/*
 * refrigerant_other_dp_sat_dT:
 * ----------------------------
 *
 * Calculates derivative of vapor pressure wrt. temperature in Pa/K depending
 * on saturation temperature T in K if not using cubic equation of state.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Saturation temperature in K.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for refrigerant function.
 * 	struct *Refrigerant
 *		Pointer of Refrigerant-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of vapor pressure wrt. temperature in Pa/K.
 *
 * Remarks:
 * --------
 *	Due to initialization procedure of Refrigerant-struct, this function is only
 *	callable when Refrigerant-struct and sub-functions exist. Thus, no further
 *	checks are necessary.
 *	This function is valid for all approaches except for cubic equations of
 *	state.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_other_dp_sat_dT(double T_K, double refrigerant_par[],
	void *refrigerant);


/*
 * refrigerant_cubic_p_sat:
 * ------------------------
 *
 * Calculates vapor pressure in Pa depending on saturation temperature T in K
 * if using cubic equation of state.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Saturation temperature in K.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for cubic equation of state.
 * 	struct *Refrigerant
 *		Pointer of Refrigerant-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Vapor pressure in Pa.
 *
 * Remarks:
 * --------
 *	Due to initialization procedure of Refrigerant-struct, this function is only
 *	callable when Refrigerant-struct and sub-functions exist. Thus, no further
 *	checks are necessary.
 *	This function is only valid for cubic equations of state.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_cubic_p_sat(double T_K, double refrigerant_par[],
	void *refrigerant);


/*
 * refrigerant_cubic_T_sat:
 * ------------------------
 *
 * Calculates saturation temperature in K depending on vapor pressure in Pa
 * if using cubic equation of state.
 *
 * Parameters:
 * -----------
 *	double p_Pa:
 *		Vapor pressure in Pa.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for cubic equation of state.
 * 	struct *Refrigerant
 *		Pointer of Refrigerant-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Saturation temperature in K.
 *
 * Remarks:
 * --------
 *	Due to initialization procedure of Refrigerant-struct, this function is only
 *	callable when Refrigerant-struct and sub-functions exist. Thus, no further
 *	checks are necessary.
 *	This function is only valid for cubic equations of state.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_cubic_T_sat(double p_Pa, double refrigerant_par[],
	void *refrigerant);


/*
 * refrigerant_cubic_dp_sat_dT:
 * ----------------------------
 *
 * Calculates derivative of vapor pressure wrt. temperature in Pa/K depending
 * on saturation temperature T in K if using cubic equation of state.
 *
 * Parameters:
 * -----------
 *	double T_K:
 *		Saturation temperature in K.
 *	double refrigerant_par[]:
 *		Array of doubles that contains coefficients for cubic equation of state.
 * 	struct *Refrigerant
 *		Pointer of Refrigerant-struct.
 *
 * Returns:
 * --------
 *	double:
 *		Derivative of vapor pressure wrt. temperature in Pa/K.
 *
 * Remarks:
 * --------
 *	Due to initialization procedure of Refrigerant-struct, this function is only
 *	callable when Refrigerant-struct and sub-functions exist. Thus, no further
 *	checks are necessary.
 *	This function is only valid for cubic equations of state.
 *
 * History:
 * --------
 *	04/07/2020, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double refrigerant_cubic_dp_sat_dT(double T_K, double refrigerant_par[],
	void *refrigerant);

#endif
