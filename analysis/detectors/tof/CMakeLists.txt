set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

set(SRCS
  CbmTofAnaTestbeam.cxx 
  CbmTofGeometryQa.cxx
  CbmTofHitFinderQa.cxx
  CbmTofHitFinderTBQa.cxx
  CbmTofTests.cxx 
  )


set(LIBRARY_NAME CbmTofAna)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmBase
  CbmData
  CbmTofReco
  FairRoot::Base
  ROOT::Core
  )

set(PRIVATE_DEPENDENCIES
  CbmTofBase
  FairRoot::ParBase
  ROOT::Geom
  ROOT::Hist
  ROOT::MathCore
  ROOT::RIO
  )

generate_cbm_library()
