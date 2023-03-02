set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

set(SRCS
  CbmQaCanvas.cxx
  CbmQaEff.cxx
  CbmQaPie.cxx
  CbmQaHist.cxx
  CbmQaTable.cxx
  CbmQaTask.cxx
  )


set(LIBRARY_NAME CbmQaBase)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  FairRoot::Base
  FairLogger::FairLogger
  external::yaml-cpp
  ROOT::Core
  ROOT::Gpad
  ROOT::Hist
  )

set(INTERFACE_DEPENDENCIES
  ROOT::Graf
  )

generate_cbm_library()

Install(FILES CbmQaTask.h CbmQaCanvas.h CbmQaTable.h CbmQaHist.h CbmQaEff.h CbmQaPie.h
        DESTINATION include
       )
