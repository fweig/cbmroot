Set(INCLUDE_DIRECTORIES
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/qa
${CMAKE_CURRENT_SOURCE_DIR}/unpack

${CBMDETECTORBASE_DIR}/much

${CBMROOT_SOURCE_DIR}/reco/base
${CBMROOT_SOURCE_DIR}/core/qa

${CBMBASE_DIR}

${CBMDATA_DIR}
${CBMDATA_DIR}/raw
${CBMDATA_DIR}/much
${CBMDATA_DIR}/global
${CBMDATA_DIR}/base
)

Include_Directories( ${INCLUDE_DIRECTORIES})

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES}
  ${FLES_IPC_INCLUDE_DIRECTORY}
)

Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
${Boost_LIBRARY_DIRS}
)

link_directories( ${LINK_DIRECTORIES})

set(SRCS
CbmMuchFindHitsGem.cxx
CbmMuchHitProducerIdeal.cxx

CbmMuchFindTracks.cxx
CbmMuchMatchTracks.cxx
CbmMuchTrackFinderIdeal.cxx

qa/CbmMuchHitFinderQa.cxx

unpack/CbmMuchUnpackAlgo.cxx
unpack/CbmMuchUnpackMonitor.cxx
unpack/CbmMuchUnpackConfig.cxx
)

set(LINKDEF CbmMuchRecoLinkDef.h)
Set(LIBRARY_NAME CbmMuchReco)
Set(DEPENDENCIES
    CbmMuchBase CbmRecoBase CbmBase CbmQaBase CbmData Base
)

GENERATE_LIBRARY()
