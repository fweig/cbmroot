/* Copyright (C) 2010-2021 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov [committer] */

#include "L1AlgoDraw.h"

#include "CbmKF.h"
#include "CbmL1.h"

#include "TApplication.h"
#include "TCanvas.h"
#include "TEllipse.h"
#include "TFrame.h"
#include "TLatex.h"
#include "TLine.h"
#include "TMarker.h"
#include "TPad.h"
#include "TPaveText.h"
#include "TPolyLine.h"
#include "TPolyLine3D.h"
#include "TPolyMarker.h"
#include "TStyle.h"
#include "TText.h"
#include "TView3D.h"

#include "L1Algo.h"

// NOTE: using std::cout, std::endl were removed from one of the headers, and it
//       caused errors. Please, don't use "using" in headers
using std::cout;
using std::endl;

L1AlgoDraw::L1AlgoDraw()
{

  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasBorderSize(1);
  gStyle->SetCanvasColor(0);

  YZ = new TCanvas("YZ", "YZ Side View", -1, 0, 500, 500);
  YZ->Range(-5.0, -50.0, 105.0, 50.0);
  YZ->Draw();
  YZ->Update();

  XZ = new TCanvas("XZ", "XZ Top View", -1, 500, 500, 500);
  XZ->Range(-5.0, -50.0, 105.0, 50.0);
  XZ->Draw();
  XZ->Update();

  YX = new TCanvas("YX", "YX Front View", -506, 0, 500, 500);
  YX->Range(-50.0, -50.0, 50.0, 50.0);
  YX->Draw();
  YX->Update();

  //   XYZ = new TCanvas ("XYZ", "XYZ 3D View",  -0, 0, 1000, 1000);
  XYZ = new TCanvas("XYZ", "XYZ 3D View", -500, 500, 500, 500);
  XYZ->Range(-5.0, -50.0, 115.0, 60.0);
  //   TView3D *view = (TView3D*) XYZ->GetView3D();
  //   view = (TView3D*) TView::CreateView(1);
  //   TView3D *view = new TView3D();
  //   view->SetRange(-50.0, -50.0, -0.0, 50.0, 50.0, 110.0);
  //   view->SetRange(0,0,0,4,4,4);
  //   view->Draw();
  //   XYZ->ResetView3D(view);
  XYZ->Draw();
  XYZ->Update();

  fVerbose = CbmL1::Instance()->fVerbose;
  ask      = true;
}

void L1AlgoDraw::InitL1Draw(L1Algo* algo_)
{
  //   algo = CbmL1::Instance()->algo;
  algo = algo_;

  vHits.clear();
  vHits.reserve(algo->fGridHits.size());
  for (unsigned int i = 0; i < algo->GetInputData().GetNhits(); i++) {
    vHits.push_back(algo->fGridHits[i]);
  }
  NStations = algo->GetParameters()->GetNstationsActive();
  for (int i = 0; i < NStations; i++) {
    HitsStartIndex[i] = algo->fGridHitStartIndex[i];
    HitsStopIndex[i]  = algo->fGridHitStopIndex[i];
    vStations[i]      = algo->GetParameters()->GetStation(i);
  }
}

