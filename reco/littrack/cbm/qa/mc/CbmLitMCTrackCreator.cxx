/* Copyright (C) 2011-2020 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer], Timur Ablyazimov */

/**
 * \file CbmLitMCTrackCreator.cxx
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2011
 **/
#include "CbmLitMCTrackCreator.h"

#include "CbmDigiManager.h"
#include "CbmHit.h"
#include "CbmMCDataArray.h"
#include "CbmMCDataManager.h"
#include "CbmMCTrack.h"
#include "CbmMatchRecoToMC.h"
#include "CbmMuchGeoScheme.h"
#include "CbmMuchPoint.h"
#include "CbmMvdPoint.h"
#include "CbmRichGeoManager.h"
#include "CbmRichHit.h"
#include "CbmRichHitProducer.h"
#include "CbmRichPoint.h"
#include "CbmRichRingFitterEllipseTau.h"
#include "CbmStsPoint.h"
#include "CbmStsSetup.h"
#include "CbmTrdAddress.h"
#include "CbmTrdPoint.h"

#include "FairGeoNode.h"
#include "FairMCPoint.h"
#include "FairRootManager.h"

#include "TClonesArray.h"
#include "TDatabasePDG.h"
#include "TGeoManager.h"

#include <map>      // for map
#include <utility>  // for make_pair, pair

using std::make_pair;
using std::map;
using std::pair;

CbmLitMCTrackCreator::CbmLitMCTrackCreator()
  : fMCTracks(NULL)
  , fMvdPoints(NULL)
  , fStsPoints(NULL)
  , fTrdPoints(NULL)
  , fMuchPoints(NULL)
  , fTofPoints(NULL)
  , fRichPoints(NULL)
  , fRichHits(NULL)
  , fDigiMan(nullptr)
  , fLitMCTracks()
  , fMvdStationsMap()
  , fStsStationsMap()
  , fTrdStationsMap()
  , fMuchStationsMap()
  , fTauFit(NULL)
{
  ReadDataBranches();

  fTauFit = new CbmRichRingFitterEllipseTau();
}

CbmLitMCTrackCreator::~CbmLitMCTrackCreator() {}

CbmLitMCTrackCreator* CbmLitMCTrackCreator::Instance()
{
  static CbmLitMCTrackCreator instance;
  return &instance;
}

void CbmLitMCTrackCreator::Create(Int_t eventNum)
{
  FillStationMaps(eventNum);


  fLitMCTracks.clear();

  AddPoints(ECbmModuleId::kMvd, fMvdPoints, eventNum);

  AddPoints(ECbmModuleId::kSts, fStsPoints, eventNum);

  AddPoints(ECbmModuleId::kTrd, fTrdPoints, eventNum);

  AddPoints(ECbmModuleId::kMuch, fMuchPoints, eventNum);

  AddPoints(ECbmModuleId::kTof, fTofPoints, eventNum);

  AddPoints(ECbmModuleId::kRich, fRichPoints, eventNum);

  AddRichHits(eventNum);

  AddRingParameters(eventNum);

  std::map<std::pair<Int_t, Int_t>, CbmLitMCTrack>::iterator it;
  for (it = fLitMCTracks.begin(); it != fLitMCTracks.end(); it++) {
    it->second.CalculateNofConsecutivePoints();
  }

  //    std::cout << "CbmLitMCTrackCreator: nof MC tracks=" << fLitMCTracks.size() << std::endl;
  //    for (it = fLitMCTracks.begin(); it != fLitMCTracks.end(); it++){
  //        std::cout << (*it).first.first  << "-" << (*it).first.second<< " => " << (*it).second.ToString();
  //    }
}


