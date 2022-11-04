Macro(CreateGTestExeAndAddTest _testname _includeDirs _linkDirs _sources _pub_dep _pvt_dep _int_dep _testdepend)

  add_executable(${_testname} ${_sources})
  if(CBM_LOCAL_GTEST)
    add_dependencies(${_testname} GTEST)
  endif()
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

set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/..
  ${CMAKE_CURRENT_SOURCE_DIR}/sts
  ${CMAKE_CURRENT_SOURCE_DIR}/global
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



if (CMAKE_SYSTEM_NAME MATCHES Linux)
  set(DEPENDENCIES ${DEPENDENCIES} rt)
endif()



Set(CbmLinkSources
  ../CbmLink.cxx
  _GTestCbmLink.cxx
)

CreateGTestExeAndAddTest(_GTestCbmLink "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmLinkSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "")

Set(CbmMatchSources
  ../CbmMatch.cxx
  _GTestCbmMatch.cxx
)

CreateGTestExeAndAddTest(_GTestCbmMatch "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmMatchSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestCbmLink")

Set(CbmClusterSources
  ../CbmCluster.cxx
  _GTestCbmCluster.cxx
)

CreateGTestExeAndAddTest(_GTestCbmCluster "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmClusterSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestCbmDigi")

Set(CbmHitSources
  ../CbmHit.cxx
  _GTestCbmHit.cxx
)

CreateGTestExeAndAddTest(_GTestCbmHit "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmHitSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestCbmCluster")

Set(CbmPixelHitSources
  ../CbmPixelHit.cxx
  _GTestCbmPixelHit.cxx
)

CreateGTestExeAndAddTest(_GTestCbmPixelHit "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmPixelHitSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestCbmHit")

Set(CbmStripHitSources
  ../CbmStripHit.cxx
  _GTestCbmStripHit.cxx
)

CreateGTestExeAndAddTest(_GTestCbmStripHit "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmStripHitSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestCbmPixelHit")


Set(CbmEventSources
  ../CbmEvent.cxx
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
  ../CbmErrorMessage.cxx
  _GTestCbmErrorMessage.cxx
)

CreateGTestExeAndAddTest(_GTestCbmErrorMessage "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmErrorMessageSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestCbmConstants")


Set(CbmAddressSources
  ../CbmAddress.cxx
  _GTestCbmAddress.cxx
)  

CreateGTestExeAndAddTest(_GTestCbmAddress "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmAddressSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestCbmErrorMessage")



#Set(CbmModuleListSources
#  ${CBMDATA_DIR}/CbmModuleList.cxx
#  _GTestCbmModuleList.cxx
#)  
#
#CreateGTestExeAndAddTest(_GTestCbmModuleList "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
#                         "${CbmModuleListSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestCbmVertex")


Set(CbmStsDigiSources
  ../sts/CbmStsDigi.cxx
  sts/_GTestCbmStsDigi.cxx
)

CreateGTestExeAndAddTest(_GTestCbmStsDigi "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmStsDigiSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestCbmVertex")

Set(CbmTofDigiSources
 ../tof/CbmTofDigi.cxx
  tof/_GTestCbmTofDigi.cxx
)

CreateGTestExeAndAddTest(_GTestCbmTofDigi "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmTofDigiSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestStsDigi")

Set(CbmRichDigiSources
  ../rich/CbmRichDigi.cxx
  rich/_GTestCbmRichDigi.cxx
)

CreateGTestExeAndAddTest(_GTestCbmRichDigi "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmRichDigiSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestTofDigi")


Set(CbmMuchDigiSources
  ../much/CbmMuchDigi.cxx
  much/_GTestCbmMuchDigi.cxx
)

CreateGTestExeAndAddTest(_GTestCbmMuchDigi "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmMuchDigiSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTesRichDigi")

Set(CbmMuchBeamTimeDigiSources
  ../much/CbmMuchBeamTimeDigi.cxx
  much/_GTestCbmMuchBeamTimeDigi.cxx
)

CreateGTestExeAndAddTest(_GTestCbmMuchBeamTimeDigi "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmMuchBeamTimeDigiSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestMuchDigi")

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

