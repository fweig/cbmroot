/* Copyright (C) 2022 Fair GmbH, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

#include "CbmMuchUnpackMonitor.h"

#include "MicrosliceDescriptor.hpp"

#include <FairRun.h>
#include <FairRunOnline.h>
#include <Logger.h>

#include "TCanvas.h"
#include <RtypesCore.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <THttpServer.h>
#include <TProfile.h>
#include <TROOT.h>

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <cmath>

CbmMuchUnpackMonitor::CbmMuchUnpackMonitor(/* args */) : vNbMessType(7, 0), fvpAllHistoPointers()
{
  // Miscroslice component properties histos
  for (UInt_t component = 0; component < kiMaxNbFlibLinks; component++) {
    fvhMsSize[component]     = nullptr;
    fvhMsSizeTime[component] = nullptr;
  }
}

CbmMuchUnpackMonitor::~CbmMuchUnpackMonitor()
{
  for (auto iter = fvpAllHistoPointers.begin(); iter != fvpAllHistoPointers.end();) {
    if (iter->first != nullptr) { delete iter->first; }
    iter = fvpAllHistoPointers.erase(iter);
  }
  for (auto iter = fvpAllCanvasPointers.begin(); iter != fvpAllCanvasPointers.end();) {
    if (iter->first != nullptr) { delete iter->first; }
    iter = fvpAllCanvasPointers.erase(iter);
  }
}