void L1AlgoDraw::DrawMCTracks()
{
  int NRegMCTracks = 0;
  CbmL1& L1        = *CbmL1::Instance();
  TPolyLine pline;
  if (fVerbose >= 10) {
    cout << "Only reconstructable tracks are shown." << endl;
    cout << "Red - primary p > 0.5 - (first iteration)" << endl;
    cout << "Blue - primary p < 0.5 - (second iteration)" << endl;
    cout << "Green - secondary p > 0.5 - (third\\first iteration)" << endl;
    cout << "Gray - secondary p < 0.5 - (third\\second iteration)" << endl;
  };

  for (vector<CbmL1MCTrack>::iterator it = L1.fvMCTracks.begin(); it != L1.fvMCTracks.end(); ++it) {
    CbmL1MCTrack& T = *it;
    //draw reconstructable tracks only
    if (!T.IsReconstructable()) continue;
    //if (( T.mother_ID< 0 )&&( T.nContStations<3 )) continue;
    //if (( T.mother_ID>=0 )&&( T.nContStations<4 )) continue;
    //if( T.p<.2 ) continue;
    if (T.p < 0.1) continue;
    pline.SetLineColor(kRed);
    if (T.p < 0.5) pline.SetLineColor(kBlue);
    if (T.mother_ID != -1) pline.SetLineColor(8);
    if ((T.mother_ID != -1) && (T.p < 0.5)) pline.SetLineColor(12);
    if (fVerbose >= 1)
      cout << "MC Track: p = " << T.p << "  mother_ID = " << T.mother_ID << "  PDG = " << T.pdg << " x,y,z = (" << T.x
           << ", " << T.y << ", " << T.z << ")" << endl;
    double par[6];
    par[0] = T.x;
    par[1] = T.y;
    //if( fabs(T.pz)<0.1 ) continue;
    par[2] = T.px / T.pz;
    par[3] = T.py / T.pz;
    par[4] = T.q / T.p;
    par[5] = T.z;

    int npoints = T.Points.size();
    if (fVerbose >= 10) cout << " NMCPoints = " << npoints << endl;
    if (npoints < 1) continue;

    vector<double> lx, ly, lz;
    lx.push_back(par[0]);
    ly.push_back(par[1]);
    lz.push_back(par[5]);

    bool ok = true;

    if (fVerbose >= 4) {
      cout << "hits = ";
      for (unsigned int ih = 0; ih < T.Hits.size(); ih++)
        cout << T.Hits[ih] << " ";
      cout << endl;
    }
    for (int ip = 0; ip < npoints; ip++) {
      CbmL1MCPoint& p = L1.fvMCPoints[T.Points[ip]];
      double par1[6];
      //if( fabs(p.pz)<0.05 ) continue;
      par1[0] = p.x;
      par1[1] = p.y;
      par1[2] = p.px / p.pz;
      par1[3] = p.py / p.pz;
      par1[4] = p.q / p.p;
      par1[5] = p.z;
      if (fVerbose >= 5) {
        static fscal pz = -1;
        if (fabs(pz - p.z) > 1.0) cout << "-- ";
        cout << "point.z = " << p.z << endl;
        pz = p.z;
      }

      double Zfrst = par[5];
      double Zlast = par1[5];
      double step  = .5;
      if (step > fabs(Zfrst - Zlast) / 5) step = fabs(Zfrst - Zlast) / 5;
      if (Zlast < par[5]) step = -step;
      while (fabs(par[5] - Zlast) > fabs(step)) {
        double znxt = par[5] + step;
        CbmKF::Instance()->Propagate(par1, 0, znxt, par1[4]);
        CbmKF::Instance()->Propagate(par, 0, znxt, par[4]);
        double w  = fabs(znxt - Zfrst);
        double w1 = fabs(znxt - Zlast);
        if (w + w1 < 1.e-3) {
          w  = 1;
          w1 = 0;
        }
        double xl = (w1 * par[0] + w * par1[0]) / (w + w1);
        double yl = (w1 * par[1] + w * par1[1]) / (w + w1);
        double zl = (w1 * par[5] + w * par1[5]) / (w + w1);
        if ((fabs(xl) > 50.0) || (fabs(yl) > 50.0)) {
          //cout << "*** track " << NRegMCTracks+1 << " xl = " << xl << ", zl = " << zl << endl;
          //cout << "*** track " << NRegMCTracks+1 << " yl = " << yl << ", zl = " << zl << endl;
          ok = false;
          continue;
        }
        lx.push_back(xl);
        ly.push_back(yl);
        lz.push_back(zl);
      }

      par[0] = p.x;
      par[1] = p.y;
      par[2] = p.px / p.pz;
      par[3] = p.py / p.pz;
      par[4] = p.q / p.p;
      par[5] = p.z;
      lx.push_back(par[0]);
      ly.push_back(par[1]);
      lz.push_back(par[5]);
    }

    if (ok) {

      NRegMCTracks++;

      YZ->cd();
      pline.DrawPolyLine(lx.size(), &(lz[0]), &(ly[0]));
      XZ->cd();
      pline.DrawPolyLine(lx.size(), &(lz[0]), &(lx[0]));
      YX->cd();
      pline.DrawPolyLine(lx.size(), &(lx[0]), &(ly[0]));

      //       YZ->cd(); YZ->Update();
      //       XZ->cd(); XZ->Update();
      //       YX->cd(); YX->Update();
      //       DrawAsk();
    }
  }

  cout << "L1CADraw: number of registered MC tracks: " << NRegMCTracks << endl;

  YZ->cd();
  YZ->Update();
  XZ->cd();
  XZ->Update();
  YX->cd();
  YX->Update();
}

