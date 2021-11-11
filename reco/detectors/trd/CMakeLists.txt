# Create a library called "libTrdReco" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/qa
  ${CMAKE_CURRENT_SOURCE_DIR}/pid
  ${CMAKE_CURRENT_SOURCE_DIR}/rawToDigiMethods
  ${CMAKE_CURRENT_SOURCE_DIR}/unpack

  ${CBMDETECTORBASE_DIR}/trd

  ${CBMBASE_DIR}
  ${CBMBASE_DIR}/draw

  ${CBMROOT_SOURCE_DIR}/reco/base
  
  ${CBMDATA_DIR}
  ${CBMDATA_DIR}/base
  ${CBMDATA_DIR}/trd 
  ${CBMDATA_DIR}/global

  ${CBMROOT_SOURCE_DIR}/reco/L1/vectors

)

Include_Directories( ${INCLUDE_DIRECTORIES})

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES} 
  ${Boost_INCLUDE_DIR}
  ${IPC_INCLUDE_DIRECTORY} # for fles infos for unpacker
)

Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
) 

link_directories( ${LINK_DIRECTORIES})

set(SRCS

CbmTrdTrackFinderIdeal.cxx 
CbmTrdTrackFitterIdeal.cxx 
CbmTrdClusterFinder.cxx 
CbmTrdHitProducer.cxx 
CbmTrdModuleRec.cxx 
CbmTrdModuleRecR.cxx 
CbmTrdModuleRecT.cxx 
CbmTrdDigiRec.cxx 

unpack/CbmTrdUnpackConfig.cxx
unpack/CbmTrdUnpackAlgoBaseR.cxx
unpack/CbmTrdUnpackAlgoR.cxx
unpack/CbmTrdUnpackAlgoLegacy2020R.cxx
unpack/CbmTrdUnpackMonitor.cxx

unpack/CbmTrdUnpackAlgoFasp2D.cxx
unpack/CbmTrdUnpackConfigFasp2D.cxx

qa/CbmTrdClusterizerFastQa.cxx
qa/CbmTrdHitDensityQa.cxx
qa/CbmTrdHitProducerClusterQa.cxx
qa/CbmTrdHitProducerQa.cxx
qa/CbmTrdOccupancyQa.cxx
qa/CbmTrdQa.cxx
qa/CbmTrdRecoQa.cxx
qa/CbmTrdTracksPidQa.cxx

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

set(LINKDEF CbmTrdRecoLinkDef.h)
Set(LIBRARY_NAME CbmTrdReco)
Set(DEPENDENCIES
    CbmRecoBase
    CbmBase
    CbmData
    Base
    CbmTrdBase
    TMVA
)

GENERATE_LIBRARY()
