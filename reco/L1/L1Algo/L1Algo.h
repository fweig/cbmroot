/* Copyright (C) 2007-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Maksym Zyzak, Valentina Akishina, Igor Kulakov [committer], Sergei Zharko */

#ifndef L1Algo_h
#define L1Algo_h

// #define TBB // TODO: Doesn't work now. Renew

/// Debug features
// #define PULLS            // triplets pulls
// #define TRIP_PERFORMANCE // triplets efficiencies
// #define DOUB_PERFORMANCE // doublets efficiencies
// #define DRAW             // event display
#ifdef DRAW
class L1AlgoDraw;
#include "CbmL1Track.h"
#endif
//#define XXX               // time debug
//#define COUNTERS          // diff counters (hits, doublets, ... )

// =====>dispatched<===== // #define MERGE_CLONES
// #define TRACKS_FROM_TRIPLETS_ITERATION kAllPrimIter

// =====>dispatched<===== // #define HitErrors
//#define GLOBAL
//#define mCBM

// =====>dispatched<===== // #define LAST_ITERATION kAllSecIter
#define FIND_GAPED_TRACKS  // use triplets with gaps
// =====>dispatched<===== // #define USE_RL_TABLE
#ifndef TRACKS_FROM_TRIPLETS
#define EXTEND_TRACKS
#endif
//#define USE_EVENT_NUMBER
//#endif


#include <array>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>

#include "L1Branch.h"
#include "L1ClonesMerger.h"
#include "L1Field.h"
#include "L1Grid.h"
#include "L1Hit.h"
#include "L1HitPoint.h"
#include "L1HitsSortHelper.h"
#include "L1InitManager.h"
#include "L1InputData.h"
#include "L1Parameters.h"
#include "L1Portion.h"
#include "L1Station.h"
#include "L1Track.h"
#include "L1TrackPar.h"
#include "L1TrackParFit.h"
#include "L1Triplet.h"
#include "L1Utils.h"
#include "L1Vector.h"

#ifdef _OPENMP
#include "omp.h"
#endif

// *******************************
// ** Types definition (global) **
// *******************************

using L1StationsArray_t = std::array<L1Station, L1Constants::size::kMaxNstations>;
using L1MaterialArray_t = std::array<L1Material, L1Constants::size::kMaxNstations>;
using Tindex            = int;  // TODO: Replace with L1HitIndex_t, if suitable

#ifdef PULLS
#define TRIP_PERFORMANCE
class L1AlgoPulls;
#endif
#ifdef TRIP_PERFORMANCE
template<Tindex NHits>
class L1AlgoEfficiencyPerformance;
#endif
#ifdef DOUB_PERFORMANCE
template<Tindex NHits>
class L1AlgoEfficiencyPerformance;
#endif


/// Main class of L1 CA track finder algorithm
///
class L1Algo {
public:
  // *************************
  // ** Friend classes list **
  // *************************

  friend class CbmL1;  /// TODO: Remove this dependency
  friend class ParalleledDup;
  friend class ParalleledTrip;
#ifdef DRAW
  friend class L1AlgoDraw;
#endif

  // **********************************
  // ** Member functions declaration **
  // **********************************

  // ** Constructors and destructor

  /// Constructor
  /// \param nThreads  Number of threads for multi-threaded mode
  L1Algo(unsigned int nThreads = 1);

  /// Copy constructor
  L1Algo(const L1Algo&) = delete;

  /// Move constructor
  L1Algo(L1Algo&&) = delete;

  /// Copy assignment operator
  L1Algo& operator=(const L1Algo&) = delete;

  /// Move assignment operator
  L1Algo& operator=(L1Algo&&) = delete;

  /// Destructor
  ~L1Algo() = default;


  // ** Functions, which pack and unpack indexes of station, thread and triplet **

  // TODO: move to L1Triplet class (S.Zharko)
  /// Packs station, thread and triplet indices to an unique triplet ID
  /// \param  iStation  Index of station in the active stations array
  /// \param  iThread   Index of thread processing triplet
  /// \param  iTriplet  Index of triplet
  static unsigned int PackTripletId(unsigned int iStation, unsigned int iThread, unsigned int iTriplet);

