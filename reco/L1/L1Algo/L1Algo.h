#ifndef L1Algo_h
#define L1Algo_h 1

// #define TBB // TODO: Doesn't work now. Renew

/// Debug features
// #define PULLS            // triplets pulls
// #define TRIP_PERFORMANCE // triplets efficiencies
// #define DOUB_PERFORMANCE // doublets efficiencies
//#define DRAW             // event display
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


#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

#include "L1Branch.h"
#include "L1Field.h"
#include "L1Grid.h"
#include "L1HitPoint.h"
#include "L1HitsSortHelper.h"
#include "L1Portion.h"
#include "L1Station.h"
#include "L1StsHit.h"
#include "L1Track.h"
#include "L1TrackPar.h"
#include "L1TrackParFit.h"
#include "L1Triplet.h"

#ifdef _OPENMP
#include "omp.h"
#endif

using std::map;
using std::vector;

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


class L1Algo {
public:
  //  L1Algo(int nThreads=7):
  L1Algo(int nThreads = 1, int TypicalSize = 200000)
    : n_g1("L1Algo::n_g1")
    , FirstHit("L1Algo::FirstHit")
    , LastHit("L1Algo::LastHit")
    , FirstHitIndex("L1Algo::FirstHitIndex")
    , LastHitIndex("L1Algo::LastHitIndex")
    , Neighbour("L1Algo::Neighbour")
    , TrackChi2("L1Algo::TrackChi2")
    , vRecoHitsNew("L1Algo::vRecoHitsNew")
    , vTracksNew("L1Algo::vTracksNew")
    , NStations(0)
    ,  // number of all detector stations
    NMvdStations(0)
    ,  // number of mvd stations
    NStsStations(0)
    , NFStations(0)
    , fRadThick()
    , NStsStrips(0)  // strips positions created from hits
    , vStsZPos(0)
    ,            // all possible z-positions of hits
    vStsHits(0)  // hits as a combination of front-, backstrips and z-position
    , vSFlag(0)  // information of hits station & using hits in tracks(),
    , CATime(0)
    ,  // time of trackfinding
    vTracks("L1Algo::vTracks", 40000)
    ,  // reconstructed tracks
    vRecoHits("L1Algo::vRecoHits", 400000)
    ,  // packed hits of reconstructed tracks
    StsHitsStartIndex(nullptr)
    , StsHitsStopIndex(nullptr)
    , NHitsIsecAll(0)
    , NTracksIsecAll(0)
    , vStsDontUsedHits_A(TypicalSize)
    , vStsDontUsedHits_B(TypicalSize)
    , vStsDontUsedHits_Buf(TypicalSize)
    , vStsDontUsedHitsxy_A(TypicalSize)
    , vStsDontUsedHitsxy_buf(TypicalSize)
    , vStsDontUsedHitsxy_B(TypicalSize)
    , RealIHit_v(TypicalSize)
    , RealIHit_v_buf(TypicalSize)
    , RealIHit_v_buf2(TypicalSize)
    ,

#ifdef _OPENMP
    hitToBestTrackF("L1Algo::hitToBestTrackF", TypicalSize * 2)
    , hitToBestTrackB("L1Algo::hitToBestTrackB", TypicalSize * 2)
    ,
#endif
    vStripToTrack("L1Algo::vStripToTrack", TypicalSize * 4)
    , vStripToTrackB("L1Algo::vStripToTrackB", TypicalSize * 4)
    ,
    //sh (),
    fNThreads(nThreads)
    , fUseHitErrors(0)
    , fmCBMmode(0)
    , fGlobal(0)
    , isec(0)
    , vStsHitsUnused()
    , RealIHitP()
    , RealIHitPBuf()
    , vStsHitPointsUnused()
    , RealIHit(nullptr)
    , FIRSTCASTATION()
    , threadNumberToCpuMap()
    , TRACK_CHI2_CUT(10.)
    , TRIPLET_CHI2_CUT(5.)
    , DOUBLET_CHI2_CUT(5.)
    , TIME_CUT1(0.)
    , TIME_CUT2(0.)
    , MaxDZ(0.)
    ,
#ifdef DRAW
    draw(0)
    ,
#endif
    Pick_gather(0)
    , PickNeighbour(0)
    ,  // (PickNeighbour < dp/dp_error)  =>  triplets are neighbours
    MaxInvMom(0)
    ,  // max considered q/p for tracks
    MaxSlope(0)
    , targX(0)
    , targY(0)
    , targZ(0)
    ,  // target coor
    targB()
    ,  // field in the target point
    TargetXYInfo()
    ,  // target constraint  [cm]
    vtxFieldRegion()
    ,  // really doesn't used
    vtxFieldValue()
    ,  // field at teh vertex position.
    //vTripletsP(), // container for triplets got in finding
    fTrackingLevel(0)
    , fGhostSuppression(0)
    ,                   // really doesn't used
    fMomentumCutOff(0)  // really doesn't used
  {

    n_g1.resize(100000);

    for (int i = 0; i < fNThreads; i++) {

      vTracks_local[i].resize(100000);
      vRecoHits_local[i].resize(400000);


      numberCandidateThread[i] = 0;
      SavedCand[i]             = 0;
      SavedHits[i]             = 0;

      TripForHit[0].resize(TypicalSize);
      TripForHit[1].resize(TypicalSize);
      CandidatesTrack[i].resize(10000);

      fT_3[i].reserve(MaxPortionTriplets / fvecLen);
      fhitsl_3[i].reserve(MaxPortionTriplets);
      fhitsm_3[i].reserve(MaxPortionTriplets);
      fhitsr_3[i].reserve(MaxPortionTriplets);
      fu_front3[i].reserve(MaxPortionTriplets / fvecLen);
      fu_back3[i].reserve(MaxPortionTriplets / fvecLen);
      fz_pos3[i].reserve(MaxPortionTriplets / fvecLen);
      fTimeR[i].reserve(MaxPortionTriplets / fvecLen);
      fTimeER[i].reserve(MaxPortionTriplets / fvecLen);
      dx[i].reserve(MaxPortionTriplets / fvecLen);
      dy[i].reserve(MaxPortionTriplets / fvecLen);
      du[i].reserve(MaxPortionTriplets / fvecLen);
      dv[i].reserve(MaxPortionTriplets / fvecLen);

      for (int j = 0; j < MaxNStations; j++)
        TripletsLocal1[j][i].resize(400000);
    }

    for (int i = 0; i < MaxNStations; i++)
      vGridTime[i].AllocateMemory(fNThreads);

#ifdef _OPENMP

    for (unsigned int j = 0; j < hitToBestTrackB.size(); j++) {
      omp_init_lock(&hitToBestTrackB[j]);
      omp_init_lock(&hitToBestTrackF[j]);
    }

#endif


    for (int i = 0; i < nThreads; i++)
      for (int k = 0; k < MaxNStations; k++)
        nTripletsThread[k][i] = 0;

    NTracksIsecAll = 20000;
    NHitsIsecAll   = TypicalSize;


    FirstHit.resize(NTracksIsecAll);
    LastHit.resize(NTracksIsecAll);
    FirstHitIndex.resize(NTracksIsecAll);
    LastHitIndex.resize(NTracksIsecAll);
    //     IsUsed.resize(NTracksIsecAll);
    TrackChi2.resize(NTracksIsecAll);
    Neighbour.resize(NTracksIsecAll);
    //     IsNext.resize(NTracksIsecAll);
  }

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