void CbmLitMCTrackCreator::ReadDataBranches()
{
  FairRootManager* ioman = FairRootManager::Instance();

  CbmMCDataManager* mcManager = (CbmMCDataManager*) ioman->GetObject("MCDataManager");

  if (0 == mcManager) LOG(fatal) << "CbmMatchRecoToMC::ReadAndCreateDataBranches() NULL MCDataManager.";

  fMCTracks   = mcManager->InitBranch("MCTrack");
  fMvdPoints  = mcManager->InitBranch("MvdPoint");
  fStsPoints  = mcManager->InitBranch("StsPoint");
  fTrdPoints  = mcManager->InitBranch("TrdPoint");
  fMuchPoints = mcManager->InitBranch("MuchPoint");
  fTofPoints  = mcManager->InitBranch("TofPoint");
  fRichPoints = mcManager->InitBranch("RichPoint");
  fRichHits   = (TClonesArray*) ioman->GetObject("RichHit");
  fDigiMan    = CbmDigiManager::Instance();
  fDigiMan->Init();
}

void CbmLitMCTrackCreator::AddPoints(ECbmModuleId detId, CbmMCDataArray* array, Int_t iEvent)
{
  if (array == NULL) return;

  Int_t nofPoints = array->Size(0, iEvent);

  for (Int_t iPoint = 0; iPoint < nofPoints; ++iPoint) {
    FairMCPoint* fairPoint = static_cast<FairMCPoint*>(array->Get(0, iEvent, iPoint));
    if (NULL == fairPoint) continue;
    CbmLitMCPoint litPoint;
    Int_t stationId = -1;
    if (detId == ECbmModuleId::kMvd) {
      stationId = fMvdStationsMap[make_pair(iEvent, iPoint)];
      MvdPointCoordinatesAndMomentumToLitMCPoint(static_cast<CbmMvdPoint*>(fairPoint), &litPoint);
    }
    else if (detId == ECbmModuleId::kSts) {
      stationId = fStsStationsMap[make_pair(iEvent, iPoint)];
      StsPointCoordinatesAndMomentumToLitMCPoint(static_cast<CbmStsPoint*>(fairPoint), &litPoint);
    }
    else if (detId == ECbmModuleId::kTrd) {
      stationId = fTrdStationsMap[make_pair(iEvent, iPoint)];
      TrdPointCoordinatesAndMomentumToLitMCPoint(static_cast<CbmTrdPoint*>(fairPoint), &litPoint);
    }
    else if (detId == ECbmModuleId::kMuch) {
      stationId = fMuchStationsMap[make_pair(iEvent, iPoint)];
      MuchPointCoordinatesAndMomentumToLitMCPoint(static_cast<CbmMuchPoint*>(fairPoint), &litPoint);
    }
    else if (detId == ECbmModuleId::kTof) {
      stationId = 0;
      FairMCPointCoordinatesAndMomentumToLitMCPoint(fairPoint, &litPoint);
    }
    else if (detId == ECbmModuleId::kRich) {
      stationId = 0;
      FairMCPointCoordinatesAndMomentumToLitMCPoint(fairPoint, &litPoint);
    }
    if (stationId < 0) continue;
    FairMCPointToLitMCPoint(fairPoint, &litPoint, iEvent, iPoint, stationId);
    if (detId != ECbmModuleId::kRich) {
      fLitMCTracks[make_pair(iEvent, fairPoint->GetTrackID())].AddPoint(detId, litPoint);
    }
    else {
      const CbmMCTrack* mcTrack = static_cast<const CbmMCTrack*>(fMCTracks->Get(0, iEvent, fairPoint->GetTrackID()));
      fLitMCTracks[make_pair(iEvent, mcTrack->GetMotherId())].AddPoint(detId, litPoint);
    }
  }
}

