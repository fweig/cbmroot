set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/qa
  ${CMAKE_CURRENT_SOURCE_DIR}/unpack
  )

set(SRCS
  CbmMuchFindHitsGem.cxx
  CbmMuchHitProducerIdeal.cxx

  CbmMuchFindTracks.cxx
  CbmMuchMatchTracks.cxx
  CbmMuchTrackFinderIdeal.cxx

  qa/CbmMuchHitFinderQa.cxx

  unpack/CbmMuchUnpackAlgo.cxx
  unpack/CbmMuchUnpackMonitor.cxx
  unpack/CbmMuchUnpackConfig.cxx
  )


set(LIBRARY_NAME CbmMuchReco)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmData
  CbmMuchBase
  CbmRecoBase
  FairRoot::Base
  ROOT::Core
  ROOT::Hist
  )

set(PRIVATE_DEPENDENCIES
  CbmBase
  CbmQaBase
  FairRoot::ParBase
  ROOT::Gpad
  ROOT::Graf
  ROOT::MathCore
  ROOT::Physics
  ROOT::RHTTP
  ROOT::RIO
  )

set(INTERFACE_DEPENDENCIES
  FairLogger::FairLogger
  external::fles_ipc
  )


generate_cbm_library()
