/* Copyright (C) 2010-2017 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Igor Kulakov [committer] */

#ifndef L1AlgoEfficiencyPerformance_h
#define L1AlgoEfficiencyPerformance_h

/**
*   Class for tracklets reconstruction efficiency calculation. Used for debug of CATrackFinder.
*
*   Reconstructed tracklet is one, which has all 3 hits from one mcTrack.
*   Reconstructable mcTracklet is one, which has all 3 hits and momentum bigger than fL1->MinRecoMom.
*/

#include "CbmL1.h"
#include "CbmL1Counters.h"

#include <iostream>
#include <vector>

#include "L1Algo.h"
#include "L1Def.h"

using std::cout;
using std::endl;
using std::vector;


template<int NHits>
class L1Tracklet {
public:
  L1Tracklet() = default;

  static bool compare(const L1Tracklet<NHits>& a, const L1Tracklet<NHits>& b)
  {
    return (a.mcTrackId < b.mcTrackId) || ((a.mcTrackId == b.mcTrackId) && (a.iStation < b.iStation));
  }

  bool operator==(const L1Tracklet<NHits>& a) { return ((a.mcTrackId == mcTrackId) && (a.iStation == iStation)); }
  bool operator!=(const L1Tracklet<NHits>& a) { return !operator==(a); }
  bool operator<(const L1Tracklet<NHits>& a) { return compare(*this, a); }

  int mcTrackId {-1};
  int iStation {-1};
};

// reconstructed Tracklet
template<int NHits>
class L1RTracklet : public L1Tracklet<NHits> {
public:
  L1RTracklet() = default;
  L1RTracklet(L1HitIndex_t* ih_) : L1Tracklet<NHits>()
  {
    for (int iih = 0; iih < NHits; iih++) {
      i[iih] = ih_[iih];
    }
  };

  bool IsGhost() { return L1Tracklet<NHits>::mcTrackId < 0; };  // is it ghost or reconstructed

  bool operator!=(const L1Tracklet<NHits>& a) { return L1Tracklet<NHits>::operator!=(a); }

  bool operator==(const L1RTracklet<NHits>& a)
  {
    bool flag = 1;
    //     flag &= L1Tracklet<NHits>::operator==(a);
    for (int ih = 0; ih < NHits; ih++) {
      flag &= (a.i[ih] == i[ih]);
    }
    return flag;
  }
  bool operator!=(const L1RTracklet<NHits>& a) { return !operator==(a); }

  L1HitIndex_t i[NHits] {0};  // indices of left, middle and right hits
};

// MC Tracklet
template<int NHits>
class L1MTracklet : public L1Tracklet<NHits> {
public:
  L1MTracklet() = default;

  void AddReconstructed(int recoId) { recoIds.push_back(recoId); };
  void SetAsReconstructable() { isReconstructable = true; };

  int GetNClones() { return (recoIds.size() > 1) ? recoIds.size() - 1 : 0; };  // number of created clones
  bool IsReconstructed() { return recoIds.size(); };                           // is it reconstructed
  bool IsReconstructable() { return isReconstructable; };                      // is it possible to reconstruct
  bool IsPrimary() { return mcMotherTrackId < 0; };                            // is it primary or secondary

  int mcMotherTrackId {-1};
  float p {0.f};           // momentum
  vector<int> recoIds {};  // indices of reco tracklets in sorted recoArray

  vector<int> hitIds[NHits] {};  // indices of hits in L1::vHits. Separated by stations
private:
  bool isReconstructable {false};  // is it reconstructed
};

// tracklets categories
struct TL1AlgoEfficiencies : public TL1Efficiencies {
  TL1AlgoEfficiencies() : TL1Efficiencies()
  {
    AddCounter("total", "Allset    efficiency");
    AddCounter("fast", "Refset    efficiency");
    AddCounter("fast_prim", "RefPrim   efficiency");
    AddCounter("fast_sec", "RefSec    efficiency");
    AddCounter("slow", "Extra     efficiency");
    AddCounter("slow_prim", "ExtraPrim efficiency");
    AddCounter("slow_sec", "ExtraSec  efficiency");
  }
};


