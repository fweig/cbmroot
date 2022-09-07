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
  PUBLIC ${CMAKE_SOURCE_DIR}/core/data
  PUBLIC ${CMAKE_SOURCE_DIR}/core/data/base
  PUBLIC ${CMAKE_SOURCE_DIR}/core/data/global
  PUBLIC ${CMAKE_SOURCE_DIR}/core/data/sts
  PUBLIC ${CMAKE_SOURCE_DIR}/core/data/rich
  PUBLIC ${CMAKE_SOURCE_DIR}/core/data/much
  PUBLIC ${CMAKE_SOURCE_DIR}/core/data/trd
  PUBLIC ${CMAKE_SOURCE_DIR}/core/data/tof
  PUBLIC ${CMAKE_SOURCE_DIR}/core/data/psd
  PUBLIC ${CMAKE_SOURCE_DIR}/core/data/raw
  PUBLIC ${FLES_IPC_INCLUDE_DIRECTORY}
)

target_include_directories(Algo SYSTEM
  PUBLIC ${Boost_INCLUDE_DIR}
)

target_link_libraries(Algo OnlineData)

target_compile_definitions(Algo PUBLIC NO_ROOT)

install(TARGETS Algo DESTINATION lib)

install(FILES ca/simd/CaSimd.h
  ca/simd/CaSimdVc.h
  ca/simd/CaSimdPseudo.h
  DESTINATION include/ca/simd
)
