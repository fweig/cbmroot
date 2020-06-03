# Create a library called "libTrdSim" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(INCLUDE_DIRECTORIES
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/qa

${CBMDETECTORBASE_DIR}/trd

${CBMBASE_DIR}
${CBMBASE_DIR}/utils
${CBMBASE_DIR}/report
${CBMBASE_DIR}/draw

${CBMROOT_SOURCE_DIR}/sim/transport/base

${CBMDATA_DIR}
${CBMDATA_DIR}/base
${CBMDATA_DIR}/trd 
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
${Vc_LIB_DIR}
) 

link_directories( ${LINK_DIRECTORIES})

set(SRCS
CbmTrd.cxx
CbmTrdDigitizer.cxx 
CbmTrdModuleSim.cxx 
CbmTrdModuleSimR.cxx 
CbmTrdModuleSimT.cxx 
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



set(LINKDEF CbmTrdSimLinkDef.h)
Set(LIBRARY_NAME CbmTrdSim)
Set(DEPENDENCIES
    CbmBase CbmData Base CbmTrdBase
)

GENERATE_LIBRARY()
