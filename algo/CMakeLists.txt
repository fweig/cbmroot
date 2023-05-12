add_subdirectory(data)
add_subdirectory(test)

set(DEVICE_SRCS
  base/gpu/DeviceImage.cxx
  detectors/sts/UnpackStsXpu.cxx
  detectors/sts/StsHitfinder.cxx
)

set(SRCS
  ${DEVICE_SRCS}
  base/Options.cxx
  evbuild/EventBuilder.cxx
  global/Reco.cxx
  trigger/TimeClusterTrigger.cxx
  evselector/DigiEventSelector.cxx
  detectors/sts/StsHitfinderChain.cxx
  detectors/sts/StsReadoutConfig.cxx
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
)

add_library(Algo SHARED ${SRCS})

target_include_directories(Algo
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
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
 )

target_link_libraries(Algo
  PUBLIC    OnlineData
            ROOT::GenVector
            GSL
            Boost::program_options
            xpu
  INTERFACE FairLogger::FairLogger
            external::fles_ipc
)
target_compile_definitions(Algo PUBLIC NO_ROOT)
xpu_attach(Algo ${DEVICE_SRCS})

install(TARGETS Algo DESTINATION lib)
install(DIRECTORY base/gpu TYPE INCLUDE
  FILES_MATCHING PATTERN "*.h"
)

install(
  FILES       ca/simd/CaSimd.h
              ca/simd/CaSimdVc.h
              ca/simd/CaSimdPseudo.h
              detectors/sts/StsHitfinder.h
              detectors/sts/StsHitfinderChain.h
  DESTINATION include/
)
