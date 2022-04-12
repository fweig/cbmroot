 ################################################################################
 #    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    #
 #                                                                              #
 #              This software is distributed under the terms of the             #
 #              GNU Lesser General Public Licence (LGPL) version 3,             #
 #                  copied verbatim in the file "LICENSE"                       #
 ################################################################################

Set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CBMROOT_SOURCE_DIR}/fles/flestools
)

Set(SYSTEM_INCLUDE_DIRECTORIES
    ${SYSTEM_INCLUDE_DIRECTORIES}
    ${Boost_INCLUDE_DIR}
    ${FAIRROOT_INCLUDE_DIR}
    ${FAIRMQ_INCLUDE_DIR}
    ${FAIRMQ_INCLUDE_DIR}/options
    ${FAIRLOGGER_INCLUDE_DIR}
)

Include_Directories(${INCLUDE_DIRECTORIES})
Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

Set(LINK_DIRECTORIES
  ${Boost_LIBRARY_DIRS}
  ${FAIRROOT_LIBRARY_DIR}
  ${ROOT_LIBRARY_DIR}
)

Link_Directories(${LINK_DIRECTORIES})

set(FAIR_LIBS
  FairMQ
)

If(FAIRLOGGER_FOUND)
  set(FAIR_LIBS
    ${FAIR_LIBS}
    FairLogger
  )
EndIf()


Set(BOOST_LIBS
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
)
If(UNIX AND NOT APPLE)
  List(APPEND BOOST_LIBS pthread)
EndIf()

# Dependencies common to all executables
set(DEPENDENCIES_ALL
  ${DEPENDENCIES}
  ${FAIR_LIBS}
  ${BOOST_LIBS}
)

# Set the install path within the build directory
set(EXECUTABLE_OUTPUT_PATH "${EXECUTABLE_OUTPUT_PATH}/MQ/histogramServer")
# Set the install path within the installation directory
set(BIN_DESTINATION bin/MQ/histogramServer)

set(EXE_NAME HistoServer)
set(SRCS CbmHistoServer.cxx runCbmHistoServer.cxx)
set(DEPENDENCIES
  ${DEPENDENCIES_ALL}
  external::fles_ipc
  Core
  RIO
  Net
  Hist
  RHTTP
)
GENERATE_EXECUTABLE()

set(EXE_NAME MqHistoServer)
set(SRCS CbmMqHistoServer.cxx runCbmMqHistoServer.cxx)
set(DEPENDENCIES
  ${DEPENDENCIES_ALL}
  CbmFlibFlesTools
  Core
  RIO
  Net
  Hist
  Gpad
  RHTTP
)
GENERATE_EXECUTABLE()
