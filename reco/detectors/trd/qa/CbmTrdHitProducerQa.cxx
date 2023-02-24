/* Copyright (C) 2005-2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Matus Kalisky, Florian Uhlig, Denis Bertini [committer] */

// -----------------------------------------------------------------------
// -----                     CbmTrdHitProducerQa                     -----
// -----               Created 13/12/05  by M. Kalisky               -----
// -----------------------------------------------------------------------

#include "CbmTrdHitProducerQa.h"

#include "CbmDigiManager.h"
#include "CbmMCDataArray.h"
#include "CbmMCDataManager.h"
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
    fvhHitPullX.push_back(new TH1F(Form("L%iHitPullX", i), "", 500, -50, 50));
    fvhHitPullY.push_back(new TH1F(Form("L%iHitPullY", i), "", 500, -50, 50));
    fvhHitPullT.push_back(new TH1F(Form("L%iHitPullT", i), "", 500, -50, 50));
    fvhHitPullX[i]->SetCanExtend(TH1::kAllAxes);
    fvhHitPullY[i]->SetCanExtend(TH1::kAllAxes);
    fvhHitPullT[i]->SetCanExtend(TH1::kAllAxes);
    histFolder->Add(fvhHitPullX[i]);
    histFolder->Add(fvhHitPullY[i]);
    histFolder->Add(fvhHitPullT[i]);

    fvhHitResX.push_back(new TH1F(Form("L%iHitResX", i), "", 500, -50, 50));
    fvhHitResY.push_back(new TH1F(Form("L%iHitResY", i), "", 500, -50, 50));
    fvhHitResT.push_back(new TH1F(Form("L%iHitResT", i), "", 500, -50, 50));
    fvhHitResX[i]->SetCanExtend(TH1::kAllAxes);
    fvhHitResY[i]->SetCanExtend(TH1::kAllAxes);
    fvhHitResT[i]->SetCanExtend(TH1::kAllAxes);
    histFolder->Add(fvhHitResX[i]);
    histFolder->Add(fvhHitResY[i]);
    histFolder->Add(fvhHitResT[i]);

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

    fvPullCanvas.push_back(
      new CbmQaCanvas(Form("cL%iPull", i), Form("Pull Distribution Layer %i", i), 3 * 400, 2 * 400));
    fvPullCanvas[i]->Divide2D(6);
    fOutFolder.Add(fvPullCanvas[i]);
  }

  // Get pointer to the ROOT I/O manager
  FairRootManager* rootMgr = FairRootManager::Instance();
  if (nullptr == rootMgr) {
    cout << "-E- CbmTrdHitProducerQa::Init : "
         << "ROOT manager is not instantiated !" << endl;
    return kFATAL;
  }

  // Get a pointer to the previous already existing data level
  fDigiMan = CbmDigiManager::Instance();
  if (nullptr == fDigiMan) {
    cout << "-W- CbmTrdHitProducerQa::Init : "
         << "no digi manager found !" << endl;
    return kERROR;
  }
  fDigiMan->Init();

  CbmMCDataManager* mcManager = (CbmMCDataManager*) FairRootManager::Instance()->GetObject("MCDataManager");

  // Get pointer to TRD point array
  fTrdPoints = mcManager->InitBranch("TrdPoint");
  if (nullptr == fTrdPoints) {
    cout << "-W- CbmTrdHitProducerQa::Init : "
         << "no TRD point array !" << endl;
    return kERROR;
  }

  // Get pointer to TRD hit array
  fTrdHitCollection = (TClonesArray*) rootMgr->GetObject("TrdHit");
  if (nullptr == fTrdHitCollection) {
    cout << "-W- CbmTrdHitProducerQa::Init : "
         << "no TRD hit array !" << endl;
    return kERROR;
  }

  // Get MCTrack array
  fMCTrackArray = mcManager->InitBranch("MCTrack");
  if (nullptr == fMCTrackArray) {
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
  for (int iHit = 0; iHit < fTrdHitCollection->GetEntriesFast(); iHit++) {
    const CbmTrdHit* trdHit = (CbmTrdHit*) fTrdHitCollection->At(iHit);
    if (nullptr == trdHit) continue;

    const CbmMatch* trdDigiMatch = fDigiMan->GetMatch(ECbmModuleId::kTrd, trdHit->GetRefId());
    if (nullptr == trdDigiMatch) continue;

    if (0 == trdDigiMatch->GetNofLinks()) continue;  // catch case w/o links as then MatchedLink is invalid
    const CbmTrdPoint* trdPoint =
      dynamic_cast<CbmTrdPoint*>(fTrdPoints->Get(trdDigiMatch->GetMatchedLink()));  // file, event, object
    if (nullptr == trdPoint) continue;

    const int planeId = trdHit->GetPlaneId();

    if (planeId >= fNoTrdStations * fNoTrdPerStation) {
      cout << GetName() << ": Warning, TRD plane out of bounds, skipping hit."
           << " (" << planeId << " VS " << fNoTrdStations << " x " << fNoTrdPerStation << ")" << endl;
      continue;
    }

    //get particle ID of track corresponding to point
    const int fileId        = trdDigiMatch->GetMatchedLink().GetFile();
    const int event         = trdDigiMatch->GetMatchedLink().GetEntry();
    const int index         = trdPoint->GetTrackID();
    const CbmMCTrack* track = dynamic_cast<CbmMCTrack*>(fMCTrackArray->Get(fileId, event, index));
    const int partID        = track->GetPdgCode();

    const float momentum = TMath::Sqrt((trdPoint->GetPx() * trdPoint->GetPx()) + (trdPoint->GetPy() * trdPoint->GetPy())
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
    const float hitPosX   = trdHit->GetX();
    const float pointPosX = (trdPoint->GetX() + trdPoint->GetXOut()) / 2.;
    const float hitResX   = hitPosX - pointPosX;
    const float hitPullX  = hitResX / trdHit->GetDx();

    const float hitPosY   = trdHit->GetY();
    const float pointPosY = (trdPoint->GetY() + trdPoint->GetYOut()) / 2.;
    const float hitResY   = hitPosY - pointPosY;
    const float hitPullY  = hitResY / trdHit->GetDy();

    const double hitPosT   = trdHit->GetTime();
    const double pointPosT = trdPoint->GetTime();
    const double hitResT   = hitPosT - pointPosT;
    const double hitPullT  = hitResT / trdHit->GetTimeError();

    // fill histograms
    fvhHitPullX[planeId]->Fill(hitPullX);
    fvhHitPullY[planeId]->Fill(hitPullY);
    fvhHitPullT[planeId]->Fill(hitPullT);

    fvhHitResX[planeId]->Fill(hitResX);
    fvhHitResY[planeId]->Fill(hitResY);
    fvhHitResT[planeId]->Fill(hitResT);
  }
}
// --------------------------------------------------------------------------

// ---- Finish --------------------------------------------------------------
void CbmTrdHitProducerQa::Finish() { WriteHistograms(); }
// --------------------------------------------------------------------------

// ---- Write test histograms ------------------------------------------------
void CbmTrdHitProducerQa::WriteHistograms()
{
  for (size_t i = 0; i < fvdECanvas.size(); i++) {
    fvdECanvas[i]->cd(1);
    fvhedEcut[i]->DrawCopy("", "");

    fvdECanvas[i]->cd(2);
    fvhedEall[i]->DrawCopy("", "");

    fvdECanvas[i]->cd(3);
    fvhpidEcut[i]->DrawCopy("", "");

    fvdECanvas[i]->cd(4);
    fvhpidEall[i]->DrawCopy("", "");
  }

  for (size_t i = 0; i < fvPullCanvas.size(); i++) {
    fvPullCanvas[i]->cd(1);
    fvhHitPullX[i]->DrawCopy("", "");

    fvPullCanvas[i]->cd(2);
    fvhHitPullY[i]->DrawCopy("", "");

    fvPullCanvas[i]->cd(3);
    fvhHitPullT[i]->DrawCopy("", "");

    fvPullCanvas[i]->cd(4);
    fvhHitResX[i]->DrawCopy("", "");

    fvPullCanvas[i]->cd(5);
    fvhHitResY[i]->DrawCopy("", "");

    fvPullCanvas[i]->cd(6);
    fvhHitResT[i]->DrawCopy("", "");
  }

  FairSink* sink = FairRootManager::Instance()->GetSink();
  sink->WriteObject(&fOutFolder, nullptr);
}
// --------------------------------------------------------------------------

ClassImp(CbmTrdHitProducerQa);
