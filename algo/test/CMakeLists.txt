Macro(CreateGTestExeAndAddTest _testname _includeDirs _linkDirs _sources _pub_dep _pvt_dep _int_dep _testdepend)

  add_executable(${_testname} ${_sources})
  if(CBM_LOCAL_GTEST)
    add_dependencies(${_testname} GTEST)
  endif()
      add_dependencies(${_testname} GTEST)
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


  get_target_property(_inDir Gtest INTERFACE_INCLUDE_DIRECTORIES)
  set_property(TARGET Gtest APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${_inDir}/..")

set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}  
  ${CMAKE_SOURCE_DIR}/algo/trigger
  ${CMAKE_SOURCE_DIR}/algo/evbuild
  )
  
set(PUB_DEPS
  CbmData
  Gtest
  GtestMain
  ROOT::Physics
  )
  
set(PVT_DEPS
  ROOT::Core
  ROOT::Matrix
  ROOT::EG
  )
  
set(INT_DEPS  
  FairRoot::Base
  external::fles_ipc
  )
  
  

Set(TimeClusterTriggerSources
  ${CMAKE_SOURCE_DIR}/algo/trigger/TimeClusterTrigger.cxx
  _GTestTimeClusterTrigger.cxx 
)

CreateGTestExeAndAddTest(_GTestTimeClusterTrigger "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${TimeClusterTriggerSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "")

Set(EventBuilderSources
  ${CMAKE_SOURCE_DIR}/algo/evbuild/EventBuilder.cxx
  _GTestEventBuilder.cxx 
)

CreateGTestExeAndAddTest(_GTestEventBuilder "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${EventBuilderSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "")


