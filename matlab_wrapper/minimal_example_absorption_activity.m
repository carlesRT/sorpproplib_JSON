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
wp_st = 'POE';
wp_rf = 'R-134a';

wp_iso = 'WilsonFixedDl';
no_iso = 1;
rf_psat = 'VaporPressure_EoS1';
no_p_sat = 1;
rf_rhol ='SaturatedLiquidDensity_EoS1';
no_rhol = 1;

% Define some input parameters for calculations
%
p_start = 5e5;              % Pa
T_fix = 333.15;             % K

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
%
p_sat = zeros(1, 1000);

x_Txv1v2 = zeros(2, 1000);
p_Txv1v2 = zeros(1, 1000);
g_Txv1v2 = zeros(1, 1000);

for i = 1 : 1000
    % Execute refrigerant functions
    %
    p_sat(i) = ref_p_sat_T(T(i), refWPair);
    
    % Execute functions that are always defined
    %
    x_Txv1v2(1, i) = abs_act_x_pTv1v2(p(i), T(i), -1, -1, refWPair);
    p_Txv1v2(1, i) = abs_act_p_Txv1v2(T(i), x_Txv1v2(1, i), -1, -1, ...
                                      refWPair);
    
    g_Txv1v2(1, i) = abs_act_g_Txv1v2(T(i), x_Txv1v2(1, i), -1, -1, ...
                                      refWPair);
                                  
    x_Txv1v2(2, i) = abs_act_x_pTv1v2psat(p(i), T(i), -1, -1, ...
                                          p_sat(i), refWPair);
    p_Txv1v2(2, i) = abs_act_p_Txv1v2psat(T(i), x_Txv1v2(1, i), -1, -1, ...
                                          p_sat(i), refWPair);
end

% Delete working pair object (i.e., necessary to free memory!)
%
delWorkingPair(refWPair);            
clear refWPair;
unloadlibrary libsorpPropLib;
