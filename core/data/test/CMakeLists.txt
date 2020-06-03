Macro(CreateGTestExeAndAddTest _testname _includeDirs _linkDirs _sources _dependencies _testdepend)

  Include_Directories(SYSTEM "${_includeDirs}")
  Link_Directories(${_linkDirs})

  Add_Executable(${_testname} ${_sources})
  Target_Link_Libraries(${_testname} "${_dependencies}")
  Gen_Exe_Script(${_testname})
  string(REPLACE ${PROJECT_SOURCE_DIR}
         ${PROJECT_BINARY_DIR} new_path ${CMAKE_CURRENT_SOURCE_DIR}
        )
  Add_Test(${_testname} ${new_path}/${_testname}.sh)
  If(NOT ${_testdepend} STREQUAL "")
    Set_Tests_Properties(${_testname} PROPERTIES DEPENDS ${_testdepend})
  EndIf()
EndMacro(CreateGTestExeAndAddTest)

add_subdirectory(psd)
add_subdirectory(trd)

Set(INCLUDE_DIRECTORIES
  ${CBMBASE_DIR}
  ${CBMDATA_DIR}
  ${CBMDATA_DIR}/global
  ${CBMDATA_DIR}/sts
  ${CBMDATA_DIR}/tof
  ${CBMDATA_DIR}/rich
  ${CBMDATA_DIR}/much
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
  Base
  ParBase
  GeoBase
  MbsAPI
)

Set(SPECIAL_DEPENDENCIES
  ${ROOT_LIBRARIES} 
  ${GTEST_BOTH_LIBRARIES}
  FairTools
)

If(FAIRLOGGER_FOUND)
  set(DEPENDENCIES
      ${DEPENDENCIES}
      FairLogger
     )
  set(SPECIAL_DEPENDENCIES
      ${SPECIAL_DEPENDENCIES}
      FairLogger
     )
EndIf()

if (CMAKE_SYSTEM_NAME MATCHES Linux)
  set(DEPENDENCIES ${DEPENDENCIES} rt)
endif()

Set(CbmLinkSources
  ${CBMDATA_DIR}/CbmLink.cxx
  _GTestCbmLink.cxx
)

CreateGTestExeAndAddTest(_GTestCbmLink "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmLinkSources}" "${DEPENDENCIES}" "")

Set(CbmMatchSources
  ${CBMDATA_DIR}/CbmMatch.cxx
  _GTestCbmMatch.cxx
)

CreateGTestExeAndAddTest(_GTestCbmMatch "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmMatchSources}" "${DEPENDENCIES}" "_GTestCbmLink")

Set(CbmClusterSources
  ${CBMDATA_DIR}/CbmCluster.cxx
  _GTestCbmCluster.cxx
)

CreateGTestExeAndAddTest(_GTestCbmCluster "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmClusterSources}" "${DEPENDENCIES}" "_GTestCbmDigi")

Set(CbmHitSources
  ${CBMDATA_DIR}/CbmHit.cxx
  _GTestCbmHit.cxx
)

CreateGTestExeAndAddTest(_GTestCbmHit "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmHitSources}" "${DEPENDENCIES}" "_GTestCbmCluster")

Set(CbmPixelHitSources
  ${CBMDATA_DIR}/CbmPixelHit.cxx
  _GTestCbmPixelHit.cxx
)

CreateGTestExeAndAddTest(_GTestCbmPixelHit "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmPixelHitSources}" "${DEPENDENCIES}" "_GTestCbmHit")

Set(CbmStripHitSources
  ${CBMDATA_DIR}/CbmStripHit.cxx
  _GTestCbmStripHit.cxx
)

CreateGTestExeAndAddTest(_GTestCbmStripHit "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmStripHitSources}" "${DEPENDENCIES}" "_GTestCbmPixelHit")


Set(CbmEventSources
  ${CBMDATA_DIR}/CbmEvent.cxx
  _GTestCbmEvent.cxx
)

CreateGTestExeAndAddTest(_GTestCbmEvent "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmEventSources}" "${DEPENDENCIES}" "_GTestCbmStripHit")


Set(CbmConstantsSources
  _GTestCbmConstants.cxx
)

