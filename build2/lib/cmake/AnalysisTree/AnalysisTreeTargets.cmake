# Generated by CMake

if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" LESS 2.6)
   message(FATAL_ERROR "CMake >= 2.6.0 required")
endif()
cmake_policy(PUSH)
cmake_policy(VERSION 2.6...3.20)
#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Protect against multiple inclusion, which would fail when already imported targets are added once more.
set(_targetsDefined)
set(_targetsNotDefined)
set(_expectedTargets)
foreach(_expectedTarget AnalysisTreeBase AnalysisTreeInfra)
  list(APPEND _expectedTargets ${_expectedTarget})
  if(NOT TARGET ${_expectedTarget})
    list(APPEND _targetsNotDefined ${_expectedTarget})
  endif()
  if(TARGET ${_expectedTarget})
    list(APPEND _targetsDefined ${_expectedTarget})
  endif()
endforeach()
if("${_targetsDefined}" STREQUAL "${_expectedTargets}")
  unset(_targetsDefined)
  unset(_targetsNotDefined)
  unset(_expectedTargets)
  set(CMAKE_IMPORT_FILE_VERSION)
  cmake_policy(POP)
  return()
endif()
if(NOT "${_targetsDefined}" STREQUAL "")
  message(FATAL_ERROR "Some (but not all) targets in this export set were already defined.\nTargets Defined: ${_targetsDefined}\nTargets not yet defined: ${_targetsNotDefined}\n")
endif()
unset(_targetsDefined)
unset(_targetsNotDefined)
unset(_expectedTargets)


# Compute the installation prefix relative to this file.
get_filename_component(_IMPORT_PREFIX "${CMAKE_CURRENT_LIST_FILE}" PATH)
get_filename_component(_IMPORT_PREFIX "${_IMPORT_PREFIX}" PATH)
get_filename_component(_IMPORT_PREFIX "${_IMPORT_PREFIX}" PATH)
get_filename_component(_IMPORT_PREFIX "${_IMPORT_PREFIX}" PATH)
if(_IMPORT_PREFIX STREQUAL "/")
  set(_IMPORT_PREFIX "")
endif()

# Create imported target AnalysisTreeBase
add_library(AnalysisTreeBase SHARED IMPORTED)

set_target_properties(AnalysisTreeBase PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/include;${_IMPORT_PREFIX}/include/AnalysisTree"
  INTERFACE_LINK_LIBRARIES "/home/cbmdata/_deps/fairsoft_apr22/install/lib/libCore.so;/home/cbmdata/_deps/fairsoft_apr22/install/lib/libImt.so;/home/cbmdata/_deps/fairsoft_apr22/install/lib/libRIO.so;/home/cbmdata/_deps/fairsoft_apr22/install/lib/libNet.so;/home/cbmdata/_deps/fairsoft_apr22/install/lib/libHist.so;/home/cbmdata/_deps/fairsoft_apr22/install/lib/libGraf.so;/home/cbmdata/_deps/fairsoft_apr22/install/lib/libGraf3d.so;/home/cbmdata/_deps/fairsoft_apr22/install/lib/libGpad.so;/home/cbmdata/_deps/fairsoft_apr22/install/lib/libROOTDataFrame.so;/home/cbmdata/_deps/fairsoft_apr22/install/lib/libTree.so;/home/cbmdata/_deps/fairsoft_apr22/install/lib/libTreePlayer.so;/home/cbmdata/_deps/fairsoft_apr22/install/lib/libRint.so;/home/cbmdata/_deps/fairsoft_apr22/install/lib/libPostscript.so;/home/cbmdata/_deps/fairsoft_apr22/install/lib/libMatrix.so;/home/cbmdata/_deps/fairsoft_apr22/install/lib/libPhysics.so;/home/cbmdata/_deps/fairsoft_apr22/install/lib/libMathCore.so;/home/cbmdata/_deps/fairsoft_apr22/install/lib/libThread.so;/home/cbmdata/_deps/fairsoft_apr22/install/lib/libMultiProc.so;/home/cbmdata/_deps/fairsoft_apr22/install/lib/libROOTVecOps.so;EG"
)

# Create imported target AnalysisTreeInfra
add_library(AnalysisTreeInfra SHARED IMPORTED)

set_target_properties(AnalysisTreeInfra PROPERTIES
  INTERFACE_COMPILE_DEFINITIONS "\$<\$<BOOL:>:ANALYSISTREE_BOOST_FOUND>"
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/include;\$<\$<BOOL:>:>;${_IMPORT_PREFIX}/include/AnalysisTree"
  INTERFACE_LINK_LIBRARIES "AnalysisTreeBase;\$<\$<BOOL:>:>"
)

if(CMAKE_VERSION VERSION_LESS 2.8.12)
  message(FATAL_ERROR "This file relies on consumers using CMake 2.8.12 or greater.")
endif()

# Load information for each installed configuration.
get_filename_component(_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
file(GLOB CONFIG_FILES "${_DIR}/AnalysisTreeTargets-*.cmake")
foreach(f ${CONFIG_FILES})
  include(${f})
endforeach()

# Cleanup temporary variables.
set(_IMPORT_PREFIX)

# Loop over all imported files and verify that they actually exist
foreach(target ${_IMPORT_CHECK_TARGETS} )
  foreach(file ${_IMPORT_CHECK_FILES_FOR_${target}} )
    if(NOT EXISTS "${file}" )
      message(FATAL_ERROR "The imported target \"${target}\" references the file
   \"${file}\"
but this file does not exist.  Possible reasons include:
* The file was deleted, renamed, or moved to another location.
* An install or uninstall procedure did not complete successfully.
* The installation package was faulty and contained
   \"${CMAKE_CURRENT_LIST_FILE}\"
but not all the files it references.
")
    endif()
  endforeach()
  unset(_IMPORT_CHECK_FILES_FOR_${target})
endforeach()
unset(_IMPORT_CHECK_TARGETS)

# This file does not depend on other imported targets which have
# been exported from the same project but in a separate export set.

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
cmake_policy(POP)
