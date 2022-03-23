/* Copyright (C) 2021 Variable Energy Cyclotron Centre, Kolkata
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Vikas Singhal [committer] */

#include "CbmMcbm2018MonitorAlgoMuchLite.h"

// Data
//
// // CbmRoot
#include "CbmFormatMsHeaderPrintout.h"
#include "CbmHistManager.h"
#include "CbmMcbm2018MuchPar.h"

// // FairRoot
#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"
//
// // Root
#include "TClonesArray.h"
#include "THttpServer.h"
#include "TMath.h"
#include "TProfile.h"
#include "TROOT.h"
#include "TRandom.h"
#include "TString.h"
#include "TStyle.h"
#include <TFile.h>

// C++11
#include <bitset>

// C/C++
#include <iomanip>
#include <iostream>

#include <stdint.h>


/*Bool_t bMcbmResetAlgoMuchLite = kFALSE;
 Bool_t bMcbm2018WriteAlgoMuchLite = kFALSE;
 Bool_t bMcbm2018ScanNoisyAlgoMuchLite = kFALSE;*/

CbmMcbm2018MonitorAlgoMuchLite::CbmMcbm2018MonitorAlgoMuchLite()
  : CbmStar2019Algo()
  , fbMuchMode(kFALSE)
  , fvbMaskedComponents()
  , fvmHitsInMs()
  , fsHistoFilename("data/HistosMonitorMuch.root")
  , fbPrintMessages(kFALSE)
  , fPrintMessCtrl(stsxyter::MessagePrintMask::msg_print_Human)
  , fulCurrentTsIdx(0)
  , fuNbCoreMsPerTs(0)
  , fuNrOfDpbs(0)
  , fDpbIdIndexMap()
  , fvbCrobActiveFlag()
  , fuNbFebs(0)
  , fuNbStsXyters(0)
  , fmMsgCounter()
  , fUnpackParMuch(NULL)
  , fuCurrentEquipmentId(0)
  , fuCurrDpbId(0)
  , fuCurrDpbIdx(0)
  , fiRunStartDateTimeSec(-1)
  , fiBinSizeDatePlots(-1)
  , fvulCurrentTsMsb()
  , fvuCurrentTsMsbCycle()
  , fvuInitialHeaderDone()
  , fvuInitialTsMsbCycleHeader()
  , fvuElinkLastTsHit()
  , fvulChanLastHitTime()
  , fvdChanLastHitTime()
  , fvdPrevMsTime()
  , fvdMsTime()
  , fvdChanLastHitTimeInMs()
  , fvusChanLastHitAdcInMs()
  , fvmAsicHitsInMs()
  , fvmFebHitsInMs()
  , fiTimeIntervalRateUpdate(10)
  , fviFebTimeSecLastRateUpdate()
  , fviFebCountsSinceLastRateUpdate()
  , fvdFebChanCountsSinceLastRateUpdate()
  , Counter(0)
  , Counter1(0)
  , fHM(new CbmHistManager())
  , fhMuchMessType(NULL)
  , fhMuchSysMessType(NULL)
  , fhMuchFebChanAdcRaw_combined(NULL)
  , fhMuchMessTypePerDpb(NULL)
  , fhMuchSysMessTypePerDpb(NULL)
  , fhStatusMessType(NULL)
  , fhMsStatusFieldType(NULL)
  , fhMuchHitsElinkPerDpb(NULL)
  , fhRate(NULL)
  , fhRateAdcCut(NULL)
  , fHistPadDistr()
  , fRealHistPadDistr()
  , fhMuchFebChanCntRaw()
  , fhMuchFebChanAdcRaw()
  , fhMuchFebChanAdcRawProf()
  , fhMuchFebChanRawTs()
  , fhMuchFebChanHitRateEvo()
  , fhMuchFebChanHitRateProf()
  , fhMuchFebHitRateEvo()
  , fhMuchFebHitRateEvo_mskch()
  , fhMuchFebHitRateEvo_mskch_adccut()
  , fhMuchFebHitRateEvo_WithoutDupli()
  , fdMuchFebChanLastTimeForDist()
  , fhMuchFebChanDistT()
  , fhMuchFebDuplicateHitProf()
  , fcMsSizeAll(NULL)
  , fvMsComponentsList()
  , fuMaxNbMicroslices(100)
  , fvuChanNbHitsInMs()
  , fhElinkIdxHit(NULL)
  , fuNbOverMsPerTs(0)
//  , fhElinkIdxTsMsb(NULL)
//  , fhElinkIdxEpoch(NULL)
//  , fhElinkIdxStatus(NULL)
{
}

CbmMcbm2018MonitorAlgoMuchLite::~CbmMcbm2018MonitorAlgoMuchLite() {}
Bool_t CbmMcbm2018MonitorAlgoMuchLite::Init()
{
  LOG(info) << "Initializing flib StsXyter unpacker for MUCH";

  FairRootManager* ioman = FairRootManager::Instance();
  if (ioman == NULL) { LOG(fatal) << "No FairRootManager instance"; }

  return kTRUE;
}

void CbmMcbm2018MonitorAlgoMuchLite::Reset() {}

void CbmMcbm2018MonitorAlgoMuchLite::Finish()
{
  //fhElinkIdx->Draw();
  LOG(info) << "-------------------------------------";
  LOG(info) << "CbmMcbm2018MonitorAlgoMuchLite statistics are ";
  LOG(info) << " Hit      messages: " << fmMsgCounter[stsxyter::MessType::Hit] << "\n"
            << " Ts MSB   messages: " << fmMsgCounter[stsxyter::MessType::TsMsb] << "\n"
            << " Dummy    messages: " << fmMsgCounter[stsxyter::MessType::Dummy] << "\n"
            << " Epoch    messages: " << fmMsgCounter[stsxyter::MessType::Epoch] << "\n"
            << " Empty    messages: " << fmMsgCounter[stsxyter::MessType::Empty];

  LOG(info) << "-------------------------------------";

  //SaveAllHistos(fsHistoFileFullname);
  //SaveAllHistos();
}

Bool_t CbmMcbm2018MonitorAlgoMuchLite::InitContainers()
{
  LOG(info) << "Init parameter containers for CbmMcbm2018MonitorAlgoMuchLite";

  Bool_t bInit = InitMuchParameters();
  //LOG(info) << "bInit ok.";
  //if (kTRUE == bInit) CreateHistograms();
  //fhElinkIdxHit = new TH1I("ElinkIdxHit", "Active Elink Ids for Hit Messages", 42, 0,41);
  //fhElinkIdxTsMsb = new TH1I("ElinkIdxTsMsb", "Active Elink Ids for TsMsb", 42, 0,41);
  //fhElinkIdxEpoch = new TH1I("ElinkIdxEpoch", "Active Elink Ids for Epoch", 42, 0,41);
  //fhElinkIdxStatus = new TH1I("ElinkIdxStatus", "Active Elink Ids for Status", 42, 0,41);

  return bInit;
}

Bool_t CbmMcbm2018MonitorAlgoMuchLite::ReInitContainers()
{
  LOG(info) << "ReInit parameter containers for CbmMcbm2018MonitorAlgoMuchLite";

  return InitMuchParameters();
}

TList* CbmMcbm2018MonitorAlgoMuchLite::GetParList()
{
  if (nullptr == fParCList) fParCList = new TList();
  fUnpackParMuch = new CbmMcbm2018MuchPar("CbmMcbm2018MuchPar");
  fParCList->Add(fUnpackParMuch);

  return fParCList;
}

