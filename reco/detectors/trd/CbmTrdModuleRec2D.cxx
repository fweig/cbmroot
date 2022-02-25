/* Copyright (C) 2018-2020 Horia Hulubei National Institute of Physics and Nuclear Engineering, Bucharest
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Alexandru Bercuci [committer] */

#include "CbmTrdModuleRec2D.h"

#include "CbmDigiManager.h"
#include "CbmTrdCluster.h"
#include "CbmTrdDigi.h"
#include "CbmTrdDigiRec.h"
#include "CbmTrdFASP.h"
#include "CbmTrdHit.h"
#include "CbmTrdParModDigi.h"

#include <FairLogger.h>

#include <TClonesArray.h>
#include <TF1.h>
#include <TGeoPhysicalNode.h>
#include <TGraphErrors.h>

#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

Double_t CbmTrdModuleRec2D::fgDT[]      = {4.181e-6, 1586, 24};
TGraphErrors* CbmTrdModuleRec2D::fgEdep = nullptr;
TGraphErrors* CbmTrdModuleRec2D::fgT    = nullptr;
TF1* CbmTrdModuleRec2D::fgPRF           = nullptr;
//_______________________________________________________________________________
CbmTrdModuleRec2D::CbmTrdModuleRec2D() : CbmTrdModuleRec() {}

//_______________________________________________________________________________
CbmTrdModuleRec2D::CbmTrdModuleRec2D(Int_t mod, Int_t ly, Int_t rot) : CbmTrdModuleRec(mod, ly, rot)
{
  SetNameTitle(Form("Trd2dReco%d", mod), "Reconstructor for triangular pads.");
  // printf("%s (%s)\n", GetName(), GetTitle()); Config(1,0);
}

//_______________________________________________________________________________
CbmTrdModuleRec2D::~CbmTrdModuleRec2D() {}

//_______________________________________________________________________________
Bool_t CbmTrdModuleRec2D::AddDigi(const CbmTrdDigi* d, Int_t id)
{
  /** Add digi to cluster fragments. At first clusters are ordered on pad rows and time. 
 * No channel ordering is assumed. The time condition for a digi to enter a cluster 
 * chunk is to have abs(dt)<5 wrt cluster t0 
 */

  if (CWRITE()) cout << "\nadd @" << id << " " << d->ToString();

  Int_t ch = d->GetAddressChannel(), col, row = GetPadRowCol(ch, col), dtime;
  Double_t t, r = d->GetCharge(t, dtime);
  Int_t tm = d->GetTimeDAQ() - fT0, terminator(0);
  if (dtime < 0) tm += dtime;  // correct for the difference between tilt and rect
  if (r < 1) terminator = 1;
  else if (t < 1)
    terminator = -1;

  if (CWRITE()) printf("row[%2d] col[%2d] tm[%2d] terminator[%d]\n", row, col, tm, terminator);
  CbmTrdCluster* cl(nullptr);

  // get the link to saved clusters
  std::map<Int_t, std::list<CbmTrdCluster*>>::iterator it = fBuffer.find(row);
  // check for saved
  if (it != fBuffer.end()) {
    Bool_t kINSERT(kFALSE);
    std::list<CbmTrdCluster*>::iterator itc = fBuffer[row].begin();
    for (; itc != fBuffer[row].end(); itc++) {
      //if (CWRITE()) cout << (*itc)->ToString();

      UShort_t tc = (*itc)->GetStartTime();
      Int_t dt    = tc - tm;

      if (dt < -5) continue;
      else if (dt < 5) {
        if (CWRITE()) printf("match time dt=%d\n", dt);
        if ((*itc)->IsChannelInRange(ch) == 0) {
          if (!(*itc)->AddDigi(id, ch, terminator, dt)) break;
          kINSERT = kTRUE;
          if (CWRITE()) cout << "          => Cl (Ad) " << (*itc)->ToString();
          break;
        }
      }
      else {
        if (CWRITE()) printf("break for time dt=%d\n", dt);
        break;
      }
    }

    if (!kINSERT) {
      if (itc != fBuffer[row].end() && itc != fBuffer[row].begin()) {
        itc--;
        fBuffer[row].insert(itc, cl = new CbmTrdCluster(fModAddress, id, ch, row, tm));
        if (CWRITE()) cout << "          => Cl (I) ";
      }
      else {
        fBuffer[row].push_back(cl = new CbmTrdCluster(fModAddress, id, ch, row, tm));
        if (CWRITE()) cout << "          => Cl (Pb) ";
      }
      cl->SetFaspDigis((d->GetType() == CbmTrdDigi::eCbmTrdAsicType::kFASP));
      if (terminator < 0) cl->SetProfileStart();
      else if (terminator > 0)
        cl->SetProfileStop();
      if (CWRITE()) cout << cl->ToString();
    }
  }
  else {
    fBuffer[row].push_back(cl = new CbmTrdCluster(fModAddress, id, ch, row, tm));
    cl->SetFaspDigis((d->GetType() == CbmTrdDigi::eCbmTrdAsicType::kFASP));
    if (terminator < 0) cl->SetProfileStart();
    else if (terminator > 0)
      cl->SetProfileStop();
    if (CWRITE()) cout << "          => Cl (Nw) " << cl->ToString();
  }

  return kTRUE;
}

//_______________________________________________________________________________
Int_t CbmTrdModuleRec2D::GetOverThreshold() const
{
  Int_t nch(0);
  for (std::map<Int_t, std::list<CbmTrdCluster*>>::const_iterator ir = fBuffer.cbegin(); ir != fBuffer.cend(); ir++) {
    for (std::list<CbmTrdCluster*>::const_iterator itc = (*ir).second.cbegin(); itc != (*ir).second.cend(); itc++)
      nch += (*itc)->GetNCols();
  }
  return nch;
}

//_______________________________________________________________________________
Int_t CbmTrdModuleRec2D::FindClusters()
{
  CbmTrdCluster* cl(nullptr);

  // get the link to saved clusters
  Int_t ncl(0);
  std::list<CbmTrdCluster*>::iterator itc0, itc1;
  for (std::map<Int_t, std::list<CbmTrdCluster*>>::iterator ir = fBuffer.begin(); ir != fBuffer.end(); ir++) {
    itc0 = (*ir).second.begin();
    while ((*ir).second.size() > 1 && itc0 != (*ir).second.end()) {  // try merge clusters
      itc1 = itc0;
      itc1++;

      Bool_t kMERGE = kFALSE;
      while (itc1 != (*ir).second.end()) {
        if (CWRITE()) cout << "  base cl[0] : " << (*itc0)->ToString() << "     + cl[1] : " << (*itc1)->ToString();
        if ((*itc0)->Merge((*itc1))) {
          if (CWRITE()) cout << "  SUM        : " << (*itc0)->ToString();
          kMERGE = kTRUE;
          delete (*itc1);
          itc1 = (*ir).second.erase(itc1);
          if (itc1 == (*ir).second.end()) break;
        }
        else
          itc1++;
      }
      if (!kMERGE) itc0++;
    }

    for (itc0 = (*ir).second.begin(); itc0 != (*ir).second.end(); itc0++) {
      cl = (*itc0);
      cl = new ((*fClusters)[ncl++]) CbmTrdCluster(*cl);
      cl->SetFaspDigis((*itc0)->HasFaspDigis());
      delete (*itc0);
    }
  }
  fBuffer.clear();

  // printf("fClusters[%p] nCl[%d]\n", (void*)fClusters, fClusters->GetEntriesFast());
  // LOG(info) << GetName() << "::FindClusters : " << ncl;
  return ncl;
}

//_______________________________________________________________________________
Bool_t CbmTrdModuleRec2D::MakeHits() { return kTRUE; }

//_______________________________________________________________________________
Bool_t CbmTrdModuleRec2D::PreProcessHits()
{
  /** Steering routine for classifying hits and apply further analysis
 * -> hit deconvolution (see Deconvolute)
 */

  Int_t nhits = fHits->GetEntriesFast();
  if (CWRITE()) LOG(info) << GetName() << "::PreProcessHits(" << nhits << ")";

  CbmTrdHit* hit(nullptr);
  for (Int_t ih(0); ih < nhits; ih++) {
    hit = (CbmTrdHit*) ((*fHits)[ih]);
    if (!CheckConvolution(hit)) continue;
    nhits += Deconvolute(hit);
  }
  nhits = fHits->GetEntriesFast();
  if (CWRITE()) LOG(info) << GetName() << "::Deconvolute(" << nhits << ")";
  return kTRUE;
}

//_______________________________________________________________________________
Bool_t CbmTrdModuleRec2D::CheckConvolution(CbmTrdHit* /*h*/) const { return kFALSE; }

//_______________________________________________________________________________
Bool_t CbmTrdModuleRec2D::Deconvolute(CbmTrdHit* /*h*/) { return kFALSE; }

