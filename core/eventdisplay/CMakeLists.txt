set(INCLUDE_DIRECTORIES
${CMAKE_CURRENT_SOURCE_DIR}

${CBMBASE_DIR}

${CBMDATA_DIR}
${CBMDATA_DIR}/sts
${CBMDATA_DIR}/trd
${CBMDATA_DIR}/tof
${CBMDATA_DIR}/rich
${CBMDATA_DIR}/global

${CMAKE_SOURCE_DIR}/core/detectors/trd
)

include_directories( ${INCLUDE_DIRECTORIES})

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES} 
)

Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})


set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
${Boost_LIBRARY_DIRS}
) 

link_directories( ${LINK_DIRECTORIES})

set(SRCS
CbmTrdDigiDraw.cxx
CbmPixelHitSetDraw.cxx
CbmEvDisTracks.cxx
CbmRecoTracks.cxx
CbmPointSetArray.cxx
CbmPointSetArrayDraw.cxx
CbmPointSetArrayEditor.cxx
)

set(LINKDEF  CbmDisplayLinkDef.h)
Set(LIBRARY_NAME CbmDisplay)
Set(DEPENDENCIES
    CbmData EventDisplay Base CbmTrdBase
)

GENERATE_LIBRARY()
