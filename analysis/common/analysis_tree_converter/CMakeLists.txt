set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

set(SRCS
   CbmConverterManager.cxx
   CbmConverterTask.cxx
   CbmSimEventHeaderConverter.cxx
   CbmRecEventHeaderConverter.cxx
   CbmStsTracksConverter.cxx
   CbmSimTracksConverter.cxx
   CbmPsdModulesConverter.cxx
   CbmTofHitsConverter.cxx
   CbmTrdTracksConverter.cxx
   CbmRichRingsConverter.cxx
   CbmMatchEvents.cxx
  )


IF (SSE_FOUND)
  Message(STATUS "${LIBRARY_NAME} will be compiled with SSE support")
  ADD_DEFINITIONS(-DHAVE_SSE)
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS "-msse -O3")
ELSE (SSE_FOUND)
  MESSAGE(STATUS "${LIBRARY_NAME} will be compiled without SSE support")
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS "-msse -O3")
ENDIF (SSE_FOUND)


set(LIBRARY_NAME CbmAnalysisTreeInterface)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  FairRoot::Base
  AnalysisTreeInfra
  ROOT::Core
  )

set(PRIVATE_DEPENDENCIES
  CbmData
  KF
  L1
  AnalysisTreeBase
  CbmSimGenerators
  FairLogger::FairLogger
  ROOT::Geom
  ROOT::MathCore
  ROOT::RIO
  ROOT::Tree
  )

generate_cbm_library()

Install(FILES ../../../macro/analysis/common/analysis_tree_converter/run_analysis_tree_maker.C
        DESTINATION share/cbmroot/macro/analysis_tree
       )
