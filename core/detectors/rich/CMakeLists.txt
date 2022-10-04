set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/utils
  )

set(SRCS
  CbmRichPmt.cxx
  CbmRichEventDisplay.cxx
  CbmRichContFact.cxx
  CbmMcbm2018RichPar.cxx
  )

set(NO_DICT_SRCS
  CbmRichElectronIdAnn.cxx
  CbmRichGeoManager.cxx
  CbmRichDigiMapManager.cxx
  CbmRichMCbmDigiMapManager.cxx
  utils/CbmRichUtil.cxx
  )


IF (SSE_FOUND)
  ADD_DEFINITIONS(-DHAVE_SSE)
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS "-msse -O3")
ELSE (SSE_FOUND)
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS "-O3")
ENDIF (SSE_FOUND)

set(LIBRARY_NAME CbmRichBase)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES 
  CbmData
  FairRoot::Base
  FairRoot::ParBase
  ROOT::Core 
  CbmBase
  ROOT::MathCore 
  )

set(PRIVATE_DEPENDENCIES 
  FairLogger::FairLogger 
  ROOT::EG 
  ROOT::Geom 
  ROOT::Hist
  ROOT::RIO
  ROOT::MLP
  )

generate_cbm_library()


# Install file which has no corresponding source file
install(FILES
        CbmRichRecGeoPar.h
        DESTINATION include
       )

