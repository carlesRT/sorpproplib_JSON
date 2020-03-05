Attribute VB_Name = "DirectFunctionCalls"
Option Explicit

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Create function wrapper to access functions from SorpPropLib-DLL '
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

'
' direct_ads_w_pT_workingPair:
' ----------------------------
'
' Calculates equilibrium loading w in kg/kg depending on equilibrium pressure
' p in Pa, equilibrium temperature T in K, and specified working pair.
'
' Parameters:
' -----------
'  double p_Pa:
'      Equilibrium pressure in Pa.
'  double T_K:
'      Equilibrium temperature in K.
'
'  string path_db:
'      Path to database.
'  string wp_as:
'      Name of sorbent.
'  string wp_st:
'      Name of sub-type of sorbent.
'  string wp_rf:
'      Name of refrigerant.
'  string wp_iso:
'      Name of isotherm.
'  int no_iso:
'      Number of isotherm (i.e. when more than one isotherm is available)
'  string rf_psat:
'      Name of calculation approach for vapour pressure.
'  int no_p_sat:
'      Number of vapour pressure equation (i.e. when more than one equation is
'      available)
'  string rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      Number of liquid density equation (i.e. when more than one equation is
'      available)
'
' Returns:
' --------
'  double:
'      Equilibrium loading in kg/kg.
'
' History:
' --------
'  03/02/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_ads_w_pT_workingPair Lib "libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


