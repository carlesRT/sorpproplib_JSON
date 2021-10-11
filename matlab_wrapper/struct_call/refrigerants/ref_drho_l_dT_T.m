function [ drho_l_dT ] = ref_drho_l_dT_T( T, refWPair )
%Calculates derivative of saturated liquid density wrt. temperature
%
%  Calculates derivative of saturated liquid density wrt temperature in kg/m³/K
%  depending on equilibrium temperature T in K.
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
% 		Derivative of saturated liquid density wrt temperature in kg/m³/K.
% 
%  History:
%  --------
% 	07/15/2021, by Mirko Engelpracht:
% 		First implementation.

drho_l_dT = calllib('libsorpPropLib', 'ref_drho_l_dT_T', T, refWPair);

end