Bool_t CbmMcbm2018MonitorAlgoMuchLite::InitMuchParameters()
{

  fUnpackParMuch = (CbmMcbm2018MuchPar*) fParCList->FindObject("CbmMcbm2018MuchPar");
  if (nullptr == fUnpackParMuch) {
    LOG(info) << "Much parameters could not be initialized";
    return kFALSE;
  }

  fuNrOfDpbs = fUnpackParMuch->GetNrOfDpbs();
  LOG(info) << "Nr. of MUCH DPBs:       " << fuNrOfDpbs;

  fDpbIdIndexMap.clear();
  for (UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb) {
    fDpbIdIndexMap[fUnpackParMuch->GetDpbId(uDpb)] = uDpb;
    LOG(info) << "Eq. ID for DPB #" << std::setw(2) << uDpb << " = 0x" << std::setw(4) << std::hex
              << fUnpackParMuch->GetDpbId(uDpb) << std::dec << " => " << fDpbIdIndexMap[fUnpackParMuch->GetDpbId(uDpb)];
  }  // for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )

  fuNbFebs = fUnpackParMuch->GetNrOfFebs();
  LOG(info) << "Nr. of FEBs:           " << fuNbFebs;

  fuNbStsXyters = fUnpackParMuch->GetNrOfAsics();
  LOG(info) << "Nr. of StsXyter ASICs: " << fuNbStsXyters;

  fvbCrobActiveFlag.resize(fuNrOfDpbs);
  //fvdFebAdcGain.resize(     fuNrOfDpbs );
  //  //fvdFebAdcOffs.resize(     fuNrOfDpbs );
  for (UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb) {
    fvbCrobActiveFlag[uDpb].resize(fUnpackParMuch->GetNbCrobsPerDpb());
    //fvdFebAdcGain[ uDpb ].resize(        fUnpackParMuch->GetNbCrobsPerDpb() );
    //    //fvdFebAdcOffs[ uDpb ].resize(        fUnpackParMuch->GetNbCrobsPerDpb() );
    for (UInt_t uCrobIdx = 0; uCrobIdx < fUnpackParMuch->GetNbCrobsPerDpb(); ++uCrobIdx) {
      fvbCrobActiveFlag[uDpb][uCrobIdx] = fUnpackParMuch->IsCrobActive(uDpb, uCrobIdx);
      // fvdFebAdcGain[ uDpb ][ uCrobIdx ].resize(     fUnpackParMuch->GetNbFebsPerCrob(), 0.0 );
      //       //fvdFebAdcOffs[ uDpb ][ uCrobIdx ].resize(     fUnpackParMuch->GetNbFebsPerCrob(), 0.0 );
    }  // for( UInt_t uCrobIdx = 0; uCrobIdx < fUnpackParMuch->GetNbCrobsPerDpb(); ++uCrobIdx )
  }    // for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )

  for (UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb) {
    for (UInt_t uCrobIdx = 0; uCrobIdx < fUnpackParMuch->GetNbCrobsPerDpb(); ++uCrobIdx) {
      LOG(info) << Form("DPB #%02u CROB #%02u Active:  ", uDpb, uCrobIdx) << fvbCrobActiveFlag[uDpb][uCrobIdx];
    }  // for( UInt_t uCrobIdx = 0; uCrobIdx < fUnpackParMuch->GetNbCrobsPerDpb(); ++uCrobIdx )
  }    // for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )

  if (fbBinningFw) LOG(info) << "Unpacking data in bin sorter FW mode";
  else
    LOG(info) << "Unpacking data in full time sorter FW mode (legacy)";

  // Internal status initialization
  LOG(info) << "Internal status initialization";
  fvulCurrentTsMsb.resize(fuNrOfDpbs);
  fvuCurrentTsMsbCycle.resize(fuNrOfDpbs);
  fvuInitialHeaderDone.resize(fuNrOfDpbs);
  fvuInitialTsMsbCycleHeader.resize(fuNrOfDpbs);
  fvuElinkLastTsHit.resize(fuNrOfDpbs);
  for (UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb) {
    fvulCurrentTsMsb[uDpb]           = 0;
    fvuCurrentTsMsbCycle[uDpb]       = 0;
    fvuInitialHeaderDone[uDpb]       = kFALSE;
    fvuInitialTsMsbCycleHeader[uDpb] = 0;
  }  // for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )
  LOG(info) << "ongoing internal status initialization";

  fvdPrevMsTime.resize(kiMaxNbFlibLinks);
  fvulChanLastHitTime.resize(fuNbStsXyters);
  fvdChanLastHitTime.resize(fuNbStsXyters);
  fvdMsTime.resize(fuMaxNbMicroslices);
  fvuChanNbHitsInMs.resize(fuNbFebs);
  fvdChanLastHitTimeInMs.resize(fuNbFebs);
  fvusChanLastHitAdcInMs.resize(fuNbFebs);
  fvmAsicHitsInMs.resize(fuNbFebs);

  //fvdMsTime.resize( fuMaxNbMicroslices );
  //fvuChanNbHitsInMs.resize( fuNbStsXyters );
  //fvdChanLastHitTimeInMs.resize( fuNbStsXyters );
  //fvusChanLastHitAdcInMs.resize( fuNbStsXyters );
  //fvmAsicHitsInMs.resize( fuNbStsXyters );

  for (UInt_t uXyterIdx = 0; uXyterIdx < fuNbFebs; ++uXyterIdx) {
    fvulChanLastHitTime[uXyterIdx].resize(fUnpackParMuch->GetNbChanPerAsic());
    fvdChanLastHitTime[uXyterIdx].resize(fUnpackParMuch->GetNbChanPerAsic());
    fvuChanNbHitsInMs[uXyterIdx].resize(fUnpackParMuch->GetNbChanPerAsic());
    fvdChanLastHitTimeInMs[uXyterIdx].resize(fUnpackParMuch->GetNbChanPerAsic());
    fvusChanLastHitAdcInMs[uXyterIdx].resize(fUnpackParMuch->GetNbChanPerAsic());
    fvmAsicHitsInMs[uXyterIdx].clear();

    for (UInt_t uChan = 0; uChan < fUnpackParMuch->GetNbChanPerAsic(); ++uChan) {
      fvulChanLastHitTime[uXyterIdx][uChan] = 0;
      fvdChanLastHitTime[uXyterIdx][uChan]  = -1.0;

      fvuChanNbHitsInMs[uXyterIdx][uChan].resize(fuMaxNbMicroslices);
      fvdChanLastHitTimeInMs[uXyterIdx][uChan].resize(fuMaxNbMicroslices);
      fvusChanLastHitAdcInMs[uXyterIdx][uChan].resize(fuMaxNbMicroslices);
      for (UInt_t uMsIdx = 0; uMsIdx < fuMaxNbMicroslices; ++uMsIdx) {
        fvuChanNbHitsInMs[uXyterIdx][uChan][uMsIdx]      = 0;
        fvdChanLastHitTimeInMs[uXyterIdx][uChan][uMsIdx] = -1.0;
        fvusChanLastHitAdcInMs[uXyterIdx][uChan][uMsIdx] = 0;
      }  // for( UInt_t uMsIdx = 0; uMsIdx < fuMaxNbMicroslices; ++uMsIdx )
    }    // for( UInt_t uChan = 0; uChan < fUnpackParMuch->GetNbChanPerAsic(); ++uChan )
  }      // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )

  LOG(info) << "CbmMcbm2018MonitorAlgoMuchLite::ReInitContainers => Changed "
               "fvuChanNbHitsInMs size "
            << fvuChanNbHitsInMs.size() << " VS " << fuNbFebs;
  LOG(info) << "CbmMcbm2018MonitorAlgoMuchLite::ReInitContainers =>  Changed "
               "fvuChanNbHitsInMs size "
            << fvuChanNbHitsInMs[0].size() << " VS " << fUnpackParMuch->GetNbChanPerAsic();
  LOG(info) << "CbmMcbm2018MonitorAlgoMuchLite::ReInitContainers =>  Changed "
               "fvuChanNbHitsInMs size "
            << fvuChanNbHitsInMs[0][0].size() << " VS " << fuMaxNbMicroslices;

  fvmFebHitsInMs.resize(fuNbFebs);
  fviFebTimeSecLastRateUpdate.resize(fuNbFebs, -1);
  fviFebCountsSinceLastRateUpdate.resize(fuNbFebs, -1);
  fvdFebChanCountsSinceLastRateUpdate.resize(fuNbFebs);
  fdMuchFebChanLastTimeForDist.resize(fuNbFebs);
  for (UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx) {
    fvmFebHitsInMs[uFebIdx].clear();
    fvdFebChanCountsSinceLastRateUpdate[uFebIdx].resize(fUnpackParMuch->GetNbChanPerFeb(), 0.0);
    fdMuchFebChanLastTimeForDist[uFebIdx].resize(fUnpackParMuch->GetNbChanPerFeb(), -1.0);
  }  // for( UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx )

  ///----------------- SXM 2.0 Logic Error Tagging --------------------///
  //   SmxErrInitializeVariables();
  ///------------------------------------------------------------------///

  return kTRUE;
}

/* void CbmMcbm2018MonitorAlgoMuchLite::SetNbMsInTs(size_t uCoreMsNb,
		                                             size_t uOverlapMsNb) {
	  fuNbCoreMsPerTs = uCoreMsNb;
	  fuNbOverMsPerTs = uOverlapMsNb;
	  //LOG(info) <<" fuNbCoreMsPerTs "<<fuNbCoreMsPerTs<<" fuNbOverMsPerTs "<<fuNbOverMsPerTs;
	UInt_t uNbMsTotal = fuNbCoreMsPerTs + fuNbOverMsPerTs;

	  if (fuMaxNbMicroslices < uNbMsTotal) {
	fuMaxNbMicroslices = uNbMsTotal;
	fvdMsTime.resize(fuMaxNbMicroslices);
	fvuChanNbHitsInMs.resize(fuNbStsXyters);
	fvdChanLastHitTimeInMs.resize(fuNbStsXyters);
	fvusChanLastHitAdcInMs.resize(fuNbStsXyters);
	for (UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx) {
	fvuChanNbHitsInMs[uXyterIdx].resize(fUnpackParMuch->GetNbChanPerAsic());
	fvdChanLastHitTimeInMs[uXyterIdx].resize(
	fUnpackParMuch->GetNbChanPerAsic());
	fvusChanLastHitAdcInMs[uXyterIdx].resize(
	fUnpackParMuch->GetNbChanPerAsic());
	for (UInt_t uChan = 0; uChan < fUnpackParMuch->GetNbChanPerAsic();
	++uChan) {
	fvuChanNbHitsInMs[uXyterIdx][uChan].resize(fuMaxNbMicroslices);
	fvdChanLastHitTimeInMs[uXyterIdx][uChan].resize(fuMaxNbMicroslices);
	fvusChanLastHitAdcInMs[uXyterIdx][uChan].resize(fuMaxNbMicroslices);
	for (UInt_t uMsIdx = 0; uMsIdx < fuMaxNbMicroslices; ++uMsIdx) {
	fvuChanNbHitsInMs[uXyterIdx][uChan][uMsIdx]      = 0;
	fvdChanLastHitTimeInMs[uXyterIdx][uChan][uMsIdx] = -1.0;
	fvusChanLastHitAdcInMs[uXyterIdx][uChan][uMsIdx] = 0;
	LOG(info) << "CbmMcbm2018MonitorMuchLite::DoUnpack => Changed "
		                 "fvuChanNbHitsInMs size "
				               << fvuChanNbHitsInMs.size() << " VS " << fuNbStsXyters;
	LOG(info) << "CbmMcbm2018MonitorMuchLite::DoUnpack =>  Changed "
		                     "fvuChanNbHitsInMs size "
				                   << fvuChanNbHitsInMs[0].size() << " VS "
						                 << fUnpackParMuch->GetNbChanPerAsic();
	LOG(info) << "CbmMcbm2018MonitorMuchLite::DoUnpack =>  Changed "
			                 "fvuChanNbHitsInMs size "
					               << fvuChanNbHitsInMs[0][0].size() << " VS " << fuMaxNbMicroslices;
		  }  // if( fuMaxNbMicroslices < uNbMsTotal )
		}
	}
	  }
} */


void CbmMcbm2018MonitorAlgoMuchLite::AddMsComponentToList(size_t component, UShort_t /*usDetectorId*/)
{
  /// Check for duplicates and ignore if it is the case
  for (UInt_t uCompIdx = 0; uCompIdx < fvMsComponentsList.size(); ++uCompIdx)
    if (component == fvMsComponentsList[uCompIdx]) return;

  /// Check if this does not go above hardcoded limits
  if (kiMaxNbFlibLinks <= component) {
    LOG(error) << "CbmMcbm2018MonitorAlgoMuchLite::AddMsComponentToList => "
               << "Ignored the addition of component " << component << " as it is above the hadcoded limit of "
               << static_cast<Int_t>(kiMaxNbFlibLinks) << " !!!!!!!!! "
               << "\n"
               << "         To change this behavior check kiMaxNbFlibLinks in "
                  "CbmMcbm2018MonitorAlgoMuchLite.cxx";
    return;
  }  // if( kiMaxNbFlibLinks <= component  )


  /// Add to list
  fvMsComponentsList.push_back(component);
  LOG(info) << "CbmMcbm2018MonitorAlgoMuchLite::AddMsComponentToList => Added component: " << component;

  /// Create MS size monitoring histos
  if (NULL == fhMsSz[component]) {
    TString sMsSzName  = Form("MsSz_link_%02lu", component);
    TString sMsSzTitle = Form("Size of MS for nDPB of link %02lu; Ms Size [bytes]", component);
    fhMsSz[component]  = new TH1F(sMsSzName.Data(), sMsSzTitle.Data(), 160000, 0., 20000.);
    fHM->Add(sMsSzName.Data(), fhMsSz[component]);

    sMsSzName             = Form("MsSzTime_link_%02lu", component);
    sMsSzTitle            = Form("Size of MS vs time for gDPB of link %02lu; Time[s] ; Ms Size [bytes]", component);
    fhMsSzTime[component] = new TProfile(sMsSzName.Data(), sMsSzTitle.Data(), 15000, 0., 300.);
    fHM->Add(sMsSzName.Data(), fhMsSzTime[component]);

    if (NULL != fcMsSizeAll) {
      fcMsSizeAll->cd(1 + component);
      gPad->SetLogy();
      fhMsSzTime[component]->Draw("hist le0");
    }  // if( NULL != fcMsSizeAll )
    LOG(info) << "Added MS size histo for component: " << component << " (DPB)";

    //  THttpServer* server = FairRunOnline::Instance()->GetHttpServer();
    //  if (server) {
    //server->Register("/FlibRaw", fhMsSz[component]); //closed by me
    AddHistoToVector(fhMsSz[component], "FlibRaw");
    //server->Register("/FlibRaw", fhMsSzTime[component]);// closed by me
    AddHistoToVector(fhMsSzTime[component], "FlibRaw");
    // }  // if( server )
  }  // if( NULL == fhMsSz[ component ] )
}


