set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}

  ${CBMDATA_DIR}
  ${CBMDATA_DIR}/tof
  ${CBMDATA_DIR}/raw
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
  CbmTofCell.cxx
  CbmTofContFact.cxx
  CbmTofCreateDigiPar.cxx
  CbmTofDigiBdfPar.cxx
  CbmTofDigiPar.cxx
  CbmMcbm2018TofPar.cxx
  CbmTofGeoHandler.cxx
  TTrbHeader.cxx
)

set(LINKDEF CbmTofBaseLinkDef.h)
Set(LIBRARY_NAME CbmTofBase)
Set(DEPENDENCIES
    CbmData Base
)

GENERATE_LIBRARY()
