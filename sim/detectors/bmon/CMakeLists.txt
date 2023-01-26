set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )


set(SRCS
  CbmBmonDigitize.cxx
  )


set(LIBRARY_NAME CbmBmonSim)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmBase
  CbmData
  )

set(PRIVATE_DEPENDENCIES
  ROOT::Core
  ROOT::MathCore
  )

generate_cbm_library()

