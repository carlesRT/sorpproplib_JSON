function [ dX_dp ] = abs_con_dX_dp_pT( p, T, refWPair )
%Calculates derivative of concentration wrt. pressure
%
%  Calculates derivative of equilibrium concentration X with respect to pressure
%  p in kg/kg/Pa depending on equilibrium pressure p in Pa and equilibrium
%  temperature T in K.
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
% 		Derivative of equilibrium concentration wrt. pressure in kg/kg/Pa.
% 
%  Remarks:
%  --------
% 	This function is only valid for isotherm models based on the conventional
% 	approach.
% 
%  History:
%  --------
% 	07/15/2021, by Mirko Engelpracht:
% 		First implementation.

dX_dp = calllib('libsorpPropLib', 'abs_con_dX_dp_pT', p, T, refWPair);

end

