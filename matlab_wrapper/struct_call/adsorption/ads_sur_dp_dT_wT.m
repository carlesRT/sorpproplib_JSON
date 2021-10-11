function [ dp_dT ] = ads_sur_dp_dT_wT( w, T, refWPair )
%Calculates derivative of pressure wrt. temperature
%
%  Calculates derivative of equilibrium pressure p with respect to temperature
%  T in kg/kg/K depending on equilibrium loading w in kg/kg and equilibrium
%  temperature T in K.
% 
%  Parameters:
%  -----------
% 	double w_kgkg:
% 		Equilibrium loading in kg/kg.
% 	double T_K:
% 		Equilibrium temperature in K.
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
% 	approach.
% 
%  History:
%  --------
% 	07/15/2021, by Mirko Engelpracht:
% 		First implementation.

dp_dT = calllib('libsorpPropLib', 'ads_sur_dp_dT_wT', w, T, refWPair);

end

