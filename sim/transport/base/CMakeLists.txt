set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

set(SRCS
  CbmBeam.cxx
  CbmBeamProfile.cxx
  CbmEventGenerator.cxx
  CbmMCEventFilter.cxx
  CbmStack.cxx
  CbmStackFilter.cxx
  CbmTarget.cxx
  )

set(LIBRARY_NAME CbmSimBase)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmData
  FairRoot::Base
  ROOT::Core
  ROOT::Physics
  )

set(PRIVATE_DEPENDENCIES
  ROOT::EG
  ROOT::Geom
  ROOT::MathCore
  )

generate_cbm_library()
