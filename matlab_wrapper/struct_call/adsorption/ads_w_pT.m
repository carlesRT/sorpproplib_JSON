function [ w ] = ads_w_pT( p, T, refWPair )
%Calculates loading
%
%  Calculates equilibrium loading w in kg/kg depending on equilibrium 
%  pressure p in Pa and equilibrium temperature T in K.
% 
%  Parameters:
%  -----------
%  	double p_Pa:
% 		Equilibrium pressure in Pa.
% 	double T_K:
% 		Equilibrium temperature in K.
%  	struct *WorkingPair:
% 		Pointer of WorkingPair-struct.
% 
%  Returns:
%  --------
% 	double:
% 		Equilibrium loading in kg/kg.
% 
%  History:
%  --------
% 	07/15/2021, by Mirko Engelpracht:
% 		First implementation.

w = calllib('libsorpPropLib', 'ads_w_pT', p, T, refWPair);

end

