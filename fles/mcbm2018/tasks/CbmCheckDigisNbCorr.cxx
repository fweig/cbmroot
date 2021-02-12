/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "CbmCheckDigisNbCorr.h"

#include "CbmDigiManager.h"
#include "CbmMuchBeamTimeDigi.h"
#include "TimesliceMetaData.h"

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

#include "CbmMuchDigi.h"
#include "CbmRichDigi.h"
#include "CbmStsDigi.h"
#include "CbmTofDigi.h"

#include <iomanip>
using std::fixed;
using std::setprecision;

// ---- Default constructor -------------------------------------------
CbmCheckDigisNbCorr::CbmCheckDigisNbCorr()
  : FairTask("CbmCheckDigisNbCorr")
  , fuMinTotPulserT0(90)
  , fuMaxTotPulserT0(100)

{}

// ---- Destructor ----------------------------------------------------
CbmCheckDigisNbCorr::~CbmCheckDigisNbCorr() {}

// ----  Initialisation  ----------------------------------------------
void CbmCheckDigisNbCorr::SetParContainers() {
  // Load all necessary parameter containers from the runtime data base
  /*
  FairRunAna* ana = FairRunAna::Instance();
  FairRuntimeDb* rtdb=ana->GetRuntimeDb();

  <CbmCheckDigisNbCorrDataMember> = (<ClassPointer>*)
    (rtdb->getContainer("<ContainerName>"));
  */
}

// ---- Init ----------------------------------------------------------
InitStatus CbmCheckDigisNbCorr::Init() {

  // Get a handle from the IO manager
  FairRootManager* ioman = FairRootManager::Instance();

  // Get a pointer to the previous already existing data level
  fTsMetaData =
    static_cast<TClonesArray*>(ioman->GetObject("TimesliceMetaData"));
  if (!fTsMetaData) { LOG(info) << "No TClonesArray with TS meta data found."; }
  // DigiManager
  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->Init();

  // Get a pointer to the previous already existing data level
  fT0DigiVec = ioman->InitObjectAs<std::vector<CbmTofDigi> const*>("T0Digi");
  if (!fT0DigiVec) {
    fT0DigiArr = dynamic_cast<TClonesArray*>(ioman->GetObject("T0Digi"));
    if (!fT0DigiArr) { LOG(fatal) << "No TClonesArray with T0 digis found."; }
  }

  if (!fDigiMan->IsPresent(ECbmModuleId::kSts)) {
    LOG(info) << "No TClonesArray with STS digis found.";
  }

  if (!fDigiMan->IsPresent(ECbmModuleId::kMuch)) {
    LOG(info) << "No TClonesArray with MUCH digis found.";
  }

  if (!fDigiMan->IsPresent(ECbmModuleId::kTof)) {
    LOG(info) << "No TClonesArray with TOF digis found.";
  }

  if (!fDigiMan->IsPresent(ECbmModuleId::kRich)) {
    LOG(info) << "No TClonesArray with RICH digis found.";
  }

  CreateHistos();

  return kSUCCESS;
}

void CbmCheckDigisNbCorr::CalcNrBins() {
  fiBinNb = fdTsLengthNs / fdBinWidthNs;
}