void CbmLitMCTrackCreator::AddRichHits(Int_t iEvent)
{
  if (NULL == fRichHits) return;
  map<pair<Int_t, Int_t>, Int_t> nofHitsInRing;
  Int_t nofRichHits = fRichHits->GetEntriesFast();
  for (Int_t iHit = 0; iHit < nofRichHits; iHit++) {
    const CbmRichHit* hit = static_cast<const CbmRichHit*>(fRichHits->At(iHit));
    if (NULL == hit) continue;
    vector<pair<Int_t, Int_t>> motherIds =
      CbmMatchRecoToMC::GetMcTrackMotherIdsForRichHit(fDigiMan, hit, fRichPoints, fMCTracks, iEvent);
    for (UInt_t i = 0; i < motherIds.size(); i++) {
      nofHitsInRing[motherIds[i]]++;
    }
  }

  map<pair<Int_t, Int_t>, Int_t>::const_iterator it;
  for (it = nofHitsInRing.begin(); it != nofHitsInRing.end(); it++) {
    fLitMCTracks[it->first].SetNofRichHits(it->second);
  }
}

void CbmLitMCTrackCreator::AddRingParameters(Int_t iEvent)
{
  if (NULL == fRichPoints || NULL == fMCTracks) return;
  map<pair<Int_t, Int_t>, CbmRichRingLight> mapRings;

  int nofRichPoints = fRichPoints->Size(0, iEvent);

  for (int iPoint = 0; iPoint < nofRichPoints; iPoint++) {
    CbmRichPoint* richPoint = (CbmRichPoint*) fRichPoints->Get(0, iEvent, iPoint);
    if (NULL == richPoint) continue;
    Int_t trackId = richPoint->GetTrackID();
    if (trackId < 0) continue;
    CbmMCTrack* mcTrackRich = (CbmMCTrack*) fMCTracks->Get(0, iEvent, trackId);
    if (NULL == mcTrackRich) continue;
    int motherIdRich = mcTrackRich->GetMotherId();
    if (motherIdRich == -1) continue;
    TVector3 posPoint;
    richPoint->Position(posPoint);
    TVector3 detPoint;

    CbmRichGeoManager::GetInstance().RotatePoint(&posPoint, &detPoint);
    CbmRichHitLight hit(detPoint.X(), detPoint.Y());
    mapRings[make_pair(iEvent, motherIdRich)].AddHit(hit);
  }


  map<pair<Int_t, Int_t>, CbmRichRingLight>::const_iterator it;
  for (it = mapRings.begin(); it != mapRings.end(); it++) {
    CbmRichRingLight ring(it->second);
    fTauFit->DoFit(&ring);  //fLitMCTracks[it->first].SetNofRichHits(it->second);
    fLitMCTracks[it->first].SetRingAaxis(ring.GetAaxis());
    fLitMCTracks[it->first].SetRingBaxis(ring.GetBaxis());
    fLitMCTracks[it->first].SetRingCenterX(ring.GetCenterX());
    fLitMCTracks[it->first].SetRingCenterY(ring.GetCenterY());
    //std::cout << ++i << " " << ring.GetAaxis() << " " << ring.GetBaxis() << std::endl;
  }
}

void CbmLitMCTrackCreator::FairMCPointToLitMCPoint(const FairMCPoint* fairPoint, CbmLitMCPoint* litPoint, Int_t eventId,
                                                   Int_t refId, Int_t stationId)
{
  if (fairPoint == NULL || litPoint == NULL) return;
  litPoint->SetRefId(refId);
  litPoint->SetStationId(stationId);
  const CbmMCTrack* mcTrack = static_cast<const CbmMCTrack*>(fMCTracks->Get(0, eventId, fairPoint->GetTrackID()));
  if (mcTrack == NULL) return;
  TParticlePDG* pdgParticle = TDatabasePDG::Instance()->GetParticle(mcTrack->GetPdgCode());
  double charge             = (pdgParticle != NULL) ? pdgParticle->Charge() : 0.;
  Double_t q                = (charge > 0) ? 1. : -1.;
  litPoint->SetQ(q);
}

