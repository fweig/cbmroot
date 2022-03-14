# CMakeList file for library CbmRecoSteer
# V. Friese,     2 June 2021



# -----   Library name   ----------------------------------
Set(LIBRARY_NAME CbmRecoSteer)
# ---------------------------------------------------------

# -----  Compilation sources   ----------------------------
set(SRCS
CbmReco.cxx
CbmRecoUnpack.cxx
CbmSourceTsArchive.cxx
)
# ---------------------------------------------------------



# ----  Include directories -------------------------------
set(INCLUDE_DIRECTORIES
${CBMROOT_SOURCE_DIR}/reco/steer
${CBMROOT_SOURCE_DIR}/reco/base
${CBMROOT_SOURCE_DIR}/reco/tasks

${CBMROOT_SOURCE_DIR}/reco/detectors/psd
${CBMROOT_SOURCE_DIR}/reco/detectors/psd/unpack
${CBMROOT_SOURCE_DIR}/reco/detectors/rich/unpack
${CBMROOT_SOURCE_DIR}/reco/detectors/sts/
${CBMROOT_SOURCE_DIR}/reco/detectors/sts/unpack
${CBMROOT_SOURCE_DIR}/reco/detectors/tof/unpack
${CBMROOT_SOURCE_DIR}/reco/detectors/trd
${CBMROOT_SOURCE_DIR}/reco/detectors/trd/rawToDigiMethods
${CBMROOT_SOURCE_DIR}/reco/detectors/trd/unpack

${CBMROOT_SOURCE_DIR}/core/base
${CBMROOT_SOURCE_DIR}/core/data
${CBMROOT_SOURCE_DIR}/core/qa
${CBMROOT_SOURCE_DIR}/core/data/raw
${CBMROOT_SOURCE_DIR}/core/data/base
${CBMROOT_SOURCE_DIR}/core/data/psd
${CBMROOT_SOURCE_DIR}/core/data/rich
${CBMROOT_SOURCE_DIR}/core/data/sts
${CBMROOT_SOURCE_DIR}/core/data/tof
${CBMROOT_SOURCE_DIR}/core/data/trd
${CBMROOT_SOURCE_DIR}/core/data/much
${CBMROOT_SOURCE_DIR}/core/data/global

${CBMROOT_SOURCE_DIR}/core/detectors/psd
${CBMROOT_SOURCE_DIR}/core/detectors/rich
${CBMROOT_SOURCE_DIR}/core/detectors/sts
${CBMROOT_SOURCE_DIR}/core/detectors/tof
${CBMROOT_SOURCE_DIR}/core/detectors/trd

${CBMROOT_SOURCE_DIR}/algo/detectors/sts
${CBMROOT_SOURCE_DIR}/algo/trigger
${CBMROOT_SOURCE_DIR}/algo/evbuild

)

set(SYSTEM_INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${IPC_INCLUDE_DIRECTORY}
)
# ---------------------------------------------------------



# ----  Link directories ----------------------------------
set(LINK_DIRECTORIES
${KFParticle_LIB_DIR} # for rich reco
${FAIRROOT_LIBRARY_DIR}
${ROOT_LIBRARY_DIR}
${Boost_LIBRARY_DIRS}
)
# ---------------------------------------------------------



# -----   Library dependences   ---------------------------
Set(DEPENDENCIES
fles_ipc
Base
CbmBase
CbmPsdBase
CbmPsdReco
CbmRichReco
CbmRecoSts
CbmTofBase
CbmTofReco
CbmTrdReco
CbmRecoTasks
CbmData
KF
KFParticleInterface
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
