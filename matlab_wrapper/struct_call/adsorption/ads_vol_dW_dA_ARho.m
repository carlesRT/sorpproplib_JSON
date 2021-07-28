function [ dW_dA ] = ads_vol_dW_dA_ARho( A, rho_liq, refWPair )
%Calculates derivative of volumetric loading wrt. adsorption potential
%  Calculates derivative of equilibrium volumetric loading dW_dA in m³mol/kg/J
%  depending on equilibrium adsorption potential A in J/mol and saturated liquid
%  density of adsorpt rho_l in kg/m3.
% 
%  Parameters:
%  -----------
%  	double A_Jmol:
% 		Equilibrium adsorption potential A in mol/J.
%  	double rho_l_kgm3:
% 		Saturated liquid density of adsorpt rho_l in kg/m3.
%  	struct *WorkingPair:
% 		Pointer of WorkingPair-struct.
% 
%  Returns:
%  --------
% 	double:
% 		Derivative of volumetric loading wrt. adsorption potential in
% 		m³mol/kg/J.
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

dW_dA = calllib('libsorpPropLib', 'ads_vol_dW_dA_ARho', A, rho_liq, ...
                refWPair);

end

