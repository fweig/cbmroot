# CMakeList file for library CbmSim
# V. Friese, 1 August 2019


# -----   Library name   ----------------------------------
Set(LIBRARY_NAME CbmSimBase)
# ---------------------------------------------------------

# -----  Compilation sources   ----------------------------
set(SRCS
CbmBeam.cxx
CbmBeamProfile.cxx
CbmEventGenerator.cxx
CbmMCEventFilter.cxx
CbmStack.cxx
CbmStackFilter.cxx
CbmTarget.cxx
)
# ---------------------------------------------------------



# ----  Include directories -------------------------------
set(INCLUDE_DIRECTORIES
${CBMDATA_DIR}
${CBMROOT_SOURCE_DIR}/sim
${CBMROOT_SOURCE_DIR}/sim/transport
${CBMROOT_SOURCE_DIR}/sim/transport/base
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