void CbmCheckDigisNbCorr::CreateHistos() {
  /// Resize storage array
  CalcNrBins();
  fvuNbDigisPerBinT0.resize(fiBinNb, 0);
  fvuNbDigisPerBinSts.resize(fiBinNb, 0);
  fvuNbDigisPerBinMuch.resize(fiBinNb, 0);
  fvuNbDigisPerBinTof.resize(fiBinNb, 0);
  fvuNbDigisPerBinRich.resize(fiBinNb, 0);

  fvuNbDigisPerBinStsDpb.resize(kuMaxNbStsDpbs);
  for (UInt_t uStsDpb = 0; uStsDpb < kuMaxNbStsDpbs; ++uStsDpb)
    fvuNbDigisPerBinStsDpb[uStsDpb].resize(fiBinNb, 0);

  /// 2D correlations between systems
  // T0 vs. TST
  fT0StsCorr =
    new TH2F("fT0StsCorr",
             Form("T0 - STS digis Nb correlation per %.0f ns time interval; Nb "
                  "T0 Digis []; Nb STS Digis []; Counts",
                  fdBinWidthNs),
             1000,
             0,
             1000,
             1000,
             0,
             1000);
  // T0 vs. MUCH
  fT0MuchCorr =
    new TH2F("fT0MuchCorr",
             Form("T0 - MUCH digis Nb correlation per %.0f ns time interval; "
                  "Nb T0 Digis []; Nb MUCH Digis []; Counts",
                  fdBinWidthNs),
             1000,
             0,
             1000,
             1000,
             0,
             1000);
  // T0 vs. TOF
  fT0TofCorr =
    new TH2F("fT0TofCorr",
             Form("T0 - TOF digis Nb correlation per %.0f ns time interval; Nb "
                  "T0 Digis []; Nb TOF Digis []; Counts",
                  fdBinWidthNs),
             1000,
             0,
             1000,
             1000,
             0,
             1000);
  // T0 vs. RICH
  fT0RichCorr =
    new TH2F("fT0RichCorr",
             Form("T0 - RICH digis Nb correlation per %.0f ns time interval; "
                  "Nb T0 Digis []; Nb RICH Digis []; Counts",
                  fdBinWidthNs),
             1000,
             0,
             1000,
             1000,
             0,
             1000);

  // STS vs. MUCH
  fStsMuchCorr =
    new TH2F("fStsMuchCorr",
             Form("STS - MUCH digis Nb correlation per %.0f ns time interval; "
                  "Nb STS Digis []; Nb STS Digis []; Counts",
                  fdBinWidthNs),
             1000,
             0,
             1000,
             1000,
             0,
             1000);
  // STS vs. TOF
  fStsTofCorr =
    new TH2F("fStsTofCorr",
             Form("STS - TOF digis Nb correlation per %.0f ns time interval; "
                  "Nb STS Digis []; Nb TOF Digis []; Counts",
                  fdBinWidthNs),
             1000,
             0,
             1000,
             1000,
             0,
             1000);
  // STS vs. RICH
  fStsRichCorr =
    new TH2F("fStsRichCorr",
             Form("STS - RICH digis Nb correlation per %.0f ns time interval; "
                  "Nb STS Digis []; Nb RICH Digis []; Counts",
                  fdBinWidthNs),
             1000,
             0,
             1000,
             1000,
             0,
             1000);

  // MUCH vs. TOF
  fMuchTofCorr =
    new TH2F("fMuchTofCorr",
             Form("MUCH - TOF digis Nb correlation per %.0f ns time interval; "
                  "Nb MUCH Digis []; Nb TOF Digis []; Counts",
                  fdBinWidthNs),
             1000,
             0,
             1000,
             1000,
             0,
             1000);
  // MUCH vs. RICH
  fMuchRichCorr =
    new TH2F("fMuchRichCorr",
             Form("MUCH - RICH digis Nb correlation per %.0f ns time interval; "
                  "Nb MUCH Digis []; Nb RICH Digis []; Counts",
                  fdBinWidthNs),
             1000,
             0,
             1000,
             1000,
             0,
             1000);

  // TOF vs. RICH
  fTofRichCorr =
    new TH2F("fTofRichCorr",
             Form("TOF - RICH digis Nb correlation per %.0f ns time interval; "
                  "Nb TOF Digis []; Nb RICH Digis []; Counts",
                  fdBinWidthNs),
             1000,
             0,
             1000,
             1000,
             0,
             1000);

  /// Profile correlations between systems
  // T0 vs. TST
  fT0StsCorrProf =
    new TProfile("fT0StsCorrProf",
                 Form("T0 - STS digis Nb correlation per %.0f ns time "
                      "interval; Nb T0 Digis []; Nb STS Digis []",
                      fdBinWidthNs),
                 1000,
                 0,
                 1000);
  // T0 vs. MUCH
  fT0MuchCorrProf =
    new TProfile("fT0MuchCorrProf",
                 Form("T0 - MUCH digis Nb correlation per %.0f ns time "
                      "interval; Nb T0 Digis []; Nb MUCH Digis []",
                      fdBinWidthNs),
                 1000,
                 0,
                 1000);
  // T0 vs. TOF
  fT0TofCorrProf =
    new TProfile("fT0TofCorrProf",
                 Form("T0 - TOF digis Nb correlation per %.0f ns time "
                      "interval; Nb T0 Digis []; Nb TOF Digis []",
                      fdBinWidthNs),
                 1000,
                 0,
                 1000);
  // T0 vs. RICH
  fT0RichCorrProf =
    new TProfile("fT0RichCorrProf",
                 Form("T0 - RICH digis Nb correlation per %.0f ns time "
                      "interval; Nb T0 Digis []; Nb RICH Digis []",
                      fdBinWidthNs),
                 1000,
                 0,
                 1000);

  // STS vs. MUCH
  fStsMuchCorrProf =
    new TProfile("fStsMuchCorrProf",
                 Form("STS - MUCH digis Nb correlation per %.0f ns time "
                      "interval; Nb STS Digis []; Nb STS Digis []",
                      fdBinWidthNs),
                 1000,
                 0,
                 1000);
  // STS vs. TOF
  fStsTofCorrProf =
    new TProfile("fStsTofCorrProf",
                 Form("STS - TOF digis Nb correlation per %.0f ns time "
                      "interval; Nb STS Digis []; Nb TOF Digis []",
                      fdBinWidthNs),
                 1000,
                 0,
                 1000);
  // STS vs. RICH
  fStsRichCorrProf =
    new TProfile("fStsRichCorrProf",
                 Form("STS - RICH digis Nb correlation per %.0f ns time "
                      "interval; Nb STS Digis []; Nb RICH Digis []",
                      fdBinWidthNs),
                 1000,
                 0,
                 1000);

  // MUCH vs. TOF
  fMuchTofCorrProf =
    new TProfile("fMuchTofCorrProf",
                 Form("MUCH - TOF digis Nb correlation per %.0f ns time "
                      "interval; Nb MUCH Digis []; Nb TOF Digis []",
                      fdBinWidthNs),
                 1000,
                 0,
                 1000);
  // MUCH vs. RICH
  fMuchRichCorrProf =
    new TProfile("fMuchRichCorrProf",
                 Form("MUCH - RICH digis Nb correlation per %.0f ns time "
                      "interval; Nb MUCH Digis []; Nb RICH Digis []",
                      fdBinWidthNs),
                 1000,
                 0,
                 1000);

  // TOF vs. RICH
  fTofRichCorrProf =
    new TProfile("fTofRichCorrProf",
                 Form("TOF - RICH digis Nb correlation per %.0f ns time "
                      "interval; Nb TOF Digis []; Nb RICH Digis []",
                      fdBinWidthNs),
                 1000,
                 0,
                 1000);

  for (UInt_t uStsDpb = 0; uStsDpb < kuMaxNbStsDpbs; ++uStsDpb) {
    fT0StsDpbCorr[uStsDpb] =
      new TH2F(Form("fT0StsDpbCorr%02u", uStsDpb),
               Form("T0 - STS digis Nb correlation per %.0f ns time interval, "
                    "DPB %02u; Nb T0 Digis []; Nb STS Digis []; Counts",
                    fdBinWidthNs,
                    uStsDpb),
               1000,
               0,
               1000,
               1000,
               0,
               1000);
    fStsMuchDpbCorr[uStsDpb] = new TH2F(
      Form("fStsMuchDpbCorr%02u", uStsDpb),
      Form("STS - MUCH digis Nb correlation per %.0f ns time interval, DPB "
           "%02u; Nb STS Digis []; Nb STS Digis []; Counts",
           fdBinWidthNs,
           uStsDpb),
      1000,
      0,
      1000,
      1000,
      0,
      1000);
    fStsTofDpbCorr[uStsDpb] =
      new TH2F(Form("fStsTofDpbCorr%02u", uStsDpb),
               Form("STS - TOF digis Nb correlation per %.0f ns time interval, "
                    "DPB %02u; Nb STS Digis []; Nb TOF Digis []; Counts",
                    fdBinWidthNs,
                    uStsDpb),
               1000,
               0,
               1000,
               1000,
               0,
               1000);
    fStsRichDpbCorr[uStsDpb] = new TH2F(
      Form("fStsRichDpbCorr%02u", uStsDpb),
      Form("STS - RICH digis Nb correlation per %.0f ns time interval, DPB "
           "%02u; Nb STS Digis []; Nb RICH Digis []; Counts",
           fdBinWidthNs,
           uStsDpb),
      1000,
      0,
      1000,
      1000,
      0,
      1000);

    fT0StsDpbCorrProf[uStsDpb] =
      new TProfile(Form("fT0StsDpbCorrProf%02u", uStsDpb),
                   Form("T0 - STS digis Nb correlation per %.0f ns time "
                        "interval, DPB %02u; Nb T0 Digis []; Nb STS Digis []",
                        fdBinWidthNs,
                        uStsDpb),
                   1000,
                   0,
                   1000);
    fStsMuchDpbCorrProf[uStsDpb] =
      new TProfile(Form("fStsMuchDpbCorrProf%02u", uStsDpb),
                   Form("STS - MUCH digis Nb correlation per %.0f ns time "
                        "interval, DPB %02u; Nb STS Digis []; Nb STS Digis []",
                        fdBinWidthNs,
                        uStsDpb),
                   1000,
                   0,
                   1000);
    fStsTofDpbCorrProf[uStsDpb] =
      new TProfile(Form("fStsTofDpbCorrProf%02u", uStsDpb),
                   Form("STS - TOF digis Nb correlation per %.0f ns time "
                        "interval, DPB %02u; Nb STS Digis []; Nb TOF Digis []",
                        fdBinWidthNs,
                        uStsDpb),
                   1000,
                   0,
                   1000);
    fStsRichDpbCorrProf[uStsDpb] =
      new TProfile(Form("fStsRichDpbCorrProf%02u", uStsDpb),
                   Form("STS - RICH digis Nb correlation per %.0f ns time "
                        "interval, DPB %02u; Nb STS Digis []; Nb RICH Digis []",
                        fdBinWidthNs,
                        uStsDpb),
                   1000,
                   0,
                   1000);
  }  // for( UInt_t uStsDpb = 0; uStsDpb < kuMaxNbStsDpbs; ++uStsDpb )

  /// Register the histos in the HTTP server
  FairRunOnline* run = FairRunOnline::Instance();
  if (run) {
    THttpServer* server = run->GetHttpServer();
    if (nullptr != server) {
      /// 2D correlations between systems
      server->Register("CheckDigisNbCorr", fT0StsCorr);
      server->Register("CheckDigisNbCorr", fT0MuchCorr);
      server->Register("CheckDigisNbCorr", fT0TofCorr);
      server->Register("CheckDigisNbCorr", fT0RichCorr);

      server->Register("CheckDigisNbCorr", fStsMuchCorr);
      server->Register("CheckDigisNbCorr", fStsTofCorr);
      server->Register("CheckDigisNbCorr", fStsRichCorr);

      server->Register("CheckDigisNbCorr", fMuchTofCorr);
      server->Register("CheckDigisNbCorr", fMuchRichCorr);

      server->Register("CheckDigisNbCorr", fTofRichCorr);

      /// Profile correlations between systems
      server->Register("CheckDigisNbCorr", fT0StsCorrProf);
      server->Register("CheckDigisNbCorr", fT0MuchCorrProf);
      server->Register("CheckDigisNbCorr", fT0TofCorrProf);
      server->Register("CheckDigisNbCorr", fT0RichCorrProf);

      server->Register("CheckDigisNbCorr", fStsMuchCorrProf);
      server->Register("CheckDigisNbCorr", fStsTofCorrProf);
      server->Register("CheckDigisNbCorr", fStsRichCorrProf);

      server->Register("CheckDigisNbCorr", fMuchTofCorrProf);
      server->Register("CheckDigisNbCorr", fMuchRichCorrProf);

      server->Register("CheckDigisNbCorr", fTofRichCorrProf);

      for (UInt_t uStsDpb = 0; uStsDpb < kuMaxNbStsDpbs; ++uStsDpb) {
        server->Register("CheckDigisNbCorr", fT0StsDpbCorr[uStsDpb]);
        server->Register("CheckDigisNbCorr", fStsMuchDpbCorr[uStsDpb]);
        server->Register("CheckDigisNbCorr", fStsTofDpbCorr[uStsDpb]);
        server->Register("CheckDigisNbCorr", fStsRichDpbCorr[uStsDpb]);

        server->Register("CheckDigisNbCorr", fT0StsDpbCorrProf[uStsDpb]);
        server->Register("CheckDigisNbCorr", fStsMuchDpbCorrProf[uStsDpb]);
        server->Register("CheckDigisNbCorr", fStsTofDpbCorrProf[uStsDpb]);
        server->Register("CheckDigisNbCorr", fStsRichDpbCorrProf[uStsDpb]);
      }  // for( UInt_t uStsDpb = 0; uStsDpb < kuMaxNbStsDpbs; ++uStsDpb )
    }    // if( nullptr != server )
  }      // if (run)
}
// ---- ReInit  -------------------------------------------------------
InitStatus CbmCheckDigisNbCorr::ReInit() { return kSUCCESS; }