//_______________________________________________________________________________
Bool_t CbmTrdModuleRec2D::PostProcessHits()
{
  /**  Steering routine for classifying hits and apply further analysis
 * -> hit merging for row-cross (see RowCross)
 */

  CbmTrdHit *h0(nullptr), *h1(nullptr);

  Int_t a0, nhits = fHits->GetEntriesFast();
  Float_t Dx(2 * fDigiPar->GetPadSizeX(0)), Dy(2 * fDigiPar->GetPadSizeY(0));
  for (Int_t ih(0); ih < nhits; ih++) {
    h0 = (CbmTrdHit*) ((*fHits)[ih]);
    if (h0->IsUsed()) continue;

    for (Int_t jh(ih + 1); jh < nhits; jh++) {
      h1 = (CbmTrdHit*) ((*fHits)[jh]);
      if (h1->IsUsed()) continue;

      // basic check on Row
      if (TMath::Abs(h1->GetY() - h0->GetY()) > Dy) continue;

      // basic check on Col
      if (TMath::Abs(h1->GetX() - h0->GetX()) > Dx) continue;

      // basic check on Time
      if (TMath::Abs(h1->GetTime() - h0->GetTime()) > 4000) continue;  // TODO check with preoper time calibration

      // go to topologic checks
      if (!(a0 = CheckMerge(h0->GetRefId(), h1->GetRefId()))) continue;

      ProjectDigis(a0 < 0 ? h0->GetRefId() : h1->GetRefId(), a0 < 0 ? h1->GetRefId() : h0->GetRefId());

      // call the working algorithm
      if (MergeHits(h0, a0)) h0->SetRowCross(h1);
      if (CWRITE()) {
        cout << ih << " : " << h0->ToString();
        cout << jh << " : " << h1->ToString();
        cout << "\n" << endl;
      }
    }
  }
  nhits = 0;
  for (Int_t ih(0); ih < fHits->GetEntriesFast(); ih++) {
    h0 = (CbmTrdHit*) ((*fHits)[ih]);
    if (!h0->IsUsed()) continue;
    fHits->RemoveAt(ih);  //delete h0;
    nhits++;
  }
  fHits->Compress();

  // clear all calibrated digis
  for (map<Int_t, vector<CbmTrdDigiRec*>>::iterator ic = fDigis.begin(); ic != fDigis.end(); ic++) {
    for (vector<CbmTrdDigiRec*>::iterator id = ic->second.begin(); id != ic->second.end(); id++)
      delete *id;
    ic->second.clear();
  }
  fDigis.clear();

  if (CWRITE()) LOG(info) << GetName() << "::MergeHits(" << nhits << ")";
  return kTRUE;
}


//_______________________________________________________________________________
Int_t CbmTrdModuleRec2D::CheckMerge(Int_t cid, Int_t cjd)
{
  /** Check topologic conditions if the 2 clusters (in digi representation) can be merged.
 * The first pair is always from the bottom row
 * return the anode candidate wrt boundary 1, 2, 3 for the first 3 anodes of the upper row; -1, -2, -3 for the bottom row (r0) or 0 if the check fails  
 */

  Bool_t on(kFALSE);
  Int_t /*row, */ col, rowMax(0), vc[2] = {-1, -1}, vm[2] = {0}, vcid[2] = {cid, cjd};
  Double_t t(0.), r(0.), rtMax(0.), T(0.), m, d, mdMax(0.), M[2] = {-1., -1.}, S[2] = {0.};
  vector<CbmTrdDigiRec*>::iterator id, jd, jp[2];
  for (Int_t rowId(0); rowId < 2; rowId++) {
    rtMax = 0.;
    mdMax = 0.;
    for (id = fDigis[vcid[rowId]].begin(); id != fDigis[vcid[rowId]].end(); id++) {
      GetPadRowCol((*id)->GetAddressChannel(), col);
      //cout<<(*id)->ToString();

      // mark max position and type
      t = (*id)->GetTiltCharge(on);
      if (on && t > rtMax) {
        vc[rowId] = col;
        vm[rowId] = 0;
        rtMax     = t;
      }
      r = (*id)->GetRectCharge(on);
      if (on && r > rtMax) {
        vc[rowId] = col;
        vm[rowId] = 1;
        rtMax     = r;
      }

      m = 0.;
      d = 0.;
      if (!rowId) {  // compute TR pairs on the bottom row
        m = 0.5 * (t + r);
        d = r - t;
      }
      else {  // compute RT pairs on the upper row
        jd = id + 1;
        T  = 0.;
        if (jd != fDigis[vcid[rowId]].end()) T = (*jd)->GetTiltCharge(on);

        m = 0.5 * (r + T);
        d = r - T;
      }
      if (TMath::Abs(m) > 0.) d = 1.e2 * d / m;
      if (m > mdMax) {
        mdMax     = m;
        M[rowId]  = m;
        S[rowId]  = d;
        jp[rowId] = id;
        rowMax    = rowId;
      }
    }
  }
  rowMax = M[0] > M[1] ? 0 : 1;

  // basic check on col of the max signal
  Int_t dc = vc[1] - vc[0];
  if (dc < 0 || dc > 1) return 0;

  // special care for both tilt maxima :  the TT case
  // recalculate values on the min row on neighbor column
  if (!vm[0] && !vm[1]) {
    if (rowMax == 0) {  // modify r=1
      r = T = 0.;
      if (M[1] >= 0) {
        if (jp[1] != fDigis[cjd].end()) jp[1]++;
        if (jp[1] != fDigis[cjd].end()) {
          r = (*jp[1])->GetRectCharge(on);
          jp[1]++;
          if (jp[1] != fDigis[cjd].end()) T = (*jp[1])->GetTiltCharge(on);
        }
      }
      M[1] = 0.5 * (r + T);
      S[1] = r - T;
    }
    else {  // modify r=0
      r = t = 0.;
      if (M[0] >= 0) {
        if (jp[0] != fDigis[cid].begin()) jp[0]--;
        if (jp[0] != fDigis[cid].begin()) {
          r = (*jp[0])->GetRectCharge(on);
          t = (*jp[0])->GetTiltCharge(on);
        }
      }
      M[0] = 0.5 * (t + r);
      S[0] = r - t;
    }
  }
  rowMax = M[0] > M[1] ? 0 : 1;

  // Build the ratio of the charge
  Float_t mM = M[rowMax ? 0 : 1] / M[rowMax];

  //   Float_t mM_c[] = {0.03, 0.243, 0.975},  //  center of distribution for each anode hypo
  //           mM_s[] = {0., 1.7e-3, 8.8e-3},  //  slope of distribution for each anode hypo
  //           mM_r[] = {0.03,  0.03,  0.01};  //  range of distribution for each anode hypo in proper coordinates
  //   for(Int_t ia(0); ia<3; ia++)
  //     if(TMath::Abs(mM_s[ia] * S[rowMax] + mM_c[ia] - mM) < mM_r[ia] ) return (rowMax?1:-1) * (3-ia);

  Float_t mS = TMath::Abs(S[rowMax]), mM_l[3] = {0.15, 0.5, 1}, mM_r[3] = {0, 0.28, 1}, mS_r[3] = {43, 27, 20}, dSdM[2],
          S0[2];
  for (Int_t i(0); i < 2; i++) {
    dSdM[i] = (mS_r[i + 1] - mS_r[i]) / (mM_r[i + 1] - mM_r[i]);
    S0[i]   = mS_r[i] - dSdM[i] * mM_r[i];
  }
  Int_t irange = mM < mM_r[1] ? 0 : 1;
  if (mS > S0[irange] + dSdM[irange] * mM) return 0;

  for (Int_t ia(0); ia < 3; ia++) {
    if (mM < mM_l[ia]) return (rowMax ? 1 : -1) * (3 - ia);
  }
  return 0;
}


