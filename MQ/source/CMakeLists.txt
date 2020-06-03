configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startMQInfo.sh.in ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startMQInfo.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startMQSampler.sh.in ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startMQSampler.sh)

set(INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/MQ/base
    ${CBMBASE_DIR}
    ${CBMDATA_DIR}
    ${CBMDATA_DIR}/base
    ${CBMDATA_DIR}/mvd
    ${CBMDATA_DIR}/sts
    ${CBMDATA_DIR}/rich
    ${CBMDATA_DIR}/much
    ${CBMDATA_DIR}/trd
    ${CBMDATA_DIR}/tof
#    ${CBMDATA_DIR}/ecal
    ${CBMDATA_DIR}/psd
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
  ${Boost_LIBRARY_DIRS}
)

link_directories(${LINK_DIRECTORIES})

set(EXECUTABLE_OUTPUT_PATH "${EXECUTABLE_OUTPUT_PATH}/MQ/source")

Set(BOOST_LIBS
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SERIALIZATION_LIBRARY}
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  ${Boost_LOG_LIBRARY}
  ${Boost_REGEX_LIBRARY}
)

If(UNIX AND NOT APPLE)
  List(APPEND BOOST_LIBS pthread)
EndIf()

set(FAIR_LIBS
  FairMQ
)

If(FAIRLOGGER_FOUND)
  set(FAIR_LIBS
      ${FAIR_LIBS}
      FairLogger
     )
EndIf()

set(EXE_NAME TsaSampler)
set(SRCS CbmMQTsaSampler.cxx runTsaSampler.cxx)
set(DEPENDENCIES
  ${DEPENDENCIES}
  ${FAIR_LIBS}
  ${BOOST_LIBS}
  fles_ipc
  CbmMQBase
)
GENERATE_EXECUTABLE()

set(EXE_NAME MultiTsaSampler)
set(SRCS CbmMQTsaMultiSampler.cxx runTsaMultiSampler.cxx)
set(DEPENDENCIES
  ${DEPENDENCIES}
  ${FAIR_LIBS}
  ${BOOST_LIBS}
  fles_ipc
  CbmMQBase
)
GENERATE_EXECUTABLE()


set(EXE_NAME TsaSamplerTof)
set(SRCS CbmMQTsaSamplerTof.cxx runTsaSamplerTof.cxx)
set(DEPENDENCIES
  ${DEPENDENCIES}
  ${FAIR_LIBS}
  ${BOOST_LIBS}
  fles_ipc
  CbmMQBase
)
GENERATE_EXECUTABLE()

set(EXE_NAME TsaMultiSamplerTof)
set(SRCS CbmMQTsaMultiSamplerTof.cxx runTsaMultiSamplerTof.cxx)
set(DEPENDENCIES
  ${DEPENDENCIES}
  ${FAIR_LIBS}
  ${BOOST_LIBS}
  fles_ipc
  CbmMQBase
)
GENERATE_EXECUTABLE()

set(EXE_NAME TsaInfo)
set(SRCS CbmMQTsaInfo.cxx runTsaInfo.cxx)
set(DEPENDENCIES
  ${DEPENDENCIES}
  ${FAIR_LIBS}
  ${BOOST_LIBS}
  fles_ipc
  CbmMQBase
)
GENERATE_EXECUTABLE()

set(EXE_NAME StsDigiSource)
set(SRCS CbmStsDigiSource.cxx runStsDigiSource.cxx)
set(DEPENDENCIES
  ${DEPENDENCIES}
  ${FAIR_LIBS}
  ${BOOST_LIBS}
  CbmBase
  Core
  CbmMQBase
)
GENERATE_EXECUTABLE()

set(EXE_NAME MCPointSource)
set(SRCS CbmMCPointSource.cxx runMCPointSource.cxx)
set(DEPENDENCIES
  ${DEPENDENCIES}
  ${FAIR_LIBS}
  ${BOOST_LIBS}
  CbmBase
  Core
  CbmMQBase
)
GENERATE_EXECUTABLE()
