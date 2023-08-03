# Install script for directory: /home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format

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
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaCbmFormat_rdict.pcm"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaCbmFormat.rootmap"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaCbmFormat.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaCbmFormat.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaCbmFormat.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaCbmFormat.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaCbmFormat.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaCbmFormat.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaCbmFormat.so"
         OLD_RPATH "/home/heinemann/gpuunpacker/cbmroot/build2/lib:/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaCbmFormat.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/NicaCbmFormatTypes.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/combined/NicaCbmFullEvent.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/combined/CbmHbtFullEvent.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/unigen/NicaGeneratorReadUnigen.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/unigen/NicaGeneratorWriteUnigen.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/unigen/NicaUnigenSource.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/unigen/NicaUnigenEvent.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/unigen/NicaUnigenEventInterface.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/unigen/NicaUnigenTrack.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/unigen/NicaUnigenTrackInterface.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/anatree/mc/NicaCbmMCEvent.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/anatree/mc/NicaCbmMCEventInterface.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/anatree/mc/NicaCbmMCTrack.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/anatree/mc/NicaCbmMCTrackInterface.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/anatree/reco/NicaCbmEvent.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/anatree/reco/NicaCbmEventInterface.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/anatree/reco/NicaCbmTrack.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/anatree/reco/NicaCbmTrackInterface.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/anatree/femto/CbmHbtTrack.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/anatree/femto/CbmHbtEvent.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/anatree/CbmAnaTreeContainer.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGC2F/femtoscopy/nicafemto/format/anatree/CbmAnaTreeSource.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaCbmFormat.rootmap"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaCbmFormat_rdict.pcm"
    )
endif()

