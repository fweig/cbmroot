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
${NICA_CBM}/cuts
${NICA_CBM}/cuts/rawcuts/trackcuts
${NICA_CBM}/cuts/rawcuts/paircuts
${NICA_CBM}/cuts/rawcuts
${NICA_CBM}/cuts/commoncuts/pair
${NICA_CBM}/cuts/commoncuts/track
${NICA_CBM}/cuts/anatreecuts/pair
${NICA_CBM}/cuts/anatreecuts/track
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
    rawcuts/paircuts/CbmAvXYCut.cxx
    rawcuts/paircuts/CbmSailorCowboyCut.cxx
    rawcuts/trackcuts/CbmHasTofTrackCut.cxx
    rawcuts/trackcuts/CbmMCPointsCuts.cxx
    rawcuts/trackcuts/CbmStsFarDetectorHit.cxx
    rawcuts/trackcuts/CbmStsStationsCut.cxx
    rawcuts/trackcuts/CbmStsTrackHitsCut.cxx
    rawcuts/trackcuts/CbmTofTrackCuts.cxx
    commoncuts/pair/CbmDeltaPhiDeltaThetaCut.cxx
    commoncuts/pair/CbmStsExitSepCut.cxx
    commoncuts/track/CbmTofM2PolyCut.cxx
    anatreecuts/pair/CbmBasicFemtoPairCut.cxx
    anatreecuts/track/CbmBasicTrackCuts.cxx
    anatreecuts/track/CbmNHitsCut.cxx
    anatreecuts/track/CbmTofCut.cxx
)

Set(HEADERS )
Set(LINKDEF NicaCbmCutsLinkDef.h)
Set(LIBRARY_NAME NicaCbmCuts)
Set(DEPENDENCIES NicaFeatures NicaDataFormat NicaCut
CbmData NicaCbmFormat)

GENERATE_LIBRARY()

Add_Dependencies(NicaCbmCuts NICAFEMTO)