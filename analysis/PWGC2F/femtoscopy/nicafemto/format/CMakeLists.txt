# Create a library called "" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.
Set(INCLUDE_DIRECTORIES
	${BASE_INCLUDE_DIRECTORIES}
	${CMAKE_SOURCE_FAIRMCSTACK}
	${NICAFEMTO_DATA}
	${NICAFEMTO_FEATURES}
	${NICAFEMTO_GEN}
	${KF_HEADERS}
	${CBMROOT_SOURCE_DIR}/sim/transport/generators/unigen
	${CBMROOT_SOURCE_DIR}/core/data
	${NICA_CBM}/helpers
	${NICA_CBM_FORMAT}
	${AnalysisTree_INCLUDE_DIR}
)
Set(SYSTEM_INCLUDE_DIRECTORIES
 ${ROOT_INCLUDE_DIR} 
 ${CLHEP_INCLUDE_DIR}
 ${Boost_INCLUDE_DIRS}
)

Set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
  ${Vc_LIB_DIR}
  ${KFParticle_LIB_DIR}
  ${AnalysisTree_LIBRARY_DIR}
)

Include_Directories(${INCLUDE_DIRECTORIES})
Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})
 
Link_Directories(${LINK_DIRECTORIES})

# List of source files
set(SRCS

NicaCbmFormatTypes.cxx

combined/NicaCbmGlobalMCEvent.cxx
combined/NicaCbmATFullEvent.cxx
combined/CbmHbtFullEvent.cxx

full/NicaCbmEvent.cxx
full/NicaCbmEventInterface.cxx
full/NicaCbmTrack.cxx
full/NicaCbmTrackInterface.cxx

mc/NicaCbmMCEvent.cxx
mc/NicaCbmMCEventInterface.cxx
mc/NicaCbmMCTrack.cxx
mc/NicaCbmMCTrackInterface.cxx

global/NicaCbmGlobalEvent.cxx
global/NicaCbmGlobalEventInterface.cxx
global/NicaCbmGlobalTrack.cxx
global/NicaCbmGlobalTrackInterface.cxx


matched/NicaCbmMatchedEvent.cxx
matched/NicaCbmMatchedEventInterface.cxx
matched/NicaCbmMatchedTrack.cxx
matched/NicaCbmMatchedTrackInterface.cxx

unigen/NicaGeneratorReadUnigen.cxx
unigen/NicaGeneratorWriteUnigen.cxx
unigen/NicaUnigenSource.cxx
unigen/NicaUnigenEvent.cxx
unigen/NicaUnigenEventInterface.cxx
unigen/NicaUnigenTrack.cxx
unigen/NicaUnigenTrackInterface.cxx

anatree/mc/NicaCbmATMCEvent.cxx
anatree/mc/NicaCbmATMCEventInterface.cxx
anatree/mc/NicaCbmATMCTrack.cxx
anatree/mc/NicaCbmATMCTrackInterface.cxx
anatree/reco/NicaCbmATEvent.cxx
anatree/reco/NicaCbmATEventInterface.cxx
anatree/reco/NicaCbmATTrack.cxx
anatree/reco/NicaCbmATTrackInterface.cxx
anatree/femto/CbmHbtTrack.cxx
anatree/femto/CbmHbtEvent.cxx
anatree/CbmAnaTreeContainer.cxx
anatree/CbmAnaTreeSource.cxx
)

Set(HEADERS )
Set(LINKDEF NicaCbmFormatLinkDef.h)
Set(LIBRARY_NAME NicaCbmFormat)

If(UNIX AND NOT APPLE)
  Set(_AnalysisTree_LIB  AnalysisTreeBase.so AnalysisTreeInfra.so)
  Else()
  Set(_AnalysisTree_LIB  AnalysisTreeBase.dylib AnalysisTreeInfra.dylib)
EndIf()

Set(DEPENDENCIES ${_AnalysisTree_LIB} CbmData NicaFeatures NicaGen NicaDataFormat NicaCbmHelper CbmSimGenerators Tree)

GENERATE_LIBRARY()

Add_Dependencies(NicaCbmFormat NICAFEMTO ANALYSISTREE )