void L1AlgoDraw::DrawRecoTracks()
{
  //   XYZ->cd();
  //   TView *view = TView::CreateView(1);
  //   view->SetRange(-100,-100,-100, 100, 100, 100);

  int NRecTracks = 0;
  //   CbmL1 &L1 = *CbmL1::Instance();

  int curRecoHit                   = 0;
  L1Vector<L1HitIndex_t>& recoHits = algo->fSliceRecoHits;
  for (vector<L1Track>::iterator it = algo->fSliceRecoTracks.begin(); it != algo->fSliceRecoTracks.end(); ++it) {
    L1Track& T = *it;
    int nHits  = T.NHits;
    //     if (nHits > 5) continue; // draw clones
    //     YZ->cd(); YZ->Update();
    //     XZ->cd(); XZ->Update();
    //     YX->cd(); YX->Update();
    //     DrawAsk();
    vector<double> lx, ly, lz;
    vector<double> lx_turned, ly_turned, lz_turned;

    TPolyLine pline;
    pline.SetLineColor(kBlue);
    //     TPolyLine3D pline3D(nHits);
    //     pline3D.SetLineColor(kBlue);
    if (fVerbose >= 4) { cout << "hits = "; }
    for (int iHit = 0; iHit < nHits; iHit++) {
      unsigned int ih = recoHits[curRecoHit++];
      if (fVerbose >= 4) { cout << ih << " "; }

      Point p = GetHitCoor(ih);
      lx.push_back(p.x);
      ly.push_back(p.y);
      lz.push_back(p.z);

      TVector3 v3(p.x, p.y, p.z);
      v3.RotateX(TMath::Pi() / 5);
      v3.RotateY(TMath::Pi() / 20);
      v3.RotateZ(TMath::Pi() / 100);
      lx_turned.push_back(v3.x());
      ly_turned.push_back(v3.y());
      lz_turned.push_back(v3.z());
      //       pline3D.SetPoint(iHit, p.x, p.y, p.z);
    }
    if (fVerbose >= 4) { cout << endl; }
    if (1) {

      NRecTracks++;

      YZ->cd();
      pline.DrawPolyLine(lx.size(), &(lz[0]), &(ly[0]));
      XZ->cd();
      pline.DrawPolyLine(lx.size(), &(lz[0]), &(lx[0]));
      YX->cd();
      pline.DrawPolyLine(lx.size(), &(lx[0]), &(ly[0]));
      XYZ->cd();
      pline.DrawPolyLine(lx_turned.size(), &(lz_turned[0]), &(lx_turned[0]));
      //       pline3D.Draw();
    }
  }

  cout << "L1CADraw: number of reconstructed tracks: " << NRecTracks << endl;

  YZ->cd();
  YZ->Update();
  XZ->cd();
  XZ->Update();
  YX->cd();
  YX->Update();

  XYZ->cd();
  XYZ->Update();
}

void L1AlgoDraw::DrawTriplets(vector<L1Triplet>& triplets, const L1HitIndex_t* realIHit)
{
  //   vector <L1Triplet> triplets = algo->vTriplets;
  for (unsigned int iTrip = 0; iTrip < triplets.size(); iTrip++) {
    L1Triplet& trip = triplets[iTrip];

    unsigned int iLHit = trip.GetLHit();
    iLHit              = realIHit[iLHit];
    unsigned int iMHit = trip.GetMHit();
    iMHit              = realIHit[iMHit];
    unsigned int iRHit = trip.GetRHit();
    iRHit              = realIHit[iRHit];

    DrawTriplet(iLHit, iMHit, iRHit);
  }

  YZ->cd();
  YZ->Update();
  XZ->cd();
  XZ->Update();
  YX->cd();
  YX->Update();
};

