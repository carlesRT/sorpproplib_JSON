function [ T_sat ] = ref_T_sat_p( p, refWPair )
%Calculates saturation temperature
%
%  Calculates saturation temperature in K depending on equilibrium pressure p
%  in Pa.
% 
%  Parameters:
%  -----------
% 	double p_Pa:
% 		Saturation pressure in Pa.
%  	struct *WorkingPair:
% 		Pointer of WorkingPair-struct.
% 
%  Returns:
%  --------
% 	double:
% 		Equilibrium temperature in K.
% 
%  History:
%  --------
% 	07/15/2021, by Mirko Engelpracht:
% 		First implementation.

T_sat = calllib('libsorpPropLib', 'ref_T_sat_p', p, refWPair);

end

