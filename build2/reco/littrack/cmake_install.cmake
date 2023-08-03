# Install script for directory: /home/heinemann/gpuunpacker/cbmroot/reco/littrack

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
  include("/home/heinemann/gpuunpacker/cbmroot/build2/reco/littrack/clustering/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/heinemann/gpuunpacker/cbmroot/build2/reco/littrack/parallel/cmake_install.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibrariesx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libLittrack_rdict.pcm"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libLittrack.rootmap"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libLittrack.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libLittrack.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libLittrack.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libLittrack.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libLittrack.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libLittrack.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libLittrack.so"
         OLD_RPATH "/home/heinemann/gpuunpacker/cbmroot/build2/lib:/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libLittrack.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/base/CbmLitDetectorSetup.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/elid/CbmLitGlobalElectronId.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/reco/CbmLitFindMvdTracks.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/reco/CbmLitFitTracksParallel.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/reco/CbmLitFitTracks.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/generators/CbmLitPolarizedGenerator.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/base/CbmLitHitData.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/finder/CbmLitTrackFinderBranch.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/finder/CbmLitTrackFinderNN.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/finder/CbmLitNearestHitTofMerger.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/finder/CbmLitAllHitsTofMerger.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/fitter/CbmLitTrackFitterImp.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/fitter/CbmLitKalmanFilter.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/fitter/CbmLitKalmanSmoother.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/fitter/CbmLitTrackFitterIter.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/propagation/CbmLitTGeoTrackPropagator.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/propagation/CbmLitRK4TrackExtrapolator.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/propagation/CbmLitLineTrackExtrapolator.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/propagation/CbmLitTGeoNavigator.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/propagation/CbmLitCleverTrackExtrapolator.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/selection/CbmLitQualitySort.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/selection/CbmLitTrackSelectionCuts.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/selection/CbmLitTrackSelectionEmpty.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/selection/CbmLitTrackSelectionMuch.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/selection/CbmLitTrackSelectionSharedHits.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/selection/CbmLitTrackSelectionTrd.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/utils/CbmLitMath.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/utils/CbmLitMatrixMath.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/propagation/CbmLitMaterialEffectsImp.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/base/CbmLitFieldGridCreator.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/base/CbmLitFieldFitter.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/base/CbmLitToolFactory.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/base/CbmLitTrackingGeometryConstructor.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/reco/CbmLitFindGlobalTracks.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/reco/CbmLitFindGlobalTracksIdeal.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/reco/CbmLitFindGlobalTracksParallel.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/utils/CbmLitConverterParallel.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libLittrack.rootmap"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libLittrack_rdict.pcm"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibrariesx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libLittrackQA_rdict.pcm"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libLittrackQA.rootmap"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libLittrackQA.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libLittrackQA.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libLittrackQA.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libLittrackQA.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libLittrackQA.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libLittrackQA.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libLittrackQA.so"
         OLD_RPATH "/home/heinemann/gpuunpacker/cbmroot/build2/lib:/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libLittrackQA.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/base/CbmLitResultChecker.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/material/CbmLitCheckEnergyLossMuons.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/material/CbmLitCheckBrem.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/mc/CbmLitMCTrack.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/mc/CbmLitMCTrackCreator.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/CbmLitCreateStandaloneData.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/CbmLitTestMatrixMath.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/tracking/CbmLitTrackingQa.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/tracking/CbmLitTrackingQaStudyReport.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/tracking/CbmLitTrackingQaReport.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/clustering/CbmLitClusteringQa.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/clustering/CbmLitClusteringQaReport.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/clustering/CbmLitClusteringQaStudyReport.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/fit/CbmLitFitQa.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/fit/CbmLitFitQaReport.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/fit/CbmLitFitQaStudyReport.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/field/CbmLitFieldQa.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/field/CbmLitFieldQaReport.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/fieldapr/CbmLitFieldApproximationQa.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/fieldapr/CbmLitFieldApproximationQaReport.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/radlength/CbmLitRadLengthQa.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/radlength/CbmLitRadLengthQaReport.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/radlength/CbmLitRadLengthGenerator.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/tof/CbmLitTofQa.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/qa/tof/CbmLitTofQaReport.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libLittrackQA.rootmap"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libLittrackQA_rdict.pcm"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/littrack/base" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/base/CbmLitTypes.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/base/CbmLitFloat.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/base/CbmLitPtrTypes.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/base/CbmLitEnums.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/base/CbmLitHitData.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/littrack/base" TYPE FILE FILES "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/base/CbmLitDetectorSetup.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/littrack/cbm/base" TYPE FILE FILES "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/cbm/base/CbmLitDetectorSetup.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/littrack/data" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/data/CbmLitFitNode.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/data/CbmLitTrackParam.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/data/CbmLitHit.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/littrack/interface" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/interface/CbmLitTrackExtrapolator.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/interface/CbmLitTrackPropagator.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/interface/CbmLitTrackFitter.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/interface/CbmLitTrackSelection.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/interface/CbmLitTrackUpdate.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/interface/CbmLitGeoNavigator.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/interface/CbmLitMaterialEffects.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/interface/CbmLitTrackFinder.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/interface/CbmLitHitToTrackMerger.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/littrack/propagation" TYPE FILE FILES "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/propagation/CbmLitMaterialInfo.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/littrack/selection" TYPE FILE FILES "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/std/selection/CbmLitQualitySort.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/parallel/LitFieldGrid.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/parallel/LitFieldValue.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/parallel/LitTypes.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/parallel/LitUtils.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/parallel/LitFieldSlice.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/parallel/LitStation.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/parallel/LitVirtualStation.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/parallel/LitMaterialGrid.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/parallel/LitScalStripHit.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/parallel/LitTrackParam.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/parallel" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/parallel/LitDetectorLayout.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/parallel/LitMaterialGrid.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/parallel/LitScalPixelHit.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/parallel/LitScalStripHit.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/parallel/LitScalTrack.h"
    "/home/heinemann/gpuunpacker/cbmroot/reco/littrack/parallel/LitTrackParam.h"
    )
endif()

