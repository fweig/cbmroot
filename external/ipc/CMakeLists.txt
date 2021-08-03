# Build the fles_ipc library which is maintained in the flesnet repository on github
#
# This library provides the interface to the experiment data in timeslices
# both online as delivered by flesnet and to timeslice archive (.tsa) files.

download_project_if_needed(PROJECT         fles_ipc
                           GIT_REPOSITORY  "https://github.com/cbm-fles/flesnet"
                           GIT_TAG         "406773b9f882d5e7d6fe226b5aab7ed435d51fb4"
                           GIT_STASH       TRUE
                           SOURCE_DIR      ${CMAKE_CURRENT_SOURCE_DIR}/ipc
                           TEST_FILE       CMakeLists.txt
                           )

# Create the logging library
add_library(fles_logging SHARED ipc/lib/logging/log.cpp ipc/lib/logging/log.hpp)

target_compile_definitions(fles_logging
  PUBLIC BOOST_LOG_DYN_LINK
  PUBLIC BOOST_LOG_USE_NATIVE_SYSLOG
  PUBLIC BOOST_ERROR_CODE_HEADER_ONLY
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
ipc/lib/fles_ipc/TimesliceAutoSource.cpp
)


Set_Source_Files_Properties(${SRCS} COMPILE_FLAGS "-std=c++17 -O3 -ggdb -msse4.2 -Wall -Wpedantic -Wextra -Winit-self -Wundef -Wold-style-cast -Woverloaded-virtual -Wwrite-strings -Wnon-virtual-dtor -fno-omit-frame-pointer")

Set(LIBRARY_NAME fles_ipc)
If(UNIX AND NOT APPLE)
  Set(DEPENDENCIES boost_thread boost_system boost_serialization boost_log boost_regex fles_logging zmq rt)
Else()
  Set(DEPENDENCIES boost_thread boost_system boost_serialization boost_log boost_regex fles_logging zmq)
EndIf()


GENERATE_LIBRARY()

# Install header files
install(FILES ipc/lib/logging/log.hpp DESTINATION include)
file(GLOB_RECURSE IPC_HEADERS ipc/lib/fles_ipc/*.hpp)
install(FILES ${IPC_HEADERS} DESTINATION include)
