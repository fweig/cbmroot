add_subdirectory(log)
add_subdirectory(data)
add_subdirectory(test)

set(DEVICE_SRCS
  base/gpu/DeviceImage.cxx
  base/gpu/Params.cxx
  detectors/sts/UnpackStsXpu.cxx
  detectors/sts/StsHitfinder.cxx
)

set(SRCS
  ${DEVICE_SRCS}
  base/Options.cxx
  base/Types.cxx
  base/util/TimingsFormat.cxx
  data/sts/LandauTable.cxx
  evbuild/EventBuilder.cxx
  trigger/TimeClusterTrigger.cxx
  evselector/DigiEventSelector.cxx
  detectors/sts/StsHitfinderChain.cxx
  detectors/sts/StsReadoutConfig.cxx
  detectors/sts/StsUnpackChain.cxx
  detectors/sts/UnpackSts.cxx
  detectors/much/MuchReadoutConfig.cxx
  detectors/much/UnpackMuch.cxx
  detectors/tof/HitFinderTof.cxx
  detectors/tof/TofReadoutConfig.cxx
  detectors/tof/UnpackTof.cxx
  detectors/bmon/BmonReadoutConfig.cxx
  detectors/bmon/UnpackBmon.cxx
  detectors/trd/TrdReadoutConfig.cxx
  detectors/trd/UnpackTrd.cxx
  detectors/trd2d/Trd2dReadoutConfig.cxx
  detectors/trd2d/UnpackTrd2d.cxx
  global/Reco.cxx
)

set(BUILD_INFO_CXX ${CMAKE_CURRENT_BINARY_DIR}/base/BuildInfo.cxx)
set(SRCS ${SRCS} ${BUILD_INFO_CXX})

add_custom_target(generateBuildInfo
    COMMAND ${CMAKE_COMMAND}
        -DBUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DXPU_DEBUG=${XPU_DEBUG}
        -DINFILE=${CMAKE_CURRENT_SOURCE_DIR}/base/BuildInfo.cxx.in
        -DOUTFILE=${BUILD_INFO_CXX}
        -P ${CMAKE_SOURCE_DIR}/cmake/modules/GenerateBuildInfo.cmake
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/base/BuildInfo.cxx.in
            ${CMAKE_SOURCE_DIR}/cmake/modules/GenerateBuildInfo.cmake
    BYPRODUCTS ${BUILD_INFO_CXX}
    COMMENT "Generating build info"
)

add_library(Algo SHARED ${SRCS})

target_include_directories(Algo
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
         ${CMAKE_CURRENT_SOURCE_DIR}/data
         ${CMAKE_CURRENT_SOURCE_DIR}/base
         ${CMAKE_CURRENT_SOURCE_DIR}/evbuild
         ${CMAKE_CURRENT_SOURCE_DIR}/global
         ${CMAKE_CURRENT_SOURCE_DIR}/trigger
         ${CMAKE_CURRENT_SOURCE_DIR}/evselector
         ${CMAKE_CURRENT_SOURCE_DIR}/detectors/sts
         ${CMAKE_CURRENT_SOURCE_DIR}/detectors/much
         ${CMAKE_CURRENT_SOURCE_DIR}/detectors/tof
         ${CMAKE_CURRENT_SOURCE_DIR}/detectors/bmon
         ${CMAKE_CURRENT_SOURCE_DIR}/detectors/trd
         ${CMAKE_CURRENT_SOURCE_DIR}/detectors/trd2d
 )

target_link_libraries(Algo
  PUBLIC    OnlineData
            ROOT::GenVector
            GSL
            fmt::fmt
            Boost::program_options
            Boost::filesystem
            xpu
            external::yaml-cpp
            external::fles_logging
            external::fles_ipc
)
target_compile_definitions(Algo PUBLIC NO_ROOT)
xpu_attach(Algo ${DEVICE_SRCS})

install(TARGETS Algo DESTINATION lib)
install(DIRECTORY base/compat TYPE INCLUDE FILES_MATCHING PATTERN "*.h")
install(DIRECTORY base/config TYPE INCLUDE FILES_MATCHING PATTERN "*.h")
install(DIRECTORY base/gpu TYPE INCLUDE FILES_MATCHING PATTERN "*.h")
install(DIRECTORY data/sts TYPE INCLUDE FILES_MATCHING PATTERN "*.h")

install(
  FILES
    base/ChainContext.h
    base/Options.h
    base/Prelude.h
    base/RecoParams.h
    base/SubChain.h
    base/Types.h
    ca/simd/CaSimd.h
    ca/simd/CaSimdVc.h
    ca/simd/CaSimdPseudo.h
    detectors/sts/StsHitfinder.h
    detectors/sts/StsHitfinderChain.h
  DESTINATION
    include/
)
