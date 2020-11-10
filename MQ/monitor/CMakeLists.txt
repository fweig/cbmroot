configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startMQSamplerMonitor.sh.in ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startMQSamplerMonitor.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startMQSamplerT0Monitor2020.sh.in ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startMQSamplerT0Monitor2020.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startMQSamplerTofMonitor2020.sh.in ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startMQSamplerTofMonitor2020.sh)

set(INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/MQ/base
    ${CBMROOT_SOURCE_DIR}/fles/mcbm2018/monitor
    ${CBMROOT_SOURCE_DIR}/fles/mcbm2018/parameter
    ${CBMROOT_SOURCE_DIR}/fles/mcbm2018/dataformat
    ${CBMROOT_SOURCE_DIR}/fles/mcbm2018/commonMQ
    ${CBMROOT_SOURCE_DIR}/fles/flestools
    ${CBMDATA_DIR}
    ${CBMDATA_DIR}/tof
    ${CBMDATA_DIR}/raw
    ${CBMBASE_DIR}
)

Set(SYSTEM_INCLUDE_DIRECTORIES
    ${SYSTEM_INCLUDE_DIRECTORIES}
    ${ZeroMQ_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR}
    ${FAIRROOT_INCLUDE_DIR}
    ${FAIRMQ_INCLUDE_DIR}
    ${FAIRMQ_INCLUDE_DIR}/options

    ${IPC_INCLUDE_DIRECTORY}
    ${CBMROOT_SOURCE_DIR}/external/cppzmq
)

include_directories(${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
  ${FAIRMQ_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
)

link_directories(${LINK_DIRECTORIES})

set(EXECUTABLE_OUTPUT_PATH "${EXECUTABLE_OUTPUT_PATH}/MQ/monitor")

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

set(EXE_NAME T0MonitorMcbm2018)
set(SRCS CbmDeviceMonitorT0.cxx runMonitorT0.cxx)

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

set(EXE_NAME TofMonitorMcbm2018)
set(SRCS CbmDeviceMonitorTof.cxx runMonitorTof.cxx)

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

