# Create a library called "libCbmOnlineData" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(SRCS
  ${CMAKE_SOURCE_DIR}/core/data/sts/CbmStsDigi.cxx
)

add_library(OnlineData SHARED ${SRCS})

target_include_directories(OnlineData
  PUBLIC ${CMAKE_SOURCE_DIR}/core/data
  PUBLIC ${CMAKE_SOURCE_DIR}/core/data/sts
)

target_include_directories(OnlineData SYSTEM
  PUBLIC ${Boost_INCLUDE_DIR}
)

target_compile_definitions(OnlineData PUBLIC NO_ROOT)
