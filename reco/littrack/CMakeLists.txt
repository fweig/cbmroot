# Create a library called "libLittrack".

add_subdirectory(clustering)
add_subdirectory(parallel)

INCLUDE(std/CMakeLists.txt)

set(INCLUDE_DIRECTORIES
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/std
${CMAKE_CURRENT_SOURCE_DIR}/cbm
${CMAKE_CURRENT_SOURCE_DIR}/cbm/elid
${CMAKE_CURRENT_SOURCE_DIR}/cbm/reco
${CMAKE_CURRENT_SOURCE_DIR}/cbm/generators
${CMAKE_CURRENT_SOURCE_DIR}/cbm/qa
${CMAKE_CURRENT_SOURCE_DIR}/cbm/qa/material
${CMAKE_CURRENT_SOURCE_DIR}/cbm/qa/tracking
${CMAKE_CURRENT_SOURCE_DIR}/cbm/qa/clustering
${CMAKE_CURRENT_SOURCE_DIR}/cbm/qa/fit
${CMAKE_CURRENT_SOURCE_DIR}/cbm/qa/field
${CMAKE_CURRENT_SOURCE_DIR}/cbm/qa/fieldapr
${CMAKE_CURRENT_SOURCE_DIR}/cbm/qa/radlength
${CMAKE_CURRENT_SOURCE_DIR}/cbm/qa/tof

${CBMROOT_SOURCE_DIR}/reco/base
${CBMROOT_SOURCE_DIR}/reco/detectors/rich
${CBMROOT_SOURCE_DIR}/reco/detectors/rich/fitter
${CBMROOT_SOURCE_DIR}/reco/detectors/rich/utils

${CBMBASE_DIR}
${CBMBASE_DIR}/report
${CBMBASE_DIR}/draw
${CBMBASE_DIR}/utils

${CBMDATA_DIR}
${CBMDATA_DIR}/base
${CBMDATA_DIR}/much
${CBMDATA_DIR}/trd
${CBMDATA_DIR}/sts
${CBMDATA_DIR}/tof
${CBMDATA_DIR}/global
${CBMDATA_DIR}/mvd
${CBMDATA_DIR}/rich

${CBMDETECTORBASE_DIR}/much
${CBMDETECTORBASE_DIR}/sts


${CBMDETECTORBASE_DIR}/rich

${CBMROOT_SOURCE_DIR}/reco/KF
${CBMROOT_SOURCE_DIR}/reco/KF/Interface

${CBMROOT_SOURCE_DIR}/run
)

Include_Directories( ${INCLUDE_DIRECTORIES})

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES}
  ${Boost_INCLUDE_DIR}
)

Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
${Boost_LIBRARY_DIRS}
${Vc_LIB_DIR}
${KFParticle_LIB_DIR}
)

link_directories( ${LINK_DIRECTORIES})

set(SRCS

   cbm/base/CbmLitDetectorSetup.cxx
   
   cbm/elid/CbmLitGlobalElectronId.cxx
   
   cbm/reco/CbmLitFindMvdTracks.cxx
   cbm/reco/CbmLitFitTracksParallel.cxx
   cbm/reco/CbmLitFitTracks.cxx
   
   cbm/generators/CbmLitPolarizedGenerator.cxx
   
   cbm/qa/base/CbmLitResultChecker.cxx  
   cbm/qa/material/CbmLitCheckEnergyLossMuons.cxx
   cbm/qa/material/CbmLitCheckBrem.cxx 
   cbm/qa/mc/CbmLitMCTrack.cxx
   cbm/qa/mc/CbmLitMCTrackCreator.cxx
   cbm/qa/CbmLitCreateStandaloneData.cxx
   cbm/qa/CbmLitTestMatrixMath.cxx
   ${LITTRACKSTD_SRCS}
   cbm/base/CbmLitFieldGridCreator.cxx
   cbm/base/CbmLitFieldFitter.cxx
   cbm/base/CbmLitToolFactory.cxx
   cbm/base/CbmLitTrackingGeometryConstructor.cxx
   cbm/reco/CbmLitFindGlobalTracks.cxx 
   cbm/reco/CbmLitFindGlobalTracksIdeal.cxx 
   cbm/reco/CbmLitFindGlobalTracksParallel.cxx
   cbm/utils/CbmLitConverterParallel.cxx
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

set(HEADERS
   cbm/elid/CbmLitGlobalElectronId.h
   cbm/reco/CbmLitFindGlobalTracks.h
   cbm/reco/CbmLitFindMvdTracks.h
   cbm/reco/CbmLitFindGlobalTracksIdeal.h
   cbm/reco/CbmLitFitTracksParallel.h
   cbm/reco/CbmLitFitTracks.h
   cbm/reco/CbmLitFindGlobalTracksParallel.h
   cbm/generators/CbmLitPolarizedGenerator.h
   cbm/qa/material/CbmLitCheckEnergyLossMuons.h 
   cbm/qa/material/CbmLitCheckBrem.h 
   cbm/qa/CbmLitCreateStandaloneData.h
   cbm/qa/CbmLitTestMatrixMath.h
   cbm/qa/tracking/CbmLitTrackingQa.h
   cbm/qa/tracking/CbmLitTrackingQaStudyReport.h
   cbm/qa/tracking/CbmLitTrackingQaReport.h
   cbm/qa/clustering/CbmLitClusteringQa.h
   cbm/qa/clustering/CbmLitClusteringQaReport.h
   cbm/qa/clustering/CbmLitClusteringQaStudyReport.h
   cbm/qa/fit/CbmLitFitQa.h 
   cbm/qa/fit/CbmLitFitQaReport.h
   cbm/qa/fit/CbmLitFitQaStudyReport.h
   cbm/qa/field/CbmLitFieldQa.h
   cbm/qa/field/CbmLitFieldQaReport.h
   cbm/qa/fieldapr/CbmLitFieldApproximationQa.h
   cbm/qa/fieldapr/CbmLitFieldApproximationQaReport.h
   cbm/qa/radlength/CbmLitRadLengthQa.h
   cbm/qa/radlength/CbmLitRadLengthQaReport.h
   cbm/qa/radlength/CbmLitRadLengthGenerator.h
   cbm/qa/tof/CbmLitTofQa.h
   cbm/qa/tof/CbmLitTofQaReport.h
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

set(LINKDEF  LittrackLinkDef.h)
Set(LIBRARY_NAME Littrack)
Set(DEPENDENCIES
    Littrackparallel KF CbmMuchBase CbmStsBase CbmRichReco CbmRichBase CbmRecoBase CbmBase CbmData Base 
)

GENERATE_LIBRARY()

Install(FILES std/base/CbmLitTypes.h
              std/base/CbmLitFloat.h
              std/base/CbmLitPtrTypes.h
              std/base/CbmLitEnums.h
        DESTINATION include/littrack/base
       )
Install(FILES cbm/base/CbmLitDetectorSetup.h
        DESTINATION include/littrack/base
       )
Install(FILES cbm/base/CbmLitDetectorSetup.h
        DESTINATION include/littrack/cbm/base
       )
Install(FILES std/data/CbmLitFitNode.h
              std/data/CbmLitTrackParam.h
              std/data/CbmLitHit.h
        DESTINATION include/littrack/data
       )
Install(FILES std/interface/CbmLitTrackExtrapolator.h
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
Install(FILES std/propagation/CbmLitMaterialInfo.h
        DESTINATION include/littrack/propagation
       )
