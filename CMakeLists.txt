# The name of our project is "CBMROOT".  CMakeLists files in this project can
# refer to the root source directory of the project as ${CBMROOT_SOURCE_DIR} or
# as ${CMAKE_SOURCE_DIR} and to the root binary directory of the project as
# ${CBMROOT_BINARY_DIR} or ${CMAKE_BINARY_DIR}.

# Check if cmake has the required version
# Since flesnet requires 3.14.0 we should be consistent while we
# build it in the CbmRoot context
cmake_minimum_required(VERSION 3.14.0 FATAL_ERROR)
cmake_policy(VERSION 3.14...3.23)

# Define compiler on macosx to avoid problems when comparing to compilers
# used for FairSoft/FairRoot compilation
if(APPLE)
  if(NOT DEFINED CMAKE_C_COMPILER)
    set(CMAKE_C_COMPILER clang)
  endif()
  if(NOT DEFINED CMAKE_CXX_COMPILER)
    set(CMAKE_CXX_COMPILER clang++)
  endif()
endif()

# Default CMake settings
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set the library version in the main CMakeLists.txt
set(CBMROOT_MAJOR_VERSION 22)
set(CBMROOT_MINOR_VERSION 2)
set(CBMROOT_PATCH_VERSION 99)

# Set name of our project to "CBMROOT"
# This also sets ${CBMROOT_VERSION} to the provided VERSION value, which would be empty if not set here explicitly
project(CBMROOT 
        VERSION ${CBMROOT_MAJOR_VERSION}.${CBMROOT_MINOR_VERSION}.${CBMROOT_PATCH_VERSION}
        LANGUAGES C CXX
)

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/
# is checked
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

# Load some basic cmake functionality
include(ExternalProject)
include(CTest)

