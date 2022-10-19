# Create a library called "libClustering".

set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

set(SRCS
  CbmClusteringGeometry.cxx
  CbmMuchClustering.cxx
  CbmClusteringA1.cxx
  CbmClusteringSL.cxx
  CbmClusteringWard.cxx
  CbmMuchTest.cxx
  #CbmMvdGeoCl.cxx
  #CbmMvdClustering.cxx
  #CbmMvdClusteringNoCharges.cxx
  )


IF (SSE_FOUND)
  ADD_DEFINITIONS(-DHAVE_SSE)
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS 
  "-msse -O3 -w")
  Message(STATUS "Clustering will be compiled with SSE support")
ELSE (SSE_FOUND)
  Message(STATUS "Clustering will be compiled without SSE support")
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS 
  "-O3 -w")
ENDIF (SSE_FOUND)

If (Boost_FOUND)
  set(DEFINITIONS BOOST_FOUND)
EndIf (Boost_FOUND)


set(LIBRARY_NAME CbmClustering)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmMuchBase
  FairRoot::Base
  ROOT::Hist
  )

set(PRIVATE_DEPENDENCIES
  CbmBase
  CbmData
  ROOT::Core
  ROOT::Gpad
  ROOT::MathCore
  ROOT::RIO
  )

generate_cbm_library()
