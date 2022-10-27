# CMakeList file for library libStsAna
# Last update: V. Friese, 09.04.2020

set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

set(SRCS
  CbmStsWkn.cxx
  )


set(LIBRARY_NAME CbmStsAna)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  ROOT::Core
  )

set(PRIVATE_DEPENDENCIES
  CbmData
  FairRoot::Base
  ROOT::MathCore
  )

generate_cbm_library()
