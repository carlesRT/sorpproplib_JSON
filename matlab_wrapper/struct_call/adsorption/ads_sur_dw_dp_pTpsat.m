function [ dw_dp ] = ads_sur_dw_dp_pTpsat( p, T, p_sat, refWPair )
%Calculates derivative of loading wrt. temperature
%
%  Calculates derivative of equilibrium loading w with respect to pressure
%  p in kg/kg/Pa depending on equilibrium pressure p in Pa, equilibrium
%  temperature T in K, and saturation pressure p_sat in Pa.
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
% 		Derivative of equilibrium loading wrt. pressure in kg/kg/Pa.
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

dw_dp = calllib('libsorpPropLib', 'ads_sur_dw_dp_pTpsat', ...
                p, T, p_sat, refWPair);

end

