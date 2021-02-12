/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "CbmMcbm2019CheckDtInDet.h"

#include "CbmDigiManager.h"
#include "CbmFlesHistosTools.h"
#include "CbmMuchBeamTimeDigi.h"
#include "CbmPsdDigi.h"
#include "CbmRichDigi.h"
#include "CbmStsDigi.h"
#include "CbmTofDigi.h"
#include "CbmTrdDigi.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunOnline.h"

#include "TClonesArray.h"
#include "TH1.h"
#include "TH2.h"
#include "THttpServer.h"
#include "TProfile.h"
#include <TDirectory.h>
#include <TFile.h>

#include <iomanip>
#include <iostream>
#include <type_traits>
using std::fixed;
using std::setprecision;

// ---- Default constructor -------------------------------------------
CbmMcbm2019CheckDtInDet::CbmMcbm2019CheckDtInDet()
  : FairTask("CbmMcbm2019CheckDtInDet") {}

// ---- Destructor ----------------------------------------------------
CbmMcbm2019CheckDtInDet::~CbmMcbm2019CheckDtInDet() {}

// ----  Initialisation  ----------------------------------------------
void CbmMcbm2019CheckDtInDet::SetParContainers() {
  // Load all necessary parameter containers from the runtime data base
  /*
  FairRunAna* ana = FairRunAna::Instance();
  FairRuntimeDb* rtdb=ana->GetRuntimeDb();

  <CbmMcbm2019CheckDtInDetDataMember> = (<ClassPointer>*)
    (rtdb->getContainer("<ContainerName>"));
  */
}

// ---- Init ----------------------------------------------------------
InitStatus CbmMcbm2019CheckDtInDet::Init() {

  // Get a handle from the IO manager
  FairRootManager* ioman = FairRootManager::Instance();

  // Digi manager
  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->UseMuchBeamTimeDigi();
  fDigiMan->Init();

  // T0 is not included in DigiManager; have to take care here
  // Try to find a vector branch for the digi
  fT0DigiVector = ioman->InitObjectAs<std::vector<CbmTofDigi> const*>("T0Digi");
  if (!fT0DigiVector) {
    LOG(info) << "No T0 digi vector found; trying TClonesArray";
    if (std::is_convertible<TObject*, CbmTofDigi*>::value) {
      fT0DigiArray = dynamic_cast<TClonesArray*>(ioman->GetObject("T0Digi"));
      if (!fT0DigiArray) LOG(info) << "No T0 digi input found.";
    }  //? CbmTofDigi derives from TObject
  }    //? No vector for T0 digis

  if (!fDigiMan->IsPresent(ECbmModuleId::kSts)) {
    LOG(info) << "No STS digi input found.";
  }

  if (!fDigiMan->IsPresent(ECbmModuleId::kMuch)) {
    LOG(info) << "No MUCH digi input found.";
  }

  if (!fDigiMan->IsPresent(ECbmModuleId::kTrd)) {
    LOG(info) << "No TRD digi input found.";
  }  // if ( ! fDigiMan->IsPresent( ECbmModuleId::kTrd) )
  else {
    /// The TRD digi time is relative to the TS start, so we need the metadata to offset it
    fTimeSliceMetaDataArray =
      dynamic_cast<TClonesArray*>(ioman->GetObject("TimesliceMetaData"));
    if (!fTimeSliceMetaDataArray)
      LOG(fatal) << "No TS metadata input found while TRD needs it.";
  }  // else of if ( ! fDigiMan->IsPresent( ECbmModuleId::kTrd) )

  if (!fDigiMan->IsPresent(ECbmModuleId::kTof)) {
    LOG(info) << "No TOF digi input found.";
  }

  if (!fDigiMan->IsPresent(ECbmModuleId::kRich)) {
    LOG(info) << "No RICH digi input found.";
  }

  if (!fDigiMan->IsPresent(ECbmModuleId::kPsd)) {
    LOG(info) << "No PSD digi input found.";
  }

  CreateHistos();

  return kSUCCESS;
}

