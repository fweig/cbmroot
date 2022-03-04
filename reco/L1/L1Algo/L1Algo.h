/* Copyright (C) 2007-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Maksym Zyzak, Valentina Akishina, Igor Kulakov [committer] */

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

#define MERGE_CLONES
// #define TRACKS_FROM_TRIPLETS_ITERATION kAllPrimIter

//#define HitErrors
//#define GLOBAL
//#define mCBM

#define LAST_ITERATION kAllSecIter
#define FIND_GAPED_TRACKS  // use triplets with gaps
#define USE_RL_TABLE
#ifndef TRACKS_FROM_TRIPLETS
#define EXTEND_TRACKS
#endif
//#define USE_EVENT_NUMBER
//#endif
//#define MERGE_CLONES


/*********************************************************************************
 ************ TEMPORARY MACROS, SHOULD BE REMOVED (TODO!!, S. Zharko)  ***********
 *                                                                               */

#define FEATURING_L1ALGO_INIT 1  // If defined, new initialization will be used, if not - the old one
// Macro: 1 - new track finder loop, other - old track finder loop
#define FEATURING_L1ALGO_CATRACKFINDER_ITERATIONLOOP 1

/*                                                                               *
 *********************************************************************************/

#include <array>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>

#include "L1Branch.h"
#include "L1Field.h"
#include "L1Grid.h"
#include "L1Hit.h"
#include "L1HitPoint.h"
#include "L1HitsSortHelper.h"
#include "L1InitManager.h"
#include "L1Parameters.h"
#include "L1Portion.h"
#include "L1Station.h"
#include "L1Track.h"
#include "L1TrackPar.h"
#include "L1TrackParFit.h"
#include "L1Triplet.h"
#include "L1Vector.h"

#ifdef _OPENMP
#include "omp.h"
#endif

using std::map;

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
typedef int Tindex;

/// Central class of L1 tracking
///
class L1Algo {
public:
  L1Algo(unsigned int nThreads = 1);

  L1Algo(const L1Algo&) = delete;
  L1Algo operator=(const L1Algo&) = delete;

  /// set a default particle mass for the track fit
  /// it is used during reconstruction
  /// for the multiple scattering and energy loss estimation
  void SetDefaultParticleMass(float mass) { fDefaultMass = mass; }

  /// get default particle mass
  float GetDefaultParticleMass() const { return fDefaultMass; }

  /// get default particle mass squared
  float GetDefaultParticleMass2() const { return fDefaultMass * fDefaultMass; }

  float fDefaultMass = 0.10565800;  // muon mass
  // TODO: make fDefaultMass a private member (S.Zh.)

  /// pack station, thread and triplet indices to an unique triplet ID
  static unsigned int PackTripletId(unsigned int Station, unsigned int Thread, unsigned int Triplet)
  {
#ifndef FAST_CODE
    assert(Station < L1Parameters::kMaxNstations);
    assert(Thread < L1Parameters::kMaxNthreads);
    assert(Triplet < L1Parameters::kMaxNtriplets);
#endif
    constexpr unsigned int kMoveThread  = L1Parameters::kTripletBits;
    constexpr unsigned int kMoveStation = L1Parameters::kTripletBits + L1Parameters::kThreadBits;
    return (Station << kMoveStation) + (Thread << kMoveThread) + Triplet;
  }

  /// unpack the triplet ID to its station index
  static unsigned int TripletId2Station(unsigned int ID)
  {
    constexpr unsigned int kMoveStation = L1Parameters::kTripletBits + L1Parameters::kThreadBits;
    return ID >> kMoveStation;
  }

  /// unpack the triplet ID to its thread index
  static unsigned int TripletId2Thread(unsigned int ID)
  {
    constexpr unsigned int kMoveThread = L1Parameters::kTripletBits;
    constexpr unsigned int kThreadMask = (1u << L1Parameters::kThreadBits) - 1u;
    return (ID >> kMoveThread) & kThreadMask;
  }

  /// unpack the triplet ID to its triplet index
  static unsigned int TripletId2Triplet(unsigned int ID)
  {
    constexpr unsigned int kTripletMask = (1u << L1Parameters::kTripletBits) - 1u;
    return ID & kTripletMask;
  }


