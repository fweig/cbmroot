/* Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer]  */

#include "CbmTofUnpackMonitor.h"

#include "CbmFlesHistosTools.h"

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
#include <TPaveStats.h>
#include <TProfile.h>

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <cmath>

#include "CriGet4Mess001.h"

CbmTofUnpackMonitor::CbmTofUnpackMonitor(/* args */) : fvpAllHistoPointers()
{
  // Miscroslice component properties histos
  for (UInt_t component = 0; component < kuMaxNbFlibLinks; component++) {
    fvhMsSize[component]     = nullptr;
    fvhMsSizeTime[component] = nullptr;
  }
}

CbmTofUnpackMonitor::~CbmTofUnpackMonitor()
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

Bool_t CbmTofUnpackMonitor::CreateHistograms()
{
  /// Avoid name collision for the histos and canvases in Root memory
  std::string sSystem = "tof";
  if (fBmonMode) {
    //
    sSystem = "bmon";
  }

  // clang-format off
  fhGet4MessType = new TH2I(Form("%sGet4MessType", sSystem.data()),
                             "Nb of message for each type per GET4; GET4 chip # ; Type",
                              fuNbOfGet4InSyst, 0., fuNbOfGet4InSyst,
                              4, 0., 4.);
  fhGet4MessType->GetYaxis()->SetBinLabel(1, "DATA 32b");
  fhGet4MessType->GetYaxis()->SetBinLabel(2, "EPOCH");
  fhGet4MessType->GetYaxis()->SetBinLabel(3, "S.C.M");
  fhGet4MessType->GetYaxis()->SetBinLabel(4, "ERROR");
  //   fhGet4MessType->GetYaxis()->SetBinLabel( 5, "DATA 24b");
  //   fhGet4MessType->GetYaxis()->SetBinLabel( 6, "STAR Trigger");

  fhGet4EpochFlags = new TH2I(Form("%sGet4EpochFlags", sSystem.data()),
                              "Epoch flags per GET4; GET4 chip # ; Type",
                              fuNbOfGet4InSyst, 0., fuNbOfGet4InSyst,
                              4, 0., 4.);
  fhGet4EpochFlags->GetYaxis()->SetBinLabel(1, "SYNC");
  fhGet4EpochFlags->GetYaxis()->SetBinLabel(2, "Ep LOSS");
  fhGet4EpochFlags->GetYaxis()->SetBinLabel(3, "Da LOSS");
  fhGet4EpochFlags->GetYaxis()->SetBinLabel(4, "MISSMAT");

  fhGet4ScmType = new TH2I(Form("%sGet4ScmType", sSystem.data()),
                           "SC messages per GET4 channel; GET4 channel # ; SC type",
                           fuNbOfGet4InSyst, 0., fuNbOfGet4InSyst,
                           5, 0., 5.);
  fhGet4ScmType->GetYaxis()->SetBinLabel(1, "Hit Scal");
  fhGet4ScmType->GetYaxis()->SetBinLabel(2, "Deadtime");
  fhGet4ScmType->GetYaxis()->SetBinLabel(3, "SPI");
  fhGet4ScmType->GetYaxis()->SetBinLabel(4, "SEU Scal");
  fhGet4ScmType->GetYaxis()->SetBinLabel(5, "START");

  fhGet4SysMessType = new TH2I(Form("%sGet4SysMessType", sSystem.data()),
                               "Nb of system message for each type per Get4; Get4; System Type",
                               fuNbOfGet4InSyst, 0., fuNbOfGet4InSyst,
                               1 + critof001::SYS_PATTERN, 0., 1 + critof001::SYS_PATTERN);
  fhGet4SysMessType->GetYaxis()->SetBinLabel(1 + critof001::SYS_GET4_ERROR, "GET4 ERROR");
  fhGet4SysMessType->GetYaxis()->SetBinLabel(1 + critof001::SYS_GDPB_UNKWN, "UNKW GET4 MSG");
  fhGet4SysMessType->GetYaxis()->SetBinLabel(1 + critof001::SYS_GET4_SYNC_MISS, "SYS_GET4_SYNC_MISS");
  fhGet4SysMessType->GetYaxis()->SetBinLabel(1 + critof001::SYS_PATTERN, "SYS_PATTERN");

  fhGet4ErrorsType = new TH2I(Form("%sGet4ErrorsType", sSystem.data()),
                              "Error messages per GET4 channel; GET4 channel # ; Error",
                              fuNbOfGet4InSyst, 0., fuNbOfGet4InSyst,
                              22, 0., 22.);
  fhGet4ErrorsType->GetYaxis()->SetBinLabel(1, "0x00: Readout Init    ");
  fhGet4ErrorsType->GetYaxis()->SetBinLabel(2, "0x01: Sync            ");
  fhGet4ErrorsType->GetYaxis()->SetBinLabel(3, "0x02: Epoch count sync");
  fhGet4ErrorsType->GetYaxis()->SetBinLabel(4, "0x03: Epoch           ");
  fhGet4ErrorsType->GetYaxis()->SetBinLabel(5, "0x04: FIFO Write      ");
  fhGet4ErrorsType->GetYaxis()->SetBinLabel(6, "0x05: Lost event      ");
  fhGet4ErrorsType->GetYaxis()->SetBinLabel(7, "0x06: Channel state   ");
  fhGet4ErrorsType->GetYaxis()->SetBinLabel(8, "0x07: Token Ring state");
  fhGet4ErrorsType->GetYaxis()->SetBinLabel(9, "0x08: Token           ");
  fhGet4ErrorsType->GetYaxis()->SetBinLabel(10, "0x09: Error Readout   ");
  fhGet4ErrorsType->GetYaxis()->SetBinLabel(11, "0x0a: SPI             ");
  fhGet4ErrorsType->GetYaxis()->SetBinLabel(12, "0x0b: DLL Lock error  ");  // <- From GET4 v1.2
  fhGet4ErrorsType->GetYaxis()->SetBinLabel(13, "0x0c: DLL Reset invoc.");  // <- From GET4 v1.2
  fhGet4ErrorsType->GetYaxis()->SetBinLabel(14, "0x11: Overwrite       ");
  fhGet4ErrorsType->GetYaxis()->SetBinLabel(15, "0x12: ToT out of range");
  fhGet4ErrorsType->GetYaxis()->SetBinLabel(16, "0x13: Event Discarded ");
  fhGet4ErrorsType->GetYaxis()->SetBinLabel(17, "0x14: Add. Rising edge");  // <- From GET4 v1.3
  fhGet4ErrorsType->GetYaxis()->SetBinLabel(18, "0x15: Unpaired Falling");  // <- From GET4 v1.3
  fhGet4ErrorsType->GetYaxis()->SetBinLabel(19, "0x16: Sequence error  ");  // <- From GET4 v1.3
  fhGet4ErrorsType->GetYaxis()->SetBinLabel(20, "0x7f: Unknown         ");
  fhGet4ErrorsType->GetYaxis()->SetBinLabel(21, "Corrupt/unsuprtd error");

  /// Add pointers to the vector with all histo for access by steering class
  std::string sFolder = "Get4InSys";
  AddHistoToVector(fhGet4MessType, sFolder);
  AddHistoToVector(fhGet4EpochFlags, sFolder);
  AddHistoToVector(fhGet4ScmType, sFolder);
  AddHistoToVector(fhGet4SysMessType, sFolder);
  AddHistoToVector(fhGet4ErrorsType, sFolder);

  for (UInt_t uComp = 0; uComp < fuNbOfComps; ++uComp) {
    UInt_t uCompIndex = uComp;

    std::string sFolderComp     = Form("c%02u", uCompIndex);

    /// ---> Per GET4 in Component
    fvhCompGet4MessType.push_back(
      new TH2I(Form("%sCompGet4MessType_c%02u", sSystem.data(), uComp),
               Form("Nb of message for each type per GET4 in Comp %02u; GET4 chip # ; Type", uCompIndex),
               fuNbOfGet4PerComp, 0., fuNbOfGet4PerComp,
               4, 0., 4.));
    fvhCompGet4MessType[uComp]->GetYaxis()->SetBinLabel(1, "DATA 32b");
    fvhCompGet4MessType[uComp]->GetYaxis()->SetBinLabel(2, "EPOCH");
    fvhCompGet4MessType[uComp]->GetYaxis()->SetBinLabel(3, "S.C. M");
    fvhCompGet4MessType[uComp]->GetYaxis()->SetBinLabel(4, "ERROR");

    fvhCompGet4ChScm.push_back(
      new TH2I(Form("%sCompGet4ChanScm_c%02u", sSystem.data(), uComp),
               Form("SC messages per GET4 channel in Comp %02u; GET4 channel # ; SC type", uCompIndex),
               2 * fuNbOfChannelsPerComp, 0., fuNbOfChannelsPerComp,
               5, 0., 5.));
    fvhCompGet4ChScm[uComp]->GetYaxis()->SetBinLabel(1, "Hit Scal");
    fvhCompGet4ChScm[uComp]->GetYaxis()->SetBinLabel(2, "Deadtime");
    fvhCompGet4ChScm[uComp]->GetYaxis()->SetBinLabel(3, "SPI");
    fvhCompGet4ChScm[uComp]->GetYaxis()->SetBinLabel(4, "SEU Scal");
    fvhCompGet4ChScm[uComp]->GetYaxis()->SetBinLabel(5, "START");

    fvhCompGet4ChErrors.push_back(
      new TH2I(Form("%sCompGet4ChanErrors_c%02u", sSystem.data(), uComp),
               Form("Error messages per GET4 channel in Comp %02u; GET4 channel # ; Error", uCompIndex),
               fuNbOfChannelsPerComp, 0., fuNbOfChannelsPerComp,
               22, 0., 22.));
    fvhCompGet4ChErrors[uComp]->GetYaxis()->SetBinLabel(1, "0x00: Readout Init    ");
    fvhCompGet4ChErrors[uComp]->GetYaxis()->SetBinLabel(2, "0x01: Sync            ");
    fvhCompGet4ChErrors[uComp]->GetYaxis()->SetBinLabel(3, "0x02: Epoch count sync");
    fvhCompGet4ChErrors[uComp]->GetYaxis()->SetBinLabel(4, "0x03: Epoch           ");
    fvhCompGet4ChErrors[uComp]->GetYaxis()->SetBinLabel(5, "0x04: FIFO Write      ");
    fvhCompGet4ChErrors[uComp]->GetYaxis()->SetBinLabel(6, "0x05: Lost event      ");
    fvhCompGet4ChErrors[uComp]->GetYaxis()->SetBinLabel(7, "0x06: Channel state   ");
    fvhCompGet4ChErrors[uComp]->GetYaxis()->SetBinLabel(8, "0x07: Token Ring state");
    fvhCompGet4ChErrors[uComp]->GetYaxis()->SetBinLabel(9, "0x08: Token           ");
    fvhCompGet4ChErrors[uComp]->GetYaxis()->SetBinLabel(10, "0x09: Error Readout   ");
    fvhCompGet4ChErrors[uComp]->GetYaxis()->SetBinLabel(11, "0x0a: SPI             ");
    fvhCompGet4ChErrors[uComp]->GetYaxis()->SetBinLabel(12, "0x0b: DLL Lock error  ");  // <- From GET4 v1.2
    fvhCompGet4ChErrors[uComp]->GetYaxis()->SetBinLabel(13, "0x0c: DLL Reset invoc.");  // <- From GET4 v1.2
    fvhCompGet4ChErrors[uComp]->GetYaxis()->SetBinLabel(14, "0x11: Overwrite       ");  // <- From GET4 v1.0 to 1.3
    fvhCompGet4ChErrors[uComp]->GetYaxis()->SetBinLabel(15, "0x12: ToT out of range");
    fvhCompGet4ChErrors[uComp]->GetYaxis()->SetBinLabel(16, "0x13: Event Discarded ");
    fvhCompGet4ChErrors[uComp]->GetYaxis()->SetBinLabel(17, "0x14: Add. Rising edge");  // <- From GET4 v1.3
    fvhCompGet4ChErrors[uComp]->GetYaxis()->SetBinLabel(18, "0x15: Unpaired Falling");  // <- From GET4 v1.3
    fvhCompGet4ChErrors[uComp]->GetYaxis()->SetBinLabel(19, "0x16: Sequence error  ");  // <- From GET4 v1.3
    fvhCompGet4ChErrors[uComp]->GetYaxis()->SetBinLabel(20, "0x17: Epoch Overflow  ");  // <- From GET4 v2.0
    fvhCompGet4ChErrors[uComp]->GetYaxis()->SetBinLabel(21, "0x7f: Unknown         ");
    fvhCompGet4ChErrors[uComp]->GetYaxis()->SetBinLabel(22, "Corrupt/unsuprtd error");

    /// ---> Per raw channel in Component

    fvhCompRawChCount.push_back(new TH1I(Form("%sCompRawChCount_c%02u", sSystem.data(), uCompIndex),
                                         Form("Channel counts comp. %02u raw; Channel; Hits",
                                              uCompIndex),
                                         fuNbOfChannelsPerComp, 0, fuNbOfChannelsPerComp));

    fvhCompRawChRate.push_back(new TH2D(Form("%sCompRawChRate_c%02u", sSystem.data(), uCompIndex),
                                        Form("Raw channel rate comp. %02u; Time in run [s]; Channel; "
                                             "Rate [1/s]",
                                             uCompIndex),
                                        fuHistoryHistoSize, 0, fuHistoryHistoSize,
                                        fuNbOfChannelsPerComp, 0, fuNbOfChannelsPerComp));

    fvhCompRawChTot.push_back(new TH2I(Form("%sCompRawChTot_c%02u", sSystem.data(), uCompIndex),
                                       Form("Raw TOT comp. %02u, raw channel; Channel; TOT [bin]", uCompIndex),
                                       fuNbOfChannelsPerComp, 0, fuNbOfChannelsPerComp,
                                       256, 0, 256));

    /// ---> Per remapped (PADI) channel in Component
    fvhCompRemapChCount.push_back(new TH1I(Form("%sCompRemapChCount_c%02u", sSystem.data(), uCompIndex),
                                           Form("PADI Channel counts comp. %02u, remapped; PADI channel; Hits",
                                                uCompIndex),
                                           fuNbOfChannelsPerComp, 0, fuNbOfChannelsPerComp));

    fvhCompRemapChRate.push_back(new TH2D(Form("%sCompRemapChRate_c%02u", sSystem.data(), uCompIndex),
                                          Form("PADI channel rate comp. %02u, remapped; Time in run [s]; PADI channel; "
                                               "Rate [1/s]",
                                               uCompIndex),
                                          fuHistoryHistoSize, 0, fuHistoryHistoSize,
                                          fuNbOfChannelsPerComp, 0, fuNbOfChannelsPerComp));

    fvhCompRemapChTot.push_back(new TH2I(Form("%sCompRemapChTot_c%02u", sSystem.data(), uCompIndex),
                                         Form("Raw TOT comp. %02u, remapped; PADI channel; TOT [bin]", uCompIndex),
                                         fuNbOfChannelsPerComp, 0, fuNbOfChannelsPerComp,
                                         256, 0, 256));

    // clang-format on

    /// Add pointers to the vector with all histo for access by steering class
    /// Per GET4 in gDPB
    AddHistoToVector(fvhCompGet4MessType[uComp], sFolderComp);
    AddHistoToVector(fvhCompGet4ChScm[uComp], sFolderComp);
    AddHistoToVector(fvhCompGet4ChErrors[uComp], sFolderComp);
    /// ---> Per raw channel in Component
    AddHistoToVector(fvhCompRawChCount[uComp], sFolderComp);
    AddHistoToVector(fvhCompRawChRate[uComp], sFolderComp);
    AddHistoToVector(fvhCompRawChTot[uComp], sFolderComp);
    /// ---> Per remapped (PADI) channel in Component
    AddHistoToVector(fvhCompRemapChCount[uComp], sFolderComp);
    AddHistoToVector(fvhCompRemapChRate[uComp], sFolderComp);
    AddHistoToVector(fvhCompRemapChTot[uComp], sFolderComp);
  }  // for( UInt_t uComp = 0; uComp < fuNbOfComps; ++uComp )

  return kTRUE;
}

