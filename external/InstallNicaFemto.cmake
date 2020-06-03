set(NICAFEMTO_VERSION 7c5c9286cc18da15b80baaaa972010f0ee134ebe)

set(NICAFEMTO_SRC_URL "https://git.jinr.ru/nica/nicafemto.git")
set(NICAFEMTO_DESTDIR "${CMAKE_BINARY_DIR}/external/NICAFEMTO-prefix")
set(JSROOT_SRC_URL "https://github.com/root-project/jsroot.git")
set(JSROOT_VERSION "5.7.1")


list(APPEND NICA_LIB_LIST "NicaAna" "NicaCut" "NicaDataFormat" "NicaFeatures" "NicaFemto")
list(APPEND NICA_LIB_LIST "NicaFlow" "NicaFluct" "NicaGen" "NicaSpectra")

download_project_if_needed(PROJECT         NicaFemto_source
                           GIT_REPOSITORY  ${NICAFEMTO_SRC_URL}
                           GIT_TAG         ${NICAFEMTO_VERSION}
                           SOURCE_DIR      ${CMAKE_CURRENT_SOURCE_DIR}/NicaFemto
                          )

If(ProjectUpdated)
  File(REMOVE_RECURSE ${NICAFEMTO_DESTDIR})
  Message("NicaFemto source directory was changed so build directory was deleted")
EndIf()

ExternalProject_Add(NICAFEMTO
  BUILD_IN_SOURCE 0
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/NicaFemto
  BUILD_BYPRODUCTS ${NICAFEMTO_LIBRARY}
  LOG_DOWNLOAD 1 LOG_CONFIGURE 1 LOG_BUILD 1 LOG_INSTALL 1
  CMAKE_ARGS -G ${CMAKE_GENERATOR}
             -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
             -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
             -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
             -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
             -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
             -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}
             -DROOTSYS=${SIMPATH}
	     -DJSROOT=${CMAKE_CURRENT_SOURCE_DIR}
             -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON
  INSTALL_COMMAND  ${CMAKE_COMMAND} --build . --target install
)

add_library(NicaFeatures SHARED IMPORTED)
set_target_properties(NicaFeatures PROPERTIES IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/lib)
add_dependencies(NicaFeatures NICAFEMTO)


set(NicaFemto_LIBRARIES NicaFeatures PARENT_SCOPE)
set(NicaFemto_LIB_DIR ${CMAKE_BINARY_DIR}/lib PARENT_SCOPE)
set(NicaFemto_INCLUDE_DIR "${CMAKE_BINARY_DIR}/include" PARENT_SCOPE)
set(NicaFemto_FOUND TRUE PARENT_SCOPE)

foreach(LIB_NAME ${NICA_LIB_LIST})
  if(APPLE)
    Install( FILES
    "${CMAKE_BINARY_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}${LIB_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}"
    "${CMAKE_BINARY_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}${LIB_NAME}.0${CMAKE_SHARED_LIBRARY_SUFFIX}"
    "${CMAKE_BINARY_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}${LIB_NAME}.1${CMAKE_SHARED_LIBRARY_SUFFIX}"
    "${CMAKE_BINARY_DIR}/lib/G__${LIB_NAME}Dict_rdict.pcm"
    "${CMAKE_BINARY_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}${LIB_NAME}.rootmap"
  DESTINATION lib)
  else()
    Install( FILES
      "${CMAKE_BINARY_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}${LIB_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}"
      "${CMAKE_BINARY_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}${LIB_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}.0"
      "${CMAKE_BINARY_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}${LIB_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}.1"
      "${CMAKE_BINARY_DIR}/lib/G__${LIB_NAME}Dict_rdict.pcm"
      "${CMAKE_BINARY_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}${LIB_NAME}.rootmap"
    DESTINATION lib)
  endif()
endforeach(LIB_NAME)

## addditional stuff
Install(FILES ${CMAKE_BINARY_DIR}/bin/nica-report
	          ${CMAKE_BINARY_DIR}/bin/nica-merger
        DESTINATION bin
)

Install(DIRECTORY
		${CMAKE_CURRENT_SOURCE_DIR}/jsroot
		${CMAKE_CURRENT_SOURCE_DIR}/NicaFemto/features/nicafemto_plus
        DESTINATION share
)





