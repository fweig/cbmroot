/** @file pl_all_LHTime.C
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [orginator] **/

void pl_all_LHTime(Int_t iNDet = 22, Double_t Tstart = 1., Double_t Tend = 1000.)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  //  TCanvas *can = new TCanvas("can","can",48,55,700,900);
  TCanvas* can = new TCanvas("can", "can", 48, 56, 900, 900);
  can->Divide(5, 5, 0.01, 0.01);
  //  can->Divide(2,2,0,0);
  Float_t lsize = 0.07;

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetLabelSize(lsize);

  TH1* h;
  TH2* h2;

  Int_t iCanv = 0;
  // if (h!=NULL) h->Delete();

  for (Int_t iDet = 0; iDet < iNDet; iDet++) {

    can->cd(iCanv + 1);
    iCanv++;
    gROOT->cd();
    TString hname = Form("hLHTime_Det%d", iDet);
    h             = (TH1*) gROOT->FindObjectAny(hname);
    if (h != NULL) {
      //     h->GetXaxis()->SetRange(Tstart,Tend);
      h->Draw("colz");
    }
    else {
      cout << "Histogram " << hname << " not existing. " << endl;
    }
  }
  can->SaveAs(Form("pl_all_CluRate.pdf"));
}
