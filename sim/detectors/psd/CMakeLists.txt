set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CBMDATA_DIR}
  ${CBMDATA_DIR}/psd
  ${CBMBASE_DIR}
  ${CBMBASE_DIR}/utils
  ${CBMROOT_SOURCE_DIR}/sim/transport/base
)

include_directories(${INCLUDE_DIRECTORIES})

set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES} 
)

include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
)
 
link_directories(${LINK_DIRECTORIES})


set(SRCS
  CbmPsdMC.cxx  
  CbmPsdIdealDigitizer.cxx
  CbmPsdSimpleDigitizer.cxx
)

set(LINKDEF CbmPsdSimLinkDef.h)
set(LIBRARY_NAME CbmPsdSim)
set(DEPENDENCIES
    CbmSimBase CbmData CbmBase Base
)

GENERATE_LIBRARY()

