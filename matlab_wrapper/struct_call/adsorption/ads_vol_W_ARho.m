function [ W ] = ads_vol_W_ARho( A, rho_liq, refWPair )
%Calculates volumetric loading
%  Calculates equilibrium volumetric loading W in m³/kg depending on equilibrium
%  adsorption potential A in J/mol and saturated liquid density of adsorpt rho_l
%  in kg/m3.
% 
%  Parameters:
%  -----------
%  	double A_Jmol:
% 		Equilibrium adsorption potential A in J/mol.
%  	double rho_l_kgm3:
% 		Saturated liquid density of adsorpt rho_l in kg/m3.
%  	struct *WorkingPair:
% 		Pointer of WorkingPair-struct.
% 
%  Returns:
%  --------
% 	double:
% 		Equilibrium volumetric loading W in m³/kg.
% 
%  Remarks:
%  --------
% 	This function is only valid for isotherm models based on the volumetric
% 	approach.
% 	Saturated liquid density of adsorpt is required to calculate volumetric
% 	loading from characteristic curve in m3/kg when coefficients would directly
% 	return loading w in kg/kg. Therefore, function checks unit of coefficients
% 	by checking isotherm coefficients.
% 
%  History:
%  --------
% 	07/15/2021, by Mirko Engelpracht:
% 		First implementation.

W = calllib('libsorpPropLib', 'ads_vol_W_ARho', A, rho_liq, refWPair);

end