Bool_t CbmMuchUnpackMonitor::CreateHistograms(CbmMuchUnpackPar* pUnpackPar)
{
  TString sHistName {""};
  TString title {""};
  const UInt_t uNbAsics       = pUnpackPar->GetNrOfAsics();
  const UInt_t uNbFebs        = pUnpackPar->GetNrOfFebs();
  const UInt_t uNbAsicsPerFeb = pUnpackPar->GetNbAsicsPerFeb();
  const UInt_t uNbChanPerFeb  = pUnpackPar->GetNbChanPerFeb();

  /// Initialize the per timeslice counters
  fvuNbRawTsFeb.resize(uNbFebs);
  fvvuNbRawTsChan.resize(uNbFebs);
  fvuNbDigisTsFeb.resize(uNbFebs);
  fvvuNbDigisTsChan.resize(uNbFebs);
  for (uint32_t uFebIdx = 0; uFebIdx < uNbFebs; ++uFebIdx) {
    fvvuNbRawTsChan[uFebIdx].resize(uNbChanPerFeb, 0);
    fvvuNbDigisTsChan[uFebIdx].resize(uNbChanPerFeb, 0);
  }

  /// Create general unpacking histograms
  fhDigisTimeInRun = new TH1I("hMuchDigisTimeInRun", "Digis Nb vs Time in Run; Time in run [s]; Digis Nb []", 10, 0, 1);
  fhDigisTimeInRun->SetCanExtend(TH1::kAllAxes);
  AddHistoToVector(fhDigisTimeInRun, "");

  fhVectorSize = new TH1I("fhMuchVectorSize", "Size of the vector VS TS index; TS index; Size [bytes]", 10, 0, 10);
  fhVectorSize->SetCanExtend(TH1::kAllAxes);
  AddHistoToVector(fhVectorSize, "");

  fhVectorCapacity =
    new TH1I("fhMuchVectorCapacity", "Size of the vector VS TS index; TS index; Size [bytes]", 10000, 0., 10000.);
  AddHistoToVector(fhVectorCapacity, "");

  fhMsCntEvo = new TH1I("fhMuchMsCntEvo", "; MS index [s]; Counts []", 600, 0.0, 600.0);
  AddHistoToVector(fhMsCntEvo, "");

  fhMsErrorsEvo =
    new TH2I("fhMuchMsErrorsEvo", "; MS index [s]; Error type []; Counts []", 600, 0.0, 600.0, 4, -0.5, 3.5);
  AddHistoToVector(fhMsErrorsEvo, "");

  /// Hit rates evo per FEB in system
  sHistName               = "hMuchAllFebsHitRateEvo";
  title                   = "Hits per second & FEB; Time [s]; FEB []; Hits []";
  fhMuchAllFebsHitRateEvo = new TH2I(sHistName, title, 600, 0, 600, uNbFebs, -0.5, uNbFebs - 0.5);
  // fhMuchAllFebsHitRateEvo->SetCanExtend(TH1::kAllAxes);
  AddHistoToVector(fhMuchAllFebsHitRateEvo, "");

  /// Hit rates evo per ASIC in system
  sHistName                = "hMuchAllAsicsHitRateEvo";
  title                    = "Hits per second & ASIC; Time [s]; ASIC []; Hits []";
  fhMuchAllAsicsHitRateEvo = new TH2I(sHistName, title, 600, 0, 600, uNbAsics, -0.5, uNbAsics - 0.5);
  // fhMuchAllAsicsHitRateEvo->SetCanExtend(TH1::kAllAxes);
  AddHistoToVector(fhMuchAllAsicsHitRateEvo, "");

  /// Hit counts map in system
  sHistName = "hMuchFebAsicHitCounts";
  title     = "Hits per FEB & ASIC; FEB []; ASIC in FEB[]; Hits []";
  fhMuchFebAsicHitCounts =
    new TH2I(sHistName, title, uNbFebs, -0.5, uNbFebs - 0.5, uNbAsicsPerFeb, -0.5, uNbAsicsPerFeb - 0.5);
  AddHistoToVector(fhMuchFebAsicHitCounts, "");

  sHistName            = "hMuchStatusMessType";
  title                = "Nb of status message of each type for each DPB; ASIC; Status Type";
  fhMuchStatusMessType = new TH2I(sHistName, title, uNbAsics, 0, uNbAsics, 16, 0., 16.);
  AddHistoToVector(fhMuchStatusMessType, "");

  /// Timeslice counter ratio Plots
  sHistName           = "hMuchRawHitRatioPerFeb";
  title               = "Proportion of digis over raw hits in each FEB; FEB []; digis/raw [Prct]";
  fhRawHitRatioPerFeb = new TProfile(sHistName, title, uNbFebs, -0.5, uNbFebs - 0.5);
  AddHistoToVector(fhRawHitRatioPerFeb, "");

  ///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
  UInt_t uAlignedLimit = fuLongHistoNbSeconds - (fuLongHistoNbSeconds % fuLongHistoBinSizeSec);
  fuLongHistoBinNb     = uAlignedLimit / fuLongHistoBinSizeSec;

  ///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
  /// FEB-8 plots
  /// All histos per FEB: with channels or ASIC as axis!!
  for (UInt_t uFebIdx = 0; uFebIdx < uNbFebs; ++uFebIdx) {
    /// Timeslice counter ratio Plots
    sHistName = Form("hMuchRawChRatio_%03d", uFebIdx);
    title = Form("Proportion of raw hits in each channel of FEB %2d; Channel []; Share of FEB raw msg [Prct]", uFebIdx);
    fvhRawChRatio.push_back(new TProfile(sHistName, title, uNbChanPerFeb, -0.5, uNbChanPerFeb - 0.5));
    sHistName = Form("hMuchHitChRatio_%03d", uFebIdx);
    title     = Form("Proportion of digis in each channel of FEB %2d; Channel []; Share of FEB digis [Prct]", uFebIdx);
    fvhHitChRatio.push_back(new TProfile(sHistName, title, uNbChanPerFeb, -0.5, uNbChanPerFeb - 0.5));
    sHistName = Form("hMuchDupliChRatio_%03d", uFebIdx);
    title =
      Form("Proportion of duplicates in each channel of FEB %2d; Channel []; Share of FEB duplicates [Prct]", uFebIdx);
    fvhDupliChRatio.push_back(new TProfile(sHistName, title, uNbChanPerFeb, -0.5, uNbChanPerFeb - 0.5));
    sHistName = Form("hMuchRawHitRatioPerCh_%03d", uFebIdx);
    title = Form("Proportion of digis over raw hits in each channel of FEB %2d; Channel []; digis/raw [Prct]", uFebIdx);
    fvhRawHitRatioPerCh.push_back(new TProfile(sHistName, title, uNbChanPerFeb, -0.5, uNbChanPerFeb - 0.5));
    sHistName = Form("hMuchRawDupliRatioPerCh_%03d", uFebIdx);
    title =
      Form("Proportion of duplicates over raw hits in each channel of FEB %2d; Channel []; dupli/raw [Prct]", uFebIdx);
    fvhRawDupliRatioPerCh.push_back(new TProfile(sHistName, title, uNbChanPerFeb, -0.5, uNbChanPerFeb - 0.5));

    /// Channel counts
    sHistName = Form("hMuchFebChanCntRaw_%03u", uFebIdx);
    title     = Form("Hits Count per channel, FEB #%03u; Channel; Hits []", uFebIdx);
    fvhMuchFebChanCntRaw.push_back(new TH1I(sHistName, title, uNbChanPerFeb, -0.5, uNbChanPerFeb - 0.5));

    /// Raw Adc Distribution
    sHistName = Form("hMuchFebChanAdcRaw_%03u", uFebIdx);
    title     = Form("Raw Adc distribution per channel, FEB #%03u; Channel []; Adc "
                 "[]; Hits []",
                 uFebIdx);
    fvhMuchFebChanAdcRaw.push_back(new TH2I(sHistName, title, uNbChanPerFeb, -0.5, uNbChanPerFeb - 0.5,
                                            stsxyter::kuHitNbAdcBins, -0.5, stsxyter::kuHitNbAdcBins - 0.5));

    /// Raw Adc Distribution profile
    sHistName = Form("hMuchFebChanAdcRawProfc_%03u", uFebIdx);
    title     = Form("Raw Adc prodile per channel, FEB #%03u; Channel []; Adc []", uFebIdx);
    fvhMuchFebChanAdcRawProf.push_back(new TProfile(sHistName, title, uNbChanPerFeb, -0.5, uNbChanPerFeb - 0.5));

    /// Cal Adc Distribution
    sHistName = Form("hMuchFebChanAdcCal_%03u", uFebIdx);
    title     = Form("Cal. Adc distribution per channel, FEB #%03u; Channel []; Adc [e-]; Hits []", uFebIdx);
    fvhMuchFebChanAdcCal.push_back(
      new TH2I(sHistName, title, uNbChanPerFeb, -0.5, uNbChanPerFeb - 0.5, 50, 0., 100000.));

    /// Cal Adc Distribution profile
    sHistName = Form("hMuchFebChanAdcCalProfc_%03u", uFebIdx);
    title     = Form("Cal. Adc prodile per channel, FEB #%03u; Channel []; Adc [e-]", uFebIdx);
    fvhMuchFebChanAdcCalProf.push_back(new TProfile(sHistName, title, uNbChanPerFeb, -0.5, uNbChanPerFeb - 0.5));

    /// Raw Ts Distribution
    sHistName = Form("hMuchFebChanRawTs_%03u", uFebIdx);
    title     = Form("Raw Timestamp distribution per channel, FEB #%03u; Channel "
                 "[]; Ts []; Hits []",
                 uFebIdx);
    fvhMuchFebChanRawTs.push_back(new TH2I(sHistName, title, uNbChanPerFeb, -0.5, uNbChanPerFeb - 0.5,
                                           stsxyter::kuHitNbTsBins, -0.5, stsxyter::kuHitNbTsBins - 0.5));

    /// Missed event flag
    sHistName = Form("hMuchFebChanMissEvt_%03u", uFebIdx);
    title     = Form("Missed Event flags per channel, FEB #%03u; Channel []; Miss "
                 "Evt []; Hits []",
                 uFebIdx);
    fvhMuchFebChanMissEvt.push_back(new TH2I(sHistName, title, uNbChanPerFeb, -0.5, uNbChanPerFeb - 0.5, 2, -0.5, 1.5));

    /// Missed event flag counts evolution
    sHistName = Form("hMuchFebChanMissEvtEvo_%03u", uFebIdx);
    title     = Form("Missed Evt flags per second & channel in FEB #%03u; Time "
                 "[s]; Channel []; Missed Evt flags []",
                 uFebIdx);
    fvhMuchFebChanMissEvtEvo.push_back(
      new TH2I(sHistName, title, 600, 0, 600, uNbChanPerFeb, -0.5, uNbChanPerFeb - 0.5));
    // fvhMuchFebChanMissEvtEvo.back()->SetCanExtend(TH1::kAllAxes);

    /// Missed event flag counts evolution
    sHistName = Form("hMuchFebAsicMissEvtEvo_%03u", uFebIdx);
    title     = Form("Missed Evt flags per second & StsXyter in FEB #%03u; Time "
                 "[s]; Asic []; Missed Evt flags []",
                 uFebIdx);
    fvhMuchFebAsicMissEvtEvo.push_back(
      new TH2I(sHistName, title, 600, 0, 600, uNbAsicsPerFeb, -0.5, uNbAsicsPerFeb - 0.5));
    // fvhMuchFebAsicMissEvtEvo.back()->SetCanExtend(TH1::kAllAxes);

    /// Missed event flag counts evolution
    sHistName = Form("hMuchFebMissEvtEvo_%03u", uFebIdx);
    title     = Form("Missed Evt flags per second & channel in FEB #%03u; Time "
                 "[s]; Missed Evt flags []",
                 uFebIdx);
    fvhMuchFebMissEvtEvo.push_back(new TH1I(sHistName, title, 600, 0, 600));
    // fvhMuchFebMissEvtEvo.back()->SetCanExtend(TH1::kAllAxes);

    /// Hit rates evo per channel
    sHistName = Form("hMuchFebChanRateEvo_%03u", uFebIdx);
    title     = Form("Hits per second & channel in FEB #%03u; Time [s]; Channel []; Hits []", uFebIdx);
    fvhMuchFebChanHitRateEvo.push_back(
      new TH2I(sHistName, title, 600, 0, 600, uNbChanPerFeb, -0.5, uNbChanPerFeb - 0.5));
    // fvhMuchFebChanHitRateEvo.back()->SetCanExtend(TH1::kAllAxes);

    /// Hit rates evo per MuchXyter
    sHistName = Form("hMuchFebAsicRateEvo_%03u", uFebIdx);
    title     = Form("Hits per second & StsXyter in FEB #%03u; Time [s]; Asic []; Hits []", uFebIdx);
    fvhMuchFebAsicHitRateEvo.push_back(
      new TH2I(sHistName, title, 600, 0, 600, uNbAsicsPerFeb, -0.5, uNbAsicsPerFeb - 0.5));
    // fvhMuchFebAsicHitRateEvo.back()->SetCanExtend(TH1::kAllAxes);

    /// Hit rates evo per FEB
    sHistName = Form("hMuchFebRateEvo_%03u", uFebIdx);
    title     = Form("Hits per second in FEB #%03u; Time [s]; Hits []", uFebIdx);
    fvhMuchFebHitRateEvo.push_back(new TH1I(sHistName, title, 600, 0, 600));
    // fvhMuchFebHitRateEvo.back()->SetCanExtend(TH1::kAllAxes);

    /// Hit rates evo per channel, 1 minute bins, 24h
    sHistName = Form("hMuchFebChanRateEvoLong_%03u", uFebIdx);
    title     = Form("Hits per second & channel in FEB #%03u; Time [min]; Channel []; Hits []", uFebIdx);
    fvhMuchFebChanHitRateEvoLong.push_back(new TH2D(sHistName, title, fuLongHistoBinNb, -0.5, uAlignedLimit - 0.5,
                                                    uNbChanPerFeb, -0.5, uNbChanPerFeb - 0.5));

    /// Hit rates evo per channel, 1 minute bins, 24h
    sHistName = Form("hMuchFebAsicRateEvoLong_%03u", uFebIdx);
    title     = Form("Hits per second & StsXyter in FEB #%03u; Time [min]; Asic []; Hits []", uFebIdx);
    fvhMuchFebAsicHitRateEvoLong.push_back(new TH2D(sHistName, title, fuLongHistoBinNb, -0.5, uAlignedLimit - 0.5,
                                                    uNbAsicsPerFeb, -0.5, uNbAsicsPerFeb - 0.5));

    /// Hit rates evo per FEB, 1 minute bins, 24h
    sHistName = Form("hMuchFebRateEvoLong_%03u", uFebIdx);
    title     = Form("Hits per second in FEB #%03u; Time [min]; Hits []", uFebIdx);
    fvhMuchFebHitRateEvoLong.push_back(new TH1D(sHistName, title, fuLongHistoBinNb, -0.5, uAlignedLimit - 0.5));

    AddHistoToVector(fvhRawChRatio[uFebIdx], "perFeb");
    AddHistoToVector(fvhHitChRatio[uFebIdx], "perFeb");
    AddHistoToVector(fvhDupliChRatio[uFebIdx], "perFeb");
    AddHistoToVector(fvhRawHitRatioPerCh[uFebIdx], "perFeb");
    AddHistoToVector(fvhRawDupliRatioPerCh[uFebIdx], "perFeb");
    AddHistoToVector(fvhMuchFebChanCntRaw[uFebIdx], "perFeb");
    AddHistoToVector(fvhMuchFebChanAdcRaw[uFebIdx], "perFeb");
    AddHistoToVector(fvhMuchFebChanAdcRawProf[uFebIdx], "perFeb");
    AddHistoToVector(fvhMuchFebChanAdcCal[uFebIdx], "perFeb");
    AddHistoToVector(fvhMuchFebChanAdcCalProf[uFebIdx], "perFeb");
    AddHistoToVector(fvhMuchFebChanRawTs[uFebIdx], "perFeb");
    AddHistoToVector(fvhMuchFebChanMissEvt[uFebIdx], "perFeb");
    AddHistoToVector(fvhMuchFebChanMissEvtEvo[uFebIdx], "perFeb");
    AddHistoToVector(fvhMuchFebAsicMissEvtEvo[uFebIdx], "perFeb");
    AddHistoToVector(fvhMuchFebMissEvtEvo[uFebIdx], "perFeb");
    AddHistoToVector(fvhMuchFebChanHitRateEvo[uFebIdx], "perFeb");
    AddHistoToVector(fvhMuchFebAsicHitRateEvo[uFebIdx], "perFeb");
    AddHistoToVector(fvhMuchFebHitRateEvo[uFebIdx], "perFeb");
    AddHistoToVector(fvhMuchFebChanHitRateEvoLong[uFebIdx], "perFeb");
    AddHistoToVector(fvhMuchFebAsicHitRateEvoLong[uFebIdx], "perFeb");
    AddHistoToVector(fvhMuchFebHitRateEvoLong[uFebIdx], "perFeb");
  }

  /** Summary canvases **/
  const Double_t w = 2 * 400;
  const Double_t h = 3 * 200;

  // Create summary canvases per FEB
  fvcMuchSumm.resize(uNbFebs);
  fvcMuchSmxErr.resize(uNbFebs);
  for (UInt_t uFebIdx = 0; uFebIdx < uNbFebs; ++uFebIdx) {
    // if (kTRUE == fUnpackParMuch->IsFebActive(uFebIdx)) {
    fvcMuchSumm[uFebIdx] =
      new TCanvas(Form("cMuchSum_%03u", uFebIdx), Form("Summary plots for FEB %03u", uFebIdx), w, h);
    AddCanvasToVector(fvcMuchSumm[uFebIdx], "perFebCanvases");

    fvcMuchSumm[uFebIdx]->Divide(2, 3);
  }
  return kTRUE;
}

