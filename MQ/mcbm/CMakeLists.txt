configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startMQMcbmPulserMonitor2020.sh.in ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startMQMcbmPulserMonitor2020.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startMQMcbmEvtBuilderWin2020.sh.in ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startMQMcbmEvtBuilderWin2020.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startMQBuildRawEvents.sh.in ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startMQBuildRawEvents.sh)

set(INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/MQ/base
    ${CBMROOT_SOURCE_DIR}/fles/mcbm2018/unpacker
    ${CBMROOT_SOURCE_DIR}/fles/mcbm2018/tasks
    ${CBMROOT_SOURCE_DIR}/fles/mcbm2018/parameter
    ${CBMROOT_SOURCE_DIR}/fles/mcbm2018/dataformat
    ${CBMROOT_SOURCE_DIR}/fles/mcbm2018/commonMQ
    ${CBMROOT_SOURCE_DIR}/fles/flestools
    ${CBMROOT_SOURCE_DIR}/reco/eventbuilder/digis
    ${CBMROOT_SOURCE_DIR}/reco/base/
    ${CBMROOT_SOURCE_DIR}/reco/detectors/psd/unpack
    ${CBMROOT_SOURCE_DIR}/reco/detectors/sts/unpack
    ${CBMROOT_SOURCE_DIR}/reco/detectors/tof/unpack
    ${CBMROOT_SOURCE_DIR}/reco/detectors/trd/unpack
    ${CBMROOT_SOURCE_DIR}/reco/detectors/trd/rawToDigiMethods
    ${CBMROOT_SOURCE_DIR}/reco/detectors/rich/unpack
    ${CBMDATA_DIR}
    ${CBMDATA_DIR}/global
    ${CBMDATA_DIR}/raw
    ${CBMDATA_DIR}/sts
    ${CBMDATA_DIR}/much
    ${CBMDATA_DIR}/rich
    ${CBMDATA_DIR}/tof
    ${CBMDATA_DIR}/psd
    ${CBMDATA_DIR}/trd
    ${CBMDATA_DIR}/mvd # Feint to avoid crash of DigiManager due to missing source pointer
    ${CBMBASE_DIR}
    ${CBMROOT_SOURCE_DIR}/sim/transport/steer # For CbmSetup.h!
    ${CBMROOT_SOURCE_DIR}/sim/transport/geosetup # For CbmGeoSetupDbProvider.h, needed by CbmSetup

    ${CBMDETECTORBASE_DIR}/sts # required for parameter handling of the sts
    ${CBMDETECTORBASE_DIR}/trd # required for parameter handling of the trd
    ${CBMDETECTORBASE_DIR}/tof # required for parameter handling of the tof
    ${CBMDETECTORBASE_DIR}/rich # required for parameter handling of the rich
    ${CBMDETECTORBASE_DIR}/psd # required for fitting tools of the psd
)

Set(SYSTEM_INCLUDE_DIRECTORIES
    ${SYSTEM_INCLUDE_DIRECTORIES}
    ${ZeroMQ_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR}
    ${FAIRROOT_INCLUDE_DIR}
    ${FAIRMQ_INCLUDE_DIR}
    ${FAIRMQ_INCLUDE_DIR}/options
    ${VMC_INCLUDE_DIRS}
    ${FAIRLOGGER_INCLUDE_DIR}

    ${IPC_INCLUDE_DIRECTORY}
    ${CBMROOT_SOURCE_DIR}/external/cppzmq
)