template<int NHits = 3>
class L1AlgoEfficiencyPerformance {
public:
  typedef L1RTracklet<NHits> L1RecoTracklet;
  typedef L1MTracklet<NHits> L1MCTracklet;
  L1AlgoEfficiencyPerformance() = default;
  void Init();

  bool AddOne(L1HitIndex_t* ih_);  // add one recoTracklets. Return is it reconstructed or not
  void CalculateEff();
  void Print();  // Print efficiencies
  void Print(TString title = "Triplets performance statistic",
             bool station  = 0);  // Print efficiencies

  static constexpr int MaxNStations = 10;

private:
  L1AlgoEfficiencyPerformance<NHits>(const L1AlgoEfficiencyPerformance<NHits>&);
  L1AlgoEfficiencyPerformance<NHits>& operator=(const L1AlgoEfficiencyPerformance<NHits>&);

  void FillMC();  // collect mcTracklets from mcTracks
  void MatchTracks();

  CbmL1* fL1 {nullptr};  // TODO: remove dependency (S.Zharko)

  vector<L1RecoTracklet> recoTracklets {};
  vector<L1MCTracklet> mcTracklets {};

  TL1AlgoEfficiencies ntra {};  // current event efficiencies
  TL1AlgoEfficiencies NTRA {};  // efficiencies statistic

  TL1AlgoEfficiencies ntra_sta[MaxNStations] {};  // for each station separately
  TL1AlgoEfficiencies NTRA_STA[MaxNStations] {};
};

// ===================================================================================

template<int NHits>
void L1AlgoEfficiencyPerformance<NHits>::Init()
{
  fL1 = CbmL1::Instance();
  recoTracklets.clear();
  mcTracklets.clear();
  ntra = TL1AlgoEfficiencies();
  for (int iSta = 0; iSta < MaxNStations; iSta++) {
    ntra_sta[iSta] = ntra;
  }

  FillMC();
};

template<int NHits>
void L1AlgoEfficiencyPerformance<NHits>::FillMC()
{
  for (vector<CbmL1MCTrack>::iterator mtraIt = fL1->fvMCTracks.begin(); mtraIt != fL1->fvMCTracks.end(); mtraIt++) {
    CbmL1MCTrack& mtra = *(mtraIt);
    //     if( ! mtra.IsReconstructable() ) continue;

    const int NMCPoints = mtra.Points.size();
    //    const int NIter = mtra.NStations()-NHits+1; // number of possible tracklets
    int lastIterSta = -1;
    for (int iterOffset = 0; iterOffset < NMCPoints; iterOffset++) {  // first mcPoint on the station
      //      const int iterMcId = mtra.Points[iterOffset];
      int iterSta = fL1->fvMCPoints[mtra.Points[iterOffset]].iStation;
      if (iterSta == lastIterSta) continue;  // find offset for next station
      lastIterSta = iterSta;

      // read hits
      // take all points on each station
      // TODO: Should we create all possible tracklets?
      L1MCTracklet trlet;  // TODO: don't use hits in mcTracklet
      for (int is = 0, offset = iterOffset; ((offset < NMCPoints) && (is < NHits)); offset++) {
        const int mcId     = mtra.Points[offset];
        CbmL1MCPoint* mcPs = &(fL1->fvMCPoints[mcId]);
        is                 = mcPs->iStation - iterSta;

        if (is < NHits) {
          const int NPointHits = mcPs->hitIds.size();
          for (int ih = 0; ih < NPointHits; ih++) {
            trlet.hitIds[is].push_back(mcPs->hitIds[ih]);
          }
        }
      }  // for is
      // check tracklet
      bool good = 1;
      for (int is = 0; is < NHits; is++) {
        good &= trlet.hitIds[is].size();
      }
      if (!good) continue;  // can't create tracklet

      trlet.iStation        = iterSta;
      trlet.mcTrackId       = mtra.ID;
      trlet.mcMotherTrackId = mtra.mother_ID;
      trlet.p               = mtra.p;
      if (mtra.p > 1. / fL1->fpAlgo->GetMaxInvMom()) trlet.SetAsReconstructable();

      mcTracklets.push_back(trlet);
    }  // for Iter = stations
  }    // for mcTracks
}  // void L1AlgoEfficiencyPerformance::FillMC()


