/* Copyright (C) 2005-2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: M. Kalisky, Florian Uhlig, Denis Bertini [committer] */

// -----------------------------------------------------------------------
// -----                     CbmTrdHitProducerQa                     -----
// -----               Created 13/12/05  by M. Kalisky               -----
// -----------------------------------------------------------------------

#include "CbmTrdHitProducerQa.h"

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
  , fTrdHitCollection(NULL)
  , fTrdDigiCollection(NULL)
  , fTrdDigiMatchCollection(NULL)
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
  // Get pointer to the ROOT I/O manager
  FairRootManager* rootMgr = FairRootManager::Instance();
  if (NULL == rootMgr) {
    cout << "-E- CbmTrdHitProducerQa::Init : "
         << "ROOT manager is not instantiated !" << endl;
    return kFATAL;
  }

  // Get pointer to TRD point array
  fTrdPointCollection = (TClonesArray*) rootMgr->GetObject("TrdPoint");
  if (NULL == fTrdPointCollection) {
    cout << "-W- CbmTrdHitProducerQa::Init : "
         << "no TRD point array !" << endl;
    return kERROR;
  }

  // Get pointer to TRD digi array
  fTrdDigiCollection = (TClonesArray*) rootMgr->GetObject("TrdDigi");
  if (NULL == fTrdDigiCollection) {
    cout << "-W- CbmTrdHitProducerQa::Init : "
         << "no TRD digi array !" << endl;
    return kERROR;
  }

  // Get pointer to TRD digi array
  fTrdDigiMatchCollection = (TClonesArray*) rootMgr->GetObject("TrdDigiMatch");
  if (NULL == fTrdDigiMatchCollection) {
    cout << "-W- CbmTrdHitProducerQa::Init : "
         << "no TRD digi match array !" << endl;
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

  return kSUCCESS;
}

// --------------------------------------------------------------------------


