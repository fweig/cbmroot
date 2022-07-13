# The name of our project is "CBMROOT".  CMakeLists files in this project can
# refer to the root source directory of the project as ${CBMROOT_SOURCE_DIR} or
# as ${CMAKE_SOURCE_DIR} and to the root binary directory of the project as
# ${CBMROOT_BINARY_DIR} or ${CMAKE_BINARY_DIR}.
# This difference is important for the base classes which are in CBMROOT
# and PANDAROOT.

# Check if cmake has the required version
# For the CTest fixtures at least CMake 3.7.2 is needed.
# Since flesnet requires 3.14.0 we should be consistent while we
# build it in the CbmRoot context
cmake_minimum_required(VERSION 3.14)

# Set name of our project to "CBMROOT". Has to be done
# after check of cmake version
project(CBMROOT)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Make the CbmRoot cmake functions available
list(PREPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
include(CbmChecks)
include(CbmGenerateConfig)

# Load some basic macros which are needed later on
include(CbmMacros)
include(CbmTargets)
include(CbmGenerateConfig)
include(CbmCompilerSettings)

# Do some basic checks 
# out-of-source build, unix system and
# existence of FAIRROOTPATH and SIMPATH variables 
Check_Prerequisites()

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/
# is checked
list(APPEND CMAKE_MODULE_PATH "${FAIRROOTPATH}/share/fairbase/cmake/modules_old")
list(APPEND CMAKE_MODULE_PATH "${FAIRROOTPATH}/share/fairbase/cmake/modules")

set(CMAKE_PREFIX_PATH ${SIMPATH} ${CMAKE_PREFIX_PATH})
set(CMAKE_INSTALL_LIBDIR lib)

set(FairRoot_DIR ${FAIRROOTPATH})
find_package(FairRoot)

include(FairMacros)
include(WriteConfigFile)
include(CTest)
include(CheckCompiler)
Include(ExternalProject)

# Check if the compiler supports the needed C++ standard
# Check for the minimum required versions of FairSoft and FairRoot
check_external_stack()
 
# Set the compiler flags
set_compiler_flags()

Set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
Set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
Set(INCLUDE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/include")

Set(VMCWORKDIR ${CBMROOT_SOURCE_DIR})

# searches for needed packages
# REQUIRED means that cmake will stop if this packages are not found
# For example the framework can run without GEANT4, but ROOT is
# mandatory
find_package(ROOT 6.00.00 REQUIRED)
if("${ROOT_VERSION_MAJOR}.${ROOT_VERSION_MINOR}" VERSION_GREATER 6.16)
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
#  find_package(IWYU)
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

# Set the library version in the main CMakeLists.txt
SET(CBMROOT_MAJOR_VERSION 0)
SET(CBMROOT_MINOR_VERSION 0)
SET(CBMROOT_PATCH_VERSION 0)
SET(CBMROOT_VERSION "${CBMROOT_MAJOR_VERSION}.${CBMROOT_MINOR_VERSION}.${CBMROOT_PATCH_VERSION}")
If(NOT ROOT_FOUND_VERSION OR ROOT_FOUND_VERSION LESS 59999)
  SET(FAIRROOT_LIBRARY_PROPERTIES ${FAIRROOT_LIBRARY_PROPERTIES}
      VERSION "${CBMROOT_VERSION}"
      SOVERSION "${CBMROOT_MAJOR_VERSION}"
      SUFFIX ".so"
  )
Else()
  SET(FAIRROOT_LIBRARY_PROPERTIES ${FAIRROOT_LIBRARY_PROPERTIES}
      VERSION "${CBMROOT_VERSION}"
      SOVERSION "${CBMROOT_MAJOR_VERSION}"
  )
EndIf()

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

### Externals included and build from sveral sources
add_subdirectory (external)

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

Option(BUILD_DOXYGEN "Build Doxygen" OFF)
if(BUILD_DOXYGEN)
  MESSAGE(STATUS "*** Building the Doxygen documentaion ***")
  ADD_SUBDIRECTORY(doxygen)
endif(BUILD_DOXYGEN)

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
