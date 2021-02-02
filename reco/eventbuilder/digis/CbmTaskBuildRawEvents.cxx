/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "CbmTaskBuildRawEvents.h"
#include "CbmAlgoBuildRawEvents.h"

#include "CbmDigiManager.h"
#include "CbmEvent.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunOnline.h"

#include "TClonesArray.h"
#include "TH1.h"
#include "TH2.h"
#include "THttpServer.h"
#include <TFile.h>

// ---- Default constructor -------------------------------------------
CbmTaskBuildRawEvents::CbmTaskBuildRawEvents()
  : FairTask("CbmTaskBuildRawEvents") {
  /// Create Algo. To be made generic/switchable when more event building algo are available!
  fpAlgo = new CbmAlgoBuildRawEvents();
}

// ---- Destructor ----------------------------------------------------
CbmTaskBuildRawEvents::~CbmTaskBuildRawEvents() {}

// ----  Initialisation  ----------------------------------------------
void CbmTaskBuildRawEvents::SetParContainers() {
  /// Nothing to do
}

// ---- Init ----------------------------------------------------------
InitStatus CbmTaskBuildRawEvents::Init() {
  /// Get a handle from the IO manager
  FairRootManager* ioman = FairRootManager::Instance();

  //T0 not included in digi manager.
  fT0Digis = ioman->InitObjectAs<std::vector<CbmTofDigi> const*>("T0Digi");
  if (!fT0Digis) {
    LOG(info) << "No T0 digi input.";
  } else {
    LOG(info) << "T0 digi input.";
    fpAlgo->SetT0Digis(fT0Digis);
  }

  // Get a pointer to the previous already existing data level
  fDigiMan = CbmDigiManager::Instance();
  if (fbUseMuchBeamtimeDigi) { fDigiMan->UseMuchBeamTimeDigi(); }
  fDigiMan->Init();

  //Init STS digis
  if (!fDigiMan->IsPresent(ECbmModuleId::kSts)) {
    LOG(info) << "No STS digi input.";
  } else {
    LOG(info) << "STS digi input.";
    fStsDigis = new std::vector<CbmStsDigi>;
    fpAlgo->SetStsDigis(fStsDigis);
  }

  //Init MUCH digis
  if (!fDigiMan->IsPresent(ECbmModuleId::kMuch)) {
    LOG(info) << "No MUCH digi input.";
  } else {
    LOG(info) << "MUCH digi input.";
    if (fbUseMuchBeamtimeDigi) {
      fMuchBeamTimeDigis = new std::vector<CbmMuchBeamTimeDigi>;
      fpAlgo->SetMuchBeamTimeDigis(fMuchBeamTimeDigis);
    } else {
      fMuchDigis = new std::vector<CbmMuchDigi>;
      fpAlgo->SetMuchDigis(fMuchDigis);
    }
  }

  //Init TRD digis
  if (!fDigiMan->IsPresent(ECbmModuleId::kTrd)) {
    LOG(info) << "No TRD digi input.";
  } else {
    LOG(info) << "TRD digi input.";
    fTrdDigis = new std::vector<CbmTrdDigi>;
    fpAlgo->SetTrdDigis(fTrdDigis);
  }

  //Init TOF digis
  if (!fDigiMan->IsPresent(ECbmModuleId::kTof)) {
    LOG(info) << "No TOF digi input.";
  } else {
    LOG(info) << "TOF digi input.";
    fTofDigis = new std::vector<CbmTofDigi>;
    fpAlgo->SetTofDigis(fTofDigis);
  }

  //Init RICH digis
  if (!fDigiMan->IsPresent(ECbmModuleId::kRich)) {
    LOG(info) << "No RICH digi input.";
  } else {
    LOG(info) << "RICH digi input.";
    fRichDigis = new std::vector<CbmRichDigi>;
    fpAlgo->SetRichDigis(fRichDigis);
  }

  //Init PSD digis
  if (!fDigiMan->IsPresent(ECbmModuleId::kPsd)) {
    LOG(info) << "No PSD digi input.";
  } else {
    LOG(info) << "PSD digi input.";
    fPsdDigis = new std::vector<CbmPsdDigi>;
    fpAlgo->SetPsdDigis(fPsdDigis);
  }

  /// Register output array (CbmEvent)
  fEvents = new TClonesArray("CbmEvent", 100);
  ioman->Register(
    "CbmEvent", "Cbm_Event", fEvents, IsOutputBranchPersistent("CbmEvent"));

  if (!fEvents) LOG(fatal) << "Output branch was not created";

  /// Call Algo Init method
  if (kTRUE == fpAlgo->InitAlgo())
    return kSUCCESS;
  else
    return kFATAL;
}

// ---- ReInit  -------------------------------------------------------
InitStatus CbmTaskBuildRawEvents::ReInit() { return kSUCCESS; }

