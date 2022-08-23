# Create a library called "libCbmOnlineData" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(SRCS
  ${CMAKE_SOURCE_DIR}/core/data/CbmDefs.cxx
  ${CMAKE_SOURCE_DIR}/core/data/CbmAddress.cxx

  ${CMAKE_SOURCE_DIR}/core/data/bmon/CbmBmonDigi.cxx

  ${CMAKE_SOURCE_DIR}/core/data/sts/CbmStsAddress.cxx
  ${CMAKE_SOURCE_DIR}/core/data/sts/CbmStsDigi.cxx

  ${CMAKE_SOURCE_DIR}/core/data/rich/CbmRichDigi.cxx

  ${CMAKE_SOURCE_DIR}/core/data/much/CbmMuchDigi.cxx
  ${CMAKE_SOURCE_DIR}/core/data/much/CbmMuchAddress.cxx

  ${CMAKE_SOURCE_DIR}/core/data/trd/CbmTrdDigi.cxx

  ${CMAKE_SOURCE_DIR}/core/data/tof/CbmTofDigi.cxx
  ${CMAKE_SOURCE_DIR}/core/data/tof/CbmTofAddress.cxx
  ${CMAKE_SOURCE_DIR}/core/data/tof/CbmTofDetectorId.cxx
  ${CMAKE_SOURCE_DIR}/core/data/tof/CbmTofDetectorId_v12b.cxx

  ${CMAKE_SOURCE_DIR}/core/data/psd/CbmPsdDigi.cxx
  ${CMAKE_SOURCE_DIR}/core/data/psd/CbmPsdAddress.cxx

  ${CMAKE_SOURCE_DIR}/core/data/raw/CriGet4Mess001.cxx
  ${CMAKE_SOURCE_DIR}/core/data/raw/StsXyterMessage.cxx
)

add_library(OnlineData SHARED ${SRCS})

target_include_directories(OnlineData
  PUBLIC ${CMAKE_SOURCE_DIR}/core/data
  PUBLIC ${CMAKE_SOURCE_DIR}/core/data/base
  PUBLIC ${CMAKE_SOURCE_DIR}/core/data/bmon
  PUBLIC ${CMAKE_SOURCE_DIR}/core/data/sts
  PUBLIC ${CMAKE_SOURCE_DIR}/core/data/rich
  PUBLIC ${CMAKE_SOURCE_DIR}/core/data/much
  PUBLIC ${CMAKE_SOURCE_DIR}/core/data/trd
  PUBLIC ${CMAKE_SOURCE_DIR}/core/data/tof
  PUBLIC ${CMAKE_SOURCE_DIR}/core/data/psd
  PUBLIC ${CMAKE_SOURCE_DIR}/core/data/global
  PUBLIC ${CMAKE_SOURCE_DIR}/core/data/raw
)

target_compile_definitions(OnlineData PUBLIC NO_ROOT)
target_link_libraries(OnlineData
                      FairLogger::FairLogger
                      external::fles_ipc
                      xpu
                      Boost::serialization
                     )

install(TARGETS OnlineData DESTINATION lib)
