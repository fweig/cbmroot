Set(INCLUDE_DIRECTORIES

#${CBMROOT_SOURCE_DIR}/analysis
${CMAKE_CURRENT_SOURCE_DIR}

${CBMROOT_SOURCE_DIR}/sim/transport/generators/unigen

${CBMBASE_DIR} 
${CBMBASE_DIR}/draw
${CBMBASE_DIR}/report
${CBMBASE_DIR}/utils

${CBMDATA_DIR}
${CBMDATA_DIR}/sts
${CBMDATA_DIR}/much
${CBMDATA_DIR}/mvd
${CBMDATA_DIR}/global
${CBMDATA_DIR}/rich
${CBMDATA_DIR}/trd
${CBMDATA_DIR}/tof
${CBMDATA_DIR}/psd

${CBMROOT_SOURCE_DIR}/mvd 
#${CBMROOT_SOURCE_DIR}/sts 
${CBMROOT_SOURCE_DIR}/reco/detectors/rich

${CBMROOT_SOURCE_DIR}/run

${CBMROOT_SOURCE_DIR}/reco/KF
${CBMROOT_SOURCE_DIR}/reco/KF/Interface 
${KFParticle_INCLUDE_DIR}

${CBMROOT_SOURCE_DIR}/reco/L1
${CBMROOT_SOURCE_DIR}/reco/L1/ParticleFinder
${CBMROOT_SOURCE_DIR}/reco/L1/L1Algo

${CBMROOT_SOURCE_DIR}/reco/littrack
${CBMROOT_SOURCE_DIR}/reco/littrack/cbm
${CBMROOT_SOURCE_DIR}/reco/littrack/cbm/base
${CBMROOT_SOURCE_DIR}/reco/littrack/cbm/utils
${CBMROOT_SOURCE_DIR}/reco/littrack/std
${CBMROOT_SOURCE_DIR}/reco/littrack/std/data
${CBMROOT_SOURCE_DIR}/reco/littrack/std/base

)

Include_Directories( ${INCLUDE_DIRECTORIES})

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES} 
  ${Boost_INCLUDE_DIR}
)

Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${KFParticle_LIB_DIR}
${FAIRROOT_LIBRARY_DIR}
${ROOT_LIBRARY_DIR}
${Boost_LIBRARY_DIRS}
)

#Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -Weffc++ -Wno-unused-variable -Wno-unused-parameter -Wno-sign-compare -Wno-ignored-qualifiers -Wno-overloaded-virtual")

link_directories( ${LINK_DIRECTORIES})

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

PairAnalysisCutQA.cxx
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


Set(HEADERS )

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
  If(${MAC_OS_VERSION} LESS 10.9)
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

Set(LINKDEF CbmPapaLinkDef.h)
Set(LIBRARY_NAME CbmPapa)
Set(DEPENDENCIES
    L1 KF Littrack CbmRichReco CbmRichBase CbmBase CbmData CbmSimGenerators Base)

GENERATE_LIBRARY()


Install(FILES configs/Config_trd_PidLI.C
        DESTINATION share/cbmroot/macro/papa
       )