//_______________________________________________________________________________
Bool_t CbmTrdModuleRec2D::MergeHits(CbmTrdHit* h, Int_t a0)
{
  Int_t n0(vs.size() - 2);
  Double_t dx(0.), dy(0.);

  switch (n0) {
    case 1:
      if (IsMaxTilt()) {  // T
        dx = -0.5;
        dy = 0;
      }
      else {  // R
        dx = 0.5;
        dy = 0;
      }
      break;
    case 2:
      if (IsOpenLeft() && IsOpenRight()) {  // RT
        dx = viM == 1 ? 0. : -1;
        dy = -0.5;
      }
      else {  // TR
        dx = 0.;
        dy = 0.5;
      }
      break;
    case 3:
      if (IsMaxTilt() && !IsSymmHit()) {  // TRT asymm
        dx = viM == 1 ? 0. : -1;
        dy = GetYoffset();
      }
      else if (!IsMaxTilt() && IsSymmHit()) {  // TRT symm
        dx = 0.;
        dy = GetYoffset();
      }
      else if (IsMaxTilt() && IsSymmHit()) {  // RTR symm
        dx = GetXoffset();
        dy = 0.;
      }
      else if (!IsMaxTilt() && !IsSymmHit()) {  // RTR asymm
        dx = GetXoffset();
        dy = viM == 1 ? -0.5 : 0.5;
      }
      break;
    default:
      dx = GetXoffset();
      dy = GetYoffset();
      break;
  }

  RecenterXoffset(dx);
  Int_t typ(GetHitClass());
  // get position correction [pw]
  Double_t xcorr = GetXcorr(dx, typ, 1) / fDigiPar->GetPadSizeX(0), xcorrBias(xcorr);
  if (IsBiasX()) {
    typ        = GetHitRcClass(a0);
    Int_t xmap = vyM & 0xff;
    switch (n0) {
      case 4:
        if (dx < 0) xcorrBias += (IsBiasXleft() ? -0.12 : 0.176);
        else
          xcorrBias += (xmap == 53 || xmap == 80 || xmap == 113 || xmap == 117 ? -0.176 : 0.12);
        break;
      case 5:
      case 7:
        if (typ < 0) break;
        if (xmap == 50 || xmap == 58 || xmap == 146 || xmap == 154) {
          if (typ == 2) xcorr += 0.0813;
          else if (typ == 3) {
            xcorr -= 0.0813;
            typ = 2;
          }
          dx -= xcorr;
          RecenterXoffset(dx);
          xcorrBias = GetXcorr(dx, typ, 2) / fDigiPar->GetPadSizeX(0);
        }
        else {
          dx -= xcorr;
          RecenterXoffset(dx);
          if (typ < 2) xcorrBias = GetXcorr(dx, typ, 2) / fDigiPar->GetPadSizeX(0);
          else if (typ == 2)
            xcorrBias = 0.12;
          else if (typ == 3)
            xcorrBias = -0.12;
        }
        break;
      default:
        if (typ < 0) break;
        else if (typ == 2)
          xcorr += 0.0813;
        else if (typ == 3) {
          xcorr -= 0.0813;
          typ = 2;
        }
        dx -= xcorr;
        RecenterXoffset(dx);
        xcorrBias = GetXcorr(dx, typ, 2) / fDigiPar->GetPadSizeX(0);
        break;
    }
  }
  else {
    if (typ) xcorrBias += (dx < 0 ? 1 : -1) * 0.0293;
  }
  dx -= xcorrBias;
  RecenterXoffset(dx);
  dy = dx - dy;
  RecenterYoffset(dy);
  if (dy < -0.5 || dy > 0.5) printf("!!! dy = %f r[%d]\n", dy, vrM);

  Double_t ycorr = GetYcorr(dy) / fDigiPar->GetPadSizeY(0);
  dy += ycorr;
  RecenterYoffset(dy);
  dx *= fDigiPar->GetPadSizeX(0);
  dy *= fDigiPar->GetPadSizeY(0);

  TVector3 local_pad, local_pad_err;
  Int_t srow, sector = fDigiPar->GetSectorRow(vrM, srow);
  fDigiPar->GetPadPosition(sector, vcM, srow, local_pad, local_pad_err);

  Double_t edx(1), edy(1), edt(60), time(-21), tdrift(100), e(200);
  Double_t local[3] = {local_pad[0] + dx, local_pad[1] + dy, local_pad[2]}, global[3];
  // globalErr[3] = {0/*edx*/, 0/*edy*/, 0.};
  LocalToMaster(local, global);
  h->SetX(global[0]);
  h->SetY(global[1]);
  h->SetZ(global[2]);
  h->SetDx(edx);
  h->SetDy(edy);
  h->SetDz(0.);
  h->SetDxy(0.);
  h->SetTime(CbmTrdDigi::Clk(CbmTrdDigi::eCbmTrdAsicType::kFASP) * (vt0 + time) - tdrift + 30.29, edt);
  h->SetELoss(e);
  // hit->SetClassType();
  // hit->SetMaxType(tM);
  // if(ovf) hit->SetOverFlow();

  if (CWRITE()) {
    printf("-> loc[%6.2f %6.2f %6.2f] err[%6.2f %6.2f %6.2f]\n", local_pad[0], local_pad[1], local_pad[2],
           local_pad_err[0], local_pad_err[1], local_pad_err[2]);
    printf("REC col[%2d] row[%2d] dx[%7.3f(pw) %7.3f(cm)] x[%7.2f] y[%7.2f] dy[%5.2f] t0[%llu]\n", vcM, vrM,
           dx / fDigiPar->GetPadSizeX(0), dx, global[0], global[1], dy, vt0);
  }

  return kTRUE;
}

//_______________________________________________________________________________
Bool_t CbmTrdModuleRec2D::BuildHit(CbmTrdHit* h)
{
  Int_t n0(vs.size() - 2);
  Double_t dx(0.), dy(0.);  //, da(0.);

  switch (n0) {
    case 1:
      if (IsMaxTilt()) {  // T
        dx = -0.5;
        dy = 0;
      }
      else {  // R
        dx = 0.5;
        dy = 0;
      }
      break;
    case 2:
      if (IsOpenLeft() && IsOpenRight()) {  // RT
        dx = viM == 1 ? 0. : -1;
        dy = -0.5;
      }
      else {  // TR
        dx = 0.;
        dy = 0.5;
      }
      break;
    case 3:
      if (IsMaxTilt() && !IsSymmHit()) {  // TRT asymm
        dx = viM == 1 ? 0. : -1;
        dy = GetYoffset();
      }
      else if (!IsMaxTilt() && IsSymmHit()) {  // TRT symm
        dx = 0.;
        dy = GetYoffset();
      }
      else if (IsMaxTilt() && IsSymmHit()) {  // RTR symm
        dx = GetXoffset();
        dy = 0.;
      }
      else if (!IsMaxTilt() && !IsSymmHit()) {  // RTR asymm
        dx = GetXoffset();
        dy = viM == 1 ? -0.5 : 0.5;
      }
      break;
    default:
      dx = GetXoffset();
      dy = GetYoffset();
      break;
  }
  RecenterXoffset(dx);

  // get position correction
  Double_t xcorr = GetXcorr(dx, GetHitClass()) / fDigiPar->GetPadSizeX(0);
  dx -= xcorr;
  RecenterXoffset(dx);
  dy = dx - dy;
  RecenterYoffset(dy);
  if (dy < -0.5 || dy > 0.5) printf("!!! dy = %f r[%d]\n", dy, vrM);

  Double_t ycorr = GetYcorr(dy) / fDigiPar->GetPadSizeY(0);
  dy += ycorr;
  RecenterYoffset(dy);
  dx *= fDigiPar->GetPadSizeX(0);
  dy *= fDigiPar->GetPadSizeY(0);

  // get anode wire offset
  Int_t ia(0);
  Float_t ya(0.);  //  anode position in local pad coordinates
  for (; ia <= NANODE; ia++) {
    ya = -1.35 + ia * 0.3;
    if (dy > ya + 0.15) continue;
    break;
  }
  //  da = dy-ya;
  //   //correct inside apmlification region
  //   da*=-0.7;
  //   if(da<-0.015) da+=0.1;
  //   else if(da>0.015) da-=0.1;
  //   dy+=da;
  //   da = dy - ya;

  // Error parametrization X : parabolic model on cl size
  Double_t parX[] = {0.713724, -0.318667, 0.0366036};
  Double_t parY[] = {0.0886413, 0., 0.0435141};
  Int_t nex       = TMath::Min(n0, 7);
  Double_t edx = parX[0] + parX[1] * nex + parX[2] * nex * nex, edy = parY[0] + parY[2] * dy * dy,
           edt = 26.33;  // should be parametrized as function of da TODO
  // use this trick to force larger roads on CbmLitTrackFinderBranch
  // target code from CbmLitTrackFinderBranch::FollowTracks and CbmLitHitData::AddHit
  // bool hitInside = (pixelHit->GetX() < (tpar.GetX() + devX)) && (pixelHit->GetX() > (tpar.GetX() - devX))
  if (n0 < 3) {
    edx = 1.;
    edy = 1.;
    edt = 60.;
  }

  TVector3 local_pad, local_pad_err;
  Int_t srow, sector = fDigiPar->GetSectorRow(vrM, srow);
  fDigiPar->GetPadPosition(sector, vcM, srow, local_pad, local_pad_err);

  Double_t local[3] = {local_pad[0] + dx, local_pad[1] + dy, local_pad[2]}, global[3];
  //globalErr[3] = {edx, edy, 0.};
  LocalToMaster(local, global);

  // COMPUTE TIME
  for (Int_t idx(1); idx <= n0; idx++) {
    Double_t dtFEE =
      fgDT[0] * (vs[idx] - fgDT[1]) * (vs[idx] - fgDT[1]) / CbmTrdDigi::Clk(CbmTrdDigi::eCbmTrdAsicType::kFASP);
    if (vxe[idx] > 0) vx[idx] += dy / fDigiPar->GetPadSizeY(0);
    fgT->SetPoint(idx - 1, vx[idx], vt[idx] - dtFEE);
  }
  Double_t xc = vx[n0 + 2];
  for (Int_t ip(n0); ip < fgT->GetN(); ip++) {
    fgT->SetPoint(ip, xc, 0);
    xc += 0.5;
  }
  Double_t time(-21.), tdrift(100);  // should depend on Ua
  if (n0 > 1 && (fgT->Fit("pol1", "QC", "goff") == 0)) {
    TF1* f = fgT->GetFunction("pol1");
    time   = f->GetParameter(0) - fgDT[2];
    if (TMath::IsNaN(time)) time = -21;
    //dtime += TMath::Abs(f->GetParameter(1)*(vx[n0+1] - vx[1]));
  }

  // COMPUTE ENERGY
  for (UInt_t idx(0); idx < vs.size(); idx++) {
    if (vxe[idx] > 0) {
      fgEdep->SetPoint(idx, vx[idx] + dy / fDigiPar->GetPadSizeY(0), vs[idx]);
      fgEdep->SetPointError(idx, vxe[idx], vse[idx]);
    }
    else {
      fgEdep->SetPoint(idx, vx[idx], vs[idx]);
      fgEdep->SetPointError(idx, vxe[idx], vse[idx]);
    }
  }
  xc = vx[n0 + 2];
  for (Int_t ip(vs.size()); ip < fgEdep->GetN(); ip++) {
    //fgEdep->RemovePoint(ip);
    xc += 0.5;
    fgEdep->SetPoint(ip, xc, 0);
    fgEdep->SetPointError(ip, 0., 300);
  }
  //if(CWRITE()) fgEdep->Print();

  Double_t e(0.), xlo(*vx.begin()), xhi(*vx.rbegin());
  fgPRF->SetParameter(0, vs[viM]);
  fgPRF->FixParameter(1, dx / fDigiPar->GetPadSizeX(0));
  fgPRF->SetParameter(2, 0.65);
  fgPRF->SetParLimits(2, 0.45, 10.5);
  fgEdep->Fit(fgPRF, "QBN", "goff", xlo - 0.5, xhi + 0.5);
  if (!fgPRF->GetNDF()) return nullptr;
  //chi = fgPRF->GetChisquare()/fgPRF->GetNDF();
  e = fgPRF->Integral(xlo - 0.5, xhi + 0.5);

  // apply MC correction
  Float_t gain0 = 210.21387;  //(XeCO2 @ 1900V)
  //   Float_t grel[3] = {1., 0.98547803, 0.93164071},
  //           goff[3][3] = {
  //             {0.05714, -0.09, -0.09},
  //             {0., -0.14742, -0.14742},
  //             {0., -0.29, -0.393}
  //           };
  //   Int_t ian=0;
  //   if(TMath::Abs(dy)<=0.3) ian=0;
  //   else if(TMath::Abs(dy)<=0.6) ian=1;
  //   else if(TMath::Abs(dy)<=0.9) ian=2;
  //   Int_t isize=0;
  //   if(n0<=3) isize=0;
  //   else if(n0<=4) isize=1;
  //   else isize=2;
  Float_t gain = gain0;  //*grel[ian];
  e /= gain;             // apply effective gain
  //e+=goff[ian][isize];  // apply missing energy offset

  h->SetX(global[0]);
  h->SetY(global[1]);
  h->SetZ(global[2]);
  h->SetDx(edx);
  h->SetDy(edy);
  h->SetDz(0);
  h->SetDxy(0.);
  h->SetTime(CbmTrdDigi::Clk(CbmTrdDigi::eCbmTrdAsicType::kFASP) * (vt0 + time) - tdrift + 30.29, edt);
  h->SetELoss(e);
  // hit->SetClassType();
  // hit->SetMaxType(tM);
  // if(ovf) hit->SetOverFlow();

  if (CWRITE()) {
    printf("-> loc[%6.2f %6.2f %6.2f] err[%6.2f %6.2f %6.2f]\n", local_pad[0], local_pad[1], local_pad[2],
           local_pad_err[0], local_pad_err[1], local_pad_err[2]);
    printf("REC col[%2d] row[%2d] x[%7.2f] dx[%5.2f] y[%7.2f] dy[%5.2f] t0[%llu]\n", vcM, vrM, global[0], dx, global[1],
           dy, vt0);
  }

  return kTRUE;
}

