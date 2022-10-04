set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

set(SRCS
  CbmTofCell.cxx
  CbmTofContFact.cxx
  CbmTofCreateDigiPar.cxx
  CbmTofDigiBdfPar.cxx
  CbmTofDigiPar.cxx
  CbmMcbm2018TofPar.cxx
  CbmTofGeoHandler.cxx
  CbmTofTrackingInterface.cxx
  TTrbHeader.cxx
  )

set(LIBRARY_NAME CbmTofBase)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES 
  CbmData
  FairRoot::Base
  FairRoot::ParBase
  ROOT::Core 
  ROOT::Geom 
  ROOT::MathCore 
  )

set(PRIVATE_DEPENDENCIES 
  CbmBase
  FairLogger::FairLogger 
  ROOT::Hist
  ROOT::RIO
  )

set(INTERFACE_DEPENDENCIES
  ROOT::Graf3d
  )

generate_cbm_library()
