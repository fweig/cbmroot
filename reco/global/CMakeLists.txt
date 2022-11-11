# Create a library called "libGlobal" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )


set(SRCS
  #CbmEcalTrackingMuch.cxx
  CbmFindGlobalTracks.cxx    
  #CbmFindGlobalTracksQa.cxx  
  CbmFindPrimaryVertex.cxx   
  CbmFindPrimaryVertexEvents.cxx   
  CbmFitGlobalTracks.cxx 
  CbmFitGlobalTracksQa.cxx 
  CbmGlobalTrackFitterIdeal.cxx 
  CbmPVFinderIdeal.cxx       
  CbmTrackMergerIdeal.cxx  
  CbmRecoTzero.cxx  
  #CbmTofMergerIdeal.cxx 
  )


set(LIBRARY_NAME CbmGlobal)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmData
  CbmRecoBase
  FairRoot::Base
  ROOT::Core
  )

set(PRIVATE_DEPENDENCIES
  KF
  ROOT::Hist
  ROOT::MathCore
  ROOT::Matrix
  ROOT::Physics
  )


generate_cbm_library()
