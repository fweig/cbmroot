
Set(INCLUDE_DIRECTORIES
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/qa

${CBMDETECTORBASE_DIR}/much

${CBMBASE_DIR} 
${CBMBASE_DIR}/utils

${CBMROOT_SOURCE_DIR}/sim/transport/base
${CBMROOT_SOURCE_DIR}/core/qa

${CBMDATA_DIR} 
${CBMDATA_DIR}/much 
)

Include_Directories( ${INCLUDE_DIRECTORIES})

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
CbmGeoMuch.cxx  
CbmMuch.cxx

CbmMuchDigitizeGem.cxx
CbmMuchReadoutBuffer.cxx
CbmMuchSignal.cxx

qa/CbmMuchTransportQa.cxx
)

set(LINKDEF CbmMuchSimLinkDef.h)
Set(LIBRARY_NAME CbmMuchSim)
Set(DEPENDENCIES
    CbmMuchBase CbmBase CbmData Base CbmQaBase
)

GENERATE_LIBRARY()
