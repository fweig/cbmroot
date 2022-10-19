## Create a library called "libRun" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/digis
  ${CMAKE_CURRENT_SOURCE_DIR}/tracks
  )


set(SRCS
  digis/CbmEvbuildRawAlgo.cxx
  digis/CbmEvbuildRawTask.cxx
  digis/CbmAlgoBuildRawEvents.cxx
  digis/CbmTaskBuildRawEvents.cxx
  digis/CbmBuildEventsIdeal.cxx
  digis/CbmBuildEventsIdealNew.cxx
  digis/CbmBuildEventsQa.cxx
  digis/CbmBuildEventsSimple.cxx
  digis/CbmSeedFinderSlidingWindow.cxx
  digis/CbmSeedFinderQa.cxx
  #digis/CbmEvBuildSource.cxx

  tracks/CbmBuildEventsFromTracksIdeal.cxx
  tracks/CbmBuildEventsFromTracksReal.cxx
  tracks/CbmEventBuilderQa.cxx
)


IF (SSE_FOUND)
  ADD_DEFINITIONS(-DHAVE_SSE)
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS 
  "-msse -O3")
  Message(STATUS "eventbuilder will be compiled with SSE support")
ELSE (SSE_FOUND)
  Message(STATUS "eventbuilder will be compiled without SSE support")
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS 
  "-O3")
ENDIF (SSE_FOUND)


set(LIBRARY_NAME CbmEventBuilder)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmBase
  CbmData
  FairRoot::Base
  ROOT::Core
  ROOT::Hist
  )

set(PRIVATE_DEPENDENCIES
  CbmQaBase
  CbmStsBase
  FairLogger::FairLogger
  L1
  ROOT::EG
  ROOT::Gpad
  ROOT::MathCore
  ROOT::RHTTP
  ROOT::RIO
  ROOT::Tree
  )


generate_cbm_library()
