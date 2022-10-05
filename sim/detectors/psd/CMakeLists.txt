set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

set(SRCS
  CbmPsdMC.cxx  
  CbmPsdIdealDigitizer.cxx
  CbmPsdSimpleDigitizer.cxx
  )

set(LIBRARY_NAME CbmPsdSim)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmBase
  CbmData
  FairRoot::Base
  ROOT::Core
  ROOT::MathCore
  )

set(PRIVATE_DEPENDENCIES
  CbmSimBase
  ROOT::Geom
  ROOT::RIO
  )

set(INTERFACE_DEPENDENCIES
  ROOT::Physics
  )


generate_cbm_library()
