void pl_ext_SUT(Double_t dThr = 0., Double_t MinEff = 0.)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can = new TCanvas("can", "can", 50, 0, 800, 800);
  can->Divide(3, 3);

  //gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(kTRUE);

  gROOT->cd();
  gROOT->SetDirLevel(1);

  TH1* h;
  TH1* h1;
  TH2* h2;
  TH2* h2f;  // found hits
  TH2* h2m;  // missed hits
  TH3* h3;
  TString hname = "";
  Double_t Nall, eff;

  // if (h!=NULL) h->Delete();

  can->cd(1);
  gROOT->cd();
  hname = Form("hExtSutXY_Found");
  h2f   = (TH2*) gROOT->FindObjectAny(hname);
  if (h2f != NULL) {
    h2f->Draw("colz");
    //gPad->SetLogz();
    //h2->ProfileX()->Draw("same");
  }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(2);
  gROOT->cd();
  hname = Form("hExtSutXY_Missed");
  h2m   = (TH2*) gROOT->FindObjectAny(hname);
  if (h2m != NULL) {
    h2m->Draw("colz");
    //gPad->SetLogz();
    //h2->ProfileX()->Draw("same");
  }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(3);
  if (1) {
    if (h2f != NULL) {
      TH2* h2acc = (TH2*) h2f->Clone(Form("hAccSUT"));
      //h2acc->Divide(h2,h2,1.,1.,"B");
      h2acc->Reset();
      Int_t Nbins   = h2f->GetNbinsX() * h2f->GetNbinsY();
      Double_t dMax = dThr * h2f->GetMaximum();
      for (Int_t i = 0; i < (Int_t) h2f->GetNbinsX(); i++)
        for (Int_t j = 0; j < (Int_t) h2f->GetNbinsY(); j++) {
          h2f->GetBinContent(i + 1, j + 1) < dMax ? h2acc->SetBinContent(i + 1, j + 1, 0.)
                                                  : h2acc->SetBinContent(i + 1, j + 1, 1.);
        }
      TH2* h2aall = (TH2*) h2f->Clone(Form("hALL_accepted"));
      h2aall->Multiply(h2aall, h2acc, 1., 1., "B");
      Nall = h2aall->Integral();
      if (h2m != NULL) {
        Double_t Nmis  = h2m->GetEntries();
        Double_t NmisI = h2m->Integral();
        TH2* h2missed  = (TH2*) h2m->Clone(Form("Missed_acc"));
        h2missed->Multiply(h2m, h2acc, 1., 1., "B");
        Double_t NmisaI = h2missed->Integral();
        eff             = 1. - NmisaI / (Nall + NmisaI);
        cout << "Efficiency of Sut: all " << Nall << ", mis " << Nmis << ", " << NmisI << ", " << NmisaI << " -> "
             << Form("%6.3f", eff) << endl;

        hname      = Form("Efficiency");
        TH2* h2eff = (TH2*) gROOT->FindObjectAny(hname);
        if (NULL != h2eff) h2eff->Delete();
        h2eff      = (TH2*) h2f->Clone(hname);
        hname      = Form("Total");
        TH2* h2tot = (TH2*) gROOT->FindObjectAny(hname);
        if (NULL != h2tot) h2tot->Delete();
        h2tot = (TH2*) h2m->Clone(hname);
        h2tot->Add(h2tot, h2f, 1., 1.);
        h2tot->Multiply(h2tot, h2acc, 1., 1., "B");
        h2eff->Reset();
        h2eff->Divide(h2f, h2tot, 1., 1., "B");
        h2eff->SetTitle(Form("Efficiency of Sut"));
        //h2frac->SetMaximum(0.99);
        h2eff->GetZaxis()->SetRangeUser(MinEff, 1.);
        h2eff->Draw("colz");
        gPad->Update();
        h2eff->Draw("colz");
      }
    }
  }
  can->cd(4);
  gROOT->cd();
  hname = Form("hExtSutXY_DX");
  h3    = (TH3*) gROOT->FindObjectAny(hname);
  if (h3 != NULL) {
    h3->Project3DProfile("yx")->Draw("colz");
    //h3->Draw("colz");
    //gPad->SetLogz();
    can->cd(7);
    h3->ProjectionZ()->Draw("");
  }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(5);
  gROOT->cd();
  hname = Form("hExtSutXY_DY");
  h3    = (TH3*) gROOT->FindObjectAny(hname);
  if (h3 != NULL) {
    h3->Project3DProfile("yx")->Draw("colz");
    //h3->Draw("colz");
    //gPad->SetLogz();
    can->cd(8);
    h3->ProjectionZ()->Draw("");
  }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(6);
  gROOT->cd();
  hname = Form("hExtSutXY_DT");
  h3    = (TH3*) gROOT->FindObjectAny(hname);
  if (h3 != NULL) {
    h3->Project3DProfile("yx")->Draw("colz");
    //h3->Draw("colz");
    //gPad->SetLogz();
    can->cd(9);
    h3->ProjectionZ()->Draw("");
  }
  else {
    cout << hname << " not found" << endl;
  }


  can->SaveAs("pl_ext_SUT.pdf");
}