'
' direct_ads_p_wT_workingPair:
' ----------------------------
'
' Calculates equilibrium pressure p in Pa depending on equilibrium temperature
' T in K, equilibrium loading w in kg/kg, equilibrium temperature T in K, and
' specified working pair.
'
' Parameters:
' -----------
'  double w_kgkg:
'      Equilibrium loading in kg/kg.
'  double T_K:
'      Equilibrium temperature in K.
'
'  string path_db:
'      Path to database.
'  string wp_as:
'      Name of sorbent.
'  string wp_st:
'      Name of sub-type of sorbent.
'  string wp_rf:
'      Name of refrigerant.
'  string wp_iso:
'      Name of isotherm.
'  int no_iso:
'      Number of isotherm (i.e. when more than one isotherm is available)
'  string rf_psat:
'      Name of calculation approach for vapour pressure.
'  int no_p_sat:
'      Number of vapour pressure equation (i.e. when more than one equation is
'      available)
'  string rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      Number of liquid density equation (i.e. when more than one equation is
'      available)
'      Name of calculation approach for liquid density.
'
' Returns:
' --------
'  double:
'      Equilibrium pressure in Pa.
'
' History:
' --------
'  03/02/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_ads_p_wT_workingPair Lib "libsorpPropLib.dll" _
(ByVal w_kgkg As Double, ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


'
'  direct_ads_T_pw_workingPair:
'  ----------------------------
'
'  Calculates equilibrium temperature in K depending on equilibrium pressure p
'  in Pa, equilibrium loading w in kg/kg, and specified working pair.
'
'  Parameters:
'  -----------
'   double p_Pa:
'       Equilibrium pressure in Pa.
'   double w_kgkg:
'       Equilibrium loading in kg/kg.
'
'   const string path_db:
'       Path to database.
'   const string wp_as:
'       Name of sorbent.
'   const string wp_st:
'       Name of sub-type of sorbent.
'   const string wp_rf:
'       Name of refrigerant.
'   const string wp_iso:
'       Name of isotherm.
'   int no_iso:
'       Number of isotherm (i.e. when more than one isotherm is available)
'   const string rf_psat:
'       Name of calculation approach for vapour pressure.
'   int no_p_sat:
'       Number of vapour pressure equation (i.e. when more than one equation is
'       available)
'   const string rf_rhol:
'       Name of calculation approach for liquid density.
'   int no_rhol:
'       Number of liquid density equation (i.e. when more than one equation is
'       available)
'       Name of calculation approach for liquid density.
'
'  Returns:
'  --------
'   double:
'       Equilibrium temperature in K.
'
'  Remarks:
'  --------
'   Function uses equation of states implemented for calculation of vapour
'   pressure, saturated liquid density, derivative of vapour pressure wrt.
'   temperature, and derivative of saturated liquid density wrt. temperature
'   when isotherm of working pair is based on volumetric approach (e.g.
'   Dubinin theory).
'
' History:
' --------
'  03/02/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_ads_T_pw_workingPair Lib "libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal w_kgkg As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
 

'
' direct_ads_dw_dp_pT_workingPair:
' --------------------------------
'
' Calculates derivative of equilibrium loading w with respect to pressure
' p in kg/kg/Pa depending on equilibrium pressure p in Pa and equilibrium
' temperature T in K.
'
' Parameters:
' -----------
'  double p_Pa:
'      Equilibrium pressure in Pa.
'  double T_K:
'      Equilibrium temperature in K.
'
'  string path_db:
'      Path to database.
'  string wp_as:
'      Name of sorbent.
'  string wp_st:
'      Name of sub-type of sorbent.
'  string wp_rf:
'      Name of refrigerant.
'  string wp_iso:
'      Name of isotherm.
'  int no_iso:
'      Number of isotherm (i.e. when more than one isotherm is available)
'  string rf_psat:
'      Name of calculation approach for vapour pressure.
'  int no_p_sat:
'      Number of vapour pressure equation (i.e. when more than one equation is
'      available)
'  string rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      Number of liquid density equation (i.e. when more than one equation is
'      available)
'
' Returns:
' --------
'  double:
'      Derivative of equilibrium loading wrt. pressure in kg/kg/Pa.
'
' History:
' --------
'  03/02/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_ads_dw_dp_pT_workingPair Lib "libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


'
' direct_ads_dw_dT_pT_workingPair:
' --------------------------------
'
' Calculates derivative of equilibrium loading w with respect to temperature
' T in kg/kg/K depending on equilibrium pressure p in Pa and equilibrium
' temperature T in K.
'
' Parameters:
' -----------
'  double p_Pa:
'      Equilibrium pressure in Pa.
'  double T_K:
'      Equilibrium temperature in K.
'
'  string path_db:
'      Path to database.
'  string wp_as:
'      Name of sorbent.
'  string wp_st:
'      Name of sub-type of sorbent.
'  string wp_rf:
'      Name of refrigerant.
'  string wp_iso:
'      Name of isotherm.
'  int no_iso:
'      Number of isotherm (i.e. when more than one isotherm is available)
'  string rf_psat:
'      Name of calculation approach for vapour pressure.
'  int no_p_sat:
'      Number of vapour pressure equation (i.e. when more than one equation is
'      available)
'  string rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      Number of liquid density equation (i.e. when more than one equation is
'      available)
'
' Returns:
' --------
'  double:
'      Derivative of equilibrium loading wrt. temperature in kg/kg/K.
'
' History:
' --------
'  03/02/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_ads_dw_dT_pT_workingPair Lib "libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


'
' direct_ads_dp_dw_wT_workingPair:
' --------------------------------
'
' Calculates derivative of equilibrium pressure p with respect to loading
' w in kgPa/kg depending on equilibrium loading w in kg/kg and equilibrium
' temperature T in K.
'
' Parameters:
' -----------
'  double w_kgkg:
'      Equilibrium loading in kg/kg.
'  double T_K:
'      Equilibrium temperature in K.
'
'  string path_db:
'      Path to database.
'  string wp_as:
'      Name of sorbent.
'  string wp_st:
'      Name of sub-type of sorbent.
'  string wp_rf:
'      Name of refrigerant.
'  string wp_iso:
'      Name of isotherm.
'  int no_iso:
'      Number of isotherm (i.e. when more than one isotherm is available)
'  string rf_psat:
'      Name of calculation approach for vapour pressure.
'  int no_p_sat:
'      Number of vapour pressure equation (i.e. when more than one equation is
'      available)
'  string rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      Number of liquid density equation (i.e. when more than one equation is
'      available)
'      Name of calculation approach for liquid density.
'
' Returns:
' --------
'  double:
'      Derivative of equilibrium pressure wrt. loading in Pakg/kg.
'
' History:
' --------
'  03/02/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_ads_dp_dw_wT_workingPair Lib "libsorpPropLib.dll" _
(ByVal w_kgkg As Double, ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


'
' direct_ads_dp_dT_wT_workingPair:
' --------------------------------
'
' Calculates derivative of equilibrium pressure p with respect to temperature
' T in kg/kg/K depending on equilibrium loading w in kg/kg and equilibrium
' temperature T in K.
'
' Parameters:
' -----------
'  double w_kgkg:
'      Equilibrium loading in kg/kg.
'  double T_K:
'      Equilibrium temperature in K.
'
'  string path_db:
'      Path to database.
'  string wp_as:
'      Name of sorbent.
'  string wp_st:
'      Name of sub-type of sorbent.
'  string wp_rf:
'      Name of refrigerant.
'  string wp_iso:
'      Name of isotherm.
'  int no_iso:
'      Number of isotherm (i.e. when more than one isotherm is available)
'  string rf_psat:
'      Name of calculation approach for vapour pressure.
'  int no_p_sat:
'      Number of vapour pressure equation (i.e. when more than one equation is
'      available)
'  string rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      Number of liquid density equation (i.e. when more than one equation is
'      available)
'      Name of calculation approach for liquid density.
'
' Returns:
' --------
'  double:
'      Derivative of equilibrium pressure wrt. loading in Pakg/kg.
'
' History:
' --------
'  03/02/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_ads_dp_dT_wT_workingPair Lib "libsorpPropLib.dll" _
(ByVal w_kgkg As Double, ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


'
' direct_ads_piStar_pyxgTM_workingPair:
' -------------------------------------
'
' Calculates reduced spreading pressure piStar in kg/mol depending on
' equilibrium pressure p in Pa, molar fraction of refrigerant in vapour phase
' in mol/mol, molar fraction of refrigerant in adsorbed phase in mol/mol,
' activity coefficient of refrigerant in adsorbed phase, equilibrium
' temperature T in K and molar mass of refrigerant M in kg/mol. The reduced
' spreading pressure is defined as follows:
'
'  piStar = A * pi / (R * T * m_sorbent) = 1 / M *
'      Integral_0^p0{w(p,T) / p * dp}
'
'  where p0 = p_total*y / (gamma*x)
'
' Parameters:
' -----------
'  double p_total_Pa:
'      Total pressure of vapour phase in Pa.
'  double y_molmol:
'      Molar fraction of refrigerant in vapour phase in mol/mol.
'  double x_molmol:
'      Molar fraction of refrigerant in adsorbed phase in mol/mol.
'  double gamma:
'      Activity coefficent of refrigerant in adsorbed phase.
'  double T_K:
'      Equilibrium temperature in K.
'  double M_kgmol:
'      Molar mass of refrigerant M in kg/mol.
'
'  string path_db:
'      Path to database.
'  string wp_as:
'      Name of sorbent.
'  string wp_st:
'      Name of sub-type of sorbent.
'  string wp_rf:
'      Name of refrigerant.
'  string wp_iso:
'      Name of isotherm.
'  int no_iso:
'      Number of isotherm (i.e. when more than one isotherm is available)
'  string rf_psat:
'      Name of calculation approach for vapour pressure.
'  int no_p_sat:
'      Number of vapour pressure equation (i.e. when more than one equation is
'      available)
'  string rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      Number of liquid density equation (i.e. when more than one equation is
'      available)
'      Name of calculation approach for liquid density.
'
' Returns:
' --------
'  double:
'      Derivative of equilibrium pressure wrt. loading in Pakg/kg.
'
' History:
' --------
'  03/02/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_ads_piStar_pyxgTM_workingPair Lib "libsorpPropLib.dll" _
(ByVal p_total_Pa As Double, ByVal y_molmol As Double, ByVal x_molmol As Double, _
 ByVal gamma As Double, ByVal T_K As Double, ByVal M_kgmol As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


'
' direct_ads_W_ARho_workingPair:
' ------------------------------
'
' Calculates equilibrium volumetric loading W in m³/kg depending on equilibrium
' adsorption potential A in J/mol and saturated liquid density of adsorpt rho_l
' in kg/m3.
'
' Parameters:
' -----------
'  double A_Jmol:
'      Equilibrium adsorption potential A in J/mol.
'  double rho_l_kgm3:
'      Saturated liquid density of adsorpt rho_l in kg/m3.
'
'  string path_db:
'      Path to database.
'  string wp_as:
'      Name of sorbent.
'  string wp_st:
'      Name of sub-type of sorbent.
'  string wp_rf:
'      Name of refrigerant.
'  string wp_iso:
'      Name of isotherm.
'  int no_iso:
'      Number of isotherm (i.e. when more than one isotherm is available)
'  string rf_psat:
'      Name of calculation approach for vapour pressure.
'  int no_p_sat:
'      Number of vapour pressure equation (i.e. when more than one equation is
'      available)
'  string rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      Number of liquid density equation (i.e. when more than one equation is
'      available)
'      Name of calculation approach for liquid density.
'
' Returns:
' --------
'  double:
'      Equilibrium volumetric loading W in m³/kg.
'
' Remarks:
' --------
'  Saturated liquid density of adsorpt is requird to calculate volumetric
'  loading from characteristic curve in m3/kg when coefficients would directly
'  return loading w in kg/kg. Therefore, function checks unit of coefficents
'  by checking isotherm coefficients.
'
' History:
' --------
'  03/02/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_ads_W_ARho_workingPair Lib "libsorpPropLib.dll" _
(ByVal A_Jmol As Double, ByVal rho_l_kgm3 As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


'
' direct_ads_A_WRho_workingPair:
' ------------------------------
'
' Calculates equilibrium adsorption potential A in J/mol depending on
' equilibrium volumetric loading W in m³/kg and saturated liquid density of
' adsorpt rho_l in kg/m3.
'
' Parameters:
' -----------
'  double W_m3kg:
'      Equilibrium volumetric loading W in m³/kg.
'  double rho_l_kgm3:
'      Saturated liquid density of adsorpt rho_l in kg/m3.
'
'  string path_db:
'      Path to database.
'  string wp_as:
'      Name of sorbent.
'  string wp_st:
'      Name of sub-type of sorbent.
'  string wp_rf:
'      Name of refrigerant.
'  string wp_iso:
'      Name of isotherm.
'  int no_iso:
'      Number of isotherm (i.e. when more than one isotherm is available)
'  string rf_psat:
'      Name of calculation approach for vapour pressure.
'  int no_p_sat:
'      Number of vapour pressure equation (i.e. when more than one equation is
'      available)
'  string rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      Number of liquid density equation (i.e. when more than one equation is
'      available)
'      Name of calculation approach for liquid density.
'
' Returns:
' --------
'  double:
'      Equilibrium volumetric loading W in m³/kg.
'
' Remarks:
' --------
'  Saturated liquid density of adsorpt is requird to calculate volumetric
'  loading from characteristic curve in m3/kg when coefficients would directly
'  return loading w in kg/kg.
'
' History:
' --------
'  03/02/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_ads_A_WRho_workingPair Lib "libsorpPropLib.dll" _
(ByVal W_m3kg As Double, ByVal rho_l_kgm3 As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


'
' direct_ads_dW_dA_ARho_workingPair:
' ----------------------------------
'
' Calculates derivative of equilibrium volumetric loading dW_dA in m³mol/kg/J
' depending on equilibrium adsorption potential A in J/mol and saturated liquid
' density of adsorpt rho_l in kg/m3.
'
' Parameters:
' -----------
'  double A_Jmol:
'      Equilibrium adsorption potential A in J/mol.
'  double rho_l_kgm3:
'      Saturated liquid density of adsorpt rho_l in kg/m3.
'
'  string path_db:
'      Path to database.
'  string wp_as:
'      Name of sorbent.
'  string wp_st:
'      Name of sub-type of sorbent.
'  string wp_rf:
'      Name of refrigerant.
'  string wp_iso:
'      Name of isotherm.
'  int no_iso:
'      Number of isotherm (i.e. when more than one isotherm is available)
'  string rf_psat:
'      Name of calculation approach for vapour pressure.
'  int no_p_sat:
'      Number of vapour pressure equation (i.e. when more than one equation is
'      available)
'  string rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      Number of liquid density equation (i.e. when more than one equation is
'      available)
'      Name of calculation approach for liquid density.
'
' Returns:
' --------
'  double:
'      Derivative of volumetric loading wrt. adsorption potential in
'      m³mol/kg/J.
'
' Remarks:
' --------
'  Saturated liquid density of adsorpt is requird to calculate volumetric
'  loading from characteristic curve in m3/kg when coefficients would directly
'  return loading w in kg/kg. Therefore, function checks unit of coefficents
'  by checking isotherm coefficients.
'
' History:
' --------
'  03/02/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_ads_dW_dA_ARho_workingPair Lib "libsorpPropLib.dll" _
(ByVal A_Jmol As Double, ByVal rho_l_kgm3 As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


'
' direct_ads_dA_dW_WRho_workingPair:
' ----------------------------------
'
' Calculates derivative of equilibrium adsorption potential dA_dW in kgJ/mol/m³
' depending on equilibrium volumetric loading W in m³/kg and saturated liquid
' density of adsorpt rho_l in kg/m3.
'
' Parameters:
' -----------
'  double W_m3kg:
'      Equilibrium volumetric loading W in m³/kg.
'  double rho_l_kgm3:
'      Saturated liquid density of adsorpt rho_l in kg/m3.
'
'  string path_db:
'      Path to database.
'  string wp_as:
'      Name of sorbent.
'  string wp_st:
'      Name of sub-type of sorbent.
'  string wp_rf:
'      Name of refrigerant.
'  string wp_iso:
'      Name of isotherm.
'  int no_iso:
'      Number of isotherm (i.e. when more than one isotherm is available)
'  string rf_psat:
'      Name of calculation approach for vapour pressure.
'  int no_p_sat:
'      Number of vapour pressure equation (i.e. when more than one equation is
'      available)
'  string rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      Number of liquid density equation (i.e. when more than one equation is
'      available)
'      Name of calculation approach for liquid density.
'
' Returns:
' --------
'  double:
'      Derivative of adsorption potential wrt. volumetric loading in
'      kgJ/mol/m³.
'
' Remarks:
' --------
'  Saturated liquid density of adsorpt is requird to calculate volumetric
'  loading from characteristic curve in m3/kg when coefficients would directly
'  return loading w in kg/kg.
'
' History:
' --------
'  03/02/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_ads_dA_dW_WRho_workingPair Lib "libsorpPropLib.dll" _
(ByVal W_m3kg As Double, ByVal rho_l_kgm3 As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double

'
' direct_ads_w_pTpsatRho_workingPair:
' -----------------------------------
'
' Calculates equilibrium loading w in kg/kg depending on equilibrium pressure
' p in Pa, equilibrium temperature T in K, saturation pressure p_Sat in Pa and
' saturated liquid density of adsorpt rho in kg/m³.
'
' Parameters:
' -----------
'  double p_Pa:
'      Equilibrium pressure in Pa.
'  double T_K:
'      Equilibrium temperature in K.
'  double p_sat_Pa:
'      Saturation pressure in Pa.
'  double rho_l_kgm3:
'      Saturated liquid density of adsorpt in kg/m³.
'
'  string path_db:
'      Path to database.
'  string wp_as:
'      Name of sorbent.
'  string wp_st:
'      Name of sub-type of sorbent.
'  string wp_rf:
'      Name of refrigerant.
'  string wp_iso:
'      Name of isotherm.
'  int no_iso:
'      Number of isotherm (i.e. when more than one isotherm is available)
'  string rf_psat:
'      Name of calculation approach for vapour pressure.
'  int no_p_sat:
'      Number of vapour pressure equation (i.e. when more than one equation is
'      available)
'  string rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      Number of liquid density equation (i.e. when more than one equation is
'      available)
'
' Returns:
' --------
'  double:
'      Equilibrium loading in kg/kg.
'
' History:
' --------
'  03/02/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_ads_w_pTpsatRho_workingPair Lib "libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal T_K As Double, ByVal p_sat_Pa As Double, ByVal rho_l_kgm3 As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


'
' direct_ads_p_wTpsatRho_workingPair:
' -----------------------------------
'
' Calculates equilibrium pressure p in Pa depending on equilibrium loading w in
' kg/kg, equilibrium temperature T in K, saturation pressure p_Sat in Pa and
' density of adsorpt rho in kg/m³.
'
' Parameters:
' -----------
'  double w_kgkg:
'      Equilibrium loading in kg/kg.
'  double T_K:
'      Equilibrium temperature in K.
'  double p_sat_Pa:
'      Saturation pressure in Pa.
'  double rho_l_kgm3:
'      Saturated liquid density of adsorpt in kg/m³.
'
'  string path_db:
'      Path to database.
'  string wp_as:
'      Name of sorbent.
'  string wp_st:
'      Name of sub-type of sorbent.
'  string wp_rf:
'      Name of refrigerant.
'  string wp_iso:
'      Name of isotherm.
'  int no_iso:
'      Number of isotherm (i.e. when more than one isotherm is available)
'  string rf_psat:
'      Name of calculation approach for vapour pressure.
'  int no_p_sat:
'      Number of vapour pressure equation (i.e. when more than one equation is
'      available)
'  string rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      Number of liquid density equation (i.e. when more than one equation is
'      available)
'      Name of calculation approach for liquid density.
'
' Returns:
' --------
'  double:
'      Equilibrium pressure in Pa.
'
' History:
' --------
'  03/02/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_ads_p_wTpsatRho_workingPair Lib "libsorpPropLib.dll" _
(ByVal w_kgkg As Double, ByVal T_K As Double, ByVal p_sat_Pa As Double, ByVal rho_l_kgm3 As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


'
' direct_ads_T_pwpsatRho_workingPair:
' -----------------------------------
'
' Calculates equilibrium temperature T in K depending on equilibrium pressure
' in Pa, equilibrium loading w in kg/kg, saturation pressure p_Sat in Pa and
' density of adsorpt rho in kg/m³.
'
' Parameters:
' -----------
'  double p_Pa:
'      Equilibrium pressure in Pa.
'  double w_kgkg:
'      Equilibrium loading in kg/kg.
'  double p_sat_Pa:
'      Saturation pressure in Pa.
'  double rho_l_kgm3:
'      Saturated liquid density of adsorpt in kg/m³.
'
'  string path_db:
'      Path to database.
'  string wp_as:
'      Name of sorbent.
'  string wp_st:
'      Name of sub-type of sorbent.
'  string wp_rf:
'      Name of refrigerant.
'  string wp_iso:
'      Name of isotherm.
'  int no_iso:
'      Number of isotherm (i.e. when more than one isotherm is available)
'  string rf_psat:
'      Name of calculation approach for vapour pressure.
'  int no_p_sat:
'      Number of vapour pressure equation (i.e. when more than one equation is
'      available)
'  string rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      Number of liquid density equation (i.e. when more than one equation is
'      available)
'      Name of calculation approach for liquid density.
'
' Returns:
' --------
'  double:
'      Equilibrium temperature in K.
'
' Remarks:
' --------
'  Uses internal euqation of states to calculate vapour pressure, saturated
'  liquid density of adsorpt, derivative of vapour pressure wrt. temperature
'  and derivative of saturated liquid density of adsorpt wrt. temperature. If
'  equation of states are not implemented for refrigerant, function returns -1
'  and throws warning. If user want to use his own equation of states, this
'  function cannot be used and determination of root to calculate T must
'  be implemented by user.
'
' History:
' --------
'  03/02/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_ads_T_pwpsatRho_workingPair Lib "libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal w_kgkg As Double, ByVal p_sat_Pa As Double, ByVal rho_l_kgm3 As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
 

'
' direct_ads_dw_dp_pTpsatRho_workingPair:
' ---------------------------------------
'
' Calculates derivative of equilibrium loading dw_dp with respect to pressure
' in kg/kg/Pa depending on equilibrium pressure p in Pa, equilibrium
' temperature T in K, saturation pressure p_Sat in Pa and density of adsorpt
' rho in kg/m³.
'
' Parameters:
' -----------
'  double p_Pa:
'      Equilibrium pressure in Pa.
'  double T_K:
'      Equilibrium temperature in K.
'  double p_sat_Pa:
'      Saturation pressure in Pa.
'  double rho_l_kgm3:
'      Saturated liquid density of adsorpt in kg/m³.
'
'  string path_db:
'      Path to database.
'  string wp_as:
'      Name of sorbent.
'  string wp_st:
'      Name of sub-type of sorbent.
'  string wp_rf:
'      Name of refrigerant.
'  string wp_iso:
'      Name of isotherm.
'  int no_iso:
'      Number of isotherm (i.e. when more than one isotherm is available)
'  string rf_psat:
'      Name of calculation approach for vapour pressure.
'  int no_p_sat:
'      Number of vapour pressure equation (i.e. when more than one equation is
'      available)
'  string rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      Number of liquid density equation (i.e. when more than one equation is
'      available)
'
' Returns:
' --------
'  double:
'      Derivative of equilibrium loading wrt. pressure in kg/kg/Pa.
'
' History:
' --------
'  03/02/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_ads_dw_dp_pTpsatRho_workingPair Lib "libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal T_K As Double, ByVal p_sat_Pa As Double, ByVal rho_l_kgm3 As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


'
' direct_ads_dw_dT_pTpsatRho_workingPair:
' ---------------------------------------
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
'  double p_Pa:
'      Equilibrium pressure in Pa.
'  double T_K:
'      Equilibrium temperature in K.
'  double p_sat_Pa:
'      Saturation pressure in Pa.
'  double rho_l_kgm3:
'      Saturated liquid density of adsorpt in kg/m³.
'  double dp_sat_dT_PaK:
'      Derivative of saturation pressure wrt. temperature in Pa/K.
'  double drho_dT_kgm3K:
'      Derivative of density of adsorpt wrt. temperature in kg/m³/K.
'
'  string path_db:
'      Path to database.
'  string wp_as:
'      Name of sorbent.
'  string wp_st:
'      Name of sub-type of sorbent.
'  string wp_rf:
'      Name of refrigerant.
'  string wp_iso:
'      Name of isotherm.
'  int no_iso:
'      Number of isotherm (i.e. when more than one isotherm is available)
'  string rf_psat:
'      Name of calculation approach for vapour pressure.
'  int no_p_sat:
'      Number of vapour pressure equation (i.e. when more than one equation is
'      available)
'  string rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      Number of liquid density equation (i.e. when more than one equation is
'      available)
'
' Returns:
' --------
'  double:
'      Derivative of equilibrium loading wrt. temperature in kg/kg/K.
'
' History:
' --------
'  03/02/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_ads_dw_dT_pTpsatRho_workingPair Lib "libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal T_K As Double, ByVal p_sat_Pa As Double, ByVal rho_l_kgm3 As Double, _
 ByVal dp_sat_dT_PaK As Double, ByVal drho_dT_kgm3K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


'
' direct_ads_dp_dw_wTpsatRho_workingPair:
' ---------------------------------------
'
' Calculates derivative of equilibrium pressure p with respect to loading
' w in kgPa/kg depending on equilibrium loading w in kg/kg, equilibrium
' temperature T in K, saturation pressure p_Sat in Pa and density of adsorpt
' rho in kg/m³.
'
' Parameters:
' -----------
'  double w_kgkg:
'      Equilibrium loading in kg/kg.
'  double T_K:
'      Equilibrium temperature in K.
'  double p_sat_Pa:
'      Saturation pressure in Pa.
'  double rho_l_kgm3:
'      Saturated liquid density of adsorpt in kg/m³.
'
'  string path_db:
'      Path to database.
'  string wp_as:
'      Name of sorbent.
'  string wp_st:
'      Name of sub-type of sorbent.
'  string wp_rf:
'      Name of refrigerant.
'  string wp_iso:
'      Name of isotherm.
'  int no_iso:
'      Number of isotherm (i.e. when more than one isotherm is available)
'  string rf_psat:
'      Name of calculation approach for vapour pressure.
'  int no_p_sat:
'      Number of vapour pressure equation (i.e. when more than one equation is
'      available)
'  string rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      Number of liquid density equation (i.e. when more than one equation is
'      available)
'      Name of calculation approach for liquid density.
'
' Returns:
' --------
'  double:
'      Derivative of equilibrium pressure wrt. loading in Pakg/kg.
'
' History:
' --------
'  03/02/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_ads_dp_dw_wTpsatRho_workingPair Lib "libsorpPropLib.dll" _
(ByVal w_kgkg As Double, ByVal T_K As Double, ByVal p_sat_Pa As Double, ByVal rho_l_kgm3 As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


'
' direct_ads_dp_dT_wTpsatRho_workingPair:
' ---------------------------------------
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
'  double w_kgkg:
'      Equilibrium loading in kg/kg.
'  double T_K:
'      Equilibrium temperature in K.
'  double p_sat_Pa:
'      Saturation pressure in Pa.
'  double rho_l_kgm3:
'      Saturated liquid density of adsorpt in kg/m³.
'  double dp_sat_dT_PaK:
'      Derivative of saturation pressure wrt. temperature in Pa/K.
'  double drho_dT_kgm3K:
'      Derivative of density of adsorpt wrt. temperature in kg/m³/K.
'
'  string path_db:
'      Path to database.
'  string wp_as:
'      Name of sorbent.
'  string wp_st:
'      Name of sub-type of sorbent.
'  string wp_rf:
'      Name of refrigerant.
'  string wp_iso:
'      Name of isotherm.
'  int no_iso:
'      Number of isotherm (i.e. when more than one isotherm is available)
'  string rf_psat:
'      Name of calculation approach for vapour pressure.
'  int no_p_sat:
'      Number of vapour pressure equation (i.e. when more than one equation is
'      available)
'  string rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      Number of liquid density equation (i.e. when more than one equation is
'      available)
'      Name of calculation approach for liquid density.
'
' Returns:
' --------
'  double:
'      Derivative of equilibrium pressure wrt. loading in Pakg/kg.
'
' History:
' --------
'  03/02/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_ads_dp_dT_wTpsatRho_workingPair Lib "libsorpPropLib.dll" _
(ByVal w_kgkg As Double, ByVal T_K As Double, ByVal p_sat_Pa As Double, ByVal rho_l_kgm3 As Double, _
 ByVal dp_sat_dT_PaK As Double, ByVal drho_dT_kgm3K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


'
' direct_ads_piStar_pyxgTpsatRhoM_workingPair:
' --------------------------------------------
'
' Calculates reduced spreading pressure in kg/mol depending on equilibrium
' pressure p in Pa, molar fraction of refrigerant in vapour phase in mol/mol,
' molar fraction of refrigerant in adsorbed phase in mol/mol, activity
' coefficient of refrigerant in adsorbed phase, equilibrium temperature T in K,
' saturation pressure p_Sat in Pa and density of adsorpt rho in kg/m³ and molar
' mass of refrigerant M in kg/mol. The reduced spreading pressure is defined as
' follows:
'
'  piStar = A * pi / (R * T * m_sorbent) = 1 / M *
'      Integral_0^p0{w(p,T) / p * dp}
'
'  where p0 = p_total*y / (gamma*x)
'
' Parameters:
' -----------
'  double p_total_Pa:
'      Total pressure of vapour phase in Pa.
'  double y_molmol:
'      Molar fraction of refrigerant in vapour phase in mol/mol.
'  double x_molmol:
'      Molar fraction of refrigerant in adsorbed phase in mol/mol.
'  double gamma:
'      Activity coefficent of refrigerant in adsorbed phase.
'  double T_K:
'      Equilibrium temperature in K.
'  double p_sat_Pa:
'      Saturation pressure in Pa.
'  double rho_l_kgm3:
'      Saturated liquid density of adsorpt in kg/m³.
'  double M_kgmol:
'      Molar mass of refrigerant M in kg/mol.
'
'  string path_db:
'      Path to database.
'  string wp_as:
'      Name of sorbent.
'  string wp_st:
'      Name of sub-type of sorbent.
'  string wp_rf:
'      Name of refrigerant.
'  string wp_iso:
'      Name of isotherm.
'  int no_iso:
'      Number of isotherm (i.e. when more than one isotherm is available)
'  string rf_psat:
'      Name of calculation approach for vapour pressure.
'  int no_p_sat:
'      Number of vapour pressure equation (i.e. when more than one equation is
'      available)
'  string rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      Number of liquid density equation (i.e. when more than one equation is
'      available)
'      Name of calculation approach for liquid density.
'
' Returns:
' --------
'  double:
'      Derivative of equilibrium pressure wrt. loading in Pakg/kg.
'
' History:
' --------
'  03/02/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_ads_piStar_pyxgTpsatRhoM_workingPair Lib "libsorpPropLib.dll" _
(ByVal p_total_Pa As Double, ByVal y_molmol As Double, ByVal x_molmol As Double, ByVal gamma As Double, _
 ByVal T_K As Double, ByVal p_sat_Pa As Double, ByVal rho_l_kgm3 As Double, ByVal M_kgmol As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


'
' direct_abs_X_pT_workingPair:
' ----------------------------
'
' Calculates equilibrium concentration X in kg/kg depending on equilibrium
' pressure p in Pa and equilibrium temperature T in K.
'
' Parameters:
' -----------
'  double p_Pa:
'      Equilibrium pressure in Pa.
'  double T_K:
'      Equilibrium temperature in K.
'
'  string path_db:
'      Path to database.
'  string wp_as:
'      Name of sorbent.
'  string wp_st:
'      Name of sub-type of sorbent.
'  string wp_rf:
'      Name of refrigerant.
'  string wp_iso:
'      Name of isotherm.
'  int no_iso:
'      Number of isotherm (i.e. when more than one isotherm is available)
'  string rf_psat:
'      Name of calculation approach for vapour pressure.
'  int no_p_sat:
'      Number of vapour pressure equation (i.e. when more than one equation is
'      available)
'  string rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      Number of liquid density equation (i.e. when more than one equation is
'      available)
'
' Returns:
' --------
'  double:
'      Equilibrium concentration in kg/kg.
'
' History:
' --------
'  03/04/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_abs_X_pT_workingPair Lib "libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


'
' direct_abs_p_XT_workingPair:
' ----------------------------
'
' Calculates equilibrium pressure p in Pa depending on equilibrium
' concentration X in kg/kg and equilibrium temperature T in K.
'
' Parameters:
' -----------
'  double X_kgkg:
'      Equilibrium concentration in kg/kg.
'  double T_K:
'      Equilibrium temperature in K.
'
'  string path_db:
'      Path to database.
'  string wp_as:
'      Name of sorbent.
'  string wp_st:
'      Name of sub-type of sorbent.
'  string wp_rf:
'      Name of refrigerant.
'  string wp_iso:
'      Name of isotherm.
'  int no_iso:
'      Number of isotherm (i.e. when more than one isotherm is available)
'  string rf_psat:
'      Name of calculation approach for vapour pressure.
'  int no_p_sat:
'      Number of vapour pressure equation (i.e. when more than one equation is
'      available)
'  string rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      Number of liquid density equation (i.e. when more than one equation is
'      available)
'
' Returns:
' --------
'  double:
'      Equilibrium pressure in Pa.
'
' History:
' --------
'  03/04/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_abs_p_XT_workingPair Lib "libsorpPropLib.dll" _
(ByVal X_kgkg As Double, ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


'
' direct_abs_T_pX_workingPair:
' ----------------------------
'
' Calculates equilibrium temperature in K depending on equilibrium pressure p
' in Pa and equilibrium concentration X in kg/kg.
'
' Parameters:
' -----------
'  double p_Pa:
'      Equilibrium pressure in Pa.
'  double X_kgkg:
'      Equilibrium concentration in kg/kg.
'
'  string path_db:
'      Path to database.
'  string wp_as:
'      Name of sorbent.
'  string wp_st:
'      Name of sub-type of sorbent.
'  string wp_rf:
'      Name of refrigerant.
'  string wp_iso:
'      Name of isotherm.
'  int no_iso:
'      Number of isotherm (i.e. when more than one isotherm is available)
'  string rf_psat:
'      Name of calculation approach for vapour pressure.
'  int no_p_sat:
'      Number of vapour pressure equation (i.e. when more than one equation is
'      available)
'  string rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      Number of liquid density equation (i.e. when more than one equation is
'      available)
'
' Returns:
' --------
'  double:
'      Equilibrium temperature in K.
'
' History:
' --------
'  03/04/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_abs_T_pX_workingPair Lib "libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal X_kgkg As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


'
' direct_abs_dX_dp_pT_workingPair:
' --------------------------------
'
' Calculates derivative of equilibrium concentration X with respect to pressure
' p in kg/kg/Pa depending on equilibrium pressure p in Pa and equilibrium
' temperature T in K.
'
' Parameters:
' -----------
'  double p_Pa:
'      Equilibrium pressure in Pa.
'  double T_K:
'      Equilibrium temperature in K.
'
'  string path_db:
'      Path to database.
'  string wp_as:
'      Name of sorbent.
'  string wp_st:
'      Name of sub-type of sorbent.
'  string wp_rf:
'      Name of refrigerant.
'  string wp_iso:
'      Name of isotherm.
'  int no_iso:
'      Number of isotherm (i.e. when more than one isotherm is available)
'  string rf_psat:
'      Name of calculation approach for vapour pressure.
'  int no_p_sat:
'      Number of vapour pressure equation (i.e. when more than one equation is
'      available)
'  string rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      Number of liquid density equation (i.e. when more than one equation is
'      available)
'
' Returns:
' --------
'  double:
'      Derivative of equilibrium concentration wrt. pressure in kg/kg/Pa.
'
' History:
' --------
'  03/04/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_abs_dX_dp_pT_workingPair Lib "libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


'
' direct_abs_dX_dT_pT_workingPair:
' --------------------------------
'
' Calculates derivative of equilibrium concentration X with respect to
' temperature T in kg/kg/K depending on equilibrium pressure p in Pa and
' equilibrium temperature T in K.
'
' Parameters:
' -----------
'  double p_Pa:
'      Equilibrium pressure in Pa.
'  double T_K:
'      Equilibrium temperature in K.
'
'  string path_db:
'      Path to database.
'  string wp_as:
'      Name of sorbent.
'  string wp_st:
'      Name of sub-type of sorbent.
'  string wp_rf:
'      Name of refrigerant.
'  string wp_iso:
'      Name of isotherm.
'  int no_iso:
'      Number of isotherm (i.e. when more than one isotherm is available)
'  string rf_psat:
'      Name of calculation approach for vapour pressure.
'  int no_p_sat:
'      Number of vapour pressure equation (i.e. when more than one equation is
'      available)
'  string rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      Number of liquid density equation (i.e. when more than one equation is
'      available)
'
' Returns:
' --------
'  double:
'      Derivative of equilibrium concentration wrt. temperature in kg/kg/K.
'
' History:
' --------
'  03/04/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_abs_dX_dT_pT_workingPair Lib "libsorpPropLib.dll" _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


'
' direct_abs_dp_dX_XT_workingPair:
' --------------------------------
'
' Calculates derivative of equilibrium pressure p with respect to concentration
' w in kgPa/kg depending on equilibrium concentration X in kg/kg and
' equilibrium temperature T in K.
'
' Parameters:
' -----------
'  double X_kgkg:
'      Equilibrium concentration in kg/kg.
'  double T_K:
'      Equilibrium temperature in K.
'
'  string path_db:
'      Path to database.
'  string wp_as:
'      Name of sorbent.
'  string wp_st:
'      Name of sub-type of sorbent.
'  string wp_rf:
'      Name of refrigerant.
'  string wp_iso:
'      Name of isotherm.
'  int no_iso:
'      Number of isotherm (i.e. when more than one isotherm is available)
'  string rf_psat:
'      Name of calculation approach for vapour pressure.
'  int no_p_sat:
'      Number of vapour pressure equation (i.e. when more than one equation is
'      available)
'  string rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      Number of liquid density equation (i.e. when more than one equation is
'      available)
'
' Returns:
' --------
'  double:
'      Derivative of equilibrium pressure wrt. concentration in Pakg/kg.
'
' History:
' --------
'  03/04/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_abs_dp_dX_XT_workingPair Lib "libsorpPropLib.dll" _
(ByVal X_kgkg As Double, ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


'
' direct_abs_dp_dT_XT_workingPair:
' --------------------------------
'
' Calculates derivative of equilibrium pressure p with respect to temperature
' T in kg/kg/K depending on equilibrium concentration X in kg/kg and
' equilibrium temperature T in K.
'
' Parameters:
' -----------
'  double X_kgkg:
'      Equilibrium concentration in kg/kg.
'  double T_K:
'      Equilibrium temperature in K.
'
'  string path_db:
'      Path to database.
'  string wp_as:
'      Name of sorbent.
'  string wp_st:
'      Name of sub-type of sorbent.
'  string wp_rf:
'      Name of refrigerant.
'  string wp_iso:
'      Name of isotherm.
'  int no_iso:
'      Number of isotherm (i.e. when more than one isotherm is available)
'  string rf_psat:
'      Name of calculation approach for vapour pressure.
'  int no_p_sat:
'      Number of vapour pressure equation (i.e. when more than one equation is
'      available)
'  string rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      Number of liquid density equation (i.e. when more than one equation is
'      available)
'
' Returns:
' --------
'  double:
'      Derivative of equilibrium pressure wrt. temperature in Pa/K.
'
' History:
' --------
'  03/04/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_abs_dp_dT_XT_workingPair Lib "libsorpPropLib.dll" _
(ByVal X_kgkg As Double, ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


'
' direct_abs_g_Txv1v2_workingPair:
' --------------------------------
'
' Calculates activity coefficient of first component depending on temperature
' T_K in K, mole fraction in liquid phase x_molmol in mol/mol, molar volume of
' first component in m³/mol, and molar volume of second component in m³/mol.
'
' Parameters:
' -----------
'  double T_K:
'      Equilibrium temperature in K.
'  double x_molmol:
'      Equilibrium mole fraction in liquid phase in mol/mol.
'  double v1_m3mol:
'      Equilibrium molar volume of first component in m³/mol.
'  double v2_m3mol:
'      Equilibrium molar volume of second component in m³/mol.
'
'  const string path_db:
'      Path to database.
'  const string wp_as:
'      Name of sorbent.
'  const string wp_st:
'      Name of sub-type of sorbent.
'  const string wp_rf:
'      Name of refrigerant.
'  const string wp_iso:
'      Name of isotherm.
'  int no_iso:
'      Number of isotherm (i.e. when more than one isotherm is available)
'  const string rf_psat:
'      Name of calculation approach for vapour pressure.
'  int no_p_sat:
'      Number of vapour pressure equation (i.e. when more than one equation is
'      available)
'  const string rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      Number of liquid density equation (i.e. when more than one equation is
'      available)
'
' Returns:
' --------
'  double:
'      Activity coefficient of first component.
'
' Remarks:
' --------
'  Molar volumes may are not required and ignored. When molar volumes are
'  required, uses molar volumes stored in JSON file when input v1_m3mol or
'  v2_m3mol is -1.
'
' History:
' --------
'  03/05/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_abs_g_Txv1v2_workingPair Lib "libsorpPropLib.dll" _
(ByVal T_K As Double, ByVal x_molmol As Double, ByVal v1_m3mol As Double, ByVal v2_m3mol As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


'
' direct_abs_p_Txv1v2psat_workingPair:
' ------------------------------------
'
' Calculates equilibrium pressure p_Pa in Pa of first component depending on
' temperature T_K in K, mole fraction in liquid phase x_molmol in mol/mol,
' molar volume of first component in m³/mol, molar volume of second component
' in m³/mol,and saturation pressure of first component p_sat_Pa in Pa.
'
' Parameters:
' -----------
'  double T_K:
'      Equilibrium temperature in K.
'  double x_molmol:
'      Equilibrium mole fraction in liquid phase in mol/mol.
'  double v1_m3mol:
'      Equilibrium molar volume of first component in m³/mol.
'  double v2_m3mol:
'      Equilibrium molar volume of second component in m³/mol.
'
'  const string path_db:
'      Path to database.
'  const string wp_as:
'      Name of sorbent.
'  const string wp_st:
'      Name of sub-type of sorbent.
'  const string wp_rf:
'      Name of refrigerant.
'  const string wp_iso:
'      Name of isotherm.
'  int no_iso:
'      Number of isotherm (i.e. when more than one isotherm is available)
'  const string rf_psat:
'      Name of calculation approach for vapour pressure.
'  int no_p_sat:
'      Number of vapour pressure equation (i.e. when more than one equation is
'      available)
'  const string rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      Number of liquid density equation (i.e. when more than one equation is
'      available)
'
' Returns:
' --------
'  double:
'      Equilibrium pressure p_Pa in Pa.
'
' Remarks:
' --------
'  Molar volumes may are not required and ignored. When molar volumes are
'  required, uses molar volumes stored in JSON file when input v1_m3mol or
'  v2_m3mol is -1.
'
' History:
' --------
'  03/05/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_abs_p_Txv1v2psat_workingPair Lib "libsorpPropLib.dll" _
(ByVal T_K As Double, ByVal x_molmol As Double, ByVal v1_m3mol As Double, ByVal v2_m3mol As Double, ByVal p_sat_Pa As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


'
' direct_abs_p_Txv1v2_workingPair:
' --------------------------------
'
' Calculates equilibrium pressure p_Pa in Pa of first component depending on
' temperature T_K in K, mole fraction in liquid phase x_molmol in mol/mol,
' molar volume of first component in m³/mol and molar volume of second component
' in m³/mol.
'
' Parameters:
' -----------
'  double T_K:
'      Equilibrium temperature in K.
'  double x_molmol:
'      Equilibrium mole fraction in liquid phase in mol/mol.
'  double v1_m3mol:
'      Equilibrium molar volume of first component in m³/mol.
'  double v2_m3mol:
'      Equilibrium molar volume of second component in m³/mol.
'
'  const string path_db:
'      Path to database.
'  const string wp_as:
'      Name of sorbent.
'  const string wp_st:
'      Name of sub-type of sorbent.
'  const string wp_rf:
'      Name of refrigerant.
'  const string wp_iso:
'      Name of isotherm.
'  int no_iso:
'      Number of isotherm (i.e. when more than one isotherm is available)
'  const string rf_psat:
'      Name of calculation approach for vapour pressure.
'  int no_p_sat:
'      Number of vapour pressure equation (i.e. when more than one equation is
'      available)
'  const string rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      Number of liquid density equation (i.e. when more than one equation is
'      available)
'
' Returns:
' --------
'  double:
'      Equilibrium pressure p_Pa in Pa.
'
' Remarks:
' --------
'  Molar volumes may are not required and ignored. When molar volumes are
'  required, uses molar volumes stored in JSON file when input v1_m3mol or
'  v2_m3mol is -1.
'
' History:
' --------
'  03/05/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_abs_p_Txv1v2_workingPair Lib "libsorpPropLib.dll" _
(ByVal T_K As Double, ByVal x_molmol As Double, ByVal v1_m3mol As Double, ByVal v2_m3mol As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


'
' direct_abs_p_Tvx_workingPair:
' -----------------------------
'
' Calculates equilibrium pressure p_Pa in Pa of first component depending on
' temperature T_K in K, molar mixing volume v_m3mol in m³/mol, and mole
' fraction in liquid phase x_molmol in mol/mol
'
' Parameters:
' -----------
'  double T_K:
'      Equilibrium temperature in K.
'  double v_m3mol:
'      Molar mixing volume in m³/mol.
'  double x_molmol:
'      Equilibrium mole fraction in liquid phase in mol/mol.
'
'  const string path_db:
'      Path to database.
'  const string wp_as:
'      Name of sorbent.
'  const string wp_st:
'      Name of sub-type of sorbent.
'  const string wp_rf:
'      Name of refrigerant.
'  const string wp_iso:
'      Name of isotherm.
'  int no_iso:
'      Number of isotherm (i.e. when more than one isotherm is available)
'  const string rf_psat:
'      Name of calculation approach for vapour pressure.
'  int no_p_sat:
'      Number of vapour pressure equation (i.e. when more than one equation is
'      available)
'  const string rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      Number of liquid density equation (i.e. when more than one equation is
'      available)
'
' Returns:
' --------
'  double:
'      Equilibrium pressure p_Pa in Pa.
'
' History:
' --------
'  03/05/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_abs_p_Tvx_workingPair Lib "libsorpPropLib.dll" _
(ByVal T_K As Double, ByVal v_m3mol As Double, ByVal x_molmol As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double


'
' direct_ref_p_sat_T_workingPair:
' -------------------------------
'
' Calculates saturation pressure in Pa depending on equilibrium temperature T
' in K.
'
' Parameters:
' -----------
'  double p_Pa:
'      Equilibrium pressure in Pa.
'  double T_K:
'      Equilibrium temperature in K.
'
'  string path_db:
'      Path to database.
'  string wp_as:
'      Name of sorbent.
'  string wp_st:
'      Name of sub-type of sorbent.
'  string wp_rf:
'      Name of refrigerant.
'  string wp_iso:
'      Name of isotherm.
'  int no_iso:
'      Number of isotherm (i.e. when more than one isotherm is available)
'  string rf_psat:
'      Name of calculation approach for vapour pressure.
'  int no_p_sat:
'      Number of vapour pressure equation (i.e. when more than one equation is
'      available)
'  string rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      Number of liquid density equation (i.e. when more than one equation is
'      available)
'
' Returns:
' --------
'  double:
'      Saturation pressure in Pa.
'
' History:
' --------
'  03/02/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_ref_p_sat_T_workingPair Lib "libsorpPropLib.dll" _
(ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
 

'
' direct_ref_dp_sat_dT_T_workingPair:
' -----------------------------------
'
' Calculates derivative of saturation pressure in Pa/K wrt to temperature
' depending on equilibrium temperature T in K.
'
' Parameters:
' -----------
'  double p_Pa:
'      Equilibrium pressure in Pa.
'  double T_K:
'      Equilibrium temperature in K.
'
'  string path_db:
'      Path to database.
'  string wp_as:
'      Name of sorbent.
'  string wp_st:
'      Name of sub-type of sorbent.
'  string wp_rf:
'      Name of refrigerant.
'  string wp_iso:
'      Name of isotherm.
'  int no_iso:
'      Number of isotherm (i.e. when more than one isotherm is available)
'  string rf_psat:
'      Name of calculation approach for vapour pressure.
'  int no_p_sat:
'      Number of vapour pressure equation (i.e. when more than one equation is
'      available)
'  string rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      Number of liquid density equation (i.e. when more than one equation is
'      available)
'
' Returns:
' --------
'  double:
'      Derivative of saturation pressure wrt temperature in Pa/K.
'
' History:
' --------
'  03/02/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_ref_dp_sat_dT_T_workingPair Lib "libsorpPropLib.dll" _
(ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
 

'
' direct_ref_rho_l_T_workingPair:
' -------------------------------
'
' Calculates saturated liquid density in kg/m³ depending on equilibrium
' temperature T in K.
'
' Parameters:
' -----------
'  double p_Pa:
'      Equilibrium pressure in Pa.
'  double T_K:
'      Equilibrium temperature in K.
'
'  string path_db:
'      Path to database.
'  string wp_as:
'      Name of sorbent.
'  string wp_st:
'      Name of sub-type of sorbent.
'  string wp_rf:
'      Name of refrigerant.
'  string wp_iso:
'      Name of isotherm.
'  int no_iso:
'      Number of isotherm (i.e. when more than one isotherm is available)
'  string rf_psat:
'      Name of calculation approach for vapour pressure.
'  int no_p_sat:
'      Number of vapour pressure equation (i.e. when more than one equation is
'      available)
'  string rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      Number of liquid density equation (i.e. when more than one equation is
'      available)
'
' Returns:
' --------
'  double:
'      Saturated liquid density in kg/m³.
'
' History:
' --------
'  03/02/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_ref_rho_l_T_workingPair Lib "libsorpPropLib.dll" _
(ByVal T_K As Double, _
 ByVal path_db As String, ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
 

'
' direct_ref_drho_l_dT_T_workingPair:
' -----------------------------------
'
' Calculates derivative of saturated liquid density wrt temperature in kg/m³/K
' depending on equilibrium temperature T in K.
'
' Parameters:
' -----------
'  double p_Pa:
'      Equilibrium pressure in Pa.
'  double T_K:
'      Equilibrium temperature in K.
'
'  string path_db:
'      Path to database.
'  string wp_as:
'      Name of sorbent.
'  string wp_st:
'      Name of sub-type of sorbent.
'  string wp_rf:
'      Name of refrigerant.
'  string wp_iso:
'      Name of isotherm.
'  int no_iso:
'      Number of isotherm (i.e. when more than one isotherm is available)
'  string rf_psat:
'      Name of calculation approach for vapour pressure.
'  int no_p_sat:
'      Number of vapour pressure equation (i.e. when more than one equation is
'      available)
'  string rf_rhol:
'      Name of calculation approach for liquid density.
'  int no_rhol:
'      Number of liquid density equation (i.e. when more than one equation is
'      available)
'
' Returns:
' --------
'  double:
'      Derivative of saturated liquid density wrt temperature in kg/m³/K.
'
' History:
' --------
'  03/02/2020, by Mirko Engelpracht:
'      First implementation.
'
Private Declare PtrSafe Function direct_ref_drho_l_dT_T_workingPair Lib "libsorpPropLib.dll" _
(ByVal T_K As Double, _
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
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_ads_w_pT = direct_ads_w_pT_workingPair(p_Pa, T_K, _
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
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_ads_p_wT = direct_ads_p_wT_workingPair(w_kgkg, T_K, _
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
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_ads_T_pw = direct_ads_T_pw_workingPair(p_Pa, w_kgkg, _
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
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_ads_dw_dp_pT = direct_ads_dw_dp_pT_workingPair(p_Pa, T_K, _
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
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_ads_dw_dT_pT = direct_ads_dw_dT_pT_workingPair(p_Pa, T_K, _
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
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_ads_dp_dw_wT = direct_ads_dp_dw_wT_workingPair(w_kgkg, T_K, _
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
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_ads_dp_dT_wT = direct_ads_dp_dT_wT_workingPair(w_kgkg, T_K, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)
        
End Function

' ws_direct_ads_piStar_pyxgTM
'
Function ws_direct_ads_piStar_pyxgTM _
(ByVal p_total_Pa As Double, ByVal y_molmol As Double, ByVal x_molmol As Double, _
 ByVal gamma As Double, ByVal T_K As Double, ByVal M_kgmol As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_ads_piStar_pyxgTM = direct_ads_piStar_pyxgTM_workingPair(p_total_Pa, y_molmol, x_molmol, _
        gamma, T_K, M_kgmol, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)
        
End Function

' ws_direct_ads_W_ARho
'
Function ws_direct_ads_W_ARho _
(ByVal A_Jmol As Double, ByVal rho_l_kgm3 As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_ads_W_ARho = direct_ads_W_ARho_workingPair(A_Jmol, rho_l_kgm3, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)
        
End Function

' ws_direct_ads_A_WRho
'
Function ws_direct_ads_A_WRho _
(ByVal W_m3kg As Double, ByVal rho_l_kgm3 As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_ads_A_WRho = direct_ads_A_WRho_workingPair(W_m3kg, rho_l_kgm3, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)
        
End Function

' ws_direct_ads_dW_dA_ARho
'
Function ws_direct_ads_dW_dA_ARho _
(ByVal A_Jmol As Double, ByVal rho_l_kgm3 As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_ads_dW_dA_ARho = direct_ads_dW_dA_ARho_workingPair(A_Jmol, rho_l_kgm3, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)
        
End Function

' ws_direct_ads_dA_dW_WRho
'
Function ws_direct_ads_dA_dW_WRho _
(ByVal W_m3kg As Double, ByVal rho_l_kgm3 As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_ads_dA_dW_WRho = direct_ads_dA_dW_WRho_workingPair(W_m3kg, rho_l_kgm3, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)
        
End Function

' ws_direct_ads_w_pTpsatRho
'
Function ws_direct_ads_w_pTpsatRho _
(ByVal p_Pa As Double, ByVal T_K As Double, ByVal p_sat_Pa As Double, ByVal rho_l_kgm3 As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_ads_w_pTpsatRho = direct_ads_w_pTpsatRho_workingPair(p_Pa, T_K, p_sat_Pa, rho_l_kgm3, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)
        
End Function

' ws_direct_ads_p_wTpsatRho
'
Function ws_direct_ads_p_wTpsatRho _
(ByVal w_kgkg As Double, ByVal T_K As Double, ByVal p_sat_Pa As Double, ByVal rho_l_kgm3 As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_ads_p_wTpsatRho = direct_ads_p_wTpsatRho_workingPair(w_kgkg, T_K, p_sat_Pa, rho_l_kgm3, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)
        
End Function

' ws_direct_ads_T_pwpsatRho
'
Function ws_direct_ads_T_pwpsatRho _
(ByVal p_Pa As Double, ByVal w_kgkg As Double, ByVal p_sat_Pa As Double, ByVal rho_l_kgm3 As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_ads_T_pwpsatRho = direct_ads_T_pwpsatRho_workingPair(p_Pa, w_kgkg, p_sat_Pa, rho_l_kgm3, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)
        
End Function

' ws_direct_ads_dw_dp_pTpsatRho
'
Function ws_direct_ads_dw_dp_pTpsatRho _
(ByVal p_Pa As Double, ByVal T_K As Double, ByVal p_sat_Pa As Double, ByVal rho_l_kgm3 As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_ads_dw_dp_pTpsatRho = direct_ads_dw_dp_pTpsatRho_workingPair(p_Pa, T_K, p_sat_Pa, rho_l_kgm3, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)
        
End Function

' ws_direct_ads_dw_dT_pTpsatRho
'
Function ws_direct_ads_dw_dT_pTpsatRho _
(ByVal p_Pa As Double, ByVal T_K As Double, ByVal p_sat_Pa As Double, ByVal rho_l_kgm3 As Double, _
 ByVal dp_sat_dT_PaK As Double, ByVal drho_dT_kgm3K As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_ads_dw_dT_pTpsatRho = direct_ads_dw_dT_pTpsatRho_workingPair(p_Pa, T_K, p_sat_Pa, rho_l_kgm3, _
        dp_sat_dT_PaK, drho_dT_kgm3K, path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)
        
End Function

' ws_direct_ads_dp_dw_wTpsatRho
'
Function ws_direct_ads_dp_dw_wTpsatRho _
(ByVal w_kgkg As Double, ByVal T_K As Double, ByVal p_sat_Pa As Double, ByVal rho_l_kgm3 As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_ads_dp_dw_wTpsatRho = direct_ads_dp_dw_wTpsatRho_workingPair(w_kgkg, T_K, p_sat_Pa, rho_l_kgm3, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)
        
End Function

' ws_direct_ads_dp_dT_wTpsatRho
'
Function ws_direct_ads_dp_dT_wTpsatRho _
(ByVal w_kgkg As Double, ByVal T_K As Double, ByVal p_sat_Pa As Double, ByVal rho_l_kgm3 As Double, _
 ByVal dp_sat_dT_PaK As Double, ByVal drho_dT_kgm3K As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_ads_dp_dT_wTpsatRho = direct_ads_dp_dT_wTpsatRho_workingPair(w_kgkg, T_K, p_sat_Pa, rho_l_kgm3, _
        dp_sat_dT_PaK, drho_dT_kgm3K, path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)
        
End Function

' ws_direct_ads_piStar_pyxgTpsatRhoM
'
Function ws_direct_ads_piStar_pyxgTpsatRhoM _
(ByVal p_total_Pa As Double, ByVal y_molmol As Double, ByVal x_molmol As Double, _
 ByVal gamma As Double, ByVal T_K As Double, ByVal p_sat_Pa As Double, _
 ByVal rho_l_kgm3 As Double, ByVal M_kgmol As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_ads_piStar_pyxgTpsatRhoM = direct_ads_piStar_pyxgTpsatRhoM_workingPair(p_total_Pa, y_molmol, x_molmol, _
        gamma, T_K, p_sat_Pa, rho_l_kgm3, M_kgmol, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)
        
End Function

' ws_direct_abs_X_pT
'
Function ws_direct_abs_X_pT _
(ByVal p_Pa As Double, ByVal T_K As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_abs_X_pT = direct_abs_X_pT_workingPair(p_Pa, T_K, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)
        
End Function

' ws_direct_abs_p_XT
'
Function ws_direct_abs_p_XT _
(ByVal X_kgkg As Double, ByVal T_K As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_abs_p_XT = direct_abs_p_XT_workingPair(X_kgkg, T_K, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)
        
End Function

' ws_direct_abs_T_pX
'
Function ws_direct_abs_T_pX _
(ByVal p_Pa As Double, ByVal X_kgkg As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_abs_T_pX = direct_abs_T_pX_workingPair(p_Pa, X_kgkg, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)
        
End Function

' ws_direct_abs_dX_dp_pT
'
Function ws_direct_abs_dX_dp_pT _
(ByVal X_kgkg As Double, ByVal T_K As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_abs_dX_dp_pT = direct_abs_dX_dp_pT_workingPair(X_kgkg, T_K, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)
        
End Function

' ws_direct_abs_dX_dT_pT
'
Function ws_direct_abs_dX_dT_pT _
(ByVal X_kgkg As Double, ByVal T_K As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_abs_dX_dT_pT = direct_abs_dX_dT_pT_workingPair(X_kgkg, T_K, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)
        
End Function

' ws_direct_abs_dp_dX_XT
'
Function ws_direct_abs_dp_dX_XT _
(ByVal X_kgkg As Double, ByVal T_K As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_abs_dp_dX_XT = direct_abs_dp_dX_XT_workingPair(X_kgkg, T_K, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)
        
End Function

' ws_direct_abs_dp_dT_XT
'
Function ws_direct_abs_dp_dT_XT _
(ByVal X_kgkg As Double, ByVal T_K As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_abs_dp_dT_XT = direct_abs_dp_dT_XT_workingPair(X_kgkg, T_K, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)
        
End Function

' ws_direct_abs_g_Txv1v2
'
Function ws_direct_abs_g_Txv1v2 _
(ByVal T_K As Double, ByVal x_molmol As Double, ByVal v1_m3mol As Double, ByVal v2_m3mol As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_abs_g_Txv1v2 = direct_abs_g_Txv1v2_workingPair(T_K, x_molmol, v1_m3mol, v2_m3mol, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)
        
End Function

' ws_direct_abs_p_Txv1v2psat
'
Function ws_direct_abs_p_Txv1v2psat _
(ByVal T_K As Double, ByVal x_molmol As Double, ByVal v1_m3mol As Double, ByVal v2_m3mol As Double, ByVal p_sat_Pa As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_abs_p_Txv1v2psat = direct_abs_p_Txv1v2psat_workingPair(T_K, x_molmol, v1_m3mol, v2_m3mol, p_sat_Pa, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)
        
End Function

' ws_direct_abs_p_Txv1v2
'
Function ws_direct_abs_p_Txv1v2 _
(ByVal T_K As Double, ByVal x_molmol As Double, ByVal v1_m3mol As Double, ByVal v2_m3mol As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_abs_p_Txv1v2 = direct_abs_p_Txv1v2_workingPair(T_K, x_molmol, v1_m3mol, v2_m3mol, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)
        
End Function

' ws_direct_abs_p_Tvx
'
Function ws_direct_abs_p_Tvx _
(ByVal T_K As Double, ByVal v_m3mol As Double, ByVal x_molmol As Double, _
 ByVal wp_as As String, ByVal wp_st As String, ByVal wp_rf As String, _
 ByVal wp_iso As String, ByVal no_iso As Integer, _
 ByVal rf_psat As String, ByVal no_p_sat As Integer, _
 ByVal rf_rhol As String, ByVal no_rhol As Integer) As Double
    ' Call function wrapper to access DLL function
    '
    Dim path_JSON As String
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_abs_p_Tvx = direct_abs_p_Tvx_workingPair(T_K, v_m3mol, x_molmol, _
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
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_ref_p_sat_T = direct_ref_p_sat_T_workingPair(T_K, _
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
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_ref_dp_sat_dT_T = direct_ref_dp_sat_dT_T_workingPair(T_K, _
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
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_ref_rho_l_T = direct_ref_rho_l_T_workingPair(T_K, _
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
    path_JSON = ThisWorkbook.Path & "\data\sorpproplib_ValidationCInterface.json"
    
    ws_direct_ref_drho_l_dT_T = direct_ref_drho_l_dT_T_workingPair(T_K, _
        path_JSON, wp_as, wp_st, wp_rf, wp_iso, no_iso, rf_psat, no_p_sat, rf_rhol, no_rhol)
        
End Function