void CbmTofUnpackMonitor::DrawCanvases()
{
  /// Avoid name collision for the histos and canvases in Root memory
  std::string sSystem = "tof";
  if (fBmonMode) {
    //
    sSystem = "bmon";
  }

  /// General summary: Messages types per GET4 (index in system)
  fcSummaryGet4s =
    new TCanvas(Form("c%sSummaryGet4s", sSystem.data()), Form("GET4s message stats, %s", sSystem.data()));
  fcSummaryGet4s->Divide(3, 2);

  fcSummaryGet4s->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhGet4MessType->Draw("colz");

  fcSummaryGet4s->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhGet4EpochFlags->Draw("colz");

  fcSummaryGet4s->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhGet4ScmType->Draw("colz");

  fcSummaryGet4s->cd(4);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhGet4SysMessType->Draw("colz");

  fcSummaryGet4s->cd(5);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhGet4ErrorsType->Draw("colz");

  std::string sFolder = "canvases";
  AddCanvasToVector(fcSummaryGet4s, sFolder);
  ///----------------------------------------------------------------------------------------------------------------///

  for (UInt_t uComp = 0; uComp < fuNbOfComps; ++uComp) {

    fvcSumComp.push_back(new TCanvas(Form("c%sSumComp%02u", sSystem.data(), uComp),
                                     Form("Component %2u summary, %s", uComp, sSystem.data())));
    fvcSumComp[uComp]->Divide(3, 3);

    /// ---> Per GET4 in Component
    fvcSumComp[uComp]->cd(1);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fvhCompGet4MessType[uComp]->Draw("colz");

    fvcSumComp[uComp]->cd(2);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fvhCompGet4ChScm[uComp]->Draw("colz");

    fvcSumComp[uComp]->cd(3);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fvhCompGet4ChErrors[uComp]->Draw("colz");

    /// ---> Per raw channel in Component
    fvcSumComp[uComp]->cd(4);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fvhCompRawChCount[uComp]->Draw("colz");

    fvcSumComp[uComp]->cd(5);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fvhCompRawChRate[uComp]->Draw("colz");

    fvcSumComp[uComp]->cd(6);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fvhCompRawChTot[uComp]->Draw("colz");

    /// ---> Per remapped (PADI) channel in Component
    fvcSumComp[uComp]->cd(7);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fvhCompRemapChCount[uComp]->Draw("colz");

    fvcSumComp[uComp]->cd(8);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fvhCompRemapChRate[uComp]->Draw("colz");

    fvcSumComp[uComp]->cd(9);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogz();
    fvhCompRemapChTot[uComp]->Draw("colz");

    AddCanvasToVector(fvcSumComp[uComp], sFolder);
  }
}

