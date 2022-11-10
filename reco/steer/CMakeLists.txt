# CMakeList file for library CbmRecoSteer
# V. Friese,     2 June 2021

set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )


set(SRCS
  CbmRecoUnpack.cxx
  CbmSourceTsArchive.cxx
  )


set(LIBRARY_NAME CbmRecoSteer)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmData
  FairRoot::Base
  ROOT::Core
  ROOT::Hist
  )

set(PRIVATE_DEPENDENCIES
  CbmTofReco
  CbmMuchReco
  CbmPsdReco
  CbmPsdBase
  CbmRichReco
  CbmRecoSts
  CbmTrdReco
  CbmTzdReco
  )

set(INTERFACE_DEPENDENCIES
  CbmRecoBase
  FairRoot::ParBase
  external::fles_ipc
  )


generate_cbm_library()
