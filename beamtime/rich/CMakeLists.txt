# Create a library called "libRich" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(INCLUDE_DIRECTORIES
${CBMBASE_DIR}
${CBMBASE_DIR}/report
${CBMBASE_DIR}/draw
${CBMBASE_DIR}/utils

${CBMDATA_DIR}
${CBMDATA_DIR}/rich

${CBMROOT_SOURCE_DIR}/beamtime/data/raw

${CMAKE_CURRENT_SOURCE_DIR}

${CBMROOT_SOURCE_DIR}/reco/detectors/rich/fitter
${CBMROOT_SOURCE_DIR}/reco/detectors/rich
)

Include_Directories( ${INCLUDE_DIRECTORIES})

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES}
  ${Boost_INCLUDE_DIR}
)
Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
${Boost_LIBRARY_DIRS}
#${Vc_LIB_DIR}
${KFParticle_LIB_DIR}
)
link_directories( ${LINK_DIRECTORIES})

set(SRCS
CbmRichTrbUnpack.cxx
CbmRichTrbUnpack2.cxx
CbmTrbEdgeMatcher.cxx
CbmTrbCalibrator.cxx
CbmRichTrbEventBuilder.cxx
CbmRichProtoAnalysis.cxx
CbmRichEventDebug.cxx
CbmRichRingHitsAnalyser.cxx
CbmRichTrbRecoQa.cxx
CbmRichTrbPulserQa.cxx
CbmRichHitInfo.cxx
CbmRichTrbRecoQaStudyReport.cxx
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

set(LINKDEF  CbmRichBeamtimeLinkDef.h)

Set(LIBRARY_NAME CbmRichBeamtime)
Set(DEPENDENCIES
    CbmBase CbmData CbmRichBase Base
)

GENERATE_LIBRARY()
