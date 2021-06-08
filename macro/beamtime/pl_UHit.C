/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

void pl_UHit(Int_t iNSt = 10, Int_t iOpt = 0, Int_t i2D = 0, Int_t i1D = 0, Double_t dXmax = 0., Double_t dXmin = 0.)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can;
  if (dXmax == -1.) {
    //    gROOT->GetObject("TCanvas::can",can);
    can = (TCanvas*) gROOT->FindObject("can");
    if (NULL == can) {
      cout << "No pointer to Canvas, return" << endl;
      return;
    }
  }
  else {
    can = new TCanvas("can", "can", 50, 0, 800, 800);
    if (iOpt < 100) can->Divide(3, 4);
    gPad->SetFillColor(0);
    gStyle->SetPalette(1);
    gStyle->SetOptStat(kFALSE);
  }
  gROOT->cd();
  gROOT->SetDirLevel(1);

  TH1* h;
  TH2* hn;
  TH1* h1;
  TH2* h2;
  TH3* h3;
  TH3* h3f;

  TString hsrc;
  TString hname;
  TString hnorm;
  TString cOpt;
  TString c2D;
  TString c1D;

  const Int_t ColMap[8] = {1, 2, 3, 4, 6, 7, 8, 9};

  Double_t NHits = 1.;
  Double_t dProb = 0.;

  switch (i2D) {
    case 0: c2D = "yx"; break;
    case 1: c2D = "zx"; break;
    case 2: c2D = "zy"; break;
    default: cout << "i2D mode not implemented " << endl; return;
  }

  switch (i1D) {
    case 0: hsrc = "hUDXDY_DT_"; break;
    case 1: hsrc = "hUCDXDY_DT_"; break;
  }

  iCan = 1;
  can->cd(iCan);

  const Int_t iPadLoc[10] = {2, 11, 5, 8, 1, 4, 7, 3, 6, 9};
  if (iOpt == 0) {
    for (Int_t iSt = 0; iSt < iNSt; iSt++) {
      can->cd(iPadLoc[iSt]);
      hname = Form("%s%d", hsrc.Data(), iSt);
      h3f   = (TH3*) gROOT->FindObjectAny(hname);
      cout << hname.Data() << " with pointer  " << h3f << " at iCan = " << iCan << endl;
      if (h3f != NULL) {
        h2 = (TH2*) h3f->Project3D(c2D.Data());
        h2->SetTitle(Form("%s", h2->GetName()));
        h2->Draw("colz");
      }
      else
        cout << hname << " not found" << endl;
    }
  }
  else  // 1D projections
  {
    for (Int_t iSt = 0; iSt < iNSt; iSt++) {
      can->cd(iPadLoc[iSt]);
      hname = Form("%s%d", hsrc.Data(), iSt);
      hnorm = Form("hXY_AllTracks_%d", iSt);
      h3f   = (TH3*) gROOT->FindObjectAny(hname);
      hn    = (TH2*) gROOT->FindObjectAny(hnorm);
      //cout << hname.Data() <<" with pointer  "<<h3f<<" at iCan = "<<iCan<<endl;
      if (h3f != NULL) {
        switch (iOpt) {
          case 1: h = (TH1*) h3f->Project3DProfile(c2D.Data())->ProfileX(); break;
          case 2: h = (TH1*) h3f->Project3DProfile(c2D.Data())->ProfileY(); break;
          case 3: break;

          case 10: h = (TH1*) h3f->ProjectionX(); break;
          case 11: h = (TH1*) h3f->ProjectionY(); break;
          case 12: h = (TH1*) h3f->ProjectionZ(); break;

          case 20:
            h     = (TH1*) h3f->ProjectionX();
            NHits = hn->Integral();
            dProb = h->Integral() / NHits;
            h->Scale(1. / NHits);
            break;
          case 21:
            h     = (TH1*) h3f->ProjectionY();
            NHits = hn->Integral();
            dProb = h->Integral() / NHits;
            h->Scale(1. / NHits);
            break;
          case 22:
            h     = (TH1*) h3f->ProjectionZ();
            NHits = hn->Integral();
            dProb = h->Integral() / NHits;
            cout << " Got  " << NHits << " normalisation hits from histo " << hn->GetName()
                 << " => probability = " << dProb << endl;
            h->Scale(1. / NHits);
            break;

          case 30:
            h     = (TH1*) h3f->ProjectionX(Form("%s%s", h3f->GetName(), "_pz_prompt"), 1, 11, 51, 51);
            NHits = hn->Integral();
            dProb = h->Integral() / NHits;
            h->Scale(1. / NHits);
            break;
          case 31:
            h     = (TH1*) h3f->ProjectionY(Form("%s%s", h3f->GetName(), "_pz_prompt"), 1, 11, 51, 51);
            NHits = hn->Integral();
            dProb = h->Integral() / NHits;
            h->Scale(1. / NHits);
            break;
          case 32:
            //h = (TH1 *)h3f->ProjectionZ(Form("%s%s",h3f->GetName(),"_pzcen"),6,6,6,6);
            h     = (TH1*) h3f->ProjectionZ(Form("%s%s", h3f->GetName(), "_pzcen"), 5, 7, 5, 7);
            NHits = hn->Integral();
            dProb = h->Integral() / NHits;
            cout << " Got  " << NHits << " normalisation hits from histo " << hn->GetName()
                 << " => probability = " << dProb << endl;
            h->Scale(1. / NHits);
            break;
          case 33:
            h     = (TH1*) h3f->ProjectionZ(Form("%s%s", h3f->GetName(), "_pzxl"), 6, 6, 1, 11);
            NHits = hn->Integral();
            dProb = h->Integral() / NHits;
            cout << " Got  " << NHits << " normalisation hits from histo " << hn->GetName()
                 << " => probability = " << dProb << endl;
            h->Scale(1. / NHits);
            break;
          case 34:
            h     = (TH1*) h3f->ProjectionZ(Form("%s%s", h3f->GetName(), "_pzyl"), 1, 11, 6, 6);
            NHits = hn->Integral();
            dProb = h->Integral() / NHits;
            cout << " Got  " << NHits << " normalisation hits from histo " << hn->GetName()
                 << " => probability = " << dProb << endl;
            h->Scale(1. / NHits);
            break;


          default: cout << "Option not available " << endl; return;
        }
        h->SetTitle(Form("%s", h->GetName()));
        //  h->SetLineColor(ColMap[iSt]);  // to distinguish stations in overlay mode
        /*
       if(iSt==0) {
	 h->Draw();
	 if (dYmax>0.) {
	   h->SetMinimum(-dYmax);
	   h->SetMaximum(dYmax);
	 }
       } 
       else {
	 h->Draw("same");
       }
       */
        if (dXmax != -1.) {
          if (dXmax != 0.) h->GetXaxis()->SetRangeUser(dXmin, dXmax);
          h->Draw();
          gPad->SetGridx();
          gPad->SetGridy();
          gPad->SetLogy();
          if (dProb > 0.) {
            TPad* newpad = new TPad("newpad", "a transparent pad", 0, 0, 1, 1);
            newpad->SetFillStyle(4000);
            newpad->Draw();
            newpad->cd();
            TPaveLabel* tit = new TPaveLabel(0.2, 0.75, 0.45, 0.9, Form(" prob  %5.3f ", dProb));
            tit->SetFillColor(0);
            tit->SetTextFont(52);
            tit->SetBorderSize(1);
            tit->Draw();
          }
        }
        else {
          h->Draw("same");
          h->SetLineColor(kRed);
        }
      }
      else
        cout << hname << " not found" << endl;
    }
  }
  can->SaveAs("pl_UHit.pdf");
}
