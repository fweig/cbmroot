## Create a library called "libRun" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(INCLUDE_DIRECTORIES
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/digis
${CMAKE_CURRENT_SOURCE_DIR}/tracks

${CBMBASE_DIR}
${CBMROOT_SOURCE_DIR}/run

${CBMDATA_DIR}
${CBMDATA_DIR}/base
${CBMDATA_DIR}/mvd
${CBMDATA_DIR}/sts
${CBMDATA_DIR}/sts
${CBMDATA_DIR}/rich
${CBMDATA_DIR}/much
${CBMDATA_DIR}/trd
${CBMDATA_DIR}/tof
${CBMDATA_DIR}/psd
${CBMDATA_DIR}/global

${CBMROOT_SOURCE_DIR}/reco/L1
${CBMROOT_SOURCE_DIR}/reco/L1/L1Algo

${CBMROOT_SOURCE_DIR}/mvd
${CBMDETECTORBASE_DIR}/sts

${CBMROOT_SOURCE_DIR}/rich
${CBMROOT_SOURCE_DIR}/tof/TofMC
${CBMROOT_SOURCE_DIR}/psd
${CBMROOT_SOURCE_DIR}/field
${CBMROOT_SOURCE_DIR}/core/data/raw
${CBMROOT_SOURCE_DIR}/core/qa
)

Include_Directories( ${INCLUDE_DIRECTORIES})

set(SYSTEM_INCLUDE_DIRECTORIES
  ${KFParticle_INCLUDE_DIR}
  ${BASE_INCLUDE_DIRECTORIES} 
)

Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})


set(LINK_DIRECTORIES
${KFParticle_LIB_DIR}
${FAIRROOT_LIBRARY_DIR}
${ROOT_LIBRARY_DIR}
${Boost_LIBRARY_DIRS}
)
 
link_directories( ${LINK_DIRECTORIES})

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
#digis/CbmEvBuildSource.cxx

tracks/CbmBuildEventsFromTracksIdeal.cxx
tracks/CbmBuildEventsFromTracksReal.cxx
tracks/CbmEventBuilderQa.cxx
)


IF (SSE_FOUND)
  ADD_DEFINITIONS(-DHAVE_SSE)
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS 
  "-msse -O3")
  Message(STATUS "L1 will be compiled with SSE support")
ELSE (SSE_FOUND)
  Message(STATUS "L1 will be compiled without SSE support")
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS 
  "-O3")
ENDIF (SSE_FOUND)

set(LINKDEF CbmEventBuilderLinkDef.h)
Set(LIBRARY_NAME CbmEventBuilder)
Set(DEPENDENCIES
    CbmData CbmBase CbmField CbmStsBase CbmMvd Base CbmQaBase
)

GENERATE_LIBRARY()
