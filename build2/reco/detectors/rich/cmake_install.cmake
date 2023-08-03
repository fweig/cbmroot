# Install script for directory: /home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich

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
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmRichReco_rdict.pcm"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmRichReco.rootmap"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmRichReco.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmRichReco.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmRichReco.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmRichReco.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmRichReco.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmRichReco.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmRichReco.so"
         OLD_RPATH "/home/heinemann/gpuunpacker/cbmroot/build2/lib:/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmRichReco.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/CbmRichTrainAnnSelect.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/CbmRichTrainAnnElectrons.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/CbmRichReconstruction.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/CbmRichHitProducer.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/mcbm/CbmRichMCbmQa.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/mcbm/CbmRichMCbmQaReal.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/mcbm/CbmRichMCbmQaRichOnly.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/mcbm/CbmRichMCbmHitProducer.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/mcbm/CbmRichMCbmAerogelAna.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/mcbm/CbmRichMCbmToTShifter.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/mcbm/CbmRichMCbmSEDisplay.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/qa/CbmRichRingFitterQa.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/qa/CbmRichGeoOpt.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/qa/CbmRichRecoTbQa.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/qa/CbmRichGeoTest.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/qa/CbmRichUrqmdTest.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/qa/CbmRichGeoTestOpt.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/qa/CbmRichRecoQa.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/qa/CbmL1RichRingQa.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/unpack/CbmRichUnpackAlgo.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/unpack/CbmRichUnpackAlgo2022.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/unpack/CbmRichUnpackAlgoBase.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/unpack/CbmRichUnpackMonitor.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/unpack/CbmRichUnpackConfig.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmRichReco.rootmap"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmRichReco_rdict.pcm"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/fitter/CbmRichRingFitterCOP.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/fitter/CbmRichRingFitterBase.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/fitter/CbmRichRingFitterEllipseBase.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/selection/CbmRichRingSelectImpl.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/finder/CbmRichRingFinderHoughImpl.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/detectors/rich/finder/CbmRichRingFinderData.h"
    )
endif()

