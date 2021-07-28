function [ p ] = ads_vol_p_wTpsatRho( w, T, p_sat, rho_liq, refWPair )
%Calculates pressure
%
%  Calculates equilibrium pressure p in Pa depending on equilibrium loading w in
%  kg/kg, equilibrium temperature T in K, saturation pressure p_Sat in Pa and
%  density of adsorpt rho in kg/m³.
% 
%  Parameters:
%  -----------
%  	double w_kgkg:
% 		Equilibrium loading in kg/kg.
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
% 		Equilibrium pressure in Pa.
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

p = calllib('libsorpPropLib', 'ads_vol_p_wTpsatRho', w, T, p_sat, ...
            rho_liq, refWPair);

end

