function [ x, y1, y2 ] = abs_mix_x_pT( p, T, refWPair )
%Calculates molar fraction of refrigerant in liquid phase
%
%  Calculates equilibrium liquid mole fraction of first component in mol/mol
%  depending on pressure in Pa and temperature T_K in K.
% 
%  Parameters:
%  -----------
% 	double p_Pa:
% 		Equilibrium pressure p_Pa in Pa.
% 	double T_K:
% 		Equilibrium temperature in K.
%  	struct *WorkingPair:
% 		Pointer of WorkingPair-struct.
% 
%  Returns:
%  --------
% 	double:
% 		Equilibrium mole fraction in of first component liquid phase in mol/mol.
% 	double% ret_y_1_molmol:
% 		Equilibrium mole fraction of first component.
% 	double% ret_y_2_molmol:
% 		Equilibrium mole fraction of second component.
% 
%  Remarks:
%  --------
% 	This function is only valid for isotherm models based on mixing rules.
% 
%  History:
%  --------
% 	07/15/2021, by Mirko Engelpracht:
% 		First implementation.
p_y1 = libpointer('doublePtr', 0);
p_y2 = libpointer('doublePtr', 0);

x = calllib('libsorpPropLib', 'abs_mix_x_pT', p_y1, p_y2, p, T, refWPair);
y1 = p_y1.Value;
y2 = p_y2.Value;
end