  /// Unpacks the triplet ID to its station index
  /// \param  id  Unique triplet ID
  static unsigned int TripletId2Station(unsigned int id);

  /// Unpacks the triplet ID to its thread index
  /// \param  id  Unique triplet ID
  static unsigned int TripletId2Thread(unsigned int id);

  /// Unpacks the triplet ID to its triplet index
  /// \param  id  Unique triplet ID
  static unsigned int TripletId2Triplet(unsigned int id);

  /// pack thread and track indices to an unique track ID
  static int PackTrackId(int Thread, int Track)
  {
    return PackTripletId(0, (unsigned int) Thread, (unsigned int) Track);
  }

  /// unpack the track ID to its thread index
  static int TrackId2Thread(int ID) { return TripletId2Thread((unsigned int) ID); }

  /// unpack the track ID to its track index
  static int TrackId2Track(int ID) { return TripletId2Triplet((unsigned int) ID); }

  /// Sets L1Algo parameters object
  /// \param other - reference to the L1Parameters object
  void SetParameters(const L1Parameters& other) { fParameters = other; }
  // TODO: remove it (S.Zharko)

  /// Gets a pointer to the L1Algo parameters object
  const L1Parameters* GetParameters() const { return &fParameters; }

  /// Gets a pointer to the L1Algo initialization object
  L1InitManager* GetInitManager() { return &fInitManager; }

  /// Receives input data
  void ReceiveInputData(L1InputData&& inputData);

  /// Gets pointer to input data object for external access
  const L1InputData* GetInputData() const { return &fInputData; }

  /// ----- Hit-point-strips conversion routines ------

  void GetHitCoor(const L1Hit& _h, fscal& _x, fscal& _y, fscal& _z, const L1Station& sta);


  void GetHitCoor(const L1Hit& _h, fscal& _x, fscal& _y, char iS);

  L1HitPoint CreateHitPoint(const L1Hit& hit);  // full the hit point by hit information.

  void CreateHitPoint(const L1Hit& hit, L1HitPoint& point);
  inline int PackIndex(const int& a, const int& b, const int& c);

  inline int UnPackIndex(const int& i, int& a, int& b, int& c);
  /// -- Flags routines --
  [[gnu::always_inline]] static unsigned char GetFStation(unsigned char flag) { return flag / 4; }
  [[gnu::always_inline]] static bool GetFUsed(unsigned char flag) { return (flag & 0x02) != 0; }
  //   bool GetFUsedD  ( unsigned char flag ){ return (flag&0x01)!=0; }


  /// \brief Sets a default particle mass for the track fit
  /// It is used during reconstruction
  /// for the multiple scattering and energy loss estimation
  /// \param mass Default particle mass
  void SetDefaultParticleMass(float mass) { fDefaultMass = mass; }

  /// Gets default particle mass
  /// \return particle mass
  float GetDefaultParticleMass() const { return fDefaultMass; }

  /// Gets default particle mass squared
  /// \return particle mass squared
  float GetDefaultParticleMass2() const { return fDefaultMass * fDefaultMass; }


  /*********************************************************************************************/ /**
   *                             ------  FUNCTIONAL PART ------
   ************************************************************************************************/

  /// ----- Subroutines used by L1Algo::CATrackFinder() ------

  void CAFindTrack(int ista, L1Branch& best_tr, unsigned char& best_L, fscal& best_chi2, const L1Triplet* curr_trip,
                   L1Branch& curr_tr, unsigned char& curr_L, fscal& curr_chi2, unsigned char min_best_l,
                   L1Branch* new_tr);


  /// Fits track
  /// \param t - track with hits
  /// \param T - track parameters
  /// \param dir - false - forward, true - backward
  /// \param qp0 - momentum for extrapolation
  /// \param initParams - should be params ititialized. 1 - yes.
  void BranchFitterFast(const L1Branch& t, L1TrackPar& T, const bool dir, const fvec qp0 = 0.,
                        const bool initParams = true);

