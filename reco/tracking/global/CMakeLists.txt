# Create a library called "libLxTrack" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed
# here.

set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )


set(SRCS
  CbmGlobalTrackingTask.cxx
  CbmGlobalTrackingQa.cxx
  )

set(NO_DICT_SRCS
  CbmGlobalTrackingTof.cxx
  )


set(LIBRARY_NAME CbmGlobalTracking)
set(LINKDEF GlobalLinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmBase
  CbmData
  KF
  )

set(PRIVATE_DEPENDENCIES
  CbmMuchSim
  Littrack
  CbmMuchBase
  ROOT::Geom
  ROOT::Hist
  ROOT::MathCore
  KFParticleInterface 
  )

set(INTERFACE_DEPENDENCIES
  FairRoot::Base 
  ROOT::Core
  )


generate_cbm_library()