# Make the CbmRoot cmake functions available
list(PREPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
include(CbmChecks)
include(CbmGenerateConfig)
include(CbmMacros)
include(CbmTargets)
include(CbmCompilerSettings)
include(ROOTMacros)

include(FairMacros)
include(WriteConfigFile)
include(CheckCompiler)
include(ROOTMacros)

# Do some basic checks
# out-of-source build, unix system and
# existence of FAIRROOTPATH and SIMPATH variables
Check_Prerequisites()

#Fairsoft Modules Path
list(PREPEND CMAKE_PREFIX_PATH ${SIMPATH})

#Searches for ROOT Package
find_package(ROOT 6.22 CONFIG REQUIRED)
if(ROOT_FOUND)
  Message("Root was found")
  set(ROOT_CINT_EXECUTABLE ${SIMPATH}/bin/rootcling)
  set(ROOT_CONFIG_EXECUTABLE ${SIMPATH}/bin/root-config)
  set(ROOT_INCLUDE_DIR ${ROOT_INCLUDE_DIRS})
endif()

find_package(FairCMakeModules REQUIRED)
include(FairFindPackage2)

#Searches for FairLogger Package
find_package2(PUBLIC FairLogger)
# at the end after all find_package[2] calls, call once
find_package2_implicit_dependencies()
if(FairLogger_FOUND)
  Message("FairLogger was found")
  set(FAIRLOGGER_FOUND TRUE)
  set(FAIRLOGGER_INCLUDE_DIR ${FairLogger_INCDIR}/fairlogger)

  # CbmRoot expects that the Logger include statement does not need any
  # additional path in the statement
  # The target FairLogger::FairLogger does not provide the needed info
  # So we add the additional subdirectory here to the target
  get_target_property(_incDirs FairLogger::FairLogger INTERFACE_INCLUDE_DIRECTORIES)
  set_target_properties(FairLogger::FairLogger PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${_incDirs}/fairlogger"
    )
endif()

#Searches for VMC Package
if(ROOT_vmc_FOUND)
  set(VMCLIB ROOT::VMC)
  Message("Root_VMC was found")
else()
  find_package2(PUBLIC VMC REQUIRED)
  set(VMCLIB VMCLibrary)
endif()
if(VMC_FOUND)
  Message("VMC was found")
endif()

#Searches for FairROOT Package
#include(FairRootTargets)
find_package(FairRoot REQUIRED)
if(FAIRROOT_FOUND)
  Message("FairRoot was found")
endif()


#Searching for Boost Package
find_package(Boost COMPONENTS serialization regex filesystem log container program_options)
if(Boost_FOUND)
  Message("Boost was found")
endif()


#Searching for Geant4 Package
find_package(Geant4 CONFIG REQUIRED)
if(Geant4_FOUND)
  Message("Geant4 was found")
endif()


#Searching for VGM Package
find_package(VGM CONFIG REQUIRED)
if(VGM_FOUND)
  Message("VGM was found")
endif()

#Searching for Geant4VMC Package
find_package(Geant4VMC CONFIG REQUIRED)
if(Geant4VMC_FOUND)
  Message("Geant4VMC was found")
endif()

#Searching for Geant3 Package
find_package(Geant3 CONFIG REQUIRED)
if(Geant3_FOUND)
  Message("Geant3 was found")
endif()

#Searching for SSE
find_package(SSE REQUIRED)
if(SSE_FOUND)
  Message("SSE was found")
endif()


#Searching for FairMQ
find_package(FairMQ CONFIG REQUIRED)
if(FairMQ_FOUND)
  Message("FairMQ was found")
  set(FAIRMQ_INCLUDE_DIR ${FairMQ_INCDIR}/fairmq)
endif()

# Check if the compiler supports the needed C++ standard
# Check for the minimum required versions of FairSoft and FairRoot
check_external_stack()

# Set the compiler flags
set_compiler_flags()

# Define where the produced targets are placed in the binary directory
Set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
Set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
Set(INCLUDE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/include")
set(CMAKE_INSTALL_LIBDIR lib)

# VMCWORKDIR is needed by the CbmRoot runtime to find macros, geometries and
# parameters
Set(VMCWORKDIR ${CBMROOT_SOURCE_DIR})

# Set library versioning
set(FAIRROOT_LIBRARY_PROPERTIES ${FAIRROOT_LIBRARY_PROPERTIES}
    VERSION "${CBMROOT_VERSION}"
    SOVERSION "${CBMROOT_MAJOR_VERSION}"
)

find_package(OpenMP)

# set a variable which should be used in all CMakeLists.txt
# Defines all basic include directories from fairbase
SetBasicVariables()

# Add the Geant3 include directory such that ROOT can find TGeant3.h
# Has to be done after calling SetBasicVariables, otherwise the value
# is overwritten
SET(ROOT_INCLUDE_PATH ${ROOT_INCLUDE_PATH} "${Geant3_INCLUDE_DIRS}")

Set(BASE_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES}
  ${SYSTEM_INCLUDE_DIRECTORIES}
  ${FAIRLOGGER_INCLUDE_DIR}
  ${VMC_INCLUDE_DIRS}
)

SET(CBMLIBDIR ${CMAKE_BINARY_DIR}/lib)
SET(LD_LIBRARY_PATH  ${CBMLIBDIR} ${Boost_LIBRARY_DIRS} ${LD_LIBRARY_PATH})

# Recurse into the given subdirectories.  This does not actually
# cause another cmake executable to run.  The same process will walk through
# the project's entire directory structure.

Option(BUILD_UNITTESTS "Build all unit tests and add them as new tests" ON)
If(BUILD_UNITTESTS)
  Message("Build the unit tests.")

  Set(GTEST_ROOT ${SIMPATH})
    find_package2(PRIVATE GTest VERSION 1.7.0)
    if(NOT GTEST_FOUND)
      Message(STATUS "No gtest installation found, build our internal one.")
      Include(external/InstallGtest.cmake)
      set(CBM_LOCAL_GTEST TRUE)
    else()
      set(CBM_LOCAL_GTEST FALSE)
    endif()
endif()

### Subdirectories to be compiled #####

### Externals included and build from several sources
# Must be the first subdirectory since the defined targets are needed by
# following targets
add_subdirectory (external)

set (BASE_INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${XPU_INCLUDE_DIRECTORY} # Required for XPU_D macro in base data types
)


set(BASE_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES}
  ${GSL_INCLUDE_DIRECTORY}
)


### Base directories
add_subdirectory (core)
add_subdirectory (algo)
add_subdirectory (sim)
add_subdirectory (reco)
add_subdirectory (analysis)

### Detector directories
add_subdirectory (mvd)

### Others
Option(LARGE_TEST_STATISTIC "Run the test suite with large statistic (100 events)" OFF)

if(NOT CBM_TEST_MODEL)
  set(CBM_TEST_MODEL Experimental)
endif()

add_subdirectory (macro)
add_subdirectory(fles)

If(FairMQ_FOUND)
  add_subdirectory(MQ)
EndIf()

add_subdirectory(tutorials)

option(BUILD_DOXYGEN "Build Doxygen" OFF)
if(BUILD_DOXYGEN)
  message(STATUS "*** Building the Doxygen documentaion ***")
  add_subdirectory(doxygen)
endif()

# Definde additional targets 
# cleanlib: rm al Cbm libraries and dictionaries
# cleantest: remove all files from build directory created when running ROOT macros
# FormatCheck: execute clang-format for all changed files (needed for CI) 
define_additional_targets()

# generate the needed config files to run macros from build and 
# installation directories. The later will be installed into the
# install directory
generate_config_files()

# install the parameters, geometries and input directories
Install(DIRECTORY geometry input parameters
        DESTINATION share/cbmroot
        PATTERN ".git" EXCLUDE)
