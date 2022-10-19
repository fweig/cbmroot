# Create a library called "libLittrack".

add_subdirectory(clustering)
add_subdirectory(parallel)

include(std/CMakeLists.txt)

set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/std
  ${CMAKE_CURRENT_SOURCE_DIR}/std/finder
  ${CMAKE_CURRENT_SOURCE_DIR}/std/fitter
  ${CMAKE_CURRENT_SOURCE_DIR}/std/propagation
  ${CMAKE_CURRENT_SOURCE_DIR}/std/selection
  ${CMAKE_CURRENT_SOURCE_DIR}/std/data
  ${CMAKE_CURRENT_SOURCE_DIR}/std/base
  ${CMAKE_CURRENT_SOURCE_DIR}/std/utils
  ${CMAKE_CURRENT_SOURCE_DIR}/cbm
  ${CMAKE_CURRENT_SOURCE_DIR}/cbm/base
  ${CMAKE_CURRENT_SOURCE_DIR}/cbm/utils
  ${CMAKE_CURRENT_SOURCE_DIR}/cbm/elid
  ${CMAKE_CURRENT_SOURCE_DIR}/cbm/reco
  ${CMAKE_CURRENT_SOURCE_DIR}/cbm/generators
  ${CMAKE_CURRENT_SOURCE_DIR}/cbm/qa
  ${CMAKE_CURRENT_SOURCE_DIR}/cbm/qa/mc
  ${CMAKE_CURRENT_SOURCE_DIR}/cbm/qa/base
  ${CMAKE_CURRENT_SOURCE_DIR}/cbm/qa/material
  ${CMAKE_CURRENT_SOURCE_DIR}/cbm/qa/tracking
  ${CMAKE_CURRENT_SOURCE_DIR}/cbm/qa/clustering
  ${CMAKE_CURRENT_SOURCE_DIR}/cbm/qa/fit
  ${CMAKE_CURRENT_SOURCE_DIR}/cbm/qa/field
  ${CMAKE_CURRENT_SOURCE_DIR}/cbm/qa/fieldapr
  ${CMAKE_CURRENT_SOURCE_DIR}/cbm/qa/radlength
  ${CMAKE_CURRENT_SOURCE_DIR}/cbm/qa/tof
  )

set(SRCS

   cbm/base/CbmLitDetectorSetup.cxx

   cbm/elid/CbmLitGlobalElectronId.cxx

   cbm/reco/CbmLitFindMvdTracks.cxx
   cbm/reco/CbmLitFitTracksParallel.cxx
   cbm/reco/CbmLitFitTracks.cxx

   cbm/generators/CbmLitPolarizedGenerator.cxx

   ${LITTRACKSTD_SRCS}

   cbm/base/CbmLitFieldGridCreator.cxx
   cbm/base/CbmLitFieldFitter.cxx
   cbm/base/CbmLitToolFactory.cxx
   cbm/base/CbmLitTrackingGeometryConstructor.cxx
   cbm/reco/CbmLitFindGlobalTracks.cxx 
   cbm/reco/CbmLitFindGlobalTracksIdeal.cxx 
   cbm/reco/CbmLitFindGlobalTracksParallel.cxx
   cbm/utils/CbmLitConverterParallel.cxx
)

set(QA_SRCS
   cbm/qa/base/CbmLitResultChecker.cxx
   cbm/qa/material/CbmLitCheckEnergyLossMuons.cxx
   cbm/qa/material/CbmLitCheckBrem.cxx
   cbm/qa/mc/CbmLitMCTrack.cxx
   cbm/qa/mc/CbmLitMCTrackCreator.cxx
   cbm/qa/CbmLitCreateStandaloneData.cxx
   cbm/qa/CbmLitTestMatrixMath.cxx
   cbm/qa/tracking/CbmLitTrackingQa.cxx
   cbm/qa/tracking/CbmLitTrackingQaStudyReport.cxx
   cbm/qa/tracking/CbmLitTrackingQaReport.cxx
   cbm/qa/clustering/CbmLitClusteringQa.cxx
   cbm/qa/clustering/CbmLitClusteringQaReport.cxx
   cbm/qa/clustering/CbmLitClusteringQaStudyReport.cxx
   cbm/qa/fit/CbmLitFitQa.cxx
   cbm/qa/fit/CbmLitFitQaReport.cxx
   cbm/qa/fit/CbmLitFitQaStudyReport.cxx
   cbm/qa/field/CbmLitFieldQa.cxx
   cbm/qa/field/CbmLitFieldQaReport.cxx
   cbm/qa/fieldapr/CbmLitFieldApproximationQa.cxx
   cbm/qa/fieldapr/CbmLitFieldApproximationQaReport.cxx
   cbm/qa/radlength/CbmLitRadLengthQa.cxx
   cbm/qa/radlength/CbmLitRadLengthQaReport.cxx
   cbm/qa/radlength/CbmLitRadLengthGenerator.cxx
   cbm/qa/tof/CbmLitTofQa.cxx
   cbm/qa/tof/CbmLitTofQaReport.cxx
)


