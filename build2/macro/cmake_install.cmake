# Install script for directory: /home/heinemann/gpuunpacker/cbmroot/macro

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "FullWarnings")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/heinemann/gpuunpacker/cbmroot/build2/macro/run/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/heinemann/gpuunpacker/cbmroot/build2/macro/qa/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/heinemann/gpuunpacker/cbmroot/build2/macro/mcbm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/heinemann/gpuunpacker/cbmroot/build2/macro/mvd/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/heinemann/gpuunpacker/cbmroot/build2/macro/much/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/heinemann/gpuunpacker/cbmroot/build2/macro/include/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/heinemann/gpuunpacker/cbmroot/build2/macro/reco/cmake_install.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cbmroot/macro" TYPE PROGRAM FILES "/home/heinemann/gpuunpacker/cbmroot/scripts/checklibs.sh")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cbmroot/macro" TYPE FILE FILES "/home/heinemann/gpuunpacker/cbmroot/scripts/loadlib.C")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cbmroot/macro/PWG/common/production" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/macro/PWG/common/production/run_json.sh"
    "/home/heinemann/gpuunpacker/cbmroot/macro/PWG/common/production/run_sim_reco_json.sh"
    "/home/heinemann/gpuunpacker/cbmroot/macro/PWG/common/production/run_transport_json_config.C"
    "/home/heinemann/gpuunpacker/cbmroot/macro/PWG/common/production/run_digi_json_config.C"
    "/home/heinemann/gpuunpacker/cbmroot/macro/PWG/common/production/run_reco_json_config.C"
    "/home/heinemann/gpuunpacker/cbmroot/macro/PWG/common/production/run_analysis_tree_maker_json_config.C"
    "/home/heinemann/gpuunpacker/cbmroot/macro/PWG/common/production/config.json"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  FILE(MAKE_DIRECTORY /usr/local/share/cbmroot/macro/PWG/common/production)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cbmroot/macro/KF" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/macro/KF/DecayConfig.C"
    "/home/heinemann/gpuunpacker/cbmroot/macro/KF/registerGeantDecays.C"
    "/home/heinemann/gpuunpacker/cbmroot/macro/KF/registerLightIons.C"
    "/home/heinemann/gpuunpacker/cbmroot/macro/KF/registerPythiaDecays.C"
    "/home/heinemann/gpuunpacker/cbmroot/macro/KF/kf_kfparticle.C"
    "/home/heinemann/gpuunpacker/cbmroot/macro/KF/kf_thermal_signal_generator.C"
    "/home/heinemann/gpuunpacker/cbmroot/macro/KF/kf_transport.C"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  FILE(MAKE_DIRECTORY /usr/local/share/cbmroot/macro/KF/data)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cbmroot/macro/c2f" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/macro/C2F/.rootrc"
    "/home/heinemann/gpuunpacker/cbmroot/macro/C2F/c2f_transport.C"
    "/home/heinemann/gpuunpacker/cbmroot/macro/C2F/sis100_electron.tree.list"
    "/home/heinemann/gpuunpacker/cbmroot/macro/analysis/common/analysis_tree_converter/run_analysis_tree_maker.C"
    "/home/heinemann/gpuunpacker/cbmroot/macro/analysis/common/at_kfpf_interface/run_at_kfpf.C"
    "/home/heinemann/gpuunpacker/cbmroot/macro/analysis/common/qa/run_analysistree_qa.C"
    "/home/heinemann/gpuunpacker/cbmroot/macro/analysis/common/cuts/cbm_cuts.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  FILE(MAKE_DIRECTORY /usr/local/share/cbmroot/macro/c2f/data)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cbmroot/macro/dimuon" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/macro/dimuon/.rootrc"
    "/home/heinemann/gpuunpacker/cbmroot/macro/dimuon/dimuon_transport.C"
    "/home/heinemann/gpuunpacker/cbmroot/macro/dimuon/invM.C"
    "/home/heinemann/gpuunpacker/cbmroot/macro/much/run_ana.C"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  FILE(MAKE_DIRECTORY /usr/local/share/cbmroot/macro/dimuon/data)
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/heinemann/gpuunpacker/cbmroot/build2/macro/beamtime/cmake_install.cmake")
endif()