  L1Vector<L1Triplet> fTriplets[L1Parameters::kMaxNstations][L1Parameters::kMaxNthreads] {
    {"L1Algo::fTriplets"}};  // created triplets at station + thread

  // Track candidates created out of adjacent triplets before the final track selection.
  // The candidates may share any amount of hits.
  L1Vector<L1Branch> fTrackCandidates[L1Parameters::kMaxNthreads] {"L1Algo::fTrackCandidates"};

  Tindex fDupletPortionStopIndex[L1Parameters::kMaxNstations] {0};     // end of the duplet portions for the station
  L1Vector<Tindex> fDupletPortionSize {"L1Algo::fDupletPortionSize"};  // N duplets in a portion

  //
  // Temporary vectors used by the clone merger
  //
  // vectors that are parallel to fTracks
  L1Vector<unsigned short> fMergerTrackFirstStation {"L1Algo::fMergerTrackFirstStation"};  // first station of a track
  L1Vector<unsigned short> fMergerTrackLastStation {"L1Algo::fMergerTrackLastStation"};    // last station of a track
  L1Vector<THitI> fMergerTrackFirstHit {"L1Algo::fMergerTrackFirstHit"};  // index of the first tracks hit
  L1Vector<THitI> fMergerTrackLastHit {"L1Algo::fMergerTrackLastHit"};    // index of the last tracks hit
  L1Vector<unsigned short> fMergerTrackNeighbour {
    "L1Algo::fMergerTrackNeighbour"};                             // track that can be merged with the given track
  L1Vector<float> fMergerTrackChi2 {"L1Algo::fMergerTrackChi2"};  // chi2 of the merge
  L1Vector<char> fMergerTrackIsStored {"L1Algo::fMergerTrackIsStored"};  // is the track already stored to the output
  L1Vector<char> fMergerTrackIsDownstreamNeighbour {
    "L1Algo::fMergerTrackIsDownstreamNeighbour"};  // is the track a downstream neighbor of another track
  // other vectors
  L1Vector<L1Track> fMergerTracksNew {"L1Algo::fMergerTracksNew"};    // vector of tracks after the merge
  L1Vector<THitI> fMergerRecoHitsNew {"L1Algo::fMergerRecoHitsNew"};  // vector of track hits after the merge


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

  void Init(const L1Vector<fscal>& geo, const bool UseHitErrors, const TrackingMode mode, const bool MissingHits);

  void SetData(L1Vector<L1Hit>& StsHits_, int nStsStrips_, L1Vector<unsigned char>& SFlag_,
               const THitI* StsHitsStartIndex_, const THitI* StsHitsStopIndex_);

  void PrintHits();

  /// The main procedure - find tracks.
  void CATrackFinder();

  /// Track fitting procedures
  void KFTrackFitter_simple();  // version, which use procedured used during the reconstruction
  void L1KFTrackFitter();       // version from SIMD-KF benchmark

  void L1KFTrackFitterMuch();

  float GetMaxInvMom() const { return MaxInvMom[0]; }

  /// ----- Input data -----
  // filled in CbmL1::ReadEvent();

  void SetNThreads(unsigned int n);

  int NStations {0};        ///< number of all detector stations
  int NMvdStations {0};     ///< number of mvd stations
  int NStsStations {0};     ///< number of sts stations
  int fNfieldStations {0};  ///< number of stations in the field region


  // TODO: Replace _fvecalignment with C++11 alignas(16) attibute, see vStationsNew (S.Zh.)
  L1Station vStations[L1Parameters::kMaxNstations] _fvecalignment;  // station info
  alignas(16) std::array<L1Station, L1Parameters::kMaxNstations> fStationsNew;
  L1Vector<L1Material> fRadThick {"fRadThick"};        // material for each station

  int NStsStrips {0};                         // number of strips
  L1Vector<L1Hit>* vStsHits {nullptr};        // hits as a combination of front-, backstrips and z-position
  L1Grid vGrid[L1Parameters::kMaxNstations];  // hits as a combination of front-, backstrips and z-position
  L1Grid vGridTime[L1Parameters::kMaxNstations];

