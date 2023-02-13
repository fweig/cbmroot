/* Copyright (C) 2011-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Maksym Zyzak [committer] */

/*
 *=====================================================
 *
 *  CBM Level 1 Reconstruction
 *
 *  Authors: M.Zyzak
 *
 *  e-mail :
 *
 *=====================================================
 *
 *  SIMD Fitter for CbmL1Track class
 *
 */

#include "CbmL1PFFitter.h"

#include "CbmL1.h"
#include "CbmStsAddress.h"
#include "CbmStsHit.h"
#include "CbmStsSetup.h"
#include "CbmStsTrack.h"

#include "TClonesArray.h"

//L1Algo tools
#include "CbmKFVertex.h"
#include "CbmL1Track.h"

#include "FairRootManager.h"

#include "TDatabasePDG.h"

#include "KFParticleDatabase.h"
#include "L1Algo.h"  // Also defines L1Parameters
#include "L1Def.h"
#include "L1Field.h"
#include "L1Fit.h"
#include "L1Station.h"
#include "L1TrackPar.h"

//typedef L1Fit1 L1Fit;

using std::vector;

namespace NS_L1TrackFitter
{
  const fvec c_light(0.000299792458), c_light_i = fvec(1.) / c_light;
  const fvec ZERO = fvec(0.);
  const fvec ONE  = fvec(1.);
  const fvec vINF = fvec(0.1);
}  // namespace NS_L1TrackFitter
using namespace NS_L1TrackFitter;

CbmL1PFFitter::CbmL1PFFitter() {}

CbmL1PFFitter::~CbmL1PFFitter() {}

inline void CbmL1PFFitter::PFFieldRegion::setFromL1FieldRegion(const L1FieldRegion& fld, int i)
{
  fP[0] = fld.cx0[i];
  fP[1] = fld.cx1[i];
  fP[2] = fld.cx2[i];

  fP[3] = fld.cy0[i];
  fP[4] = fld.cy1[i];
  fP[5] = fld.cy2[i];

  fP[6] = fld.cz0[i];
  fP[7] = fld.cz1[i];
  fP[8] = fld.cz2[i];

  fP[9] = fld.z0[i];
}

inline void CbmL1PFFitter::PFFieldRegion::getL1FieldRegion(L1FieldRegion& fld, int i)
{
  fld.cx0[i] = fP[0];
  fld.cx1[i] = fP[1];
  fld.cx2[i] = fP[2];

  fld.cy0[i] = fP[3];
  fld.cy1[i] = fP[4];
  fld.cy2[i] = fP[5];

  fld.cz0[i] = fP[6];
  fld.cz1[i] = fP[7];
  fld.cz2[i] = fP[8];

  fld.z0[i] = fP[9];
}

inline CbmL1PFFitter::PFFieldRegion::PFFieldRegion(const L1FieldRegion& fld, int i) { setFromL1FieldRegion(fld, i); }


void FilterFirst(L1Fit& fit, fvec& x, fvec& y, fvec& dxx, fvec& dxy, fvec& dyy)
{
  L1TrackPar& tr = fit.Tr();
  tr.ResetErrors(dxx, dyy, vINF, vINF, 1., 1.e6, 1.e2);
  tr.C10 = dxy;
  tr.x   = x;
  tr.y   = y;
  tr.t   = 0.;
  tr.vi  = 0.;
  tr.NDF = -3.0;
}

