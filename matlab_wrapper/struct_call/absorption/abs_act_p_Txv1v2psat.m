function [ p ] = abs_act_p_Txv1v2psat( T, x, v1, v2, p_sat, refWPair )
%Calculates pressure
%
%  Calculates equilibrium pressure p_Pa in Pa of first component depending on
%  temperature T_K in K, mole fraction in liquid phase x_molmol in mol/mol,
%  molar volume of first component in m�/mol, molar volume of second component
%  in m�/mol,and saturation pressure of first component p_sat_Pa in Pa.
% 
%  Parameters:
%  -----------
% 	double T_K:
% 		Equilibrium temperature in K.
% 	double x_molmol:
% 		Equilibrium mole fraction in liquid phase in mol/mol.
% 	double v1_m3mol:
% 		Equilibrium molar volume of first component in m�/mol.
% 	double v2_m3mol:
% 		Equilibrium molar volume of second component in m�/mol.
% 	double p_sat_Pa:
% 		Saturation pressure of first component in Pa.
%  	struct *WorkingPair:
% 		Pointer of WorkingPair-struct.
% 
%  Returns:
%  --------
% 	double:
% 		Equilibrium pressure p_Pa in Pa.
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

p = calllib('libsorpPropLib', 'abs_act_p_Txv1v2psat', T, x, v1, v2, ...
            p_sat, refWPair);

end

