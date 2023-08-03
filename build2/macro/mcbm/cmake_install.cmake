# Install script for directory: /home/heinemann/gpuunpacker/cbmroot/macro/mcbm

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

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cbmroot/macro/mcbm" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/.rootrc"
    "/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/mcbm_transport.C"
    "/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/mcbm_digi.C"
    "/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/mcbm_reco_event.C"
    "/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/mcbm_check_overlaps.C"
    "/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/mcbm_match_check.C"
    "/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/mcbm_hadron_analysis.C"
    "/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/mcbm_qa.C"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cbmroot/macro/mcbm" TYPE DIRECTORY FILES "/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/modules")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  FILE(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/share/cbmroot/macro/mcbm/data)
endif()

