set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/tools
  ${CMAKE_CURRENT_SOURCE_DIR}/tools/readout
  ${CMAKE_CURRENT_SOURCE_DIR}/SensorDataSheets
  ${CMAKE_CURRENT_SOURCE_DIR}/plugins
  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/tasks
  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/buffers
  ${CMAKE_CURRENT_SOURCE_DIR}/qa

  )

set(SRCS
  plugins/tasks/CbmMvdSensorTask.cxx
  plugins/tasks/CbmMvdSensorDigitizerTask.cxx
  plugins/tasks/CbmMvdSensorDigitizerTBTask.cxx
  plugins/tasks/CbmMvdSensorDigiToHitTask.cxx
  #OLD
  plugins/tasks/CbmMvdSensorHitfinderTask.cxx
  plugins/tasks/CbmMvdSensorClusterfinderTask.cxx
  ####
  plugins/tasks/CbmMvdSensorReadoutTask.cxx
  plugins/CbmMvdSensorPlugin.cxx
  
  SensorDataSheets/CbmMvdMimosa26AHR.cxx
  SensorDataSheets/CbmMvdMimosa34.cxx
  SensorDataSheets/CbmMvdMimosis.cxx
  SensorDataSheets/CbmMvdSensorDataSheet.cxx
  
  tools/CbmMvdGeoHandler.cxx
  tools/CbmMvdHelper.h
  tools/readout/CbmMvdReadoutSimple.cxx
  tools/readout/CbmMvdReadoutCluster.cxx
  
  qa/CbmMvdQa.cxx
  
  CbmMvd.cxx
  
  CbmMvdDigitizer.cxx
  CbmMvdPileupManager.cxx
  CbmMvdPixelCharge.cxx
  CbmMvdHitfinder.cxx  #OLD
  CbmMvdStationPar.cxx
  CbmMvdDetector.cxx
  CbmMvdSensor.cxx
  CbmMvdDigiToHit.cxx
  CbmMvdDigiToHitTB.cxx
  CbmMvdTrackingInterface.cxx
  
  #OLD
  CbmMvdClusterfinder.cxx
  CbmMvdClusterfinderTB.cxx
  ####
  CbmMvdReadout.cxx
  )


set(LIBRARY_NAME CbmMvd)
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


# Install file 
install(FILES tools/CbmMvdHelper.h
              tools/CbmMvdGeoHandler.h
        DESTINATION include/mvd/tools
       )
install(FILES plugins/CbmMvdSensorPlugin.h
        DESTINATION include/mvd/plugins
       )
install(FILES SensorDataSheets/CbmMvdMimosa26AHR.h
              SensorDataSheets/CbmMvdSensorDataSheet.h
              SensorDataSheets/CbmMvdMimosa34.h
              SensorDataSheets/CbmMvdMimosis.h
        DESTINATION include/mvd/SensorDataSheets
       )
install(FILES plugins/buffers/CbmMvdSensorBuffer.h
              plugins/buffers/CbmMvdSensorFrameBuffer.h
              plugins/buffers/CbmMvdSensorTrackingBuffer.h
        DESTINATION include/mvd/plugins/buffers
       )
install(FILES plugins/tasks/CbmMvdSensorClusterfinderTask.h
              plugins/tasks/CbmMvdSensorDigitizerTask.h
              plugins/tasks/CbmMvdSensorDigitizerTBTask.h
              plugins/tasks/CbmMvdSensorDigiToHitTask.h
#              plugins/tasks/CbmMvdSensorFindHitTask.h
              plugins/tasks/CbmMvdSensorHitfinderTask.h
              plugins/tasks/CbmMvdSensorTask.h
        DESTINATION include/mvd/plugins/tasks
       )

      
