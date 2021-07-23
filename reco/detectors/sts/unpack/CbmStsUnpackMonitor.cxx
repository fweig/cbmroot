#include "CbmStsUnpackMonitor.h"

#include "CbmQaCanvas.h"

#include <FairRun.h>
#include <FairRunOnline.h>
#include <Logger.h>

#include <RtypesCore.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <THttpServer.h>
#include <TProfile.h>

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <cmath>

CbmStsUnpackMonitor::CbmStsUnpackMonitor(/* args */) : fvpAllHistoPointers()
{
  // Miscroslice component properties histos
  for (UInt_t component = 0; component < kiMaxNbFlibLinks; component++) {
    fvhMsSize[component]     = nullptr;
    fvhMsSizeTime[component] = nullptr;
  }
}

CbmStsUnpackMonitor::~CbmStsUnpackMonitor()
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

Bool_t CbmStsUnpackMonitor::CreateHistograms(CbmMcbm2018StsPar* pUnpackPar)
{
  TString sHistName {""};
  TString title {""};
  const UInt_t uNbAsics       = pUnpackPar->GetNrOfAsics();
  const UInt_t uNbFebs        = pUnpackPar->GetNrOfFebs();
  const UInt_t uNbAsicsPerFeb = pUnpackPar->GetNbAsicsPerFeb();
  const UInt_t uNbChanPerFeb  = pUnpackPar->GetNbChanPerFeb();

  /// Create general unpacking histograms
  fhDigisTimeInRun = new TH1I("hStsDigisTimeInRun", "Digis Nb vs Time in Run; Time in run [s]; Digis Nb []", 10, 0, 1);
  fhDigisTimeInRun->SetCanExtend(TH1::kAllAxes);
  AddHistoToVector(fhDigisTimeInRun, "");

  fhVectorSize = new TH1I("fhVectorSize", "Size of the vector VS TS index; TS index; Size [bytes]", 10, 0, 10);
  fhVectorSize->SetCanExtend(TH1::kAllAxes);
  AddHistoToVector(fhVectorSize, "");

  fhVectorCapacity =
    new TH1I("fhVectorCapacity", "Size of the vector VS TS index; TS index; Size [bytes]", 10000, 0., 10000.);
  AddHistoToVector(fhVectorCapacity, "");

  fhMsCntEvo = new TH1I("fhMsCntEvo", "; MS index [s]; Counts []", 600, 0.0, 600.0);
  AddHistoToVector(fhMsCntEvo, "");

  fhMsErrorsEvo = new TH2I("fhMsErrorsEvo", "; MS index [s]; Error type []; Counts []", 600, 0.0, 600.0, 4, -0.5, 3.5);
  AddHistoToVector(fhMsErrorsEvo, "");

  /// Hit rates evo per FEB in system
  sHistName              = "hStsAllFebsHitRateEvo";
  title                  = "Hits per second & FEB; Time [s]; FEB []; Hits []";
  fhStsAllFebsHitRateEvo = new TH2I(sHistName, title, 5, 0, 5, uNbFebs, -0.5, uNbFebs - 0.5);
  fhStsAllFebsHitRateEvo->SetCanExtend(TH1::kAllAxes);
  AddHistoToVector(fhStsAllFebsHitRateEvo, "");

  /// Hit rates evo per ASIC in system
  sHistName               = "hStsAllAsicsHitRateEvo";
  title                   = "Hits per second & ASIC; Time [s]; ASIC []; Hits []";
  fhStsAllAsicsHitRateEvo = new TH2I(sHistName, title, 5, 0, 5, uNbAsics, -0.5, uNbAsics - 0.5);
  fhStsAllAsicsHitRateEvo->SetCanExtend(TH1::kAllAxes);
  AddHistoToVector(fhStsAllAsicsHitRateEvo, "");

  /// Hit counts map in system
  sHistName = "hStsFebAsicHitCounts";
  title     = "Hits per FEB & ASIC; FEB []; ASIC in FEB[]; Hits []";
  fhStsFebAsicHitCounts =
    new TH2I(sHistName, title, uNbFebs, -0.5, uNbFebs - 0.5, uNbAsicsPerFeb, -0.5, uNbAsicsPerFeb - 0.5);
  AddHistoToVector(fhStsFebAsicHitCounts, "");

  sHistName           = "hStsStatusMessType";
  title               = "Nb of status message of each type for each DPB; ASIC; Status Type";
  fhStsStatusMessType = new TH2I(sHistName, title, uNbAsics, 0, uNbAsics, 16, 0., 16.);
  AddHistoToVector(fhStsStatusMessType, "");

  ///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
  UInt_t uAlignedLimit = fuLongHistoNbSeconds - (fuLongHistoNbSeconds % fuLongHistoBinSizeSec);
  fuLongHistoBinNb     = uAlignedLimit / fuLongHistoBinSizeSec;

  ///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
  /// FEB-8 plots
  /// All histos per FEB: with channels or ASIC as axis!!
  for (UInt_t uFebIdx = 0; uFebIdx < uNbFebs; ++uFebIdx) {

    /// Channel counts
    sHistName = Form("hStsFebChanCntRaw_%03u", uFebIdx);
    title     = Form("Hits Count per channel, FEB #%03u; Channel; Hits []", uFebIdx);
    fvhStsFebChanCntRaw.push_back(new TH1I(sHistName, title, uNbChanPerFeb, -0.5, uNbChanPerFeb - 0.5));

    /// Raw Adc Distribution
    sHistName = Form("hStsFebChanAdcRaw_%03u", uFebIdx);
    title     = Form("Raw Adc distribution per channel, FEB #%03u; Channel []; Adc "
                 "[]; Hits []",
                 uFebIdx);
    fvhStsFebChanAdcRaw.push_back(new TH2I(sHistName, title, uNbChanPerFeb, -0.5, uNbChanPerFeb - 0.5,
                                           stsxyter::kuHitNbAdcBins, -0.5, stsxyter::kuHitNbAdcBins - 0.5));

    /// Raw Adc Distribution profile
    sHistName = Form("hStsFebChanAdcRawProfc_%03u", uFebIdx);
    title     = Form("Raw Adc prodile per channel, FEB #%03u; Channel []; Adc []", uFebIdx);
    fvhStsFebChanAdcRawProf.push_back(new TProfile(sHistName, title, uNbChanPerFeb, -0.5, uNbChanPerFeb - 0.5));

    /// Cal Adc Distribution
    sHistName = Form("hStsFebChanAdcCal_%03u", uFebIdx);
    title     = Form("Cal. Adc distribution per channel, FEB #%03u; Channel []; Adc [e-]; Hits []", uFebIdx);
    fvhStsFebChanAdcCal.push_back(
      new TH2I(sHistName, title, uNbChanPerFeb, -0.5, uNbChanPerFeb - 0.5, 50, 0., 100000.));

    /// Cal Adc Distribution profile
    sHistName = Form("hStsFebChanAdcCalProfc_%03u", uFebIdx);
    title     = Form("Cal. Adc prodile per channel, FEB #%03u; Channel []; Adc [e-]", uFebIdx);
    fvhStsFebChanAdcCalProf.push_back(new TProfile(sHistName, title, uNbChanPerFeb, -0.5, uNbChanPerFeb - 0.5));

    /// Raw Ts Distribution
    sHistName = Form("hStsFebChanRawTs_%03u", uFebIdx);
    title     = Form("Raw Timestamp distribution per channel, FEB #%03u; Channel "
                 "[]; Ts []; Hits []",
                 uFebIdx);
    fvhStsFebChanRawTs.push_back(new TH2I(sHistName, title, uNbChanPerFeb, -0.5, uNbChanPerFeb - 0.5,
                                          stsxyter::kuHitNbTsBins, -0.5, stsxyter::kuHitNbTsBins - 0.5));

    /// Missed event flag
    sHistName = Form("hStsFebChanMissEvt_%03u", uFebIdx);
    title     = Form("Missed Event flags per channel, FEB #%03u; Channel []; Miss "
                 "Evt []; Hits []",
                 uFebIdx);
    fvhStsFebChanMissEvt.push_back(new TH2I(sHistName, title, uNbChanPerFeb, -0.5, uNbChanPerFeb - 0.5, 2, -0.5, 1.5));

    /// Missed event flag counts evolution
    sHistName = Form("hStsFebChanMissEvtEvo_%03u", uFebIdx);
    title     = Form("Missed Evt flags per second & channel in FEB #%03u; Time "
                 "[s]; Channel []; Missed Evt flags []",
                 uFebIdx);
    fvhStsFebChanMissEvtEvo.push_back(new TH2I(sHistName, title, 5, 0, 5, uNbChanPerFeb, -0.5, uNbChanPerFeb - 0.5));
    fvhStsFebChanMissEvtEvo.back()->SetCanExtend(TH1::kAllAxes);

    /// Missed event flag counts evolution
    sHistName = Form("hStsFebAsicMissEvtEvo_%03u", uFebIdx);
    title     = Form("Missed Evt flags per second & StsXyter in FEB #%03u; Time "
                 "[s]; Asic []; Missed Evt flags []",
                 uFebIdx);
    fvhStsFebAsicMissEvtEvo.push_back(new TH2I(sHistName, title, 5, 0, 5, uNbAsicsPerFeb, -0.5, uNbAsicsPerFeb - 0.5));
    fvhStsFebAsicMissEvtEvo.back()->SetCanExtend(TH1::kAllAxes);

    /// Missed event flag counts evolution
    sHistName = Form("hStsFebMissEvtEvo_%03u", uFebIdx);
    title     = Form("Missed Evt flags per second & channel in FEB #%03u; Time "
                 "[s]; Missed Evt flags []",
                 uFebIdx);
    fvhStsFebMissEvtEvo.push_back(new TH1I(sHistName, title, 5, 0, 5));
    fvhStsFebMissEvtEvo.back()->SetCanExtend(TH1::kAllAxes);

    /// Hit rates evo per channel
    sHistName = Form("hStsFebChanRateEvo_%03u", uFebIdx);
    title     = Form("Hits per second & channel in FEB #%03u; Time [s]; Channel []; Hits []", uFebIdx);
    fvhStsFebChanHitRateEvo.push_back(new TH2I(sHistName, title, 5, 0, 5, uNbChanPerFeb, -0.5, uNbChanPerFeb - 0.5));
    fvhStsFebChanHitRateEvo.back()->SetCanExtend(TH1::kAllAxes);

    /// Hit rates evo per StsXyter
    sHistName = Form("hStsFebAsicRateEvo_%03u", uFebIdx);
    title     = Form("Hits per second & StsXyter in FEB #%03u; Time [s]; Asic []; Hits []", uFebIdx);
    fvhStsFebAsicHitRateEvo.push_back(new TH2I(sHistName, title, 5, 0, 5, uNbAsicsPerFeb, -0.5, uNbAsicsPerFeb - 0.5));
    fvhStsFebAsicHitRateEvo.back()->SetCanExtend(TH1::kAllAxes);

    /// Hit rates evo per FEB
    sHistName = Form("hStsFebRateEvo_%03u", uFebIdx);
    title     = Form("Hits per second in FEB #%03u; Time [s]; Hits []", uFebIdx);
    fvhStsFebHitRateEvo.push_back(new TH1I(sHistName, title, 5, 0, 5));
    fvhStsFebHitRateEvo.back()->SetCanExtend(TH1::kAllAxes);

    /// Hit rates evo per channel, 1 minute bins, 24h
    sHistName = Form("hStsFebChanRateEvoLong_%03u", uFebIdx);
    title     = Form("Hits per second & channel in FEB #%03u; Time [min]; Channel []; Hits []", uFebIdx);
    fvhStsFebChanHitRateEvoLong.push_back(new TH2D(sHistName, title, fuLongHistoBinNb, -0.5, uAlignedLimit - 0.5,
                                                   uNbChanPerFeb, -0.5, uNbChanPerFeb - 0.5));

    /// Hit rates evo per channel, 1 minute bins, 24h
    sHistName = Form("hStsFebAsicRateEvoLong_%03u", uFebIdx);
    title     = Form("Hits per second & StsXyter in FEB #%03u; Time [min]; Asic []; Hits []", uFebIdx);
    fvhStsFebAsicHitRateEvoLong.push_back(new TH2D(sHistName, title, fuLongHistoBinNb, -0.5, uAlignedLimit - 0.5,
                                                   uNbAsicsPerFeb, -0.5, uNbAsicsPerFeb - 0.5));

    /// Hit rates evo per FEB, 1 minute bins, 24h
    sHistName = Form("hStsFebRateEvoLong_%03u", uFebIdx);
    title     = Form("Hits per second in FEB #%03u; Time [min]; Hits []", uFebIdx);
    fvhStsFebHitRateEvoLong.push_back(new TH1D(sHistName, title, fuLongHistoBinNb, -0.5, uAlignedLimit - 0.5));

    AddHistoToVector(fvhStsFebChanCntRaw[uFebIdx], "perFeb");
    AddHistoToVector(fvhStsFebChanAdcRaw[uFebIdx], "perFeb");
    AddHistoToVector(fvhStsFebChanAdcRawProf[uFebIdx], "perFeb");
    AddHistoToVector(fvhStsFebChanAdcCal[uFebIdx], "perFeb");
    AddHistoToVector(fvhStsFebChanAdcCalProf[uFebIdx], "perFeb");
    AddHistoToVector(fvhStsFebChanRawTs[uFebIdx], "perFeb");
    AddHistoToVector(fvhStsFebChanMissEvt[uFebIdx], "perFeb");
    AddHistoToVector(fvhStsFebChanMissEvtEvo[uFebIdx], "perFeb");
    AddHistoToVector(fvhStsFebAsicMissEvtEvo[uFebIdx], "perFeb");
    AddHistoToVector(fvhStsFebMissEvtEvo[uFebIdx], "perFeb");
    AddHistoToVector(fvhStsFebChanHitRateEvo[uFebIdx], "perFeb");
    AddHistoToVector(fvhStsFebAsicHitRateEvo[uFebIdx], "perFeb");
    AddHistoToVector(fvhStsFebHitRateEvo[uFebIdx], "perFeb");
    AddHistoToVector(fvhStsFebChanHitRateEvoLong[uFebIdx], "perFeb");
    AddHistoToVector(fvhStsFebAsicHitRateEvoLong[uFebIdx], "perFeb");
    AddHistoToVector(fvhStsFebHitRateEvoLong[uFebIdx], "perFeb");
  }

  /** Summary canvases **/
  const Double_t w = 2 * 400;
  const Double_t h = 3 * 200;

  // Create summary canvases per FEB
  fvcStsSumm.resize(uNbFebs);
  fvcStsSmxErr.resize(uNbFebs);
  for (UInt_t uFebIdx = 0; uFebIdx < uNbFebs; ++uFebIdx) {
    // if (kTRUE == fUnpackParSts->IsFebActive(uFebIdx)) {
    fvcStsSumm[uFebIdx] =
      new CbmQaCanvas(Form("cStsSum_%03u", uFebIdx), Form("Summary plots for FEB %03u", uFebIdx), w, h);
    AddCanvasToVector(fvcStsSumm[uFebIdx], "perFebCanvases");

    fvcStsSumm[uFebIdx]->Divide(2, 3);
  }
  return kTRUE;
}

