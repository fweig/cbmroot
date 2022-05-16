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


void L1Algo::Init(const L1Vector<fscal>& geo, const bool UseHitErrors, const TrackingMode mode, const bool MissingHits)
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

  //lxir039
  //  for (int i=0; i<8; i++){
  //     threadNumberToCpuMap[2*i+0] = 15-i;
  //     threadNumberToCpuMap[2*i+1] = 15-(i+8);
  //   }

  int ind = 0;
  {
    L1FieldValue B[3];
    fvec z[3];
    for (int i = 0; i < 3; i++) {
      z[i]   = geo[ind++];
      B[i].x = geo[ind++];
      B[i].y = geo[ind++];
      B[i].z = geo[ind++];
#ifndef TBB2
      std::cout << "L1Algo Input Magnetic field:" << z[i][0] << " " << B[i].x[0] << " " << B[i].y[0] << " " << B[i].z[0]
                << std::endl;
#endif  // TBB2
    }
    vtxFieldRegion.Set(B[0], z[0], B[1], z[1], B[2], z[2]);
    vtxFieldValue = B[0];
  }
  //vStations.clear();
  NStations    = static_cast<int>(geo[ind++]);
  NMvdStations = static_cast<int>(geo[ind++]);  // TODO: get rid of NMbdStations (S. Zh.)
  NStsStations = static_cast<int>(geo[ind++]);  // TODO: get rid of NStsStations (S. Zh.)

  fNfieldStations = NStsStations + NMvdStations;

  if (fTrackingMode == kMcbm) { fNfieldStations = -1; }


  // cout << "N MVD & STS stations: " << NMvdStations << " " << NStations-NMvdStations << endl;
#ifndef TBB2
  std::cout << "L1Algo Input " << NStations << " Stations:" << std::endl;
