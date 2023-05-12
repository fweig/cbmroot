set(SRCS main.cxx)

add_executable(cbmreco ${SRCS})
target_link_libraries(cbmreco Algo)

install(TARGETS cbmreco DESTINATION bin)
