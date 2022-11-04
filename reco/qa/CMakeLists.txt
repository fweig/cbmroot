set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )


set(SRCS
  CbmRecoQa.cxx
  CbmTrackingTrdQa.cxx
  )

set(LIBRARY_NAME CbmRecoQa)
set(LINKDEF  RecoQaLinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmBase
  CbmData
  FairRoot::Base
  ROOT::Hist
  ROOT::Core
  ROOT::RIO
  ROOT::Physics
  )

set(PRIVATE_DEPENDENCIES
  CbmTrdBase
  CbmQaBase
  NicaCbmFormat
  ROOT::EG
  ROOT::Geom
  )

generate_cbm_library()