#endif  // TBB2
  for (int i = 0; i < NStations; i++) {
    L1Station& st = vStations[i];
    st.type       = geo[ind++];
    st.timeInfo   = 1;
    if (st.type == 1) st.timeInfo = 0;
    st.z                        = geo[ind++];
    st.materialInfo.thick       = geo[ind++];
    st.Rmin                     = geo[ind++];
    st.Rmax                     = geo[ind++];
    st.materialInfo.RL          = geo[ind++];
    st.materialInfo.RadThick    = st.materialInfo.thick / st.materialInfo.RL;
    st.materialInfo.logRadThick = log(st.materialInfo.RadThick);

    double f_phi   = geo[ind++];
    double f_sigma = geo[ind++];
    double b_phi   = geo[ind++];
    double b_sigma = geo[ind++];
    double dt      = geo[ind++];  //TODO: Add this field to L1BaseStationInfo and to ToString fcn (S.Zharko)
    double c_f     = cos(f_phi);
    double s_f     = sin(f_phi);
    double c_b     = cos(b_phi);
    double s_b     = sin(b_phi);

    st.frontInfo.cos_phi = c_f;
    st.frontInfo.sin_phi = s_f;
    st.frontInfo.sigma2  = f_sigma * f_sigma;

    st.backInfo.cos_phi = c_b;
    st.backInfo.sin_phi = s_b;
    st.backInfo.sigma2  = b_sigma * b_sigma;
    st.dt               = dt;

    if (fabs(b_phi - f_phi) < .1) {
      double th  = b_phi - f_phi;
      double det = cos(th);
      det *= det;
      st.XYInfo.C00 = (s_b * s_b * f_sigma * f_sigma + s_f * s_f * b_sigma * b_sigma) / det;
      st.XYInfo.C10 = -(s_b * c_b * f_sigma * f_sigma + s_f * c_f * b_sigma * b_sigma) / det;
      st.XYInfo.C11 = (c_b * c_b * f_sigma * f_sigma + c_f * c_f * b_sigma * b_sigma) / det;
      //std::cout << "!!!   det  "<< det<<std::endl;
    }
    else {
      double det = c_f * s_b - s_f * c_b;
      det *= det;
      st.XYInfo.C00 = (s_b * s_b * f_sigma * f_sigma + s_f * s_f * b_sigma * b_sigma) / det;
      st.XYInfo.C10 = -(s_b * c_b * f_sigma * f_sigma + s_f * c_f * b_sigma * b_sigma) / det;
      st.XYInfo.C11 = (c_b * c_b * f_sigma * f_sigma + c_f * c_f * b_sigma * b_sigma) / det;
      //std::cout << "!!!   det  "<< det<<std::endl;
    }
    //std::cout.precision(10);
    //std::cout << "Station  "<<i<<"  " << st.XYInfo.C00[0]<<"  "<<st.XYInfo.C11[0]<<"  "<<st.XYInfo.C10[0]<<std::endl;
    //std::cout << "         "<< i<<"   fsigma " << f_sigma<<" bsigma "<<b_sigma<<std::endl;

    //    st.xInfo.cos_phi = c_f/(c_f*s_b - c_b*s_f);
    //    st.xInfo.sin_phi =-c_b/(c_f*s_b - c_b*s_f);
    st.xInfo.cos_phi = -s_f / (c_f * s_b - c_b * s_f);
    st.xInfo.sin_phi = s_b / (c_f * s_b - c_b * s_f);
    st.xInfo.sigma2  = st.XYInfo.C00;

    st.yInfo.cos_phi = c_b / (c_b * s_f - c_f * s_b);
    st.yInfo.sin_phi = -c_f / (c_b * s_f - c_f * s_b);
    st.yInfo.sigma2  = st.XYInfo.C11;
    //std::cout << "st.xInfo.cos_phi "<<st.xInfo.cos_phi<< " st.yInfo.cos_phi " << st.yInfo.cos_phi << std::endl;
    //std::cout << "st.xInfo.sin_phi "<<st.xInfo.sin_phi<< " st.yInfo.sin_phi " << st.yInfo.sin_phi << std::endl;

    //std::cout << "cos_b "<<c_b<< " sin_b " << s_b << std::endl;
    //std::cout << "cos_f "<<c_f<< " sin_f " << s_f << std::endl;
    int N = static_cast<int>(geo[ind++]);
    for (int iC = 0; iC < N; iC++)
      st.fieldSlice.cx[iC] = geo[ind++];
    for (int iC = 0; iC < N; iC++)
      st.fieldSlice.cy[iC] = geo[ind++];
    for (int iC = 0; iC < N; iC++)
      st.fieldSlice.cz[iC] = geo[ind++];
#ifndef TBB2
    std::cout << "    " << st.z[0] << " " << st.materialInfo.thick[0] << " " << st.materialInfo.RL[0] << ", " << N
              << " field coeff." << std::endl;
    std::cout << "       " << f_phi << " " << f_sigma << " " << b_phi << " " << b_sigma << std::endl;
#endif  // TBB2
  }

  fTrackingLevel    = static_cast<int>(geo[ind++]);
  fMomentumCutOff   = geo[ind++];
  fGhostSuppression = static_cast<int>(geo[ind++]);

  {
    //   fvec By0 = vStations[NStations-1].fieldSlice.cy[0];
    fvec z0 = vStations[NStations - 1].z;
    fvec sy = 0., Sy = 0.;
    for (int i = NStations - 1; i >= 0; i--) {
      L1Station& st = vStations[i];
      fvec dz       = st.z - z0;
      fvec By       = vStations[i].fieldSlice.cy[0];
      Sy += dz * sy + dz * dz * By / 2.;
      sy += dz * By;
      //st.Sy = Sy; // commented, because is not used in the code (S.Zharko)
      z0 = st.z;
    }
  }
  //    for( int iS = 0; iS < NStations; ++iS ) {     /// Grid is created for each station with the same step: xStep,yStep
  //       L1Grid &grid = vGrid[iS];
  //
  //      // grid.Create(-1,1,-0.6,0.6,xStep,yStep);
  //       grid.Create(-1,1,-0.6,0.6,0.00317899,0.00105966);
  //     }

#ifndef TBB2
  std::cout << "L1Algo initialized" << std::endl;
