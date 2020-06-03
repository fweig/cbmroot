set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR} 
  ${CMAKE_CURRENT_SOURCE_DIR}/tools
  ${CMAKE_CURRENT_SOURCE_DIR}/tools/readout
  ${CMAKE_CURRENT_SOURCE_DIR}/SensorDataSheets
  ${CMAKE_CURRENT_SOURCE_DIR}/plugins
  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/tasks
  ${CMAKE_CURRENT_SOURCE_DIR}/plugins/buffers

  ${CBMDATA_DIR}
  ${CBMDATA_DIR}/base
  ${CBMDATA_DIR}/sts
  ${CBMDATA_DIR}/mvd
  ${CBMDATA_DIR}/global
  ${CBMBASE_DIR} 

  ${CBMROOT_SOURCE_DIR}/sim/transport/base
)

include_directories(${INCLUDE_DIRECTORIES})

set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES} 
#  ${CLHEP_INCLUDE_DIR}
#  ${GSL_INCLUDE_DIRS}
)

include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
#${GSL_LIBRARY_DIRS}
  ${FAIRROOT_LIBRARY_DIR}
# ${CLHEP_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
)

link_directories(${LINK_DIRECTORIES})

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

#OLD
  CbmMvdClusterfinder.cxx
  CbmMvdClusterfinderTB.cxx 
####
  CbmMvdReadout.cxx
)



set(LINKDEF CbmMvdLinkDef.h)
set(LIBRARY_NAME CbmMvd)
set(DEPENDENCIES
    CbmBase CbmData Base
)

GENERATE_LIBRARY()

Install(FILES tools/CbmMvdHelper.h 
              tools/CbmMvdGeoHandler.h
        DESTINATION include/mvd/tools
       )
Install(FILES plugins/CbmMvdSensorPlugin.h 
        DESTINATION include/mvd/plugins
       )
Install(FILES SensorDataSheets/CbmMvdMimosa26AHR.h 
              SensorDataSheets/CbmMvdSensorDataSheet.h
              SensorDataSheets/CbmMvdMimosa34.h
              SensorDataSheets/CbmMvdMimosis.h
        DESTINATION include/mvd/SensorDataSheets
       )
Install(FILES plugins/buffers/CbmMvdSensorBuffer.h
              plugins/buffers/CbmMvdSensorFrameBuffer.h
              plugins/buffers/CbmMvdSensorTrackingBuffer.h
        DESTINATION include/mvd/plugins/buffers
       )
Install(FILES plugins/tasks/CbmMvdSensorClusterfinderTask.h
              plugins/tasks/CbmMvdSensorDigitizerTask.h
              plugins/tasks/CbmMvdSensorDigitizerTBTask.h
              plugins/tasks/CbmMvdSensorDigiToHitTask.h
              plugins/tasks/CbmMvdSensorFindHitTask.h
              plugins/tasks/CbmMvdSensorHitfinderTask.h
              plugins/tasks/CbmMvdSensorTask.h
        DESTINATION include/mvd/plugins/tasks
       )

