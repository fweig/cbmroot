# CMakeList file for library libSts.
# Last update: V. Friese, 17.02.2014


# -----   Specify library name   --------------------------
Set(LIBRARY_NAME CbmTaskToAlgoTutorial)
# ---------------------------------------------------------


# --- All sources
set(SRCS
  CbmStsHitProducerTaskIdeal.cxx
  CbmStsHitProducerIdealAlgo.cxx
  CbmStsHitProducerIdealWrapper.cxx
)

set(HEADERS
  CbmStsHitProducerTaskIdeal.h
  CbmStsHitProducerIdealWrapper.h
)

# -----  End of sources   ---------------------------------

# ----  Specify include directories -----------------------
set(INCLUDE_DIRECTORIES
  ${CBMROOT_SOURCE_DIR}/tutorials/TaskToAlgo
  ${CBMDATA_DIR}
  ${CBMDATA_DIR}/sts
  ${CBMDETECTORBASE_DIR}/trd
  ${CBMROOT_SOURCE_DIR}/MQ/base
)

set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES}
  ${ZeroMQ_INCLUDE_DIR}
  ${Boost_INCLUDE_DIR}
  ${FAIRROOT_INCLUDE_DIR}
  ${FAIRMQ_INCLUDE_DIR}
  ${FAIRMQ_INCLUDE_DIR}/options
)
# ----  End of include directories ------------------------


# ----  Specify link directories --------------------------
set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
  ${FAIRMQ_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
)
# -----  End of link directories   ------------------------


# -----   Specify library dependences   -------------------
Set(DEPENDENCIES
    CbmData CbmTrdBase Base
)
# ---------------------------------------------------------


# -----  Specify LinkDef file   ---------------------------
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
# ---------------------------------------------------------


# -----  Let cmake do the job   ---------------------------
include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})
link_directories( ${LINK_DIRECTORIES})
GENERATE_LIBRARY()
# ---------------------------------------------------------

set(EXECUTABLE_OUTPUT_PATH "${EXECUTABLE_OUTPUT_PATH}/MQ/sts")

Set(BOOST_LIBS
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_SERIALIZATION_LIBRARY}
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
)

If(UNIX AND NOT APPLE)
  List(APPEND BOOST_LIBS pthread)
EndIf()

set(FAIR_LIBS
  FairMQ
)

If(FAIRLOGGER_FOUND)
  set(FAIR_LIBS
      ${FAIR_LIBS}
      FairLogger
     )
EndIf()

set(EXE_NAME StsHitProducerIdeal)
set(SRCS CbmDeviceStsHitProducerIdeal.cxx runStsHitProducerIdeal.cxx)

set(DEPENDENCIES
  ${DEPENDENCIES}
  ${FAIR_LIBS}
  ${BOOST_LIBS}
  CbmMQBase
  CbmTaskToAlgoTutorial
  CbmTrdBase
  Core
  RIO
  Net
)
GENERATE_EXECUTABLE()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startTest.sh.in ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startTaskToAlgoTutorial.sh)
