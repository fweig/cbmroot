configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startMQMcbmPulserMonitor2020.sh.in   ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startMQMcbmPulserMonitor2020.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startMQMcbmEvtBuilderWin2020.sh.in   ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startMQMcbmEvtBuilderWin2020.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startMQBuildRawEvents.sh.in          ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startMQBuildRawEvents.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startBuildRawEvents2021.sh.in        ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startBuildRawEvents2021.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startBuildRawEventsCosmics2021.sh.in ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startBuildRawEventsCosmics2021.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startBuildRawEvents2022.sh.in        ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startBuildRawEvents2022.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startBmonMoni2022.sh.in              ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startBmonMoni2022.sh)

set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

# Set the install path within the build directory
set(EXECUTABLE_OUTPUT_PATH "${EXECUTABLE_OUTPUT_PATH}/MQ/mcbm")
# Set the install path within the installation directory
set(BIN_DESTINATION bin/MQ/mcbm)


If(UNIX AND NOT APPLE)
  List(APPEND BOOST_LIBS pthread)
EndIf()

set(PUBLIC_DEPS
  CbmData
  CbmFlibMcbm2018
  CbmMQBase
  FairRoot::ParBase
  ROOT::Core
  Boost::program_options
  L1
  )

set(PRIVATE_DEPS
  CbmFlibFlesTools
  CbmMuchReco
  CbmPsdReco
  CbmRecoSts
  CbmRichReco
  CbmSimSteer
  CbmTofReco
  CbmTrdReco
  CbmEventBuilder
  FairMQ::Tools
  FairRoot::Base
  ROOT::Gpad
  ROOT::Hist
  ROOT::RIO
  )

set(INTERFACE_DEPS
  FairMQ::FairMQ
  external::fles_ipc
  )

set(EXE_NAME McbmUnpack)
set(SRCS CbmDeviceMcbmUnpack.cxx runMcbmUnpack.cxx)

set(PUBLIC_DEPENDENCIES ${PUBLIC_DEPS}) 
set(PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}) 
set(INTERFACE_DEPENDENCIES ${INTERFACE_DEPS})

generate_cbm_executable()

set(EXE_NAME McbmMonitorPulser)
set(SRCS CbmDeviceMcbmMonitorPulser.cxx runMcbmMonitorPulser.cxx)

set(PUBLIC_DEPENDENCIES ${PUBLIC_DEPS}) 
set(PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}) 
set(INTERFACE_DEPENDENCIES ${INTERFACE_DEPS})

generate_cbm_executable()

set(EXE_NAME BuildRawEvents)
set(SRCS CbmDeviceBuildRawEvents.cxx runBuildRawEvents.cxx)

set(PUBLIC_DEPENDENCIES ${PUBLIC_DEPS}) 
set(PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}) 
set(INTERFACE_DEPENDENCIES ${INTERFACE_DEPS})

generate_cbm_executable()


set(EXE_NAME McbmEventBuilderWin)
set(SRCS CbmDeviceMcbmEventBuilderWin.cxx runMcbmEvtBuilderWin.cxx)

set(PUBLIC_DEPENDENCIES ${PUBLIC_DEPS}) 
set(PRIVATE_DEPENDENCIES "${PRIVATE_DEPS};FairRoot::Online")
set(INTERFACE_DEPENDENCIES ${INTERFACE_DEPS})

generate_cbm_executable()

set(EXE_NAME McbmEventSink)
set(SRCS CbmDeviceMcbmEventSink.cxx runMcbmEventSink.cxx)

set(PUBLIC_DEPENDENCIES ${PUBLIC_DEPS}) 
set(PRIVATE_DEPENDENCIES "${PRIVATE_DEPS};FairRoot::Online")
set(INTERFACE_DEPENDENCIES ${INTERFACE_DEPS})

generate_cbm_executable()


set(EXE_NAME MqUnpack)
set(SRCS CbmDeviceUnpack.cxx runUnpack.cxx)

set(PUBLIC_DEPENDENCIES ${PUBLIC_DEPS}) 
set(PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}) 
set(INTERFACE_DEPENDENCIES ${INTERFACE_DEPS})

generate_cbm_executable()

set(EXE_NAME BuildDigiEvents)
set(SRCS CbmDeviceBuildDigiEvents.cxx runBuildDigiEvents.cxx)

set(PUBLIC_DEPENDENCIES ${PUBLIC_DEPS}) 
set(PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}) 
set(INTERFACE_DEPENDENCIES ${INTERFACE_DEPS})

generate_cbm_executable()

set(EXE_NAME DigiEventSink)
set(SRCS CbmDeviceDigiEventSink.cxx runDigiEventSink.cxx)

set(PUBLIC_DEPENDENCIES ${PUBLIC_DEPS}) 
set(PRIVATE_DEPENDENCIES "${PRIVATE_DEPS};FairRoot::Online")
set(INTERFACE_DEPENDENCIES ${INTERFACE_DEPS})

generate_cbm_executable()


set(EXE_NAME BmonMonitor)
set(SRCS CbmDeviceBmonMonitor.cxx runBmonMonitor.cxx)

set(PUBLIC_DEPENDENCIES ${PUBLIC_DEPS}) 
set(PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}) 
set(INTERFACE_DEPENDENCIES ${INTERFACE_DEPS})

generate_cbm_executable()

# Set the correct variables for the installation
set(VMCWORKDIR ${CMAKE_INSTALL_PREFIX}/share/cbmroot)

set(MY_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CURRENT_SOURCE_DIR ${VMCWORKDIR}/input)

set(TMPDIR "${CMAKE_BINARY_DIR}")
set(CMAKE_BINARY_DIR ${CMAKE_INSTALL_PREFIX})

# Configure file for installation directory
configure_file(${MY_SOURCE_DIR}/startMQMcbmPulserMonitor2020.sh.in   ${TMPDIR}/bin/MQ/topologies/install/startMQMcbmPulserMonitor2020.sh)
configure_file(${MY_SOURCE_DIR}/startMQMcbmEvtBuilderWin2020.sh.in   ${TMPDIR}/bin/MQ/topologies/install/startMQMcbmEvtBuilderWin2020.sh)
configure_file(${MY_SOURCE_DIR}/startMQBuildRawEvents.sh.in          ${TMPDIR}/bin/MQ/topologies/install/startMQBuildRawEvents.sh)
configure_file(${MY_SOURCE_DIR}/startBuildRawEvents2021.sh.in        ${TMPDIR}/bin/MQ/topologies/install/startBuildRawEvents2021.sh)
configure_file(${MY_SOURCE_DIR}/startBuildRawEventsCosmics2021.sh.in ${TMPDIR}/bin/MQ/topologies/install/startBuildRawEventsCosmics2021.sh)
configure_file(${MY_SOURCE_DIR}/startBmonMoni2022.sh.in              ${TMPDIR}/bin/MQ/topologies/install/startBmonMoni2022.sh)

install(PROGRAMS ${TMPDIR}/bin/MQ/topologies/install/startMQMcbmPulserMonitor2020.sh
                 ${TMPDIR}/bin/MQ/topologies/install/startMQMcbmEvtBuilderWin2020.sh
                 ${TMPDIR}/bin/MQ/topologies/install/startMQBuildRawEvents.sh
                 ${TMPDIR}/bin/MQ/topologies/install/startBuildRawEvents2021.sh
                 ${TMPDIR}/bin/MQ/topologies/install/startBuildRawEventsCosmics2021.sh
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/MQ/topologies
       )
