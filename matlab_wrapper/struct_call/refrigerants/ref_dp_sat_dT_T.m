function [ dp_sat_dT ] = ref_dp_sat_dT_T( T, refWPair )
%Calculates derivative of vapor pressure wrt. temperature
%
%  Calculates derivative of saturation pressure in Pa/K wrt to temperature
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
% 		Derivative of saturation pressure wrt temperature in Pa/K.
% 
%  History:
%  --------
% 	07/15/2021, by Mirko Engelpracht:
% 		First implementation.

dp_sat_dT = calllib('libsorpPropLib', 'ref_dp_sat_dT_T', T, refWPair);

end

