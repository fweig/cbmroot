# The name of our project is "CBMROOT".  CMakeLists files in this project can

# refer to the root source directory of the project as ${CBMROOT_SOURCE_DIR} or
# as ${CMAKE_SOURCE_DIR} and to the root binary directory of the project as
# ${CBMROOT_BINARY_DIR} or ${CMAKE_BINARY_DIR}.
# This difference is important for the base classes which are in CBMROOT
# and PANDAROOT.

# Check if cmake has the required version
# For the CTest fixtures at least CMake 3.7.2 is needed.
# Since NicaFempto requires 3.11.0 we should be consistent while we
# build it in the CbmRoot context
CMAKE_MINIMUM_REQUIRED(VERSION 3.11.0 FATAL_ERROR)

# Set name of our project to "CBMROOT". Has to be done
# after check of cmake version
project(CBMROOT)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

FIND_PATH(FAIRBASE NAMES FairRun.h  PATHS
  ${CBMBASE_DIR}/steer
  NO_DEFAULT_PATH
)

If (FAIRBASE)
  Message(STATUS "Found FAIRBASE")
  SET(FAIRBASE ${FAIRBASE})
Else (FAIRBASE)
  Message(STATUS "NOT Found FAIRBASE")
  IF(NOT DEFINED ENV{FAIRROOTPATH})
     MESSAGE(FATAL_ERROR "You did not define the environment variable FAIRROOTPATH which is needed to find FairRoot. Please set this variable and execute cmake again.")
  ENDIF(NOT DEFINED ENV{FAIRROOTPATH})
  SET(FAIRROOTPATH $ENV{FAIRROOTPATH})

EndIf (FAIRBASE)

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/
# is checked
set(CMAKE_MODULE_PATH "${FAIRROOTPATH}/share/fairbase/cmake/modules_old" ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH "${FAIRROOTPATH}/share/fairbase/cmake/modules"  ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})

#set(CMAKE_PREFIX_PATH "$ENV{SIMPATH}/share/cmake/ZeroMQ" ${CMAKE_PREFIX_PATH})
set(CMAKE_PREFIX_PATH $ENV{SIMPATH} ${CMAKE_PREFIX_PATH})
set(CMAKE_INSTALL_LIBDIR lib)

if(FAIRROOTPATH)
  find_package(FairRoot)
  SET(MY_ROOT_INCLUDE_PATH ${FAIRROOT_INCLUDE_DIR})
  If(CMAKE_INSTALL_PREFIX)
     Message("***** HERE *****")
     SET(MY_ROOT_INCLUDE_PATH 
         ${MY_ROOT_INCLUDE_PATH} 
         "${CMAKE_INSTALL_PREFIX}/include" 
         "${CMAKE_INSTALL_PREFIX}/include/mvd"
         "${CMAKE_INSTALL_PREFIX}/include/littrack"
         "${CMAKE_INSTALL_PREFIX}/include/KF"
        )
  EndIf(CMAKE_INSTALL_PREFIX)
endif(FAIRROOTPATH)

# Load some basic macros which are needed later on
include(CbmMacros)
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

Execute_process(COMMAND $ENV{SIMPATH}/bin/fairsoft-config --cxxflags OUTPUT_VARIABLE _res_fairsoft_config OUTPUT_STRIP_TRAILING_WHITESPACE)
String(FIND ${_res_fairsoft_config} "-std=c++17" POS_C++17)
If(${POS_C++17} EQUAL -1)
  Message(FATAL_ERROR "FairSoft wasn't compiled with support for c++17. Please recompile FairSoft with a compiler which supports c++17.")
else()
  set(CMAKE_CXX_STANDARD 17)
EndIf()

Execute_process(COMMAND $ENV{SIMPATH}/bin/fairsoft-config --root-version OUTPUT_VARIABLE _res_root_version OUTPUT_STRIP_TRAILING_WHITESPACE)
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

