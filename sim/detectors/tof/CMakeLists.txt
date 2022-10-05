set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

set(SRCS
  CbmTof.cxx 
  CbmTofMergeMcPoints.cxx 
  CbmTofDigitize.cxx
  )


set(LIBRARY_NAME CbmTofSim)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmBase
  CbmData
  FairRoot::Base
  ROOT::Core
  ROOT::Hist
  ROOT::Physics
  )

set(PRIVATE_DEPENDENCIES
  CbmSimBase
  CbmTofBase
  FairRoot::GeoBase
  FairRoot::ParBase
  ROOT::EG
  ROOT::Geom
  ROOT::Graf
  ROOT::MathCore
  ROOT::RIO
  )

generate_cbm_library()
