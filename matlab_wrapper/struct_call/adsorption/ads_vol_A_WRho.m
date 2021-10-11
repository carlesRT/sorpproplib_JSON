function [ A ] = ads_vol_A_WRho( W, rho_liq, refWPair )
%Calculates adsorption potential
%
%  Calculates equilibrium adsorption potential A in J/mol depending on
%  equilibrium volumetric loading W in m³/kg and saturated liquid density of
%  adsorpt rho_l in kg/m3.
% 
%  Parameters:
%  -----------
%  	double W_m3kg:
% 		Equilibrium volumetric loading W in m³/kg.
%  	double rho_l_kgm3:
% 		Saturated liquid density of adsorpt rho_l in kg/m3.
%  	struct *WorkingPair:
% 		Pointer of WorkingPair-struct.
% 
%  Returns:
%  --------
% 	double:
% 		Equilibrium adsorption potential A in J/mol.
% 
%  Remarks:
%  --------
% 	This function is only valid for isotherm models based on the volumetric
% 	approach.
% 	Saturated liquid density of adsorpt is required to calculate volumetric
% 	loading from characteristic curve in m3/kg when coefficients would directly
% 	return loading w in kg/kg.
% 
%  History:
%  --------
% 	07/15/2021, by Mirko Engelpracht:
% 		First implementation.

A = calllib('libsorpPropLib', 'ads_vol_A_WRho', W, rho_liq, refWPair);

end

