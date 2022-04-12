# Build parts of the flesnet software, which is maintained in the flesnet repository on github
#
# The included libraries provide the interface to the experiment data in timeslices
# both online and in timeslice archive (.tsa) files.

set(FLESNET_VERSION b1b8c37d6db9a66122ee375bea608c6c6e848665) # 2022-02-24

set(FLESNET_SRC_URL "https://github.com/cbm-fles/flesnet")
set(FLESNET_DESTDIR "${PROJECT_BINARY_DIR}/external/flesnet-prefix")

download_project_if_needed(
  PROJECT         flesnet
  GIT_REPOSITORY  ${FLESNET_SRC_URL}
  GIT_TAG         ${FLESNET_VERSION}
  GIT_STASH       TRUE
  SOURCE_DIR      ${CMAKE_CURRENT_SOURCE_DIR}/flesnet
  TEST_FILE       CMakeLists.txt
)

If(ProjectUpdated)
  File(REMOVE_RECURSE ${FLESNET_DESTDIR})
  Message("flesnet source directory was changed so build directory was deleted")
EndIf()

ExternalProject_Add(
  flesnet
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/flesnet
  CMAKE_CACHE_ARGS -DINCLUDE_ZMQ:BOOL=TRUE -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=ON
  BUILD_IN_SOURCE 0
  LOG_DOWNLOAD 1 LOG_CONFIGURE 1 LOG_BUILD 1 LOG_INSTALL 1
  BUILD_COMMAND ${CMAKE_COMMAND} --build . --target logging fles_ipc
  BUILD_BYPRODUCTS ${FLESNET_DESTDIR}/src/flesnet-build/lib/fles_ipc/${CMAKE_STATIC_LIBRARY_PREFIX}fles_ipc${CMAKE_STATIC_LIBRARY_SUFFIX}
  INSTALL_COMMAND ""
)

add_library(external::fles_logging STATIC IMPORTED GLOBAL)
add_dependencies(external::fles_logging flesnet)
set_target_properties(external::fles_logging PROPERTIES
  IMPORTED_LOCATION ${FLESNET_DESTDIR}/src/flesnet-build/lib/logging/${CMAKE_STATIC_LIBRARY_PREFIX}logging${CMAKE_STATIC_LIBRARY_SUFFIX}
)
target_include_directories(external::fles_logging INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/flesnet/lib/logging
  ${Boost_INCLUDE_DIRS}
)
target_compile_definitions(external::fles_logging
  INTERFACE BOOST_LOG_DYN_LINK
  INTERFACE BOOST_LOG_USE_NATIVE_SYSLOG
  INTERFACE BOOST_ERROR_CODE_HEADER_ONLY
)

add_library(external::fles_ipc STATIC IMPORTED GLOBAL)
add_dependencies(external::fles_ipc flesnet external::fles_logging)
set_target_properties(external::fles_ipc PROPERTIES
  IMPORTED_LOCATION ${FLESNET_DESTDIR}/src/flesnet-build/lib/fles_ipc/${CMAKE_STATIC_LIBRARY_PREFIX}fles_ipc${CMAKE_STATIC_LIBRARY_SUFFIX}
  INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/flesnet/lib/fles_ipc
)
