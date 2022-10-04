set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

set(SRCS
  CbmTrdDigiDraw.cxx
  CbmPixelHitSetDraw.cxx
  CbmEvDisTracks.cxx
  CbmRecoTracks.cxx
  CbmPointSetArray.cxx
  CbmPointSetArrayDraw.cxx
  CbmPointSetArrayEditor.cxx
  )


set(LIBRARY_NAME CbmDisplay)
set(LINKDEF  ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  FairRoot::Base
  FairRoot::EventDisplay
  ROOT::Core
  ROOT::Eve
  ROOT::Ged
  ROOT::Gui
  )

set(PRIVATE_DEPENDENCIES
  CbmData
  CbmTrdBase
  FairRoot::ParBase
  ROOT::EG
  ROOT::Gpad
  ROOT::Graf3d
  ROOT::Physics
  )

generate_cbm_library()
