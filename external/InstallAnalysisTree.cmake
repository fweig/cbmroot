set(ANALYSISTREE_VERSION 6cfd74c707fee8268ef987fb91d9df6eb667ee2d)  #v2.3.3

set(ANALYSISTREE_SRC_URL "https://github.com/HeavyIonAnalysis/AnalysisTree.git")
set(ANALYSISTREE_DESTDIR "${CMAKE_BINARY_DIR}/external/ANALYSISTREE-prefix")

list(APPEND ANALYSISTREE_LIBNAME "AnalysisTreeBase" "AnalysisTreeInfra")

download_project_if_needed(PROJECT         AnalysisTree_source
        GIT_REPOSITORY  ${ANALYSISTREE_SRC_URL}
        GIT_TAG         ${ANALYSISTREE_VERSION}
        SOURCE_DIR      ${CMAKE_CURRENT_SOURCE_DIR}/AnalysisTree
        TEST_FILE       CMakeLists.txt
        PATCH_COMMAND   "patch -p1 < ${CMAKE_CURRENT_SOURCE_DIR}/AnalysisTree.patch"
        )

If(ProjectUpdated)
    File(REMOVE_RECURSE ${ANALYSISTREE_DESTDIR})
    Message("AnalysisTree source directory was changed so build directory was deleted")
EndIf()

ExternalProject_Add(ANALYSISTREE
        BUILD_IN_SOURCE 0
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/AnalysisTree
        BUILD_BYPRODUCTS ${ANALYSISTREE_LIBRARY}
        LOG_DOWNLOAD 1 LOG_CONFIGURE 1 LOG_BUILD 1 LOG_INSTALL 1
        CMAKE_ARGS -G ${CMAKE_GENERATOR}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
        -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}
        -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
        -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}
        -DBoost_NO_BOOST_CMAKE=ON
        -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON
        INSTALL_COMMAND  ${CMAKE_COMMAND} --build . --target install
        )

add_library(AnalysisTreeBase SHARED IMPORTED GLOBAL)
set_target_properties(AnalysisTreeBase PROPERTIES
  IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}AnalysisTreeBase${CMAKE_SHARED_LIBRARY_SUFFIX}
  INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_BINARY_DIR}/include)
add_dependencies(AnalysisTreeBase ANALYSISTREE)

add_library(AnalysisTreeInfra SHARED IMPORTED GLOBAL)
set_target_properties(AnalysisTreeInfra PROPERTIES
  IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}AnalysisTreeInfra${CMAKE_SHARED_LIBRARY_SUFFIX}
  INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_BINARY_DIR}/include)
add_dependencies(AnalysisTreeInfra ANALYSISTREE)


foreach(LIB_NAME ${ANALYSISTREE_LIBNAME})
    Install( FILES
    ${CMAKE_BINARY_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}${LIB_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}
    ${CMAKE_BINARY_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}${LIB_NAME}_rdict.pcm
    ${CMAKE_BINARY_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}${LIB_NAME}.rootmap
        DESTINATION lib)
endforeach(LIB_NAME)

Install(DIRECTORY  ${CMAKE_BINARY_DIR}/include/AnalysisTree
        DESTINATION include
       )