  static const int nTh  = 1;
  static const int nSta = 25;

  float fDefaultMass = 0.10565800;  // muon mass

  L1Vector<L1Triplet> TripletsLocal1[nSta][nTh];
  L1Vector<L1Branch> CandidatesTrack[nTh];

  Tindex portionStopIndex[nSta];
  L1Vector<Tindex> n_g1;


  int SavedCand[nTh];
  int SavedHits[nTh];

  int numberCandidateThread[nTh];

  int nTripletsThread[nSta][nTh];

  //for merger
  L1Vector<unsigned short> FirstHit;
  L1Vector<unsigned short> LastHit;
  L1Vector<THitI> FirstHitIndex;
  L1Vector<THitI> LastHitIndex;
  L1Vector<unsigned short> Neighbour;
  L1Vector<float> TrackChi2;
  //   L1Vector<bool> IsNext;
  //   L1Vector<bool> IsUsed;
  L1Vector<THitI> vRecoHitsNew;
  L1Vector<L1Track> vTracksNew;


#ifdef DRAW
  L1AlgoDraw* draw;
  void DrawRecoTracksTime(const vector<CbmL1Track>& tracks);
#endif


  void Init(const vector<fscal>& geo, const bool UseHitErrors, const bool mCBMmode);

  void SetData(const vector<L1StsHit>& StsHits_, int nStsStrips_, const vector<fscal>& StsZPos_,
               const vector<unsigned char>& SFlag_, const THitI* StsHitsStartIndex_, const THitI* StsHitsStopIndex_,
               const int NhitsGlobal

  );
  void PrintHits();

