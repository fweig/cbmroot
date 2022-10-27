# Create a library called "" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.
set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )
  
# List of source files
set(SRCS
  CbmDetectorID.cxx
  NicaCbmFirstTask.cxx
  CbmHelix.cxx
  )


set(LIBRARY_NAME NicaCbmHelper)
set(LINKDEF NicaCbmHelpersLinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmData
  FairRoot::Base
  NicaFeatures
  NicaDataFormat
  )

set(PRIVATE_DEPENDENCIES
  ROOT::MathCore
  )

set(INTERFACE_DEPENDENCIES
  ROOT::Core
  )


generate_cbm_library()

