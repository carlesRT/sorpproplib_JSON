function [ dp_dT ] = ads_vol_dp_dT_wTpsatRho( w, T, p_sat, rho_liq, ...
                                              dp_sat_dT, drho_liq_dT, ...
                                              refWPair )
%Calculates derivative of pressure wrt. temperature
%
%  Calculates derivative of equilibrium pressure p with respect to temperature
%  T in kg/kg/K depending on equilibrium loading w in kg/kg, equilibrium
%  temperature T in K, saturation pressure p_Sat in Pa and density of adsorpt
%  rho in kg/m³, derivative of saturation pressure with respect to temperature
%  dp_sat_dT in Pa/K and derivative of density of adsorpt with respect to
%  temperature drho_dT in kg/m³/K.
% 
%  Parameters:
%  -----------
% 	double w_kgkg:
% 		Equilibrium loading in kg/kg.
% 	double T_K:
% 		Equilibrium temperature in K.
%  	double p_sat_Pa:
% 		Saturation pressure in Pa.
% 	double rho_kgm3:
% 		Density of adsorpt in kg/m³.
%  	double dp_sat_dT_PaK:
% 		Derivative of saturation pressure wrt. temperature in Pa/K.
% 	double drho_dT_kgm3K:
% 		Derivative of density of adsorpt wrt. temperature in kg/m³/K.
%  	struct *WorkingPair:
% 		Pointer of WorkingPair-struct.
% 
%  Returns:
%  --------
% 	double:
% 		Derivative of equilibrium pressure wrt. temperature in Pa/K.
% 
%  Remarks:
%  --------
% 	This function is only valid for isotherm models based on the volumetric
% 	approach.
% 
%  History:
%  --------
% 	07/15/2021, by Mirko Engelpracht:
% 		First implementation.

dp_dT = calllib('libsorpPropLib', 'ads_vol_dp_dT_wTpsatRho', ...
                w, T, p_sat, rho_liq, dp_sat_dT, drho_liq_dT, refWPair);

end

