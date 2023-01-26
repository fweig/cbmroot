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


#  get_target_property(_inDir Gtest INTERFACE_INCLUDE_DIRECTORIES)
#  set_property(TARGET Gtest APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${_inDir}/..")

set(PVT_DEPS
  Algo
  Gtest
  GtestMain
  )



Set(TimeClusterTriggerSources
  _GTestTimeClusterTrigger.cxx
)

CreateGTestExeAndAddTest(_GTestTimeClusterTrigger "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${TimeClusterTriggerSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "")

Set(EventBuilderSources
  _GTestEventBuilder.cxx
)

CreateGTestExeAndAddTest(_GTestEventBuilder "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${EventBuilderSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "")

Set(DigiEventSelectorSources
  _GTestDigiEventSelector.cxx
)

CreateGTestExeAndAddTest(_GTestDigiEventSelector "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${DigiEventSelectorSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "")
