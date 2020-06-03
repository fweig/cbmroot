set(DATATREEQA_VERSION e549c95c1489012a3fa2c5d0c8908478babd08b7) # hash is tag 1.0. Needed for test
set(DATATREEQA_SRC_URL "https://git.cbm.gsi.de/pwg-c2f/DataTreeQA.git")
set(DATATREEQA_DESTDIR "${CMAKE_BINARY_DIR}/external/DATATREEQA-prefix")
set(DATATREEQA_LIBNAME "${CMAKE_SHARED_LIBRARY_PREFIX}DataTreeQA${CMAKE_SHARED_LIBRARY_SUFFIX}")


download_project_if_needed(PROJECT         DataTreeQA_source
                           GIT_REPOSITORY  ${DATATREEQA_SRC_URL}
                           GIT_TAG         ${DATATREEQA_VERSION}
                           SOURCE_DIR      ${CMAKE_CURRENT_SOURCE_DIR}/DataTreeQA
                           TEST_FILE       CMakeLists.txt
                          )

If(ProjectUpdated)
  File(REMOVE_RECURSE ${DATATREEQA_DESTDIR})
  Message("DataTreeQA source directory was changed so build directory was deleted")  
EndIf()

ExternalProject_Add(DATATREEQA
  DEPENDS DataTree
  BUILD_IN_SOURCE 0
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/DataTreeQA
  BUILD_BYPRODUCTS ${DATATREEQA_LIBRARY}
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

add_library(DataTreeQA SHARED IMPORTED)
set_target_properties(DataTreeQA PROPERTIES IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/lib)
add_dependencies(DataTreeQA DATATREE)

set(DataTreeQA_LIB_DIR ${CMAKE_BINARY_DIR}/lib)
set(DataTreeQA_LIBRARIES DataTreeQA)
set(DataTreeQA_INCLUDE_DIR "${CMAKE_BINARY_DIR}/include")
set(DataTreeQA_FOUND TRUE)

Install(FILES ${CMAKE_BINARY_DIR}/lib/${DATATREEQA_LIBNAME}
              ${CMAKE_BINARY_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}DataTreeQA.rootmap
              ${CMAKE_BINARY_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}DataTreeQA_rdict.pcm
        DESTINATION lib
       )