template<int NHits>
bool L1AlgoEfficiencyPerformance<NHits>::AddOne(L1HitIndex_t* iHits)
{
  L1RecoTracklet trlet(iHits);

  // --- check is all hits belong to one mcTrack ---
  vector<int> mcIds[NHits];

  // obtain mc data
  for (int iih = 0; iih < NHits; iih++) {
    const int iMCP = fL1->fvHitDebugInfo[iHits[iih]].GetMCPointIndex();
    if (iMCP > -1) {
      int mcId       = fL1->fvMCPoints[iMCP].ID;
      mcIds[iih].push_back(mcId);
    }
  }    // for hits

  // find all reconstructed track id-s
  for (int level = 0; level < NHits - 1; level++) {
    vector<int>& mcs1 = mcIds[level];
    vector<int>& mcs2 = mcIds[level + 1];

    // leave only matched ID-s.
    for (unsigned int i2 = 0; i2 < mcs2.size(); i2++) {
      int& mc2  = mcs2[i2];
      bool flag = 0;
      // is the same ID on prev station?
      for (unsigned int i1 = 0; i1 < mcs1.size(); i1++) {
        flag |= (mcs1[i1] == mc2);
      }
      if (!flag) mc2 = -2;
    }  // for i2
  }

  // use first found // TODO: save all!!!
  // UPD: SZh 24.05.2023: We save only one point index (the best matched)
  vector<int>& mcsN = mcIds[NHits - 1];
  for (unsigned int i = 0; i < mcsN.size(); i++) {
    if (mcsN[i] >= 0) {
      trlet.mcTrackId = mcsN[i];
      int iMCP        = fL1->fvHitDebugInfo[iHits[0]].GetMCPointIndex();
      assert(iMCP > -1);
      trlet.iStation = fL1->fvMCPoints[fL1->fvHitDebugInfo[iHits[0]].GetMCPointIndex()].iStation;
      break;
    }
  }

  // --- save ---
  //   if ( std::find(recoTracklets.begin(), recoTracklets.end(), trlet) == recoTracklets.end() )
  recoTracklets.push_back(trlet);

  return (trlet.mcTrackId >= 0);
};  // inline bool L1AlgoEfficiencyPerformance::AddOne(L1HitIndex_t ihl, L1HitIndex_t ihm, L1HitIndex_t ihr)


template<int NHits>
void L1AlgoEfficiencyPerformance<NHits>::MatchTracks()
{  // TODO: If we create all possible tracklets on NHits station - addapt to it.
  std::sort(recoTracklets.begin(), recoTracklets.end(), L1Tracklet<NHits>::compare);
  std::sort(mcTracklets.begin(), mcTracklets.end(), L1Tracklet<NHits>::compare);

  const int NRecoTrlets = recoTracklets.size();
  const int NMCTrlets   = mcTracklets.size();
  //   cout << NMCTrlets <<  " " << NRecoTrlets <<  " " << endl;
  for (int iReco = 0, iMC = 0; (iReco < NRecoTrlets) && (iMC < NMCTrlets);) {
    L1MCTracklet& mcTrlet     = mcTracklets[iMC];
    L1RecoTracklet& recoTrlet = recoTracklets[iReco];

    //     if (recoTrlet.mcTrackId >= 0)
    //       cout << iMC << " " << mcTrlet.iStation << " "  << mcTrlet.mcTrackId << " "  << mcTrlet.p  << " | "
    //       << iReco << " " << recoTrlet.iStation << " "  << recoTrlet.mcTrackId << " " << endl;

    if (recoTrlet != mcTrlet) {
      if (recoTrlet < mcTrlet) iReco++;
      else
        iMC++;
    }
    else {
      //       cout << iMC << " " << mcTrlet.iStation << " "  << mcTrlet.mcTrackId << " "  << mcTrlet.p  << " | "
      //            << iReco << " " << recoTrlet.iStation << " "  << recoTrlet.mcTrackId << " " << endl;
      // check if same tracklet has been matched, and save it if not.
      bool flag       = 1;
      const int nReco = mcTrlet.recoIds.size();
      for (int iR = 0; iR < nReco; iR++) {
        flag &= (recoTrlet != recoTracklets[mcTrlet.recoIds[iR]]);
      }
      if (flag) mcTrlet.AddReconstructed(iReco);

      iReco++;  // suppose we have only one tracklet with certain (Id,Station)
    }
  };
}  // void L1AlgoEfficiencyPerformance::MatchTracks()

