# CMakeList file for library libCbmRecoTasks
# V. Friese,     2 June 2021



# -----   Library name   ----------------------------------
Set(LIBRARY_NAME CbmRecoTasks)
# ---------------------------------------------------------


# -----  Compilation sources   ----------------------------
set(SRCS
CbmReco.cxx
CbmSourceTs.cxx
CbmTaskBuildEvents.cxx
CbmTaskDigiEventQa.cxx
CbmTaskMakeRecoEvents.cxx
CbmTaskTriggerDigi.cxx
CbmTaskUnpack.cxx
)
# ---------------------------------------------------------


# ----  Include directories -------------------------------
set(INCLUDE_DIRECTORIES
${CMAKE_CURRENT_SOURCE_DIR}

${CBMROOT_SOURCE_DIR}/core/base
${CBMROOT_SOURCE_DIR}/core/data
${CBMROOT_SOURCE_DIR}/core/data/base
${CBMROOT_SOURCE_DIR}/core/data/global
${CBMROOT_SOURCE_DIR}/core/data/sts
${CBMROOT_SOURCE_DIR}/core/data/rich
${CBMROOT_SOURCE_DIR}/core/data/much
${CBMROOT_SOURCE_DIR}/core/data/trd
${CBMROOT_SOURCE_DIR}/core/data/tof
${CBMROOT_SOURCE_DIR}/core/data/psd
${CBMROOT_SOURCE_DIR}/core/data/raw

${CBMROOT_SOURCE_DIR}/algo/evbuild
${CBMROOT_SOURCE_DIR}/algo/trigger
${CBMROOT_SOURCE_DIR}/algo/detectors/sts

)

set(SYSTEM_INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${IPC_INCLUDE_DIRECTORY}
)
# ---------------------------------------------------------



# ----  Link directories ----------------------------------
set(LINK_DIRECTORIES
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
CbmData
Algo
external::yaml-cpp
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
