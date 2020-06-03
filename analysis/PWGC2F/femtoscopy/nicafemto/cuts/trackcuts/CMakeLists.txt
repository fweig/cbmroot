# Create a library called "" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.
Set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${CMAKE_SOURCE_FAIRMCSTACK}
${NICAFEMTO_CUTS}
${NICAFEMTO_FEATURES}
${NICAFEMTO_DATA}
${NICA_CBM_FORMAT}
${KF_HEADERS}
${NICA_CBM}/helpers
${NICA_CBM}/cuts/trackcuts
)

Set(SYSTEM_INCLUDE_DIRECTORIES
 ${ROOT_INCLUDE_DIR} 
  ${CLHEP_INCLUDE_DIR}
 ${Boost_INCLUDE_DIRS}
)

Set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
  ${Vc_LIB_DIR}
  ${KFParticle_LIB_DIR}
)

Include_Directories(${INCLUDE_DIRECTORIES})
Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})
 
Link_Directories(${LINK_DIRECTORIES})

# List of source files
set(SRCS
	CbmStsTrackHitsCut.cxx
	CbmHasTofTrackCut.cxx
	CbmMCPointsCuts.cxx
	CbmTofTrackCuts.cxx
	CbmStsFarDetectorHit.cxx
	CbmStsStationsCut.cxx
)

Set(HEADERS )
Set(LINKDEF NicaCbmTrackCutLinkDef.h)
Set(LIBRARY_NAME NicaCbmTrackCut)
Set(DEPENDENCIES NicaFeatures NicaDataFormat NicaCut
CbmData NicaCbmFormat)

GENERATE_LIBRARY()

Add_Dependencies(NicaCbmTrackCut NICAFEMTO)

