set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/qa
  )

set(SRCS
  CbmTrd.cxx
  CbmTrdDigitizer.cxx 
  CbmTrdModuleSim.cxx 
  CbmTrdModuleSimR.cxx 
  CbmTrdModuleSim2D.cxx 
  CbmTrdTrianglePRF.cxx
  CbmTrdRawToDigiR.cxx
  CbmTrdCheckUtil.cxx
  
  qa/CbmTrdDigitizerPRFQa.cxx
  qa/CbmTrdMCQa.cxx
  qa/CbmTrdHitRateFastQa.cxx
  qa/CbmTrdHitRateQa.cxx
  )

IF (SSE_FOUND)
  ADD_DEFINITIONS(-DHAVE_SSE)
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS 
    "-msse -O3")
  Message(STATUS "TRD SIM will be compiled with SSE support")
ELSE (SSE_FOUND)
  Message(STATUS "TRD SIM will be compiled without SSE support")
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS 
    "-O3")
ENDIF (SSE_FOUND)


set(LIBRARY_NAME CbmTrdSim)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmBase
  CbmData
  CbmTrdBase
  FairRoot::Base
  ROOT::Core
  ROOT::Hist
  ROOT::Physics
  ROOT::RIO
  )

set(PRIVATE_DEPENDENCIES
  CbmSimBase
  FairRoot::GeoBase
  FairRoot::ParBase
  ROOT::Geom
  ROOT::Gpad
  ROOT::Graf
  ROOT::MathCore
  )


generate_cbm_library()
