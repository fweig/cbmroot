# Install script for directory: /home/heinemann/gpuunpacker/cbmroot/core/data

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
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmData_rdict.pcm"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmData.rootmap"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmData.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmData.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmData.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmData.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmData.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmData.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmData.so"
         OLD_RPATH "/home/cbmdata/_deps/fairroot_v18.6.9/install/lib:/home/heinemann/gpuunpacker/cbmroot/build2/lib:/home/cbmdata/_deps/fairsoft_apr22/install/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmData.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/core/data/base/CbmDigiData.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/global/CbmDigiEvent.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/global/CbmDigiTimeslice.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/bmon/CbmBmonDigiData.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/sts/CbmStsDigiData.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/much/CbmMuchDigiData.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/rich/CbmRichDigiData.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/trd/CbmTrdDigiData.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/tof/CbmTofDigiData.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/psd/CbmPsdDigiData.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/CbmAddress.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/CbmTimeSlice.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/CbmEvent.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/CbmEventStore.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/CbmHit.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/CbmPixelHit.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/CbmStripHit.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/CbmTrack.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/CbmTrackMatch.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/CbmCluster.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/CbmModuleList.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/CbmErrorMessage.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/CbmMCTrack.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/CbmMCEventInfo.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/CbmMCEventList.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/CbmLink.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/CbmMatch.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/CbmTrackMatchNew.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/CbmTsEventHeader.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/base/CbmDigiBranchBase.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/base/CbmDigiContainer.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/mvd/CbmMvdCluster.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/mvd/CbmMvdDetectorId.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/mvd/CbmMvdDigi.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/mvd/CbmMvdHit.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/mvd/CbmMvdPoint.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/mvd/CbmMvdHitMatch.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/bmon/CbmBmonDigi.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/sts/CbmStsAddress.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/sts/CbmStsCluster.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/sts/CbmStsDigi.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/sts/CbmStsHit.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/sts/CbmStsPoint.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/sts/CbmStsTrack.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/rich/CbmRichRing.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/rich/CbmRichHit.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/rich/CbmRichPoint.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/rich/CbmRichTrbDigi.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/rich/CbmRichDigi.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/much/CbmMuchPixelHit.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/much/CbmMuchPoint.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/much/CbmMuchCluster.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/much/CbmMuchDigi.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/much/CbmMuchBeamTimeDigi.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/much/CbmMuchDigiMatch.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/much/CbmMuchTrack.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/much/CbmMuchAddress.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/trd/CbmTrdHit.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/trd/CbmTrdTrack.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/trd/CbmTrdPoint.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/trd/CbmTrdDigi.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/trd/CbmTrdAddress.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/trd/CbmTrdCluster.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/trd/CbmTrdRawMessageSpadic.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/tof/CbmTofHit.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/tof/CbmTofPoint.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/tof/CbmTofDetectorId.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/tof/CbmTofDetectorId_v07a.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/tof/CbmTofDetectorId_v12b.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/tof/CbmTofDetectorId_v14a.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/tof/CbmTofDetectorId_v21a.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/tof/CbmTofAddress.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/tof/CbmTofDigi.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/tof/CbmTofTrack.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/tof/CbmTofTracklet.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/tof/CbmTofTrackletParam.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/psd/CbmPsdDigi.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/psd/CbmPsdDsp.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/psd/CbmPsdHit.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/psd/CbmPsdPoint.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/psd/CbmPsdAddress.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/psd/CbmPsdMCbmHit.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/global/CbmGlobalTrack.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/global/CbmVertex.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/global/CbmTrackParam.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/raw/AccDataSis18.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/raw/StsXyterMessage.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/raw/gDpbMessv100.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/raw/CriGet4Mess001.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/raw/TimesliceMetaData.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/raw/PsdGbtReader-v0.00.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/raw/PsdGbtReader-v1.00.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmData.rootmap"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmData_rdict.pcm"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/core/data/CbmDefs.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/base/CbmDigiData.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/base/CbmDigiVector.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/bmon/CbmBmonDigiData.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/much/CbmMuchDigiData.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/psd/CbmPsdDigiData.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/rich/CbmRichDigiData.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/rich/CbmRichRingLight.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/sts/CbmStsDigiData.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/trd/CbmTrdDigiData.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/tof/CbmTofDigiData.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/global/CbmDigiEvent.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/global/CbmDigiTimeslice.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/raw/bitmask_operators.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/raw/StsXyterFinalHit.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/raw/PsdGbtDataFormat-v0.00.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/data/raw/PsdGbtDataFormat-v1.00.h"
    )
endif()

