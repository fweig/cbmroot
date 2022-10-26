configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startMQSamplerUnpackerParserverHitBuilder.sh.in ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startMQSamplerUnpackerParserverHitBuilder.sh)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startMQ_Mcbm.sh.in ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startMQ_Mcbm.sh)

set(INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

# Set the install path within the build directory
set(EXECUTABLE_OUTPUT_PATH "${EXECUTABLE_OUTPUT_PATH}/MQ/hitbuilder")
# Set the install path within the installation directory
set(BIN_DESTINATION bin/MQ/hitbuilder)


If(UNIX AND NOT APPLE)
  List(APPEND BOOST_LIBS pthread)
EndIf()


set(EXE_NAME HitBuilderTof)
set(SRCS CbmDeviceHitBuilderTof.cxx runHitBuilderTof.cxx)
set(PUBLIC_DEPENDENCIES
  CbmData
  CbmTofBase
  CbmMQBase
  FairRoot::Base
  ROOT::Geom

  )

set(PRIVATE_DEPENDENCIES
  CbmBase
  CbmTofReco  
  FairMQ::Tools
  FairRoot::ParBase
  ROOT::Core
  ROOT::Graf
  ROOT::Hist
  ROOT::MathCore
  ROOT::Minuit
  ROOT::Physics
  )

set(INTERFACE_DEPENDENCIES
  FairMQ::FairMQ
  external::fles_ipc
  ROOT::Net
  ROOT::RIO
  ROOT::Tree
  )

generate_cbm_executable()

# Set the correct variables for the installation
set(VMCWORKDIR ${CMAKE_INSTALL_PREFIX}/share/cbmroot)

set(MY_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CURRENT_SOURCE_DIR ${VMCWORKDIR}/input)

set(TMPDIR "${CMAKE_BINARY_DIR}")
set(CMAKE_BINARY_DIR ${CMAKE_INSTALL_PREFIX})

# Configure file for installation directory
configure_file(${MY_SOURCE_DIR}/startMQSamplerUnpackerParserverHitBuilder.sh.in ${TMPDIR}/bin/MQ/topologies/install/startMQSamplerUnpackerParserverHitBuilder.sh)
configure_file(${MY_SOURCE_DIR}/startMQ_Mcbm.sh.in                      ${TMPDIR}/bin/MQ/topologies/install/startMQ_Mcbm.sh)

install(PROGRAMS ${TMPDIR}/bin/MQ/topologies/install/startMQSamplerUnpackerParserverHitBuilder.sh
                 ${TMPDIR}/bin/MQ/topologies/install/startMQ_Mcbm.sh
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/MQ/topologies
       )