Bool_t CbmMcbm2018MonitorAlgoMuchLite::CreateHistograms()
{
  TString sHistName {""};
  TString title {""};

  sHistName      = "hMessageType";
  title          = "Nb of message for each type; Type";
  fhMuchMessType = new TH1I(sHistName, title, 6, 0., 6.);
  fhMuchMessType->GetXaxis()->SetBinLabel(1, "Dummy");
  fhMuchMessType->GetXaxis()->SetBinLabel(2, "Hit");
  fhMuchMessType->GetXaxis()->SetBinLabel(3, "TsMsb");
  fhMuchMessType->GetXaxis()->SetBinLabel(4, "Epoch");
  fhMuchMessType->GetXaxis()->SetBinLabel(5, "Status");
  fhMuchMessType->GetXaxis()->SetBinLabel(6, "Empty");


  sHistName         = "hSysMessType";
  title             = "Nb of system message for each type; System Type";
  fhMuchSysMessType = new TH1I(sHistName, title, 17, 0., 17.);

  sHistName = "hMuchFebChanAdcRaw_combined";
  title     = "ADC hist combined";
  fhMuchFebChanAdcRaw_combined =
    new TH1I(sHistName, title, stsxyter::kuHitNbAdcBins, -0.5, stsxyter::kuHitNbAdcBins - 0.5);

  LOG(debug) << "Initialized 1st Histo";
  sHistName            = "hMessageTypePerDpb";
  title                = "Nb of message of each type for each DPB; DPB; Type";
  fhMuchMessTypePerDpb = new TH2I(sHistName, title, fuNrOfDpbs, 0, fuNrOfDpbs, 6, 0., 6.);
  fhMuchMessTypePerDpb->GetYaxis()->SetBinLabel(1, "Dummy");
  fhMuchMessTypePerDpb->GetYaxis()->SetBinLabel(2, "Hit");
  fhMuchMessTypePerDpb->GetYaxis()->SetBinLabel(3, "TsMsb");
  fhMuchMessTypePerDpb->GetYaxis()->SetBinLabel(4, "Epoch");
  fhMuchMessTypePerDpb->GetYaxis()->SetBinLabel(5, "Status");
  fhMuchMessTypePerDpb->GetYaxis()->SetBinLabel(6, "Empty");

  sHistName              = "hMuchMessTypePerElink";
  title                  = "Nb of message of each type for each Elink; Elink; Type";
  fhMuchMessTypePerElink = new TH2I(sHistName, title, fuNrOfDpbs * fUnpackParMuch->GetNbElinkPerCrob(), 0,
                                    fuNrOfDpbs * fUnpackParMuch->GetNbElinkPerCrob(), 6, 0., 6.);
  fhMuchMessTypePerElink->GetYaxis()->SetBinLabel(1, "Dummy");
  fhMuchMessTypePerElink->GetYaxis()->SetBinLabel(2, "Hit");
  fhMuchMessTypePerElink->GetYaxis()->SetBinLabel(3, "TsMsb");
  fhMuchMessTypePerElink->GetYaxis()->SetBinLabel(4, "Epoch");
  fhMuchMessTypePerElink->GetYaxis()->SetBinLabel(5, "Status");
  fhMuchMessTypePerElink->GetYaxis()->SetBinLabel(6, "Empty");


  fhElinkIdxHit = new TH1I("ElinkIdxHit", "Active Elink Ids for Hit Messages", 42, 0, 41);

  for (UInt_t uModuleId = 0; uModuleId < 2; ++uModuleId) {
    /// Raw Ts Distribution
    sHistName = Form("HistPadDistr_Module_%01u", uModuleId);
    title     = Form("Pad distribution for, Module #%01u; ", uModuleId);

    //Below for Rectangular Module shape VS
    fHistPadDistr.push_back(new TH2I(sHistName, title, 23, -0.5, 22.5, 97, -0.5, 96.5));

    sHistName = Form("RealHistPadDistr_Module_%01u", uModuleId);
    title     = Form("Progressive Pad distribution for, Module #%01u; ", uModuleId);
    //Below for Progressive Geometry Module shape VS
    fRealHistPadDistr.push_back(new TH2D(sHistName, title, 500, -0.5, 499.5, 1000, -0.5, 999.5));


    /// FEB wise Duplicate Hit profile for each Module (If same hit at same time, same channel and same FEB)
    sHistName = Form("hMuchFebDuplicateHitProf_%01u", uModuleId);
    title     = Form("FEB wise Duplicate Hit for Module #%01u; FEB []; Hit []", uModuleId);
    if (uModuleId == 0)
      //fhMuchFebDuplicateHitProf.push_back( new TProfile(sHistName, title,fUnpackParMuch->GetNrOfFebsInGemA(), -0.5, fUnpackParMuch->GetNrOfFebsInGemA() - 0.5 ) );
      fhMuchFebDuplicateHitProf.push_back(new TProfile(sHistName, title, 18, -0.5, 18 - 0.5));
    if (uModuleId == 1)
      //fhMuchFebDuplicateHitProf.push_back( new TProfile(sHistName, title,fUnpackParMuch->GetNrOfFebsInGemB(), -0.5, fUnpackParMuch->GetNrOfFebsInGemB() - 0.5 ) );
      fhMuchFebDuplicateHitProf.push_back(new TProfile(sHistName, title, 18, -0.5, 18 - 0.5));
  }

  sHistName = "hRate";
  title     = "Rate in kHz";
  fhRate    = new TH1I(sHistName, title, 10000, -0.5, 9999.5);

  sHistName    = "hRateAdcCut";
  title        = "Rate in kHz with Adc cut";
  fhRateAdcCut = new TH1I(sHistName, title, 10000, -0.5, 9999.5);


  sHistName  = "hFEBcount";
  title      = "Count vs FEB number; FEB Number; Count";
  fhFEBcount = new TH1I(sHistName, title, 40, -0.5, 39.5);


  sHistName               = "hSysMessTypePerDpb";
  title                   = "Nb of system message of each type for each DPB; DPB; System Type";
  fhMuchSysMessTypePerDpb = new TH2I(sHistName, title, fuNrOfDpbs, 0, fuNrOfDpbs, 17, 0., 17.);

  sHistName        = "hStatusMessType";
  title            = "Nb of status message of each type for each DPB; ASIC; Status Type";
  fhStatusMessType = new TH2I(sHistName, title, fuNbStsXyters, 0, fuNbStsXyters, 16, 0., 16.);


  sHistName           = "hMsStatusFieldType";
  title               = "For each flag in the MS header, ON/OFF counts; Flag bit []; ON/OFF; MS []";
  fhMsStatusFieldType = new TH2I(sHistName, title, 16, -0.5, 15.5, 2, -0.5, 1.5);

  //For mCBM March 2019 data taking we will have only one eLink enable for each FEB
  sHistName             = "hMuchHitsElinkPerDpb";
  title                 = "Nb of hit messages per eLink for each DPB; DPB; eLink; Hits nb []";
  fhMuchHitsElinkPerDpb = new TH2I(sHistName, title, fuNrOfDpbs, 0, fuNrOfDpbs, 42, 0., 42.);


  LOG(debug) << "Initialized 2nd Histo";
  /*
   // Number of rate bins =
   //      9 for the sub-unit decade
   //    + 9 for each unit of each decade * 10 for the subdecade range
   //    + 1 for the closing bin top edge
   const Int_t iNbDecadesRate    = 9;
   const Int_t iNbStepsDecade    = 9;
   const Int_t iNbSubStepsInStep = 10;
   const Int_t iNbBinsRate = iNbStepsDecade
                           + iNbStepsDecade * iNbSubStepsInStep * iNbDecadesRate
                           + 1;
   Double_t dBinsRate[iNbBinsRate];
      // First fill sub-unit decade
   for( Int_t iSubU = 0; iSubU < iNbStepsDecade; iSubU ++ )
      dBinsRate[ iSubU ] = 0.1 * ( 1 + iSubU );
   std::cout << std::endl;
      // Then fill the main decades
   Double_t dSubstepSize = 1.0 / iNbSubStepsInStep;
   for( Int_t iDecade = 0; iDecade < iNbDecadesRate; iDecade ++)
   {
      Double_t dBase = std::pow( 10, iDecade );
      Int_t iDecadeIdx = iNbStepsDecade
                       + iDecade * iNbStepsDecade * iNbSubStepsInStep;
      for( Int_t iStep = 0; iStep < iNbStepsDecade; iStep++ )
      {
         Int_t iStepIdx = iDecadeIdx + iStep * iNbSubStepsInStep;
         for( Int_t iSubStep = 0; iSubStep < iNbSubStepsInStep; iSubStep++ )
         {
            dBinsRate[ iStepIdx + iSubStep ] = dBase * (1 + iStep)
                                             + dBase * dSubstepSize * iSubStep;
         } // for( Int_t iSubStep = 0; iSubStep < iNbSubStepsInStep; iSubStep++ )
      } // for( Int_t iStep = 0; iStep < iNbStepsDecade; iStep++ )
   } // for( Int_t iDecade = 0; iDecade < iNbDecadesRate; iDecade ++)
   dBinsRate[ iNbBinsRate - 1 ] = std::pow( 10, iNbDecadesRate );
*/
  LOG(debug) << "Initialized 3rd Histo";
  ///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
  //UInt_t uAlignedLimit = fuLongHistoNbSeconds - (fuLongHistoNbSeconds % fuLongHistoBinSizeSec);
  //   UInt_t uAlignedLimit = 0;

  //   UInt_t uNbBinEvo = (32768 + 1) * 2;
  //   Double_t dMaxEdgeEvo = stsxyter::kdClockCycleNs
  //                         * static_cast< Double_t >( uNbBinEvo ) / 2.0;
  //   Double_t dMinEdgeEvo = dMaxEdgeEvo * -1.0;

  //UInt_t uNbBinDt     = static_cast<UInt_t>( (fdCoincMax - fdCoincMin )/stsxyter::kdClockCycleNs );

  // Miscroslice properties histos
  for (Int_t component = 0; component < kiMaxNbFlibLinks; component++) {
    fhMsSz[component]     = NULL;
    fhMsSzTime[component] = NULL;
  }  // for( Int_t component = 0; component < kiMaxNbFlibLinks; component ++ )

  /// All histos per FEB: with channels or ASIC as axis!!
  // fhMuchFebChanDtCoinc.resize( fuNbFebs );
  // fhMuchFebChanCoinc.resize( fuNbFebs );
  for (UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx) {
    /// Channel counts
    sHistName = Form("hMuchFebChanCntRaw_%03u", uFebIdx);
    title     = Form("Hits Count per channel, FEB #%03u; Channel; Hits []", uFebIdx);
    fhMuchFebChanCntRaw.push_back(
      new TH1I(sHistName, title, fUnpackParMuch->GetNbChanPerFeb(), -0.5, fUnpackParMuch->GetNbChanPerFeb() - 0.5));

    //sHistName = Form( "hMuchFebChanCntRawGood_%03u", uFebIdx );
    //title = Form( "Hits Count per channel in good MS (SX2 bug flag off), FEB #%03u; Channel; Hits []", uFebIdx );
    //fhMuchFebChanCntRawGood.push_back( new TH1I(sHistName, title,
    //                          fUnpackParMuch->GetNbChanPerFeb(), -0.5, fUnpackParMuch->GetNbChanPerFeb() - 0.5 ) );

    sHistName = Form("fhMuchFebSpill_%03u", uFebIdx);
    title     = Form("Time distribution of hits, FEB #%03u; Time ; Counts ", uFebIdx);
    fhMuchFebSpill.push_back(new TH1I(sHistName, title, 2000, 0, 2000));


    sHistName = Form("hMuchChannelTime_FEB%03u", uFebIdx);
    title     = Form("Time vs Channel, FEB #%03u; TIME(s) ; CHANNEL ", uFebIdx);
    fhMuchChannelTime.push_back(new TH2I(sHistName, title, 1000, 0, 1000, 129, -0.5, 128.5));


    sHistName = Form("hMuchFebADC_%03u", uFebIdx);
    title     = Form("CHANNEL vs ADC, FEB #%03u; CHANNEL ; ADC ", uFebIdx);
    fhMuchFebADC.push_back(new TH2I(sHistName, title, 129, -0.5, 128.5, 34, -0.5, 33.5));


    /// Raw Adc Distribution
    sHistName = Form("hMuchFebChanAdcRaw_%03u", uFebIdx);
    title     = Form("Raw Adc distribution per channel, FEB #%03u; Channel []; Adc "
                 "[]; Hits []",
                 uFebIdx);
    fhMuchFebChanAdcRaw.push_back(new TH2I(sHistName, title, fUnpackParMuch->GetNbChanPerFeb(), -0.5,
                                           fUnpackParMuch->GetNbChanPerFeb() - 0.5, stsxyter::kuHitNbAdcBins, -0.5,
                                           stsxyter::kuHitNbAdcBins - 0.5));

    /// Raw Adc Distribution profile
    sHistName = Form("hMuchFebChanAdcRawProfc_%03u", uFebIdx);
    title     = Form("Raw Adc prodile per channel, FEB #%03u; Channel []; Adc []", uFebIdx);
    fhMuchFebChanAdcRawProf.push_back(
      new TProfile(sHistName, title, fUnpackParMuch->GetNbChanPerFeb(), -0.5, fUnpackParMuch->GetNbChanPerFeb() - 0.5));

    /// Cal Adc Distribution
    //sHistName = Form( "hMuchFebChanAdcCal_%03u", uFebIdx );
    //title = Form( "Cal. Adc distribution per channel, FEB #%03u; Channel []; Adc [e-]; Hits []", uFebIdx );
    //fhMuchFebChanAdcCal.push_back( new TH2I(sHistName, title,
    //                           fUnpackParMuch->GetNbChanPerFeb(), -0.5, fUnpackParMuch->GetNbChanPerFeb() - 0.5,
    //                            50, 0., 100000. ) );

    /// Cal Adc Distribution profile
    //sHistName = Form( "hMuchFebChanAdcCalProfc_%03u", uFebIdx );
    //title = Form( "Cal. Adc prodile per channel, FEB #%03u; Channel []; Adc [e-]", uFebIdx );
    //fhMuchFebChanAdcCalProf.push_back( new TProfile(sHistName, title,
    //                           fUnpackParMuch->GetNbChanPerFeb(), -0.5, fUnpackParMuch->GetNbChanPerFeb() - 0.5 ) );

    /// Raw Ts Distribution
    sHistName = Form("hMuchFebChanRawTs_%03u", uFebIdx);
    title     = Form("Raw Timestamp distribution per channel, FEB #%03u; Channel "
                 "[]; Ts []; Hits []",
                 uFebIdx);
    fhMuchFebChanRawTs.push_back(new TH2I(sHistName, title, fUnpackParMuch->GetNbChanPerFeb(), -0.5,
                                          fUnpackParMuch->GetNbChanPerFeb() - 0.5, stsxyter::kuHitNbTsBins, -0.5,
                                          stsxyter::kuHitNbTsBins - 0.5));

    /// Hit rates evo per channel
    sHistName = Form("hMuchFebChanRateEvo_%03u", uFebIdx);
    title     = Form("Hits per second & channel in FEB #%03u; Time [s]; Channel []; Hits []", uFebIdx);
    fhMuchFebChanHitRateEvo.push_back(new TH2I(sHistName, title, 1800, 0, 1800, fUnpackParMuch->GetNbChanPerFeb(), -0.5,
                                               fUnpackParMuch->GetNbChanPerFeb() - 0.5));

    /// Hit rates profile per channel
    sHistName = Form("hMuchFebChanRateProf_%03u", uFebIdx);
    title     = Form("Hits per second for each channel in FEB #%03u; Channel []; Hits/s []", uFebIdx);
    fhMuchFebChanHitRateProf.push_back(
      new TProfile(sHistName, title, fUnpackParMuch->GetNbChanPerFeb(), -0.5, fUnpackParMuch->GetNbChanPerFeb() - 0.5));

    /// Hit rates evo per StsXyter
    // sHistName = Form( "hMuchFebAsicRateEvo_%03u", uFebIdx );
    // title = Form( "Hits per second & StsXyter in FEB #%03u; Time [s]; Asic []; Hits []", uFebIdx );
    // fhMuchFebAsicHitRateEvo.push_back( new TH2I( sHistName, title, 1800, 0, 1800,
    //                                         fUnpackParMuch->GetNbAsicsPerFeb(), -0.5, fUnpackParMuch->GetNbAsicsPerFeb() - 0.5  ) );

    /// Hit rates evo per FEB
    sHistName = Form("hMuchFebRateEvo_%03u", uFebIdx);
    title     = Form("Hits per second in FEB #%03u; Time [s]; Hits []", uFebIdx);
    fhMuchFebHitRateEvo.push_back(new TH1I(sHistName, title, 1800, 0, 1800));


    /// Hit rates evo per FEB for Mask Channel
    sHistName = Form("hMuchFebRateEvo_mskch_%03u", uFebIdx);
    title     = Form("Hits per second in FEB #%03u; Time [s]; Hits []", uFebIdx);
    fhMuchFebHitRateEvo_mskch.push_back(new TH1I(sHistName, title, 1800, 0, 1800));

    /// Hit rates evo per FEB for Mask Channel with ADC Cut
    sHistName = Form("hMuchFebRateEvo_mskch_adcut_%03u", uFebIdx);
    title     = Form("Hits per second in FEB #%03u; Time [s]; Hits []", uFebIdx);
    fhMuchFebHitRateEvo_mskch_adccut.push_back(new TH1I(sHistName, title, 1800, 0, 1800));

    /// Hit rates evo per FEB
    sHistName = Form("hMuchFebRateEvo_WithoutDupli_%03u", uFebIdx);
    title     = Form("Hits per second in FEB #%03u; Time [s]; Hits []", uFebIdx);
    fhMuchFebHitRateEvo_WithoutDupli.push_back(new TH1I(sHistName, title, 50000, 0, 5000));

    /// Hit rates evo per channel, 1 minute bins, 24h
    //sHistName = Form( "hMuchFebChanRateEvoLong_%03u", uFebIdx );
    //title = Form( "Hits per second & channel in FEB #%03u; Time [min]; Channel []; Hits []", uFebIdx );
    //fhMuchFebChanHitRateEvoLong.push_back( new TH2D( sHistName, title,
    //                                          fuLongHistoBinNb, -0.5, uAlignedLimit - 0.5,
    //                                          fUnpackParMuch->GetNbChanPerFeb(), -0.5, fUnpackParMuch->GetNbChanPerFeb() - 0.5 ) );

    /// Hit rates evo per channel, 1 minute bins, 24h
    //sHistName = Form( "hMuchFebAsicRateEvoLong_%03u", uFebIdx );
    //title = Form( "Hits per second & StsXyter in FEB #%03u; Time [min]; Asic []; Hits []", uFebIdx );
    //fhMuchFebAsicHitRateEvoLong.push_back( new TH2D( sHistName, title,
    //                                          fuLongHistoBinNb, -0.5, uAlignedLimit - 0.5,
    //                                          fUnpackParMuch->GetNbAsicsPerFeb(), -0.5, fUnpackParMuch->GetNbAsicsPerFeb() - 0.5 ) );

    /// Hit rates evo per FEB, 1 minute bins, 24h
    //sHistName = Form( "hMuchFebRateEvoLong_%03u", uFebIdx );
    //title = Form( "Hits per second in FEB #%03u; Time [min]; Hits []", uFebIdx );
    //fhMuchFebHitRateEvoLong.push_back( new TH1D(sHistName, title,
    //                                          fuLongHistoBinNb, -0.5, uAlignedLimit - 0.5 ) );

    /// Distance between hits on same channel
    sHistName = Form("hMuchFebChanDistT_%03u", uFebIdx);
    title     = Form("Time distance between hits on same channel in between FEB "
                 "#%03u; Time difference [ns]; Channel []; ",
                 uFebIdx);
    fhMuchFebChanDistT.push_back(new TH2I(sHistName, title, 1000, -0.5, 6250.0 - 0.5, fUnpackParMuch->GetNbChanPerFeb(),
                                          -0.5, fUnpackParMuch->GetNbChanPerFeb() - 0.5));

  }  // for( UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx )

  ///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
  fhDpbMsErrors =
    new TH2I("fhDpbMsErrors", "; DPB []; Error type []; Counts []", fuNrOfDpbs, 0, fuNrOfDpbs, 4, -0.5, 3.5);
  ///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///

  // Miscroslice properties histos
  for (Int_t component = 0; component < kiMaxNbFlibLinks; component++) {
    fhMsSz[component]     = NULL;
    fhMsSzTime[component] = NULL;
  }  // for( Int_t component = 0; component < kiMaxNbFlibLinks; component ++ )

  LOG(debug) << "Initialized 6th Histo before FairRunOnlne Instance";
  // THttpServer* server = FairRunOnline::Instance()->GetHttpServer();
  // if (server) {
  for (UInt_t uModuleId = 0; uModuleId < 2; ++uModuleId) {
    //server->Register("/MuchRaw", fHistPadDistr[uModuleId] );
    AddHistoToVector(fHistPadDistr[uModuleId], "MuchRaw");
    //server->Register("/MuchRaw", fRealHistPadDistr[uModuleId] );
    AddHistoToVector(fRealHistPadDistr[uModuleId], "MuchRaw");
    // server->Register("/MuchFeb", fhMuchFebDuplicateHitProf[uModuleId] );
  }

  //  server->Register("/MuchRaw", fhRate );
  //  server->Register("/MuchRaw", fhRateAdcCut );
  // server->Register("/MuchRaw", fhFEBcount); //closed by me
  AddHistoToVector(fhFEBcount, "MuchRaw");
  AddHistoToVector(fhMuchMessType, "MuchRaw");
  // server->Register("/MuchRaw", fhMuchMessType); //closed by me
  AddHistoToVector(fhElinkIdxHit, "MuchRaw");
  //AddHistoToVector(fhElinkIdxTsMsb, "MuchRaw");
  //AddHistoToVector(fhElinkIdxEpoch, "MuchRaw");
  //AddHistoToVector(fhElinkIdxStatus, "MuchRaw");
  // server->Register("/MuchRaw", fhMuchSysMessType );
  //server->Register("/MuchRaw", fhMuchMessTypePerDpb); //closed by me
  AddHistoToVector(fhMuchMessTypePerDpb, "MuchRaw");
  AddHistoToVector(fhMuchMessTypePerElink, "MuchRaw");
  //server->Register("/MuchRaw", fhMuchSysMessTypePerDpb); //closed by me
  AddHistoToVector(fhMuchSysMessTypePerDpb, "MuchRaw");
  // server->Register("/MuchRaw", fhStatusMessType );
  //server->Register("/MuchRaw", fhMsStatusFieldType); // closed by me
  AddHistoToVector(fhMsStatusFieldType, "MuchRaw");
  //server->Register("/MuchRaw", fhMuchHitsElinkPerDpb); // closed by me
  AddHistoToVector(fhMuchHitsElinkPerDpb, "MuchRaw");
  //server->Register("/MuchRaw", fhMuchFebChanAdcRaw_combined); // closed by me
  AddHistoToVector(fhMuchFebChanAdcRaw_combined, "MuchRaw");
  for (UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx) {
    if (kTRUE == fUnpackParMuch->IsFebActive(uFebIdx)) {
      //server->Register("/MuchFeb", fhMuchFebChanCntRaw[uFebIdx]); //closed by me
      AddHistoToVector(fhMuchFebChanCntRaw[uFebIdx], "MuchFeb");
      //server->Register("/MuchFeb", fhMuchFebSpill[uFebIdx]); // closed by me
      AddHistoToVector(fhMuchFebSpill[uFebIdx], "MuchFeb");
      //server->Register("/MuchFeb", fhMuchFebADC[uFebIdx]); // closed by me
      AddHistoToVector(fhMuchFebADC[uFebIdx], "MuchFeb");
      //server->Register("/MuchFeb", fhMuchChannelTime[uFebIdx]); // closed by me
      AddHistoToVector(fhMuchChannelTime[uFebIdx], "MuchFeb");

      //server->Register("/MuchFeb", fhMuchFebChanCntRawGood[ uFebIdx ] );
      ////   server->Register("/MuchFeb", fhMuchFebChanAdcRaw[ uFebIdx ] );
      //  server->Register("/MuchFeb", fhMuchFebChanAdcRawProf[ uFebIdx ] );
      //server->Register("/MuchFeb", fhMuchFebChanAdcCal[ uFebIdx ] );
      //server->Register("/MuchFeb", fhMuchFebChanAdcCalProf[ uFebIdx ] );
      //// server->Register("/MuchFeb", fhMuchFebChanRawTs[ uFebIdx ] );
      //server->Register("/MuchFeb", fhMuchFebChanHitRateEvo[ uFebIdx ] );
      // server->Register("/MuchFeb", fhMuchFebChanHitRateProf[ uFebIdx ] );
      //server->Register("/MuchFeb", fhMuchFebAsicHitRateEvo[ uFebIdx ] );
      // server->Register("/MuchFeb", fhMuchFebHitRateEvo[ uFebIdx ] );
      ////  server->Register("/MuchFeb", fhMuchFebHitRateEvo_mskch[ uFebIdx ] );
      // server->Register("/MuchFeb", fhMuchFebHitRateEvo_mskch_adccut[ uFebIdx ] );
      // server->Register("/MuchFeb", fhMuchFebHitRateEvo_WithoutDupli[ uFebIdx ] );
      LOG(debug) << "Initialized fhMuchFebHitRateEvo_WithoutDupli number " << uFebIdx;
      /*server->Register("/MuchFeb", fhMuchFebChanHitRateEvoLong[ uFebIdx ] );
            server->Register("/MuchFeb", fhMuchFebAsicHitRateEvoLong[ uFebIdx ] );
            server->Register("/MuchFeb", fhMuchFebHitRateEvoLong[ uFebIdx ] );
            server->Register("/MuchFeb", fhMuchFebChanDistT[ uFebIdx ] );*/

    }  // if( kTRUE == fUnpackParMuch->IsFebActive( uFebIdx ) )
    //server->Register("/MuchRaw", fhDpbMsErrors); //closed by me
    //AddHistoToVector(fhDpbMsErrors, "MuchRaw");
  }  // for( UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx )
  AddHistoToVector(fhDpbMsErrors, "MuchRaw");

  //  LOG(debug) << "Initialized FEB  8th Histo";
  /*server->RegisterCommand("/Reset_All", "bMcbm2018ResetAlgoMuchLite=kTRUE");
    server->RegisterCommand("/Write_All", "bMcbm2018WriteAlgoMuchLite=kTRUE");
    server->RegisterCommand("/ScanNoisyCh", "bMcbm2018ScanNoisyAlgoMuchLite=kTRUE");
    server->Restrict("/Reset_All", "allow=admin");
    server->Restrict("/Write_All", "allow=admin");
    server->Restrict("/ScanNoisyCh", "allow=admin"); */
  //  }  // if( server )

  LOG(debug) << "Initialized All Histos  8th Histo";
  /** Create summary Canvases for mCBM 2019 **/
  Double_t w = 10;
  Double_t h = 10;
  LOG(debug) << "Initialized 7th Histo before Summary per FEB";

  TCanvas* cChannel = new TCanvas(Form("CHANNELS"),
                                  Form("CHANNELS"));  //,
  //                                          w, h);
  cChannel->Divide(4, 9);
  // Summary per FEB
  for (UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx) {
    if (kTRUE == fUnpackParMuch->IsFebActive(uFebIdx)) {
      cChannel->cd(uFebIdx + 1);
      //// gPad->SetGridx();
      // gPad->SetGridy();
      gPad->SetLogy();
      fhMuchFebChanCntRaw[uFebIdx]->Draw();

    }  // if( kTRUE == fUnpackParMuch->IsFebActive( uFebIdx ) )
  }    // for( UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx )

  //server->Register("/canvases", cChannel); //closed by me
  AddCanvasToVector(cChannel, "canvases");
  //All Feb hit rate together on one Canvas

  TCanvas* cspill = new TCanvas(Form("SPILLS"), Form("SPILLS"));  //,w, h);
  cspill->Divide(4, 9);

  for (UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx) {
    if (kTRUE == fUnpackParMuch->IsFebActive(uFebIdx)) {
      UInt_t flPad = 1 + uFebIdx;
      cspill->cd(flPad);
      //gPad->SetGridx();
      //gPad->SetGridy();
      // gPad->SetLogy();
      fhMuchFebSpill[uFebIdx]->Draw();
    }
    // server->Register("/canvases", cspill);
  }
  // server->Register("/canvases", cspill); //closed by me
  AddCanvasToVector(cspill, "canvases");

  TCanvas* cadc = new TCanvas(Form("ADC"), Form("ADC"), w, h);
  cadc->Divide(4, 9);

  for (UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx) {
    if (kTRUE == fUnpackParMuch->IsFebActive(uFebIdx)) {
      UInt_t flPad = 1 + uFebIdx;
      cadc->cd(flPad);
      //gPad->SetGridx();
      //gPad->SetGridy();
      //gPad->SetLogy();
      fhMuchFebADC[uFebIdx]->Draw("colz");
    }
  }
  //server->Register("/canvases", cadc); //closed by me
  AddCanvasToVector(cadc, "canvases");
  TCanvas* cChanneltime = new TCanvas(Form("ChannelvsTime"), Form("ChannelvsTime"), w, h);
  cChanneltime->Divide(4, 9);

  for (UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx) {
    if (kTRUE == fUnpackParMuch->IsFebActive(uFebIdx)) {
      UInt_t flPad = 1 + uFebIdx;
      cChanneltime->cd(flPad);
      //gPad->SetGridx();
      //gPad->SetGridy();
      //gPad->SetLogy();
      fhMuchChannelTime[uFebIdx]->Draw("colz");
    }
  }
  // server->Register("/canvases", cChanneltime); //closed by me
  AddCanvasToVector(cChanneltime, "canvases");
  //====================================================================//
  LOG(debug) << "Initialized Last Histo before exiting CreateHistograms";
  //====================================================================//
  /** Recovers/Create Ms Size Canvase for CERN 2016 **/
  // Try to recover canvas in case it was created already by another monitor
  // If not existing, create it
  fcMsSizeAll = dynamic_cast<TCanvas*>(gROOT->FindObject("cMsSizeAll"));
  if (NULL == fcMsSizeAll) {
    fcMsSizeAll = new TCanvas("cMsSizeAll",
                              "Evolution of MS size in last 300 s");  //, w, h);
    fcMsSizeAll->Divide(1, 8);
    LOG(info) << "Created MS size canvas in Much monitor";
    //server->Register("/canvases", fcMsSizeAll); //closed by me
    AddCanvasToVector(fcMsSizeAll, "canvases");
  }  // if( NULL == fcMsSizeAll )
  else
    LOG(info) << "Recovered MS size canvas in Much monitor";
  //====================================================================//

  /*****************************/
  return kTRUE;
}

