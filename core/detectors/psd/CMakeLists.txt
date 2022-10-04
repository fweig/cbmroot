set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

set(SRCS
  PronyFitter.cxx
  CbmPsdContFact.cxx
  CbmMcbm2018PsdPar.cxx
  )

set(LIBRARY_NAME CbmPsdBase)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES 
  FairRoot::ParBase
  )

set(PRIVATE_DEPENDENCIES 
  FairLogger::FairLogger 
  ROOT::Core 
  )

generate_cbm_library()
