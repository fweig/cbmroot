set(INCLUDE_DIRECTORIES
   ${CMAKE_CURRENT_SOURCE_DIR}
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
  
  
Set(CbmPsdDigiSources
  ${CBMDATA_DIR}/psd/CbmPsdDigi.cxx
  _GTestCbmPsdDigi.cxx
)

CreateGTestExeAndAddTest(_GTestCbmPsdDigi "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmPsdDigiSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "")

Set(CbmPsdAddressSources
  ${CBMDATA_DIR}/psd/CbmPsdAddress.cxx
  _GTestCbmPsdAddress.cxx
)

CreateGTestExeAndAddTest(_GTestCbmPsdAddress "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmPsdAddressSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestPsdDigi")

Set(CbmPsdPointSources
  ${CBMDATA_DIR}/psd/CbmPsdPoint.cxx
  _GTestCbmPsdPoint.cxx
)

CreateGTestExeAndAddTest(_GTestCbmPsdPoint "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmPsdPointSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestPsdAddress")
Set(CbmPsdHitSources
  ${CBMDATA_DIR}/psd/CbmPsdHit.cxx
  _GTestCbmPsdHit.cxx
)

CreateGTestExeAndAddTest(_GTestCbmPsdHit "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmPsdHitSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestPsdPoint")
