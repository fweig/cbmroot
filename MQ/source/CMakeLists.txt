configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startMQInfo.sh.in ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startMQInfo.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startMQSampler.sh.in ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startMQSampler.sh)

set(INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/MQ/base
    ${CBMROOT_SOURCE_DIR}/fles/flestools
    ${CBMBASE_DIR}
    ${CBMDATA_DIR}
    ${CBMDATA_DIR}/base
    ${CBMDATA_DIR}/mvd
    ${CBMDATA_DIR}/sts
    ${CBMDATA_DIR}/rich
    ${CBMDATA_DIR}/much
    ${CBMDATA_DIR}/trd
    ${CBMDATA_DIR}/tof
    ${CBMDATA_DIR}/psd
)

Set(SYSTEM_INCLUDE_DIRECTORIES
    ${BASE_INCLUDE_DIRECTORIES}
    ${SYSTEM_INCLUDE_DIRECTORIES}
    ${ZeroMQ_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR}
    ${FAIRROOT_INCLUDE_DIR}
    ${FAIRMQ_INCLUDE_DIR}
    ${FAIRMQ_INCLUDE_DIR}/options
    ${VMC_INCLUDE_DIRS}
    ${FAIRLOGGER_INCLUDE_DIR}

    ${FLES_IPC_INCLUDE_DIRECTORY}
    ${FLES_LOGGING_INCLUDE_DIRECTORY}
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

# Set the install path within the build directory
set(EXECUTABLE_OUTPUT_PATH "${EXECUTABLE_OUTPUT_PATH}/MQ/source")
# Set the install path within the installation directory
set(BIN_DESTINATION bin/MQ/source)

Set(BOOST_LIBS
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SERIALIZATION_LIBRARY}
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  ${Boost_LOG_LIBRARY}
  ${Boost_LOG_SETUP_LIBRARY}
  ${Boost_REGEX_LIBRARY}
  ${Boost_THREAD_LIBRARY}
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

# Dependencies common to all executables
set(DEPENDENCIES_ALL
  ${DEPENDENCIES}
  ${FAIR_LIBS}
  ${BOOST_LIBS}
)

set(EXE_NAME TsaSampler)
set(SRCS CbmMQTsaSampler.cxx runTsaSampler.cxx)
set(DEPENDENCIES
  external::fles_ipc
  ${DEPENDENCIES_ALL}
  CbmMQBase
)
GENERATE_EXECUTABLE()

set(EXE_NAME MultiTsaSampler)
set(SRCS CbmMQTsaMultiSampler.cxx runTsaMultiSampler.cxx)
set(DEPENDENCIES
  external::fles_ipc
  CbmFlibFlesTools
  ${DEPENDENCIES_ALL}
  CbmMQBase
  Core
  Gpad
  Hist
  Net
  RHTTP
  RIO
)
GENERATE_EXECUTABLE()

set(EXE_NAME RepReqTsSampler)
set(SRCS CbmMQTsSamplerRepReq.cxx runTsSamplerRepReq.cxx)
set(DEPENDENCIES
  external::fles_ipc
  CbmFlibFlesTools
  ${DEPENDENCIES_ALL}
  CbmMQBase
  Core
  Gpad
  Hist
  Net
  RHTTP
  RIO
)
GENERATE_EXECUTABLE()


set(EXE_NAME TsaSamplerTof)
set(SRCS CbmMQTsaSamplerTof.cxx runTsaSamplerTof.cxx)
set(DEPENDENCIES
  ${DEPENDENCIES_ALL}
  external::fles_ipc
  CbmMQBase
)
GENERATE_EXECUTABLE()

set(EXE_NAME TsaMultiSamplerTof)
set(SRCS CbmMQTsaMultiSamplerTof.cxx runTsaMultiSamplerTof.cxx)
set(DEPENDENCIES
  external::fles_ipc
  ${DEPENDENCIES_ALL}
  CbmMQBase
)
GENERATE_EXECUTABLE()

set(EXE_NAME TsaInfo)
set(SRCS CbmMQTsaInfo.cxx runTsaInfo.cxx)
set(DEPENDENCIES
  ${DEPENDENCIES_ALL}
  external::fles_ipc
  CbmMQBase
)
GENERATE_EXECUTABLE()

set(EXE_NAME StsDigiSource)
set(SRCS CbmStsDigiSource.cxx runStsDigiSource.cxx)
set(DEPENDENCIES
  ${DEPENDENCIES_ALL}
  CbmBase
  Core
  CbmMQBase
)
GENERATE_EXECUTABLE()

set(EXE_NAME MCPointSource)
set(SRCS CbmMCPointSource.cxx runMCPointSource.cxx)
set(DEPENDENCIES
  ${DEPENDENCIES_ALL}
  CbmBase
  Core
  CbmMQBase
)
GENERATE_EXECUTABLE()

set(EXE_NAME TsConsumerReqExample)
set(SRCS CbmTsConsumerReqDevExample.cxx runTsConsumerReqExample.cxx)
set(DEPENDENCIES
  ${DEPENDENCIES_ALL}
  external::fles_ipc
  CbmMQBase
  CbmFlibFlesTools
  Core
  Gpad
  Hist
  Net
  RHTTP
  RIO
)
GENERATE_EXECUTABLE()

# Install scripts and input file
if(EXISTS ${VMCWORKDIR}/input/tofget4_hd2018.tsa)
  install(FILES ${VMCWORKDIR}/input/tofget4_hd2018.tsa
          DESTINATION ${CMAKE_INSTALL_PREFIX}/share/cbmroot/input
         )
endif()

# Set the correct variables for the installation
set(VMCWORKDIR ${CMAKE_INSTALL_PREFIX}/share/cbmroot)

set(MY_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CURRENT_SOURCE_DIR ${VMCWORKDIR}/input)

set(TMPDIR "${CMAKE_BINARY_DIR}")
set(CMAKE_BINARY_DIR ${CMAKE_INSTALL_PREFIX})

# Configure file for installation directory
configure_file(${MY_SOURCE_DIR}/startMQInfo.sh.in ${TMPDIR}/bin/MQ/topologies/install/startMQInfo.sh)
configure_file(${MY_SOURCE_DIR}/startMQSampler.sh.in ${TMPDIR}/bin/MQ/topologies/install/startMQSampler.sh)

install(PROGRAMS ${TMPDIR}/bin/MQ/topologies/install/startMQInfo.sh
                 ${TMPDIR}/bin/MQ/topologies/install/startMQSampler.sh
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/MQ/topologies
       )
