set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CBMROOT_SOURCE_DIR}/core/config
  )

set(SRCS
  CbmSetup.cxx
  CbmTransport.cxx
  CbmVMCSettings.cxx
  CbmGeant3Settings.cxx
  CbmGeant4Settings.cxx
  CbmTransportConfig.cxx
  )


# Work around linker problems on macosx 11 (Big Sur)
# Somehow libfreetype isn't properly found in /usr/local/lib
cmake_host_system_information(RESULT _OS_RELEASE QUERY OS_RELEASE)
if(APPLE AND ${_OS_RELEASE} VERSION_GREATER 11)
  Message(STATUS "Fixing linker issue on macosx ${_OS_RELEASE}")
  list(APPEND LINK_DIRECTORIES /usr/local/lib)
endif()


Set(LIBRARY_NAME CbmSimSteer)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmData
  CbmSimBase
  ROOT::Core
  ROOT::Physics
  CbmGeoSetup
  )

set(PRIVATE_DEPENDENCIES
  CbmBase
  CbmField
  CbmSimGenerators
  FairRoot::Base
  FairRoot::FairTools
  FairRoot::Gen
  FairRoot::ParBase
  geant321
  geant4vmc
  ROOT::EG
  ROOT::EGPythia6
  ROOT::Geom
  ROOT::MathCore
  ROOT::RIO
  )

generate_cbm_library()
