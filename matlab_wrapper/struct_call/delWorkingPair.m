function [  ] = delWorkingPair( refWPair )
%Destroys working pair object to free allocated memory
%
%  Destructor function to free allocated memory of a WorkingPair-struct.
% 
%  Parameters:
%  -----------
%  	struct *WorkingPair:
% 		Pointer of WorkingPair-struct.
% 
%  History:
%  --------
% 	07/15/2021, by Mirko Engelpracht:
% 		First implementation.

% Destroy object
%
calllib('libsorpPropLib', 'delWorkingPair', refWPair);
% clear refWPair;

% Unload library
%
% unloadlibrary libsorpPropLib;
end

