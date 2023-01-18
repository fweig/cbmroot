set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/qa
  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/tasks
  )

set(SRCS
  CbmMvdReadoutSimple.cxx
  CbmMvdReadoutCluster.cxx

  CbmMvdReadout.cxx
  plugins/tasks/CbmMvdSensorReadoutTask.cxx

  qa/CbmMvdQa.cxx
#  qa/CbmMvdClusterAna.cxx
  )


set(LIBRARY_NAME CbmMvdAna)
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
