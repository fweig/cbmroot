/** @file T0CoincPlotSingleRun.C
  * @copyright Copyright (C) 2019 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pierre-Alain Loizeau [committer] **/

Bool_t T0CoincPlotSingleRun(UInt_t uRunId)
{
  /// Obtaining the plots
  TH1* tempH1 = NULL;
  TH1* phStsT0;
  TH1* phMuchT0;
  TH1* phTofT0;
  TH1* phRichT0;

  /// Open File
  TFile* pFile = TFile::Open(Form("HistosTimeCheck_%03u.root", uRunId));
  gROOT->cd();

  if (nullptr == pFile) return kFALSE;

  phStsT0 = new TH1D();
  tempH1  = (TH1*) (pFile->FindObjectAny("fT0StsDiff"));
  if (NULL != tempH1) {
    tempH1->Copy(*(phStsT0));  ///?
  }                            //  if( NULL != tempH1 )
  else
    return kFALSE;

  phMuchT0 = new TH1D();
  tempH1   = (TH1*) (pFile->FindObjectAny("fT0MuchDiff"));
  if (NULL != tempH1) {
    tempH1->Copy(*(phMuchT0));  ///?
  }                             //  if( NULL != tempH1 )
  else
    return kFALSE;

  phTofT0 = new TH1D();
  tempH1  = (TH1*) (pFile->FindObjectAny("fT0TofDiff"));
  if (NULL != tempH1) {
    tempH1->Copy(*(phTofT0));  ///?
  }                            //  if( NULL != tempH1 )
  else
    return kFALSE;

  phRichT0 = new TH1D();
  tempH1   = (TH1*) (pFile->FindObjectAny("fT0RichDiff"));
  if (NULL != tempH1) {
    tempH1->Copy(*(phRichT0));  ///?
  }                             //  if( NULL != tempH1 )
  else
    return kFALSE;

  /// Plotting: THStacks with all available detectors VS T0
  TCanvas* cT0Coinc = new TCanvas(Form("cT0Coinc_%03u", uRunId), Form("Time Coincidence with T0 in run %3u", uRunId));

  THStack* pStacksT0Coinc = new THStack(
    Form("stackT0Coinc_%02u", uRunId), Form("Time Coincidence with T0 in run %02u; tT0 - Tdet [ns]; Pairs []", uRunId));
  TLegend* legend = new TLegend(0.1, 0.7, 0.3, 0.9);

  phStsT0->SetLineColor(kBlack);
  phMuchT0->SetLineColor(kRed);
  phTofT0->SetLineColor(kBlue);
  phRichT0->SetLineColor(kViolet);

  phStsT0->SetLineWidth(2);
  phMuchT0->SetLineWidth(2);
  phTofT0->SetLineWidth(2);
  phRichT0->SetLineWidth(2);

  pStacksT0Coinc->Add(phStsT0);
  pStacksT0Coinc->Add(phMuchT0);
  pStacksT0Coinc->Add(phTofT0);
  pStacksT0Coinc->Add(phRichT0);

  legend->AddEntry(phStsT0, "STS", "l");
  legend->AddEntry(phMuchT0, "MUCH", "l");
  legend->AddEntry(phTofT0, "TOF", "l");
  legend->AddEntry(phRichT0, "RICH", "l");

  cT0Coinc->cd();
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  pStacksT0Coinc->Draw("nostack,hist");
  legend->Draw();
  pStacksT0Coinc->GetXaxis()->SetRangeUser(-2000.0, 2000.0);

  /// Plotting: rescaled to baseline plots with all available detectors VS T0
  /// => TODO: fit baseline out of sync peak
  Double_t dBaselineSts;
  Double_t dBaselineMuch;
  Double_t dBaselineTof;
  Double_t dBaselineRich;

  /// TEMP solution: read bin at -5000 ns
  dBaselineSts  = phStsT0->GetBinContent(phStsT0->FindBin(-5000));
  dBaselineMuch = phMuchT0->GetBinContent(phMuchT0->FindBin(-5000));
  dBaselineTof  = phTofT0->GetBinContent(phTofT0->FindBin(-5000));
  dBaselineRich = phRichT0->GetBinContent(phRichT0->FindBin(-5000));

  TH1* phRescaledStsT0  = static_cast<TH1*>(phStsT0->Clone("hRescaledStsT0"));
  TH1* phRescaledMuchT0 = static_cast<TH1*>(phMuchT0->Clone("hRescaledMuchT0"));
  TH1* phRescaledTofT0  = static_cast<TH1*>(phTofT0->Clone("hRescaledTofT0"));
  TH1* phRescaledRichT0 = static_cast<TH1*>(phRichT0->Clone("hRescaledRichT0"));

  phRescaledStsT0->Scale(1 / dBaselineSts);
  phRescaledMuchT0->Scale(1 / dBaselineMuch);
  phRescaledTofT0->Scale(1 / dBaselineTof);
  phRescaledRichT0->Scale(1 / dBaselineRich);

  TCanvas* cT0CoincRescale =
    new TCanvas(Form("cT0CoincRescale_%03u", uRunId), Form("Time Coincidence with T0 in run %3u", uRunId));

  THStack* pStacksT0CoincRescale =
    new THStack(Form("stackT0CoincRescale_%02u", uRunId), Form("Time Coincidence with T0 in run %02u; tT0 - Tdet [ns]; "
                                                               "Pairs nb rescaled [1/baseline]",
                                                               uRunId));
  TLegend* legendRescale = new TLegend(0.1, 0.7, 0.3, 0.9);

  phRescaledStsT0->SetLineColor(kBlack);
  phRescaledMuchT0->SetLineColor(kRed);
  phRescaledTofT0->SetLineColor(kBlue);
  phRescaledRichT0->SetLineColor(kViolet);

  phRescaledStsT0->SetLineWidth(2);
  phRescaledMuchT0->SetLineWidth(2);
  phRescaledTofT0->SetLineWidth(2);
  phRescaledRichT0->SetLineWidth(2);

  pStacksT0CoincRescale->Add(phRescaledStsT0);
  pStacksT0CoincRescale->Add(phRescaledMuchT0);
  pStacksT0CoincRescale->Add(phRescaledTofT0);
  pStacksT0CoincRescale->Add(phRescaledRichT0);

  legendRescale->AddEntry(phRescaledStsT0, "STS", "l");
  legendRescale->AddEntry(phRescaledMuchT0, "MUCH", "l");
  legendRescale->AddEntry(phRescaledTofT0, "TOF", "l");
  legendRescale->AddEntry(phRescaledRichT0, "RICH", "l");

  cT0CoincRescale->cd();
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  pStacksT0CoincRescale->Draw("nostack,hist");
  legendRescale->Draw();
  pStacksT0CoincRescale->GetXaxis()->SetRangeUser(-2000.0, 2000.0);

  return kTRUE;
}