  L1Vector<unsigned char>* fStripFlag {nullptr};  // information of hits station & using hits in tracks;

  double fCATime {0.};  // time of track finding

  L1Vector<L1Track> fTracks {"L1Algo::fTracks"};    // reconstructed tracks
  L1Vector<THitI> fRecoHits {"L1Algo::fRecoHits"};  // packed hits of reconstructed tracks

  const THitI* StsHitsStartIndex {nullptr};  // station-bounders in vStsHits array
  const THitI* StsHitsStopIndex {nullptr};   // station-bounders in vStsHits array


  //  L1Branch* pointer;
  unsigned int NHitsIsecAll {0};

  L1Vector<L1Hit> vStsDontUsedHits_A {"L1Algo::vStsDontUsedHits_A"};
  L1Vector<L1Hit> vStsDontUsedHits_B {"L1Algo::vStsDontUsedHits_B"};
  L1Vector<L1Hit> vStsDontUsedHits_Buf {"L1Algo::vStsDontUsedHits_Buf"};
  L1Vector<L1HitPoint> vStsDontUsedHitsxy_A {"L1Algo::vStsDontUsedHitsxy_A"};
  L1Vector<L1HitPoint> vStsDontUsedHitsxy_buf {"L1Algo::vStsDontUsedHitsxy_buf"};
  L1Vector<L1HitPoint> vStsDontUsedHitsxy_B {"L1Algo::vStsDontUsedHitsxy_B"};
  L1Vector<L1Track> fTracks_local[L1Parameters::kMaxNthreads] {"L1Algo::fTracks_local"};
  L1Vector<THitI> fRecoHits_local[L1Parameters::kMaxNthreads] {"L1Algo::fRecoHits_local"};

  L1Vector<THitI> RealIHit_v {"L1Algo::RealIHit_v"};
  L1Vector<THitI> RealIHit_v_buf {"L1Algo::RealIHit_v_buf"};
  L1Vector<THitI> RealIHit_v_buf2 {"L1Algo::RealIHit_v_buf2"};

#ifdef _OPENMP
  L1Vector<omp_lock_t> fHitToBestTrackF {"L1Algo::fHitToBestTrackF"};
  L1Vector<omp_lock_t> fHitToBestTrackB {"L1Algo::fHitToBestTrackB"};
#endif

  L1Vector<int> fStripToTrack {"L1Algo::fStripToTrack"};    // front strip to track pointers
  L1Vector<int> fStripToTrackB {"L1Algo::fStripToTrackB"};  // back strip to track pointers

  int fNThreads {0};
  bool fUseHitErrors {true};
  bool fMissingHits {0};
  TrackingMode fTrackingMode {kSts};

  fvec EventTime[L1Parameters::kMaxNthreads][L1Parameters::kMaxNthreads] {{0}};
  fvec Err[L1Parameters::kMaxNthreads][L1Parameters::kMaxNthreads] {{0}};


  /// standard sizes of the arrays
  enum
  {
    multiCoeff = 1,  // central - 1, mbias


    coeff = 64 / 4,

    Portion = 1024 / coeff,  // portion of left hits

    MaxPortionDoublets = 10000 / 5 * 64 / 2 / coeff /*/ multiCoeff*/ * 1,
    MaxPortionTriplets = 10000 * 5 * 64 / 2 / coeff /*/ multiCoeff*/ * 1,
    MaxNPortion        = 40 * coeff / multiCoeff,


    MaxArrSize = MaxNPortion * MaxPortionDoublets
                 / L1Parameters::
                   kMaxNstations  //200000,  // standart size of big arrays  // mas be 40000 for normal work in cbmroot!
  };


  /// --- data used during finding iterations

  int isec {0};  // iteration
  L1Vector<L1Hit>* vStsHitsUnused {nullptr};
  L1Vector<THitI>* RealIHitP {nullptr};
  L1Vector<THitI>* RealIHitPBuf {nullptr};
  L1Vector<L1HitPoint>* vStsHitPointsUnused {nullptr};
  THitI* RealIHit {nullptr};  // index in vStsHits indexed by index in vStsHitsUnused

