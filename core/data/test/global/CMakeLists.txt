# --- CMake steering file for core/data/test/global
# --- V. Friese, 2021

Set(INCLUDE_DIRECTORIES
  ${CBMDATA_DIR}
  ${CBMDATA_DIR}/base
  ${CBMDATA_DIR}/global
  ${CBMDATA_DIR}/sts
  ${CBMDATA_DIR}/test/sts
  ${CMAKE_SOURCE_DIR}/external/ipc/ipc/lib/fles_ipc 
  ${BASE_INCLUDE_DIRECTORIES}
  ${GTEST_INCLUDE_DIR}
)

MESSAGE("FAIRROOT_LIBRARY_DIR: ${FAIRROOT_LIBRARY_DIR}")
Set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
  ${SIMPATH}/lib
)


Set(DEPENDENCIES
  Core
  Matrix
  Physics
  ${GTEST_BOTH_LIBRARIES}
  FairTools
  CbmData
  boost_serialization
  ${CMAKE_THREAD_LIBS_INIT}
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

# --- Test CbmDigiEvent
Set(CbmDigiEventSources
  ${CBMDATA_DIR}/global/CbmDigiEvent.h
  _GTestCbmDigiEvent.cxx
)
CreateGTestExeAndAddTest(_GTestCbmDigiEvent "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmDigiEventSources}" "${DEPENDENCIES}" "")
                         
# --- Test CbmDigiTimeslice
Set(CbmDigiTimesliceSources
  ${CBMDATA_DIR}/global/CbmDigiTimeslice.h
  _GTestCbmDigiTimeslice.cxx
)
CreateGTestExeAndAddTest(_GTestCbmDigiTimeslice "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmDigiTimesliceSources}" "${DEPENDENCIES}" "_GTestCbmDigiEvent")
                         
 # --- Test CbmVertex
Set(CbmVertexSources
  ${CBMDATA_DIR}/global/CbmVertex.cxx
  _GTestCbmVertex.cxx
)
CreateGTestExeAndAddTest(_GTestCbmVertex "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmVertexSources}" "${DEPENDENCIES}" "_GTestCbmDigiTimeslice")
                         

