set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}/base
  )

set(SRCS
  base/CbmDigitization.cxx
  base/CbmDigitizationSource.cxx
  base/CbmDigitizeInfo.cxx
  base/CbmMCInput.cxx
  base/CbmMCInputSet.cxx
  base/CbmRunAna.cxx
  base/CbmDigitizationConfig.cxx
  )

set(LIBRARY_NAME CbmSimResponse)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  FairRoot::Base
  ROOT::Core
  ROOT::Tree
  )

set(PRIVATE_DEPENDENCIES
  CbmMuchSim
  CbmMvdSim
  CbmPsdSim
  CbmRichSim
  CbmSimSteer
  CbmStsSim
  CbmTofSim
  CbmTrdSim
  CbmBmonSim
  FairRoot::Tools
  FairRoot::ParBase
  )

set(INTERFACE_DEPENDENCIES
  CbmBase
  CbmData
  )

generate_cbm_library()