  THitI StsHitsUnusedStartIndex[L1Parameters::kMaxNstations + 1] {0};
  THitI StsHitsUnusedStopIndex[L1Parameters::kMaxNstations + 1] {0};
  THitI StsHitsUnusedStartIndexEnd[L1Parameters::kMaxNstations + 1] {0};
  THitI StsHitsUnusedStopIndexEnd[L1Parameters::kMaxNstations + 1] {0};


  L1Vector<int> TripForHit[2] {"L1Algo::TripForHit"};  // TODO: what does '2' stand for?


  //  fvec u_front[Portion/fvecLen], u_back[Portion/fvecLen];
  //  fvec zPos[Portion/fvecLen];
  //  fvec fHitTime[Portion/fvecLen];

  nsL1::vector<L1TrackPar>::TSimd fT_3[L1Parameters::kMaxNthreads];

  L1Vector<THitI> fhitsl_3[L1Parameters::kMaxNthreads] {"L1Algo::fhitsl_3"};
  L1Vector<THitI> fhitsm_3[L1Parameters::kMaxNthreads] {"L1Algo::fhitsm_3"};
  L1Vector<THitI> fhitsr_3[L1Parameters::kMaxNthreads] {"L1Algo::fhitsr_3"};

  nsL1::vector<fvec>::TSimd fu_front3[L1Parameters::kMaxNthreads];
  nsL1::vector<fvec>::TSimd fu_back3[L1Parameters::kMaxNthreads];
  nsL1::vector<fvec>::TSimd fz_pos3[L1Parameters::kMaxNthreads];
  nsL1::vector<fvec>::TSimd fTimeR[L1Parameters::kMaxNthreads];
  nsL1::vector<fvec>::TSimd fTimeER[L1Parameters::kMaxNthreads];
  nsL1::vector<fvec>::TSimd dx[L1Parameters::kMaxNthreads];
  nsL1::vector<fvec>::TSimd dy[L1Parameters::kMaxNthreads];
  nsL1::vector<fvec>::TSimd du[L1Parameters::kMaxNthreads];
  nsL1::vector<fvec>::TSimd dv[L1Parameters::kMaxNthreads];


  //   Tindex NHits_l[L1Parameters::kMaxNstations];
  //   Tindex NHits_l_P[L1Parameters::kMaxNstations];
  /// ----- Output data -----

  friend class CbmL1;

  const L1FieldValue& GetVtxFieldValue() const { return vtxFieldValue; }
  const L1FieldRegion& GetVtxFieldRegion() const { return vtxFieldRegion; }
  /// ----- Hit-point-strips conversion routines ------

  void GetHitCoor(const L1Hit& _h, fscal& _x, fscal& _y, fscal& _z, const L1Station& sta);

  void dUdV_to_dY(const fvec& u, const fvec& v, fvec& _y, const L1Station& sta);

  void dUdV_to_dX(const fvec& u, const fvec& v, fvec& _x, const L1Station& sta);

  void dUdV_to_dXdY(const fvec& u, const fvec& v, fvec& _xy, const L1Station& sta);

  void GetHitCoor(const L1Hit& _h, fscal& _x, fscal& _y, char iS);
  void StripsToCoor(const fscal& u, const fscal& v, fscal& x, fscal& y,
                    const L1Station& sta) const;  // convert strip positions to coordinates
  void StripsToCoor(const fscal& u, const fscal& v, fvec& x, fvec& y,
                    const L1Station& sta) const;  // convert strip positions to coordinates
  void StripsToCoor(const fvec& u, const fvec& v, fvec& x, fvec& y, const L1Station& sta) const;
  L1HitPoint CreateHitPoint(const L1Hit& hit);  // full the hit point by hit information.

  void CreateHitPoint(const L1Hit& hit, L1HitPoint& point);
  inline int PackIndex(const int& a, const int& b, const int& c);

  inline int UnPackIndex(const int& i, int& a, int& b, int& c);
  /// -- Flags routines --
  inline __attribute__((always_inline)) static unsigned char GetFStation(unsigned char flag) { return flag / 4; }
  inline __attribute__((always_inline)) static bool GetFUsed(unsigned char flag) { return (flag & 0x02) != 0; }
  //   bool GetFUsedD  ( unsigned char flag ){ return (flag&0x01)!=0; }


