/* Copyright (C) 2007-2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Maksym Zyzak, Valentina Akishina, Igor Kulakov [committer], Sergei Zharko */

#ifndef L1Algo_h
#define L1Algo_h

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

//#define GLOBAL
//#define mCBM

#define FIND_GAPED_TRACKS  // use triplets with gaps

#include <array>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>

#include "L1Branch.h"
#include "L1CloneMerger.h"
#include "L1Field.h"
#include "L1Fit.h"
#include "L1Grid.h"
#include "L1Hit.h"
#include "L1HitPoint.h"
#include "L1InputData.h"
#include "L1Parameters.h"
#include "L1Portion.h"
#include "L1Station.h"
#include "L1Track.h"
#include "L1TrackPar.h"
#include "L1Triplet.h"
#include "L1Utils.h"
#include "L1Vector.h"

class CbmL1MCTrack;

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

struct L1HitTimeInfo {
  fscal fEventTimeMin {-std::numeric_limits<fscal>::max() / 2.};
  fscal fEventTimeMax {std::numeric_limits<fscal>::max() / 2.};
  fscal fMaxTimeBeforeHit {0.};  //< max event time for hits [0 .. hit] in the station hit array
  fscal fMinTimeAfterHit {0.};   //< min event time for hits [hit ... ] in the station hit array
};

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

  /// Receives input data
  void ReceiveInputData(L1InputData&& inputData);

  /// Receives tracking parameters
  void ReceiveParameters(L1Parameters&& parameters);

  /// Gets pointer to input data object for external access
  const L1InputData& GetInputData() const { return fInputData; }

  /// ----- Hit-point-strips conversion routines ------

  void GetHitCoor(const L1Hit& _h, fscal& _x, fscal& _y, fscal& _z, const L1Station& sta);

  std::pair<fscal, fscal> GetHitCoorOnGrid(const L1Hit& h, char iS);


  L1HitPoint CreateHitPoint(const L1Hit& hit);  // full the hit point by hit information.

  void CreateHitPoint(const L1Hit& hit, L1HitPoint& point);
  inline int PackIndex(const int& a, const int& b, const int& c);

  inline int UnPackIndex(const int& i, int& a, int& b, int& c);

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

  /// ----- Subroutines used by L1Algo::CaTrackFinder() ------

  void ResetSliceData();

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

  /// Prepare the portion of left hits data
  void findSingletsStep0(  // input
    Tindex start_lh, Tindex n1_l, L1HitPoint* Hits_l,
    // output
    fvec* u_front_l, fvec* u_back_l, fvec* zPos_l, L1HitIndex_t* hitsl, fvec* t_l, fvec* dt2_l, fvec* du2_l,
    fvec* dv2_l);

  /// Get the field approximation. Add the target to parameters estimation. Propagate to middle station.
  void findSingletsStep1(  // input
    int istal, int istam, Tindex n1_V,

    fvec* u_front_l, fvec* u_back_l, fvec* zPos_l, fvec* t_l, fvec* dt2_l,
    // output
    L1TrackPar* T_1, L1FieldRegion* fld_1, fvec* du2_l, fvec* dv2_l);

  /// Find the doublets. Reformat data in the portion of doublets.
  void findDoubletsStep0(  // input
    Tindex n1, const L1Station& stal, L1HitPoint* vHits_l, const L1Station& stam, L1HitPoint* vHits_m, L1TrackPar* T_1,
    L1HitIndex_t* hitsl_1,
    // output
    Tindex& n2, L1Vector<L1HitIndex_t>& i1_2,
#ifdef DOUB_PERFORMANCE
    L1Vector<L1HitIndex_t>& hitsl_2,
#endif  // DOUB_PERFORMANCE
    L1Vector<L1HitIndex_t>& hitsm_2);

  /// Add the middle hits to parameters estimation. Propagate to right station.
  /// Find the triplets (right hit). Reformat data in the portion of triplets.
  void findTripletsStep0(  // input
    L1HitPoint* vHits_r, int istal, int istam, int istar, L1HitPoint* vHits_m, L1TrackPar* T_1, L1FieldRegion* fld_1,
    L1HitIndex_t* hitsl_1,

    Tindex n2, L1Vector<L1HitIndex_t>& hitsm_2, L1Vector<L1HitIndex_t>& i1_2,

    // output
    Tindex& n3, L1Vector<L1TrackPar>& T_3, L1Vector<L1HitIndex_t>& hitsl_3, L1Vector<L1HitIndex_t>& hitsm_3,
    L1Vector<L1HitIndex_t>& hitsr_3, L1Vector<fvec>& u_front_3, L1Vector<fvec>& u_back_3, L1Vector<fvec>& z_Pos_3,
    L1Vector<fvec>& du2_3, L1Vector<fvec>& dv2_3, L1Vector<fvec>& t_3, L1Vector<fvec>& dt2_3);

  /// Add the right hits to parameters estimation.
  void findTripletsStep1(  // input
    Tindex n3_V, const L1Station& star, L1Vector<fvec>& u_front_3, L1Vector<fvec>& u_back_3, L1Vector<fvec>& z_Pos_3,
    L1Vector<fvec>& du2_3, L1Vector<fvec>& dv2_3, L1Vector<fvec>& t_3, L1Vector<fvec>& dt2_3,
    // output
    L1Vector<L1TrackPar>& T_3);

  /// Refit Triplets.
  void findTripletsStep2(  // input
    Tindex n3, int istal, int istam, int istar, L1Vector<L1TrackPar>& T_3, L1Vector<L1HitIndex_t>& hitsl_3,
    L1Vector<L1HitIndex_t>& hitsm_3, L1Vector<L1HitIndex_t>& hitsr_3, int nIterations = 0);

  /// Select triplets. Save them into vTriplets.
  void findTripletsStep3(  // input
    Tindex n3, int istal, int istam, int istar, L1Vector<L1TrackPar>& T_3, L1Vector<L1HitIndex_t>& hitsl_3,
    L1Vector<L1HitIndex_t>& hitsm_3, L1Vector<L1HitIndex_t>& hitsr_3);


  /// Find neighbours of triplets. Calculate level of triplets.
  void f5(  // input
    // output
    int* nlevel);


  /// Find doublets on station
  void CreatePortionOfDoublets(  // input
    int istal, int istam, Tindex iSingletPortion, Tindex singletPortionSize,
    // output
    L1TrackPar* T_1, L1FieldRegion* fld_1, L1HitIndex_t* hitsl_1,
    //
    Tindex& n_2, L1Vector<L1HitIndex_t>& i1_2, L1Vector<L1HitIndex_t>& hitsm_2);

  /// Find triplets on station
  void CreatePortionOfTriplets(  // input
    int istal, int istam, int istar, L1TrackPar* T_1, L1FieldRegion* fld_1, L1HitIndex_t* hitsl_1, Tindex& n_2,
    L1Vector<L1HitIndex_t>& i1_2, L1Vector<L1HitIndex_t>& hitsm_2);

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

  void Init(const TrackingMode mode, const bool MissingHits);

  void Finish();

  void PrintHits();

  /// The main procedure - find tracks
  void CaTrackFinder();

  /// find tracks in a sub-timeslice
  void CaTrackFinderSlice();

  /// Track fitting procedures

  void L1KFTrackFitter();  // version from SIMD-KF benchmark


  float GetMaxInvMom() const { return fMaxInvMom[0]; }

  void SetNThreads(unsigned int n);

