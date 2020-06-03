# CMakeList file for library CbmSim
# V. Friese, 1 August 2019


# -----   Library name   ----------------------------------
Set(LIBRARY_NAME CbmSimGenerators)
# ---------------------------------------------------------

# -----  Compilation sources   ----------------------------
set(SRCS
CbmPhsdGenerator.cxx
CbmPlutoGenerator.cxx
CbmShieldGenerator.cxx
CbmUnigenGenerator.cxx
unigen/URun.cxx
unigen/UEvent.cxx
unigen/UParticle.cxx
pluto/PDataBase.cxx
pluto/PMesh.cxx
pluto/PParticle.cxx
pluto/PStaticData.cxx
pluto/PStdData.cxx
pluto/PValues.cxx
)
# ---------------------------------------------------------



# ----  Include directories -------------------------------
set(INCLUDE_DIRECTORIES
${CBMDATA_DIR}
${CBMROOT_SOURCE_DIR}/sim
${CBMROOT_SOURCE_DIR}/sim/transport
${CBMROOT_SOURCE_DIR}/sim/transport/base
${CBMROOT_SOURCE_DIR}/sim/transport/generators
${CBMROOT_SOURCE_DIR}/sim/transport/generators/pluto
${CBMROOT_SOURCE_DIR}/sim/transport/generators/unigen
)

set(SYSTEM_INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
)
# ---------------------------------------------------------



# ----  Link directories ----------------------------------
set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
${Boost_LIBRARY_DIRS}
)
# ---------------------------------------------------------



# -----   Library dependences   ---------------------------
Set(DEPENDENCIES
CbmBase
CbmData
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
