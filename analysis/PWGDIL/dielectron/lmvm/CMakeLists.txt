set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

set(SRCS
  LmvmTask.cxx
  LmvmDraw.cxx
  LmvmDrawAll.cxx
  LmvmHist.cxx
  LmvmUtils.cxx
  )


IF (SSE_FOUND)
  Message(STATUS "LMVM analysis will be compiled with SSE support")
  ADD_DEFINITIONS(-DHAVE_SSE)
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS 
  "-msse -O3")
ELSE (SSE_FOUND)
  MESSAGE(STATUS "LMVM analysis will be compiled without SSE support")
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS 
  "-O3")
ENDIF (SSE_FOUND)

ADD_DEFINITIONS(-DDO_TPCCATRACKER_EFF_PERFORMANCE -DNonhomogeneousField -DCBM -DUSE_TIMERS)



set(LIBRARY_NAME CbmLmvm)
set(LINKDEF LmvmLinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmBase
  CbmData
  KF
  NicaCbmFormat
  FairRoot::Base
  ROOT::Core
  ROOT::Hist
  ROOT::MathCore
  ROOT::Physics
  )

set(PRIVATE_DEPENDENCIES
  L1
  Littrack
  LittrackQA
  ROOT::EG
  ROOT::Gpad
  ROOT::Graf
  ROOT::RIO
  ROOT::Tree
  )


generate_cbm_library()




