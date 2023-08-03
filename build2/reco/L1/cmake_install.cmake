# Install script for directory: /home/heinemann/gpuunpacker/cbmroot/reco/L1

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
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libL1_rdict.pcm"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libL1.rootmap"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libL1.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libL1.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libL1.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libL1.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libL1.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libL1.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libL1.so"
         OLD_RPATH "/home/heinemann/gpuunpacker/cbmroot/build2/lib:/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libL1.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/CbmL1Constants.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/CbmL1CATrdTrackFinderSA.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/CbmL1DetectorID.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/CbmL1MCPoint.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/CbmL1Hit.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/CbmL1Track.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/CbmL1TrackPar.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/CbmL1TrdHit.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/CbmL1Vtx.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1Def.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1Vector.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1EArray.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1Undef.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/catools/CaToolsWindowFinder.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/catools/CaToolsLinkKey.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/catools/CaToolsHitRecord.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/qa/CbmCaInputQaBase.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/CbmL1TrdTracklet.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/CbmL1TrdTracklet4.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/CbmL1.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/CbmL1TrackMerger.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/CbmL1TofMerger.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/OffLineInterface/CbmL1StsTrackFinder.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/OffLineInterface/CbmL1GlobalTrackFinder.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/OffLineInterface/CbmL1GlobalFindTracksEvents.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/utils/CaAlgoRandom.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1Algo.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1TrackPar.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1Grid.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/CbmCaMCModule.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/CbmCaTimeSliceReader.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1Station.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1Fit.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1MCEvent.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/CbmL1MCTrack.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/CbmL1Track.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1Material.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1UMeasurementInfo.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1XYMeasurementInfo.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1Field.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1CAIteration.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1BaseStationInfo.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1InitManager.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1Parameters.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1InputData.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1IODataManager.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1CloneMerger.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1ConfigRW.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1SearchWindow.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/utils/L1AlgoDraw.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/utils/L1AlgoEfficiencyPerformance.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/utils/L1AlgoPulls.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/catools/CaToolsMCData.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/catools/CaToolsMCPoint.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/catools/CaToolsMCTrack.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/catools/CaToolsQa.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/catools/CaToolsDebugger.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/catools/CaToolsWindowFinder.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/catools/CaToolsWFExpression.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/ParticleFinder/CbmL1PFFitter.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/ParticleFinder/CbmL1PFMCParticle.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/qa/CbmTrackerInputQaTrd.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/qa/CbmTrackerInputQaTof.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/qa/CbmTrackingInputQaSts.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/qa/CbmCaInputQaSts.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/qa/CbmCaInputQaMuch.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/qa/CbmCaInputQaTrd.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/qa/CbmCaInputQaTof.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/qa/CbmCaOutputQa.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/qa/CbmCaTrackTypeQa.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/qa/CbmCaTrackFitQa.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/qa/CbmTofInteraction.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libL1.rootmap"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libL1_rdict.pcm"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/CbmL1Counters.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1Assert.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1EventEfficiencies.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1Branch.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1HitPoint.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1Hit.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1Portion.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1Triplet.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1Event.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1EventMatch.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1ObjectInitController.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1Constants.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1Utils.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1SimdSerializer.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1TrackPar.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1Track.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/qa/CbmCaInputQaBase.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/L1Algo" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1Algo.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1Branch.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1Field.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1Hit.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1Vector.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/L1/L1Algo/L1EArray.h"
    )
endif()

