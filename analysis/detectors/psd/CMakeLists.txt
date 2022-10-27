set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

set(SRCS
  CbmPsdMCbmQaReal.cxx
)


set(LIBRARY_NAME CbmPsdAna)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmBase
  CbmData
  )

set(PRIVATE_DEPENDENCIES
  ROOT::Core
  ROOT::Geom
  ROOT::Gpad
  ROOT::Graf
  ROOT::Hist
  ROOT::MathCore
  ROOT::RIO
  )

set(INTERFACE_DEPENDENCIES
  FairRoot::Base
  )


generate_cbm_library()
