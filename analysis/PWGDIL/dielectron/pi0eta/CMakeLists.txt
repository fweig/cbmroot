set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

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



set(LIBRARY_NAME CbmConversion2Analysis)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmData
  CbmRichReco
  CbmLmvm
  KFParticleInterface
  NicaCbmFormat
  FairRoot::Base
  ROOT::Core
  ROOT::Gpad
  ROOT::Hist
  ROOT::MathCore
  ROOT::MLP
  )

set(PRIVATE_DEPENDENCIES
  CbmBase
  CbmRichBase
  KFParticle
  L1
  ROOT::RIO
  ROOT::Tree
  )

set(INTERFACE_DEPENDENCIES
  KF
  ROOT::Matrix
  ROOT::Physics
  )

generate_cbm_library()

add_dependencies(G__CbmConversion2Analysis KFPARTICLE)