void L1AlgoDraw::DrawTriplet(int il, int im, int ir)
{
  TPolyLine pline;
  pline.SetLineColor(kBlack);
  TMarker marker;
  marker.SetMarkerColor(kBlack);
  marker.SetMarkerStyle(26);
  marker.SetMarkerSize(HitSize * 2);

  vector<double> lx, ly, lz;

  Point coor;

  coor = GetHitCoor(il);
  lx.push_back(coor.x);
  ly.push_back(coor.y);
  lz.push_back(coor.z);

  coor = GetHitCoor(im);
  lx.push_back(coor.x);
  ly.push_back(coor.y);
  lz.push_back(coor.z);

  coor = GetHitCoor(ir);
  lx.push_back(coor.x);
  ly.push_back(coor.y);
  lz.push_back(coor.z);

  const int nHits = 3;
  YZ->cd();
  pline.DrawPolyLine(nHits, &(lz[0]), &(ly[0]));
  marker.DrawMarker(lz[nHits - 1], ly[nHits - 1]);
  XZ->cd();
  pline.DrawPolyLine(nHits, &(lz[0]), &(lx[0]));
  marker.DrawMarker(lz[nHits - 1], lx[nHits - 1]);
  YX->cd();
  pline.DrawPolyLine(nHits, &(lx[0]), &(ly[0]));
  marker.DrawMarker(lx[nHits - 1], ly[nHits - 1]);
}

void L1AlgoDraw::DrawDoublets(vector<L1HitIndex_t>* Doublets_hits, map<L1HitIndex_t, L1HitIndex_t>* Doublets_start,
                              const int /*MaxArrSize*/, L1HitIndex_t* StsRestHitsStartIndex, unsigned int* realIHit)
{
  for (int iSta = 0; iSta < NStations - 1; iSta++) {
    const int firstHitOnSta                           = StsRestHitsStartIndex[iSta];
    const int firstHitOnNextSta                       = StsRestHitsStartIndex[iSta + 1];
    L1HitIndex_t* staDoubletsHits                     = &(Doublets_hits[iSta][0]);
    map<L1HitIndex_t, L1HitIndex_t>& staDoubletsStart = Doublets_start[iSta];

    for (int iRestLHit = firstHitOnSta; iRestLHit < firstHitOnNextSta; iRestLHit++) {
      const int ilh       = iRestLHit - firstHitOnSta;
      const int iirhFirst = staDoubletsStart[ilh];
      const int iirhLast  = staDoubletsStart[ilh + 1] - 1;

      for (int iirh = iirhFirst; iirh <= iirhLast; iirh++) {
        const int iRestRHit = staDoubletsHits[iirh] + firstHitOnNextSta;

        const int iLHit = realIHit[iRestLHit];
        const int iRHit = realIHit[iRestRHit];

        DrawDoublet(iLHit, iRHit);
      }
    }
  }

  YZ->cd();
  YZ->Update();
  XZ->cd();
  XZ->Update();
  YX->cd();
  YX->Update();
};

void L1AlgoDraw::DrawDoubletsOnSta(int iSta, L1HitIndex_t* Doublets_hits, L1HitIndex_t* Doublets_start,
                                   const int MaxArrSize, L1HitIndex_t* StsRestHitsStartIndex, unsigned int* realIHit)
{
  const int firstHitOnSta        = StsRestHitsStartIndex[iSta];
  const int firstHitOnNextSta    = StsRestHitsStartIndex[iSta + 1];
  L1HitIndex_t* staDoubletsHits  = Doublets_hits + MaxArrSize * iSta;
  L1HitIndex_t* staDoubletsStart = Doublets_start + MaxArrSize * iSta;

  for (int iRestLHit = firstHitOnSta; iRestLHit < firstHitOnNextSta; iRestLHit++) {
    const int ilh       = iRestLHit - firstHitOnSta;
    const int iirhFirst = staDoubletsStart[ilh];
    const int iirhLast  = staDoubletsStart[ilh + 1] - 1;

    for (int iirh = iirhFirst; iirh <= iirhLast; iirh++) {
      const int iRestRHit = staDoubletsHits[iirh] + firstHitOnNextSta;

      const int iLHit = realIHit[iRestLHit];
      const int iRHit = realIHit[iRestRHit];

      DrawDoublet(iLHit, iRHit);
    }
  }

  YZ->cd();
  YZ->Update();
  XZ->cd();
  XZ->Update();
  YX->cd();
  YX->Update();
};

