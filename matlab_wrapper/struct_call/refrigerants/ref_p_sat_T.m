function [ p_sat ] = ref_p_sat_T( T, refWPair )
%Calculates vapor pressure
%
%  Calculates saturation pressure in Pa depending on equilibrium temperature T
%  in K.
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
% 		Saturation pressure in Pa.
% 
%  History:
%  --------
% 	07/15/2021, by Mirko Engelpracht:
% 		First implementation.

p_sat = calllib('libsorpPropLib', 'ref_p_sat_T', T, refWPair);

end

