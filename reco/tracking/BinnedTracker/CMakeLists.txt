Set(INCLUDE_DIRECTORIES
  ${CBMROOT_SOURCE_DIR}/reco/tracking/BinnedTracker

  ${CBMBASE_DIR} 

  ${CBMDATA_DIR}
  ${CBMDATA_DIR}/sts
  ${CBMDATA_DIR}/much
  ${CBMDATA_DIR}/trd
  ${CBMDATA_DIR}/tof

  ${CBMDETECTORBASE_DIR}/sts

  ${CBMDETECTORBASE_DIR}/much

  ${CBMROOT_SOURCE_DIR}/run

  ${CBMROOT_SOURCE_DIR}/sim/transport
  ${CBMROOT_SOURCE_DIR}/sim/transport/base
  ${CBMROOT_SOURCE_DIR}/sim/transport/steer
  ${CBMROOT_SOURCE_DIR}/sim/transport/geosetup
)

Include_Directories( ${INCLUDE_DIRECTORIES})

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES}
  #${Boost_INCLUDE_DIRS}
)

Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

Set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
)
 
Link_Directories( ${LINK_DIRECTORIES})

Set(SRCS
  CbmBinnedTrackerConfigurator.cxx
  CbmBinnedTrackerTask.cxx
  CbmBinnedTrackerQa.cxx
  CbmBinnedTrackerQaNew.cxx
  ContFactory.cxx
  Settings.cxx
  CbmTrackParam2.cxx
)

Set(NO_DICT_SRCS
  GeoReader.cxx
  HitReader.cxx
  Station.cxx
  Station3D.cxx
  Station4D.cxx
  Tracker.cxx
)

set(LINKDEF CbmBinnedTrackerLinkDef.h)
Set(LIBRARY_NAME CbmBinnedTracker)
Set(DEPENDENCIES
    CbmBase CbmData CbmMuchBase CbmStsBase Base CbmSimSteer
)

GENERATE_LIBRARY()

Install(FILES Tracker.h
              Station.h
              Station3D.h
              Station4D.h
              Bins.h
              HitReader.h
        DESTINATION include
       )
