# Create a library called "libLxTrack" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed
# here.

Set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}

  ${CBMBASE_DIR} 

  ${CBMDATA_DIR} 
  ${CBMDATA_DIR}/base
  ${CBMDATA_DIR}/sts
  ${CBMDATA_DIR}/mvd
  ${CBMDATA_DIR}/tof
  ${CBMDATA_DIR}/much
  ${CBMDATA_DIR}/trd
  ${CBMDATA_DIR}/global

  ${CBMDETECTORBASE_DIR}/much

  ${CBMROOT_SOURCE_DIR}/run

  ${CBMROOT_SOURCE_DIR}/reco/littrack/std
  ${CBMROOT_SOURCE_DIR}/reco/littrack/cbm

  ${CBMROOT_SOURCE_DIR}/reco/KF
  ${CBMROOT_SOURCE_DIR}/reco/KF/Interface
)

Include_Directories( ${INCLUDE_DIRECTORIES})

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES}
)

Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

Set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
)
 
Link_Directories( ${LINK_DIRECTORIES})

Set(SRCS
  CbmGlobalTrackingTask.cxx
  CbmGlobalTrackingQA.cxx
)

Set(NO_DICT_SRCS
  CbmGlobalTrackingTof.cxx
)

set(LINKDEF GlobalLinkDef.h)
Set(LIBRARY_NAME CbmGlobalTracking)
Set(DEPENDENCIES
    CbmBase CbmData Base CbmMuchBase 
#   CbmLitTrack CbmKF CbmRun ????
)

GENERATE_LIBRARY()