#include <TCanvas.h>
#include <TH1.h>
//_______________________________________________________________________________
CbmTrdHit* CbmTrdModuleRec2D::MakeHit(Int_t ic, const CbmTrdCluster* cl, std::vector<const CbmTrdDigi*>* digis)
{
  if (!cl->HasFaspDigis()) {
    LOG(debug) << GetName() << "::MakeHit: Hybrid cluster SPADIC/Trd2d. Skipped for the moment.";
    // std::cout << cl->ToString();
    return nullptr;
  }
  //printf("%s (%s)\n", GetName(), GetTitle()); Config(1,0);

  if (!fgEdep) {  // first use initialization of PRF helppers
    LOG(info) << GetName() << "::MakeHit: Init static helpers. ";
    fgEdep = new TGraphErrors;
    fgEdep->SetLineColor(kBlue);
    fgEdep->SetLineWidth(2);
    fgT = new TGraphErrors;
    fgT->SetLineColor(kBlue);
    fgT->SetLineWidth(2);
    fgPRF = new TF1("prf", "[0]*exp(-0.5*((x-[1])/[2])**2)", -10, 10);
    fgPRF->SetLineColor(kRed);
    fgPRF->SetParNames("E", "x", "prf");
  }
  TH1* hf(nullptr);
  TCanvas* cvs(nullptr);
  if (CDRAW()) {
    cvs = new TCanvas("c", "TRD Anode Hypothesis", 10, 600, 1000, 500);
    cvs->Divide(2, 1, 1.e-5, 1.e-5);
    TVirtualPad* vp = cvs->cd(1);
    vp->SetLeftMargin(0.06904908);
    vp->SetRightMargin(0.009969325);
    vp->SetTopMargin(0.01402806);
    vp = cvs->cd(2);
    vp->SetLeftMargin(0.06904908);
    vp->SetRightMargin(0.009969325);
    vp->SetTopMargin(0.01402806);
    hf = new TH1I("hf", ";x [pw];s [ADC chs]", 100, -3, 3);
    hf->GetYaxis()->SetRangeUser(-50, 4500);
    hf->Draw("p");
  }

  if (CWRITE()) cout << cl->ToString();
  if (!LoadDigis(digis, ic)) return nullptr;
  if (!ProjectDigis(ic)) return nullptr;
  Int_t nofHits  = fHits->GetEntriesFast();
  CbmTrdHit* hit = new ((*fHits)[nofHits]) CbmTrdHit();
  hit->SetAddress(fModAddress);
  hit->SetRefId(ic);
  //hit->SetMatch();
  BuildHit(hit);
  if (CWRITE()) cout << hit->ToString();
  if (CDRAW()) DrawHit(hit);
  return hit;
}

//_______________________________________________________________________________
Int_t CbmTrdModuleRec2D::GetHitClass() const
{
  /** Incapsulate hit classification criteria based on signal topology
 * [0] : center hit type
 * [1]  : side hit type
 */

  Int_t n0(vs.size() - 2);
  if ((n0 == 5 && ((IsMaxTilt() && IsSymmHit()) || (!IsMaxTilt() && !IsSymmHit()))) ||  // TRTRT symm/asymm
      n0 == 4 || (n0 == 3 && ((IsMaxTilt() && IsSymmHit()) || (!IsMaxTilt() && !IsSymmHit()))))
    return 1;  // RTR symm/asymm
  else if (n0 > 5 && HasOvf())
    return 2;
  return 0;
}

//_______________________________________________________________________________
Int_t CbmTrdModuleRec2D::GetHitRcClass(Int_t a0) const
{
  Int_t a0m    = TMath::Abs(a0);
  UChar_t xmap = vyM & 0xff;
  if (a0m == 2 && IsBiasXleft() && IsBiasXright() && !IsBiasXmid()) return 0;
  else if (a0m == 3 && ((IsBiasXleft() && IsBiasXright()) || xmap == 116 || xmap == 149 || xmap == 208))
    return 1;
  else if (!IsBiasXleft()
           && (a0m == 2
               || (a0m == 3 && ((!IsBiasXright() && IsBiasXmid()) || xmap == 209 || xmap == 212 || xmap == 145))))
    return 2;
  else if (!IsBiasXright()
           && (a0m == 2 || (a0m == 3 && ((!IsBiasXleft() && IsBiasXmid()) || xmap == 112 || xmap == 117))))
    return 3;
  else
    return -1;
}

//_______________________________________________________________________________
Double_t CbmTrdModuleRec2D::GetXoffset(Int_t n0) const
{
  Double_t dx(0.), R(0.);
  Int_t n(n0 ? n0 : vs.size());
  for (Int_t ir(0); ir < n; ir++) {
    if (vxe[ir] > 0) continue;  // select rectangular coupling
    R += vs[ir];
    dx += vs[ir] * vx[ir];
  }
  if (TMath::Abs(R) > 0) return dx / R;
  LOG(warn) << GetName() << "::GetXoffset : Unexpected null sum.";
  return 0.;
}

//_______________________________________________________________________________
Double_t CbmTrdModuleRec2D::GetYoffset(Int_t n0) const
{
  Double_t dy(0.), T(0.);
  Int_t n(n0 ? n0 : vs.size());
  for (Int_t it(0); it < n; it++) {
    if (vxe[it] > 0) {  // select tilted coupling
      T += vs[it];
      dy += vs[it] * vx[it];
    }
  }
  if (TMath::Abs(T) > 0) return dy / T;
  LOG(warn) << GetName() << "::GetYoffset : Unexpected null sum.";
  return 0.;
}

