# CMakeList file for library libSts.
# Last update: V. Friese, 17.02.2014


set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )


set(SRCS
  CbmStsHitProducerTaskIdeal.cxx
  CbmStsHitProducerIdealAlgo.cxx
  CbmStsHitProducerIdealWrapper.cxx
  )

set(HEADERS
  CbmStsHitProducerTaskIdeal.h
  CbmStsHitProducerIdealWrapper.h
  )
  
set(LIBRARY_NAME CbmTaskToAlgoTutorial)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmData
  CbmMQBase
  FairRoot::Base
  )

set(PRIVATE_DEPENDENCIES
  CbmTrdBase
  FairMQ::Tools
  FairRoot::ParBase
  FairLogger::FairLogger
  ROOT::Core
  )

set(INTERFACE_DEPENDENCIES
  FairMQ::FairMQ
  ROOT::Net
  )

generate_cbm_library()
# ---------------------------------------------------------

set(EXECUTABLE_OUTPUT_PATH "${EXECUTABLE_OUTPUT_PATH}/MQ/sts")

set(EXE_NAME StsHitProducerIdeal)
set(SRCS CbmDeviceStsHitProducerIdeal.cxx runStsHitProducerIdeal.cxx)
set(PUBLIC_DEPENDENCIES
  CbmData
  CbmMQBase
  CbmTaskToAlgoTutorial
  FairRoot::Base
  )

set(PRIVATE_DEPENDENCIES
  CbmTrdBase
  FairMQ::Tools
  FairRoot::ParBase
  FairLogger::FairLogger
  ROOT::Core
  )

set(INTERFACE_DEPENDENCIES
  FairMQ::FairMQ
  ROOT::Net
  )
  
generate_cbm_executable()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startTest.sh.in ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startTaskToAlgoTutorial.sh)
