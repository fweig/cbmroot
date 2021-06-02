/** @file CbmMcbm2018MonitorMcbmRate.cxx
  * @copyright Copyright (C) 2018-2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pierre-Alain Loizeau [committer] **/

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                   CbmMcbm2018MonitorMcbmRate                      -----
// -----                Created 11/05/18  by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmMcbm2018MonitorMcbmRate.h"

#include "CbmFormatMsHeaderPrintout.h"

// Data

// CbmRoot
#include "CbmMcbm2018StsPar.h"
#include "CbmMcbm2018TofPar.h"

// FairRoot
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"
#include <Logger.h>

// Root
#include "TClonesArray.h"
#include "TF1.h"
#include "THttpServer.h"
#include "TMath.h"
#include "TROOT.h"
#include "TRandom.h"
#include "TString.h"
#include "TStyle.h"
#include <TFile.h>

// C++11

// C/C++
#include <iomanip>
#include <iostream>

#include <stdint.h>
/*
Bool_t bMcbm2018ResetSync = kFALSE;
Bool_t bMcbm2018WriteSync = kFALSE;
*/
/*
Bool_t bMcbm2018ResetSync = kFALSE;
Bool_t bMcbm2018WriteSync = kFALSE;
*/
/*
Bool_t bMcbm2018SyncResetHistosTof = kFALSE;
Bool_t bMcbm2018SyncSaveHistosTof  = kFALSE;
Bool_t bMcbm2018SyncUpdateZoomedFit = kFALSE;
*/

CbmMcbm2018MonitorMcbmRate::CbmMcbm2018MonitorMcbmRate()
  : CbmMcbmUnpack()
  , fvMsComponentsListSts()
  , fvMsComponentsListTof()
  , fuNbCoreMsPerTs(0)
  , fuNbOverMsPerTs(0)
  , fbIgnoreOverlapMs(kFALSE)
  , fUnpackParSts(nullptr)
  , fuStsNrOfDpbs(0)
  , fmStsDpbIdIndexMap()
  , fuMuchDpbIdx(1)
  , fUnpackParTof(nullptr)
  , fuTofNrOfDpbs(0)
  , fmTofDpbIdIndexMap()
  , fuDiamondDpbIdx(2)
  , fuTotalNrOfDpb(0)
  , fdStsTofOffsetNs(0.0)
  , fdMuchTofOffsetNs(0.0)
  , fsHistoFileFullname("data/mCBMsyncHistos.root")
  , fbPrintMessages(kFALSE)
  , fPrintMessCtrlSts(stsxyter::MessagePrintMask::msg_print_Human)
  , fulCurrentTsIdx(0)
  , fulCurrentMsIdx(0)
  , fmMsgCounter()
  , fuCurrentEquipmentId(0)
  , fuCurrDpbId(0)
  , fuCurrDpbIdx(0)
  , fiRunStartDateTimeSec(-1)
  , fiBinSizeDatePlots(-1)
  , fvulStsCurrentTsMsb()
  , fvuStsCurrentTsMsbCycle()
  , fvmStsSdpbHitsInMs()
  , fvmStsSdpbHitsInTs()
  , fulTofCurrentTsIndex(0)
  , fuTofCurrentMs(0)
  , fdTofMsIndex(0)
  , fuTofGdpbId(0)
  , fuTofGdpbNr(0)
  , fiTofEquipmentId(0)
  , fviTofMsgCounter(1 + gdpbv100::MSG_STAR_TRI_D, 0)
  , fvulTofCurrentEpoch()
  , fvulTofCurrentEpochCycle()
  , fvulTofCurrentEpochFull()
  , fulTofCurrentEpochTime(0)
  , fvmTofEpSupprBuffer()
  , fvmTofGdpbHitsInMs()
  , fvmTofGdpbHitsInTs()
  , fvuHitsNbInTimeBinSts(kuNbTimeBins, 0)
  , fvuHitsNbInTimeBinMuch(kuNbTimeBins, 0)
  , fvuHitsNbInTimeBinTof(kuNbTimeBins, 0)
  , fvuHitsNbInTimeBinDiam(kuNbTimeBins, 0)
  , fhMcbmHitsNbPerTsEvo()
  , fhMcbmHitsNbFineEvo()
  , fhMcbmHitsRateEvo()
  , fhDiamondHitsRateMapEvo()
  , fhDiamondHitsRateDerivative()
  , fhMuchVsStsHitsNbPerTimeBin()
  , fhTofVsStsHitsNbPerTimeBin()
  , fhDiamVsStsHitsNbPerTimeBin()
  , fhStsVsMuchHitsNbPerTimeBin()
  , fhTofVsMuchHitsNbPerTimeBin()
  , fhDiamVsMuchHitsNbPerTimeBin()
  , fhStsVsTofHitsNbPerTimeBin()
  , fhMuchVsTofHitsNbPerTimeBin()
  , fhDiamVsTofHitsNbPerTimeBin()
  , fhStsVsDiamHitsNbPerTimeBin()
  , fhTofVsDiamHitsNbPerTimeBin()
  , fhMuchVsDiamHitsNbPerTimeBin()
{
}

CbmMcbm2018MonitorMcbmRate::~CbmMcbm2018MonitorMcbmRate() {}

Bool_t CbmMcbm2018MonitorMcbmRate::Init()
{
  LOG(info) << "Initializing flib StsXyter unpacker for STS";

  FairRootManager* ioman = FairRootManager::Instance();
  if (ioman == nullptr) { LOG(fatal) << "No FairRootManager instance"; }

  return kTRUE;
}

void CbmMcbm2018MonitorMcbmRate::SetParContainers()
{
  LOG(info) << "Setting parameter containers for " << GetName();
  fUnpackParSts = (CbmMcbm2018StsPar*) (FairRun::Instance()->GetRuntimeDb()->getContainer("CbmMcbm2018StsPar"));
  fUnpackParTof = (CbmMcbm2018TofPar*) (FairRun::Instance()->GetRuntimeDb()->getContainer("CbmMcbm2018TofPar"));
}


Bool_t CbmMcbm2018MonitorMcbmRate::InitContainers()
{
  LOG(info) << "Init parameter containers for " << GetName();

  Bool_t bReInit = ReInitContainers();
  CreateStsHistograms();
  CreateTofHistograms();
  CreateMcbmHistograms();

  return bReInit;
}

Bool_t CbmMcbm2018MonitorMcbmRate::ReInitContainers()
{
  LOG(info) << "ReInit parameter containers for " << GetName();

  /***************** STS parameters *************************************/
  fuStsNrOfDpbs = fUnpackParSts->GetNrOfDpbs();
  LOG(info) << "Nr. of STS DPBs:       " << fuStsNrOfDpbs;

  fmStsDpbIdIndexMap.clear();
  for (UInt_t uDpb = 0; uDpb < fuStsNrOfDpbs; ++uDpb) {
    fmStsDpbIdIndexMap[fUnpackParSts->GetDpbId(uDpb)] = uDpb;
    LOG(info) << "Eq. ID for DPB #" << std::setw(2) << uDpb << " = 0x" << std::setw(4) << std::hex
              << fUnpackParSts->GetDpbId(uDpb) << std::dec << " => "
              << fmStsDpbIdIndexMap[fUnpackParSts->GetDpbId(uDpb)];
  }  // for( UInt_t uDpb = 0; uDpb < fuStsNrOfDpbs; ++uDpb )

  fvulStsCurrentTsMsb.resize(fuStsNrOfDpbs, 0);
  fvuStsCurrentTsMsbCycle.resize(fuStsNrOfDpbs, 0);
  fvmStsSdpbHitsInMs.resize(fuStsNrOfDpbs);
  fvmStsSdpbHitsInTs.resize(fuStsNrOfDpbs);
  /***************** STS parameters *************************************/

  /***************** TOF parameters *************************************/
  fuTofNrOfDpbs = fUnpackParTof->GetNrOfGdpbs();
  LOG(info) << "Nr. of Tof GDPBs: " << fuTofNrOfDpbs;

  fmTofDpbIdIndexMap.clear();
  for (UInt_t i = 0; i < fuTofNrOfDpbs; ++i) {
    fmTofDpbIdIndexMap[fUnpackParTof->GetGdpbId(i)] = i;
    LOG(info) << "GDPB Id of TOF  " << i << " : " << std::hex << fUnpackParTof->GetGdpbId(i) << std::dec;
  }  // for( UInt_t i = 0; i < fuTofNrOfDpbs; ++i )

  /// System sync monitoring
  fvulTofCurrentEpoch.resize(fuTofNrOfDpbs, 0);
  fvulTofCurrentEpochCycle.resize(fuTofNrOfDpbs, 0);
  fvulTofCurrentEpochFull.resize(fuTofNrOfDpbs, 0);
  fvmTofEpSupprBuffer.resize(fuTofNrOfDpbs);
  fvmTofGdpbHitsInMs.resize(fuTofNrOfDpbs);
  fvmTofGdpbHitsInTs.resize(fuTofNrOfDpbs);
  /***************** TOF parameters *************************************/

  return kTRUE;
}