Bool_t CbmTofUnpackMonitor::CreateMsComponentSizeHistos(UInt_t component)
{
  if (nullptr == fvhMsSize[component]) {
    TString sMsSizeName  = Form("MsSize_c%02u", component);
    TString sMsSizeTitle = Form("Size of MS for component %02u; Ms Size [bytes]", component);
    fvhMsSize[component] = new TH1F(sMsSizeName.Data(), sMsSizeTitle.Data(), 30000, 0., 30000.);
    fvhMsSize[component]->SetCanExtend(TH2::kAllAxes);
    AddHistoToVector(fvhMsSize[component], "MsSzComp");
  }
  if (nullptr == fvhMsSizeTime[component]) {
    TString sMsSizeName      = Form("MsSizeTime_c%02u", component);
    TString sMsSizeTitle     = Form("Size of MS vs time for component %02u; Time[s] ; Ms Size [bytes]", component);
    fvhMsSizeTime[component] = new TProfile(sMsSizeName.Data(), sMsSizeTitle.Data(), 15000, 0., 300.);
    fvhMsSizeTime[component]->SetCanExtend(TH2::kAllAxes);
    AddHistoToVector(fvhMsSizeTime[component], "MsSzComp");
  }
  return kTRUE;
}

Bool_t CbmTofUnpackMonitor::ResetMsComponentSizeHistos(UInt_t component)
{
  if (nullptr != fvhMsSize[component]) { fvhMsSize[component]->Reset(); }
  if (nullptr != fvhMsSizeTime[component]) { fvhMsSizeTime[component]->Reset(); }
  return kTRUE;
}

Bool_t CbmTofUnpackMonitor::ResetHistograms()
{
  fhGet4MessType->Reset();
  fhGet4ScmType->Reset();
  fhGet4ErrorsType->Reset();
  fhGet4EpochFlags->Reset();
  /// ---> Per GET4 in Component
  for (UInt_t uComp = 0; uComp < fuNbOfComps; ++uComp) {
    fvhCompGet4MessType[uComp]->Reset();
    fvhCompGet4ChScm[uComp]->Reset();
    fvhCompGet4ChErrors[uComp]->Reset();
    /// ---> Per raw channel in Component
    fvhCompRawChCount[uComp]->Reset();
    fvhCompRawChRate[uComp]->Reset();
    fvhCompRawChTot[uComp]->Reset();
    /// ---> Per remapped (PADI) channel in Component
    fvhCompRemapChCount[uComp]->Reset();
    fvhCompRemapChRate[uComp]->Reset();
    fvhCompRemapChTot[uComp]->Reset();
  }

  return kTRUE;
}