void CbmStsUnpackMonitor::DrawCanvases()
{
  for (UInt_t uFebIdx = 0; uFebIdx < fvcStsSumm.size(); ++uFebIdx) {
    // if (kTRUE == fUnpackParSts->IsFebActive(uFebIdx)) {
    fvcStsSumm[uFebIdx]->cd(1);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogy();
    fvhStsFebChanCntRaw[uFebIdx]->DrawCopy();

    //fvcStsSumm[uFebIdx]->cd(2);
    //gPad->SetGridx();
    //gPad->SetGridy();
    //gPad->SetLogy();
    //fvhStsFebChanHitRateProf[uFebIdx]->DrawCopy("e0");

    fvcStsSumm[uFebIdx]->cd(3);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fvhStsFebChanAdcRaw[uFebIdx]->DrawCopy("colz");

    fvcStsSumm[uFebIdx]->cd(4);
    gPad->SetGridx();
    gPad->SetGridy();
    //gPad->SetLogy();
    fvhStsFebChanAdcRawProf[uFebIdx]->DrawCopy();

    fvcStsSumm[uFebIdx]->cd(5);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fvhStsFebChanHitRateEvo[uFebIdx]->DrawCopy("colz");

    fvcStsSumm[uFebIdx]->cd(6);
    gPad->SetGridx();
    gPad->SetGridy();
    //gPad->SetLogy();
    fvhStsFebChanMissEvt[uFebIdx]->DrawCopy("colz");

    // two following two are inactive as currently adc raw and cal are the same

    //fvcStsSumm[ uFebIdx ]->cd(5);
    //gPad->SetGridx();
    //gPad->SetGridy();
    //gPad->SetLogz();
    //fvhStsFebChanAdcCal[ uFebIdx ]->DrawCopy( "colz" );

    //fvcStsSumm[ uFebIdx ]->cd(6);
    //gPad->SetGridx();
    //gPad->SetGridy();
    //gPad->SetLogy();
    //fvhStsFebChanAdcCalProf[ uFebIdx ]->DrawCopy();
  }
}