void CbmMcbm2018MonitorMcbmRate::AddMsComponentToList(size_t component, UShort_t usDetectorId)
{
  switch (usDetectorId) {
    case 0x10:  //fles::SubsystemIdentifier::STS:
    {
      AddMsComponentToListSts(component);
      break;
    }           // case 0x10:
    case 0x60:  //fles::SubsystemIdentifier::RPC:
    {
      AddMsComponentToListTof(component);
      break;
    }  // case 0x60:
    default: {
      LOG(warning) << "CbmMcbm2018MonitorStsSync::AddMsComponentToList => "
                   << "Ignore component " << component << " as detector id " << usDetectorId
                   << " is not supported by this unpacker.";
      break;
    }  // default:
  }    // switch( iDetectorId )
}
void CbmMcbm2018MonitorMcbmRate::AddMsComponentToListSts(size_t component)
{
  /// Check for duplicates and ignore if it is the case
  for (UInt_t uCompIdx = 0; uCompIdx < fvMsComponentsListSts.size(); ++uCompIdx)
    if (component == fvMsComponentsListSts[uCompIdx]) return;

  /// Add to list
  fvMsComponentsListSts.push_back(component);
}
void CbmMcbm2018MonitorMcbmRate::AddMsComponentToListTof(size_t component)
{
  /// Check for duplicates and ignore if it is the case
  for (UInt_t uCompIdx = 0; uCompIdx < fvMsComponentsListTof.size(); ++uCompIdx)
    if (component == fvMsComponentsListTof[uCompIdx]) return;

  /// Add to list
  fvMsComponentsListTof.push_back(component);
}
void CbmMcbm2018MonitorMcbmRate::SetNbMsInTs(size_t uCoreMsNb, size_t uOverlapMsNb)
{
  fuNbCoreMsPerTs = uCoreMsNb;
  fuNbOverMsPerTs = uOverlapMsNb;

  //   UInt_t uNbMsTotal = fuNbCoreMsPerTs + fuNbOverMsPerTs;
}

/***************** STS Histograms *************************************/
void CbmMcbm2018MonitorMcbmRate::CreateStsHistograms()
{
  TString sHistName {""};
  TString title {""};
  /*
   sHistName = "hPulserMessageType";
   title = "Nb of message for each type; Type";
   fhStsMessType = new TH1I(sHistName, title, 5, 0., 5.);
   fhStsMessType->GetXaxis()->SetBinLabel( 1, "Dummy");
   fhStsMessType->GetXaxis()->SetBinLabel( 2, "Hit");
   fhStsMessType->GetXaxis()->SetBinLabel( 3, "TsMsb");
   fhStsMessType->GetXaxis()->SetBinLabel( 4, "Epoch");
   fhStsMessType->GetXaxis()->SetBinLabel( 5, "Empty");

   // Online histo browser commands
   THttpServer* server = FairRunOnline::Instance()->GetHttpServer();
   if( server )
   {
      server->Register("/StsRaw", fhStsMessType );

      server->RegisterCommand("/Reset_All_Pulser", "bMcbm2018ResetSync=kTRUE");
      server->RegisterCommand("/Write_All_Pulser", "bMcbm2018WriteSync=kTRUE");

      server->Restrict("/Reset_All_Pulser", "allow=admin");
      server->Restrict("/Write_All_Pulser", "allow=admin");
   } // if( server )
*/
  /** Create summary Canvases for CERN 2017 **/
  /*
   Double_t w = 10;
   Double_t h = 10;

      // Summary per StsXyter
   for( UInt_t uXyterIdx = 0; uXyterIdx < fuStsNbStsXyters; ++uXyterIdx )
   {
      TCanvas* cStsSumm = new TCanvas( Form("cStsSum_%03u", uXyterIdx ),
                                       Form("Summary plots for StsXyter %03u", uXyterIdx ),
                                       w, h);
      cStsSumm->Divide( 2, 2 );

      cStsSumm->cd(1);
      gPad->SetLogy();
      fhStsChanCntRaw[ uXyterIdx ]->Draw();

      cStsSumm->cd(2);
      gPad->SetLogz();
      fhStsChanAdcRaw[ uXyterIdx ]->Draw( "colz" );

      cStsSumm->cd(3);
      gPad->SetLogz();
      fhStsChanHitRateEvo[ uXyterIdx ]->Draw( "colz" );

      cStsSumm->cd(4);
//      gPad->SetLogy();
      fhStsChanAdcRawProf[ uXyterIdx ]->Draw();
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuStsNbStsXyters; ++uXyterIdx )

//====================================================================//
*/
  /*****************************/
  LOG(info) << "Done Creating STS Histograms";
}
/***************** STS Histograms *************************************/

/***************** TOF Histograms *************************************/
void CbmMcbm2018MonitorMcbmRate::CreateTofHistograms()
{
  TString sHistName {""};
  TString title {""};

  /*******************************************************************/
  /*

   sHistName = "hMessageType";
   title = "Nb of message for each type; Type";
   // Test Big Data readout with plotting
   fhTofMessType = new TH1I(sHistName, title, 1 + gdpbv100::MSG_STAR_TRI_A, 0., 1 + gdpbv100::MSG_STAR_TRI_A);
   fhTofMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_HIT,        "HIT");
   fhTofMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_EPOCH,      "EPOCH");
   fhTofMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_SLOWC,      "SLOWC");
   fhTofMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_SYST,       "SYST");
   fhTofMessType->GetXaxis()->SetBinLabel(1 + gdpbv100::MSG_STAR_TRI_A, "MSG_STAR_TRI");
*/
  /*******************************************************************/
  /*
   THttpServer* server = FairRunOnline::Instance()->GetHttpServer();
   if( server )
   {
      server->Register("/TofRaw", fhTofMessType );

      server->RegisterCommand("/Reset_All_TOF",  "bMcbm2018SyncResetHistosTof=kTRUE");
      server->RegisterCommand("/Save_All_Tof",   "bMcbm2018SyncSaveHistosTof=kTRUE");
      server->RegisterCommand("/Update_PulsFit", "bMcbm2018SyncUpdateZoomedFit=kTRUE");

      server->Restrict("/Reset_All_Tof", "allow=admin");
      server->Restrict("/Save_All_Tof",  "allow=admin");
      server->Restrict("/Update_PulsFit", "allow=admin");
   } // if( server )
*/
  /** Create summary Canvases for STAR 2018 **/
  /*
   Double_t w = 10;
   Double_t h = 10;
   TCanvas* cSummary = new TCanvas("cSummary", "gDPB Monitoring Summary", w, h);
   cSummary->Divide(2, 3);

   // 1st Column: Messages types
   cSummary->cd(1);
   gPad->SetLogy();
   fhTofMessType->Draw();

   cSummary->cd(2);
   gPad->SetLogy();
   fhTofSysMessType->Draw();

   cSummary->cd(3);
   gPad->SetLogz();
   fhTofGet4MessType->Draw("colz");

   // 2nd Column: GET4 Errors + Epoch flags + SCm
   cSummary->cd(4);
   gPad->SetLogz();
   fhTofGet4ChanErrors->Draw("colz");

   cSummary->cd(5);
   gPad->SetLogz();
   fhTofGet4EpochFlags->Draw("colz");

   cSummary->cd(6);
   fhTofGet4ChanScm->Draw("colz");
*/
  /*****************************/

  LOG(info) << "Done Creating TOF Histograms";
}
/***************** TOF Histograms *************************************/

