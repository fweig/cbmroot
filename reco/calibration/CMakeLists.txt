# CMakeList file for library CbmRecoBase
# V. Friese,      1 August   2019
# P.-A. Loizeau, 13 February 2020

set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

set(SRCS
  CbmOffsetDigiTimeTester.cxx
  )


set(LIBRARY_NAME CbmRecoCal)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PRIVATE_DEPENDENCIES
  CbmData
  )

set(INTERFACE_DEPENDENCIES
  FairRoot::Base
  ROOT::Core
  )

generate_cbm_library()
