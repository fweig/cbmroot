/* Copyright (C) 2010-2021 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Igor Kulakov [committer], Maksym Zyzak, Valentina Akishina */

#include "L1Algo.h"

#include "CbmL1.h"

#include "L1Grid.h"
#include "L1HitPoint.h"

L1Algo::L1Algo(unsigned int nThreads)
{
  SetNThreads(nThreads);
  for (unsigned int i = 0; i < L1Constants::size::kMaxNstations; i++) {
    vGridTime[i].AllocateMemory(fNThreads);
  }
}

void L1Algo::SetNThreads(unsigned int n)
{
  if (n > static_cast<unsigned int>(L1Constants::size::kMaxNthreads)) {
    LOG(fatal) << "L1Algo: n threads " << n << " is greater than the maximum "
               << static_cast<unsigned int>(L1Constants::size::kMaxNthreads);
  }
  fNThreads = n;

  for (int i = 0; i < fNThreads; i++) {

    fTracks_local[i].SetName(std::stringstream() << "L1Algo::fTracks_local[" << i << "]");

    fRecoHits_local[i].SetName(std::stringstream() << "L1Algo::fRecoHits_local[" << i << "]");

    fTrackCandidates[i].SetName(std::stringstream() << "L1Algo::fTrackCandidates[" << i << "]");


    fT_3[i].reserve(L1Constants::size::kMaxPortionTripletsP);

    fhitsl_3[i].SetName(std::stringstream() << "L1Algo::fhitsl_3[" << i << "]");
    fhitsm_3[i].SetName(std::stringstream() << "L1Algo::fhitsm_3[" << i << "]");
    fhitsr_3[i].SetName(std::stringstream() << "L1Algo::fhitsr_3[" << i << "]");

    fhitsl_3[i].reserve(L1Constants::size::kMaxPortionTriplets);
    fhitsm_3[i].reserve(L1Constants::size::kMaxPortionTriplets);
    fhitsr_3[i].reserve(L1Constants::size::kMaxPortionTriplets);

    fu_front3[i].reserve(L1Constants::size::kMaxPortionTripletsP);
    fu_back3[i].reserve(L1Constants::size::kMaxPortionTripletsP);
    fz_pos3[i].reserve(L1Constants::size::kMaxPortionTripletsP);
    fTimeR[i].reserve(L1Constants::size::kMaxPortionTripletsP);
    fTimeER[i].reserve(L1Constants::size::kMaxPortionTripletsP);
    dx[i].reserve(L1Constants::size::kMaxPortionTripletsP);
    dy[i].reserve(L1Constants::size::kMaxPortionTripletsP);
    du[i].reserve(L1Constants::size::kMaxPortionTripletsP);
    dv[i].reserve(L1Constants::size::kMaxPortionTripletsP);

    for (unsigned int j = 0; j < L1Constants::size::kMaxNstations; j++) {
      fTriplets[j][i].SetName(std::stringstream() << "L1Algo::fTriplets[" << i << "][" << j << "]");
    }
  }
}


void L1Algo::Init(const bool UseHitErrors, const TrackingMode mode, const bool MissingHits)
{
  for (int iProc = 0; iProc < 4; iProc++) {
    for (int i = 0; i < 8; i++) {
      threadNumberToCpuMap[2 * i + 0 + iProc * 20] = 4 * i + iProc;
      threadNumberToCpuMap[2 * i + 1 + iProc * 20] = 4 * i + 32 + iProc;
    }
    for (int i = 0; i < 2; i++) {
      threadNumberToCpuMap[2 * i + 0 + 16 + iProc * 20] = 4 * i + iProc + 64;
      threadNumberToCpuMap[2 * i + 1 + 16 + iProc * 20] = 4 * i + 8 + iProc + 64;
    }
  }

  fUseHitErrors = UseHitErrors;
  fTrackingMode = mode;
  fMissingHits  = MissingHits;


  //int NMvdStations = static_cast<int>(geo[ind++]);  // TODO: get rid of NMbdStations (S. Zh.)
  int nStationsSts     = fInitManager.GetNstationsActive(static_cast<L1DetectorID>(1));
  fNstationsBeforePipe = fInitManager.GetNstationsActive(static_cast<L1DetectorID>(0));
  //int NStsStations = static_cast<int>(geo[ind++]);  // TODO: get rid of NStsStations (S. Zh.)

  fNfieldStations = nStationsSts + fNstationsBeforePipe;  // TODO: Provide special getter for it (S.Zharko, 12.05.2022)

  if (fTrackingMode == kMcbm) { fNfieldStations = -1; }


  fInitManager.TransferParametersContainer(fParameters);
  LOG(info) << fParameters.ToString(3);

  // Get number of station
  fNstations = fParameters.GetNstationsActive();

  fTrackingLevel    = fInitManager.GetTrackingLevel();
  fGhostSuppression = fInitManager.GetGhostSuppression();
  fMomentumCutOff   = fInitManager.GetMomentumCutOff();
}

// ---------------------------------------------------------------------------------------------------------------------
//
void L1Algo::ReceiveInputData(L1InputData&& inputData)
{
  // ----- Get input data ----------------------------------------------------------------------------------------------
  fInputData = std::move(inputData);
  // TODO: Reset here internal data (probably, we should have additional class for internal data)

  // ----- Reset data arrays -------------------------------------------------------------------------------------------
  fvHitKeyFlags.reset(fInputData.GetNhitKeys());
}


