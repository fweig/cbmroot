# CMakeList file for library libRecoSts
# Last update: V. Friese, 25.03.2020


# -----   Specify library name   --------------------------
Set(LIBRARY_NAME CbmRecoSts)
# ---------------------------------------------------------


# -----  Specify sources for compilation   ----------------
set(SRCS
CbmRecoSts.cxx
CbmStsAlgoAnaCluster.cxx
CbmStsAlgoFindClusters.cxx
CbmStsAlgoFindHits.cxx
CbmStsAlgoFindHitsOrtho.cxx
CbmStsFindTracks.cxx
CbmStsFindTracksEvents.cxx
CbmStsFindTracksQa.cxx
CbmStsRecoModule.cxx
CbmStsTrackFinderIdeal.cxx
)
# -----  End of sources   ---------------------------------



# ----  Specify include directories -----------------------
set(INCLUDE_DIRECTORIES

# This directory
${CBMROOT_SOURCE_DIR}/reco/detectors/sts

# Reco
${CBMROOT_SOURCE_DIR}/reco/base

# Base
${CBMBASE_DIR}

# Data
${CBMDATA_DIR}
${CBMDATA_DIR}/base
${CBMDATA_DIR}/global
${CBMDATA_DIR}/sts
${CBMDATA_DIR}/mvd

# MVD
${CBMROOT_SOURCE_DIR}/mvd

# STS
${CBMDETECTORBASE_DIR}/sts
)

set(SYSTEM_INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
)
# ----  End of include directories ------------------------


# ----  Specify link directories --------------------------
set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
${Boost_LIBRARY_DIRS}
)
# -----  End of link directories   ------------------------


# -----   Specify library dependences   -------------------
Set(DEPENDENCIES
    CbmBase CbmData CbmRecoBase CbmStsBase
)
# ---------------------------------------------------------


# -----  Specify LinkDef file   ---------------------------
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
# ---------------------------------------------------------


# ---- Enable OpenMP  -------------------------------------
find_package(OpenMP)
if (OPENMP_FOUND)
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()
# ---------------------------------------------------------


# -----  Let cmake do the job   ---------------------------
include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})
link_directories( ${LINK_DIRECTORIES})
GENERATE_LIBRARY()
# ---------------------------------------------------------
