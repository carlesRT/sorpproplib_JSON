Attribute VB_Name = "DirectFunctionCalls"
Option Explicit

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Create function wrappers to access functions from SorpPropLib-DLL '
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' direct_ads_w_pT:
' ----------------
'
' Calculates equilibrium loading w in kg/kg depending on equilibrium pressure
' p in Pa, equilibrium temperature T in K, and specified working pair.
'
' Parameters:
' -----------
' 	double p_Pa:
'		Equilibrium pressure in Pa.
'	double T_K:
'		Equilibrium temperature in K.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Equilibrium loading in kg/kg.
'
' History:
' --------
'	07/14/2021, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_w_pT Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_p_wT:
' ----------------
'
' Calculates equilibrium pressure p in Pa depending on equilibrium temperature
' T in K, equilibrium loading w in kg/kg, equilibrium temperature T in K, and
' specified working pair.
'
' Parameters:
' -----------
'  double w_kgkg:
'	  Equilibrium loading in kg/kg.
'  double T_K:
'	  Equilibrium temperature in K.
'
'  const char *path_db:
'      Path to database.
'  const char *wp_as:
'      Name of sorbent.
'  const char *wp_st:
'      Name of sub-type of sorbent.
'  const char *wp_rf:
'      Name of refrigerant.
'  const char *wp_iso:
'      Name of isotherm.
'  int no_iso:
'      ID of isotherm (i.e. when more than one isotherm is available)
'  const char *rf_psat:
'      Name of calculation approach for vapor pressure.
'  int no_p_sat:
'      ID of vapor pressure equation (i.e. when more than one equation is
'      available)
'  const char *rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      ID of liquid density equation (i.e. when more than one equation is
'      available)
'
' Returns:
' --------
'	double:
'		Equilibrium pressure in Pa.
'
' History:
' --------
'	07/14/2021, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_p_wT Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal w_kgkg As Double, ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_T_pw:
' ----------------
'
' Calculates equilibrium temperature in K depending on equilibrium pressure p
' in Pa, equilibrium loading w in kg/kg, and specified working pair.
'
' Parameters:
' -----------
'	double p_Pa:
'		Equilibrium pressure in Pa.
'	double w_kgkg:
'		Equilibrium loading in kg/kg.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'		Name of calculation approach for liquid density.
'
' Returns:
' --------
' 	double:
'		Equilibrium temperature in K.
'
' Remarks:
' --------
'	Function uses equation of states implemented for calculation of vapor
'	pressure, saturated liquid density, derivative of vapor pressure wrt.
'	temperature, and derivative of saturated liquid density wrt. temperature
'	when isotherm of working pair is based on volumetric approach (e.g.
'	Dubinin theory).
'
' History:
' --------
'	07/14/2021, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_T_pw Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal w_kgkg As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_dw_dp_pT:
' --------------------
'
' Calculates derivative of equilibrium loading w with respect to pressure
' p in kg/kg/Pa depending on equilibrium pressure p in Pa and equilibrium
' temperature T in K.
'
' Parameters:
' -----------
' 	double p_Pa:
'		Equilibrium pressure in Pa.
'	double T_K:
'		Equilibrium temperature in K.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Derivative of equilibrium loading wrt. pressure in kg/kg/Pa.
'
' History:
' --------
'	07/14/2021, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_dw_dp_pT Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_dw_dT_pT:
' --------------------
'
' Calculates derivative of equilibrium loading w with respect to temperature
' T in kg/kg/K depending on equilibrium pressure p in Pa and equilibrium
' temperature T in K.
'
' Parameters:
' -----------
' 	double p_Pa:
'		Equilibrium pressure in Pa.
'	double T_K:
'		Equilibrium temperature in K.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Derivative of equilibrium loading wrt. temperature in kg/kg/K.
'
' History:
' --------
'	07/14/2021, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_dw_dT_pT Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_dp_dw_wT:
' --------------------
'
' Calculates derivative of equilibrium pressure p with respect to loading
' w in kgPa/kg depending on equilibrium loading w in kg/kg and equilibrium
' temperature T in K.
'
' Parameters:
' -----------
'	double w_kgkg:
'		Equilibrium loading in kg/kg.
'	double T_K:
'		Equilibrium temperature in K.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Derivative of equilibrium pressure wrt. loading in Pakg/kg.
'
' History:
' --------
'	07/14/2021, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_dp_dw_wT Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal w_kgkg As Double, ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_dp_dT_wT:
' --------------------
'
' Calculates derivative of equilibrium pressure p with respect to temperature
' T in kg/kg/K depending on equilibrium loading w in kg/kg and equilibrium
' temperature T in K.
'
' Parameters:
' -----------
'	double w_kgkg:
'		Equilibrium loading in kg/kg.
'	double T_K:
'		Equilibrium temperature in K.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Derivative of equilibrium pressure wrt. temperature in Pa/K.
'
' History:
' --------
'	07/14/2021, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_dp_dT_wT Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal w_kgkg As Double, ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_piStar_pyxgTM:
' -------------------------
'
' Calculates reduced spreading pressure piStar in kg/mol depending on
' equilibrium pressure p in Pa, molar fraction of refrigerant in vapor phase
' in mol/mol, molar fraction of refrigerant in adsorbed phase in mol/mol,
' activity coefficient of refrigerant in adsorbed phase, equilibrium
' temperature T in K and molar mass of refrigerant M in kg/mol. The reduced
' spreading pressure is defined as follows:
'
' 	piStar = A' pi / (R' T' m_sorbent) = 1 / M'
'		Integral_0^p0{w(p,T) / p' dp}
'
'	where p0 = p_total*y / (gamma*x)
'
' Parameters:
' -----------
'	double p_total_Pa:
'		Total pressure of vapor phase in Pa.
'	double y_molmol:
'		Molar fraction of refrigerant in vapor phase in mol/mol.
'	double x_molmol:
'		Molar fraction of refrigerant in adsorbed phase in mol/mol.
'	double gamma:
'		Activity coefficient of refrigerant in adsorbed phase.
'	double T_K:
'		Equilibrium temperature in K.
'	double M_kgmol:
'		Molar mass of refrigerant M in kg/mol.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Reduced spreading pressure in kg/mol.
'
' History:
' --------
'	07/14/2021, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_piStar_pyxgTM Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal y_molmol As Double, ByVal x_molmol As Double, _
 ByVal gamma As Double, ByVal T_K As Double, ByVal M_kgmol As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_sur_w_pT:
' --------------------
'
' Calculates equilibrium loading w in kg/kg depending on equilibrium pressure
' p in Pa, equilibrium temperature T in K, and specified working pair.
'
' Parameters:
' -----------
' 	double p_Pa:
'		Equilibrium pressure in Pa.
'	double T_K:
'		Equilibrium temperature in K.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Equilibrium loading in kg/kg.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on the surface
'	approach.
'
' History:
' --------
'	07/14/2021, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_sur_w_pT Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_sur_p_wT:
' --------------------
'
' Calculates equilibrium pressure p in Pa depending on equilibrium temperature
' T in K, equilibrium loading w in kg/kg, equilibrium temperature T in K, and
' specified working pair.
'
' Parameters:
' -----------
'  double w_kgkg:
'	  Equilibrium loading in kg/kg.
'  double T_K:
'	  Equilibrium temperature in K.
'
'  const char *path_db:
'      Path to database.
'  const char *wp_as:
'      Name of sorbent.
'  const char *wp_st:
'      Name of sub-type of sorbent.
'  const char *wp_rf:
'      Name of refrigerant.
'  const char *wp_iso:
'      Name of isotherm.
'  int no_iso:
'      ID of isotherm (i.e. when more than one isotherm is available)
'  const char *rf_psat:
'      Name of calculation approach for vapor pressure.
'  int no_p_sat:
'      ID of vapor pressure equation (i.e. when more than one equation is
'      available)
'  const char *rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      ID of liquid density equation (i.e. when more than one equation is
'      available)
'
' Returns:
' --------
'	double:
'		Equilibrium pressure in Pa.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on the surface
'	approach.
'
' History:
' --------
'	07/14/2021, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_sur_p_wT Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal w_kgkg As Double, ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_sur_T_pw:
' --------------------
'
' Calculates equilibrium temperature in K depending on equilibrium pressure p
' in Pa, equilibrium loading w in kg/kg, and specified working pair.
'
' Parameters:
' -----------
'	double p_Pa:
'		Equilibrium pressure in Pa.
'	double w_kgkg:
'		Equilibrium loading in kg/kg.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'		Name of calculation approach for liquid density.
'
' Returns:
' --------
' 	double:
'		Equilibrium temperature in K.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on the surface
'	approach.
'	Function uses equation of states implemented for calculation of vapor
'	pressure, saturated liquid density, derivative of vapor pressure wrt.
'	temperature, and derivative of saturated liquid density wrt. temperature
'	when isotherm of working pair is based on volumetric approach (e.g.
'	Dubinin theory).
'
' History:
' --------
'	07/14/2021, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_sur_T_pw Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal w_kgkg As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_sur_dw_dp_pT:
' ------------------------
'
' Calculates derivative of equilibrium loading w with respect to pressure
' p in kg/kg/Pa depending on equilibrium pressure p in Pa and equilibrium
' temperature T in K.
'
' Parameters:
' -----------
' 	double p_Pa:
'		Equilibrium pressure in Pa.
'	double T_K:
'		Equilibrium temperature in K.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Derivative of equilibrium loading wrt. pressure in kg/kg/Pa.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on the surface
'	approach.
'
' History:
' --------
'	07/14/2021, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_sur_dw_dp_pT Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_sur_dw_dT_pT:
' ------------------------
'
' Calculates derivative of equilibrium loading w with respect to temperature
' T in kg/kg/K depending on equilibrium pressure p in Pa and equilibrium
' temperature T in K.
'
' Parameters:
' -----------
' 	double p_Pa:
'		Equilibrium pressure in Pa.
'	double T_K:
'		Equilibrium temperature in K.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Derivative of equilibrium loading wrt. temperature in kg/kg/K.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on the surface
'	approach.
'
' History:
' --------
'	07/14/2021, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_sur_dw_dT_pT Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_sur_dp_dw_wT:
' ------------------------
'
' Calculates derivative of equilibrium pressure p with respect to loading
' w in kgPa/kg depending on equilibrium loading w in kg/kg and equilibrium
' temperature T in K.
'
' Parameters:
' -----------
'	double w_kgkg:
'		Equilibrium loading in kg/kg.
'	double T_K:
'		Equilibrium temperature in K.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Derivative of equilibrium pressure wrt. loading in Pakg/kg.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on the surface
'	approach.
'
' History:
' --------
'	07/14/2021, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_sur_dp_dw_wT Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal w_kgkg As Double, ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_sur_dp_dT_wT:
' ------------------------
'
' Calculates derivative of equilibrium pressure p with respect to temperature
' T in kg/kg/K depending on equilibrium loading w in kg/kg and equilibrium
' temperature T in K.
'
' Parameters:
' -----------
'	double w_kgkg:
'		Equilibrium loading in kg/kg.
'	double T_K:
'		Equilibrium temperature in K.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Derivative of equilibrium pressure wrt. temperature in Pa/K.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on the surface
'	approach.
'
' History:
' --------
'	07/14/2021, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_sur_dp_dT_wT Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal w_kgkg As Double, ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_sur_piStar_pyxgTM:
' -----------------------------
'
' Calculates reduced spreading pressure piStar in kg/mol depending on
' equilibrium pressure p in Pa, molar fraction of refrigerant in vapor phase
' in mol/mol, molar fraction of refrigerant in adsorbed phase in mol/mol,
' activity coefficient of refrigerant in adsorbed phase, equilibrium
' temperature T in K and molar mass of refrigerant M in kg/mol. The reduced
' spreading pressure is defined as follows:
'
' 	piStar = A' pi / (R' T' m_sorbent) = 1 / M'
'		Integral_0^p0{w(p,T) / p' dp}
'
'	where p0 = p_total*y / (gamma*x)
'
' Parameters:
' -----------
'	double p_total_Pa:
'		Total pressure of vapor phase in Pa.
'	double y_molmol:
'		Molar fraction of refrigerant in vapor phase in mol/mol.
'	double x_molmol:
'		Molar fraction of refrigerant in adsorbed phase in mol/mol.
'	double gamma:
'		Activity coefficient of refrigerant in adsorbed phase.
'	double T_K:
'		Equilibrium temperature in K.
'	double M_kgmol:
'		Molar mass of refrigerant M in kg/mol.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Reduced spreading pressure in kg/mol.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on the surface
'	approach.
'
' History:
' --------
'	07/14/2021, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_sur_piStar_pyxgTM Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal y_molmol As Double, ByVal x_molmol As Double, _
 ByVal gamma As Double, ByVal T_K As Double, ByVal M_kgmol As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_sur_w_pTpsat:
' ------------------------
'
' Calculates equilibrium loading w in kg/kg depending on equilibrium pressure
' p in Pa, equilibrium temperature T in K, and saturation pressure p_sat in Pa.
'
' Parameters:
' -----------
' 	double p_Pa:
'		Equilibrium pressure in Pa.
'	double T_K:
'		Equilibrium temperature in K.
'	double p_sat_Pa:
'		Saturation vapor pressure in Pa.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Equilibrium loading in kg/kg.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on the surface
'	approach using the saturated vapor pressure as additional argument.
'
' History:
' --------
'	07/14/2021, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_sur_w_pTpsat Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal T_K As Double, ByVal p_sat_Pa As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_sur_p_wTpsat:
' ------------------------
'
' Calculates equilibrium pressure p in Pa depending on equilibrium loading w
' in kg/kg, equilibrium temperature T in K, and saturation pressure p_sat in
' Pa.
'
' Parameters:
' -----------
' 	double w_kgkg:
'		Equilibrium loading in kg/kg.
'	double T_K:
'		Equilibrium temperature in K.
'	double p_sat_Pa:
'		Saturation vapor pressure in Pa.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'		Name of calculation approach for liquid density.
'
' Returns:
' --------
'	double:
'		Equilibrium pressure in Pa.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on the surface
'	approach using the saturated vapor pressure as additional argument.
'
' History:
' --------
'	07/14/2021, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_sur_p_wTpsat Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal w_kgkg As Double, ByVal T_K As Double, ByVal p_sat_Pa As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_sur_T_pwpsat:
' ------------------------
'
' Calculates equilibrium temperature in K depending on equilibrium pressure p
' in Pa, equilibrium loading w in kg/kg, and saturation pressure p_sat in Pa.
'
' Parameters:
' -----------
'	double p_Pa:
'		Equilibrium pressure in Pa.
'	double w_kgkg:
'		Equilibrium loading in kg/kg.
'	double p_sat_Pa:
'		Saturation vapor pressure in Pa.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'		Name of calculation approach for liquid density.
'
' Returns:
' --------
' 	double:
'		Equilibrium temperature in K.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on the surface
'	approach using the saturated vapor pressure as additional argument.
'	Uses internal equation of states to calculate vapor pressure and derivative
'	of vapor pressure wrt. temperature. If equation of state is not implemented
'	for refrigerant, function returns -1 and throws warning. If user want to use
'	his own equation of state, this function cannot be used and determination of
'	root to calculate T must be implemented by user.
'
' History:
' --------
'	07/14/2021, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_sur_T_pwpsat Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal w_kgkg As Double, ByVal p_sat_Pa As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_sur_dw_dp_pTpsat:
' ----------------------------
'
' Calculates derivative of equilibrium loading w with respect to pressure
' p in kg/kg/Pa depending on equilibrium pressure p in Pa, equilibrium
' temperature T in K, and saturation pressure p_sat in Pa.
'
' Parameters:
' -----------
' 	double p_Pa:
'		Equilibrium pressure in Pa.
'	double T_K:
'		Equilibrium temperature in K.
'	double p_sat_Pa:
'		Saturation vapor pressure in Pa.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Derivative of equilibrium loading wrt. pressure in kg/kg/Pa.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on the surface
'	approach using the saturated vapor pressure as additional argument.
'
' History:
' --------
'	07/14/2021, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_sur_dw_dp_pTpsat Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal T_K As Double, ByVal p_sat_Pa As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_sur_dw_dT_pTpsat:
' ----------------------------
'
' Calculates derivative of equilibrium loading w with respect to temperature
' T in kg/kg/K depending on equilibrium pressure p in Pa, equilibrium
' temperature T in K, saturation pressure p_sat in Pa, and derivative of
' saturation pressure wrt. temperature dp_sat_dT in Pa/K.
'
'
' Parameters:
' -----------
' 	double p_Pa:
'		Equilibrium pressure in Pa.
'	double T_K:
'		Equilibrium temperature in K.
'	double p_sat_Pa:
'		Saturation vapor pressure in Pa.
' 	double dp_sat_dT_PaK:
'		Derivative of saturation pressure wrt. temperature in Pa/K.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Derivative of equilibrium loading wrt. temperature in kg/kg/K.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on the surface
'	approach using the saturated vapor pressure as additional argument.
'
' History:
' --------
'	07/14/2021, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_sur_dw_dT_pTpsat Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal T_K As Double, ByVal p_sat_Pa As Double, ByVal dp_sat_dT_PaK As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_sur_dp_dw_wTpsat:
' ----------------------------
'
' Calculates derivative of equilibrium pressure p with respect to loading
' w in kgPa/kg depending on equilibrium loading w in kg/kg, equilibrium
' temperature T in K, and saturation pressure p_sat in Pa.
'
' Parameters:
' -----------
'	double w_kgkg:
'		Equilibrium loading in kg/kg.
'	double T_K:
'		Equilibrium temperature in K.
'	double p_sat_Pa:
'		Saturation vapor pressure in Pa.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Derivative of equilibrium pressure wrt. loading in Pakg/kg.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on the surface
'	approach using the saturated vapor pressure as additional argument.
'
' History:
' --------
'	07/14/2021, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_sur_dp_dw_wTpsat Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal w_kgkg As Double, ByVal T_K As Double, ByVal p_sat_Pa As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_sur_dp_dT_wTpsat:
' ----------------------------
'
' Calculates derivative of equilibrium pressure p with respect to temperature
' T in kg/kg/K depending on equilibrium loading w in kg/kg, equilibrium
' temperature T in K, saturation pressure p_sat in Pa, and derivative of
' saturation pressure wrt. temperature dp_sat_dT in Pa/K.
'
' Parameters:
' -----------
'	double w_kgkg:
'		Equilibrium loading in kg/kg.
'	double T_K:
'		Equilibrium temperature in K.
'	double p_sat_Pa:
'		Saturation vapor pressure in Pa.
' 	double dp_sat_dT_PaK:
'		Derivative of saturation pressure wrt. temperature in Pa/K.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Derivative of equilibrium pressure wrt. temperature in Pa/K.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on the surface
'	approach using the saturated vapor pressure as additional argument.
'
' History:
' --------
'	07/14/2021, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_sur_dp_dT_wTpsat Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal w_kgkg As Double, ByVal T_K As Double, ByVal p_sat_Pa As Double, ByVal dp_sat_dT_PaK As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_sur_piStar_pyxgTpsatM:
' ---------------------------------
'
' Calculates reduced spreading pressure piStar in kg/mol depending on
' equilibrium pressure p in Pa, molar fraction of refrigerant in vapor phase
' in mol/mol, molar fraction of refrigerant in adsorbed phase in mol/mol,
' activity coefficient of refrigerant in adsorbed phase, equilibrium
' temperature T in K, saturation pressure p_sat in Pa, and molar mass of
' refrigerant M in kg/mol. The reduced spreading pressure is defined as
' follows:
'
' 	piStar = A' pi / (R' T' m_sorbent) = 1 / M'
'		Integral_0^p0{w(p,T) / p' dp}
'
'	where p0 = p_total*y / (gamma*x)
'
' Parameters:
' -----------
'	double p_total_Pa:
'		Total pressure of vapor phase in Pa.
'	double y_molmol:
'		Molar fraction of refrigerant in vapor phase in mol/mol.
'	double x_molmol:
'		Molar fraction of refrigerant in adsorbed phase in mol/mol.
'	double gamma:
'		Activity coefficient of refrigerant in adsorbed phase.
'	double T_K:
'		Equilibrium temperature in K.
'	double p_sat_Pa:
'		Saturation vapor pressure in Pa.
'	double M_kgmol:
'		Molar mass of refrigerant M in kg/mol.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Reduced spreading pressure in kg/mol.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on the surface
'	approach using the saturated vapor pressure as additional argument.
'
' History:
' --------
'	07/14/2021, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_sur_piStar_pyxgTpsatM Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal y_molmol As Double, ByVal x_molmol As Double, _
 ByVal gamma As Double, ByVal T_K As Double, ByVal p_sat_Pa As Double, ByVal M_kgmol As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_vol_W_ARho:
' ----------------------
'
' Calculates equilibrium volumetric loading W in m³/kg depending on equilibrium
' adsorption potential A in J/mol and saturated liquid density of adsorpt rho_l
' in kg/m3.
'
' Parameters:
' -----------
' 	double A_Jmol:
'		Equilibrium adsorption potential A in J/mol.
' 	double rho_l_kgm3:
'		Saturated liquid density of adsorpt rho_l in kg/m3.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Equilibrium volumetric loading W in m³/kg.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on the volumetric
'	approach.
'	Saturated liquid density of adsorpt is required to calculate volumetric
'	loading from characteristic curve in m3/kg when coefficients would directly
'	return loading w in kg/kg. Therefore, function checks unit of coefficients
'	by checking isotherm coefficients.
'
' History:
' --------
'	07/14/2021, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_vol_W_ARho Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal A_Jmol As Double, ByVal rho_l_kgm3 As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_vol_A_WRho:
' ----------------------
'
' Calculates equilibrium adsorption potential A in J/mol depending on
' equilibrium volumetric loading W in m³/kg and saturated liquid density of
' adsorpt rho_l in kg/m3.
'
' Parameters:
' -----------
' 	double W_m3kg:
'		Equilibrium volumetric loading W in m³/kg.
' 	double rho_l_kgm3:
'		Saturated liquid density of adsorpt rho_l in kg/m3.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Equilibrium adsorption potential A in J/mol.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on the volumetric
'	approach.
'	Saturated liquid density of adsorpt is required to calculate volumetric
'	loading from characteristic curve in m3/kg when coefficients would directly
'	return loading w in kg/kg.
'
' History:
' --------
'	07/14/2021, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_vol_A_WRho Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal W_m3kg As Double, ByVal rho_l_kgm3 As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_vol_dW_dA_ARho:
' --------------------------
'
' Calculates derivative of equilibrium volumetric loading dW_dA in m³mol/kg/J
' depending on equilibrium adsorption potential A in J/mol and saturated liquid
' density of adsorpt rho_l in kg/m3.
'
' Parameters:
' -----------
' 	double A_Jmol:
'		Equilibrium adsorption potential A in J/mol.
' 	double rho_l_kgm3:
'		Saturated liquid density of adsorpt rho_l in kg/m3.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Derivative of volumetric loading wrt. adsorption potential in
'		m³mol/kg/J.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on the volumetric
'	approach.
'	Saturated liquid density of adsorpt is required to calculate volumetric
'	loading from characteristic curve in m3/kg when coefficients would directly
'	return loading w in kg/kg.
'
' History:
' --------
'	07/14/2021, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_vol_dW_dA_ARho Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal A_Jmol As Double, ByVal rho_l_kgm3 As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_vol_dA_dW_WRho:
' --------------------------
'
' Calculates derivative of equilibrium adsorption potential dA_dW in kgJ/mol/m³
' depending on equilibrium volumetric loading W in m³/kg and saturated liquid
' density of adsorpt rho_l in kg/m3.
'
' Parameters:
' -----------
' 	double W_m3kg:
'		Equilibrium volumetric loading W in m³/kg.
' 	double rho_l_kgm3:
'		Saturated liquid density of adsorpt rho_l in kg/m3.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Derivative of adsorption potential wrt. volumetric loading in
'		kgJ/mol/m³.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on the volumetric
'	approach.
'	Saturated liquid density of adsorpt is required to calculate volumetric
'	loading from characteristic curve in m3/kg when coefficients would directly
'	return loading w in kg/kg.
'
' History:
' --------
'	07/14/2021, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_vol_dA_dW_WRho Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal W_m3kg As Double, ByVal rho_l_kgm3 As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_vol_w_pTpsatRho:
' ---------------------------
'
' Calculates equilibrium loading w in kg/kg depending on equilibrium pressure
' p in Pa, equilibrium temperature T in K, saturation pressure p_Sat in Pa and
' saturated liquid density of adsorpt rho in kg/m³.
'
' Parameters:
' -----------
' 	double p_Pa:
'		Equilibrium pressure in Pa.
'	double T_K:
'		Equilibrium temperature in K.
' 	double p_sat_Pa:
'		Saturation pressure in Pa.
'	double rho_l_kgm3:
'		Saturated liquid density of adsorpt in kg/m³.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Equilibrium loading in kg/kg.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on the volumetric
'	approach.
'
' History:
' --------
'	02/14/2020, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_vol_w_pTpsatRho Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal p_sat_Pa As Double, ByVal rho_l_kgm3 As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_vol_p_wTpsatRho:
' ---------------------------
'
' Calculates equilibrium pressure p in Pa depending on equilibrium loading w in
' kg/kg, equilibrium temperature T in K, saturation pressure p_Sat in Pa and
' density of adsorpt rho in kg/m³.
'
' Parameters:
' -----------
' 	double w_kgkg:
'		Equilibrium loading in kg/kg.
'	double T_K:
'		Equilibrium temperature in K.
' 	double p_sat_Pa:
'		Saturation pressure in Pa.
'	double rho_kgm3:
'		Density of adsorpt in kg/m³.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Equilibrium pressure in Pa.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on the volumetric
'	approach.
'
' History:
' --------
'	02/14/2020, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_vol_p_wTpsatRho Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal w_kgkg As Double, ByVal T_K As Double, _
 ByVal p_sat_Pa As Double, ByVal rho_l_kgm3 As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_vol_T_pwpsatRho:
' ---------------------------
'
' Calculates equilibrium temperature T in K depending on equilibrium pressure
' in Pa, equilibrium loading w in kg/kg, saturation pressure p_Sat in Pa and
' density of adsorpt rho in kg/m³.
'
' Parameters:
' -----------
'	double p_Pa:
'		Equilibrium pressure in Pa.
' 	double w_kgkg:
'		Equilibrium loading in kg/kg.
' 	double p_sat_Pa:
'		Saturation pressure in Pa.
'	double rho_kgm3:
'		Density of adsorpt in kg/m³.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Equilibrium temperature in K.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on the volumetric
'	approach.
'	Uses internal equation of states to calculate vapor pressure, saturated
'	liquid density of adsorpt, derivative of vapor pressure wrt. temperature
'	and derivative of saturated liquid density of adsorpt wrt. temperature. If
'	equation of states are not implemented for refrigerant, function returns -1
'  and throws warning. If user want to use his own equation of states, this
'  function cannot be used and determination of root to calculate T must
'  be implemented by user.
'
' History:
' --------
'	02/14/2020, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_vol_T_pwpsatRho Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal w_kgkg As Double, _
 ByVal p_sat_Pa As Double, ByVal rho_l_kgm3 As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_vol_dw_dp_pTpsatRho:
' -------------------------------
'
' Calculates derivative of equilibrium loading dw_dp with respect to pressure
' in kg/kg/Pa depending on equilibrium pressure p in Pa, equilibrium
' temperature T in K, saturation pressure p_Sat in Pa and density of adsorpt
' rho in kg/m³.
'
' Parameters:
' -----------
' 	double p_Pa:
'		Equilibrium pressure in Pa.
'	double T_K:
'		Equilibrium temperature in K.
' 	double p_sat_Pa:
'		Saturation pressure in Pa.
'	double rho_kgm3:
'		Density of adsorpt in kg/m³.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Derivative of equilibrium loading wrt. pressure in kg/kg/Pa.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on the volumetric
'	approach.
'
' History:
' --------
'	02/14/2020, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_vol_dw_dp_pTpsatRho Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal p_sat_Pa As Double, ByVal rho_l_kgm3 As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_vol_dw_dT_pTpsatRho:
' -------------------------------
'
' Calculates derivative of equilibrium loading dw_dp with respect to
' temperature in kg/kg/K depending on equilibrium pressure p in Pa, equilibrium
' temperature T in K, saturation pressure p_Sat in Pa, density of adsorpt
' rho in kg/m³, derivative of saturation pressure with respect to temperature
' dp_sat_dT in Pa/K and derivative of density of adsorpt with respect to
' temperature drho_dT in kg/m³/K.
'
' Parameters:
' -----------
' 	double p_Pa:
'		Equilibrium pressure in Pa.
'	double T_K:
'		Equilibrium temperature in K.
' 	double p_sat_Pa:
'		Saturation pressure in Pa.
'	double rho_kgm3:
'		Density of adsorpt in kg/m³.
' 	double dp_sat_dT_PaK:
'		Derivative of saturation pressure wrt. temperature in Pa/K.
'	double drho_dT_kgm3K:
'		Derivative of density of adsorpt wrt. temperature in kg/m³/K.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Derivative of equilibrium loading wrt. temperature in kg/kg/K.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on the volumetric
'	approach.
'
' History:
' --------
'	02/14/2020, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_vol_dw_dT_pTpsatRho Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal p_sat_Pa As Double, ByVal rho_l_kgm3 As Double, _
 ByVal dp_sat_dT_PaK As Double, ByVal drho_dT_kgm3K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_vol_dp_dw_wTpsatRho:
' -------------------------------
'
' Calculates derivative of equilibrium pressure p with respect to loading
' w in kgPa/kg depending on equilibrium loading w in kg/kg, equilibrium
' temperature T in K, saturation pressure p_Sat in Pa and density of adsorpt
' rho in kg/m³.
'
' Parameters:
' -----------
'	double w_kgkg:
'		Equilibrium loading in kg/kg.
'	double T_K:
'		Equilibrium temperature in K.
' 	double p_sat_Pa:
'		Saturation pressure in Pa.
'	double rho_kgm3:
'		Density of adsorpt in kg/m³.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Derivative of equilibrium pressure wrt. loading in Pakg/kg.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on the volumetric
'	approach.
'
' History:
' --------
'	02/14/2020, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_vol_dp_dw_wTpsatRho Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal w_kgkg As Double, ByVal T_K As Double, _
 ByVal p_sat_Pa As Double, ByVal rho_l_kgm3 As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_vol_dp_dT_wTpsatRho:
' -------------------------------
'
' Calculates derivative of equilibrium pressure p with respect to temperature
' T in kg/kg/K depending on equilibrium loading w in kg/kg, equilibrium
' temperature T in K, saturation pressure p_Sat in Pa and density of adsorpt
' rho in kg/m³, derivative of saturation pressure with respect to temperature
' dp_sat_dT in Pa/K and derivative of density of adsorpt with respect to
' temperature drho_dT in kg/m³/K.
'
' Parameters:
' -----------
'	double w_kgkg:
'		Equilibrium loading in kg/kg.
'	double T_K:
'		Equilibrium temperature in K.
' 	double p_sat_Pa:
'		Saturation pressure in Pa.
'	double rho_kgm3:
'		Density of adsorpt in kg/m³.
' 	double dp_sat_dT_PaK:
'		Derivative of saturation pressure wrt. temperature in Pa/K.
'	double drho_dT_kgm3K:
'		Derivative of density of adsorpt wrt. temperature in kg/m³/K.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Derivative of equilibrium pressure wrt. temperature in Pa/K.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on the volumetric
'	approach.
'
' History:
' --------
'	02/14/2020, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_vol_dp_dT_wTpsatRho Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal w_kgkg As Double, ByVal T_K As Double, _
 ByVal p_sat_Pa As Double, ByVal rho_l_kgm3 As Double, _
 ByVal dp_sat_dT_PaK As Double, ByVal drho_dT_kgm3K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ads_vol_piStar_pyxgTpsatRhoM:
' ------------------------------------
'
' Calculates reduced spreading pressure in kg/mol depending on equilibrium
' pressure p in Pa, molar fraction of refrigerant in vapor phase in mol/mol,
' molar fraction of refrigerant in adsorbed phase in mol/mol, activity
' coefficient of refrigerant in adsorbed phase, equilibrium temperature T in K,
' saturation pressure p_Sat in Pa and density of adsorpt rho in kg/m³ and molar
' mass of refrigerant M in kg/mol. The reduced spreading pressure is defined as
' follows:
'
' 	piStar = A' pi / (R' T' m_sorbent) = 1 / M'
'		Integral_0^p0{w(p,T) / p' dp}
'
'	where p0 = p_total*y / (gamma*x)
'
' Parameters:
' -----------
'	double p_total_Pa:
'		Total pressure of vapor phase in Pa.
'	double y_molmol:
'		Molar fraction of refrigerant in vapor phase in mol/mol.
'	double x_molmol:
'		Molar fraction of refrigerant in adsorbed phase in mol/mol.
'	double gamma:
'		Activity coefficient of refrigerant in adsorbed phase.
'	double T_K:
'		Equilibrium temperature in K.
' 	double p_sat_Pa:
'		Saturation pressure in Pa.
'	double rho_kgm3:
'		Density of adsorpt in kg/m³.
'	double M_kgmol:
'		Molar mass of refrigerant M in kg/mol.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Reduced spreading pressure in kg/mol.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on the volumetric
'	approach.
'
' History:
' --------
'	02/14/2020, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ads_vol_piStar_pyxgTpsatRhoM Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal y_molmol As Double, ByVal x_molmol As Double, ByVal gamma As Double, _
 ByVal T_K As Double, ByVal p_sat_Pa As Double, ByVal rho_l_kgm3 As Double, ByVal M_kgmol As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_abs_con_X_pT:
