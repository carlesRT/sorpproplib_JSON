//////////////////////
// json_interface.c //
//////////////////////
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json_interface.h"


/////////////////////////////
// Definition of constants //
/////////////////////////////
#ifndef BUF
#define BUF 256
#endif


/////////////////////////////
// Definition of functions //
/////////////////////////////
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
 *	Currently, function uses subfunction "fread()" that does not work on all
 *	platforms. Thus, "fread()" should be replaced by platform-independent
 *	functions as fscanf().
 *
 * History:
 * --------
 *	10/18/2019, by Mirko Engelpracht:
 *		First implementation.
 *
 */
char *json_read_file(const char *path) {
	// Check length of user input "path" to avoid buffer overflow
	//
	if (strlen(path) > BUF) {
		// Stop execution of function and return error message
		//
		printf("Error: File-name is to long! Cannot find file!\n");
		return NULL;
	}

	// Read JSON-file and save its content to data
	//
	FILE *fp;
	char *data;
	errno_t err;
	
	if ((err = fopen_s(&fp, path, "rb")) != 0) {
		// Cannot open JSON-file
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("Cannot open file JSON-file.");
		return NULL;
			  
	} else if (fseek(fp, 0, SEEK_END) != 0) {
		// Cannot set JSON-file position
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nCannot set JSON-file position to end of file.");
		fclose(fp);
		return NULL;

	} else {
		// Read JSON-file and save its content to data
		//
		long size = ftell(fp);

		if (size > 0 && (data = (char *) malloc(size+1)) != NULL) {
			// Set position indicator to beginning of JSON-file
			//
			fseek(fp, 0, SEEK_SET);

			if (fread(data, 1, size, fp) != (size_t) size) {
				// Cannot read content of JSON-file
				//
				printf("\n\n###########\n# Warning #\n###########");
				printf("\nCannot read content of JSON-file.");
				free(data);
				fclose(fp);
				return NULL;

			} else {
				// Add end-of-line character
				//
				data[size] = '\0';
				fclose(fp);
				return data;
				
			}

		} else {
			// Cannot allocate memory for data
			//
			printf("\n\n###########\n# Warning #\n###########");
			printf("\nCannot allocate memory for content of JSON-file.");
			fclose(fp);
			return NULL;
		}
	}
}


