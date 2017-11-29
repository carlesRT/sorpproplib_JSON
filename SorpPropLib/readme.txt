Changes to SorpPropLib:

	The input equation parameters, equation names and literature citings are no longer hardcoded.  All of these values can be read in by using CSV files with a known format or by JSON files in the SorpPropLib format.
	
	The CSV format is as follows:
	
		Encoding:  The CSV file must be UTF8 encoded.  This can be done using Notepad by opening the file and saving it as UTF8.
	
		Header row:  first three columns are skipped,
					 the last column is the literature citing
					 the remaining columns are the parameter names.
					 
			NOTE:  These parameter names must match the parameter names (lower case)
					 hardcoded in the equation header files.  Some do not presently match.
					 
	The JSON format is an internal object format and will be written on a single line for compactness.
	You can use JSONLint to check and format the JSON: https://jsonlint.com/
	The formatted JSON looks like:
	
	{
	"k": {								// This first section is the database key:
		"_as_": "IL",					// 		adsorbant
		"_rf_": "2-propanol",			// 		refrigerant
		"_st_": "[EMIM][(CF3SO2)2N]"	// 		subtype
	},
	"v": {
		"_ep_": {						// equation parameters (NOTE the unicode encoding (\u00f6) in the literature citations below):
			"nrtl-fixeddg": {
				"_c_": "D\u00f6ker M. and J. Gmehling Measurement and prediction of vapor\u2013liquid equilibria of ternary systems containing ionic liquids. Fluid phase equilibria 2005. 227(2): p. 255-266.",
				"_e_": "NRTL-fixedDg",
				"_p_": {
					"alpha-12": 0.47,
					"g-12": 4090.0,
					"g-21": -5084.6
				}
			},
			"wilson": {
				"_c_": "D\u00f6ker M. and J. Gmehling Measurement and prediction of vapor\u2013liquid equilibria of ternary systems containing ionic liquids. Fluid phase equilibria 2005. 227(2): p. 255-266.",
				"_e_": "Wilson",
				"_p_": {
					"a12": 1189.3,
					"a21": 16736.0
				}
			}
		},								// This last section is the referigerant-sorbant pair data which is currently a copy of the database key
		"_r_": "2-propanol",			// 		referigerant
		"_s_": "IL",					// 		adsorbant
		"_t_": "[EMIM][(CF3SO2)2N]"		//		subtype
	}
	
Changes to SorpPropBatch:
	