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
wp_as = 'NaOH-KOH-CsOH';
wp_st = '';
wp_rf = 'Water';

wp_iso = 'Duehring';
no_iso = 1;
rf_psat = 'VaporPressure_EoS1';
no_p_sat = 1;
rf_rhol ='SaturatedLiquidDensity_EoS1';
no_rhol = 1;

% Define some input parameters for calculations
%
p_start = 0.25e5;           % Pa
T_fix = 373.15;             % K

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
X_pT = zeros(1, 1000);
p_XT = zeros(1, 1000);
T_pX = zeros(1, 1000);

dX_dp_pT = zeros(1, 1000);
dX_dT_pT = zeros(1, 1000);
dp_dX_XT = zeros(1, 1000);
dp_dT_XT = zeros(1, 1000);

for i = 1 : 1000
    % Execute functions that are always defined
    %
    X_pT(1, i) = abs_con_X_pT(p(i), T(i), refWPair);
    p_XT(1, i) = abs_con_p_XT(X_pT(1, i), T(i), refWPair);
    T_pX(1, i) = abs_con_T_pX(p(i), X_pT(1, i), refWPair);
    
    dX_dp_pT(1, i) = abs_con_dX_dp_pT(p(i), T(i), refWPair);
    dX_dT_pT(1, i) = abs_con_dX_dT_pT(p(i), T(i), refWPair);
    dp_dX_XT(1, i) = abs_con_dp_dX_XT(X_pT(1, i), T(i), refWPair);
    dp_dT_XT(1, i) = abs_con_dp_dT_XT(X_pT(1, i), T(i), refWPair);
end

% Delete working pair object (i.e., necessary to free memory!)
%
delWorkingPair(refWPair);            
clear refWPair;
unloadlibrary libsorpPropLib;
