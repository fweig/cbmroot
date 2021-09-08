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
  ${KFParticle_LIB_DIR}
  ${AnalysisTree_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
  ${ROOT_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
)

Include_Directories(${INCLUDE_DIRECTORIES})
Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})
 
Link_Directories(${LINK_DIRECTORIES})

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

