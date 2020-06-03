set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}

  ${CBMDETECTORBASE_DIR}/tof

  ${CMAKE_SOURCE_DIR}/reco/detectors/tof

  ${CBMDATA_DIR}
  ${CBMDATA_DIR}/tof
  ${CBMDATA_DIR}/global
  ${CBMDATA_DIR}/base

  ${CBMBASE_DIR} 

  ${CBMROOT_SOURCE_DIR}/reco/base
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
  CbmTofAnaTestbeam.cxx 
  CbmTofGeometryQa.cxx
  CbmTofHitFinderQa.cxx
  CbmTofHitFinderTBQA.cxx
  CbmTofTests.cxx 
)

set(LINKDEF CbmTofAnaLinkDef.h)
Set(LIBRARY_NAME CbmTofAna)
Set(DEPENDENCIES
    CbmTofBase CbmTofReco CbmRecoBase CbmBase CbmData 
)

GENERATE_LIBRARY()
