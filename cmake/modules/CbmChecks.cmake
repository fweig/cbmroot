function (CHECK_OUT_OF_SOURCE_BUILD)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" insource)
if(insource)
  file(REMOVE_RECURSE ${CMAKE_SOURCE_DIR}/Testing)
  file(REMOVE ${CMAKE_SOURCE_DIR}/DartConfiguration.tcl)
  message(FATAL_ERROR  "\
CbmRoot must be installed as an out of source build, to keep the source directory clean. \
Please create a extra build directory and run the command 'cmake<path_to_source_dir>' in this newly created directory. \
You have also to delete the directory CMakeFiles and the file CMakeCache.txt in the source directory. \
Otherwise cmake will complain even if you run it from an out-of-source directory.\
")
endif()

endfunction (CHECK_OUT_OF_SOURCE_BUILD)

function(Check_Prerequisites)

  # Check if the user wants to build the project in the source
  # directory
  CHECK_OUT_OF_SOURCE_BUILD()

  # Check if we are on an UNIX system. If not stop with an error
  # message
  if(NOT UNIX)
message(FATAL_ERROR "\
You're not on an UNIX system. The project was up to now only tested on UNIX systems, so we break here. \
If you want to go on please edit the CMakeLists.txt in the source
directory.\
")
  endif()

  #Check if necessary environment variables are set
  #If not stop execution
  if(NOT DEFINED ENV{SIMPATH} AND NOT DEFINED SIMPATH)
message (FATAL_ERROR "\
You did not define the environment variable SIMPATH or define SIMPATH when calling cmake. \
Either of the two is needed to properly find the external packages. \
Please either set this environment variable or pass -DSIMPATH=<path> and execute cmake again. \
")
  endif()
  if (NOT DEFINED SIMPATH)
    set(SIMPATH $ENV{SIMPATH} PARENT_SCOPE)
  endif()

  if(NOT DEFINED ENV{FAIRROOTPATH} AND NOT DEFINED FAIRROOTPATH)
  message(FATAL_ERROR "\
You did not define the environment variable FAIRROOTPATH or define FAIRROOTPATH when calling cmake. \
Either of the two is needed to properly find the external packages. \
Please set this environment variable or pass -DFAIRROOTPATH=<path> and and execute cmake again.")
  endif()
  if (NOT DEFINED FAIRROOTPATH)
    set(FAIRROOTPATH $ENV{FAIRROOTPATH} PARENT_SCOPE)
  endif()
  if (NOT DEFINED FairRoot_DIR)
    set(FairRoot_DIR ${FAIRROOTPATH} PARENT_SCOPE)
  endif()
endfunction()

function(check_external_stack)
  # CbmRoot needs from 07.04.22 at least C++17 support, so we need an compiler which supports
  # this C++ standard.
  # Check if the used compiler support C++11. If not stop with an error message
  # Check also if FairSoft and FairRoot have been compiled with C++17 support
  include(CheckCXXCompilerFlag)
  check_cxx_compiler_flag("-std=c++17" _HAS_CXX17_FLAG)

  If(NOT _HAS_CXX17_FLAG)
    Message(FATAL_ERROR "The used C++ compiler (${CMAKE_CXX_COMPILER}) does not support C++17. CbmRoot can only be compiled with compilers supporting C++17.
    Please install such a compiler.")
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

  # Make the variables available in the parent scope
  # Since the function is executed from the main CMakeLists.txt the
  # variables are defined in global scope
  set(FairRoot_VERSION ${FairRoot_VERSION} PARENT_SCOPE)
  set(FairSoft_VERSION ${FairSoft_VERSION} PARENT_SCOPE)

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
endfunction()
