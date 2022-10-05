set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/qa
  )

set(SRCS
  CbmGeoMuch.cxx  
  CbmMuch.cxx
  
  CbmMuchDigitizeGem.cxx
  CbmMuchReadoutBuffer.cxx
  CbmMuchSignal.cxx
  
  qa/CbmMuchTransportQa.cxx
  qa/CbmMuchDigitizerQa.cxx
  )

set(LIBRARY_NAME CbmMuchSim)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmBase
  CbmData
  CbmMuchBase
  FairRoot::Base
  FairRoot::GeoBase
  ROOT::Core
  ROOT::Graf
  ROOT::Hist
  ROOT::MathCore
  ROOT::Physics
  )

set(PRIVATE_DEPENDENCIES
  CbmQaBase
  CbmSimBase
  FairRoot::ParBase
  VMCLibrary
  ROOT::EG
  ROOT::Geom
  ROOT::Gpad
  ROOT::RIO
  ROOT::Tree
  )


generate_cbm_library()
