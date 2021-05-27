void pl_resi2D_trk(Int_t NSt = 8, Int_t iVar = 0, Int_t iFit = 0)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can = new TCanvas("can", "can", 50, 0, 800, 800);
  switch (NSt) {
    case 6:
    case 5:
    case 4: can->Divide(3, 3); break;
    case 7:
    case 8: can->Divide(3, 4); break;
    case 16:
    case 18: can->Divide(4, 5); break;
    case 30: can->Divide(6, 6); break;
    case 39: can->Divide(6, 7); break;
    default: can->Divide(5, 5); ;
  }
  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(kTRUE);

  gROOT->cd();
  gROOT->SetDirLevel(1);

  TH1* h;
  TH1* h1;
  TH2* h2;

  const Int_t MSt = 30;
  Double_t vSt[MSt];
  Double_t vMean[MSt];
  Double_t vSig[MSt];
  Double_t vRes[MSt];
  Double_t vStErr[MSt];
  Double_t vMeanErr[MSt];
  Double_t vSigErr[MSt];
  Double_t vResErr[MSt];
  // if (h!=NULL) h->Delete();
  Int_t iCan = 1;
  TString var;
  Double_t Nall;

  switch (iVar) {
    case 0: var = "ResidualTBWalk"; break;
    case 1: var = "ResidualYWalk"; break;
  }

  for (Int_t iSt = 0; iSt < NSt; iSt++) {
    if (iCan == 36) iCan = 1;
    can->cd(iCan++);
    gROOT->cd();
    TString hname = Form("h%s_Station_%d", var.Data(), iSt);
    h2            = (TH2*) gROOT->FindObjectAny(hname);
    if (h2 != NULL) {
      h2->Draw("colz");
      gPad->SetLogz();
      gPad->SetGridx();
      gPad->SetGridy();
    }
    else {
      cout << hname << " not found" << endl;
    }
  }

  can->SaveAs(Form("pl_trk_%s_%02d.pdf", var.Data(), NSt));
}
