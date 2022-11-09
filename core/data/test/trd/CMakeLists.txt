set(INCLUDE_DIRECTORIES
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/..
  )

set(PVT_DEPS
  CbmData
  Gtest
  GtestMain
  ROOT::Core
  FairRoot::Base
  )
  
  
Set(CbmTrdDigiSources
  _GTestCbmTrdDigi.cxx
)

CreateGTestExeAndAddTest(_GTestCbmTrdDigi "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmTrdDigiSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "")

Set(CbmTrdPointSources
  _GTestCbmTrdPoint.cxx
)

CreateGTestExeAndAddTest(_GTestCbmTrdPoint "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmTrdPointSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestTrdDigi")

Set(CbmTrdHitSources
  _GTestCbmTrdHit.cxx
)

CreateGTestExeAndAddTest(_GTestCbmTrdHit "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmTrdHitSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestTrdPoint")
