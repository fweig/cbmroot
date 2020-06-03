# Build the spadic message reader library.
# The code is external in github as defined in the svn:extrenals.
# Add some extra compile warnings 

download_project_if_needed(PROJECT spadic
                           GIT_REPOSITORY https://github.com/spadic/spadic10-software
                           GIT_TAG 286875d1f543a169e29388d9673d2738f337de18
                           SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/spadic
                           TEST_FILE setup.py
                          )

Set(INCLUDE_DIRECTORIES
  ${CBMROOT_SOURCE_DIR}/external/spadic/spadic/lib/message
  ${CBMROOT_SOURCE_DIR}/external/spadic/spadic/lib/message/wrap/cpp
)

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${Boost_INCLUDE_DIR}
  ${CBMROOT_SOURCE_DIR}/external/flib_dpb/flib_dpb
)

Include_Directories( ${INCLUDE_DIRECTORIES})
Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

Set(LINK_DIRECTORIES
${Boost_LIBRARY_DIRS}
)
 
Link_Directories( ${LINK_DIRECTORIES})

Set(C_SRCS
spadic/lib/message/message.c
spadic/lib/message/message_reader.c
)

Set(CXX_SRCS
spadic/lib/message/wrap/cpp/Message.cpp
)

Set_Source_Files_Properties(${CXX_SRCS} COMPILE_FLAGS "-Wall -pedantic -Wextra -Winit-self -Wundef -Wold-style-cast -Woverloaded-virtual -Wwrite-strings -Wnon-virtual-dtor")

Set(SRCS ${C_SRCS} ${CXX_SRCS})

Set(LIBRARY_NAME spadicMessage)
Set(DEPENDENCIES)

GENERATE_LIBRARY()
