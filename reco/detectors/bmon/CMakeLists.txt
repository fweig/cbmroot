set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/unpack
  )


set(SRCS
  unpack/CbmBmonUnpackAlgo.cxx
  unpack/CbmBmonUnpackConfig.cxx
)


set(LIBRARY_NAME CbmBmonReco)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmBase
  CbmData
  CbmRecoBase
  CbmTofBase
  CbmTofReco
  FairLogger::FairLogger
  FairRoot::Base
  ROOT::Core
  ROOT::Graf3d
  ROOT::Hist
  ROOT::MathCore
  ROOT::Minuit
  )

set(PRIVATE_DEPENDENCIES
  CbmDisplay
  CbmFlibFlesTools
  FairRoot::EventDisplay
  FairRoot::ParBase
  ROOT::Eve
  ROOT::Geom
  ROOT::Gpad
  ROOT::Graf
  ROOT::Matrix
  ROOT::Physics
  ROOT::RHTTP
  ROOT::RIO
  ROOT::Spectrum
  )

set(INTERFACE_DEPENDENCIES
  external::fles_ipc
  )


generate_cbm_library()
