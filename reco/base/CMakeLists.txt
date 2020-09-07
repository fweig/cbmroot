# CMakeList file for library CbmRecoBase
# V. Friese,      1 August   2019
# P.-A. Loizeau, 13 February 2020


# -----   Library name   ----------------------------------
Set(LIBRARY_NAME CbmRecoBase)
# ---------------------------------------------------------

# -----  Compilation sources   ----------------------------
set(SRCS
CbmUnpackTaskBase.cxx

CbmTrdTrackFinder.cxx  
CbmTrdTrackFitter.cxx  

CbmTofTrackFinder.cxx
CbmTofTrackFitter.cxx

CbmStsTrackFinder.cxx 
CbmStsTrackFitter.cxx 

CbmMuchTrackFinder.cxx

CbmRichRingFinder.cxx 

CbmGlobalTrackFitter.cxx
CbmPrimaryVertexFinder.cxx
CbmTofMerger.cxx
CbmTrackMerger.cxx
CbmRichMerger.cxx
)
# ---------------------------------------------------------



# ----  Include directories -------------------------------
set(INCLUDE_DIRECTORIES
#${CBMDATA_DIR}
#${CBMROOT_SOURCE_DIR}/reco
${CBMROOT_SOURCE_DIR}/reco/base
${CBMROOT_SOURCE_DIR}/core/base
${CBMROOT_SOURCE_DIR}/core/data
${CBMROOT_SOURCE_DIR}/core/data/base
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
#CbmBase
#CbmData
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
