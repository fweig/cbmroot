void pl_cmp_1D(Int_t iOpt      = 1,
               TString fNameMC = "/home/nh/KRONOS/mc/mcbm/"
                                 "mcbm_beam_2019_03.agau.1.58gev.mbias.reco_hst.root",
               TString fNameData = "./hst/"
                                   "159.50.5.1.0_050_010020500_000_002012_022_0.9_2.5_"
                                   "trk001_Cal159.50.5.1.0_Ana.hst.root",
               TString hname = "hTrklVelHMul")
{
  //plot initialisation
  TCanvas* can = new TCanvas("can", "can", 50, 50, 686, 686);
  can->Divide(1, 1, 0.01, 0.01);
  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(kTRUE);
  gStyle->SetOptStat(" ");
  gStyle->SetOptStat(10);
  //cout << "gStyle label size: "<<  gStyle->GetLabelSize() << endl;

  // file opening

  TFile* f1 = new TFile(fNameMC.Data());
  TH2* h21  = (TH2*) f1->Get(hname.Data())->Clone();

  TFile* f2 = new TFile(fNameData.Data());
  TH2* h22  = (TH2*) f2->Get(hname.Data())->Clone();
  //  f2->Close();
  gDirectory->cd();
  //  cout << Form("h21 %p, h22 %p",h21,h22)<<endl;

  TH1* h1;
  TH1* h2;

  switch (iOpt) {
    case 0:  // x-projection
      ;
      break;

    case 10:
    case 1:  // y- projections
      h1 = (TH1*) h21->ProjectionY("_py1");
      h2 = (TH1*) h22->ProjectionY("_py2");
      //    cout << Form("h21 %p, h22 %p",h21,h22)<<endl;
      //    cout << Form("h1 %p, h2 %p",h1,h2)<<endl;

      break;

    case 2:  // x-projection with box fit
      ;
      break;

    default: cout << "Option " << iOpt << " not available " << endl; return;
  }

  can->cd(1);

  h1->Draw("");
  h1->SetLineColor(kRed);
  h2->SetLineColor(kBlue);
  Double_t N1 = h1->GetEntries();
  Double_t N2 = h2->GetEntries();
  TH1* h1s    = (TH1*) h1->Clone();
  h1s->Scale(N2 / N1);
  h2->SetMaximum(TMath::Max(h1s->GetMaximum(), h2->GetMaximum()) * 1.1);
  h2->Draw("");
  h1s->Draw("sameHIST");

  TLegend* leg = new TLegend(0.25, 0.7, 0.5, 0.85);
  leg->SetTextSize(0.03);
  leg->AddEntry(h1s, "MC", "l");
  leg->AddEntry(h2, "data", "l");
  leg->Draw();
  /*
  gPad->SetLogz();
  Float_t newx1=0.2; // left side of stat window 
  Float_t newx2=0.4; // right side of stat window
  Float_t newy1=0.84; // bottom side of stat window 
  Float_t newy2=0.9; // top side of stat window
  TPaveStats *st = (TPaveStats*)h1->GetListOfFunctions()->FindObject("stats");
  if(NULL !=st){
    st->SetX1NDC(newx1);
    st->SetX2NDC(newx2);
    st->SetY1NDC(newy1);
    st->SetY2NDC(newy2);
  }
  */
}
