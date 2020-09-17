Set(LIBRARY_NAME CbmAnalysisTreeInterface)

set(SRCS
        CbmConverterManager.cxx
        CbmConverterTask.cxx
        CbmSimEventHeaderConverter.cxx
        CbmRecEventHeaderConverter.cxx
        CbmStsTracksConverter.cxx
        CbmSimTracksConverter.cxx
        CbmPsdModulesConverter.cxx
        CbmTofHitsConverter.cxx
    )

Set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}

  ${CBMROOT_SOURCE_DIR}/core/base
  ${CBMROOT_SOURCE_DIR}/core/data
  ${CBMROOT_SOURCE_DIR}/core/data/base
  ${CBMROOT_SOURCE_DIR}/core/data/global
  ${CBMROOT_SOURCE_DIR}/core/data/mvd
  ${CBMROOT_SOURCE_DIR}/core/data/sts
  ${CBMROOT_SOURCE_DIR}/core/data/tof
  ${CBMROOT_SOURCE_DIR}/core/data/psd

  ${CBMROOT_SOURCE_DIR}/sim/transport/generators
  ${CBMROOT_SOURCE_DIR}/sim/transport/steer
  ${CBMROOT_SOURCE_DIR}/sim/transport/geosetup

  ${CBMROOT_SOURCE_DIR}/reco/KF
  ${CBMROOT_SOURCE_DIR}/reco/KF/Interface
  ${CBMROOT_SOURCE_DIR}/reco/KF/KFQA
  ${CBMROOT_SOURCE_DIR}/reco/L1
  ${CBMROOT_SOURCE_DIR}/reco/L1/L1Algo
  ${CBMROOT_SOURCE_DIR}/reco/L1/ParticleFinder
  ${KFParticle_INCLUDE_DIR}
  ${AnalysisTree_INCLUDE_DIR}
)

Include_Directories (${INCLUDE_DIRECTORIES})

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${VC_INCLUDE_DIRS}
  ${BASE_INCLUDE_DIRECTORIES}
  ${Boost_INCLUDE_DIR} 
)

Include_Directories (SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set (LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
  ${Vc_LIB_DIR}
  ${KFParticle_LIB_DIR}
  ${AnalysisTree_LIBRARY_DIR}
)
 
link_directories(${LINK_DIRECTORIES})

IF (SSE_FOUND)
  Message(STATUS "${LIBRARY_NAME} will be compiled with SSE support")
  ADD_DEFINITIONS(-DHAVE_SSE)
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS "-msse -O3")
ELSE (SSE_FOUND)
  MESSAGE(STATUS "${LIBRARY_NAME} will be compiled without SSE support")
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS "-msse -O3")
ENDIF (SSE_FOUND)

Set(LINKDEF CbmAnalysisTreeInterfaceLinkDef.h)

Set(_AnalysisTree_LIB AnalysisTreeBase AnalysisTreeInfra)

Set(DEPENDENCIES 
	${_AnalysisTree_LIB} 
	CbmData 
	CbmBase 
	KF 
	L1 
	Vc.a)
Set(DEFINITIONS -DDO_TPCCATRACKER_EFF_PERFORMANCE -DNonhomogeneousField -DCBM -DUSE_TIMERS)

ADD_DEFINITIONS(${DEFINITIONS})

GENERATE_LIBRARY()

Install(FILES ../../../macro/analysis/common/analysis_tree_converter/run_analysis_tree_maker.C
        DESTINATION share/cbmroot/macro/analysis_tree
       )

Add_Dependencies(CbmAnalysisTreeInterface ANALYSISTREE)