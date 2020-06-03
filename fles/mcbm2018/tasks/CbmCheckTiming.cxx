#/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "CbmCheckTiming.h"

#include "CbmDigiManager.h"
#include "CbmStsDigi.h"
#include "CbmMuchBeamTimeDigi.h"
#include "CbmRichDigi.h"
#include "CbmTofDigi.h"
#include "CbmPsdDigi.h"
#include "CbmFlesHistosTools.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunOnline.h"

#include "TClonesArray.h"
#include "TH1.h"
#include "TH2.h"
#include "THttpServer.h"
#include <TFile.h>

#include <iomanip>
#include <iostream>
using std::fixed;
using std::setprecision;

// ---- Default constructor -------------------------------------------
CbmCheckTiming::CbmCheckTiming()
  : FairTask("CbmCheckTiming")
{
}

// ---- Destructor ----------------------------------------------------
CbmCheckTiming::~CbmCheckTiming()
{
}

// ----  Initialisation  ----------------------------------------------
void CbmCheckTiming::SetParContainers()
{
  // Load all necessary parameter containers from the runtime data base
  /*
  FairRunAna* ana = FairRunAna::Instance();
  FairRuntimeDb* rtdb=ana->GetRuntimeDb();

  <CbmCheckTimingDataMember> = (<ClassPointer>*)
    (rtdb->getContainer("<ContainerName>"));
  */
}

// ---- Init ----------------------------------------------------------
InitStatus CbmCheckTiming::Init()
{

  // Get a handle from the IO manager
  FairRootManager* ioman = FairRootManager::Instance();
  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->UseMuchBeamTimeDigi();
  fDigiMan->Init();

  // Get a pointer to the previous already existing data level
  fT0DigiVec = ioman->InitObjectAs<std::vector<CbmTofDigi> const *>("T0Digi");
  if ( ! fT0DigiVec ) {
    fT0DigiArr = dynamic_cast<TClonesArray*>(ioman->GetObject("T0Digi"));
    if ( ! fT0DigiArr ) {
      LOG(fatal) << "No TClonesArray with T0 digis found.";
    }
  }

  if ( ! fDigiMan->IsPresent(ECbmModuleId::kSts) ) {
    LOG(info) << "No STS digis found.";
  }

  if ( ! fDigiMan->IsPresent(ECbmModuleId::kMuch) ) {
    LOG(info) << "No MUCH digis found.";
  }

  if ( ! fDigiMan->IsPresent(ECbmModuleId::kTof) ) {
    LOG(info) << "No TOF digis found.";
  }

  if ( ! fDigiMan->IsPresent(ECbmModuleId::kRich) ) {
    LOG(info) << "No RICH digis found.";
  }

  if ( ! fDigiMan->IsPresent(ECbmModuleId::kPsd) ) {
    LOG(info) << "No PSD digis found.";
  }

  if (fCheckInterSystemOffset) CreateHistos();

  return kSUCCESS;
}

Int_t CbmCheckTiming::CalcNrBins(Int_t offsetRange) {

  if (offsetRange < 1001) {
    fBinWidth = 5;
  } else if (offsetRange < 10001) {
    fBinWidth = 10;
  } else if (offsetRange < 100001) {
    fBinWidth = 100;
  } else {
    fBinWidth = 100;
  }

  return (offsetRange/fBinWidth *2);
}

