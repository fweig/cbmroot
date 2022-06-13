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

# Do some basic checks 
# out-of-source build, unix system and
# existence of FAIRROOTPATH and SIMPATH variables 
Check_Prerequisites()

# Load cmake functionality from the FairRoot installation
if(IS_DIRECTORY ${FAIRROOTPATH}/share/fairbase/cmake/modules)
  list(APPEND CMAKE_MODULE_PATH "${FAIRROOTPATH}/share/fairbase/cmake/modules")
endif()
if(IS_DIRECTORY ${FAIRROOTPATH}/share/fairbase/cmake/modules_old)
  list(APPEND CMAKE_MODULE_PATH "${FAIRROOTPATH}/share/fairbase/cmake/modules_old")
endif()
include(FairMacros)
include(WriteConfigFile)
include(CheckCompiler)


# Has to be called after the CMAKE_MODULE_PATH contains the FairRoot
# directories since the file is part of FairRoot 
set(FairRoot_DIR ${FAIRROOTPATH})
find_package(FairRoot REQUIRED)




# Check if the compiler supports the needed C++ standard
# Check for the minimum required versions of FairSoft and FairRoot
check_external_stack()

# Set the compiler flags
set_compiler_flags()

# Allow to find <package>Config.cmake files in the FairSoft installation 
set(CMAKE_PREFIX_PATH ${SIMPATH} ${CMAKE_PREFIX_PATH})

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

# searches for needed packages
# REQUIRED means that cmake will stop if this packages are not found
# For example the framework can run without GEANT4, but ROOT is
# mandatory
find_package(ROOT 6.22.08 REQUIRED)
if("${ROOT_VERSION_MAJOR}.${ROOT_VERSION_MINOR}" VERSION_LESS 6.26)
  Execute_Process(COMMAND ${ROOT_CONFIG_EXECUTABLE} --has-vmc
                  OUTPUT_VARIABLE ROOT_vmc_FOUND
                 )

  String(STRIP ${ROOT_vmc_FOUND} ROOT_vmc_FOUND)
  If(NOT ROOT_vmc_FOUND)
    set(CMAKE_PREFIX_PATH ${SIMPATH} ${CMAKE_PREFIX_PATH})
    find_package2(PUBLIC VMC REQUIRED)
    set(VMCLIB VMCLibrary)
  endif()
else()
  set(CMAKE_PREFIX_PATH ${SIMPATH} ${CMAKE_PREFIX_PATH})
  find_package2(PUBLIC VMC REQUIRED)
  set(VMCLIB VMCLibrary)
endif()

find_package2(PUBLIC Geant3 REQUIRED)
if(Geant3_FOUND)
  Message("Geant3 was found")
  Set(Geant3_LIBRARY_DIR ${Geant3_DIR}/../../${Geant3_CMAKE_INSTALL_LIBDIR})
  FIND_PATH(Geant3_SYSTEM_DIR NAMES data PATHS
            ${Geant3_DIR}/../../share/geant3
            ${Geant3_DIR}/../../share/Geant3-${Geant3_VERSION}
            NO_DEFAULT_PATH)
endif()

find_package(GEANT4)
find_package(GEANT4DATA)
find_package(GEANT4VMC)
find_package(TBB)
find_package(SSE)
find_package(ZeroMQ)

Set(Boost_NO_SYSTEM_PATHS TRUE)
Set(Boost_NO_BOOST_CMAKE TRUE)
set(BOOST_ROOT ${SIMPATH})
Message("-- Looking for Boost ...")
# If an older version of boost is found both of the variables below are
# cached and in a second cmake run, a good boost version is found even
# if the version is to old.
# To overcome this problem both variables are cleared before checking
# for boost.
Unset(Boost_INCLUDE_DIR CACHE)
Unset(Boost_LIBRARY_DIRS CACHE)
find_package(Boost 1.48 COMPONENTS program_options thread system timer serialization regex log log_setup filesystem)
If (Boost_FOUND)
  Set(Boost_Avail 1)
Else (Boost_FOUND)
  Set(Boost_Avail 0)
EndIf (Boost_FOUND)

Find_Package(FairLogger)
Find_Package(FairMQ)

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

If(FAIRMQ_FOUND)
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
