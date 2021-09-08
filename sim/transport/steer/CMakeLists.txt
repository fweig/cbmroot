# CMakeList file for library CbmSim
# V. Friese, 1 August 2019


# -----   Library name   ----------------------------------
Set(LIBRARY_NAME CbmSimSteer)
# ---------------------------------------------------------

# -----  Compilation sources   ----------------------------
set(SRCS
CbmSetup.cxx
CbmTransport.cxx
CbmVMCSettings.cxx
CbmGeant3Settings.cxx
CbmGeant4Settings.cxx
CbmTransportConfig.cxx
)
# ---------------------------------------------------------



# ----  Include directories -------------------------------
set(INCLUDE_DIRECTORIES
${SIMPATH}/include/Geant4
${SIMPATH}/include/geant4vmc
${SIMPATH}/include/TGeant3

${CBMROOT_SOURCE_DIR}/generators
${CBMFIELD_DIR}
${CBMROOT_SOURCE_DIR}/sim
${CBMROOT_SOURCE_DIR}/sim/transport
${CBMROOT_SOURCE_DIR}/sim/transport/base
${CBMROOT_SOURCE_DIR}/sim/transport/generators
${CBMROOT_SOURCE_DIR}/sim/transport/steer
${CBMROOT_SOURCE_DIR}/sim/transport/geosetup
${CBMDATA_DIR}
${CBMBASE_DIR}/utils
${CBMROOT_SOURCE_DIR}/core/config
)

set(SYSTEM_INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
)
# ---------------------------------------------------------



# ----  Link directories ----------------------------------
set(LINK_DIRECTORIES
${FAIRROOT_LIBRARY_DIR}
${ROOT_LIBRARY_DIR}
${Boost_LIBRARY_DIRS}
${SIMPATH}/lib
)

# Work around linker problems on macosx 11 (Big Sur)
# Somehow libfreetype isn't properly found in /usr/local/lib
cmake_host_system_information(RESULT _OS_RELEASE QUERY OS_RELEASE)
if(APPLE AND ${_OS_RELEASE} VERSION_GREATER 11)
  Message(STATUS "Fixing linker issue on macosx ${_OS_RELEASE}")
  list(APPEND LINK_DIRECTORIES /usr/local/lib)
endif()
# ---------------------------------------------------------



# -----   Library dependences   ---------------------------
Set(DEPENDENCIES
CbmBase
CbmData
CbmSimBase
CbmSimGenerators
CbmField
CbmSimGenerators
Gen
geant321
geant4vmc
EGPythia6
boost_filesystem
CbmGeoSetup
CbmStsSim
)
# ---------------------------------------------------------


# -----  LinkDef file   -----------------------------------
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
# ---------------------------------------------------------


# -----  Let cmake do the job   ---------------------------
include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})
link_directories( ${LINK_DIRECTORIES})
GENERATE_LIBRARY()
# ---------------------------------------------------------