Bool_t CbmTofUnpackMonitor::CreateBmonHistograms()
{
  /// Logarithmic bining
  uint32_t iNbBinsLog = 0;
  /// Parameters are NbDecadesLog, NbStepsDecade, NbSubStepsInStep
  std::vector<double> dBinsLogVector = GenerateLogBinArray(4, 9, 1, iNbBinsLog);
  double* dBinsLog                   = dBinsLogVector.data();
  //   double * dBinsLog = GenerateLogBinArray( 4, 9, 1, iNbBinsLog );

  // clang-format off

  fhBmonCompMapAll = new TH1I("hBmonCompMapAll", "Map of hits on Bmon detector; Comp.; Hits Count []",
                             fuNbOfComps, -0.5, fuNbOfComps - 0.5);
  fhBmonCompGet4 = new TH2I("hBmonCompGet4", "Map of hits on Bmon detector; Comp.; GET4; Counts []",
                           fuNbOfComps*80, -0.5, fuNbOfComps*80 - 0.5,
                           2*kuNbChanBmon, -0.5, 2*kuNbChanBmon - 0.5);
  fhBmonGet4Map = new TH1I("hBmonGet4Map", "Map of hits on Bmon detector; GET4; Hits Count []",
                          fuNbOfComps*80, -0.5, fuNbOfComps*80 - 0.5);
  fhBmonGet4MapEvo     = new TH2I("hBmonGet4MapEvo",
                                  "Map of hits on Bmon detector vs time in run; GET4; "
                                  "Time in run [s]; Hits Count []",
                                  fuHistoryHistoSize, 0, fuHistoryHistoSize,
                                  fuNbOfComps*80, -0.5, fuNbOfComps*80 - 0.5);
  fhBmonChannelMapAll    = new TH1I("hChannelMapAll", "Map of hits on Bmon detector; Strip; Hits Count []",
                                    kuNbChanBmon, -0.5, kuNbChanBmon - 0.5);
  fhBmonChannelTotAll    = new TH2I("hChannelTotAll", "Tot of hits on Bmon detector per channel; Strip; Tot; Hits Count []",
                                    kuNbChanBmon, -0.5, kuNbChanBmon - 0.5, 256, -0.5, 255.5);
  fhBmonHitMapEvoAll     = new TH2I("hBmonHitMapEvoAll",
                                     "Map of hits on Bmon detector vs time in run; Chan; "
                                     "Time in run [s]; Hits Count []",
                                     kuNbChanBmon, -0.5, kuNbChanBmon - 0.5,
                                     fuHistoryHistoSize, 0, fuHistoryHistoSize);
  fhBmonHitTotEvoAll     = new TH2I("hBmonHitTotEvoAll",
                                    "Evolution of TOT in Bmon detector vs time in run; Time "
                                    "in run [s]; TOT [ bin ]; Hits Count []",
                                    fuHistoryHistoSize, 0, fuHistoryHistoSize, 256, -0.5, 255.5);
  fhBmonChanHitMapAll    = new TH1D("fhBmonChanHitMapAll", "Map of hits on Bmon detector; Strip; Hits Count []",
                                    kuNbChanBmon, -0.5, kuNbChanBmon - 0.5);
  fhBmonChanHitMapEvoAll = new TH2I("hBmonChanHitMapEvoAll",
                                    "Map of hits on Bmon detector vs time in run; "
                                    "Strip; Time in run [s]; Hits Count []",
                                    kuNbChanBmon, 0., kuNbChanBmon, fuHistoryHistoSize, 0, fuHistoryHistoSize);


  fhBmonCompMap = new TH1I("hBmonCompMap", "Map of hits on Bmon detector; Comp.; Hits Count []",
                          fuNbOfComps, -0.5, fuNbOfComps - 0.5);
  fhBmonChannelMap    = new TH1I("hChannelMap", "Map of hits on Bmon detector; Strip; Hits Count []",
                                 kuNbChanBmon, -0.5, kuNbChanBmon - 0.5);
  fhBmonHitMapEvo     = new TH2I("hBmonHitMapEvo",
                         "Map of hits on Bmon detector vs time in run; Chan; "
                         "Time in run [s]; Hits Count []",
                         kuNbChanBmon, -0.5, kuNbChanBmon - 0.5, fuHistoryHistoSize, 0, fuHistoryHistoSize);
  fhBmonHitTotEvo     = new TH2I("hBmonHitTotEvo",
                         "Evolution of TOT in Bmon detector vs time in run; Time "
                         "in run [s]; TOT [ bin ]; Hits Count []",
                         fuHistoryHistoSize, 0, fuHistoryHistoSize, 256, -0.5, 255.5);
  fhBmonChanHitMap    = new TH1D("fhBmonChanHitMap", "Map of hits on Bmon detector; Strip; Hits Count []",
                                 kuNbChanBmon, -0.5, kuNbChanBmon - 0.5);
  fhBmonChanHitMapEvo = new TH2I("hBmonChanHitMapEvo",
                             "Map of hits on Bmon detector vs time in run; "
                             "Strip; Time in run [s]; Hits Count []",
                             kuNbChanBmon, 0., kuNbChanBmon, fuHistoryHistoSize, 0, fuHistoryHistoSize);
  for (UInt_t uSpill = 0; uSpill < kuNbSpillPlots; uSpill++) {
    fvhBmonCompMapSpill.push_back(
      new TH1I(Form("hBmonCompMapSpill%02u", uSpill),
               Form("Map of hits on Bmon detector in current spill %02u; Comp.; Hits Count []", uSpill),
               fuNbOfComps, -0.5, fuNbOfComps - 0.5));
    fvhBmonChannelMapSpill.push_back(new TH1I(Form("hBmonChannelMapSpill%02u", uSpill),
                                          Form("Map of hits on Bmon detector in current spill %02u; Strip; "
                                               "Hits Count []",
                                               uSpill),
                                          kuNbChanBmon, -0.5, kuNbChanBmon - 0.5));
  }  // for( UInt_t uSpill = 0; uSpill < kuNbSpillPlots; uSpill ++)
  fhBmonHitsPerSpill = new TH1I("hBmonHitsPerSpill", "Hit count per spill; Spill; Hits Count []", 2000, 0., 2000);

  fhBmonMsgCntEvo   = new TH1I("hBmonMsgCntEvo",
                         "Evolution of Hit & error msgs counts vs time in run; "
                         "Time in run [s]; Msgs Count []",
                         fuHistoryHistoSize, 0, fuHistoryHistoSize);
  fhBmonHitCntEvo   = new TH1I("hBmonHitCntEvo",
                               "Evolution of Hit counts vs time in run; Time in run [s]; Hits Count []",
                               fuHistoryHistoSize, 0, fuHistoryHistoSize);
  fhBmonErrorCntEvo = new TH1I("hBmonErrorCntEvo",
                               "Evolution of Error counts vs time in run; Time in run [s]; Error Count []",
                               fuHistoryHistoSize, 0, fuHistoryHistoSize);
  fhBmonLostEvtCntEvo = new TH1I("hBmonLostEvtCntEvo",
                             "Evolution of LostEvent counts vs time in run; "
                             "Time in run [s]; LostEvent Count []",
                             fuHistoryHistoSize, 0, fuHistoryHistoSize);

  fhBmonErrorFractEvo   = new TProfile("hBmonErrorFractEvo",
                                 "Evolution of Error Fraction vs time in run; "
                                 "Time in run [s]; Error Fract []",
                                 fuHistoryHistoSize, 0, fuHistoryHistoSize);
  fhBmonLostEvtFractEvo = new TProfile("hBmonLostEvtFractEvo",
                                   "Evolution of LostEvent Fraction vs time in "
                                   "run; Time in run [s]; LostEvent Fract []",
                                   fuHistoryHistoSize, 0, fuHistoryHistoSize);

  fhBmonMsgCntPerMsEvo     = new TH2I("hBmonMsgCntPerMsEvo",
                              "Evolution of Hit & error msgs counts, per MS vs time in run; "
                              "Time in run [s]; Hits Count/MS []; MS",
                              fuHistoryHistoSize, 0, fuHistoryHistoSize, iNbBinsLog, dBinsLog);
  fhBmonHitCntPerMsEvo     = new TH2I("hBmonHitCntPerMsEvo",
                              "Evolution of Hit counts, per MS vs time in run; "
                              "Time in run [s]; Hits Count/MS []; MS",
                              fuHistoryHistoSize, 0, fuHistoryHistoSize, iNbBinsLog, dBinsLog);
  fhBmonErrorCntPerMsEvo   = new TH2I("hBmonErrorCntPerMsEvo",
                                "Evolution of Error counts, per MS vs time in "
                                "run; Time in run [s]; Error Count/MS []; MS",
                                fuHistoryHistoSize, 0, fuHistoryHistoSize, iNbBinsLog, dBinsLog);
  fhBmonLostEvtCntPerMsEvo = new TH2I("hBmonLostEvtCntPerMsEvo",
                                  "Evolution of LostEvent, per MS counts vs time in run; Time in "
                                  "run [s]; LostEvent Count/MS []; MS",
                                  fuHistoryHistoSize, 0, fuHistoryHistoSize, iNbBinsLog, dBinsLog);

  fhBmonErrorFractPerMsEvo   = new TH2I("hBmonErrorFractPerMsEvo",
                                  "Evolution of Error Fraction, per MS vs time in run; Time in run "
                                  "[s]; Error Fract/MS []; MS",
                                  fuHistoryHistoSize, 0, fuHistoryHistoSize, 1000, 0, 1);
  fhBmonLostEvtFractPerMsEvo = new TH2I("hBmonLostEvtFractPerMsEvo",
                                    "Evolution of LostEvent Fraction, per MS vs time in run; Time in "
                                    "run [s]; LostEvent Fract/MS []; MS",
                                    fuHistoryHistoSize, 0, fuHistoryHistoSize, 1000, 0, 1);

  fhBmonChannelMapPulser = new TH1I("fhBmonChannelMapPulser", "Map of pulser hits on Bmon detector; Chan; Hits Count []",
                                kuNbChanBmon, 0., kuNbChanBmon);
  fhBmonHitMapEvoPulser  = new TH2I("fhBmonHitMapEvoPulser",
                               "Map of hits on Bmon detector vs time in run; "
                               "Chan; Time in run [s]; Hits Count []",
                               kuNbChanBmon, 0., kuNbChanBmon, fuHistoryHistoSize, 0, fuHistoryHistoSize);
  // clang-format on

  /// Add pointers to the vector with all histo for access by steering class
  std::string sFolder = "All";
  AddHistoToVector(fhBmonCompMapAll, sFolder);
  AddHistoToVector(fhBmonCompGet4, sFolder);
  AddHistoToVector(fhBmonGet4Map, sFolder);
  AddHistoToVector(fhBmonGet4MapEvo, sFolder);
  AddHistoToVector(fhBmonChannelMapAll, sFolder);
  AddHistoToVector(fhBmonChannelTotAll, sFolder);
  AddHistoToVector(fhBmonHitMapEvoAll, sFolder);
  AddHistoToVector(fhBmonHitTotEvoAll, sFolder);
  AddHistoToVector(fhBmonChanHitMapAll, sFolder);
  AddHistoToVector(fhBmonChanHitMapEvoAll, sFolder);

  sFolder = "NoPulser";
  AddHistoToVector(fhBmonCompMap, sFolder);
  AddHistoToVector(fhBmonChannelMap, sFolder);
  AddHistoToVector(fhBmonHitMapEvo, sFolder);
  AddHistoToVector(fhBmonHitTotEvo, sFolder);
  AddHistoToVector(fhBmonChanHitMap, sFolder);
  AddHistoToVector(fhBmonChanHitMapEvo, sFolder);
  sFolder = "Spills";
  for (UInt_t uSpill = 0; uSpill < kuNbSpillPlots; uSpill++) {
    AddHistoToVector(fvhBmonCompMapSpill[uSpill], sFolder);
    AddHistoToVector(fvhBmonChannelMapSpill[uSpill], sFolder);
  }  // for( UInt_t uSpill = 0; uSpill < kuNbSpillPlots; uSpill ++)
  AddHistoToVector(fhBmonHitsPerSpill, sFolder);

  sFolder = "GlobRates";
  AddHistoToVector(fhBmonMsgCntEvo, sFolder);
  AddHistoToVector(fhBmonHitCntEvo, sFolder);
  AddHistoToVector(fhBmonErrorCntEvo, sFolder);
  AddHistoToVector(fhBmonLostEvtCntEvo, sFolder);

  AddHistoToVector(fhBmonErrorFractEvo, sFolder);
  AddHistoToVector(fhBmonLostEvtFractEvo, sFolder);

  sFolder = "GlobRatesMs";
  AddHistoToVector(fhBmonMsgCntPerMsEvo, sFolder);
  AddHistoToVector(fhBmonHitCntPerMsEvo, sFolder);
  AddHistoToVector(fhBmonErrorCntPerMsEvo, sFolder);
  AddHistoToVector(fhBmonLostEvtCntPerMsEvo, sFolder);
  AddHistoToVector(fhBmonErrorFractPerMsEvo, sFolder);
  AddHistoToVector(fhBmonLostEvtFractPerMsEvo, sFolder);

  sFolder = "Pulser";
  AddHistoToVector(fhBmonChannelMapPulser, sFolder);
  AddHistoToVector(fhBmonHitMapEvoPulser, sFolder);

  /*******************************************************************/
  sFolder = "RatePerChan";
  for (UInt_t uChan = 0; uChan < kuNbChanBmon; ++uChan) {
    // clang-format off
    fvhBmonMsgCntEvoChan[uChan]      = new TH1I(Form("hBmonMsgCntEvoChan%02u", uChan),
                                       Form("Evolution of Messages counts vs time in run for channel "
                                            "%02u; Time in run [s]; Messages Count []",
                                            uChan),
                                       fuHistoryHistoSize, 0, fuHistoryHistoSize);
    fvhBmonMsgCntPerMsEvoChan[uChan] = new TH2I(Form("hBmonMsgCntPerMsEvoChan%02u", uChan),
                                            Form("Evolution of Hit counts per MS vs time in run for channel "
                                                 "%02u; Time in run [s]; Hits Count/MS []; MS",
                                                 uChan),
                                            fuHistoryHistoSize, 0, fuHistoryHistoSize, iNbBinsLog, dBinsLog);

    fvhBmonHitCntEvoChan[uChan]      = new TH1I(Form("hBmonHitCntEvoChan%02u", uChan),
                                       Form("Evolution of Hit counts vs time in run for channel %02u; "
                                            "Time in run [s]; Hits Count []",
                                            uChan),
                                       fuHistoryHistoSize, 0, fuHistoryHistoSize);
    fvhBmonHitCntPerMsEvoChan[uChan] = new TH2I(Form("hBmonHitCntPerMsEvoChan%02u", uChan),
                                            Form("Evolution of Hit counts per MS vs time in run for channel "
                                                 "%02u; Time in run [s]; Hits Count/MS []; MS",
                                                 uChan),
                                            fuHistoryHistoSize, 0, fuHistoryHistoSize, iNbBinsLog, dBinsLog);

    fvhBmonErrorCntEvoChan[uChan]      = new TH1I(Form("hBmonErrorCntEvoChan%02u", uChan),
                                         Form("Evolution of Error counts vs time in run for channel "
                                              "%02u; Time in run [s]; Error Count []",
                                              uChan),
                                         fuHistoryHistoSize, 0, fuHistoryHistoSize);
    fvhBmonErrorCntPerMsEvoChan[uChan] = new TH2I(Form("hBmonErrorCntPerMsEvoChan%02u", uChan),
                                              Form("Evolution of Error counts per MS vs time in run for "
                                                   "channel %02u; Time in run [s]; Error Count/MS []; MS",
                                                   uChan),
                                              fuHistoryHistoSize, 0, fuHistoryHistoSize, iNbBinsLog, dBinsLog);

    fvhBmonEvtLostCntEvoChan[uChan]      = new TH1I(Form("hBmonEvtLostCntEvoChan%02u", uChan),
                                           Form("Evolution of LostEvent counts vs time in run for channel "
                                                "%02u; Time in run [s]; LostEvent Count []",
                                                uChan),
                                           fuHistoryHistoSize, 0, fuHistoryHistoSize);
    fvhBmonEvtLostCntPerMsEvoChan[uChan] = new TH2I(Form("hBmonEvtLostCntPerMsEvoChan%02u", uChan),
                                                Form("Evolution of LostEvent counts per MS vs time in run for "
                                                     "channel %02u; Time in run [s]; LostEvent Count/MS []; MS",
                                                     uChan),
                                                fuHistoryHistoSize, 0, fuHistoryHistoSize, iNbBinsLog, dBinsLog);

    fvhBmonErrorFractEvoChan[uChan]      = new TProfile(Form("hBmonErrorFractEvoChan%02u", uChan),
                                               Form("Evolution of Error Fraction vs time in run for "
                                                    "channel %02u; Time in run [s]; Error Fract []",
                                                    uChan),
                                               fuHistoryHistoSize, 0, fuHistoryHistoSize);
    fvhBmonErrorFractPerMsEvoChan[uChan] = new TH2I(Form("hBmonErrorFractPerMsEvoChan%02u", uChan),
                                                Form("Evolution of Error Fraction, per MS vs time in run for "
                                                     "channel %02u; Time in run [s]; Error Fract/MS []; MS",
                                                     uChan),
                                                fuHistoryHistoSize, 0, fuHistoryHistoSize, 1000, 0, 1);

    fvhBmonEvtLostFractEvoChan[uChan] = new TProfile(Form("hBmonEvtLostFractEvoChan%02u", uChan),
                                                 Form("Evolution of LostEvent Fraction vs time in run for "
                                                      "channel %02u; Time in run [s]; LostEvent Fract []",
                                                      uChan),
                                                 fuHistoryHistoSize, 0, fuHistoryHistoSize);
    fvhBmonEvtLostFractPerMsEvoChan[uChan] =
      new TH2I(Form("hBmonEvtLostFractPerMsEvoChan%02u", uChan),
               Form("Evolution of LostEvent Fraction, per MS vs time in run for channel "
                    "%02u; Time in run [s]; LostEvent Fract/MS []; MS",
                    uChan),
               fuHistoryHistoSize, 0, fuHistoryHistoSize, 1000, 0, 1);
    // clang-format on

    /// Add pointers to the vector with all histo for access by steering class
    AddHistoToVector(fvhBmonMsgCntEvoChan[uChan], sFolder);
    AddHistoToVector(fvhBmonMsgCntPerMsEvoChan[uChan], sFolder);
    AddHistoToVector(fvhBmonHitCntEvoChan[uChan], sFolder);
    AddHistoToVector(fvhBmonHitCntPerMsEvoChan[uChan], sFolder);
    AddHistoToVector(fvhBmonErrorCntEvoChan[uChan], sFolder);
    AddHistoToVector(fvhBmonErrorCntPerMsEvoChan[uChan], sFolder);
    AddHistoToVector(fvhBmonEvtLostCntEvoChan[uChan], sFolder);
    AddHistoToVector(fvhBmonEvtLostCntPerMsEvoChan[uChan], sFolder);
    AddHistoToVector(fvhBmonErrorFractEvoChan[uChan], sFolder);
    AddHistoToVector(fvhBmonErrorFractPerMsEvoChan[uChan], sFolder);
    AddHistoToVector(fvhBmonEvtLostFractEvoChan[uChan], sFolder);
    AddHistoToVector(fvhBmonEvtLostFractPerMsEvoChan[uChan], sFolder);
  }  // for( UInt_t uChan = 0; uChan < kuNbChanBmon; ++uChan )

  return kTRUE;
}

