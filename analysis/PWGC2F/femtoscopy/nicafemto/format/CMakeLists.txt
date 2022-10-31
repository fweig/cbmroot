# Create a library called "" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.
set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/combined
  ${CMAKE_CURRENT_SOURCE_DIR}/unigen
  ${CMAKE_CURRENT_SOURCE_DIR}/anatree
  ${CMAKE_CURRENT_SOURCE_DIR}/anatree/mc
  ${CMAKE_CURRENT_SOURCE_DIR}/anatree/reco
  ${CMAKE_CURRENT_SOURCE_DIR}/anatree/femto
  )

# List of source files
set(SRCS
  NicaCbmFormatTypes.cxx

  combined/NicaCbmFullEvent.cxx
  combined/CbmHbtFullEvent.cxx

  unigen/NicaGeneratorReadUnigen.cxx
  unigen/NicaGeneratorWriteUnigen.cxx
  unigen/NicaUnigenSource.cxx
  unigen/NicaUnigenEvent.cxx
  unigen/NicaUnigenEventInterface.cxx
  unigen/NicaUnigenTrack.cxx
  unigen/NicaUnigenTrackInterface.cxx

  anatree/mc/NicaCbmMCEvent.cxx
  anatree/mc/NicaCbmMCEventInterface.cxx
  anatree/mc/NicaCbmMCTrack.cxx
  anatree/mc/NicaCbmMCTrackInterface.cxx
  anatree/reco/NicaCbmEvent.cxx
  anatree/reco/NicaCbmEventInterface.cxx
  anatree/reco/NicaCbmTrack.cxx
  anatree/reco/NicaCbmTrackInterface.cxx
  anatree/femto/CbmHbtTrack.cxx
  anatree/femto/CbmHbtEvent.cxx
  anatree/CbmAnaTreeContainer.cxx
  anatree/CbmAnaTreeSource.cxx
  )

set(LIBRARY_NAME NicaCbmFormat)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmData
  NicaCbmHelper
  FairRoot::Base
  ROOT::Core
  ROOT::Physics
  ROOT::Tree
)


set(PRIVATE_DEPENDENCIES
  CbmSimGenerators
  FairLogger::FairLogger
  ROOT::EG
  ROOT::RIO
  NicaDataFormat
  NicaFeatures
  NicaGen
  AnalysisTreeBase
  AnalysisTreeInfra
  )


generate_cbm_library()

add_dependencies(G__NicaCbmFormat NICAFEMTO ANALYSISTREE)
