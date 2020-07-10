void pl_XY_trk(Int_t NSt = 4, Double_t MinEff = 0.5, Double_t dThr = 0.01) {
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can = new TCanvas("can", "can", 50, 0, 800, 800);
  can->Divide(3, NSt);

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(kTRUE);
  gStyle->SetOptStat(11);

  gROOT->cd();
  gROOT->SetDirLevel(1);

  TH1* h;
  TH1* h1;
  TH2* h2;
  TH2* h2m;
  TString hname = "";

  // if (h!=NULL) h->Delete();
  Int_t iCan = 1;
  Double_t eff[NSt];
  Double_t Nall;
  //if(NULL != h2)  h2->Delete();
  //if(NULL != h2m) h2m->Delete();

  for (Int_t iSt = 0; iSt < NSt; iSt++) {
    can->cd(iCan++);
    gROOT->cd();
    hname = Form("hXY_AllStations_%d", iSt);
    h2    = (TH2*) gROOT->FindObjectAny(hname);
    if (h2 != NULL) {
      h2->Draw("colz");
      TH2D* h2acc = (TH2D*) h2->Clone(Form("Acc_%d", iSt));
      //h2acc->Divide(h2,h2,1.,1.,"B");
      h2acc->Reset();
      Int_t Nbins   = h2->GetNbinsX() * h2->GetNbinsY();
      Double_t dMax = dThr * h2->GetMaximum();
      for (Int_t i = 0; i < Nbins; i++)
        h2->GetBinContent(i + 1) < dMax ? h2acc->SetBinContent(i + 1, 0.)
                                        : h2acc->SetBinContent(i + 1, 1.);
      TH2D* h2aall = (TH2D*) h2->Clone(Form("ALL_acc%d", iSt));
      h2aall->Multiply(h2aall, h2acc, 1., 1., "B");
      Nall = h2aall->Integral();
      can->cd(iCan++);
      gROOT->cd();
      hname = Form("hXY_MissedStation_%d", iSt);
      h2m   = (TH2*) gROOT->FindObjectAny(hname);
      if (h2m != NULL) {
        h2m->Draw("colz");
        Double_t Nmis  = h2->GetEntries();
        Double_t NmisI = h2->Integral();
        TH2D* h2missed = (TH2D*) h2->Clone(Form("Missed_%d", iSt));
        h2missed->Multiply(h2m, h2acc, 1., 1., "B");
        Double_t NmisaI = h2missed->Integral();
        eff[iSt]        = 1. - NmisaI / (Nall + NmisaI);
        cout << "Efficiency of Station " << iSt << ": all " << Nall << ", mis "
             << Nmis << ", " << NmisI << ", " << NmisaI << " -> "
             << Form("%6.3f", eff[iSt]) << endl;

        can->cd(iCan++);
        hname       = Form("Efficiency_%d", iSt);
        TH2D* h2eff = (TH2D*) gROOT->FindObjectAny(hname);
        if (NULL != h2eff) h2eff->Delete();
        h2eff       = (TH2D*) h2->Clone(hname);
        hname       = Form("Total_%d", iSt);
        TH2D* h2tot = (TH2D*) gROOT->FindObjectAny(hname);
        if (NULL != h2tot) h2tot->Delete();
        h2tot = (TH2D*) h2m->Clone(hname);
        h2tot->Add(h2tot, h2, 1., 1.);
        h2tot->Multiply(h2tot, h2acc, 1., 1., "B");
        h2eff->Reset();
        h2eff->Divide(h2, h2tot, 1., 1., "B");
        h2eff->SetTitle(Form("Efficiency of station %d", iSt));
        //h2frac->SetMaximum(0.99);
        h2eff->GetZaxis()->SetRangeUser(MinEff, 1.);
        h2eff->Draw("colz");
        gPad->Update();
        h2eff->Draw("colz");

        if (1) {
          TPad* newpad = new TPad("newpad", "a transparent pad", 0, 0, 1, 1);
          newpad->SetFillStyle(4000);
          newpad->Draw();
          newpad->cd();
          TPaveLabel* tit = new TPaveLabel(
            0.2, 0.75, 0.45, 0.9, Form(" <eff>: %5.3f", eff[iSt]));
          tit->SetFillColor(0);
          tit->SetTextFont(52);
          tit->SetBorderSize(1);
          tit->Draw();
        }
      }
    } else {
      cout << hname << " not found" << endl;
    }
  }
  cout << "Eff-summary: Nall, effs = " << Nall;
  for (Int_t iSt = 0; iSt < NSt; iSt++)
    cout << Form(", %6.3f", eff[iSt]);
  cout << endl;

  can->SaveAs("pl_XY_trk.pdf");
}
