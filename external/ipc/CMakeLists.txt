# Build the fles_ipc library which is tacken from
# github as defined in the svn:externals definition
# This library provides the interface to the FLIB board or to files in tca
# format written by the FLIB

download_project_if_needed(PROJECT         fles_ipc
                           GIT_REPOSITORY  "https://github.com/cbm-fles/flesnet"
                           GIT_TAG         "e2d20813a74561cf58661b077c046c0da1f28288"
                           GIT_STASH       TRUE
                           SOURCE_DIR      ${CMAKE_CURRENT_SOURCE_DIR}/ipc
                           PATCH_COMMAND   "patch -p1 < ${CMAKE_CURRENT_SOURCE_DIR}/ipc.patch"
                           TEST_FILE       CMakeLists.txt
                           )


configure_file(Timeslice.hpp ${CMAKE_CURRENT_SOURCE_DIR}/ipc/lib/fles_ipc/Timeslice.hpp COPYONLY)
configure_file(TimesliceMultiInputArchive.hpp ${CMAKE_CURRENT_SOURCE_DIR}/ipc/lib/fles_ipc/TimesliceMultiInputArchive.hpp COPYONLY)
configure_file(TimesliceMultiInputArchive.cpp ${CMAKE_CURRENT_SOURCE_DIR}/ipc/lib/fles_ipc/TimesliceMultiInputArchive.cpp COPYONLY)
configure_file(TimesliceMultiSubscriber.hpp ${CMAKE_CURRENT_SOURCE_DIR}/ipc/lib/fles_ipc/TimesliceMultiSubscriber.hpp COPYONLY)
configure_file(TimesliceMultiSubscriber.cpp ${CMAKE_CURRENT_SOURCE_DIR}/ipc/lib/fles_ipc/TimesliceMultiSubscriber.cpp COPYONLY)


# Create the logging library
add_library(fles_logging SHARED ipc/lib/logging/log.cpp ipc/lib/logging/log.hpp)

target_compile_definitions(fles_logging
  PUBLIC BOOST_LOG_DYN_LINK
  PUBLIC BOOST_LOG_USE_NATIVE_SYSLOG
)

target_include_directories(fles_logging PUBLIC ipc/lib/logging/)

target_include_directories(fles_logging SYSTEM PUBLIC ${Boost_INCLUDE_DIRS})

Install(TARGETS fles_logging DESTINATION lib)

Set(INCLUDE_DIRECTORIES
)

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${Boost_INCLUDE_DIR}
  ${ZMQ_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}/external/cppzmq
  ${CMAKE_SOURCE_DIR}/external/ipc/ipc
  ${CMAKE_SOURCE_DIR}/external/ipc/ipc/lib/logging
)

Include_Directories( ${INCLUDE_DIRECTORIES})

Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})


Set(LINK_DIRECTORIES
  ${Boost_LIBRARY_DIRS}
  ${ZMQ_LIBRARY_DIRS}
)

Link_Directories( ${LINK_DIRECTORIES})

Set(SRCS
ipc/lib/fles_ipc/Microslice.cpp
ipc/lib/fles_ipc/MicrosliceView.cpp
ipc/lib/fles_ipc/StorableMicroslice.cpp
ipc/lib/fles_ipc/StorableTimeslice.cpp
ipc/lib/fles_ipc/System.cpp
ipc/lib/fles_ipc/Timeslice.cpp
ipc/lib/fles_ipc/TimeslicePublisher.cpp
ipc/lib/fles_ipc/TimesliceReceiver.cpp
ipc/lib/fles_ipc/TimesliceSubscriber.cpp
ipc/lib/fles_ipc/TimesliceView.cpp
ipc/lib/fles_ipc/TimesliceMultiInputArchive.cpp
ipc/lib/fles_ipc/TimesliceMultiSubscriber.cpp
)


Set_Source_Files_Properties(${SRCS} COMPILE_FLAGS "-std=c++11 -O3 -ggdb -msse4.2 -Wall -Wpedantic -Wextra -Winit-self -Wundef -Wold-style-cast -Woverloaded-virtual -Wwrite-strings -Wnon-virtual-dtor -fno-omit-frame-pointer")

Set(LIBRARY_NAME fles_ipc)
If(UNIX AND NOT APPLE)
  Set(DEPENDENCIES boost_thread boost_system boost_serialization boost_log boost_regex fles_logging zmq rt)
Else()
  Set(DEPENDENCIES boost_thread boost_system boost_serialization boost_log boost_regex fles_logging zmq)
EndIf()


GENERATE_LIBRARY()
