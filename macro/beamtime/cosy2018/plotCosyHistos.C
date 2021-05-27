
void plotCosyHistos()
{
  /// Efficiency without corrections
  TProfile* fhEfficiencyNoDeadCorr;
  TH2* fhMapBestPairsHodoGeoCutS1_H1_NoDeadCorr;
  TH2* fhMapBestPairsHodoGeoCutS1_H2_NoDeadCorr;
  TH2* fhMapBestPairsHodoGeoCutS2_H1_NoDeadCorr;
  TH2* fhMapBestPairsHodoGeoCutS2_H2_NoDeadCorr;

  /// Histos with attempt at dead strip correction in Geometric cut
  TH1* fhDtAllPairsHodoS1;
  TH1* fhDtAllPairsHodoS2;

  TH1* fhDtBestPairsHodoS1;
  TH1* fhDtBestPairsHodoS2;

  TH2* fhMapBestPairsHodo_ProjS1;
  TH2* fhMapBestPairsHodo_ProjS2;
  TH2* fhMapBestPairsHodoS1_Proj;
  TH2* fhMapBestPairsHodoS2_Proj;

  TH2* fhResidualsBestPairsHodoS1;
  TH2* fhResidualsBestPairsHodoS2;

  TH1* fhDtBestPairsHodoS1GeoCut;
  TH1* fhDtBestPairsHodoS2GeoCut;
  TH1* fhDtBestPairsHodoS1S2GeoCut;
  TH1* fhDtBestPairsHodoS2S1GeoCut;

  TH2* fhResidualsBestPairsHodoS1GeoCut;
  TH2* fhResidualsBestPairsHodoS2GeoCut;

  TH2* fhMapBestPairsHodoGeoCutS1_H1;
  TH2* fhMapBestPairsHodoGeoCutS1_H2;
  TH2* fhMapBestPairsHodoGeoCutS2_H1;
  TH2* fhMapBestPairsHodoGeoCutS2_H2;
  TH2* fhMapBestPairsHodoS1GeoCut_Proj;
  TH2* fhMapBestPairsHodoS2GeoCut_Proj;

  TProfile* fhEfficiencyCorr;
  TProfile2D* fhEfficiencyMapS1;
  TProfile2D* fhEfficiencyMapS2;
  TProfile2D* fhEfficiencyMapS1S2;
  TProfile2D* fhEfficiencyMapS2S1;
  TProfile2D* fhEfficiencyMapS1GeoCut;
  TProfile2D* fhEfficiencyMapS2GeoCut;
  TProfile2D* fhEfficiencyMapS1S2GeoCut;
  TProfile2D* fhEfficiencyMapS2S1GeoCut;

  TFile* fileNoCorr = TFile::Open("EfficiencyHistos_NoDeadCorr_20180425_2000.root");

  if (NULL != fileNoCorr) {

    gDirectory->cd("Sts_Eff");

    fhEfficiencyNoDeadCorr = dynamic_cast<TProfile*>(gDirectory->FindObjectAny("fhEfficiency"));
    if (NULL == fhEfficiencyNoDeadCorr) return;

    gDirectory->cd("../Sts_GeoCuts");
    fhMapBestPairsHodoGeoCutS1_H1_NoDeadCorr =
      dynamic_cast<TH2*>(gDirectory->FindObjectAny("fhMapBestPairsHodoGeoCutS1_H1"));
    if (NULL == fhMapBestPairsHodoGeoCutS1_H1_NoDeadCorr) return;
    fhMapBestPairsHodoGeoCutS1_H2_NoDeadCorr =
      dynamic_cast<TH2*>(gDirectory->FindObjectAny("fhMapBestPairsHodoGeoCutS1_H2"));
    if (NULL == fhMapBestPairsHodoGeoCutS1_H2_NoDeadCorr) return;
    fhMapBestPairsHodoGeoCutS2_H1_NoDeadCorr =
      dynamic_cast<TH2*>(gDirectory->FindObjectAny("fhMapBestPairsHodoGeoCutS2_H1"));
    if (NULL == fhMapBestPairsHodoGeoCutS2_H1_NoDeadCorr) return;
    fhMapBestPairsHodoGeoCutS2_H2_NoDeadCorr =
      dynamic_cast<TH2*>(gDirectory->FindObjectAny("fhMapBestPairsHodoGeoCutS2_H2"));
    if (NULL == fhMapBestPairsHodoGeoCutS2_H2_NoDeadCorr) return;
  }  // if( NULL != fileNoCorr )
  else
    return;
  //   fileNoCorr->Close();
  gROOT->cd();

  std::cout << " Done reading histos from NoCorr file" << std::endl;

  TFile* fileDeadCorr = TFile::Open("EfficiencyHistos_DeadCorr_20180425_1800.root");

  if (NULL != fileDeadCorr) {
    gDirectory->cd("Sts_Cuts");

    fhDtAllPairsHodoS1 = dynamic_cast<TH1*>(gDirectory->FindObjectAny("fhDtAllPairsHodoS1"));
    if (NULL == fhDtAllPairsHodoS1) return;
    fhDtAllPairsHodoS2 = dynamic_cast<TH1*>(gDirectory->FindObjectAny("fhDtAllPairsHodoS2"));
    if (NULL == fhDtAllPairsHodoS2) return;

    fhDtBestPairsHodoS1 = dynamic_cast<TH1*>(gDirectory->FindObjectAny("fhDtBestPairsHodoS1"));
    if (NULL == fhDtBestPairsHodoS1) return;
    fhDtBestPairsHodoS2 = dynamic_cast<TH1*>(gDirectory->FindObjectAny("fhDtBestPairsHodoS2"));
    if (NULL == fhDtBestPairsHodoS2) return;

    fhMapBestPairsHodo_ProjS1 = dynamic_cast<TH2*>(gDirectory->FindObjectAny("fhMapBestPairsHodo_ProjS1"));
    if (NULL == fhMapBestPairsHodo_ProjS1) return;
    fhMapBestPairsHodo_ProjS2 = dynamic_cast<TH2*>(gDirectory->FindObjectAny("fhMapBestPairsHodo_ProjS2"));
    if (NULL == fhMapBestPairsHodo_ProjS2) return;
    fhMapBestPairsHodoS1_Proj = dynamic_cast<TH2*>(gDirectory->FindObjectAny("fhMapBestPairsHodoS1_Proj"));
    if (NULL == fhMapBestPairsHodoS1_Proj) return;
    fhMapBestPairsHodoS2_Proj = dynamic_cast<TH2*>(gDirectory->FindObjectAny("fhMapBestPairsHodoS2_Proj"));
    if (NULL == fhMapBestPairsHodoS2_Proj) return;

    fhResidualsBestPairsHodoS1 = dynamic_cast<TH2*>(gDirectory->FindObjectAny("fhResidualsBestPairsHodoS1"));
    if (NULL == fhResidualsBestPairsHodoS1) return;
    fhResidualsBestPairsHodoS2 = dynamic_cast<TH2*>(gDirectory->FindObjectAny("fhResidualsBestPairsHodoS2"));
    if (NULL == fhResidualsBestPairsHodoS2) return;

    gDirectory->cd("../Sts_GeoCuts");
    fhDtBestPairsHodoS1GeoCut = dynamic_cast<TH1*>(gDirectory->FindObjectAny("fhDtBestPairsHodoS1GeoCut"));
    if (NULL == fhDtBestPairsHodoS1GeoCut) return;
    fhDtBestPairsHodoS2GeoCut = dynamic_cast<TH1*>(gDirectory->FindObjectAny("fhDtBestPairsHodoS2GeoCut"));
    if (NULL == fhDtBestPairsHodoS2GeoCut) return;
    fhDtBestPairsHodoS1S2GeoCut = dynamic_cast<TH1*>(gDirectory->FindObjectAny("fhDtBestPairsHodoS1S2GeoCut"));
    if (NULL == fhDtBestPairsHodoS1S2GeoCut) return;
    fhDtBestPairsHodoS2S1GeoCut = dynamic_cast<TH1*>(gDirectory->FindObjectAny("fhDtBestPairsHodoS2S1GeoCut"));
    if (NULL == fhDtBestPairsHodoS2S1GeoCut) return;

    fhMapBestPairsHodoS1GeoCut_Proj = dynamic_cast<TH2*>(gDirectory->FindObjectAny("fhMapBestPairsHodoS1GeoCut_Proj"));
    if (NULL == fhMapBestPairsHodoS1GeoCut_Proj) return;
    fhMapBestPairsHodoS2GeoCut_Proj = dynamic_cast<TH2*>(gDirectory->FindObjectAny("fhMapBestPairsHodoS2GeoCut_Proj"));
    if (NULL == fhMapBestPairsHodoS2GeoCut_Proj) return;

    fhMapBestPairsHodoGeoCutS1_H1 = dynamic_cast<TH2*>(gDirectory->FindObjectAny("fhMapBestPairsHodoGeoCutS1_H1"));
    if (NULL == fhMapBestPairsHodoGeoCutS1_H1) return;
    fhMapBestPairsHodoGeoCutS1_H2 = dynamic_cast<TH2*>(gDirectory->FindObjectAny("fhMapBestPairsHodoGeoCutS1_H2"));
    if (NULL == fhMapBestPairsHodoGeoCutS1_H2) return;
    fhMapBestPairsHodoGeoCutS2_H1 = dynamic_cast<TH2*>(gDirectory->FindObjectAny("fhMapBestPairsHodoGeoCutS2_H1"));
    if (NULL == fhMapBestPairsHodoGeoCutS2_H1) return;
    fhMapBestPairsHodoGeoCutS2_H2 = dynamic_cast<TH2*>(gDirectory->FindObjectAny("fhMapBestPairsHodoGeoCutS2_H2"));
    if (NULL == fhMapBestPairsHodoGeoCutS2_H2) return;

    fhResidualsBestPairsHodoS1GeoCut =
      dynamic_cast<TH2*>(gDirectory->FindObjectAny("fhResidualsBestPairsHodoS1GeoCut"));
    if (NULL == fhResidualsBestPairsHodoS1GeoCut) return;
    fhResidualsBestPairsHodoS2GeoCut =
      dynamic_cast<TH2*>(gDirectory->FindObjectAny("fhResidualsBestPairsHodoS2GeoCut"));
    if (NULL == fhResidualsBestPairsHodoS2GeoCut) return;

    gDirectory->cd("../Sts_Eff");

    fhEfficiencyCorr = dynamic_cast<TProfile*>(gDirectory->FindObjectAny("fhEfficiency"));
    if (NULL == fhEfficiencyCorr) return;

    fhEfficiencyMapS1 = dynamic_cast<TProfile2D*>(gDirectory->FindObjectAny("fhEfficiencyMapS1"));
    if (NULL == fhEfficiencyMapS1) return;
    fhEfficiencyMapS2 = dynamic_cast<TProfile2D*>(gDirectory->FindObjectAny("fhEfficiencyMapS2"));
    if (NULL == fhEfficiencyMapS2) return;
    fhEfficiencyMapS1S2 = dynamic_cast<TProfile2D*>(gDirectory->FindObjectAny("fhEfficiencyMapS1S2"));
    if (NULL == fhEfficiencyMapS1S2) return;
    fhEfficiencyMapS2S1 = dynamic_cast<TProfile2D*>(gDirectory->FindObjectAny("fhEfficiencyMapS2S1"));
    if (NULL == fhEfficiencyMapS2S1) return;
    fhEfficiencyMapS1GeoCut = dynamic_cast<TProfile2D*>(gDirectory->FindObjectAny("fhEfficiencyMapS1GeoCut"));
    if (NULL == fhEfficiencyMapS1GeoCut) return;
    fhEfficiencyMapS2GeoCut = dynamic_cast<TProfile2D*>(gDirectory->FindObjectAny("fhEfficiencyMapS2GeoCut"));
    if (NULL == fhEfficiencyMapS2GeoCut) return;
    fhEfficiencyMapS1S2GeoCut = dynamic_cast<TProfile2D*>(gDirectory->FindObjectAny("fhEfficiencyMapS1S2GeoCut"));
    if (NULL == fhEfficiencyMapS1S2GeoCut) return;
    fhEfficiencyMapS2S1GeoCut = dynamic_cast<TProfile2D*>(gDirectory->FindObjectAny("fhEfficiencyMapS2S1GeoCut"));
    if (NULL == fhEfficiencyMapS2S1GeoCut) return;

  }  // if( NULL != fileDeadCorr )
  else
    return;
  //   fileDeadCorr->Close();
  gROOT->cd();

  std::cout << " Done reading histos from Corr file" << std::endl;

  //====================================================================//
  TCanvas* fcCanvasDt = new TCanvas("cDt", "Time difference between STS and Hodo");
  fcCanvasDt->Divide(2);

  // S1
  fcCanvasDt->cd(1);
  THStack* stackDtS1 = new THStack("hsDtS1", "S1-Hodo dt, raw and with selections");

  TLegend* legDtS1 = new TLegend(0.55, 0.65, 0.9, 0.9);
  legDtS1->SetHeader("S1-Hodo time differences");

  fhDtAllPairsHodoS1->SetLineColor(kBlue);
  fhDtAllPairsHodoS1->SetLineWidth(2);
  stackDtS1->Add(fhDtAllPairsHodoS1);
  legDtS1->AddEntry(fhDtAllPairsHodoS1, "All", "l");

  fhDtBestPairsHodoS1->SetLineColor(kRed);
  fhDtBestPairsHodoS1->SetLineWidth(2);
  stackDtS1->Add(fhDtBestPairsHodoS1);
  legDtS1->AddEntry(fhDtBestPairsHodoS1, "W/ time cut", "l");

  fhDtBestPairsHodoS1GeoCut->SetLineColor(kGreen);
  fhDtBestPairsHodoS1GeoCut->SetLineWidth(2);
  stackDtS1->Add(fhDtBestPairsHodoS1GeoCut);
  legDtS1->AddEntry(fhDtBestPairsHodoS1GeoCut, "W/ time and geocut", "l");

  fhDtBestPairsHodoS1S2GeoCut->SetLineColor(kMagenta);
  fhDtBestPairsHodoS1S2GeoCut->SetLineWidth(2);
  stackDtS1->Add(fhDtBestPairsHodoS1S2GeoCut);
  legDtS1->AddEntry(fhDtBestPairsHodoS1S2GeoCut, "W/ cuts and hit in other STS ", "l");

  stackDtS1->Draw("nostack hist");
  stackDtS1->GetXaxis()->SetRangeUser(-1000., 1000.);
  gPad->SetLogy();
  gPad->SetGridx();
  gPad->SetGridy();
  legDtS1->Draw();

  // S1
  fcCanvasDt->cd(2);
  THStack* stackDtS2 = new THStack("hsDtS2", "S2-Hodo dt, raw and with selections");

  TLegend* legDtS2 = new TLegend(0.55, 0.65, 0.9, 0.9);
  legDtS2->SetHeader("S2-Hodo time differences");

  fhDtAllPairsHodoS2->SetLineColor(kBlue);
  fhDtAllPairsHodoS2->SetLineWidth(2);
  stackDtS2->Add(fhDtAllPairsHodoS2);
  legDtS2->AddEntry(fhDtAllPairsHodoS2, "All", "l");

  fhDtBestPairsHodoS2->SetLineColor(kRed);
  fhDtBestPairsHodoS2->SetLineWidth(2);
  stackDtS2->Add(fhDtBestPairsHodoS2);
  legDtS2->AddEntry(fhDtBestPairsHodoS2, "W/ time cut", "l");

  fhDtBestPairsHodoS2GeoCut->SetLineColor(kGreen);
  fhDtBestPairsHodoS2GeoCut->SetLineWidth(2);
  stackDtS2->Add(fhDtBestPairsHodoS2GeoCut);
  legDtS2->AddEntry(fhDtBestPairsHodoS2GeoCut, "W/ time and geocut", "l");

  fhDtBestPairsHodoS2S1GeoCut->SetLineColor(kMagenta);
  fhDtBestPairsHodoS2S1GeoCut->SetLineWidth(2);
  stackDtS2->Add(fhDtBestPairsHodoS2S1GeoCut);
  legDtS2->AddEntry(fhDtBestPairsHodoS2S1GeoCut, "W/ cuts and hit in other STS ", "l");

  stackDtS2->Draw("nostack hist");
  stackDtS2->GetXaxis()->SetRangeUser(-1000., 1000.);
  gPad->SetLogy();
  gPad->SetGridx();
  gPad->SetGridy();
  legDtS2->Draw();
  //====================================================================//

  std::cout << " Done with dt canvas" << std::endl;

  //====================================================================//
  TCanvas* fcCanvasResidualsMaps = new TCanvas("cResiduals", "Residuals between Hodo projection and STS hit");
  fcCanvasResidualsMaps->Divide(2, 2);

  // S1
  fcCanvasResidualsMaps->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhResidualsBestPairsHodoS1->Draw("colz");
  fhResidualsBestPairsHodoS1->GetXaxis()->SetRangeUser(-40., 40.);
  fhResidualsBestPairsHodoS1->GetYaxis()->SetRangeUser(-40., 40.);

  fcCanvasResidualsMaps->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhResidualsBestPairsHodoS1GeoCut->Draw("colz");
  fhResidualsBestPairsHodoS1GeoCut->GetXaxis()->SetRangeUser(-40., 40.);
  fhResidualsBestPairsHodoS1GeoCut->GetYaxis()->SetRangeUser(-40., 40.);

  // S2
  fcCanvasResidualsMaps->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhResidualsBestPairsHodoS2->Draw("colz");
  fhResidualsBestPairsHodoS2->GetXaxis()->SetRangeUser(-40., 40.);
  fhResidualsBestPairsHodoS2->GetYaxis()->SetRangeUser(-40., 40.);

  fcCanvasResidualsMaps->cd(4);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhResidualsBestPairsHodoS2GeoCut->Draw("colz");
  fhResidualsBestPairsHodoS2GeoCut->GetXaxis()->SetRangeUser(-40., 40.);
  fhResidualsBestPairsHodoS2GeoCut->GetYaxis()->SetRangeUser(-40., 40.);
  //====================================================================//

  std::cout << " Done with projections canvas" << std::endl;

  //====================================================================//
  TCanvas* fcCanvasProjectionMaps = new TCanvas("cProjMap", "Hodo pairs projection in STS sensor plane maps");
  fcCanvasProjectionMaps->Divide(3, 2);

  // S1
  fcCanvasProjectionMaps->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhMapBestPairsHodo_ProjS1->Draw("colz");

  fcCanvasProjectionMaps->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhMapBestPairsHodoS1_Proj->Draw("colz");

  fcCanvasProjectionMaps->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhMapBestPairsHodoS1GeoCut_Proj->Draw("colz");

  // S2
  fcCanvasProjectionMaps->cd(4);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhMapBestPairsHodo_ProjS2->Draw("colz");

  fcCanvasProjectionMaps->cd(5);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhMapBestPairsHodoS2_Proj->Draw("colz");

  fcCanvasProjectionMaps->cd(6);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhMapBestPairsHodoS2GeoCut_Proj->Draw("colz");
  //====================================================================//

  std::cout << " Done with projections canvas" << std::endl;

  //====================================================================//
  TCanvas* fcCanvasEfficiencyMaps = new TCanvas("cEffMap", "Sts Efficiency maps");
  fcCanvasEfficiencyMaps->Divide(4, 2);

  // S1
  fcCanvasEfficiencyMaps->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  fhEfficiencyMapS1->Draw("colz");

  fcCanvasEfficiencyMaps->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  fhEfficiencyMapS1GeoCut->Draw("colz");

  fcCanvasEfficiencyMaps->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  fhEfficiencyMapS1S2->Draw("colz");

  fcCanvasEfficiencyMaps->cd(4);
  gPad->SetGridx();
  gPad->SetGridy();
  fhEfficiencyMapS1S2GeoCut->Draw("colz");

  // S2
  fcCanvasEfficiencyMaps->cd(5);
  gPad->SetGridx();
  gPad->SetGridy();
  fhEfficiencyMapS2->Draw("colz");

  fcCanvasEfficiencyMaps->cd(6);
  gPad->SetGridx();
  gPad->SetGridy();
  fhEfficiencyMapS2GeoCut->Draw("colz");

  fcCanvasEfficiencyMaps->cd(7);
  gPad->SetGridx();
  gPad->SetGridy();
  fhEfficiencyMapS2S1->Draw("colz");

  fcCanvasEfficiencyMaps->cd(8);
  gPad->SetGridx();
  gPad->SetGridy();
  fhEfficiencyMapS2S1GeoCut->Draw("colz");
  //====================================================================//

  std::cout << " Done with efficiency map canvas" << std::endl;

  //====================================================================//
  TCanvas* fcCanvasEff = new TCanvas("cEff", "Efficiency of STS VS Hodo");

  fcCanvasEff->cd(1);
  THStack* stackEff = new THStack("hsEff", "STS/Hodo efficiency, w/o and w/ dead strip correction in GeoCut");

  TLegend* legEff = new TLegend(0.55, 0.25, 0.9, 0.6);
  legEff->SetHeader("Efficiencies");

  fhEfficiencyNoDeadCorr->SetLineColor(kBlue);
  fhEfficiencyNoDeadCorr->SetLineWidth(2);
  stackEff->Add(fhEfficiencyNoDeadCorr);
  legEff->AddEntry(fhEfficiencyNoDeadCorr, "No corrections", "l");

  fhEfficiencyCorr->SetLineColor(kRed);
  fhEfficiencyCorr->SetLineWidth(2);
  stackEff->Add(fhEfficiencyCorr);
  legEff->AddEntry(fhEfficiencyCorr, "With Dead strips correction", "l");

  stackEff->Draw("nostack hist");
  gPad->SetGridx();
  gPad->SetGridy();
  legEff->Draw();
  //====================================================================//

  std::cout << " Done with efficiency canvas" << std::endl;
  std::cout << " S1 efficiency without dead strips correction: " << fhEfficiencyNoDeadCorr->GetBinContent(5)
            << std::endl;
  std::cout << " S2 efficiency without dead strips correction: " << fhEfficiencyNoDeadCorr->GetBinContent(6)
            << std::endl;

  //====================================================================//
  TCanvas* fcCanvasDeadCorr = new TCanvas("cDearCorr", "Hodo pairs within sensors w/ and w/o Dead strip rejection");
  fcCanvasDeadCorr->Divide(4, 2);

  // No Dead Correction
  fcCanvasDeadCorr->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhMapBestPairsHodoGeoCutS1_H1_NoDeadCorr->Draw("colz");

  fcCanvasDeadCorr->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhMapBestPairsHodoGeoCutS1_H2_NoDeadCorr->Draw("colz");

  fcCanvasDeadCorr->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhMapBestPairsHodoGeoCutS2_H1_NoDeadCorr->Draw("colz");

  fcCanvasDeadCorr->cd(4);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhMapBestPairsHodoGeoCutS2_H2_NoDeadCorr->Draw("colz");

  // Dead strips out
  fcCanvasDeadCorr->cd(5);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhMapBestPairsHodoGeoCutS1_H1->Draw("colz");

  fcCanvasDeadCorr->cd(6);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhMapBestPairsHodoGeoCutS1_H2->Draw("colz");

  fcCanvasDeadCorr->cd(7);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhMapBestPairsHodoGeoCutS2_H1->Draw("colz");

  fcCanvasDeadCorr->cd(8);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhMapBestPairsHodoGeoCutS2_H2->Draw("colz");
  //====================================================================//

  std::cout << " Done with projections canvas" << std::endl;

  Double_t dNoDeadEntriesS1 = fhMapBestPairsHodoGeoCutS1_H1->GetEntries();
  Double_t dNoCorrEntriesS1 = fhMapBestPairsHodoGeoCutS1_H1_NoDeadCorr->GetEntries();
  Double_t dEffRatioS1      = dNoDeadEntriesS1 / dNoCorrEntriesS1;
  Double_t dEffCorrS1       = 1.0 - dEffRatioS1;
  std::cout << " Dead strips efficiency ratio for S1 active area: " << dNoDeadEntriesS1 << " / " << dNoCorrEntriesS1
            << " = " << dEffRatioS1 << " => Correction of " << dEffCorrS1 << std::endl;

  Double_t dNoDeadEntriesS2 = fhMapBestPairsHodoGeoCutS2_H1->GetEntries();
  Double_t dNoCorrEntriesS2 = fhMapBestPairsHodoGeoCutS2_H1_NoDeadCorr->GetEntries();
  Double_t dEffRatioS2      = dNoDeadEntriesS2 / dNoCorrEntriesS2;
  Double_t dEffCorrS2       = 1.0 - dEffRatioS2;
  std::cout << " Dead strips efficiency ratio for S2 active area: " << dNoDeadEntriesS2 << " / " << dNoCorrEntriesS2
            << " = " << dEffRatioS2 << " => Correction of " << dEffCorrS2 << std::endl;

  std::cout << " S1 efficiency with dead strips correction: " << fhEfficiencyNoDeadCorr->GetBinContent(5) + dEffCorrS1
            << std::endl;
  std::cout << " S2 efficiency with dead strips correction: " << fhEfficiencyNoDeadCorr->GetBinContent(6) + dEffCorrS2
            << std::endl;
}