  /// The main procedure - find tracks.
  void CATrackFinder();

  /// Track fitting procedures
  void KFTrackFitter_simple();  // version, which use procedured used during the reconstruction
  void L1KFTrackFitter();       // version from SIMD-KF benchmark

  void L1KFTrackFitterMuch();

  /// ----- Input data -----
  // filled in CbmL1::ReadEvent();

  void SetNThreads(int n = 1) { fNThreads = n; }

  enum
  {
    MaxNStations = 25
  };

  int NStations,   // number of all detector stations
    NMvdStations,  // number of mvd stations
    NStsStations, NFStations;
  L1Station vStations[MaxNStations] _fvecalignment;  // station info
  vector<L1Material> fRadThick;                      // material for each station

  int NStsStrips;                    // number of strips
  const vector<fscal>* vStsZPos;     // all possible z-positions of hits
  const vector<L1StsHit>* vStsHits;  // hits as a combination of front-, backstrips and z-position
  L1Grid vGrid[MaxNStations];        // hits as a combination of front-, backstrips and z-position
  L1Grid vGridTime[MaxNStations];

  const vector<unsigned char>* vSFlag;  // information of hits station & using hits in tracks;

  double CATime;  // time of trackfinding

  L1Vector<L1Track> vTracks;
  L1Vector<THitI> vRecoHits;

  const THitI *StsHitsStartIndex,
    *StsHitsStopIndex;  // station-bounders in vStsHits array


  //  L1Branch* pointer;
  unsigned int NHitsIsecAll;
  unsigned int NTracksIsecAll;

  vector<L1StsHit> vStsDontUsedHits_A;
  vector<L1StsHit> vStsDontUsedHits_B;
  vector<L1StsHit> vStsDontUsedHits_Buf;
  vector<L1HitPoint> vStsDontUsedHitsxy_A;
  vector<L1HitPoint> vStsDontUsedHitsxy_buf;
  vector<L1HitPoint> vStsDontUsedHitsxy_B;
  L1Vector<L1Track> vTracks_local[nTh];
  L1Vector<THitI> vRecoHits_local[nTh];

  vector<THitI> RealIHit_v;
  vector<THitI> RealIHit_v_buf;
  vector<THitI> RealIHit_v_buf2;

#ifdef _OPENMP

  L1Vector<omp_lock_t> hitToBestTrackF;
  L1Vector<omp_lock_t> hitToBestTrackB;

#endif

  L1Vector<int> vStripToTrack;
  L1Vector<int> vStripToTrackB;

  int fNThreads;
  bool fUseHitErrors;
  bool fmCBMmode;
  bool fGlobal;

