function [ T ] = ads_T_pw( p, w, refWPair )
%Calculates temperature
%
%  Calculates equilibrium temperature in K depending on equilibrium pressure p
%  in Pa and equilibrium loading w in kg/kg.
% 
%  Parameters:
%  -----------
% 	double p_Pa:
% 		Equilibrium pressure in Pa.
% 	double w_kgkg:
% 		Equilibrium loading in kg/kg.
%  	struct *WorkingPair:
% 		Pointer of WorkingPair-struct.
% 
%  Returns:
%  --------
%  	double:
% 		Equilibrium temperature in K.
% 
%  Remarks:
%  --------
% 	Function uses equation of states implemented for calculation of vapor
% 	pressure, saturated liquid density, derivative of vapor pressure wrt.
% 	temperature, and derivative of saturated liquid density wrt. temperature
% 	when isotherm of working pair is based on volumetric approach (e.g.
% 	Dubinin theory).
% 
%  History:
%  --------
% 	07/15/2021, by Mirko Engelpracht:
% 		First implementation.

T = calllib('libsorpPropLib', 'ads_T_pw', p, w, refWPair);

end