/***************** mCBM Histograms ************************************/
void CbmMcbm2018MonitorMcbmRate::CreateMcbmHistograms()
{
  TString sHistName {""};
  TString sHistTitle {""};

  fuTotalNrOfDpb = fuStsNrOfDpbs + fuTofNrOfDpbs;

  sHistName            = "hMcbmHitsNbPerTsEvo";
  sHistTitle           = "Nb STS or TOF hits, per DPB and per TS; TS index []; DPB []; Nb Hits []";
  fhMcbmHitsNbPerTsEvo = new TH2D(sHistName, sHistTitle, 400001, -0.5, 400000.5, fuTotalNrOfDpb, 0., fuTotalNrOfDpb);

  sHistName           = "hMcbmHitsNbFineEvo";
  sHistTitle          = "Nb STS or TOF hits, per DPB and per 100 ms; t [s]; DPB []; Hit rate [1/s]";
  fhMcbmHitsNbFineEvo = new TH2D(sHistName, sHistTitle, 40001, -0.05, 4000.05, fuTotalNrOfDpb, 0., fuTotalNrOfDpb);

  sHistName  = "hMcbmHitsRateEvo";
  sHistTitle = "STS or TOF hits rate per DPB as function of time in run; t "
               "[s]; DPB []; Hit rate [1/s]";
  fhMcbmHitsRateEvo = new TH2D(sHistName, sHistTitle, 4001, -0.5, 4000.5, fuTotalNrOfDpb, 0., fuTotalNrOfDpb);

  sHistName  = "hDiamondHitsRateMapEvo";
  sHistTitle = "Counts per diamond strip and 100 ms as function of time in "
               "run; t [s]; strip []; Counts []";
  fhDiamondHitsRateMapEvo = new TH2D(sHistName, sHistTitle, 40001, -0.05, 4000.05, 8, 0., 8.);

  sHistName  = "hDiamondHitsRateDerivative";
  sHistTitle = "Variation of the diamond counts per s in 100 ms bins as "
               "function of time in run; t [s];  Delta(Counts/s) []";
  fhDiamondHitsRateDerivative = new TH1D(sHistName, sHistTitle, 40000, 0., 4000.);

  sHistName                   = "hMuchVsStsHitsNbPerTimeBin";
  sHistTitle                  = Form("Nb Hits in MUCH vs Sts in time bins of %5u ns; Nb Hits "
                    "STS [];  Nb Hits TOF []; Time bins []",
                    kuTimeBinSizeNs);
  fhMuchVsStsHitsNbPerTimeBin = new TH2D(sHistName, sHistTitle, 2000, 0., 2000., 2000, 0., 2000.);
  sHistName                   = "hTofVsStsHitsNbPerTimeBin";
  sHistTitle                  = Form("Nb Hits in TOF vs Sts in time bins of %5u ns; Nb Hits STS "
                    "[];  Nb Hits MUCH []; Time bins []",
                    kuTimeBinSizeNs);
  fhTofVsStsHitsNbPerTimeBin  = new TH2D(sHistName, sHistTitle, 2000, 0., 2000., 2000, 0., 2000.);
  sHistName                   = "hDiamVsStsHitsNbPerTimeBin";
  sHistTitle                  = Form("Nb Hits in Diam vs Sts in time bins of %5u ns; Nb Hits "
                    "STS [];  Nb Hits Diam []; Time bins []",
                    kuTimeBinSizeNs);
  fhDiamVsStsHitsNbPerTimeBin = new TH2D(sHistName, sHistTitle, 2000, 0., 2000., 2000, 0., 2000.);

  sHistName                    = "hStsVsMuchHitsNbPerTimeBin";
  sHistTitle                   = Form("Nb Hits in STS vs MUCH in time bins of %5u ns; Nb Hits "
                    "MUCH [];  Nb Hits STS []; Time bins []",
                    kuTimeBinSizeNs);
  fhStsVsMuchHitsNbPerTimeBin  = new TH2D(sHistName, sHistTitle, 2000, 0., 2000., 2000, 0., 2000.);
  sHistName                    = "hTofVsMuchHitsNbPerTimeBin";
  sHistTitle                   = Form("Nb Hits in TOF vs MUCH in time bins of %5u ns; Nb Hits "
                    "MUCH [];  Nb Hits TOF []; Time bins []",
                    kuTimeBinSizeNs);
  fhTofVsMuchHitsNbPerTimeBin  = new TH2D(sHistName, sHistTitle, 2000, 0., 2000., 2000, 0., 2000.);
  sHistName                    = "hDiamVsMuchHitsNbPerTimeBin";
  sHistTitle                   = Form("Nb Hits in Diam vs MUCH in time bins of %5u ns; Nb Hits "
                    "MUCH [];  Nb Hits Diam []; Time bins []",
                    kuTimeBinSizeNs);
  fhDiamVsMuchHitsNbPerTimeBin = new TH2D(sHistName, sHistTitle, 2000, 0., 2000., 2000, 0., 2000.);

  sHistName                   = "hStsVsTofHitsNbPerTimeBin";
  sHistTitle                  = Form("Nb Hits in STS vs TOF in time bins of %5u ns; Nb Hits TOF "
                    "[];  Nb Hits STS []; Time bins []",
                    kuTimeBinSizeNs);
  fhStsVsTofHitsNbPerTimeBin  = new TH2D(sHistName, sHistTitle, 2000, 0., 2000., 2000, 0., 2000.);
  sHistName                   = "hMuchVsTofHitsNbPerTimeBin";
  sHistTitle                  = Form("Nb Hits in MUCH vs TOF in time bins of %5u ns; Nb Hits "
                    "TOF [];  Nb Hits MUCH []; Time bins []",
                    kuTimeBinSizeNs);
  fhMuchVsTofHitsNbPerTimeBin = new TH2D(sHistName, sHistTitle, 2000, 0., 2000., 2000, 0., 2000.);
  sHistName                   = "hDiamVsTofHitsNbPerTimeBin";
  sHistTitle                  = Form("Nb Hits in Diam vs TOF in time bins of %5u ns; Nb Hits "
                    "TOF [];  Nb Hits Diam []; Time bins []",
                    kuTimeBinSizeNs);
  fhDiamVsTofHitsNbPerTimeBin = new TH2D(sHistName, sHistTitle, 2000, 0., 2000., 2000, 0., 2000.);

  sHistName                    = "hStsVsDiamHitsNbPerTimeBin";
  sHistTitle                   = Form("Nb Hits in STS vs Diam in time bins of %5u ns; Nb Hits "
                    "Diam [];  Nb Hits STS []; Time bins []",
                    kuTimeBinSizeNs);
  fhStsVsDiamHitsNbPerTimeBin  = new TH2D(sHistName, sHistTitle, 2000, 0., 2000., 2000, 0., 2000.);
  sHistName                    = "hTofVsDiamHitsNbPerTimeBin";
  sHistTitle                   = Form("Nb Hits in TOF vs Diam in time bins of %5u ns; Nb Hits "
                    "Diam [];  Nb Hits TOF []; Time bins []",
                    kuTimeBinSizeNs);
  fhTofVsDiamHitsNbPerTimeBin  = new TH2D(sHistName, sHistTitle, 2000, 0., 2000., 2000, 0., 2000.);
  sHistName                    = "hMuchVsDiamHitsNbPerTimeBin";
  sHistTitle                   = Form("Nb Hits in MUCH vs Diam in time bins of %5u ns; Nb Hits "
                    "Diam [];  Nb Hits MUCH []; Time bins []",
                    kuTimeBinSizeNs);
  fhMuchVsDiamHitsNbPerTimeBin = new TH2D(sHistName, sHistTitle, 2000, 0., 2000., 2000, 0., 2000.);

  THttpServer* server = FairRunOnline::Instance()->GetHttpServer();
  if (server) {
    server->Register("/mCbmRate", fhMcbmHitsNbPerTsEvo);
    server->Register("/mCbmRate", fhMcbmHitsNbFineEvo);
    server->Register("/mCbmRate", fhMcbmHitsRateEvo);
    /*
      server->RegisterCommand("/Reset_All",  "bMcbm2018ResetSync=kTRUE");
      server->RegisterCommand("/Save_All",   "bMcbm2018WriteSync=kTRUE");

      server->Restrict("/Reset_All", "allow=admin");
      server->Restrict("/Save_All",  "allow=admin");
*/
  }  // if( server )

  //   Double_t w = 10;
  //   Double_t h = 10;
  /** Create Pulser check Canvas for STS vs TOF **/
  /*
   TCanvas* cSyncMcbm = new TCanvas("cSyncMcbm", "Time difference for STS and TOF hits, per DPB, against any Diamond hit", w, h);
   cSyncMcbm->Divide( 2);

   cSyncMcbm->cd(1);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhMcbmTimeDiffToDiamond->Draw( "colz" );

   cSyncMcbm->cd(2);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhMcbmTimeDiffToDiamondWide->Draw( "colz" );
*/
  /*****************************/
  LOG(info) << "Done Creating mCBM Histograms";
}
/***************** mCBM Histograms ************************************/

