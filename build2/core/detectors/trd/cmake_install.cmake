# Install script for directory: /home/heinemann/gpuunpacker/cbmroot/core/detectors/trd

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

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibrariesx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmTrdBase_rdict.pcm"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmTrdBase.rootmap"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmTrdBase.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmTrdBase.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmTrdBase.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmTrdBase.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmTrdBase.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmTrdBase.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmTrdBase.so"
         OLD_RPATH "/home/heinemann/gpuunpacker/cbmroot/build2/lib:/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmTrdBase.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd/CbmTrdGas.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd/CbmTrdContFact.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd/CbmTrdParManager.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd/CbmTrdModuleAbstract.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd/CbmMcbm2020TrdTshiftPar.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd/CbmTrdParSet.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd/CbmTrdParSetAsic.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd/CbmTrdParSetGas.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd/CbmTrdParSetGain.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd/CbmTrdParSetGeo.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd/CbmTrdParSetDigi.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd/CbmTrdParMod.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd/CbmTrdParAsic.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd/CbmTrdParSpadic.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd/CbmTrdSpadic.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd/CbmTrdFASP.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd/CbmTrdParFasp.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd/CbmTrdParModDigi.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd/CbmTrdParModGas.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd/CbmTrdParModGain.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd/CbmTrdParModGeo.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd/CbmTrdTrackingInterface.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd/CbmTrdGeoHandler.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd/CbmTrdUtils.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd/CbmTrddEdxUtils.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd/CbmTrdHardwareSetupR.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd/CbmTrdRadiator.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmTrdBase.rootmap"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmTrdBase_rdict.pcm"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd/CbmTrdDefs.h")
endif()