public:
  /// Gets number of stations before the pipe (MVD stations in CBM)
  int GetNstationsBeforePipe() const { return fNstationsBeforePipe; }

  /// Gets number of stations situated in field region (MVD + STS in CBM)
  int GetNfieldStations() const { return fNfieldStations; }

  /// Get mc track ID for a hit (debug tool)
  int GetMcTrackIdForCaHit(int iHit) const;

  /// Get mc track ID for a hit (debug tool)
  int GetMcTrackIdForGridHit(int iGridHit) const;

  const CbmL1MCTrack* GetMcTrackForGridHit(int iHit) const;

private:
  bool checkTripletMatch(const L1Triplet& l, const L1Triplet& r, fscal& dchi2) const;

  int fNstationsBeforePipe {0};  ///< number of stations before pipe (MVD stations in CBM)
  int fNfieldStations {0};       ///< number of stations in the field region
  //alignas(16) L1StationsArray_t fStations {};  ///< array of L1Station objects
  //alignas(16) L1MaterialArray_t fRadThick {};  ///< material for each station
  float fDefaultMass {L1Constants::phys::kMuonMass};  ///< mass of the propagated particle [GeV/c2]


  // ***************************
  // ** Member variables list **
  // ***************************

  L1Parameters fParameters;  ///< Object of L1Algo parameters class
  L1InputData fInputData;    ///< Tracking input data

  L1Vector<unsigned char> fvHitKeyFlags {
    "L1Algo::fvHitKeyFlags"};  ///< List of key flags: has been this hit or cluster already used

