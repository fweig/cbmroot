# Create a library called "libBase" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )


set(SRCS
  CbmHadron.cxx
  CbmHadronAnalysis.cxx
  )


set(LIBRARY_NAME CbmHadron)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  KF
  FairRoot::Base
  ROOT::Core
  )

set(PRIVATE_DEPENDENCIES
  CbmBase
  CbmData
  CbmTofBase
  ROOT::Hist
  ROOT::MathCore
  ROOT::Physics
  ROOT::RIO
  )


generate_cbm_library()