  /// Fits track. more precise than FitterFast
  /// \param t - track with hits
  /// \param T - track parameters
  /// \param dir - false - forward, true - backward
  /// \param qp0 - momentum for extrapolation
  /// \param initParams - should be params ititialized. 1 - yes.
  void BranchFitter(const L1Branch& t, L1TrackPar& T, const bool dir, const fvec qp0 = 0.,
                    const bool initParams = true);

  /// Finds additional hits for already found track
  /// \param t - track with hits
  /// \param T - track params
  /// \param dir - 0 - forward, 1 - backward
  /// \param qp0 - momentum for extrapolation
  void FindMoreHits(L1Branch& t, L1TrackPar& T, const bool dir, const fvec qp0 = 0.0);

  /// Find additional hits for existing track
  /// \return chi2
  fscal BranchExtender(L1Branch& t);

  [[gnu::always_inline]] void PackLocation(unsigned int& location, unsigned int& triplet, unsigned int iStation,
                                           unsigned int& thread)
  {
    location = (triplet << 11) | (thread << 3) | iStation;
  }

  [[gnu::always_inline]] void UnPackStation(unsigned int& location, unsigned int& iStation)
  {
    iStation = location & 0x7;
  }

  [[gnu::always_inline]] void UnPackThread(unsigned int& location, unsigned int& thread)
  {
    thread = (location >> 3) & 0xFF;
  }

  [[gnu::always_inline]] void UnPackTriplet(unsigned int& location, unsigned int& triplet)
  {
    triplet = (location >> 11);
  }

  [[gnu::always_inline]] void SetFStation(unsigned char& flag, unsigned int iStation)
  {
    flag = iStation * 4 + (flag % 4);
  }

  [[gnu::always_inline]] void SetFUsed(unsigned char& flag) { flag |= 0x02; }
  //   void SetFUsedD   ( unsigned char &flag ){ flag |= 0x01; }

  [[gnu::always_inline]] void SetFUnUsed(unsigned char& flag) { flag &= 0xFC; }
  //   void SetFUnUsedD ( unsigned char &flag ){ flag &= 0xFE; }

  /// Prepare the portion of left hits data
  void findSingletsStep0(  // input
    Tindex start_lh, Tindex n1_l, L1HitPoint* Hits_l,
    // output
    fvec* u_front_l, fvec* u_back_l, fvec* zPos_l, L1HitIndex_t* hitsl, fvec* HitTime_l, fvec* HitTimeEr, fvec* Event_l,
    fvec* d_u, fvec* d_v);

  /// Get the field approximation. Add the target to parameters estimation. Propagate to middle station.
  void findSingletsStep1(  // input
    int istal, int istam, Tindex n1_V,

    fvec* u_front_l, fvec* u_back_l, fvec* zPos_l, fvec* HitTime_l, fvec* HitTimeEr,
    // output
    L1TrackPar* T_1, L1FieldRegion* fld_1, fvec* d_u, fvec* d_v);

  /// Find the doublets. Reformat data in the portion of doublets.
  void findDoubletsStep0(  // input
    Tindex n1, const L1Station& stal, const L1Station& stam, L1HitPoint* vHits_m, L1TrackPar* T_1,
    L1HitIndex_t* hitsl_1,
    // output
    Tindex& n2, L1Vector<L1HitIndex_t>& i1_2,
#ifdef DOUB_PERFORMANCE
    L1Vector<L1HitIndex_t>& hitsl_2,
#endif  // DOUB_PERFORMANCE
    L1Vector<L1HitIndex_t>& hitsm_2, fvec* Event, L1Vector<char>& lmDuplets);

