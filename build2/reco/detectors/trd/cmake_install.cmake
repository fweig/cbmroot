# Install script for directory: /home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd

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
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmTrdReco_rdict.pcm"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmTrdReco.rootmap"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmTrdReco.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmTrdReco.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmTrdReco.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmTrdReco.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmTrdReco.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmTrdReco.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmTrdReco.so"
         OLD_RPATH "/home/heinemann/gpuunpacker/cbmroot/build2/lib:/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmTrdReco.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/CbmTrdTrackFinderIdeal.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/CbmTrdTrackFitterIdeal.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/CbmTrdClusterFinder.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/CbmTrdHitProducer.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/CbmTrdModuleRec.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/CbmTrdModuleRecR.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/CbmTrdModuleRec2D.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/CbmTrdDigiRec.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/unpack/CbmTrdUnpackConfig.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/unpack/CbmTrdUnpackAlgoBaseR.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/unpack/CbmTrdUnpackAlgoR.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/unpack/CbmTrdUnpackAlgoLegacy2020R.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/unpack/CbmTrdUnpackMonitor.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/unpack/CbmTrdUnpackFaspAlgo.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/unpack/CbmTrdUnpackFaspConfig.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/unpack/CbmTrdUnpackFaspMonitor.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/qa/CbmTrdClusterizerFastQa.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/qa/CbmTrdHitDensityQa.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/qa/CbmTrdHitProducerClusterQa.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/qa/CbmTrdHitProducerQa.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/qa/CbmTrdOccupancyQa.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/qa/CbmTrdQa.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/qa/CbmTrdRecoQa.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/qa/CbmTrdTracksPidQa.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/qa/CbmTrdCalibTracker.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/qa/data/CbmTrdHitMC.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/pid/CbmTrdElectronsTrainAnn.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/pid/CbmTrdSetTracksPidWkn.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/pid/CbmTrdSetTracksPidModWkn.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/pid/CbmTrdSetTracksPidLike.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/pid/CbmTrdSetTracksPidANN.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/rawToDigiMethods/CbmTrdRawToDigiBaseR.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/rawToDigiMethods/CbmTrdRawToDigiMaxAdcR.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/rawToDigiMethods/CbmTrdRawToDigiFitR.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/trd/rawToDigiMethods/CbmTrdRawToDigiLookUpCorrR.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmTrdReco.rootmap"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmTrdReco_rdict.pcm"
    )
endif()