Bool_t CbmMcbm2018MonitorMcbmRate::DoUnpack(const fles::Timeslice& ts, size_t component)
{
  /// General commands
  /*
   if( bMcbm2018ResetSync )
   {
      ResetAllHistos();
      bMcbm2018ResetSync = kFALSE;
   } // if( bMcbm2018ResetSync )
   if( bMcbm2018WriteSync )
   {
      SaveAllHistos( fsHistoFileFullname );
      bMcbm2018WriteSync = kFALSE;
   } // if( bMcbm2018WriteSync )
*/

  LOG(debug) << "Timeslice contains " << ts.num_microslices(component) << "microslices.";
  fulCurrentTsIdx       = ts.index();
  fulCurrentTsStartTime = ts.descriptor(0, 0).idx;

  /// Ignore first TS as often data corruption there
  if (0 == fulCurrentTsIdx) return kTRUE;

  if (fulCurrentTsIdx < 30) LOG(info) << Form("TS %2llu", fulCurrentTsIdx);

  // Ignore overlap ms if flag set by user
  UInt_t uNbMsLoop = fuNbCoreMsPerTs;
  if (kFALSE == fbIgnoreOverlapMs) uNbMsLoop += fuNbOverMsPerTs;

  // Loop over core microslices (and overlap ones if chosen)
  for (UInt_t uMsIdx = 0; uMsIdx < uNbMsLoop; uMsIdx++) {
    // Loop over registered STS components
    for (UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsListSts.size(); ++uMsCompIdx) {
      UInt_t uMsComp = fvMsComponentsListSts[uMsCompIdx];

      if (kFALSE == ProcessStsMs(ts, uMsComp, uMsIdx)) return kFALSE;
    }  // for( UInt_t uMsComp = 0; uMsComp < fvMsComponentsListSts.size(); ++uMsComp )

    // Loop over registered TOF components
    for (UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsListTof.size(); ++uMsCompIdx) {
      UInt_t uMsComp = fvMsComponentsListTof[uMsCompIdx];

      if (kFALSE == ProcessTofMs(ts, uMsComp, uMsIdx)) return kFALSE;
    }  // for( UInt_t uMsComp = 0; uMsComp < fvMsComponentsListSts.size(); ++uMsComp )

    /****************** STS Sync ******************************************/
    /*
      /// Sort the buffers of hits
      for( UInt_t uSdpb = 0; uSdpb < fuStsNrOfDpbs; ++uSdpb )
         std::sort( fvmStsSdpbHitsInMs[ uSdpb ].begin(), fvmStsSdpbHitsInMs[ uSdpb ].end() );
*/
    /****************** STS Sync ******************************************/

    /****************** TOF Sync ******************************************/
    /*
      /// Sort the buffers of hits
      for( UInt_t uGdpb = 0; uGdpb < fuTofNrOfDpbs; ++uGdpb )
         std::sort( fvmTofGdpbHitsInMs[ uGdpb ].begin(), fvmTofGdpbHitsInMs[ uGdpb ].end() );
*/
    UInt_t uNbDiaHits = fvmTofGdpbHitsInMs[fuDiamondDpbIdx].size();
    for (UInt_t uHitDia = 0; uHitDia < uNbDiaHits; uHitDia++) {
      //         Double_t dDiaTime = fvmTofGdpbHitsInMs[ fuDiamondDpbIdx ][ uHitDia ].GetFullTimeNs() / 1e-9;
      UInt_t uChan = 8;

      switch (fvmTofGdpbHitsInMs[fuDiamondDpbIdx][uHitDia].getGdpbHitChanId()) {
        case 0: uChan = 0; break;
        case 32: uChan = 1; break;
        case 64: uChan = 2; break;
        case 96: uChan = 3; break;
        case 160: uChan = 4; break;
        case 192: uChan = 5; break;
        case 224: uChan = 6; break;
        case 256: uChan = 7; break;
      }  // switch( fvmTofGdpbHitsInMs[ fuDiamondDpbIdx ][ uHitDia ].GetHitChannel() )

      if (uChan < 8)
        //            fhDiamondHitsRateMapEvo->Fill( dDiaTime, uChan, 0.1 );
        fhDiamondHitsRateMapEvo->Fill((1e-9) * static_cast<double>(fulCurrentMsIdx), uChan, 0.1);
    }  // for( UInt_t uHitDia = 0; uHitDia < uNbDiaHits; uHitDia++)
       /****************** TOF Sync ******************************************/

    /****************** mCBM Sync *****************************************/
    /// Fill histos and Clear buffers
    for (UInt_t uSdpb = 0; uSdpb < fuStsNrOfDpbs; ++uSdpb) {
      UInt_t uNbHitsInMs = fvmStsSdpbHitsInMs[uSdpb].size();
      fhMcbmHitsNbPerTsEvo->Fill(fulCurrentTsIdx, uSdpb, uNbHitsInMs);
      fhMcbmHitsNbFineEvo->Fill((1e-9) * static_cast<double>(fulCurrentMsIdx), uSdpb, uNbHitsInMs / 0.1);
      fhMcbmHitsRateEvo->Fill((1e-9) * static_cast<double>(fulCurrentMsIdx), uSdpb, uNbHitsInMs);

      for (UInt_t uHit = 0; uHit < uNbHitsInMs; ++uHit) {
        Double_t dHitTimeInTs =
          stsxyter::kdClockCycleNs * fvmStsSdpbHitsInMs[uSdpb][uHit].GetTs() - fulCurrentTsStartTime;

        /// Apply time offset to STS and MUCH
        if (fuMuchDpbIdx == uSdpb) dHitTimeInTs -= fdMuchTofOffsetNs;
        else
          dHitTimeInTs -= fdStsTofOffsetNs;

        Int_t iTimeBin = static_cast<Int_t>(dHitTimeInTs / kuTimeBinSizeNs);
        if (iTimeBin < 0 || kuNbTimeBins <= static_cast<UInt_t>(iTimeBin)) {
          LOG(debug) << "sDPB hits with time out of the TS!! " << dHitTimeInTs << " " << iTimeBin;
          continue;
        }  // if( iTimeBin < 0 || kuNbTimeBins <= iTimeBin )

        if (fuMuchDpbIdx == uSdpb) fvuHitsNbInTimeBinMuch[iTimeBin]++;
        else
          fvuHitsNbInTimeBinSts[iTimeBin]++;
      }  // for( UInt_t uHit = 0; uHit < uNbHitsInMs; ++uHit )
         /*
      /// Jump Sts has it has far too many hits!!!
         if( 0 < uSdpb )
         fvmStsSdpbHitsInTs[ uSdpb ].insert( fvmStsSdpbHitsInTs[ uSdpb ].end(),
                                             fvmStsSdpbHitsInMs[ uSdpb ].begin(),
                                             fvmStsSdpbHitsInMs[ uSdpb ].end() );
*/
      fvmStsSdpbHitsInMs[uSdpb].clear();
    }  // for( UInt_t uSdpb = 0; uSdpb < fuStsNrOfDpbs; ++uSdpb )
    for (UInt_t uGdpb = 0; uGdpb < fuTofNrOfDpbs; ++uGdpb) {
      UInt_t uNbHitsInMs = fvmTofGdpbHitsInMs[uGdpb].size();
      fhMcbmHitsNbPerTsEvo->Fill(fulCurrentTsIdx, uGdpb + fuStsNrOfDpbs, uNbHitsInMs);
      fhMcbmHitsNbFineEvo->Fill((1e-9) * static_cast<double>(fulCurrentMsIdx), uGdpb + fuStsNrOfDpbs,
                                uNbHitsInMs / 0.1);
      fhMcbmHitsRateEvo->Fill((1e-9) * static_cast<double>(fulCurrentMsIdx), uGdpb + fuStsNrOfDpbs, uNbHitsInMs);

      if (fuDiamondDpbIdx == uGdpb) {
        /// Add to Previous bin to get N( T + 1 ) - N( T )
        fhDiamondHitsRateDerivative->Fill((1e-9) * static_cast<double>(fulCurrentMsIdx) - 0.05,
                                          fvmTofGdpbHitsInMs[uGdpb].size() / 0.1);
        /// Sub to Next bin to get N( T ) - N( T - 1 )
        fhDiamondHitsRateDerivative->Fill((1e-9) * static_cast<double>(fulCurrentMsIdx) + 0.05,
                                          fvmTofGdpbHitsInMs[uGdpb].size() / -0.1);
      }  // if( fuDiamondDpbIdx == uGdpb )

      for (UInt_t uHit = 0; uHit < uNbHitsInMs; ++uHit) {
        Double_t dHitTimeInTs = fvmTofGdpbHitsInMs[uGdpb][uHit].GetFullTimeNs() - fulCurrentTsStartTime;

        Int_t iTimeBin = static_cast<Int_t>(dHitTimeInTs / kuTimeBinSizeNs);
        if (iTimeBin < 0 || kuNbTimeBins <= static_cast<UInt_t>(iTimeBin)) {
          LOG(debug) << "gDPB hits with time out of the TS!! " << fvmTofGdpbHitsInMs[uGdpb][uHit].GetFullTimeNs() << " "
                     << fulCurrentTsStartTime << " " << iTimeBin;
          continue;
        }  // if( iTimeBin < 0 || kuNbTimeBins <= iTimeBin )

        if (fuDiamondDpbIdx == uGdpb) fvuHitsNbInTimeBinDiam[iTimeBin]++;
        else
          fvuHitsNbInTimeBinTof[iTimeBin]++;
      }  // for( UInt_t uHit = 0; uHit < uNbHitsInMs; ++uHit )

      /*
         fvmTofGdpbHitsInTs[ uGdpb ].insert( fvmTofGdpbHitsInTs[ uGdpb ].end(),
                                             fvmTofGdpbHitsInMs[ uGdpb ].begin(),
                                             fvmTofGdpbHitsInMs[ uGdpb ].end() );
*/
      fvmTofGdpbHitsInMs[uGdpb].clear();
    }  // for( UInt_t uGdpb = 0; uGdpb < fuTofNrOfDpbs; ++uGdpb )
       /****************** mCBM Sync *****************************************/
  }    // for( UInt_t uMsIdx = 0; uMsIdx < uNbMsLoop; uMsIdx ++ )

  /****************** mCBM Sync *****************************************/
  /*
   /// Build time differences for each DPB hit against each diamond hit
   UInt_t uNbDiaHits = fvmTofGdpbHitsInTs[ fuDiamondDpbIdx ].size();
   for( UInt_t uHitDia = 0; uHitDia < uNbDiaHits; uHitDia++)
   {
      Double_t dDiaTime = fvmTofGdpbHitsInTs[ fuDiamondDpbIdx ][ uHitDia ].GetFullTimeNs();

      /// Jump Sts has it has far too many hits!!!
      for( UInt_t uSdpb = 1; uSdpb < fuStsNrOfDpbs; ++uSdpb )
      {
         UInt_t uNbHits = fvmStsSdpbHitsInTs[ uSdpb ].size();
         for( UInt_t uHit = 0; uHit < uNbHits; ++uHit )
         {
            Double_t dHitTime = stsxyter::kdClockCycleNs * fvmStsSdpbHitsInTs[ uSdpb ][ uHit ].GetTs();

            Double_t dDt = dHitTime - dDiaTime;
            fhMcbmTimeDiffToDiamondTs->Fill( dDt / 1e6, uSdpb );

            fvhMcbmTimeDiffToDiamondTsEvoDpb[ uSdpb ]->Fill( fulCurrentTsIdx, dDt / 1e6 );
         } // for( UInt_t uHit = 0; uHit < uNbHits; ++uHit )
      } // for( UInt_t uSdpb = 0; uSdpb < fuStsNrOfDpbs; ++uSdpb )

      for( UInt_t uGdpb = 0; uGdpb < fuTofNrOfDpbs; ++uGdpb )
      {
         if( fuDiamondDpbIdx == uGdpb )
            continue;

         UInt_t uNbHits = fvmTofGdpbHitsInTs[ uGdpb ].size();
         for( UInt_t uHit = 0; uHit < uNbHits; ++uHit )
         {
            Double_t dHitTime = fvmTofGdpbHitsInTs[ uGdpb ][ uHit ].GetFullTimeNs();

            Double_t dDt = dHitTime - dDiaTime;
            fhMcbmTimeDiffToDiamondTs->Fill( dDt / 1e6, uGdpb + fuStsNrOfDpbs );

            fvhMcbmTimeDiffToDiamondTsEvoDpb[ uGdpb + fuStsNrOfDpbs ]->Fill( fulCurrentTsIdx, dDt / 1e6 );
         } // for( UInt_t uHit = 0; uHit < uNbHits; ++uHit )
      } // for( UInt_t uGdpb = 0; uGdpb < fuTofNrOfDpbs; ++uGdpb )
   } // for( UInt_t uHitDia = 0; uHitDia < uNbDiaHits; uHitDia++)

   /// Clear buffers
   for( UInt_t uSdpb = 0; uSdpb < fuStsNrOfDpbs; ++uSdpb )
   {
      fvmStsSdpbHitsInTs[ uSdpb ].clear();
   } // for( UInt_t uSdpb = 0; uSdpb < fuStsNrOfDpbs; ++uSdpb )
   for( UInt_t uGdpb = 0; uGdpb < fuTofNrOfDpbs; ++uGdpb )
   {
      fvmTofGdpbHitsInTs[ uGdpb ].clear();
   } // for( UInt_t uGdpb = 0; uGdpb < fuTofNrOfDpbs; ++uGdpb )
*/

  for (UInt_t uTimeBin = 0; uTimeBin < kuNbTimeBins; ++uTimeBin) {
    if (0 < fvuHitsNbInTimeBinSts[uTimeBin]) {
      if (0 < fvuHitsNbInTimeBinMuch[uTimeBin])
        fhMuchVsStsHitsNbPerTimeBin->Fill(fvuHitsNbInTimeBinSts[uTimeBin], fvuHitsNbInTimeBinMuch[uTimeBin]);
      if (0 < fvuHitsNbInTimeBinTof[uTimeBin])
        fhTofVsStsHitsNbPerTimeBin->Fill(fvuHitsNbInTimeBinSts[uTimeBin], fvuHitsNbInTimeBinTof[uTimeBin]);
      if (0 < fvuHitsNbInTimeBinDiam[uTimeBin])
        fhDiamVsStsHitsNbPerTimeBin->Fill(fvuHitsNbInTimeBinSts[uTimeBin], fvuHitsNbInTimeBinDiam[uTimeBin]);
    }  // if( 0 < fvuHitsNbInTimeBinSts[ uTimeBin ] )

    if (0 < fvuHitsNbInTimeBinMuch[uTimeBin]) {
      if (0 < fvuHitsNbInTimeBinSts[uTimeBin])
        fhStsVsMuchHitsNbPerTimeBin->Fill(fvuHitsNbInTimeBinMuch[uTimeBin], fvuHitsNbInTimeBinSts[uTimeBin]);
      if (0 < fvuHitsNbInTimeBinTof[uTimeBin])
        fhTofVsMuchHitsNbPerTimeBin->Fill(fvuHitsNbInTimeBinMuch[uTimeBin], fvuHitsNbInTimeBinTof[uTimeBin]);
      if (0 < fvuHitsNbInTimeBinDiam[uTimeBin])
        fhDiamVsMuchHitsNbPerTimeBin->Fill(fvuHitsNbInTimeBinMuch[uTimeBin], fvuHitsNbInTimeBinDiam[uTimeBin]);
    }  // if( 0 < fvuHitsNbInTimeBinMuch[ uTimeBin ] )

    if (0 < fvuHitsNbInTimeBinTof[uTimeBin]) {
      if (0 < fvuHitsNbInTimeBinSts[uTimeBin])
        fhStsVsTofHitsNbPerTimeBin->Fill(fvuHitsNbInTimeBinTof[uTimeBin], fvuHitsNbInTimeBinSts[uTimeBin]);
      if (0 < fvuHitsNbInTimeBinMuch[uTimeBin])
        fhMuchVsTofHitsNbPerTimeBin->Fill(fvuHitsNbInTimeBinTof[uTimeBin], fvuHitsNbInTimeBinMuch[uTimeBin]);
      if (0 < fvuHitsNbInTimeBinDiam[uTimeBin])
        fhDiamVsTofHitsNbPerTimeBin->Fill(fvuHitsNbInTimeBinTof[uTimeBin], fvuHitsNbInTimeBinDiam[uTimeBin]);
    }  // if( 0 < fvuHitsNbInTimeBinTof[ uTimeBin ] )

    if (0 < fvuHitsNbInTimeBinDiam[uTimeBin]) {
      if (0 < fvuHitsNbInTimeBinSts[uTimeBin])
        fhStsVsDiamHitsNbPerTimeBin->Fill(fvuHitsNbInTimeBinDiam[uTimeBin], fvuHitsNbInTimeBinSts[uTimeBin]);
      if (0 < fvuHitsNbInTimeBinMuch[uTimeBin])
        fhMuchVsDiamHitsNbPerTimeBin->Fill(fvuHitsNbInTimeBinDiam[uTimeBin], fvuHitsNbInTimeBinMuch[uTimeBin]);
      if (0 < fvuHitsNbInTimeBinTof[uTimeBin])
        fhTofVsDiamHitsNbPerTimeBin->Fill(fvuHitsNbInTimeBinDiam[uTimeBin], fvuHitsNbInTimeBinTof[uTimeBin]);
    }  // if( 0 < fvuHitsNbInTimeBinDiam[ uTimeBin ] )

    fvuHitsNbInTimeBinSts[uTimeBin]  = 0;
    fvuHitsNbInTimeBinMuch[uTimeBin] = 0;
    fvuHitsNbInTimeBinTof[uTimeBin]  = 0;
    fvuHitsNbInTimeBinDiam[uTimeBin] = 0;
  }  // for( UInt_t uTimeBin = 0; uTimeBin < kuNbTimeBins; ++uTimeBin )
     /****************** mCBM Sync *****************************************/

  if (0 == ts.index() % 1000) {
    LOG(info) << "End of TS " << std::setw(7) << ts.index();
  }  // if( 0 == ts.index() % 1000 )

  return kTRUE;
}