include_directories(${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
  ${KFParticle_LIB_DIR}
  ${FAIRMQ_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
  ${ROOT_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
)

link_directories(${LINK_DIRECTORIES})

set(EXECUTABLE_OUTPUT_PATH "${EXECUTABLE_OUTPUT_PATH}/MQ/mcbm")

Set(BOOST_LIBS
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_SERIALIZATION_LIBRARY}
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  ${Boost_LOG_LIBRARY}
)
If(UNIX AND NOT APPLE)
  List(APPEND BOOST_LIBS pthread)
EndIf()

set(FAIR_LIBS
  ${FAIRMQ_LIBS}
)

If(FAIRLOGGER_FOUND)
  set(FAIR_LIBS
      ${FAIR_LIBS}
      FairLogger
     )
EndIf()

set(EXE_NAME McbmUnpack)
set(SRCS CbmDeviceMcbmUnpack.cxx runMcbmUnpack.cxx)

set(DEPENDENCIES
  ${DEPENDENCIES}
  ${FAIR_LIBS}
  ${BOOST_LIBS}
  fles_ipc
  CbmFlibMcbm2018
  CbmFlibFlesTools
  CbmBase
  CbmData
  Core
  RIO
  Net
  Hist
  RHTTP
)
#GENERATE_LIBRARY()
GENERATE_EXECUTABLE()

set(EXE_NAME McbmMonitorPulser)
set(SRCS CbmDeviceMcbmMonitorPulser.cxx runMcbmMonitorPulser.cxx)

set(DEPENDENCIES
  ${DEPENDENCIES}
  ${FAIR_LIBS}
  ${BOOST_LIBS}
  fles_ipc
  CbmFlibMcbm2018
  CbmFlibFlesTools
  CbmBase
  CbmData
  Core
  RIO
  Net
  Hist
  RHTTP
)
#GENERATE_LIBRARY()
GENERATE_EXECUTABLE()

set(EXE_NAME BuildRawEvents)
set(SRCS CbmDeviceBuildRawEvents.cxx runBuildRawEvents.cxx)

set(DEPENDENCIES
  ${DEPENDENCIES}
  ${FAIR_LIBS}
  ${BOOST_LIBS}
  fles_ipc
  KFParticle
  CbmFlibMcbm2018
  CbmFlibFlesTools
  CbmEventBuilder
  KF
  L1
  CbmBase
  CbmData
  CbmTofBase
  CbmSimBase
  Core
  RIO
  Net
  Hist
  RHTTP
)
#GENERATE_LIBRARY()
GENERATE_EXECUTABLE()


set(EXE_NAME McbmEventBuilderWin)
set(SRCS CbmDeviceMcbmEventBuilderWin.cxx runMcbmEvtBuilderWin.cxx)

set(DEPENDENCIES
  ${DEPENDENCIES}
  ${FAIR_LIBS}
  ${BOOST_LIBS}
  fles_ipc
  CbmFlibMcbm2018
  CbmFlibFlesTools
  CbmBase
  CbmData
  Core
  RIO
  Net
  Hist
  RHTTP
)
#GENERATE_LIBRARY()
GENERATE_EXECUTABLE()

set(EXE_NAME McbmEventSink)
set(SRCS CbmDeviceMcbmEventSink.cxx runMcbmEventSink.cxx)

set(DEPENDENCIES
  ${DEPENDENCIES}
  ${FAIR_LIBS}
  ${BOOST_LIBS}
  fles_ipc
  CbmFlibMcbm2018
  CbmFlibFlesTools
  CbmBase
  CbmData
  Core
  RIO
  Tree
  Net
  Hist
  RHTTP
)
#GENERATE_LIBRARY()
GENERATE_EXECUTABLE()


set(EXE_NAME MqUnpack)
set(SRCS CbmDeviceUnpack.cxx runUnpack.cxx)

set(DEPENDENCIES
  ${DEPENDENCIES}
  ${FAIR_LIBS}
  ParBase
  ${BOOST_LIBS}
  fles_ipc
  CbmFlibMcbm2018
  CbmFlibFlesTools
  CbmBase
  CbmRecoBase
  CbmPsdReco
  CbmRichReco
  CbmRecoSts
  CbmTofReco
  CbmTrdReco
  CbmData
  CbmSimSteer # for CbmSetup!
  Core
  RIO
  Net
  Hist
  RHTTP
)
GENERATE_EXECUTABLE()

