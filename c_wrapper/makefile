###################
# Specify project #
###################
#
# Name of project
#
PROJECT = sorpPropLib


#
# Change between 32 and 64 bit
#
# "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Auxiliary/Build/vcvarsall.bat" x86
# "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Auxiliary/Build/vcvarsall.bat" amd64
#


# Specify build release:
#
# YES: Disable debugging information
# NO: Debugging information
#
BUILD_RELEASE = YES


# Names of sources
#	
SOURCES_LIB = $(DIR_SRC)\json_interface.c\
	$(DIR_SRC)\cJSON.c\
	$(DIR_SRC)\adsorption.c\
	$(DIR_SRC)\adsorption_dualSiteSips.c\
	$(DIR_SRC)\adsorption_langmuir.c\
	$(DIR_SRC)\adsorption_toth.c\
	$(DIR_SRC)\adsorption_dubininAstakhov.c\
	$(DIR_SRC)\refrigerant.c\
	$(DIR_SRC)\refrigerant_vapourPressure.c\
	$(DIR_SRC)\refrigerant_saturatedLiquidDensity.c
	
SOURCES = $(SOURCES_LIB)\
	$(DIR_SRC)\workingPair.c


# Names of objects generated automatically:
#
# 1) Substitute path "src" with "obj"
# 2) Substitute extension ".c" with ".obj"
#
TMP_OBJECTS_LIB = $(SOURCES_LIB:src=obj)
OBJECTS_LIB = $(TMP_OBJECTS_LIB:.c=.obj)

TMP_OBJECTS = $(SOURCES:src=obj)
OBJECTS = $(TMP_OBJECTS:.c=.obj)




#################
# Specify paths #
#################
#
# Top directory
#
DIR_TOP 	= $(MAKEDIR)


# Subdirectories
#
DIR_INCL 	= $(DIR_TOP)\incl
DIR_SRC 	= $(DIR_TOP)\src
DIR_OBJ 	= $(DIR_TOP)\obj
DIR_LIB 	= $(DIR_TOP)\lib
DIR_TEST 	= $(DIR_TOP)\test




####################
# Specify programs #
####################
#
# C compiler
#
CC		= cl


# Linker
#
LINK	= link


# Library
#
LIBRARY	= lib




###########################
# Specify program options #
###########################
#
# C compiler options
#
FLAGS_CC 		= /c
FLAGS_CC_OBJ	= $(FLAGS_CC_BUILD_DEBUG) /Fo$(DIR_OBJ)\ /I$(DIR_INCL)


# Linking otpions
#
FLAGS_LINK	= $(FLAGS_LINK_BUILD_DEBUG) /OUT:$(DIR_TEST)^\


# Library options
#
FLAGS_LIBRARY	= /OUT:$(DIR_LIB)\$(PROJECT).lib 


# Program options depending on build
# or debug release /Zi
#
!IF "$(BUILD_RELEASE)"=="YES"
FLAGS_CC_BUILD_DEBUG = /Ot
FLAGS_LINK_BUILD_DEBUG = 
!ELSE
FLAGS_CC_BUILD_DEBUG = /W4 /WX
FLAGS_LINK_BUILD_DEBUG = /DEBUG
!ENDIF




##########################
# Specify explicit rules #
##########################
#
# Test all files
#
all: test_adsorption\
	test_refrigerants\
	test_workingPair\
	test_libraries


# Create libraries
#
static_library: $(DIR_LIB)\$(PROJECT).lib

shared_library: $(DIR_LIB)\workingPair.dll
	
	
# Execute test scripts for adsorption
#
test_adsorption: test_adsorption_dualSiteSips.exe\
	test_adsorption_langmuir.exe\
	test_adsorption_toth.exe\
	test_adsorption_dubininAstakhov.exe\
	test_adsorption.exe		
	cd $(DIR_TEST)	
	
	test_adsorption_dualSiteSips.exe
	test_adsorption_langmuir.exe
	test_adsorption_toth.exe
	test_adsorption_dubininAstakhov.exe
	test_adsorption.exe
	
	
# Execute test scripts for refrigerants
#
test_refrigerants: test_refrigerant_vapourPressure.exe\
	test_refrigerant_saturatedLiquidDensity.exe\
	test_refrigerant.exe	
	cd $(DIR_TEST)	
	
	test_refrigerant_vapourPressure.exe
	test_refrigerant_saturatedLiquidDensity.exe
	test_refrigerant.exe
	
	
