add_subdirectory (DataTreeCbmInterface)


#set(UNIGENQA_VERSION "1.0")
#set(UNIGENQA_SRC_URL "https://cbmgsi.githost.io/pwg-c2f/UnigenQA.git")
#set(UNIGENQA_DESTDIR "${CMAKE_BINARY_DIR}/analysis/flow/UNIGENQA-prefix")
#set(UNIGENQA_LIBNAME "${CMAKE_SHARED_LIBRARY_PREFIX}UnigenQA${CMAKE_SHARED_LIBRARY_SUFFIX}")

#download_project(PROJ            UnigenQA_source
#                 GIT_REPOSITORY  ${UNIGENQA_SRC_URL}
#                 GIT_TAG         ${UNIGENQA_VERSION}
#                 SOURCE_DIR      ${CMAKE_CURRENT_SOURCE_DIR}/UnigenQA
#)


#If(ProjectUpdated)
#  File(REMOVE_RECURSE ${UNIGENQA_DESTDIR})
#  Message("UnigenQA source directory was changed so build directory was deleted")  
#EndIf()

#ExternalProject_Add(UNIGENQA
#  BUILD_IN_SOURCE 0
#  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/UnigenQA
#  BUILD_BYPRODUCTS ${UNIGENQA_LIBRARY}
#  LOG_DOWNLOAD 1 LOG_CONFIGURE 1 LOG_BUILD 1 LOG_INSTALL 1
#  CMAKE_ARGS -G ${CMAKE_GENERATOR}
#             -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
#             -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
#             -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
#             -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
#             -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
#             -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}
#             -DFIXTARGET=TRUE
#             -DROOTSYS=${SIMPATH}
#             -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON
#  INSTALL_COMMAND  ${CMAKE_COMMAND} --build . --target install
#)

#add_library(UnigenQA SHARED IMPORTED)
#set_target_properties(UnigenQA PROPERTIES IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/lib)
#add_dependencies(UnigenQA UNIGENQA)

#set(UnigenQA_LIB_DIR ${CMAKE_BINARY_DIR}/lib)
#set(UnigenQA_LIBRARIES UnigenQA)
#set(UnigenQA_INCLUDE_DIR "${CMAKE_BINARY_DIR}/include")
#set(UnigenQA_FOUND TRUE)

#Install(FILES ${CMAKE_BINARY_DIR}/lib/${UNIGENQA_LIBNAME}
#              ${CMAKE_BINARY_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}UnigenQA.rootmap
#              ${CMAKE_BINARY_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}UnigenQA_rdict.pcm
#        DESTINATION lib
#       )

