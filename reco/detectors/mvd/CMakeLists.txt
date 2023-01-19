set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/qa
  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/tasks
  )

set(SRCS
  #plugins/tasks/CbmMvdSensorDigiToHitTask.cxx
  plugins/tasks/CbmMvdSensorHitfinderTask.cxx
  plugins/tasks/CbmMvdSensorClusterfinderTask.cxx
  #plugins/tasks/CbmMvdSensorFindHitTask.cxx
  
  CbmMvdHitfinder.cxx  #OLD
# CbmMvdDigiToHit.cxx
  
  CbmMvdClusterfinder.cxx
  #CbmMvdClusterfinderTB.cxx
  )


set(LIBRARY_NAME CbmMvdReco)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmBase
  CbmData
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
  CbmMvdBase
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
