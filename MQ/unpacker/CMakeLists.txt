configure_file(${CMAKE_CURRENT_SOURCE_DIR}/startMQSamplerUnpackerParserver.sh.in ${CMAKE_BINARY_DIR}/bin/MQ/topologies/startMQSamplerUnpackerParserver.sh)

set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

set(EXECUTABLE_OUTPUT_PATH "${EXECUTABLE_OUTPUT_PATH}/MQ/unpacker")
# set the install dir
set(BIN_DESTINATION bin/MQ/unpacker)

If(UNIX AND NOT APPLE)
  List(APPEND BOOST_LIBS pthread)
EndIf()

set(PUBLIC_DEPS
  CbmData
  CbmMQBase
  CbmTofBase
  CbmTofReco
  )

set(PRIVATE_DEPS
  CbmBase
  CbmFlibMcbm2018
  FairLogger::FairLogger
  FairMQ::Tools
  FairRoot::ParBase
  ROOT::Hist
  external::fles_ipc
  ROOT::Net
  )

set(INTERFACE_DEPS
  FairMQ::FairMQ
  )

set(EXE_NAME UnpackTofMcbm2018)
set(SRCS CbmTbDaqBuffer.cxx CbmDeviceUnpackTofMcbm2018.cxx runUnpackTofMcbm2018.cxx)
set(PUBLIC_DEPENDENCIES ${PUBLIC_DEPS}) 
set(PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}) 
set(INTERFACE_DEPENDENCIES ${INTERFACE_DEPS})

generate_cbm_executable()

set(EXE_NAME UnpackTofCri)
set(SRCS CbmTbDaqBuffer.cxx CbmDeviceUnpackTofCri.cxx runUnpackTofCri.cxx)
set(PUBLIC_DEPENDENCIES ${PUBLIC_DEPS}) 
set(PRIVATE_DEPENDENCIES ${PRIVATE_DEPS}) 
set(INTERFACE_DEPENDENCIES ${INTERFACE_DEPS})

generate_cbm_executable()

# Install script and input file(s)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/MapTofHdCosmics_v18d.par
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/cbmroot/input
       )

if(EXISTS ${VMCWORKDIR}/input/tofget4_hd2018.tsa)
  install(FILES ${VMCWORKDIR}/input/tofget4_hd2018.tsa
          DESTINATION ${CMAKE_INSTALL_PREFIX}/share/cbmroot/input
         )
endif()

# Set the correct variables for the installation
set(VMCWORKDIR ${CMAKE_INSTALL_PREFIX}/share/cbmroot)

set(MY_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CURRENT_SOURCE_DIR ${VMCWORKDIR}/input)

set(TMPDIR "${CMAKE_BINARY_DIR}")
set(CMAKE_BINARY_DIR ${CMAKE_INSTALL_PREFIX})

# Configure file for installation directory
configure_file(${MY_SOURCE_DIR}/startMQSamplerUnpackerParserver.sh.in ${TMPDIR}/bin/MQ/topologies/install/startMQSamplerUnpackerParserver.sh)

install(PROGRAMS ${TMPDIR}/bin/MQ/topologies/install/startMQSamplerUnpackerParserver.sh
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/MQ/topologies
       )
