set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )


set(SRCS
  CbmAnaJpsiTask.cxx
  CbmAnaJpsiReport.cxx
  CbmAnaJpsiHist.cxx
  CbmAnaJpsiCandidate.cxx
  CbmAnaJpsiSuperEvent.cxx
  CbmAnaJpsiSuperEventReport.cxx
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



set(LIBRARY_NAME CbmJPsiAnalysis)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmBase
  CbmData
  ROOT::Core
  ROOT::Hist
  ROOT::MathCore
  )

set(PRIVATE_DEPENDENCIES
  KF
  L1
  Littrack
  ROOT::Gpad
  ROOT::Graf
  ROOT::RIO
  ROOT::Tree
  )

set(INTERFACE_DEPENDENCIES
  FairRoot::Base
  ROOT::EG
  ROOT::Physics
  )

generate_cbm_library()
