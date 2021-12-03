# Create a library called "libLxTrack" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed
# here.

Set(INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES}

  ${CBMROOT_SOURCE_DIR}/reco/tracking/lx
  ${CBMROOT_SOURCE_DIR}/reco/tracking/lx/TBBinned
  ${CBMROOT_SOURCE_DIR}/reco/tracking/lx/GenNoiseElectrons
  ${CBMROOT_SOURCE_DIR}/reco/tracking/lx/CalcStats

  ${CBMBASE_DIR} 

  ${CBMDATA_DIR} 
  ${CBMDATA_DIR}/much
  ${CBMDATA_DIR}/sts
  ${CBMDATA_DIR}/mvd
  ${CBMDATA_DIR}/trd
  ${CBMDATA_DIR}/tof
  ${CBMDATA_DIR}/global

  ${CBMDETECTORBASE_DIR}/much

  ${CBMROOT_SOURCE_DIR}/sim/detectors/much

  ${CBMROOT_SOURCE_DIR}/KF
  ${CBMROOT_SOURCE_DIR}/KF/Interface

  ${CBMROOT_SOURCE_DIR}/sts

  ${CBMROOT_SOURCE_DIR}/reco/littrack/std
  ${CBMROOT_SOURCE_DIR}/reco/littrack/cbm
)

Include_Directories( ${INCLUDE_DIRECTORIES})

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES}
)

Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

Set(LINK_DIRECTORIES
  ${KFParticle_LIB_DIR}
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
)
 
Link_Directories( ${LINK_DIRECTORIES})

Set(SRCS
  TBBinned/LxTBTask.cxx
#  TBBinned/LxTBMLTask.cxx
  TBBinned/LxTBTieTracks.cxx
  TBBinned/LxTBTrdTask.cxx
  GenNoiseElectrons/GenNoiseElectrons.cxx
  CalcStats/CalcStats.cxx
)

Set(NO_DICT_SRCS
#  LxTBMatEffs.cxx
#  LxTBNaive.cxx
)

set(LINKDEF LxLinkDef.h)
Set(LIBRARY_NAME LxTrack)
Set(DEPENDENCIES
    CbmBase CbmData Base CbmMuchBase Littrack
# CbmMuchSim CbmKF CbmLitTrack CbmSrt ????
)

GENERATE_LIBRARY()

