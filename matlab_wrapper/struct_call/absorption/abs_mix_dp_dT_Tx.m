function [ dp_dT ] = abs_mix_dp_dT_Tx( T, x, refWPair )
%Calculates derivative of pressure wrt. temperature
%
%  Calculates derivative of equilibrium pressure with respect to temperature in
%  Pa/K depending on temperature T_K in K and mole fraction in liquid phase
%  x_molmol in mol/mol.
% 
%  Parameters:
%  -----------
% 	double T_K:
% 		Equilibrium temperature in K.
% 	double x_1_molmol:
% 		Equilibrium mole fraction in liquid phase in mol/mol.
%  	struct *WorkingPair:
% 		Pointer of WorkingPair-struct.
% 
%  Returns:
%  --------
% 	double:
% 		Derivative of equilibrium pressure wrt temperature in Pa/K.
% 
%  Remarks:
%  --------
% 	This function is only valid for isotherm models based on mixing rules.
% 	Derivative is calculated numerically by the symmetric derivative using
%   h = 0.0001 K as small change.
% 
%  History:
%  --------
% 	07/15/2021, by Mirko Engelpracht:
% 		First implementation.

dp_dT = calllib('libsorpPropLib', 'abs_mix_dp_dT_Tx', T, x, refWPair);

end

