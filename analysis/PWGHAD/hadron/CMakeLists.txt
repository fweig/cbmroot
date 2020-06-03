# Create a library called "libBase" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(INCLUDE_DIRECTORIES
${CMAKE_CURRENT_SOURCE_DIR}

${CBMROOT_SOURCE_DIR}/reco/base

${CBMBASE_DIR} 

${CBMDATA_DIR} 
${CBMDATA_DIR}/sts
${CBMDATA_DIR}/trd
${CBMDATA_DIR}/tof
${CBMDATA_DIR}/global
${CBMDATA_DIR}/base

${CBMROOT_SOURCE_DIR}/reco/KF
${CBMROOT_SOURCE_DIR}/reco/KF/Interface


  ${CBMDETECTORBASE_DIR}/tof
)

Include_Directories( ${INCLUDE_DIRECTORIES})

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES} 
)

Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
${Boost_LIBRARY_DIRS}
${Vc_LIB_DIR}
${KFParticle_LIB_DIR}
)
 
link_directories( ${LINK_DIRECTORIES})

set(SRCS
CbmHadron.cxx
CbmHadronAnalysis.cxx
)

set(LINKDEF CbmHadronLinkDef.h)
Set(LIBRARY_NAME CbmHadron)
Set(DEPENDENCIES
    KF CbmTofBase CbmRecoBase CbmBase CbmData Base 
)

GENERATE_LIBRARY()