void CbmMcbm2019CheckDtInDet::CreateHistos() {
  /// Logarithmic bining for self time comparison
  uint32_t iNbBinsLog = 0;
  /// Parameters are NbDecadesLog, NbStepsDecade, NbSubStepsInStep
  std::vector<double> dBinsLogVector = GenerateLogBinArray(9, 9, 1, iNbBinsLog);
  double* dBinsLog                   = dBinsLogVector.data();
  //  double * dBinsLog = GenerateLogBinArray( 9, 9, 1, iNbBinsLog );

  /// Proportion of hits with same time
  // T0 vs. T0
  fT0T0SameTime = new TH1F(
    "fT0T0SameTime", "Fract. same time T0;Same Time? [];Counts", 2, -0.5, 1.5);
  // sts vs. Sts
  fStsStsSameTime = new TH1F("fStsStsSameTime",
                             "Fract. same time Sts;Same Time? [];Counts",
                             2,
                             -0.5,
                             1.5);
  // Much vs. Much
  fMuchMuchSameTime = new TH1F("fMuchMuchSameTime",
                               "Fract. same time Much;Same Time? [];Counts",
                               2,
                               -0.5,
                               1.5);
  // Trd vs. Trd
  fTrdTrdSameTime = new TH1F("fTrdTrdSameTime",
                             "Fract. same time Trd;Same Time? [];Counts",
                             2,
                             -0.5,
                             1.5);
  // Tof vs. Tof
  fTofTofSameTime = new TH1F("fTofTofSameTime",
                             "Fract. same time Tof;Same Time? [];Counts",
                             2,
                             -0.5,
                             1.5);
  // Rich vs. Rich
  fRichRichSameTime = new TH1F("fRichRichSameTime",
                               "Fract. same time Rich;Same Time? [];Counts",
                               2,
                               -0.5,
                               1.5);
  // Psd vs. Psd
  fPsdPsdSameTime = new TH1F("fPsdPsdSameTime",
                             "Fract. same time Psd;Same Time? [];Counts",
                             2,
                             -0.5,
                             1.5);

  /// Per detector
  // T0 vs. T0
  fT0T0Diff = new TH1F(
    "fT0T0Diff", "T0-T0_prev;time diff [ns];Counts", 10001, -0.5, 10000.5);
  // sts vs. Sts
  fStsStsDiff = new TH1F(
    "fStsStsDiff", "Sts-Sts_prev;time diff [ns];Counts", 10001, -0.5, 10000.5);
  // Much vs. Much
  fMuchMuchDiff = new TH1F("fMuchMuchDiff",
                           "Much-Much_prev;time diff [ns];Counts",
                           10001,
                           -0.5,
                           10000.5);
  // Trd vs. Trd
  fTrdTrdDiff = new TH1F(
    "fTrdTrdDiff", "Trd-Trd_prev;time diff [ns];Counts", 10001, -0.5, 10000.5);
  // Tof vs. Tof
  fTofTofDiff = new TH1F(
    "fTofTofDiff", "Tof-Tof_prev;time diff [ns];Counts", 10001, -0.5, 10000.5);
  // Rich vs. Rich
  fRichRichDiff = new TH1F("fRichRichDiff",
                           "Rich-Rich_prev;time diff [ns];Counts",
                           10001,
                           -0.5,
                           10000.5);
  // Psd vs. Psd
  fPsdPsdDiff = new TH1F(
    "fPsdPsdDiff", "Psd-Psd_prev;time diff [ns];Counts", 10001, -0.5, 10000.5);
  // T0 vs. T0
  fT0T0DiffLog = new TH1F(
    "fT0T0DiffLog", "T0-T0_prev;time diff [ns];Counts", iNbBinsLog, dBinsLog);
  // sts vs. Sts
  fStsStsDiffLog = new TH1F("fStsStsDiffLog",
                            "Sts-Sts_prev;time diff [ns];Counts",
                            iNbBinsLog,
                            dBinsLog);
  // Much vs. Much
  fMuchMuchDiffLog = new TH1F("fMuchMuchDiffLog",
                              "Much-Much_prev;time diff [ns];Counts",
                              iNbBinsLog,
                              dBinsLog);
  // Trd vs. Trd
  fTrdTrdDiffLog = new TH1F("fTrdTrdDiffLog",
                            "Trd-Trd_prev;time diff [ns];Counts",
                            iNbBinsLog,
                            dBinsLog);
  // Tof vs. Tof
  fTofTofDiffLog = new TH1F("fTofTofDiffLog",
                            "Tof-Tof_prev;time diff [ns];Counts",
                            iNbBinsLog,
                            dBinsLog);
  // Rich vs. Rich
  fRichRichDiffLog = new TH1F("fRichRichDiffLog",
                              "Rich-Rich_prev;time diff [ns];Counts",
                              iNbBinsLog,
                              dBinsLog);
  // Psd vs. Psd
  fPsdPsdDiffLog = new TH1F("fPsdPsdDiffLog",
                            "Psd-Psd_prev;time diff [ns];Counts",
                            iNbBinsLog,
                            dBinsLog);

  /// Per channel
  // T0 vs. T0
  fT0T0DiffPerChan =
    new TH2F("fT0T0DiffPerChan",
             "T0-T0_prev Per Channel;time diff [ns]; Channel [];Counts",
             iNbBinsLog,
             dBinsLog,
             fuNbChanT0,
             0,
             fuNbChanT0);
  // sts vs. Sts
  fStsStsDiffPerChan =
    new TH2F("fStsStsDiffPerChan",
             "Sts-Sts_prev Per Channel;time diff [ns]; Channel [];Counts",
             iNbBinsLog,
             dBinsLog,
             fuNbChanSts,
             0,
             fuNbChanSts);
  // Much vs. Much
  fMuchMuchDiffPerChan =
    new TH2F("fMuchMuchDiffPerChan",
             "Much-Much_prev Per Channel;time diff [ns]; Channel [];Counts",
             iNbBinsLog,
             dBinsLog,
             fuNbChanMuch,
             0,
             fuNbChanMuch);
  // Trd vs. Trd
  fTrdTrdDiffPerChan =
    new TH2F("fTrdTrdDiffPerChan",
             "Trd-Trd_prev Per Channel;time diff [ns]; Channel [];Counts",
             iNbBinsLog,
             dBinsLog,
             fuNbChanTrd,
             0,
             fuNbChanTrd);
  // Tof vs. Tof
  fTofTofDiffPerChan =
    new TH2F("fTofTofDiffPerChan",
             "Tof-Tof_prev Per Channel;time diff [ns]; Channel [];Counts",
             iNbBinsLog,
             dBinsLog,
             fuNbChanTof,
             0,
             fuNbChanTof);
  // Rich vs. Rich
  fRichRichDiffPerChan =
    new TH2F("fRichRichDiffPerChan",
             "Rich-Rich_prev Per Channel;time diff [ns]; Channel [];Counts",
             iNbBinsLog,
             dBinsLog,
             fuNbChanRich,
             0,
             fuNbChanRich);
  // Psd vs. Psd
  fPsdPsdDiffPerChan =
    new TH2F("fPsdPsdDiffPerChan",
             "Psd-Psd_prev Per Channel;time diff [ns]; Channel [];Counts",
             iNbBinsLog,
             dBinsLog,
             fuNbChanPsd,
             0,
             fuNbChanPsd);

  /// Register the histos in the HTTP server
  FairRunOnline* run = FairRunOnline::Instance();
  if (run) {
    THttpServer* server = run->GetHttpServer();
    if (nullptr != server) {

      server->Register("/Dt", fT0T0Diff);
      server->Register("/Dt", fStsStsDiff);
      server->Register("/Dt", fMuchMuchDiff);
      server->Register("/Dt", fTrdTrdDiff);
      server->Register("/Dt", fTofTofDiff);
      server->Register("/Dt", fRichRichDiff);
      server->Register("/Dt", fPsdPsdDiff);

      server->Register("/Dt", fT0T0DiffLog);
      server->Register("/Dt", fStsStsDiffLog);
      server->Register("/Dt", fMuchMuchDiffLog);
      server->Register("/Dt", fTrdTrdDiffLog);
      server->Register("/Dt", fTofTofDiffLog);
      server->Register("/Dt", fRichRichDiffLog);
      server->Register("/Dt", fPsdPsdDiffLog);

      server->Register("/DtPerChan", fT0T0DiffPerChan);
      server->Register("/DtPerChan", fStsStsDiffPerChan);
      server->Register("/DtPerChan", fMuchMuchDiffPerChan);
      server->Register("/DtPerChan", fTrdTrdDiffPerChan);
      server->Register("/DtPerChan", fTofTofDiffPerChan);
      server->Register("/DtPerChan", fRichRichDiffPerChan);
      server->Register("/DtPerChan", fPsdPsdDiffPerChan);
    }
  }
}
// ---- ReInit  -------------------------------------------------------
InitStatus CbmMcbm2019CheckDtInDet::ReInit() { return kSUCCESS; }

