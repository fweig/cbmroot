set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/tools
  ${CMAKE_CURRENT_SOURCE_DIR}/SensorDataSheets
  ${CMAKE_CURRENT_SOURCE_DIR}/plugins
  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/tasks
  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/buffers
  )

set(SRCS
  plugins/CbmMvdSensorPlugin.cxx

  plugins/tasks/CbmMvdSensorTask.cxx

  plugins/buffers/CbmMvdSensorBuffer.cxx
  plugins/buffers/CbmMvdSensorFrameBuffer.cxx
#  plugins/buffers/CbmMvdSensorTrackingBuffer.cxx

  SensorDataSheets/CbmMvdMimosa26AHR.cxx
  SensorDataSheets/CbmMvdMimosa34.cxx
  SensorDataSheets/CbmMvdMimosis.cxx
  SensorDataSheets/CbmMvdSensorDataSheet.cxx
  
  tools/CbmMvdGeoHandler.cxx
  tools/CbmMvdHelper.h

  CbmMvdStationPar.cxx
  CbmMvdDetector.cxx
  CbmMvdSensor.cxx

  CbmMvdPileupManager.cxx
  CbmMvdTrackingInterface.cxx
  )


set(LIBRARY_NAME CbmMvdBase)
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
