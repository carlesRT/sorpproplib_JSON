% Minimal example demonstrating how to use Matlab interface to the
% SorpPropLib
%
% Setup: Define paths, load functions
%
clc;
clear;

path_db = '.\data\JSON\sorpproplib.json';
path_dll = '.\data\win64bit\libsorpPropLib.dll';
path_header = '.\data\win64bit\workingPair.h';

addpath('.\struct_call');
addpath('.\struct_call\refrigerants');
addpath('.\struct_call\adsorption');
addpath('.\struct_call\absorption');

% Adsorption: Working pair based on the volumetric approach
%
% Setup for working pair
%
wp_as = 'silica gel pellet';
wp_st = '123';
wp_rf = 'Water';

wp_iso = 'DubininArctan1';
no_iso = 1;
rf_psat = 'VaporPressure_EoS1';
no_p_sat = 1;
rf_rhol ='SaturatedLiquidDensity_EoS1';
no_rhol = 1;

% Define some input parameters for calculations
%
p_start = 1000;             % Pa
T_fix = 323.15;             % K

p = zeros(1, 1000);
T = zeros(1, 1000);

for i = 1 : 1000
   p(i) = (i-1) + p_start;  % Pa
   T(i) = T_fix;            % K
end

% Initialize working pair object
%
refWPair = newWorkingPair(path_db, path_dll, path_header, ...
                    wp_as, wp_st, wp_rf, ...
                    wp_iso, no_iso, rf_psat, ...
                    no_p_sat, rf_rhol, no_rhol);
                
% Perform calculations:
% Results in first row are performed with fucntion that are always defined
% Results in second row are performed with functions depending on isotherm
% approach
%
p_sat = zeros(1, 1000);
dp_sat_dT = zeros(1, 1000);
T_sat = zeros(1, 1000);
rho_liq = zeros(1, 1000);
drho_liq_dT = zeros(1, 1000);

w_pT = zeros(2, 1000);
p_wT = zeros(2, 1000);
T_pw = zeros(2, 1000);

dw_dp_pT = zeros(2, 1000);
dw_dT_pT = zeros(2, 1000);
dp_dw_wT = zeros(2, 1000);
dp_dT_wT = zeros(2, 1000);

A = zeros(2, 1000);
W = zeros(1, 1000);
dW_dA = zeros(1, 1000);
dA_dW = zeros(1, 1000);

for i = 1 : 1000
    % Execute refrigerant functions
    %
    p_sat(i) = ref_p_sat_T(T(i), refWPair);
    dp_sat_dT(i) = ref_dp_sat_dT_T(T(i), refWPair);
    T_sat(i) = ref_T_sat_p(p_sat(i), refWPair);
    rho_liq(i) = ref_rho_l_T(T(i), refWPair);
    drho_liq_dT(i) = ref_drho_l_dT_T(T(i), refWPair);
    
    % Execute functions that are always defined
    %
    w_pT(1, i) = ads_w_pT(p(i), T(i), refWPair);
    p_wT(1, i) = ads_p_wT(w_pT(1, i), T(i), refWPair);
    T_pw(1, i) = ads_T_pw(p(i), w_pT(1, i), refWPair);
    
    dw_dp_pT(1, i) = ads_dw_dp_pT(p(i), T(i), refWPair);
    dw_dT_pT(1, i) = ads_dw_dT_pT(p(i), T(i), refWPair);
    dp_dw_wT(1, i) = ads_dp_dw_wT(w_pT(1, i), T(i), refWPair);
    dp_dT_wT(1, i) = ads_dp_dT_wT(w_pT(1, i), T(i), refWPair);
    
    % Execute functions that are only defined for adsorption isotherms
    % based on the volumetric approach
    %
    w_pT(2, i) = ads_vol_w_pTpsatRho(p(i), T(i), p_sat(i), rho_liq(i), ...
                                     refWPair);
    p_wT(2, i) = ads_vol_p_wTpsatRho(w_pT(2, i), T(i), p_sat(i), ...
                                     rho_liq(i), refWPair);
    T_pw(2, i) = ads_vol_T_pwpsatRho(p(i), w_pT(2, i), p_sat(i), ...
                                     rho_liq(i), refWPair);
    
    dw_dp_pT(2, i) = ads_vol_dw_dp_pTpsatRho(p(i), T(i), p_sat(i),  ...
                                             rho_liq(i), refWPair);
    dw_dT_pT(2, i) = ads_vol_dw_dT_pTpsatRho(p(i), T(i), p_sat(i), ...
                                             rho_liq(i), dp_sat_dT(i), ...
                                             drho_liq_dT(i), refWPair);
    dp_dw_wT(2, i) = ads_vol_dp_dw_wTpsatRho(w_pT(2, i), T(i), ...
                                             p_sat(i), rho_liq(i), ...
                                             refWPair);
    dp_dT_wT(2, i) = ads_vol_dp_dT_wTpsatRho(w_pT(2, i), T(i), ...
                                             p_sat(i), rho_liq(i), ...
                                             dp_sat_dT(i), ...
                                             drho_liq_dT(i), ...
                                             refWPair);
                                         
    
    A(1, i) = 8.314 * T(i) * log(p_sat(i) / p(i));
    W(i) = ads_vol_W_ARho(A(1, i), rho_liq(i), refWPair);
    A(2, i) = ads_vol_A_WRho(W(i), rho_liq(i), refWPair);
    
    dW_dA(i) = ads_vol_dW_dA_ARho(A(1, i), rho_liq(i), refWPair);
    dA_dW(i) = ads_vol_dA_dW_WRho(W(i), rho_liq(i), refWPair);
end

% Delete working pair object (i.e., necessary to free memory!)
%
delWorkingPair(refWPair);            
clear refWPair;
unloadlibrary libsorpPropLib;
