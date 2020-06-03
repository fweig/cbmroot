set(DATATREE_VERSION f9b31d3d3362f5a98665d8d192440fba98181f31)
#set(DATATREE_VERSION a99224d7e9fd20b7a7e3b203cfe2778af8632ecb)

set(DATATREE_SRC_URL "https://git.cbm.gsi.de/pwg-c2f/DataTree.git")
set(DATATREE_DESTDIR "${CMAKE_BINARY_DIR}/external/DATATREE-prefix")
set(DATATREE_LIBNAME "${CMAKE_SHARED_LIBRARY_PREFIX}DataTree${CMAKE_SHARED_LIBRARY_SUFFIX}")

download_project_if_needed(PROJECT         DataTree_source
                           GIT_REPOSITORY  ${DATATREE_SRC_URL}
                           GIT_TAG         ${DATATREE_VERSION}
                           SOURCE_DIR      ${CMAKE_CURRENT_SOURCE_DIR}/DataTree
                           TEST_FILE       CMakeLists.txt
                          )

If(ProjectUpdated)
  File(REMOVE_RECURSE ${DATATREE_DESTDIR})
  Message("DataTree source directory was changed so build directory was deleted")  
EndIf()

ExternalProject_Add(DATATREE
  BUILD_IN_SOURCE 0
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/DataTree
  BUILD_BYPRODUCTS ${DATATREE_LIBRARY}
  LOG_DOWNLOAD 1 LOG_CONFIGURE 1 LOG_BUILD 1 LOG_INSTALL 1
  CMAKE_ARGS -G ${CMAKE_GENERATOR}
             -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
             -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
             -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
             -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
             -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
             -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}
             -DFIXTARGET=TRUE
             -DROOTSYS=${SIMPATH}
             -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON
             -DEXPERIMENT=CBM
  INSTALL_COMMAND  ${CMAKE_COMMAND} --build . --target install
)

add_library(DataTree SHARED IMPORTED)
set_target_properties(DataTree PROPERTIES IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/lib)
add_dependencies(DataTree DATATREE)

set(DataTree_LIB_DIR ${CMAKE_BINARY_DIR}/lib)
set(DataTree_LIBRARIES DataTree)
set(DataTree_INCLUDE_DIR "${CMAKE_BINARY_DIR}/include")
set(DataTree_FOUND TRUE)

Install(FILES ${CMAKE_BINARY_DIR}/lib/${DATATREE_LIBNAME} 
              ${CMAKE_BINARY_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}DataTree.rootmap
              ${CMAKE_BINARY_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}DataTree_rdict.pcm
        DESTINATION lib
       )
