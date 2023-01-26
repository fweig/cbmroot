Macro(CreateGTestExeAndAddTest _testname _includeDirs _linkDirs _sources _pub_dep _pvt_dep _int_dep _testdepend)

  add_executable(${_testname} ${_sources})
  target_link_libraries(${_testname} PUBLIC ${_pub_dep} PRIVATE ${_pvt_dep} INTERFACE ${_int_dep} )
  target_include_directories(${_testname} PUBLIC ${_includeDirs})
  Gen_Exe_Script(${_testname})
  string(REPLACE ${PROJECT_SOURCE_DIR}
         ${PROJECT_BINARY_DIR} new_path ${CMAKE_CURRENT_SOURCE_DIR}
        )
  Add_Test(${_testname} ${new_path}/${_testname}.sh)
  If(NOT ${_testdepend} STREQUAL "")
    Set_Tests_Properties(${_testname} PROPERTIES DEPENDS ${_testdepend})
  EndIf()
EndMacro(CreateGTestExeAndAddTest)


add_subdirectory(global)
add_subdirectory(psd)
add_subdirectory(trd)

set(PVT_DEPS
  CbmData
  Gtest
  GtestMain

  FairRoot::Base

  ROOT::Physics
  ROOT::Core
  ROOT::Matrix

  Boost::serialization
  )



if (CMAKE_SYSTEM_NAME MATCHES Linux)
  set(DEPENDENCIES ${DEPENDENCIES} rt)
endif()



Set(CbmLinkSources
  _GTestCbmLink.cxx
)

CreateGTestExeAndAddTest(_GTestCbmLink "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmLinkSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "")

Set(CbmMatchSources
  _GTestCbmMatch.cxx
)

CreateGTestExeAndAddTest(_GTestCbmMatch "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmMatchSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestCbmLink")

Set(CbmClusterSources
  _GTestCbmCluster.cxx
)

CreateGTestExeAndAddTest(_GTestCbmCluster "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmClusterSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestCbmDigi")

Set(CbmHitSources
  _GTestCbmHit.cxx
)

CreateGTestExeAndAddTest(_GTestCbmHit "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmHitSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestCbmCluster")

Set(CbmPixelHitSources
  _GTestCbmPixelHit.cxx
)

CreateGTestExeAndAddTest(_GTestCbmPixelHit "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmPixelHitSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestCbmHit")

Set(CbmStripHitSources
  _GTestCbmStripHit.cxx
)

CreateGTestExeAndAddTest(_GTestCbmStripHit "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmStripHitSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestCbmPixelHit")


Set(CbmEventSources
  _GTestCbmEvent.cxx
)

CreateGTestExeAndAddTest(_GTestCbmEvent "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmEventSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestCbmStripHit")


Set(CbmConstantsSources
  _GTestCbmConstants.cxx
)

CreateGTestExeAndAddTest(_GTestCbmConstants "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmConstantsSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestCbmEvent")

Set(CbmErrorMessageSources
  _GTestCbmErrorMessage.cxx
)

CreateGTestExeAndAddTest(_GTestCbmErrorMessage "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmErrorMessageSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestCbmConstants")


Set(CbmAddressSources
  _GTestCbmAddress.cxx
)

CreateGTestExeAndAddTest(_GTestCbmAddress "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmAddressSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestCbmErrorMessage")


Set(CbmStsDigiSources
  sts/_GTestCbmStsDigi.cxx
)

CreateGTestExeAndAddTest(_GTestCbmStsDigi "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmStsDigiSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestCbmAddress")

Set(CbmTofDigiSources
  tof/_GTestCbmTofDigi.cxx
)

CreateGTestExeAndAddTest(_GTestCbmTofDigi "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmTofDigiSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestStsDigi")

Set(CbmRichDigiSources
  rich/_GTestCbmRichDigi.cxx
)

CreateGTestExeAndAddTest(_GTestCbmRichDigi "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmRichDigiSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestTofDigi")


Set(CbmMuchDigiSources
  much/_GTestCbmMuchDigi.cxx
)

CreateGTestExeAndAddTest(_GTestCbmMuchDigi "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmMuchDigiSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTesRichDigi")

Set(CbmMuchBeamTimeDigiSources
  much/_GTestCbmMuchBeamTimeDigi.cxx
)

CreateGTestExeAndAddTest(_GTestCbmMuchBeamTimeDigi "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmMuchBeamTimeDigiSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestMuchDigi")

Set(CbmBmonDigiSources
  bmon/_GTestCbmBmonDigi.cxx
)

CreateGTestExeAndAddTest(_GTestCbmBmonDigi "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmBmonDigiSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestCbmMuchBeamTimeDigi")

