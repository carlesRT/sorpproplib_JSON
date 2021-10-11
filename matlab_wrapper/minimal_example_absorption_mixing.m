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
wp_as = 'lubricant';
wp_st = 'POE ISO7';
wp_rf = 'Isobutane';

wp_iso = 'MixingRule';
no_iso = 1;
rf_psat = 'VaporPressure_EoS1';
no_p_sat = 1;
rf_rhol ='SaturatedLiquidDensity_EoS1';
no_rhol = 1;

% Define some input parameters for calculations
%
p_start = 200000;           % Pa
T_fix = 303.15;             % K

p = zeros(1, 1000);
T = zeros(1, 1000);

for i = 1 : 1000
   p(i) = 50 * (i-1) + p_start;     % Pa
   T(i) = T_fix;                    % K
end

% Initialize working pair object
%
refWPair = newWorkingPair(path_db, path_dll, path_header, ...
                    wp_as, wp_st, wp_rf, ...
                    wp_iso, no_iso, rf_psat, ...
                    no_p_sat, rf_rhol, no_rhol);
                
% Perform calculations:
%
p_sat = zeros(1, 1000);
x_pT = zeros(1, 1000);
y1 = zeros(3, 1000);
y2 = zeros(3, 1000);
p_xT = zeros(1, 1000);
T_px = zeros(1, 1000);

dx_dp_pT = zeros(1, 1000);
dx_dT_pT = zeros(1, 1000);

for i = 1 : 1000
    % Execute functions that are always defined , y1_pT(1, i), y2_pT(1, i)
    %
    [x_pT(1, i), y1(1, i), y2(1, i)] = ...
        abs_mix_x_pT(p(i), T(i), refWPair);
    [p_xT(1, i), y1(2, i), y2(2, i)] = ...
        abs_mix_p_Tx(T(i), x_pT(1, i), refWPair);
    [T_px(1, i), y1(3, i), y2(3, i)] = ...
        abs_mix_T_px(p(i), x_pT(1, i), refWPair);
    
    dx_dp_pT(1, i) = abs_mix_dp_dx_Tx(p(i), T(i), refWPair);
    dx_dT_pT(1, i) = abs_mix_dp_dT_Tx(p(i), T(i), refWPair);
end

% Delete working pair object (i.e., necessary to free memory!)
%
delWorkingPair(refWPair);            
clear refWPair;
unloadlibrary libsorpPropLib;