/****************** STS Sync ******************************************/
Bool_t CbmMcbm2018MonitorMcbmRate::ProcessStsMs(const fles::Timeslice& ts, size_t uMsComp, UInt_t uMsIdx)
{
  auto msDescriptor        = ts.descriptor(uMsComp, uMsIdx);
  fuCurrentEquipmentId     = msDescriptor.eq_id;
  const uint8_t* msContent = reinterpret_cast<const uint8_t*>(ts.content(uMsComp, uMsIdx));

  uint32_t uSize  = msDescriptor.size;
  fulCurrentMsIdx = msDescriptor.idx;
  //   Double_t dMsTime = (1e-9) * static_cast<double>(fulCurrentMsIdx);
  LOG(debug) << "Microslice: " << fulCurrentMsIdx << " from EqId " << std::hex << fuCurrentEquipmentId << std::dec
             << " has size: " << uSize;

  fuCurrDpbId = static_cast<uint32_t>(fuCurrentEquipmentId & 0xFFFF);
  //   fuCurrDpbIdx = fmStsDpbIdIndexMap[ fuCurrDpbId ];

  /// Check if this sDPB ID was declared in parameter file and stop there if not
  auto it = fmStsDpbIdIndexMap.find(fuCurrDpbId);
  if (it == fmStsDpbIdIndexMap.end()) {
    LOG(info) << "---------------------------------------------------------------";
    /*
       LOG(info) << "hi hv eqid flag si sv idx/start        crc      size     offset";
       LOG(info) << Form( "%02x %02x %04x %04x %02x %02x %016llx %08x %08x %016llx",
                         static_cast<unsigned int>(msDescriptor.hdr_id),
                         static_cast<unsigned int>(msDescriptor.hdr_ver), msDescriptor.eq_id, msDescriptor.flags,
                         static_cast<unsigned int>(msDescriptor.sys_id),
                         static_cast<unsigned int>(msDescriptor.sys_ver), msDescriptor.idx, msDescriptor.crc,
                         msDescriptor.size, msDescriptor.offset );
*/
    LOG(info) << FormatMsHeaderPrintout(msDescriptor);
    LOG(warning) << "Could not find the sDPB index for AFCK id 0x" << std::hex << fuCurrDpbId << std::dec
                 << " in timeslice " << fulCurrentTsIdx << " in microslice " << uMsIdx << " component " << uMsComp
                 << "\n"
                 << "If valid this index has to be added in the STS/MUCH "
                    "parameter file in the RocIdArray field";
    return kFALSE;
  }  // if( it == fmStsDpbIdIndexMap.end() )
  else
    fuCurrDpbIdx = fmStsDpbIdIndexMap[fuCurrDpbId];

  /** Check the current TS_MSb cycle and correct it if wrong **/
  UInt_t uTsMsbCycleHeader = std::floor(fulCurrentMsIdx / (stsxyter::kulTsCycleNbBins * stsxyter::kdClockCycleNs));

  if (0 == uMsIdx) {
    fvuStsCurrentTsMsbCycle[fuCurrDpbIdx] = uTsMsbCycleHeader;
    fvulStsCurrentTsMsb[fuCurrDpbIdx]     = 0;
  }  // if( 0 == uMsIdx )
  else if (uTsMsbCycleHeader != fvuStsCurrentTsMsbCycle[fuCurrDpbIdx] && 4194303 != fvulStsCurrentTsMsb[fuCurrDpbIdx]) {
    LOG(warning) << "TS MSB cycle from MS header does not match current cycle from data "
                 << "for TS " << std::setw(12) << fulCurrentTsIdx << " MS " << std::setw(12) << fulCurrentMsIdx
                 << " MsInTs " << std::setw(3) << uMsIdx << " ====> " << fvuStsCurrentTsMsbCycle[fuCurrDpbIdx] << " VS "
                 << uTsMsbCycleHeader;
    fvuStsCurrentTsMsbCycle[fuCurrDpbIdx] = uTsMsbCycleHeader;
  }

  // If not integer number of message in input buffer, print warning/error
  if (0 != (uSize % kuStsBytesPerMessage))
    LOG(error) << "The input microslice buffer does NOT "
               << "contain only complete nDPB messages!";

  // Compute the number of complete messages in the input microslice buffer
  uint32_t uNbMessages = (uSize - (uSize % kuStsBytesPerMessage)) / kuStsBytesPerMessage;

  // Prepare variables for the loop on contents
  const uint32_t* pInBuff = reinterpret_cast<const uint32_t*>(msContent);

  for (uint32_t uIdx = 0; uIdx < uNbMessages; ++uIdx) {
    // Fill message
    uint32_t ulData = static_cast<uint32_t>(pInBuff[uIdx]);

    stsxyter::Message mess(static_cast<uint32_t>(ulData & 0xFFFFFFFF));

    // Print message if requested
    if (fbPrintMessages) mess.PrintMess(std::cout, fPrintMessCtrlSts);

    stsxyter::MessType typeMess = mess.GetMessType();
    fmMsgCounter[typeMess]++;

    switch (typeMess) {
      case stsxyter::MessType::Hit: {

        FillStsHitInfo(mess, uMsIdx);
        break;
      }  // case stsxyter::MessType::Hit :
      case stsxyter::MessType::TsMsb: {
        FillStsTsMsbInfo(mess, uIdx, uMsIdx);
        break;
      }  // case stsxyter::MessType::TsMsb :
      case stsxyter::MessType::Epoch: {
        // The first message in the TS is a special ones: EPOCH
        FillStsEpochInfo(mess);

        if (0 < uIdx)
          LOG(info) << "CbmMcbm2018MonitorMcbmRate::DoUnpack => "
                    << "EPOCH message at unexpected position in MS: message " << uIdx << " VS message 0 expected!";
        break;
      }  // case stsxyter::MessType::TsMsb :
      case stsxyter::MessType::Status:
      case stsxyter::MessType::Empty:
      case stsxyter::MessType::Dummy: {
        break;
      }  // case stsxyter::MessType::Dummy / ReadDataAck / Ack :
      default: {
        LOG(fatal) << "CbmMcbm2018MonitorMcbmRate::DoUnpack => "
                   << "Unknown message type, should never happen, stopping here!";
      }
    }  // switch( mess.GetMessType() )
  }    // for( uint32_t uIdx = 0; uIdx < uNbMessages; ++uIdx )
  return kTRUE;
}

