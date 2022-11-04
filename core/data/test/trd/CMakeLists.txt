set(INCLUDE_DIRECTORIES
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CBMDATA_DIR}/trd
   ${CMAKE_CURRENT_SOURCE_DIR}/..
  )

set(PUB_DEPS
  CbmData
  Gtest
  GtestMain
  ROOT::Physics
  )
  
set(PVT_DEPS
  CbmTrdBase
  NicaCbmFormat
  ROOT::Core
  ROOT::Matrix
  ROOT::EG
  )
  
set(INT_DEPS  
  FairRoot::Base
  external::fles_ipc
  xpu
  VMCLibrary
  )
  
  
#Set(CbmTrdDetectorIdSources
#  ${CBMDATA_DIR}/trd/CbmTrdAddress.cxx
#  _GTestCbmTrdDetectorId.cxx
#)

#CreateGTestExeAndAddTest(_GTestCbmTrdDetectorId "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
#                         "${CbmTrdDetectorIdSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "")


Set(CbmTrdDigiSources
  ${CBMDATA_DIR}/trd/CbmTrdDigi.cxx
  _GTestCbmTrdDigi.cxx
)

CreateGTestExeAndAddTest(_GTestCbmTrdDigi "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmTrdDigiSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "")

#Set(CbmTrdAddressSources
#  ${CBMDATA_DIR}/trd/CbmTrdAddress.cxx
#  _GTestCbmTrdAddress.cxx
#)

#CreateGTestExeAndAddTest(_GTestCbmTrdAddress "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
#                         "${CbmTrdAddressSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestTrdDigi")

Set(CbmTrdPointSources
  ${CBMDATA_DIR}/trd/CbmTrdPoint.cxx
  _GTestCbmTrdPoint.cxx
)

CreateGTestExeAndAddTest(_GTestCbmTrdPoint "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmTrdPointSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestTrdDigi")

Set(CbmTrdHitSources
  ${CBMDATA_DIR}/trd/CbmTrdHit.cxx
  _GTestCbmTrdHit.cxx
)

CreateGTestExeAndAddTest(_GTestCbmTrdHit "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmTrdHitSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestTrdPoint")