void L1AlgoDraw::DrawDoublet(int il, int ir)
{
  TPolyLine pline;
  pline.SetLineColor(kBlue);
  TMarker marker;
  marker.SetMarkerColor(kBlue);
  marker.SetMarkerStyle(27);
  marker.SetMarkerSize(HitSize * 2);

  vector<double> lx, ly, lz;

  Point coor;

  coor = GetHitCoor(il);
  lx.push_back(coor.x);
  ly.push_back(coor.y);
  lz.push_back(coor.z);

  coor = GetHitCoor(ir);
  lx.push_back(coor.x);
  ly.push_back(coor.y);
  lz.push_back(coor.z);

  const int nHits = 2;
  YZ->cd();
  pline.DrawPolyLine(nHits, &(lz[0]), &(ly[0]));
  marker.DrawMarker(lz[nHits - 1], ly[nHits - 1]);
  XZ->cd();
  pline.DrawPolyLine(nHits, &(lz[0]), &(lx[0]));
  marker.DrawMarker(lz[nHits - 1], lx[nHits - 1]);
  YX->cd();
  pline.DrawPolyLine(nHits, &(lx[0]), &(ly[0]));
  marker.DrawMarker(lx[nHits - 1], ly[nHits - 1]);
}


void L1AlgoDraw::DrawInfo()
{
  cout << " vHits.size = " << algo->GetInputData().GetNhits() << endl;
  cout << " vRecoHits.size = " << algo->fSliceRecoHits.size() << endl;
  cout << " vTracks.size = " << algo->fSliceRecoTracks.size() << endl;
}

void L1AlgoDraw::DrawTarget()
{

  double x = 0, y = 0, z = 0;
  double x_t, z_t;

  TVector3 v3(x, y, z);
  v3.RotateX(TMath::Pi() / 5);
  v3.RotateY(TMath::Pi() / 20);
  v3.RotateZ(TMath::Pi() / 100);
  x_t = v3.x();
  z_t = v3.z();

  {
    YZ->cd();

    TMarker* marker = new TMarker(z, y, targetMStyle);
    marker->SetMarkerColor(kRed);
    marker->SetMarkerStyle(targetMStyle);
    marker->SetMarkerSize(HitSize);
    marker->Draw();
  }

  {
    XZ->cd();

    TMarker* marker = new TMarker(z, x, targetMStyle);
    marker->SetMarkerColor(kRed);
    marker->SetMarkerStyle(targetMStyle);
    marker->SetMarkerSize(HitSize);
    marker->Draw();
  }

  {
    YX->cd();

    TMarker* marker = new TMarker(x, y, targetMStyle);
    marker->SetMarkerColor(kRed);
    marker->SetMarkerStyle(targetMStyle);
    marker->SetMarkerSize(HitSize);
    marker->Draw();
  }

  {
    XYZ->cd();

    TMarker* marker = new TMarker(z_t, x_t, targetMStyle);
    marker->SetMarkerColor(kRed);
    marker->SetMarkerStyle(targetMStyle);
    marker->SetMarkerSize(HitSize);
    marker->Draw();
  }
}

