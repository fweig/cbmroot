# Create a library called "libKF" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/Interface
  )

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
  #CbmKFParticleFinder.cxx
  #CbmKFParticleFinderPID.cxx
  #CbmKFParticleFinderQa.cxx
  #CbmKFParticleInterface.cxx
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


set(LIBRARY_NAME KF)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmBase
  CbmData
  CbmRecoBase
  FairRoot::Base
  ROOT::Core
  ROOT::Geom
  ROOT::Hist
  )

set(PRIVATE_DEPENDENCIES
  CbmMuchBase
  CbmSimSteer
  CbmMvdBase
  CbmStsBase
  CbmTofBase
  CbmTrdBase
  FairLogger::FairLogger
  FairRoot::GeoBase
  FairRoot::ParBase
  ROOT::EG
  ROOT::Gpad
  ROOT::MathCore
  ROOT::Matrix
  ROOT::Physics
  ROOT::RIO
  ROOT::Tree
  )

Set(DEFINITIONS -DDO_TPCCATRACKER_EFF_PERFORMANCE -DNonhomogeneousField -DCBM -DUSE_TIMERS)

generate_cbm_library()

install(FILES Interface/CbmKFVertex.h
        DESTINATION include
       )
