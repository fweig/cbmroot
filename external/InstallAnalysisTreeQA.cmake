set(ANALYSISTREEQA_VERSION v1.0.1)

set(ANALYSISTREEQA_SRC_URL "https://github.com/HeavyIonAnalysis/AnalysisTreeQA.git")
set(ANALYSISTREEQA_DESTDIR "${CMAKE_BINARY_DIR}/external/ANALYSISTREEQA-prefix")
set(ANALYSISTREEQA_LIBNAME "${CMAKE_SHARED_LIBRARY_PREFIX}AnalysisTreeQA${CMAKE_STATIC_LIBRARY_SUFFIX}")

download_project_if_needed(PROJECT         AnalysisTreeQA_source
        GIT_REPOSITORY  ${ANALYSISTREEQA_SRC_URL}
        GIT_TAG         ${ANALYSISTREEQA_VERSION}
        SOURCE_DIR      ${CMAKE_CURRENT_SOURCE_DIR}/AnalysisTreeQA
        TEST_FILE       CMakeLists.txt
        )

If(ProjectUpdated)
    File(REMOVE_RECURSE ${ANALYSISTREEQA_DESTDIR})
    Message("AnalysisTreeQA source directory was changed so build directory was deleted")
EndIf()

ExternalProject_Add(ANALYSISTREEQA
        DEPENDS AnalysisTreeBase
        BUILD_IN_SOURCE 0
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/AnalysisTreeQA
        BUILD_BYPRODUCTS ${ANALYSISTREEQA_LIBRARY}
        LOG_DOWNLOAD 1 LOG_CONFIGURE 1 LOG_BUILD 1 LOG_INSTALL 1
        CMAKE_ARGS -G ${CMAKE_GENERATOR}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
        -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}
        -DCMAKE_CXX_STANDARD=11
        -DROOTSYS=${SIMPATH}
        -DUSEBOOST=TRUE
        -DBOOST_ROOT=${SIMPATH}
        -DBoost_NO_BOOST_CMAKE=ON
        -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON
        INSTALL_COMMAND  ${CMAKE_COMMAND} --build . --target install
        )

add_library(AnalysisTreeQA SHARED IMPORTED)
set_target_properties(AnalysisTreeQA PROPERTIES IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/lib)
add_dependencies(AnalysisTreeQA ANALYSISTREE)

set(AnalysisTreeQA_LIB_DIR ${CMAKE_BINARY_DIR}/lib)
set(AnalysisTreeQA_LIBRARIES AnalysisTreeQA)
set(AnalysisTreeQA_INCLUDE_DIR "${CMAKE_BINARY_DIR}/include")
set(AnalysisTreeQA_FOUND TRUE)

Install(FILES ${CMAKE_BINARY_DIR}/lib/${ANALYSISTREEQA_LIBNAME}
        ${CMAKE_BINARY_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}AnalysisTreeQA.rootmap
        ${CMAKE_BINARY_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}AnalysisTreeQA_rdict.pcm
        DESTINATION lib
        )
