function [ T, y1, y2 ] = abs_mix_T_px( p, x, refWPair )
%Calculates temperature
%
%  Calculates equilibrium temperature T_K in K depending on pressure p_Pa in Pa
%  and mole fraction in liquid phase x_molmol in mol/mol.
% 
%  Parameters:
%  -----------
% 	double p_Pa:
% 		Equilibrium pressure p_Pa in Pa.
% 	double x_1_molmol:
% 		Equilibrium mole fraction in liquid phase in mol/mol.
%  	struct *WorkingPair:
% 		Pointer of WorkingPair-struct.
% 
%  Returns:
%  --------
% 	double:
% 		Equilibrium mole fraction in liquid phase in mol/mol.
% 	double% ret_y_1_molmol:
% 		Equilibrium mole fraction of first component.
% 	double% ret_y_2_molmol:
% 		Equilibrium mole fraction of second component.
% 
%  Remarks:
%  --------
% 	This function is only valid for isotherm models based on mixing rules.
% 	Uses Newton-Raphson method for calculating equilibrium temperature.
% 
%  History:
%  --------
% 	07/15/2021, by Mirko Engelpracht:
% 		First implementation.
p_y1 = libpointer('doublePtr', 0);
p_y2 = libpointer('doublePtr', 0);

T = calllib('libsorpPropLib', 'abs_mix_T_px', p_y1, p_y2, p, x, refWPair);
y1 = p_y1.Value;
y2 = p_y2.Value;
end

