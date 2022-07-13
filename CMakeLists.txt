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

#Check if necessary environment variables are set
#If not stop execution
IF(NOT DEFINED ENV{SIMPATH} AND NOT DEFINED SIMPATH)
message (FATAL_ERROR "\
You did not define the environment variable SIMPATH or define SIMPATH when calling cmake. \
Either of the two is needed to properly find the external packages. \
Please either set this environment variable or pass -DSIMPATH=<path> and execute cmake again. \
")
ENDIF()
if (NOT DEFINED SIMPATH)
  set(SIMPATH $ENV{SIMPATH})
endif()

IF(NOT DEFINED ENV{FAIRROOTPATH} AND NOT DEFINED FAIRROOTPATH)
  message(FATAL_ERROR "\
You did not define the environment variable FAIRROOTPATH or define FAIRROOTPATH when calling cmake. \
Either of the two is needed to properly find the external packages. \
Please set this environment variable or pass -DFAIRROOTPATH=<path> and and execute cmake again.")
ENDIF()
if (NOT DEFINED FAIRROOTPATH)
  set(FAIRROOTPATH $ENV{FAIRROOTPATH})
endif()


# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/
# is checked
set(CMAKE_MODULE_PATH "${FAIRROOTPATH}/share/fairbase/cmake/modules_old" ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH "${FAIRROOTPATH}/share/fairbase/cmake/modules"  ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})

#set(CMAKE_PREFIX_PATH "$ENV{SIMPATH}/share/cmake/ZeroMQ" ${CMAKE_PREFIX_PATH})
set(CMAKE_PREFIX_PATH ${SIMPATH} ${CMAKE_PREFIX_PATH})
set(CMAKE_INSTALL_LIBDIR lib)

set(FairRoot_DIR ${FAIRROOTPATH})
find_package(FairRoot)

# Load some basic macros which are needed later on
include(CbmMacros)
include(CbmTargets)
include(CbmGenerateConfig)
include(CbmCompilerSettings)
include(FairMacros)
include(WriteConfigFile)
include(CTest)
include(CheckCompiler)
Include(ExternalProject)

# CbmRoot needs from 07.04.22 at least C++17 support, so we need an compiler which supports
# this C++ standard.
# Check if the used compiler support C++11. If not stop with an error message
# Check also if FairSoft and FairRoot have been compiled with C++17 support
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++17" _HAS_CXX17_FLAG)

If(NOT _HAS_CXX17_FLAG)
  Message(FATAL_ERROR "The used C++ compiler (${CMAKE_CXX_COMPILER}) does not support C++17. CbmRoot can only be compiled with compilers supporting C++17. Please install such an compiler.")
EndIf()

Execute_process(COMMAND ${SIMPATH}/bin/fairsoft-config --cxxflags OUTPUT_VARIABLE _res_fairsoft_config OUTPUT_STRIP_TRAILING_WHITESPACE)
String(FIND ${_res_fairsoft_config} "-std=c++17" POS_C++17)
If(${POS_C++17} EQUAL -1)
  Message(FATAL_ERROR "FairSoft wasn't compiled with support for c++17. Please recompile FairSoft with a compiler which supports c++17.")
else()
  set(CMAKE_CXX_STANDARD 17)
EndIf()

Execute_process(COMMAND ${SIMPATH}/bin/fairsoft-config --root-version OUTPUT_VARIABLE _res_root_version OUTPUT_STRIP_TRAILING_WHITESPACE)
If(NOT ${_res_root_version} EQUAL 6)
  Message(FATAL_ERROR "FairSoft was not compiled with ROOT6.")
EndIf()

# Extract the FairRoot version from fairroot-config
# The version info is of the form Major.Minor.Patch e.g. 15.11.1 and
# is stored in the variable FairRoot_VERSION and FairSoft_VERSION
FairRootVersion()
FairSoftVersion()

# Since 07.04.2022 CbmRoot need at least FairRoot v18.6.7 and FairSoft apr21p2
# Check if FairRoot and FairSoft have at least this versions
If(FairRoot_VERSION VERSION_LESS 18.6.7)
  Message(FATAL_ERROR "\n CbmRoot needs at least FairRoot version v18.6.7. \n You use FairRoot ${FairRoot_VERSION}. Please upgrade your FairRoot version.")
EndIf()
# FairSoft version 21.4.0 means apr21
If(FairSoft_VERSION VERSION_LESS 21.4.0)
  Message(FATAL_ERROR "\n CbmRoot needs at least FairSoft version apr21p2. \n You use FairSoft ${_fairsoft_version}. Please upgrade your FairSoft version.")
EndIf()

Execute_process(COMMAND ${FAIRROOTPATH}/bin/fairroot-config --fairsoft_path OUTPUT_VARIABLE _simpath OUTPUT_STRIP_TRAILING_WHITESPACE)
Remove_Trailing_Slash(${SIMPATH})
Set(_simpath ${_ret_val})
Remove_Trailing_Slash(${_simpath})
Set(_fairroot_config ${_ret_val})
String(COMPARE EQUAL ${_simpath} ${_fairroot_config}  _same_fairsoft)
If(NOT _same_fairsoft)
    Message(STATUS "FairSoft version used for FairRoot compilation: ${_fairroot_config}")
    Message(STATUS "FairSoft version now (defined by SIMPATH): ${_simpath}")
    Message(FATAL_ERROR "Both versions must be the same.")
EndIf()


# Set the compiler flags
set_compiler_flags()

Set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
Set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
Set(INCLUDE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/include")

Set(VMCWORKDIR ${CBMROOT_SOURCE_DIR})


# Check if the user wants to build the project in the source
# directory
CHECK_OUT_OF_SOURCE_BUILD()

# Check if we are on an UNIX system. If not stop with an error
# message
IF(NOT UNIX)
  MESSAGE(FATAL_ERROR "You're not on an UNIX system. The project was up to now only tested on UNIX systems, so we break here. If you want to go on please edit the CMakeLists.txt in the source directory.")
ENDIF(NOT UNIX)


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