void CbmCheckTiming::CreateHistos()
{
   /// Logarithmic bining for self time comparison
  uint32_t iNbBinsLog = 0;
    /// Parameters are NbDecadesLog, NbStepsDecade, NbSubStepsInStep
  std::vector<double> dBinsLogVector = GenerateLogBinArray( 9, 9, 1, iNbBinsLog );
  double* dBinsLog = dBinsLogVector.data();
//  double * dBinsLog = GenerateLogBinArray( 9, 9, 1, iNbBinsLog );

  Int_t nrOfBinsSts =  CalcNrBins(fStsOffsetRange);
  // T0 vs. Sts
  fT0StsDiff = new TH1F("fT0StsDiff","T0-Sts;time diff [ns];Counts",
			nrOfBinsSts, -fStsOffsetRange, fStsOffsetRange);

  fT0StsDiffCharge = new TH2F("fT0StsDiffCharge","T0-Sts;time diff [ns]; Charge [a.u]; Counts",
			nrOfBinsSts, -fStsOffsetRange, fStsOffsetRange,
      256, 0, 256
      );

  fT0StsDiffEvo = new TH2F("fT0StsDiffEvo","T0-Sts;TS; time diff [ns];Counts",
       1000, 0, 10000,
			nrOfBinsSts, -fStsOffsetRange, fStsOffsetRange);


  Int_t nrOfBinsMuch =  CalcNrBins(fMuchOffsetRange);
  // T0 vs. Much
  fT0MuchDiff = new TH1F("fT0MuchDiff","T0-Much;time diff [ns];Counts",
			 nrOfBinsMuch, -fMuchOffsetRange, fMuchOffsetRange);

  fT0MuchDiffCharge = new TH2F("fT0MuchDiffCharge","T0-Much;time diff [ns]; Charge [a.u]; ;Counts",
			 nrOfBinsMuch, -fMuchOffsetRange, fMuchOffsetRange,
      256, 0, 256
       );

  fT0MuchDiffEvo = new TH2F("fT0MuchDiffEvo","T0-Much;TS; time diff [ns];Counts",
       1000, 0, 10000,
			 nrOfBinsMuch, -fMuchOffsetRange, fMuchOffsetRange);


  Int_t nrOfBinsTof =  CalcNrBins(fTofOffsetRange);
  // To vs. Tof
  fT0TofDiff = new TH1F("fT0TofDiff","T0-Tof;time diff [ns];Counts",
			nrOfBinsTof, -fTofOffsetRange, fTofOffsetRange);

  fT0TofDiffCharge = new TH2F("fT0TofDiffCharge","T0-Tof;time diff [ns]; Charge [a.u]; ;Counts",
			nrOfBinsTof, -fTofOffsetRange, fTofOffsetRange,
      256, 0, 256
      );

  fT0TofDiffEvo = new TH2F("fT0TofDiffEvo","T0-Tof;TS; time diff [ns];Counts",
       1000, 0, 10000,
			nrOfBinsTof, -fTofOffsetRange, fTofOffsetRange);


  Int_t nrOfBinsRich =  CalcNrBins(fRichOffsetRange);
  // To vs. Rich
  fT0RichDiff = new TH1F("fT0RichDiff","T0-Rich;time diff [ns];Counts",
			nrOfBinsRich, -fRichOffsetRange, fRichOffsetRange);

  fT0RichDiffCharge = new TH2F("fT0RichDiffCharge","T0-Rich;time diff [ns]; Charge [a.u]; ;Counts",
			nrOfBinsRich, -fRichOffsetRange, fRichOffsetRange,
      256, 0, 256
      );

  fT0RichDiffEvo = new TH2F("fT0RichDiffEvo","T0-Rich;TS; time diff [ns];Counts",
       1000, 0, 10000,
			nrOfBinsRich, -fRichOffsetRange, fRichOffsetRange);

  Int_t nrOfBinsPsd =  CalcNrBins(fPsdOffsetRange);
  // To vs. Psd
  fT0PsdDiff = new TH1F("fT0PsdDiff","T0-Psd;time diff [ns];Counts",
			nrOfBinsPsd, -fPsdOffsetRange, fPsdOffsetRange);

  fT0PsdDiffCharge = new TH2F("fT0PsdDiffCharge","T0-Psd;time diff [ns]; Charge [a.u]; ;Counts",
			nrOfBinsPsd, -fPsdOffsetRange, fPsdOffsetRange,
      7000, 0, 70000
      );

  fT0PsdDiffEvo = new TH2F("fT0PsdDiffEvo","T0-Psd;TS; time diff [ns];Counts",
       1000, 0, 10000,
			nrOfBinsPsd, -fPsdOffsetRange, fPsdOffsetRange);

  // T0 vs. Sts
  fT0StsDiffEvoLong = new TH2F("fT0StsDiffEvoLong","T0-Sts;TS; time diff [ns];Counts",
       1800, 0, 180000,
			nrOfBinsSts, -fStsOffsetRange, fStsOffsetRange);
  // T0 vs. Much
  fT0MuchDiffEvoLong = new TH2F("fT0MuchDiffEvoLong","T0-Much;TS; time diff [ns];Counts",
       1800, 0, 180000,
			 nrOfBinsMuch, -fMuchOffsetRange, fMuchOffsetRange);
  // To vs. Tof
  fT0TofDiffEvoLong = new TH2F("fT0TofDiffEvoLong","T0-Tof;TS; time diff [ns];Counts",
       1800, 0, 180000,
			nrOfBinsTof, -fTofOffsetRange, fTofOffsetRange);
  // To vs. Rich
  fT0RichDiffEvoLong = new TH2F("fT0RichDiffEvoLong","T0-Rich;TS; time diff [ns];Counts",
       1800, 0, 180000,
			nrOfBinsRich, -fRichOffsetRange, fRichOffsetRange);

  // To vs. Psd
  fT0PsdDiffEvoLong = new TH2F("fT0PsdDiffEvoLong","T0-Psd;TS; time diff [ns];Counts",
       1800, 0, 180000,
			nrOfBinsPsd, -fPsdOffsetRange, fPsdOffsetRange);

  // T0 vs. STS for the different DPBs
  fT0StsDpbDiff = new TH2F("fT0StsDpbDiff","T0-Much;DPB; time diff [ns];Counts",
       2, -0.5, 1.5, nrOfBinsSts, -fStsOffsetRange, fStsOffsetRange);

  for( UInt_t uStsDpb = 0; uStsDpb < kuMaxNbStsDpbs; ++uStsDpb )
  {
    fT0StsDpbDiffEvo[uStsDpb] = new TH2F( Form("fT0StsDpbDiffEvo%02u", uStsDpb),
      Form( "T0-STS DPB %02u;TS; time diff [ns];Counts", uStsDpb),
       1800, 0, 180000,
			nrOfBinsSts, -fStsOffsetRange, fStsOffsetRange);
    fStsDpbCntsEvo[uStsDpb] = new TH1F( Form("fStsDpbCntsEvo%02u", uStsDpb),
      Form( "Time STS DPB %02u;TS; Hit Counts", uStsDpb),
       1800, 0, 180000);
  } // for( UInt_t uStsDpb = 0; uStsDpb < kuMaxNbStsDpbs; ++uStsDpb )

  // T0 vs. Much for the different DPBs/AFCK
  fT0MuchRocDiff = new TH2F("fT0MuchRocDiff","T0-Much;AFCK; time diff [ns];Counts",
       kuMaxNbMuchDpbs, -0.5, kuMaxNbMuchDpbs - 0.5, nrOfBinsMuch, -fMuchOffsetRange, fMuchOffsetRange);

  // T0 vs. Much for the different ASICs
  fT0MuchAsicDiff = new TH2F("fT0MuchAsicDiff","T0-Much;ASIC; time diff [ns];Counts",
       kuMaxNbMuchAsics, -0.5, kuMaxNbMuchAsics - 0.5, nrOfBinsMuch, -fMuchOffsetRange, fMuchOffsetRange);

  for( UInt_t uMuchAsic = 0; uMuchAsic < kuMaxNbMuchAsics; ++uMuchAsic )
    fT0MuchAsicDiffEvo[uMuchAsic] = new TH2F( Form("fT0MuchAsicDiffEvo%02u", uMuchAsic),
      Form( "T0-Much ASIC %02u;TS; time diff [ns];Counts", uMuchAsic),
       1800, 0, 180000,
			nrOfBinsMuch, -fMuchOffsetRange, fMuchOffsetRange);

  // T0 vs. T0
  fT0T0Diff = new TH1F("fT0T0Diff","T0-T0_prev;time diff [ns];Counts",
		       iNbBinsLog, dBinsLog);
  // sts vs. Sts
  fStsStsDiff = new TH1F("fStsStsDiff","Sts-Sts_prev;time diff [ns];Counts",
		       iNbBinsLog, dBinsLog);
  // Much vs. Much
  fMuchMuchDiff = new TH1F("fMuchMuchDiff","Much-Much_prev;time diff [ns];Counts",
		       iNbBinsLog, dBinsLog);
  // Tof vs. Tof
  fTofTofDiff = new TH1F("fTofTofDiff","Tof-Tof_prev;time diff [ns];Counts",
		       iNbBinsLog, dBinsLog);
  // Rich vs. Rich
  fRichRichDiff = new TH1F("fRichRichDiff","Rich-Rich_prev;time diff [ns];Counts",
		       iNbBinsLog, dBinsLog);
  // Psd vs. Psd
  fPsdPsdDiff = new TH1F("fPsdPsdDiff","Psd-Psd_prev;time diff [ns];Counts",
		       iNbBinsLog, dBinsLog);

  fT0Address = new TH1F("fT0Address","T0 address;address;Counts",
                        1000000, 0, 1000000.);

  fT0Channel = new TH1F("fT0Channel","T0 channel;channel nr;Counts",
                        100, -0.5, 99.5);

  fSelT0StsDiff = new TH1F("fSelT0StsDiff","T0-Sts if T0 in coinc with TOF;time diff [ns];Counts",
			nrOfBinsSts, -fStsOffsetRange, fStsOffsetRange);
  fSelT0MuchDiff = new TH1F("fSelT0MuchDiff","T0-Much if T0 in coinc with TOF;time diff [ns];Counts",
			 nrOfBinsMuch, -fMuchOffsetRange, fMuchOffsetRange);
  fSelT0TofDiff = new TH1F("fSelT0TofDiff","T0-Tof if T0 in coinc with TOF;time diff [ns];Counts",
			nrOfBinsTof, -fTofOffsetRange, fTofOffsetRange);
  fSelT0RichDiff = new TH1F("fSelT0RichDiff","T0-Rich if T0 in coinc with TOF;time diff [ns];Counts",
			nrOfBinsRich, -fRichOffsetRange, fRichOffsetRange);
  fSelT0PsdDiff = new TH1F("fSelT0PsdDiff","T0-Psd if T0 in coinc with TOF;time diff [ns];Counts",
			nrOfBinsPsd, -fPsdOffsetRange, fPsdOffsetRange);

  /// Cleanup array of log bins
//  delete dBinsLog;

  /// Register the histos in the HTTP server
  FairRunOnline* run = FairRunOnline::Instance();
  if (run) {
    THttpServer* server = run->GetHttpServer();
    if( nullptr != server ) {
      server->Register("/CheckTiming", fT0StsDiff);
      server->Register("/CheckTiming", fT0MuchDiff);
      server->Register("/CheckTiming", fT0TofDiff);
      server->Register("/CheckTiming", fT0RichDiff);
      server->Register("/CheckTiming", fT0PsdDiff);
      server->Register("/CheckTiming", fT0StsDiffCharge);
      server->Register("/CheckTiming", fT0MuchDiffCharge);
      server->Register("/CheckTiming", fT0TofDiffCharge);
      server->Register("/CheckTiming", fT0RichDiffCharge);
      server->Register("/CheckTiming", fT0PsdDiffCharge);
      server->Register("/CheckTiming", fT0StsDiffEvo);
      server->Register("/CheckTiming", fT0MuchDiffEvo);
      server->Register("/CheckTiming", fT0TofDiffEvo);
      server->Register("/CheckTiming", fT0RichDiffEvo);
      server->Register("/CheckTiming", fT0PsdDiffEvo);
      server->Register("/CheckTiming", fT0StsDiffEvoLong);
      server->Register("/CheckTiming", fT0MuchDiffEvoLong);
      server->Register("/CheckTiming", fT0TofDiffEvoLong);
      server->Register("/CheckTiming", fT0RichDiffEvoLong);
      server->Register("/CheckTiming", fT0PsdDiffEvoLong);
      server->Register("/CheckTiming", fT0T0Diff);
      server->Register("/CheckTiming", fStsStsDiff);
      server->Register("/CheckTiming", fMuchMuchDiff);
      server->Register("/CheckTiming", fTofTofDiff);
      server->Register("/CheckTiming", fRichRichDiff);
      server->Register("/CheckTiming", fPsdPsdDiff);

      server->Register("/CheckTiming", fT0StsDpbDiff);
      for( UInt_t uStsDpb = 0; uStsDpb < kuMaxNbStsDpbs; ++uStsDpb )
        server->Register("/CheckTiming/sts", fT0StsDpbDiffEvo[uStsDpb]);

      server->Register("/CheckTiming", fT0MuchRocDiff);
      server->Register("/CheckTiming", fT0MuchAsicDiff);
      for( UInt_t uMuchAsic = 0; uMuchAsic < kuMaxNbMuchAsics; ++uMuchAsic )
        server->Register("/CheckTiming/much", fT0MuchAsicDiffEvo[uMuchAsic]);

      server->Register("/CheckTiming", fSelT0StsDiff);
      server->Register("/CheckTiming", fSelT0MuchDiff);
      server->Register("/CheckTiming", fSelT0TofDiff);
      server->Register("/CheckTiming", fSelT0RichDiff);
      server->Register("/CheckTiming", fSelT0PsdDiff);
    }
  }
}
// ---- ReInit  -------------------------------------------------------
InitStatus CbmCheckTiming::ReInit()
{
  return kSUCCESS;
}