void CbmLitMCTrackCreator::FairMCPointCoordinatesAndMomentumToLitMCPoint(const FairMCPoint* fairPoint,
                                                                         CbmLitMCPoint* litPoint)
{
  if (fairPoint == NULL || litPoint == NULL) return;
  litPoint->SetXIn(fairPoint->GetX());
  litPoint->SetYIn(fairPoint->GetY());
  litPoint->SetZIn(fairPoint->GetZ());
  litPoint->SetPxIn(fairPoint->GetPx());
  litPoint->SetPyIn(fairPoint->GetPy());
  litPoint->SetPzIn(fairPoint->GetPz());
  litPoint->SetXOut(fairPoint->GetX());
  litPoint->SetYOut(fairPoint->GetY());
  litPoint->SetZOut(fairPoint->GetZ());
  litPoint->SetPxOut(fairPoint->GetPx());
  litPoint->SetPyOut(fairPoint->GetPy());
  litPoint->SetPzOut(fairPoint->GetPz());
}

void CbmLitMCTrackCreator::MvdPointCoordinatesAndMomentumToLitMCPoint(const CbmMvdPoint* mvdPoint,
                                                                      CbmLitMCPoint* litPoint)
{
  if (mvdPoint == NULL || litPoint == NULL) return;
  litPoint->SetXIn(mvdPoint->GetX());
  litPoint->SetYIn(mvdPoint->GetY());
  litPoint->SetZIn(mvdPoint->GetZ());
  litPoint->SetPxIn(mvdPoint->GetPx());
  litPoint->SetPyIn(mvdPoint->GetPy());
  litPoint->SetPzIn(mvdPoint->GetPz());
  litPoint->SetXOut(mvdPoint->GetXOut());
  litPoint->SetYOut(mvdPoint->GetYOut());
  litPoint->SetZOut(mvdPoint->GetZOut());
  litPoint->SetPxOut(mvdPoint->GetPxOut());
  litPoint->SetPyOut(mvdPoint->GetPyOut());
  litPoint->SetPzOut(mvdPoint->GetPzOut());
}

void CbmLitMCTrackCreator::StsPointCoordinatesAndMomentumToLitMCPoint(const CbmStsPoint* stsPoint,
                                                                      CbmLitMCPoint* litPoint)
{
  if (stsPoint == NULL || litPoint == NULL) return;
  litPoint->SetXIn(stsPoint->GetXIn());
  litPoint->SetYIn(stsPoint->GetYIn());
  litPoint->SetZIn(stsPoint->GetZIn());
  litPoint->SetPxIn(stsPoint->GetPx());
  litPoint->SetPyIn(stsPoint->GetPy());
  litPoint->SetPzIn(stsPoint->GetPz());
  litPoint->SetXOut(stsPoint->GetXOut());
  litPoint->SetYOut(stsPoint->GetYOut());
  litPoint->SetZOut(stsPoint->GetZOut());
  litPoint->SetPxOut(stsPoint->GetPxOut());
  litPoint->SetPyOut(stsPoint->GetPyOut());
  litPoint->SetPzOut(stsPoint->GetPzOut());
}

void CbmLitMCTrackCreator::TrdPointCoordinatesAndMomentumToLitMCPoint(const CbmTrdPoint* trdPoint,
                                                                      CbmLitMCPoint* litPoint)
{
  if (trdPoint == NULL || litPoint == NULL) return;
  litPoint->SetXIn(trdPoint->GetXIn());
  litPoint->SetYIn(trdPoint->GetYIn());
  litPoint->SetZIn(trdPoint->GetZIn());
  litPoint->SetPxIn(trdPoint->GetPxIn());
  litPoint->SetPyIn(trdPoint->GetPyIn());
  litPoint->SetPzIn(trdPoint->GetPzIn());
  litPoint->SetXOut(trdPoint->GetXOut());
  litPoint->SetYOut(trdPoint->GetYOut());
  litPoint->SetZOut(trdPoint->GetZOut());
  litPoint->SetPxOut(trdPoint->GetPxOut());
  litPoint->SetPyOut(trdPoint->GetPyOut());
  litPoint->SetPzOut(trdPoint->GetPzOut());
}