Bool_t CbmMcbm2018MonitorAlgoMuchLite::ProcessTs(const fles::Timeslice& ts)
{
  fulCurrentTsIdx = ts.index();
  fdTsStartTime   = static_cast<Double_t>(ts.descriptor(0, 0).idx);

  /// Ignore First TS as first MS is typically corrupt
  if (0 == fulCurrentTsIdx) return kTRUE;

  /// On first TS, extract the TS parameters from header (by definition stable over time)
  if (-1.0 == fdTsCoreSizeInNs) {
    fuNbCoreMsPerTs  = ts.num_core_microslices();
    fuNbOverMsPerTs  = ts.num_microslices(0) - ts.num_core_microslices();
    fdTsCoreSizeInNs = fdMsSizeInNs * (fuNbCoreMsPerTs);
    fdTsFullSizeInNs = fdMsSizeInNs * (fuNbCoreMsPerTs + fuNbOverMsPerTs);
    LOG(info) << "Timeslice parameters: each TS has " << fuNbCoreMsPerTs << " Core MS and " << fuNbOverMsPerTs
              << " Overlap MS, for a core duration of " << fdTsCoreSizeInNs << " ns and a full duration of "
              << fdTsFullSizeInNs << " ns";

    /// Ignore overlap ms if flag set by user
    fuNbMsLoop = fuNbCoreMsPerTs;
    if (kFALSE == fbIgnoreOverlapMs) fuNbMsLoop += fuNbOverMsPerTs;
    LOG(info) << "In each TS " << fuNbMsLoop << " MS will be looped over";
  }  // if( -1.0 == fdTsCoreSizeInNs )

  /// Compute time of TS core end
  fdTsStopTimeCore = fdTsStartTime + fdTsCoreSizeInNs;
  //      LOG(info) << Form( "TS %5d Start %12f Stop %12f", fulCurrentTsIdx, fdTsStartTime, fdTsStopTimeCore );

  /// Loop over core microslices (and overlap ones if chosen)
  for (fuMsIndex = 0; fuMsIndex < fuNbMsLoop; fuMsIndex++) {
    /// Loop over registered components
    for (UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsList.size(); ++uMsCompIdx) {
      UInt_t uMsComp = fvMsComponentsList[uMsCompIdx];

      if (kFALSE == ProcessMs(ts, uMsComp, fuMsIndex)) {
        LOG(error) << "Failed to process ts " << fulCurrentTsIdx << " MS " << fuMsIndex << " for component " << uMsComp;
        return kFALSE;
      }  // if( kFALSE == ProcessMs( ts, uMsCompIdx, fuMsIndex ) )
      // LOG(info) << "TS index : " << fulCurrentTsIdx << " end processing MS";
    }  // for( UInt_t uMsCompIdx = 0; uMsCompIdx < fvMsComponentsList.size(); ++uMsCompIdx )
       /*
      /// Sort the buffers of hits
      std::sort( fvmHitsInMs.begin(), fvmHitsInMs.end() );

      /// Add the hits to the output buffer as Digis
      for( auto itHitIn = fvmHitsInMs.begin(); itHitIn < fvmHitsInMs.end(); ++itHitIn )
      {
         UInt_t uFebIdx =  itHitIn->GetAsic() / fUnpackPar->GetNbAsicsPerFeb()
                          + ( itHitIn->GetDpb() * fUnpackPar->GetNbCrobsPerDpb() + itHitIn->GetCrob() )
                            * fUnpackPar->GetNbFebsPerCrob();
         UInt_t uChanInFeb = itHitIn->GetChan()
                            + fUnpackPar->GetNbChanPerAsic() * (itHitIn->GetAsic() % fUnpackPar->GetNbAsicsPerFeb());

         ULong64_t ulTimeInNs = static_cast< ULong64_t >( itHitIn->GetTs() * stsxyter::kdClockCycleNs - fdTimeOffsetNs );

         fDigiVect.push_back( CbmTofDigi( fviFebAddress[ uFebIdx ], uChanInFeb, ulTimeInNs, itHitIn->GetAdc() ) );
      } // for( auto itHitIn = fvmHitsInMs.begin(); itHitIn < fvmHitsInMs.end(); ++itHitIn )
*/
    /// Clear the buffer of hits
    fvmHitsInMs.clear();
  }  // for( fuMsIndex = 0; fuMsIndex < uNbMsLoop; fuMsIndex ++ )

  /// Clear buffers to prepare for the next TS
  fvmHitsInMs.clear();
  /*
   for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )
   {
      fvmHitsInMs[ uDpb ].clear();
   } // for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )
*/
  /// Fill plots if in monitor mode
  if (fbMonitorMode) {
    //if (kFALSE == FillHistograms()) {
    /* if (kFALSE == CreateHistograms()) {
      LOG(error) << "Failed to fill histos in ts " << fulCurrentTsIdx;
      return kFALSE;
    }*/  // if( kFALSE == FillHistograms() ) //closed by me
  }  // if( fbMonitorMode )

  return kTRUE;
}

