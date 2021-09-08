Set(LIBRARY_NAME AnalysisTreeKfpfInterface)

set(SRCS
        ATKFParticleFinder.cxx
        CutsContainer.cxx
    )

Set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}

  ${KFParticle_INCLUDE_DIR}
  ${AnalysisTree_INCLUDE_DIR}
)

Include_Directories (${INCLUDE_DIRECTORIES})

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES}
  ${Boost_INCLUDE_DIR} 
)

Include_Directories (SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set (LINK_DIRECTORIES
  ${AnalysisTree_LIBRARY_DIR}
  ${KFParticle_LIB_DIR}
  ${FAIRROOT_LIBRARY_DIR}
  ${ROOT_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
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

Set(LINKDEF AnalysisTreeKfpfInterfaceLinkDef.h)

Set(_AnalysisTree_LIB AnalysisTreeBase AnalysisTreeInfra)

Set(DEPENDENCIES 
	${_AnalysisTree_LIB} 
	KFParticle)
Set(DEFINITIONS -DDO_TPCCATRACKER_EFF_PERFORMANCE -DNonhomogeneousField -DCBM -DUSE_TIMERS)

ADD_DEFINITIONS(${DEFINITIONS})

GENERATE_LIBRARY()

# Install(FILES ../../../macro/analysis/common/analysis_tree_converter/run_treemaker.C
#         DESTINATION share/cbmroot/macro/analysis_tree
#        )

Add_Dependencies(AnalysisTreeKfpfInterface ANALYSISTREE KFPARTICLE)