Execute_process(COMMAND $ENV{FAIRROOTPATH}/bin/fairroot-config --fairsoft_path OUTPUT_VARIABLE _simpath OUTPUT_STRIP_TRAILING_WHITESPACE)
Remove_Trailing_Slash($ENV{SIMPATH})
Set(_simpath ${_ret_val})
Remove_Trailing_Slash(${_simpath})
Set(_fairroot_config ${_ret_val})
String(COMPARE EQUAL ${_simpath} ${_fairroot_config}  _same_fairsoft)
If(NOT _same_fairsoft)
    Message(STATUS "FairSoft version used for FairRoot compilation: ${_fairroot_config}")
    Message(STATUS "FairSoft version now (defined by SIMPATH): ${_simpath}")
    Message(FATAL_ERROR "Both versions must be the same.")
EndIf()


#Check the compiler and set the compile and link flags
If(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  Set(_default_build_type FullWarnings)
  Set(_cflags "-g -O2")
  Set(_cxxflags "-g -O2 -Wshadow -Wall -Wextra -Wunused-variable")

  String(TOUPPER ${_default_build_type} upper_build_type )
  Set(_build_type_cxx CMAKE_CXX_FLAGS_${upper_build_type})
  Set(_build_type_c CMAKE_C_FLAGS_${upper_build_type})
  Set(_exe_linker_flags CMAKE_EXE_LINKER_FLAGS_${upper_build_type})
  Set(_module_linker_flags CMAKE_MODULE_LINKER_FLAGS_${upper_build_type})
  Set(_shared_linker_flags CMAKE_SHARED_LINKER_FLAGS_${upper_build_type})
  Set(_static_linker_flags CMAKE_STATIC_LINKER_FLAGS_${upper_build_type})

  Message("No build type defined on command line. Set the build type ${_default_build_type}")
  Message("CXX_FLAGS: ${_cxxflags}")
  Message("C_FLAGS: ${_cflags}")

  Set(CMAKE_BUILD_TYPE ${_default_build_type} CACHE STRING "Choose the type of build." FORCE)
  If(CMAKE_SYSTEM_NAME MATCHES Linux OR CMAKE_SYSTEM_NAME MATCHES Darwin)
    If(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
      Set(${_build_type_cxx} ${_cxxflags} CACHE STRING "Flags used by the CXX compiler during FullWarnings builds.")
      Set(${_build_type_c} ${_cflags} CACHE STRING "Flags used by the C compiler during FullWarnings builds.")
      Set(${_exe_linker_flags} "" CACHE STRING "Flags used by the linker during FullWarnings builds.")
      Set(${_module_linker_flags} "" CACHE STRING "Flags used by the linker during the creation of modules during FullWarnings builds.")
      Set(${_shared_linker_flags} "" CACHE STRING "Flags used by the linker during the creation of shared libraries during FullWarnings builds.")
      Set(${_static_linker_flags} "" CACHE STRING "Flags used by the linker during the creation of static libraries during FullWarnings builds.")
      mark_as_advanced(
        ${_build_type_cxx}
        ${_build_type_c}
        ${_exe_linker_flags}
        ${_module_linker_flags}
        ${_shared_linker_flags}
        ${_static_linker_flags}
      )
    EndIf()
  EndIf()
EndIf()
If(CMAKE_BUILD_TYPE MATCHES CONTINUOUS)
  Set(CMAKE_CXX_FLAGS_CONTINUOUS "-g -O2 -Wshadow -Wall -Wextra -Wunused-variable")
  Set(CMAKE_C_FLAGS_CONTINUOUS "-g -O2")
  Message("The build type is CONTINUOUS")
  Message("CXX_FLAGS: ${CMAKE_CXX_FLAGS_CONTINUOUS}")
  Message("C_FLAGS: ${CMAKE_C_FLAGS_CONTINUOUS}")
EndIf()
Check_Compiler()
if(APPLE)
  SET(CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS} -single_module -undefined dynamic_lookup")
endif()
set(CMAKE_CONFIGURATION_TYPES ${CMAKE_CONFIGURATION_TYPES} "CONTINUOUS" "FullWarnings")
#Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -Weffc++ -Wno-unused-variable -Wno-unused-parameter -Wno-sign-compare -Wno-ignored-qualifiers -Wno-overloaded-virtual")
#Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -Wno-padded -Wno-global-constructors")
#Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-documentation -Wno-padded -Wno-global-constructors -Wno-exit-time-destructors -Wno-extra-semi -Wno-weak-vtables -Wno-unused-member-function")
#Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wdeprecated -Wunused-exception-parameter -Wconversion -Wsign-conversion -Wold-style-cast -Wshorten-64-to-32 -Wswitch-enum -Wfloat-equal -Wunreachable-code")
#Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wold-style-cast")

Set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
Set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
Set(INCLUDE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/include")

Set(VMCWORKDIR ${CBMROOT_SOURCE_DIR})

Option(USE_PATH_INFO "Information from PATH and LD_LIBRARY_PATH are used." OFF)
If(USE_PATH_INFO)
  Set(PATH $ENV{PATH})
  If (APPLE)
    Set(LD_LIBRARY_PATH $ENV{DYLD_LIBRARY_PATH})
  Else (APPLE)
    Set(LD_LIBRARY_PATH $ENV{LD_LIBRARY_PATH})
  EndIf (APPLE)
Else(USE_PATH_INFO)
  STRING(REGEX MATCHALL "[^:]+" PATH $ENV{PATH})
EndIf(USE_PATH_INFO)

# Check if the user wants to build the project in the source
# directory
CHECK_OUT_OF_SOURCE_BUILD()

# Check if we are on an UNIX system. If not stop with an error
# message
IF(NOT UNIX)
  MESSAGE(FATAL_ERROR "You're not on an UNIX system. The project was up to now only tested on UNIX systems, so we break here. If you want to go on please edit the CMakeLists.txt in the source directory.")
ENDIF(NOT UNIX)

#Check if necessary environment variables are set
#If not stop execution
if(NOT CBMROOT_MINIMAL)
  IF(NOT DEFINED ENV{SIMPATH})
    MESSAGE(FATAL_ERROR "You did not define the environment variable SIMPATH which is nedded to find the external packages. Please set this variable and execute cmake again.")
  ENDIF(NOT DEFINED ENV{SIMPATH})
  SET(SIMPATH $ENV{SIMPATH})
else(NOT CBMROOT_MINIMAL)
  if(NOT DEFINED ENV{ROOTSYS})
    MESSAGE(FATAL_ERROR "You did not define the environment variable ROOTSYS which is nedded for the installation of the minimal version of CbmRoot. Please set this variable and execute cmake again.")
  endif(NOT DEFINED ENV{ROOTSYS})
endif(NOT CBMROOT_MINIMAL)


# Check if the external packages are installed into a separate install
# directory
CHECK_EXTERNAL_PACKAGE_INSTALL_DIR()

# searches for needed packages
# REQUIRED means that cmake will stop if this packages are not found
# For example the framework can run without GEANT4, but ROOT is
# mandatory
if(CBMROOT_MINIMAL)
  MESSAGE("******* Install minimal CbmRoot Version ********")
  find_package(ROOT 6.00.00 REQUIRED)
else(CBMROOT_MINIMAL)
  find_package(ROOT 6.00.00 REQUIRED)
  if("${ROOT_VERSION_MAJOR}.${ROOT_VERSION_MINOR}" VERSION_GREATER 6.16)
    Execute_Process(COMMAND ${ROOT_CONFIG_EXECUTABLE} --has-vmc
                    OUTPUT_VARIABLE ROOT_vmc_FOUND
                   )
    String(STRIP ${ROOT_vmc_FOUND} ROOT_vmc_FOUND)
    If(NOT ROOT_vmc_FOUND)
      find_package2(PUBLIC VMC REQUIRED)
      message("VMC_INCLUDE_DIRS: ${VMC_INCLUDE_DIRS}")
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

endif(CBMROOT_MINIMAL)

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

If(CMAKE_SYSTEM_NAME MATCHES Darwin)
  Include(CheckIncludeFile)
  Check_Include_File("_types/_uint8_t.h" _UINT8_T_EXIST)
EndIf()

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

add_custom_target(cleanlib
                  COMMAND ${CMAKE_COMMAND} -E remove libCbm*
                  COMMAND ${CMAKE_COMMAND} -E remove G__Cbm*
                  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                 )

add_custom_target(cleantest
                  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/scripts/cleantest.cmake
                 )

find_package2(PRIVATE ClangFormat)
if(ClangFormat_FOUND AND EXISTS ${CMAKE_SOURCE_DIR}/.git)
  if (FAIRROOT_FORMAT_BASE)
    set(BASE_COMMIT ${FAIRROOT_FORMAT_BASE})
  else()
    set(BASE_COMMIT upstream/master)
  endif()

  if (FAIRROOT_GIT_CLANG_FORMAT_BIN)
    set(GIT_CLANG_FORMAT_BIN ${FAIRROOT_GIT_CLANG_FORMAT_BIN})
  else()
    set(GIT_CLANG_FORMAT_BIN git-clang-format)
  endif()

  # Create a list C, C++ and header files which have been changed in the
  # current commit
  execute_process(COMMAND ${CMAKE_SOURCE_DIR}/cmake/scripts/find_files.sh
                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                  OUTPUT_VARIABLE FileList
                  OUTPUT_STRIP_TRAILING_WHITESPACE
                 )
  string(REGEX REPLACE " " ";" FileList "${FileList}")

  # Loop over the files and create the code whch is executed when running
  # "make FormatCheck".
  # The produced code will run clang-format on one of the files. If
  # clang-format finds code which are not satisfying our code rules a
  # detailed error message is created. This error message can be checked on
  # our CDash web page.
  foreach(file ${FileList})
    set(file1 ${CMAKE_BINARY_DIR}/${file}.cf_out)

    list(APPEND myfilelist ${file1})
    add_custom_command(OUTPUT ${file1}
                       COMMAND ${CMAKE_SOURCE_DIR}/cmake/scripts/check-format-1.sh ${file} ${file1}
                       WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                      )
  endforeach()

  # Create the target FormatCheck which only depends on the files creted in
  # previous step. When running "make FormatCheck" clang-format is executed
  # for all C, C++ and header files which have changed in the commit.
  add_custom_target(FormatCheck
                     DEPENDS ${myfilelist}
                    )

endif()

Option(BUILD_DOXYGEN "Build Doxygen" OFF)
if(BUILD_DOXYGEN)
  MESSAGE(STATUS "*** Building the Doxygen documentaion ***")
  ADD_SUBDIRECTORY(doxygen)
endif(BUILD_DOXYGEN)

if(RULE_CHECKER_FOUND)

  ADD_CUSTOM_TARGET(RULES
     COMMAND ${RULE_CHECKER_SCRIPT1} ${CMAKE_BINARY_DIR} viol > violations.html
     DEPENDS $ENV{ALL_RULES})

endif(RULE_CHECKER_FOUND)

if(IWYU_FOUND)

  ADD_CUSTOM_TARGET(checkHEADERS
     DEPENDS $ENV{ALL_HEADER_RULES}
  )

endif(IWYU_FOUND)

if(NOT CBMROOT_MINIMAL)
  WRITE_CONFIG_FILE(config.sh)
  WRITE_CONFIG_FILE(config.csh)

  SET(ROOT_INCLUDE_PATH 
      ${ROOT_INCLUDE_PATH} 
      "${CMAKE_INSTALL_PREFIX}/include" 
      "${CMAKE_INSTALL_PREFIX}/include/mvd"
      "${CMAKE_INSTALL_PREFIX}/include/littrack"
      "${CMAKE_INSTALL_PREFIX}/include/KF"
      "${CMAKE_INSTALL_PREFIX}/include/AnalysisTree"
      "${CMAKE_INSTALL_PREFIX}/include/AnalysisTreeQA"
     )
  set(CMAKE_INSTALL_LIBDIR lib)
  SET(VMCWORKDIR ${CMAKE_INSTALL_PREFIX}/share/cbmroot)
  WRITE_CONFIG_FILE(config.sh_install)
  WRITE_CONFIG_FILE(config.csh_install)

else(NOT CBMROOT_MINIMAL)
  SET(FULL_CONFIG_FILE "false")
  WRITE_CONFIG_FILE(config.sh)
endif(NOT CBMROOT_MINIMAL)


Install(FILES ${CMAKE_BINARY_DIR}/config.sh_install
        DESTINATION bin
        RENAME CbmRootConfig.sh
       )

Install(FILES ${CMAKE_BINARY_DIR}/check_system.sh
        DESTINATION bin
       )

Install(FILES ${CMAKE_BINARY_DIR}/config.csh_install
        DESTINATION bin
        RENAME CbmRootConfig.csh
       )


Install(DIRECTORY geometry input parameters
        DESTINATION share/cbmroot
        PATTERN ".git" EXCLUDE)