// ---- Exec ----------------------------------------------------------
void CbmCheckTiming::Exec(Option_t* /*option*/)
{
  LOG(debug) << "executing TS " << fNrTs;

  if (fCheckTimeOrdering) CheckTimeOrder();
  if (fCheckInterSystemOffset) CheckInterSystemOffset();

  fNrTs++;
}

void CbmCheckTiming::CheckInterSystemOffset()
{
  LOG(debug) <<"Begin";
  Int_t nrT0Digis = 0;
  if ( fT0DigiVec ) nrT0Digis = fT0DigiVec->size();
  else if ( fT0DigiArr ) nrT0Digis = fT0DigiArr->GetEntriesFast();
  LOG(debug) << "T0Digis: " << nrT0Digis;

  Int_t nrStsDigis{0};
  if (fDigiMan->IsPresent(ECbmModuleId::kSts)) {
    nrStsDigis=fDigiMan->GetNofDigis(ECbmModuleId::kSts);
    LOG(debug) << "StsDigis: " << nrStsDigis;
  }

  Int_t nrMuchDigis{0};
  if (fDigiMan->IsPresent(ECbmModuleId::kMuch)) {
    nrMuchDigis=fDigiMan->GetNofDigis(ECbmModuleId::kMuch);
    LOG(debug) << "MuchDigis: " << nrMuchDigis;
  }

  Int_t nrTofDigis{0};
  if (fDigiMan->IsPresent(ECbmModuleId::kTof)) {
    nrTofDigis=fDigiMan->GetNofDigis(ECbmModuleId::kTof);
    LOG(debug) << "TofDigis: " << nrTofDigis;
  }

  Int_t nrRichDigis{0};
  if (fDigiMan->IsPresent(ECbmModuleId::kRich)) {
    nrRichDigis=fDigiMan->GetNofDigis(ECbmModuleId::kRich);
    LOG(debug) << "RichDigis: " << nrRichDigis;
  }

  Int_t nrPsdDigis{0};
  if (fDigiMan->IsPresent(ECbmModuleId::kPsd)) {
    nrPsdDigis=fDigiMan->GetNofDigis(ECbmModuleId::kPsd);
    LOG(debug) << "PsdDigis: " << nrPsdDigis;
  }

//  if (nrT0Digis < 100000) {
  if (nrT0Digis < 1000000) {
    /// Re-initialize array references
    fPrevT0FirstDigiSts = 0.;
    fPrevT0FirstDigiMuch = 0.;
    fPrevT0FirstDigiTof = 0.;
    fPrevT0FirstDigiRich = 0.;
    fPrevT0FirstDigiPsd = 0.;

    fvdTimeSelectedT0.clear();

    for (Int_t iT0 = 0; iT0 < nrT0Digis; ++iT0) {

      if (iT0%1000 == 0) LOG(debug) << "Executing entry " << iT0;

      const CbmTofDigi* T0Digi = nullptr;
      if ( fT0DigiVec ) T0Digi = &(fT0DigiVec->at(iT0));
      else if ( fT0DigiArr ) T0Digi = dynamic_cast<const CbmTofDigi*>(fT0DigiArr->At(iT0));
      assert (T0Digi);

      /// Skip pulser events.
      if( fuMinTotPulserT0 < T0Digi->GetCharge() && T0Digi->GetCharge() < fuMaxTotPulserT0 )
        continue;

      Double_t T0Time = T0Digi->GetTime();
      Int_t T0Address = T0Digi->GetAddress();
      fT0Address->Fill(T0Address);

      fT0Channel->Fill(T0Digi->GetChannel());

      if (nrStsDigis > 0 && nrStsDigis < 1000000)
        fPrevT0FirstDigiSts = FillSystemOffsetHistos<CbmStsDigi>(fT0StsDiff, fT0StsDiffCharge, fT0StsDiffEvo,
                                                      fT0StsDiffEvoLong, fT0StsDpbDiff,
                                                      T0Time, fStsOffsetRange, fPrevT0FirstDigiSts,
                                                      kTRUE, kFALSE, kFALSE, kFALSE);
      /// Templating to CbmMuchBeamTimeDigi fails with a bad any_cast due to hardcoded Digi type in
      /// CbmDigiManager!
      if (nrMuchDigis > 0 && nrMuchDigis < 1000000)
        fPrevT0FirstDigiMuch = FillSystemOffsetHistos<CbmMuchBeamTimeDigi>(fT0MuchDiff, fT0MuchDiffCharge, fT0MuchDiffEvo,
                                                      fT0MuchDiffEvoLong, fT0MuchRocDiff,
                                                      T0Time, fMuchOffsetRange, fPrevT0FirstDigiMuch,
                                                      kFALSE, kTRUE, kFALSE, kFALSE);
      fuNbTofDigiInSync = 0;
      if (nrTofDigis > 0 && nrTofDigis < 1000000)
        fPrevT0FirstDigiTof = FillSystemOffsetHistos<CbmTofDigi>(fT0TofDiff, fT0TofDiffCharge,fT0TofDiffEvo,
                                                      fT0TofDiffEvoLong, nullptr,
                                                      T0Time, fTofOffsetRange, fPrevT0FirstDigiTof,
                                                      kFALSE, kFALSE, kTRUE, kFALSE);
      if (nrRichDigis > 0 && nrRichDigis < 1000000)
        fPrevT0FirstDigiRich = FillSystemOffsetHistos<CbmRichDigi>(fT0RichDiff, fT0RichDiffCharge,fT0RichDiffEvo,
                                                      fT0RichDiffEvoLong, nullptr,
                                                      T0Time, fRichOffsetRange, fPrevT0FirstDigiRich,
                                                      kFALSE, kFALSE, kFALSE, kFALSE);
      if (nrPsdDigis > 0 && nrPsdDigis < 1000000)
        fPrevT0FirstDigiPsd = FillSystemOffsetHistos<CbmPsdDigi>(fT0PsdDiff, fT0PsdDiffCharge,fT0PsdDiffEvo,
                                                      fT0PsdDiffEvoLong, nullptr,
                                                      T0Time, fPsdOffsetRange, fPrevT0FirstDigiPsd,
                                                      kFALSE, kFALSE, kFALSE, kTRUE);

      if( fuNbTofDigisSel <= fuNbTofDigiInSync )
        fvdTimeSelectedT0.push_back( T0Time );
    }

    /// Re-initialize array references
    fPrevT0FirstDigiSts = 0.;
    fPrevT0FirstDigiMuch = 0.;
    fPrevT0FirstDigiTof = 0.;
    fPrevT0FirstDigiRich = 0.;
    fPrevT0FirstDigiPsd = 0.;
    for( UInt_t uIdxSelT0 = 0; uIdxSelT0 < fvdTimeSelectedT0.size(); ++uIdxSelT0 )
    {
      if (nrStsDigis > 0 && nrStsDigis < 1000000)
        fPrevT0FirstDigiSts = FillHistosSelT0<CbmStsDigi>(
                                                fSelT0StsDiff,
                                                fvdTimeSelectedT0[ uIdxSelT0 ],
                                                fStsOffsetRange, fPrevT0FirstDigiSts,
                                                kTRUE, kFALSE, kFALSE, kFALSE);
      /// Templating to CbmMuchBeamTimeDigi fails with a bad any_cast due to hardcoded Digi type in
      /// CbmDigiManager!
      if (nrMuchDigis > 0 && nrMuchDigis < 1000000)
        fPrevT0FirstDigiMuch = FillHistosSelT0<CbmMuchBeamTimeDigi>(
                                                fSelT0MuchDiff,
                                                fvdTimeSelectedT0[ uIdxSelT0 ],
                                                fMuchOffsetRange, fPrevT0FirstDigiMuch,
                                                kFALSE, kTRUE, kFALSE, kFALSE);
      if (nrTofDigis > 0 && nrTofDigis < 1000000)
        fPrevT0FirstDigiTof = FillHistosSelT0<CbmTofDigi>(
                                                fSelT0TofDiff,
                                                fvdTimeSelectedT0[ uIdxSelT0 ],
                                                fTofOffsetRange, fPrevT0FirstDigiTof,
                                                kFALSE, kFALSE, kTRUE, kFALSE);
      if (nrRichDigis > 0 && nrRichDigis < 1000000)
        fPrevT0FirstDigiRich = FillHistosSelT0<CbmRichDigi>(
                                                fSelT0RichDiff,
                                                fvdTimeSelectedT0[ uIdxSelT0 ],
                                                fRichOffsetRange, fPrevT0FirstDigiRich,
                                                kFALSE, kFALSE, kFALSE, kFALSE);
      if (nrPsdDigis > 0 && nrPsdDigis < 1000000)
        fPrevT0FirstDigiPsd = FillHistosSelT0<CbmPsdDigi>(
                                                fSelT0PsdDiff,
                                                fvdTimeSelectedT0[ uIdxSelT0 ],
                                                fPsdOffsetRange, fPrevT0FirstDigiPsd,
                                                kFALSE, kFALSE, kFALSE, kTRUE);
    } //  for( UInt_t uIdxSelT0 = 0; uIdxSelT0 < fvdTimeSelectedT0.size(); ++uIdxSelT0 )
  }
}