void CbmMuchUnpackMonitor::DrawCanvases()
{
  for (UInt_t uFebIdx = 0; uFebIdx < fvcMuchSumm.size(); ++uFebIdx) {
    // if (kTRUE == fUnpackParMuch->IsFebActive(uFebIdx)) {
    fvcMuchSumm[uFebIdx]->cd(1);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogy();
    fvhMuchFebChanCntRaw[uFebIdx]->Draw();

    //fvcMuchSumm[uFebIdx]->cd(2);
    //gPad->SetGridx();
    //gPad->SetGridy();
    //gPad->SetLogy();
    //fvhMuchFebChanHitRateProf[uFebIdx]->Draw("e0");

    fvcMuchSumm[uFebIdx]->cd(3);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fvhMuchFebChanAdcRaw[uFebIdx]->Draw("colz");

    fvcMuchSumm[uFebIdx]->cd(4);
    gPad->SetGridx();
    gPad->SetGridy();
    //gPad->SetLogy();
    fvhMuchFebChanAdcRawProf[uFebIdx]->Draw();

    fvcMuchSumm[uFebIdx]->cd(5);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fvhMuchFebChanHitRateEvo[uFebIdx]->Draw("colz");

    fvcMuchSumm[uFebIdx]->cd(6);
    gPad->SetGridx();
    gPad->SetGridy();
    //gPad->SetLogy();
    fvhMuchFebChanMissEvt[uFebIdx]->Draw("colz");

    // two following two are inactive as currently adc raw and cal are the same

    //fvcMuchSumm[ uFebIdx ]->cd(5);
    //gPad->SetGridx();
    //gPad->SetGridy();
    //gPad->SetLogz();
    //fvhMuchFebChanAdcCal[ uFebIdx ]->Draw( "colz" );

    //fvcMuchSumm[ uFebIdx ]->cd(6);
    //gPad->SetGridx();
    //gPad->SetGridy();
    //gPad->SetLogy();
    //fvhMuchFebChanAdcCalProf[ uFebIdx ]->Draw();
  }
}

