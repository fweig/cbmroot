
# Build Vc as external project
set(_LIBDIR_DEFAULT "lib")

set(Vc_DESTDIR "${CMAKE_BINARY_DIR}/external/VC-prefix")
set(Vc_ROOTDIR "${Vc_DESTDIR}/${CMAKE_BINARY_DIR}")
set(Vc_LIBNAME "${CMAKE_STATIC_LIBRARY_PREFIX}Vc${CMAKE_STATIC_LIBRARY_SUFFIX}")
set(Vc_LIBRARY "${Vc_ROOTDIR}/${_LIBDIR_DEFAULT}/${Vc_LIBNAME}")

set(Vc_VERSION f8a18751dcb001c8d9820a6c90a1f605e27ca692) # Hash for tag 1.4.1, Need hash for tests 
#set(Vc_VERSION fc2d75d0f23f33c2224d546acb965c535a45454a) # Hash for tag 1.4.2, Need hash for tests
set(Vc_SRC_URL "https://github.com/VcDevel/Vc")

download_project_if_needed(PROJECT         vc_source
                           GIT_REPOSITORY  ${Vc_SRC_URL}
                           GIT_TAG         ${Vc_VERSION}
                           SOURCE_DIR      ${CMAKE_CURRENT_SOURCE_DIR}/Vc
                           TEST_FILE       CMakeLists.txt
                          )
If(ProjectUpdated)
  File(REMOVE_RECURSE ${Vc_DESTDIR})
  Message("VC source directory was changed so build directory was deleted")
EndIf()

ExternalProject_Add(VC
#  GIT_REPOSITORY ${Vc_SRC_URL}
#  GIT_TAG ${Vc_VERSION}
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Vc
  BUILD_IN_SOURCE 0   
  BUILD_BYPRODUCTS ${Vc_LIBRARY}
  LOG_DOWNLOAD 1 LOG_CONFIGURE 1 LOG_BUILD 1 LOG_INSTALL 1
  CMAKE_ARGS -G ${CMAKE_GENERATOR}
             -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
             -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
             -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
             -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
             -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
             -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}
#             -DBUILD_TESTING=TRUE
  INSTALL_COMMAND env DESTDIR=${Vc_DESTDIR} ${CMAKE_COMMAND} --build . --target install
)

add_library(Vc STATIC IMPORTED)
set_target_properties(Vc PROPERTIES IMPORTED_LOCATION ${Vc_LIBRARY})
add_dependencies(Vc VC)

set(Vc_LIBRARIES Vc)
set(Vc_INCLUDE_DIR "${Vc_ROOTDIR}/include")
set(Vc_CMAKE_MODULES_DIR "${Vc_ROOTDIR}/${_LIBDIR_DEFAULT}/cmake/Vc")

Include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Vc
  FOUND_VAR Vc_FOUND
  REQUIRED_VARS Vc_INCLUDE_DIR Vc_LIBRARIES Vc_CMAKE_MODULES_DIR
  VERSION_VAR Vc_VERSION
)

install(DIRECTORY ${Vc_ROOTDIR}/ DESTINATION ".")

if(Vc_FOUND)
  # Missing from VcConfig.cmake
  set(VC_INCLUDE_DIRS ${Vc_INCLUDE_DIR} PARENT_SCOPE)
  set(Vc_LIB_DIR ${Vc_ROOTDIR}/${_LIBDIR_DEFAULT} PARENT_SCOPE)
endif()