// ---- Exec ----------------------------------------------------------
void CbmCheckDigisNbCorr::Exec(Option_t* /*option*/) {
  /// Initialize the counters for each bin
  for (Int_t uBin = 0; uBin < fiBinNb; ++uBin) {
    fvuNbDigisPerBinT0[uBin]   = 0;
    fvuNbDigisPerBinSts[uBin]  = 0;
    fvuNbDigisPerBinMuch[uBin] = 0;
    fvuNbDigisPerBinTof[uBin]  = 0;
    fvuNbDigisPerBinRich[uBin] = 0;
    for (UInt_t uStsDpb = 0; uStsDpb < kuMaxNbStsDpbs; ++uStsDpb)
      fvuNbDigisPerBinStsDpb[uStsDpb][uBin] = 0;
  }  // for( UInt_t uBin = 0; uBin < fiBinNb; ++uBin )

  LOG(debug) << "executing TS " << fNrTs;
  Double_t dTsStart = fNrTs * fdTsLengthNs + 20393267200. - fdTsLengthNs;
  if (1 == fTsMetaData->GetEntriesFast())
    dTsStart =
      static_cast<TimesliceMetaData*>(fTsMetaData->At(0))->GetStartTime();

  LOG(debug) << "Begin";
  Int_t nrT0Digis = -1;
  if (fT0DigiVec)
    nrT0Digis = fT0DigiVec->size();
  else if (fT0DigiArr)
    nrT0Digis = fT0DigiArr->GetEntriesFast();
  Int_t nrStsDigis  = fDigiMan->GetNofDigis(ECbmModuleId::kSts);
  Int_t nrMuchDigis = fDigiMan->GetNofDigis(ECbmModuleId::kMuch);
  Int_t nrTofDigis  = fDigiMan->GetNofDigis(ECbmModuleId::kTof);
  Int_t nrRichDigis = fDigiMan->GetNofDigis(ECbmModuleId::kRich);

  LOG(debug) << "T0Digis: " << nrT0Digis;
  LOG(debug) << "StsDigis: " << nrStsDigis;
  LOG(debug) << "MuchDigis: " << nrMuchDigis;
  LOG(debug) << "TofDigis: " << nrTofDigis;
  LOG(debug) << "RichDigis: " << nrRichDigis;

  /// Loop on digis for each detector and counts digis in proper bin
  /// T0
  for (Int_t iDigi = 0; iDigi < nrT0Digis; ++iDigi) {
    const CbmTofDigi* pDigi = nullptr;
    if (fT0DigiVec)
      pDigi = &(fT0DigiVec->at(iDigi));
    else if (fT0DigiArr)
      pDigi = dynamic_cast<const CbmTofDigi*>(fT0DigiArr->At(iDigi));
    assert(pDigi);

    /// Ignore pulser hits in T0
    if (fuMinTotPulserT0 < pDigi->GetCharge()
        && pDigi->GetCharge() < fuMaxTotPulserT0)
      continue;

    Double_t dTime = pDigi->GetTime() - dTsStart;
    /// Jump hits with time before start of TS after offseting
    if (dTime < 0) continue;
    /// Stop on first hit with time after end of TS after offseting
    if (fdTsLengthNs <= dTime) break;

    /// Increase count in corresponding bin
    UInt_t uBin = dTime / fdBinWidthNs;
    fvuNbDigisPerBinT0[uBin]++;
  }  // for( Int_t iDigi = 0; iDigi < nrT0Digis; ++iDigi )

  /// STS
  for (Int_t iDigi = 0; iDigi < nrStsDigis; ++iDigi) {
    const CbmStsDigi* pDigi = fDigiMan->Get<CbmStsDigi>(iDigi);

    Double_t dTime = pDigi->GetTime() - dTsStart - fdStsOffset;
    /// Jump hits with time before start of TS after offseting
    if (dTime < 0) continue;
    /// Stop on first hit with time after end of TS after offseting
    if (fdTsLengthNs <= dTime) break;

    /// Increase count in corresponding bin
    UInt_t uBin = dTime / fdBinWidthNs;
    fvuNbDigisPerBinSts[uBin]++;

    UInt_t uDPB = (0 < (pDigi->GetAddress() & 0x00000400));
    fvuNbDigisPerBinStsDpb[uDPB][uBin]++;
  }  // for( Int_t iDigi = 0; iDigi < nrStsDigis; ++iDigi )

  /// MUCH
  for (Int_t iDigi = 0; iDigi < nrMuchDigis; ++iDigi) {
    const CbmMuchDigi* pDigi = fDigiMan->Get<CbmMuchDigi>(iDigi);

    Double_t dTime = pDigi->GetTime() - dTsStart - fdMuchOffset;
    /// Jump hits with time before start of TS after offseting
    if (dTime < 0) continue;
    /// Stop on first hit with time after end of TS after offseting
    if (fdTsLengthNs <= dTime) break;

    /// Increase count in corresponding bin
    UInt_t uBin = dTime / fdBinWidthNs;
    fvuNbDigisPerBinMuch[uBin]++;
  }  // for( Int_t iDigi = 0; iDigi < nrMuchDigis; ++iDigi )

  /// TOF
  for (Int_t iDigi = 0; iDigi < nrTofDigis; ++iDigi) {
    const CbmTofDigi* pDigi = fDigiMan->Get<CbmTofDigi>(iDigi);

    /// Ignore pulser hits in TOF
    if (92 < pDigi->GetCharge() && pDigi->GetCharge() < 96) continue;

    Double_t dTime = pDigi->GetTime() - dTsStart - fdTofOffset;
    /// Jump hits with time before start of TS after offseting
    if (dTime < 0) continue;
    /// Stop on first hit with time after end of TS after offseting
    if (fdTsLengthNs <= dTime) break;

    /// Increase count in corresponding bin
    UInt_t uBin = dTime / fdBinWidthNs;
    fvuNbDigisPerBinTof[uBin]++;
  }  // for( Int_t iDigi = 0; iDigi < nrTofDigis; ++iDigi )

  /// RICH
  for (Int_t iDigi = 0; iDigi < nrRichDigis; ++iDigi) {
    const CbmRichDigi* pDigi = fDigiMan->Get<CbmRichDigi>(iDigi);

    Double_t dTime = pDigi->GetTime() - dTsStart - fdRichOffset;
    /// Jump hits with time before start of TS after offseting
    if (dTime < 0) continue;
    /// Stop on first hit with time after end of TS after offseting
    if (fdTsLengthNs <= dTime) break;

    /// Increase count in corresponding bin
    UInt_t uBin = dTime / fdBinWidthNs;
    fvuNbDigisPerBinRich[uBin]++;
  }  // for( Int_t iDigi = 0; iDigi < nrRichDigis; ++iDigi )

  /// Fill the histograms for each bin
  for (Int_t uBin = 0; uBin < fiBinNb; ++uBin) {
    /// 2D & Profiles
    if (0 < fvuNbDigisPerBinT0[uBin] || 0 < fvuNbDigisPerBinSts[uBin]) {
      fT0StsCorr->Fill(fvuNbDigisPerBinT0[uBin], fvuNbDigisPerBinSts[uBin]);
      fT0StsCorrProf->Fill(fvuNbDigisPerBinT0[uBin], fvuNbDigisPerBinSts[uBin]);
    }  // if( 0 < fvuNbDigisPerBinT0[   uBin ] || 0 < fvuNbDigisPerBinSts[  uBin ] )
    if (0 < fvuNbDigisPerBinT0[uBin] || 0 < fvuNbDigisPerBinMuch[uBin]) {
      fT0MuchCorr->Fill(fvuNbDigisPerBinT0[uBin], fvuNbDigisPerBinMuch[uBin]);
      fT0MuchCorrProf->Fill(fvuNbDigisPerBinT0[uBin],
                            fvuNbDigisPerBinMuch[uBin]);
    }  // if( 0 < fvuNbDigisPerBinT0[   uBin ] || 0 < fvuNbDigisPerBinMuch[  uBin ] )
    if (0 < fvuNbDigisPerBinT0[uBin] || 0 < fvuNbDigisPerBinTof[uBin]) {
      fT0TofCorr->Fill(fvuNbDigisPerBinT0[uBin], fvuNbDigisPerBinTof[uBin]);
      fT0TofCorrProf->Fill(fvuNbDigisPerBinT0[uBin], fvuNbDigisPerBinTof[uBin]);
    }  // if( 0 < fvuNbDigisPerBinT0[   uBin ] || 0 < fvuNbDigisPerBinTof[  uBin ] )
    if (0 < fvuNbDigisPerBinT0[uBin] || 0 < fvuNbDigisPerBinRich[uBin]) {
      fT0RichCorr->Fill(fvuNbDigisPerBinT0[uBin], fvuNbDigisPerBinRich[uBin]);
      fT0RichCorrProf->Fill(fvuNbDigisPerBinT0[uBin],
                            fvuNbDigisPerBinRich[uBin]);
    }  // if( 0 < fvuNbDigisPerBinT0[   uBin ] || 0 < fvuNbDigisPerBinRich[  uBin ] )

    if (0 < fvuNbDigisPerBinSts[uBin] || 0 < fvuNbDigisPerBinMuch[uBin]) {
      fStsMuchCorr->Fill(fvuNbDigisPerBinSts[uBin], fvuNbDigisPerBinMuch[uBin]);
      fStsMuchCorrProf->Fill(fvuNbDigisPerBinSts[uBin],
                             fvuNbDigisPerBinMuch[uBin]);
    }  // if( 0 < fvuNbDigisPerBinSts[   uBin ] || 0 < fvuNbDigisPerBinMuch[  uBin ] )
    if (0 < fvuNbDigisPerBinSts[uBin] || 0 < fvuNbDigisPerBinTof[uBin]) {
      fStsTofCorr->Fill(fvuNbDigisPerBinSts[uBin], fvuNbDigisPerBinTof[uBin]);
      fStsTofCorrProf->Fill(fvuNbDigisPerBinSts[uBin],
                            fvuNbDigisPerBinTof[uBin]);
    }  // if( 0 < fvuNbDigisPerBinSts[   uBin ] || 0 < fvuNbDigisPerBinTof[  uBin ] )
    if (0 < fvuNbDigisPerBinSts[uBin] || 0 < fvuNbDigisPerBinRich[uBin]) {
      fStsRichCorr->Fill(fvuNbDigisPerBinSts[uBin], fvuNbDigisPerBinRich[uBin]);
      fStsRichCorrProf->Fill(fvuNbDigisPerBinSts[uBin],
                             fvuNbDigisPerBinRich[uBin]);
    }  // if( 0 < fvuNbDigisPerBinSts[   uBin ] || 0 < fvuNbDigisPerBinRich[  uBin ] )

    if (0 < fvuNbDigisPerBinMuch[uBin] || 0 < fvuNbDigisPerBinTof[uBin]) {
      fMuchTofCorr->Fill(fvuNbDigisPerBinMuch[uBin], fvuNbDigisPerBinTof[uBin]);
      fMuchTofCorrProf->Fill(fvuNbDigisPerBinMuch[uBin],
                             fvuNbDigisPerBinTof[uBin]);
    }  // if( 0 < fvuNbDigisPerBinMuch[   uBin ] || 0 < fvuNbDigisPerBinTof[  uBin ] )
    if (0 < fvuNbDigisPerBinMuch[uBin] || 0 < fvuNbDigisPerBinRich[uBin]) {
      fMuchRichCorr->Fill(fvuNbDigisPerBinMuch[uBin],
                          fvuNbDigisPerBinRich[uBin]);
      fMuchRichCorrProf->Fill(fvuNbDigisPerBinMuch[uBin],
                              fvuNbDigisPerBinRich[uBin]);
    }  // if( 0 < fvuNbDigisPerBinMuch[   uBin ] || 0 < fvuNbDigisPerBinRich[  uBin ] )

    if (0 < fvuNbDigisPerBinTof[uBin] || 0 < fvuNbDigisPerBinRich[uBin]) {
      fTofRichCorr->Fill(fvuNbDigisPerBinTof[uBin], fvuNbDigisPerBinRich[uBin]);
      fTofRichCorrProf->Fill(fvuNbDigisPerBinTof[uBin],
                             fvuNbDigisPerBinRich[uBin]);
    }  // if( 0 < fvuNbDigisPerBinTof[   uBin ] || 0 < fvuNbDigisPerBinRich[  uBin ] )

    for (UInt_t uStsDpb = 0; uStsDpb < kuMaxNbStsDpbs; ++uStsDpb) {
      if (0 < fvuNbDigisPerBinT0[uBin]
          || 0 < fvuNbDigisPerBinStsDpb[uStsDpb][uBin]) {
        fT0StsDpbCorr[uStsDpb]->Fill(fvuNbDigisPerBinT0[uBin],
                                     fvuNbDigisPerBinStsDpb[uStsDpb][uBin]);
        fT0StsDpbCorrProf[uStsDpb]->Fill(fvuNbDigisPerBinT0[uBin],
                                         fvuNbDigisPerBinStsDpb[uStsDpb][uBin]);
      }  // if( 0 < fvuNbDigisPerBinT0[   uBin ] || 0 < fvuNbDigisPerBinStsDpb[uStsDpb][  uBin ] )

      if (0 < fvuNbDigisPerBinStsDpb[uStsDpb][uBin]
          || 0 < fvuNbDigisPerBinMuch[uBin]) {
        fStsMuchDpbCorr[uStsDpb]->Fill(fvuNbDigisPerBinStsDpb[uStsDpb][uBin],
                                       fvuNbDigisPerBinMuch[uBin]);
        fStsMuchDpbCorrProf[uStsDpb]->Fill(
          fvuNbDigisPerBinStsDpb[uStsDpb][uBin], fvuNbDigisPerBinMuch[uBin]);
      }  // if( 0 < fvuNbDigisPerBinStsDpb[uStsDpb][   uBin ] || 0 < fvuNbDigisPerBinMuch[  uBin ] )
      if (0 < fvuNbDigisPerBinStsDpb[uStsDpb][uBin]
          || 0 < fvuNbDigisPerBinTof[uBin]) {
        fStsTofDpbCorr[uStsDpb]->Fill(fvuNbDigisPerBinStsDpb[uStsDpb][uBin],
                                      fvuNbDigisPerBinTof[uBin]);
        fStsTofDpbCorrProf[uStsDpb]->Fill(fvuNbDigisPerBinStsDpb[uStsDpb][uBin],
                                          fvuNbDigisPerBinTof[uBin]);
      }  // if( 0 < fvuNbDigisPerBinStsDpb[uStsDpb][   uBin ] || 0 < fvuNbDigisPerBinTof[  uBin ] )
      if (0 < fvuNbDigisPerBinStsDpb[uStsDpb][uBin]
          || 0 < fvuNbDigisPerBinRich[uBin]) {
        fStsRichDpbCorr[uStsDpb]->Fill(fvuNbDigisPerBinStsDpb[uStsDpb][uBin],
                                       fvuNbDigisPerBinRich[uBin]);
        fStsRichDpbCorrProf[uStsDpb]->Fill(
          fvuNbDigisPerBinStsDpb[uStsDpb][uBin], fvuNbDigisPerBinRich[uBin]);
      }  // if( 0 < fvuNbDigisPerBinStsDpb[uStsDpb][   uBin ] || 0 < fvuNbDigisPerBinRich[  uBin ] )
    }    // for( UInt_t uStsDpb = 0; uStsDpb < kuMaxNbStsDpbs; ++uStsDpb )
  }      // for( UInt_t uBin = 0; uBin < fiBinNb; ++uBin )

  fNrTs++;
}


