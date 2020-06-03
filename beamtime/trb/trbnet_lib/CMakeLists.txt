# Build the trbnet library  which is taken from
# git://jspc29.x-matter.uni-frankfurt.de/projects/cbmfles.git
# This executable uses the fles_ipc library to read timeslices
# This file is only called if the compiler provides c++11 support

Set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
)

Include_Directories( ${INCLUDE_DIRECTORIES})

Include_Directories(SYSTEM 
  ${Boost_INCLUDE_DIR}  
#  ${ZMQ_INCLUDE_DIR}
  ${CBMROOT_SOURCE_DIR}/external/ipc_legacy/ipc/src
)

Set(LINK_DIRECTORIES
  ${Boost_LIBRARY_DIRS}
#  ${ZMQ_LIBRARY_DIR}
)
 
Link_Directories( ${LINK_DIRECTORIES})

Set(SRCS
TrbBridge.cpp
TrbBridgeStatistics.cpp
TrbBridgeTrbNetHeaders.cpp
TrbBridgeFrame.cpp
TrbBridgeTransaction.cpp
)

Set_Source_Files_Properties(${SRCS} COMPILE_FLAGS "-Wall -pedantic -Wextra -Winit-self -Wundef -Wold-style-cast -Woverloaded-virtual -Wwrite-strings -Wnon-virtual-dtor")

Set(LIBRARY_NAME trbnet)

GENERATE_LIBRARY()