Bool_t CbmMuchUnpackMonitor::CreateMsComponentSizeHistos(UInt_t component)
{
  if (nullptr == fvhMsSize[component]) {
    TString sMsSizeName  = Form("MuchMsSize_link_%02u", component);
    TString sMsSizeTitle = Form("Size of MS for nDPB of link %02u; Ms Size [bytes]", component);
    fvhMsSize[component] = new TH1F(sMsSizeName.Data(), sMsSizeTitle.Data(), 30000, 0., 30000.);
    fvhMsSize[component]->SetCanExtend(TH2::kAllAxes);
    AddHistoToVector(fvhMsSize[component], "perComponent");
  }
  if (nullptr == fvhMsSizeTime[component]) {
    TString sMsSizeName      = Form("MuchMsSizeTime_link_%02u", component);
    TString sMsSizeTitle     = Form("Size of MS vs time for gDPB of link %02u; Time[s] ; Ms Size [bytes]", component);
    fvhMsSizeTime[component] = new TProfile(sMsSizeName.Data(), sMsSizeTitle.Data(), 15000, 0., 300.);
    fvhMsSizeTime[component]->SetCanExtend(TH2::kAllAxes);
    AddHistoToVector(fvhMsSizeTime[component], "perComponent");
  }
  return kTRUE;
}

Bool_t CbmMuchUnpackMonitor::ResetMsComponentSizeHistos(UInt_t component)
{
  if (nullptr != fvhMsSize[component]) { fvhMsSize[component]->Reset(); }
  if (nullptr != fvhMsSizeTime[component]) { fvhMsSizeTime[component]->Reset(); }
  return kTRUE;
}

Bool_t CbmMuchUnpackMonitor::ResetHistograms()
{
  fhDigisTimeInRun->Reset();
  fhVectorSize->Reset();
  fhVectorCapacity->Reset();
  fhMsCntEvo->Reset();
  fhMsErrorsEvo->Reset();
  fhMuchAllFebsHitRateEvo->Reset();
  fhMuchAllAsicsHitRateEvo->Reset();
  fhMuchFebAsicHitCounts->Reset();
  fhMuchStatusMessType->Reset();
  fhRawHitRatioPerFeb->Reset();

  for (UInt_t uFebIdx = 0; uFebIdx < fvhRawChRatio.size(); ++uFebIdx) {
    fvhRawChRatio[uFebIdx]->Reset();
    fvhHitChRatio[uFebIdx]->Reset();
    fvhDupliChRatio[uFebIdx]->Reset();
    fvhRawHitRatioPerCh[uFebIdx]->Reset();
    fvhRawDupliRatioPerCh[uFebIdx]->Reset();
  }
  for (UInt_t uFebIdx = 0; uFebIdx < fvhMuchFebChanCntRaw.size(); ++uFebIdx) {
    fvhMuchFebChanCntRaw[uFebIdx]->Reset();
  }
  for (UInt_t uFebIdx = 0; uFebIdx < fvhMuchFebChanAdcRaw.size(); ++uFebIdx) {
    fvhMuchFebChanAdcRaw[uFebIdx]->Reset();
  }
  for (UInt_t uFebIdx = 0; uFebIdx < fvhMuchFebChanAdcRawProf.size(); ++uFebIdx) {
    fvhMuchFebChanAdcRawProf[uFebIdx]->Reset();
  }
  for (UInt_t uFebIdx = 0; uFebIdx < fvhMuchFebChanAdcCal.size(); ++uFebIdx) {
    fvhMuchFebChanAdcCal[uFebIdx]->Reset();
  }
  for (UInt_t uFebIdx = 0; uFebIdx < fvhMuchFebChanAdcCalProf.size(); ++uFebIdx) {
    fvhMuchFebChanAdcCalProf[uFebIdx]->Reset();
  }
  for (UInt_t uFebIdx = 0; uFebIdx < fvhMuchFebChanRawTs.size(); ++uFebIdx) {
    fvhMuchFebChanRawTs[uFebIdx]->Reset();
  }
  for (UInt_t uFebIdx = 0; uFebIdx < fvhMuchFebChanMissEvt.size(); ++uFebIdx) {
    fvhMuchFebChanMissEvt[uFebIdx]->Reset();
  }
  for (UInt_t uFebIdx = 0; uFebIdx < fvhMuchFebChanMissEvtEvo.size(); ++uFebIdx) {
    fvhMuchFebChanMissEvtEvo[uFebIdx]->Reset();
  }
  for (UInt_t uFebIdx = 0; uFebIdx < fvhMuchFebAsicMissEvtEvo.size(); ++uFebIdx) {
    fvhMuchFebAsicMissEvtEvo[uFebIdx]->Reset();
  }
  for (UInt_t uFebIdx = 0; uFebIdx < fvhMuchFebMissEvtEvo.size(); ++uFebIdx) {
    fvhMuchFebMissEvtEvo[uFebIdx]->Reset();
  }
  for (UInt_t uFebIdx = 0; uFebIdx < fvhMuchFebChanHitRateEvo.size(); ++uFebIdx) {
    fvhMuchFebChanHitRateEvo[uFebIdx]->Reset();
  }
  for (UInt_t uFebIdx = 0; uFebIdx < fvhMuchFebAsicHitRateEvo.size(); ++uFebIdx) {
    fvhMuchFebAsicHitRateEvo[uFebIdx]->Reset();
  }
  for (UInt_t uFebIdx = 0; uFebIdx < fvhMuchFebHitRateEvo.size(); ++uFebIdx) {
    fvhMuchFebHitRateEvo[uFebIdx]->Reset();
  }
  for (UInt_t uFebIdx = 0; uFebIdx < fvhMuchFebChanHitRateEvoLong.size(); ++uFebIdx) {
    fvhMuchFebChanHitRateEvoLong[uFebIdx]->Reset();
  }
  for (UInt_t uFebIdx = 0; uFebIdx < fvhMuchFebAsicHitRateEvoLong.size(); ++uFebIdx) {
    fvhMuchFebAsicHitRateEvoLong[uFebIdx]->Reset();
  }
  for (UInt_t uFebIdx = 0; uFebIdx < fvhMuchFebHitRateEvoLong.size(); ++uFebIdx) {
    fvhMuchFebHitRateEvoLong[uFebIdx]->Reset();
  }
  return kTRUE;
}

