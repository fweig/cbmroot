# CMakeList file for library CbmRecoBase
# V. Friese,      1 August   2019
# P.-A. Loizeau, 13 February 2020

set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

set(SRCS
CbmUnpackTaskBase.cxx

CbmTrdTrackFinder.cxx  
CbmTrdTrackFitter.cxx  

CbmTofTrackFinder.cxx
CbmTofTrackFitter.cxx

CbmStsTrackFinder.cxx 
CbmStsTrackFitter.cxx 

CbmMuchTrackFinder.cxx

CbmRichRingFinder.cxx 

CbmGlobalTrackFitter.cxx
CbmPrimaryVertexFinder.cxx
CbmTofMerger.cxx
CbmTrackMerger.cxx
CbmRichMerger.cxx
)

set(HEADERS
  CbmRecoUnpackConfig.tmpl
  CbmRecoUnpackAlgo.tmpl
  )


set(LIBRARY_NAME CbmRecoBase)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmData
  FairRoot::Base
  ROOT::Core
  )

set(PRIVATE_DEPENDENCIES
  CbmBase
  )

set(INTERFACE_DEPENDENCIES
  CbmFlibMcbm2018
  external::fles_ipc
  ROOT::Hist
  )


generate_cbm_library()