// ---- Exec ----------------------------------------------------------
void CbmMcbm2019CheckDtInDet::Exec(Option_t* /*option*/) {
  LOG(debug) << "executing TS " << fNrTs;

  if (0 < fNrTs && 0 == fNrTs % 1000)
    LOG(info) << Form("Processing TS %6d", fNrTs);

  /// Get nb entries per detector
  LOG(debug) << "Begin";
  Int_t nrT0Digis = 0;
  if (fT0DigiVector)
    nrT0Digis = fT0DigiVector->size();
  else if (fT0DigiArray)
    nrT0Digis = fT0DigiArray->GetEntriesFast();
  LOG(debug) << "T0Digis: " << nrT0Digis;

  /*
  Int_t nrStsDigis  = fDigiMan->GetNofDigis( ECbmModuleId::kSts);
  Int_t nrMuchDigis = fDigiMan->GetNofDigis( ECbmModuleId::kMuch);
  Int_t nrTrdDigis  = fDigiMan->GetNofDigis( ECbmModuleId::kTrd);
  Int_t nrTofDigis  = fDigiMan->GetNofDigis( ECbmModuleId::kTof);
  Int_t nrRichDigis = fDigiMan->GetNofDigis( ECbmModuleId::kRich);
  Int_t nrPsdDigis  = fDigiMan->GetNofDigis( ECbmModuleId::kPsd);
*/

  /// Check dT in T0
  for (Int_t iT0 = 0; iT0 < nrT0Digis; ++iT0) {

    if (iT0 % 1000 == 0) LOG(debug) << "Executing entry " << iT0;

    const CbmTofDigi* T0Digi = nullptr;
    if (fT0DigiVector)
      T0Digi = &(fT0DigiVector->at(iT0));
    else if (fT0DigiArray)
      T0Digi = dynamic_cast<CbmTofDigi*>(fT0DigiArray->At(iT0));
    assert(T0Digi);

    Double_t T0Time = T0Digi->GetTime();
    //    Int_t T0Address = T0Digi->GetAddress();

    Double_t T0TimeDiff = T0Time - fPrevTimeT0;

    if (0 < iT0) {
      fT0T0Diff->Fill(T0TimeDiff);
      if (0 < T0TimeDiff) {
        fT0T0SameTime->Fill(0);
        fT0T0DiffLog->Fill(T0TimeDiff);
      }  // if( 0 < T0TimeDiff)
      else
        fT0T0SameTime->Fill(1);
    }  // if( 0 < iT0 )

    fPrevTimeT0 = T0Time;
  }  // for( Int_t iT0 = 0; iT0 < nrT0Digis; ++iT0 )

  /// Check dT in the other channels
  FillHistosPerDet<CbmStsDigi>(fStsStsSameTime,
                               fStsStsDiff,
                               fStsStsDiffLog,
                               fStsStsDiffPerChan,
                               ECbmModuleId::kSts);
  FillHistosPerDet<CbmMuchBeamTimeDigi>(fMuchMuchSameTime,
                                        fMuchMuchDiff,
                                        fMuchMuchDiffLog,
                                        fMuchMuchDiffPerChan,
                                        ECbmModuleId::kMuch);
  FillHistosPerDet<CbmTrdDigi>(fTrdTrdSameTime,
                               fTrdTrdDiff,
                               fTrdTrdDiffLog,
                               fTrdTrdDiffPerChan,
                               ECbmModuleId::kTrd);
  FillHistosPerDet<CbmTofDigi>(fTofTofSameTime,
                               fTofTofDiff,
                               fTofTofDiffLog,
                               fTofTofDiffPerChan,
                               ECbmModuleId::kTof);
  FillHistosPerDet<CbmRichDigi>(fRichRichSameTime,
                                fRichRichDiff,
                                fRichRichDiffLog,
                                fRichRichDiffPerChan,
                                ECbmModuleId::kRich);
  FillHistosPerDet<CbmPsdDigi>(fPsdPsdSameTime,
                               fPsdPsdDiff,
                               fPsdPsdDiffLog,
                               fPsdPsdDiffPerChan,
                               ECbmModuleId::kPsd);

  fNrTs++;

  if (0 < fNrTs && 0 == fNrTs % 90000) WriteHistos();
}


