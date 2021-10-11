function [ dp_dx ] = abs_mix_dp_dx_Tx( T, x, refWPair )
%Calculates derivative of pressure wrt. liquid molar fraction of refrigerant
%
%  Calculates derivative of equilibrium pressure with respect to liquid mole
%  fraction in Pa depending on temperature T_K in K and mole fraction in liquid
%  phase x_molmol in mol/mol.
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
% 		Derivative of equilibrium pressure wrt liquid mole fraction in Pa.
% 
%  Remarks:
%  --------
% 	This function is only valid for isotherm models based on mixing rules.
% 	Derivative is calculated numerically by the symmetric derivative using
%   h = 0.00000001 mol/mol as small change.
% 
%  History:
%  --------
% 	07/15/2021, by Mirko Engelpracht:
% 		First implementation.

dp_dx = calllib('libsorpPropLib', 'abs_mix_dp_dx_Tx', T, x, refWPair);

end