template <class Digi>
Int_t CbmCheckTiming::FillSystemOffsetHistos(TH1* histo, TH2* histoCharge,
                                             TH2* histoEvo, TH2* histoEvoLong, TH2* histoAFCK,
                                             const Double_t T0Time, const Int_t offsetRange,
                                             Int_t iStartDigi,
                                             Bool_t bSts, Bool_t bMuch, Bool_t bTof, Bool_t bPsd )
{
  Int_t nrDigis=fDigiMan->GetNofDigis(Digi::GetSystem());
  Int_t iFirstDigiInWin = iStartDigi;

  for (Int_t i = iStartDigi; i < nrDigis; ++i) {

    const Digi* digi = fDigiMan->Get<Digi>(i);

    /// Ignore pulser events in TOF
    if( kTRUE == bTof )
      if( fuMinTotPulserTof < digi->GetCharge() && digi->GetCharge() < fuMaxTotPulserTof )
        continue;
/*
    /// Keep only hits with higher probability to be signal
    if( kTRUE == bMuch )
      if( digi->GetCharge() < 2 )
        continue;

    /// Ignore even STS channels as they have ~10-100 x more noise
    if( kTRUE == bSts )
      if( ( digi->GetCharge() < 7 || 31 == digi->GetCharge() ) ||
          ( 0 == ( digi->GetChannel() % 2 ) ) )
        continue;
*/
    /// Take selected PSD channel
    if( kTRUE == bPsd )
      if( digi->GetAddress() != ( ( 1 << 10 ) + 8 ) )
         continue;

    Double_t diffTime = T0Time - digi->GetTime();

    if (diffTime > offsetRange) {
      ++ iFirstDigiInWin; // Update Index of first digi in Win to next digi
      continue; // not yet in interesting range
    } // if (diffTime > offsetRange)
    if (diffTime < -offsetRange) break;     // already past interesting range
    histo->Fill(diffTime);
    histoCharge->Fill( diffTime, digi->GetCharge() );
    histoEvo->Fill(fNrTs, diffTime);
    histoEvoLong->Fill(fNrTs, diffTime);

    /// STS DPB mapping: ladder 1 is in DPB 1
    if (bSts && histoAFCK) {
      UInt_t uDPB = ( 0 < (digi->GetAddress() & 0x00000400 ) );
      histoAFCK->Fill(uDPB, diffTime);
      if( uDPB < kuMaxNbStsDpbs )
        fT0StsDpbDiffEvo[ uDPB ]->Fill(fNrTs, diffTime);
    } // if (bSts && histoAFCK)

    /// MUCH DPB mapping
    if (bMuch && histoAFCK) {
      const CbmMuchBeamTimeDigi* muchDigi = nullptr;
      try {
        muchDigi =
            boost::any_cast<const CbmMuchBeamTimeDigi*>( digi );
      } catch( ... ) {
          LOG( fatal ) << "Failed boost any_cast in CbmCheckTiming::FillSystemOffsetHistos for a digi of type "
                       << Digi::GetClassName();
      } // try/catch
      assert(muchDigi);
      UInt_t afck = muchDigi->GetRocId();
      UInt_t asic = muchDigi->GetNxId();
      histoAFCK->Fill(afck, diffTime);
      fT0MuchAsicDiff->Fill(asic, diffTime);
      if( asic < kuMaxNbMuchAsics )
        fT0MuchAsicDiffEvo[ asic ]->Fill(fNrTs, diffTime);
    } // if (bMuch && histoAFCK)

    /// TOF coincidence counting
    if( kTRUE == bTof  && -200 < diffTime && diffTime < 200 )
      fuNbTofDigiInSync++;
  }

  return iFirstDigiInWin;
}