void L1AlgoDraw::DrawInputHits()
{

  TLatex latex;
  latex.SetTextFont(132);
  latex.SetTextAlign(12);
  latex.SetTextSize(0.035);

  YZ->cd();
  latex.DrawLatex(0.0, 45.0, "YZ Side View");
  YZ->Draw();
  XZ->cd();
  latex.DrawLatex(0.0, 45.0, "XZ Top View");
  XZ->Draw();
  YX->cd();
  latex.DrawLatex(-45.0, 45.0, "YX Front View");
  YX->Draw();


  int nhits = vHits.size();
  Double_t x_poly[nhits], y_poly[nhits], z_poly[nhits];
  Double_t x_poly_fake[nhits], y_poly_fake[nhits], z_poly_fake[nhits];
  Double_t x_poly_turned[nhits], z_poly_turned[nhits];
  Double_t x_poly_fake_turned[nhits], z_poly_fake_turned[nhits];


  for (int ista = NStations - 1; ista >= 0; ista--) {  //  //start downstream chambers
    L1Station& st     = vStations[ista];
    Int_t n_poly      = 0;
    Int_t n_poly_fake = 0;
    for (int ih = HitsStartIndex[ista]; ih < HitsStopIndex[ista]; ih++) {
      L1Hit& h = vHits[ih];
      int iMC  = CbmL1::Instance()->fvHitPointIndexes[ih];
      //if( (vSFlag[h.f] | vSFlagB[h.b] )&0x02 ) continue; // if used

      fscal x, y, z;
      fscal x_t, z_t;
      algo->GetHitCoor(h, x, y, z, st);

      TVector3 v3(x, y, z);
      v3.RotateX(TMath::Pi() / 5);
      v3.RotateY(TMath::Pi() / 20);
      v3.RotateZ(TMath::Pi() / 100);
      x_t = v3.x();
      z_t = v3.z();

      if (iMC >= 0) {
        x_poly[n_poly]        = x;
        y_poly[n_poly]        = y;
        z_poly[n_poly]        = z;
        x_poly_turned[n_poly] = x_t;
        z_poly_turned[n_poly] = z_t;
        n_poly++;
      }
      else {
        x_poly_fake[n_poly_fake]        = x;
        y_poly_fake[n_poly_fake]        = y;
        z_poly_fake[n_poly_fake]        = z;
        x_poly_fake_turned[n_poly_fake] = x_t;
        z_poly_fake_turned[n_poly_fake] = z_t;
        n_poly_fake++;
      }
    }

    YZ->cd();

    TLine* line = new TLine();
    line->SetLineColor(StaColor);
    line->DrawLine(st.fZ[0], -st.Rmax[0], st.fZ[0], st.Rmax[0]);

    TPolyMarker* pmyz = new TPolyMarker(n_poly, z_poly, y_poly);
    pmyz->SetMarkerColor(mcolor[ista]);
    pmyz->SetMarkerStyle(hitsMStyle);
    pmyz->SetMarkerSize(HitSize);
    pmyz->Draw();

    TPolyMarker* pmyz_fake = new TPolyMarker(n_poly_fake, z_poly_fake, y_poly_fake);
    pmyz_fake->SetMarkerColor(mcolor[ista]);
    pmyz_fake->SetMarkerStyle(fakesMStyle);
    pmyz_fake->SetMarkerSize(HitSize);
    pmyz_fake->Draw();

    XZ->cd();

    line->DrawLine(st.fZ[0], -st.Rmax[0], st.fZ[0], st.Rmax[0]);

    TPolyMarker* pmxz = new TPolyMarker(n_poly, z_poly, x_poly);
    pmxz->SetMarkerColor(mcolor[ista]);
    pmxz->SetMarkerStyle(hitsMStyle);
    pmxz->SetMarkerSize(HitSize);
    pmxz->Draw();

    TPolyMarker* pmxz_fake = new TPolyMarker(n_poly_fake, z_poly_fake, x_poly_fake);
    pmxz_fake->SetMarkerColor(mcolor[ista]);
    pmxz_fake->SetMarkerStyle(fakesMStyle);
    pmxz_fake->SetMarkerSize(HitSize);
    pmxz_fake->Draw();

    YX->cd();

    TEllipse* ellipse = new TEllipse(0.0, 0.0, st.Rmax[0]);
    ellipse->SetLineColor(StaColor);
    ellipse->SetFillStyle(0);
    ellipse->Draw();

    TPolyMarker* pmyx = new TPolyMarker(n_poly, x_poly, y_poly);
    pmyx->SetMarkerColor(mcolor[ista]);
    pmyx->SetMarkerStyle(hitsMStyle);
    pmyx->SetMarkerSize(HitSize);
    pmyx->Draw();

    TPolyMarker* pmyx_fake = new TPolyMarker(n_poly_fake, x_poly_fake, y_poly_fake);
    pmyx_fake->SetMarkerColor(mcolor[ista]);
    pmyx_fake->SetMarkerStyle(fakesMStyle);
    pmyx_fake->SetMarkerSize(HitSize);
    pmyx_fake->Draw();

    XYZ->cd();

    TPolyMarker* pmxyz = new TPolyMarker(n_poly, z_poly_turned, x_poly_turned);
    pmxyz->SetMarkerColor(mcolor[ista]);
    pmxyz->SetMarkerStyle(hitsMStyle);
    pmxyz->SetMarkerSize(HitSize);
    pmxyz->Draw();

    TPolyMarker* pmxyz_fake = new TPolyMarker(n_poly_fake, z_poly_fake_turned, x_poly_fake_turned);
    pmxyz_fake->SetMarkerColor(mcolor[ista]);
    pmxyz_fake->SetMarkerStyle(fakesMStyle);
    pmxyz_fake->SetMarkerSize(HitSize);
    pmxyz_fake->Draw();
  }

}  // DrawInputHits