  /// Add the middle hits to parameters estimation. Propagate to right station.
  /// Find the triplets (right hit). Reformat data in the portion of triplets.
  void findTripletsStep0(  // input
    L1HitPoint* vHits_r, const L1Station& stam, const L1Station& star,

    int istam, int istar, L1HitPoint* vHits_m, L1TrackPar* T_1, L1FieldRegion* fld_1, L1HitIndex_t* hitsl_1,

    Tindex n2, L1Vector<L1HitIndex_t>& hitsm_2, L1Vector<L1HitIndex_t>& i1_2,

    const L1Vector<char>& mrDuplets,
    // output
    Tindex& n3, nsL1::vector<L1TrackPar>::TSimd& T_3, L1Vector<L1HitIndex_t>& hitsl_3, L1Vector<L1HitIndex_t>& hitsm_3,
    L1Vector<L1HitIndex_t>& hitsr_3, nsL1::vector<fvec>::TSimd& u_front_3, nsL1::vector<fvec>::TSimd& u_back_3,
    nsL1::vector<fvec>::TSimd& z_Pos_3,
    //    nsL1::vector<fvec>::TSimd& dx_,
    //    nsL1::vector<fvec>::TSimd& dy_,
    nsL1::vector<fvec>::TSimd& du_, nsL1::vector<fvec>::TSimd& dv_, nsL1::vector<fvec>::TSimd& timeR,
    nsL1::vector<fvec>::TSimd& timeER);

  /// Add the right hits to parameters estimation.
  void findTripletsStep1(  // input
    Tindex n3_V, const L1Station& star, nsL1::vector<fvec>::TSimd& u_front_3, nsL1::vector<fvec>::TSimd& u_back_3,
    nsL1::vector<fvec>::TSimd& z_Pos_3,
    //    nsL1::vector<fvec>::TSimd& dx_,
    //    nsL1::vector<fvec>::TSimd& dy_,
    nsL1::vector<fvec>::TSimd& du_, nsL1::vector<fvec>::TSimd& dv_, nsL1::vector<fvec>::TSimd& timeR,
    nsL1::vector<fvec>::TSimd& timeER,
    // output
    nsL1::vector<L1TrackPar>::TSimd& T_3);

  /// Refit Triplets.
  void findTripletsStep2(  // input
    Tindex n3, int istal, nsL1::vector<L1TrackPar>::TSimd& T_3, L1Vector<L1HitIndex_t>& hitsl_3,
    L1Vector<L1HitIndex_t>& hitsm_3, L1Vector<L1HitIndex_t>& hitsr_3, int nIterations = 0);

  /// Select triplets. Save them into vTriplets.
  void findTripletsStep3(  // input
    Tindex n3, int istal, int istam, int istar, nsL1::vector<L1TrackPar>::TSimd& T_3, L1Vector<L1HitIndex_t>& hitsl_3,
    L1Vector<L1HitIndex_t>& hitsm_3, L1Vector<L1HitIndex_t>& hitsr_3,
    // output
    Tindex& nstaltriplets);


  /// Find neighbours of triplets. Calculate level of triplets.
  void f5(  // input
    // output
    int* nlevel);


  /// Find doublets on station
  void DupletsStaPort(  // input
    int istal, int istam, Tindex ip, L1Vector<Tindex>& n_g, Tindex* portionStopIndex_,

    // output
    L1TrackPar* T_1, L1FieldRegion* fld_1, L1HitIndex_t* hitsl_1,

    L1Vector<char>& lmDuplets,


    Tindex& n_2, L1Vector<L1HitIndex_t>& i1_2, L1Vector<L1HitIndex_t>& hitsm_2);

  /// Find triplets on station
  void TripletsStaPort(  // input
    int istal, int istam, int istar, Tindex& nstaltriplets, L1TrackPar* T_1, L1FieldRegion* fld_1,
    L1HitIndex_t* hitsl_1,
    Tindex& n_2, L1Vector<L1HitIndex_t>& i1_2, L1Vector<L1HitIndex_t>& hitsm_2,
    const L1Vector<char>& mrDuplets
    // output
  );


  ///  ------ Subroutines used by L1Algo::KFTrackFitter()  ------

  void GuessVec(L1TrackPar& t, fvec* xV, fvec* yV, fvec* zV, fvec* Sy, fvec* wV, int NHits, fvec* zCur = 0);
  void GuessVec(L1TrackParFit& t, fvec* xV, fvec* yV, fvec* zV, fvec* Sy, fvec* wV, int NHits, fvec* zCur = 0,
                fvec* timeV = 0, fvec* w_time = 0);

