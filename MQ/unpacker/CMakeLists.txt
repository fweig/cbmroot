configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startMQSamplerUnpackerParserver.sh.in ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startMQSamplerUnpackerParserver.sh)

set(INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CBMROOT_SOURCE_DIR}/MQ/base
#    ${CBMROOT_SOURCE_DIR}/fles/mcbm2018/parameter
    ${CBMROOT_SOURCE_DIR}/fles/mcbm2018/dataformat
    ${CBMROOT_SOURCE_DIR}/fles/mcbm2018/unpacker
    ${CBMROOT_SOURCE_DIR}/fles/mcbm2018/commonMQ

    ${CBMDATA_DIR}
    ${CBMDATA_DIR}/raw
    ${CBMDATA_DIR}/tof

    ${CBMBASE_DIR}

    ${CBMDETECTORBASE_DIR}/tof
)

Set(SYSTEM_INCLUDE_DIRECTORIES
    ${SYSTEM_INCLUDE_DIRECTORIES}
    ${ZeroMQ_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR}
    ${FAIRROOT_INCLUDE_DIR}
    ${FAIRMQ_INCLUDE_DIR}
    ${FAIRMQ_INCLUDE_DIR}/options

    ${IPC_INCLUDE_DIRECTORY}
    ${CBMROOT_SOURCE_DIR}/external/cppzmq
)

include_directories(${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
)

link_directories(${LINK_DIRECTORIES})

set(EXECUTABLE_OUTPUT_PATH "${EXECUTABLE_OUTPUT_PATH}/MQ/unpacker")

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
  Base
  ParBase
  ${FAIRMQ_LIBS}
)

If(FAIRLOGGER_FOUND)
  set(FAIR_LIBS
      ${FAIR_LIBS}
      FairLogger
     )
EndIf()

set(EXE_NAME UnpackTofMcbm2018)
set(SRCS CbmTbDaqBuffer.cxx CbmDeviceUnpackTofMcbm2018.cxx runUnpackTofMcbm2018.cxx)

set(DEPENDENCIES
  ${DEPENDENCIES}
  ${FAIR_LIBS}
  ${BOOST_LIBS}
  fles_ipc
  CbmFlibMcbm2018
  CbmTofBase
  CbmBase
#  CbmBeamtimeBase
  CbmData
  Core
  RIO
  Net
  Hist
)

# set the install dir
set(BIN_DESTINATION bin/MQ/unpacker)
GENERATE_EXECUTABLE()

# Install script and input file(s)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/MapTofHdCosmics_v18d.par
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/cbmroot/input
       )

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
configure_file(${MY_SOURCE_DIR}/startMQSamplerUnpackerParserver.sh.in ${TMPDIR}/bin/MQ/topologies/install/startMQSamplerUnpackerParserver.sh)

install(PROGRAMS ${TMPDIR}/bin/MQ/topologies/install/startMQSamplerUnpackerParserver.sh
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/MQ/topologies
       )
