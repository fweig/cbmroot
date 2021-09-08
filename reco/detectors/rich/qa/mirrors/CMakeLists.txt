# Create a library called "libRich" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

FIND_PACKAGE(TIFF)

Set(INCLUDE_DIRECTORIES
${CMAKE_CURRENT_SOURCE_DIR}/

${CBMBASE_DIR}
${CBMBASE_DIR}/report
${CBMBASE_DIR}/draw
${CBMBASE_DIR}/utils
)

Include_Directories( ${INCLUDE_DIRECTORIES})

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES}
  ${Boost_INCLUDE_DIR}
  ${TIFF_INCLUDE_DIR}
)

Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

get_filename_component(TIFF_LIBRARY_DIR ${TIFF_LIBRARY} DIRECTORY)

set(LINK_DIRECTORIES
${FAIRROOT_LIBRARY_DIR}
${ROOT_LIBRARY_DIR}
${Boost_LIBRARY_DIRS}
${TIFF_LIBRARY_DIR}
)

link_directories( ${LINK_DIRECTORIES})
set(SRCS
CbmRichRonchiAna.cxx
)


set(HEADERS
CbmRichRonchiAna.h
)


IF (SSE_FOUND)
  ADD_DEFINITIONS(-DHAVE_SSE)
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS "-msse -O3")
  Message(STATUS "Rich will be compiled with SSE support")
ELSE (SSE_FOUND)
  Message(STATUS "Rich will be compiled without SSE support")
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS "-O3")
ENDIF (SSE_FOUND)

# END SSE

Set(LINKDEF CbmRichMirrorsLinkDef.h)
Set(LIBRARY_NAME CbmRichMirrors)

Set(DEPENDENCIES
    CbmBase CbmData Base MLP boost_regex tiff)

GENERATE_LIBRARY()