template <class Digi>
Int_t CbmCheckTiming::FillHistosSelT0( TH1* histo,
                                       const Double_t T0Time, const Int_t offsetRange,
                                       Int_t iStartDigi,
                                       Bool_t /*bSts*/, Bool_t /*bMuch*/, Bool_t bTof, Bool_t bPsd )
{
  Int_t nrDigis=fDigiMan->GetNofDigis(Digi::GetSystem());
  Int_t iFirstDigiInWin = iStartDigi;

  for (Int_t i = iStartDigi; i < nrDigis; ++i) {

    const Digi* digi = fDigiMan->Get<Digi>(i);

    /// Ignore pulser events in TOF
    if( kTRUE == bTof )
      if( fuMinTotPulserTof < digi->GetCharge() && digi->GetCharge() < fuMaxTotPulserTof )
        continue;
/*
    /// Keep only hits with higher probability to be signal
    if( kTRUE == bMuch )
      if( digi->GetCharge() < 2 )
        continue;

    /// Ignore even STS channels as they have ~10-100 x more noise
    if( kTRUE == bSts )
      if( ( digi->GetCharge() < 7 || 31 == digi->GetCharge() ) ||
          ( 0 == ( digi->GetChannel() % 2 ) ) )
        continue;
*/

    /// Take selected PSD channel
    if( kTRUE == bPsd )
      if( digi->GetAddress() != ( ( 1 << 10 ) + 8 ) )
         continue;

    Double_t diffTime = T0Time - digi->GetTime();

    if (diffTime > offsetRange) {
      ++ iFirstDigiInWin; // Update Index of first digi in Win to next digi
      continue; // not yet in interesting range
    } // if (diffTime > offsetRange)
    if (diffTime < -offsetRange) break;     // already past interesting range
    histo->Fill(diffTime);
  }

  return iFirstDigiInWin;
}

