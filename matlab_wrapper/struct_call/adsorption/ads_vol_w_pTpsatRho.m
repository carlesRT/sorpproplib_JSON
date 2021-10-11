function [ w ] = ads_vol_w_pTpsatRho( p, T, p_sat, rho_liq, refWPair )
%Calculates loading
%
%  Calculates equilibrium loading w in kg/kg depending on equilibrium pressure
%  p in Pa, equilibrium temperature T in K, saturation pressure p_Sat in Pa and
%  saturated liquid density of adsorpt rho in kg/m³.
% 
%  Parameters:
%  -----------
%  	double p_Pa:
% 		Equilibrium pressure in Pa.
% 	double T_K:
% 		Equilibrium temperature in K.
%  	double p_sat_Pa:
% 		Saturation pressure in Pa.
% 	double rho_l_kgm3:
% 		Saturated liquid density of adsorpt in kg/m³.
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
% 	This function is only valid for isotherm models based on the volumetric
% 	approach.
% 
%  History:
%  --------
% 	07/15/2021, by Mirko Engelpracht:
% 		First implementation.

w = calllib('libsorpPropLib', 'ads_vol_w_pTpsatRho', p, T, p_sat, ...
            rho_liq, refWPair);

end

