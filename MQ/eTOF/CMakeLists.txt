#configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startMQSamplerUnpackerParserver.sh.in ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startMQSamplerUnpackerParserver.sh)

set(INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CBMROOT_SOURCE_DIR}/MQ/base
    ${CBMROOT_SOURCE_DIR}/MQ/unpacker
    ${CBMROOT_SOURCE_DIR}/fles/cern2016/param
    ${CBMROOT_SOURCE_DIR}/fles/cern2016/unpacker
    ${CBMROOT_SOURCE_DIR}/fles/mcbm2018/parameter
    ${CBMROOT_SOURCE_DIR}/fles/mcbm2018/dataformat
    ${CBMROOT_SOURCE_DIR}/fles/mcbm2018/unpacker
    ${CBMROOT_SOURCE_DIR}/fles/mcbm2018/commonMQ
    ${CBMROOT_SOURCE_DIR}/fles/star2019/eventbuilder
    ${CBMROOT_SOURCE_DIR}/fles/star2019/parameter
    ${CBMROOT_SOURCE_DIR}/beamtime/base
    ${CBMDATA_DIR}
    ${CBMDATA_DIR}/tof
    ${CBMBASE_DIR}
)

Set(SYSTEM_INCLUDE_DIRECTORIES
    ${SYSTEM_INCLUDE_DIRECTORIES}
    ${ZeroMQ_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR}
    ${FAIRROOT_INCLUDE_DIR}
    ${FAIRMQ_INCLUDE_DIR}
    ${FAIRMQ_INCLUDE_DIR}/options

    ${FLES_IPC_INCLUDE_DIRECTORY}
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

set(EXECUTABLE_OUTPUT_PATH "${EXECUTABLE_OUTPUT_PATH}/MQ/eTOF")

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
  FairMQ
)

If(FAIRLOGGER_FOUND)
  set(FAIR_LIBS
      ${FAIR_LIBS}
      FairLogger
     )
EndIf()

# Dependencies common to all executables
set(DEPENDENCIES_ALL
  ${DEPENDENCIES}
  ${FAIR_LIBS}
  ${BOOST_LIBS}
)

set(EXE_NAME EventBuilderEtofStar2019)
set(SRCS CbmDeviceEventBuilderEtofStar2019.cxx runEventBuilderEtofStar2019.cxx)
set(DEPENDENCIES
  ${DEPENDENCIES_ALL}
  external::fles_ipc
  CbmFlibStar2019
  CbmFlibMcbm2018
  CbmBase
  CbmData
  Core
  RIO
  Net
  Hist
)
GENERATE_EXECUTABLE()

set(EXE_NAME TriggerHandlerEtof)
set(SRCS CbmDeviceTriggerHandlerEtof.cxx runTriggerHandlerEtof.cxx)
 Message(STATUS "Compiling with support for STAR DAQ Event writing")
Set(NO_DICT_SRCS
  # STAR sub-event writer source code
  ${CBMROOT_SOURCE_DIR}/fles/star2017/unpacker/star_rhicf.c
)
# Mask warning from file provided by STAR
SET_SOURCE_FILES_PROPERTIES(${CBMROOT_SOURCE_DIR}/fles/star2017/unpacker/star_rhicf.c PROPERTIES COMPILE_FLAGS -Wno-pointer-sign)
set(DEPENDENCIES
  ${DEPENDENCIES_ALL}
  fles_ipc
  external::fles_ipc
  CbmFlibStar2019
  CbmFlibMcbm2018
  CbmBase
  CbmData
  Core
  RIO
  Net
  Hist
)
GENERATE_EXECUTABLE()