Bool_t CbmMuchUnpackMonitor::CreateDebugHistograms(CbmMuchUnpackPar* pUnpackPar)
{
  const UInt_t uNbAsics       = pUnpackPar->GetNrOfAsics();
  const UInt_t uNrOfDpbs      = pUnpackPar->GetNrOfDpbs();
  const UInt_t uNbChanPerAsic = pUnpackPar->GetNbChanPerAsic();
  const UInt_t uNbFebs        = pUnpackPar->GetNrOfFebs();
  const UInt_t uNbChanPerFeb  = pUnpackPar->GetNbChanPerFeb();
  TString sHistName {""};
  TString title {""};

  sHistName      = "hMuchPulserMessageType";
  title          = "Nb of message for each type; Type";
  fhMuchMessType = new TH1I(sHistName, title, 6, 0., 6.);
  fhMuchMessType->GetXaxis()->SetBinLabel(1, "Dummy");
  fhMuchMessType->GetXaxis()->SetBinLabel(2, "Hit");
  fhMuchMessType->GetXaxis()->SetBinLabel(3, "TsMsb");
  fhMuchMessType->GetXaxis()->SetBinLabel(4, "Epoch");
  fhMuchMessType->GetXaxis()->SetBinLabel(5, "Status");
  fhMuchMessType->GetXaxis()->SetBinLabel(6, "Empty");
  AddHistoToVector(fhMuchMessType, "");

  sHistName            = "hMuchPulserMessageTypePerDpb";
  title                = "Nb of message of each type for each DPB; DPB; Type";
  fhMuchMessTypePerDpb = new TH2I(sHistName, title, uNrOfDpbs, 0, uNrOfDpbs, 6, 0., 6.);
  fhMuchMessTypePerDpb->GetYaxis()->SetBinLabel(1, "Dummy");
  fhMuchMessTypePerDpb->GetYaxis()->SetBinLabel(2, "Hit");
  fhMuchMessTypePerDpb->GetYaxis()->SetBinLabel(3, "TsMsb");
  fhMuchMessTypePerDpb->GetYaxis()->SetBinLabel(4, "Epoch");
  fhMuchMessTypePerDpb->GetYaxis()->SetBinLabel(5, "Status");
  fhMuchMessTypePerDpb->GetYaxis()->SetBinLabel(6, "Empty");
  AddHistoToVector(fhMuchMessTypePerDpb, "");

  sHistName = "hMuchMessTypePerElink";
  title     = "Nb of message of each type for each eLink; eLink; Type";
  fhMuchMessTypePerElink =
    new TH2I(sHistName, title, uNrOfDpbs * fNrElinksPerDpb, 0, uNrOfDpbs * fNrElinksPerDpb, 6, 0., 6.);
  fhMuchMessTypePerElink->GetYaxis()->SetBinLabel(1, "Dummy");
  fhMuchMessTypePerElink->GetYaxis()->SetBinLabel(2, "Hit");
  fhMuchMessTypePerElink->GetYaxis()->SetBinLabel(3, "TsMsb");
  fhMuchMessTypePerElink->GetYaxis()->SetBinLabel(4, "Epoch");
  fhMuchMessTypePerElink->GetYaxis()->SetBinLabel(5, "Status");
  fhMuchMessTypePerElink->GetYaxis()->SetBinLabel(6, "Empty");
  AddHistoToVector(fhMuchMessTypePerElink, "");

  sHistName             = "hMuchHitsElinkPerDpb";
  title                 = "Nb of hit messages per eLink for each DPB; DPB; eLink; Hits nb []";
  fhMuchHitsElinkPerDpb = new TH2I(sHistName, title, uNrOfDpbs, 0, uNrOfDpbs, 42, 0., 42.);
  AddHistoToVector(fhMuchHitsElinkPerDpb, "");

  sHistName         = "hMuchDpbRawTsMsb";
  title             = "MSB messages for each DPB; DPB; TsMsb; Count []";
  fhMuchDpbRawTsMsb = new TH2I(sHistName, title, uNrOfDpbs, 0, uNrOfDpbs, 10, 0, 10);
  fhMuchDpbRawTsMsb->SetCanExtend(TH2::kAllAxes);
  AddHistoToVector(fhMuchDpbRawTsMsb, "");

  sHistName           = "hMuchDpbRawTsMsbSx";
  title               = "MSB SX messages for each DPB; DPB; TsMsb & 0x1F; Count []";
  fhMuchDpbRawTsMsbSx = new TH2I(sHistName, title, uNrOfDpbs, 0, uNrOfDpbs, 10, 0, 10);
  fhMuchDpbRawTsMsbSx->SetCanExtend(TH2::kAllAxes);
  AddHistoToVector(fhMuchDpbRawTsMsbSx, "");

  sHistName            = "hMuchDpbRawTsMsbDpb";
  title                = "MSB DPB messages for each DPB; DPB; TsMsb >> 5; Count []";
  fhMuchDpbRawTsMsbDpb = new TH2I(sHistName, title, uNrOfDpbs, 0, uNrOfDpbs, 10, 0, 10);
  fhMuchDpbRawTsMsbDpb->SetCanExtend(TH2::kAllAxes);
  AddHistoToVector(fhMuchDpbRawTsMsbDpb, "");

  /// Timeslice counter ratio Plots
  sHistName              = "hMuchRawHitRatioEvoPerFeb";
  title                  = "Proportion of digis over raw hits in each FEB; Time [s]; FEB []; digis/raw [Prct]";
  fhRawHitRatioEvoPerFeb = new TProfile2D(sHistName, title, 600, -0.5, 599.5, uNbFebs, -0.5, uNbFebs - 0.5);
  AddHistoToVector(fhRawHitRatioEvoPerFeb, "");
  for (uint32_t uFebIdx = 0; uFebIdx < uNbFebs; ++uFebIdx) {
    sHistName = Form("hMuchChDupliAdc_%03d", uFebIdx);
    title     = Form("ADC in duplicate raw in each channel of FEB %2d; Channel []; ADC []", uFebIdx);
    fvhChDupliAdc.push_back(new TH2I(sHistName, title, uNbChanPerFeb, -0.5, uNbChanPerFeb - 0.5, 32, -0.5, 31.5));

    sHistName = Form("hMuchRawChRatioEvo_%03d", uFebIdx);
    title = Form("Proportion of raw hits in each channel of FEB %2d; Time [s]; Channel []; Share of FEB raw msg [Prct]",
                 uFebIdx);
    fvhRawChRatioEvo.push_back(
      new TProfile2D(sHistName, title, 600, -0.5, 599.5, uNbChanPerFeb, -0.5, uNbChanPerFeb - 0.5));
    sHistName = Form("hMuchHitChRatioEvo_%03d", uFebIdx);
    title =
      Form("Proportion of digis in each channel of FEB %2d; Time [s]; Channel []; Share of FEB digis [Prct]", uFebIdx);
    fvhHitChRatioEvo.push_back(
      new TProfile2D(sHistName, title, 600, -0.5, 599.5, uNbChanPerFeb, -0.5, uNbChanPerFeb - 0.5));
    sHistName = Form("hMuchDupliChRatioEvo_%03d", uFebIdx);
    title =
      Form("Proportion of duplicates in each channel of FEB %2d; Time [s]; Channel []; Share of FEB duplicates [Prct]",
           uFebIdx);
    fvhDupliChRatioEvo.push_back(
      new TProfile2D(sHistName, title, 600, -0.5, 599.5, uNbChanPerFeb, -0.5, uNbChanPerFeb - 0.5));
    sHistName = Form("hMuchRawHitRatioEvoPerCh_%03d", uFebIdx);
    title = Form("Proportion of digis over raw hits in each channel of FEB %2d; Time [s]; Channel []; digis/raw [Prct]",
                 uFebIdx);
    fvhRawHitRatioEvoPerCh.push_back(
      new TProfile2D(sHistName, title, 600, -0.5, 599.5, uNbChanPerFeb, -0.5, uNbChanPerFeb - 0.5));
    sHistName = Form("hMuchRawDupliRatioEvoPerCh_%03d", uFebIdx);
    title =
      Form("Proportion of duplicates over raw hits in each channel of FEB %2d; Time [s]; Channel []; dupli/raw [Prct]",
           uFebIdx);
    fvhRawDupliRatioEvoPerCh.push_back(
      new TProfile2D(sHistName, title, 600, -0.5, 599.5, uNbChanPerFeb, -0.5, uNbChanPerFeb - 0.5));

    AddHistoToVector(fvhChDupliAdc[uFebIdx], "perFeb");
    AddHistoToVector(fvhRawChRatioEvo[uFebIdx], "perFeb");
    AddHistoToVector(fvhHitChRatioEvo[uFebIdx], "perFeb");
    AddHistoToVector(fvhDupliChRatioEvo[uFebIdx], "perFeb");
    AddHistoToVector(fvhRawHitRatioEvoPerCh[uFebIdx], "perFeb");
    AddHistoToVector(fvhRawDupliRatioEvoPerCh[uFebIdx], "perFeb");
  }

  /// Asic plots
  /// All histos per Asic: with channels or ASIC as axis!!
  for (UInt_t uAsicIdx = 0; uAsicIdx < uNbAsics; ++uAsicIdx) {

    /// Channel counts
    sHistName = Form("hMuchChanCntRaw_%03u", uAsicIdx);
    title     = Form("Hits Count per channel, Asic #%03u; Channel; Hits []", uAsicIdx);
    fvhMuchChanCntRaw.push_back(new TH1I(sHistName, title, uNbChanPerAsic, -0.5, uNbChanPerAsic - 0.5));

    /// Raw Adc Distribution
    sHistName = Form("hMuchChanAdcRaw_%03u", uAsicIdx);
    title     = Form("Raw Adc distribution per channel, Asic #%03u; Channel []; Adc "
                 "[]; Hits []",
                 uAsicIdx);
    fvhMuchChanAdcRaw.push_back(new TH2I(sHistName, title, uNbChanPerAsic, -0.5, uNbChanPerAsic - 0.5,
                                         stsxyter::kuHitNbAdcBins, -0.5, stsxyter::kuHitNbAdcBins - 0.5));

    /// Raw Adc Distribution profile
    sHistName = Form("hMuchChanAdcRawProfc_%03u", uAsicIdx);
    title     = Form("Raw Adc prodile per channel, Asic #%03u; Channel []; Adc []", uAsicIdx);
    fvhMuchChanAdcRawProf.push_back(new TProfile(sHistName, title, uNbChanPerAsic, -0.5, uNbChanPerAsic - 0.5));

    /// Raw Ts Distribution
    sHistName = Form("hMuchChanRawTs_%03u", uAsicIdx);
    title     = Form("Raw Timestamp distribution per channel, Asic #%03u; Channel "
                 "[]; Ts []; Hits []",
                 uAsicIdx);
    fvhMuchChanRawTs.push_back(new TH2I(sHistName, title, uNbChanPerAsic, -0.5, uNbChanPerAsic - 0.5,
                                        stsxyter::kuHitNbTsBins, -0.5, stsxyter::kuHitNbTsBins - 0.5));

    /// Missed event flag
    sHistName = Form("hMuchChanMissEvt_%03u", uAsicIdx);
    title     = Form("Missed Event flags per channel, Asic #%03u; Channel []; Miss "
                 "Evt []; Hits []",
                 uAsicIdx);
    fvhMuchChanMissEvt.push_back(new TH2I(sHistName, title, uNbChanPerAsic, -0.5, uNbChanPerAsic - 0.5, 2, -0.5, 1.5));

    AddHistoToVector(fvhMuchChanCntRaw[uAsicIdx], "perAsic");
    AddHistoToVector(fvhMuchChanAdcRaw[uAsicIdx], "perAsic");
    AddHistoToVector(fvhMuchChanAdcRawProf[uAsicIdx], "perAsic");
    AddHistoToVector(fvhMuchChanRawTs[uAsicIdx], "perAsic");
    AddHistoToVector(fvhMuchChanMissEvt[uAsicIdx], "perAsic");
  }
  return kTRUE;
}

