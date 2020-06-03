# Build the tsclient executable  which is taken from
# git@code.compeng.uni-frankfurt.de:flesnet.git
# This executable uses the fles_ipc library to read files
# in the tca format
# This file is only called if the compiler provides c++11 support

Set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CBMROOT_SOURCE_DIR}/beamtime/trb/trbnet_lib
)

Include_Directories( ${INCLUDE_DIRECTORIES})

Include_Directories(SYSTEM 
  ${Boost_INCLUDE_DIR}  
  ${ZMQ_INCLUDE_DIR}
  ${CBMROOT_SOURCE_DIR}/external/ipc_legacy/ipc/src
)

Set(LINK_DIRECTORIES
  ${Boost_LIBRARY_DIRS}
  ${ZMQ_LIBRARY_DIR}
)
 
Link_Directories( ${LINK_DIRECTORIES})

Set(SRCS
main.cpp
Application.cpp
Parameters.cpp
)

Set_Source_Files_Properties(${SRCS} COMPILE_FLAGS "-Wall -pedantic -Wextra -Winit-self -Wundef -Wold-style-cast -Woverloaded-virtual -Wwrite-strings -Wnon-virtual-dtor")

Set(EXE_NAME trbnet_client)
If(UNIX AND NOT APPLE)
  Set(DEPENDENCIES fles_ipc_legacy trbnet boost_program_options ${CMAKE_THREAD_LIBS_INIT} zmq rt)
Else()
  Set(DEPENDENCIES fles_ipc_legacy trbnet boost_program_options ${CMAKE_THREAD_LIBS_INIT} zmq)
EndIf()

GENERATE_EXECUTABLE()
