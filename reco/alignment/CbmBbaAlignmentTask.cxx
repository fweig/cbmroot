/* Copyright (C) 2023-2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: S.Gorbunov[committer], N.Bluhme */

/// @file    CbmBbaAlignmentTask.cxx
/// @author  Sergey Gorbunov
/// @author  Nora Bluhme
/// @date    20.01.2023
/// @brief   Task class for alignment
///

// Cbm Headers ----------------------
#include "CbmBbaAlignmentTask.h"

#include "CbmL1PFFitter.h"

// ROOT headers

#include "FairRootManager.h"

#include "TClonesArray.h"
#include "TFile.h"
#include "TH1F.h"

//

#include "CbmStsTrack.h"

#include "bba/BBA.h"

// c++ and std headers

#include <iostream>


CbmBbaAlignmentTask::CbmBbaAlignmentTask(const char* name, Int_t iVerbose, TString histoFileName)
  : FairTask(name, iVerbose)
  , fHistoFileName(histoFileName)
{
  TFile* curFile           = gFile;
  TDirectory* curDirectory = gDirectory;

  if (!(fHistoFileName == "")) { fHistoFile = new TFile(fHistoFileName.Data(), "RECREATE"); }
  else {
    fHistoFile = gFile;
  }

  fHistoFile->cd();

  fHistoDir = fHistoFile->mkdir("Bba");
  fHistoDir->cd();

  hTestHisto = new TH1F("hTestHisto", "hTestHisto", 100, 0., 1.);

  gFile      = curFile;
  gDirectory = curDirectory;
}


CbmBbaAlignmentTask::~CbmBbaAlignmentTask() {}


InitStatus CbmBbaAlignmentTask::Init()
{
  //Get ROOT Manager
  FairRootManager* ioman = FairRootManager::Instance();

  if (!ioman) {
    LOG(error) << "CbmBbaAlignmentTask::Init :: RootManager not instantiated!";
    return kERROR;
  }

  // Get sts tracks
  fStsTrackArray = (TClonesArray*) ioman->GetObject("StsTrack");
  if (!fStsTrackArray) {
    LOG(error) << "CbmBbaAlignmentTask::Init: Sts track-array not found!";
    return kERROR;
  }

  // MC track match
  fMCTrackArray = (TClonesArray*) ioman->GetObject("MCTrack");
  if (!fMCTrackArray) {
    Warning("CbmBbaAlignmentTask::Init", "mc track array not found!");
    return kERROR;
  }

  // Track match
  fStsTrackMatchArray = (TClonesArray*) ioman->GetObject("StsTrackMatch");
  if (fStsTrackMatchArray == 0) {
    LOG(error) << "CbmBbaAlignmentTask::Init: track match array not found!";
    return kERROR;
  }

  return kSUCCESS;
}

void CbmBbaAlignmentTask::Exec(Option_t* /*opt*/)
{

  std::cout << "BBA: exec event N " << fNEvents << std::endl;

  fNEvents++;

  bba::BBA alignment;

  alignment.printInfo();

  // Check fit quality of the STS tracks

  std::vector<CbmStsTrack> vTracks;
  vTracks.reserve(fStsTrackArray->GetEntriesFast());

  for (int iTr = 0; iTr < fStsTrackArray->GetEntriesFast(); iTr++) {
    CbmStsTrack* stsTrack = ((CbmStsTrack*) fStsTrackArray->At(iTr));
    const auto* par       = stsTrack->GetParamFirst();
    if (!std::isfinite(par->GetQp())) continue;
    if (fabs(par->GetQp()) > 1.) continue;
    vTracks.push_back(*stsTrack);
  }

  std::vector<int> pdg(vTracks.size(), 211);  // pion PDG

  CbmL1PFFitter fitter;
  fitter.Fit(vTracks, pdg);

  double chi2Total  = 0;
  long int ndfTotal = 1;

  for (unsigned int iTr = 0; iTr < vTracks.size(); iTr++) {
    if (!std::isfinite(vTracks[iTr].GetChiSq())) continue;
    chi2Total += vTracks[iTr].GetChiSq();
    ndfTotal += vTracks[iTr].GetNDF();
  }
  std::cout << "BBA: chi2/ndf = " << chi2Total / ndfTotal << std::endl;
}

void CbmBbaAlignmentTask::Finish()
{
  TDirectory* curr   = gDirectory;
  TFile* currentFile = gFile;
  // Open output file and write histograms

  fHistoFile->cd();
  WriteHistosCurFile(fHistoDir);
  if (!(fHistoFileName == "")) {
    fHistoFile->Close();
    fHistoFile->Delete();
  }
  gFile      = currentFile;
  gDirectory = curr;
}

void CbmBbaAlignmentTask::WriteHistosCurFile(TObject* obj)
{
  if (!obj->IsFolder()) obj->Write();
  else {
    TDirectory* cur    = gDirectory;
    TFile* currentFile = gFile;

    TDirectory* sub = cur->GetDirectory(obj->GetName());
    sub->cd();
    TList* listSub = (static_cast<TDirectory*>(obj))->GetList();
    TIter it(listSub);
    while (TObject* obj1 = it())
      WriteHistosCurFile(obj1);
    cur->cd();
    gFile      = currentFile;
    gDirectory = cur;
  }
}

ClassImp(CbmBbaAlignmentTask);
