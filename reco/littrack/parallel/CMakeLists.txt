# Create a library called "libLittrackparallel".

set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CBMROOT_SOURCE_DIR}/algo/ca/simd
  )


set(LITTRACKPARALLEL_SRCS
    LitTrackFinderNN.cxx
  )

if(RULE_CHECKER_FOUND)
  CHECK_RULES("${LITTRACKPARALLEL_SRCS}" "${INCLUDE_DIRECTORIES}" LITTRACKPARALLEL_RULES)
endif(RULE_CHECKER_FOUND)

IF (SSE_FOUND)
  ADD_DEFINITIONS(-DHAVE_SSE)
  SET_SOURCE_FILES_PROPERTIES(${LITTRACKPARALLEL_SRCS} PROPERTIES COMPILE_FLAGS "-msse -O3 ")
  Message(STATUS "Littrackparallel will be compiled with SSE support")
ELSE (SSE_FOUND)
  Message(STATUS "Littrackparallel will be compiled without SSE support")
  SET_SOURCE_FILES_PROPERTIES(${LITTRACKPARALLEL_SRCS} PROPERTIES COMPILE_FLAGS "-O3 ")
ENDIF (SSE_FOUND)


set(SRCS ${LITTRACKPARALLEL_SRCS})
# Since there was a problem with defining Vc::Vc as a dependecy a dependency
# to KFParticle was set which includes the Vc dependency
# This has to be fixed and done properly
set(PUBLIC_DEPENDENCIES Vc::Vc)
set(LIBRARY_NAME Littrackparallel)

generate_cbm_library()