// ---- Exec ----------------------------------------------------------
void CbmTaskBuildRawEvents::Exec(Option_t* /*option*/) {
  LOG(debug2) << "CbmTaskBuildRawEvents::Exec => Starting sequence";

  //Read STS digis
  if (fDigiMan->IsPresent(ECbmModuleId::kSts)) {
    fStsDigis->clear();
    for (Int_t i = 0; i < fDigiMan->GetNofDigis(ECbmModuleId::kSts); i++) {
      const CbmStsDigi* Digi = fDigiMan->Get<CbmStsDigi>(i);
      fStsDigis->insert(fStsDigis->begin() + i, *Digi);
    }
    LOG(debug) << "Read: " << fStsDigis->size() << " STS digis.";
    LOG(debug) << "In DigiManager: "
               << fDigiMan->GetNofDigis(ECbmModuleId::kSts) << " STS digis.";
  }

  //Read MUCH digis
  if (fDigiMan->IsPresent(ECbmModuleId::kMuch)) {
    if (fbUseMuchBeamtimeDigi) {
      fMuchBeamTimeDigis->clear();
      for (Int_t i = 0; i < fDigiMan->GetNofDigis(ECbmModuleId::kMuch); i++) {
        const CbmMuchBeamTimeDigi* Digi = fDigiMan->Get<CbmMuchBeamTimeDigi>(i);
        fMuchBeamTimeDigis->insert(fMuchBeamTimeDigis->begin() + i, *Digi);
      }
      LOG(debug) << "Read: " << fDigiMan->GetNofDigis(ECbmModuleId::kMuch)
                 << " MUCH digis.";
      LOG(debug) << "In DigiManager: " << fMuchBeamTimeDigis->size()
                 << " MUCH digis.";
    } else {
      fMuchDigis->clear();
      for (Int_t i = 0; i < fDigiMan->GetNofDigis(ECbmModuleId::kMuch); i++) {
        const CbmMuchDigi* Digi = fDigiMan->Get<CbmMuchDigi>(i);
        fMuchDigis->insert(fMuchDigis->begin() + i, *Digi);
      }
      LOG(debug) << "Read: " << fDigiMan->GetNofDigis(ECbmModuleId::kMuch)
                 << " MUCH digis.";
      LOG(debug) << "In DigiManager: " << fMuchDigis->size() << " MUCH digis.";
    }
  }

  //Read TRD digis
  if (fDigiMan->IsPresent(ECbmModuleId::kTrd)) {
    fTrdDigis->clear();
    for (Int_t i = 0; i < fDigiMan->GetNofDigis(ECbmModuleId::kTrd); i++) {
      const CbmTrdDigi* Digi = fDigiMan->Get<CbmTrdDigi>(i);
      fTrdDigis->insert(fTrdDigis->begin() + i, *Digi);
    }
    LOG(debug) << "Read: " << fDigiMan->GetNofDigis(ECbmModuleId::kTrd)
               << " TRD digis.";
    LOG(debug) << "In DigiManager: " << fTrdDigis->size() << " TRD digis.";
  }

  //Read TOF digis
  if (fDigiMan->IsPresent(ECbmModuleId::kTof)) {
    fTofDigis->clear();
    for (Int_t i = 0; i < fDigiMan->GetNofDigis(ECbmModuleId::kTof); i++) {
      const CbmTofDigi* Digi = fDigiMan->Get<CbmTofDigi>(i);
      fTofDigis->insert(fTofDigis->begin() + i, *Digi);
    }
    LOG(debug) << "Read: " << fDigiMan->GetNofDigis(ECbmModuleId::kTof)
               << " TOF digis.";
    LOG(debug) << "In DigiManager: " << fTofDigis->size() << " TOF digis.";
  }

  //Read RICH digis
  if (fDigiMan->IsPresent(ECbmModuleId::kRich)) {
    fRichDigis->clear();
    for (Int_t i = 0; i < fDigiMan->GetNofDigis(ECbmModuleId::kRich); i++) {
      const CbmRichDigi* Digi = fDigiMan->Get<CbmRichDigi>(i);
      fRichDigis->insert(fRichDigis->begin() + i, *Digi);
    }
    LOG(debug) << "Read: " << fDigiMan->GetNofDigis(ECbmModuleId::kRich)
               << " RICH digis.";
    LOG(debug) << "In DigiManager: " << fRichDigis->size() << " RICH digis.";
  }

  //Read PSD digis
  if (fDigiMan->IsPresent(ECbmModuleId::kPsd)) {
    fPsdDigis->clear();
    for (Int_t i = 0; i < fDigiMan->GetNofDigis(ECbmModuleId::kPsd); i++) {
      const CbmPsdDigi* Digi = fDigiMan->Get<CbmPsdDigi>(i);
      fPsdDigis->insert(fPsdDigis->begin() + i, *Digi);
    }
    LOG(debug) << "Read: " << fDigiMan->GetNofDigis(ECbmModuleId::kPsd)
               << " PSD digis.";
    LOG(debug) << "In DigiManager: " << fPsdDigis->size() << " PSD digis.";
  }

  /// Call Algo ProcessTs method
  fpAlgo->ProcessTs();

  /// Save the resulting vector of events in TClonesArray
  FillOutput();
  LOG(debug2) << "CbmTaskBuildRawEvents::Exec => Done";
}