  fvec EventTime[nTh][nTh];
  fvec Err[nTh][nTh];


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
                 / MaxNStations  //200000,  // standart size of big arrays  // mas be 40000 for normal work in cbmroot!
  };


  /// --- data used during finding iterations

  int isec;  // iteration
  vector<L1StsHit>* vStsHitsUnused;
  vector<THitI>* RealIHitP;
  vector<THitI>* RealIHitPBuf;
  vector<L1HitPoint>* vStsHitPointsUnused;
  THitI* RealIHit;  // index in vStsHits indexed by index in vStsHitsUnused
  THitI StsHitsUnusedStartIndex[MaxNStations + 1], StsHitsUnusedStopIndex[MaxNStations + 1];
  THitI StsHitsUnusedStartIndexEnd[MaxNStations + 1], StsHitsUnusedStopIndexEnd[MaxNStations + 1];


  vector<int> TripForHit[2];


  //  fvec u_front[Portion/fvecLen], u_back[Portion/fvecLen];
  //  fvec zPos[Portion/fvecLen];
  //  fvec fHitTime[Portion/fvecLen];

  nsL1::vector<L1TrackPar>::TSimd fT_3[nTh];

  vector<THitI> fhitsl_3[nTh], fhitsm_3[nTh], fhitsr_3[nTh];

  nsL1::vector<fvec>::TSimd fu_front3[nTh], fu_back3[nTh], fz_pos3[nTh], fTimeR[nTh], fTimeER[nTh], dx[nTh], dy[nTh],
    du[nTh], dv[nTh];


  //   Tindex NHits_l[MaxNStations];
  //   Tindex NHits_l_P[MaxNStations];
  /// ----- Output data -----

  friend class CbmL1;

  const L1FieldValue& GetVtxFieldValue() const { return vtxFieldValue; }
  const L1FieldRegion& GetVtxFieldRegion() const { return vtxFieldRegion; }
  /// ----- Hit-point-strips conversion routines ------

  void GetHitCoor(const L1StsHit& _h, fscal& _x, fscal& _y, fscal& _z, const L1Station& sta);

  void dUdV_to_dY(const fvec& u, const fvec& v, fvec& _y, const L1Station& sta);

  void dUdV_to_dX(const fvec& u, const fvec& v, fvec& _x, const L1Station& sta);

  void dUdV_to_dXdY(const fvec& u, const fvec& v, fvec& _xy, const L1Station& sta);

  void GetHitCoor(const L1StsHit& _h, fscal& _x, fscal& _y, char iS);
  void StripsToCoor(const fscal& u, const fscal& v, fscal& x, fscal& y,
                    const L1Station& sta) const;  // convert strip positions to coordinates
  void StripsToCoor(const fscal& u, const fscal& v, fvec& x, fvec& y,
                    const L1Station& sta) const;  // convert strip positions to coordinates
  void StripsToCoor(const fvec& u, const fvec& v, fvec& x, fvec& y, const L1Station& sta) const;
  L1HitPoint CreateHitPoint(const L1StsHit& hit,
                            char ista);  // full the hit point by hit information.

  void CreateHitPoint(const L1StsHit& hit, char ista, L1HitPoint& point);
  inline int PackIndex(const int& a, const int& b, const int& c);

  inline int UnPackIndex(const int& i, int& a, int& b, int& c);
  /// -- Flags routines --
  inline __attribute__((always_inline)) static unsigned char GetFStation(unsigned char flag) { return flag / 4; }
  inline __attribute__((always_inline)) static bool GetFUsed(unsigned char flag) { return (flag & 0x02) != 0; }
  //   bool GetFUsedD  ( unsigned char flag ){ return (flag&0x01)!=0; }

private:
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
    Tindex n1, L1Station& stam, L1HitPoint* vStsHits_m, L1TrackPar* T_1, THitI* hitsl_1,

    // output
    Tindex& n2, vector<THitI>& i1_2,

#ifdef DOUB_PERFORMANCE
    vector<THitI>& hitsl_2,
