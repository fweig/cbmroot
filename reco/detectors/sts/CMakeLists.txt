# CMakeList file for library libRecoSts
# Last update: V. Friese, 25.03.2020


set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/sts
  ${CMAKE_CURRENT_SOURCE_DIR}/unpack
  ${CMAKE_CURRENT_SOURCE_DIR}/qa
  ${CMAKE_CURRENT_SOURCE_DIR}/experimental
  )


set(SRCS
  CbmRecoSts.cxx
  CbmStsAlgoAnaCluster.cxx
  CbmStsAlgoFindClusters.cxx
  CbmStsAlgoFindHits.cxx
  CbmStsAlgoFindHitsOrtho.cxx
  CbmStsFindTracks.cxx
  CbmStsFindTracksEvents.cxx
  CbmStsRecoModule.cxx
  CbmRecoStsPixel.cxx
  CbmStsTrackFinderIdeal.cxx

  unpack/CbmStsUnpackAlgo.cxx
  unpack/CbmStsUnpackAlgoBase.cxx
  unpack/CbmStsUnpackAlgoLegacy.cxx
  unpack/CbmStsUnpackConfig.cxx
  unpack/CbmStsUnpackMonitor.cxx

  qa/CbmStsFindTracksQa.cxx
  )

set(LIBRARY_NAME CbmRecoSts)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  Algo
  CbmData
  CbmStsBase
  FairRoot::Base
  ROOT::Core
  ROOT::Hist
  xpu
  )

set(PRIVATE_DEPENDENCIES
  CbmBase
  CbmRecoBase
  CbmFlibFlesTools
  CbmMvdBase
  FairRoot::ParBase
  ROOT::Geom
  ROOT::Gpad
  ROOT::MathCore
  ROOT::RHTTP
  ROOT::RIO
  )

set(INTERFACE_DEPENDENCIES
  FairLogger::FairLogger
  external::fles_ipc
  ROOT::Physics
  )


# ---- Enable OpenMP  -------------------------------------
# Comented since the OpenMP code is also commented in the moment
#if (OPENMP_FOUND)
#  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
#endif()
# ---------------------------------------------------------

generate_cbm_library()