  void FilterFirst(L1TrackPar& track, fvec& x, fvec& y, L1Station& st);
  void FilterFirst(L1TrackParFit& track, fvec& x, fvec& y, fvec& t, L1Station& st);
  void FilterFirst(L1TrackParFit& track, fvec& x, fvec& y, fvec& t, fvec& t_er, L1Station& st);

  void FilterFirst(L1TrackParFit& track, fvec& x, fvec& y, fvec& t, fvec& t_er, L1Station& st, fvec& dx, fvec& dy,
                   fvec& dxy);
  void FilterFirstL(L1TrackParFit& track, fvec& x, fvec& y, fvec& t, fvec& t_er, L1Station& st, fvec& dx, fvec& dy,
                    fvec& dxy);


#ifdef DRAW
  L1AlgoDraw* draw {nullptr};
  void DrawRecoTracksTime(const L1Vector<CbmL1Track>& tracks);
#endif

  enum TrackingMode
  {
    kSts,
    kGlobal,
    kMcbm
  };

  void Init(const bool UseHitErrors, const TrackingMode mode, const bool MissingHits);

  void PrintHits();

  /// The main procedure - find tracks.
  void CATrackFinder();

  /// Track fitting procedures
  void KFTrackFitter_simple();  // version, which use procedured used during the reconstruction
  void L1KFTrackFitter();       // version from SIMD-KF benchmark

  void L1KFTrackFitterMuch();

  float GetMaxInvMom() const { return fMaxInvMom[0]; }

  void SetNThreads(unsigned int n);

public:
  /// Gets total number of stations used in tracking
  int GetNstations() const { return fNstations; }

  /// Gets number of stations before the pipe (MVD stations in CBM)
  int GetNstationsBeforePipe() const { return fNstationsBeforePipe; }

  /// Gets number of stations situated in field region (MVD + STS in CBM)
  int GetNfieldStations() const { return fNfieldStations; }

  /// Get mc track ID for a hit (debug tool)
  int GetMcTrackIdForHit(int iHit);

  /// Get mc track ID for a hit (debug tool)
  int GetMcTrackIdForUnusedHit(int iHit);

private:
  int fNstations {0};            ///< number of all detector stations
  int fNstationsBeforePipe {0};  ///< number of stations before pipe (MVD stations in CBM)
  int fNfieldStations {0};       ///< number of stations in the field region
  //alignas(16) L1StationsArray_t fStations {};  ///< array of L1Station objects
  //alignas(16) L1MaterialArray_t fRadThick {};  ///< material for each station
  float fDefaultMass {L1Constants::phys::kMuonMass};  ///< mass of the propagated particle [GeV/c2]


  // ***************************
  // ** Member variables list **
  // ***************************

  L1InputData fInputData;  ///< Tracking input data

  L1Vector<unsigned char> fvHitKeyFlags {
    "L1Algo::fvHitKeyFlags"};  ///< List of key flags: has been this hit or cluster already used

public:
  L1Grid vGrid[L1Constants::size::kMaxNstations];      ///<
  L1Grid vGridTime[L1Constants::size::kMaxNstations];  ///<


  double fCATime {0.};  // time of track finding

  L1Vector<L1Track> fTracks {"L1Algo::fTracks"};           ///< reconstructed tracks
  L1Vector<L1HitIndex_t> fRecoHits {"L1Algo::fRecoHits"};  ///< packed hits of reconstructed tracks

  /// Created triplets vs station and thread index
  L1Vector<L1Triplet> fTriplets[L1Constants::size::kMaxNstations][L1Constants::size::kMaxNthreads] {
    {"L1Algo::fTriplets"}};

  /// Track candidates created out of adjacent triplets before the final track selection.
  /// The candidates may share any amount of hits.
  L1Vector<L1Branch> fTrackCandidates[L1Constants::size::kMaxNthreads] {"L1Algo::fTrackCandidates"};

  Tindex fDupletPortionStopIndex[L1Constants::size::kMaxNstations] {0};  ///< end of the duplet portions for the station
  L1Vector<Tindex> fDupletPortionSize {"L1Algo::fDupletPortionSize"};    ///< Number of duplets in a portion