void CbmCheckTiming::CheckTimeOrder()
{
  if (fT0DigiVec || fT0DigiArr) {
    Int_t nrT0Digis = 0;
    if ( fT0DigiVec ) nrT0Digis = fT0DigiVec->size();
    else if ( fT0DigiArr ) nrT0Digis = fT0DigiArr->GetEntriesFast();
    fNrOfT0Digis += nrT0Digis;
    fNrOfT0Errors += CheckIfSortedT0(fT0T0Diff, fPrevTimeT0, "T0");
  }
  if (fDigiMan->IsPresent(ECbmModuleId::kSts)) {
    Int_t nrStsDigis = fDigiMan->GetNofDigis(ECbmModuleId::kSts);
    fNrOfStsDigis += nrStsDigis;
    fNrOfStsErrors += CheckIfSorted<CbmStsDigi>(fStsStsDiff, fPrevTimeSts, "Sts");

    for (Int_t i = 0; i < nrStsDigis; ++i) {
      const CbmStsDigi* Digi = fDigiMan->Get<CbmStsDigi>(i);
      UInt_t uDPB = ( 0 < (Digi->GetAddress() & 0x00000400 ) );
      if( uDPB < kuMaxNbStsDpbs )
        fStsDpbCntsEvo[ uDPB ]->Fill( fNrTs );
    }
  }
  if (fDigiMan->IsPresent(ECbmModuleId::kMuch)) {
    Int_t nrMuchDigis = fDigiMan->GetNofDigis(ECbmModuleId::kMuch);
    fNrOfMuchDigis += nrMuchDigis;
    fNrOfMuchErrors += CheckIfSorted<CbmMuchBeamTimeDigi>(fMuchMuchDiff, fPrevTimeMuch, "Much");
  }
  if (fDigiMan->IsPresent(ECbmModuleId::kTof)) {
    Int_t nrTofDigis = fDigiMan->GetNofDigis(ECbmModuleId::kTof);
    fNrOfTofDigis += nrTofDigis;
    fNrOfTofErrors += CheckIfSorted<CbmTofDigi>(fTofTofDiff, fPrevTimeTof, "Tof");
  }
  if (fDigiMan->IsPresent(ECbmModuleId::kRich)) {
    Int_t nrRichDigis = fDigiMan->GetNofDigis(ECbmModuleId::kRich);
    fNrOfRichDigis += nrRichDigis;
    fNrOfRichErrors += CheckIfSorted<CbmRichDigi>(fRichRichDiff, fPrevTimeRich, "Rich");
  }
  if (fDigiMan->IsPresent(ECbmModuleId::kPsd)) {
    Int_t nrPsdDigis = fDigiMan->GetNofDigis(ECbmModuleId::kPsd);
    fNrOfPsdDigis += nrPsdDigis;
    fNrOfPsdErrors += CheckIfSorted<CbmPsdDigi>(fPsdPsdDiff, fPrevTimePsd, "Psd");
  }

}