void CbmMcbm2018MonitorMcbmRate::FillStsHitInfo(stsxyter::Message mess, const UInt_t& /*uMsIdx*/)
{
  UShort_t usChan   = mess.GetHitChannel();
  UShort_t usRawAdc = mess.GetHitAdc();
  //   UShort_t usFullTs = mess.GetHitTimeFull();
  //   UShort_t usTsOver = mess.GetHitTimeOver();
  UShort_t usRawTs = mess.GetHitTime();

  UInt_t uAsicIdx = 0;  /// Not used here, otherwise should be extracted from eLink mapping

  if (fuCurrDpbIdx == fuMuchDpbIdx) {
    /// MUCH bad channels
    switch (usChan) {
      case 101:
      case 99:
      case 91:
      case 89:
      case 88:
      case 86:
      case 84:
      case 83:
      case 80:
      case 78:
      case 76:
      case 50:
      case 39:
      case 37:
      case 35:
      case 20: {
        return;
        break;
      }  // if bad channel
      default: break;
    }  // switch( usChan )
  }    // if( fuCurrDpbIdx == fuMuchDpbIdx )
  else {
    /// STS bad channels
    uAsicIdx        = fUnpackParSts->ElinkIdxToAsicIdx(kFALSE, mess.GetLinkIndex());
    UInt_t uChanIdx = usChan + fUnpackParSts->GetNbChanPerAsic() * uAsicIdx;
    switch (uChanIdx) {
      case 781:
      case 270:
      case 411:
      case 518: {
        return;
        break;
      }  // if bad channel
      default: break;
    }                                                                             // switch( mess.GetLinkIndex() )
    if ((0 == uChanIdx % 2) && (543 < uChanIdx) && (uChanIdx < 633)) { return; }  // if bad channel
  }  // else of if( fuCurrDpbIdx == fuMuchDpbIdx )

  // Compute the Full time stamp
  // Use TS w/o overlap bits as they will anyway come from the TS_MSB
  Long64_t ulStsHitTime = usRawTs;

  ulStsHitTime +=
    static_cast<ULong64_t>(stsxyter::kuHitNbTsBins) * static_cast<ULong64_t>(fvulStsCurrentTsMsb[fuCurrDpbIdx])
    + static_cast<ULong64_t>(stsxyter::kulTsCycleNbBins)
        * static_cast<ULong64_t>(fvuStsCurrentTsMsbCycle[fuCurrDpbIdx]);

  // Convert the Hit time in bins to Hit time in ns
  //   Double_t dHitTimeNs = ulStsHitTime * stsxyter::kdClockCycleNs;

  // Pulser and MS
  fvmStsSdpbHitsInMs[fuCurrDpbIdx].push_back(stsxyter::FinalHit(ulStsHitTime, usRawAdc, uAsicIdx, usChan));
}

