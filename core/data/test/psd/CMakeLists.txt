Set(INCLUDE_DIRECTORIES
  ${CBMDATA_DIR}
  ${CBMDATA_DIR}/psd
  ${CBMDATA_DIR}/test
  ${BASE_INCLUDE_DIRECTORIES}
)

MESSAGE("FAIRROOT_LIBRARY_DIR: ${FAIRROOT_LIBRARY_DIR}")
Set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
  ${SIMPATH}/lib
)


Set(DEPENDENCIES
  ${ROOT_LIBRARIES}
  ${GTEST_BOTH_LIBRARIES}
  FairTools
  CbmData
)

If(FAIRLOGGER_FOUND)
  set(DEPENDENCIES
      ${DEPENDENCIES}
      FairLogger
     )
EndIf()

if (CMAKE_SYSTEM_NAME MATCHES Linux)
  set(DEPENDENCIES ${DEPENDENCIES} rt)
endif()

Set(CbmPsdDigiSources
  ${CBMDATA_DIR}/psd/CbmPsdDigi.cxx
  _GTestCbmPsdDigi.cxx
)

CreateGTestExeAndAddTest(_GTestCbmPsdDigi "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmPsdDigiSources}" "${DEPENDENCIES}" "")

Set(CbmPsdAddressSources
  ${CBMDATA_DIR}/psd/CbmPsdAddress.cxx
  _GTestCbmPsdAddress.cxx
)

CreateGTestExeAndAddTest(_GTestCbmPsdAddress "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmPsdAddressSources}" "${DEPENDENCIES}" "_GTestPsdDigi")

Set(CbmPsdPointSources
  ${CBMDATA_DIR}/psd/CbmPsdPoint.cxx
  _GTestCbmPsdPoint.cxx
)

CreateGTestExeAndAddTest(_GTestCbmPsdPoint "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmPsdPointSources}" "${DEPENDENCIES}" "_GTestPsdAddress")
Set(CbmPsdHitSources
  ${CBMDATA_DIR}/psd/CbmPsdHit.cxx
  _GTestCbmPsdHit.cxx
)

CreateGTestExeAndAddTest(_GTestCbmPsdHit "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmPsdHitSources}" "${DEPENDENCIES}" "_GTestPsdPoint")
