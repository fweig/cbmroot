set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}

  ${CBMDETECTORBASE_DIR}/tof

  ${CBMDATA_DIR}
  ${CBMDATA_DIR}/tof

  ${CBMBASE_DIR} 
  ${CBMBASE_DIR}/utils

  ${CBMROOT_SOURCE_DIR}/sim/transport/base
)

include_directories(${INCLUDE_DIRECTORIES})

Set(SYSTEM_INCLUDE_DIRECTORIES
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
  CbmTof.cxx 
  CbmTofMergeMcPoints.cxx 
  CbmTofDigitize.cxx
)

set(LINKDEF CbmTofSimLinkDef.h)
set(LIBRARY_NAME CbmTofSim)
set(DEPENDENCIES
    CbmBase CbmData CbmSimBase CbmTofBase
)

GENERATE_LIBRARY()