//_______________________________________________________________________________
Double_t CbmTrdModuleRec2D::GetXcorr(Double_t dxIn, Int_t typ, Int_t cls) const
{
  /** Give the linear interpolation of SYS correction for current position offset "dx" based 
 * on LUT calculated wrt MC EbyE data. The position offset is expresed in [pw] units 
 * while the output is in [cm]
 */

  if (typ < 0 || typ > 2) {
    //LOG(error)<< GetName() << "::GetXcorr : type in-param "<<typ<<" out of range.";
    return 0;
  }
  Double_t dx = TMath::Abs(dxIn);
  Int_t ii    = TMath::Max(0, TMath::Nint(dx / fgCorrXdx) - 1), i0;  //  i1;

  if (ii < 0 || ii > NBINSCORRX) {
    LOG(warn) << GetName() << "::GetXcorr : LUT idx " << ii << " out of range for dx=" << dxIn;
    return 0;
  }
  if (dx < fgCorrXdx * ii) {
    i0 = TMath::Max(0, ii - 1);
    /*i1=ii;*/
  }
  else {
    i0 = ii;
    /*i1=TMath::Min(NBINSCORRX-1,ii+1);*/
  }

  Float_t* xval = &fgCorrXval[typ][i0];
  if (cls == 1) xval = &fgCorrRcXval[typ][i0];
  else if (cls == 2)
    xval = &fgCorrRcXbiasXval[typ][i0];
  Double_t DDx = (xval[1] - xval[0]), a = DDx / fgCorrXdx, b = xval[0] - DDx * (i0 + 0.5);
  return (dxIn > 0 ? 1 : -1) * b + a * dxIn;
}

//_______________________________________________________________________________
Double_t CbmTrdModuleRec2D::GetYcorr(Double_t dy, Int_t /* cls*/) const
{
  /** Process y offset. Apply systematic correction for y (MC derived).
 * The position offset is expresed in [pw] units while the output is in [cm]
 */
  Float_t fdy(1.), yoff(0.);
  Int_t n0(vs.size() - 2);
  switch (n0) {
    case 3:
      fdy  = fgCorrYval[0][0];
      yoff = fgCorrYval[0][1];
      if (IsMaxTilt() && IsSymmHit()) {
        fdy  = 0.;
        yoff = (dy > 0 ? -1 : 1) * 1.56;
      }
      else if (!IsMaxTilt() && !IsSymmHit()) {
        fdy  = 0.;
        yoff = (dy > 0 ? -1 : 1) * 1.06;
      }
      else if (!IsMaxTilt() && IsSymmHit()) {
        fdy  = 2.114532;
        yoff = -0.263;
      }
      else /*if(IsMaxTilt()&&!IsSymmHit())*/ {
        fdy  = 2.8016010;
        yoff = -1.38391;
      }
      break;
    case 4:
      fdy  = fgCorrYval[1][0];
      yoff = fgCorrYval[1][1];
      if ((!IsMaxTilt() && IsLeftHit()) || (IsMaxTilt() && !IsLeftHit())) yoff *= -1;
      break;
    case 5:
    case 7:
    case 9:
    case 11:
      fdy  = fgCorrYval[2][0];
      yoff = fgCorrYval[2][1];
      break;
    case 6:
    case 8:
    case 10:
      fdy  = fgCorrYval[3][0];
      yoff = fgCorrYval[3][1];
      if ((!IsMaxTilt() && IsLeftHit()) || (IsMaxTilt() && !IsLeftHit())) yoff *= -1;
      break;
  }
  return dy * fdy + yoff;
}

//_______________________________________________________________________________
void CbmTrdModuleRec2D::RecenterXoffset(Double_t& dx)
{
  /** Shift graph representation to fit dx[pw] in [-0.5, 0.5]
   */

  if (dx >= -0.5 && dx < 0.5) return;
  Int_t ishift = Int_t(dx - 0.5) + (dx > 0.5 ? 1 : 0);
  if (vcM + ishift < 0) ishift = -vcM;
  else if (vcM + ishift >= GetNcols())
    ishift = GetNcols() - vcM - 1;
  LOG(debug) << GetName() << "::RecenterXoffset : shift dx offset by " << ishift << " from dx=" << dx << " to "
             << dx - ishift << " center col from " << (Int_t) vcM << " to " << Int_t(vcM + ishift);
  dx -= ishift;
  vcM += ishift;
  for (UInt_t idx(0); idx < vx.size(); idx++)
    vx[idx] -= ishift;
}


//_______________________________________________________________________________
void CbmTrdModuleRec2D::RecenterYoffset(Double_t& dy)
{
  /** Shift graph representation to fit dy[ph] in [-0.5, 0.5]
   */

  if (dy >= -0.5 && dy < 0.5) return;
  Int_t ishift = Int_t(dy - 0.5) + (dy > 0.5 ? 1 : 0);
  //   if(vrM+ishift < 0) ishift = - vrM;
  //   else if(vrM+ishift >= GetNrows()) ishift = GetNrows() - vrM -1;
  LOG(debug) << GetName() << "::RecenterYoffset : shift dy offset by " << ishift << " from dy=" << dy << " to "
             << dy - ishift;
  dy -= ishift;
  //vrM+= ishift;
  //   if(vrM==0 && dy<-0.5) dy=-0.5;
  //   if(vrM==GetNrows() -1 && dy>0.5) dy=0.5;
}

//_______________________________________________________________________________
Int_t CbmTrdModuleRec2D::LoadDigis(vector<const CbmTrdDigi*>* din, Int_t cid)
{
  /** Load RAW digis info in calibration aware strucuture CbmTrdDigiReco
 * Do basic sanity checks; also incomplete adjacent digi and if found merge them.
 */
  Int_t col(-1), /*row, */ colT(-1), colR(-1);
  const CbmTrdDigi *dgT(nullptr), *dgR(nullptr);
  for (vector<const CbmTrdDigi*>::iterator i = din->begin(), j = i + 1; i != din->end(); i++) {
    dgT = (*i);
    //row =
    GetPadRowCol(dgT->GetAddressChannel(), colT);
    // check column order for cluster
    if (col >= 0 && colT != col + 1) {
      LOG(error) << GetName() << "::LoadDigis : digis in cluster " << cid << " not in increasing order !";
      return 0;
    }
    col  = colT;
    colR = -1;
    dgR  = nullptr;
    if (j != din->end()) {
      dgR = (*j);
      //row =
      GetPadRowCol(dgR->GetAddressChannel(), colR);
    }
    if (colR == colT) {
      fDigis[cid].push_back(new CbmTrdDigiRec(*dgT, *dgR));
      j = din->erase(j);
    }
    else
      fDigis[cid].push_back(new CbmTrdDigiRec(*dgT));

    if (j != din->end()) j++;
  }
  return fDigis[cid].size();
}