void CbmL1PFFitter::Fit(vector<CbmStsTrack>& Tracks, vector<int>& pidHypo)
{

  L1FieldValue b0, b1, b2 _fvecalignment;
  L1FieldRegion fld _fvecalignment;


  FairRootManager* fManger  = FairRootManager::Instance();
  TClonesArray* listStsHits = (TClonesArray*) fManger->GetObject("StsHit");
  int NMvdStations          = CbmL1::Instance()->fpAlgo->GetNstationsBeforePipe();
  TClonesArray* listMvdHits = 0;
  if (NMvdStations > 0.) listMvdHits = (TClonesArray*) fManger->GetObject("MvdHit");

  static int nHits = CbmL1::Instance()->fpAlgo->GetParameters()->GetNstationsActive();
  int iVec = 0, i = 0;
  int nTracks_SIMD = fvec::size();

  L1Fit fit;
  fit.SetParticleMass(CbmL1::Instance()->fpAlgo->GetDefaultParticleMass());

  L1TrackPar& T = fit.Tr();  // fitting parametr coresponding to current track

  CbmStsTrack* tr[fvec::size()] {nullptr};

  int ista;
  const L1Station* sta = CbmL1::Instance()->fpAlgo->GetParameters()->GetStations().begin();

  fvec* x   = new fvec[nHits];
  fvec* u   = new fvec[nHits];
  fvec* v   = new fvec[nHits];
  fvec* t   = new fvec[nHits];
  fvec* du2 = new fvec[nHits];
  fvec* dv2 = new fvec[nHits];
  fvec* dt2 = new fvec[nHits];
  fvec* y   = new fvec[nHits];
  fvec* z   = new fvec[nHits];
  fmask* w  = new fmask[nHits];

  //  fvec y_temp;
  fvec x_first, y_first, x_last, y_last;
  fvec fstC00, fstC10, fstC11;
  fvec lstC00, lstC10, lstC11;

  fvec z0, z1, z2, dz, z_start, z_end;
  L1FieldValue* fB = new L1FieldValue[nHits];
  L1FieldValue fB_temp _fvecalignment;

  unsigned short N_vTracks = Tracks.size();


  for (unsigned short itrack = 0; itrack < N_vTracks; itrack++) {
    Tracks[itrack].SetPidHypo(pidHypo[itrack]);
  }

  fvec mass = 0.000511f;

  for (unsigned short itrack = 0; itrack < N_vTracks; itrack += fvec::size()) {

    T.t  = 0.;
    T.vi = 0.;
    T.ResetErrors(1.e2, 1.e2, 1.e4, 1.e4, 1.e4, 1.e6, 1.e2);

    if (N_vTracks - itrack < static_cast<unsigned short>(fvec::size())) nTracks_SIMD = N_vTracks - itrack;
    for (i = 0; i < nTracks_SIMD; i++) {
      tr[i]    = &Tracks[itrack + i];  // current track
      T.x[i]   = tr[i]->GetParamFirst()->GetX();
      T.y[i]   = tr[i]->GetParamFirst()->GetY();
      T.tx[i]  = tr[i]->GetParamFirst()->GetTx();
      T.ty[i]  = tr[i]->GetParamFirst()->GetTy();
      T.qp[i]  = tr[i]->GetParamFirst()->GetQp();
      T.z[i]   = tr[i]->GetParamFirst()->GetZ();
      T.C00[i] = tr[i]->GetParamFirst()->GetCovariance(0, 0);
      T.C10[i] = tr[i]->GetParamFirst()->GetCovariance(1, 0);
      T.C11[i] = tr[i]->GetParamFirst()->GetCovariance(1, 1);
      T.C20[i] = tr[i]->GetParamFirst()->GetCovariance(2, 0);
      T.C21[i] = tr[i]->GetParamFirst()->GetCovariance(2, 1);
      T.C22[i] = tr[i]->GetParamFirst()->GetCovariance(2, 2);
      T.C30[i] = tr[i]->GetParamFirst()->GetCovariance(3, 0);
      T.C31[i] = tr[i]->GetParamFirst()->GetCovariance(3, 1);
      T.C32[i] = tr[i]->GetParamFirst()->GetCovariance(3, 2);
      T.C33[i] = tr[i]->GetParamFirst()->GetCovariance(3, 3);
      T.C40[i] = tr[i]->GetParamFirst()->GetCovariance(4, 0);
      T.C41[i] = tr[i]->GetParamFirst()->GetCovariance(4, 1);
      T.C42[i] = tr[i]->GetParamFirst()->GetCovariance(4, 1);
      T.C43[i] = tr[i]->GetParamFirst()->GetCovariance(4, 3);
      T.C44[i] = tr[i]->GetParamFirst()->GetCovariance(4, 4);

      int pid = pidHypo[itrack + i];
      if (pid == -1) pid = 211;
      //       mass[i] = TDatabasePDG::Instance()->GetParticle(pid)->Mass();
      mass[i] = KFParticleDatabase::Instance()->GetMass(pid);
    }

    fit.SetParticleMass(mass);

    // get hits of current track
    for (i = 0; i < nHits; i++) {
      w[i] = fmask::Zero();
      z[i] = sta[i].fZ;
    }

    for (iVec = 0; iVec < nTracks_SIMD; iVec++) {
      int nHitsTrackMvd = tr[iVec]->GetNofMvdHits();
      int nHitsTrackSts = tr[iVec]->GetNofStsHits();
      int nHitsTrack    = nHitsTrackMvd + nHitsTrackSts;
      for (i = 0; i < nHitsTrack; i++) {
        float posx = 0.f, posy = 0.f, posz = 0.f, time = 0.;

        if (i < nHitsTrackMvd) {
          if (!listMvdHits) continue;
          int hitIndex   = tr[iVec]->GetMvdHitIndex(i);
          CbmMvdHit* hit = L1_DYNAMIC_CAST<CbmMvdHit*>(listMvdHits->At(hitIndex));

          posx = hit->GetX();
          posy = hit->GetY();
          posz = hit->GetZ();
          time = hit->GetTime();

          // ista = hit->GetStationNr();
          ista =
            CbmL1::Instance()->fpAlgo->GetParameters()->GetStationIndexActive(hit->GetStationNr(), L1DetectorID::kMvd);
          if (ista == -1) continue;
          du2[ista][iVec] = hit->GetDx() * hit->GetDx();
          dv2[ista][iVec] = hit->GetDy() * hit->GetDy();
          dt2[ista][iVec] = hit->GetTimeError() * hit->GetTimeError();
        }
        else {
          if (!listStsHits) continue;
          int hitIndex   = tr[iVec]->GetHitIndex(i - nHitsTrackMvd);
          CbmStsHit* hit = L1_DYNAMIC_CAST<CbmStsHit*>(listStsHits->At(hitIndex));

          posx = hit->GetX();
          posy = hit->GetY();
          posz = hit->GetZ();
          time = hit->GetTime();

          //  ista = CbmStsSetup::Instance()->GetStationNumber(hit->GetAddress())
          //       + NMvdStations;  //hit->GetStationNr() - 1 + NMvdStations;
          ista = CbmL1::Instance()->fpAlgo->GetParameters()->GetStationIndexActive(
            CbmStsSetup::Instance()->GetStationNumber(hit->GetAddress()), L1DetectorID::kSts);
          if (ista == -1) continue;
          du2[ista][iVec] = hit->GetDu() * hit->GetDu();
          dv2[ista][iVec] = hit->GetDv() * hit->GetDv();
          dt2[ista][iVec] = hit->GetTimeError() * hit->GetTimeError();
        }
        w[ista][iVec] = true;

        x[ista][iVec] = posx;
        y[ista][iVec] = posy;
        u[ista][iVec] = posx * sta[ista].frontInfo.cos_phi[0] + posy * sta[ista].frontInfo.sin_phi[0];
        v[ista][iVec] = posx * sta[ista].backInfo.cos_phi[0] + posy * sta[ista].backInfo.sin_phi[0];
        z[ista][iVec] = posz;
        t[ista][iVec] = time;

        sta[ista].fieldSlice.GetFieldValue(x[ista], y[ista], fB_temp);
        fB[ista].x[iVec] = fB_temp.x[iVec];
        fB[ista].y[iVec] = fB_temp.y[iVec];
        fB[ista].z[iVec] = fB_temp.z[iVec];
        if (i == 0) {
          z_start[iVec] = posz;
          x_first[iVec] = x[ista][iVec];
          y_first[iVec] = y[ista][iVec];

          auto [dxx, dxy, dyy] = sta[ista].FormXYCovarianceMatrix((fscal) du2[ista][iVec], (fscal) dv2[ista][iVec]);

          fstC00[iVec] = dxx;
          fstC10[iVec] = dxy;
          fstC11[iVec] = dyy;
        }
        if (i == nHitsTrack - 1) {
          z_end[iVec]  = posz;
          x_last[iVec] = x[ista][iVec];
          y_last[iVec] = y[ista][iVec];

          auto [dxx, dxy, dyy] = sta[ista].FormXYCovarianceMatrix((fscal) du2[ista][iVec], (fscal) dv2[ista][iVec]);

          lstC00[iVec] = dxx;
          lstC10[iVec] = dxy;
          lstC11[iVec] = dyy;
        }
      }
    }
    // fit forward
    i = 0;

    FilterFirst(fit, x_first, y_first, fstC00, fstC10, fstC11);
    fit.SetQp0(fit.Tr().qp);

    z1 = z[i];
    sta[i].fieldSlice.GetFieldValue(T.x, T.y, b1);
    b1.Combine(fB[i], w[i]);
    z2 = z[i + 2];
    dz = z2 - z1;
    sta[i].fieldSlice.GetFieldValue(T.x + T.tx * dz, T.y + T.ty * dz, b2);
    b2.Combine(fB[i + 2], w[i + 2]);
    fld.Set(b2, z2, b1, z1, b0, z0);
    for (++i; i < nHits; i++) {
      z0 = z[i];
      dz = (z1 - z0);
      sta[i].fieldSlice.GetFieldValue(T.x - T.tx * dz, T.y - T.ty * dz, b0);
      b0.Combine(fB[i], w[i]);
      fld.Set(b0, z0, b1, z1, b2, z2);

      fmask initialised = (z[i] <= z_end) & (z_start < z[i]);

      fit.SetMask(initialised);
      fit.Extrapolate(z[i], fld);
      auto radThick = CbmL1::Instance()->fpAlgo->GetParameters()->GetMaterialThickness(i, fit.Tr().x, fit.Tr().y);
      fit.AddMsInMaterial(radThick);
      fit.EnergyLossCorrection(radThick, -fvec::One());

      fit.SetMask(initialised && w[i]);
      fit.Filter(sta[i].frontInfo, u[i], du2[i]);
      fit.Filter(sta[i].backInfo, v[i], dv2[i]);
      fit.FilterTime(t[i], dt2[i], sta[i].timeInfo);

      b2 = b1;
      z2 = z1;
      b1 = b0;
      z1 = z0;
    }

    L1TrackPar Tout = T;
    for (iVec = 0; iVec < nTracks_SIMD; iVec++) {
      FairTrackParam par;
      par.SetX(T.x[iVec]);
      par.SetY(T.y[iVec]);
      par.SetTx(T.tx[iVec]);
      par.SetTy(T.ty[iVec]);
      par.SetQp(T.qp[iVec]);
      par.SetZ(T.z[iVec]);

      par.SetCovariance(0, 0, Tout.C00[iVec]);
      par.SetCovariance(1, 0, Tout.C10[iVec]);
      par.SetCovariance(1, 1, Tout.C11[iVec]);
      par.SetCovariance(2, 0, Tout.C20[iVec]);
      par.SetCovariance(2, 1, Tout.C21[iVec]);
      par.SetCovariance(2, 2, Tout.C22[iVec]);
      par.SetCovariance(3, 0, Tout.C30[iVec]);
      par.SetCovariance(3, 1, Tout.C31[iVec]);
      par.SetCovariance(3, 2, Tout.C32[iVec]);
      par.SetCovariance(3, 3, Tout.C33[iVec]);
      par.SetCovariance(4, 0, Tout.C40[iVec]);
      par.SetCovariance(4, 1, Tout.C41[iVec]);
      par.SetCovariance(4, 2, Tout.C42[iVec]);
      par.SetCovariance(4, 3, Tout.C43[iVec]);
      par.SetCovariance(4, 4, Tout.C44[iVec]);
      tr[iVec]->SetParamLast(&par);
    }

    //fit backward
    fit.SetQp0(T.qp);

    i = nHits - 1;

    FilterFirst(fit, x_last, y_last, lstC00, lstC10, lstC11);

    z1 = z[i];
    sta[i].fieldSlice.GetFieldValue(T.x, T.y, b1);
    b1.Combine(fB[i], w[i]);

    z2 = z[i - 2];
    dz = z2 - z1;
    sta[i].fieldSlice.GetFieldValue(T.x + T.tx * dz, T.y + T.ty * dz, b2);
    b2.Combine(fB[i - 2], w[i - 2]);
    fld.Set(b2, z2, b1, z1, b0, z0);
    for (--i; i >= 0; i--) {
      z0 = z[i];
      dz = (z1 - z0);
      sta[i].fieldSlice.GetFieldValue(T.x - T.tx * dz, T.y - T.ty * dz, b0);
      b0.Combine(fB[i], w[i]);
      fld.Set(b0, z0, b1, z1, b2, z2);

      fmask initialised = (z[i] < z_end) & (z_start <= z[i]);

      fit.SetMask(initialised);
      fit.Extrapolate(z[i], fld);
      auto radThick = CbmL1::Instance()->fpAlgo->GetParameters()->GetMaterialThickness(i, fit.Tr().x, fit.Tr().y);
      fit.AddMsInMaterial(radThick);
      fit.EnergyLossCorrection(radThick, fvec::One());

      fit.SetMask(initialised && w[i]);
      fit.Filter(sta[i].frontInfo, u[i], du2[i]);
      fit.Filter(sta[i].backInfo, v[i], dv2[i]);
      fit.FilterTime(t[i], dt2[i], sta[i].timeInfo);

      b2 = b1;
      z2 = z1;
      b1 = b0;
      z1 = z0;
    }

    for (iVec = 0; iVec < nTracks_SIMD; iVec++) {
      FairTrackParam par;
      par.SetX(T.x[iVec]);
      par.SetY(T.y[iVec]);
      par.SetTx(T.tx[iVec]);
      par.SetTy(T.ty[iVec]);
      par.SetQp(T.qp[iVec]);
      par.SetZ(T.z[iVec]);

      par.SetCovariance(0, 0, T.C00[iVec]);
      par.SetCovariance(1, 0, T.C10[iVec]);
      par.SetCovariance(1, 1, T.C11[iVec]);
      par.SetCovariance(2, 0, T.C20[iVec]);
      par.SetCovariance(2, 1, T.C21[iVec]);
      par.SetCovariance(2, 2, T.C22[iVec]);
      par.SetCovariance(3, 0, T.C30[iVec]);
      par.SetCovariance(3, 1, T.C31[iVec]);
      par.SetCovariance(3, 2, T.C32[iVec]);
      par.SetCovariance(3, 3, T.C33[iVec]);
      par.SetCovariance(4, 0, T.C40[iVec]);
      par.SetCovariance(4, 1, T.C41[iVec]);
      par.SetCovariance(4, 2, T.C42[iVec]);
      par.SetCovariance(4, 3, T.C43[iVec]);
      par.SetCovariance(4, 4, T.C44[iVec]);
      tr[iVec]->SetParamFirst(&par);

      tr[iVec]->SetChiSq(T.chi2[iVec]);
      tr[iVec]->SetNDF(static_cast<int>(T.NDF[iVec]));
    }
  }

  delete[] x;
  delete[] u;
  delete[] v;
  delete[] y;
  delete[] z;
  delete[] w;
  delete[] fB;
}

