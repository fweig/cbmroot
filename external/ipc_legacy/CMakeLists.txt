# Build the fles_ipc library which is tacken from
# github as defined in the svn:externals definition
# This library provides the interface to the FLIB board or to files in tca
# format written by the FLIB

download_project_if_needed(PROJECT         fles_ipc_legacy
                            GIT_REPOSITORY  "https://github.com/cbm-fles/fles_ipc"
                            GIT_TAG         "88dc25a4e0301b0ada25d9a22ca2541c1eb3590e"
                            SOURCE_DIR      ${CMAKE_CURRENT_SOURCE_DIR}/ipc
                            TEST_FILE       CMakeLists.txt
                           )

Set(INCLUDE_DIRECTORIES
  ${CBMROOT_SOURCE_DIR}/external/ipc_legacy/ipc/src
)

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${Boost_INCLUDE_DIR}
  ${ZMQ_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}/external/cppzmq
)

Include_Directories( ${INCLUDE_DIRECTORIES})

Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})


Set(LINK_DIRECTORIES
  ${Boost_LIBRARY_DIRS}
  ${ZMQ_LIBRARY_DIRS}
)

Link_Directories( ${LINK_DIRECTORIES})

Set(SRCS
ipc/src/Microslice.cpp
ipc/src/MicrosliceView.cpp
ipc/src/StorableMicroslice.cpp
ipc/src/StorableTimeslice.cpp
ipc/src/System.cpp
ipc/src/Timeslice.cpp
ipc/src/TimeslicePublisher.cpp
ipc/src/TimesliceReceiver.cpp
ipc/src/TimesliceSubscriber.cpp
ipc/src/TimesliceView.cpp
)

Set_Source_Files_Properties(${SRCS} COMPILE_FLAGS "-Wall -pedantic -Wextra -Winit-self -Wundef -Wold-style-cast -Woverloaded-virtual -Wwrite-strings -Wnon-virtual-dtor")

Set(LIBRARY_NAME fles_ipc_legacy)
If(UNIX AND NOT APPLE)
  Set(DEPENDENCIES boost_thread boost_system boost_serialization zmq rt)
Else()
  Set(DEPENDENCIES boost_thread boost_system boost_serialization zmq)
EndIf()


GENERATE_LIBRARY()
