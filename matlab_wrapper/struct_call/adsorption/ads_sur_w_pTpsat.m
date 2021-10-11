function [ w ] = ads_sur_w_pTpsat( p, T, p_sat, refWPair )
%Calculates loading
%
%  Calculates equilibrium loading w in kg/kg depending on equilibrium pressure
%  p in Pa, equilibrium temperature T in K, and saturation pressure p_sat in Pa.
% 
%  Parameters:
%  -----------
%  	double p_Pa:
% 		Equilibrium pressure in Pa.
% 	double T_K:
% 		Equilibrium temperature in K.
% 	double p_sat_Pa:
% 		Saturation vapor pressure in Pa.
%  	struct *WorkingPair:
% 		Pointer of WorkingPair-struct.
% 
%  Returns:
%  --------
% 	double:
% 		Equilibrium loading in kg/kg.
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

w = calllib('libsorpPropLib', 'ads_sur_w_pTpsat', p, T, p_sat, refWPair);

end

