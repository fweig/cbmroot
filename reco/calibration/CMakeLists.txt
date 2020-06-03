# CMakeList file for library CbmRecoBase
# V. Friese,      1 August   2019
# P.-A. Loizeau, 13 February 2020


# -----   Library name   ----------------------------------
Set(LIBRARY_NAME CbmRecoCal)
# ---------------------------------------------------------

# -----  Compilation sources   ----------------------------
set(SRCS
CbmOffsetDigiTimeTester.cxx
)
# ---------------------------------------------------------



# ----  Include directories -------------------------------
set(INCLUDE_DIRECTORIES
${CBMROOT_SOURCE_DIR}/core/data
//  ${CBMROOT_SOURCE_DIR}/core/data/beamtime
  ${CBMROOT_SOURCE_DIR}/core/data/sts
  ${CBMROOT_SOURCE_DIR}/core/data/much
  ${CBMROOT_SOURCE_DIR}/core/data/tof
  ${CBMROOT_SOURCE_DIR}/core/data/trd
  ${CBMROOT_SOURCE_DIR}/core/data/rich
  ${CBMROOT_SOURCE_DIR}/core/data/psd
  ${CBMROOT_SOURCE_DIR}/core/data/global

${CBMROOT_SOURCE_DIR}/reco
${CBMROOT_SOURCE_DIR}/reco/base
${CBMROOT_SOURCE_DIR}/reco/calibration
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
#CbmBase
CbmData
CbmRecoBase
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
