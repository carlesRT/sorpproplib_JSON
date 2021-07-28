function [ dw_dT ] = ads_dw_dT_pT( p, T, refWPair )
%Calculates derivative of loading wrt. temperature
%
%  Calculates derivative of equilibrium loading w with respect to temperature
%  T in kg/kg/K depending on equilibrium pressure p in Pa and equilibrium
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
% 		Derivative of equilibrium loading wrt. temperature in kg/kg/K.
% 
%  History:
%  --------
% 	07/15/2021, by Mirko Engelpracht:
% 		First implementation.

dw_dT = calllib('libsorpPropLib', 'ads_dw_dT_pT', p, T, refWPair);

end