void CbmL1PFFitter::GetChiToVertex(vector<CbmStsTrack>& Tracks, vector<PFFieldRegion>& field, vector<float>& chiToVtx,
                                   CbmKFVertex& primVtx, float chiPrim)
{
  chiToVtx.reserve(Tracks.size());

  int nTracks_SIMD = fvec::size();

  L1Fit fit;
  L1TrackPar& T = fit.Tr();  // fitting parametr coresponding to current track

  CbmStsTrack* tr[fvec::size()] {nullptr};

  int nStations        = CbmL1::Instance()->fpAlgo->GetParameters()->GetNstationsActive();
  int NMvdStations     = CbmL1::Instance()->fpAlgo->GetNstationsBeforePipe();
  const L1Station* sta = CbmL1::Instance()->fpAlgo->GetParameters()->GetStations().begin();
  fvec* zSta           = new fvec[nStations];
  for (int iSta = 0; iSta < nStations; iSta++) {
    zSta[iSta] = sta[iSta].fZ;
  }

  field.reserve(Tracks.size());

  L1FieldRegion fld _fvecalignment;
  L1FieldValue fB[3], fB_temp _fvecalignment;
  fvec zField[3];
  FairRootManager* fManger  = FairRootManager::Instance();
  TClonesArray* listStsHits = (TClonesArray*) fManger->GetObject("StsHit");
  TClonesArray* listMvdHits = 0;
  if (NMvdStations > 0.) listMvdHits = (TClonesArray*) fManger->GetObject("MvdHit");

  unsigned short N_vTracks = Tracks.size();
  int ista;
  for (unsigned short itrack = 0; itrack < N_vTracks; itrack += fvec::size()) {
    if (N_vTracks - itrack < static_cast<unsigned short>(fvec::size())) { nTracks_SIMD = N_vTracks - itrack; }

    fvec mass2;
    for (int iVec = 0; iVec < nTracks_SIMD; iVec++) {
      tr[iVec]    = &Tracks[itrack + iVec];  // current track
      T.x[iVec]   = tr[iVec]->GetParamFirst()->GetX();
      T.y[iVec]   = tr[iVec]->GetParamFirst()->GetY();
      T.tx[iVec]  = tr[iVec]->GetParamFirst()->GetTx();
      T.ty[iVec]  = tr[iVec]->GetParamFirst()->GetTy();
      T.qp[iVec]  = tr[iVec]->GetParamFirst()->GetQp();
      T.z[iVec]   = tr[iVec]->GetParamFirst()->GetZ();
      T.C00[iVec] = tr[iVec]->GetParamFirst()->GetCovariance(0, 0);
      T.C10[iVec] = tr[iVec]->GetParamFirst()->GetCovariance(1, 0);
      T.C11[iVec] = tr[iVec]->GetParamFirst()->GetCovariance(1, 1);
      T.C20[iVec] = tr[iVec]->GetParamFirst()->GetCovariance(2, 0);
      T.C21[iVec] = tr[iVec]->GetParamFirst()->GetCovariance(2, 1);
      T.C22[iVec] = tr[iVec]->GetParamFirst()->GetCovariance(2, 2);
      T.C30[iVec] = tr[iVec]->GetParamFirst()->GetCovariance(3, 0);
      T.C31[iVec] = tr[iVec]->GetParamFirst()->GetCovariance(3, 1);
      T.C32[iVec] = tr[iVec]->GetParamFirst()->GetCovariance(3, 2);
      T.C33[iVec] = tr[iVec]->GetParamFirst()->GetCovariance(3, 3);
      T.C40[iVec] = tr[iVec]->GetParamFirst()->GetCovariance(4, 0);
      T.C41[iVec] = tr[iVec]->GetParamFirst()->GetCovariance(4, 1);
      T.C42[iVec] = tr[iVec]->GetParamFirst()->GetCovariance(4, 1);
      T.C43[iVec] = tr[iVec]->GetParamFirst()->GetCovariance(4, 3);
      T.C44[iVec] = tr[iVec]->GetParamFirst()->GetCovariance(4, 4);
      //      float mass = TDatabasePDG::Instance()->GetParticle(tr[iVec]->GetPidHypo())->Mass();
      const float mass = KFParticleDatabase::Instance()->GetMass(tr[iVec]->GetPidHypo());
      mass2[iVec]      = mass * mass;

      int nHitsTrackMvd = tr[iVec]->GetNofMvdHits();
      for (int iH = 0; iH < 2; iH++) {
        float posx = 0.f, posy = 0.f, posz = 0.f;

        if (iH < nHitsTrackMvd) {
          if (!listMvdHits) continue;
          int hitIndex   = tr[iVec]->GetMvdHitIndex(iH);
          CbmMvdHit* hit = L1_DYNAMIC_CAST<CbmMvdHit*>(listMvdHits->At(hitIndex));

          posx = hit->GetX();
          posy = hit->GetY();
          posz = hit->GetZ();
          ista = hit->GetStationNr();
        }
        else {
          if (!listStsHits) continue;
          int hitIndex   = tr[iVec]->GetHitIndex(iH - nHitsTrackMvd);
          CbmStsHit* hit = L1_DYNAMIC_CAST<CbmStsHit*>(listStsHits->At(hitIndex));

          posx = hit->GetX();
          posy = hit->GetY();
          posz = hit->GetZ();
          ista = CbmStsSetup::Instance()->GetStationNumber(hit->GetAddress())
                 + NMvdStations;  //hit->GetStationNr()-1+NMvdStations;
        }

        sta[ista].fieldSlice.GetFieldValue(posx, posy, fB_temp);
        fB[iH + 1].x[iVec]   = fB_temp.x[iVec];
        fB[iH + 1].y[iVec]   = fB_temp.y[iVec];
        fB[iH + 1].z[iVec]   = fB_temp.z[iVec];
        zField[iH + 1][iVec] = posz;
      }
    }

    fB[0]     = CbmL1::Instance()->fpAlgo->GetParameters()->GetVertexFieldValue();
    zField[0] = CbmL1::Instance()->fpAlgo->GetParameters()->GetTargetPositionZ();
    fld.Set(fB[2], zField[2], fB[1], zField[1], fB[0], zField[0]);
    for (int i = 0; i < nTracks_SIMD; i++) {
      field.emplace_back(fld, i);
    }

    fit.SetQp0(fit.Tr().qp);

    for (int iSt = nStations - 4; iSt >= 0; iSt--) {
      fit.SetMask(T.z > zSta[iSt] + fvec(2.5));
      fit.Extrapolate(zSta[iSt], fld);
      auto radThick = CbmL1::Instance()->fpAlgo->GetParameters()->GetMaterialThickness(iSt, fit.Tr().x, fit.Tr().y);
      fit.AddMsInMaterial(radThick);
      fit.EnergyLossCorrection(radThick, fvec::One());
    }
    fit.SetMask(fmask::One());
    fit.Extrapolate(primVtx.GetRefZ(), fld);
    fit.AddMsInMaterial(fit.GetTargetRadThick());
    fit.EnergyLossCorrection(fit.GetTargetRadThick(), fvec::One());

    Double_t Cv[3] = {primVtx.GetCovMatrix()[0], primVtx.GetCovMatrix()[1], primVtx.GetCovMatrix()[2]};

    fvec dx   = T.x - fvec(primVtx.GetRefX());
    fvec dy   = T.y - fvec(primVtx.GetRefY());
    fvec c[3] = {T.C00, T.C10, T.C11};
    c[0] += fvec(Cv[0]);
    c[1] += fvec(Cv[1]);
    c[2] += fvec(Cv[2]);
    fvec d   = c[0] * c[2] - c[1] * c[1];
    fvec chi = sqrt(abs(fvec(0.5) * (dx * dx * c[0] - fvec(2.) * dx * dy * c[1] + dy * dy * c[2]) / d));
    chi.setZero(abs(d) < fvec(1.e-20));

    for (int iVec = 0; iVec < nTracks_SIMD; iVec++) {
      chiToVtx.push_back(chi[iVec]);
    }

    if (chiPrim > 0) {
      for (int iVec = 0; iVec < nTracks_SIMD; iVec++) {
        if (chi[iVec] < chiPrim) {
          FairTrackParam par;
          par.SetX(T.x[iVec]);
          par.SetY(T.y[iVec]);
          par.SetTx(T.tx[iVec]);
          par.SetTy(T.ty[iVec]);
          par.SetQp(T.qp[iVec]);
          par.SetZ(T.z[iVec]);

          par.SetCovariance(0, 0, T.C00[iVec]);
          par.SetCovariance(1, 0, T.C10[iVec]);
          par.SetCovariance(1, 1, T.C11[iVec]);
          par.SetCovariance(2, 0, T.C20[iVec]);
          par.SetCovariance(2, 1, T.C21[iVec]);
          par.SetCovariance(2, 2, T.C22[iVec]);
          par.SetCovariance(3, 0, T.C30[iVec]);
          par.SetCovariance(3, 1, T.C31[iVec]);
          par.SetCovariance(3, 2, T.C32[iVec]);
          par.SetCovariance(3, 3, T.C33[iVec]);
          par.SetCovariance(4, 0, T.C40[iVec]);
          par.SetCovariance(4, 1, T.C41[iVec]);
          par.SetCovariance(4, 2, T.C42[iVec]);
          par.SetCovariance(4, 3, T.C43[iVec]);
          par.SetCovariance(4, 4, T.C44[iVec]);
          tr[iVec]->SetParamFirst(&par);
        }
      }
    }
  }
  delete[] zSta;
}

