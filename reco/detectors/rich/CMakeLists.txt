#add_subdirectory(qa/mirrors)

Set(INCLUDE_DIRECTORIES
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/finder
${CMAKE_CURRENT_SOURCE_DIR}/fitter
${CMAKE_CURRENT_SOURCE_DIR}/tracks
${CMAKE_CURRENT_SOURCE_DIR}/selection
${CMAKE_CURRENT_SOURCE_DIR}/qa
${CMAKE_CURRENT_SOURCE_DIR}/alignment
${CMAKE_CURRENT_SOURCE_DIR}/mcbm
${CMAKE_CURRENT_SOURCE_DIR}/unpack

${CBMBASE_DIR}
${CBMBASE_DIR}/report
${CBMBASE_DIR}/draw
${CBMBASE_DIR}/utils

${CBMDATA_DIR}
${CBMDATA_DIR}/base
${CBMDATA_DIR}/rich
${CBMDATA_DIR}/trd
${CBMDATA_DIR}/tof
${CBMDATA_DIR}/sts
${CBMDATA_DIR}/global

${CBMROOT_SOURCE_DIR}/core/detectors/rich
${CBMROOT_SOURCE_DIR}/core/detectors/rich/utils

${CBMROOT_SOURCE_DIR}/reco/base

${CBMROOT_SOURCE_DIR}/reco/littrack
${CBMROOT_SOURCE_DIR}/reco/littrack/std
${CBMROOT_SOURCE_DIR}/reco/littrack/std/base
${CBMROOT_SOURCE_DIR}/reco/littrack/cbm
${CBMROOT_SOURCE_DIR}/reco/littrack/cbm/utils
${CBMROOT_SOURCE_DIR}/reco/littrack/cbm/base

${CBMROOT_SOURCE_DIR}/reco/KF
${CBMROOT_SOURCE_DIR}/reco/KF/Interface

${CBMROOT_SOURCE_DIR}/reco/L1/OffLineInterface
)

Include_Directories( ${INCLUDE_DIRECTORIES})

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES}
  ${Boost_INCLUDE_DIR}
  ${IPC_INCLUDE_DIRECTORY} # for fles for unpacker
)

Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${KFParticle_LIB_DIR}
${FAIRROOT_LIBRARY_DIR}
${ROOT_LIBRARY_DIR}
${Boost_LIBRARY_DIRS}
)

link_directories( ${LINK_DIRECTORIES})

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

set(LINKDEF  CbmRichRecoLinkDef.h)

Set(LIBRARY_NAME CbmRichReco)
Set(DEPENDENCIES
    KF KFParticleInterface L1 Littrack CbmRichBase CbmRecoBase CbmBase CbmData Base MLP boost_regex Gdml 
    )

GENERATE_LIBRARY()

Install(FILES
        fitter/CbmRichRingFitterCOP.h fitter/CbmRichRingFitterBase.h
        fitter/CbmRichRingFitterEllipseBase.h selection/CbmRichRingSelectImpl.h
        finder/CbmRichRingFinderHoughImpl.h finder/CbmRichRingFinderData.h
        DESTINATION include
       )

