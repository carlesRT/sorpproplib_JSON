function [ x, y1, y2 ] = abs_mix_p_Tx( p, T, refWPair )
%Calculates pressure
%
%  Calculates equilibrium pressure p_Pa in Pa depending on temperature T_K in K
%  and mole fraction in liquid phase x_molmol in mol/mol.
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
% 		Equilibrium pressure p_Pa in Pa.
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

x = calllib('libsorpPropLib', 'abs_mix_p_Tx', p_y1, p_y2, p, T, refWPair);
y1 = p_y1.Value;
y2 = p_y2.Value;
end

