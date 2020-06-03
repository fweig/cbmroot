# CMakeList file for library CbmSimResponse


# -----   Library name   ----------------------------------
Set(LIBRARY_NAME CbmSimResponse)
# ---------------------------------------------------------


# -----  Compilation sources   ----------------------------
set(SRCS
base/CbmDigitization.cxx
base/CbmDigitizationSource.cxx
base/CbmDigitizeInfo.cxx
base/CbmMCInput.cxx
base/CbmMCInputSet.cxx
base/CbmRunAna.cxx
)
# ---------------------------------------------------------


# ----  Include directories -------------------------------
set(INCLUDE_DIRECTORIES
${CBMROOT_SOURCE_DIR}/sim/response
${CBMROOT_SOURCE_DIR}/sim/response/base

${CBMROOT_SOURCE_DIR}/sim/transport/steer
${CBMROOT_SOURCE_DIR}/sim/transport/geosetup
${CBMROOT_SOURCE_DIR}/sim/transport

${CBMROOT_SOURCE_DIR}/sim/detectors/much
${CBMROOT_SOURCE_DIR}/sim/detectors/psd
${CBMROOT_SOURCE_DIR}/sim/detectors/rich
${CBMROOT_SOURCE_DIR}/sim/detectors/sts
${CBMROOT_SOURCE_DIR}/sim/detectors/tof
${CBMROOT_SOURCE_DIR}/sim/detectors/trd


${CBMBASE_DIR}

${CBMDATA_DIR}
${CBMDATA_DIR}/much
${CBMDATA_DIR}/sts
${CBMDATA_DIR}/rich
${CBMDATA_DIR}/trd
${CBMDATA_DIR}/tof
${CBMDATA_DIR}/psd

${CBMROOT_SOURCE_DIR}/mvd

  ${CBMDETECTORBASE_DIR}/sts

${CBMROOT_SOURCE_DIR}/sts/sim

${CBMROOT_SOURCE_DIR}/rich/sim

${CBMDETECTORBASE_DIR}/rich

${CBMROOT_SOURCE_DIR}/much/sim

  ${CBMDETECTORBASE_DIR}/much

${CBMROOT_SOURCE_DIR}/trd/sim

${CBMROOT_SOURCE_DIR}/tof/sim

${CBMROOT_SOURCE_DIR}/psd/sim
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
${KFParticle_LIB_DIR}
)
# ---------------------------------------------------------



# -----   Library dependences   ---------------------------
Set(DEPENDENCIES
CbmBase
CbmData
CbmMvd
CbmStsSim
CbmRichSim
CbmMuchSim
CbmTrdSim
CbmTofSim
CbmPsdSim
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

