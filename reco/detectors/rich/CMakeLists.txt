#add_subdirectory(qa/mirrors)

set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/finder
  ${CMAKE_CURRENT_SOURCE_DIR}/fitter
  ${CMAKE_CURRENT_SOURCE_DIR}/tracks
  ${CMAKE_CURRENT_SOURCE_DIR}/selection
  ${CMAKE_CURRENT_SOURCE_DIR}/qa
  ${CMAKE_CURRENT_SOURCE_DIR}/alignment
  ${CMAKE_CURRENT_SOURCE_DIR}/mcbm
  ${CMAKE_CURRENT_SOURCE_DIR}/unpack
  )

set(SRCS
  CbmRichTrainAnnSelect.cxx
  CbmRichTrainAnnElectrons.cxx
  CbmRichReconstruction.cxx
  CbmRichHitProducer.cxx

  mcbm/CbmRichMCbmQa.cxx
  mcbm/CbmRichMCbmQaReal.cxx
  mcbm/CbmRichMCbmQaRichOnly.cxx
  mcbm/CbmRichMCbmHitProducer.cxx
  mcbm/CbmRichMCbmAerogelAna.cxx
  mcbm/CbmRichMCbmToTShifter.cxx
  mcbm/CbmRichMCbmSEDisplay.cxx

  qa/CbmRichRingFitterQa.cxx
  qa/CbmRichGeoOpt.cxx
  qa/CbmRichRecoTbQa.cxx
  qa/CbmRichGeoTest.cxx
  qa/CbmRichUrqmdTest.cxx
  qa/CbmRichGeoTestOpt.cxx
  qa/CbmRichRecoQa.cxx

  unpack/CbmRichUnpackAlgo.cxx
  unpack/CbmRichUnpackAlgo2022.cxx
  unpack/CbmRichUnpackAlgoBase.cxx
  unpack/CbmRichUnpackMonitor.cxx
  unpack/CbmRichUnpackConfig.cxx
  )

set(NO_DICT_SRCS
  CbmRichConverter.cxx
  finder/CbmRichRingFinderIdeal.cxx
  finder/CbmRichRingFinderHoughImpl.cxx
  finder/CbmRichRingFinderHough.cxx

  fitter/CbmRichRingFitterCircle.cxx
  fitter/CbmRichRingFitterCOP.cxx
  fitter/CbmRichRingFitterRobustCOP.cxx
  fitter/CbmRichRingFitterTAU.cxx
  fitter/CbmRichRingFitterEllipseMinuit.cxx
  fitter/CbmRichRingFitterEllipseTau.cxx

  tracks/CbmRichProjectionProducerAnalytical.cxx
  tracks/CbmRichProjectionProducerTGeo.cxx
  tracks/CbmRichRingTrackAssignClosestD.cxx
  tracks/CbmRichRingTrackAssignIdeal.cxx
  tracks/CbmRichTrackExtrapolationIdeal.cxx
  tracks/CbmRichTrackExtrapolationKF.cxx
  tracks/CbmRichTrackExtrapolationLittrack.cxx
  tracks/CbmRichTrackExtrapolationMirrorIdeal.cxx

  selection/CbmRichRingSelectAnn.cxx
  )


IF (SSE_FOUND)
  ADD_DEFINITIONS(-DHAVE_SSE)
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS "-msse -O3")
  Message(STATUS "Rich will be compiled with SSE support")
ELSE (SSE_FOUND)
  Message(STATUS "Rich will be compiled without SSE support")
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS "-O3")
ENDIF (SSE_FOUND)

# END SSE



set(LIBRARY_NAME CbmRichReco)
set(LINKDEF  ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmBase
  CbmData
  CbmRichBase
  L1
  Littrack
  FairRoot::Base
  ROOT::Core
  ROOT::Geom
  ROOT::Gpad
  ROOT::Hist
  ROOT::MathCore
  ROOT::Matrix
  ROOT::Physics
  ROOT::RIO
  )

set(PRIVATE_DEPENDENCIES
  CbmSimSteer
  KF
  CbmRecoBase
  FairRoot::GeoBase
  FairRoot::ParBase
  ROOT::Graf
  ROOT::MLP
  ROOT::RHTTP
  ROOT::Tree
  ROOT::Minuit2
  )

set(INTERFACE_DEPENDENCIES
  FairLogger::FairLogger
  external::fles_ipc
  )


generate_cbm_library()



install(FILES
        fitter/CbmRichRingFitterCOP.h fitter/CbmRichRingFitterBase.h
        fitter/CbmRichRingFitterEllipseBase.h selection/CbmRichRingSelectImpl.h
        finder/CbmRichRingFinderHoughImpl.h finder/CbmRichRingFinderData.h
        DESTINATION include
       )
