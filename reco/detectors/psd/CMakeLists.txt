set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/unpack

  ${CBMDATA_DIR}
  ${CBMDATA_DIR}/global
  ${CBMDATA_DIR}/base
  ${CBMDATA_DIR}/psd
  ${CBMDATA_DIR}/raw
  ${CBMBASE_DIR}
  ${CBMDETECTORBASE_DIR}/psd

  ${CBMROOT_SOURCE_DIR}/reco/base
)

include_directories(${INCLUDE_DIRECTORIES})

set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES} 
  ${IPC_INCLUDE_DIRECTORY} # for fles infos for unpacker
)

include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
)
 
link_directories(${LINK_DIRECTORIES})

set(SRCS
  CbmPsdHitProducer.cxx
  CbmPsdMCbmHitProducer.cxx

  unpack/CbmPsdUnpackAlgo.cxx
  unpack/CbmPsdUnpackConfig.cxx  
)

set(LINKDEF CbmPsdRecoLinkDef.h)
set(LIBRARY_NAME CbmPsdReco)
set(DEPENDENCIES
    CbmData 
    CbmBase 
    Base 
    CbmRecoBase 
    fles_ipc # for unpacker
)

GENERATE_LIBRARY()

