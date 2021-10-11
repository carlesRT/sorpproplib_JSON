function [ dp_dT ] = ads_sur_dp_dT_wTpsat( w, T, p_sat, dp_sat_dT, ...
                                           refWPair )
%Calculates derivative of pressure wrt. temperature
%
%  Calculates derivative of equilibrium pressure p with respect to temperature
%  T in kg/kg/K depending on equilibrium loading w in kg/kg, equilibrium
%  temperature T in K, saturation pressure p_sat in Pa, and derivative of
%  saturation pressure wrt. temperature dp_sat_dT in Pa/K.
% 
%  Parameters:
%  -----------
% 	double w_kgkg:
% 		Equilibrium loading in kg/kg.
% 	double T_K:
% 		Equilibrium temperature in K.
% 	double p_sat_Pa:
% 		Saturation vapor pressure in Pa.
%  	double dp_sat_dT_PaK:
% 		Derivative of saturation pressure wrt. temperature in Pa/K.
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
% 	This function is only valid for isotherm models based on the surface
% 	approach using the saturated vapor pressure as additional argument.
% 
%  History:
%  --------
% 	07/15/2021, by Mirko Engelpracht:
% 		First implementation.

dp_dT = calllib('libsorpPropLib', 'ads_sur_dp_dT_wTpsat', ...
                w, T, p_sat, dp_sat_dT, refWPair);

end

