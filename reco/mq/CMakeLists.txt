If(FairSoft_VERSION VERSION_LESS 18.6.0)
  Add_Definitions(-DHAVE_FAIRMQSTATEMACHINE)
EndIf()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startUnpack.sh.in ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startUnpack.sh)

set(INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}
  )

#Set the install path within the build directory
set(EXECUTABLE_OUTPUT_PATH "${EXECUTABLE_OUTPUT_PATH}/mq")
#Set the install path within the installation directory
set(BIN_DESTINATION bin/reco/mq)



If(UNIX AND NOT APPLE)
  List(APPEND BOOST_LIBS pthread)
EndIf()


set(PUBLIC_DEPS
  CbmData
  CbmMQBase
  FairRoot::ParBase
  FairMQ::Tools
  ROOT::Core
  )

set(PRIVATE_DEPS
  FairRoot::Base
  Algo
  )

set(INTERFACE_DEPENDENCIES
  external::fles_ipc
  )


set(EXE_NAME MqDevUnpack)
set(SRCS CbmDevUnpack.cxx runUnpack.cxx)

set(PUBLIC_DEPENDENCIES ${PUBLIC_DEPS}) 
set(PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}) 
set(INTERFACE_DEPENDENCIES ${INTERFACE_DEPS})

generate_cbm_executable()


set(EXE_NAME MqDevTrigger)
set(SRCS CbmDevTrigger.cxx runTrigger.cxx)

set(PUBLIC_DEPENDENCIES ${PUBLIC_DEPS}) 
set(PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}) 
set(INTERFACE_DEPENDENCIES ${INTERFACE_DEPS})

generate_cbm_executable()


set(EXE_NAME MqDevBuildEvents)
set(SRCS CbmDevBuildEvents.cxx runBuildEvents.cxx)

set(PUBLIC_DEPENDENCIES ${PUBLIC_DEPS}) 
set(PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}) 
set(INTERFACE_DEPENDENCIES ${INTERFACE_DEPS})

generate_cbm_executable()


set(EXE_NAME MqDevEventSink)
set(SRCS CbmDevEventSink.cxx runEventSink.cxx)

set(PUBLIC_DEPENDENCIES ${PUBLIC_DEPS}) 
set(PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}) 
set(INTERFACE_DEPENDENCIES ${INTERFACE_DEPS})

generate_cbm_executable()

#Set the correct variables for the installation
set(VMCWORKDIR ${CMAKE_INSTALL_PREFIX}/share/cbmroot)

set(MY_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CURRENT_SOURCE_DIR ${VMCWORKDIR}/input)

set(TMPDIR "${CMAKE_BINARY_DIR}")
set(CMAKE_BINARY_DIR ${CMAKE_INSTALL_PREFIX})

#Configure file for installation directory
configure_file(${MY_SOURCE_DIR}/startUnpack.sh.in              ${TMPDIR}/bin/MQ/topologies/install/startUnpack.sh)

install(PROGRAMS ${TMPDIR}/bin/MQ/topologies/install/startUnpack.sh
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/MQ/topologies
       )
