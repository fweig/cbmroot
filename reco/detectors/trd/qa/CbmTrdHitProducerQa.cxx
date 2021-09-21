/* Copyright (C) 2005-2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Matus Kalisky, Florian Uhlig, Denis Bertini [committer] */

// -----------------------------------------------------------------------
// -----                     CbmTrdHitProducerQa                     -----
// -----               Created 13/12/05  by M. Kalisky               -----
// -----------------------------------------------------------------------

#include "CbmTrdHitProducerQa.h"

#include "CbmDigiManager.h"
#include "CbmMCTrack.h"
#include "CbmMatch.h"
#include "CbmTrdDigi.h"
#include "CbmTrdHit.h"
#include "CbmTrdPoint.h"

#include "FairBaseParSet.h"
#include "FairDetector.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

#include "TClonesArray.h"
#include "TH1F.h"
#include "TMath.h"
#include "TObjArray.h"

#include <iostream>
using std::cout;
using std::endl;

// ---- Default constructor -------------------------------------------------

CbmTrdHitProducerQa::CbmTrdHitProducerQa() : CbmTrdHitProducerQa("TrdHitProducerQa", "") {}
// --------------------------------------------------------------------------


// ---- Standard constructor ------------------------------------------------
CbmTrdHitProducerQa::CbmTrdHitProducerQa(const char* name, const char*)
  : FairTask(name)
  , fOutFolder("TrdHitProducerQA", "TrdHitProducerQA")
  , fTrdHitCollection(NULL)
  , fTrdPointCollection(NULL)
  , fMCTrackArray(NULL)
  , fNoTrdStations(-1)
  , fNoTrdPerStation(-1)
  , fHitPoolsX(new TH1F("fHitPoolsX", "", 500, -50, 50))
  , fHitPoolsY(new TH1F("fHitPoolsY", "", 500, -50, 50))
  , S1L1eTR15(new TH1F("S1L1eTR15", "TR of e- for first layer ", 600, 0., 60.))
  , S1L1edEdx15(new TH1F("S1L1edEdx15", "dEdx of e- for first layer ", 600, 0., 60.))
  , S1L1edE15(new TH1F("S1L1edE15", "dEdx+TR of e- for first layer ", 600, 0., 60.))
  , S1L1edEall(new TH1F("S1L1edEall", "dEdx+TR of e- for first layer ", 600, 0., 60.))
  , S1L1pidE15(new TH1F("S1L1pidE15", "dEdx+TR of pi- for first layer ", 600, 0., 60.))
  , S1L1pidEall(new TH1F("S1L1pidEall", "dEdx+TR of pi- for first layer ", 600, 0., 60.))
  , S3L4eTR15(NULL)
  , S3L4edEdx15(NULL)
  , S3L4edE15(NULL)
  , S3L4edEall(new TH1F("S3L4edEall", "dEdx+TR of e- for layer 12", 600, 0., 60.))
  , S3L4pidE15(NULL)
  , S3L4pidEall(new TH1F("S3L4pidEall", "dEdx+TR of pi- for layer 12", 600, 0., 60.))
{
}
// --------------------------------------------------------------------------


// ---- Destructor ---------------------------------------------------------
CbmTrdHitProducerQa::~CbmTrdHitProducerQa() {}
// --------------------------------------------------------------------------


// ---- Initialisation ------------------------------------------------------
InitStatus CbmTrdHitProducerQa::Init()
{
  fOutFolder.Clear();
  histFolder = fOutFolder.AddFolder("hist", "Histogramms");

  histFolder->Add(fHitPoolsX);
  histFolder->Add(fHitPoolsY);
  histFolder->Add(S1L1eTR15);
  histFolder->Add(S1L1edEdx15);
  histFolder->Add(S1L1edE15);
  histFolder->Add(S1L1edEall);
  histFolder->Add(S1L1pidE15);
  histFolder->Add(S1L1pidEall);
  histFolder->Add(S3L4edEall);
  histFolder->Add(S3L4pidEall);

  // Get pointer to the ROOT I/O manager
  FairRootManager* rootMgr = FairRootManager::Instance();
  if (NULL == rootMgr) {
    cout << "-E- CbmTrdHitProducerQa::Init : "
         << "ROOT manager is not instantiated !" << endl;
    return kFATAL;
  }

  // Get a pointer to the previous already existing data level
  fDigiMan = CbmDigiManager::Instance();
  if (NULL == fDigiMan) {
    cout << "-W- CbmTrdHitProducerQa::Init : "
         << "no digi manager found !" << endl;
    return kERROR;
  }
  fDigiMan->Init();

  // Get pointer to TRD point array
  fTrdPointCollection = (TClonesArray*) rootMgr->GetObject("TrdPoint");
  if (NULL == fTrdPointCollection) {
    cout << "-W- CbmTrdHitProducerQa::Init : "
         << "no TRD point array !" << endl;
    return kERROR;
  }

  // Get pointer to TRD hit array
  fTrdHitCollection = (TClonesArray*) rootMgr->GetObject("TrdHit");
  if (NULL == fTrdHitCollection) {
    cout << "-W- CbmTrdHitProducerQa::Init : "
         << "no TRD hit array !" << endl;
    return kERROR;
  }

  // Get MCTrack array
  fMCTrackArray = (TClonesArray*) rootMgr->GetObject("MCTrack");
  if (!fMCTrackArray) {
    cout << "-E- CbmTrdHitProducerQa::Init : No MCTrack array!" << endl;
    return kFATAL;
  }

  // Get pointer to TRD digi array match
  if (!fDigiMan->IsMatchPresent(ECbmModuleId::kTrd)) {
    cout << GetName() << ": no TRD match branch in digi manager." << endl;
    return kERROR;
  }

  return kSUCCESS;
}