  //  L1Branch* pointer;
  unsigned int NHitsIsecAll {0};
  
  L1Vector<L1Hit> vNotUsedHits_A {"L1Algo::vNotUsedHits_A"};
  L1Vector<L1Hit> vNotUsedHits_B {"L1Algo::vNotUsedHits_B"};
  L1Vector<L1Hit> vNotUsedHits_Buf {"L1Algo::vNotUsedHits_Buf"};
  L1Vector<L1HitPoint> vNotUsedHitsxy_A {"L1Algo::vNotUsedHitsxy_A"};
  L1Vector<L1HitPoint> vNotUsedHitsxy_buf {"L1Algo::vNotUsedHitsxy_buf"};
  L1Vector<L1HitPoint> vNotUsedHitsxy_B {"L1Algo::vNotUsedHitsxy_B"};
  L1Vector<L1Track> fTracks_local[L1Constants::size::kMaxNthreads] {"L1Algo::fTracks_local"};
  L1Vector<L1HitIndex_t> fRecoHits_local[L1Constants::size::kMaxNthreads] {"L1Algo::fRecoHits_local"};

  L1Vector<L1HitIndex_t> RealIHit_v {"L1Algo::RealIHit_v"};
  L1Vector<L1HitIndex_t> RealIHit_v_buf {"L1Algo::RealIHit_v_buf"};
  L1Vector<L1HitIndex_t> RealIHit_v_buf2 {"L1Algo::RealIHit_v_buf2"};

#ifdef _OPENMP
  L1Vector<omp_lock_t> fStripToTrackLock {"L1Algo::fStripToTrackLock"};
#endif

  L1Vector<int> fStripToTrack {"L1Algo::fStripToTrack"};    // front strip to track pointers
  L1Vector<int> fStripToTrackB {"L1Algo::fStripToTrackB"};  // back strip to track pointers

  int fNThreads {0};
  bool fUseHitErrors {true};
  bool fMissingHits {0};  ///< TODO ???
  TrackingMode fTrackingMode {kSts};

  fvec EventTime[L1Constants::size::kMaxNthreads][L1Constants::size::kMaxNthreads] {{0}};
  fvec Err[L1Constants::size::kMaxNthreads][L1Constants::size::kMaxNthreads] {{0}};

  /// --- data used during finding iterations
  int isec {0};                                       // iteration TODO: to be dispatched (S.Zharko, 21.06.2022)
  const L1CAIteration* fpCurrentIteration = nullptr;  ///< pointer to the current CA track finder iteration

  L1Vector<L1Hit>* vHitsUnused           = nullptr;
  L1Vector<L1HitIndex_t>* RealIHitP      = nullptr;
  L1Vector<L1HitIndex_t>* RealIHitPBuf   = nullptr;
  L1Vector<L1HitPoint>* vHitPointsUnused = nullptr;
  L1HitIndex_t* RealIHit                 = nullptr;  // index in vHits indexed by index in vHitsUnused

  L1HitIndex_t HitsUnusedStartIndex[L1Constants::size::kMaxNstations + 1] {0};
  L1HitIndex_t HitsUnusedStopIndex[L1Constants::size::kMaxNstations + 1] {0};
  L1HitIndex_t HitsUnusedStartIndexEnd[L1Constants::size::kMaxNstations + 1] {0};
  L1HitIndex_t HitsUnusedStopIndexEnd[L1Constants::size::kMaxNstations + 1] {0};

  L1Vector<int> fHitFirstTriplet {"L1Algo::fHitFirstTriplet"};  /// link hit -> first triplet { hit, *, *}
  L1Vector<int> fHitNtriplets {"L1Algo::fHitNtriplets"};        /// link hit ->n triplets { hit, *, *}


  //  fvec u_front[Portion/fvecLen], u_back[Portion/fvecLen];
  //  fvec zPos[Portion/fvecLen];
  //  fvec fHitTime[Portion/fvecLen];

  nsL1::vector<L1TrackPar>::TSimd fT_3[L1Constants::size::kMaxNthreads];

