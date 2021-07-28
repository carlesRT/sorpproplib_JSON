function [ T ] = abs_con_T_pX( p, X, refWPair )
%Calculates temperature
%
%  Calculates equilibrium temperature in K depending on equilibrium pressure p
%  in Pa and equilibrium concentration X in kg/kg.
% 
%  Parameters:
%  -----------
% 	double p_Pa:
% 		Equilibrium pressure in Pa.
% 	double X_kgkg:
% 		Equilibrium concentration in kg/kg.
%  	struct *WorkingPair:
% 		Pointer of WorkingPair-struct.
% 
%  Returns:
%  --------
%  	double:
% 		Equilibrium temperature in K.
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

T = calllib('libsorpPropLib', 'abs_con_T_pX', p, X, refWPair);

end