void CbmLitMCTrackCreator::MuchPointCoordinatesAndMomentumToLitMCPoint(const CbmMuchPoint* muchPoint,
                                                                       CbmLitMCPoint* litPoint)
{
  if (muchPoint == NULL || litPoint == NULL) return;
  litPoint->SetXIn(muchPoint->GetXIn());
  litPoint->SetYIn(muchPoint->GetYIn());
  litPoint->SetZIn(muchPoint->GetZIn());
  litPoint->SetPxIn(muchPoint->GetPx());
  litPoint->SetPyIn(muchPoint->GetPy());
  litPoint->SetPzIn(muchPoint->GetPz());
  litPoint->SetXOut(muchPoint->GetXOut());
  litPoint->SetYOut(muchPoint->GetYOut());
  litPoint->SetZOut(muchPoint->GetZOut());
  litPoint->SetPxOut(muchPoint->GetPxOut());
  litPoint->SetPyOut(muchPoint->GetPyOut());
  litPoint->SetPzOut(muchPoint->GetPzOut());
}

void CbmLitMCTrackCreator::FillStationMaps(Int_t iEvent)
{
  fMvdStationsMap.clear();
  fStsStationsMap.clear();
  fTrdStationsMap.clear();
  fMuchStationsMap.clear();


  // MVD
  if (NULL != fMvdPoints) {
    Int_t nofMvdPoints = fMvdPoints->Size(0, iEvent);
    for (Int_t iPoint = 0; iPoint < nofMvdPoints; iPoint++) {
      CbmMvdPoint* point = static_cast<CbmMvdPoint*>(fMvdPoints->Get(0, iEvent, iPoint));
      if (NULL == point) continue;
      fMvdStationsMap[make_pair(iEvent, iPoint)] = point->GetStationNr() - 1;
    }
  }
  // end MVD

  // STS
  if (NULL != fStsPoints) {
    Int_t nofStsPoints = fStsPoints->Size(0, iEvent);
    for (Int_t iPoint = 0; iPoint < nofStsPoints; iPoint++) {
      const CbmStsPoint* point = static_cast<const CbmStsPoint*>(fStsPoints->Get(0, iEvent, iPoint));
      if (NULL == point) continue;
      UInt_t address                             = point->GetDetectorID();
      Int_t stationId                            = CbmStsSetup::Instance()->GetStationNumber(address);
      fStsStationsMap[make_pair(iEvent, iPoint)] = stationId;
    }
  }
  // end STS

  // MUCH
  if (NULL != fMuchPoints) {
    Int_t nofMuchPoints = fMuchPoints->Size(0, iEvent);
    for (Int_t iPoint = 0; iPoint < nofMuchPoints; iPoint++) {
      const FairMCPoint* point = static_cast<const FairMCPoint*>(fMuchPoints->Get(0, iEvent, iPoint));
      if (NULL == point) continue;
      Int_t stationId = 100 * CbmMuchGeoScheme::GetStationIndex(point->GetDetectorID())
                        + 10 * CbmMuchGeoScheme::GetLayerIndex(point->GetDetectorID())
                        + CbmMuchGeoScheme::GetLayerSideIndex(point->GetDetectorID());
      //         Int_t stationId = CbmMuchGeoScheme::Instance()->GetLayerSideNr(point->GetDetectorID());
      fMuchStationsMap[make_pair(iEvent, iPoint)] = stationId;
    }
  }
  // end MUCH

  // TRD
  if (NULL != fTrdPoints) {
    Int_t nofTrdPoints = fTrdPoints->Size(0, iEvent);
    for (Int_t iPoint = 0; iPoint < nofTrdPoints; iPoint++) {
      const FairMCPoint* point = static_cast<const FairMCPoint*>(fTrdPoints->Get(0, iEvent, iPoint));
      if (NULL == point) continue;
      //Int_t stationId = 10 * CbmTrdAddress::GetStationNr(point->GetDetectorID()) + CbmTrdAddress::GetLayerNr(point->GetDetectorID());
      Int_t stationId                            = CbmTrdAddress::GetLayerId(point->GetDetectorID());
      fTrdStationsMap[make_pair(iEvent, iPoint)] = stationId;
    }
  }
  // end TRD
}