void CbmMcbm2018MonitorMcbmRate::FillStsTsMsbInfo(stsxyter::Message mess, UInt_t uMessIdx, UInt_t uMsIdx)
{
  UInt_t uVal = mess.GetTsMsbVal();

  // Update Status counters
  if (uVal < fvulStsCurrentTsMsb[fuCurrDpbIdx]) {

    LOG(info) << " TS " << std::setw(12) << fulCurrentTsIdx << " MS " << std::setw(12) << fulCurrentMsIdx << " DPB "
              << std::setw(2) << fuCurrDpbIdx << " Old TsMsb " << std::setw(5) << fvulStsCurrentTsMsb[fuCurrDpbIdx]
              << " Old MsbCy " << std::setw(5) << fvuStsCurrentTsMsbCycle[fuCurrDpbIdx] << " new TsMsb " << std::setw(5)
              << uVal;

    fvuStsCurrentTsMsbCycle[fuCurrDpbIdx]++;
  }  // if( uVal < fvulStsCurrentTsMsb[fuCurrDpbIdx] )
  if (uVal != fvulStsCurrentTsMsb[fuCurrDpbIdx] + 1 && 0 != uVal && 4194303 != fvulStsCurrentTsMsb[fuCurrDpbIdx]
      && 1 != uMessIdx) {
    LOG(info) << "TS MSb Jump in "
              << " TS " << std::setw(12) << fulCurrentTsIdx << " MS " << std::setw(12) << fulCurrentMsIdx << " MS Idx "
              << std::setw(4) << uMsIdx << " Msg Idx " << std::setw(5) << uMessIdx << " DPB " << std::setw(2)
              << fuCurrDpbIdx << " => Old TsMsb " << std::setw(5) << fvulStsCurrentTsMsb[fuCurrDpbIdx] << " new TsMsb "
              << std::setw(5) << uVal;
  }  // if( uVal + 1 != fvulStsCurrentTsMsb[fuCurrDpbIdx] && 4194303 != uVal && 0 != fvulStsCurrentTsMsb[fuCurrDpbIdx] )
  fvulStsCurrentTsMsb[fuCurrDpbIdx] = uVal;
  /*
   ULong64_t ulNewTsMsbTime =  static_cast< ULong64_t >( stsxyter::kuHitNbTsBins )
                             * static_cast< ULong64_t >( fvulStsCurrentTsMsb[fuCurrDpbIdx])
                             + static_cast< ULong64_t >( stsxyter::kulTsCycleNbBins )
                             * static_cast< ULong64_t >( fvuStsCurrentTsMsbCycle[fuCurrDpbIdx] );
*/
}

void CbmMcbm2018MonitorMcbmRate::FillStsEpochInfo(stsxyter::Message /*mess*/)
{
  //   UInt_t uVal    = mess.GetTsMsbVal();
}
/****************** STS Sync ******************************************/

/****************** TOF Sync ******************************************/

Bool_t CbmMcbm2018MonitorMcbmRate::ProcessTofMs(const fles::Timeslice& ts, size_t uMsComp, UInt_t uMsIdx)
{
  auto msDescriptor        = ts.descriptor(uMsComp, uMsIdx);
  fuCurrentEquipmentId     = msDescriptor.eq_id;
  fdTofMsIndex             = static_cast<double>(msDescriptor.idx);
  const uint8_t* msContent = reinterpret_cast<const uint8_t*>(ts.content(uMsComp, uMsIdx));

  uint32_t size = msDescriptor.size;
  //    fulLastMsIdx = msDescriptor.idx;
  if (size > 0) LOG(debug) << "Microslice: " << msDescriptor.idx << " has size: " << size;

  Int_t messageType = -111;

  // If not integer number of message in input buffer, print warning/error
  if (0 != (size % kuTofBytesPerMessage))
    LOG(error) << "The input microslice buffer does NOT "
               << "contain only complete nDPB messages!";

  // Compute the number of complete messages in the input microslice buffer
  uint32_t uNbMessages = (size - (size % kuTofBytesPerMessage)) / kuTofBytesPerMessage;

  // Get the gDPB ID from the MS header
  fuTofGdpbId = fuCurrentEquipmentId;
  //   fuTofGdpbNr = fmTofDpbIdIndexMap[fuTofGdpbId];

  /// Check if this gDPB ID was declared in parameter file and stop there if not
  auto it = fmTofDpbIdIndexMap.find(fuTofGdpbId);
  if (it == fmTofDpbIdIndexMap.end()) {
    LOG(info) << "---------------------------------------------------------------";
    /*
       LOG(info) << "hi hv eqid flag si sv idx/start        crc      size     offset";
       LOG(info) << Form( "%02x %02x %04x %04x %02x %02x %016llx %08x %08x %016llx",
                         static_cast<unsigned int>(msDescriptor.hdr_id),
                         static_cast<unsigned int>(msDescriptor.hdr_ver), msDescriptor.eq_id, msDescriptor.flags,
                         static_cast<unsigned int>(msDescriptor.sys_id),
                         static_cast<unsigned int>(msDescriptor.sys_ver), msDescriptor.idx, msDescriptor.crc,
                         msDescriptor.size, msDescriptor.offset );
*/
    LOG(info) << FormatMsHeaderPrintout(msDescriptor);
    LOG(warning) << "Could not find the gDPB index for AFCK id 0x" << std::hex << fuTofGdpbId << std::dec
                 << " in timeslice " << fulCurrentTsIdx << " in microslice " << uMsIdx << " component " << uMsComp
                 << "\n"
                 << "If valid this index has to be added in the TOF parameter "
                    "file in the RocIdArray field";
    return kFALSE;
  }  // if( it == fmTofDpbIdIndexMap.end() )
  else
    fuTofGdpbNr = fmTofDpbIdIndexMap[fuTofGdpbId];

  // Prepare variables for the loop on contents
  const uint64_t* pInBuff = reinterpret_cast<const uint64_t*>(msContent);
  for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx++) {
    // Fill message
    uint64_t ulData = static_cast<uint64_t>(pInBuff[uIdx]);
    gdpbv100::Message mess(ulData);

    /// Catch the Epoch cycle block which is always the first 64b of the MS
    if (0 == uIdx) {
      FillTofEpochCycle(ulData);
      continue;
    }  // if( 0 == uIdx )

    if (fair::Logger::Logging(fair::Severity::debug2)) {
      mess.printDataCout();
    }  // if (fair::Logger::Logging( fair::Severity::debug2 ))

    // Increment counter for different message types
    // and fill the corresponding histogram
    messageType = mess.getMessageType();
    fviTofMsgCounter[messageType]++;

    switch (messageType) {
      case gdpbv100::MSG_HIT: {
        if (!mess.getGdpbHitIs24b()) { fvmTofEpSupprBuffer[fuTofGdpbNr].push_back(mess); }  // if( !getGdpbHitIs24b() )
        break;
      }  // case gdpbv100::MSG_HIT:
      case gdpbv100::MSG_EPOCH: {
        if (gdpbv100::kuChipIdMergedEpoch == mess.getGdpbGenChipId()) {
          FillTofEpochInfo(mess);
        }  // if this epoch message is a merged one valiud for all chips
        else
          LOG(fatal) << "Bad epoch: " << mess.getGdpbGenChipId();
        break;
      }  // case gdpbv100::MSG_EPOCH:
      case gdpbv100::MSG_SLOWC:
      case gdpbv100::MSG_SYST:
      case gdpbv100::MSG_STAR_TRI_A:
      case gdpbv100::MSG_STAR_TRI_B:
      case gdpbv100::MSG_STAR_TRI_C:
      case gdpbv100::MSG_STAR_TRI_D: break;
      default:
        LOG(error) << "Message type " << std::hex << std::setw(2) << static_cast<uint16_t>(messageType)
                   << " not included in Get4 unpacker.";
    }  // switch( mess.getMessageType() )
  }    // for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx ++)

  return kTRUE;
}

