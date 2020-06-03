Set(INCLUDE_DIRECTORIES
  ${CBMDATA_DIR}
  ${CBMDATA_DIR}/trd
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



#Set(CbmTrdDetectorIdSources
#  ${CBMDATA_DIR}/trd/CbmTrdAddress.cxx
#  _GTestCbmTrdDetectorId.cxx
#)

#CreateGTestExeAndAddTest(_GTestCbmTrdDetectorId "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
#                         "${CbmTrdDetectorIdSources}" "${DEPENDENCIES}" "")


Set(CbmTrdDigiSources
  ${CBMDATA_DIR}/trd/CbmTrdDigi.cxx
  _GTestCbmTrdDigi.cxx
)

CreateGTestExeAndAddTest(_GTestCbmTrdDigi "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmTrdDigiSources}" "${DEPENDENCIES}" "")

#Set(CbmTrdAddressSources
#  ${CBMDATA_DIR}/trd/CbmTrdAddress.cxx
#  _GTestCbmTrdAddress.cxx
#)

#CreateGTestExeAndAddTest(_GTestCbmTrdAddress "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
#                         "${CbmTrdAddressSources}" "${DEPENDENCIES}" "_GTestTrdDigi")

Set(CbmTrdPointSources
  ${CBMDATA_DIR}/trd/CbmTrdPoint.cxx
  _GTestCbmTrdPoint.cxx
)

CreateGTestExeAndAddTest(_GTestCbmTrdPoint "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmTrdPointSources}" "${DEPENDENCIES}" "_GTestTrdDigi")

Set(CbmTrdHitSources
  ${CBMDATA_DIR}/trd/CbmTrdHit.cxx
  _GTestCbmTrdHit.cxx
)

CreateGTestExeAndAddTest(_GTestCbmTrdHit "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmTrdHitSources}" "${DEPENDENCIES}" "_GTestTrdPoint")
