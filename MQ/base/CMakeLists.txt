set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

set(SRCS
  CbmMQChannels.cxx
  )

set(LIBRARY_NAME CbmMQBase)
set(PUBLIC_DEPENDENCIES
  FairMQ::FairMQ
  )

set(INTERFACE_DEPENDENCIES
  ROOT::Net
  )

generate_cbm_library()