Bool_t CbmMuchUnpackMonitor::ResetDebugHistograms()
{
  fhMuchMessType->Reset();
  fhMuchMessTypePerDpb->Reset();
  fhMuchMessTypePerElink->Reset();
  fhMuchHitsElinkPerDpb->Reset();
  fhMuchDpbRawTsMsb->Reset();
  fhMuchDpbRawTsMsbSx->Reset();
  fhMuchDpbRawTsMsbDpb->Reset();
  fhRawHitRatioEvoPerFeb->Reset();

  for (UInt_t uFebIdx = 0; uFebIdx < fvhRawChRatioEvo.size(); ++uFebIdx) {
    fvhChDupliAdc[uFebIdx]->Reset();
    fvhRawChRatioEvo[uFebIdx]->Reset();
    fvhHitChRatioEvo[uFebIdx]->Reset();
    fvhDupliChRatioEvo[uFebIdx]->Reset();
    fvhRawHitRatioEvoPerCh[uFebIdx]->Reset();
    fvhRawDupliRatioEvoPerCh[uFebIdx]->Reset();
  }

  for (UInt_t uAsicIdx = 0; uAsicIdx < fvhMuchChanCntRaw.size(); ++uAsicIdx) {
    fvhMuchChanCntRaw[uAsicIdx]->Reset();
  }
  for (UInt_t uAsicIdx = 0; uAsicIdx < fvhMuchChanAdcRaw.size(); ++uAsicIdx) {
    fvhMuchChanAdcRaw[uAsicIdx]->Reset();
  }
  for (UInt_t uAsicIdx = 0; uAsicIdx < fvhMuchChanAdcRawProf.size(); ++uAsicIdx) {
    fvhMuchChanAdcRawProf[uAsicIdx]->Reset();
  }
  for (UInt_t uAsicIdx = 0; uAsicIdx < fvhMuchChanRawTs.size(); ++uAsicIdx) {
    fvhMuchChanRawTs[uAsicIdx]->Reset();
  }
  for (UInt_t uAsicIdx = 0; uAsicIdx < fvhMuchChanMissEvt.size(); ++uAsicIdx) {
    fvhMuchChanMissEvt[uAsicIdx]->Reset();
  }
  return kTRUE;
}
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
void CbmMuchUnpackMonitor::FillHitMonitoringHistos(const UInt_t& uFebIdx, const UShort_t& usChan,
                                                   const UInt_t& uChanInFeb, const UShort_t& usRawAdc,
                                                   const Double_t& dCalAdc, const UShort_t& usRawTs,
                                                   const bool& isHitMissedEvts)
{
  FillMuchFebChanAdcCal(uFebIdx, uChanInFeb, dCalAdc);
  FillMuchFebChanAdcCalProf(uFebIdx, uChanInFeb, dCalAdc);
  FillMuchFebChanCntRaw(uFebIdx, uChanInFeb);
  FillMuchFebChanAdcRaw(uFebIdx, uChanInFeb, usRawAdc);
  FillMuchFebChanRawTs(uFebIdx, usChan, usRawTs);
  FillMuchFebChanMissEvt(uFebIdx, usChan, isHitMissedEvts);
  FillMuchFebChanAdcRawProf(uFebIdx, uChanInFeb, usRawAdc);
}


// -------------------------------------------------------------------------
void CbmMuchUnpackMonitor::FillHitDebugMonitoringHistos(const UInt_t& uAsicIdx, const UShort_t& usChan,
                                                        const UShort_t& usRawAdc, const UShort_t& usRawTs,
                                                        const bool& isHitMissedEvts)
{
  FillMuchChanCntRaw(uAsicIdx, usChan);
  FillMuchChanAdcRaw(uAsicIdx, usChan, usRawAdc);
  FillMuchChanAdcRawProf(uAsicIdx, usChan, usRawAdc);
  FillMuchChanRawTs(uAsicIdx, usChan, usRawTs);
  FillMuchChanMissEvt(uAsicIdx, usChan, isHitMissedEvts);
}


