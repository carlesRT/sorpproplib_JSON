function [ refWPair ] = newWorkingPair( path_db, path_dll, path_header, ...
                                        wp_as, wp_st, wp_rf, ...
                                        wp_iso, no_iso, rf_psat, ...
                                        no_p_sat, rf_rhol, no_rhol )
%Initializes a new working pair object and returns its reference
%
%  Initialization function to create a new WorkingPair-struct. Therefore,
%  function checks first if enough memory available to allocate WorkingPair-
%  struct and then reads coefficients of functions from database and sets
%  pointers within the struct depending on working pair given as input.
%  If memory allocation fails, functions returns NULL and prints a warning.
% 
%  Parameters:
%  -----------
% 	const char *path_db:
% 		Path to database.
%  	const char *wp_as:
% 		Name of sorbent.
%  	const char *wp_st:
% 		Name of sub-type of sorbent.
%  	const char *wp_rf:
% 		Name of refrigerant.
%  	const char *wp_iso:
% 		Name of isotherm.
% 	int no_iso:
% 		ID of isotherm (i.e. when more than one isotherm is available)
%  	const char *rf_psat:
% 		Name of calculation approach for vapor pressure.
% 	int no_p_sat:
% 		ID of vapor pressure equation (i.e. when more than one equation is
% 		available)
%  	const char *rf_rhol:
% 		Name of calculation approach for liquid density.
% 	int no_rhol:
% 		ID of liquid density equation (i.e. when more than one equation is
% 		available)
% 
%  Returns:
%  --------
% 	struct *WorkingPair:
% 		Returns malloc-ed WorkingPair-struct containing all information required
% 		to calculate the resorption equilibrium of the working working.
% 
%  History:
%  --------
% 	07/15/2021, by Mirko Engelpracht:
% 		First implementation.

% Load library
%
loadlibrary(path_dll,path_header);
libfunctions libsorpPropLib;

% Initialize working pair
%
refWPair = calllib('libsorpPropLib', 'newWorkingPair', ...
                   libpointer('cstring', path_db), ...
                   libpointer('cstring', wp_as), ...
                   libpointer('cstring', wp_st), ...
                   libpointer('cstring', wp_rf), ...
                   libpointer('cstring', wp_iso), no_iso, ...
                   libpointer('cstring', rf_psat), no_p_sat, ...
                   libpointer('cstring', rf_rhol), no_rhol);
end