// --------------------------------------------------------------------------


// ---- Task execution ------------------------------------------------------
void CbmTrdHitProducerQa::Exec(Option_t*)
{
  //    Float_t  hitErrX  = 0;
  //    Float_t  hitErrY  = 0;
  Int_t partID = 0;
  Float_t momentum;

  // Event counters
  Int_t nTrdHit = fTrdHitCollection->GetEntriesFast();

  // Loop over TRD hits
  for (Int_t iHit = 0; iHit < nTrdHit; iHit++) {
    const CbmTrdHit* trdHit = (CbmTrdHit*) fTrdHitCollection->At(iHit);
    if (NULL == trdHit) continue;

    // This will have to change in the future, when the creation of the point
    // will not be necessarily connected to existence of the point

    const CbmMatch* trdDigiMatch = fDigiMan->GetMatch(ECbmModuleId::kTrd, trdHit->GetRefId());
    if (NULL == trdDigiMatch) continue;

    const CbmTrdPoint* trdPoint = (CbmTrdPoint*) fTrdPointCollection->At(trdDigiMatch->GetMatchedLink().GetIndex());
    if (NULL == trdPoint) continue;

    const Int_t plane = trdHit->GetPlaneId();

    if (plane == 1) {
      partID   = (((CbmMCTrack*) fMCTrackArray->At(trdPoint->GetTrackID()))->GetPdgCode());
      momentum = TMath::Sqrt((trdPoint->GetPx() * trdPoint->GetPx()) + (trdPoint->GetPy() * trdPoint->GetPy())
                             + (trdPoint->GetPz() * trdPoint->GetPz()));

      if ((TMath::Abs(partID) == 11) && (momentum > 1.25) && (momentum < 1.75)) {
        S1L1eTR15->Fill(0);  //(trdHit->GetELossTR())*1000000);
        S1L1edEdx15->Fill((trdHit->GetELoss()) * 1000000);
        S1L1edE15->Fill((trdHit->GetELoss()) * 1000000);
      }
      if (TMath::Abs(partID) == 11) { S1L1edEall->Fill((trdHit->GetELoss()) * 1000000); }
      if ((TMath::Abs(partID) == 211) && (momentum > 1.25) && (momentum < 1.75)) {
        S1L1pidE15->Fill((trdHit->GetELoss()) * 1000000);
      }
      if (TMath::Abs(partID) == 211) { S1L1pidEall->Fill((trdHit->GetELoss()) * 1000000); }
    }

    if (plane == 12) {
      partID   = (((CbmMCTrack*) fMCTrackArray->At(trdPoint->GetTrackID()))->GetPdgCode());
      momentum = TMath::Sqrt((trdPoint->GetPx() * trdPoint->GetPx()) + (trdPoint->GetPy() * trdPoint->GetPy())
                             + (trdPoint->GetPz() * trdPoint->GetPz()));
      if (TMath::Abs(partID) == 11) { S3L4edEall->Fill((trdHit->GetELoss()) * 1000000); }
      if (TMath::Abs(partID) == 211) { S3L4pidEall->Fill((trdHit->GetELoss()) * 1000000); }
    }

    if (plane == 1) {
      // compute the Hit pools for X and Y coordinate
      const Float_t hitPosX = trdHit->GetX();
      //	hitErrX   = trdHit->GetDx();
      //        hitErrX  /= 10000;           // micrometers to centimeters
      const Float_t pointPosX = trdPoint->GetX();
      const Float_t hitPoolX  = (hitPosX - pointPosX);  ///hitErrX;

      const Float_t hitPosY = trdHit->GetY();
      //	hitErrY   = trdHit->GetDy();
      //        hitErrY  /= 10000;          // micrometers to centimeters
      const Float_t pointPosY = trdPoint->GetY();
      const Float_t hitPoolY  = (hitPosY - pointPosY);  ///hitErrY;

      // fill histograms
      fHitPoolsX->Fill(hitPoolX);
      fHitPoolsY->Fill(hitPoolY);
    }
  }
}
// --------------------------------------------------------------------------

// ---- Finish --------------------------------------------------------------
void CbmTrdHitProducerQa::Finish() { WriteHistograms(); }
// --------------------------------------------------------------------------

// ---- Write test histograms ------------------------------------------------
void CbmTrdHitProducerQa::WriteHistograms()
{
  FairSink* sink = FairRootManager::Instance()->GetSink();
  sink->WriteObject(&fOutFolder, nullptr);
}
// --------------------------------------------------------------------------

ClassImp(CbmTrdHitProducerQa);