//_______________________________________________________________________________
Int_t CbmTrdModuleRec2D::ProjectDigis(Int_t cid, Int_t cjd)
{
  /** Load digis information in working vectors Digis are represented in the normal coordinate system of
   * (pad width [pw], DAQ time [clk], signal [ADC chs]) with rectangular signals at integer 
   * positions.  
   */

  if (fDigis.find(cid) == fDigis.end()) {
    LOG(warn) << GetName() << "::ProjectDigis : Request cl id " << cid << " not found.";
    return 0;
  }

  vs.clear();
  vse.clear();
  vx.clear();
  vxe.clear();
  vt.clear();
  vt0 = 0;
  vrM = 0;
  vcM = 0;
  vyM = 0;
  viM = 0;

  Bool_t on(0);         // flag signal transmition
  Int_t n0(0), nsr(0),  // no of signals in the cluster (all/rect)
    NR(0), nr(0),       // no of rect signals/channel in case of RC
    NT(0), nt(0),       // no of tilt signals/channel in case of RC
    ovf(1);             // over-flow flag for at least one of the digis
  //dt;
  Char_t ddt,            // signal time offset wrt prompt
    dt0(0);              // cluster time offset wrt arbitrary t0
  Double_t r, re(100.),  // rect signal
    t, te(100.),         // tilt signal
    err,                 // final error parametrization for signal
    xc(-0.5),            // running signal-pad position
    max(0.);             // max signal
  Int_t j(0), col(-1), col0(0), col1(0), step(0), row1;

  // link second row if needed
  vector<CbmTrdDigiRec*>::iterator i1;
  if (cjd >= 0) {
    if (fDigis.find(cjd) == fDigis.end()) {
      LOG(warn) << GetName() << "::ProjectDigis : Request cl id " << cjd << " not found. Skip second row.";
      cjd = -1;
    }
    else
      i1 = fDigis[cjd].begin();
  }

  const CbmTrdDigiRec *dg(nullptr), *dg0(nullptr), *dg1(nullptr);
  for (vector<CbmTrdDigiRec*>::iterator i = fDigis[cid].begin(); i != fDigis[cid].end(); i++, j++) {
    dg  = (*i);
    dg0 = nullptr;
    dg1 = nullptr;
    if (CWRITE()) cout << "dg0 :" << dg->ToString();

    //  initialize
    if (col < 0) {
      vrM = GetPadRowCol(dg->GetAddressChannel(), col);
      vt0 = dg->GetTimeDAQ();  // set arbitrary t0 to avoid double digis loop
    }
    GetPadRowCol(dg->GetAddressChannel(), col0);
    nr = nt = 0;

    // read calibrated signals
    t = dg->GetTiltCharge(on);
    if (on) nt = 1;
    r = dg->GetRectCharge(on);
    if (on) nr = 1;
    // look for matching neighbor digis in case of pad row cross
    if (cjd >= 0) {
      if ((dg0 = (i1 != fDigis[cjd].end()) ? (*i1) : nullptr)) {
        row1 = GetPadRowCol(dg0->GetAddressChannel(), col1);
        if (!step) step = vrM - row1;
        if (col1 == col0) {
          r += dg0->GetRectCharge(on);
          if (on) nr++;
        }
        else
          dg0 = nullptr;
      }
      if (step == 1 && i1 != fDigis[cjd].begin()) {
        dg1 = (*(i1 - 1));
        GetPadRowCol(dg1->GetAddressChannel(), col1);
        if (col1 == col0 - 1) {
          t += dg1->GetTiltCharge(on);
          if (on) nt++;
        }
        else
          dg1 = nullptr;
      }
      if (step == -1 && i1 != fDigis[cjd].end() && i1 + 1 != fDigis[cjd].end()) {
        dg1 = (*(i1 + 1));
        GetPadRowCol(dg1->GetAddressChannel(), col1);
        if (col1 == col0 + 1) {
          t += dg1->GetTiltCharge(on);
          if (on) nt++;
        }
        else
          dg1 = nullptr;
      }
      if (dg0) i1++;
    }
    if (CWRITE()) {
      if (dg0) cout << "dgR :" << dg0->ToString();
      if (dg1) cout << "dgT :" << dg1->ToString();
      cout << "-------------------------------------" << endl;
    }

    // process tilt signal/time
    ddt = dg->GetTiltTime() - vt0;
    if (ddt < dt0) dt0 = ddt;
    if (abs(t) > 0) {
      if (nt > 1) t *= 0.5;
      err = te * (nt > 1 ? 0.707 : 1);
      if (dg->HasTiltOvf()) {
        ovf = -1;
        err = 150.;
      }
      if (t > max) {
        max = t;
        vcM = j;
        SetMaxTilt(1);
        viM = vs.size();
      }
    }
    else
      err = 300.;
    vt.push_back(ddt);
    vs.push_back(t);
    vse.push_back(err);
    vx.push_back(xc);
    vxe.push_back(0.035);
    xc += 0.5;

    // process rect signal/time
    ddt = dg->GetRectTime() - vt0;
    if (ddt < dt0) dt0 = ddt;
    if (abs(r) > 0) {
      nsr++;
      if (nr > 1) r *= 0.5;
      err = re * (nr > 1 ? 0.707 : 1);
      if (dg->HasRectOvf()) {
        ovf = -1;
        err = 150.;
      }
      if (r > max) {
        max = r;
        vcM = j;
        SetMaxTilt(0);
        viM = vs.size();
      }
    }
    else
      err = 300.;
    vt.push_back(ddt);
    vs.push_back(r);
    vse.push_back(err);
    vx.push_back(xc);
    vxe.push_back(0.);
    xc += 0.5;
    NR += nr;
    NT += nt;
  }

  // add front and back anchor points if needed
  if (TMath::Abs(vs[0]) > 1.e-3) {
    xc  = vx[0];
    ddt = vt[0];
    vs.insert(vs.begin(), 0);
    vse.insert(vse.begin(), 300);
    vt.insert(vt.begin(), ddt);
    vx.insert(vx.begin(), xc - 0.5);
    vxe.insert(vxe.begin(), 0);
    viM++;
  }
  Int_t n(vs.size() - 1);
  if (TMath::Abs(vs[n]) > 1.e-3) {
    xc  = vx[n] + 0.5;
    ddt = vt[n];
    vs.push_back(0);
    vse.push_back(300);
    vt.push_back(ddt);
    vx.push_back(xc);
    vxe.push_back(0.035);
  }

  n0 = vs.size() - 2;
  // compute cluster asymmetry
  Int_t nR = n0 + 1 - viM;
  if (nR == viM) {
    SetSymmHit(1);
    if (vs.size() % 2) {
      Double_t LS(0.), RS(0.);
      for (UChar_t idx(0); idx < viM; idx++)
        LS += vs[idx];
      for (UInt_t idx(viM + 1); idx < vx.size(); idx++)
        RS += vs[idx];
      SetLeftSgn(LS < RS ? 0 : 1);
    }
  }
  else {
    SetSymmHit(0);
    if (viM < nR) SetLeftHit(0);
    else if (viM > nR)
      SetLeftHit(1);
  }
  // recenter time and space profile
  vt0 += dt0;
  for (UInt_t idx(0); idx < vx.size(); idx++) {
    vt[idx] -= dt0;
    vx[idx] -= vcM;
  }
  vcM += col;

  // check if all signals have same significance
  Int_t nmiss = 2 * nsr - NR;  //printf("R : nsr[%d] NR[%d] nmiss[%d]\n", nsr, NR, nmiss);
  if (cjd >= 0 && nmiss) {
    SetBiasX(1);
    for (UChar_t idx(1); idx < viM; idx++) {
      if (vxe[idx] > 0.) continue;  //  select rect signals
      if (vse[idx] > re * 0.8) SetBiasXleft(1);
    }
    if (vxe[viM] <= 0. && vse[viM] > re * 0.8) SetBiasXmid(1);
    for (UChar_t idx(viM + 1); idx < vs.size() - 1; idx++) {
      if (vxe[idx] > 0.) continue;  //  select rect signals
      if (vse[idx] > re * 0.8) SetBiasXright(1);
    }
  }
  else
    SetBiasX(0);
  nmiss = 2 * n0 - 2 * nsr - NT;  //printf("T : n0[%d] nsr[%d] NT[%d] nmiss[%d]\n", n0, nsr, NT, nmiss);
  if (cjd >= 0 && nmiss) {
    SetBiasY();
    for (UChar_t idx(1); idx < viM; idx++) {
      if (vxe[idx] > 0. && vse[idx] > te * 0.8) SetBiasYleft(1);  //  select tilt signals
    }
    if (vxe[viM] > 0. && vse[viM] > te * 0.8) SetBiasYmid(1);
    for (UChar_t idx(viM + 1); idx < vs.size() - 1; idx++) {
      if (vxe[idx] > 0. && vse[idx] > te * 0.8) SetBiasYright(1);  //  select tilt signals
    }
  }
  else
    SetBiasY(0);

  if (CWRITE()) {
    printf("t0[clk]=%llu col[%2d] row[%2d] sz[%d] OVF[%c] %c", vt0, vcM, vrM, Int_t(vs.size() - 2),
           (ovf < 0 ? 'y' : 'n'), IsOpenLeft() ? '(' : '[');
    if (IsSymmHit()) {
      if (HasLeftSgn()) printf("<|");
      else
        printf("|>");
    }
    else
      printf("%s", IsLeftHit() ? "<<" : ">>");
    printf("%c bias[%c %c]\n", IsOpenRight() ? ')' : ']', IsBiasX() ? (IsBiasXleft() ? '<' : '>') : 'o',
           IsBiasY() ? (IsBiasYleft() ? '<' : '>') : 'o');
    for (UInt_t idx(0); idx < vs.size(); idx++) {
      printf("%2d dt[%2d] s[ADC]=%6.1f+-%6.1f x[pw]=%5.2f+-%5.2f ", idx, vt[idx], vs[idx], vse[idx], vx[idx], vxe[idx]);
      if (idx == viM) printf("[%s]", (IsMaxTilt() ? "//" : "||"));
      printf("\n");
    }
  }

  if (ovf < 0) SetOvf();  //printf("SetOvf %d\n", vyM); }
  return ovf * (vs.size() - 2);
}

