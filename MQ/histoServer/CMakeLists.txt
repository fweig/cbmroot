 ################################################################################
 #    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    #
 #                                                                              #
 #              This software is distributed under the terms of the             #
 #              GNU Lesser General Public Licence (LGPL) version 3,             #
 #                  copied verbatim in the file "LICENSE"                       #
 ################################################################################

set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
)

If(UNIX AND NOT APPLE)
  List(APPEND BOOST_LIBS pthread)
EndIf()

# Set the install path within the build directory
set(EXECUTABLE_OUTPUT_PATH "${EXECUTABLE_OUTPUT_PATH}/MQ/histogramServer")
# Set the install path within the installation directory
set(BIN_DESTINATION bin/MQ/histogramServer)


set(PUBLIC_DEPS
  ROOT::Core
  ROOT::RHTTP
  )

set(PRIVATE_DEPS
  CbmFlibFlesTools
  FairRoot::Base
  FairMQ::Tools
  FairMQ::FairMQ
  ROOT::Gpad
  ROOT::Hist
  ROOT::Net
  ROOT::RIO
  )
    
set(EXE_NAME HistoServer)
set(SRCS CbmHistoServer.cxx runCbmHistoServer.cxx)

set(PUBLIC_DEPENDENCIES ${PUBLIC_DEPS}) 
set(PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}) 
set(INTERFACE_DEPENDENCIES ${INTERFACE_DEPS})

generate_cbm_executable()

set(EXE_NAME MqHistoServer)
set(SRCS CbmMqHistoServer.cxx runCbmMqHistoServer.cxx)

set(PUBLIC_DEPENDENCIES ${PUBLIC_DEPS}) 
set(PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}) 
set(INTERFACE_DEPENDENCIES ${INTERFACE_DEPS})

generate_cbm_executable()