Bool_t CbmTofUnpackMonitor::ResetBmonHistograms(Bool_t bResetTime)
{
  for (UInt_t uChan = 0; uChan < kuNbChanBmon; ++uChan) {
    fvhBmonMsgCntEvoChan[uChan]->Reset();
    fvhBmonMsgCntPerMsEvoChan[uChan]->Reset();

    fvhBmonHitCntEvoChan[uChan]->Reset();
    fvhBmonHitCntPerMsEvoChan[uChan]->Reset();

    fvhBmonErrorCntEvoChan[uChan]->Reset();
    fvhBmonErrorCntPerMsEvoChan[uChan]->Reset();

    fvhBmonEvtLostCntEvoChan[uChan]->Reset();
    fvhBmonEvtLostCntPerMsEvoChan[uChan]->Reset();

    fvhBmonErrorFractEvoChan[uChan]->Reset();
    fvhBmonErrorFractPerMsEvoChan[uChan]->Reset();

    fvhBmonEvtLostFractEvoChan[uChan]->Reset();
    fvhBmonEvtLostFractPerMsEvoChan[uChan]->Reset();
  }  // for( UInt_t uChan = 0; uChan < kuNbChanBmon; ++uChan )

  fhBmonCompMap->Reset();
  fhBmonCompGet4->Reset();
  fhBmonGet4Map->Reset();
  fhBmonGet4MapEvo->Reset();
  fhBmonChannelMapAll->Reset();
  fhBmonChannelTotAll->Reset();
  fhBmonHitMapEvoAll->Reset();
  fhBmonHitTotEvoAll->Reset();
  fhBmonChanHitMapAll->Reset();
  fhBmonChanHitMapEvoAll->Reset();

  fhBmonCompMap->Reset();
  fhBmonCompMap->Reset();
  fhBmonChannelMap->Reset();
  fhBmonHitMapEvo->Reset();
  fhBmonHitTotEvo->Reset();
  fhBmonChanHitMap->Reset();
  fhBmonChanHitMapEvo->Reset();
  for (UInt_t uSpill = 0; uSpill < kuNbSpillPlots; uSpill++) {
    fvhBmonCompMapSpill[uSpill]->Reset();
    fvhBmonChannelMapSpill[uSpill]->Reset();
  }  // for( UInt_t uSpill = 0; uSpill < kuNbSpillPlots; uSpill ++)
  fhBmonHitsPerSpill->Reset();

  fhBmonMsgCntEvo->Reset();
  fhBmonHitCntEvo->Reset();
  fhBmonErrorCntEvo->Reset();

  fhBmonErrorFractEvo->Reset();
  fhBmonLostEvtFractEvo->Reset();

  fhBmonMsgCntPerMsEvo->Reset();
  fhBmonHitCntPerMsEvo->Reset();
  fhBmonErrorCntPerMsEvo->Reset();
  fhBmonLostEvtCntPerMsEvo->Reset();
  fhBmonErrorFractPerMsEvo->Reset();
  fhBmonLostEvtFractPerMsEvo->Reset();

  fhBmonChannelMapPulser->Reset();
  fhBmonHitMapEvoPulser->Reset();

  if (kTRUE == bResetTime) {
    /// Also reset the Start time for the evolution plots!
    fdStartTime = -1.0;

    fuCurrentSpillIdx  = 0;
    fuCurrentSpillPlot = 0;
  }  // if( kTRUE == bResetTime )

  return kTRUE;
}