//_______________________________________________________________________________
Int_t CbmTrdModuleRec2D::LoadDigis(vector<const CbmTrdDigi*>* digis, vector<CbmTrdDigi*>* vdgM, vector<Bool_t>* vmask,
                                   ULong64_t& t0, Int_t& cM)
{
  /** Load digis information in working vectors. 
 * The digis as provided by the digitizer are replaced by the merged one 
 * according to the vmask map. Digis are represented in the normal coordinate system of
 * (pad width [pw], DAQ time [clk], signal [ADC chs]) with rectangular signals at integer 
 * positions.  
 */
  vs.clear();
  vse.clear();
  vx.clear();
  vxe.clear();
  vt.clear();

  cM = 0;       // relative position of maximum signal
  Int_t n0(0),  // number of measured signals
    ovf(1),     // over-flow flag for at least one of the digis
    dt;
  Char_t ddt,            // signal time offset wrt prompt
    dt0(0);              // cluster time offset wrt arbitrary t0
  Double_t r, re(100.),  // rect signal
    t, te(100.),         // tilt signal
    err,                 // final error parametrization for signal
    xc(-0.5),            // running signal-pad position
    max(0.);             // max signal
  Int_t j(0), col0(-1), col1(0);
  const CbmTrdDigi* dg(nullptr);
  vector<CbmTrdDigi*>::iterator idgM = vdgM->begin();
  for (vector<const CbmTrdDigi*>::iterator i = digis->begin(); i != digis->end(); i++, j++) {
    dg = (*i);
    if ((*vmask)[j]) {
      dg = (*idgM);
      idgM++;
    }
    if (CWRITE()) cout << dg->ToString();
    r = dg->GetCharge(t, dt);
    if (t0 == 0) t0 = dg->GetTimeDAQ();                         // set arbitrary t0 to avoid double digis loop
    if (col0 < 0) GetPadRowCol(dg->GetAddressChannel(), col0);  //  initialilze
    ddt = dg->GetTimeDAQ() - t0;
    if (ddt < dt0) dt0 = ddt;

    // check column wise organization
    GetPadRowCol(dg->GetAddressChannel(), col1);
    if (col0 + j != col1) {
      LOG(error) << GetName() << "::LoadDigis : digis in cluster not in increasing order !";
      return 0;
    }

    // process tilt signal
    if (t > 0) {
      err = te;
      if (t > 4094) {
        ovf = -1;
        err = 150.;
      }
      t -= CbmTrdFASP::GetBaselineCorr();
      n0++;
      if (t > max) {
        max = t;
        cM  = j;
      }
    }
    else
      err = 300.;
    vt.push_back(ddt);
    vs.push_back(t);
    vse.push_back(err);
    vx.push_back(xc);
    vxe.push_back(0.035);
    xc += 0.5;

    // process rect signal
    ddt += dt;
    if (ddt < dt0) dt0 = ddt;
    if (r > 0) {
      err = re;
      if (r > 4094) {
        ovf = -1;
        err = 150.;
      }
      r -= CbmTrdFASP::GetBaselineCorr();
      n0++;
      if (r > max) {
        max = r;
        cM  = j;
      }
    }
    else
      err = 300.;
    vt.push_back(ddt);
    vs.push_back(r);
    vse.push_back(err);
    vx.push_back(xc);
    vxe.push_back(0.);
    xc += 0.5;
  }

  // remove merged digis if they were created
  if (idgM != vdgM->end()) LOG(warn) << GetName() << "::LoadDigis : not all merged digis have been consumed !";
  for (idgM = vdgM->begin(); idgM != vdgM->end(); idgM++)
    delete (*idgM);

  // add front and back anchor points if needed
  if (TMath::Abs(vs[0]) > 1.e-3) {
    xc  = vx[0];
    ddt = vt[0];
    vs.insert(vs.begin(), 0);
    vse.insert(vse.begin(), 300);
    vt.insert(vt.begin(), ddt);
    vx.insert(vx.begin(), xc - 0.5);
    vxe.insert(vxe.begin(), 0);
  }
  Int_t n(vs.size() - 1);
  if (TMath::Abs(vs[n]) > 1.e-3) {
    xc  = vx[n] + 0.5;
    ddt = vt[n];
    vs.push_back(0);
    vse.push_back(300);
    vt.push_back(ddt);
    vx.push_back(xc);
    vxe.push_back(0.035);
  }
  // recenter time and space profile
  t0 += dt0;
  for (UInt_t idx(0); idx < vx.size(); idx++) {
    vt[idx] -= dt0;
    vx[idx] -= cM;
  }
  return ovf * n0;
}


//_______________________________________________________________________________
Int_t CbmTrdModuleRec2D::LoadDigisRC(vector<const CbmTrdDigi*>* digis, const Int_t r0, const Int_t a0,
                                     /*vector<CbmTrdDigi*> *vdgM, */ ULong64_t& t0, Int_t& cM)
{
  /** Load digis information for row-cross hits in working vectors. 
  * The digis as provided by the digitizer are replaced by the merged one (TODO) 
  * according to the vmask map. Digis are represented in the normal coordinate system of
  * (pad width [pw], DAQ time [clk], signal [ADC chs]) with rectangular signals at integer 
  * positions.  
  * TODO 
  * 1. Time information in the secondary row not used. 
  * 2. Time walk (dependence on charge) not recovered by calibration. 
  * 
  */
  vs.clear();
  vse.clear();
  vx.clear();
  vxe.clear();
  vt.clear();

  cM = 0;  // relative position of maximum signal
  Int_t step,
    n0(0),   // number of measured signals
    ovf(1),  // over-flow flag for at least one of the digis
    dt, dT;
  Char_t ddt,               // signal time offset wrt prompt
    dt0(0);                 // cluster time offset wrt arbitrary t0
  Double_t r, R, re(100.),  // rect signal
    t, T, te(100.),         // tilt signal
    err,                    // final error parametrization for signal
    xc(0.),                 // running signal-pad position
    max(0.);                // max signal
  Int_t j(0), row, col, col0(-1), col1(0);
  //vector<CbmTrdDigi*>::iterator idgM=vdgM->begin();

  vector<const CbmTrdDigi*>::iterator i0, i1, ix0, ix1;
  i0 = digis->begin();
  i1 = i0;
  do {
    row = GetPadRowCol((*i1)->GetAddressChannel(), col1);
    if (col0 < 0) col0 = col1;
    if (row == r0) i1++;
    else
      break;
  } while (i1 != digis->end());
  ix0  = i1;
  ix1  = digis->end();
  col  = col0;
  step = -1;
  if (a0 > 0) {
    i0   = i1;
    ix0  = digis->end();
    ix1  = i1;
    i1   = digis->begin();
    col  = col0;
    col0 = col1;
    col1 = col;
    col  = col0;
    step = 1;
  }
  if (CWRITE()) printf("col0[%d] col1[%d] step[%2d]\n", col0, col1, step);
  const CbmTrdDigi *dg0(nullptr), *dg1(nullptr), *dg10(nullptr);

  // always loop on the largest cluster
  for (; i0 != ix0; i0++, j++) {
    dg0  = (*i0);
    dg1  = nullptr;
    dg10 = nullptr;
    if (CWRITE()) cout << "dg0 :" << dg0->ToString();

    r = dg0->GetCharge(t, dt);
    if (t > 0) t -= CbmTrdFASP::GetBaselineCorr();
    if (r > 0) r -= CbmTrdFASP::GetBaselineCorr();

    if (t0 == 0) t0 = dg0->GetTimeDAQ();  // set arbitrary t0 to avoid double digis loop
    ddt = dg0->GetTimeDAQ() - t0;
    if (ddt < dt0) dt0 = ddt;

    // check column wise organization
    row = GetPadRowCol(dg0->GetAddressChannel(), col0);
    if (col + j != col0) {
      LOG(error) << GetName() << "::LoadDigisRC : digis in cluster not in increasing order " << col0 << " !";
      return 0;
    }

    // look for matching neighbor digis
    if ((dg1 = (i1 != ix1) ? (*i1) : nullptr)) {
      GetPadRowCol(dg1->GetAddressChannel(), col1);
      if (col1 == col0) {
        R = dg1->GetCharge(T, dT);
        if (R > 0.) r += R - CbmTrdFASP::GetBaselineCorr();
      }
      else
        dg1 = nullptr;
    }
    if (step == 1 && i1 != digis->begin()) {
      dg10 = (*(i1 - 1));
      GetPadRowCol(dg10->GetAddressChannel(), col1);
      if (col1 == col0 - 1) {
        dg10->GetCharge(T, dT);
        if (T > 0.) t += T - CbmTrdFASP::GetBaselineCorr();
      }
      else
        dg10 = nullptr;
    }
    if (step == -1 && i1 != ix1 && i1 + 1 != ix1) {
      dg10 = (*(i1 + 1));
      GetPadRowCol(dg10->GetAddressChannel(), col1);
      if (col1 == col0 + 1) {
        dg10->GetCharge(T, dT);
        if (T > 0.) t += T - CbmTrdFASP::GetBaselineCorr();
      }
      else
        dg10 = nullptr;
    }
    if (dg1) i1++;

    if (CWRITE()) {
      if (dg1) cout << "dgR :" << dg1->ToString();
      if (dg10) cout << "dgT :" << dg10->ToString();
      cout << "-------------------------------------" << endl;
    }

    // process tilt signal
    if (t > 0) {
      err = te;
      n0++;
      if (t > max) {
        max = t;
        cM  = j;
      }
    }
    else
      err = 300.;
    vt.push_back(ddt);
    vs.push_back(t);
    vse.push_back(err);
    vx.push_back(xc);
    vxe.push_back(0.035);

    // process rect signal
    ddt += dt;
    if (ddt < dt0) dt0 = ddt;
    if (r > 0) {
      err = re;
      n0++;
      if (r > max) {
        max = r;
        cM  = j;
      }
    }
    else
      err = 300.;
    vt.push_back(ddt);
    vs.push_back(r);
    vse.push_back(err);
    vx.push_back(xc);
    vxe.push_back(0.);
    xc += 1;
  }

  //   // remove merged digis if they were created
  //   if(idgM != vdgM->end()) LOG(warn) << GetName() << "::LoadDigis : not all merged digis have been consumed !";
  //   for(idgM=vdgM->begin(); idgM!=vdgM->end(); idgM++) delete (*idgM);
  //
  // add front and back anchor points if needed
  if (TMath::Abs(vs[0]) > 1.e-3) {
    xc  = vx[0];
    ddt = vt[0];
    vs.insert(vs.begin(), 0);
    vse.insert(vse.begin(), 300);
    vt.insert(vt.begin(), ddt);
    vx.insert(vx.begin(), xc - 1);
    vxe.insert(vxe.begin(), 0);
  }
  Int_t n(vs.size() - 1);
  if (TMath::Abs(vs[n]) > 1.e-3) {
    xc  = vx[n] + 1;
    ddt = vt[n];
    vs.push_back(0);
    vse.push_back(300);
    vt.push_back(ddt);
    vx.push_back(xc);
    vxe.push_back(0.035);
  }

  // recenter time and space profile
  if (cM + col >= fDigiPar->GetNofColumns()) cM = fDigiPar->GetNofColumns() - col - 1;
  else if (cM + col < 0)
    cM = -col;
  t0 += dt0;
  for (UInt_t idx(0); idx < vx.size(); idx++) {
    vt[idx] -= dt0;
    vx[idx] -= cM;
  }
  cM += col;
  return ovf * n0;
}