Bool_t CbmMcbm2018MonitorAlgoMuchLite::ProcessMs(const fles::Timeslice& ts, size_t uMsComp, size_t uMsIdx)
{
  auto msDescriptor        = ts.descriptor(uMsComp, uMsIdx);
  fuCurrentEquipmentId     = msDescriptor.eq_id;
  const uint8_t* msContent = reinterpret_cast<const uint8_t*>(ts.content(uMsComp, uMsIdx));

  fulCurrentTsIdx = ts.index();
  if (0 == fvbMaskedComponents.size()) fvbMaskedComponents.resize(ts.num_components(), kFALSE);

  if (0 == fulCurrentTsIdx && 0 == uMsIdx) {
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
  }  // if( 0 == fulCurrentTsIndex && 0 == uMsIdx )
  if (kFALSE == fvbMaskedComponents[uMsComp] && 0 == uMsIdx) {
    auto it = fDpbIdIndexMap.find(fuCurrentEquipmentId);
    if (fDpbIdIndexMap.end() == it) {
      LOG(warning) << "Could not find the sDPB index for AFCK id 0x" << std::hex << fuCurrentEquipmentId << std::dec
                   << " component " << uMsComp << "\n"
                   << "If valid this index has to be added in the TOF parameter file in "
                      "the RocIdArray field"
                   << "\n"
                   << "For now we remove it from the list of components analyzed";
      fvbMaskedComponents[uMsComp] = kTRUE;
    }  // if( fDpbIdIndexMap.end() == it )

  }  // if( kFALSE == fvbMaskedComponents[ uMsComp ] && 0 == uMsIdx )

  if (kTRUE == fvbMaskedComponents[uMsComp]) return kTRUE;

  uint32_t uSize   = msDescriptor.size;
  fulCurrentMsIdx  = msDescriptor.idx;
  Double_t dMsTime = (1e-9) * static_cast<double>(fulCurrentMsIdx);
  LOG(debug) << "Microslice: " << fulCurrentMsIdx << " from EqId " << std::hex << fuCurrentEquipmentId << std::dec
             << " has size: " << uSize;

  fuCurrDpbId  = static_cast<uint32_t>(fuCurrentEquipmentId & 0xFFFF);
  fuCurrDpbIdx = fDpbIdIndexMap[fuCurrDpbId];
  //LOG(info) <<" fuCurrDpbIdx "<<fuCurrDpbIdx<<" fuCurrDpbId "<<fuCurrDpbId;

  if (uMsComp < kiMaxNbFlibLinks) {
    if (fdStartTimeMsSz < 0) fdStartTimeMsSz = dMsTime;
    fhMsSz[uMsComp]->Fill(uSize);
    fhMsSzTime[uMsComp]->Fill(dMsTime - fdStartTimeMsSz, uSize);
  }  // if( uMsComp < kiMaxNbFlibLinks )

  /// Plots in [X/s] update
  if (static_cast<Int_t>(fvdPrevMsTime[uMsComp]) < static_cast<Int_t>(dMsTime)) {
    /// "new second"
    UInt_t uFebIdxOffset = fUnpackParMuch->GetNbFebsPerDpb() * fuCurrDpbIdx;
    for (UInt_t uFebIdx = 0; uFebIdx < fUnpackParMuch->GetNbFebsPerDpb(); ++uFebIdx) {
      UInt_t uFebIdxInSyst = uFebIdxOffset + uFebIdx;

      /// Ignore first interval is not clue how late the data taking was started
      if (0 == fviFebTimeSecLastRateUpdate[uFebIdxInSyst]) {
        fviFebTimeSecLastRateUpdate[uFebIdxInSyst]     = static_cast<Int_t>(dMsTime);
        fviFebCountsSinceLastRateUpdate[uFebIdxInSyst] = 0;
        for (UInt_t uChan = 0; uChan < fUnpackParMuch->GetNbChanPerFeb(); ++uChan)
          fvdFebChanCountsSinceLastRateUpdate[uFebIdxInSyst][uChan] = 0.0;
        continue;
      }  // if( 0 == fviFebTimeSecLastRateUpdate[uFebIdxInSyst] )

      Int_t iTimeInt = static_cast<Int_t>(dMsTime) - fviFebTimeSecLastRateUpdate[uFebIdxInSyst];
      if (fiTimeIntervalRateUpdate <= iTimeInt) {
        /// Jump empty FEBs without looping over channels
        if (0 == fviFebCountsSinceLastRateUpdate[uFebIdxInSyst]) {
          fviFebTimeSecLastRateUpdate[uFebIdxInSyst] = static_cast<Int_t>(dMsTime);
          continue;
        }  // if( 0 == fviFebCountsSinceLastRateUpdate[uFebIdxInSyst] )

        for (UInt_t uChan = 0; uChan < fUnpackParMuch->GetNbChanPerFeb(); ++uChan) {
          fhMuchFebChanHitRateProf[uFebIdxInSyst]->Fill(uChan, fvdFebChanCountsSinceLastRateUpdate[uFebIdxInSyst][uChan]
                                                                 / iTimeInt);
          fvdFebChanCountsSinceLastRateUpdate[uFebIdxInSyst][uChan] = 0.0;
        }  // for( UInt_t uChan = 0; uChan < fUnpackParMuch->GetNbChanPerFeb(); ++uChan )

        fviFebTimeSecLastRateUpdate[uFebIdxInSyst]     = static_cast<Int_t>(dMsTime);
        fviFebCountsSinceLastRateUpdate[uFebIdxInSyst] = 0;
      }  // if( fiTimeIntervalRateUpdate <= iTimeInt )
    }    // for( UInt_t uFebIdx = 0; uFebIdx < fUnpackParMuch->GetNbFebsPerDpb(); ++uFebIdx )
  }      // if( static_cast<Int_t>( fvdMsTime[ uMsCompIdx ] ) < static_cast<Int_t>( dMsTime )  )

  // Store MS time for coincidence plots
  fvdPrevMsTime[uMsComp] = dMsTime;

  /// Check Flags field of MS header
  uint16_t uMsHeaderFlags = msDescriptor.flags;
  for (UInt_t uBit = 0; uBit < 16; ++uBit)
    fhMsStatusFieldType->Fill(uBit, (uMsHeaderFlags >> uBit) & 0x1);

  /** Check the current TS_MSb cycle and correct it if wrong **/
  UInt_t uTsMsbCycleHeader = std::floor(fulCurrentMsIdx / (stsxyter::kulTsCycleNbBins * stsxyter::kdClockCycleNs))
                             - fvuInitialTsMsbCycleHeader[fuCurrDpbIdx];

  /// => Quick and dirty hack for binning FW!!!
  if (kTRUE == fbBinningFw)
    uTsMsbCycleHeader = std::floor(fulCurrentMsIdx / (stsxyter::kulTsCycleNbBinsBinning * stsxyter::kdClockCycleNs))
                        - fvuInitialTsMsbCycleHeader[fuCurrDpbIdx];

  if (kFALSE == fvuInitialHeaderDone[fuCurrDpbIdx]) {
    fvuInitialTsMsbCycleHeader[fuCurrDpbIdx] = uTsMsbCycleHeader;
    fvuInitialHeaderDone[fuCurrDpbIdx]       = kTRUE;
  }  // if( kFALSE == fvuInitialHeaderDone[ fuCurrDpbIdx ] )
  else if (uTsMsbCycleHeader != fvuCurrentTsMsbCycle[fuCurrDpbIdx] && 4194303 != fvulCurrentTsMsb[fuCurrDpbIdx]) {
    /*
      LOG(warning) << "TS MSB cycle from MS header does not match current cycle from data "
                    << "for TS " << std::setw( 12 ) << fulCurrentTsIdx
                    << " MS " << std::setw( 12 ) << fulCurrentMsIdx
                    << " MsInTs " << std::setw( 3 ) << uMsIdx
                    << " ====> " << fvuCurrentTsMsbCycle[ fuCurrDpbIdx ]
                    << " VS " << uTsMsbCycleHeader;
*/
    fvuCurrentTsMsbCycle[fuCurrDpbIdx] = uTsMsbCycleHeader;
  }

  // If not integer number of message in input buffer, print warning/error
  if (0 != (uSize % kuBytesPerMessage))
    LOG(error) << "The input microslice buffer does NOT "
               << "contain only complete nDPB messages!";

  // Compute the number of complete messages in the input microslice buffer
  uint32_t uNbMessages = (uSize - (uSize % kuBytesPerMessage)) / kuBytesPerMessage;

  // Prepare variables for the loop on contents
  const uint32_t* pInBuff = reinterpret_cast<const uint32_t*>(msContent);

  for (uint32_t uIdx = 0; uIdx < uNbMessages; ++uIdx) {
    // Fill message
    uint32_t ulData = static_cast<uint32_t>(pInBuff[uIdx]);

    stsxyter::Message mess(static_cast<uint32_t>(ulData & 0xFFFFFFFF));

    // Print message if requested
    if (fbPrintMessages) mess.PrintMess(std::cout, fPrintMessCtrl);
    /*
      if( 1000 == fulCurrentTsIdx )
      {
         mess.PrintMess( std::cout, fPrintMessCtrl );
      } // if( 0 == fulCurrentTsIdx )
*/
    stsxyter::MessType typeMess = mess.GetMessType();
    fmMsgCounter[typeMess]++;
    // fhMuchMessType->Fill( static_cast< uint16_t > (typeMess) );
    // fhMuchMessTypePerDpb->Fill( fuCurrDpbIdx, static_cast< uint16_t > (typeMess) );

    switch (typeMess) {
      case stsxyter::MessType::Hit: {
        // Extract the eLink and Asic indices => Should GO IN the fill method now that obly hits are link/asic specific!
        UShort_t usElinkIdx = mess.GetLinkIndex();
        /// => Quick and dirty hack for binning FW!!!
        if (kTRUE == fbBinningFw) usElinkIdx = mess.GetLinkIndexHitBinning();
        fhElinkIdxHit->Fill(usElinkIdx);
        //LOG(info) << "Elink Id" << usElinkIdx ;
        UInt_t uCrobIdx = usElinkIdx / fUnpackParMuch->GetNbElinkPerCrob();
        Int_t uFebIdx   = fUnpackParMuch->ElinkIdxToFebIdx(usElinkIdx);
        //            if(usElinkIdx!=0)
        LOG(debug) << " usElinkIdx " << usElinkIdx << " uCrobIdx " << uCrobIdx << " uFebIdx " << uFebIdx
                   << " MessageTypeValue " << static_cast<uint16_t>(typeMess);
        if (kTRUE == fbMuchMode) uFebIdx = usElinkIdx;
        fhMuchHitsElinkPerDpb->Fill(fuCurrDpbIdx, usElinkIdx);
        fhMuchMessTypePerElink->Fill(fuCurrDpbIdx * fUnpackParMuch->GetNbElinkPerCrob(),
                                     static_cast<uint16_t>(typeMess));
        if (-1 == uFebIdx) {
          LOG(warning) << "CbmMcbm2018MonitorMuchLite::DoUnpack => "
                       << "Wrong elink Idx! Elink raw " << Form("%d remap %d", usElinkIdx, uFebIdx);
          continue;
        }  // if( -1 == uFebIdx )
        //LOG(info) << " uCrobIdx "<<uCrobIdx<<" fUnpackParMuch->ElinkIdxToAsicIdx( usElinkIdx ) "<<fUnpackParMuch->ElinkIdxToAsicIdx( usElinkIdx )<<" usElinkIdx "<<usElinkIdx;
        UInt_t uAsicIdx =
          (fuCurrDpbIdx * fUnpackParMuch->GetNbCrobsPerDpb() + uCrobIdx) * fUnpackParMuch->GetNbAsicsPerCrob()
          + fUnpackParMuch->ElinkIdxToAsicIdx(usElinkIdx);

        FillHitInfo(mess, usElinkIdx, uAsicIdx, uMsIdx);
        break;
      }  // case stsxyter::MessType::Hit :
      case stsxyter::MessType::TsMsb: {

        fhMuchMessTypePerElink->Fill(fuCurrDpbIdx * fUnpackParMuch->GetNbElinkPerCrob(),
                                     static_cast<uint16_t>(typeMess));

        FillTsMsbInfo(mess, uIdx, uMsIdx);
        break;
      }  // case stsxyter::MessType::TsMsb :
      case stsxyter::MessType::Epoch: {

        fhMuchMessTypePerElink->Fill(fuCurrDpbIdx * fUnpackParMuch->GetNbElinkPerCrob(),
                                     static_cast<uint16_t>(typeMess));

        // The first message in the TS is a special ones: EPOCH
        FillEpochInfo(mess);

        if (0 < uIdx)
          LOG(info) << "CbmMcbm2018MonitorMuchLite::DoUnpack => "
                    << "EPOCH message at unexpected position in MS: message " << uIdx << " VS message 0 expected!";
        break;
      }  // case stsxyter::MessType::TsMsb :
      case stsxyter::MessType::Status: {
        //            UShort_t usElinkIdx    = mess.GetStatusLink(); // commented 03.07.20 FU unused
        //            UInt_t   uCrobIdx   = usElinkIdx / fUnpackParMuch->GetNbElinkPerCrob(); // commented 03.07.20 FU unused
        //            Int_t   uFebIdx    = fUnpackParMuch->ElinkIdxToFebIdx( usElinkIdx );
        //            UInt_t   uAsicIdx   = ( fuCurrDpbIdx * fUnpackParMuch->GetNbCrobsPerDpb() + uCrobIdx // commented 03.07.20 FU unused
        //                                  ) * fUnpackParMuch->GetNbAsicsPerCrob()
        //                                 + fUnpackParMuch->ElinkIdxToAsicIdx( usElinkIdx );

        //            UShort_t usStatusField = mess.GetStatusStatus(); // commented 03.07.20 FU unused

        // fhStatusMessType->Fill( uAsicIdx, usStatusField );
        /// Always print status messages... or not?
        // UShort_t usElinkIdx = mess.GetLinkIndex();
        /// => Quick and dirty hack for binning FW!!!
        //if (kTRUE == fbBinningFw) usElinkIdx = mess.GetLinkIndexHitBinning();
        // fhElinkIdxStatus->Fill(usElinkIdx);
        fhMuchMessTypePerElink->Fill(fuCurrDpbIdx * fUnpackParMuch->GetNbElinkPerCrob(),
                                     static_cast<uint16_t>(typeMess));

        if (fbPrintMessages) {
          std::cout << Form("DPB %2u TS %12llu MS %12llu mess %5u ", fuCurrDpbIdx, fulCurrentTsIdx, fulCurrentMsIdx,
                            uIdx);
          mess.PrintMess(std::cout, fPrintMessCtrl);
        }  // if( fbPrintMessages )
           //                   FillTsMsbInfo( mess );
        break;
      }  // case stsxyter::MessType::Status
      case stsxyter::MessType::Empty: {
        //                   FillTsMsbInfo( mess );
        fhMuchMessTypePerElink->Fill(fuCurrDpbIdx * fUnpackParMuch->GetNbElinkPerCrob(),
                                     static_cast<uint16_t>(typeMess));
        break;
      }  // case stsxyter::MessType::Empty :
      case stsxyter::MessType::EndOfMs: {
        if (mess.IsMsErrorFlagOn()) {
          fhDpbMsErrors->Fill(fuCurrDpbIdx, mess.GetMsErrorType());
        }  // if( pMess[uIdx].IsMsErrorFlagOn() )
        break;
      }  // case stsxyter::MessType::EndOfMs :
      case stsxyter::MessType::Dummy: {
        break;
      }  // case stsxyter::MessType::Dummy / ReadDataAck / Ack :
      default: {
        LOG(fatal) << "CbmMcbm2018MonitorMuchLite::DoUnpack => "
                   << "Unknown message type, should never happen, stopping "
                      "here! Type found was: "
                   << static_cast<int>(typeMess);
      }
    }  // switch( mess.GetMessType() )
  }    // for( uint32_t uIdx = 0; uIdx < uNbMessages; ++uIdx )

  //LOG(info) << "TS index:  " << fulCurrentTsIdx << "a processing MS done";
  return kTRUE;
}