void CbmL1PFFitter::CalculateFieldRegion(vector<CbmStsTrack>& Tracks, vector<PFFieldRegion>& field)
{
  field.reserve(Tracks.size());

  L1FieldRegion fld _fvecalignment;

  FairRootManager* fManger  = FairRootManager::Instance();
  TClonesArray* listStsHits = (TClonesArray*) fManger->GetObject("StsHit");
  TClonesArray* listMvdHits = 0;
  int NMvdStations          = CbmL1::Instance()->fpAlgo->GetNstationsBeforePipe();
  if (NMvdStations > 0.) listMvdHits = (TClonesArray*) fManger->GetObject("MvdHit");

  int nTracks_SIMD = fvec::size();
  L1TrackPar T;  // fitting parametr coresponding to current track

  CbmStsTrack* tr[fvec::size()];

  int ista;
  const L1Station* sta = CbmL1::Instance()->fpAlgo->GetParameters()->GetStations().begin();
  L1FieldValue fB[3], fB_temp _fvecalignment;
  fvec zField[3];

  unsigned short N_vTracks = Tracks.size();

  for (unsigned short itrack = 0; itrack < N_vTracks; itrack += fvec::size()) {
    if (N_vTracks - itrack < static_cast<unsigned short>(fvec::size())) nTracks_SIMD = N_vTracks - itrack;

    for (int i = 0; i < nTracks_SIMD; i++)
      tr[i] = &Tracks[itrack + i];  // current track

    for (int iVec = 0; iVec < nTracks_SIMD; iVec++) {
      int nHitsTrackMvd = tr[iVec]->GetNofMvdHits();
      for (int iH = 0; iH < 2; iH++) {
        float posx = 0.f, posy = 0.f, posz = 0.f;

        if (iH < nHitsTrackMvd) {
          if (!listMvdHits) continue;
          int hitIndex   = tr[iVec]->GetMvdHitIndex(iH);
          CbmMvdHit* hit = L1_DYNAMIC_CAST<CbmMvdHit*>(listMvdHits->At(hitIndex));

          posx = hit->GetX();
          posy = hit->GetY();
          posz = hit->GetZ();
          ista = hit->GetStationNr();
        }
        else {
          if (!listStsHits) continue;
          int hitIndex   = tr[iVec]->GetHitIndex(iH - nHitsTrackMvd);
          CbmStsHit* hit = L1_DYNAMIC_CAST<CbmStsHit*>(listStsHits->At(hitIndex));

          posx = hit->GetX();
          posy = hit->GetY();
          posz = hit->GetZ();
          ista = CbmStsSetup::Instance()->GetStationNumber(hit->GetAddress())
                 + NMvdStations;  //hit->GetStationNr()-1+NMvdStations;
        }

        sta[ista].fieldSlice.GetFieldValue(posx, posy, fB_temp);
        fB[iH + 1].x[iVec]   = fB_temp.x[iVec];
        fB[iH + 1].y[iVec]   = fB_temp.y[iVec];
        fB[iH + 1].z[iVec]   = fB_temp.z[iVec];
        zField[iH + 1][iVec] = posz;
      }
    }

    fB[0]     = CbmL1::Instance()->fpAlgo->GetParameters()->GetVertexFieldValue();
    zField[0] = CbmL1::Instance()->fpAlgo->GetParameters()->GetTargetPositionZ();
    fld.Set(fB[2], zField[2], fB[1], zField[1], fB[0], zField[0]);
    for (int i = 0; i < nTracks_SIMD; i++) {
      field.emplace_back(fld, i);
    }
  }
}

