function [ rho_l ] = ref_rho_l_T( T, refWPair )
%Calculates saturated liquid density
%
%  Calculates saturated liquid density in kg/m³ depending on equilibrium
%  temperature T in K.
% 
%  Parameters:
%  -----------
% 	double T_K:
% 		Equilibrium temperature in K.
%  	struct *WorkingPair:
% 		Pointer of WorkingPair-struct.
% 
%  Returns:
%  --------
% 	double:
% 		Saturated liquid density in kg/m³.
% 
%  History:
%  --------
% 	07/15/2021, by Mirko Engelpracht:
% 		First implementation.

rho_l = calllib('libsorpPropLib', 'ref_rho_l_T', T, refWPair);

end