// -------------------------------------------------------------------------
void CbmMuchUnpackMonitor::FillHitEvoMonitoringHistos(const UInt_t& uFebIdx, const UInt_t& uAsicIdx,
                                                      const UInt_t& uAsicInFeb, const UInt_t& uChanInFeb,
                                                      const Double_t& dAbsTimeSec, const bool& isHitMissedEvts)
{
  // Check Starting point of histos with time as X axis
  if (-1 == fdStartTime) { fdStartTime = dAbsTimeSec; }

  Double_t dTimeSinceStartSec = dAbsTimeSec - fdStartTime;

  // Fill histos with time as X axis
  FillMuchFebAsicHitCounts(uFebIdx, uAsicInFeb);
  FillMuchFebChanHitRateEvo(uFebIdx, dTimeSinceStartSec, uChanInFeb);
  FillMuchFebAsicHitRateEvo(uFebIdx, dTimeSinceStartSec, uAsicInFeb);
  FillMuchFebHitRateEvo(uFebIdx, dTimeSinceStartSec);
  FillMuchAllFebsHitRateEvo(dTimeSinceStartSec, uFebIdx);
  FillMuchAllAsicsHitRateEvo(dTimeSinceStartSec, uAsicIdx);
  if (isHitMissedEvts) {
    FillMuchFebChanMissEvtEvo(uFebIdx, dTimeSinceStartSec, uChanInFeb);
    FillMuchFebAsicMissEvtEvo(uFebIdx, dTimeSinceStartSec, uAsicInFeb);
    FillMuchFebMissEvtEvo(uFebIdx, dTimeSinceStartSec);
  }
  const Double_t dTimeSinceStartMin = dTimeSinceStartSec / 60.0;
  FillMuchFebHitRateEvoLong(uFebIdx, dTimeSinceStartMin);
  FillMuchFebChanHitRateEvoLong(uFebIdx, dTimeSinceStartMin, uChanInFeb);
  FillMuchFebAsicHitRateEvoLong(uFebIdx, dTimeSinceStartMin, uAsicInFeb);

  // fviFebCountsSinceLastRateUpdate[uFebIdx]++;
  // fvdFebChanCountsSinceLastRateUpdate[uFebIdx][uChanInFeb] += 1;
  /*
   if( kTRUE == fbLongHistoEnable )
   {
      std::chrono::steady_clock::time_point tNow = std::chrono::steady_clock::now();
      Double_t dUnixTimeInRun = std::chrono::duration_cast< std::chrono::seconds >(tNow - ftStartTimeUnix).count();
      fhFebRateEvoLong[ uAsicIdx ]->Fill( dUnixTimeInRun , 1.0 / fuLongHistoBinSizeSec );
      fhFebChRateEvoLong[ uAsicIdx ]->Fill( dUnixTimeInRun , usChan, 1.0 / fuLongHistoBinSizeSec );
   }
*/
}

// -------------------------------------------------------------------------
void CbmMuchUnpackMonitor::FillPerTimesliceCountersHistos(double_t dTsStartTimeS)
{
  if (0 == dFirstTsStartTime) dFirstTsStartTime = dTsStartTimeS;
  double_t dTimeInRun     = dTsStartTimeS - dFirstTsStartTime;
  double_t dRatio         = 0;
  uint32_t uNbFebs        = fvuNbRawTsFeb.size();
  uint32_t uNbChansPerFeb = (uNbFebs ? fvvuNbRawTsChan[0].size() : 0);
  for (uint32_t uFebIdx = 0; uFebIdx < uNbFebs; ++uFebIdx) {
    uint32_t uNbDupliFeb = fvuNbRawTsFeb[uFebIdx] - fvuNbDigisTsFeb[uFebIdx];
    if (fvuNbRawTsFeb[uFebIdx]) {
      dRatio = fvuNbDigisTsFeb[uFebIdx] * 100.0 / fvuNbRawTsFeb[uFebIdx];
      fhRawHitRatioPerFeb->Fill(uFebIdx, dRatio);
      if (fDebugMode) {  //
        fhRawHitRatioEvoPerFeb->Fill(dTimeInRun, uFebIdx, dRatio);
      }
    }

    for (uint32_t uChan = 0; uChan < uNbChansPerFeb; ++uChan) {
      uint32_t uNbDupliChan = fvvuNbRawTsChan[uFebIdx][uChan] - fvvuNbDigisTsChan[uFebIdx][uChan];
      if (fvuNbRawTsFeb[uFebIdx]) {
        dRatio = fvvuNbRawTsChan[uFebIdx][uChan] * 100.0 / fvuNbRawTsFeb[uFebIdx];
        fvhRawChRatio[uFebIdx]->Fill(uChan, dRatio);
        if (fDebugMode) {  //
          fvhRawChRatioEvo[uFebIdx]->Fill(dTimeInRun, uChan, dRatio);
        }
      }

      if (fvuNbDigisTsFeb[uFebIdx]) {
        dRatio = fvvuNbDigisTsChan[uFebIdx][uChan] * 100.0 / fvuNbDigisTsFeb[uFebIdx];
        fvhHitChRatio[uFebIdx]->Fill(uChan, dRatio);
        if (fDebugMode) {  //
          fvhHitChRatioEvo[uFebIdx]->Fill(dTimeInRun, uChan, dRatio);
        }
      }

      if (uNbDupliFeb) {
        dRatio = uNbDupliChan * 100.0 / uNbDupliFeb;
        fvhDupliChRatio[uFebIdx]->Fill(uChan, dRatio);
        if (fDebugMode) {  //
          fvhDupliChRatioEvo[uFebIdx]->Fill(dTimeInRun, uChan, dRatio);
        }
      }

      if (fvvuNbRawTsChan[uFebIdx][uChan]) {
        dRatio = fvvuNbDigisTsChan[uFebIdx][uChan] * 100.0 / fvvuNbRawTsChan[uFebIdx][uChan];
        fvhRawHitRatioPerCh[uFebIdx]->Fill(uChan, dRatio);
        if (fDebugMode) {  //
          fvhRawHitRatioEvoPerCh[uFebIdx]->Fill(dTimeInRun, uChan, dRatio);
        }

        dRatio = uNbDupliChan * 100.0 / fvvuNbRawTsChan[uFebIdx][uChan];
        fvhRawDupliRatioPerCh[uFebIdx]->Fill(uChan, dRatio);
        if (fDebugMode) {  //
          fvhRawDupliRatioEvoPerCh[uFebIdx]->Fill(dTimeInRun, uChan, dRatio);
        }
      }

      fvvuNbRawTsChan[uFebIdx][uChan]   = 0;
      fvvuNbDigisTsChan[uFebIdx][uChan] = 0;
    }
    fvuNbRawTsFeb[uFebIdx]   = 0;
    fvuNbDigisTsFeb[uFebIdx] = 0;
  }
}

// -------------------------------------------------------------------------
void CbmMuchUnpackMonitor::FillDuplicateHitsAdc(const uint32_t& uFebIdx, const uint32_t& uChanInFeb,
                                                const uint16_t& usAdc)
{
  if (fDebugMode) {  //
    fvhChDupliAdc[uFebIdx]->Fill(uChanInFeb, usAdc);
  }
}
// -------------------------------------------------------------------------
void CbmMuchUnpackMonitor::ResetDebugInfo()
{
  std::fill(vNbMessType.begin(), vNbMessType.end(), 0);
  sMessPatt = "";
  bError    = false;
}