void CbmTofUnpackMonitor::DrawBmonCanvases()
{
  std::string sFolder = "canvases";

  /*******************************************************************/
  /// General summary: Hit maps, Hit rate vs time in run, error fraction vs time un run
  fcBmonSummary = new TCanvas("cBmonSummary", "Hit maps, Hit rate, Error fraction");
  fcBmonSummary->Divide(2, 2);

  fcBmonSummary->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fhBmonChannelMap->Draw();

  fcBmonSummary->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhBmonHitMapEvo->Draw("colz");

  fcBmonSummary->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fhBmonHitCntEvo->Draw();

  fcBmonSummary->cd(4);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhBmonErrorFractEvo->Draw("hist");

  AddCanvasToVector(fcBmonSummary, sFolder);
  /*******************************************************************/

  /*******************************************************************/
  /// General summary after mapping: Hit maps, Hit rate vs time in run, error fraction vs time un run
  fcBmonSummaryMap = new TCanvas("cBmonSummaryMap", "Hit maps, Hit rate, Error fraction");
  fcBmonSummaryMap->Divide(2, 2);

  fcBmonSummaryMap->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fhBmonChanHitMap->Draw();

  fcBmonSummaryMap->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhBmonChanHitMapEvo->Draw("colz");

  fcBmonSummaryMap->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fhBmonHitCntEvo->Draw();

  fcBmonSummaryMap->cd(4);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhBmonErrorFractEvo->Draw("hist");

  AddCanvasToVector(fcBmonSummaryMap, sFolder);
  /*******************************************************************/

  /*******************************************************************/
  /// Map of hits over Bmon detector and same vs time in run
  fcBmonHitMaps = new TCanvas("cBmonHitMaps", "Hit maps");
  fcBmonHitMaps->Divide(2);

  fcBmonHitMaps->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fhBmonChannelMap->Draw();

  fcBmonHitMaps->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  fhBmonHitMapEvo->Draw("colz");

  AddCanvasToVector(fcBmonHitMaps, sFolder);
  /*******************************************************************/

  /*******************************************************************/
  /// General summary: Hit maps, Hit rate vs time in run, error fraction vs time un run
  fcBmonGenCntsPerMs =
    new TCanvas("cBmonGenCntsPerMs", "Messages and hit cnt per MS, Error and Evt Loss Fract. per MS ");
  fcBmonGenCntsPerMs->Divide(2, 2);

  fcBmonGenCntsPerMs->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  gPad->SetLogz();
  fhBmonMsgCntPerMsEvo->Draw("colz");

  fcBmonGenCntsPerMs->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  gPad->SetLogz();
  fhBmonHitCntPerMsEvo->Draw("colz");

  fcBmonGenCntsPerMs->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  gPad->SetLogz();
  fhBmonErrorFractPerMsEvo->Draw("colz");

  fcBmonGenCntsPerMs->cd(4);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  gPad->SetLogz();
  fhBmonLostEvtFractPerMsEvo->Draw("colz");

  AddCanvasToVector(fcBmonGenCntsPerMs, sFolder);
  /*******************************************************************/

  /*******************************************************************/
  /// General summary: Hit maps, Hit rate vs time in run, error fraction vs time un run
  fcBmonSpillCounts = new TCanvas("cBmonSpillCounts", "Counts per spill, last 5 spills including current one");
  fcBmonSpillCounts->Divide(1, kuNbSpillPlots);

  for (UInt_t uSpill = 0; uSpill < kuNbSpillPlots; uSpill++) {
    fcBmonSpillCounts->cd(1 + uSpill);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogy();
    //      fvhChannelMapSpill[ uSpill ]->SetStats( kTRUE );
    fvhBmonChannelMapSpill[uSpill]->Draw();
    gPad->Update();
    TPaveStats* st = (TPaveStats*) fvhBmonChannelMapSpill[uSpill]->FindObject("stats");
    st->SetOptStat(10);
    st->SetX1NDC(0.25);
    st->SetX2NDC(0.95);
    st->SetY1NDC(0.90);
    st->SetY2NDC(0.95);
  }  // for( UInt_t uSpill = 0; uSpill < kuNbSpillPlots; uSpill ++)

  AddCanvasToVector(fcBmonSpillCounts, sFolder);
  /*******************************************************************/

  /*******************************************************************/
  /// General summary: Hit maps, Hit rate vs time in run, error fraction vs time un run
  fcBmonSpillCountsHori = new TCanvas("cBmonSpillCountsHori", "Counts per spill, last 5 spills including current one");
  fcBmonSpillCountsHori->Divide(kuNbSpillPlots);

  for (UInt_t uSpill = 0; uSpill < kuNbSpillPlots; uSpill++) {
    fcBmonSpillCountsHori->cd(1 + uSpill);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogy();
    fvhBmonChannelMapSpill[uSpill]->Draw();
    gPad->Update();
    TPaveStats* st = (TPaveStats*) fvhBmonChannelMapSpill[uSpill]->FindObject("stats");
    st->SetOptStat(10);
    st->SetX1NDC(0.25);
    st->SetX2NDC(0.95);
    st->SetY1NDC(0.90);
    st->SetY2NDC(0.95);
  }  // for( UInt_t uSpill = 0; uSpill < kuNbSpillPlots; uSpill ++)

  AddCanvasToVector(fcBmonSpillCountsHori, sFolder);
  /*******************************************************************/

  /*******************************************************************/
  /// General summary: Hit maps, Hit rate vs time in run, error fraction vs time un run
  fcBmonSpillCompCountsHori =
    new TCanvas("cBmonSpillCompCountsHori", "Counts in Comp. per spill, last 5 spills including current one");
  fcBmonSpillCompCountsHori->Divide(kuNbSpillPlots);

  for (UInt_t uSpill = 0; uSpill < kuNbSpillPlots; uSpill++) {
    fcBmonSpillCompCountsHori->cd(1 + uSpill);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogy();
    fvhBmonCompMapSpill[uSpill]->Draw();
    gPad->Update();
    TPaveStats* st = (TPaveStats*) fvhBmonCompMapSpill[uSpill]->FindObject("stats");
    st->SetOptStat(110);
    st->SetX1NDC(0.25);
    st->SetX2NDC(0.95);
    st->SetY1NDC(0.90);
    st->SetY2NDC(0.95);
  }  // for( UInt_t uSpill = 0; uSpill < kuNbSpillPlots; uSpill ++)

  AddCanvasToVector(fcBmonSpillCompCountsHori, sFolder);
  /*******************************************************************/
}
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
void CbmTofUnpackMonitor::FillHitMonitoringHistos(const double_t& dMsTime, const uint32_t& uCurrCompIdx,
                                                  const uint32_t& uGet4Id, const uint32_t& uRawCh,
                                                  const uint32_t& uRemapCh, const uint32_t& uTot)
{
  if (-1 == fdStartTime) {
    /// Initialize Start time for evolution plots
    fdStartTime = dMsTime;
  }

  /// ---> Per GET4 in system
  uint32_t uGet4InSys = uGet4Id + uCurrCompIdx * fuNbOfGet4PerComp;
  fhGet4MessType->Fill(uGet4InSys, 0);
  /// ---> Per GET4 in Component
  fvhCompGet4MessType[uCurrCompIdx]->Fill(uGet4Id, 0);
  /// ---> Per raw channel in Component
  fvhCompRawChCount[uCurrCompIdx]->Fill(uRawCh);
  fvhCompRawChRate[uCurrCompIdx]->Fill(dMsTime - fdStartTime, uRawCh);
  fvhCompRawChTot[uCurrCompIdx]->Fill(uRawCh, uTot);
  /// ---> Per remapped (PADI) channel in Component
  fvhCompRemapChCount[uCurrCompIdx]->Fill(uRemapCh);
  fvhCompRemapChRate[uCurrCompIdx]->Fill(dMsTime - fdStartTime, uRemapCh);
  fvhCompRemapChTot[uCurrCompIdx]->Fill(uRemapCh, uTot);
}
void CbmTofUnpackMonitor::FillEpochMonitoringHistos(const uint32_t& uCurrCompIdx, const uint32_t& uGet4Id,
                                                    const bool& bSyncFlag,   // mess.getGdpbEpSync
                                                    const bool& bDataLoss,   // mess.getGdpbEpDataLoss()
                                                    const bool& bEpochLoss,  // mess.getGdpbEpEpochLoss()
                                                    const bool& bMissmMatch  // mess.getGdpbEpMissmatch()
)
{
  /// ---> Per GET4 in system
  uint32_t uGet4InSys = uGet4Id + uCurrCompIdx * fuNbOfGet4PerComp;
  fhGet4MessType->Fill(uGet4InSys, 1);
  if (bSyncFlag) fhGet4EpochFlags->Fill(uGet4InSys, 0);
  if (bDataLoss) fhGet4EpochFlags->Fill(uGet4InSys, 1);
  if (bEpochLoss) fhGet4EpochFlags->Fill(uGet4InSys, 2);
  if (bMissmMatch) fhGet4EpochFlags->Fill(uGet4InSys, 3);
  /// ---> Per GET4 in Component
  fvhCompGet4MessType[uCurrCompIdx]->Fill(uGet4Id, 1);
}
void CbmTofUnpackMonitor::FillScmMonitoringHistos(const uint32_t& uCurrCompIdx, const uint32_t& uGet4Id,
                                                  const uint32_t& uCh,    // mess.getGdpbSlcChan()
                                                  const uint32_t& uEdge,  // mess.getGdpbSlcEdge()
                                                  const uint32_t& uType   // mess.getGdpbSlcType()
)
{
  /// ---> Per GET4 in system
  uint32_t uGet4InSys = uGet4Id + uCurrCompIdx * fuNbOfGet4PerComp;
  fhGet4MessType->Fill(uGet4InSys, 2);

  /// ---> Per GET4 in Component
  fvhCompGet4MessType[uCurrCompIdx]->Fill(uGet4Id, 2);
  double_t uChInComp = uCh + 0.5 * uEdge + uGet4Id * fuNbOfChannelsPerGet4;
  if (uType == critof001::GET4_32B_SLC_START_SEU && 0 == uEdge) {
    /// Start/SEU + Start flag is set
    fhGet4ScmType->Fill(uGet4InSys, uType + 1);
    fvhCompGet4ChScm[uCurrCompIdx]->Fill(uChInComp, uType + 1);
  }  // if (uType == critof001::GET4_32B_SLC_START_SEU && 0 == uEdge)
  else {
    fhGet4ScmType->Fill(uGet4InSys, uType);
    fvhCompGet4ChScm[uCurrCompIdx]->Fill(uChInComp, uType);
  }  // else of if (uType == critof001::GET4_32B_SLC_START_SEU && 0 == uEdge)
}
void CbmTofUnpackMonitor::FillSysMonitoringHistos(const uint32_t& uCurrCompIdx, const uint32_t& uGet4Id,
                                                  const uint32_t& uType  // mess.getGdpbSysSubType()
)
{
  /// ---> Per GET4 in system
  uint32_t uGet4InSys = uGet4Id + uCurrCompIdx * fuNbOfGet4PerComp;
  fhGet4SysMessType->Fill(uGet4InSys, uType);
}
void CbmTofUnpackMonitor::FillErrMonitoringHistos(const uint32_t& uCurrCompIdx, const uint32_t& uGet4Id,
                                                  const uint32_t& uCh,   // mess.getGdpbSysErrChanId()
                                                  const uint32_t& uType  // mess.getGdpbSysErrData()
)
{
  /// ---> Per GET4 in system
  uint32_t uGet4InSys = uGet4Id + uCurrCompIdx * fuNbOfGet4PerComp;
  fhGet4MessType->Fill(uGet4InSys, 3);
  /// ---> Per GET4 in Component
  fvhCompGet4MessType[uCurrCompIdx]->Fill(uGet4Id, 3);

  uint32_t uChInComp = uCh + uGet4Id * fuNbOfChannelsPerGet4;
  switch (uType) {
    case critof001::GET4_V2X_ERR_READ_INIT: {
      fhGet4ErrorsType->Fill(uGet4InSys, 0);
      fvhCompGet4ChErrors[uCurrCompIdx]->Fill(uChInComp, 0);
      break;
    }  // case critof001::GET4_V2X_ERR_READ_INIT:
    case critof001::GET4_V2X_ERR_SYNC: {
      fhGet4ErrorsType->Fill(uGet4InSys, 1);
      fvhCompGet4ChErrors[uCurrCompIdx]->Fill(uChInComp, 1);
      break;
    }  // case critof001::GET4_V2X_ERR_SYNC:
    case critof001::GET4_V2X_ERR_EP_CNT_SYNC: {
      fhGet4ErrorsType->Fill(uGet4InSys, 2);
      fvhCompGet4ChErrors[uCurrCompIdx]->Fill(uChInComp, 2);
      break;
    }  // case critof001::GET4_V2X_ERR_EP_CNT_SYNC:
    case critof001::GET4_V2X_ERR_EP: {
      fhGet4ErrorsType->Fill(uGet4InSys, 3);
      fvhCompGet4ChErrors[uCurrCompIdx]->Fill(uChInComp, 3);
      break;
    }  // case critof001::GET4_V2X_ERR_EP:
    case critof001::GET4_V2X_ERR_FIFO_WRITE: {
      fhGet4ErrorsType->Fill(uGet4InSys, 4);
      fvhCompGet4ChErrors[uCurrCompIdx]->Fill(uChInComp, 4);
      break;
    }  // case critof001::GET4_V2X_ERR_FIFO_WRITE:
    case critof001::GET4_V2X_ERR_LOST_EVT: {
      fhGet4ErrorsType->Fill(uGet4InSys, 5);
      fvhCompGet4ChErrors[uCurrCompIdx]->Fill(uChInComp, 5);
      break;
    }  // case critof001::GET4_V2X_ERR_LOST_EVT:
    case critof001::GET4_V2X_ERR_CHAN_STATE: {
      fhGet4ErrorsType->Fill(uGet4InSys, 6);
      fvhCompGet4ChErrors[uCurrCompIdx]->Fill(uChInComp, 6);
      break;
    }  // case critof001::GET4_V2X_ERR_CHAN_STATE:
    case critof001::GET4_V2X_ERR_TOK_RING_ST: {
      fhGet4ErrorsType->Fill(uGet4InSys, 7);
      fvhCompGet4ChErrors[uCurrCompIdx]->Fill(uChInComp, 7);
      break;
    }  // case critof001::GET4_V2X_ERR_TOK_RING_ST:
    case critof001::GET4_V2X_ERR_TOKEN: {
      fhGet4ErrorsType->Fill(uGet4InSys, 8);
      fvhCompGet4ChErrors[uCurrCompIdx]->Fill(uChInComp, 8);
      break;
    }  // case critof001::GET4_V2X_ERR_TOKEN:
    case critof001::GET4_V2X_ERR_READOUT_ERR: {
      fhGet4ErrorsType->Fill(uGet4InSys, 9);
      fvhCompGet4ChErrors[uCurrCompIdx]->Fill(uChInComp, 9);
      break;
    }  // case critof001::GET4_V2X_ERR_READOUT_ERR:
    case critof001::GET4_V2X_ERR_SPI: {
      fhGet4ErrorsType->Fill(uGet4InSys, 10);
      fvhCompGet4ChErrors[uCurrCompIdx]->Fill(uChInComp, 10);
      break;
    }  // case critof001::GET4_V2X_ERR_SPI:
    case critof001::GET4_V2X_ERR_DLL_LOCK: {
      fhGet4ErrorsType->Fill(uGet4InSys, 11);
      fvhCompGet4ChErrors[uCurrCompIdx]->Fill(uChInComp, 11);
      break;
    }  // case critof001::GET4_V2X_ERR_DLL_LOCK:
    case critof001::GET4_V2X_ERR_DLL_RESET: {
      fhGet4ErrorsType->Fill(uGet4InSys, 12);
      fvhCompGet4ChErrors[uCurrCompIdx]->Fill(uChInComp, 12);
      break;
    }  // case critof001::GET4_V2X_ERR_DLL_RESET:
    case critof001::GET4_V2X_ERR_TOT_OVERWRT: {
      fhGet4ErrorsType->Fill(uGet4InSys, 13);
      fvhCompGet4ChErrors[uCurrCompIdx]->Fill(uChInComp, 13);
      break;
    }  // case critof001::GET4_V2X_ERR_TOT_OVERWRT:
    case critof001::GET4_V2X_ERR_TOT_RANGE: {
      fhGet4ErrorsType->Fill(uGet4InSys, 14);
      fvhCompGet4ChErrors[uCurrCompIdx]->Fill(uChInComp, 14);
      break;
    }  // case critof001::GET4_V2X_ERR_TOT_RANGE:
    case critof001::GET4_V2X_ERR_EVT_DISCARD: {
      fhGet4ErrorsType->Fill(uGet4InSys, 15);
      fvhCompGet4ChErrors[uCurrCompIdx]->Fill(uChInComp, 15);
      break;
    }  // case critof001::GET4_V2X_ERR_EVT_DISCARD:
    case critof001::GET4_V2X_ERR_ADD_RIS_EDG: {
      fhGet4ErrorsType->Fill(uGet4InSys, 16);
      fvhCompGet4ChErrors[uCurrCompIdx]->Fill(uChInComp, 16);
      break;
    }  // case critof001::GET4_V2X_ERR_ADD_RIS_EDG:
    case critof001::GET4_V2X_ERR_UNPAIR_FALL: {
      fhGet4ErrorsType->Fill(uGet4InSys, 17);
      fvhCompGet4ChErrors[uCurrCompIdx]->Fill(uChInComp, 17);
      break;
    }  // case critof001::GET4_V2X_ERR_UNPAIR_FALL:
    case critof001::GET4_V2X_ERR_SEQUENCE_ER: {
      fhGet4ErrorsType->Fill(uGet4InSys, 18);
      fvhCompGet4ChErrors[uCurrCompIdx]->Fill(uChInComp, 18);
      break;
    }  // case critof001::GET4_V2X_ERR_SEQUENCE_ER:
    case critof001::GET4_V2X_ERR_EPOCH_OVERF: {
      fhGet4ErrorsType->Fill(uGet4InSys, 19);
      fvhCompGet4ChErrors[uCurrCompIdx]->Fill(uChInComp, 19);
      break;
    }  // case critof001::GET4_V2X_ERR_EPOCH_OVERF:
    case critof001::GET4_V2X_ERR_UNKNOWN: {
      fhGet4ErrorsType->Fill(uGet4InSys, 20);
      fvhCompGet4ChErrors[uCurrCompIdx]->Fill(uChInComp, 20);
      break;
    }  // case critof001::GET4_V2X_ERR_UNKNOWN:
    default: // Corrupt error or not yet supported error
      {
      fhGet4ErrorsType->Fill(uGet4InSys, 21);
      fvhCompGet4ChErrors[uCurrCompIdx]->Fill(uChInComp, 21);
      break;
    }  //
  }    // Switch( mess.getGdpbSysErrData() )
}


