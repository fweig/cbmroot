# Build GTEST as external project
set(_LIBDIR_DEFAULT "lib")

set(Gtest_DESTDIR "${CMAKE_BINARY_DIR}/GTEST-prefix")
set(Gtest_ROOTDIR "${Gtest_DESTDIR}/${CMAKE_BINARY_DIR}")
set(Gtest_LIBNAME "${CMAKE_STATIC_LIBRARY_PREFIX}gtest${CMAKE_STATIC_LIBRARY_SUFFIX}")
set(Gtest_LIBRARY "${Gtest_ROOTDIR}/${_LIBDIR_DEFAULT}/${Gtest_LIBNAME}")

set(Gtest_VERSION 870a6b5563e065b4b9fc3b633c4015ee17d2cbaa) # Hash
set(Gtest_SRC_URL "https://github.com/google/googletest")

download_project_if_needed(PROJECT         gtest_source
                           GIT_REPOSITORY  ${Gtest_SRC_URL}
                           GIT_TAG         ${Gtest_VERSION}
                           SOURCE_DIR      ${CMAKE_SOURCE_DIR}/external/googletest
                           TEST_FILE       CMakeLists.txt
                          )
If(ProjectUpdated)
  File(REMOVE_RECURSE ${Gtest_DESTDIR})
  Message("GTEST source directory was changed so build directory was deleted")
EndIf()

ExternalProject_Add(GTEST
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/googletest
  BUILD_IN_SOURCE 0
  BUILD_BYPRODUCTS ${Gtest_LIBRARY}
  LOG_DOWNLOAD 1 LOG_CONFIGURE 1 LOG_BUILD 1 LOG_INSTALL 1
  CMAKE_ARGS -G ${CMAKE_GENERATOR}
             -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
             -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
             -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
             -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
             -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
             -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}
             -DCMAKE_INSTALL_LIBDIR=${CMAKE_INSTALL_LIBDIR}
  INSTALL_COMMAND ${CMAKE_COMMAND} --build . --target install
)

add_library(Gtest STATIC IMPORTED)
set_target_properties(Gtest PROPERTIES IMPORTED_LOCATION ${Gtest_LIBRARY})
add_dependencies(Gtest GTEST)

set(GTEST_LIBRARIES gtest)
set(GTEST_INCLUDE_DIR "${CMAKE_BINARY_DIR}/include")
set(GTEST_LIBRARY  ${CMAKE_BINARY_DIR}/${_LIBDIR_DEFAULT}/${CMAKE_STATIC_LIBRARY_PREFIX}gtest${CMAKE_STATIC_LIBRARY_SUFFIX})
set(GTEST_MAIN_LIBRARY ${CMAKE_BINARY_DIR}/${_LIBDIR_DEFAULT}/${CMAKE_STATIC_LIBRARY_PREFIX}gtest_main${CMAKE_STATIC_LIBRARY_SUFFIX})
set(GTEST_BOTH_LIBRARIES "${GTEST_LIBRARY};${GTEST_MAIN_LIBRARY}")

Include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GTEST
  FOUND_VAR GTEST_FOUND
  REQUIRED_VARS GTEST_INCLUDE_DIR GTEST_LIBRARY GTEST_MAIN_LIBRARY GTEST_BOTH_LIBRARIES
)

#Installation is not needed and currently fails to a strange PATH
#install(DIRECTORY ${Gtest_ROOTDIR}/ DESTINATION ".")

#if(GTEST_FOUND)
#  set(GTEST_INCLUDE_DIRS ${Gtest_INCLUDE_DIR})
#  set(GTEST_LIB_DIR ${Gtest_ROOTDIR}/${_LIBDIR_DEFAULT})
#endif()