// ---- Finish --------------------------------------------------------
void CbmCheckDigisNbCorr::Finish() { WriteHistos(); }

void CbmCheckDigisNbCorr::WriteHistos() {
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  TFile* outfile = TFile::Open(fOutFileName, "RECREATE");


  /// 2D
  fT0StsCorr->Write();
  fT0MuchCorr->Write();
  fT0TofCorr->Write();
  fT0RichCorr->Write();
  fStsMuchCorr->Write();
  fStsTofCorr->Write();
  fStsRichCorr->Write();
  fMuchTofCorr->Write();
  fMuchRichCorr->Write();
  fTofRichCorr->Write();

  /// Profiles
  fT0StsCorrProf->Write();
  fT0MuchCorrProf->Write();
  fT0TofCorrProf->Write();
  fT0RichCorrProf->Write();
  fStsMuchCorrProf->Write();
  fStsTofCorrProf->Write();
  fStsRichCorrProf->Write();
  fMuchTofCorrProf->Write();
  fMuchRichCorrProf->Write();
  fTofRichCorrProf->Write();

  for (UInt_t uStsDpb = 0; uStsDpb < kuMaxNbStsDpbs; ++uStsDpb) {
    fT0StsDpbCorr[uStsDpb]->Write();
    fStsMuchDpbCorr[uStsDpb]->Write();
    fStsTofDpbCorr[uStsDpb]->Write();
    fStsRichDpbCorr[uStsDpb]->Write();

    fT0StsDpbCorrProf[uStsDpb]->Write();
    fStsMuchDpbCorrProf[uStsDpb]->Write();
    fStsTofDpbCorrProf[uStsDpb]->Write();
    fStsRichDpbCorrProf[uStsDpb]->Write();
  }  // for( UInt_t uStsDpb = 0; uStsDpb < kuMaxNbStsDpbs; ++uStsDpb )

  outfile->Close();
  delete outfile;

  gFile      = oldFile;
  gDirectory = oldDir;
}

ClassImp(CbmCheckDigisNbCorr)
