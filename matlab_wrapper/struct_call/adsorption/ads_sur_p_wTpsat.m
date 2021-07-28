function [ p ] = ads_sur_p_wTpsat( w, T, p_sat, refWPair )
%Calculates pressure
%
%  Calculates equilibrium pressure p in Pa depending on equilibrium loading w
%  in kg/kg, equilibrium temperature T in K, and saturation pressure p_sat in
%  Pa.
% 
%  Parameters:
%  -----------
% 	double w_kgkg:
% 		Equilibrium loading in kg/kg.
% 	double T_K:
% 		Equilibrium temperature in K.
% 	double p_sat_Pa:
% 		Saturation vapor pressure in Pa.
%  	struct *WorkingPair:
% 		Pointer of WorkingPair-struct.
% 
%  Returns:
%  --------
%  	double:
% 		Equilibrium pressure in Pa.
% 
%  Remarks:
%  --------
% 	This function is only valid for isotherm models based on the surface
% 	approach using the saturated vapor pressure as additional argument.
% 
%  History:
%  --------
% 	07/15/2021, by Mirko Engelpracht:
% 		First implementation.

p = calllib('libsorpPropLib', 'ads_sur_p_wTpsat', w, T, p_sat, refWPair);

end

