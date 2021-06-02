/** @file CbmRichHitProducer.cxx
  * @copyright Copyright (C) 2004-2021 GSI/JINR-LIT
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors B. Polichtchouk, Andrey Lebedev [committer], Volker Friese **/

/**
 * \file CbmRichHitProducer.cxx
 *
 * \author B. Polichtchouk
 * \date 2004
 **/

#include "CbmRichHitProducer.h"

#include "CbmDigiManager.h"
#include "CbmEvent.h"
#include "CbmRichDetectorData.h"  // for CbmRichPmtData, CbmRichPixelData
#include "CbmRichDigi.h"
#include "CbmRichDigiMapManager.h"
#include "CbmRichGeoManager.h"
#include "CbmRichHit.h"
#include "CbmRichPoint.h"

#include <Logger.h>

#include "TClonesArray.h"
#include <TStopwatch.h>

#include <iomanip>
#include <iostream>

using namespace std;


CbmRichHitProducer::CbmRichHitProducer() : FairTask("CbmRichHitProducer") {}

CbmRichHitProducer::~CbmRichHitProducer()
{
  FairRootManager* manager = FairRootManager::Instance();
  manager->Write();
}

void CbmRichHitProducer::SetParContainers() {}

InitStatus CbmRichHitProducer::Init()
{
  FairRootManager* manager = FairRootManager::Instance();

  fCbmEvents = dynamic_cast<TClonesArray*>(manager->GetObject("CbmEvent"));
  if (fCbmEvents == nullptr) { LOG(info) << ": CbmEvent NOT found \n \n \n"; }
  else {
    LOG(info) << ": CbmEvent found \n \n \n";
  }

  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->Init();
  if (!fDigiMan->IsPresent(ECbmModuleId::kRich)) { Fatal("CbmRichHitProducer::Init", "No RichDigi array!"); }

  fRichHits = new TClonesArray("CbmRichHit");
  manager->Register("RichHit", "RICH", fRichHits, IsOutputBranchPersistent("RichHit"));

  CbmRichDigiMapManager::GetInstance();

  return kSUCCESS;
}

void CbmRichHitProducer::Exec(Option_t* /*option*/)
{

  TStopwatch timer;
  timer.Start();
  Int_t nDigisAll  = fDigiMan->GetNofDigis(ECbmModuleId::kRich);
  Int_t nDigisUsed = 0;
  Int_t nEvents    = 0;
  Int_t result     = 0;
  fRichHits->Delete();

  // Time-slice processing
  if (fCbmEvents == nullptr) nDigisUsed = ProcessData(nullptr);

  // Event processing
  else {
    nEvents = fCbmEvents->GetEntriesFast();
    for (Int_t iEvent = 0; iEvent < nEvents; iEvent++) {
      CbmEvent* event = static_cast<CbmEvent*>(fCbmEvents->At(iEvent));
      result          = ProcessData(event);
      nDigisUsed += result;
    }
  }

  timer.Stop();
  stringstream logOut;
  logOut << setw(20) << left << GetName() << " [";
  logOut << fixed << setw(8) << setprecision(1) << right << timer.RealTime() * 1000. << " ms] ";
  logOut << "TS " << fNofTs;
  if (fCbmEvents != nullptr) logOut << ", events " << nEvents;
  logOut << ", digis " << nDigisUsed << " / " << nDigisAll;
  LOG(info) << logOut.str();
  fNofTs++;
  fNofDigisAll += nDigisAll;
  fNofDigisUsed += nDigisUsed;
  fTime += timer.RealTime();
}

Int_t CbmRichHitProducer::ProcessData(CbmEvent* event)
{
  Int_t nDigis = 0;
  if (event != NULL) {
    Int_t nofDigis = event->GetNofData(ECbmDataType::kRichDigi);
    LOG(debug) << GetName() << ": Event mode. Event # " << event->GetNumber() << ", digis: " << nofDigis;
    for (Int_t iDigi = 0; iDigi < nofDigis; iDigi++) {
      Int_t digiIndex = event->GetIndex(ECbmDataType::kRichDigi, iDigi);
      ProcessDigi(event, digiIndex);
    }
    nDigis = nofDigis;
  }
  else {
    nDigis = fDigiMan->GetNofDigis(ECbmModuleId::kRich);
    for (Int_t iDigi = 0; iDigi < fDigiMan->GetNofDigis(ECbmModuleId::kRich); iDigi++) {
      ProcessDigi(event, iDigi);
    }
  }
  return nDigis;
}

void CbmRichHitProducer::ProcessDigi(CbmEvent* event, Int_t digiIndex)
{
  const CbmRichDigi* digi = fDigiMan->Get<CbmRichDigi>(digiIndex);
  if (digi == nullptr) return;
  if (digi->GetAddress() < 0) return;
  CbmRichPixelData* data = CbmRichDigiMapManager::GetInstance().GetPixelDataByAddress(digi->GetAddress());
  TVector3 posPoint;
  posPoint.SetXYZ(data->fX, data->fY, data->fZ);
  TVector3 detPoint;

  CbmRichGeoManager::GetInstance().RotatePoint(&posPoint, &detPoint, !fRotationNeeded);
  AddHit(event, detPoint, digi->GetTime(), digiIndex);
}


void CbmRichHitProducer::AddHit(CbmEvent* event, TVector3& posHit, Double_t time, Int_t index)
{
  Int_t nofHits = fRichHits->GetEntriesFast();
  new ((*fRichHits)[nofHits]) CbmRichHit();
  CbmRichHit* hit = (CbmRichHit*) fRichHits->At(nofHits);
  hit->SetPosition(posHit);
  hit->SetDx(fHitError);
  hit->SetDy(fHitError);
  hit->SetRefId(index);
  hit->SetTime(time);

  if (event != NULL) { event->AddData(ECbmDataType::kRichHit, nofHits); }
}

void CbmRichHitProducer::Finish()
{
  fRichHits->Clear();
  std::cout << std::endl;
  LOG(info) << "=====================================";
  LOG(info) << GetName() << ": Run summary";
  LOG(info) << "Time slices     : " << fNofTs;
  if (fCbmEvents) LOG(info) << "Events          : " << fNofEvents;
  LOG(info) << "Digis      / TS : " << fixed << setprecision(2) << Double_t(fNofDigisAll) / Double_t(fNofTs);
  LOG(info) << "Used digis / TS : " << fixed << setprecision(2) << Double_t(fNofDigisUsed) / Double_t(fNofTs);
  LOG(info) << "Time       / TS : " << fixed << setprecision(2) << 1000. * fTime / Double_t(fNofTs) << " ms";
  LOG(info) << "=====================================\n";
}


ClassImp(CbmRichHitProducer)