' --------------------
'
' Calculates equilibrium concentration X in kg/kg depending on equilibrium
' pressure p in Pa and equilibrium temperature T in K.
'
' Parameters:
' -----------
' 	double p_Pa:
'		Equilibrium pressure in Pa.
'	double T_K:
'		Equilibrium temperature in K.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Equilibrium concentration in kg/kg.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on the conventional
'	approach.
'
' History:
' --------
'	07/14/2021, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_abs_con_X_pT Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_abs_con_p_XT:
' --------------------
'
' Calculates equilibrium pressure p in Pa depending on equilibrium
' concentration X in kg/kg and equilibrium temperature T in K.
'
' Parameters:
' -----------
'	double X_kgkg:
'		Equilibrium concentration in kg/kg.
'	double T_K:
'		Equilibrium temperature in K.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
' 	double:
'		Equilibrium pressure in Pa.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on the conventional
'	approach.
'
' History:
' --------
'	07/14/2021, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_abs_con_p_XT Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal X_kgkg As Double, ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_abs_con_T_pX:
' --------------------
'
' Calculates equilibrium temperature in K depending on equilibrium pressure p
' in Pa and equilibrium concentration X in kg/kg.
'
' Parameters:
' -----------
'	double p_Pa:
'		Equilibrium pressure in Pa.
'	double X_kgkg:
'		Equilibrium concentration in kg/kg.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
' 	double:
'		Equilibrium temperature in K.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on the conventional
'	approach.
'
' History:
' --------
'	07/14/2021, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_abs_con_T_pX Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal X_kgkg As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_abs_con_dX_dp_pT:
' ------------------------
'
' Calculates derivative of equilibrium concentration X with respect to pressure
' p in kg/kg/Pa depending on equilibrium pressure p in Pa and equilibrium
' temperature T in K.
'
' Parameters:
' -----------
' 	double p_Pa:
'		Equilibrium pressure in Pa.
'	double T_K:
'		Equilibrium temperature in K.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Derivative of equilibrium concentration wrt. pressure in kg/kg/Pa.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on the conventional
'	approach.
'
Private Declare PtrSafe Function direct_abs_con_dX_dp_pT Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_abs_con_dX_dT_pT:
' ------------------------
'
' Calculates derivative of equilibrium concentration X with respect to
' temperature T in kg/kg/K depending on equilibrium pressure p in Pa and
' equilibrium temperature T in K.
'
' Parameters:
' -----------
' 	double p_Pa:
'		Equilibrium pressure in Pa.
'	double T_K:
'		Equilibrium temperature in K.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Derivative of equilibrium concentration wrt. temperature in kg/kg/K.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on the conventional
'	approach.
'
' History:
' --------
'	07/14/2021, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_abs_con_dX_dT_pT Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_abs_con_dp_dX_XT:
' ------------------------
'
' Calculates derivative of equilibrium pressure p with respect to concentration
' w in kgPa/kg depending on equilibrium concentration X in kg/kg and
' equilibrium temperature T in K.
'
' Parameters:
' -----------
'	double X_kgkg:
'		Equilibrium concentration in kg/kg.
'	double T_K:
'		Equilibrium temperature in K.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Derivative of equilibrium pressure wrt. concentration in Pakg/kg.
'
' History:
' --------
'	07/14/2021, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_abs_con_dp_dX_XT Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal X_kgkg As Double, ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_abs_con_dp_dT_XT:
' ------------------------
'
' Calculates derivative of equilibrium pressure p with respect to temperature
' T in kg/kg/K depending on equilibrium concentration X in kg/kg and
' equilibrium temperature T in K.
'
' Parameters:
' -----------
'	double X_kgkg:
'		Equilibrium concentration in kg/kg.
'	double T_K:
'		Equilibrium temperature in K.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Derivative of equilibrium pressure wrt. temperature in Pa/K.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on the conventional
'	approach.
'
' History:
' --------
'	07/14/2021, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_abs_con_dp_dT_XT Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal X_kgkg As Double, ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double















' direct_ref_p_sat_T:
' -------------------
'
' Calculates saturation pressure in Pa depending on equilibrium temperature T
' in K.
'
' Parameters:
' -----------
'	double T_K:
'		Equilibrium temperature in K.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Saturation pressure in Pa.
'
' History:
' --------
'	02/14/2020, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ref_p_sat_T Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ref_dp_sat_dT_T:
' -----------------------
'
' Calculates derivative of saturation pressure in Pa/K wrt to temperature
' depending on equilibrium temperature T in K.
'
' Parameters:
' -----------
'	double T_K:
'		Equilibrium temperature in K.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Derivative of saturation pressure wrt temperature in Pa/K.
'
' History:
' --------
'	02/14/2020, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ref_dp_sat_dT_T Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ref_rho_l_T:
' -------------------
'
' Calculates saturated liquid density in kg/m³ depending on equilibrium
' temperature T in K.
'
' Parameters:
' -----------
'	double T_K:
'		Equilibrium temperature in K.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Saturated liquid density in kg/m³.
'
' History:
' --------
'	02/14/2020, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ref_rho_l_T Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ref_drho_l_dT_T:
' -----------------------
'
' Calculates derivative of saturated liquid density wrt temperature in kg/m³/K
' depending on equilibrium temperature T in K.
'
' Parameters:
' -----------
'	double T_K:
'		Equilibrium temperature in K.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Derivative of saturated liquid density wrt temperature in kg/m³/K.
'
' History:
' --------
'	02/14/2020, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ref_drho_l_dT_T Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_ref_T_sat_p:
' -------------------
'
' Calculates saturation temperature in K depending on equilibrium pressure p
' in Pa.
'
' Parameters:
' -----------
'	double p_Pa:
'		Saturation pressure in Pa.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Equilibrium temperature in K.
'
' History:
' --------
'	02/14/2020, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_ref_T_sat_p Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
' direct_abs_act_g_Txv1v2:
' ------------------------
'
' Calculates activity coefficient of first component depending on temperature
' T_K in K, mole fraction in liquid phase x_molmol in mol/mol, molar volume of
' first component in m³/mol, and molar volume of second component in m³/mol.
'
' Parameters:
' -----------
'	double T_K:
'		Equilibrium temperature in K.
'	double x_molmol:
'		Equilibrium mole fraction in liquid phase in mol/mol.
'	double v1_m3mol:
'		Equilibrium molar volume of first component in m³/mol.
'	double v2_m3mol:
'		Equilibrium molar volume of second component in m³/mol.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Activity coefficient of first component.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on activity
'	coefficients.
'	Molar volumes may are not required and ignored. When molar volumes are
' 	required, function uses molar volumes stored in JSON file when input
'	v1_m3mol or v2_m3mol is -1. Otherwise, function uses molar volumes given by
'	inputs.
'
' History:
' --------
'	02/14/2020, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_abs_act_g_Txv1v2 Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal T_K As Double, ByVal x_molmol As Double, ByVal v1_m3mol As Double, ByVal v2_m3mol As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_abs_act_p_Txv1v2psat:
' ----------------------------
'
' Calculates equilibrium pressure p_Pa in Pa of first component depending on
' temperature T_K in K, mole fraction in liquid phase x_molmol in mol/mol,
' molar volume of first component in m³/mol, molar volume of second component
' in m³/mol,and saturation pressure of first component p_sat_Pa in Pa.
'
' Parameters:
' -----------
'	double T_K:
'		Equilibrium temperature in K.
'	double x_molmol:
'		Equilibrium mole fraction in liquid phase in mol/mol.
'	double v1_m3mol:
'		Equilibrium molar volume of first component in m³/mol.
'	double v2_m3mol:
'		Equilibrium molar volume of second component in m³/mol.
'	double p_sat_Pa:
'		Saturation pressure of first component in Pa.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Equilibrium pressure p_Pa in Pa.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on activity
'	coefficients.
'	Molar volumes may are not required and ignored. When molar volumes are
' 	required, function uses molar volumes stored in JSON file when input
'	v1_m3mol or v2_m3mol is -1. Otherwise, function uses molar volumes given by
'	inputs.
'
' History:
' --------
'	02/14/2020, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_abs_act_p_Txv1v2psat Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal T_K As Double, ByVal x_molmol As Double, ByVal v1_m3mol As Double, ByVal v2_m3mol As Double, ByVal p_sat_Pa As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_abs_act_x_pTv1v2psat:
' ----------------------------
'
' Calculates mole fraction of first component in liquid phase x_molmol in
' mol/mol depending on equilibrium pressure p_Pa in Pa of first component,
' temperature T_K in K, molar volume of first component in m³/mol, molar volume
' of second component in m³/mol and saturation pressure of first component
' p_sat_Pa in Pa
'
' Parameters:
' -----------
'	double p_Pa:
'		Equilibrium pressure p_Pa in Pa.
'	double T_K:
'		Equilibrium temperature in K.
'	double v1_m3mol:
'		Equilibrium molar volume of first component in m³/mol.
'	double v2_m3mol:
'		Equilibrium molar volume of second component in m³/mol.
'	double p_sat_Pa:
'		Saturation pressure of first component in Pa.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Equilibrium mole fraction in liquid phase in mol/mol.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on activity
'	coefficients.
'	Molar volumes may are not required and ignored. When molar volumes are
' 	required, function uses molar volumes stored in JSON file when input
'	v1_m3mol or v2_m3mol is -1. Otherwise, function uses molar volumes given by
'	inputs.
'
' History:
' --------
'	02/14/2020, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_abs_act_x_pTv1v2psat Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal T_K As Double, ByVal v1_m3mol As Double, ByVal v2_m3mol As Double, ByVal p_sat_Pa As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_abs_act_p_Txv1v2:
' ------------------------
'
' Calculates equilibrium pressure p_Pa in Pa of first component depending on
' temperature T_K in K, mole fraction in liquid phase x_molmol in mol/mol,
' molar volume of first component in m³/mol and molar volume of second
' component in m³/mol.
'
' Parameters:
' -----------
'	double T_K:
'		Equilibrium temperature in K.
'	double x_molmol:
'		Equilibrium mole fraction in liquid phase in mol/mol.
'	double v1_m3mol:
'		Equilibrium molar volume of first component in m³/mol.
'	double v2_m3mol:
'		Equilibrium molar volume of second component in m³/mol.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Equilibrium pressure p_Pa in Pa.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on activity
'	coefficients.
'	Molar volumes may are not required and ignored. When molar volumes are
' 	required, function uses molar volumes stored in JSON file when input
'	v1_m3mol or v2_m3mol is -1. Otherwise, function uses molar volumes given by
'	inputs.
'
' History:
' --------
'	02/14/2020, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_abs_act_p_Txv1v2 Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal x_molmol As Double, ByVal v1_m3mol As Double, ByVal v2_m3mol As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_abs_act_x_pTv1v2:
' ------------------------
'
' Calculates mole fraction of first component in liquid phase x_molmol in
' mol/mol depending on equilibrium pressure p_Pa in Pa of first component,
' temperature T_K in K, molar volume of first component in m³/mol and molar
' volume of second component in m³/mol.
'
' Parameters:
' -----------
'	double p_Pa:
'		Equilibrium pressure p_Pa in Pa.
'	double T_K:
'		Equilibrium temperature in K.
'	double v1_m3mol:
'		Equilibrium molar volume of first component in m³/mol.
'	double v2_m3mol:
'		Equilibrium molar volume of second component in m³/mol.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Equilibrium mole fraction in liquid phase in mol/mol.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on activity
'	coefficients.
'	Molar volumes may are not required and ignored. When molar volumes are
' 	required, function uses molar volumes stored in JSON file when input
'	v1_m3mol or v2_m3mol is -1. Otherwise, function uses molar volumes given by
'	inputs.
'
' History:
' --------
'	02/14/2020, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_abs_act_x_pTv1v2 Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal T_K As Double, ByVal v1_m3mol As Double, ByVal v2_m3mol As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


 ' direct_abs_mix_x_pT:
 ' --------------------
 '
 ' Calculates equilibrium liquid mole fraction of first component in mol/mol
 ' depending on pressure in Pa and temperature T_K in K.
 '
 ' Parameters:
 ' -----------
 '	double p_Pa:
 '		Equilibrium pressure p_Pa in Pa.
 '	double T_K:
 '		Equilibrium temperature in K.
 '
 '	const char 'path_db:
 '		Path to database.
 ' 	const char 'wp_as:
 '		Name of sorbent.
 ' 	const char 'wp_st:
 '		Name of sub-type of sorbent.
 ' 	const char 'wp_rf:
 '		Name of refrigerant.
 ' 	const char 'wp_iso:
 '		Name of isotherm.
 '	int no_iso:
 '		ID of isotherm (i.e. when more than one isotherm is available)
 ' 	const char 'rf_psat:
 '		Name of calculation approach for vapor pressure.
 '	int no_p_sat:
 '		ID of vapor pressure equation (i.e. when more than one equation is
 '		available)
 ' 	const char 'rf_rhol:
 '		Name of calculation approach for liquid density.
 '	int no_rhol:
 '		ID of liquid density equation (i.e. when more than one equation is
 '		available)
 '
 ' Returns:
 ' --------
 '	double:
 '		Equilibrium mole fraction in of first component liquid phase in mol/mol.
 '	double 'ret_y_1_molmol:
 '		Equilibrium mole fraction of first component.
 '	double 'ret_y_2_molmol:
 '		Equilibrium mole fraction of second component.
 '
 ' Remarks:
 ' --------
 '	This function is only valid for isotherm models based on mixing rules.