void L1AlgoDraw::DrawRestHits(L1HitIndex_t* StsRestHitsStartIndex, L1HitIndex_t* StsRestHitsStopIndex,
                              unsigned int* realIHit)
{

  TLatex latex;
  latex.SetTextFont(132);
  latex.SetTextAlign(12);
  latex.SetTextSize(0.035);

  YZ->cd();
  latex.DrawLatex(0.0, 45.0, "YZ Side View");
  YZ->Draw();
  XZ->cd();
  latex.DrawLatex(0.0, 45.0, "XZ Top View");
  XZ->Draw();
  YX->cd();
  latex.DrawLatex(-45.0, 45.0, "YX Front View");
  YX->Draw();


  int nhits = vHits.size();
  Double_t x_poly[nhits], y_poly[nhits], z_poly[nhits];
  Double_t x_poly_fake[nhits], y_poly_fake[nhits], z_poly_fake[nhits];


  for (int ista = NStations - 1; ista >= 0; ista--) {  //  //start downstream chambers
    L1Station& st     = vStations[ista];
    Int_t n_poly      = 0;
    Int_t n_poly_fake = 0;
    for (L1HitIndex_t iRestHit = StsRestHitsStartIndex[ista]; iRestHit < StsRestHitsStopIndex[ista]; iRestHit++) {
      int ih   = realIHit[iRestHit];
      L1Hit& h = vHits[ih];
      int iMC  = CbmL1::Instance()->fvHitPointIndexes[ih];
      //if( (vSFlag[h.f] | vSFlagB[h.b] )&0x02 ) continue; // if used

      fscal x, y, z;
      algo->GetHitCoor(h, x, y, z, st);
      if (iMC >= 0) {
        x_poly[n_poly] = x;
        y_poly[n_poly] = y;
        z_poly[n_poly] = z;
        n_poly++;
      }
      else {
        x_poly_fake[n_poly_fake] = x;
        y_poly_fake[n_poly_fake] = y;
        z_poly_fake[n_poly_fake] = z;
        n_poly_fake++;
      }
    }

    YZ->cd();

    TLine* line = new TLine();
    line->SetLineColor(StaColor);
    line->DrawLine(st.fZ[0], -st.Rmax[0], st.fZ[0], st.Rmax[0]);

    TPolyMarker* pmyz = new TPolyMarker(n_poly, z_poly, y_poly);
    pmyz->SetMarkerColor(mcolor[ista]);
    pmyz->SetMarkerStyle(hitsMStyle);
    pmyz->SetMarkerSize(HitSize);
    pmyz->Draw();

    TPolyMarker* pmyz_fake = new TPolyMarker(n_poly_fake, z_poly_fake, y_poly_fake);
    pmyz_fake->SetMarkerColor(mcolor[ista]);
    pmyz_fake->SetMarkerStyle(fakesMStyle);
    pmyz_fake->SetMarkerSize(HitSize);
    pmyz_fake->Draw();

    XZ->cd();

    line->DrawLine(st.fZ[0], -st.Rmax[0], st.fZ[0], st.Rmax[0]);

    TPolyMarker* pmxz = new TPolyMarker(n_poly, z_poly, x_poly);
    pmxz->SetMarkerColor(mcolor[ista]);
    pmxz->SetMarkerStyle(hitsMStyle);
    pmxz->SetMarkerSize(HitSize);
    pmxz->Draw();

    TPolyMarker* pmxz_fake = new TPolyMarker(n_poly_fake, z_poly_fake, x_poly_fake);
    pmxz_fake->SetMarkerColor(mcolor[ista]);
    pmxz_fake->SetMarkerStyle(fakesMStyle);
    pmxz_fake->SetMarkerSize(HitSize);
    pmxz_fake->Draw();

    YX->cd();

    TEllipse* ellipse = new TEllipse(0.0, 0.0, st.Rmax[0]);
    ellipse->SetLineColor(StaColor);
    ellipse->SetFillStyle(0);
    ellipse->Draw();

    TPolyMarker* pmyx = new TPolyMarker(n_poly, x_poly, y_poly);
    pmyx->SetMarkerColor(mcolor[ista]);
    pmyx->SetMarkerStyle(hitsMStyle);
    pmyx->SetMarkerSize(HitSize);
    pmyx->Draw();

    TPolyMarker* pmyx_fake = new TPolyMarker(n_poly_fake, x_poly_fake, y_poly_fake);
    pmyx_fake->SetMarkerColor(mcolor[ista]);
    pmyx_fake->SetMarkerStyle(fakesMStyle);
    pmyx_fake->SetMarkerSize(HitSize);
    pmyx_fake->Draw();
  }

}  // DrawCurHits

