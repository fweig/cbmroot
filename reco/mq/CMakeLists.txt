If(FairSoft_VERSION VERSION_LESS 18.6.0)
  Add_Definitions(-DHAVE_FAIRMQSTATEMACHINE)
EndIf()

Set(FAIRMQ_LIBS FairMQStateMachine FairMQ)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startUnpack.sh.in ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startUnpack.sh)

set(INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/MQ/base
    ${CBMROOT_SOURCE_DIR}/algo
    ${CBMROOT_SOURCE_DIR}/algo/evbuild
    ${CBMROOT_SOURCE_DIR}/algo/trigger
    ${CBMROOT_SOURCE_DIR}/algo/detectors/sts
    ${CBMROOT_SOURCE_DIR}/fles/flestools
    ${CBMROOT_SOURCE_DIR}/reco/mq/
    ${CBMDATA_DIR}
    ${CBMDATA_DIR}/raw
    ${CBMDATA_DIR}/sts
    ${CBMDATA_DIR}/much
    ${CBMDATA_DIR}/rich
    ${CBMDATA_DIR}/tof
    ${CBMDATA_DIR}/psd
    ${CBMDATA_DIR}/trd
    ${CBMDATA_DIR}/mvd # Feint to avoid crash of DigiManager due to missing source pointer
    ${CBMDATA_DIR}/base
    ${CBMDATA_DIR}/global
)

Set(SYSTEM_INCLUDE_DIRECTORIES
    ${SYSTEM_INCLUDE_DIRECTORIES}
    ${ZeroMQ_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR}
    ${FAIRROOT_INCLUDE_DIR}
    ${FAIRMQ_INCLUDE_DIR}
    ${FAIRMQ_INCLUDE_DIR}/options
    ${FAIRLOGGER_INCLUDE_DIR}

    ${IPC_INCLUDE_DIRECTORY}
    ${CBMROOT_SOURCE_DIR}/external/cppzmq
)

include_directories(${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
  ${KFParticle_LIB_DIR}
  ${FAIRMQ_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
  ${ROOT_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
)

link_directories(${LINK_DIRECTORIES})

# Set the install path within the build directory
set(EXECUTABLE_OUTPUT_PATH "${EXECUTABLE_OUTPUT_PATH}/mq")
# Set the install path within the installation directory
set(BIN_DESTINATION bin/mq)

Set(BOOST_LIBS
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_SERIALIZATION_LIBRARY}
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  ${Boost_LOG_LIBRARY}
)
If(UNIX AND NOT APPLE)
  List(APPEND BOOST_LIBS pthread)
EndIf()

set(FAIR_LIBS
  ${FAIRMQ_LIBS}
)

If(FAIRLOGGER_FOUND)
  set(FAIR_LIBS
      ${FAIR_LIBS}
      FairLogger
     )
EndIf()

set(EXE_NAME MqDevUnpack)
set(SRCS CbmDevUnpack.cxx runUnpack.cxx)

set(DEPENDENCIES
  ${DEPENDENCIES}
  ${FAIR_LIBS}
  ${BOOST_LIBS}
  fles_ipc
  CbmFlibFlesTools
  Algo
  CbmBase
  CbmRecoBase
  CbmPsdReco
  CbmRichReco
  CbmRecoSts
  CbmTofReco
  CbmTrdReco
  CbmData
  CbmSimSteer # for CbmSetup!
  Core
)
GENERATE_EXECUTABLE()

# Set the correct variables for the installation
set(VMCWORKDIR ${CMAKE_INSTALL_PREFIX}/share/cbmroot)

set(MY_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CURRENT_SOURCE_DIR ${VMCWORKDIR}/input)

set(TMPDIR "${CMAKE_BINARY_DIR}")
set(CMAKE_BINARY_DIR ${CMAKE_INSTALL_PREFIX})

# Configure file for installation directory
configure_file(${MY_SOURCE_DIR}/startUnpack.sh.in              ${TMPDIR}/bin/MQ/topologies/install/startUnpack.sh)

install(PROGRAMS ${TMPDIR}/bin/MQ/topologies/install/startUnpack.sh
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/MQ/topologies
       )