  /// Sets L1Algo parameters object
  void SetParameters(const L1Parameters& other) { fParameters = other; }

  /// Gets a pointer to the L1Algo parameters object
  L1Parameters* GetParameters() { return &fParameters; }
  /// Gets a pointer to the L1Algo initialization object
  L1InitManager* GetInitManager() { return &fInitManager; }

  fvec GetCbmTargetZ() const { return fCbmTargetZ; }

private:
  L1Parameters fParameters {};                ///< Object of L1Algo parameters class
  L1InitManager fInitManager {&fParameters};  ///< Object of L1Algo initialization manager class


  /// =================================  FUNCTIONAL PART  =================================

  /// ----- Subroutines used by L1Algo::CATrackFinder() ------

  void CAFindTrack(int ista, L1Branch& best_tr, unsigned char& best_L, fscal& best_chi2, const L1Triplet* curr_trip,
                   L1Branch& curr_tr, unsigned char& curr_L, fscal& curr_chi2, unsigned char min_best_l,
                   L1Branch* new_tr);


  /// Fit track
  /// t - track with hits
  /// T - track params
  /// dir - 0 - forward, 1 - backward
  /// qp0 - momentum for extrapolation
  /// initialize - should be params ititialized. 1 - yes.
  void BranchFitterFast(const L1Branch& t, L1TrackPar& T, const bool dir, const fvec qp0 = 0.,
                        const bool initParams = true);

  /// Fit track. more precise than FitterFast
  void BranchFitter(const L1Branch& t, L1TrackPar& T, const bool dir, const fvec qp0 = 0.,
                    const bool initParams = true);

  /// Find additional hits for existing track
  /// t - track with hits
  /// T - track params
  /// dir - 0 - forward, 1 - backward
  /// qp0 - momentum for extrapolation
  void FindMoreHits(L1Branch& t, L1TrackPar& T, const bool dir, const fvec qp0 = 0.0);

  /// Find additional hits for existing track
  /// return chi2
  fscal BranchExtender(L1Branch& t);

  /// ----- Subroutines used by L1Algo::CAMergeClones() ------
  void InvertCholetsky(fvec a[15]);
  void MultiplySS(fvec const C[15], fvec const V[15], fvec K[5][5]);
  void MultiplyMS(fvec const C[5][5], fvec const V[15], fvec K[15]);
  void MultiplySR(fvec const C[15], fvec const r_in[5], fvec r_out[5]);
  void FilterTracks(fvec const r[5], fvec const C[15], fvec const m[5], fvec const V[15], fvec R[5], fvec W[15],
                    fvec* chi2);
  void CAMergeClones();


  inline __attribute__((always_inline)) void PackLocation(unsigned int& location, unsigned int& triplet,
                                                          unsigned int iStation, unsigned int& thread)
  {
    location = (triplet << 11) | (thread << 3) | iStation;
  }

  inline __attribute__((always_inline)) void UnPackStation(unsigned int& location, unsigned int& iStation)
  {
    iStation = location & 0x7;
  }

  inline __attribute__((always_inline)) void UnPackThread(unsigned int& location, unsigned int& thread)
  {
    thread = (location >> 3) & 0xFF;
  }

  inline __attribute__((always_inline)) void UnPackTriplet(unsigned int& location, unsigned int& triplet)
  {
    triplet = (location >> 11);
  }

  inline __attribute__((always_inline)) void SetFStation(unsigned char& flag, unsigned int iStation)
  {
    flag = iStation * 4 + (flag % 4);
  }
  inline __attribute__((always_inline)) void SetFUsed(unsigned char& flag) { flag |= 0x02; }
  //   void SetFUsedD   ( unsigned char &flag ){ flag |= 0x01; }
  inline __attribute__((always_inline)) void SetFUnUsed(unsigned char& flag) { flag &= 0xFC; }
  //   void SetFUnUsedD ( unsigned char &flag ){ flag &= 0xFE; }

