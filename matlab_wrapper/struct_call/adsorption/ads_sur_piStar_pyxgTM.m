function [ piStar ] = ads_sur_piStar_pyxgTM( p, y, x, g, T, M, refWPair )
%Calculates reduced spreading pressure
%
%  Calculates reduced spreading pressure piStar in kg/mol depending on
%  equilibrium pressure p in Pa, molar fraction of refrigerant in vapor phase
%  in mol/mol, molar fraction of refrigerant in adsorbed phase in mol/mol,
%  activity coefficient of refrigerant in adsorbed phase, equilibrium
%  temperature T in K and molar mass of refrigerant M in kg/mol. The reduced
%  spreading pressure is defined as follows:
% 
%  	piStar = A * pi / (R * T * m_sorbent) = 1 / M * 
% 		Integral_0^p0{w(p,T) / p * dp}
% 
% 	where p0 = p_total*y / (gamma * x)
% 
%  Parameters:
%  -----------
% 	double p_total_Pa:
% 		Total pressure of vapor phase in Pa.
% 	double y_molmol:
% 		Molar fraction of refrigerant in vapor phase in mol/mol.
% 	double x_molmol:
% 		Molar fraction of refrigerant in adsorbed phase in mol/mol.
% 	double gamma:
% 		Activity coefficient of refrigerant in adsorbed phase.
% 	double T_K:
% 		Equilibrium temperature in K.
% 	double M_kgmol:
% 		Molar mass of refrigerant M in kg/mol.
%  	struct *WorkingPair:
% 		Pointer of WorkingPair-struct.
% 
%  Returns:
%  --------
% 	double:
% 		Reduced spreading pressure in kg/mol.
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

piStar = calllib('libsorpPropLib', 'ads_sur_piStar_pyxgTM', ...
                 p, y, x, g, T, M, refWPair);

end

