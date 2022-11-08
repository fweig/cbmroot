/* Copyright (C) 2010-2021 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Igor Kulakov [committer], Maksym Zyzak, Valentina Akishina */

#include "L1Algo.h"

#include "CbmL1.h"

#include "CaToolsDebugger.h"
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


    fTripletPar[i].reserve(L1Constants::size::kMaxPortionTripletsP);

    fTripletHitsL[i].SetName(std::stringstream() << "L1Algo::fhitsl_3[" << i << "]");
    fTripletHitsM[i].SetName(std::stringstream() << "L1Algo::fhitsm_3[" << i << "]");
    fTripletHitsR[i].SetName(std::stringstream() << "L1Algo::fhitsr_3[" << i << "]");

    fTripletHitsL[i].reserve(L1Constants::size::kMaxPortionTriplets);
    fTripletHitsM[i].reserve(L1Constants::size::kMaxPortionTriplets);
    fTripletHitsR[i].reserve(L1Constants::size::kMaxPortionTriplets);

    fTripletHitR_Ufront[i].reserve(L1Constants::size::kMaxPortionTripletsP);
    fTripletHitR_Uback[i].reserve(L1Constants::size::kMaxPortionTripletsP);
    fTripletHitR_Z[i].reserve(L1Constants::size::kMaxPortionTripletsP);
    fTripletHitR_Time[i].reserve(L1Constants::size::kMaxPortionTripletsP);
    fTripletHitR_TimeErr[i].reserve(L1Constants::size::kMaxPortionTripletsP);
    fTripletHitR_dUfront[i].reserve(L1Constants::size::kMaxPortionTripletsP);
    fTripletHitR_dUback[i].reserve(L1Constants::size::kMaxPortionTripletsP);

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
}

void L1Algo::Finish() { ca::tools::Debugger::Instance().Write(); }

// ---------------------------------------------------------------------------------------------------------------------
//
void L1Algo::ReceiveInputData(L1InputData&& inputData)
{
  // ----- Get input data ----------------------------------------------------------------------------------------------
  fInputData = std::move(inputData);

  // ----- Reset data arrays -------------------------------------------------------------------------------------------
  fvHitKeyFlags.reset(fInputData.GetNhitKeys());
  int nHits    = fInputData.GetNhits();
  NHitsIsecAll = nHits;
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
  fStripToTrackLock.reset(fInputData.GetNhitKeys());
  for (unsigned int j = 0; j < fStripToTrackLock.size(); j++) {
    omp_init_lock(&fStripToTrackLock[j]);
  }
#endif

  fStripToTrack.clear();
  fStripToTrack.reserve(fInputData.GetNhitKeys());

  fHitFirstTriplet.reset(nHits);
  fHitNtriplets.reset(nHits);

  for (int i = 0; i < L1Constants::size::kMaxNstations; i++) {
    fSingletPortionSize[i].clear();
    fSingletPortionSize[i].reserve(2 * fInputData.GetNhits(i));
  }

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

// ---------------------------------------------------------------------------------------------------------------------
//
void L1Algo::ReceiveParameters(L1Parameters&& parameters)
{
  fParameters = std::move(parameters);

  fNstationsBeforePipe = fParameters.GetNstationsActive(static_cast<L1DetectorID>(0));

  // FIXME: SZh 24.08.2022
  //        This approach is suitable only for a case, when all the stations inside a magnetic field come right before
  //        all the stations outside of the field!
  fNfieldStations = std::lower_bound(fParameters.GetStations().cbegin(),
                                     fParameters.GetStations().cbegin() + fParameters.GetNstationsActive(),
                                     0,  // we are looking for the first zero element
                                     [](const L1Station& s, int edge) { return bool(s.fieldStatus) > edge; })
                    - fParameters.GetStations().cbegin();

  fTrackingLevel    = fParameters.GetTrackingLevel();
  fGhostSuppression = fParameters.GetGhostSuppression();
  fMomentumCutOff   = fParameters.GetMomentumCutOff();

  L1FieldRegion::ForceUseOfOriginalField(fParameters.DevIsUseOfOriginalField());

  LOG(info) << fParameters.ToString(3);
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
  _z               = _h.z;
}

L1HitPoint L1Algo::CreateHitPoint(const L1Hit& hit)
{
  /// full the hit point by hit information: takes hit as input (2 strips)
  /// and creates hit_point with all coordinates (x,y,z,u,v,t);
  return L1HitPoint(hit.z, hit.u, hit.v, hit.t, hit.du2, hit.dv2, hit.dt2);
}

void L1Algo::CreateHitPoint(const L1Hit& hit, L1HitPoint& point)
{
  point.Set(hit.z, hit.u, hit.v, hit.t, hit.du2, hit.dv2, hit.dt2);
}

int L1Algo::GetMcTrackIdForHit(int iHit) const
{
  int hitId    = iHit;
  int iMcPoint = CbmL1::Instance()->GetHitMCRefs()[hitId];
  if (iMcPoint < 0) return -1;
  return CbmL1::Instance()->GetMcPoints()[iMcPoint].ID;
}

int L1Algo::GetMcTrackIdForUnusedHit(int iHit) const
{
  int hitId    = (*RealIHitP)[iHit];
  int iMcPoint = CbmL1::Instance()->GetHitMCRefs()[hitId];
  if (iMcPoint < 0) return -1;
  return CbmL1::Instance()->GetMcPoints()[iMcPoint].ID;
}

const CbmL1MCTrack* L1Algo::GetMcTrackForUnusedHit(int iHit) const
{
  int id = GetMcTrackIdForUnusedHit(iHit);
  if (id < 0) return nullptr;
  return &CbmL1::Instance()->GetMcTracks()[id];
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