void L1AlgoDraw::DrawAsk()
{
  char symbol;
  if (ask) {
    std::cout << "ask>";
    do {
      std::cin.get(symbol);
      if (symbol == 'r') ask = false;
      if (symbol == 'q') exit(0);
    } while (symbol != '\n');
    std::cout << endl;
  }
}

void L1AlgoDraw::ClearVeiw()
{
  YZ->Clear();
  XZ->Clear();
  YX->Clear();
  XYZ->Clear();
}

L1AlgoDraw::Point L1AlgoDraw::GetHitCoor(int ih)
{
  L1Hit& hit = vHits[ih];
  // find station
  int ista = 0;
  for (int i = 0; i < NStations; i++) {
    if ((HitsStartIndex[i] <= ih) && (HitsStopIndex[i] > ih)) {
      ista = i;
      break;
    }
  }
  L1Station& sta = vStations[ista];
  fscal x, y, z;
  algo->GetHitCoor(hit, x, y, z, sta);
  return Point(x, y, z);
};

void L1AlgoDraw::SaveCanvas(TString name)
{
  system("mkdir L1CADraw -p");
  chdir("L1CADraw");
  TString tmp = name;
  tmp += "YXView.pdf";
  YX->cd();
  //   YX->SaveAs("YXView.eps");
  YX->SaveAs(tmp);

  tmp = name;
  tmp += "XZView.pdf";
  XZ->cd();
  //   XZ->SaveAs("XZView.eps");
  XZ->SaveAs(tmp);

  tmp = name;
  tmp += "YZView.pdf";
  YZ->cd();
  //   YZ->SaveAs("YZView.eps");
  YZ->SaveAs(tmp);

  tmp = name;
  tmp += "XYZView.pdf";
  XYZ->cd();
  XYZ->SaveAs(tmp);
  //   XYZ->SaveAs("XYZView.eps");
  //   XYZ->SaveAs("XYZView.png");
  //   XYZ->SaveAs("XYZView.pdf");
  chdir("..");
}