  /// Prepare the portion of left hits data
  void f10(  // input
    Tindex start_lh, Tindex n1_l, L1HitPoint* StsHits_l,
    // output
    fvec* u_front_l, fvec* u_back_l, fvec* zPos_l, THitI* hitsl, fvec* HitTime_l, fvec* HitTimeEr, fvec* Event_l,
    fvec* d_x, fvec* d_y, fvec* d_xy, fvec* d_u, fvec* d_v);

  /// Get the field approximation. Add the target to parameters estimation. Propagate to middle station.
  void f11(  // input
    int istal, int istam, Tindex n1_V,

    fvec* u_front_l, fvec* u_back_l, fvec* zPos_l, fvec* HitTime_l, fvec* HitTimeEr,
    // output
    L1TrackPar* T_1, L1FieldRegion* fld_1, fvec* d_x, fvec* d_y, fvec* d_xy, fvec* d_u, fvec* d_v);

  /// Find the doublets. Reformat data in the portion of doublets.
  void f20(  // input
    Tindex n1, L1Station& stal, L1Station& stam, L1HitPoint* vStsHits_m, L1TrackPar* T_1, THitI* hitsl_1,

    // output
    Tindex& n2, L1Vector<THitI>& i1_2,

#ifdef DOUB_PERFORMANCE
    L1Vector<THitI>& hitsl_2,
#endif  // DOUB_PERFORMANCE
    L1Vector<THitI>& hitsm_2, fvec* Event, L1Vector<char>& lmDuplets);

  /// Add the middle hits to parameters estimation. Propagate to right station.
  /// Find the triplets (right hit). Reformat data in the portion of triplets.
  void f30(  // input
    L1HitPoint* vStsHits_r, L1Station& stam, L1Station& star,

    int istam, int istar, L1HitPoint* vStsHits_m, L1TrackPar* T_1, L1FieldRegion* fld_1, THitI* hitsl_1,

    Tindex n2, L1Vector<THitI>& hitsm_2, L1Vector<THitI>& i1_2,

    const L1Vector<char>& mrDuplets,
    // output
    Tindex& n3, nsL1::vector<L1TrackPar>::TSimd& T_3, L1Vector<THitI>& hitsl_3, L1Vector<THitI>& hitsm_3,
    L1Vector<THitI>& hitsr_3, nsL1::vector<fvec>::TSimd& u_front_3, nsL1::vector<fvec>::TSimd& u_back_3,
    nsL1::vector<fvec>::TSimd& z_Pos_3,
    //    nsL1::vector<fvec>::TSimd& dx_,
    //    nsL1::vector<fvec>::TSimd& dy_,
    nsL1::vector<fvec>::TSimd& du_, nsL1::vector<fvec>::TSimd& dv_, nsL1::vector<fvec>::TSimd& timeR,
    nsL1::vector<fvec>::TSimd& timeER);

  /// Add the right hits to parameters estimation.
  void f31(  // input
    Tindex n3_V, L1Station& star, nsL1::vector<fvec>::TSimd& u_front_3, nsL1::vector<fvec>::TSimd& u_back_3,
    nsL1::vector<fvec>::TSimd& z_Pos_3,
    //    nsL1::vector<fvec>::TSimd& dx_,
    //    nsL1::vector<fvec>::TSimd& dy_,
    nsL1::vector<fvec>::TSimd& du_, nsL1::vector<fvec>::TSimd& dv_, nsL1::vector<fvec>::TSimd& timeR,
    nsL1::vector<fvec>::TSimd& timeER,
    // output
    nsL1::vector<L1TrackPar>::TSimd& T_3);

  /// Refit Triplets.
  void f32(  // input
    Tindex n3, int istal, nsL1::vector<L1TrackPar>::TSimd& T_3, L1Vector<THitI>& hitsl_3, L1Vector<THitI>& hitsm_3,
    L1Vector<THitI>& hitsr_3, int nIterations = 0);

  /// Select triplets. Save them into vTriplets.
  void f4(  // input
    Tindex n3, int istal, int istam, int istar, nsL1::vector<L1TrackPar>::TSimd& T_3, L1Vector<THitI>& hitsl_3,
    L1Vector<THitI>& hitsm_3, L1Vector<THitI>& hitsr_3,
    // output
    Tindex& nstaltriplets, L1Vector<THitI>* hitsn_3 = 0, L1Vector<THitI>* hitsr_5 = 0

    // #ifdef XXX
    //                 ,unsigned int &stat_n_trip
    // #endif
  );