# Execute test scripts for refrigerants
#
test_workingPair: test_workingPair.exe
	cd $(DIR_TEST)	
	
	test_workingPair.exe
	
	
# Execute test scripts for libraries
#
test_libraries: test_workingPair_staticLibrary.exe\
	test_workingPair_DLL.exe	
	cd $(DIR_TEST)	
	
	test_workingPair_staticLibrary.exe
	copy $(DIR_LIB)\workingPair.dll $(DIR_TEST)\workingPair.dll
	test_workingPair_DLL.exe
	del $(DIR_TEST)\workingPair.dll


# Update external libraries:
# 1.) cJSON
#
$(DIR_SRC)\cJSON.c: $(DIR_LIB)\cJSON\cJSON.c
	copy $(**R).h $(@R:src=incl).h
	copy $** $@


# Create internal libraries:
# 1.) static
# 2.) dynamic
#
$(DIR_LIB)\$(PROJECT).lib: $(OBJECTS)
	$(LIBRARY) $(FLAGS_LIBRARY) $**
	
$(DIR_LIB)\workingPair.lib: $(OBJECTS_LIB)
	$(CC) /DDLL_EXPORTS $(DIR_SRC)\workingPair.c /LD /Fo$(DIR_OBJ)\ /I$(DIR_INCL) $**	
	
	copy workingPair.lib $(DIR_LIB)\workingPair.lib
	copy workingPair.dll $(DIR_LIB)\workingPair.dll	
	del workingPair.lib
	del workingPair.dll
	del workingPair.exp
	
$(DIR_LIB)\workingPair.dll: $(OBJECTS_LIB)
	$(CC) /DDLL_EXPORTS $(DIR_SRC)\workingPair.c /LD /Fo$(DIR_OBJ)\ /I$(DIR_INCL) $**	
	
	copy workingPair.lib $(DIR_LIB)\workingPair.lib
	copy workingPair.dll $(DIR_LIB)\workingPair.dll	
	del workingPair.lib
	del workingPair.dll
	del workingPair.exp
	

# Create test scripts
#	
test_workingPair_DLL.exe: $(DIR_OBJ)\test_workingPair_DLL.obj $(DIR_LIB)\workingPair.lib 
	$(LINK) $(FLAGS_LINK)$(@F) $**
	
test_workingPair_staticLibrary.exe: $(DIR_OBJ)\test_workingPair_staticLibrary.obj $(DIR_LIB)\$(PROJECT).lib
	$(LINK) $(FLAGS_LINK)$(@F) $**
	
test_workingPair.exe: $(DIR_OBJ)\test_workingPair.obj $(OBJECTS)
	$(LINK) $(FLAGS_LINK)$(@F) $**
	
test_adsorption_dualSiteSips.exe: $(DIR_OBJ)\test_adsorption_dualSiteSips.obj $(OBJECTS)
	$(LINK) $(FLAGS_LINK)$(@F) $**
	
test_adsorption_langmuir.exe: $(DIR_OBJ)\test_adsorption_langmuir.obj $(OBJECTS)
	$(LINK) $(FLAGS_LINK)$(@F) $**
	
test_adsorption_toth.exe: $(DIR_OBJ)\test_adsorption_toth.obj $(OBJECTS)
	$(LINK) $(FLAGS_LINK)$(@F) $**
	
test_adsorption_dubininAstakhov.exe: $(DIR_OBJ)\test_adsorption_dubininAstakhov.obj $(OBJECTS)
	$(LINK) $(FLAGS_LINK)$(@F) $**
	
test_adsorption.exe: $(DIR_OBJ)\test_adsorption.obj $(OBJECTS)
	$(LINK) $(FLAGS_LINK)$(@F) $**
	
test_refrigerant_vapourPressure.exe: $(DIR_OBJ)\test_refrigerant_vapourPressure.obj $(OBJECTS)
	$(LINK) $(FLAGS_LINK)$(@F) $**
	
test_refrigerant_saturatedLiquidDensity.exe: $(DIR_OBJ)\test_refrigerant_saturatedLiquidDensity.obj $(OBJECTS)
	$(LINK) $(FLAGS_LINK)$(@F) $**
	
test_refrigerant.exe: $(DIR_OBJ)\test_refrigerant.obj $(OBJECTS)
	$(LINK) $(FLAGS_LINK)$(@F) $**




###########################
# Specify inference rules #
###########################	
#
# Update object-files that
# depend on source files
#
{$(DIR_SRC)}.c{$(DIR_OBJ)}.obj:
	$(CC) $(FLAGS_CC) $< $(FLAGS_CC_OBJ) 