// -------------------------------------------------------------------------
void CbmTofUnpackMonitor::CheckBmonSpillLimits(const double_t& dMsTime)
{
  if (-1 == fdStartTime) {
    /// Initialize Start time for evolution plots
    fdStartTime = dMsTime;
  }

  /// Spill Detection
  /// Check only every second
  if (fdSpillCheckInterval < dMsTime - fdBmonLastInterTime) {
    /// Spill Off detection
    if (fbSpillOn && fuBmonCountsLastInter < fuOffSpillCountLimit
        && fuBmonNonPulserCountsLastInter < fuOffSpillCountLimitNonPulser) {
      fbSpillOn = kFALSE;
      fuCurrentSpillIdx++;
      fuCurrentSpillPlot = (fuCurrentSpillPlot + 1) % kuNbSpillPlots;
      fdStartTimeSpill   = dMsTime;
      fvhBmonCompMapSpill[fuCurrentSpillPlot]->Reset();
      fvhBmonChannelMapSpill[fuCurrentSpillPlot]->Reset();
    }  // if( fbSpillOn && fuCountsLastInter < fuOffSpillCountLimit && same for non pulser)
    else if (fuOffSpillCountLimit < fuBmonCountsLastInter) {
      fbSpillOn = kTRUE;
    }

    //    LOG(debug) << Form("%6llu %6.4f %9u %9u %2d", fulCurrentTsIdx, dMsTime - fdLastInterTime, fuBmonCountsLastInter,
    //                       fuBmonNonPulserCountsLastInter, fuCurrentSpillIdx);

    fuBmonCountsLastInter          = 0;
    fuBmonNonPulserCountsLastInter = 0;
    fdBmonLastInterTime            = dMsTime;
  }  // if( fdSpillCheckInterval < dMsTime - fdLastInterTime )
}
void CbmTofUnpackMonitor::FillHitBmonMonitoringHistos(const double_t& dMsTime, const uint32_t& uCurrCompIdx,
                                                      const uint32_t& uGet4Id, const uint32_t& uTot)
{
  if (-1 == fdStartTime) {
    /// Initialize Start time for evolution plots
    fdStartTime = dMsTime;
  }

  /// 2022 mapping: Y[0-3] on c0, Y[4-7] on c1, X[0-3] on c2, X[4-7] on c3
  /// Y not cabled for diamond but pulser there
  UInt_t uChannelBmon = (uGet4Id / 8) + 4 * uCurrCompIdx;

  fhBmonGet4Map->Fill(uGet4Id + 80 * uCurrCompIdx);
  fhBmonGet4MapEvo->Fill(dMsTime - fdStartTime, uGet4Id + 80 * uCurrCompIdx);
  fhBmonCompGet4->Fill(uGet4Id + 80 * uCurrCompIdx, uChannelBmon);
  if (kuNbChanBmon <= uChannelBmon) return;

  fhBmonCompMapAll->Fill(uCurrCompIdx);
  fhBmonChannelMapAll->Fill(uChannelBmon);
  fhBmonChannelTotAll->Fill(uChannelBmon, uTot);
  fhBmonHitMapEvoAll->Fill(uChannelBmon, dMsTime - fdStartTime);
  fhBmonHitTotEvoAll->Fill(dMsTime - fdStartTime, uTot);
  fhBmonChanHitMapAll->Fill(fuBmonChanMap[uChannelBmon]);
  fhBmonChanHitMapEvoAll->Fill(fuBmonChanMap[uChannelBmon], dMsTime - fdStartTime);

  /// Spill detection
  fuBmonCountsLastInter++;

  fhBmonErrorFractEvo->Fill(dMsTime - fdStartTime, 0.0);
  fhBmonLostEvtFractEvo->Fill(dMsTime - fdStartTime, 0.0);
  fvhBmonErrorFractEvoChan[uChannelBmon]->Fill(dMsTime - fdStartTime, 0.0);
  fvhBmonEvtLostFractEvoChan[uChannelBmon]->Fill(dMsTime - fdStartTime, 0.0);

  fhBmonMsgCntEvo->Fill(dMsTime - fdStartTime);
  fhBmonHitCntEvo->Fill(dMsTime - fdStartTime);

  fvhBmonHitCntEvoChan[uChannelBmon]->Fill(dMsTime - fdStartTime);

  fvuBmonHitCntChanMs[uChannelBmon]++;

  /// Do not fill the pulser hits to keep counts low for channel 0 of each Board
  /// For now hard-code the pulser channel
  if ((0 == uGet4Id / 8) && (fuMinTotPulser <= uTot) && (uTot <= fuMaxTotPulser)) {
    fhBmonChannelMapPulser->Fill(uChannelBmon);
    fhBmonHitMapEvoPulser->Fill(uChannelBmon, dMsTime - fdStartTime);
  }  // if ( (0 == uGet4Id / 8) && (fuMinTotPulser <= uTot) && (uTot <= fuMaxTotPulser) )
  else {
    fuBmonNonPulserCountsLastInter++;

    fhBmonCompMap->Fill(uCurrCompIdx);
    fhBmonChannelMap->Fill(uChannelBmon);
    fhBmonHitMapEvo->Fill(uChannelBmon, dMsTime - fdStartTime);
    fhBmonHitTotEvo->Fill(dMsTime - fdStartTime, uTot);
    fhBmonChanHitMap->Fill(fuBmonChanMap[uChannelBmon]);
    fhBmonChanHitMapEvo->Fill(fuBmonChanMap[uChannelBmon], dMsTime - fdStartTime);

    fvhBmonCompMapSpill[fuCurrentSpillPlot]->Fill(uCurrCompIdx);
    fvhBmonChannelMapSpill[fuCurrentSpillPlot]->Fill(fuBmonChanMap[uChannelBmon]);
    fhBmonHitsPerSpill->Fill(fuCurrentSpillIdx);
  }  // else of if ( (0 == uGet4Id / 8) && (fuMinTotPulser <= uTot) && (uTot <= fuMaxTotPulser) )
}