// -------------------------------------------------------------------------
void CbmMuchUnpackMonitor::ProcessDebugInfo(const stsxyter::Message& Mess, const UInt_t& uCurrDpbIdx)
{
  const stsxyter::MessType typeMess = Mess.GetMessType();

  FillMuchMessType(static_cast<uint16_t>(typeMess));
  FillMuchMessTypePerDpb(uCurrDpbIdx, static_cast<uint16_t>(typeMess));
  switch (typeMess) {
    case stsxyter::MessType::Hit: {
      ++vNbMessType[0];
      sMessPatt += " H ";
      sMessPatt += ".";

      const UShort_t usElinkIdx = Mess.GetLinkIndexHitBinning();
      FillMuchMessTypePerElink(usElinkIdx, static_cast<uint16_t>(typeMess));
      FillMuchHitsElinkPerDpb(uCurrDpbIdx, usElinkIdx);
      break;
    }
    case stsxyter::MessType::TsMsb: {
      ++vNbMessType[1];
      sMessPatt += " T ";
      FillMuchMessTypePerElink(uCurrDpbIdx * fNrElinksPerDpb, static_cast<uint16_t>(typeMess));
      break;
    }
    case stsxyter::MessType::Epoch: {
      ++vNbMessType[2];
      sMessPatt += " E ";
      FillMuchMessTypePerElink(uCurrDpbIdx * fNrElinksPerDpb, static_cast<uint16_t>(typeMess));
      break;
    }
    case stsxyter::MessType::Status: {
      ++vNbMessType[3];
      sMessPatt += " S ";
      FillMuchMessTypePerElink(Mess.GetLinkIndex(), static_cast<uint16_t>(typeMess));
      break;
    }
    case stsxyter::MessType::Empty: {
      ++vNbMessType[4];
      sMessPatt += " Em";
      FillMuchMessTypePerElink(uCurrDpbIdx * fNrElinksPerDpb, static_cast<uint16_t>(typeMess));
      break;
    }
    case stsxyter::MessType::EndOfMs: {
      ++vNbMessType[5];
      sMessPatt += " En";
      bError = Mess.IsMsErrorFlagOn();
      FillMuchMessTypePerElink(uCurrDpbIdx * fNrElinksPerDpb, static_cast<uint16_t>(typeMess));
      break;
    }
    case stsxyter::MessType::Dummy: {
      ++vNbMessType[6];
      sMessPatt += " D ";
      FillMuchMessTypePerElink(uCurrDpbIdx * fNrElinksPerDpb, static_cast<uint16_t>(typeMess));
      break;
    }
    default: {
    }
  }
}

// -------------------------------------------------------------------------
void CbmMuchUnpackMonitor::PrintDebugInfo(const uint64_t MsStartTime, const size_t NrProcessedTs,
                                          const uint16_t msDescriptorFlags, const uint32_t uSize)
{
  if (18967040000 == MsStartTime || 18968320000 == MsStartTime) { LOG(debug) << sMessPatt; }
  if (static_cast<uint16_t>(fles::MicrosliceFlags::CrcValid) != msDescriptorFlags) {
    LOG(debug) << "STS unp "
               << " TS " << std::setw(12) << NrProcessedTs << " MS " << std::setw(12) << MsStartTime << " MS flags 0x"
               << std::setw(4) << std::hex << msDescriptorFlags << std::dec << " Size " << std::setw(8) << uSize
               << " bytes "
               << " H " << std::setw(5) << vNbMessType[0] << " T " << std::setw(5) << vNbMessType[1] << " E "
               << std::setw(5) << vNbMessType[2] << " S " << std::setw(5) << vNbMessType[3] << " Em " << std::setw(5)
               << vNbMessType[4] << " En " << std::setw(5) << vNbMessType[5] << " D " << std::setw(5) << vNbMessType[6]
               << " Err " << bError << " Undet. bad " << (!bError && 400 != vNbMessType[1]);
  }
}

// ---- Init ----
Bool_t CbmMuchUnpackMonitor::Init(CbmMuchUnpackPar* parset)
{
  // Get Infos from the parset
  fNrElinksPerDpb = parset->GetNbElinkPerDpb();

  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;
  gROOT->cd();

  /// Trigger histo creation on all associated monitors
  CreateHistograms(parset);
  if (fDebugMode) CreateDebugHistograms(parset);

  /// Trigger Canvas creation on all associated monitors
  DrawCanvases();

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;

  /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
  std::vector<std::pair<TNamed*, std::string>> vHistos = GetHistoVector();

  /// Obtain vector of pointers on each canvas from the algo (+ optionally desired folder)
  std::vector<std::pair<TCanvas*, std::string>> vCanvases = GetCanvasVector();

  /// Register the histos and canvases in the HTTP server
  THttpServer* server = FairRunOnline::Instance()->GetHttpServer();
  if (nullptr != server) {
    for (UInt_t uCanvas = 0; uCanvas < vCanvases.size(); ++uCanvas) {
      server->Register(Form("/much/%s", vCanvases[uCanvas].second.data()), vCanvases[uCanvas].first);
    }
    for (UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto) {
      server->Register(Form("/much/%s", vHistos[uHisto].second.data()), vHistos[uHisto].first);
    }
    /*
    server->RegisterCommand("/Reset_UnpMuch_Hist", "bMcbm2018UnpackerTaskMuchResetHistos=kTRUE");
    server->Restrict("/Reset_UnpMuch_Hist", "allow=admin");
*/
  }

  return kTRUE;
}

// ---- Finish ----
void CbmMuchUnpackMonitor::Finish()
{
  /// Obtain vector of pointers on each histo (+ optionally desired folder)
  std::vector<std::pair<TNamed*, std::string>> vHistos = GetHistoVector();

  /// Obtain vector of pointers on each canvas (+ optionally desired folder)
  std::vector<std::pair<TCanvas*, std::string>> vCanvases = GetCanvasVector();

  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;
  TFile* histoFile   = nullptr;

  // open separate histo file in recreate mode
  histoFile = new TFile(fHistoFileName, "RECREATE");
  histoFile->cd();

  /// Write histos to output file
  for (UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto) {
    /// Make sure we end up in chosen folder
    TString sFolder = vHistos[uHisto].second.data();
    if (nullptr == gDirectory->Get(sFolder)) gDirectory->mkdir(sFolder);
    gDirectory->cd(sFolder);

    /// Write plot
    vHistos[uHisto].first->Write();

    histoFile->cd();
  }

  /// Write canvases to output file
  for (UInt_t uCanvas = 0; uCanvas < vCanvases.size(); ++uCanvas) {
    /// Make sure we end up in chosen folder
    TString sFolder = vCanvases[uCanvas].second.data();
    if (nullptr == gDirectory->Get(sFolder)) gDirectory->mkdir(sFolder);
    gDirectory->cd(sFolder);

    /// Write canvas
    vCanvases[uCanvas].first->Write();

    histoFile->cd();
  }

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;

  histoFile->Close();
  delete histoFile;
}


ClassImp(CbmMuchUnpackMonitor)
