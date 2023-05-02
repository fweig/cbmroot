/* Copyright (C) 2006-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer], Florian Uhlig */

// -------------------------------------------------------------------------
// -----                  CbmMvdPileupManager source file              -----
// -----                  Created 08/11/06  by V. Friese               -----
// -------------------------------------------------------------------------
#include "CbmMvdPileupManager.h"

#include <Logger.h>  // for Logger, LOG

#include <TClonesArray.h>  // for TClonesArray
#include <TDirectory.h>    // for TDirectoryAtomicAdapter, gDirectory, TDire...
#include <TFile.h>         // for TFile, gFile
#include <TMathBase.h>     // for Min
#include <TObjArray.h>     // for TObjArray
#include <TTree.h>         // for TTree

#include <iostream>  // for operator<<, endl, basic_ostream, char_traits
#include <memory>    // for allocator

using std::endl;

// -----   Default constructor   -------------------------------------------
CbmMvdPileupManager::CbmMvdPileupManager() : TObject(), fBuffer(nullptr) {}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
CbmMvdPileupManager::CbmMvdPileupManager(TString fileName, TString branchName, Int_t nEvents)
  : TObject()
  , fBuffer(new TObjArray(nEvents))
{
  if (!FillBuffer(fileName, branchName, nEvents)) Fatal("CbmMvdPileupManager", "Error in filling buffer");
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmMvdPileupManager::~CbmMvdPileupManager()
{
  fBuffer->Delete();
  delete fBuffer;
}
// -------------------------------------------------------------------------


// -----   Public method GetEvent   ----------------------------------------
TClonesArray* CbmMvdPileupManager::GetEvent(Int_t iEvent)
{

  if (!fBuffer) Fatal("CbmMvdPileupManager::GetEvent", "No event buffer!");

  if (iEvent > fBuffer->GetEntriesFast()) {
    LOG(warning) << "CbmMvdPileupManager::GetEvent: Event " << iEvent << " not present in buffer! ";
    LOG(warning) << "                                   Returning nullptr pointer! ";
    return nullptr;
  }

  TClonesArray* pArray = (TClonesArray*) fBuffer->At(iEvent);

  if (!pArray) {
    LOG(warning) << "CbmMvdPileupManager::GetEvent: Returning nullptr pointer!";
    return nullptr;
  }

  return pArray;
}
// -------------------------------------------------------------------------


// -----   Private method FillBuffer   -------------------------------------
Int_t CbmMvdPileupManager::FillBuffer(TString fileName, TString branchName, Int_t nEvents)
{

  if (!fBuffer) Fatal("Fill Buffer", "No event buffer!");

  fBuffer->Delete();

  TClonesArray* pointArray = nullptr;

  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  TFile* bgfile = new TFile(fileName);
  if (!bgfile) {
    LOG(warning) << "CbmMvdPileupManager::FillBuffer:  Background file " << fileName << " could noy be opened! "
                 << endl;
    return 0;
  }
  LOG(info) << "CbmMvdPileupManager::FillBuffer: Opening file " << fileName;

  TTree* bgtree = bgfile->Get<TTree>("cbmsim");
  if (!bgtree) {
    LOG(warning) << "CbmMvdPileupManager::FillBuffer:  "
                 << "Could not find cbmsim tree in background file ";
    return 0;
  }

  Int_t nEventsInFile = bgtree->GetEntries();
  LOG(info) << "CbmMvdPileupManager::FillBuffer: " << nEventsInFile << " events in file";
  Int_t nBuffer = TMath::Min(nEvents, nEventsInFile);
  LOG(info) << "CbmMvdPileupManager::FillBuffer: Buffering " << nBuffer << " events";
  if (nBuffer < 100)
    LOG(warning) << "CbmMvdPileupManager::FillBuffer: "
                 << "Number of events may not be sufficient for appropriate "
                 << "presentation of background pattern!";

  bgtree->SetBranchAddress(branchName, &pointArray);

  for (Int_t iEvent = 0; iEvent < nBuffer; iEvent++) {
    bgtree->GetEntry(iEvent);
    fBuffer->AddAt(pointArray->Clone(), iEvent);
  }

  delete bgtree;
  bgfile->Close();
  delete bgfile;

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;

  return nBuffer;
}
// -------------------------------------------------------------------------


ClassImp(CbmMvdPileupManager)
