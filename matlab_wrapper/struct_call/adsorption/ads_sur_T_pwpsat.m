function [ T ] = ads_sur_T_pwpsat( p, w, p_sat, refWPair )
%Calculates temperature
%
% Calculates equilibrium temperature in K depending on equilibrium pressure p
% in Pa, equilibrium loading w in kg/kg, and saturation pressure p_sat in Pa.
% 
% Parameters:
% -----------
% 	double p_Pa:
% 		Equilibrium pressure in Pa.
% 	double w_kgkg:
% 		Equilibrium loading in kg/kg.
% 	double p_sat_Pa:
% 		Saturation vapor pressure in Pa.
% 	struct *WorkingPair:
% 		Pointer of WorkingPair-struct.
% 
% Returns:
% --------
% 	double:
% 		Equilibrium temperature in K.
% 
% Remarks:
% --------
% 	This function is only valid for isotherm models based on the surface
% 	approach using the saturated vapor pressure as additional argument.
% 	Uses internal equation of states to calculate vapor pressure and derivative
% 	of vapor pressure wrt. temperature. If equation of state is not implemented
% 	for refrigerant, function returns -1 and throws warning. If user want to use
% 	his own equation of state, this function cannot be used and determination of
% 	root to calculate T must be implemented by user.
% 
%  History:
%  --------
% 	07/15/2021, by Mirko Engelpracht:
% 		First implementation.

T = calllib('libsorpPropLib', 'ads_sur_T_pwpsat', p, w, p_sat, refWPair);

end

