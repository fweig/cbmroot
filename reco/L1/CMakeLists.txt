#Create a library called "libL1" which includes the source files given in
#the array.
#The extension is already found.Any number of sources could be listed here.

# extra warnings to examine the code
#ADD_DEFINITIONS(-Wall -Wextra -Wshadow -Weffc++)

# L1 defines
ADD_DEFINITIONS(-DDO_TPCCATRACKER_EFF_PERFORMANCE -DNonhomogeneousField -DCBM -DUSE_TIMERS)

IF (SSE_FOUND)
  ADD_DEFINITIONS(-DHAVE_SSE)
  Message(STATUS "L1 will be compiled with SSE support")
ELSE (SSE_FOUND)
  Message(FATAL_ERROR "L1 can not be compiled without SSE support")
ENDIF (SSE_FOUND)


Set(INCLUDE_DIRECTORIES
${CBMROOT_SOURCE_DIR}/reco/base
${CBMROOT_SOURCE_DIR}/reco/detectors/sts
${CBMROOT_SOURCE_DIR}/reco/detectors/rich
${CBMROOT_SOURCE_DIR}/reco/detectors/rich/fitter
${CBMBASE_DIR} 

${CBMDATA_DIR}
${CBMDATA_DIR}/raw
${CBMDATA_DIR}/sts
${CBMDATA_DIR}/much
${CBMDATA_DIR}/mvd
${CBMDATA_DIR}/trd
${CBMDATA_DIR}/tof
${CBMDATA_DIR}/rich
${CBMDATA_DIR}/global

${CBMROOT_SOURCE_DIR}/core/qa

${CBMROOT_SOURCE_DIR}/rich/reco/fitter

${CBMROOT_SOURCE_DIR}/reco/L1
${CBMROOT_SOURCE_DIR}/reco/L1/L1Algo
${CBMROOT_SOURCE_DIR}/reco/L1/OffLineInterface
${CBMROOT_SOURCE_DIR}/reco/L1/ParticleFinder
${CBMROOT_SOURCE_DIR}/reco/L1/qa

${CBMROOT_SOURCE_DIR}/reco/KF
${CBMROOT_SOURCE_DIR}/reco/KF/KFQA
${CBMROOT_SOURCE_DIR}/reco/KF/Interface
${CBMROOT_SOURCE_DIR}/run
${CBMROOT_SOURCE_DIR}/sim/transport/steer
${CBMROOT_SOURCE_DIR}/sim/transport/geosetup

${CBMDATA_DIR}
${CBMDATA_DIR}/base

${CBMDETECTORBASE_DIR}/sts

${CBMROOT_SOURCE_DIR}/mvd
  ${CBMDETECTORBASE_DIR}/trd
  ${CBMDETECTORBASE_DIR}/much
  ${CBMDETECTORBASE_DIR}/tof
)

Include_Directories( ${INCLUDE_DIRECTORIES})

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${VC_INCLUDE_DIRS}
  ${BASE_INCLUDE_DIRECTORIES} 
  ${KFParticle_INCLUDE_DIR}
)

if (OPENMP_FOUND)
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set(SYSTEM_INCLUDE_DIRECTORIES
    ${SYSTEM_INCLUDE_DIRECTORIES}
    ${OpenMP_CXX_INCLUDE_DIRS}
  )
endif()

Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${Vc_LIB_DIR}
${KFParticle_LIB_DIR}
${FAIRROOT_LIBRARY_DIR}
${ROOT_LIBRARY_DIR}
${Boost_LIBRARY_DIRS}
)

if (OPENMP_FOUND AND APPLE)
  get_filename_component(OpenMP_CXX_LIBRARY_DIR ${OpenMP_CXX_LIBRARIES} DIRECTORY)
  Set(LINK_DIRECTORIES
    ${LINK_DIRECTORIES}
    ${OpenMP_CXX_LIBRARY_DIR}
  )
endif()
 
link_directories( ${LINK_DIRECTORIES})

