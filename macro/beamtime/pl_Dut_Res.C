void pl_Dut_Res(Int_t iDut = 911, TString sysinfo = "")
{
  gROOT->LoadMacro(((TString) gSystem->Getenv("VMCWORKDIR") + "/macro/beamtime/pl_Datime.C").Data());
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can = new TCanvas("can", "can", 48, 55, 700, 700);
  can->Divide(2, 2);

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(kTRUE);
  gStyle->SetOptFit(kTRUE);

  gROOT->cd();
  gROOT->SetDirLevel(1);
  // cout << " DirLevel "<< gROOT->GetDirLevel()<< endl;

  TH1* h;
  TH1* h1;
  TH2* h2;
  TH2* h2f;
  TH2* h2m;
  Double_t Xres, Yres, Tres, TBres;

  // if (hPla!=NULL) hPla->Delete();
  TString hname   = "";
  TProfile* h2pfx = NULL;

  can->cd(1);

  gROOT->cd();
  hname = Form("hDutPullX_Sm_%d", iDut);
  cout << " Look for histo " << hname << endl;
  h1 = (TH1*) gROOT->FindObjectAny(hname);
  if (h1 != NULL) {
    Double_t dFMean    = h1->GetMean();
    Double_t dFLim     = 3.0 * h1->GetRMS();
    TFitResultPtr fRes = h1->Fit("gaus", "S", "", dFMean - dFLim, dFMean + dFLim);
    h1->Draw();
    if (0 == (Int_t) fRes) Xres = fRes->Parameter(2);
    gPad->SetLogy();
  }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(2);
  hname = Form("hDutPullY_Sm_%d", iDut);
  cout << " Look for histo " << hname << endl;
  h1 = (TH1*) gROOT->FindObjectAny(hname);
  if (h1 != NULL) {
    Double_t dFMean    = h1->GetMean();
    Double_t dFLim     = 3.0 * h1->GetRMS();
    TFitResultPtr fRes = h1->Fit("gaus", "S", "", dFMean - dFLim, dFMean + dFLim);
    if (0 == (Int_t) fRes) Yres = fRes->Parameter(2);
    h1->Draw();
    gPad->SetLogy();
  }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(3);
  hname = Form("hDutPullT_Sm_%d", iDut);
  cout << " Look for histo " << hname << endl;
  h1 = (TH1*) gROOT->FindObjectAny(hname);
  if (h1 != NULL) {
    Double_t dFMean    = h1->GetMean();
    Double_t dFLim     = 3.0 * h1->GetRMS();
    TFitResultPtr fRes = h1->Fit("gaus", "S", "", dFMean - dFLim, dFMean + dFLim);
    if (0 == (Int_t) fRes) Tres = fRes->Parameter(2);
    h1->Draw();
    gPad->SetLogy();
  }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(4);
  hname = Form("hDutPullTB_Sm_%d", iDut);
  cout << " Look for histo " << hname << endl;
  h1 = (TH1*) gROOT->FindObjectAny(hname);
  if (h1 != NULL) {
    Double_t dFMean    = h1->GetMean();
    Double_t dFLim     = 3.0 * h1->GetRMS();
    TFitResultPtr fRes = h1->Fit("gaus", "S", "", dFMean - dFLim, dFMean + dFLim);
    if (0 == (Int_t) fRes) TBres = fRes->Parameter(2);
    h1->Draw();
    gPad->SetLogy();
  }
  else {
    cout << hname << " not found" << endl;
  }

  TString FADD = Form("pl_Datime(\"%s\")", sysinfo.Data());
  if (gROOT->IsBatch()) { gInterpreter->ProcessLine(FADD.Data()); }

  can->SaveAs(Form("pl_Dut_Res_%d.pdf", iDut));

  cout << Form("Dut gaus resolutions: X %5.3f, Y %5.3f, T %5.3f, TB %5.3f", Xres, Yres, Tres, TBres) << endl;
}
