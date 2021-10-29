Set(INCLUDE_DIRECTORIES
${CMAKE_CURRENT_SOURCE_DIR}

${CBMBASE_DIR}/draw
${CBMBASE_DIR}/utils

${CBMDATA_DIR}
${CBMDATA_DIR}/sts
${CBMDATA_DIR}/global
${CBMDATA_DIR}/rich

${CBMROOT_SOURCE_DIR}/core/detectors/rich/utils
${CBMROOT_SOURCE_DIR}/core/detectors/rich

${CBMROOT_SOURCE_DIR}/reco/base

${CBMROOT_SOURCE_DIR}/reco/KF
${CBMROOT_SOURCE_DIR}/reco/KF/Interface 

${CBMROOT_SOURCE_DIR}/reco/L1
${CBMROOT_SOURCE_DIR}/reco/L1/L1Algo
${CBMROOT_SOURCE_DIR}/reco/L1/ParticleFinder 

${CBMROOT_SOURCE_DIR}/reco/littrack/cbm/elid

${CBMROOT_SOURCE_DIR}/analysis/PWGDIL/dielectron/lmvm
)

Include_Directories( ${INCLUDE_DIRECTORIES})

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES} 
  ${Boost_INCLUDE_DIR}
  ${KFParticle_INCLUDE_DIR}
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
CbmAnaConversion.cxx
CbmAnaConversionTomography.cxx
CbmAnaConversionRich.cxx
CbmAnaConversionReco.cxx
CbmAnaConversionPhotons.cxx
CbmAnaConversionPhotons2.cxx
CbmAnaConversionRecoFull.cxx
CbmAnaConversionTest.cxx
CbmAnaConversionTest2.cxx
)

IF (SSE_FOUND)
  Message(STATUS "Analysis will be compiled with SSE support")
  ADD_DEFINITIONS(-DHAVE_SSE)
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS 
  "-msse -O3")
ELSE (SSE_FOUND)
  MESSAGE(STATUS "Analysis will be compiled without SSE support")
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS 
  "-O3")
ENDIF (SSE_FOUND)

ADD_DEFINITIONS(-DDO_TPCCATRACKER_EFF_PERFORMANCE -DNonhomogeneousField -DCBM -DUSE_TIMERS)


Set(LINKDEF CbmConversionAnalysisLinkDef.h)
Set(LIBRARY_NAME CbmConversionAnalysis)
Set(DEPENDENCIES KF L1 CbmRichReco CbmRichBase CbmRecoBase CbmData Littrack)
Set(DEFINITIONS -DDO_TPCCATRACKER_EFF_PERFORMANCE -DNonhomogeneousField -DCBM -DUSE_TIMERS)

GENERATE_LIBRARY()




