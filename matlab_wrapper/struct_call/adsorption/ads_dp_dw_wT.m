function [ dp_dw ] = ads_dp_dw_wT( w, T, refWPair )
%Calculates derivative of pressure wrt. loading
%
%  Calculates derivative of equilibrium pressure p with respect to loading
%  w in kgPa/kg depending on equilibrium loading w in kg/kg and equilibrium
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
% 		Derivative of equilibrium pressure wrt. loading in Pakg/kg.
% 
%  History:
%  --------
% 	07/15/2021, by Mirko Engelpracht:
% 		First implementation.

dp_dw = calllib('libsorpPropLib', 'ads_dp_dw_wT', w, T, refWPair);

end