  L1Vector<L1HitIndex_t> fhitsl_3[L1Constants::size::kMaxNthreads] {"L1Algo::fhitsl_3"};
  L1Vector<L1HitIndex_t> fhitsm_3[L1Constants::size::kMaxNthreads] {"L1Algo::fhitsm_3"};
  L1Vector<L1HitIndex_t> fhitsr_3[L1Constants::size::kMaxNthreads] {"L1Algo::fhitsr_3"};

  nsL1::vector<fvec>::TSimd fu_front3[L1Constants::size::kMaxNthreads];
  nsL1::vector<fvec>::TSimd fu_back3[L1Constants::size::kMaxNthreads];
  nsL1::vector<fvec>::TSimd fz_pos3[L1Constants::size::kMaxNthreads];
  nsL1::vector<fvec>::TSimd fTimeR[L1Constants::size::kMaxNthreads];
  nsL1::vector<fvec>::TSimd fTimeER[L1Constants::size::kMaxNthreads];
  nsL1::vector<fvec>::TSimd dx[L1Constants::size::kMaxNthreads];
  nsL1::vector<fvec>::TSimd dy[L1Constants::size::kMaxNthreads];
  nsL1::vector<fvec>::TSimd du[L1Constants::size::kMaxNthreads];
  nsL1::vector<fvec>::TSimd dv[L1Constants::size::kMaxNthreads];


  //   Tindex NHits_l[L1Constants::size::kMaxNstations];
  //   Tindex NHits_l_P[L1Constants::size::kMaxNstations];
  /// ----- Output data -----


private:
  L1Parameters fParameters {};           ///< Object of L1Algo parameters class
  L1InitManager fInitManager {};         ///< Object of L1Algo initialization manager class
  L1ClonesMerger fClonesMerger {*this};  ///< Object of L1Algo clones merger algorithm

#ifdef TBB
  enum
  {
    nthreads = 3,  // number of threads
    nblocks  = 1   // number of stations on one thread
  };

#endif  // TBB
#ifdef TBB2
public:
  Tindex thrId;
#endif  // TBB2
private:
  /// =================================  DATA PART  =================================

  /// ----- Different parameters of CATrackFinder -----

  Tindex fFirstCAstation {0};  //first station used in CA

  // fNFindIterations - set number of interation for trackfinding
  // itetation of finding:
#ifdef FIND_GAPED_TRACKS
  enum
  {
    kFastPrimIter,     // primary fast tracks
    kAllPrimIter,      // primary all tracks
    kAllPrimJumpIter,  // primary tracks with jumped triplets
    kAllSecIter,       // secondary all tracks
    kAllPrimEIter,     // primary all electron tracks
    kAllSecEIter,      // secondary all electron tracks

    kFastPrimJumpIter,  // primary fast tracks with jumped triplets
    kFastPrimIter2,
    kAllSecJumpIter  // secondary tracks with jumped triplets
  };

#ifdef TRACKS_FROM_TRIPLETS
  int fNFindIterations = TRACKS_FROM_TRIPLETS_ITERATION + 1;  // TODO investigate kAllPrimJumpIter & kAllSecJumpIter
#else  // not TRACKS_FROM_TRIPLETS
  int fNFindIterations = -1;  // TODO investigate kAllPrimJumpIter & kAllSecJumpIter
#endif  // TRACKS_FROM_TRIPLETS

#else  // not FIND_GAPED_TRACKS
  enum
  {
    kFastPrimIter = 0,  // primary fast tracks
    kAllPrimIter,       // primary all tracks
    kAllSecIter,        // secondary all tracks
    kFastPrimJumpIter,  // disabled
    kAllPrimJumpIter,   // disabled
    kFastPrimIter2,
    kAllSecJumpIter,
    kAllPrimEIter,
    kAllSecEIter
  };
#endif  // FIND_GAPED_TRACKS

  std::map<int, int> threadNumberToCpuMap {};

  float fTrackChi2Cut {10.f};
  float fTripletChi2Cut {5.f};  // cut for selecting triplets before collecting tracks.per one DoF
  float fDoubletChi2Cut {5.f};
  float fTimeCut1 {0.f};  // TODO: please, specify "1" and "2" (S.Zharko)
  float fTimeCut2 {0.f};