void CbmTofUnpackMonitor::FillErrBmonMonitoringHistos(const double_t& dMsTime, const uint32_t& uCurrCompIdx,
                                                      const uint32_t& uGet4Id, const bool& bErrEvtLost)
{
  /// 2022 mapping:
  /// Y[0-3] on c0, Y[4-7] on c1, X[0-3] on c2, X[4-7] on c3
  /// Y not cabled for diamond but pulser there
  UInt_t uChannelBmon = (uGet4Id / 8) + 4 * uCurrCompIdx;

  fhBmonErrorFractEvo->Fill(dMsTime - fdStartTime, 0.0);
  fhBmonLostEvtFractEvo->Fill(dMsTime - fdStartTime, 0.0);
  fvhBmonErrorFractEvoChan[uChannelBmon]->Fill(dMsTime - fdStartTime, 0.0);
  fvhBmonEvtLostFractEvoChan[uChannelBmon]->Fill(dMsTime - fdStartTime, 0.0);

  fhBmonMsgCntEvo->Fill(dMsTime - fdStartTime);
  fhBmonErrorCntEvo->Fill(dMsTime - fdStartTime);
  fhBmonErrorFractEvo->Fill(dMsTime - fdStartTime, 1.0);

  fvhBmonErrorCntEvoChan[uChannelBmon]->Fill(dMsTime - fdStartTime);
  fvhBmonErrorFractEvoChan[uChannelBmon]->Fill(dMsTime - fdStartTime, 1.0);

  fvuBmonErrorCntChanMs[uChannelBmon]++;
  if (bErrEvtLost) {
    fhBmonLostEvtCntEvo->Fill(dMsTime - fdStartTime);
    fhBmonLostEvtFractEvo->Fill(dMsTime - fdStartTime, 1.0);

    fvhBmonEvtLostCntEvoChan[uChannelBmon]->Fill(dMsTime - fdStartTime);
    fvhBmonEvtLostFractEvoChan[uChannelBmon]->Fill(dMsTime - fdStartTime, 1.0);

    fvuBmonEvtLostCntChanMs[uChannelBmon]++;
  }  // if( gdpbv100::GET4_V2X_ERR_LOST_EVT == mess.getGdpbSysErrData() )
}

// -------------------------------------------------------------------------

// ---- Init ----
Bool_t CbmTofUnpackMonitor::Init(CbmMcbm2018TofPar* parset)
{
  // Get Infos from the parset
  fUnpackPar = parset;

  fuNbOfComps = fUnpackPar->GetNrOfGdpbs();
  /// Other constants are defined for the AFCK/DPB in the current parameter class!!!
  // FIXME: Start using again the parameter class accessors once a new CRI oriented class is brought into use
  fuNbOfGet4PerComp     = 80;
  fuNbOfChannelsPerGet4 = 4;
  fuNbOfChannelsPerComp = fuNbOfGet4PerComp * fuNbOfChannelsPerGet4;
  fuNbOfGet4InSyst      = fuNbOfComps * fuNbOfGet4PerComp;

  /// Trigger histo creation on all associated monitors
  CreateHistograms();
  DrawCanvases();
  if (fBmonMode) {
    CreateBmonHistograms();
    DrawBmonCanvases();
  }

  /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
  std::vector<std::pair<TNamed*, std::string>> vHistos = GetHistoVector();

  /// Obtain vector of pointers on each canvas from the algo (+ optionally desired folder)
  std::vector<std::pair<TCanvas*, std::string>> vCanvases = GetCanvasVector();

  /// Register the histos and canvases in the HTTP server
  std::string sSystem = "tof";
  if (fBmonMode) {
    //
    sSystem = "bmon";
  }
  THttpServer* server = FairRunOnline::Instance()->GetHttpServer();
  if (nullptr != server) {
    for (UInt_t uCanvas = 0; uCanvas < vCanvases.size(); ++uCanvas) {
      server->Register(Form("/%s/%s", sSystem.data(), vCanvases[uCanvas].second.data()), vCanvases[uCanvas].first);
    }
    for (UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto) {
      server->Register(Form("/%s/%s", sSystem.data(), vHistos[uHisto].second.data()), vHistos[uHisto].first);
    }
    /*
    server->RegisterCommand(Form("/Reset_%s_Hist", sSystem.data()), "bMcbm2018UnpackerTaskStsResetHistos=kTRUE");
    server->Restrict("/Reset_UnpSts_Hist", "allow=admin");
*/
  }

  return kTRUE;
}

// ---- Finish ----
void CbmTofUnpackMonitor::Finish()
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


ClassImp(CbmTofUnpackMonitor)
