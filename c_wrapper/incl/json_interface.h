//////////////////////
// json_interface.h //
//////////////////////
#ifndef json_interface_h
#define json_interface_h


///////////////////////
// Include libraries //
///////////////////////
#include "cJSON.h"


///////////////////////////////////////
// Definition of function prototypes //
///////////////////////////////////////
/*
 * json_read_file:
 * ---------------
 *
 * Reads content of JSON-file and returns malloc-ed string containing content of
 * JSON file. Thus, the user must free the returned content after usage.
 *
 * Parameters:
 * -----------
 * 	const char *path:
 *		Read-only pointer to char that indicates path of JSON-file.
 *
 * Returns:
 * --------
 *	char *:
 *		Malloc-ed string that contains whole content of JSON-file.
 *
 * Remarks:
 * --------
 *	Currently, function uses sub-function "fread()" that does not work on all
 *	platforms. Thus, "fread()" should be replaced by platform-independent
 *	functions as fscanf().
 *
 * History:
 * --------
 *	10/18/2019, by Mirko Engelpracht:
 *		First implementation.
 *
 */
char *json_read_file(const char *path);


/*
 * json_parse_file:
 * ----------------
 *
 * Parses content of JSON-file and creates malloc-ed tree of JSON-structs that
 * represents content of JSON-file.
 *
 * Parameters:
 * -----------
 * 	const char *content:
 *		Read-only pointer to string that contains content of JSON-file.
 *
 * Returns:
 * --------
 *	cJSON *json:
 *		Malloc-ed tree of JSON-structs that represent content of JSON-file.
 *
 * History:
 * --------
 *	10/18/2019, by Mirko Engelpracht:
 *		First implementation.
 *
 */
cJSON *json_parse_file(const char *content);


/*
 * json_search_equation:
 * ---------------------
 *
 * Searches tree of JSON-structs for a specific working pair defined by user
 * input. For the working pair, function searches for sub-tree of JSON-structs
 * describing the equations of the working pair.
 *
 * Parameters:
 * -----------
 * 	const char *wp_as:
 *		Name of sorbent.
 * 	const char *wp_st:
 *		Name of sub-type of sorbent.
 * 	const char *wp_rf:
 *		Name of refrigerant.
 * 	int *wp_tp:
 *		Type of equation.
 *	cJSON *json:
 *		Tree of JSON-structs that represent content of JSON-file.
 *
 * Returns:
 * --------
 *	cJSON *json:
 *		Tree of JSON-structs that represent content of equations of selected
 *		working pair.
 *
 * History:
 * --------
 *	10/18/2019, by Mirko Engelpracht:
 *		First implementation.
 *
 */
cJSON *json_search_equation(const char *wp_as, const char *wp_st,
	const char *wp_rf, int *wp_tp, cJSON *json);


/*
 * json_search_parameters:
 * -----------------------
 *
 * Searches tree of JSON-structs that contains equations of a specific working
 * pair for coefficients of an equation that is defined by user input.
 *
 * Parameters:
 * -----------
 * 	const char *equation:
 *		Name of equation.
 *	int no_equ:
 *		ID of equation (i.e. when more than one equation is available)
 *	cJSON *json:
 *		Tree of JSON-structs that represent equations of a working pair.
 *
 * Returns:
 * --------
 *	double *equ_par:
 *		Malloc-ed array containing coefficients of selected working pair.
 *
 * History:
 * --------
 *	10/18/2019, by Mirko Engelpracht:
 *		First implementation.
 *
 */
double *json_search_parameters(const char *equation, int no_equ, cJSON *json);

#endif
