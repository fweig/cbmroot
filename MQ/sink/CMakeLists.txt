configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startMQSamplerSink.sh.in ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startMQSamplerSink.sh)

set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )
  
# Set the install path within the build directory
set(EXECUTABLE_OUTPUT_PATH "${EXECUTABLE_OUTPUT_PATH}/MQ/sink")
# Set the install path within the installation directory
set(BIN_DESTINATION bin/MQ/sink)


If(UNIX AND NOT APPLE)
  List(APPEND BOOST_LIBS pthread)
EndIf()

set(PRIVATE_DEPS
  CbmMQBase
  FairMQ::Tools
  external::fles_ipc
  )

set(INTERFACE_DEPS
  FairMQ::FairMQ
  )

set(EXE_NAME DevNullSink)
set(SRCS CbmDevNullSink.cxx runDevNullSink.cxx)

set(PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}) 
set(INTERFACE_DEPENDENCIES ${INTERFACE_DEPS})

generate_cbm_executable()


set(EXE_NAME TsaComponentSink)
set(SRCS CbmTsaComponentSink.cxx runTsaComponentSink.cxx)

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
configure_file(${MY_SOURCE_DIR}/startMQSamplerSink.sh.in ${TMPDIR}/bin/MQ/topologies/install/startMQSamplerSink.sh)

install(PROGRAMS ${TMPDIR}/bin/MQ/topologies/install/startMQSamplerSink.sh
       DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/MQ/topologies
       )