#endif  // TBB2

  //
  // NEW INITIALIZATION (BETA)
  //

  // Final init checks (the function provides purity of fields initialization and turn on the last bits of
  // the L1ObjectInitController):
  fInitManager.CheckInit();  // NOTE: after passing this frontier L1Algo is (will be) accounted as initialized

  // Check initialization
  LOG(info) << "InitManager " << fInitManager.GetInitController().ToString();

  // Get number of stations
  int nStationsNew = fInitManager.GetStationsNumber();
  // TODO: we must to get rid of station specification in the L1Algo (S.Zh.)
  int nMvdStationsNew   = fInitManager.GetStationsNumber(static_cast<L1DetectorID>(0));
  int nStsStationsNew   = fInitManager.GetStationsNumber(static_cast<L1DetectorID>(1));
  int nFieldStationsNew = nMvdStationsNew + nStsStationsNew;

  // Get field near target
  L1FieldValue vtxFieldValueNew   = fInitManager.GetTargetFieldValue();
  L1FieldRegion vtxFieldRegionNew = fInitManager.GetTargetFieldRegion();

  // Fill L1Station array
  fInitManager.TransferL1StationArray(fStationsNew);

  LOG(info) << "**********************************************************************";
  LOG(info) << "*  New L1Algo initialization cross check  (tmp log, to be removed!)  *";
  LOG(info) << "**********************************************************************";
  LOG(info) << "** Number of stations (origial) **";
  LOG(info) << "\tTotal: " << NStations;
  LOG(info) << "\tMVD:   " << NMvdStations;
  LOG(info) << "\tSTS:   " << NStsStations;
  LOG(info) << "\tField: " << fNfieldStations;
  LOG(info) << "** Number of stations (new) **";
  LOG(info) << "\tTotal: " << nStationsNew;
  LOG(info) << "\tMVD:   " << nMvdStationsNew;
  LOG(info) << "\tSTS:   " << nStsStationsNew;
  LOG(info) << "\tField: " << nFieldStationsNew;

  LOG(info) << "** Magnetic field near target (original)**";
  LOG(info) << "\tField Value:  " << '\n' << vtxFieldValue.ToString(/*indent = */ 1) << '\n';
  LOG(info) << "\tField Region: " << '\n' << vtxFieldRegion.ToString(/*indent = */ 1) << '\n';

  LOG(info) << "** Magnetic field near target (new)**";
  LOG(info) << "\tField Value:  " << '\n' << vtxFieldValueNew.ToString(/*indent = */ 1) << '\n';
  LOG(info) << "\tField Region: " << '\n' << vtxFieldRegionNew.ToString(/*indent = */ 1) << '\n';

  LOG(info) << "** Original L1Station array content **";
  int nStations = fInitManager.GetStationsNumber();
  for (int iSt = 0; iSt < nStations; ++iSt) {
    LOG(info) << "Station Global No: " << iSt;
    LOG(info) << '\n' << vStations[iSt].ToString(/*verbosity = */ 3);
  }
  LOG(info) << "** New L1Station array content **";
  nStations = fInitManager.GetStationsNumber();
  for (int iSt = 0; iSt < nStations; ++iSt) {
    LOG(info) << "Station Global No: " << iSt;
    LOG(info) << '\n' << fStationsNew[iSt].ToString(/*verbosity = */ 3);
  }

  // Print L1Parameters
  fParameters.Print(/*verbosity=*/0);
}


void L1Algo::SetData(L1Vector<L1Hit>& StsHits_, int nStsStrips_, L1Vector<unsigned char>& SFlag_,
                     const THitI* StsHitsStartIndex_, const THitI* StsHitsStopIndex_)
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
  L1Station& sta = vStations[int(iS)];
  fscal u        = _h.u;
  fscal v        = _h.v;
  _x             = (sta.xInfo.sin_phi[0] * u + sta.xInfo.cos_phi[0] * v) / (_h.z - fCbmTargetZ[0]);
  _y             = (sta.yInfo.cos_phi[0] * u + sta.yInfo.sin_phi[0] * v) / (_h.z - fCbmTargetZ[0]);
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
