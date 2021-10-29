set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/utils

  ${CBMDATA_DIR}
  ${CBMDATA_DIR}/rich
  ${CBMDATA_DIR}/global

  ${CBMBASE_DIR}
  ${CBMBASE_DIR}/draw
)

include_directories(${INCLUDE_DIRECTORIES})

set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES}
  ${Boost_INCLUDE_DIR}
)

include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
)

link_directories(${LINK_DIRECTORIES})

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
)

IF (SSE_FOUND)
  ADD_DEFINITIONS(-DHAVE_SSE)
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS "-msse -O3")
ELSE (SSE_FOUND)
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS "-O3")
ENDIF (SSE_FOUND)

set(LINKDEF  CbmRichBaseLinkDef.h)

Set(LIBRARY_NAME CbmRichBase)
Set(DEPENDENCIES CbmData CbmBase Base)

GENERATE_LIBRARY()

Install(FILES
        CbmRichRecGeoPar.h
        DESTINATION include
       )
