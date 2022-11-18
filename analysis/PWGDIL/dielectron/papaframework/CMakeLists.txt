set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

set(SRCS
  AnalysisCuts.cxx
  AnalysisFilter.cxx
  PairAnalysisTrack.cxx
  PairAnalysisEvent.cxx
  PairAnalysisPair.cxx
  PairAnalysisPairKF.cxx
  PairAnalysisPairLV.cxx
  PairAnalysisHelper.cxx
  PairAnalysisStyler.cxx

  PairAnalysisCutQa.cxx
  PairAnalysisVarCuts.cxx
  PairAnalysisObjectCuts.cxx
  PairAnalysisPairLegCuts.cxx
  PairAnalysisCutCombi.cxx
  PairAnalysisCutGroup.cxx

  PairAnalysisVarManager.cxx
  PairAnalysisMetaData.cxx

  AnalysisTaskMultiPairAnalysis.cxx
  PairAnalysis.cxx
  #PairAnalysisDebugTree.cxx
  #PairAnalysisMixedEvent.cxx
  PairAnalysisHF.cxx
  ###PairAnalysisHFhelper.cxx
  PairAnalysisHistos.cxx
  PairAnalysisMC.cxx

  PairAnalysisSignalMC.cxx
  PairAnalysisTrackRotator.cxx
  PairAnalysisMixedEvent.cxx
  PairAnalysisMixingHandler.cxx

  #PairAnalysisHistosHelper.cxx
  PairAnalysisFunction.cxx
  PairAnalysisSignalExt.cxx
  #PairAnalysisSignalFit.cxx
  PairAnalysisSpectrum.cxx
  )

IF (SSE_FOUND)
  Message(STATUS "Papa will be compiled with SSE support")
  ADD_DEFINITIONS(-DHAVE_SSE)
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS 
  "-msse -O3")
ELSE (SSE_FOUND)
  MESSAGE(STATUS "Papa will be compiled without SSE support")
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS 
  "-O3")
ENDIF (SSE_FOUND)



If(CMAKE_SYSTEM_NAME MATCHES Darwin)
  If(MAC_OS_VERSION LESS 10.9)
    ADD_DEFINITIONS(-DDO_TPCCATRACKER_EFF_PERFORMANCE -DNonhomogeneousField -DCBM -DUSE_TIMERS -DOLD_MAC)
    Set(DEFINITIONS OLD_MAC)
    Set(DEFINITIONS DO_TPCCATRACKER_EFF_PERFORMANCE NonhomogeneousField CBM USE_TIMERS OLD_MAC)
  Else()
    ADD_DEFINITIONS(-DDO_TPCCATRACKER_EFF_PERFORMANCE -DNonhomogeneousField -DCBM -DUSE_TIMERS)
    Set(DEFINITIONS DO_TPCCATRACKER_EFF_PERFORMANCE NonhomogeneousField CBM USE_TIMERS)
  EndIf()
  If(_UINT8_T_EXIST)
    Add_Definitions(-DHAVE_UINT8_T_HEADER_FILE)
    Set(DEFINITIONS ${DEFINITIONS} HAVE_UINT8_T_HEADER_FILE)
  EndIf()
Else()
  ADD_DEFINITIONS(-DDO_TPCCATRACKER_EFF_PERFORMANCE -DNonhomogeneousField -DCBM -DUSE_TIMERS)
  Set(DEFINITIONS DO_TPCCATRACKER_EFF_PERFORMANCE NonhomogeneousField CBM USE_TIMERS)
EndIf()


set(LIBRARY_NAME CbmPapa)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmData
  CbmRichBase
  KF
  L1
  FairRoot::Base
  ROOT::Core
  ROOT::EG
  ROOT::Gpad
  ROOT::Graf
  ROOT::Hist
  ROOT::MathCore
  ROOT::Matrix
  ROOT::Physics
  ROOT::Tree
  )

set(PRIVATE_DEPENDENCIES
  CbmMvd
  CbmSimBase
  KFParticleInterface
  Littrack
  LittrackQA
  CbmSimGenerators
  FairRoot::ParBase
  ROOT::Geom
  ROOT::Net
  ROOT::RIO
  KFParticle
  )

set(INTERFACE_DEPENDENCIES
  FairLogger::FairLogger
  )

generate_cbm_library()

add_dependencies(G__CbmPapa KFPARTICLE)

install(FILES configs/Config_trd_PidLI.C
        DESTINATION share/cbmroot/macro/papa
       )
