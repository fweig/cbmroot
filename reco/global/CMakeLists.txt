# Create a library called "libGlobal" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(INCLUDE_DIRECTORIES
${CMAKE_CURRENT_SOURCE_DIR}

${CBMROOT_SOURCE_DIR}/reco/base

${CBMDATA_DIR}
${CBMDATA_DIR}/sts
${CBMDATA_DIR}/trd
${CBMDATA_DIR}/tof
${CBMDATA_DIR}/global

${CBMROOT_SOURCE_DIR}/KF
${CBMROOT_SOURCE_DIR}/KF/Interface
)

Include_Directories( ${INCLUDE_DIRECTORIES})

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES} 
)

Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${KFParticle_LIB_DIR}
${FAIRROOT_LIBRARY_DIR}
${ROOT_LIBRARY_DIR}
${Boost_LIBRARY_DIRS}
)
 
link_directories( ${LINK_DIRECTORIES})

set(SRCS
#CbmEcalTrackingMuch.cxx
CbmFindGlobalTracks.cxx    
#CbmFindGlobalTracksQa.cxx  
CbmFindPrimaryVertex.cxx   
CbmFindPrimaryVertexEvents.cxx   
CbmFitGlobalTracks.cxx 
CbmFitGlobalTracksQa.cxx 
CbmGlobalTrackFitterIdeal.cxx 
CbmPVFinderIdeal.cxx       
CbmTrackMergerIdeal.cxx    
#CbmTofMergerIdeal.cxx 
)


set(LINKDEF CbmGlobalLinkDef.h)
Set(LIBRARY_NAME CbmGlobal)
Set(DEPENDENCIES
    KF CbmRecoBase CbmData Base 
)

GENERATE_LIBRARY()