Bool_t CbmStsUnpackMonitor::CreateMsComponentSizeHistos(UInt_t component)
{
  if (nullptr == fvhMsSize[component]) {
    TString sMsSizeName  = Form("MsSize_link_%02u", component);
    TString sMsSizeTitle = Form("Size of MS for nDPB of link %02u; Ms Size [bytes]", component);
    fvhMsSize[component] = new TH1F(sMsSizeName.Data(), sMsSizeTitle.Data(), 30000, 0., 30000.);
    fvhMsSize[component]->SetCanExtend(TH2::kAllAxes);
    AddHistoToVector(fvhMsSize[component], "perComponent");
  }
  if (nullptr == fvhMsSizeTime[component]) {
    TString sMsSizeName      = Form("MsSizeTime_link_%02u", component);
    TString sMsSizeTitle     = Form("Size of MS vs time for gDPB of link %02u; Time[s] ; Ms Size [bytes]", component);
    fvhMsSizeTime[component] = new TProfile(sMsSizeName.Data(), sMsSizeTitle.Data(), 15000, 0., 300.);
    fvhMsSizeTime[component]->SetCanExtend(TH2::kAllAxes);
    AddHistoToVector(fvhMsSizeTime[component], "perComponent");
  }
  return kTRUE;
}

Bool_t CbmStsUnpackMonitor::ResetMsComponentSizeHistos(UInt_t component)
{
  if (nullptr != fvhMsSize[component]) { fvhMsSize[component]->Reset(); }
  if (nullptr != fvhMsSizeTime[component]) { fvhMsSizeTime[component]->Reset(); }
  return kTRUE;
}

