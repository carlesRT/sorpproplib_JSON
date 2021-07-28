function [ X ] = abs_con_X_pT( p, T, refWPair )
%Calculates concentration
%
%  Calculates equilibrium concentration X in kg/kg depending on equilibrium
%  pressure p in Pa and equilibrium temperature T in K.
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
% 		Equilibrium concentration in kg/kg.
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

X = calllib('libsorpPropLib', 'abs_con_X_pT', p, T, refWPair);

end

