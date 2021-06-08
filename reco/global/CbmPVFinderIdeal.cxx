/* Copyright (C) 2005-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Denis Bertini [committer], Florian Uhlig, Volker Friese */

// -------------------------------------------------------------------------
// -----                   CbmPVFinderIdeal source file                -----
// -----                  Created 28/11/05  by V. Friese               -----
// -------------------------------------------------------------------------
#include "CbmPVFinderIdeal.h"

#include "CbmMCTrack.h"
#include "CbmVertex.h"

#include "FairRootManager.h"
#include <Logger.h>

#include "TClonesArray.h"
#include "TMatrixTSym.h"

#include <iostream>

using std::cout;
using std::endl;


// -----   Default constructor   -------------------------------------------
CbmPVFinderIdeal::CbmPVFinderIdeal() : CbmPrimaryVertexFinder(), fMCTracks(NULL) {}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmPVFinderIdeal::~CbmPVFinderIdeal() {}
// -------------------------------------------------------------------------


// -----   Public method Init   --------------------------------------------
void CbmPVFinderIdeal::Init()
{

  // Get and check FairRootManager
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman) {
    cout << "-E- CbmPVFinderIdeal::Init: "
         << "RootManager not instantised!" << endl;
    return;
  }

  // Get MCTrack array
  fMCTracks = (TClonesArray*) ioman->GetObject("MCTrack");
  if (!fMCTracks) {
    cout << "-E- CbmPVFinderIdeal::Init: No MCTrack array!" << endl;
    return;
  }
}
// -------------------------------------------------------------------------


// -----   Public method FindPrimaryVertex   -------------------------------
Int_t CbmPVFinderIdeal::FindPrimaryVertex(TClonesArray*, CbmVertex* vertex)
{

  if (!fMCTracks) return 1;

  CbmMCTrack* mcTrack  = NULL;
  CbmMCTrack* refTrack = NULL;
  Int_t nPrim          = 0;
  Int_t nTracks        = fMCTracks->GetEntriesFast();

  for (Int_t iTrack = 0; iTrack < nTracks; iTrack++) {
    mcTrack = (CbmMCTrack*) fMCTracks->At(iTrack);
    if (!mcTrack) continue;
    if (mcTrack->GetMotherId() != -1) continue;  // secondary
    nPrim++;
    refTrack = mcTrack;
  }

  Double_t x    = 0.;
  Double_t y    = 0.;
  Double_t z    = 0;
  Double_t chi2 = 0.;
  Int_t ndf     = 0;
  TMatrixFSym covMat(3);
  if (refTrack) {
    x = refTrack->GetStartX();
    y = refTrack->GetStartY();
    z = refTrack->GetStartZ();
  }
  else
    cout << "-W- CbmPVFinderIdeal::FindPrimaryVertex: "
         << "No primary MCTracks found!" << endl;


  vertex->SetVertex(x, y, z, chi2, ndf, nPrim, covMat);

  return 0;
}
// -------------------------------------------------------------------------


// ----   Find event vertex   ----------------------------------------------
Int_t CbmPVFinderIdeal::FindEventVertex(CbmEvent* /*event*/, TClonesArray* /*tracks*/)
{
  LOG(fatal) << GetName() << ": handling of event objects is not implemented yet.";
  return 1;
}
// -------------------------------------------------------------------------


ClassImp(CbmPVFinderIdeal)
