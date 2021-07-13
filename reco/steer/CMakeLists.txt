# CMakeList file for library CbmRecoSteer
# V. Friese,     2 June 2021



# -----   Library name   ----------------------------------
Set(LIBRARY_NAME CbmRecoSteer)
# ---------------------------------------------------------

# -----  Compilation sources   ----------------------------
set(SRCS
CbmRecoUnpack.cxx
CbmSourceTsArchive.cxx
)
# ---------------------------------------------------------



# ----  Include directories -------------------------------
set(INCLUDE_DIRECTORIES
${CBMROOT_SOURCE_DIR}/reco/steer
${CBMROOT_SOURCE_DIR}/reco/base
${CBMROOT_SOURCE_DIR}/reco/detectors/trd
${CBMROOT_SOURCE_DIR}/reco/detectors/trd/rawToDigiMethods
${CBMROOT_SOURCE_DIR}/reco/detectors/trd/unpack

${CBMROOT_SOURCE_DIR}/core/base
${CBMROOT_SOURCE_DIR}/core/data
${CBMROOT_SOURCE_DIR}/core/data/base
${CBMROOT_SOURCE_DIR}/core/data/trd
${CBMROOT_SOURCE_DIR}/core/detectors/trd
${CBMROOT_SOURCE_DIR}/core/data/sts


)

set(SYSTEM_INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${IPC_INCLUDE_DIRECTORY}
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
fles_ipc
Base
CbmBase
CbmTrdReco
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
