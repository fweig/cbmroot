configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startMQInfo.sh.in ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startMQInfo.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startMQSampler.sh.in ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startMQSampler.sh)

set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

# Set the install path within the build directory
set(EXECUTABLE_OUTPUT_PATH "${EXECUTABLE_OUTPUT_PATH}/MQ/source")
# Set the install path within the installation directory
set(BIN_DESTINATION bin/MQ/source)



If(UNIX AND NOT APPLE)
  List(APPEND BOOST_LIBS pthread)
EndIf()

set(PUBLIC_DEPS
  CbmMQBase
  )

set(PRIVATE_DEPS
  CbmBase
  CbmData
  CbmFlibFlesTools
  external::fles_logging
  cppzmq
  FairMQ::Tools
  FairRoot::Base
  FairRoot::ParBase
  )

set(INTERFACE_DEPS
  FairMQ::FairMQ
  )

set(EXE_NAME TsaSampler)
set(SRCS CbmMQTsaSampler.cxx runTsaSampler.cxx)
set(PUBLIC_DEPENDENCIES ${PUBLIC_DEPS}) 
set(PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}) 
set(INTERFACE_DEPENDENCIES ${INTERFACE_DEPS})

  generate_cbm_executable()

set(EXE_NAME MultiTsaSampler)
set(SRCS CbmMQTsaMultiSampler.cxx runTsaMultiSampler.cxx)
set(PUBLIC_DEPENDENCIES ${PUBLIC_DEPS}) 
set(PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}) 
set(INTERFACE_DEPENDENCIES ${INTERFACE_DEPS})

  generate_cbm_executable()

set(EXE_NAME RepReqTsSampler)
set(SRCS CbmMQTsSamplerRepReq.cxx runTsSamplerRepReq.cxx)
set(PUBLIC_DEPENDENCIES ${PUBLIC_DEPS}) 
set(PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}) 
set(INTERFACE_DEPENDENCIES ${INTERFACE_DEPS})

  generate_cbm_executable()


set(EXE_NAME TsaSamplerTof)
set(SRCS CbmMQTsaSamplerTof.cxx runTsaSamplerTof.cxx)
set(PUBLIC_DEPENDENCIES ${PUBLIC_DEPS}) 
set(PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}) 
set(INTERFACE_DEPENDENCIES ${INTERFACE_DEPS})

  generate_cbm_executable()

set(EXE_NAME TsaMultiSamplerTof)
set(SRCS CbmMQTsaMultiSamplerTof.cxx runTsaMultiSamplerTof.cxx)
set(PUBLIC_DEPENDENCIES ${PUBLIC_DEPS}) 
set(PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}) 
set(INTERFACE_DEPENDENCIES ${INTERFACE_DEPS})

  generate_cbm_executable()

set(EXE_NAME TsaInfo)
set(SRCS CbmMQTsaInfo.cxx runTsaInfo.cxx)
set(PUBLIC_DEPENDENCIES ${PUBLIC_DEPS}) 
set(PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}) 
set(INTERFACE_DEPENDENCIES ${INTERFACE_DEPS})

  generate_cbm_executable()

set(EXE_NAME StsDigiSource)
set(SRCS CbmStsDigiSource.cxx runStsDigiSource.cxx)
set(PUBLIC_DEPENDENCIES ${PUBLIC_DEPS}) 
set(PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}) 
set(INTERFACE_DEPENDENCIES ${INTERFACE_DEPS})

  generate_cbm_executable()

set(EXE_NAME MCPointSource)
set(SRCS CbmMCPointSource.cxx runMCPointSource.cxx)
set(PUBLIC_DEPENDENCIES ${PUBLIC_DEPS}) 
set(PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}) 
set(INTERFACE_DEPENDENCIES ${INTERFACE_DEPS})

  generate_cbm_executable()

set(EXE_NAME TsConsumerReqExample)
set(SRCS CbmTsConsumerReqDevExample.cxx runTsConsumerReqExample.cxx)
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
configure_file(${MY_SOURCE_DIR}/startMQInfo.sh.in ${TMPDIR}/bin/MQ/topologies/install/startMQInfo.sh)
configure_file(${MY_SOURCE_DIR}/startMQSampler.sh.in ${TMPDIR}/bin/MQ/topologies/install/startMQSampler.sh)

install(PROGRAMS ${TMPDIR}/bin/MQ/topologies/install/startMQInfo.sh
                 ${TMPDIR}/bin/MQ/topologies/install/startMQSampler.sh
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/MQ/topologies
       )