// ---- Finish --------------------------------------------------------
void CbmTaskBuildRawEvents::Finish() {
  if (fbFillHistos) { SaveHistos(); }  // if( fbFillHistos )

  /// Call Algo finish method
  fpAlgo->Finish();
}

//----------------------------------------------------------------------
void CbmTaskBuildRawEvents::FillOutput() {
  /// Clear TClonesArray before usage.
  fEvents->Delete();

  /// Get vector reference from algo
  std::vector<CbmEvent*> vEvents = fpAlgo->GetEventVector();

  /// Move CbmEvent from temporary vector to TClonesArray
  for (CbmEvent* event : vEvents) {
    LOG(debug) << "Vector: " << event->ToString();
    new ((*fEvents)[fEvents->GetEntriesFast()]) CbmEvent(std::move(*event));
    LOG(debug) << "TClonesArray: "
               << static_cast<CbmEvent*>(
                    fEvents->At(fEvents->GetEntriesFast() - 1))
                    ->ToString();
  }  // for( CbmEvent* event: vEvents )

  /// Clear event vector after usage
  fpAlgo->ClearEventVector();
}
//----------------------------------------------------------------------
void CbmTaskBuildRawEvents::SaveHistos() {
  /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
  std::vector<std::pair<TNamed*, std::string>> vHistos =
    fpAlgo->GetHistoVector();

  /// (Re-)Create ROOT file to store the histos
  TDirectory* oldDir = NULL;
  TFile* histoFile   = NULL;
  /// Store current directory position to allow restore later
  oldDir = gDirectory;
  /// open separate histo file in recreate mode
  histoFile = new TFile(fsOutFileName, "RECREATE");
  histoFile->cd();

  /// Save all plots and create folders if needed
  for (UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto) {
    /// Make sure we end up in chosen folder
    TString sFolder = vHistos[uHisto].second.data();
    if (nullptr == gDirectory->Get(sFolder)) gDirectory->mkdir(sFolder);
    gDirectory->cd(sFolder);

    /// Write plot
    vHistos[uHisto].first->Write();

    histoFile->cd();
  }  // for( UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto )

  /// Restore original directory position
  oldDir->cd();
  histoFile->Close();
}
//----------------------------------------------------------------------
void CbmTaskBuildRawEvents::SetFillHistos(Bool_t bFlag) {
  fbFillHistos = bFlag;
  if (nullptr != fpAlgo) fpAlgo->SetFillHistos(fbFillHistos);
}
void CbmTaskBuildRawEvents::SetOutFilename(TString sNameIn) {
  fsOutFileName = sNameIn;
}

void CbmTaskBuildRawEvents::SetReferenceDetector(
  RawEventBuilderDetector refDet) {
  if (nullptr != fpAlgo) fpAlgo->SetReferenceDetector(refDet);
}
void CbmTaskBuildRawEvents::AddDetector(RawEventBuilderDetector selDet) {
  if (nullptr != fpAlgo) fpAlgo->AddDetector(selDet);
}
void CbmTaskBuildRawEvents::RemoveDetector(RawEventBuilderDetector selDet) {
  if (nullptr != fpAlgo) fpAlgo->RemoveDetector(selDet);
}

void CbmTaskBuildRawEvents::SetTriggerMinNumber(ECbmModuleId selDet,
                                                UInt_t uVal) {
  if (nullptr != fpAlgo) fpAlgo->SetTriggerMinNumber(selDet, uVal);
}
void CbmTaskBuildRawEvents::SetTriggerMaxNumber(ECbmModuleId selDet,
                                                Int_t iVal) {
  if (nullptr != fpAlgo) fpAlgo->SetTriggerMaxNumber(selDet, iVal);
}

void CbmTaskBuildRawEvents::SetTriggerWindow(ECbmModuleId det,
                                             Double_t dWinBeg,
                                             Double_t dWinEnd) {
  if (nullptr != fpAlgo) fpAlgo->SetTriggerWindow(det, dWinBeg, dWinEnd);
}


void CbmTaskBuildRawEvents::SetTsParameters(Double_t dTsStartTime,
                                            Double_t dTsLength,
                                            Double_t dTsOverLength) {
  if (nullptr != fpAlgo)
    fpAlgo->SetTsParameters(dTsStartTime, dTsLength, dTsOverLength);
}

void CbmTaskBuildRawEvents::SetEventOverlapMode(EOverlapModeRaw mode) {
  if (nullptr != fpAlgo) fpAlgo->SetEventOverlapMode(mode);
}
void CbmTaskBuildRawEvents::SetIgnoreTsOverlap(Bool_t bFlagIn) {
  if (nullptr != fpAlgo) fpAlgo->SetIgnoreTsOverlap(bFlagIn);
}
void CbmTaskBuildRawEvents::ChangeMuchBeamtimeDigiFlag(Bool_t bFlagIn) {
  if (nullptr != fpAlgo) fpAlgo->ChangeMuchBeamtimeDigiFlag(bFlagIn);
  fbUseMuchBeamtimeDigi = bFlagIn;
}

//----------------------------------------------------------------------

ClassImp(CbmTaskBuildRawEvents)