'
' History:
' --------
'	02/14/2020, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_abs_mix_x_pT Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByRef ret_y_1_molmol As Double, ByRef ret_y_2_molmol As Double, ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


' direct_abs_mix_p_Tx:
' --------------------
'
' Calculates equilibrium pressure p_Pa in Pa depending on temperature T_K in K
' and mole fraction in liquid phase x_molmol in mol/mol.
'
' Parameters:
' -----------
'	double T_K:
'		Equilibrium temperature in K.
'	double x_1_molmol:
'		Equilibrium mole fraction in liquid phase in mol/mol.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Equilibrium pressure p_Pa in Pa.
'	double'ret_y_1_molmol:
'		Equilibrium mole fraction of first component.
'	double'ret_y_2_molmol:
'		Equilibrium mole fraction of second component.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on mixing rules.
'
' History:
' --------
'	02/14/2020, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_abs_mix_p_Tx Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByRef ret_y_1_molmol As Double, ByRef ret_y_2_molmol As Double, ByVal T_K As Double, ByVal x_1_molmol As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
 

' direct_abs_mix_T_px:
' --------------------
'
' Calculates equilibrium temperature T_K in K depending on pressure p_Pa in Pa
' and mole fraction in liquid phase x_molmol in mol/mol.
'
' Parameters:
' -----------
'	double p_Pa:
'		Equilibrium pressure p_Pa in Pa.
'	double x_1_molmol:
'		Equilibrium mole fraction in liquid phase in mol/mol.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Equilibrium mole fraction in liquid phase in mol/mol.
'	double'ret_y_1_molmol:
'		Equilibrium mole fraction of first component.
'	double'ret_y_2_molmol:
'		Equilibrium mole fraction of second component.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on mixing rules.
'	Uses Newton-Raphson method for calculating equilibrium temperature.
'
' History:
' --------
'	02/14/2020, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_abs_mix_T_px Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByRef ret_y_1_molmol As Double, ByRef ret_y_2_molmol As Double, ByVal p_Pa As Double, ByVal x_1_molmol As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
 

' direct_abs_mix_dp_dx_Tx:
' ------------------------
'
' Calculates derivative of equilibrium pressure with respect to liquid mole
' fraction in Pa depending on temperature T_K in K and mole fraction in liquid
' phase x_molmol in mol/mol.
'
' Parameters:
' -----------
'	double T_K:
'		Equilibrium temperature in K.
'	double x_1_molmol:
'		Equilibrium mole fraction in liquid phase in mol/mol.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Derivative of equilibrium pressure wrt liquid mole fraction in Pa.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on mixing rules.
'	Derivative is calculated numerically by the symmetric derivative using
'  h = 0.00000001 mol/mol as small change.
'
' History:
' --------
'	02/14/2020, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_abs_mix_dp_dx_Tx Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal T_K As Double, ByVal x_1_molmol As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
 