Bool_t CbmStsUnpackMonitor::ResetHistograms()
{
  fhDigisTimeInRun->Reset();
  fhVectorSize->Reset();
  fhVectorCapacity->Reset();
  fhMsCntEvo->Reset();
  fhMsErrorsEvo->Reset();
  fhStsAllFebsHitRateEvo->Reset();
  fhStsAllAsicsHitRateEvo->Reset();
  fhStsFebAsicHitCounts->Reset();
  fhStsStatusMessType->Reset();

  for (UInt_t uFebIdx = 0; uFebIdx < fvhStsFebChanCntRaw.size(); ++uFebIdx) {
    fvhStsFebChanCntRaw[uFebIdx]->Reset();
  }
  for (UInt_t uFebIdx = 0; uFebIdx < fvhStsFebChanAdcRaw.size(); ++uFebIdx) {
    fvhStsFebChanAdcRaw[uFebIdx]->Reset();
  }
  for (UInt_t uFebIdx = 0; uFebIdx < fvhStsFebChanAdcRawProf.size(); ++uFebIdx) {
    fvhStsFebChanAdcRawProf[uFebIdx]->Reset();
  }
  for (UInt_t uFebIdx = 0; uFebIdx < fvhStsFebChanAdcCal.size(); ++uFebIdx) {
    fvhStsFebChanAdcCal[uFebIdx]->Reset();
  }
  for (UInt_t uFebIdx = 0; uFebIdx < fvhStsFebChanAdcCalProf.size(); ++uFebIdx) {
    fvhStsFebChanAdcCalProf[uFebIdx]->Reset();
  }
  for (UInt_t uFebIdx = 0; uFebIdx < fvhStsFebChanRawTs.size(); ++uFebIdx) {
    fvhStsFebChanRawTs[uFebIdx]->Reset();
  }
  for (UInt_t uFebIdx = 0; uFebIdx < fvhStsFebChanMissEvt.size(); ++uFebIdx) {
    fvhStsFebChanMissEvt[uFebIdx]->Reset();
  }
  for (UInt_t uFebIdx = 0; uFebIdx < fvhStsFebChanMissEvtEvo.size(); ++uFebIdx) {
    fvhStsFebChanMissEvtEvo[uFebIdx]->Reset();
  }
  for (UInt_t uFebIdx = 0; uFebIdx < fvhStsFebAsicMissEvtEvo.size(); ++uFebIdx) {
    fvhStsFebAsicMissEvtEvo[uFebIdx]->Reset();
  }
  for (UInt_t uFebIdx = 0; uFebIdx < fvhStsFebMissEvtEvo.size(); ++uFebIdx) {
    fvhStsFebMissEvtEvo[uFebIdx]->Reset();
  }
  for (UInt_t uFebIdx = 0; uFebIdx < fvhStsFebChanHitRateEvo.size(); ++uFebIdx) {
    fvhStsFebChanHitRateEvo[uFebIdx]->Reset();
  }
  for (UInt_t uFebIdx = 0; uFebIdx < fvhStsFebAsicHitRateEvo.size(); ++uFebIdx) {
    fvhStsFebAsicHitRateEvo[uFebIdx]->Reset();
  }
  for (UInt_t uFebIdx = 0; uFebIdx < fvhStsFebHitRateEvo.size(); ++uFebIdx) {
    fvhStsFebHitRateEvo[uFebIdx]->Reset();
  }
  for (UInt_t uFebIdx = 0; uFebIdx < fvhStsFebChanHitRateEvoLong.size(); ++uFebIdx) {
    fvhStsFebChanHitRateEvoLong[uFebIdx]->Reset();
  }
  for (UInt_t uFebIdx = 0; uFebIdx < fvhStsFebAsicHitRateEvoLong.size(); ++uFebIdx) {
    fvhStsFebAsicHitRateEvoLong[uFebIdx]->Reset();
  }
  for (UInt_t uFebIdx = 0; uFebIdx < fvhStsFebHitRateEvoLong.size(); ++uFebIdx) {
    fvhStsFebHitRateEvoLong[uFebIdx]->Reset();
  }
  return kTRUE;
}