IF (CMAKE_BUILD_TYPE MATCHES Debug)
ELSE (CMAKE_BUILD_TYPE MATCHES Debug)
IF (SSE_FOUND)
  ADD_DEFINITIONS(-DHAVE_SSE)
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS "-msse -O3 -w")
  Message(STATUS "LitTrack will be compiled with SSE support")
ELSE (SSE_FOUND)
  Message(STATUS "LitTrack will be compiled without SSE support")
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS "-O3 -w")
ENDIF (SSE_FOUND)
ENDIF (CMAKE_BUILD_TYPE MATCHES Debug)

If (Boost_FOUND)
  set(DEFINITIONS BOOST_FOUND)
EndIf (Boost_FOUND)


set(LIBRARY_NAME Littrack)
set(LINKDEF  ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmData
  Littrackparallel
  FairRoot::Base
  ROOT::Hist
  ROOT::Physics
  ROOT::Core
  )

set(PRIVATE_DEPENDENCIES
  CbmBase
  CbmMuchBase
  CbmSimSteer
  KFParticleInterface
  CbmRichBase
  FairRoot::ParBase
  ROOT::EG
  ROOT::MathCore
  ROOT::Geom
  ROOT::Matrix
  ROOT::Minuit2
  ROOT::RIO
  )


generate_cbm_library()


set(SRCS ${QA_SRCS})


set(LIBRARY_NAME LittrackQA)
set(LINKDEF  LittrackQALinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmBase
  CbmData
  CbmMuchBase
  KF
  FairRoot::Base
  ROOT::Core
  ROOT::EG
  ROOT::MathCore
  )

set(PRIVATE_DEPENDENCIES
  CbmRichBase
  CbmRichReco
  CbmStsBase
  KFParticleInterface
  FairRoot::GeoBase
  FairRoot::ParBase
  ROOT::Geom
  ROOT::Gpad
  ROOT::Graf
  ROOT::Hist
  ROOT::Matrix
  ROOT::Physics
  ROOT::RIO
  )


generate_cbm_library()

install(FILES std/base/CbmLitTypes.h
              std/base/CbmLitFloat.h
              std/base/CbmLitPtrTypes.h
              std/base/CbmLitEnums.h
              std/base/CbmLitHitData.h
        DESTINATION include/littrack/base
       )

install(FILES cbm/base/CbmLitDetectorSetup.h
        DESTINATION include/littrack/base
       )

install(FILES cbm/base/CbmLitDetectorSetup.h
        DESTINATION include/littrack/cbm/base
       )

install(FILES std/data/CbmLitFitNode.h
              std/data/CbmLitTrackParam.h
              std/data/CbmLitHit.h
        DESTINATION include/littrack/data
       )

install(FILES std/interface/CbmLitTrackExtrapolator.h
              std/interface/CbmLitTrackPropagator.h
              std/interface/CbmLitTrackFitter.h
              std/interface/CbmLitTrackSelection.h
              std/interface/CbmLitTrackUpdate.h
              std/interface/CbmLitGeoNavigator.h
              std/interface/CbmLitMaterialEffects.h
              std/interface/CbmLitTrackFinder.h
              std/interface/CbmLitHitToTrackMerger.h
        DESTINATION include/littrack/interface
       )

install(FILES std/propagation/CbmLitMaterialInfo.h
        DESTINATION include/littrack/propagation
       )

install(FILES std/selection/CbmLitQualitySort.h
        DESTINATION include/littrack/selection
       )

install(FILES parallel/LitFieldGrid.h
              parallel/LitFieldValue.h
              parallel/LitTypes.h
              parallel/LitUtils.h
              parallel/LitFieldSlice.h
              parallel/LitStation.h
              parallel/LitVirtualStation.h
              parallel/LitMaterialGrid.h
              parallel/LitScalStripHit.h
              parallel/LitTrackParam.h
        DESTINATION include
       )

install(FILES
          parallel/LitDetectorLayout.h
          parallel/LitMaterialGrid.h
          parallel/LitScalPixelHit.h
          parallel/LitScalStripHit.h
          parallel/LitScalTrack.h
          parallel/LitTrackParam.h
        DESTINATION include/parallel
       )
