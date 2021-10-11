function [ dw_dp ] = ads_vol_dw_dp_pTpsatRho( p, T, p_sat, rho_liq, ...
                                              refWPair )
%Calculates derivative of loading wrt. temperature
%
%  Calculates derivative of equilibrium loading dw_dp with respect to pressure
%  in kg/kg/Pa depending on equilibrium pressure p in Pa, equilibrium
%  temperature T in K, saturation pressure p_Sat in Pa and density of adsorpt
%  rho in kg/m³.
% 
%  Parameters:
%  -----------
%  	double p_Pa:
% 		Equilibrium pressure in Pa.
% 	double T_K:
% 		Equilibrium temperature in K.
%  	double p_sat_Pa:
% 		Saturation pressure in Pa.
% 	double rho_kgm3:
% 		Density of adsorpt in kg/m³.
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
% 	This function is only valid for isotherm models based on the volumetric
% 	approach.
% 
%  History:
%  --------
% 	07/15/2021, by Mirko Engelpracht:
% 		First implementation.

dw_dp = calllib('libsorpPropLib', 'ads_vol_dw_dp_pTpsatRho', ...
                p, T, p_sat, rho_liq, refWPair);

end

