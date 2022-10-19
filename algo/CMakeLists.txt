add_subdirectory(data)
add_subdirectory(test)

# Create a library libCbmAlgo
set(SRCS
  evbuild/EventBuilder.cxx
  trigger/TimeClusterTrigger.cxx
  detectors/sts/StsReadoutConfig.cxx
  detectors/sts/UnpackSts.cxx
  detectors/much/MuchReadoutConfig.cxx
  detectors/much/UnpackMuch.cxx
)

add_library(Algo SHARED ${SRCS})

target_include_directories(Algo
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
         ${CMAKE_CURRENT_SOURCE_DIR}/evbuild
         ${CMAKE_CURRENT_SOURCE_DIR}/trigger
         ${CMAKE_CURRENT_SOURCE_DIR}/detectors/sts
         ${CMAKE_CURRENT_SOURCE_DIR}/detectors/much
 )

target_link_libraries(Algo PUBLIC OnlineData INTERFACE FairLogger::FairLogger external::fles_ipc)
target_compile_definitions(Algo PUBLIC NO_ROOT)

install(TARGETS Algo DESTINATION lib)

install(FILES ca/simd/CaSimd.h
  ca/simd/CaSimdVc.h
  ca/simd/CaSimdPseudo.h
  DESTINATION include/
)
