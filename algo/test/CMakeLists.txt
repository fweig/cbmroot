Macro(CreateGTestExeAndAddTest _testname _includeDirs _linkDirs _sources _dependencies _testdepend)

  Include_Directories(SYSTEM "${_includeDirs}")
  Link_Directories(${_linkDirs})

  Add_Executable(${_testname} ${_sources})
  if(CBM_LOCAL_GTEST)
    Add_Dependencies(${_testname} GTEST)
  endif()
  Target_Link_Libraries(${_testname} ${_dependencies})
  Gen_Exe_Script(${_testname})
  string(REPLACE ${PROJECT_SOURCE_DIR}
         ${PROJECT_BINARY_DIR} new_path ${CMAKE_CURRENT_SOURCE_DIR}
        )
  Add_Test(${_testname} ${new_path}/${_testname}.sh)
  If(NOT ${_testdepend} STREQUAL "")
    Set_Tests_Properties(${_testname} PROPERTIES DEPENDS ${_testdepend})
  EndIf()
EndMacro(CreateGTestExeAndAddTest)

FIND_PACKAGE(Threads REQUIRED)

Set(INCLUDE_DIRECTORIES
  ${CBMBASE_DIR}
  ${CBMDATA_DIR}
  ${CBMDATA_DIR}/base
  ${CBMDATA_DIR}/global
  ${CBMDATA_DIR}/sts
  ${CBMDATA_DIR}/tof
  ${CBMDATA_DIR}/rich
  ${CBMDATA_DIR}/much
  ${CBMDATA_DIR}/psd
  ${CBMDATA_DIR}/trd
  ${CMAKE_SOURCE_DIR}/external/ipc/ipc/lib/fles_ipc
  ${CMAKE_SOURCE_DIR}/algo
  ${CMAKE_SOURCE_DIR}/algo/trigger
  ${CMAKE_SOURCE_DIR}/algo/evbuild
  ${BASE_INCLUDE_DIRECTORIES}
  ${GTEST_INCLUDE_DIR}
)

MESSAGE("FAIRROOT_LIBRARY_DIR: ${FAIRROOT_LIBRARY_DIR}")
Set(LINK_DIRECTORIES
  ${FAIRROOT_LIBRARY_DIR}
)

Set(DEPENDENCIES
  ${GTEST_BOTH_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
  CbmData
)

if (CMAKE_SYSTEM_NAME MATCHES Linux)
  set(DEPENDENCIES ${DEPENDENCIES} rt)
endif()

Set(TimeClusterTriggerSources
  ${CMAKE_SOURCE_DIR}/algo/trigger/TimeClusterTrigger.cxx
  _GTestTimeClusterTrigger.cxx 
)

CreateGTestExeAndAddTest(_GTestTimeClusterTrigger "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${TimeClusterTriggerSources}" "${DEPENDENCIES}" "")

Set(EventBuilderSources
  ${CMAKE_SOURCE_DIR}/algo/evbuild/EventBuilder.cxx
  _GTestEventBuilder.cxx 
)

CreateGTestExeAndAddTest(_GTestEventBuilder "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${EventBuilderSources}" "${DEPENDENCIES}" "")


