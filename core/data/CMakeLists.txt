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
  ${CMAKE_CURRENT_SOURCE_DIR}/psd
  ${CMAKE_CURRENT_SOURCE_DIR}/global
  )


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
  CbmRawEvent.cxx
  CbmMCTrack.cxx
  CbmMCEventInfo.cxx
  CbmMCEventList.cxx
  CbmLink.cxx
  CbmMatch.cxx
  CbmTrackMatchNew.cxx
  CbmTsEventHeader.cxx

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
  tof/CbmTofDetectorId_v21a.cxx
  tof/CbmTofAddress.cxx
  tof/CbmTofDigi.cxx
  tof/CbmTofTrack.cxx
  tof/CbmTofTracklet.cxx
  tof/CbmTofTrackletParam.cxx

  psd/CbmPsdDigi.cxx
  psd/CbmPsdDsp.cxx
  psd/CbmPsdHit.cxx
  psd/CbmPsdPoint.cxx
  psd/CbmPsdAddress.cxx
  psd/CbmPsdMCbmHit.cxx

  global/CbmTzdDigi.cxx
  global/CbmGlobalTrack.cxx
  global/CbmVertex.cxx
  global/CbmTrackParam.cxx

  raw/AccDataSis18.cxx
  raw/StsXyterMessage.cxx
  raw/gDpbMessv100.cxx
  raw/CriGet4Mess001.cxx
  raw/TimesliceMetaData.cxx
  raw/PsdGbtReader-v0.00.cxx
  raw/PsdGbtReader-v1.00.cxx
)

set(NO_DICT_SRCS
  CbmDefs.cxx

  # STAR sub-event writer source code
  tof/etof/star_rhicf.c
  )


# Mask warning from file provided by STAR
SET_SOURCE_FILES_PROPERTIES(tof/etof/star_rhicf.c PROPERTIES COMPILE_FLAGS -Wno-pointer-sign)


list(APPEND HEADERS base/CbmDigiData.h global/CbmDigiEvent.h global/CbmDigiTimeslice.h
sts/CbmStsDigiData.h much/CbmMuchDigiData.h rich/CbmRichDigiData.h trd/CbmTrdDigiData.h
tof/CbmTofDigiData.h global/CbmTzdDigiData.h psd/CbmPsdDigiData.h)

set(LIBRARY_NAME CbmData)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  FairRoot::Base
  ROOT::Core
  ROOT::Matrix
  ROOT::Physics
  external::fles_ipc
  xpu
  )

set(PRIVATE_DEPENDENCIES
  ROOT::EG
  FairLogger::FairLogger
  ${VMCLIB}
  )

generate_cbm_library()

# Install file which has no corresponding source file
Install(FILES
        CbmDefs.h
        base/CbmDigiData.h base/CbmDigiVector.h
        much/CbmMuchDigiData.h
        psd/CbmPsdDigiData.h
        rich/CbmRichDigiData.h rich/CbmRichRingLight.h
        sts/CbmStsDigiData.h
        trd/CbmTrdDigiData.h
        tof/CbmTofDigiData.h
        global/CbmTzdDigiData.h
        global/CbmDigiEvent.h global/CbmDigiTimeslice.h
        raw/bitmask_operators.hpp raw/StsXyterFinalHit.h raw/PsdGbtDataFormat-v0.00.h raw/PsdGbtDataFormat-v1.00.h
        DESTINATION include
       )


