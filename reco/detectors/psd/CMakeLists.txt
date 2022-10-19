set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/unpack
  )

set(SRCS
  CbmPsdHitProducer.cxx
  CbmPsdMCbmHitProducer.cxx

  unpack/CbmPsdUnpackAlgo.cxx
  unpack/CbmPsdUnpackConfig.cxx
)


set(LIBRARY_NAME CbmPsdReco)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmBase
  CbmData
  FairRoot::Base
  )

set(PRIVATE_DEPENDENCIES
  CbmPsdBase
  CbmRecoBase
  FairRoot::ParBase
  ROOT::Core
  ROOT::MathCore
  ROOT::RIO
  )

set(INTERFACE_DEPENDENCIES
  FairLogger::FairLogger
  external::fles_ipc
  ROOT::Hist
  )

generate_cbm_library()
