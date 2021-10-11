function [ dw_dT ] = ads_sur_dw_dT_pTpsat( p, T, p_sat, dp_sat_dT, ...
                                           refWPair )
%Calculates derivative of loading wrt. temperature
%
%  Calculates derivative of equilibrium loading w with respect to temperature
%  T in kg/kg/K depending on equilibrium pressure p in Pa, equilibrium
%  temperature T in K, saturation pressure p_sat in Pa, and derivative of
%  saturation pressure wrt. temperature dp_sat_dT in Pa/K.
% 
%  Parameters:
%  -----------
%  	double p_Pa:
% 		Equilibrium pressure in Pa.
% 	double T_K:
% 		Equilibrium temperature in K.
% 	double p_sat_Pa:
% 		Saturation vapor pressure in Pa.
%  	double dp_sat_dT_PaK:
% 		Derivative of saturation pressure wrt. temperature in Pa/K.
%  	struct *WorkingPair:
% 		Pointer of WorkingPair-struct.
% 
%  Returns:
%  --------
% 	double:
% 		Derivative of equilibrium loading wrt. temperature in kg/kg/K.
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

dw_dT = calllib('libsorpPropLib', 'ads_sur_dw_dT_pTpsat', ...
                p, T, p_sat, dp_sat_dT, refWPair);

end