template<class Digi>
void CbmMcbm2019CheckDtInDet::FillHistosPerDet(TH1* histoSameTime,
                                               TH1* histoDt,
                                               TH1* histoDtLog,
                                               TH2* /*histoDtPerChan*/,
                                               ECbmModuleId iDetId) {
  UInt_t uNrDigis = fDigiMan->GetNofDigis(iDetId);

  Double_t dPrevTime = -1;

  for (UInt_t uDigiIdx = 0; uDigiIdx < uNrDigis; ++uDigiIdx) {
    const Digi* digi = fDigiMan->Get<Digi>(uDigiIdx);

    Double_t dNewDigiTime = digi->GetTime();

    if (0 < uDigiIdx) {
      Double_t dTimeDiff = dNewDigiTime - dPrevTime;

      histoDt->Fill(dTimeDiff);
      if (0 < dTimeDiff) {
        histoSameTime->Fill(0);
        histoDtLog->Fill(dTimeDiff);
      }  // if( 0 < dTimeDiff )
      else
        histoSameTime->Fill(1);
    }  //
       /*
    /// Fill histos per Channel
    switch( iDetId )
    {
      case kSts: ///< Silicon Tracking System
      {
        const CbmStsDigi* stsDigi;
        try {
          stsDigi =
              boost::any_cast<const CbmStsDigi*>( digi );
        } catch( ... ) {
            LOG( fatal ) << "Failed boost any_cast in CbmMcbm2019CheckPulser::FillSystemOffsetHistos for a digi of type "
                         << Digi::GetClassName();
        } // try/catch
        assert(stsDigi);
        UInt_t uAddr = stsDigi->GetAddress();
        UInt_t uChan = stsDigi->GetChannel();

        break;
      } // case kSts:
      case kMuch:        ///< Muon detection system
      {
        const CbmMuchBeamTimeDigi* muchDigi;
        try {
          muchDigi =
              boost::any_cast<const CbmMuchBeamTimeDigi*>( digi );
        } catch( ... ) {
            LOG( fatal ) << "Failed boost any_cast in CbmMcbm2019CheckPulser::FillSystemOffsetHistos for a digi of type "
                         << Digi::GetClassName();
        } // try/catch
        assert(muchDigi);
        UInt_t uAsic = muchDigi->GetNxId();
        UInt_t uChan = muchDigi->GetNxCh();

        break;
      } // case kMuch:
      case kTrd:         ///< Time-of-flight Detector
      {
        UInt_t uAddr = digi->GetAddress();
        break;
      } // case kTrd:
      case kTof:         ///< Time-of-flight Detector
      {

        break;
      } // case kTof:
      case kRich:        ///< Ring-Imaging Cherenkov Detector
      {

        break;
      } // case kRich:
      case kPsd:         ///< Projectile spectator detector
      {
        UInt_t uAddr = digi->GetAddress();

        break;
      } // case kPsd:
      default:
        return 0;
    } // switch( iDetId )
*/
    dPrevTime = dNewDigiTime;
  }  // for( UInt_t uDigiIdx = 0; uDigiIdx < uNrDigis; ++uDigiIdx )
}

