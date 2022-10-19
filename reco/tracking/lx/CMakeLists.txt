# Create a library called "libLxTrack" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed
# here.

set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/TBBinned
  ${CMAKE_CURRENT_SOURCE_DIR}/GenNoiseElectrons
  ${CMAKE_CURRENT_SOURCE_DIR}/CalcStats
  )
set(SRCS
  TBBinned/LxTBTask.cxx
#  TBBinned/LxTBMLTask.cxx
  TBBinned/LxTBTieTracks.cxx
  TBBinned/LxTBTrdTask.cxx
  GenNoiseElectrons/GenNoiseElectrons.cxx
  CalcStats/CalcStats.cxx
  )

set(NO_DICT_SRCS
#  LxTBMatEffs.cxx
#  LxTBNaive.cxx
)


set(LIBRARY_NAME LxTrack)
set(LINKDEF LxLinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmBase
  CbmData
  KF
  ROOT::Core
  ROOT::Geom
  ROOT::Hist
  )

set(PRIVATE_DEPENDENCIES
  ROOT::Gpad
  CbmMuchSim
  Littrack
  )

set(INTERFACE_DEPENDENCIES
  CbmMuchBase  
  ROOT::EG
  ROOT::Graf
  ROOT::MathCore
  ROOT::RIO
  )


generate_cbm_library()
