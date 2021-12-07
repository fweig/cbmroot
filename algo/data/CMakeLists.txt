# Create a library called "libCbmOnlineData" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(SRCS
  ${CMAKE_SOURCE_DIR}/core/data/sts/CbmStsDigi.cxx
  ${CMAKE_SOURCE_DIR}/core/data/sts/CbmStsAddress.cxx
 )

add_library(OnlineData SHARED ${SRCS})

target_include_directories(OnlineData
  PUBLIC ${CMAKE_SOURCE_DIR}/core/data
  PUBLIC ${CMAKE_SOURCE_DIR}/core/data/base
  PUBLIC ${CMAKE_SOURCE_DIR}/core/data/sts
  PUBLIC ${CMAKE_SOURCE_DIR}/core/data/global
  PUBLIC ${CMAKE_SOURCE_DIR}/external/ipc/ipc/lib/fles_ipc
)

target_include_directories(OnlineData SYSTEM
  PUBLIC ${Boost_INCLUDE_DIR}
  PUBLIC ${FAIRLOGGER_INCLUDE_DIR}
)

target_link_directories(OnlineData
  PUBLIC ${FAIRLOGGER_LIBRARY_DIR}
)

target_compile_definitions(OnlineData PUBLIC NO_ROOT)
target_link_libraries(OnlineData FairLogger)
