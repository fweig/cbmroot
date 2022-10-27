set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )


set(SRCS
  CbmAnaMuonCandidate.cxx 
  CbmAnaDimuonAnalysis.cxx
  )


set(LIBRARY_NAME CbmMuchAnalysis)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  KF
  FairRoot::Base
  ROOT::Physics
  )

set(PRIVATE_DEPENDENCIES
  CbmData
  CbmSimGenerators
  CbmMuchBase
  NicaCbmFormat
  ROOT::Core
  ROOT::Hist
  ROOT::MathCore
  ROOT::MLP
  ROOT::RIO
  ROOT::Tree
  )

generate_cbm_library()




