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
  , fHitPullsX(new TH1F("fHitPullsX", "", 500, -50, 50))
  , fHitPullsY(new TH1F("fHitPullsY", "", 500, -50, 50))
  , S1L1edEcut(new TH1F("S1L1edEcut", "dEdx of e- for first layer, mom. cut ", 600, 0., 60.))
  , S1L1edEall(new TH1F("S1L1edEall", "dEdx of e- for first layer ", 600, 0., 60.))
  , S1L1pidEcut(new TH1F("S1L1pidEcut", "dEdx of pi- for first layer, mom. cut ", 600, 0., 60.))
  , S1L1pidEall(new TH1F("S1L1pidEall", "dEdx of pi- for first layer ", 600, 0., 60.))
  , S3L4edEcut(new TH1F("S3L4edEcut", "dEdx of e- for layer 12, mom. cut ", 600, 0., 60.))
  , S3L4edEall(new TH1F("S3L4edEall", "dEdx of e- for layer 12", 600, 0., 60.))
  , S3L4pidEcut(new TH1F("S3L4pidEcut", "dEdx of pi- for layer 12, mom. cut ", 600, 0., 60.))
  , S3L4pidEall(new TH1F("S3L4pidEall", "dEdx of pi- for layer 12", 600, 0., 60.))
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

  histFolder->Add(fHitPullsX);
  histFolder->Add(fHitPullsY);
  histFolder->Add(S1L1edEcut);
  histFolder->Add(S1L1edEall);
  histFolder->Add(S1L1pidEcut);
  histFolder->Add(S1L1pidEall);
  histFolder->Add(S3L4edEcut);
  histFolder->Add(S3L4edEall);
  histFolder->Add(S3L4pidEcut);
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
  // Loop over TRD hits
  for (Int_t iHit = 0; iHit < fTrdHitCollection->GetEntriesFast(); iHit++) {
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
      const Int_t partID = (((CbmMCTrack*) fMCTrackArray->At(trdPoint->GetTrackID()))->GetPdgCode());
      const Float_t momentum =
        TMath::Sqrt((trdPoint->GetPx() * trdPoint->GetPx()) + (trdPoint->GetPy() * trdPoint->GetPy())
                    + (trdPoint->GetPz() * trdPoint->GetPz()));
      //electrons
      if ((TMath::Abs(partID) == 11) && (momentum > fMomCutLower) && (momentum < fMomCutUpper)) {
        S1L1edEcut->Fill((trdHit->GetELoss()) * 1000000);
      }
      if (TMath::Abs(partID) == 11) { S1L1edEall->Fill((trdHit->GetELoss()) * 1000000); }

      //pions
      if ((TMath::Abs(partID) == 211) && (momentum > fMomCutLower) && (momentum < fMomCutUpper)) {
        S1L1pidEcut->Fill((trdHit->GetELoss()) * 1000000);
      }
      if (TMath::Abs(partID) == 211) { S1L1pidEall->Fill((trdHit->GetELoss()) * 1000000); }
    }


    if (plane == 12) {
      const Int_t partID = (((CbmMCTrack*) fMCTrackArray->At(trdPoint->GetTrackID()))->GetPdgCode());
      const Float_t momentum =
        TMath::Sqrt((trdPoint->GetPx() * trdPoint->GetPx()) + (trdPoint->GetPy() * trdPoint->GetPy())
                    + (trdPoint->GetPz() * trdPoint->GetPz()));
      //electrons
      if ((TMath::Abs(partID) == 11) && (momentum > fMomCutLower) && (momentum < fMomCutUpper)) {
        S3L4edEcut->Fill((trdHit->GetELoss()) * 1000000);
      }
      if (TMath::Abs(partID) == 11) { S3L4edEall->Fill((trdHit->GetELoss()) * 1000000); }

      //pions
      if ((TMath::Abs(partID) == 211) && (momentum > fMomCutLower) && (momentum < fMomCutUpper)) {
        S3L4pidEcut->Fill((trdHit->GetELoss()) * 1000000);
      }
      if (TMath::Abs(partID) == 211) { S3L4pidEall->Fill((trdHit->GetELoss()) * 1000000); }
    }

    if (plane == 1) {
      // compute the Hit pools for X and Y coordinate
      const Float_t hitPosX   = trdHit->GetX();
      const Float_t pointPosX = trdPoint->GetX();
      const Float_t hitPullX  = (hitPosX - pointPosX);

      const Float_t hitPosY   = trdHit->GetY();
      const Float_t pointPosY = trdPoint->GetY();
      const Float_t hitPullY  = (hitPosY - pointPosY);

      // fill histograms
      fHitPullsX->Fill(hitPullX);
      fHitPullsY->Fill(hitPullY);

      //Float_t  hitErrX  = 0;
      //Float_t  hitErrY  = 0;
      //	hitErrX   = trdHit->GetDx();
      //        hitErrX  /= 10000;           // micrometers to centimeters
      //	hitErrY   = trdHit->GetDy();
      //        hitErrY  /= 10000;          // micrometers to centimeters
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
