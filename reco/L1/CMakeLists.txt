#Create a library called "libL1" which includes the source files given in
#the array.
#The extension is already found.Any number of sources could be listed here.

# extra warnings to examine the code
#ADD_DEFINITIONS(-Wall -Wextra -Wshadow -Weffc++)

# L1 defines
ADD_DEFINITIONS(-DDO_TPCCATRACKER_EFF_PERFORMANCE -DNonhomogeneousField -DCBM -DUSE_TIMERS)

set(INCLUDE_DIRECTORIES
  ${CMAKE_SOURCE_DIR}/algo/ca
  ${CMAKE_SOURCE_DIR}/algo/ca/simd
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/L1Algo
  ${CMAKE_CURRENT_SOURCE_DIR}/OffLineInterface
  ${CMAKE_CURRENT_SOURCE_DIR}/ParticleFinder
  ${CMAKE_CURRENT_SOURCE_DIR}/qa
  ${CMAKE_CURRENT_SOURCE_DIR}/L1Algo/utils
  ${CMAKE_CURRENT_SOURCE_DIR}/catools
  )

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
 # L1AlgoInputData.cxx
  OffLineInterface/CbmL1StsTrackFinder.cxx 
  OffLineInterface/CbmL1GlobalTrackFinder.cxx 
  OffLineInterface/CbmL1GlobalFindTracksEvents.cxx

  L1Algo/L1Algo.cxx
  L1Algo/L1CATrackFinder.cxx
  L1Algo/L1BranchExtender.cxx
  L1Algo/L1TrackFitter.cxx
  L1Algo/L1HitsSortHelper.cxx
  L1Algo/L1Grid.cxx
  CbmL1Performance.cxx 
  CbmL1ReadEvent.cxx
  CbmCaPerformance.cxx
  L1Algo/L1Station.cxx
  L1Algo/L1TrackParFit.cxx
  L1Algo/L1MCEvent.cxx
  CbmL1MCTrack.cxx
  L1Algo/L1Material.cxx
  L1Algo/L1UMeasurementInfo.cxx
  L1Algo/L1XYMeasurementInfo.cxx
  L1Algo/L1Field.cxx
  L1Algo/L1CAIteration.cxx
  L1Algo/L1BaseStationInfo.cxx
  L1Algo/L1InitManager.cxx
  L1Algo/L1Parameters.cxx
  L1Algo/L1InputData.cxx
  L1Algo/L1IODataManager.cxx
  L1Algo/L1CloneMerger.cxx
  L1Algo/L1ConfigRW.cxx
  L1Algo/L1SearchWindow.cxx
  L1Algo/utils/L1AlgoDraw.cxx
  L1Algo/utils/L1AlgoEfficiencyPerformance.cxx
  L1Algo/utils/L1AlgoPulls.cxx
  catools/CaToolsMcData.cxx
  catools/CaToolsMcDataManager.cxx
  catools/CaToolsPerformance.cxx
  catools/CaToolsDebugger.cxx
  catools/CaToolsWindowFinder.cxx
  catools/CaToolsWFExpression.cxx
  ParticleFinder/CbmL1PFFitter.cxx
  ParticleFinder/CbmL1PFMCParticle.cxx

  qa/CbmTrackerInputQaTrd.cxx
  qa/CbmTrackerInputQaTof.cxx
  qa/CbmTrackingInputQaSts.cxx
)

set(NO_DICT_SRCS
  L1Algo/L1Event.cxx
  L1Algo/L1EventMatch.cxx
)

set(HEADERS
  CbmL1CATrdTrackFinderSA.h
  CbmL1MCPoint.h
  CbmL1Hit.h
  CbmL1Track.h
  CbmL1TrackPar.h
  CbmL1TrdHit.h
  CbmL1Vtx.h
  L1Algo/L1Def.h
  L1Algo/L1Vector.h
  catools/CaToolsWindowFinder.h
)




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



set(LIBRARY_NAME L1)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmBase
  CbmData
  CbmQaBase
  KF
  FairLogger::FairLogger
  FairRoot::Base
  ROOT::Core
  ROOT::Gpad
  ROOT::Graf
  ROOT::Hist
  ROOT::Physics
  )

if(OpenMP_CXX_FOUND)
    list(APPEND PUBLIC_DEPENDENCIES OpenMP::OpenMP_CXX)
endif()

set(PRIVATE_DEPENDENCIES
  CbmMuchBase
  CbmMvd
  CbmRecoSts
  CbmSimSteer
  CbmStsBase
  CbmTofBase
  CbmTrdBase
  CbmRecoBase
  KFParticle
  external::yaml-cpp
  FairRoot::GeoBase
  FairRoot::ParBase
  ROOT::EG
  ROOT::Geom
  ROOT::Graf3d
  ROOT::MathCore
  ROOT::Matrix
  ROOT::Minuit
  ROOT::RIO
  )


generate_cbm_library()

add_dependencies(G__L1 KFPARTICLE)

install(FILES CbmL1Counters.h
              L1Algo/L1Assert.h
              L1Algo/L1EventEfficiencies.h
              L1Algo/L1Branch.h
              L1Algo/L1HitPoint.h
              L1Algo/L1Hit.h
              L1Algo/L1Portion.h
              L1Algo/L1Triplet.h
              L1Algo/L1Event.h
              L1Algo/L1EventMatch.h
              L1Algo/L1ObjectInitController.h
              L1Algo/L1Constants.h
              L1Algo/L1Utils.h
              L1Algo/L1NaN.h
              L1Algo/L1SimdSerializer.h
              L1Algo/L1TrackPar.h
              L1Algo/L1Track.h
        DESTINATION include
       )

install(FILES L1Algo/L1Algo.h
              L1Algo/L1Branch.h
              L1Algo/L1Field.h
              L1Algo/L1Hit.h
              L1Algo/L1Vector.h
        DESTINATION include/L1Algo
       )
