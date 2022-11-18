set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  #${CMAKE_CURRENT_SOURCE_DIR}/../lmvm
  )

set(SRCS
  CbmAnaConversion.cxx
  CbmAnaConversionKF.cxx
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


set(LIBRARY_NAME CbmConversionAnalysis)
set(LINKDEF CbmConversionAnalysisLinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmData
  CbmRichBase
  CbmLmvm
  KF
  KFParticleInterface
  Littrack
  FairRoot::Base
  ROOT::Core
  ROOT::Hist
  ROOT::MathCore
  )

set(PRIVATE_DEPENDENCIES
  CbmBase
  CbmRichReco
  L1
  KFParticle
  ROOT::EG
  ROOT::Gpad
  )

set(INTERFACE_DEPENDENCIES
  ParticleInterface
  ROOT::Physics
  )

generate_cbm_library()

add_dependencies(G__CbmConversionAnalysis KFPARTICLE)
