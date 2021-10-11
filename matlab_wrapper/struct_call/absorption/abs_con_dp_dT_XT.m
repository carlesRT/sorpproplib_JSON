function [ dp_dT ] = abs_con_dp_dT_XT( X, T, refWPair )
%Calculates derivative of pressure wrt. temperature
%
%  Calculates derivative of equilibrium pressure p with respect to temperature
%  T in kg/kg/K depending on equilibrium concentration X in kg/kg and
%  equilibrium temperature T in K.
% 
%  Parameters:
%  -----------
% 	double X_kgkg:
% 		Equilibrium concentration in kg/kg.
% 	double T_K:
% 		Equilibrium temperature in K.
%  	struct *WorkingPair:
% 		Pointer of WorkingPair-struct.
% 
%  Returns:
%  --------
% 	double:
% 		Derivative of equilibrium pressure wrt. temperature in Pa/K.
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

dp_dT = calllib('libsorpPropLib', 'abs_con_dp_dT_XT', X, T, refWPair);

end

