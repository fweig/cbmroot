# --- CMake steering file for core/data/test/global
# --- V. Friese, 2021

set(INCLUDE_DIRECTORIES
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/../sts
  )


set(PVT_DEPS
  CbmData
  Gtest
  GtestMain

  Boost::serialization

  ROOT::Matrix
  ROOT::Physics
  )
  
 
# --- Test CbmDigiEvent
Set(CbmDigiEventSources
  _GTestCbmDigiEvent.cxx
)
CreateGTestExeAndAddTest(_GTestCbmDigiEvent "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmDigiEventSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "")

# --- Test CbmDigiTimeslice
Set(CbmDigiTimesliceSources
  _GTestCbmDigiTimeslice.cxx
)
CreateGTestExeAndAddTest(_GTestCbmDigiTimeslice "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmDigiTimesliceSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestCbmDigiEvent")

 # --- Test CbmVertex
Set(CbmVertexSources
  _GTestCbmVertex.cxx
)
CreateGTestExeAndAddTest(_GTestCbmVertex "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmVertexSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestCbmDigiTimeslice")


