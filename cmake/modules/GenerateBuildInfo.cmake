execute_process(
    COMMAND git log -1 --format=%H
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    OUTPUT_VARIABLE GIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

if (NOT DEFINED INFILE)
    message(FATAL_ERROR "INFILE not defined")
endif()

if (NOT DEFINED OUTFILE)
    message(FATAL_ERROR "OUTFILE not defined")
endif()

if (NOT DEFINED XPU_DEBUG)
    message(FATAL_ERROR "XPU_DEBUG not defined")
endif()

if (NOT DEFINED BUILD_TYPE)
    message(FATAL_ERROR "BUILD_TYPE not defined")
endif()

# message(STATUS "Git hash: ${GIT_HASH}")
# message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
# message(STATUS "XPU_DEBUG: ${XPU_DEBUG}")

if (XPU_DEBUG)
    set(GPU_DEBUG 1)
else()
    set(GPU_DEBUG 0)
endif()

configure_file(
    ${INFILE}
    ${OUTFILE}
    @ONLY
)
