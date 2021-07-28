function [ p ] = ads_p_wT( w, T, refWPair )
%Calculates pressure
%
%  Calculates equilibrium pressure p in Pa depending on equilibrium loading w
%  in kg/kg and equilibrium temperature T in K.
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
%  	double:
% 		Equilibrium pressure in Pa.
% 
%  History:
%  --------
% 	07/15/2021, by Mirko Engelpracht:
% 		First implementation.

p = calllib('libsorpPropLib', 'ads_p_wT', w, T, refWPair);

end

