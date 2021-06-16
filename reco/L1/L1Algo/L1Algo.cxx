/* Copyright (C) 2010-2021 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Igor Kulakov [committer], Maksym Zyzak, Valentina Akishina */

#include "L1Algo.h"

#include "L1Grid.h"
#include "L1HitPoint.h"

L1Algo::L1Algo(int nThreads, int ExpectedHits)
  : fDupletPortionSize("L1Algo::fDupletPortionSize")
  , fMergerTrackFirstStation("L1Algo::fMergerTrackFirstStation")
  , fMergerTrackLastStation("L1Algo::fMergerTrackLastStation")
  , fMergerTrackFirstHit("L1Algo::fMergerTrackFirstHit")
  , fMergerTrackLastHit("L1Algo::fMergerTrackLastHit")
  , fMergerTrackNeighbour("L1Algo::fMergerTrackNeighbour")
  , fMergerTrackChi2("L1Algo::fMergerTrackChi2")
  , fMergerTrackIsStored("L1Algo::fMergerTrackIsStored")
  , fMergerTrackIsDownstreamNeighbour("L1Algo::fMergerTrackIsDownstreamNeighbour")
  , fMergerTracksNew("L1Algo::fMergerTracksNew")
  , fMergerRecoHitsNew("L1Algo::fMergerRecoHitsNew")
  , NStations(0)
  ,  // number of all detector stations
  NMvdStations(0)
  ,  // number of mvd stations
  NStsStations(0)
  , NFStations(0)
  , fRadThick()
  , NStsStrips(0)  // strips positions created from hits
  , vStsZPos(0)
  ,                // all possible z-positions of hits
  vStsHits(0)      // hits as a combination of front-, backstrips and z-position
  , fStripFlag(0)  // information of hits station & using hits in tracks(),
  , CATime(0)
  ,  // time of trackfinding
  fTracks("L1Algo::fTracks")
  ,  // reconstructed tracks
  fRecoHits("L1Algo::fRecoHits")
  ,  // packed hits of reconstructed tracks
  StsHitsStartIndex(nullptr)
  , StsHitsStopIndex(nullptr)
  , NHitsIsecAll(0)
  , vStsDontUsedHits_A(ExpectedHits)
  , vStsDontUsedHits_B(ExpectedHits)
  , vStsDontUsedHits_Buf(ExpectedHits)
  , vStsDontUsedHitsxy_A(ExpectedHits)
  , vStsDontUsedHitsxy_buf(ExpectedHits)
  , vStsDontUsedHitsxy_B(ExpectedHits)
  , RealIHit_v(ExpectedHits)
  , RealIHit_v_buf(ExpectedHits)
  , RealIHit_v_buf2(ExpectedHits)
  ,

#ifdef _OPENMP
  fHitToBestTrackF("L1Algo::fHitToBestTrackF")
  , fHitToBestTrackB("L1Algo::fHitToBestTrackB")
  ,
#endif
  fStripToTrack("L1Algo::fStripToTrack")
  , fStripToTrackB("L1Algo::fStripToTrackB")
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

  fDupletPortionSize.reserve(100000);
  fTracks.reserve(40000);
  fRecoHits.reserve(400000);

  fStripToTrack.reserve(ExpectedHits * 4);
  fStripToTrackB.reserve(ExpectedHits * 4);

  for (int i = 0; i < fNThreads; i++) {

    fTracks_local[i].SetName(std::stringstream() << "L1Algo::fTracks_local[" << i << "]");
    fTracks_local[i].clear();
    fTracks_local[i].reserve(100000);

    fRecoHits_local[i].SetName(std::stringstream() << "L1Algo::fRecoHits_local[" << i << "]");
    fRecoHits_local[i].clear();
    fRecoHits_local[i].reserve(400000);

    TripForHit[0].resize(ExpectedHits);
    TripForHit[1].resize(ExpectedHits);

    fTrackCandidates[i].SetName(std::stringstream() << "L1Algo::fTrackCandidates[" << i << "]");
    fTrackCandidates[i].clear();
    fTrackCandidates[i].reserve(10000);

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

    for (int j = 0; j < MaxNStations; j++) {
      fTriplets[j][i].SetName(std::stringstream() << "L1Algo::fTriplets[" << i << "][" << j << "]");
      fTriplets[j][i].reserve(ExpectedHits);
      fTriplets[j][i].clear();
    }
  }

  for (int i = 0; i < MaxNStations; i++) {
    vGridTime[i].AllocateMemory(fNThreads);
  }