void CbmMcbm2018MonitorMcbmRate::FillTofEpochCycle(uint64_t ulCycleData)
{
  uint64_t ulEpochCycleVal              = ulCycleData & gdpbv100::kulEpochCycleFieldSz;
  fvulTofCurrentEpochCycle[fuTofGdpbNr] = ulEpochCycleVal;

  return;
}

void CbmMcbm2018MonitorMcbmRate::FillTofHitInfo(gdpbv100::Message mess)
{
  //   UInt_t uChannel = mess.getGdpbHitChanId();
  //   UInt_t uTot     = mess.getGdpbHit32Tot();
  //   UInt_t uFts     = mess.getGdpbHitFineTs();

  ULong64_t ulCurEpochGdpbGet4 = fvulTofCurrentEpochFull[fuTofGdpbNr];

  // In Ep. Suppr. Mode, receive following epoch instead of previous
  if (0 < ulCurEpochGdpbGet4) ulCurEpochGdpbGet4--;
  else
    ulCurEpochGdpbGet4 = gdpbv100::kuEpochCounterSz;  // Catch epoch cycle!

  //   ULong_t  ulHitTime = mess.getMsgFullTime(ulCurEpochGdpbGet4);
  //   Double_t dHitTime  = mess.getMsgFullTimeD(ulCurEpochGdpbGet4);

  // In 32b mode the coarse counter is already computed back to 112 FTS bins
  // => need to hide its contribution from the Finetime
  // => FTS = Fullt TS modulo 112
  //   uFts = mess.getGdpbHitFullTs() % 112;

  /// system sync check buffering
  fvmTofGdpbHitsInMs[fuTofGdpbNr].push_back(gdpbv100::FullMessage(mess, ulCurEpochGdpbGet4));
}

void CbmMcbm2018MonitorMcbmRate::FillTofEpochInfo(gdpbv100::Message mess)
{
  ULong64_t ulEpochNr = mess.getGdpbEpEpochNb();

  fvulTofCurrentEpoch[fuTofGdpbNr] = ulEpochNr;
  fvulTofCurrentEpochFull[fuTofGdpbNr] =
    ulEpochNr + gdpbv100::kulEpochCycleBins * fvulTofCurrentEpochCycle[fuTofGdpbNr];

  fulTofCurrentEpochTime = mess.getMsgFullTime(ulEpochNr);

  /// Re-align the epoch number of the message in case it will be used later:
  /// We received the epoch after the data instead of the one before!
  if (0 < ulEpochNr) mess.setGdpbEpEpochNb(ulEpochNr - 1);
  else
    mess.setGdpbEpEpochNb(gdpbv100::kuEpochCounterSz);

  Int_t iBufferSize = fvmTofEpSupprBuffer[fuTofGdpbNr].size();
  if (0 < iBufferSize) {
    LOG(debug) << "Now processing stored messages for for gDPB " << fuTofGdpbNr << " with epoch number "
               << (fvulTofCurrentEpoch[fuTofGdpbNr] - 1);

    /// Data are sorted between epochs, not inside => Epoch level ordering
    /// Sorting at lower bin precision level
    std::stable_sort(fvmTofEpSupprBuffer[fuTofGdpbNr].begin(), fvmTofEpSupprBuffer[fuTofGdpbNr].begin());

    for (Int_t iMsgIdx = 0; iMsgIdx < iBufferSize; iMsgIdx++) {
      FillTofHitInfo(fvmTofEpSupprBuffer[fuTofGdpbNr][iMsgIdx]);
    }  // for( Int_t iMsgIdx = 0; iMsgIdx < iBufferSize; iMsgIdx++ )

    fvmTofEpSupprBuffer[fuTofGdpbNr].clear();
  }  // if( 0 < fvmTofEpSupprBuffer[fuTofGdpbNr] )
}
/****************** TOF Sync ******************************************/

/****************** STS histos ****************************************/
/****************** STS histos ****************************************/

/****************** TOF histos ****************************************/
/****************** TOF Histos ****************************************/

void CbmMcbm2018MonitorMcbmRate::Reset() {}

void CbmMcbm2018MonitorMcbmRate::Finish()
{
  /*
   LOG(info) << "-------------------------------------";
   LOG(info) << "CbmMcbm2018MonitorMcbmRate statistics are ";
   LOG(info) << " Hit      messages: " << fmMsgCounter[ stsxyter::MessType::Hit   ] << "\n"
             << " Ts MSB   messages: " << fmMsgCounter[ stsxyter::MessType::TsMsb ] << "\n"
             << " Dummy    messages: " << fmMsgCounter[ stsxyter::MessType::Dummy ] << "\n"
             << " Epoch    messages: " << fmMsgCounter[ stsxyter::MessType::Epoch ] << "\n"
             << " Empty    messages: " << fmMsgCounter[ stsxyter::MessType::Empty ];
*/
  LOG(info) << "-------------------------------------";

  SaveAllHistos(fsHistoFileFullname);
  SaveAllHistos();
}


void CbmMcbm2018MonitorMcbmRate::SaveAllHistos(TString sFileName)
{
  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  TFile* histoFile = nullptr;
  if ("" != sFileName) {
    // open separate histo file in recreate mode
    histoFile = new TFile(sFileName, "RECREATE");
    histoFile->cd();
  }  // if( "" != sFileName )

  /****************** STS Sync ******************************************/
  /****************** STS Sync ******************************************/

  /****************** TOF Sync ******************************************/
  /****************** TOF Sync ******************************************/

  /****************** mCBM Sync *****************************************/
  gDirectory->mkdir("mCbmRate");
  gDirectory->cd("mCbmRate");
  fhMcbmHitsNbPerTsEvo->Write();
  fhMcbmHitsNbFineEvo->Write();
  fhMcbmHitsRateEvo->Write();
  fhDiamondHitsRateMapEvo->Write();
  fhDiamondHitsRateDerivative->Write();
  gDirectory->cd("..");

  gDirectory->mkdir("mCbmCounts");
  gDirectory->cd("mCbmCounts");
  fhMuchVsStsHitsNbPerTimeBin->Write();
  fhTofVsStsHitsNbPerTimeBin->Write();
  fhDiamVsStsHitsNbPerTimeBin->Write();
  fhStsVsMuchHitsNbPerTimeBin->Write();
  fhTofVsMuchHitsNbPerTimeBin->Write();
  fhDiamVsMuchHitsNbPerTimeBin->Write();
  fhStsVsTofHitsNbPerTimeBin->Write();
  fhMuchVsTofHitsNbPerTimeBin->Write();
  fhDiamVsTofHitsNbPerTimeBin->Write();
  fhStsVsDiamHitsNbPerTimeBin->Write();
  fhTofVsDiamHitsNbPerTimeBin->Write();
  fhMuchVsDiamHitsNbPerTimeBin->Write();
  gDirectory->cd("..");
  /****************** mCBM Sync *****************************************/

  if ("" != sFileName) {
    // Restore original directory position
    histoFile->Close();
  }  // if( "" != sFileName )

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;
}
void CbmMcbm2018MonitorMcbmRate::ResetAllHistos()
{
  /****************** STS Sync ******************************************/
  /****************** STS Sync ******************************************/

  /****************** TOF Sync ******************************************/

  /****************** TOF Sync ******************************************/

  /****************** mCBM Sync *****************************************/
  fhMcbmHitsNbPerTsEvo->Reset();
  fhMcbmHitsNbFineEvo->Reset();
  fhMcbmHitsRateEvo->Reset();
  fhDiamondHitsRateMapEvo->Reset();
  fhDiamondHitsRateDerivative->Reset();

  fhMuchVsStsHitsNbPerTimeBin->Reset();
  fhTofVsStsHitsNbPerTimeBin->Reset();
  fhDiamVsStsHitsNbPerTimeBin->Reset();
  fhStsVsMuchHitsNbPerTimeBin->Reset();
  fhTofVsMuchHitsNbPerTimeBin->Reset();
  fhDiamVsMuchHitsNbPerTimeBin->Reset();
  fhStsVsTofHitsNbPerTimeBin->Reset();
  fhMuchVsTofHitsNbPerTimeBin->Reset();
  fhDiamVsTofHitsNbPerTimeBin->Reset();
  fhStsVsDiamHitsNbPerTimeBin->Reset();
  fhTofVsDiamHitsNbPerTimeBin->Reset();
  fhMuchVsDiamHitsNbPerTimeBin->Reset();
  /****************** mCBM Sync *****************************************/
}

ClassImp(CbmMcbm2018MonitorMcbmRate)