void CbmL1PFFitter::CalculateFieldRegionAtLastPoint(vector<CbmStsTrack>& Tracks, vector<PFFieldRegion>& field)
{
  field.reserve(Tracks.size());

  L1FieldRegion fld _fvecalignment;

  FairRootManager* fManger  = FairRootManager::Instance();
  TClonesArray* listStsHits = (TClonesArray*) fManger->GetObject("StsHit");
  TClonesArray* listMvdHits = 0;
  int NMvdStations          = CbmL1::Instance()->fpAlgo->GetNstationsBeforePipe();
  if (NMvdStations > 0.) listMvdHits = (TClonesArray*) fManger->GetObject("MvdHit");

  int nTracks_SIMD = fvec::size();
  L1TrackPar T;  // fitting parametr coresponding to current track

  CbmStsTrack* tr[fvec::size()];

  int ista;
  const L1Station* sta = CbmL1::Instance()->fpAlgo->GetParameters()->GetStations().begin();
  L1FieldValue fB[3], fB_temp _fvecalignment;
  fvec zField[3];

  unsigned short N_vTracks = Tracks.size();

  for (unsigned short itrack = 0; itrack < N_vTracks; itrack += fvec::size()) {
    if (N_vTracks - itrack < static_cast<unsigned short>(fvec::size())) nTracks_SIMD = N_vTracks - itrack;

    for (int i = 0; i < nTracks_SIMD; i++) {
      tr[i] = &Tracks[itrack + i];  // current track
    }

    for (int iVec = 0; iVec < nTracks_SIMD; iVec++) {
      int nHitsTrackMvd = tr[iVec]->GetNofMvdHits();
      int nHits         = tr[iVec]->GetNofHits();
      for (int iH = 0; iH < 3; iH++) {
        float posx = 0.f, posy = 0.f, posz = 0.f;

        int hitNumber = nHits - iH - 1;
        if (hitNumber < nHitsTrackMvd) {
          if (!listMvdHits) continue;
          int hitIndex   = tr[iVec]->GetMvdHitIndex(hitNumber);
          CbmMvdHit* hit = L1_DYNAMIC_CAST<CbmMvdHit*>(listMvdHits->At(hitIndex));

          posx = hit->GetX();
          posy = hit->GetY();
          posz = hit->GetZ();
          ista = hit->GetStationNr();
        }
        else {
          if (!listStsHits) continue;
          int hitIndex   = tr[iVec]->GetHitIndex(hitNumber - nHitsTrackMvd);
          CbmStsHit* hit = L1_DYNAMIC_CAST<CbmStsHit*>(listStsHits->At(hitIndex));

          posx = hit->GetX();
          posy = hit->GetY();
          posz = hit->GetZ();
          ista = CbmStsSetup::Instance()->GetStationNumber(hit->GetAddress())
                 + NMvdStations;  //hit->GetStationNr()-1+NMvdStations;
        }

        sta[ista].fieldSlice.GetFieldValue(posx, posy, fB_temp);

        fB[iH].x[iVec]   = fB_temp.x[iVec];
        fB[iH].y[iVec]   = fB_temp.y[iVec];
        fB[iH].z[iVec]   = fB_temp.z[iVec];
        zField[iH][iVec] = posz;
      }
    }

    fld.Set(fB[0], zField[0], fB[1], zField[1], fB[2], zField[2]);
    for (int i = 0; i < nTracks_SIMD; i++) {
      field.emplace_back(fld, i);
    }
  }
}
