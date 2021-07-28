function [ x ] = abs_act_x_pTv1v2psat( p, T, v1, v2, p_sat, refWPair )
%Calculates molar fraction of refrigerant in liquid phase
%
%  Calculates mole fraction of first component in liquid phase x_molmol in
%  mol/mol depending on equilibrium pressure p_Pa in Pa of first component,
%  temperature T_K in K, molar volume of first component in m³/mol, molar volume
%  of second component in m³/mol and saturation pressure of first component
%  p_sat_Pa in Pa
% 
%  Parameters:
%  -----------
% 	double p_Pa:
% 		Equilibrium pressure p_Pa in Pa.
% 	double T_K:
% 		Equilibrium temperature in K.
% 	double v1_m3mol:
% 		Equilibrium molar volume of first component in m³/mol.
% 	double v2_m3mol:
% 		Equilibrium molar volume of second component in m³/mol.
% 	double p_sat_Pa:
% 		Saturation pressure of first component in Pa.
%  	struct *WorkingPair:
% 		Pointer of WorkingPair-struct.
% 
%  Returns:
%  --------
% 	double:
% 		Equilibrium mole fraction in liquid phase in mol/mol.
% 
%  Remarks:
%  --------
% 	This function is only valid for isotherm models based on activity
% 	coefficients.
% 	Molar volumes may are not required and ignored. When molar volumes are
%  	required, function uses molar volumes stored in JSON file when input
% 	v1_m3mol or v2_m3mol is -1. Otherwise, function uses molar volumes given by
% 	inputs.
% 
%  History:
%  --------
% 	07/15/2021, by Mirko Engelpracht:
% 		First implementation.

x = calllib('libsorpPropLib', 'abs_act_x_pTv1v2psat', p, T, v1, v2, ...
            p_sat, refWPair);

end

