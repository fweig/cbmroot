/* Copyright (C) 2010-2021 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Igor Kulakov [committer], Maksym Zyzak, Valentina Akishina */

#include "L1Algo.h"

#include "L1Grid.h"
#include "L1HitPoint.h"

L1Algo::L1Algo(unsigned int nThreads)
{
  SetNThreads(nThreads);
  for (unsigned int i = 0; i < L1Parameters::kMaxNstations; i++) {
    vGridTime[i].AllocateMemory(fNThreads);
  }
}

void L1Algo::SetNThreads(unsigned int n)
{
  if (n > static_cast<unsigned int>(L1Parameters::kMaxNthreads)) {
    LOG(fatal) << "L1Algo: n threads " << n << " is greater than the maximum "
               << static_cast<unsigned int>(L1Parameters::kMaxNthreads);
  }
  fNThreads = n;

  for (int i = 0; i < fNThreads; i++) {

    fTracks_local[i].SetName(std::stringstream() << "L1Algo::fTracks_local[" << i << "]");

    fRecoHits_local[i].SetName(std::stringstream() << "L1Algo::fRecoHits_local[" << i << "]");

    fTrackCandidates[i].SetName(std::stringstream() << "L1Algo::fTrackCandidates[" << i << "]");


    fT_3[i].reserve(MaxPortionTriplets / fvecLen);

    fhitsl_3[i].SetName(std::stringstream() << "L1Algo::fhitsl_3[" << i << "]");
    fhitsm_3[i].SetName(std::stringstream() << "L1Algo::fhitsm_3[" << i << "]");
    fhitsr_3[i].SetName(std::stringstream() << "L1Algo::fhitsr_3[" << i << "]");

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

    for (unsigned int j = 0; j < L1Parameters::kMaxNstations; j++) {
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
  int nStationsSts     = fInitManager.GetStationsNumber(static_cast<L1DetectorID>(1));
  fNstationsBeforePipe = fInitManager.GetStationsNumber(static_cast<L1DetectorID>(0));
  //int NStsStations = static_cast<int>(geo[ind++]);  // TODO: get rid of NStsStations (S. Zh.)

  fNfieldStations = nStationsSts + fNstationsBeforePipe; // TODO: Provide special getter for it (S.Zharko, 12.05.2022)

  if (fTrackingMode == kMcbm) { fNfieldStations = -1; }


  // Final init checks (the function provides purity of fields initialization and turns on the last bit of
  // the L1ObjectInitController):
  fInitManager.CheckInit();  // NOTE: after passing this frontier L1Algo is (will be) accounted as initialized

  // Print out the bits of the init manager
  LOG(info) << "InitManager " << fInitManager.GetInitController().ToString();
 
  // Get real target position
  fRealTargetX = fInitManager.GetTargetPosition()[0];
  fRealTargetY = fInitManager.GetTargetPosition()[1];
  fRealTargetZ = fInitManager.GetTargetPosition()[2];

  // Get number of station
  fNstations = fInitManager.GetStationsNumber();

  // Get field near target
  fVtxFieldValue   = fInitManager.GetTargetFieldValue();
  fVtxFieldRegion = fInitManager.GetTargetFieldRegion();

  // Fill L1Station array
  fInitManager.TransferL1StationArray(fStations);
  
  fTrackingLevel = fInitManager.GetTrackingLevel();
  fGhostSuppression = fInitManager.GetGhostSuppression();
  fMomentumCutOff = fInitManager.GetMomentumCutOff();

  LOG(info) << "  ***********************";
  LOG(info) << "  *  L1Algo parameters  *";
  LOG(info) << "  ***********************";
  LOG(info) << "";

  LOG(info) << "----- Nominal target position -----";
  LOG(info) << "\t target X = " << fRealTargetX;
  LOG(info) << "\t target Y = " << fRealTargetY;
  LOG(info) << "\t target Z = " << fRealTargetZ;

  LOG(info) << "----- Number of stations -----";
  LOG(info) << "\tTotal stations:           " << fNstations;
  LOG(info) << "\tStations before pipe:     " << fNstationsBeforePipe;
  LOG(info) << "\tStations in field region: " << fNfieldStations;

  LOG(info) << "----- Magnetic field near target -----";
  LOG(info) << "\tField Value:  " << '\n' << fVtxFieldValue.ToString(/*indent = */ 1) << '\n';
  LOG(info) << "\tField Region: " << '\n' << fVtxFieldRegion.ToString(/*indent = */ 1) << '\n';

  LOG(info) << "----- L1 active stations list -----";
  for (int iSt = 0; iSt < fNstations; ++iSt) {
    LOG(info) << " #" << iSt << " " << fStations[iSt].ToString(/*verbosity = */ 0);
  }

  // Print L1Parameters
  fParameters.Print(/*verbosity=*/0);
}


void L1Algo::SetData(L1Vector<L1Hit>& StsHits_, int nStsStrips_, L1Vector<unsigned char>& SFlag_,
                     const L1HitIndex_t* StsHitsStartIndex_, const L1HitIndex_t* StsHitsStopIndex_)
{

  vStsHits   = &StsHits_;
  NStsStrips = nStsStrips_;
  fStripFlag = &SFlag_;

  StsHitsStartIndex = StsHitsStartIndex_;
  StsHitsStopIndex  = StsHitsStopIndex_;

  // TODO: maximal array sizes need to be adjusted

  int nHits = vStsHits->size();

  NHitsIsecAll = nHits;

  vStsDontUsedHits_A.reset(nHits);
  vStsDontUsedHits_B.reset(nHits);
  vStsDontUsedHits_Buf.reset(nHits);
  vStsDontUsedHitsxy_A.reset(nHits);
  vStsDontUsedHitsxy_buf.reset(nHits);
  vStsDontUsedHitsxy_B.reset(nHits);
  RealIHit_v.reset(nHits);
  RealIHit_v_buf.reset(nHits);
  RealIHit_v_buf2.reset(nHits);

#ifdef _OPENMP
  fHitToBestTrackF.reset(NStsStrips);
  fHitToBestTrackB.reset(NStsStrips);
  for (unsigned int j = 0; j < fHitToBestTrackB.size(); j++) {
    omp_init_lock(&fHitToBestTrackB[j]);
    omp_init_lock(&fHitToBestTrackF[j]);
  }
#endif

  fStripToTrack.clear();
  fStripToTrack.reserve(NStsStrips);

  fStripToTrackB.clear();
  fStripToTrackB.reserve(NStsStrips);

  TripForHit[0].reset(nHits);
  TripForHit[1].reset(nHits);

  fDupletPortionSize.clear();
  fDupletPortionSize.reserve(2 * nHits);

  for (int i = 0; i < fNThreads; i++) {
    fTracks_local[i].clear();
    fTracks_local[i].reserve(nHits / 10);
    fRecoHits_local[i].clear();
    fRecoHits_local[i].reserve(nHits);
    fTrackCandidates[i].clear();
    fTrackCandidates[i].reserve(nHits / 10);
    for (unsigned int j = 0; j < L1Parameters::kMaxNstations; j++) {
      fTriplets[j][i].clear();
      fTriplets[j][i].reserve(2 * nHits);
    }
  }
}


void L1Algo::GetHitCoor(const L1Hit& _h, fscal& _x, fscal& _y, char iS)
{
  L1Station& sta = fStations[int(iS)];
  fscal u        = _h.u;
  fscal v        = _h.v;
  _x             = (sta.xInfo.sin_phi[0] * u + sta.xInfo.cos_phi[0] * v) / (_h.z - fRealTargetZ[0]);
  _y             = (sta.yInfo.cos_phi[0] * u + sta.yInfo.sin_phi[0] * v) / (_h.z - fRealTargetZ[0]);
}

void L1Algo::GetHitCoor(const L1Hit& _h, fscal& _x, fscal& _y, fscal& _z, const L1Station& sta)
{
  fscal u = _h.u;
  fscal v = _h.v;
  fscal x, y;
  StripsToCoor(u, v, x, y, sta);
  _x = x;
  _y = y;
  _z = _h.z;
}

void L1Algo::StripsToCoor(
  const fscal& u, const fscal& v, fscal& _x, fscal& _y,
  const L1Station& sta) const  // TODO: Actually sta.yInfo.sin_phi is same for all stations, so ...
{
  //  fvec x,y;
  //  StripsToCoor(u,v,x,y,sta);
  //  _x = x[0];
  //  _y = y[0];
  _x = sta.xInfo.sin_phi[0] * u + sta.xInfo.cos_phi[0] * v;
  _y = sta.yInfo.cos_phi[0] * u + sta.yInfo.sin_phi[0] * v;
}
/// convert strip positions to coordinates
void L1Algo::StripsToCoor(
  const fscal& u, const fscal& v, fvec& _x, fvec& _y,
  const L1Station& sta) const  // TODO: Actually sta.yInfo.sin_phi is same for all stations, so ...
{
  //  fvec x,y;
  //  StripsToCoor(u,v,x,y,sta);
  //  _x = x[0];
  //  _y = y[0];
  _x = sta.xInfo.sin_phi * u + sta.xInfo.cos_phi * v;
  _y = sta.yInfo.cos_phi * u + sta.yInfo.sin_phi * v;
}

void L1Algo::dUdV_to_dY(const fvec& u, const fvec& v, fvec& _y, const L1Station& sta)
{
  _y = sqrt((sta.yInfo.cos_phi * u) * (sta.yInfo.cos_phi * u) + (sta.yInfo.sin_phi * v) * (sta.yInfo.sin_phi * v));
}

void L1Algo::dUdV_to_dX(const fvec& u, const fvec& v, fvec& _x, const L1Station& sta)
{
  _x = sqrt((sta.xInfo.sin_phi * u) * (sta.xInfo.sin_phi * u) + (sta.xInfo.cos_phi * v) * (sta.xInfo.cos_phi * v));
}

void L1Algo::dUdV_to_dXdY(const fvec& u, const fvec& v, fvec& _xy, const L1Station& sta)
{
  _xy = ((sta.xInfo.sin_phi * u) * (sta.yInfo.cos_phi * u) + (sta.xInfo.cos_phi * v) * (sta.yInfo.sin_phi * v));
}

void L1Algo::StripsToCoor(
  const fvec& u, const fvec& v, fvec& x, fvec& y,
  const L1Station& sta) const  // TODO: Actually sta.yInfo.sin_phi is same for all stations, so ...
{
  // only for same-z
  //   x = u;
  //   y = (sta.yInfo.cos_phi*u + sta.yInfo.sin_phi*v);
  x = sta.xInfo.sin_phi * u + sta.xInfo.cos_phi * v;
  y = sta.yInfo.cos_phi * u + sta.yInfo.sin_phi * v;
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
