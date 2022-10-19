set(INCLUDE_DIRECTORIES
  ${CBMROOT_SOURCE_DIR}/fles/flestools
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/unpack
  )


set(SRCS
  CbmTofBuildDigiEvents.cxx
  CbmTofHitMaker.cxx
  CbmTofCosmicClusterizer.cxx
  CbmTofEventClusterizer.cxx
  CbmTofFindTracks.cxx
  CbmTofExtendTracks.cxx
  CbmTofSimpClusterizer.cxx
  CbmTofTrackFinderNN.cxx
  CbmTofTestBeamClusterizer.cxx
  CbmTofTrackletTools.cxx
  CbmTofCalibrator.cxx
  LKFMinuit.cxx

  unpack/CbmTofUnpackAlgo.cxx
  unpack/CbmTofUnpackConfig.cxx
  unpack/CbmBmonUnpackConfig.cxx
  unpack/CbmTofUnpackMonitor.cxx
)


set(LIBRARY_NAME CbmTofReco)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmBase
  CbmData
  CbmRecoBase
  CbmTofBase
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
