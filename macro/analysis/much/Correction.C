//---------------------------------------------------
//
// Example of efficiency correction procedure for omega meson
//
// 3-D YPtM histograms have to be produced using:
// for reconstructed data - InvariantMassSpectra_withMC.C or InvariantMassSpectra_woMC.C
//                          or after CbmAnaDimuonAnalysis task
// for PLUTO signals      - Pluto_analysis.C
//
//
//
// Anna Senger a.senger@gsi.de
//
//---------------------------------------------------

void Correction(TString PLUTOfile     = "YPtM_8gev.pluto.omega.root",
                TString ACCfile       = "accMap_sis100_muon_lmvm.root",
                TString RECOfileOmega = "YPtM_8gev.reco.omega.root",
                TString RECOfileBg    = "YPtM_8gev.reco.centr.SE.root",
                TString OUTfile       = "YPt_8gev.corrected.omega.root",
                TString fit_type      = "expo") {
  gStyle->SetCanvasColor(10);
  gStyle->SetFrameFillColor(10);
  gStyle->SetHistLineWidth(6);
  gStyle->SetPadColor(10);
  gStyle->SetStatColor(10);
  gStyle->SetPalette(55);
  gStyle->SetOptStat(0);

  Double_t sgn_BR   = 9e-5;
  Double_t sgn_mult = 19;
  Double_t sgnMass  = 0.783;

  TFile* f1   = new TFile(RECOfileOmega.Data());
  TH3D* hReco = (TH3D*) f1->Get("YPtM");
  hReco->Scale(
    sgn_BR
    * sgn_mult);  // if YPtM file was produced after CbmAnaDimuonAnalysis task

  TFile* f11 = new TFile(RECOfileBg.Data());
  hReco->Add((TH3D*) f11->Get("YPtM_bg"));

  TFile* f2    = new TFile(PLUTOfile.Data());
  TH3D* hPluto = (TH3D*) f2->Get("YPtM_omega");

  Int_t NofBinsX = hReco->GetNbinsX();
  Int_t NofBinsY = hReco->GetNbinsY();
  Int_t NofBinsZ = hReco->GetNbinsZ();

  Double_t Xmin = hReco->GetXaxis()->GetXmin();
  Double_t Xmax = hReco->GetXaxis()->GetXmax();

  Double_t Ymin = hReco->GetYaxis()->GetXmin();
  Double_t Ymax = hReco->GetYaxis()->GetXmax();

  Double_t Zmin = hReco->GetZaxis()->GetXmin();
  Double_t Zmax = hReco->GetZaxis()->GetXmax();

  Double_t Zbin = (Zmax - Zmin) / (Double_t) NofBinsZ;
  Double_t Xbin = (Xmax - Xmin) / (Double_t) NofBinsX;
  Double_t Ybin = (Ymax - Ymin) / (Double_t) NofBinsY;

  TString dir  = getenv("VMCWORKDIR");
  TString name = dir + "/parameters/much/" + ACCfile;

  TFile* f3    = new TFile(name.Data());
  TH2D* AccMap = (TH2D*) f3->Get("AcceptanceMap");

  TH2D* plutoYPt = new TH2D(
    "plutoYPt", "plutoYPt", NofBinsX, Xmin, Xmax, NofBinsY, Ymin, Ymax);
  plutoYPt->SetTitle("pluto #omega");
  plutoYPt->GetXaxis()->SetTitle("Y");
  plutoYPt->GetYaxis()->SetTitle("Pt, GeV/c");

  TH2D* accYPt = (TH2D*) plutoYPt->Clone("accYPt");
  accYPt->SetTitle("acceptance");
  TH2D* effYPt = (TH2D*) plutoYPt->Clone("effYPt");
  effYPt->SetTitle("efficiency");
  TH2D* recoYPt = (TH2D*) plutoYPt->Clone("recoYPt");
  recoYPt->SetTitle("reconstructed #omega");
  TH2D* corrYPt = (TH2D*) plutoYPt->Clone("corrYPt");
  corrYPt->SetTitle("corrected #omega");

  Double_t initPar[] = {sgnMass,
                        0.02};  // omega mass and sigma for fit initialization

  name.Form("gaus(0)+%s(3)", fit_type.Data());
  TF1* fit = new TF1("fullFit", name.Data(), 0.60, 0.90);

  TF1* fitSgn = new TF1("sgnFit", "gaus", 0.60, 0.90);

  fit->SetParNames("Const", "Mass", "Sigma", "a0", "a1", "a2");
  fit->SetNpx(10000);
  fit->SetLineColor(2);

  static int NN = plutoYPt->GetNbinsX() * plutoYPt->GetNbinsY();

  Int_t counter = 0;
  TH1D* invM_PLUTO[NN];
  TH1D* invM[NN];

  name.Form("invM_YPt_%s_sis100_muon_lmvm.root", fit_type.Data());
  TFile* FFF = new TFile(name.Data(), "recreate");

  //-------------------------------
  for (int i = 1; i < NofBinsX; i++) {
    for (int j = 1; j < NofBinsY; j++) {

      name.Form("z%d%d", i, j);
      invM_PLUTO[counter] =
        (TH1D*) hPluto->ProjectionZ(name, i - 1, i, j - 1, j);
      plutoYPt->Fill(i * Xbin + Xmin - Xbin / 2.,
                     j * Ybin + Ymin - Ybin / 2.,
                     invM_PLUTO[counter]->Integral());

      if (AccMap->GetBinContent(i, j) != 0)
        accYPt->SetBinContent(i, j, plutoYPt->GetBinContent(i, j));
      else
        accYPt->SetBinContent(i, j, 0);

      name.Form("Y[%2.2f,%2.2f], Pt[%2.2f,%2.2f]",
                (Double_t)(i - 1) * Xbin + Xmin,
                (Double_t) i * Xbin + Xmin,
                (Double_t)(j - 1) * Ybin + Ymin,
                (Double_t) j * Ybin + Ymin);
      invM[counter] = (TH1D*) hReco->ProjectionZ(name, i - 1, i, j - 1, j);

      if (invM[counter]->GetEntries() == 0) continue;

      invM[counter]->Fit("sgnFit", "QRN", "", sgnMass - 0.1, sgnMass + 0.1);

      float start = fitSgn->GetParameter(1) - 2 * fitSgn->GetParameter(2);
      float end   = fitSgn->GetParameter(1) + 2 * fitSgn->GetParameter(2);
      Double_t I =
        invM[counter]->Integral((Int_t)(start / Zbin), (Int_t)(end / Zbin));
      if (I < 1e-6) continue;

      fit->SetParameter(1, (0.60 + 0.90) / 2);
      fit->SetParameter(2, (0.90 - 0.60) / 4);
      fit->SetParLimits(2, 0, 1);

      fit->FixParameter(1, initPar[0]);
      fit->FixParameter(2, initPar[1]);

      invM[counter]->Fit("fullFit", "QRN", "", 0.60, 0.90);

      start = fit->GetParameter(1) - 6 * fit->GetParameter(2);
      end   = fit->GetParameter(1) + 6 * fit->GetParameter(2);

      invM[counter]->Fit("fullFit", "QRN", "", start, end);

      fit->ReleaseParameter(1);
      fit->ReleaseParameter(2);

      start = fit->GetParameter(1) - 3 * fit->GetParameter(2);
      end   = fit->GetParameter(1) + 3 * fit->GetParameter(2);

      invM[counter]->Fit("fullFit", "", "", start, end);

      if (fit->GetParameter(1) > 0.8 || fit->GetParameter(1) < 0.75) continue;

      for (int iPar = 0; iPar < 3; iPar++)
        fitSgn->SetParameter(iPar, fit->GetParameter(iPar));

      float S = fitSgn->Integral(start, end) / Zbin;
      recoYPt->Fill(
        i * Xbin + Xmin - Xbin / 2., j * Ybin + Ymin - Ybin / 2., S);

      invM[counter]->Write();

      counter++;
    }
  }
  //-------------------------------
  FFF->Close();
  gPad->Close();

  effYPt->Divide(recoYPt, accYPt);
  corrYPt->Divide(recoYPt, effYPt);

  /*
 TCanvas *myc2 = new TCanvas("myc2","myc2"); 
 myc2->Divide(5,1);
 myc2->cd(1);gPad->SetLogz();
 plutoYPt->Draw("colz");
 
 myc2->cd(2);gPad->SetLogz();
 accYPt->Draw("colz");
 
 myc2->cd(3);gPad->SetLogz();
 recoYPt->Draw("colz");
 
 myc2->cd(4);gPad->SetLogz();
 effYPt->Draw("colz");

 myc2->cd(5);gPad->SetLogz();
 corrYPt->Draw("colz");

 TH1D* plutoY = (TH1D*)plutoYPt->ProjectionX("plutoY_px");plutoY->SetLineColor(kBlack);
 TH1D* accY   = (TH1D*)accYPt->ProjectionX("accY_px");    accY->SetLineColor(kRed);
 TH1D* recoY  = (TH1D*)recoYPt->ProjectionX("recoY_px");  recoY->SetLineColor(kRed);
 TH1D* corrY  = (TH1D*)corrYPt->ProjectionX("corrY_px");  corrY->SetLineColor(kBlue);

 TH1D* plutoPt = (TH1D*)plutoYPt->ProjectionY("plutoPt_py");plutoPt->SetLineColor(kBlack);
 TH1D* accPt   = (TH1D*)accYPt->ProjectionY("accY_py");     accPt->SetLineColor(kRed);
 TH1D* recoPt  = (TH1D*)recoYPt->ProjectionY("recoPt_py");  recoPt->SetLineColor(kRed);
 TH1D* corrPt  = (TH1D*)corrYPt->ProjectionY("corrPt_py");  corrPt->SetLineColor(kBlue);
 
 TCanvas *myc = new TCanvas("myc","myc"); 
 myc->Divide(2,1);
 myc->cd(1);gPad->SetLogy();
 
 plutoY->Draw();
 accY->Draw("same");
 corrY->Draw("same");
 
 myc->cd(2);gPad->SetLogy();
 plutoPt->Draw();
 accPt->Draw("same");
 corrPt->Draw("same");
 */

  TFile* FF = new TFile(OUTfile.Data(), "recreate");
  plutoYPt->Write();
  accYPt->Write();
  recoYPt->Write();
  effYPt->Write();
  corrYPt->Write();
  FF->Close();
  f1->Close();
  f11->Close();
  f2->Close();
  f3->Close();
}