#ifdef _OPENMP
  fHitToBestTrackF.reserve(ExpectedHits * 2);
  fHitToBestTrackB.reserve(ExpectedHits * 2);
#endif

  NHitsIsecAll              = ExpectedHits;
  const int kExpectedTracks = ExpectedHits / 8;

  fMergerTrackFirstStation.reserve(kExpectedTracks);
  fMergerTrackLastStation.reserve(kExpectedTracks);
  fMergerTrackFirstHit.reserve(kExpectedTracks);
  fMergerTrackLastHit.reserve(kExpectedTracks);
  fMergerTrackNeighbour.reserve(kExpectedTracks);
  fMergerTrackChi2.reserve(kExpectedTracks);
  fMergerTrackIsStored.reserve(kExpectedTracks);
  fMergerTrackIsDownstreamNeighbour.reserve(kExpectedTracks);
  fMergerTracksNew.reserve(kExpectedTracks);
  fMergerRecoHitsNew.reserve(ExpectedHits);

  //     IsNext.resize(kExpectedTracks);
}


void L1Algo::Init(const vector<fscal>& geo, const bool UseHitErrors, const bool mCBMmode)
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
  fmCBMmode     = mCBMmode;

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
  NMvdStations = static_cast<int>(geo[ind++]);
  NStsStations = static_cast<int>(geo[ind++]);

  NFStations = NStsStations + NMvdStations;

  if (fmCBMmode) { NFStations = -1; }


  // cout << "N MVD & STS stations: " << NMvdStations << " " << NStations-NMvdStations << endl;
#ifndef TBB2
  std::cout << "L1Algo Input " << NStations << " Stations:" << std::endl;
#endif  // TBB2
  for (int i = 0; i < NStations; i++) {
    L1Station& st               = vStations[i];
    st.type                     = geo[ind++];
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
      st.Sy = Sy;
      z0    = st.z;
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
}


void L1Algo::SetData(vector<L1StsHit>& StsHits_, int nStsStrips_, const vector<fscal>& StsZPos_,
                     L1Vector<unsigned char>& SFlag_, const THitI* StsHitsStartIndex_, const THitI* StsHitsStopIndex_,
                     const int NhitsGlobal)
{

  vStsHits   = &StsHits_;
  NStsStrips = nStsStrips_;
  vStsZPos   = &StsZPos_;
  fStripFlag = &SFlag_;

  StsHitsStartIndex = StsHitsStartIndex_;
  StsHitsStopIndex  = StsHitsStopIndex_;

  // TODO: maximal array sizes need to be adjusted

  int nHits = vStsHits->size();

  vStsDontUsedHits_A.resize(nHits);
  vStsDontUsedHits_B.resize(nHits);
  vStsDontUsedHits_Buf.resize(nHits);
  vStsDontUsedHitsxy_A.resize(nHits);
  vStsDontUsedHitsxy_buf.resize(nHits);
  vStsDontUsedHitsxy_B.resize(nHits);
  RealIHit_v.resize(nHits);
  RealIHit_v_buf.resize(nHits);
  RealIHit_v_buf2.resize(nHits);

#ifdef _OPENMP
  fHitToBestTrackF.resize(NStsStrips);
  fHitToBestTrackB.resize(NStsStrips);
  for (unsigned int j = 0; j < fHitToBestTrackB.size(); j++) {
    omp_init_lock(&fHitToBestTrackB[j]);
    omp_init_lock(&fHitToBestTrackF[j]);
  }
#endif

  fStripToTrack.reserve(NStsStrips);
  fStripToTrackB.reserve(NStsStrips);

  TripForHit[0].resize(nHits);
  TripForHit[1].resize(nHits);
  NHitsIsecAll = nHits;
  fDupletPortionSize.clear();
  fDupletPortionSize.reserve(2 * nHits);

  for (int i = 0; i < fNThreads; i++) {
    fTracks_local[i].clear();
    fTracks_local[i].reserve(nHits / 10);
    fRecoHits_local[i].clear();
    fRecoHits_local[i].reserve(nHits);
    fTrackCandidates[i].clear();
    fTrackCandidates[i].reserve(nHits / 10);
    for (int j = 0; j < MaxNStations; j++) {
      fTriplets[j][i].clear();
      fTriplets[j][i].reserve(2 * nHits);
    }
  }

  /*    
  vStsHits.resize(StsHits_.size());
 vStsStrips.resize(StsStrips_.size());
 vStsStripsB.resize(StsStripsB_.size());
  vStsZPos.resize(StsZPos_.size());
  fStripFlag.resize(SFlag_.size());
  fStripFlagB.resize(SFlagB_.size());
  
  for(Tindex i=0; i< static_cast<Tindex>(StsHits_.size()); ++i ) vStsHits[i] = StsHits_[i];
  for(Tindex i=0; i< static_cast<Tindex>(StsStrips_.size()); ++i ) vStsStrips[i] = StsStrips_[i];
  for(Tindex i=0; i< static_cast<Tindex>(StsStripsB_.size()); ++i ) vStsStripsB[i] = StsStripsB_[i];
  for(Tindex i=0; i< static_cast<Tindex>(StsZPos_.size()); ++i ) vStsZPos[i] = StsZPos_[i];
  for(Tindex i=0; i< static_cast<Tindex>(SFlag_.size()); ++i ) fStripFlag[i] = SFlag_[i];
  for(Tindex i=0; i< static_cast<Tindex>(SFlagB_.size()); ++i ) fStripFlagB[i] = SFlagB_[i];

  for(Tindex i=0; i<MaxNStations+1; ++i) StsHitsStartIndex[i] = StsHitsStartIndex_[i];
  for(Tindex i=0; i<MaxNStations+1; ++i) StsHitsStopIndex[i]  = StsHitsStopIndex_[i];*/
}


