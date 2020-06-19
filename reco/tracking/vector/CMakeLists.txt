# Create a library called "libBase" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed
# here.

Set(INCLUDE_DIRECTORIES
${CBMROOT_SOURCE_DIR}/tracking/vector
${CBMBASE_DIR} 
${CBMROOT_SOURCE_DIR}/run 
${CBMDATA_DIR} 
${CBMDATA_DIR}/much 
${CBMROOT_SOURCE_DIR}/much
${CBMROOT_SOURCE_DIR}/much/geo
${CBMROOT_SOURCE_DIR}/much/reco
${CBMROOT_SOURCE_DIR}/much/tracking
${CBMROOT_SOURCE_DIR}/trd/param
${CBMROOT_SOURCE_DIR}/trd/data

# needed by run/FairMCBuffer.h
${CBMDATA_DIR}/mvd 
${CBMDATA_DIR}/rich 
${CBMDATA_DIR}/sts 
${CBMDATA_DIR}/trd 
${CBMDATA_DIR}/tof 
${CBMDATA_DIR}/psd 

# needed by CbmMuchFindVectors
${CBMROOT_SOURCE_DIR}/KF
${CBMROOT_SOURCE_DIR}/KF/Interface
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
CbmVectorFinder.cxx
CbmTrdFindVectors.cxx
CbmTrdToTofVector.cxx
CbmMuchFindVectors.cxx
CbmMuchFindVectorsGem.cxx
#CbmMuchFindVectorsQA.cxx
CbmMuchMergeVectors.cxx
#CbmMuchMergeVectorsQA.cxx
CbmMuchToTrdVectors.cxx
CbmMuchToTofVectors.cxx
)

#AZ - debug
#SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS
#  "-gdwarf-2 -O0")

set(LINKDEF CbmVectorLinkDef.h)
Set(LIBRARY_NAME CbmVector)
Set(DEPENDENCIES
    CbmBase CbmData Base KF
)

GENERATE_LIBRARY()

