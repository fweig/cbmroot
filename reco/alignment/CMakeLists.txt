# Create a library called "libAlignment" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set(SRCS
  CbmBbaAlignmentTask.cxx
)

set(LIBRARY_NAME CbmAlignment)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  FairRoot::Base
)

set(PRIVATE_DEPENDENCIES
  L1
  bba::library
  CbmStsBase
)

generate_cbm_library()
