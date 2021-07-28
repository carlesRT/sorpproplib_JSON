function [ dX_dT ] = abs_con_dX_dT_pT( p, T, refWPair )
%Calculates derivative of concentration wrt. temperature
%
%  Calculates derivative of equilibrium concentration X with respect to
%  temperature T in kg/kg/K depending on equilibrium pressure p in Pa and
%  equilibrium temperature T in K.
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
% 		Derivative of equilibrium concentration wrt. temperature in kg/kg/K.
% 
%  Remarks:
%  --------
% 	This function is only valid for isotherm models based on the conventional
% 	approach.
% 
% 
%  History:
%  --------
% 	07/15/2021, by Mirko Engelpracht:
% 		First implementation.

dX_dT = calllib('libsorpPropLib', 'abs_con_dX_dT_pT', p, T, refWPair);

end

