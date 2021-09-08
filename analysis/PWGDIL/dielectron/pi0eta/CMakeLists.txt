Set(INCLUDE_DIRECTORIES
${CMAKE_CURRENT_SOURCE_DIR}

${CBMROOT_SOURCE_DIR}/analysis/PWGDIL/dielectron/lmvm

${CBMROOT_SOURCE_DIR}/reco/base

${CBMBASE_DIR}/draw
${CBMBASE_DIR}/utils

${CBMDATA_DIR}
${CBMDATA_DIR}/mvd
${CBMDATA_DIR}/sts
${CBMDATA_DIR}/global
${CBMDATA_DIR}/rich

${CBMROOT_SOURCE_DIR}/reco/KF
${CBMROOT_SOURCE_DIR}/reco/KF/Interface 

${CBMROOT_SOURCE_DIR}/reco/L1
${CBMROOT_SOURCE_DIR}/reco/L1/L1Algo
${CBMROOT_SOURCE_DIR}/reco/L1/ParticleFinder 

${CBMROOT_SOURCE_DIR}/littrack/cbm/elid

${CBMROOT_SOURCE_DIR}/rich
${CBMROOT_SOURCE_DIR}/reco/detectors/rich/fitter
${CBMROOT_SOURCE_DIR}/reco/detectors/rich/utils
)

Include_Directories( ${INCLUDE_DIRECTORIES})

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${KFParticle_INCLUDE_DIR}
  ${BASE_INCLUDE_DIRECTORIES} 
  ${Boost_INCLUDE_DIR}
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
CbmKresConversionMain.cxx
CbmKresConversionManual.cxx
CbmKresConversionReconstruction.cxx
CbmKresConversionKF.cxx
CbmKresConversionGeneral.cxx
CbmKresConversionBG.cxx
CbmKresConversionManualmbias1.cxx
CbmKresConversionManualmbias2.cxx
CbmKresConversionManualmbias3.cxx
CbmKresConversionManualmbias4.cxx
CbmKresTrainAnn.cxx
CbmKresSelectAnn.cxx
CbmKresTemperature.cxx
CbmKresTrainAnnDirectPhotons.cxx
CbmKresConversionPhotons.cxx
CbmKresSelectAnnPhotons.cxx
CbmKresGammaCorrection.cxx
CbmKresConversionCorrectedPhotons.cxx
CbmKresEta.cxx
CbmKresEtaMCAnalysis.cxx
CbmKresSelectGoodEvents.cxx
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


Set(LINKDEF CbmConversion2AnalysisLinkDef.h)
Set(LIBRARY_NAME CbmConversion2Analysis)
Set(DEPENDENCIES KF L1 CbmRichBase CbmRichReco CbmRecoBase CbmData)
Set(DEFINITIONS -DDO_TPCCATRACKER_EFF_PERFORMANCE -DNonhomogeneousField -DCBM -DUSE_TIMERS)

GENERATE_LIBRARY()




