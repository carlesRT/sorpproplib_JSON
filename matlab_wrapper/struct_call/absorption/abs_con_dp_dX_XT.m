function [ dp_dX ] = abs_con_dp_dX_XT( X, T, refWPair )
%Calculates derivative of pressure wrt. concentration
%
%  Calculates derivative of equilibrium pressure p with respect to concentration
%  w in kgPa/kg depending on equilibrium concentration X in kg/kg and
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
% 		Derivative of equilibrium pressure wrt. concentration in Pakg/kg.
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

dp_dX = calllib('libsorpPropLib', 'abs_con_dp_dX_XT', X, T, refWPair);

end