CreateGTestExeAndAddTest(_GTestCbmConstants "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmConstantsSources}" "${GTEST_BOTH_LIBRARIES};-pthread" "_GTestCbmEvent")

Set(CbmErrorMessageSources
  ${CBMDATA_DIR}/CbmErrorMessage.cxx
  _GTestCbmErrorMessage.cxx
)

CreateGTestExeAndAddTest(_GTestCbmErrorMessage "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmErrorMessageSources}" "${DEPENDENCIES};-pthread" "_GTestCbmConstants")


Set(CbmAddressSources
  ${CBMDATA_DIR}/CbmAddress.cxx
  _GTestCbmAddress.cxx
)  

CreateGTestExeAndAddTest(_GTestCbmAddress "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmAddressSources}" "${DEPENDENCIES}" "_GTestCbmErrorMessage")

Set(CbmVertexSources
  ${CBMDATA_DIR}/global/CbmVertex.cxx
  global/_GTestCbmVertex.cxx
)

CreateGTestExeAndAddTest(_GTestCbmVertex "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmVertexSources}" "${DEPENDENCIES}" "_GTestCbmAddress")


#Set(CbmModuleListSources
#  ${CBMDATA_DIR}/CbmModuleList.cxx
#  _GTestCbmModuleList.cxx
#)  
#
#CreateGTestExeAndAddTest(_GTestCbmModuleList "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
#                         "${CbmModuleListSources}" "${DEPENDENCIES}" "_GTestCbmVertex")


Set(CbmStsDigiSources
  ${CBMDATA_DIR}/sts/CbmStsDigi.cxx
  sts/_GTestCbmStsDigi.cxx
)

CreateGTestExeAndAddTest(_GTestCbmStsDigi "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmStsDigiSources}" "${DEPENDENCIES}" "_GTestCbmVertex")

Set(CbmTofDigiSources
  ${CBMDATA_DIR}/tof/CbmTofDigi.cxx
  tof/_GTestCbmTofDigi.cxx
)

CreateGTestExeAndAddTest(_GTestCbmTofDigi "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmTofDigiSources}" "${DEPENDENCIES}" "_GTestStsDigi")

Set(CbmRichDigiSources
  ${CBMDATA_DIR}/rich/CbmRichDigi.cxx
  rich/_GTestCbmRichDigi.cxx
)

CreateGTestExeAndAddTest(_GTestCbmRichDigi "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmRichDigiSources}" "${DEPENDENCIES}" "_GTestTofDigi")


Set(CbmMuchDigiSources
  ${CBMDATA_DIR}/much/CbmMuchDigi.cxx
  much/_GTestCbmMuchDigi.cxx
)

CreateGTestExeAndAddTest(_GTestCbmMuchDigi "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmMuchDigiSources}" "${DEPENDENCIES}" "_GTesRichDigi")

Set(CbmMuchBeamTimeDigiSources
  ${CBMDATA_DIR}/much/CbmMuchBeamTimeDigi.cxx
  much/_GTestCbmMuchBeamTimeDigi.cxx
)

CreateGTestExeAndAddTest(_GTestCbmMuchBeamTimeDigi "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmMuchBeamTimeDigiSources}" "${DEPENDENCIES}" "_GTestMuchDigi")

#GENERATE_ROOT_TEST_SCRIPT(${CMAKE_SOURCE_DIR}/test/cbmdata/load_all_libs.C)
#  Add_Test(load_all_libs
#          ${CMAKE_BINARY_DIR}/test/cbmdata/load_all_libs.sh)
#  Set_Tests_Properties(load_all_libs PROPERTIES TIMEOUT "30")
#  Set_Tests_Properties(load_all_libs PROPERTIES PASS_REGULAR_EXPRESSION "Macro finished succesfully")
                         
#Set(CbmTrackSources
#  ${CBMDATA_DIR}/CbmTrack.cxx
#  _GTestCbmTrack.cxx
#)  

#CreateGTestExeAndAddTest(_GTestCbmTrack "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
#                         "${CbmTrackSources}" "${DEPENDENCIES}" "_GTestCbmConstants")