//_______________________________________________________________________________
Bool_t CbmTrdModuleRec2D::MergeDigis(vector<const CbmTrdDigi*>* digis, vector<CbmTrdDigi*>* vdgM, vector<Bool_t>* vmask)
{
  /* Merge digis in the cluster if their topology within it allows it although cuts in the 
 * digi merger procedure (CbmTrdFASP::WriteDigi()) were not fulfilled. 
 * Normally this are boundary signals with large time delays wrt neighbors
 */

  CbmTrdDigi* dgM(nullptr);
  if (digis->size() < 2) {  // sanity check ... just in case
    LOG(warn) << GetName() << "::MergeDigis : Bad digi config for cluster :";
    return kFALSE;
  }

  Double_t r, t;
  Int_t colR, colT, dt, contor(0);
  Bool_t kFOUND(0);
  for (vector<const CbmTrdDigi*>::iterator idig = digis->begin(), jdig = idig + 1; jdig != digis->end();
       idig++, jdig++, contor++) {
    const CbmTrdDigi* dgT((*idig));  // tilt signal digi
    const CbmTrdDigi* dgR((*jdig));  // rect signal digi
    GetPadRowCol(dgR->GetAddressChannel(), colR);
    GetPadRowCol(dgT->GetAddressChannel(), colT);

    if (colR != colT) continue;

    dgM = new CbmTrdDigi(*dgT);
    r   = dgR->GetCharge(t, dt);
    dgT->GetCharge(t, dt);
    dt = dgR->GetTimeDAQ() - dgT->GetTimeDAQ();
    dgM->SetCharge(t, r, dt);
    Int_t rtrg(dgR->GetTriggerType() & 2), ttrg(dgT->GetTriggerType() & 1);
    dgM->SetTriggerType(rtrg | ttrg);  //merge the triggers
    if (CWRITE()) {
      cout << "MERGE" << endl;
      cout << dgT->ToString();
      cout << dgR->ToString();
      cout << "TO" << endl;
      cout << dgM->ToString();
      cout << "..." << endl;
    }
    kFOUND = 1;

    vdgM->push_back(dgM);
    (*vmask)[contor] = 1;
    jdig             = digis->erase(jdig);
    if (jdig == digis->end()) break;
  }

  if (!kFOUND) {
    LOG(warn) << GetName() << "::MergeDigis : Digi to pads matching failed for cluster :";
    return kFALSE;
  }
  return kTRUE;
}


Float_t CbmTrdModuleRec2D::fgCorrXdx                 = 0.01;
Float_t CbmTrdModuleRec2D::fgCorrXval[3][NBINSCORRX] = {
  {-0.001, -0.001, -0.002, -0.002, -0.003, -0.003, -0.003, -0.004, -0.004, -0.006, -0.006, -0.006, -0.007,
   -0.007, -0.008, -0.008, -0.008, -0.009, -0.009, -0.011, -0.011, -0.011, -0.012, -0.012, -0.012, -0.012,
   -0.013, -0.013, -0.013, -0.013, -0.014, -0.014, -0.014, -0.014, -0.014, -0.016, -0.016, -0.016, -0.016,
   -0.017, -0.017, -0.017, -0.018, -0.018, -0.018, -0.018, -0.018, 0.000,  0.000,  0.000},
  {0.467, 0.430, 0.396, 0.364, 0.335, 0.312, 0.291, 0.256, 0.234, 0.219, 0.207, 0.191, 0.172,
   0.154, 0.147, 0.134, 0.123, 0.119, 0.109, 0.122, 0.113, 0.104, 0.093, 0.087, 0.079, 0.073,
   0.067, 0.063, 0.058, 0.053, 0.049, 0.046, 0.042, 0.038, 0.036, 0.032, 0.029, 0.027, 0.024,
   0.022, 0.019, 0.017, 0.014, 0.013, 0.011, 0.009, 0.007, 0.004, 0.003, 0.001},
  {0.001,  0.001,  0.001,  0.001,  0.002,  0.002,  0.001,  0.002,  0.004,  0.003,  0.002,  0.002,  0.002,
   0.002,  0.002,  0.002,  0.003,  0.004,  0.003,  0.004,  0.004,  0.007,  0.003,  0.004,  0.002,  0.002,
   -0.011, -0.011, -0.012, -0.012, -0.012, -0.013, -0.013, -0.013, -0.014, -0.014, -0.014, -0.016, -0.016,
   -0.016, -0.017, -0.017, -0.017, -0.018, -0.018, -0.018, -0.019, 0.029,  0.018,  0.001}};
Float_t CbmTrdModuleRec2D::fgCorrYval[NBINSCORRY][2]   = {{2.421729, 0.},
                                                        {0.629389, -0.215285},
                                                        {0.23958, 0.},
                                                        {0.151913, 0.054404}};
Float_t CbmTrdModuleRec2D::fgCorrRcXval[2][NBINSCORRX] = {
  {-0.00050, -0.00050, -0.00150, -0.00250, -0.00250, -0.00350, -0.00450, -0.00450, -0.00550, -0.00650,
   -0.00650, -0.00750, -0.00850, -0.00850, -0.00850, -0.00950, -0.00950, -0.00950, -0.01050, -0.01150,
   -0.01150, -0.01150, -0.01250, -0.01250, -0.01250, -0.01250, -0.01350, -0.01350, -0.01350, -0.01350,
   -0.01450, -0.01450, -0.01450, -0.01550, -0.01550, -0.01550, -0.01550, -0.01650, -0.01650, -0.01550,
   -0.01650, -0.01614, -0.01620, -0.01624, -0.01626, -0.01627, -0.01626, -0.01624, -0.01620, -0.01615},
  {0.36412, 0.34567, 0.32815, 0.31152, 0.29574, 0.28075, 0.26652, 0.25302, 0.24020, 0.22803, 0.21647, 0.21400, 0.19400,
   0.18520, 0.17582, 0.16600, 0.14600, 0.13800, 0.14280, 0.14200, 0.13400, 0.12600, 0.12200, 0.11000, 0.10200, 0.09400,
   0.09000, 0.08600, 0.08200, 0.07400, 0.07000, 0.06600, 0.06600, 0.06200, 0.05800, 0.05400, 0.05400, 0.05000, 0.04600,
   0.04600, 0.04200, 0.03800, 0.03800, 0.03400, 0.03400, 0.03000, 0.03000, 0.02600, 0.02200, 0.02200}};
Float_t CbmTrdModuleRec2D::fgCorrRcXbiasXval[3][NBINSCORRX] = {
  {0.00100, 0.00260, 0.00540, 0.00740, 0.00900, 0.01060, 0.01300, 0.01460, 0.01660, 0.01900, 0.02060, 0.02260, 0.02420,
   0.02700, 0.02860, 0.02980, 0.03220, 0.03340, 0.03540, 0.03620, 0.03820, 0.04020, 0.04180, 0.04340, 0.04460, 0.04620,
   0.04740, 0.04941, 0.05088, 0.05233, 0.05375, 0.05515, 0.05653, 0.05788, 0.05921, 0.06052, 0.06180, 0.06306, 0.06430,
   0.06551, 0.06670, 0.06786, 0.06901, 0.07012, 0.07122, 0.07229, 0.07334, 0.07436, 0.07536, 0.07634},
  {0.00100, 0.00380, 0.00780, 0.00900, 0.01220, 0.01460, 0.01860, 0.01940, 0.02260, 0.02540, 0.02820, 0.03060, 0.03220,
   0.03660, 0.03980, 0.04094, 0.04420, 0.04620, 0.04824, 0.04980, 0.05298, 0.05532, 0.05740, 0.05991, 0.06217, 0.06500,
   0.06540, 0.06900, 0.07096, 0.07310, 0.07380, 0.07729, 0.07935, 0.08139, 0.08340, 0.08538, 0.08734, 0.08928, 0.08900,
   0.09307, 0.09493, 0.09340, 0.09858, 0.09620, 0.09740, 0.10386, 0.09980, 0.10726, 0.10892, 0.11056},
  {0.00011, 0.00140, 0.00340, 0.00420, 0.00500, 0.00620, 0.00820, 0.00860, 0.01060, 0.01100, 0.01220, 0.01340, 0.01500,
   0.01540, 0.01700, 0.01820, 0.01900, 0.02060, 0.02180, 0.02260, 0.02340, 0.02420, 0.02500, 0.02500, 0.02660, 0.02740,
   0.02820, 0.02900, 0.03020, 0.03180, 0.03300, 0.03260, 0.03380, 0.03460, 0.03500, 0.03580, 0.03780, 0.03820, 0.03860,
   0.03900, 0.04100, 0.04180, 0.04060, 0.04300, 0.04340, 0.04340, 0.04380, 0.04460, 0.04580, 0.04540}};

ClassImp(CbmTrdModuleRec2D)