void L1Algo::GetHitCoor(const L1StsHit& _h, fscal& _x, fscal& _y, char iS)
{
  L1Station& sta = vStations[int(iS)];
  fscal u        = _h.u;
  fscal v        = _h.v;
  // const fscal &z = (*vStsZPos)[_h.iz];
  // fscal x, y;
  _x = (sta.xInfo.sin_phi[0] * u + sta.xInfo.cos_phi[0] * v) / (*vStsZPos)[_h.iz];
  _y = (sta.yInfo.cos_phi[0] * u + sta.yInfo.sin_phi[0] * v) / (*vStsZPos)[_h.iz];
}

void L1Algo::GetHitCoor(const L1StsHit& _h, fscal& _x, fscal& _y, fscal& _z, const L1Station& sta)
{
  fscal u = _h.u;
  fscal v = _h.v;
  fscal x, y;
  StripsToCoor(u, v, x, y, sta);
  _x = x;
  _y = y;
  _z = (*vStsZPos)[_h.iz];
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

/// full the hit point by hit information: takes hit as input (2 strips) and creates hit_point with all coordinates (x,y,z,u,v, n - event number);
L1HitPoint L1Algo::CreateHitPoint(const L1StsHit& hit, char /*ista*/)
/// hit and station number
{
  // L1Station& sta   = vStations[int(ista)];
  const float& z    = (*vStsZPos)[hit.iz];
  const float& time = hit.t_reco;
  return L1HitPoint(z, hit.u, hit.v, hit.du, hit.dv, time, hit.t_er);
}

void L1Algo::CreateHitPoint(const L1StsHit& hit, char /*ista*/, L1HitPoint& point)
/// hit and station number
{
  // L1Station& sta   = vStations[int(ista)];
  const float& z    = (*vStsZPos)[hit.iz];
  const float& time = hit.t_reco;
  const float& du_  = hit.du;
  const float& dv_  = hit.dv;

  point.Set(z, hit.u, hit.v, du_, dv_, time, hit.t_er);
  //   point.Set(x,y,z,v.f,u.f, time, n1, hit.time1, 2.9 ); // TODO put correct time error from the hit
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
//       return   (a) + ((b)*10000) + (c*100000000);
// }
