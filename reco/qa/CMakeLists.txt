

# ----  Specify include directories -----------------------
set(INCLUDE_DIRECTORIES

# This directory
${CBMROOT_SOURCE_DIR}/reco/qa

# external
${CBMROOT_SOURCE_DIR}/external/xpu/src

# Reco
${CBMROOT_SOURCE_DIR}/reco/base

#QA
${CBMROOT_SOURCE_DIR}/core/qa

# Base
${CBMBASE_DIR}

# Data
${CBMDATA_DIR}
${CBMDATA_DIR}/base
${CBMDATA_DIR}/global
${CBMDATA_DIR}/sts
${CBMDATA_DIR}/raw
${CBMDATA_DIR}/mvd
${CBMDATA_DIR}/much
${CBMDATA_DIR}/trd
${CBMDATA_DIR}/tof

# MVD
${CBMROOT_SOURCE_DIR}/mvd

# STS
${CBMDETECTORBASE_DIR}/sts
${CBMDETECTORBASE_DIR}/trd
${CBMROOT_SOURCE_DIR}/reco/detectors/sts
${CBMROOT_SOURCE_DIR}/reco/detectors/sts/unpack
${CBMROOT_SOURCE_DIR}/reco/detectors/sts/qa
${CBMROOT_SOURCE_DIR}/reco/detectors/sts/experimental

)

set(SYSTEM_INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${FLES_IPC_INCLUDE_DIRECTORY} # for fles infos for unpacker
)
# ----  End of include directories ------------------------


Set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES} 
  ${ROOT_INCLUDE_DIR}
)

Include_Directories(${INCLUDE_DIRECTORIES})
Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
)

 

link_directories(${LINK_DIRECTORIES})

set(SRCS
  CbmRecoQa.cxx
  CbmTrackingTrdQa.cxx
)

set(LINKDEF  RecoQaLinkDef.h)

Set(LIBRARY_NAME CbmRecoQa)

Set(DEPENDENCIES
    CbmBase CbmData Base Core Hist    

    Base
    CbmBase
    CbmData
    CbmSimSteer
#    CbmGeoSetup
    CbmTrdBase
    CbmMvd
    CbmStsBase
    CbmRecoBase
    CbmRecoSts
    CbmQaBase
    Core 
    Hist
)

GENERATE_LIBRARY()