void CbmMcbm2018MonitorAlgoMuchLite::FillHitInfo(stsxyter::Message mess, const UShort_t& /*usElinkIdx*/,
                                                 const UInt_t& uAsicIdx, const UInt_t& /*uMsIdx*/)
{
  UShort_t usChan   = mess.GetHitChannel();
  UShort_t usRawAdc = mess.GetHitAdc();
  UShort_t usRawTs = mess.GetHitTime();

  /// => Quick and dirty hack for binning FW!!!
  if (kTRUE == fbBinningFw) usRawTs = mess.GetHitTimeBinning();
  //LOG(debug) << " AsicIdx " << uAsicIdx << " Channel Id " << usChan << " Adc " << usRawAdc << " Time " << usRawTs;
  //For MUCH each FEB has one StsXyter
  UInt_t uFebIdx = uAsicIdx;
  fhFEBcount->Fill(uFebIdx);

  fhMuchFebChanCntRaw[uFebIdx]->Fill(usChan);  //change the name of histogram
  fhMuchFebADC[uFebIdx]->Fill(usChan, usRawAdc);
  fvulChanLastHitTime[uAsicIdx][usChan] = usRawTs;
  fvulChanLastHitTime[uAsicIdx][usChan] +=
    static_cast<ULong64_t>(stsxyter::kuHitNbTsBins) * static_cast<ULong64_t>(fvulCurrentTsMsb[fuCurrDpbIdx])
    + static_cast<ULong64_t>(stsxyter::kulTsCycleNbBins) * static_cast<ULong64_t>(fvuCurrentTsMsbCycle[fuCurrDpbIdx]);
  /// => Quick and dirty hack for binning FW!!!
  if (kTRUE == fbBinningFw)
    fvulChanLastHitTime[uAsicIdx][usChan] =
      usRawTs
      + static_cast<ULong64_t>(stsxyter::kuHitNbTsBinsBinning) * static_cast<ULong64_t>(fvulCurrentTsMsb[fuCurrDpbIdx])
      + static_cast<ULong64_t>(stsxyter::kulTsCycleNbBinsBinning)
          * static_cast<ULong64_t>(fvuCurrentTsMsbCycle[fuCurrDpbIdx]);

  // Convert the Hit time in bins to Hit time in ns
  // Long64_t dHitTimeNs = fvulChanLastHitTime[uAsicIdx][usChan] * stsxyter::kdClockCycleNs;

  fvdChanLastHitTime[uAsicIdx][usChan] = fvulChanLastHitTime[uAsicIdx][usChan] * stsxyter::kdClockCycleNs;

  // Check Starting point of histos with time as X axis
  if (-1 == fdStartTime) fdStartTime = fvdChanLastHitTime[uAsicIdx][usChan];

  Int_t constime = (fvdChanLastHitTime[uAsicIdx][usChan] - prevtime_new);

  if (constime < 10000000) {
    if (usRawAdc > 1) { Counter1++; }
    Counter++;
  }
  else {
    //fhRate->Fill(Counter);
    // fhRateAdcCut->Fill(Counter1);
    Counter      = 0;
    Counter1     = 0;
    prevtime_new = fvdChanLastHitTime[uAsicIdx][usChan];
  }
  // Fill histos with time as X axis
  Double_t dTimeSinceStartSec = (fvdChanLastHitTime[uAsicIdx][usChan] - fdStartTime) * 1e-9;  //uTimeBin
  /* LOG(info) << "fvul Hit Time " << fvulChanLastHitTime[uAsicIdx][usChan] << " clock cycle Ns " << stsxyter::kdClockCycleNs;
  LOG(info) << "Last Hit Time " << fvdChanLastHitTime[uAsicIdx][usChan];
  LOG(info) << "1st Hit Time " << fdStartTime; */
  //LOG(info) << "Spill Time " << dTimeSinceStartSec;
  fhMuchFebSpill[uFebIdx]->Fill(dTimeSinceStartSec);
  fhMuchFebChanCntRaw[uFebIdx]->Fill(usChan);
  fhMuchChannelTime[uFebIdx]->Fill(dTimeSinceStartSec, usChan);

  //---------------------------------------
  /* Below code for Monitoring Trapezoidal shape of REAL GEM and RPC Module. RealX and RealY taken from par file. */
  //Below FebID is according to FEB Position in Module GEM A or Module GEM B (Carefully write MUCH Par file)
  /*
  Int_t FebId = fUnpackParMuch->GetFebId(uAsicIdx);

  //UInt_t uChanInFeb = usChan + fUnpackParMuch->GetNbChanPerAsic() * (uAsicIdx % fUnpackParMuch->GetNbAsicsPerFeb());
  Int_t ModuleNr    = fUnpackParMuch->GetModule(uAsicIdx);
  Int_t sector      = fUnpackParMuch->GetPadXA(FebId, usChan);
  Int_t channel     = fUnpackParMuch->GetPadYA(FebId, usChan);

  //Convert into Real X Y Position
  Double_t ActualX = fUnpackParMuch->GetRealX(channel, sector);
  Double_t ActualY = fUnpackParMuch->GetRealPadSize(channel, sector);

  //Converting Module (Small side up)
  ActualX = 1000 - ActualX;
  channel = 96 - channel;

  LOG(debug) << "Module Nr " << ModuleNr << " Sector Nr " << sector << " Channel Nr " << channel << "Actual X "
             << ActualX << "Actual Y " << ActualY << "uAsicIdx " << uAsicIdx;

  fHistPadDistr[ModuleNr]->Fill(sector, channel);
  fRealHistPadDistr[ModuleNr]->Fill(ActualY, ActualX);
  */
  //----------------------------------------------------------
}

