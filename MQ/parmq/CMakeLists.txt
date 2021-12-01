Set(INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/MQ/base
    ${CBMDATA_DIR} # For CbmDefs, needed by CbmSetup
    ${CBMROOT_SOURCE_DIR}/sim/transport/steer # For CbmSetup.h!
    ${CBMROOT_SOURCE_DIR}/sim/transport/geosetup # For CbmGeoSetupDbProvider.h, needed by CbmSetup
)

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${SYSTEM_INCLUDE_DIRECTORIES}
  ${FAIRROOT_INCLUDE_DIR}
  ${FAIRMQ_INCLUDE_DIR}
  ${FAIRMQ_INCLUDE_DIR}/options
  ${Boost_INCLUDE_DIR}
  ${ZeroMQ_INCLUDE_DIR}
  ${FAIRLOGGER_INCLUDE_DIR} # For CbmSetup.h!
)

Include_Directories(${INCLUDE_DIRECTORIES})
Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

Set(LINK_DIRECTORIES
  ${Boost_LIBRARY_DIRS}
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
)

Link_Directories(${LINK_DIRECTORIES})

set(EXECUTABLE_OUTPUT_PATH "${EXECUTABLE_OUTPUT_PATH}/MQ/parmq")

Set(BOOST_LIBS
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_SERIALIZATION_LIBRARY}
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  ${Boost_LOG_LIBRARY}
  RIO
)
If(UNIX AND NOT APPLE)
  List(APPEND BOOST_LIBS pthread)
EndIf()

set(FAIR_LIBS
  Base
  ParBase
  ${FAIRMQ_LIBS}
  Gen
)

If(FAIRLOGGER_FOUND)
  set(FAIR_LIBS
      ${FAIR_LIBS}
      FairLogger
     )
EndIf()

Set(EXE_NAME parmq-server)
Set(SRCS
  ParameterMQServer.cxx
  runParameterMQServer.cxx
#  CbmMQTestContFact.cxx
)

Set(DEPENDENCIES
  ${DEPENDENCIES}
  ${FAIR_LIBS}
  ${BOOST_LIBS}
  Core
  Net
  Geom
  CbmBase
  CbmData
  CbmField
  CbmTofBase
  CbmStsBase
  CbmStsSim
  CbmSimBase
  CbmSimSteer # for CbmSetup!
  CbmFlibMcbm2018
)
GENERATE_EXECUTABLE()