# ----- Compilation sources --------------------------------------------
set(SRCS
#L1Algo / L1CATrackFinder.cxx
#CbmL1Performance.cxx
#CbmL1ReadEvent.cxx
#CbmL1CATrdTrackFinderSA.cxx 
CbmL1TrdTracklet.cxx 
CbmL1TrdTracklet4.cxx 
CbmL1.cxx
#CbmL1TrdTrackFinderSts.cxx 
CbmL1TrackMerger.cxx 
CbmL1TofMerger.cxx
OffLineInterface/CbmL1RichENNRingFinder.cxx
OffLineInterface/CbmL1RichENNRingFinderParallel.cxx 
OffLineInterface/CbmL1StsTrackFinder.cxx 
OffLineInterface/CbmL1GlobalTrackFinder.cxx 
OffLineInterface/CbmL1GlobalFindTracksEvents.cxx
#OffLineInterface / CbmL1MuchFinder.cxx
#OffLineInterface / CbmL1MuchHit.cxx
#OffLineInterface / CbmL1MuchTrack.cxx
#OffLineInterface / CbmL1MuchFinderQa.cxx
OffLineInterface/CbmL1RichRingQa.cxx
#OffLineInterface / CbmL1SttHit.cxx
#OffLineInterface / CbmL1SttTrackFinder.cxx
#OffLineInterface / CbmL1SttTrack.cxx 

L1Algo/L1Algo.cxx
L1Algo/L1CATrackFinder.cxx
L1Algo/L1TrackExtender.cxx
L1Algo/L1TrackFitter.cxx
L1Algo/L1HitsSortHelper.cxx
L1Algo/L1Grid.cxx
CbmL1Performance.cxx 
CbmL1ReadEvent.cxx
L1Algo/L1Station.cxx
L1Algo/L1TrackParFit.cxx
L1Algo/L1Event.cxx
L1Algo/L1EventMatch.cxx
L1Algo/L1MCEvent.cxx
L1Algo/L1Fit.cxx
L1Algo/L1FitMaterial.cxx
L1Algo/L1Extrapolation.cxx
CbmL1MCTrack.cxx
L1Algo/L1MaterialInfo.cxx
L1Algo/L1UMeasurementInfo.cxx
L1Algo/L1XYMeasurementInfo.cxx
L1Algo/L1Field.cxx
L1Algo/L1CAIteration.cxx
L1Algo/L1BaseStationInfo.cxx
L1Algo/L1InitManager.cxx
L1Algo/L1Parameters.cxx
L1Algo/L1InputData.cxx
L1Algo/L1IODataManager.cxx
L1Algo/L1ClonesMerger.cxx
L1Algo/L1ConfigRW.cxx
L1Algo/utils/L1AlgoDraw.cxx
L1Algo/utils/L1AlgoEfficiencyPerformance.cxx
L1Algo/utils/L1AlgoPulls.cxx


ParticleFinder/CbmL1PFFitter.cxx
ParticleFinder/CbmL1PFMCParticle.cxx

qa/CbmTrackerInputQaTrd.cxx
qa/CbmTrackingInputQaSts.cxx
)
# ----------------------------------------------------------------------

# ----- Headers --------------------------------------------------------
set(HEADERS
CbmL1CATrdTrackFinderSA.h
CbmL1.h
CbmL1MCTrack.h
CbmL1MCPoint.h
CbmL1Hit.h
CbmL1TofMerger.h
CbmL1Track.h
CbmL1TrackMerger.h
CbmL1TrackPar.h
CbmL1TrdHit.h
#CbmL1TrdTrackFinderSts.h
CbmL1TrdTracklet4.h
CbmL1TrdTracklet.h
CbmL1Vtx.h
#OffLineInterface / CbmL1MuchFinder.h
#OffLineInterface / CbmL1MuchFinderQa.h
#OffLineInterface / CbmL1MuchHit.h
#OffLineInterface / CbmL1MuchTrack.h
OffLineInterface/CbmL1RichENNRingFinder.h
OffLineInterface/CbmL1RichRingQa.h
OffLineInterface/CbmL1StsTrackFinder.h
OffLineInterface/CbmL1GlobalTrackFinder.h
OffLineInterface/CbmL1GlobalFindTracksEvents.h
#OffLineInterface / CbmL1SttHit.h
#OffLineInterface / CbmL1SttTrackFinder.h
#OffLineInterface / CbmL1SttTrack.h
L1Algo/L1Def.h
L1Algo/L1Vector.h
qa/CbmTrackerInputQaTrd.h
qa/CbmTrackingInputQaSts.h
)
# ----------------------------------------------------------------------