// ---- Finish --------------------------------------------------------
void CbmMcbm2019CheckDtInDet::Finish() { WriteHistos(); }

void CbmMcbm2019CheckDtInDet::WriteHistos() {
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;


  TFile* outfile = TFile::Open(fOutFileName, "RECREATE");

  fT0T0SameTime->Write();
  fStsStsSameTime->Write();
  fMuchMuchSameTime->Write();
  fTrdTrdSameTime->Write();
  fTofTofSameTime->Write();
  fRichRichSameTime->Write();
  fPsdPsdSameTime->Write();

  fT0T0Diff->Write();
  fStsStsDiff->Write();
  fMuchMuchDiff->Write();
  fTrdTrdDiff->Write();
  fTofTofDiff->Write();
  fRichRichDiff->Write();
  fPsdPsdDiff->Write();

  fT0T0DiffLog->Write();
  fStsStsDiffLog->Write();
  fMuchMuchDiffLog->Write();
  fTrdTrdDiffLog->Write();
  fTofTofDiffLog->Write();
  fRichRichDiffLog->Write();
  fPsdPsdDiffLog->Write();

  fT0T0DiffPerChan->Write();
  fStsStsDiffPerChan->Write();
  fMuchMuchDiffPerChan->Write();
  fTrdTrdDiffPerChan->Write();
  fTofTofDiffPerChan->Write();
  fRichRichDiffPerChan->Write();
  fPsdPsdDiffPerChan->Write();

  outfile->Close();
  delete outfile;

  gFile      = oldFile;
  gDirectory = oldDir;
}

ClassImp(CbmMcbm2019CheckDtInDet)
