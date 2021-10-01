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
#include "CbmQaCanvas.h"
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

  const int nStations = fNoTrdStations * fNoTrdPerStation;

  for (int i = 0; i < nStations; i++) {
    fvhHitPullsX.push_back(new TH1F(Form("L%iHitPullsX", i), "", 500, -50, 50));
    fvhHitPullsY.push_back(new TH1F(Form("L%iHitPullsY", i), "", 500, -50, 50));

    histFolder->Add(fvhHitPullsX[i]);
    histFolder->Add(fvhHitPullsY[i]);

    fvhedEcut.push_back(new TH1F(Form("L%iedEcut", i), Form("dEdx of e- for layer %i, mom. cut", i), 600, 0., 60.));
    fvhedEall.push_back(new TH1F(Form("L%iedEall", i), Form("dEdx of e- for layer %i", i), 600, 0., 60.));

    fvhpidEcut.push_back(new TH1F(Form("L%ipidEcut", i), Form("dEdx of pi- for layer %i, mom. cut", i), 600, 0., 60.));
    fvhpidEall.push_back(new TH1F(Form("L%ipidEall", i), Form("dEdx of pi- for layer %i", i), 600, 0., 60.));

    histFolder->Add(fvhedEcut[i]);
    histFolder->Add(fvhedEall[i]);
    histFolder->Add(fvhpidEcut[i]);
    histFolder->Add(fvhpidEall[i]);

    fvdECanvas.push_back(new CbmQaCanvas(Form("cL%iEnergyLoss", i), Form("Energy Loss Layer %i", i), 2 * 400, 2 * 400));
    fvdECanvas[i]->Divide2D(4);
    fOutFolder.Add(fvdECanvas[i]);

    fvPullCanvas.push_back(new CbmQaCanvas(Form("cL%iPull", i), Form("Pull Distribution Layer %i", i), 2 * 400, 400));
    fvPullCanvas[i]->Divide2D(2);
    fOutFolder.Add(fvPullCanvas[i]);
  }

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

    const Int_t planeId = trdHit->GetPlaneId();

    if (planeId >= fNoTrdStations * fNoTrdPerStation) {
      cout << GetName() << ": Warning, TRD plane out of bounds, skipping hit." << endl;
      continue;
    }
    const Int_t partID = (((CbmMCTrack*) fMCTrackArray->At(trdPoint->GetTrackID()))->GetPdgCode());
    const Float_t momentum =
      TMath::Sqrt((trdPoint->GetPx() * trdPoint->GetPx()) + (trdPoint->GetPy() * trdPoint->GetPy())
                  + (trdPoint->GetPz() * trdPoint->GetPz()));

    //electrons
    if ((TMath::Abs(partID) == 11) && (momentum > fMomCutLower) && (momentum < fMomCutUpper)) {
      fvhedEcut[planeId]->Fill((trdHit->GetELoss()) * 1000000);
    }
    if (TMath::Abs(partID) == 11) { fvhedEall[planeId]->Fill((trdHit->GetELoss()) * 1000000); }

    //pions
    if ((TMath::Abs(partID) == 211) && (momentum > fMomCutLower) && (momentum < fMomCutUpper)) {
      fvhpidEcut[planeId]->Fill((trdHit->GetELoss()) * 1000000);
    }
    if (TMath::Abs(partID) == 211) { fvhpidEall[planeId]->Fill((trdHit->GetELoss()) * 1000000); }

    // compute the Hit pulls for X and Y coordinate
    const Float_t hitPosX   = trdHit->GetX();
    const Float_t pointPosX = trdPoint->GetX();
    const Float_t hitPullX  = (hitPosX - pointPosX);

    const Float_t hitPosY   = trdHit->GetY();
    const Float_t pointPosY = trdPoint->GetY();
    const Float_t hitPullY  = (hitPosY - pointPosY);

    // fill histograms
    fvhHitPullsX[planeId]->Fill(hitPullX);
    fvhHitPullsY[planeId]->Fill(hitPullY);

    //Float_t  hitErrX  = 0;
    //Float_t  hitErrY  = 0;
    //  hitErrX   = trdHit->GetDx();
    //        hitErrX  /= 10000;           // micrometers to centimeters
    //  hitErrY   = trdHit->GetDy();
    //        hitErrY  /= 10000;          // micrometers to centimeters
  }
}
// --------------------------------------------------------------------------

// ---- Finish --------------------------------------------------------------
void CbmTrdHitProducerQa::Finish() { WriteHistograms(); }
// --------------------------------------------------------------------------

// ---- Write test histograms ------------------------------------------------
void CbmTrdHitProducerQa::WriteHistograms()
{
  for (UInt_t i = 0; i < fvdECanvas.size(); i++) {
    fvdECanvas[i]->cd(1);
    fvhedEcut[i]->DrawCopy("", "");

    fvdECanvas[i]->cd(2);
    fvhedEall[i]->DrawCopy("", "");

    fvdECanvas[i]->cd(3);
    fvhpidEcut[i]->DrawCopy("", "");

    fvdECanvas[i]->cd(4);
    fvhpidEall[i]->DrawCopy("", "");
  }

  for (UInt_t i = 0; i < fvPullCanvas.size(); i++) {
    fvPullCanvas[i]->cd(1);
    fvhHitPullsX[i]->DrawCopy("", "");

    fvPullCanvas[i]->cd(2);
    fvhHitPullsY[i]->DrawCopy("", "");
  }

  FairSink* sink = FairRootManager::Instance()->GetSink();
  sink->WriteObject(&fOutFolder, nullptr);
}
// --------------------------------------------------------------------------

ClassImp(CbmTrdHitProducerQa);
