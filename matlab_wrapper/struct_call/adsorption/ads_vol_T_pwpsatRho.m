function [ T ] = ads_vol_T_pwpsatRho( p, w, p_sat, rho_liq, refWPair )
%Calculates temperature
%
%  Calculates equilibrium temperature T in K depending on equilibrium pressure
%  in Pa, equilibrium loading w in kg/kg, saturation pressure p_Sat in Pa and
%  density of adsorpt rho in kg/m³.
% 
%  Parameters:
%  -----------
% 	double p_Pa:
% 		Equilibrium pressure in Pa.
%  	double w_kgkg:
% 		Equilibrium loading in kg/kg.
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
% 		Equilibrium temperature in K.
% 
%  Remarks:
%  --------
% 	This function is only valid for isotherm models based on the volumetric
% 	approach.
% 	Uses internal equations of states to calculate vapor pressure, saturated
% 	liquid density of adsorpt, derivative of vapor pressure wrt. temperature
% 	and derivative of saturated liquid density of adsorpt wrt. temperature. If
% 	equation of states are not implemented for refrigerant, function returns -1
%   and throws warning. If user want to use his own equation of states, this
%   function cannot be used and determination of root to calculate T must
%   be implemented by user.
% 
%  History:
%  --------
% 	07/15/2021, by Mirko Engelpracht:
% 		First implementation.

T = calllib('libsorpPropLib', 'ads_vol_T_pwpsatRho', p, w, p_sat, ...
            rho_liq, refWPair);

end

