set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )


set(SRCS
  ATKFParticleFinder.cxx
  CutsContainer.cxx
  )


IF (SSE_FOUND)
  Message(STATUS "${LIBRARY_NAME} will be compiled with SSE support")
  ADD_DEFINITIONS(-DHAVE_SSE)
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS "-msse -O3")
ELSE (SSE_FOUND)
  MESSAGE(STATUS "${LIBRARY_NAME} will be compiled without SSE support")
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS "-msse -O3")
ENDIF (SSE_FOUND)


set(LIBRARY_NAME AnalysisTreeKfpfInterface)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  AnalysisTreeBase
  )

set(PRIVATE_DEPENDENCIES
  ROOT::Core
  ROOT::Tree
  KFParticle
  )

set(INTERFACE_DEPENDENCIES
  ROOT::RIO
  ROOT::Tree
  )

set(DEFINITIONS -DDO_TPCCATRACKER_EFF_PERFORMANCE -DNonhomogeneousField -DCBM -DUSE_TIMERS)

ADD_DEFINITIONS(${DEFINITIONS})

generate_cbm_library()

add_dependencies(G__AnalysisTreeKfpfInterface KFPARTICLE ANALYSISTREE)

# Install(FILES ../../../macro/analysis/common/analysis_tree_converter/run_treemaker.C
#         DESTINATION share/cbmroot/macro/analysis_tree
#        )
