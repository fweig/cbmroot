# Create a library called "libCbmData" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/base
  ${CMAKE_CURRENT_SOURCE_DIR}/raw
  ${CMAKE_CURRENT_SOURCE_DIR}/mvd
  ${CMAKE_CURRENT_SOURCE_DIR}/sts
  ${CMAKE_CURRENT_SOURCE_DIR}/much
  ${CMAKE_CURRENT_SOURCE_DIR}/trd
  ${CMAKE_CURRENT_SOURCE_DIR}/tof
  ${CMAKE_CURRENT_SOURCE_DIR}/rich
#  ${CMAKE_CURRENT_SOURCE_DIR}/ecal
  ${CMAKE_CURRENT_SOURCE_DIR}/psd
  ${CMAKE_CURRENT_SOURCE_DIR}/global
)
include_directories( ${INCLUDE_DIRECTORIES})

set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES}
)
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}

)
link_directories( ${LINK_DIRECTORIES})

set(SRCS
  CbmAddress.cxx
  CbmTimeSlice.cxx
  CbmEvent.cxx
  CbmEventStore.cxx
  CbmHit.cxx
  CbmPixelHit.cxx
  CbmStripHit.cxx
  CbmTrack.cxx
  CbmTrackMatch.cxx
  CbmCluster.cxx
  CbmModuleList.cxx
  CbmErrorMessage.cxx

  CbmMCTrack.cxx
  CbmMCEventInfo.cxx
  CbmMCEventList.cxx

  CbmLink.cxx
  CbmMatch.cxx
  CbmTrackMatchNew.cxx
  
  base/CbmDigiBranchBase.cxx
  base/CbmDigiContainer.cxx

  mvd/CbmMvdCluster.cxx
  mvd/CbmMvdDetectorId.cxx
  mvd/CbmMvdDigi.cxx
  mvd/CbmMvdHit.cxx
  mvd/CbmMvdPoint.cxx
  mvd/CbmMvdHitMatch.cxx

  sts/CbmStsAddress.cxx
  sts/CbmStsCluster.cxx
  sts/CbmStsDigi.cxx
  sts/CbmStsHit.cxx
  sts/CbmStsPoint.cxx
  sts/CbmStsTrack.cxx

  rich/CbmRichRing.cxx
  rich/CbmRichHit.cxx
  rich/CbmRichPoint.cxx
  rich/CbmRichTrbDigi.cxx
  rich/CbmRichDigi.cxx

  much/CbmMuchPixelHit.cxx
  much/CbmMuchPoint.cxx
  much/CbmMuchCluster.cxx
  much/CbmMuchDigi.cxx
  much/CbmMuchBeamTimeDigi.cxx
  much/CbmMuchDigiMatch.cxx
  much/CbmMuchTrack.cxx
  much/CbmMuchAddress.cxx

  trd/CbmTrdHit.cxx
  trd/CbmTrdTrack.cxx
  trd/CbmTrdPoint.cxx
  trd/CbmTrdDigi.cxx
  trd/CbmTrdAddress.cxx
  trd/CbmTrdCluster.cxx
  trd/CbmTrdRawMessageSpadic.cxx

  tof/CbmTofHit.cxx
  tof/CbmTofPoint.cxx
  tof/CbmTofDetectorId.cxx
  tof/CbmTofDetectorId_v07a.cxx
  tof/CbmTofDetectorId_v12b.cxx
  tof/CbmTofDetectorId_v14a.cxx
  tof/CbmTofAddress.cxx
  tof/CbmTofDigi.cxx
  tof/CbmTofTracklet.cxx
  tof/CbmTofTrackletParam.cxx

#  ecal/CbmEcalHit.cxx
#  ecal/CbmEcalPoint.cxx
#  ecal/CbmEcalPointLite.cxx
#  ecal/CbmEcalHitFastMC.cxx
#  ecal/CbmEcalCell.cxx
#  ecal/CbmEcalMaximum.cxx
#  ecal/CbmEcalCluster.cxx
#  ecal/CbmEcalRecParticle.cxx
#  ecal/CbmEcalIdParticle.cxx

  psd/CbmPsdDigi.cxx
  psd/CbmPsdHit.cxx
  psd/CbmPsdPoint.cxx
  psd/CbmPsdAddress.cxx
  psd/CbmPsdMCbmHit.cxx

  global/CbmGlobalTrack.cxx
  global/CbmVertex.cxx
  global/CbmTofTrack.cxx
  global/CbmTrackParam.cxx

#  beamtime/CbmAuxDigi.cxx
#  beamtime/CbmTbEvent.cxx
#  beamtime/fhodo/CbmFiberHodoDigi.cxx
#  beamtime/fhodo/CbmFiberHodoCluster.cxx
#  beamtime/fhodo/CbmFiberHodoAddress.cxx

#  raw/CbmRawMessage.cxx
#  raw/CbmSpadicRawMessage.cxx
#  raw/CbmTrdRawMessage.cxx
#  raw/CbmNxyterRawMessage.cxx
#  raw/CbmNxyterRawSyncMessage.cxx
#  raw/CbmStsxyterRawHitMessage.cxx
#  raw/CbmTrbRawMessage.cxx
  raw/StsXyterMessage.cxx
  raw/gDpbMessv100.cxx
  raw/TimesliceMetaData.cxx
  raw/PsdGbtReader.cxx
)

Set(NO_DICT_SRCS
  CbmDefs.cxx

  # STAR sub-event writer source code
  tof/etof/star_rhicf.c
)

# Mask warning from file provided by STAR
SET_SOURCE_FILES_PROPERTIES(tof/etof/star_rhicf.c PROPERTIES COMPILE_FLAGS -Wno-pointer-sign)

set(LINKDEF DataLinkDef.h)
set(LIBRARY_NAME CbmData)
set(DEPENDENCIES Base)

GENERATE_LIBRARY()

# Install file which has no corresponding source file
Install(FILES 
        CbmDefs.h rich/CbmRichRingLight.h base/CbmDigiVector.h
        raw/bitmask_operators.hpp raw/PsdGbtDataFormat.h
        DESTINATION include
       )
