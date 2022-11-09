set(INCLUDE_DIRECTORIES
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/..
  )
 
set(PUB_DEPS
  CbmData
  Gtest
  GtestMain
  
  FairRoot::Base
  )
  
Set(CbmPsdDigiSources
  _GTestCbmPsdDigi.cxx
)

CreateGTestExeAndAddTest(_GTestCbmPsdDigi "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmPsdDigiSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "")

Set(CbmPsdAddressSources
  _GTestCbmPsdAddress.cxx
)

CreateGTestExeAndAddTest(_GTestCbmPsdAddress "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmPsdAddressSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestPsdDigi")

Set(CbmPsdPointSources
  _GTestCbmPsdPoint.cxx
)

CreateGTestExeAndAddTest(_GTestCbmPsdPoint "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmPsdPointSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestPsdAddress")
Set(CbmPsdHitSources
  _GTestCbmPsdHit.cxx
)

CreateGTestExeAndAddTest(_GTestCbmPsdHit "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmPsdHitSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestPsdPoint")