template <class Digi> Int_t CbmCheckTiming::CheckIfSorted(TH1* histo, Double_t& prevTime, TString detector)
{
  Int_t nrOfErrors=0;
  Int_t nrDigis=fDigiMan->GetNofDigis(Digi::GetSystem());

  for (Int_t i = 0; i < nrDigis; ++i) {

     const Digi* digi = fDigiMan->Get<Digi>(i);

     Double_t diffTime = digi->GetTime() - prevTime;
     histo->Fill(diffTime);

     if (diffTime < 0.) {
       LOG(info)  << fixed << setprecision(15) << diffTime << "ns";
       LOG(info) << "Previous " << detector << " digi (" << fixed << setprecision(15)
                  << prevTime * 1.e-9
                  << ") has a larger time than the current one ("
                  << digi->GetTime() * 1.e-9 << ") for digi "
                  << i << " of ts " << fNrTs;
       nrOfErrors++;
     }

     prevTime = digi->GetTime();

  }

  return nrOfErrors;
}

Int_t CbmCheckTiming::CheckIfSortedT0(TH1* histo, Double_t& prevTime, TString detector) {

  // Extra implementation since T0 is not included in CbmDigiManager
  Int_t nrOfErrors=0;

  Int_t nrDigis = 0;
  if ( fT0DigiVec ) nrDigis = fT0DigiVec->size();
  else if ( fT0DigiArr ) nrDigis = fT0DigiArr->GetEntriesFast();

  for (Int_t i = 0; i < nrDigis; ++i) {

    const CbmTofDigi* digi = nullptr;
    if ( fT0DigiVec ) digi = &(fT0DigiVec->at(i));
    else if ( fT0DigiArr ) digi = dynamic_cast<const CbmTofDigi*>(fT0DigiArr->At(i));
    if ( ! digi ) {
      nrOfErrors++;
      continue;
    }

     Double_t diffTime = digi->GetTime() - prevTime;
     histo->Fill(diffTime);

     if (diffTime < 0.) {
       LOG(info)  << fixed << setprecision(15) << diffTime << "ns";
       LOG(info) << "Previous " << detector << " digi (" << fixed << setprecision(15)
                  << prevTime * 1.e-9
                  << ") has a larger time than the current one ("
                  << digi->GetTime() * 1.e-9 << ") for digi "
                  << i << " of ts " << fNrTs;
       nrOfErrors++;
     }

     prevTime = digi->GetTime();

  }

  return nrOfErrors;
}