Bool_t CbmStsUnpackMonitor::CreateDebugHistograms(CbmMcbm2018StsPar* pUnpackPar)
{
  const UInt_t uNbAsics       = pUnpackPar->GetNrOfAsics();
  const UInt_t uNrOfDpbs      = pUnpackPar->GetNrOfDpbs();
  const UInt_t uNbChanPerAsic = pUnpackPar->GetNbChanPerAsic();
  TString sHistName {""};
  TString title {""};

  sHistName     = "hPulserMessageType";
  title         = "Nb of message for each type; Type";
  fhStsMessType = new TH1I(sHistName, title, 6, 0., 6.);
  fhStsMessType->GetXaxis()->SetBinLabel(1, "Dummy");
  fhStsMessType->GetXaxis()->SetBinLabel(2, "Hit");
  fhStsMessType->GetXaxis()->SetBinLabel(3, "TsMsb");
  fhStsMessType->GetXaxis()->SetBinLabel(4, "Epoch");
  fhStsMessType->GetXaxis()->SetBinLabel(5, "Status");
  fhStsMessType->GetXaxis()->SetBinLabel(6, "Empty");
  AddHistoToVector(fhStsMessType, "");

  sHistName           = "hPulserMessageTypePerDpb";
  title               = "Nb of message of each type for each DPB; DPB; Type";
  fhStsMessTypePerDpb = new TH2I(sHistName, title, uNrOfDpbs, 0, uNrOfDpbs, 6, 0., 6.);
  fhStsMessTypePerDpb->GetYaxis()->SetBinLabel(1, "Dummy");
  fhStsMessTypePerDpb->GetYaxis()->SetBinLabel(2, "Hit");
  fhStsMessTypePerDpb->GetYaxis()->SetBinLabel(3, "TsMsb");
  fhStsMessTypePerDpb->GetYaxis()->SetBinLabel(4, "Epoch");
  fhStsMessTypePerDpb->GetYaxis()->SetBinLabel(5, "Status");
  fhStsMessTypePerDpb->GetYaxis()->SetBinLabel(6, "Empty");
  AddHistoToVector(fhStsMessTypePerDpb, "");

  sHistName = "hStsMessTypePerElink";
  title     = "Nb of message of each type for each DPB; DPB; Type";
  fhStsMessTypePerElink =
    new TH2I(sHistName, title, uNrOfDpbs * fNrElinksPerDpb, 0, uNrOfDpbs * fNrElinksPerDpb, 6, 0., 6.);
  fhStsMessTypePerElink->GetYaxis()->SetBinLabel(1, "Dummy");
  fhStsMessTypePerElink->GetYaxis()->SetBinLabel(2, "Hit");
  fhStsMessTypePerElink->GetYaxis()->SetBinLabel(3, "TsMsb");
  fhStsMessTypePerElink->GetYaxis()->SetBinLabel(4, "Epoch");
  fhStsMessTypePerElink->GetYaxis()->SetBinLabel(5, "Status");
  fhStsMessTypePerElink->GetYaxis()->SetBinLabel(6, "Empty");
  AddHistoToVector(fhStsMessTypePerElink, "");

  sHistName            = "hStsHitsElinkPerDpb";
  title                = "Nb of hit messages per eLink for each DPB; DPB; eLink; Hits nb []";
  fhStsHitsElinkPerDpb = new TH2I(sHistName, title, uNrOfDpbs, 0, uNrOfDpbs, 42, 0., 42.);
  AddHistoToVector(fhStsHitsElinkPerDpb, "");

  sHistName        = "hStsDpbRawTsMsb";
  title            = "MSB messages for each DPB; DPB; TsMsb; Count []";
  fhStsDpbRawTsMsb = new TH2I(sHistName, title, uNrOfDpbs, 0, uNrOfDpbs, 10, 0, 10);
  fhStsDpbRawTsMsb->SetCanExtend(TH2::kAllAxes);
  AddHistoToVector(fhStsDpbRawTsMsb, "");

  sHistName          = "hStsDpbRawTsMsbSx";
  title              = "MSB SX messages for each DPB; DPB; TsMsb & 0x1F; Count []";
  fhStsDpbRawTsMsbSx = new TH2I(sHistName, title, uNrOfDpbs, 0, uNrOfDpbs, 10, 0, 10);
  fhStsDpbRawTsMsbSx->SetCanExtend(TH2::kAllAxes);
  AddHistoToVector(fhStsDpbRawTsMsbSx, "");

  sHistName           = "hStsDpbRawTsMsbDpb";
  title               = "MSB DPB messages for each DPB; DPB; TsMsb >> 5; Count []";
  fhStsDpbRawTsMsbDpb = new TH2I(sHistName, title, uNrOfDpbs, 0, uNrOfDpbs, 10, 0, 10);
  fhStsDpbRawTsMsbDpb->SetCanExtend(TH2::kAllAxes);
  AddHistoToVector(fhStsDpbRawTsMsbDpb, "");

  /// Asic plots
  /// All histos per Asic: with channels or ASIC as axis!!
  for (UInt_t uAsicIdx = 0; uAsicIdx < uNbAsics; ++uAsicIdx) {

    /// Channel counts
    sHistName = Form("hStsChanCntRaw_%03u", uAsicIdx);
    title     = Form("Hits Count per channel, Asic #%03u; Channel; Hits []", uAsicIdx);
    fvhStsChanCntRaw.push_back(new TH1I(sHistName, title, uNbChanPerAsic, -0.5, uNbChanPerAsic - 0.5));

    /// Raw Adc Distribution
    sHistName = Form("hStsChanAdcRaw_%03u", uAsicIdx);
    title     = Form("Raw Adc distribution per channel, Asic #%03u; Channel []; Adc "
                 "[]; Hits []",
                 uAsicIdx);
    fvhStsChanAdcRaw.push_back(new TH2I(sHistName, title, uNbChanPerAsic, -0.5, uNbChanPerAsic - 0.5,
                                        stsxyter::kuHitNbAdcBins, -0.5, stsxyter::kuHitNbAdcBins - 0.5));

    /// Raw Adc Distribution profile
    sHistName = Form("hStsChanAdcRawProfc_%03u", uAsicIdx);
    title     = Form("Raw Adc prodile per channel, Asic #%03u; Channel []; Adc []", uAsicIdx);
    fvhStsChanAdcRawProf.push_back(new TProfile(sHistName, title, uNbChanPerAsic, -0.5, uNbChanPerAsic - 0.5));

    /// Raw Ts Distribution
    sHistName = Form("hStsChanRawTs_%03u", uAsicIdx);
    title     = Form("Raw Timestamp distribution per channel, Asic #%03u; Channel "
                 "[]; Ts []; Hits []",
                 uAsicIdx);
    fvhStsChanRawTs.push_back(new TH2I(sHistName, title, uNbChanPerAsic, -0.5, uNbChanPerAsic - 0.5,
                                       stsxyter::kuHitNbTsBins, -0.5, stsxyter::kuHitNbTsBins - 0.5));

    /// Missed event flag
    sHistName = Form("hStsChanMissEvt_%03u", uAsicIdx);
    title     = Form("Missed Event flags per channel, Asic #%03u; Channel []; Miss "
                 "Evt []; Hits []",
                 uAsicIdx);
    fvhStsChanMissEvt.push_back(new TH2I(sHistName, title, uNbChanPerAsic, -0.5, uNbChanPerAsic - 0.5, 2, -0.5, 1.5));

    AddHistoToVector(fvhStsChanCntRaw[uAsicIdx], "perAsic");
    AddHistoToVector(fvhStsChanAdcRaw[uAsicIdx], "perAsic");
    AddHistoToVector(fvhStsChanAdcRawProf[uAsicIdx], "perAsic");
    AddHistoToVector(fvhStsChanRawTs[uAsicIdx], "perAsic");
    AddHistoToVector(fvhStsChanMissEvt[uAsicIdx], "perAsic");
  }
  return kTRUE;
}