  /// correction in order to take into account overlaping and iff z. if sort by y then it is max diff between same station's modules (~0.4cm)
  fvec fMaxDZ {L1Utils::kNaN};

  /// parameters which are different for different iterations. Set in the begin of CAL1TrackFinder

  float fPickGather {L1Utils::kNaN};     ///< same for attaching additional hits to track
  float fPickNeighbour {L1Utils::kNaN};  ///< (fPickNeighbour < dp/dp_error)  =>  triplets are neighbours
  fvec fMaxInvMom {L1Utils::kNaN};       ///< max considered q/p for tracks
  fvec fMaxSlopePV {L1Utils::kNaN};      ///< max slope (tx\ty) in prim vertex
  float fMaxSlope {L1Utils::kNaN};       ///< max slope (tx\ty) in 3d hit position of a triplet

  fvec fTargX {L1Utils::kNaN};  ///< target position x coordinate for the current iteration (modifiable)
  fvec fTargY {L1Utils::kNaN};  ///< target position y coordinate for the current iteration (modifiable)
  fvec fTargZ {L1Utils::kNaN};  ///< target position z coordinate for the current iteration (modifiable)

  L1FieldValue fTargB _fvecalignment {};               // field in the target point (modifiable, do not touch!!)
  L1XYMeasurementInfo TargetXYInfo _fvecalignment {};  // target constraint  [cm]


  //  int TripNumThread;

  int fTrackingLevel {2};       // currently not used
  int fGhostSuppression {1};    // currently not used
  float fMomentumCutOff {0.2};  // currently not used

  /// ----- Debug features -----
#ifdef PULLS
  L1AlgoPulls* fL1Pulls;
#endif
#ifdef TRIP_PERFORMANCE
  L1AlgoEfficiencyPerformance<3>* fL1Eff_triplets;
  L1AlgoEfficiencyPerformance<3>* fL1Eff_triplets2;
#endif
#ifdef DOUB_PERFORMANCE
  L1AlgoEfficiencyPerformance<2>* fL1Eff_doublets;
#endif
} _fvecalignment;


// ********************************************
// ** Inline member functions implementation **
// ********************************************

// ---------------------------------------------------------------------------------------------------------------------
//
[[gnu::always_inline]] inline unsigned int L1Algo::PackTripletId(unsigned int iStation, unsigned int iThread,
                                                                 unsigned int iTriplet)
{
#ifndef FAST_CODE
  assert(iStation < L1Constants::size::kMaxNstations);
  assert(iThread < L1Constants::size::kMaxNthreads);
  assert(iTriplet < L1Constants::size::kMaxNtriplets);
#endif
  constexpr unsigned int kMoveThread  = L1Constants::size::kTripletBits;
  constexpr unsigned int kMoveStation = L1Constants::size::kTripletBits + L1Constants::size::kThreadBits;
  return (iStation << kMoveStation) + (iThread << kMoveThread) + iTriplet;
}

// ---------------------------------------------------------------------------------------------------------------------
//
[[gnu::always_inline]] inline unsigned int L1Algo::TripletId2Station(unsigned int id)
{
  constexpr unsigned int kMoveStation = L1Constants::size::kTripletBits + L1Constants::size::kThreadBits;
  return id >> kMoveStation;
}

// ---------------------------------------------------------------------------------------------------------------------
//
[[gnu::always_inline]] inline unsigned int L1Algo::TripletId2Thread(unsigned int id)
{
  constexpr unsigned int kMoveThread = L1Constants::size::kTripletBits;
  constexpr unsigned int kThreadMask = (1u << L1Constants::size::kThreadBits) - 1u;
  return (id >> kMoveThread) & kThreadMask;
}

// ---------------------------------------------------------------------------------------------------------------------
//
[[gnu::always_inline]] inline unsigned int L1Algo::TripletId2Triplet(unsigned int id)
{
  constexpr unsigned int kTripletMask = (1u << L1Constants::size::kTripletBits) - 1u;
  return id & kTripletMask;
}

#endif
