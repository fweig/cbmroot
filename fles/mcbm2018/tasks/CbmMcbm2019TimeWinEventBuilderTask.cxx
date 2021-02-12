/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "CbmMcbm2019TimeWinEventBuilderTask.h"

#include "CbmEvent.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunOnline.h"

#include "TClonesArray.h"
#include "TH1.h"
#include "TH2.h"
#include "THttpServer.h"
#include <TDirectory.h>
#include <TFile.h>

// ---- Default constructor -------------------------------------------
CbmMcbm2019TimeWinEventBuilderTask::CbmMcbm2019TimeWinEventBuilderTask()
  : FairTask("CbmMcbm2019TimeWinEventBuilderTask") {
  /// Create Algo. To be made generic/switchable when more event building algo are available!
  fpAlgo = new CbmMcbm2019TimeWinEventBuilderAlgo();
}

// ---- Destructor ----------------------------------------------------
CbmMcbm2019TimeWinEventBuilderTask::~CbmMcbm2019TimeWinEventBuilderTask() {}

// ----  Initialisation  ----------------------------------------------
void CbmMcbm2019TimeWinEventBuilderTask::SetParContainers() {
  /// Nothing to do
}

// ---- Init ----------------------------------------------------------
InitStatus CbmMcbm2019TimeWinEventBuilderTask::Init() {
  /// Get a handle from the IO manager
  FairRootManager* ioman = FairRootManager::Instance();

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
InitStatus CbmMcbm2019TimeWinEventBuilderTask::ReInit() { return kSUCCESS; }

// ---- Exec ----------------------------------------------------------
void CbmMcbm2019TimeWinEventBuilderTask::Exec(Option_t* /*option*/) {
  LOG(debug2)
    << "CbmMcbm2019TimeWinEventBuilderTask::Exec => Starting sequence";
  /// Call Algo ProcessTs method
  fpAlgo->ProcessTs();

  /// Save the resulting vector of events in TClonesArray
  FillOutput();
  LOG(debug2) << "CbmMcbm2019TimeWinEventBuilderTask::Exec => Done";
}


// ---- Finish --------------------------------------------------------
void CbmMcbm2019TimeWinEventBuilderTask::Finish() {
  if (fbFillHistos) { SaveHistos(); }  // if( fbFillHistos )

  /// Call Algo finish method
  fpAlgo->Finish();
}

//----------------------------------------------------------------------
void CbmMcbm2019TimeWinEventBuilderTask::FillOutput() {
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
void CbmMcbm2019TimeWinEventBuilderTask::SaveHistos() {
  /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
  std::vector<std::pair<TNamed*, std::string>> vHistos =
    fpAlgo->GetHistoVector();

  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  TFile* histoFile = nullptr;

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

  histoFile->Close();

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;
}
//----------------------------------------------------------------------
void CbmMcbm2019TimeWinEventBuilderTask::SetFillHistos(Bool_t bFlag) {
  fbFillHistos = bFlag;
  if (nullptr != fpAlgo) fpAlgo->SetFillHistos(fbFillHistos);
}
void CbmMcbm2019TimeWinEventBuilderTask::SetOutFilename(TString sNameIn) {
  fsOutFileName = sNameIn;
}

void CbmMcbm2019TimeWinEventBuilderTask::SetReferenceDetector(
  EventBuilderDetector refDet) {
  if (nullptr != fpAlgo) fpAlgo->SetReferenceDetector(refDet);
}
void CbmMcbm2019TimeWinEventBuilderTask::AddDetector(
  EventBuilderDetector selDet) {
  if (nullptr != fpAlgo) fpAlgo->AddDetector(selDet);
}
void CbmMcbm2019TimeWinEventBuilderTask::RemoveDetector(
  EventBuilderDetector selDet) {
  if (nullptr != fpAlgo) fpAlgo->RemoveDetector(selDet);
}

void CbmMcbm2019TimeWinEventBuilderTask::SetTriggerMinNumber(
  ECbmModuleId selDet,
  UInt_t uVal) {
  if (nullptr != fpAlgo) fpAlgo->SetTriggerMinNumber(selDet, uVal);
}
void CbmMcbm2019TimeWinEventBuilderTask::SetTriggerMaxNumber(
  ECbmModuleId selDet,
  Int_t iVal) {
  if (nullptr != fpAlgo) fpAlgo->SetTriggerMaxNumber(selDet, iVal);
}

void CbmMcbm2019TimeWinEventBuilderTask::SetTriggerWindow(ECbmModuleId det,
                                                          Double_t dWinBeg,
                                                          Double_t dWinEnd) {
  if (nullptr != fpAlgo) fpAlgo->SetTriggerWindow(det, dWinBeg, dWinEnd);
}


void CbmMcbm2019TimeWinEventBuilderTask::SetTsParameters(
  Double_t dTsStartTime,
  Double_t dTsLength,
  Double_t dTsOverLength) {
  if (nullptr != fpAlgo)
    fpAlgo->SetTsParameters(dTsStartTime, dTsLength, dTsOverLength);
}

void CbmMcbm2019TimeWinEventBuilderTask::SetEventOverlapMode(
  EOverlapMode mode) {
  if (nullptr != fpAlgo) fpAlgo->SetEventOverlapMode(mode);
}
void CbmMcbm2019TimeWinEventBuilderTask::SetIgnoreTsOverlap(Bool_t bFlagIn) {
  if (nullptr != fpAlgo) fpAlgo->SetIgnoreTsOverlap(bFlagIn);
}
void CbmMcbm2019TimeWinEventBuilderTask::ChangeMuchBeamtimeDigiFlag(
  Bool_t bFlagIn) {
  if (nullptr != fpAlgo) fpAlgo->ChangeMuchBeamtimeDigiFlag(bFlagIn);
}

//----------------------------------------------------------------------

ClassImp(CbmMcbm2019TimeWinEventBuilderTask)