// ---- Finish --------------------------------------------------------
void CbmCheckTiming::Finish()
{
  if (fCheckTimeOrdering) {
    LOG(info) << "Total number of T0 out of order digis: " << fNrOfT0Errors;
    LOG(info) << "Total number of T0 digis: " << fNrOfT0Digis;
    LOG(info) << "Total number of Sts out of order digis: " << fNrOfStsErrors;
    LOG(info) << "Total number of Sts digis: " << fNrOfStsDigis;
    LOG(info) << "Total number of Much out of order digis: " << fNrOfMuchErrors;
    LOG(info) << "Total number of Much digis: " << fNrOfMuchDigis;
    LOG(info) << "Total number of Tof out of order digis: " << fNrOfTofErrors;
    LOG(info) << "Total number of Tof digis: " << fNrOfTofDigis;
    LOG(info) << "Total number of Rich out of order digis: " << fNrOfRichErrors;
    LOG(info) << "Total number of Rich digis: " << fNrOfRichDigis;
    LOG(info) << "Total number of Psd out of order digis: " << fNrOfPsdErrors;
    LOG(info) << "Total number of Psd digis: " << fNrOfPsdDigis;
  }
  WriteHistos();

  LOG(info) << Form( "Checked %6d Timeslices", fNrTs);
}

void CbmCheckTiming::WriteHistos()
{
  TFile* old = gFile;
  TFile* outfile = TFile::Open(fOutFileName,"RECREATE");

  fT0StsDiff->Write();
  fT0MuchDiff->Write();
  fT0TofDiff->Write();
  fT0RichDiff->Write();
  fT0PsdDiff->Write();

  fT0StsDiffCharge->Write();
  fT0MuchDiffCharge->Write();
  fT0TofDiffCharge->Write();
  fT0RichDiffCharge->Write();
  fT0PsdDiffCharge->Write();

  fT0StsDiffEvo->Write();
  fT0MuchDiffEvo->Write();
  fT0TofDiffEvo->Write();
  fT0RichDiffEvo->Write();
  fT0PsdDiffEvo->Write();

  fT0StsDiffEvoLong->Write();
  fT0MuchDiffEvoLong->Write();
  fT0TofDiffEvoLong->Write();
  fT0RichDiffEvoLong->Write();
  fT0PsdDiffEvoLong->Write();

  fT0T0Diff->Write();
  fStsStsDiff->Write();
  fMuchMuchDiff->Write();
  fTofTofDiff->Write();
  fRichRichDiff->Write();
  fPsdPsdDiff->Write();

  fT0Address->Write();
  fT0Channel->Write();

  fT0StsDpbDiff->Write();
  for( UInt_t uStsDpb = 0; uStsDpb < kuMaxNbStsDpbs; ++uStsDpb )
  {
    fT0StsDpbDiffEvo[uStsDpb]->Write();
    fStsDpbCntsEvo[uStsDpb]->Write();
  }

  fT0MuchRocDiff->Write();
  fT0MuchAsicDiff->Write();
  for( UInt_t uMuchAsic = 0; uMuchAsic < kuMaxNbMuchAsics; ++uMuchAsic )
    fT0MuchAsicDiffEvo[uMuchAsic]->Write();

  fSelT0StsDiff->Write();
  fSelT0MuchDiff->Write();
  fSelT0TofDiff->Write();
  fSelT0RichDiff->Write();
  fSelT0PsdDiff->Write();

  outfile->Close();
  delete outfile;

  gFile = old;
}

ClassImp(CbmCheckTiming)
