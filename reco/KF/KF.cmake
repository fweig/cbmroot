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

#${CBMROOT_SOURCE_DIR}/reco/L1
#${CBMROOT_SOURCE_DIR}/reco/L1/L1Algo
#${CBMROOT_SOURCE_DIR}/reco/L1/ParticleFinder

${CBMROOT_SOURCE_DIR}/mvd 

${CBMDETECTORBASE_DIR}/much # TMP for tracker interface
${CBMDETECTORBASE_DIR}/trd # TMP for tracker interface
${CBMROOT_SOURCE_DIR}/sim/transport/steer # TMP for tracker interface
${CBMROOT_SOURCE_DIR}/sim/transport/geosetup # TMP for tracker interface
)

Include_Directories( ${INCLUDE_DIRECTORIES})

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${VC_INCLUDE_DIRS}
  ${BASE_INCLUDE_DIRECTORIES} 
)

Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${Vc_LIB_DIR}
${FAIRROOT_LIBRARY_DIR}
${ROOT_LIBRARY_DIR}
${Boost_LIBRARY_DIRS}
)
 
link_directories(BEFORE ${LINK_DIRECTORIES})

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

#### Tracker interfaces (will be moved to core/detector/*) ################
CbmStsTrackingInterface.cxx # TMP: Should be placed to the detector directory!
CbmMvdTrackingInterface.cxx # TMP: Should be placed to the detector directory!
CbmMuchTrackingInterface.cxx # TMP: Should be placed to the detector directory!
CbmTrdTrackingInterface.cxx # TMP: Should be placed to the detector directory!
CbmTrackingDetectorInterfaceInit.cxx
###########################################################################

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
#KFQA/CbmKFTrErrMCPoints.cxx
#KFQA/CbmKFTrackFitQa.cxx
#KFQA/CbmKFTrackQa.cxx
#KFQA/KFParticleMatch.cxx
#CbmKFParticleFinder.cxx
#CbmKFParticleFinderPID.cxx
#CbmKFParticleFinderQa.cxx
#CbmKFParticleInterface.cxx
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

#### Tracker interfaces (will be moved to core/detector/*) ################
CbmStsTrackingInterface.h # TMP: Should be placed to the detector directory!
CbmMvdTrackingInterface.h # TMP: Should be placed to the detector directory!
CbmMuchTrackingInterface.h # TMP: Should be placed to the detector directory!
CbmTrdTrackingInterface.h # TMP: Should be placed to the detector directory!
CbmTrackingDetectorInterfaceBase.h
CbmTrackingDetectorInterfaceInit.h
###########################################################################

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
#KFQA/CbmKFTrErrMCPoints.h
#KFQA/CbmKFTrackFitQa.h
#KFQA/CbmKFTrackQa.h
#KFQA/CbmKFPartEfficiencies.h
#KFQA/KFParticleMatch.h
#CbmKFParticleFinder.h
#CbmKFParticleFinderPID.h
#CbmKFParticleFinderQa.h
#CbmKFParticleInterface.h
)

If(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  ADD_DEFINITIONS(-Wall -Wsign-promo -Wctor-dtor-privacy -Wreorder -Wno-deprecated -Wno-parentheses -DDO_TPCCATRACKER_EFF_PERFORMANCE -DNonhomogeneousField -DCBM -DUSE_TIMERS) # -Weffc++ -Wnon-virtual-dtor -Woverloaded-virtual -Wold-style-cast   : wait for other parts of cbmroot\root.
  #---Check for compiler flags
  CHECK_CXX_COMPILER_FLAG("-Werror -Wno-pmf-conversions" HAS_PMF)
  If(HAS_PMF)
    ADD_DEFINITIONS(-Wno-pmf-conversions)
  EndIf()
  CHECK_CXX_COMPILER_FLAG("-Werror -Wstrict-null-sentinel" HAS_SENTINEL)
  If(HAS_SENTINEL)
    ADD_DEFINITIONS(-Wstrict-null-sentinel)
  EndIf()
  CHECK_CXX_COMPILER_FLAG("-Werror -Wno-non-template-friend" HAS_TEMPLATE_FRIEND)
  If(HAS_TEMPLATE_FRIEND)
    ADD_DEFINITIONS(-Wno-non-template-friend)
  EndIf()
  CHECK_CXX_COMPILER_FLAG("-Werror -Wno-pragmas" HAS_PRAGMA)
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
  CbmStsBase CbmMvd CbmBase CbmRecoBase CbmMuchBase CbmData Base Vc.a Minuit2
)
Set(DEFINITIONS -DDO_TPCCATRACKER_EFF_PERFORMANCE -DNonhomogeneousField -DCBM -DUSE_TIMERS)

GENERATE_LIBRARY()

Install(FILES Interface/CbmKFVertex.h
        DESTINATION include)
