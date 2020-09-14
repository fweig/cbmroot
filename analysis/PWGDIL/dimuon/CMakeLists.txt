Set(INCLUDE_DIRECTORIES
${CMAKE_CURRENT_SOURCE_DIR}

${CBMROOT_SOURCE_DIR}/reco/base

${CBMDATA_DIR}
${CBMDATA_DIR}/sts
${CBMDATA_DIR}/much
${CBMDATA_DIR}/trd
${CBMDATA_DIR}/tof
${CBMDATA_DIR}/global

${CBMROOT_SOURCE_DIR}/reco/KF
${CBMROOT_SOURCE_DIR}/reco/KF/Interface 

${CBMROOT_SOURCE_DIR}/sim/transport/generators/pluto 

  ${CBMDETECTORBASE_DIR}/much

)

Include_Directories( ${INCLUDE_DIRECTORIES})

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES} 
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
CbmAnaMuonCandidate.cxx 
CbmAnaDimuonAnalysis.cxx
)

Set(LINKDEF CbmMuchAnalysisLinkDef.h)
Set(LIBRARY_NAME CbmMuchAnalysis)
Set(DEPENDENCIES CbmRecoBase CbmData CbmMuchBase KF MLP)

GENERATE_LIBRARY()




