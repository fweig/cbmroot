set(KFPARTICLE_LIBNAME "${CMAKE_SHARED_LIBRARY_PREFIX}KFParticle${CMAKE_SHARED_LIBRARY_SUFFIX}")

set(KFPARTICLE_SRC_URL "https://git.cbm.gsi.de/CbmSoft/KFParticle.git")
set(KFPARTICLE_DESTDIR "${CMAKE_BINARY_DIR}/external/KFPARTICLE-prefix")

download_project_if_needed(PROJECT         kfparticle_source
                           GIT_REPOSITORY  ${KFPARTICLE_SRC_URL}
                           GIT_TAG         "24fd63922cbbf8ef1c4d0e4448853a6f984066f1"
                           SOURCE_DIR      ${CMAKE_CURRENT_SOURCE_DIR}/KFParticle
                           TEST_FILE       CMakeLists.txt
                          )

If(ProjectUpdated)
  File(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/external/KFPARTICLE-prefix)
  Message("KFParticle source directory was changed so build directory was deleted")  
EndIf()

ExternalProject_Add(KFPARTICLE
  DEPENDS Vc
  BUILD_IN_SOURCE 0
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/KFParticle
  BUILD_BYPRODUCTS ${KFPARTICLE_LIBRARY}
  LOG_DOWNLOAD 1 LOG_CONFIGURE 1 LOG_BUILD 1 LOG_INSTALL 1
  CMAKE_ARGS -G ${CMAKE_GENERATOR}
             -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
             -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
             -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
             -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
             -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
             -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}
             -DVc_INCLUDE_DIR=${Vc_INCLUDE_DIR}
             -DVc_LIBRARIES=${Vc_LIBRARY}
             -DFIXTARGET=TRUE
             -DROOTSYS=${SIMPATH}
             -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON
  INSTALL_COMMAND  ${CMAKE_COMMAND} --build . --target install
)

add_library(KFParticle SHARED IMPORTED)
set_target_properties(KFParticle PROPERTIES IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/lib)
add_dependencies(KFParticle KFPARTICLE)

set(KFParticle_LIB_DIR ${CMAKE_BINARY_DIR}/lib PARENT_SCOPE)
set(KFParticle_LIBRARIES KFParticle PARENT_SCOPE)
set(KFParticle_INCLUDE_DIR "${CMAKE_BINARY_DIR}/include" PARENT_SCOPE)
set(KFParticle_FOUND TRUE PARENT_SCOPE)

Install(FILES ${CMAKE_BINARY_DIR}/lib/${KFPARTICLE_LIBNAME} 
              ${CMAKE_BINARY_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}KFParticle.rootmap
              ${CMAKE_BINARY_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}KFParticle_rdict.pcm
        DESTINATION lib
       )

Install(FILES ${CMAKE_BINARY_DIR}/include/KFParticleBase.h 
              ${CMAKE_BINARY_DIR}/include/KFParticle.h
              ${CMAKE_BINARY_DIR}/include/KFVertex.h
        DESTINATION include/KFParticle
       )
Install(FILES ${CMAKE_BINARY_DIR}/include/KFMCParticle.h
              ${CMAKE_BINARY_DIR}/include/KFPartEfficiencies.h
        DESTINATION include/KFParticlePerformance
       )
Install(FILES ${CMAKE_BINARY_DIR}/include/KFParticleTest.h
        DESTINATION include/KFParticleTest
       )
Install(FILES ${CMAKE_BINARY_DIR}/include/KFPVertex.h
              ${CMAKE_BINARY_DIR}/include/KFMCCounter.h
        DESTINATION include
       )
