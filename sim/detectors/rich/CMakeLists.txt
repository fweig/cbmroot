set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/geo
  )

set(SRCS
  CbmRich.cxx
  CbmRichDigitizer.cxx
  geo/RichGeoCreator.cxx
  )

IF (SSE_FOUND)
  ADD_DEFINITIONS(-DHAVE_SSE)
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS "-msse -O3")
  Message(STATUS "Rich will be compiled with SSE support")
ELSE (SSE_FOUND)
  Message(STATUS "Rich will be compiled without SSE support")
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS "-O3")
ENDIF (SSE_FOUND)


set(LIBRARY_NAME CbmRichSim)
set(LINKDEF  ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmBase
  CbmData
  CbmRichBase
  FairRoot::Base
  ROOT::Core
  ROOT::Geom
  ROOT::Physics
  )

set(PRIVATE_DEPENDENCIES
  CbmSimBase
  FairRoot::GeoBase
  FairRoot::ParBase
  ROOT::EG
  ROOT::Gdml
  ROOT::MathCore
  ROOT::RIO
  VMCLibrary
  )

generate_cbm_library()