public:
  L1Vector<L1HitTimeInfo> fHitTimeInfo;

  L1Grid vGrid[L1Constants::size::kMaxNstations];      ///<
  L1Grid vGridTime[L1Constants::size::kMaxNstations];  ///<

  fscal fMaxDx[L1Constants::size::kMaxNstations];
  fscal fMaxDy[L1Constants::size::kMaxNstations];
  fscal fMaxDt[L1Constants::size::kMaxNstations];

  double fCaRecoTime {0.};  // time of the track finder + fitter

  L1Vector<L1Track> fRecoTracks {"L1Algo::fRecoTracks"};   ///< reconstructed tracks
  L1Vector<L1HitIndex_t> fRecoHits {"L1Algo::fRecoHits"};  ///< packed hits of reconstructed tracks

  L1Vector<L1Track> fSliceRecoTracks {"L1Algo::fSliceRecoTracks"};   ///< reconstructed tracks in sub-timeslice
  L1Vector<L1HitIndex_t> fSliceRecoHits {"L1Algo::fSliceRecoHits"};  ///< packed hits of reconstructed tracks

  /// Created triplets vs station and thread index
  L1Vector<L1Triplet> fTriplets[L1Constants::size::kMaxNstations][L1Constants::size::kMaxNthreads] {
    {"L1Algo::fTriplets"}};

  /// Track candidates created out of adjacent triplets before the final track selection.
  /// The candidates may share any amount of hits.
  L1Vector<L1Branch> fTrackCandidates[L1Constants::size::kMaxNthreads] {"L1Algo::fTrackCandidates"};

  L1Vector<Tindex> fSingletPortionSize[L1Constants::size::kMaxNstations] {
    "L1Algo::fSingletPortionSize"};  ///< Number of doublets in a portion


  L1Vector<L1HitIndex_t> fSliceHitIds {"L1Algo::fSliceHitIds"};                   ///< indices of the sub-slice hits
  L1HitIndex_t fSliceHitIdsStartIndex[L1Constants::size::kMaxNstations + 1] {0};  ///< start of station hit inices
  L1HitIndex_t fSliceHitIdsStopIndex[L1Constants::size::kMaxNstations + 1] {0};   ///< stop of station hit inices

  L1Vector<L1Hit> fGridHits {"L1Algo::fGridHits"};        ///< hits, ordered with respect to their grid bins
  L1Vector<L1Hit> fGridHitsBuf {"L1Algo::fGridHitsBuf"};  ///< hits, ordered with respect to their grid bins

  L1Vector<L1HitIndex_t> fGridHitIds {"L1Algo::fGridHitIds"};        ///< indices of grid hits: iGridHit -> iCaHit
  L1Vector<L1HitIndex_t> fGridHitIdsBuf {"L1Algo::fGridHitIdsBuf"};  ///< buffer for a new fGridHitIds

  L1Vector<L1HitPoint> fGridPoints {"L1Algo::fGridPoints"};  ///< grid points parallel to fGridHits
  L1Vector<L1HitPoint> fGridPointsBuf {"L1Algo::fGridPointsBuf"};

  L1HitIndex_t fGridHitStartIndex[L1Constants::size::kMaxNstations + 1] {0};
  L1HitIndex_t fGridHitStopIndex[L1Constants::size::kMaxNstations + 1] {0};

  L1Vector<L1Track> fSliceRecoTracks_thread[L1Constants::size::kMaxNthreads] {"L1Algo::fSliceRecoTracks_thread"};
  L1Vector<L1HitIndex_t> fSliceRecoHits_thread[L1Constants::size::kMaxNthreads] {"L1Algo::fSliceRecoHits_thread"};


#ifdef _OPENMP
  L1Vector<omp_lock_t> fStripToTrackLock {"L1Algo::fStripToTrackLock"};
