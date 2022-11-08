configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startMQSamplerMonitor.sh.in ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startMQSamplerMonitor.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startMQSamplerT0Monitor2020.sh.in ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startMQSamplerT0Monitor2020.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startMQSamplerTofMonitor2020.sh.in ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startMQSamplerTofMonitor2020.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startMQSamplerT0Monitor2021.sh.in ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startMQSamplerT0Monitor2021.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startMQSamplerTofMonitor2021.sh.in ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startMQSamplerTofMonitor2021.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startMQSamplerPsdMonitor2021.sh.in ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startMQSamplerPsdMonitor2021.sh)


set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )


# Set the install path within the build directory
set(EXECUTABLE_OUTPUT_PATH "${EXECUTABLE_OUTPUT_PATH}/MQ/monitor")
# Set the install path within the installation directory
set(BIN_DESTINATION bin/MQ/monitor)


If(UNIX AND NOT APPLE)
  List(APPEND BOOST_LIBS pthread)
EndIf()

set(PUBLIC_DEPS
  CbmMQBase
  ROOT::Core
  )

set(PRIVATE_DEPS
  CbmFlibFlesTools
  CbmFlibMcbm2018
  FairMQ::Tools
  FairRoot::ParBase
  Boost::serialization
  Boost::program_options
  ROOT::Gpad
  ROOT::Hist
  ROOT::RIO
  )

set(INTERFACE_DEPS
  FairMQ::FairMQ
  external::fles_ipc
  )



set(EXE_NAME T0MonitorMcbm2018)
set(SRCS CbmDeviceMonitorT0.cxx runMonitorT0.cxx)

set(PUBLIC_DEPENDENCIES ${PUBLIC_DEPS}) 
set(PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}) 
set(INTERFACE_DEPENDENCIES ${INTERFACE_DEPS}) 

generate_cbm_executable()

set(EXE_NAME TofMonitorMcbm2018)
set(SRCS CbmDeviceMonitorTof.cxx runMonitorTof.cxx)

set(PUBLIC_DEPENDENCIES  ${PUBLIC_DEPS}) 
set(PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}) 
set(INTERFACE_DEPENDENCIES ${INTERFACE_DEPS}) 

generate_cbm_executable()

set(EXE_NAME T0MonitorReqMcbm)
set(SRCS CbmDeviceMonitorReqT0.cxx runMonitorReqT0.cxx)

set(PUBLIC_DEPENDENCIES ${PUBLIC_DEPS}) 
set(PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}) 
set(INTERFACE_DEPENDENCIES ${INTERFACE_DEPS}) 

generate_cbm_executable()

set(EXE_NAME TofMonitorReqMcbm)
set(SRCS CbmDeviceMonitorReqTof.cxx runMonitorReqTof.cxx)

set(PUBLIC_DEPENDENCIES ${PUBLIC_DEPS}) 
set(PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}) 
set(INTERFACE_DEPENDENCIES ${INTERFACE_DEPS}) 

generate_cbm_executable()

set(EXE_NAME PsdMonitorMcbm2018)
set(SRCS CbmDeviceMonitorPsd.cxx runMonitorPsd.cxx)

set(PUBLIC_DEPENDENCIES ${PUBLIC_DEPS}) 
set(PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}) 
set(INTERFACE_DEPENDENCIES ${INTERFACE_DEPS}) 

generate_cbm_executable()

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
configure_file(${MY_SOURCE_DIR}/startMQSamplerMonitor.sh.in        ${TMPDIR}/bin/MQ/topologies/install/startMQSamplerMonitor.sh)
configure_file(${MY_SOURCE_DIR}/startMQSamplerT0Monitor2020.sh.in  ${TMPDIR}/bin/MQ/topologies/install/startMQSamplerT0Monitor2020.sh)
configure_file(${MY_SOURCE_DIR}/startMQSamplerTofMonitor2020.sh.in ${TMPDIR}/bin/MQ/topologies/install/startMQSamplerTofMonitor2020.sh)
configure_file(${MY_SOURCE_DIR}/startMQSamplerT0Monitor2021.sh.in  ${TMPDIR}/bin/MQ/topologies/install/startMQSamplerT0Monitor2021.sh)
configure_file(${MY_SOURCE_DIR}/startMQSamplerTofMonitor2021.sh.in ${TMPDIR}/bin/MQ/topologies/install/startMQSamplerTofMonitor2021.sh)
configure_file(${MY_SOURCE_DIR}/startMQSamplerPsdMonitor2021.sh.in ${TMPDIR}/bin/MQ/topologies/install/startMQSamplerPsdMonitor2021.sh)

install(PROGRAMS ${TMPDIR}/bin/MQ/topologies/install/startMQSamplerMonitor.sh
                 ${TMPDIR}/bin/MQ/topologies/install/startMQSamplerT0Monitor2020.sh
                 ${TMPDIR}/bin/MQ/topologies/install/startMQSamplerTofMonitor2020.sh
                 ${TMPDIR}/bin/MQ/topologies/install/startMQSamplerT0Monitor2021.sh
                 ${TMPDIR}/bin/MQ/topologies/install/startMQSamplerTofMonitor2021.sh
                 ${TMPDIR}/bin/MQ/topologies/install/startMQSamplerPsdMonitor2021.sh
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/MQ/topologies
       )