void CbmMcbm2018MonitorAlgoMuchLite::FillTsMsbInfo(stsxyter::Message mess, UInt_t /*uMessIdx*/, UInt_t /*uMsIdx*/)
{
  UInt_t uVal = mess.GetTsMsbVal();
  /// => Quick and dirty hack for binning FW!!!
  if (kTRUE == fbBinningFw) uVal = mess.GetTsMsbValBinning();

  // Update Status counters
  if (uVal < fvulCurrentTsMsb[fuCurrDpbIdx]) {

    fvuCurrentTsMsbCycle[fuCurrDpbIdx]++;
  }  // if( uVal < fvulCurrentTsMsb[fuCurrDpbIdx] )
  fvulCurrentTsMsb[fuCurrDpbIdx] = uVal;
  /*
   ULong64_t ulNewTsMsbTime =  static_cast< ULong64_t >( stsxyter::kuHitNbTsBins )
                             * static_cast< ULong64_t >( fvulCurrentTsMsb[fuCurrDpbIdx])
                             + static_cast< ULong64_t >( stsxyter::kulTsCycleNbBins )
                             * static_cast< ULong64_t >( fvuCurrentTsMsbCycle[fuCurrDpbIdx] );
*/
}

void CbmMcbm2018MonitorAlgoMuchLite::FillEpochInfo(stsxyter::Message /*mess*/)
{
  //   UInt_t uVal    = mess.GetEpochVal();
  //   UInt_t uCurrentCycle = uVal % stsxyter::kulTsCycleNbBins;
}

