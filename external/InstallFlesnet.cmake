# Build parts of the flesnet software, which is maintained in the flesnet repository on github
#
# The included libraries provide the interface to the experiment data in timeslices
# both online and in timeslice archive (.tsa) files.

set(FLESNET_VERSION 9f18207773bba0c27689a904f900e893745b4ed8) # 2022-07-29

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

if(APPLE)
  execute_process(COMMAND brew --prefix --installed openssl
                  RESULT_VARIABLE OPENSSL_FOUND
                  OUTPUT_VARIABLE OPENSSL_PATH
                  OUTPUT_STRIP_TRAILING_WHITESPACE
                 )
endif()

ExternalProject_Add(
  flesnet
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/flesnet
  CONFIGURE_COMMAND ${CMAKE_COMMAND} -E env SIMPATH=${SIMPATH}
             ${CMAKE_COMMAND}
             -G ${CMAKE_GENERATOR}
             -DINCLUDE_ZMQ:BOOL=TRUE
             -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=ON
             -DOPENSSL_ROOT_DIR:FILEPATH=${OPENSSL_PATH}
             ${CMAKE_CURRENT_SOURCE_DIR}/flesnet
  BUILD_IN_SOURCE 0
  LOG_DOWNLOAD 1 LOG_CONFIGURE 1 LOG_BUILD 1 LOG_INSTALL 1
  BUILD_COMMAND ${CMAKE_COMMAND} --build . -j 1 --target logging monitoring fles_ipc
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

add_library(external::fles_monitoring STATIC IMPORTED GLOBAL)
add_dependencies(external::fles_monitoring flesnet)
set_target_properties(external::fles_monitoring PROPERTIES
  IMPORTED_LOCATION ${FLESNET_DESTDIR}/src/flesnet-build/lib/monitoring/${CMAKE_STATIC_LIBRARY_PREFIX}monitoring${CMAKE_STATIC_LIBRARY_SUFFIX}
)
target_include_directories(external::fles_monitoring INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/flesnet/lib/monitoring
  ${Boost_INCLUDE_DIRS}
)
target_compile_definitions(external::fles_monitoring
  INTERFACE BOOST_LOG_DYN_LINK
  INTERFACE BOOST_LOG_USE_NATIVE_SYSLOG
  INTERFACE BOOST_ERROR_CODE_HEADER_ONLY
)

add_library(external::fles_ipc STATIC IMPORTED GLOBAL)
add_dependencies(external::fles_ipc flesnet external::fles_logging)

set(dir_to_link
    ${FLESNET_DESTDIR}/src/flesnet-build/src/zeromq-build/lib/${CMAKE_STATIC_LIBRARY_PREFIX}zmq${CMAKE_STATIC_LIBRARY_SUFFIX}
   )
find_package("GnuTLS" 3.6.7)
if(GNUTLS_FOUND)
  list(APPEND dir_to_link
       ${GNUTLS_LIBRARIES}
      )
endif()
list(APPEND dir_to_link
     ${FLESNET_DESTDIR}/src/flesnet-build/lib/logging/${CMAKE_STATIC_LIBRARY_PREFIX}logging${CMAKE_STATIC_LIBRARY_SUFFIX}
     ${FLESNET_DESTDIR}/src/flesnet-build/lib/monitoring/${CMAKE_STATIC_LIBRARY_PREFIX}monitoring${CMAKE_STATIC_LIBRARY_SUFFIX}
     ${FLESNET_DESTDIR}/src/flesnet-build/_deps/fmtlib-build/${CMAKE_STATIC_LIBRARY_PREFIX}fmt${CMAKE_STATIC_LIBRARY_SUFFIX}
     )
list(APPEND dir_to_link
     ${Boost_LOG_LIBRARY}
     ${Boost_FILESYSTEM_LIBRARY}
     ${Boost_REGEX_LIBRARY}
     ${Boost_SERIALIZATION_LIBRARY}
     ${Boost_ASIO_LIBRARY}
     ${Boost_BEAST_LIBRARY}
    )
if(NOT APPLE)
  list(APPEND dir_to_link ${Boost_THREAD_LIBRARY})
endif()
list(APPEND dir_to_link -lrt)

set_target_properties(external::fles_ipc PROPERTIES
  IMPORTED_LOCATION ${FLESNET_DESTDIR}/src/flesnet-build/lib/fles_ipc/${CMAKE_STATIC_LIBRARY_PREFIX}fles_ipc${CMAKE_STATIC_LIBRARY_SUFFIX}
  INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/flesnet/lib/fles_ipc
  IMPORTED_LINK_INTERFACE_LIBRARIES "${dir_to_link}"
)



Install(FILES ${FLESNET_DESTDIR}/src/flesnet-build/lib/fles_ipc/${CMAKE_STATIC_LIBRARY_PREFIX}fles_ipc${CMAKE_STATIC_LIBRARY_SUFFIX}
              ${FLESNET_DESTDIR}/src/flesnet-build/lib/shm_ipc/${CMAKE_STATIC_LIBRARY_PREFIX}shm_ipc${CMAKE_STATIC_LIBRARY_SUFFIX}
              ${FLESNET_DESTDIR}/src/flesnet-build/src/zeromq-build/lib/${CMAKE_STATIC_LIBRARY_PREFIX}zmq${CMAKE_STATIC_LIBRARY_SUFFIX}
        DESTINATION lib
       )

file(GLOB LIB_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/flesnet/lib/fles_ipc/*.hpp)

Install(FILES ${LIB_HEADERS}
        DESTINATION include
       )