#endif

  L1Vector<int> fStripToTrack {"L1Algo::fStripToTrack"};  // front strip to track pointers
  // L1Vector<int> fStripToTrack1B {"L1Algo::fStripToTrackB"};  // back strip to track pointers

  int fNThreads {0};
  bool fMissingHits {0};  ///< TODO ???
  TrackingMode fTrackingMode {kSts};

  fvec EventTime[L1Constants::size::kMaxNthreads][L1Constants::size::kMaxNthreads] {{0}};
  fvec Err[L1Constants::size::kMaxNthreads][L1Constants::size::kMaxNthreads] {{0}};

  /// --- data used during finding iterations
  int isec {0};                                       // iteration TODO: to be dispatched (S.Zharko, 21.06.2022)
  const L1CAIteration* fpCurrentIteration = nullptr;  ///< pointer to the current CA track finder iteration

  L1Vector<int> fHitFirstTriplet {"L1Algo::fHitFirstTriplet"};  /// link hit -> first triplet { hit, *, *}
  L1Vector<int> fHitNtriplets {"L1Algo::fHitNtriplets"};        /// link hit ->n triplets { hit, *, *}


  //  fvec u_front[Portion/fvecLen], u_back[Portion/fvecLen];
  //  fvec zPos[Portion/fvecLen];
  //  fvec fHitTime[Portion/fvecLen];

  L1Vector<L1TrackPar> fTripletPar[L1Constants::size::kMaxNthreads] {"fTripletPar"};

  L1Vector<L1HitIndex_t> fTripletHitsL[L1Constants::size::kMaxNthreads] {"L1Algo::fTripletHitsL"};
  L1Vector<L1HitIndex_t> fTripletHitsM[L1Constants::size::kMaxNthreads] {"L1Algo::fTripletHitsM"};
  L1Vector<L1HitIndex_t> fTripletHitsR[L1Constants::size::kMaxNthreads] {"L1Algo::fTripletHitsR"};

  L1Vector<fvec> fTripletHitR_Ufront[L1Constants::size::kMaxNthreads] {"L1Algo::fTripletHitR_Ufront"};
  L1Vector<fvec> fTripletHitR_Uback[L1Constants::size::kMaxNthreads] {"L1Algo::fTripletHitR_Uback"};
  L1Vector<fvec> fTripletHitR_Z[L1Constants::size::kMaxNthreads] {"L1Algo::fTripletHitR_Z"};
  L1Vector<fvec> fTripletHitR_Time[L1Constants::size::kMaxNthreads] {"L1Algo::fTripletHitR_Time"};
  L1Vector<fvec> fTripletHitR_TimeErr[L1Constants::size::kMaxNthreads] {"L1Algo::fTripletHitR_TimeErr"};
  L1Vector<fvec> fTripletHitR_dUfront[L1Constants::size::kMaxNthreads] {"L1Algo::fTripletHitR_dUfront"};
  L1Vector<fvec> fTripletHitR_dUback[L1Constants::size::kMaxNthreads] {"L1Algo::fTripletHitR_dUback"};


  //   Tindex NHits_l[L1Constants::size::kMaxNstations];
  //   Tindex NHits_l_P[L1Constants::size::kMaxNstations];
  /// ----- Output data -----


private:
  L1CloneMerger fCloneMerger {*this};  ///< Object of L1Algo clones merger algorithm

private:
  /// =================================  DATA PART  =================================

  /// ----- Different parameters of CATrackFinder -----

  Tindex fFirstCAstation {0};  //first station used in CA

  // fNFindIterations - set number of interation for trackfinding
  // itetation of finding:

  int fNFindIterations = -1;  // TODO investigate kAllPrimJumpIter & kAllSecJumpIter

  std::map<int, int> threadNumberToCpuMap {};

  float fTrackChi2Cut {10.f};
  float fTripletFinalChi2Cut {10.f};
  float fTripletChi2Cut {5.f};  // cut for selecting triplets before collecting tracks.per one DoF
  float fDoubletChi2Cut {5.f};
  float fTimeCut1 {0.f};  // TODO: please, specify "1" and "2" (S.Zharko)
  float fTimeCut2 {0.f};

  /// correction in order to take into account overlaping and iff z. if sort by y then it is max diff between same station's modules (~0.4cm)
  fvec fMaxDZ {L1Utils::kNaN};

  /// parameters which are different for different iterations. Set in the begin of CAL1TrackFinder

  float fPickGather {L1Utils::kNaN};       ///< same for attaching additional hits to track
  float fTripletLinkChi2 {L1Utils::kNaN};  ///< (dp2/dp_error2 < fTripletLinkChi2)  =>  triplets are neighbours
  fvec fMaxInvMom {L1Utils::kNaN};         ///< max considered q/p for tracks
  fvec fMaxSlopePV {L1Utils::kNaN};        ///< max slope (tx\ty) in prim vertex
  float fMaxSlope {L1Utils::kNaN};         ///< max slope (tx\ty) in 3d hit position of a triplet

  fvec fTargX {L1Utils::kNaN};  ///< target position x coordinate for the current iteration (modifiable)
  fvec fTargY {L1Utils::kNaN};  ///< target position y coordinate for the current iteration (modifiable)
  fvec fTargZ {L1Utils::kNaN};  ///< target position z coordinate for the current iteration (modifiable)

  bool fIsTargetField {false};  ///< is the magnetic field present at the target

  L1FieldValue fTargB _fvecalignment {};               // field in the target point (modifiable, do not touch!!)
  L1XYMeasurementInfo TargetXYInfo _fvecalignment {};  // target constraint  [cm]


  //  int TripNumThread;

  int fTrackingLevel {2};       // currently not used
  int fGhostSuppression {1};    // NOTE: Should be equal to 0 in TRACKS_FROM_TRIPLETS mode!
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