void CbmMcbm2018MonitorAlgoMuchLite::SaveAllHistos(TString sFileName)
{
  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  TFile* histoFile = NULL;
  if ("" != sFileName) {
    // open separate histo file in recreate mode
    histoFile = new TFile(sFileName, "RECREATE");
    histoFile->cd();
  }  // if( "" != sFileName )

  /***************************/
  gDirectory->mkdir("Much_Raw");
  gDirectory->cd("Much_Raw");

  for (UInt_t uModuleId = 0; uModuleId < 2; ++uModuleId) {
    fHistPadDistr[uModuleId]->Write();
    fRealHistPadDistr[uModuleId]->Write();
    // fhMuchFebDuplicateHitProf[uModuleId]->Write();
  }
  //  fhRate->Write();
  //  fhRateAdcCut->Write();
  fhFEBcount->Write();
  //   fhMuchMessType->Write();
  //  fhMuchSysMessType->Write();
  //   fhMuchMessTypePerDpb->Write();
  //  fhMuchSysMessTypePerDpb->Write();
  // fhStatusMessType->Write();
  fhMsStatusFieldType->Write();
  fhMuchHitsElinkPerDpb->Write();
  // fhMuchFebChanAdcRaw_combined->Write();
  fhDpbMsErrors->Write();
  gDirectory->cd("..");
  /***************************/

  /***************************/
  gDirectory->mkdir("Much_Feb");
  gDirectory->cd("Much_Feb");
  for (UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx) {
    if (kTRUE == fUnpackParMuch->IsFebActive(uFebIdx)) {
      fhMuchFebChanCntRaw[uFebIdx]->Write();
      fhMuchFebSpill[uFebIdx]->Write();
      fhMuchChannelTime[uFebIdx]->Write();
      fhMuchFebADC[uFebIdx]->Write();
      //fhMuchFebChanCntRawGood[ uFebIdx ]->Write();
      // fhMuchFebChanAdcRaw[ uFebIdx ]->Write();
      // fhMuchFebChanAdcRawProf[ uFebIdx ]->Write();
      //fhMuchFebChanAdcCal[ uFebIdx ]->Write();
      //fhMuchFebChanAdcCalProf[ uFebIdx ]->Write();
      // fhMuchFebChanRawTs[ uFebIdx ]->Write();
      // fhMuchFebChanHitRateProf[ uFebIdx ]->Write();
      //fhMuchFebAsicHitRateEvo[ uFebIdx ]->Write();
      // fhMuchFebHitRateEvo[ uFebIdx ]->Write();
      // fhMuchFebHitRateEvo_mskch[ uFebIdx ]->Write();
      // fhMuchFebHitRateEvo_mskch_adccut[ uFebIdx ]->Write();
      // fhMuchFebHitRateEvo_WithoutDupli[ uFebIdx ]->Write();
      /*fhMuchFebChanHitRateEvoLong[ uFebIdx ]->Write();
         fhMuchFebAsicHitRateEvoLong[ uFebIdx ]->Write();
         fhMuchFebHitRateEvoLong[ uFebIdx ]->Write();
         fhMuchFebChanDistT[ uFebIdx ]->Write();
         for( UInt_t uFebIdxB = uFebIdx; uFebIdxB < fuNbFebs; ++uFebIdxB )
         {
            fhMuchFebChanDtCoinc[ uFebIdx ][ uFebIdxB ]->Write();
            fhMuchFebChanCoinc[ uFebIdx ][ uFebIdxB ]->Write();
         } // for( UInt_t uFebIdxB = uFebIdx; uFebIdxB < fuNbFebs; ++uFebIdxB )*/
    }  // if( kTRUE == fUnpackParMuch->IsFebActive( uFebIdx ) )
  }    // for( UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx )
  gDirectory->cd("..");
  /***************************/

  /***************************/
  // Flib Histos
  gDirectory->mkdir("Flib_Raw");
  gDirectory->cd("Flib_Raw");
  for (UInt_t uLinks = 0; uLinks < kiMaxNbFlibLinks; uLinks++) {
    TString sMsSzName = Form("MsSz_link_%02u", uLinks);
    if (fHM->Exists(sMsSzName.Data())) fHM->H1(sMsSzName.Data())->Write();

    sMsSzName = Form("MsSzTime_link_%02u", uLinks);
    if (fHM->Exists(sMsSzName.Data())) fHM->P1(sMsSzName.Data())->Write();
  }  // for( UInt_t uLinks = 0; uLinks < 16; uLinks ++)

  /***************************/

  if ("" != sFileName) {
    // Restore original directory position
    histoFile->Close();
  }  // if( "" != sFileName )

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;
}

void CbmMcbm2018MonitorAlgoMuchLite::ResetAllHistos()
{
  LOG(info) << "Reseting all Much histograms.";

  for (UInt_t uModuleId = 0; uModuleId < 2; ++uModuleId) {
    //fHistPadDistr[uModuleId]->Reset();
    //fRealHistPadDistr[uModuleId]->Reset();
    fhMuchFebDuplicateHitProf[uModuleId]->Reset();
  }
  // fhRate->Reset();
  // fhRateAdcCut->Reset();
  fhFEBcount->Reset();
  // fhMuchMessType->Reset();
  // fhMuchSysMessType->Reset();
  // fhMuchMessTypePerDpb->Reset();
  // fhMuchSysMessTypePerDpb->Reset();
  // fhStatusMessType->Reset();
  fhMsStatusFieldType->Reset();
  fhMuchHitsElinkPerDpb->Reset();
  // fhMuchFebChanAdcRaw_combined->Reset();
  fhDpbMsErrors->Reset();

  for (UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx) {
    if (kTRUE == fUnpackParMuch->IsFebActive(uFebIdx)) {
      fhMuchFebChanCntRaw[uFebIdx]->Reset();
      fhMuchFebSpill[uFebIdx]->Reset();
      fhMuchChannelTime[uFebIdx]->Reset();
      fhMuchFebADC[uFebIdx]->Reset();
      //fhMuchFebChanCntRawGood[ uFebIdx ]->Reset();
      //// fhMuchFebChanAdcRaw[ uFebIdx ]->Reset();
      //fhMuchFebChanAdcRawProf[ uFebIdx ]->Reset();
      //fhMuchFebChanAdcCal[ uFebIdx ]->Reset();
      //fhMuchFebChanAdcCalProf[ uFebIdx ]->Reset();
      ////fhMuchFebChanRawTs[ uFebIdx ]->Reset();
      //    fhMuchFebChanHitRateEvo[ uFebIdx ]->Reset();
      // fhMuchFebChanHitRateProf[ uFebIdx ]->Reset();
      //fhMuchFebAsicHitRateEvo[ uFebIdx ]->Reset();
      //// fhMuchFebHitRateEvo[ uFebIdx ]->Reset();
      // fhMuchFebHitRateEvo_mskch[ uFebIdx ]->Reset();
      //  fhMuchFebHitRateEvo_mskch_adccut[ uFebIdx ]->Reset();
      //   fhMuchFebHitRateEvo_WithoutDupli[ uFebIdx ]->Reset();
      /*fhMuchFebChanHitRateEvoLong[ uFebIdx ]->Reset();
         fhMuchFebAsicHitRateEvoLong[ uFebIdx ]->Reset();
         fhMuchFebHitRateEvoLong[ uFebIdx ]->Reset();
         fhMuchFebChanDistT[ uFebIdx ]->Reset();
         for( UInt_t uFebIdxB = uFebIdx; uFebIdxB < fuNbFebs; ++uFebIdxB )
         {
            fhMuchFebChanDtCoinc[ uFebIdx ][ uFebIdxB ]->Reset();
            fhMuchFebChanCoinc[ uFebIdx ][ uFebIdxB ]->Reset();
         } // for( UInt_t uFebIdxB = uFebIdx; uFebIdxB < fuNbFebs; ++uFebIdxB )*/
    }  // if( kTRUE == fUnpackParMuch->IsFebActive( uFebIdx ) )
  }    // for( UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx )

  for (UInt_t uLinks = 0; uLinks < kiMaxNbFlibLinks; ++uLinks) {
    TString sMsSzName = Form("MsSz_link_%02u", uLinks);
    if (fHM->Exists(sMsSzName.Data())) fHM->H1(sMsSzName.Data())->Reset();

    sMsSzName = Form("MsSzTime_link_%02u", uLinks);
    if (fHM->Exists(sMsSzName.Data())) fHM->P1(sMsSzName.Data())->Reset();
  }  // for( UInt_t uLinks = 0; uLinks < kiMaxNbFlibLinks; ++uLinks )

  fdStartTime     = -1;
  fdStartTimeMsSz = -1;
}

void CbmMcbm2018MonitorAlgoMuchLite::SetRunStart(Int_t dateIn, Int_t timeIn, Int_t iBinSize)
{
  TDatime* fRunStartDateTime = new TDatime(dateIn, timeIn);
  fiRunStartDateTimeSec      = fRunStartDateTime->Convert();
  fiBinSizeDatePlots         = iBinSize;

  LOG(info) << "Assigned new MUCH Run Start Date-Time: " << fRunStartDateTime->AsString();
}

///------------------------------------------------------------------///
Bool_t CbmMcbm2018MonitorAlgoMuchLite::ScanForNoisyChannels(Double_t dNoiseThreshold)
{
  for (UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx) {
    if (kTRUE == fUnpackParMuch->IsFebActive(uFebIdx)) {
      LOG(info) << Form(" ------------------ Noisy channels scan for FEB %2d ------------", uFebIdx);
      for (UInt_t uAsicIdx = 0; uAsicIdx < fUnpackParMuch->GetNbAsicsPerFeb(); ++uAsicIdx)
        for (UInt_t uChanIdx = 0; uChanIdx < fUnpackParMuch->GetNbChanPerAsic(); ++uChanIdx) {
          UInt_t uChanInFeb = uAsicIdx * fUnpackParMuch->GetNbChanPerAsic() + uChanIdx;
          if (dNoiseThreshold < fhMuchFebChanHitRateProf[uFebIdx]->GetBinContent(1 + uChanInFeb))
            LOG(info) << Form("Noisy Channel ASIC %d channel %3d (%4d) level %6.0f", uAsicIdx, uChanIdx, uChanInFeb,
                              fhMuchFebChanHitRateProf[uFebIdx]->GetBinContent(1 + uChanInFeb));

        }  // for( UInt_t uChanIdx = 0; uChanIdx < fUnpackParMuch->GetNbChanPerAsic(); ++uChanIdx )

      LOG(info) << " ---------------------------------------------------------------";
    }  // if( kTRUE == fUnpackParMuch->IsFebActive( uFebIdx ) )
  }    // for( UInt_t uFebIdx = 0; uFebIdx < fuNbFebs; ++uFebIdx )
  return kTRUE;
}

//-----------------------------------------------------------------//

ClassImp(CbmMcbm2018MonitorAlgoMuchLite)