If(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  ADD_DEFINITIONS(-Wall -Wsign-promo -Wctor-dtor-privacy -Wreorder -Wno-deprecated -Wno-parentheses) #   -Weffc++ -Wnon-virtual-dtor -Woverloaded-virtual -Wold-style-cast : wait for other parts of cbmroot\root.
#-- - Check for compiler flags
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
Else()
  ADD_DEFINITIONS(-Wall -Wsign-promo  -Wno-pmf-conversions -Wctor-dtor-privacy -Wreorder -Wno-deprecated -Wstrict-null-sentinel -Wno-non-template-friend -Wno-parentheses) #   -Weffc++ -Wnon-virtual-dtor -Woverloaded-virtual -Wold-style-cast : wait for other parts of cbmroot\root. 
EndIf()


IF (SSE_FOUND)
  ADD_DEFINITIONS(-DHAVE_SSE)
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS 
  "-msse -O3")
  Message(STATUS "L1 will be compiled with SSE support")
ELSE (SSE_FOUND)
  Message(STATUS "L1 will be compiled without SSE support")
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS 
  "-O3")
ENDIF (SSE_FOUND)

Set(LINKDEF L1LinkDef.h)

Set(LIBRARY_NAME L1)

Set(DEPENDENCIES
    KF
    KFParticle
    Base
    CbmBase
    CbmData
    CbmSimSteer
#    CbmGeoSetup
    CbmMuchBase
    CbmTrdBase
    CbmTofBase
    CbmStsBase
    CbmRecoBase
    CbmRecoSts
    CbmQaBase
    boost_regex
    external::yaml-cpp
   )

if (OPENMP_FOUND AND APPLE)
  Set(DEPENDENCIES ${DEPENDENCIES} ${OpenMP_CXX_LIBRARIES}
  )
endif()

GENERATE_LIBRARY()
Install(FILES CbmL1Counters.h
              L1Algo/L1Assert.h
              L1Algo/L1EventEfficiencies.h
              L1Algo/L1Grid.h
              L1Algo/L1Branch.h
              L1Algo/L1Field.h
              L1Algo/L1Grid.h
              L1Algo/L1Hit.h
              L1Algo/L1HitPoint.h
              L1Algo/L1HitsSortHelper.h
              L1Algo/L1Portion.h
              L1Algo/L1Station.h
              L1Algo/L1Track.h
              L1Algo/L1TrackPar.h
              L1Algo/L1TrackParFit.h
              L1Algo/L1Triplet.h
              L1Algo/L1Vector.h
              L1Algo/L1ObjectInitController.h
              L1Algo/L1MaterialInfo.h
              L1Algo/L1UMeasurementInfo.h
              L1Algo/L1XYMeasurementInfo.h
              L1Algo/L1BaseStationInfo.h
              L1Algo/L1InitManager.h
              L1Algo/L1CAIteration.h
              L1Algo/L1Parameters.h
              L1Algo/L1InputData.h
              L1Algo/L1IODataManager.h
              L1Algo/L1ClonesMerger.h
              L1Algo/L1ConfigRW.h
              L1Algo/L1Constants.h
              L1Algo/L1Utils.h
              L1Algo/L1NaN.h
              vectors/vec_arithmetic.h
              vectors/std_alloc.h
        DESTINATION include
       )

Install(FILES L1Algo/L1Algo.h
              L1Algo/L1Branch.h
              L1Algo/L1Field.h
              L1Algo/L1Hit.h
              L1Algo/L1Vector.h
        DESTINATION include/L1Algo
       )

Install(FILES vectors/P4_F32vec4.h
        DESTINATION include/vectors
       )