Bool_t CbmStsUnpackMonitor::ResetDebugHistograms()
{
  fhStsMessType->Reset();
  fhStsMessTypePerDpb->Reset();
  fhStsMessTypePerElink->Reset();
  fhStsHitsElinkPerDpb->Reset();
  fhStsDpbRawTsMsb->Reset();
  fhStsDpbRawTsMsbSx->Reset();
  fhStsDpbRawTsMsbDpb->Reset();

  for (UInt_t uAsicIdx = 0; uAsicIdx < fvhStsChanCntRaw.size(); ++uAsicIdx) {
    fvhStsChanCntRaw[uAsicIdx]->Reset();
  }
  for (UInt_t uAsicIdx = 0; uAsicIdx < fvhStsChanAdcRaw.size(); ++uAsicIdx) {
    fvhStsChanAdcRaw[uAsicIdx]->Reset();
  }
  for (UInt_t uAsicIdx = 0; uAsicIdx < fvhStsChanAdcRawProf.size(); ++uAsicIdx) {
    fvhStsChanAdcRawProf[uAsicIdx]->Reset();
  }
  for (UInt_t uAsicIdx = 0; uAsicIdx < fvhStsChanRawTs.size(); ++uAsicIdx) {
    fvhStsChanRawTs[uAsicIdx]->Reset();
  }
  for (UInt_t uAsicIdx = 0; uAsicIdx < fvhStsChanMissEvt.size(); ++uAsicIdx) {
    fvhStsChanMissEvt[uAsicIdx]->Reset();
  }
  return kTRUE;
}
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
void CbmStsUnpackMonitor::FillHitMonitoringHistos(const UInt_t& uFebIdx, const UShort_t& usChan,
                                                  const UInt_t& uChanInFeb, const UShort_t& usRawAdc,
                                                  const Double_t& dCalAdc, const UShort_t& usRawTs,
                                                  const bool& isHitMissedEvts)
{
  FillStsFebChanAdcCal(uFebIdx, uChanInFeb, dCalAdc);
  FillStsFebChanAdcCalProf(uFebIdx, uChanInFeb, dCalAdc);
  FillStsFebChanCntRaw(uFebIdx, uChanInFeb);
  FillStsFebChanAdcRaw(uFebIdx, uChanInFeb, usRawAdc);
  FillStsFebChanRawTs(uFebIdx, usChan, usRawTs);
  FillStsFebChanMissEvt(uFebIdx, usChan, isHitMissedEvts);
  FillStsFebChanAdcRawProf(uFebIdx, uChanInFeb, usRawAdc);
}


