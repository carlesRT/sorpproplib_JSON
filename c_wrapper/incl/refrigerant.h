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
 * a refrigerant. Included functions are listed and described in attributed
 * section.
 *
 * Attributes:
 * -----------
 * 	function psat_T:
 *		Returns vapor pressure in Pa depending on temperature T in K and
 *		coefficients for refrigerant refrigerant_par.
 * 	function Tsat_p:
 *		Returns saturation temperature in K depending on pressure p in Pa and
 *		coefficients for refrigerant refrigerant_par.
 * 	function dpsat_dT:
 *		Returns derivative of vapor pressure wrt. temperature in Pa/K depending
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
typedef struct Refrigerant Refrigerant;


///////////////////////////////////////
// Definition of function prototypes //
///////////////////////////////////////
/*
 * newRefrigerant:
 * ---------------
 *
 * Initialization function to create a new Refrigerant-struct. Therefore,
 * function checks first if enough memory available to allocate refrigerant
 * struct and then set function pointers within the struct depending on the
 * calculation approach that is given as input. If memory allocation fails,
 * functions returns NULL and prints a warning.
 *
 * Parameters:
 * -----------
 * 	const char *vaporPressure_approach:
 *		Calculation approach for vapor pressure.
 * 	const char *saturatedLiquidDensity_approach:
 *		Calculation approach for saturated liquid density.
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
 *
 */
Refrigerant *newRefrigerant(const char *vaporPressure_approach,
	const char *saturatedLiquidDensity_approach);


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

#endif