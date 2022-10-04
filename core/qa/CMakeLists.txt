set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

set(SRCS
  CbmQaCanvas.cxx
  CbmQaPie.cxx
  CbmQaHist.cxx
  CbmQaTable.cxx
  )


set(LIBRARY_NAME CbmQaBase)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  FairLogger::FairLogger
  ROOT::Core
  ROOT::Gpad
  ROOT::Hist
  )

set(INTERFACE_DEPENDENCIES
  ROOT::Graf
  )

generate_cbm_library()
