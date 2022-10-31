# Create a library called "" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.
set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/pair
  ${CMAKE_CURRENT_SOURCE_DIR}/track
  )

# List of source files
set(SRCS
  pair/CbmBasicFemtoPairCut.cxx
  pair/CbmDeltaPhiDeltaThetaCut.cxx
  pair/CbmStsExitSepCut.cxx
    
  track/CbmBasicTrackCuts.cxx
  track/CbmNHitsCut.cxx
  track/CbmTofCut.cxx
  track/CbmTofM2PolyCut.cxx
 )


set(LIBRARY_NAME NicaCbmCuts)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  ROOT::Physics
  )

set(PRIVATE_DEPENDENCIES
  NicaCbmFormat
  NicaFeatures
  NicaCbmHelper
  NicaCut
  )

generate_cbm_library()

add_dependencies(G__NicaCbmCuts NICAFEMTO)
