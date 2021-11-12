add_subdirectory (data)



# Create a library libCbmAlgo

set(SRCS
  evbuild/EventBuilder.cxx
 )

add_library(Algo SHARED ${SRCS})

target_include_directories(Algo
  PUBLIC ${CMAKE_SOURCE_DIR}/core/data
  PUBLIC ${CMAKE_SOURCE_DIR}/core/data/base
  PUBLIC ${CMAKE_SOURCE_DIR}/core/data/global
  PUBLIC ${CMAKE_SOURCE_DIR}/core/data/sts
  PUBLIC ${CMAKE_SOURCE_DIR}/external/ipc/ipc/lib/fles_ipc 
)

target_include_directories(Algo SYSTEM
  PUBLIC ${Boost_INCLUDE_DIR}
)

target_compile_definitions(Algo PUBLIC NO_ROOT)

