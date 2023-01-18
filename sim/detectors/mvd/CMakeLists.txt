set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/tasks
  )

set(SRCS
  plugins/tasks/CbmMvdSensorDigitizerTask.cxx
  #plugins/tasks/CbmMvdSensorDigitizerTBTask.cxx
  
  CbmMvd.cxx

  CbmMvdDigitizer.cxx
  CbmMvdPixelCharge.cxx
  )


set(LIBRARY_NAME CbmMvdSim)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmBase
  CbmData
  CbmMvdBase
  FairRoot::Base
  ROOT::Core
  ROOT::Geom
  ROOT::Gpad
  ROOT::Hist
  ROOT::MathCore
  ROOT::Physics
  )

set(PRIVATE_DEPENDENCIES
  CbmSimBase
  FairLogger::FairLogger
  FairRoot::GeoBase
  FairRoot::ParBase
  ROOT::EG
  ROOT::Graf
  ROOT::Matrix
  ROOT::RIO
  ROOT::Tree
  )

generate_cbm_library()