#endif  // DOUB_PERFORMANCE
    vector<THitI>& hitsm_2, fvec* Event, vector<bool>& lmDuplets);

  /// Add the middle hits to parameters estimation. Propagate to right station.
  /// Find the triplets (right hit). Reformat data in the portion of triplets.
  void f30(  // input
    L1HitPoint* vStsHits_r, L1Station& stam, L1Station& star,

    int istam, int istar, L1HitPoint* vStsHits_m, L1TrackPar* T_1, L1FieldRegion* fld_1, THitI* hitsl_1,

    Tindex n2, vector<THitI>& hitsm_2, vector<THitI>& i1_2,

    const vector<bool>& mrDuplets,
    // output
    Tindex& n3, nsL1::vector<L1TrackPar>::TSimd& T_3, vector<THitI>& hitsl_3, vector<THitI>& hitsm_3,
    vector<THitI>& hitsr_3, nsL1::vector<fvec>::TSimd& u_front_3, nsL1::vector<fvec>::TSimd& u_back_3,
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
    Tindex n3, int istal, nsL1::vector<L1TrackPar>::TSimd& T_3, vector<THitI>& hitsl_3, vector<THitI>& hitsm_3,
    vector<THitI>& hitsr_3, int nIterations = 0);

  /// Select triplets. Save them into vTriplets.
  void f4(  // input
    Tindex n3, int istal, int istam, int istar, nsL1::vector<L1TrackPar>::TSimd& T_3, vector<THitI>& hitsl_3,
    vector<THitI>& hitsm_3, vector<THitI>& hitsr_3,
    // output
    Tindex& nstaltriplets, vector<THitI>* hitsn_3 = 0, vector<THitI>* hitsr_5 = 0

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
    int istal, int istam, Tindex ip, vector<Tindex>& n_g, Tindex* portionStopIndex_,

    // output
    L1TrackPar* T_1, L1FieldRegion* fld_1, THitI* hitsl_1,

    vector<bool>& lmDuplets,


    Tindex& n_2, vector<THitI>& i1_2, vector<THitI>& hitsm_2);

  /// Find triplets on station
  void TripletsStaPort(  // input
    int istal, int istam, int istar, Tindex& nstaltriplets, L1TrackPar* T_1, L1FieldRegion* fld_1, THitI* hitsl_1,

    Tindex& n_2, vector<THitI>& i1_2, vector<THitI>& hitsm_2,

    const vector<bool>& mrDuplets

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

  Tindex FIRSTCASTATION;  //first station used in CA

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
#else

  enum
  {
    fNFindIterations = 4
  };  // TODO investigate kAllPrimJumpIter & kAllSecJumpIter


#endif
#else
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

  map<int, int> threadNumberToCpuMap;


  float TRACK_CHI2_CUT;
  float TRIPLET_CHI2_CUT;  // = 5.0; // cut for selecting triplets before collecting tracks.per one DoF
  float DOUBLET_CHI2_CUT;
  float TIME_CUT1, TIME_CUT2;

  fvec
    MaxDZ;  // correction in order to take into account overlaping and iff z. if sort by y then it is max diff between same station's modules (~0.4cm)

  /// parameters which are different for different iterations. Set in the begin of CAL1TrackFinder

  float Pick_gather;                                // same for attaching additional hits to track
  float PickNeighbour;                              // (PickNeighbour < dp/dp_error)  =>  triplets are neighbours
  fvec MaxInvMom;                                   // max considered q/p for tracks
  fvec MaxSlope;                                    // max slope (tx\ty) in prim vertex
  fvec targX, targY, targZ;                         // target coor
  L1FieldValue targB _fvecalignment;                // field in the target point
  L1XYMeasurementInfo TargetXYInfo _fvecalignment;  // target constraint  [cm]


  L1FieldRegion vtxFieldRegion _fvecalignment;  // really doesn't used
  L1FieldValue vtxFieldValue _fvecalignment;    // field at teh vertex position.

  // vector <L1Triplet> vTriplets; // container for triplets got in finding
  // vector<L1Triplet*> vTripletsP;
  int numPortions[12];
  vector<L1Triplet*>* TripletsLocal[MaxNStations - 2];


  //  int TripNumThread;

  int fTrackingLevel, fGhostSuppression;  // really doesn't used
  float fMomentumCutOff;                  // really doesn't used

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
