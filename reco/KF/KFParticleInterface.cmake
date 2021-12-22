# Create a library called "libKF" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(INCLUDE_DIRECTORIES
${CBMBASE_DIR} 

${CBMROOT_SOURCE_DIR}/reco/base

${CBMDATA_DIR}
${CBMDATA_DIR}/base
${CBMDATA_DIR}/sts
${CBMDATA_DIR}/mvd
${CBMDATA_DIR}/much
${CBMDATA_DIR}/trd
${CBMDATA_DIR}/rich
${CBMDATA_DIR}/tof
${CBMDATA_DIR}/global

${CBMROOT_SOURCE_DIR}/reco/KF
${CBMROOT_SOURCE_DIR}/reco/KF/Interface
${CBMROOT_SOURCE_DIR}/reco/KF/KFQA

${CBMDETECTORBASE_DIR}/sts

${CBMROOT_SOURCE_DIR}/reco/L1
${CBMROOT_SOURCE_DIR}/reco/L1/L1Algo
#${CBMROOT_SOURCE_DIR}/reco/L1/ParticleFinder

${CBMROOT_SOURCE_DIR}/mvd 
)

Include_Directories( ${INCLUDE_DIRECTORIES})

Set(SYSTEM_INCLUDE_DIRECTORIES
#  ${KFParticle_INCLUDE_DIR}
  ${VC_INCLUDE_DIRS}
  ${BASE_INCLUDE_DIRECTORIES} 
)

Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${Vc_LIB_DIR}
#${KFParticle_LIB_DIR}
${FAIRROOT_LIBRARY_DIR}
${ROOT_LIBRARY_DIR}
${Boost_LIBRARY_DIRS}
)
 
link_directories(BEFORE ${LINK_DIRECTORIES})

set(SRCS
CbmKFParticleFinder.cxx
CbmKFParticleFinderPID.cxx
CbmKFParticleFinderQa.cxx
CbmKFParticleInterface.cxx
)


set(HEADERS
CbmKFParticleFinder.h
CbmKFParticleFinderPID.h
CbmKFParticleFinderQa.h
CbmKFParticleInterface.h
)

If (APPLE)
  ADD_DEFINITIONS(-Wall -Wsign-promo -Wctor-dtor-privacy -Wreorder -Wno-deprecated -Wno-parentheses -DDO_TPCCATRACKER_EFF_PERFORMANCE -DNonhomogeneousField -DCBM -DUSE_TIMERS) # -Weffc++ -Wnon-virtual-dtor -Woverloaded-virtual -Wold-style-cast   : wait for other parts of cbmroot\root.
  #---Check for compiler flags
  CHECK_CXX_COMPILER_FLAG("-Wno-pmf-conversions" HAS_PMF)
  If(HAS_PMF)
    ADD_DEFINITIONS(-Wno-pmf-conversions)
  EndIf()
  CHECK_CXX_COMPILER_FLAG("-Wstrict-null-sentinel" HAS_SENTINEL)
  If(HAS_SENTINEL)
    ADD_DEFINITIONS(-Wstrict-null-sentinel)
  EndIf()
  CHECK_CXX_COMPILER_FLAG("-Wno-non-template-friend" HAS_TEMPLATE_FRIEND)
  If(HAS_TEMPLATE_FRIEND)
    ADD_DEFINITIONS(-Wno-non-template-friend)
  EndIf()
  CHECK_CXX_COMPILER_FLAG("-Wno-pragmas" HAS_PRAGMA)
  If(HAS_PRAGMA)
    ADD_DEFINITIONS(-Wno-pragmas)
  EndIf()
Else()
  ADD_DEFINITIONS(-Wall -Wsign-promo  -Wno-pmf-conversions -Wctor-dtor-privacy -Wreorder -Wno-deprecated -Wstrict-null-sentinel -Wno-non-template-friend -Wno-pragmas -Wno-parentheses -DDO_TPCCATRACKER_EFF_PERFORMANCE -DNonhomogeneousField -DCBM -DUSE_TIMERS) # -Weffc++ -Wnon-virtual-dtor -Woverloaded-virtual -Wold-style-cast   : wait for other parts of cbmroot\root.
EndIf()

IF (SSE_FOUND)
  Message(STATUS "KF will be compiled with SSE support")
  ADD_DEFINITIONS(-DHAVE_SSE)
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS 
  "-msse -O3")
ELSE (SSE_FOUND)
  MESSAGE(STATUS "KF will be compiled without SSE support")
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS 
  "-O3")
ENDIF (SSE_FOUND)

set(LINKDEF KFParticleInterfaceLinkDef.h)
Set(LIBRARY_NAME KFParticleInterface)
Set(DEPENDENCIES
    KF KFQA Vc.a KFParticle L1
)
Set(DEFINITIONS -DDO_TPCCATRACKER_EFF_PERFORMANCE -DNonhomogeneousField -DCBM -DUSE_TIMERS)

GENERATE_LIBRARY()

Add_Dependencies(KFParticleInterface KFPARTICLE)