template<int NHits>
void L1AlgoEfficiencyPerformance<NHits>::CalculateEff()
{
  MatchTracks();

  const int NRecoTrlets = recoTracklets.size();
  for (int iReco = 0; iReco < NRecoTrlets; iReco++) {
    L1RecoTracklet& reco = recoTracklets[iReco];
    ntra /*_sta[reco.iStation]*/.ghosts += reco.IsGhost();
  }

  const int NMCTrlets = mcTracklets.size();
  for (int iMC = 0; iMC < NMCTrlets; iMC++) {
    L1MCTracklet& mtra = mcTracklets[iMC];
    if (!mtra.IsReconstructable()) continue;
    int iSta = mtra.iStation;

    // find used constans
    const bool reco = mtra.IsReconstructed();

    // count tracklets
    ntra_sta[iSta].Inc(reco, "total");

    if (mtra.p > 1.) {  // reference tracks
      ntra_sta[iSta].Inc(reco, "fast");
      if (mtra.IsPrimary()) {  // reference primary
        ntra_sta[iSta].Inc(reco, "fast_prim");

        //               // Debug
        //         if (!reco){
        //           cout << "iMC/N " << iMC << "/" << NMCTrlets << " sta " << mtra.iStation << " trackId " << mtra.mcTrackId << " momentum " << mtra.p << ". MotherTrackId " << mtra.mcMotherTrackId << endl;
        //           for (int iSta2 = 0; iSta2 < NHits; iSta2++){
        //             cout << iSta2 << ": ";
        //             const int NPointHits = mtra.hitIds[iSta2].size();
        //             for (int iH = 0; iH < NPointHits; iH++){
        //               cout << mtra.hitIds[iSta2][iH] << "";
        //               cout << "(" << fL1->fvHitDebugInfo[mtra.hitIds[iSta2][iH]].x << "\\" << fL1->fvHitDebugInfo[mtra.hitIds[iSta2][iH]].y << "= " << fL1->fvHitDebugInfo[mtra.hitIds[iSta2][iH]].x/fL1->fvHitDebugInfo[mtra.hitIds[iSta2][iH]].y << " ) ";
        //             }
        //             cout << endl;
        //           }
        //         }
      }
      else {  // reference secondary
        ntra_sta[iSta].Inc(reco, "fast_sec");
      }
    }
    else {  // extra set of tracks
      ntra_sta[iSta].Inc(reco, "slow");
      if (mtra.IsPrimary()) {  // extra primary
        ntra_sta[iSta].Inc(reco, "slow_prim");
      }
      else {
        ntra_sta[iSta].Inc(reco, "slow_sec");
      }
    }                                            // if extra
    ntra_sta[iSta].clones += mtra.GetNClones();  // TODO:check if works
  };                                             //for mcTracklets

  for (int iSta = 0; iSta < MaxNStations; iSta++) {
    ntra += ntra_sta[iSta];
    NTRA_STA[iSta] += ntra_sta[iSta];
    ntra_sta[iSta].CalcEff();
    NTRA_STA[iSta].CalcEff();
  }
  NTRA += ntra;
  ntra.CalcEff();
  NTRA.CalcEff();
}  // void L1AlgoEfficiencyPerformance::CalculateEff()


template<int NHits>
inline void L1AlgoEfficiencyPerformance<NHits>::Print(TString title, bool stations)
{
  //   cout << endl;
  //   cout << "-------- Triplets performance ----------" << endl;
  //   ntra.PrintEff();

  cout << endl;
  cout << "-------- " << title << " ----------" << endl;
  NTRA.PrintEff();
  cout << endl;

  if (stations) {
    for (int iSta = 0; iSta < fL1->fNStations - NHits + 1; iSta++) {
      TString title_sta = title;
      title_sta += " station ";
      title_sta += iSta;
      cout << endl;
      cout << "-------- " << title_sta << " ----------" << endl;
      NTRA_STA[iSta].PrintEff();
      cout << endl;
    }
  }
};


#endif