// -------------------------------------------------------------------------
void CbmStsUnpackMonitor::FillHitDebugMonitoringHistos(const UInt_t& uAsicIdx, const UShort_t& usChan,
                                                       const UShort_t& usRawAdc, const UShort_t& usRawTs,
                                                       const bool& isHitMissedEvts)
{
  FillStsChanCntRaw(uAsicIdx, usChan);
  FillStsChanAdcRaw(uAsicIdx, usChan, usRawAdc);
  FillStsChanAdcRawProf(uAsicIdx, usChan, usRawAdc);
  FillStsChanRawTs(uAsicIdx, usChan, usRawTs);
  FillStsChanMissEvt(uAsicIdx, usChan, isHitMissedEvts);
}


// -------------------------------------------------------------------------
void CbmStsUnpackMonitor::FillHitEvoMonitoringHistos(const UInt_t& uFebIdx, const UInt_t& uAsicIdx,
                                                     const UInt_t& uAsicInFeb, const UInt_t& uChanInFeb,
                                                     const Double_t& dTimeSinceStartSec, const bool& isHitMissedEvts)
{
  // Fill histos with time as X axis
  FillStsFebAsicHitCounts(uFebIdx, uAsicInFeb);
  FillStsFebChanHitRateEvo(uFebIdx, dTimeSinceStartSec, uChanInFeb);
  FillStsFebAsicHitRateEvo(uFebIdx, dTimeSinceStartSec, uAsicInFeb);
  FillStsFebHitRateEvo(uFebIdx, dTimeSinceStartSec);
  FillStsAllFebsHitRateEvo(dTimeSinceStartSec, uFebIdx);
  FillStsAllAsicsHitRateEvo(dTimeSinceStartSec, uAsicIdx);
  if (isHitMissedEvts) {
    FillStsFebChanMissEvtEvo(uFebIdx, dTimeSinceStartSec, uChanInFeb);
    FillStsFebAsicMissEvtEvo(uFebIdx, dTimeSinceStartSec, uAsicInFeb);
    FillStsFebMissEvtEvo(uFebIdx, dTimeSinceStartSec);
  }
  const Double_t dTimeSinceStartMin = dTimeSinceStartSec / 60.0;
  FillStsFebHitRateEvoLong(uFebIdx, dTimeSinceStartMin);
  FillStsFebChanHitRateEvoLong(uFebIdx, dTimeSinceStartMin, uChanInFeb);
  FillStsFebAsicHitRateEvoLong(uFebIdx, dTimeSinceStartMin, uAsicInFeb);

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
bool CbmStsUnpackMonitor::ProcessDebugInfo(const stsxyter::Message& Mess, std::string& sMessPatt,
                                           std::vector<uint32_t>& vNbMessType, const UInt_t& uCurrDpbIdx)
{
  bool bError                       = false;
  const stsxyter::MessType typeMess = Mess.GetMessType();

  FillStsMessType(static_cast<uint16_t>(typeMess));
  FillStsMessTypePerDpb(uCurrDpbIdx, static_cast<uint16_t>(typeMess));
  switch (typeMess) {
    case stsxyter::MessType::Hit: {
      ++vNbMessType[0];
      sMessPatt += " H ";
      sMessPatt += ".";

      const UShort_t usElinkIdx = Mess.GetLinkIndexHitBinning();
      FillStsMessTypePerElink(usElinkIdx, static_cast<uint16_t>(typeMess));
      FillStsHitsElinkPerDpb(uCurrDpbIdx, usElinkIdx);
      break;
    }
    case stsxyter::MessType::TsMsb: {
      ++vNbMessType[1];
      sMessPatt += " T ";
      FillStsMessTypePerElink(uCurrDpbIdx * fNrElinksPerDpb, static_cast<uint16_t>(typeMess));
      break;
    }
    case stsxyter::MessType::Epoch: {
      ++vNbMessType[2];
      sMessPatt += " E ";
      FillStsMessTypePerElink(uCurrDpbIdx * fNrElinksPerDpb, static_cast<uint16_t>(typeMess));
      break;
    }
    case stsxyter::MessType::Status: {
      ++vNbMessType[3];
      sMessPatt += " S ";
      FillStsMessTypePerElink(Mess.GetLinkIndex(), static_cast<uint16_t>(typeMess));
      break;
    }
    case stsxyter::MessType::Empty: {
      ++vNbMessType[4];
      sMessPatt += " Em";
      FillStsMessTypePerElink(uCurrDpbIdx * fNrElinksPerDpb, static_cast<uint16_t>(typeMess));
      break;
    }
    case stsxyter::MessType::EndOfMs: {
      ++vNbMessType[5];
      sMessPatt += " En";
      bError = Mess.IsMsErrorFlagOn();
      FillStsMessTypePerElink(uCurrDpbIdx * fNrElinksPerDpb, static_cast<uint16_t>(typeMess));
      break;
    }
    case stsxyter::MessType::Dummy: {
      ++vNbMessType[6];
      sMessPatt += " D ";
      FillStsMessTypePerElink(uCurrDpbIdx * fNrElinksPerDpb, static_cast<uint16_t>(typeMess));
      break;
    }
    default: {
    }
  }
  return bError;
}


// ---- Init ----
Bool_t CbmStsUnpackMonitor::Init(CbmMcbm2018StsPar* parset)
{
  // Get Infos from the parset
  fNrElinksPerDpb = parset->GetNbElinkPerDpb();

  /// Trigger histo creation on all associated monitors
  CreateHistograms(parset);
  if (fDebugMode) CreateDebugHistograms(parset);

  /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
  std::vector<std::pair<TNamed*, std::string>> vHistos = GetHistoVector();

  /// Obtain vector of pointers on each canvas from the algo (+ optionally desired folder)
  std::vector<std::pair<CbmQaCanvas*, std::string>> vCanvases = GetCanvasVector();

  /// Register the histos and canvases in the HTTP server
  THttpServer* server = FairRunOnline::Instance()->GetHttpServer();
  if (nullptr != server) {
    for (UInt_t uCanvas = 0; uCanvas < vCanvases.size(); ++uCanvas) {
      server->Register(Form("/%s", vCanvases[uCanvas].second.data()), vCanvases[uCanvas].first);
    }
    for (UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto) {
      server->Register(Form("/%s", vHistos[uHisto].second.data()), vHistos[uHisto].first);
    }
    server->RegisterCommand("/Reset_UnpSts_Hist", "bMcbm2018UnpackerTaskStsResetHistos=kTRUE");
    server->Restrict("/Reset_UnpSts_Hist", "allow=admin");
  }

  return kTRUE;
}

// ---- Finish ----
void CbmStsUnpackMonitor::Finish()
{
  DrawCanvases();

  /// Obtain vector of pointers on each histo (+ optionally desired folder)
  std::vector<std::pair<TNamed*, std::string>> vHistos = GetHistoVector();

  /// Obtain vector of pointers on each canvas (+ optionally desired folder)
  std::vector<std::pair<CbmQaCanvas*, std::string>> vCanvases = GetCanvasVector();

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
}


ClassImp(CbmStsUnpackMonitor)
