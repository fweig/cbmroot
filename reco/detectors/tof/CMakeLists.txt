set(INCLUDE_DIRECTORIES

  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/unpack

  ${CBMDETECTORBASE_DIR}/tof

  ${CBMDATA_DIR}
  ${CBMDATA_DIR}/base
  ${CBMDATA_DIR}/tof
  ${CBMDATA_DIR}/raw
  ${CBMDATA_DIR}/global

  ${CBMBASE_DIR}

  ${CBMROOT_SOURCE_DIR}/reco/base

  ${CBMROOT_SOURCE_DIR}/core/eventdisplay

  ${CBMROOT_SOURCE_DIR}/fles/flestools # for Timeslice/Microslice printout formatting tools
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
)

set(LINKDEF CbmTofRecoLinkDef.h)
Set(LIBRARY_NAME CbmTofReco)
Set(DEFINITIONS ${DEFINITIONS} BUILD_TOF_BEAMTIME)
Set(DEPENDENCIES
    CbmTofBase CbmRecoBase CbmBase CbmData CbmDisplay Base Minuit
)

GENERATE_LIBRARY()
