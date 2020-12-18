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
${CBMROOT_SOURCE_DIR}/reco/L1/ParticleFinder

${CBMROOT_SOURCE_DIR}/mvd 
)

Include_Directories( ${INCLUDE_DIRECTORIES})

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${KFParticle_INCLUDE_DIR}
  ${VC_INCLUDE_DIRS}
  ${BASE_INCLUDE_DIRECTORIES} 
)

Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${Vc_LIB_DIR}
${KFParticle_LIB_DIR}
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
${Boost_LIBRARY_DIRS}
)
 
link_directories( ${LINK_DIRECTORIES})

set(SRCS
CbmKF.cxx 
CbmKFFieldMath.cxx 
CbmKFHit.cxx 
CbmKFMaterial.cxx 
CbmKFMath.cxx 
CbmKFPixelMeasurement.cxx 
CbmKFPrimaryVertexFinder.cxx 
CbmKFSecondaryVertexFinder.cxx 
CbmKFTrackInterface.cxx 
CbmKFUMeasurement.cxx 
CbmKFVertexInterface.cxx 
#Interface/CbmEcalTrackExtrapolationKF.cxx
Interface/CbmKFStsHit.cxx 
Interface/CbmKFTrack.cxx 
Interface/CbmKFTrdHit.cxx 
Interface/CbmKFTofHit.cxx 
Interface/CbmPVFinderKF.cxx 
Interface/CbmStsFitPerformanceTask.cxx 
Interface/CbmStsKFTrackFitter.cxx 
Interface/CbmStsKFSecondaryVertexFinder.cxx 
Interface/CbmTrdTrackFitterKF.cxx 
Interface/CbmTofTrackFitterKF.cxx 
Interface/CbmGlobalTrackFitterKF.cxx
# CbmKFParticle.cxx
# CbmKFParticle_simd.cxx
# CbmKFParticleInterface.cxx
KFQA/CbmKFTrErrMCPoints.cxx
KFQA/CbmKFTrackFitQA.cxx
KFQA/CbmKFTrackQA.cxx
KFQA/KFParticleMatch.cxx
CbmKFParticleFinder.cxx
CbmKFParticleFinderPID.cxx
CbmKFParticleFinderQA.cxx
CbmKFParticleInterface.cxx
)


set(HEADERS
CbmKF.h 
CbmKFFieldMath.h
CbmKFHit.h
CbmKFMaterial.h
CbmKFMath.h
CbmKFPixelMeasurement.h
CbmKFPrimaryVertexFinder.h
CbmKFSecondaryVertexFinder.h
CbmKFTrackInterface.h
CbmKFUMeasurement.h 
CbmKFVertexInterface.h
#Interface/CbmEcalTrackExtrapolationKF.h
Interface/CbmKFStsHit.h
Interface/CbmKFTrack.h 
Interface/CbmKFTrdHit.h 
Interface/CbmKFTofHit.h 
Interface/CbmPVFinderKF.h
Interface/CbmStsFitPerformanceTask.h
Interface/CbmStsKFTrackFitter.h
Interface/CbmStsKFSecondaryVertexFinder.h
Interface/CbmTrdTrackFitterKF.h
Interface/CbmTofTrackFitterKF.h
Interface/CbmGlobalTrackFitterKF.h
# CbmKFParticle.h
KFQA/CbmKFTrErrMCPoints.h
KFQA/CbmKFTrackFitQA.h
KFQA/CbmKFTrackQA.h
KFQA/CbmKFPartEfficiencies.h
KFQA/KFParticleMatch.h
CbmKFParticleFinder.h
CbmKFParticleFinderPID.h
CbmKFParticleFinderQA.h
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

set(LINKDEF KFLinkDef.h)
Set(LIBRARY_NAME KF)
Set(DEPENDENCIES
    CbmStsBase CbmMvd CbmBase CbmRecoBase CbmData Base Vc.a Minuit2 KFParticle
)
Set(DEFINITIONS -DDO_TPCCATRACKER_EFF_PERFORMANCE -DNonhomogeneousField -DCBM -DUSE_TIMERS)

GENERATE_LIBRARY()

Add_Dependencies(KF KFPARTICLE)
Install(FILES KFQA/CbmKFTrErrMCPoints.h
              KFQA/CbmKFPartEfficiencies.h
        DESTINATION include/KF/KFQA)
Install(FILES Interface/CbmKFVertex.h
        DESTINATION include)