// ---- Task execution ------------------------------------------------------
void CbmTrdHitProducerQa::Exec(Option_t*)
{
  // Declare variables
  CbmTrdHit* trdHit = NULL;
  //    CbmTrdDigi* trdDigi = NULL;
  CbmMatch* trdDigiMatch = NULL;
  CbmTrdPoint* trdPoint  = NULL;
  //    CbmMCTrack* mctrack = NULL;


  Float_t hitPoolX = 0;
  Float_t hitPoolY = 0;
  Float_t hitPosX  = 0;
  Float_t hitPosY  = 0;
  //    Float_t  hitErrX  = 0;
  //    Float_t  hitErrY  = 0;

  Float_t pointPosX = 0;
  Float_t pointPosY = 0;

  Int_t plane  = 0;
  Int_t partID = 0;

  Float_t momentum;


  // Event counters
  Int_t nTrdHit = fTrdHitCollection->GetEntriesFast();

  // Loop over TRD hits
  for (Int_t iHit = 0; iHit < nTrdHit; iHit++) {
    trdHit = (CbmTrdHit*) fTrdHitCollection->At(iHit);
    if (NULL == trdHit) continue;

    // This will have to change in the future, when the creation of the poin
    // will not be necessarily connected to existence of tyhe point

    trdDigiMatch = (CbmMatch*) fTrdDigiMatchCollection->At(trdHit->GetRefId());
    if (NULL == trdDigiMatch) continue;

    trdPoint = (CbmTrdPoint*) fTrdPointCollection->At(trdDigiMatch->GetMatchedLink().GetIndex());
    if (NULL == trdPoint) continue;

    plane = trdHit->GetPlaneId();

    if (plane == 1) {

      partID = (((CbmMCTrack*) fMCTrackArray->At(trdPoint->GetTrackID()))->GetPdgCode());

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

      partID = (((CbmMCTrack*) fMCTrackArray->At(trdPoint->GetTrackID()))->GetPdgCode());

      momentum = TMath::Sqrt((trdPoint->GetPx() * trdPoint->GetPx()) + (trdPoint->GetPy() * trdPoint->GetPy())
                             + (trdPoint->GetPz() * trdPoint->GetPz()));

      if (TMath::Abs(partID) == 11) { S3L4edEall->Fill((trdHit->GetELoss()) * 1000000); }
      if (TMath::Abs(partID) == 211) { S3L4pidEall->Fill((trdHit->GetELoss()) * 1000000); }
    }


    if (plane == 1) {
      // compute the Hit pools for X and Y coordinate
      hitPosX = trdHit->GetX();
      //	hitErrX   = trdHit->GetDx();
      //        hitErrX  /= 10000;           // micrometers to centimeters
      pointPosX = trdPoint->GetX();
      hitPoolX  = (hitPosX - pointPosX);  ///hitErrX;


      hitPosY = trdHit->GetY();
      //	hitErrY   = trdHit->GetDy();
      //        hitErrY  /= 10000;          // micrometers to centimeters
      pointPosY = trdPoint->GetY();
      hitPoolY  = (hitPosY - pointPosY);  ///hitErrY;


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


// ---- Prepare test histograms ---------------------------------------------

/*
void CbmTrdHitProducerQa::PrepareHistograms()
{

    fHitPoolsX=NULL;
    fHitPoolsY=NULL; 

    S1L1eTR15=NULL; 
    S1L1edEdx15=NULL;
    S1L1edE15=NULL; 
    S1L1edEall=NULL;
    S1L1pidE15=NULL;
    S1L1pidEall=NULL;

    S3L4eTR15=NULL;
    S3L4edEdx15=NULL;
    S3L4edE15=NULL;
    S3L4edEall=NULL;
    S3L4pidE15=NULL;
    S3L4pidEall=NULL;

    fHitPoolsX = new TH1F("fHitPoolsX", "", 500, -50, 50);
    fHitPoolsY = new TH1F("fHitPoolsY", "", 500, -50, 50);

    S1L1eTR15   = new TH1F("S1L1eTR15","TR of e- for first layer ",600,0.,60.);
    S1L1edEdx15 = new TH1F("S1L1edEdx15","dEdx of e- for first layer ",600,0.,60.);
    S1L1edE15 = new TH1F("S1L1edE15","dEdx+TR of e- for first layer ",600,0.,60.);
    S1L1edEall = new TH1F("S1L1edEall","dEdx+TR of e- for first layer ",600,0.,60.);
    S1L1pidE15 = new TH1F("S1L1pidE15","dEdx+TR of pi- for first layer ",600,0.,60.);
    S1L1pidEall = new TH1F("S1L1pidEall","dEdx+TR of pi- for first layer ",600,0.,60.);
    S3L4edEall = new TH1F("S3L4edEall","dEdx+TR of e- for layer 12",600,0.,60.);
    S3L4pidEall = new TH1F("S3L4pidEall","dEdx+TR of pi- for layer 12",600,0.,60.);

}
*/
// --------------------------------------------------------------------------


// ---- Write test histograms ------------------------------------------------

void CbmTrdHitProducerQa::WriteHistograms()
{
  if (fHitPoolsX) fHitPoolsX->Write();
  if (fHitPoolsY) fHitPoolsY->Write();

  if (S1L1eTR15) S1L1eTR15->Write();
  if (S1L1edEdx15) S1L1edEdx15->Write();
  if (S1L1edE15) S1L1edE15->Write();
  if (S1L1edEall) S1L1edEall->Write();
  if (S1L1pidE15) S1L1pidE15->Write();
  if (S1L1pidEall) S1L1pidEall->Write();

  if (S3L4eTR15) S3L4eTR15->Write();
  if (S3L4edEdx15) S3L4edEdx15->Write();
  if (S3L4edE15) S3L4edE15->Write();
  if (S3L4edEall) S3L4edEall->Write();
  if (S3L4pidE15) S3L4pidE15->Write();
  if (S3L4pidEall) S3L4pidEall->Write();
}

// --------------------------------------------------------------------------

ClassImp(CbmTrdHitProducerQa);