/*
 * json_parse_file:
 * ----------------
 *
 * Parses content of JSON-file and creates mallo-ed tree of JSON-structs that
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
cJSON *json_parse_file(const char *content) {
	// Parse content of JSON-file
	//
	cJSON*retJson = cJSON_Parse(content);
	
    if (retJson == NULL) {
		// Not enough memory available for tree of JSON-structs
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nCannot allocate memory for tree of JSON-structs.");
		
        const char *error_ptr = cJSON_GetErrorPtr();		
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n\n", error_ptr);
        }
		
		return NULL;
    }
	
	// Return tree of JSON-structs
	//
	return retJson;
}


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
	const char *wp_rf, int *wp_tp, cJSON *json) {
	// Search all entries (i.e. rows) of tree of JSON-structs for working pair
	//	
    cJSON *json_workingPair = NULL;
	
    cJSON_ArrayForEach(json_workingPair, json) {
		// Performing operation for each element of JSON-struct json and save
		// current element of json into json_workingPair
		//
		// Read general information and equations of current element (i.e.
		// working pair)
		//
        cJSON *json_workingPair_info = 
			cJSON_GetObjectItemCaseSensitive(json_workingPair, "k");
        cJSON *json_workingPair_equation = 
			cJSON_GetObjectItemCaseSensitive(json_workingPair, "v");
		
		// Check general information of current element (i.e. working pair) for
		// the following:
		// 1.) Name of sorbent (i.e. wp_as)
		// 2.) Name of sub-type of sorbent (i.e. wp_st)
		// 3.) Name of refrigerant (i.e. wp_rf)
		//
		cJSON *json_workingPair_as = 
			cJSON_GetObjectItemCaseSensitive(json_workingPair_info, "_as_");
		cJSON *json_workingPair_st = 
			cJSON_GetObjectItemCaseSensitive(json_workingPair_info, "_st_");
		cJSON *json_workingPair_rf = 
			cJSON_GetObjectItemCaseSensitive(json_workingPair_info, "_rf_");
		cJSON *json_workingPair_tp = 
			cJSON_GetObjectItemCaseSensitive(json_workingPair_info, "_tp_");
		
		// Compare current working pair to working pair defined by input to
		// identify correct element of JSON-struct json and thus to return
		// correct JSON-struct containing equations of the selected working pair
		//
		// First, compare name of sorbent
		//
		if (cJSON_IsString(json_workingPair_as) && 
			(json_workingPair_as->valuestring != NULL)) {
			if (strcmp(wp_as, json_workingPair_as->valuestring) == 0) {				
				
				// Second, compare sub-type of sorbent
				//
				if (cJSON_IsString(json_workingPair_st) &&
					(json_workingPair_st->valuestring != NULL)) {					
					if (strcmp(wp_st, json_workingPair_st->valuestring) == 0) {						
					
						// Third, compare refrigerant
						//
						if (cJSON_IsString(json_workingPair_rf) &&
							(json_workingPair_rf->valuestring != NULL)) {					
							if (strcmp(wp_rf, json_workingPair_rf->valuestring) == 0) {	
								// Working pair defined by input is identified
								// First, set equation type: 1 == ads, 2 == abs, 
								// 3 == refrig, -1 == else
								//		
								if (cJSON_IsString(json_workingPair_tp) &&
									(json_workingPair_tp->valuestring != NULL)) {
										if (strcmp("ads", 
											json_workingPair_tp->valuestring) == 0) {	
											*wp_tp =  1;
										
										} else if (strcmp("abs", 
											json_workingPair_tp->valuestring) == 0) {	
											*wp_tp =  2;
										
										} else if (strcmp("refrig", 
											json_workingPair_tp->valuestring) == 0) {	
											*wp_tp =  3;
										
										} else {
											*wp_tp =  -1;
											
										}
								}
								// Second, return cJSON-struct containing
								// equation parameters
								//
								return cJSON_GetObjectItemCaseSensitive(json_workingPair_equation, "_ep_");
								
							}
						}	
					}
				}
			}
		}	
	}

	// Working pair defined by input does not exist in JSON-struct json
	//
	printf("\n\n###########\n# Warning #\n###########");
	printf("\nCannot find working pair that is defined by input in JSON-file.");
	return NULL;

}


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
 *		Numer of equation (i.e. when more than one equation is available)
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
double *json_search_parameters(const char *equation, int no_equ, cJSON *json) { 
	// Get correct equation
	//
    cJSON *json_equation = cJSON_GetObjectItemCaseSensitive(json, equation);
	
    if (cJSON_IsArray(json_equation)) {
		// Equation type exists and therefore correct ID is searched next
		//		
		cJSON *json_equation_ID = NULL;
		int ID = 0;
		
		cJSON_ArrayForEach(json_equation_ID, json_equation) {
			// Check for correct ID:
			// If ID is correct, proceed with extracting coefficients
			// Otherwise, get next item until ID is correct
			//
			ID++;
			if (ID == no_equ) {				
				// Search workingpair for equation type
				//	
				if (cJSON_IsObject(json_equation_ID)) {
					// Equation type exists and parameters are searched next
					//		
					cJSON *json_equation_parameters_single = NULL;
					cJSON *json_equation_parameters = 
						cJSON_GetObjectItemCaseSensitive(json_equation_ID, 
						"_p_");
						
					// Allocate memory to save coefficients of equation
					//
					int no_coef = cJSON_GetArraySize(json_equation_parameters);
					double *coefficients = (double *) malloc(no_coef * 
						sizeof(double));
					
					if (coefficients == NULL) {
						// Not enough memory available for coefficients
						//
						printf("\n\n###########\n# Warning #\n###########");
						printf("\nCannot allocate memory for coefficients of equation \" %s \".",
							equation);
						return NULL;
						
					}
					
					// Read coefficients from parameter-struct element for 
					// element
					//
					int tmp_counter = 0;
					
					cJSON_ArrayForEach(json_equation_parameters_single,
						json_equation_parameters) {
						// Save parameters to allocated array
						//			
						cJSON *json_equation_parameters_single_values = 
							cJSON_GetObjectItemCaseSensitive(
								json_equation_parameters, 
								json_equation_parameters_single->string);
								
						coefficients[tmp_counter] =
							json_equation_parameters_single_values->valuedouble;
						tmp_counter++;
										
					}	
					
					// Return coefficients
					//
					return coefficients;
					
				} else {
					// Equation does not saved as object
					//
					printf("\n\n###########\n# Warning #\n###########");
					printf("\nEquation \" %s \" is not saved as object.",
						equation);
					return NULL;

				}		
				
			}
			
		}
		// Equation ID does not exist for selected working pair
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nEquation ID \" %i \" does not exist for selected working pair.",
			no_equ);
		return NULL;

	} else {
		// Equation is not saved as array
		//
		printf("\n\n###########\n# Warning #\n###########");
		printf("\nEquation \" %s \"  is not saved as array or does not exist.",
			equation);
		return NULL;

	}

}
