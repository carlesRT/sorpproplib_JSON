function [ dA_dW ] = ads_vol_dA_dW_WRho( W, rho_liq, refWPair )
%Calculates derivative of adsorption potential wrt. volumetric loading
%
%  Calculates derivative of equilibrium adsorption potential dA_dW in kgJ/mol/m³
%  depending on equilibrium volumetric loading W in m³/kg and saturated liquid
%  density of adsorpt rho_l in kg/m3.
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
% 		Derivative of adsorption potential wrt. volumetric loading in
% 		kgJ/mol/m³.
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

dA_dW = calllib('libsorpPropLib', 'ads_vol_dA_dW_WRho', W, rho_liq, ...
                refWPair);

end