' direct_abs_mix_dp_dT_Tx:
' ------------------------
'
' Calculates derivative of equilibrium pressure with respect to temperature in
' Pa/K depending on temperature T_K in K and mole fraction in liquid phase
' x_molmol in mol/mol.
'
' Parameters:
' -----------
'	double T_K:
'		Equilibrium temperature in K.
'	double x_1_molmol:
'		Equilibrium mole fraction in liquid phase in mol/mol.
'
'	const char *path_db:
'		Path to database.
' 	const char *wp_as:
'		Name of sorbent.
' 	const char *wp_st:
'		Name of sub-type of sorbent.
' 	const char *wp_rf:
'		Name of refrigerant.
' 	const char *wp_iso:
'		Name of isotherm.
'	int no_iso:
'		ID of isotherm (i.e. when more than one isotherm is available)
' 	const char *rf_psat:
'		Name of calculation approach for vapor pressure.
'	int no_p_sat:
'		ID of vapor pressure equation (i.e. when more than one equation is
'		available)
' 	const char *rf_rhol:
'		Name of calculation approach for liquid density.
'	int no_rhol:
'		ID of liquid density equation (i.e. when more than one equation is
'		available)
'
' Returns:
' --------
'	double:
'		Derivative of equilibrium pressure wrt temperature in Pa/K.
'
' Remarks:
' --------
'	This function is only valid for isotherm models based on mixing rules.
'	Derivative is calculated numerically by the symmetric derivative using
'  h = 0.0001 K as small change.
'
' History:
' --------
'	02/14/2020, by Mirko Engelpracht:
'		First implementation.
'
Private Declare PtrSafe Function direct_abs_mix_dp_dT_Tx Lib "D:\Engelpracht\04-C\sorproplib\sorpproplib_JSON\excel_wrapper\data\win64bit\libsorpPropLib.dll" _
(ByVal T_K As Double, ByVal x_1_molmol As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
 


''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Define functions that can be accessed from the worksheet '
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' ws_direct_ads_w_pT
'
Function ws_direct_ads_w_pT _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_w_pT = direct_ads_w_pT(p_Pa, T_K, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function


' ws_direct_ads_p_wT
'
Function ws_direct_ads_p_wT _
(ByVal w_kgkg As Double, ByVal T_K As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_p_wT = direct_ads_p_wT(w_kgkg, T_K, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function


' ws_direct_ads_T_pw
'
Function ws_direct_ads_T_pw _
(ByVal p_Pa As Double, ByVal w_kgkg As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_T_pw = direct_ads_T_pw(p_Pa, w_kgkg, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function


' ws_direct_ads_dw_dp_pT
'
Function ws_direct_ads_dw_dp_pT _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_dw_dp_pT = direct_ads_dw_dp_pT(p_Pa, T_K, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function


' ws_direct_ads_dw_dT_pT
'
Function ws_direct_ads_dw_dT_pT _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_dw_dT_pT = direct_ads_dw_dT_pT(p_Pa, T_K, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function


' ws_direct_ads_dp_dw_wT
'
Function ws_direct_ads_dp_dw_wT _
(ByVal w_kgkg As Double, ByVal T_K As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_dp_dw_wT = direct_ads_dp_dw_wT(w_kgkg, T_K, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function


' ws_direct_ads_dp_dT_wT
'
Function ws_direct_ads_dp_dT_wT _
(ByVal w_kgkg As Double, ByVal T_K As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_dp_dT_wT = direct_ads_dp_dT_wT(w_kgkg, T_K, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function


' ws_direct_ads_piStar_pyxgTM
'
Function ws_direct_ads_piStar_pyxgTM _
(ByVal p_Pa As Double, ByVal y_molmol As Double, ByVal x_molmol As Double, _
 ByVal gamma As Double, ByVal T_K As Double, ByVal M_kgmol As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_piStar_pyxgTM = direct_ads_piStar_pyxgTM(p_Pa, y_molmol, x_molmol, _
        gamma, T_K, M_kgmol, path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, _
		no_p_sat, rf_rhol, no_rhol)

End Function


' ws_direct_ads_sur_w_pT
'
Function ws_direct_ads_sur_w_pT _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_sur_w_pT = direct_ads_sur_w_pT(p_Pa, T_K, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function


' ws_direct_ads_sur_p_wT
'
Function ws_direct_ads_sur_p_wT _
(ByVal w_kgkg As Double, ByVal T_K As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_sur_p_wT = direct_ads_sur_p_wT(w_kgkg, T_K, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function


' ws_direct_ads_sur_T_pw
'
Function ws_direct_ads_sur_T_pw _
(ByVal p_Pa As Double, ByVal w_kgkg As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_sur_T_pw = direct_ads_sur_T_pw(p_Pa, w_kgkg, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function


' ws_direct_ads_sur_dw_dp_pT
'
Function ws_direct_ads_sur_dw_dp_pT _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_sur_dw_dp_pT = direct_ads_sur_dw_dp_pT(p_Pa, T_K, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function


' ws_direct_ads_sur_dw_dT_pT
'
Function ws_direct_ads_sur_dw_dT_pT _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_sur_dw_dT_pT = direct_ads_sur_dw_dT_pT(p_Pa, T_K, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function


' ws_direct_ads_sur_dp_dw_wT
'
Function ws_direct_ads_sur_dp_dw_wT _
(ByVal w_kgkg As Double, ByVal T_K As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_sur_dp_dw_wT = direct_ads_sur_dp_dw_wT(w_kgkg, T_K, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function


' ws_direct_ads_sur_dp_dT_wT
'
Function ws_direct_ads_sur_dp_dT_wT _
(ByVal w_kgkg As Double, ByVal T_K As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_sur_dp_dT_wT = direct_ads_sur_dp_dT_wT(w_kgkg, T_K, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function


' ws_direct_ads_sur_piStar_pyxgTM
'
Function ws_direct_ads_sur_piStar_pyxgTM _
(ByVal p_Pa As Double, ByVal y_molmol As Double, ByVal x_molmol As Double, _
 ByVal gamma As Double, ByVal T_K As Double, ByVal M_kgmol As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_sur_piStar_pyxgTM = direct_ads_sur_piStar_pyxgTM(p_Pa, y_molmol, x_molmol, _
        gamma, T_K, M_kgmol, path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, _
		no_p_sat, rf_rhol, no_rhol)

End Function


' ws_direct_ads_sur_w_pTpsat
'
Function ws_direct_ads_sur_w_pTpsat _
(ByVal p_Pa As Double, ByVal T_K As Double, ByVal p_sat_Pa As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_sur_w_pTpsat = direct_ads_sur_w_pTpsat(p_Pa, T_K, p_sat_Pa, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function


' ws_direct_ads_sur_p_wTpsat
'
Function ws_direct_ads_sur_p_wTpsat _
(ByVal w_kgkg As Double, ByVal T_K As Double, ByVal p_sat_Pa As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_sur_p_wTpsat = direct_ads_sur_p_wTpsat(w_kgkg, T_K, p_sat_Pa, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function


' ws_direct_ads_sur_T_pwpsat
'
Function ws_direct_ads_sur_T_pwpsat _
(ByVal p_Pa As Double, ByVal w_kgkg As Double, ByVal p_sat_Pa As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_sur_T_pwpsat = direct_ads_sur_T_pwpsat(p_Pa, w_kgkg, p_sat_Pa, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function


' ws_direct_ads_sur_dw_dp_pTpsat
'
Function ws_direct_ads_sur_dw_dp_pTpsat _
(ByVal p_Pa As Double, ByVal T_K As Double, ByVal p_sat_Pa As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_sur_dw_dp_pTpsat = direct_ads_sur_dw_dp_pTpsat(p_Pa, T_K, p_sat_Pa, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function


' ws_direct_ads_sur_dw_dT_pTpsat
'
Function ws_direct_ads_sur_dw_dT_pTpsat _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal p_sat_Pa As Double, ByVal dp_sat_dT_PaK As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_sur_dw_dT_pTpsat = direct_ads_sur_dw_dT_pTpsat(p_Pa, T_K, p_sat_Pa, dp_sat_dT_PaK, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function


' ws_direct_ads_sur_dp_dw_wTpsat
'
Function ws_direct_ads_sur_dp_dw_wTpsat _
(ByVal w_kgkg As Double, ByVal T_K As Double, ByVal p_sat_Pa As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_sur_dp_dw_wTpsat = direct_ads_sur_dp_dw_wTpsat(w_kgkg, T_K, p_sat_Pa, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function


' ws_direct_ads_sur_dp_dT_wTpsat
'
Function ws_direct_ads_sur_dp_dT_wTpsat _
(ByVal w_kgkg As Double, ByVal T_K As Double, _
 ByVal p_sat_Pa As Double, ByVal dp_sat_dT_PaK As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_sur_dp_dT_wTpsat = direct_ads_sur_dp_dT_wTpsat(w_kgkg, T_K, p_sat_Pa, dp_sat_dT_PaK, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function


' ws_direct_ads_sur_piStar_pyxgTpsatM
'
Function ws_direct_ads_sur_piStar_pyxgTpsatM _
(ByVal p_Pa As Double, ByVal y_molmol As Double, ByVal x_molmol As Double, _
 ByVal gamma As Double, ByVal T_K As Double, ByVal p_sat_Pa As Double, ByVal M_kgmol As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_sur_piStar_pyxgTpsatM = direct_ads_sur_piStar_pyxgTpsatM(p_Pa, y_molmol, x_molmol, _
        gamma, T_K, p_sat_Pa, M_kgmol, path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, _
		no_p_sat, rf_rhol, no_rhol)

End Function

' ws_direct_ads_vol_W_ARho
'
Function ws_direct_ads_vol_W_ARho _
(ByVal A_Jmol As Double, ByVal rho_l_kgm3 As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_vol_W_ARho = direct_ads_vol_W_ARho(A_Jmol, rho_l_kgm3, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function

' ws_direct_ads_vol_A_WRho
'
Function ws_direct_ads_vol_A_WRho _
(ByVal W_m3kg As Double, ByVal rho_l_kgm3 As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_vol_A_WRho = direct_ads_vol_A_WRho(W_m3kg, rho_l_kgm3, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function

' ws_direct_ads_vol_dW_dA_ARho
'
Function ws_direct_ads_vol_dW_dA_ARho _
(ByVal A_Jmol As Double, ByVal rho_l_kgm3 As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_vol_dW_dA_ARho = direct_ads_vol_dW_dA_ARho(A_Jmol, rho_l_kgm3, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function

' ws_direct_ads_vol_dA_dW_WRho
'
Function ws_direct_ads_vol_dA_dW_WRho _
(ByVal W_m3kg As Double, ByVal rho_l_kgm3 As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_vol_dA_dW_WRho = direct_ads_vol_dA_dW_WRho(W_m3kg, rho_l_kgm3, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function

' ws_direct_ads_vol_w_pTpsatRho
'
Function ws_direct_ads_vol_w_pTpsatRho _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal p_sat_Pa As Double, ByVal rho_kgm3 As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_vol_w_pTpsatRho = direct_ads_vol_w_pTpsatRho(p_Pa, T_K, p_sat_Pa, rho_kgm3, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function

' ws_direct_ads_vol_p_wTpsatRho
'
Function ws_direct_ads_vol_p_wTpsatRho _
(ByVal w_kgkg As Double, ByVal T_K As Double, _
 ByVal p_sat_Pa As Double, ByVal rho_kgm3 As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_vol_p_wTpsatRho = direct_ads_vol_p_wTpsatRho(w_kgkg, T_K, p_sat_Pa, rho_kgm3, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function

' ws_direct_vol_T_pwpsatRho
'
Function ws_direct_ads_vol_T_pwpsatRho _
(ByVal p_Pa As Double, ByVal w_kgkg As Double, _
 ByVal p_sat_Pa As Double, ByVal rho_kgm3 As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_vol_T_pwpsatRho = direct_ads_vol_T_pwpsatRho(p_Pa, w_kgkg, p_sat_Pa, rho_kgm3, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function

' ws_direct_ads_vol_dw_dp_pTpsatRho
'
Function ws_direct_ads_vol_dw_dp_pTpsatRho _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal p_sat_Pa As Double, ByVal rho_kgm3 As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_vol_dw_dp_pTpsatRho = direct_ads_vol_dw_dp_pTpsatRho(p_Pa, T_K, p_sat_Pa, rho_kgm3, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function

' ws_direct_ads_vol_dw_dT_pTpsatRho
'
Function ws_direct_ads_vol_dw_dT_pTpsatRho _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal p_sat_Pa As Double, ByVal rho_kgm3 As Double, _
 ByVal dp_sat_dT_PaK As Double, ByVal drho_dT_kgm3K As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_vol_dw_dT_pTpsatRho = direct_ads_vol_dw_dT_pTpsatRho(p_Pa, T_K, p_sat_Pa, rho_kgm3, dp_sat_dT_PaK, drho_dT_kgm3K, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function

' ws_direct_ads_vol_dp_dw_wTpsatRho
'
Function ws_direct_ads_vol_dp_dw_wTpsatRho _
(ByVal w_kgkg As Double, ByVal T_K As Double, _
 ByVal p_sat_Pa As Double, ByVal rho_kgm3 As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_vol_dp_dw_wTpsatRho = direct_ads_vol_dp_dw_wTpsatRho(w_kgkg, T_K, p_sat_Pa, rho_kgm3, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function

' ws_direct_ads_vol_dp_dT_wTpsatRho
'
Function ws_direct_ads_vol_dp_dT_wTpsatRho _
(ByVal w_kgkg As Double, ByVal T_K As Double, _
 ByVal p_sat_Pa As Double, ByVal rho_kgm3 As Double, _
 ByVal dp_sat_dT_PaK As Double, ByVal drho_dT_kgm3K As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_vol_dp_dT_wTpsatRho = direct_ads_vol_dp_dT_wTpsatRho(w_kgkg, T_K, p_sat_Pa, rho_kgm3, dp_sat_dT_PaK, drho_dT_kgm3K, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function

' ws_direct_ads_vol_piStar_pyxgTpsatRhoM
'
Function ws_direct_ads_vol_piStar_pyxgTpsatRhoM _
(ByVal p_Pa As Double, ByVal y_molmol As Double, ByVal x_molmol As Double, ByVal gamma As Double, _
 ByVal T_K As Double, ByVal p_sat_Pa As Double, ByVal rho_kgm3 As Double, ByVal M_kgmol As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ads_vol_piStar_pyxgTpsatRhoM = direct_ads_vol_piStar_pyxgTpsatRhoM(p_Pa, y_molmol, x_molmol, _
        gamma, T_K, p_sat_Pa, rho_kgm3, M_kgmol, path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, _
		no_p_sat, rf_rhol, no_rhol)

End Function

' ws_direct_abs_con_X_pT
'
Function ws_direct_abs_con_X_pT _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_abs_con_X_pT = direct_abs_con_X_pT(p_Pa, T_K, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function

' ws_direct_abs_con_p_XT
'
Function ws_direct_abs_con_p_XT _
(ByVal X_kgkg As Double, ByVal T_K As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_abs_con_p_XT = direct_abs_con_p_XT(X_kgkg, T_K, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function

' ws_direct_abs_con_T_pX
'
Function ws_direct_abs_con_T_pX _
(ByVal p_Pa As Double, ByVal X_kgkg As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_abs_con_T_pX = direct_abs_con_T_pX(p_Pa, X_kgkg, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function

' ws_direct_abs_con_dX_dp_pT
'
Function ws_direct_abs_con_dX_dp_pT _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_abs_con_dX_dp_pT = direct_abs_con_dX_dp_pT(p_Pa, T_K, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function

' ws_direct_abs_con_dX_dT_pT
'
Function ws_direct_abs_con_dX_dT_pT _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_abs_con_dX_dT_pT = direct_abs_con_dX_dT_pT(p_Pa, T_K, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function

' ws_direct_abs_con_dp_dX_XT
'
Function ws_direct_abs_con_dp_dX_XT _
(ByVal X_kgkg As Double, ByVal T_K As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_abs_con_dp_dX_XT = direct_abs_con_dp_dX_XT(X_kgkg, T_K, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function

' ws_direct_abs_con_dp_dT_XT
'
Function ws_direct_abs_con_dp_dT_XT _
(ByVal X_kgkg As Double, ByVal T_K As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_abs_con_dp_dT_XT = direct_abs_con_dp_dT_XT(X_kgkg, T_K, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function

' ws_direct_abs_act_g_Txv1v2
'
Function ws_direct_abs_act_g_Txv1v2 _
(ByVal T_K As Double, ByVal x_molmol As Double, _
 ByVal v1_m3mol As Double, ByVal v2_m3mol As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_abs_act_g_Txv1v2 = direct_abs_act_g_Txv1v2(T_K, x_molmol, v1_m3mol, v2_m3mol, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function

' ws_direct_abs_act_p_Txv1v2psat
'
Function ws_direct_abs_act_p_Txv1v2psat _
(ByVal T_K As Double, ByVal x_molmol As Double, _
 ByVal v1_m3mol As Double, ByVal v2_m3mol As Double, ByVal p_sat_Pa As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_abs_act_p_Txv1v2psat = direct_abs_act_p_Txv1v2psat(T_K, x_molmol, v1_m3mol, v2_m3mol, p_sat_Pa, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function

' ws_direct_abs_act_x_pTv1v2psat
'
Function ws_direct_abs_act_x_pTv1v2psat _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal v1_m3mol As Double, ByVal v2_m3mol As Double, ByVal p_sat_Pa As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_abs_act_x_pTv1v2psat = direct_abs_act_x_pTv1v2psat(p_Pa, T_K, v1_m3mol, v2_m3mol, p_sat_Pa, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function

' ws_direct_abs_act_p_Txv1v2
'
Function ws_direct_abs_act_p_Txv1v2 _
(ByVal T_K As Double, ByVal x_molmol As Double, _
 ByVal v1_m3mol As Double, ByVal v2_m3mol As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_abs_act_p_Txv1v2 = direct_abs_act_p_Txv1v2(T_K, x_molmol, v1_m3mol, v2_m3mol, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function

' ws_direct_abs_act_x_pTv1v2
'
Function ws_direct_abs_act_x_pTv1v2 _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal v1_m3mol As Double, ByVal v2_m3mol As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_abs_act_x_pTv1v2 = direct_abs_act_x_pTv1v2(p_Pa, T_K, v1_m3mol, v2_m3mol, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function

' ws_direct_abs_mix_x_pT
'
Function ws_direct_abs_mix_x_pT _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    Dim ret_y_1_molmol As Double
    Dim ret_y_2_molmol As Double
	
    ws_direct_abs_mix_x_pT = direct_abs_mix_x_pT(ret_y_1_molmol, ret_y_2_molmol, p_Pa, T_K, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function

' ws_direct_abs_mix_p_Tx
'
Function ws_direct_abs_mix_p_Tx _
(ByVal T_K As Double, ByVal x_1_molmol As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    Dim ret_y_1_molmol As Double
    Dim ret_y_2_molmol As Double
	
    ws_direct_abs_mix_p_Tx= direct_abs_mix_p_Tx(ret_y_1_molmol, ret_y_2_molmol, T_K, x_1_molmol, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function

' ws_direct_abs_mix_T_px
'
Function ws_direct_abs_mix_T_px _
(ByVal p_Pa As Double, ByVal x_1_molmol As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    Dim ret_y_1_molmol As Double
    Dim ret_y_2_molmol As Double
	
    ws_direct_abs_mix_T_px = direct_abs_mix_T_px(ret_y_1_molmol, ret_y_2_molmol, p_Pa, x_1_molmol, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function

' ws_direct_abs_mix_dp_dx_Tx
'
Function ws_direct_abs_mix_dp_dx_Tx _
(ByVal p_Pa As Double, ByVal x_1_molmol As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"
	
    ws_direct_abs_mix_dp_dx_Tx = direct_abs_mix_dp_dx_Tx(p_Pa, x_1_molmol, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function

' ws_direct_abs_mix_dp_dT_Tx
'
Function ws_direct_abs_mix_dp_dT_Tx _
(ByVal p_Pa As Double, ByVal x_1_molmol As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"
	
    ws_direct_abs_mix_dp_dT_Tx = direct_abs_mix_dp_dT_Tx(p_Pa, x_1_molmol, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function

' ws_direct_ref_p_sat_T
'
Function ws_direct_ref_p_sat_T _
(ByVal T_K As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ref_p_sat_T = direct_ref_p_sat_T(T_K, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function

' ws_direct_ref_dp_sat_dT_T
'
Function ws_direct_ref_dp_sat_dT_T _
(ByVal T_K As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ref_dp_sat_dT_T = direct_ref_dp_sat_dT_T(T_K, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function

' ws_direct_ref_rho_l_T
'
Function ws_direct_ref_rho_l_T _
(ByVal T_K As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ref_rho_l_T = direct_ref_rho_l_T(T_K, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function

' ws_direct_ref_drho_l_dT_T
'
Function ws_direct_ref_drho_l_dT_T _
(ByVal T_K As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ref_drho_l_dT_T = direct_ref_drho_l_dT_T(T_K, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function

' ws_direct_ref_T_sat_p
'
Function ws_direct_ref_T_sat_p _
(ByVal T_K As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\JSON\sorpproplib.json"

    ws_direct_ref_T_sat_p = direct_ref_T_sat_p(T_K, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)

End Function
