function [ T ] = ads_sur_T_pw( p, w, refWPair )
%Calculates temperature
%
%  Calculates equilibrium temperature in K depending on equilibrium pressure p
%  in Pa and equilibrium loading w in kg/kg.
% 
%  Parameters:
%  -----------
% 	double p_Pa:
% 		Equilibrium pressure in Pa.
% 	double w_kgkg:
% 		Equilibrium loading in kg/kg.
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
% 	This function is only valid for isotherm models based on the surface
% 	approach.
% 
%  History:
%  --------
% 	07/15/2021, by Mirko Engelpracht:
% 		First implementation.

T = calllib('libsorpPropLib', 'ads_sur_T_pw', p, w, refWPair);

end

