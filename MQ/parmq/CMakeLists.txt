set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

# Set the install path within the build directory
set(EXECUTABLE_OUTPUT_PATH "${EXECUTABLE_OUTPUT_PATH}/MQ/parmq")
# Set the install path within the installation directory
set(BIN_DESTINATION bin/MQ/parmq)


If(UNIX AND NOT APPLE)
  List(APPEND BOOST_LIBS pthread)
EndIf()


set(EXE_NAME parmq-server)
set(SRCS ParameterMQServer.cxx runParameterMQServer.cxx
#  CbmMQTestContFact.cxx
  )
set(PRIVATE_DEPENDENCIES
  CbmMQBase
  CbmSimSteer
  FairMQ::Tools
  FairRoot::ParBase
  ROOT::Core
  ROOT::Geom
  ROOT::Net
  )

set(INTERFACE_DEPENDENCIES
  FairMQ::FairMQ
  )

generate_cbm_executable()
