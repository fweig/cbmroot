# --- CMake steering file for core/data/test/global
# --- V. Friese, 2021

set(INCLUDE_DIRECTORIES
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/../sts
  )


set(PUB_DEPS
  CbmData
  Gtest
  GtestMain
  )
  
set(PVT_DEPS
  FairLogger::FairLogger
  Boost::regex
  ROOT::Matrix
  ROOT::Physics
  )
  
set(INT_DEPS  
  )
  
 
# --- Test CbmDigiEvent
Set(CbmDigiEventSources
  ${CBMDATA_DIR}/global/CbmDigiEvent.h
  _GTestCbmDigiEvent.cxx
)
CreateGTestExeAndAddTest(_GTestCbmDigiEvent "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmDigiEventSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "")

# --- Test CbmDigiTimeslice
Set(CbmDigiTimesliceSources
  ${CBMDATA_DIR}/global/CbmDigiTimeslice.h
  _GTestCbmDigiTimeslice.cxx
)
CreateGTestExeAndAddTest(_GTestCbmDigiTimeslice "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmDigiTimesliceSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestCbmDigiEvent")

 # --- Test CbmVertex
Set(CbmVertexSources
  ${CBMDATA_DIR}/global/CbmVertex.cxx
  _GTestCbmVertex.cxx
)
CreateGTestExeAndAddTest(_GTestCbmVertex "${INCLUDE_DIRECTORIES}" "${LINK_DIRECTORIES}"
                         "${CbmVertexSources}" "${PUB_DEPS}" "${PVT_DEPS}" "${INT_DEPS}" "_GTestCbmDigiTimeslice")


