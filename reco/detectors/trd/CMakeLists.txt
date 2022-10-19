# Create a library called "libTrdReco" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/qa
  ${CMAKE_CURRENT_SOURCE_DIR}/qa/data
  ${CMAKE_CURRENT_SOURCE_DIR}/pid
  ${CMAKE_CURRENT_SOURCE_DIR}/rawToDigiMethods
  ${CMAKE_CURRENT_SOURCE_DIR}/unpack
  )


set(SRCS

CbmTrdTrackFinderIdeal.cxx 
CbmTrdTrackFitterIdeal.cxx 
CbmTrdClusterFinder.cxx 
CbmTrdHitProducer.cxx 
CbmTrdModuleRec.cxx 
CbmTrdModuleRecR.cxx 
CbmTrdModuleRec2D.cxx 
CbmTrdDigiRec.cxx 

unpack/CbmTrdUnpackConfig.cxx
unpack/CbmTrdUnpackAlgoBaseR.cxx
unpack/CbmTrdUnpackAlgoR.cxx
unpack/CbmTrdUnpackAlgoLegacy2020R.cxx
unpack/CbmTrdUnpackMonitor.cxx

unpack/CbmTrdUnpackFaspAlgo.cxx
unpack/CbmTrdUnpackFaspConfig.cxx
unpack/CbmTrdUnpackFaspMonitor.cxx

qa/CbmTrdClusterizerFastQa.cxx
qa/CbmTrdHitDensityQa.cxx
qa/CbmTrdHitProducerClusterQa.cxx
qa/CbmTrdHitProducerQa.cxx
qa/CbmTrdOccupancyQa.cxx
qa/CbmTrdQa.cxx
qa/CbmTrdRecoQa.cxx
qa/CbmTrdTracksPidQa.cxx

qa/CbmTrdCalibTracker.cxx
qa/data/CbmTrdHitMC.cxx

pid/CbmTrdElectronsTrainAnn.cxx
pid/CbmTrdSetTracksPidWkn.cxx   
pid/CbmTrdSetTracksPidModWkn.cxx  
pid/CbmTrdSetTracksPidLike.cxx     
pid/CbmTrdSetTracksPidANN.cxx

rawToDigiMethods/CbmTrdRawToDigiBaseR.cxx
rawToDigiMethods/CbmTrdRawToDigiMaxAdcR.cxx
rawToDigiMethods/CbmTrdRawToDigiFitR.cxx
rawToDigiMethods/CbmTrdRawToDigiLookUpCorrR.cxx
)

IF (SSE_FOUND)
  ADD_DEFINITIONS(-DHAVE_SSE)
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS 
  "-msse -O3")
  Message(STATUS "TRD REC will be compiled with SSE support")
ELSE (SSE_FOUND)
  Message(STATUS "TRD REC will be compiled without SSE support")
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS 
  "-O3")
ENDIF (SSE_FOUND)


set(LIBRARY_NAME CbmTrdReco)
set(LINKDEF CbmTrdRecoLinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmData
  CbmQaBase
  CbmTrdBase
  FairLogger::FairLogger
  FairRoot::Base
  ROOT::Core
  ROOT::Graf
  ROOT::Hist
  ROOT::MLP
  ROOT::Physics
  ROOT::RIO
  Vc::Vc
  )

set(PRIVATE_DEPENDENCIES
  CbmBase
  CbmSimSteer
  CbmTrdSim
  CbmRecoBase
  FairRoot::ParBase
  L1
  ROOT::EG
  ROOT::Geom
  ROOT::Gpad
  ROOT::MathCore
  ROOT::Tree
  TMVA
  )

set(INTERFACE_DEPENDENCIES
  external::fles_ipc
  ROOT::RHTTP
  )


generate_cbm_library()