  /// Find neighbours of triplets. Calculate level of triplets.
  void f5(  // input
    // output
    int* nlevel);


  /// Find doublets on station
  void DupletsStaPort(  // input
    int istal, int istam, Tindex ip, L1Vector<Tindex>& n_g, Tindex* portionStopIndex_,

    // output
    L1TrackPar* T_1, L1FieldRegion* fld_1, THitI* hitsl_1,

    L1Vector<char>& lmDuplets,


    Tindex& n_2, L1Vector<THitI>& i1_2, L1Vector<THitI>& hitsm_2);

  /// Find triplets on station
  void TripletsStaPort(  // input
    int istal, int istam, int istar, Tindex& nstaltriplets, L1TrackPar* T_1, L1FieldRegion* fld_1, THitI* hitsl_1,

    Tindex& n_2, L1Vector<THitI>& i1_2, L1Vector<THitI>& hitsm_2,

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

#ifdef TBB
  enum
  {
    nthreads = 3,  // number of threads
    nblocks  = 1   // number of stations on one thread
  };

  friend class ParalleledDup;
  friend class ParalleledTrip;
#endif  // TBB
#ifdef TBB2
public:
  Tindex thrId;
#endif  // TBB2
private:
  /// =================================  DATA PART  =================================

  /// ----- Different parameters of CATrackFinder -----

  Tindex FIRSTCASTATION {0};  //first station used in CA

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
  enum
  {
    fNFindIterations = TRACKS_FROM_TRIPLETS_ITERATION + 1
  };  // TODO investigate kAllPrimJumpIter & kAllSecJumpIter
#else  // not TRACKS_FROM_TRIPLETS

  enum
  {
    fNFindIterations = 4
  };  // TODO investigate kAllPrimJumpIter & kAllSecJumpIter


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

  map<int, int> threadNumberToCpuMap {};

  static constexpr float kNaN {std::numeric_limits<float>::signaling_NaN()};

  float TRACK_CHI2_CUT {10.f};
  float TRIPLET_CHI2_CUT {5.f};  // cut for selecting triplets before collecting tracks.per one DoF
  float DOUBLET_CHI2_CUT {5.f};
  float TIME_CUT1 {0.f};
  float TIME_CUT2 {0.f};

  fvec MaxDZ {
    kNaN};  // correction in order to take into account overlaping and iff z. if sort by y then it is max diff between same station's modules (~0.4cm)

  /// parameters which are different for different iterations. Set in the begin of CAL1TrackFinder

  float Pick_gather {kNaN};    // same for attaching additional hits to track
  float PickNeighbour {kNaN};  // (PickNeighbour < dp/dp_error)  =>  triplets are neighbours
  fvec MaxInvMom {kNaN};       // max considered q/p for tracks
  fvec MaxSlopePV {kNaN};      // max slope (tx\ty) in prim vertex
  float MaxSlope {kNaN};       // max slope (tx\ty) in 3d hit position of a triplet
  fvec fCbmTargetX {kNaN};     // target position
  fvec fCbmTargetY {kNaN};
  fvec fCbmTargetZ {kNaN};
  fvec fTargX {kNaN};  // target position for the current iteration
  fvec fTargY {kNaN};
  fvec fTargZ {kNaN};

  L1FieldValue targB _fvecalignment {};                // field in the target point
  L1XYMeasurementInfo TargetXYInfo _fvecalignment {};  // target constraint  [cm]


  L1FieldRegion vtxFieldRegion _fvecalignment {};  // really doesn't used
  L1FieldValue vtxFieldValue _fvecalignment {};    // field at teh vertex position.

  //  int TripNumThread;

  int fTrackingLevel {0};     // currently not used
  int fGhostSuppression {0};  // currently not used
  float fMomentumCutOff {0};  // currently not used

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
#ifdef DRAW
  friend class L1AlgoDraw;
#endif
} _fvecalignment;

#endif