///
void L1Algo::SetData(L1Vector<L1Hit>& Hits_, int nStrips_, L1Vector<unsigned char>& SFlag_,
                     const L1HitIndex_t* HitsStartIndex_, const L1HitIndex_t* HitsStopIndex_)
{
  // TODO: Remove (S.Zharko)
  vHits      = &Hits_;
  fNstrips   = nStrips_;
  fStripFlag = &SFlag_;

  HitsStartIndex = HitsStartIndex_;
  HitsStopIndex  = HitsStopIndex_;

  // TODO: maximal array sizes need to be adjusted
  // TODO: move to ReceiveInputData function (S.Zharko)
  int nHits = vHits->size();

  NHitsIsecAll = nHits; // TODO: Is it needed?

  vNotUsedHits_A.reset(nHits);
  vNotUsedHits_B.reset(nHits);
  vNotUsedHits_Buf.reset(nHits);
  vNotUsedHitsxy_A.reset(nHits);
  vNotUsedHitsxy_buf.reset(nHits);
  vNotUsedHitsxy_B.reset(nHits);
  RealIHit_v.reset(nHits);
  RealIHit_v_buf.reset(nHits);
  RealIHit_v_buf2.reset(nHits);

#ifdef _OPENMP
  fStripToTrackLock.reset(fNstrips);
  for (unsigned int j = 0; j < fStripToTrackLock.size(); j++) {
    omp_init_lock(&fStripToTrackLock[j]);
  }
#endif

  fStripToTrack.clear();
  fStripToTrack.reserve(fNstrips);

  fHitFirstTriplet.reset(nHits);
  fHitNtriplets.reset(nHits);

  fDupletPortionSize.clear();
  fDupletPortionSize.reserve(2 * nHits);

  for (int i = 0; i < fNThreads; i++) {
    fTracks_local[i].clear();
    fTracks_local[i].reserve(nHits / 10);
    fRecoHits_local[i].clear();
    fRecoHits_local[i].reserve(nHits);
    fTrackCandidates[i].clear();
    fTrackCandidates[i].reserve(nHits / 10);
    for (unsigned int j = 0; j < L1Constants::size::kMaxNstations; j++) {
      fTriplets[j][i].clear();
      fTriplets[j][i].reserve(2 * nHits);
    }
  }
}

/// TODO: Move to L1Hit
void L1Algo::GetHitCoor(const L1Hit& _h, fscal& _x, fscal& _y, char iS)
{
  const L1Station& sta = fParameters.GetStation(int(iS));
  std::tie(_x, _y)     = sta.ConvUVtoXY<fscal>(_h.u, _h.v);
  float dz             = _h.z - fParameters.GetTargetPositionZ()[0];
  _x /= dz;
  _y /= dz;
}

/// TODO: Move to L1Hit
void L1Algo::GetHitCoor(const L1Hit& _h, fscal& _x, fscal& _y, fscal& _z, const L1Station& sta)
{
  std::tie(_x, _y) = sta.ConvUVtoXY<fscal>(_h.u, _h.v);
  _z = _h.z;
}

L1HitPoint L1Algo::CreateHitPoint(const L1Hit& hit)
{
  /// full the hit point by hit information: takes hit as input (2 strips)
  /// and creates hit_point with all coordinates (x,y,z,u,v,t);
  return L1HitPoint(hit.z, hit.u, hit.v, hit.du, hit.dv, hit.t, hit.dt);
}

void L1Algo::CreateHitPoint(const L1Hit& hit, L1HitPoint& point)
{
  point.Set(hit.z, hit.u, hit.v, hit.du, hit.dv, hit.t, hit.dt);
}

int L1Algo::GetMcTrackIdForHit(int iHit)
{
  int hitId    = iHit;
  int iMcPoint = CbmL1::Instance()->GetHitMCRefs()[hitId];
  if (iMcPoint < 0) return -1;
  return CbmL1::Instance()->GetMcPoints()[iMcPoint].ID;
}

int L1Algo::GetMcTrackIdForUnusedHit(int iHit)
{
  int hitId    = (*RealIHitP)[iHit];
  int iMcPoint = CbmL1::Instance()->GetHitMCRefs()[hitId];
  if (iMcPoint < 0) return -1;
  return CbmL1::Instance()->GetMcPoints()[iMcPoint].ID;
}


//   bool L1Algo::SortTrip(TripSort const& a, TripSort const& b) {
//       return   ( a.trip.GetLevel() >  b.trip.GetLevel() );
// }
//
// bool L1Algo::SortCand(CandSort const& a, CandSort const& b) {
//     if (a.cand.Lengtha != b.cand.Lengtha) return (a.cand.Lengtha > b.cand.Lengtha);
//
//     if (a.cand.ista != b.cand.ista ) return (a.cand.ista  < b.cand.ista );
//
//     if (a.cand.chi2  != b.cand.chi2 )return (a.cand.chi2  < b.cand.chi2 );
//     //return (a->chi2  < b->chi2 );
//     //   return (a->CandIndex < b->CandIndex );
//    // return (a.cand.CandIndex > b.cand.CandIndex );
// }

//   inline int L1Algo::PackIndex(const int& a, const int& b, const int& c) {
//       return   (a) + ((b)*10000) + (c*100000000);
// }
//
//   inline int L1Algo::UnPackIndex(const int& i, int& a, int& b, int& c) {
