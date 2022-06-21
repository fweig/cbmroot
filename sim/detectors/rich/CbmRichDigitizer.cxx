/* Copyright (C) 2015-2020 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer], Volker Friese, Florian Uhlig, Semen Lebedev */

/**
* \file CbmRichDigitizer.cxx
*
* \author S.Lebedev
* \date 2015
**/

#include "CbmRichDigitizer.h"

#include "CbmLink.h"
#include "CbmMCTrack.h"
#include "CbmMatch.h"
#include "CbmRichDetectorData.h"  // for CbmRichPmtData, CbmRichPixelData
#include "CbmRichDigi.h"
#include "CbmRichDigiMapManager.h"
#include "CbmRichGeoManager.h"
#include "CbmRichPoint.h"

#include "FairEventHeader.h"
#include "FairRunAna.h"
#include "FairRunSim.h"
#include <Logger.h>

#include "TClonesArray.h"
#include "TGeoManager.h"
#include "TRandom.h"
#include "TStopwatch.h"

#include <cassert>
#include <iomanip>
#include <iostream>

using namespace std;

CbmRichDigitizer::CbmRichDigitizer()
  : CbmDigitize<CbmRichDigi>("RichDigitize")
  , fEventNum(0)
  , fRichPoints(NULL)
  , fRichDigis(NULL)
  , fRichDigiMatches(nullptr)
  , fMcTracks(NULL)
  , fNofPoints(0.)
  , fNofDigis(0.)
  , fTimeTot(0.)
  , fPmt()
  , fCrossTalkProbability(0.02)
  , fNoiseDigiRate(5.)
  , fDetectorType(CbmRichPmtTypeCosy17NoWls)
  , fMaxNofHitsPerPmtCut(65)
  , fDataMap()
  , fTimeResolution(1.)
  , fDarkRatePerPixel(1000)
  , fPixelDeadTime(30.)
  , fFiredPixelsMap()
  , fDoZShift(true)
{
}

CbmRichDigitizer::~CbmRichDigitizer() {}


InitStatus CbmRichDigitizer::Init()
{

  // Screen output
  std::cout << std::endl;
  LOG(info) << "==========================================================";
  LOG(info) << GetName() << ": Initialisation";

  //FairTask* daq = FairRun::Instance()->GetTask("Daq");
  if (!fEventMode) { LOG(info) << "CbmRichDigitizer uses TimeBased mode."; }
  else {
    LOG(info) << "CbmRichDigitizer uses Events mode.";
  }

  FairRootManager* manager = FairRootManager::Instance();

  // --- Initialise helper singletons in order not to do it in event
  // --- processing (spoils timing measurement)
  CbmRichDigiMapManager::GetInstance();
  CbmRichGeoManager::GetInstance();

  fRichPoints = (TClonesArray*) manager->GetObject("RichPoint");
  if (NULL == fRichPoints) { Fatal("CbmRichDigitizer::Init", "No RichPoint array!"); }

  fMcTracks = (TClonesArray*) manager->GetObject("MCTrack");
  if (NULL == fMcTracks) { Fatal("CbmRichDigitizer::Init", "No MCTrack array!"); }

  RegisterOutput();


  LOG(info) << GetName() << ": Initialisation successful";

  fFiredPixelsMap.clear();

  return kSUCCESS;
}

void CbmRichDigitizer::Exec(Option_t* /*option*/)
{
  TStopwatch timer;
  timer.Start();

  fEventNum++;
  LOG(debug) << fName << ": Event  " << fEventNum;


  // Clear data map
  for (auto& data : fDataMap) {
    if (data.second.first) delete data.second.first;    // digi
    if (data.second.second) delete data.second.second;  // match
  }
  fDataMap.clear();

  Double_t oldEventTime = fCurrentEventTime;
  GetEventInfo();

  LOG(debug) << fName << ": EventNumber: " << fCurrentEvent << ", EventTime: " << fCurrentEventTime;

  if (fProduceNoise) GenerateNoiseBetweenEvents(oldEventTime, fCurrentEventTime);

  Int_t nPoints = ProcessMcEvent();

  Int_t nDigis = AddDigisToOutputArray();


  // --- Statistics
  timer.Stop();
  fNofPoints += nPoints;
  fNofDigis += nDigis;
  fTimeTot += timer.RealTime();

  // --- Event log
  LOG(info) << "+ " << setw(15) << GetName() << ": Event " << setw(6) << right << fCurrentEvent << " at " << fixed
            << setprecision(3) << fCurrentEventTime << " ns, points: " << nPoints << ", digis: " << nDigis
            << ". Exec time " << setprecision(6) << timer.RealTime() << " s.";
}

Int_t CbmRichDigitizer::ProcessMcEvent()
{
  Int_t nofRichPoints = fRichPoints->GetEntriesFast();
  LOG(debug) << fName << ": EventNum:" << fCurrentEvent << " InputNum:" << fCurrentInput
             << " EventTime:" << fCurrentEventTime << " nofRichPoints:" << nofRichPoints;

  for (Int_t j = 0; j < nofRichPoints; j++) {
    CbmRichPoint* point = (CbmRichPoint*) fRichPoints->At(j);
    ProcessPoint(point, j, fCurrentMCEntry, fCurrentInput);
  }
  // cout << "nofDigis:" << fRichDigis->GetEntriesFast() << endl;

  AddNoiseDigis(fCurrentMCEntry, fCurrentInput);

  return nofRichPoints;
}

void CbmRichDigitizer::GenerateNoiseBetweenEvents(Double_t oldEventTime, Double_t newEventTime)
{
  Int_t nofPixels         = CbmRichDigiMapManager::GetInstance().GetPixelAddresses().size();
  Double_t dT             = newEventTime - oldEventTime;
  Double_t nofNoisePixels = nofPixels * dT * (fDarkRatePerPixel / 1.e9);

  LOG(debug) << "CbmRichDigitizer::GenerateNoiseBetweenEvents deltaTime:" << dT << " nofPixels:" << nofPixels
             << " nofNoisePixels:" << nofNoisePixels;

  for (Int_t j = 0; j < nofNoisePixels; j++) {
    Int_t address = CbmRichDigiMapManager::GetInstance().GetRandomPixelAddress();
    CbmLink link(1., -1, -1, -1);
    Double_t time = gRandom->Uniform(oldEventTime, newEventTime);
    AddDigi(address, time, link);
  }
}

void CbmRichDigitizer::ProcessPoint(CbmRichPoint* point, Int_t pointId, Int_t eventNum, Int_t inputNum)
{
  // LOG(info) << "ProcessPoint " << pointId;
  //	TGeoNode* node = gGeoManager->FindNode(point->GetX(), point->GetY(), point->GetZ());
  // workaround for GEANT4, probably boundary problem
  Double_t zNew = point->GetZ();
  if (fDoZShift) zNew = zNew - 0.001;
  gGeoManager->FindNode(point->GetX(), point->GetY(), zNew);
  string path(gGeoManager->GetPath());
  Int_t address = CbmRichDigiMapManager::GetInstance().GetPixelAddressByPath(path);

  Int_t trackId = point->GetTrackID();
  //LOG(info) << "address:" << address << " path:" << path << " trackId:" << trackId << "X:" << point->GetX() << " Y:" << point->GetY() << " Z:" << zNew;
  if (trackId < 0) return;
  CbmMCTrack* p = (CbmMCTrack*) fMcTracks->At(trackId);
  if (p == NULL) return;
  Int_t gcode = TMath::Abs(p->GetPdgCode());

  CbmLink link(1., pointId, eventNum, inputNum);

  Bool_t isDetected = false;
  // for photons weight with efficiency of PMT
  if (gcode == 50000050) {
    TVector3 mom;
    point->Momentum(mom);
    Double_t momTotal = sqrt(mom.Px() * mom.Px() + mom.Py() * mom.Py() + mom.Pz() * mom.Pz());
    isDetected        = fPmt.isPhotonDetected(fDetectorType, momTotal);
  }
  else {  // if not photon
    // worst case: assume that all charged particles crossing
    // the PMTplane leave Cherenkov light in the PMTglass which will be detected
    isDetected = true;
  }

  Double_t time   = fCurrentEventTime + point->GetTime();
  Double_t deltaT = gRandom->Gaus(0., fTimeResolution);
  time += deltaT;
  if (isDetected) { AddDigi(address, time, link); }
}


void CbmRichDigitizer::Finish()
{
  std::cout << std::endl;
  LOG(info) << "=====================================";
  LOG(info) << GetName() << ": Run summary";
  LOG(info) << "Events processed    : " << fEventNum;
  LOG(info) << "RichPoint / event   : " << setprecision(1) << fNofPoints / Double_t(fEventNum);
  LOG(info) << "RichDigi / event    : " << fNofDigis / Double_t(fEventNum);
  LOG(info) << "Digis per point     : " << setprecision(6) << fNofDigis / fNofPoints;
  LOG(info) << "Real time per event : " << fTimeTot / Double_t(fEventNum) << " s";
  LOG(info) << "=====================================";
}

void CbmRichDigitizer::AddCrossTalkDigis(CbmRichDigi* digi)
{
  Bool_t crosstalkDigiDetected = false;
  vector<Int_t> directPixels   = CbmRichDigiMapManager::GetInstance().GetDirectNeighbourPixels(digi->GetAddress());
  vector<Int_t> diagonalPixels = CbmRichDigiMapManager::GetInstance().GetDiagonalNeighbourPixels(digi->GetAddress());

  for (UInt_t i = 0; i < directPixels.size(); i++) {
    if (gRandom->Rndm() < fCrossTalkProbability && !crosstalkDigiDetected) {
      //FindRichHitPositionMAPMT(0., x + r, y, xHit, yHit, pmtID);
      crosstalkDigiDetected = true;
      break;
    }
  }

  if (!crosstalkDigiDetected) {
    for (UInt_t i = 0; i < diagonalPixels.size(); i++) {
      if (gRandom->Rndm() < fCrossTalkProbability / 4. && !crosstalkDigiDetected) {
        //FindRichHitPositionMAPMT(0., x + r, y, xHit, yHit, pmtID);
        crosstalkDigiDetected = true;
        break;
      }
    }
  }

  if (crosstalkDigiDetected) {
    //AddDigi(posHit, posHitErr, RichDetID, pmtID, ampl, pointInd);
    //fNofCrossTalkDigis++;
  }
}

Int_t CbmRichDigitizer::AddDigisToOutputArray()
{
  Int_t nofDigis = 0;

  // fMaxNofHitsPerPmtCut is a maximum number of hits which can be registered per PMT per event.
  // If more then the whole PMT is skipped
  map<Int_t, Int_t> digisPerPmtMap;
  for (auto const& dm : fDataMap) {
    CbmRichPixelData* pixelData =
      CbmRichDigiMapManager::GetInstance().GetPixelDataByAddress(dm.second.first->GetAddress());
    if (nullptr == pixelData) continue;
    Int_t pmtId = pixelData->fPmtId;
    digisPerPmtMap[pmtId]++;
  }

  Int_t nofSkippedPmts = 0;
  for (auto const& dm : digisPerPmtMap) {
    if (dm.second > fMaxNofHitsPerPmtCut) nofSkippedPmts++;
  }

  Int_t nofSkippedDigis = 0;
  for (auto const& dm : fDataMap) {
    CbmRichPixelData* pixelData =
      CbmRichDigiMapManager::GetInstance().GetPixelDataByAddress(dm.second.first->GetAddress());
    if (nullptr == pixelData) continue;
    if (digisPerPmtMap[pixelData->fPmtId] > fMaxNofHitsPerPmtCut) {
      nofSkippedDigis++;
      continue;
    }

    CbmRichDigi* digi = new CbmRichDigi();
    digi->SetAddress(dm.second.first->GetAddress());
    CbmMatch* digiMatch = new CbmMatch(*dm.second.second);
    digi->SetTime(dm.second.first->GetTime());
    //SendDigi(digi, digiMatch);
    if (fCreateMatches) SendData(digi->GetTime(), digi, digiMatch);
    else
      SendData(digi->GetTime(), digi);
    nofDigis++;
  }  //# digis in map

  LOG(debug) << "Number of RICH digis before skip: " << fDataMap.size();
  LOG(debug) << "Nof skipped PMTs:" << nofSkippedPmts;
  LOG(debug) << "Nof skipped digis:" << nofSkippedDigis;
  LOG(debug) << "Number of RICH digis: " << nofDigis;
  return nofDigis;
}

void CbmRichDigitizer::AddNoiseDigis(Int_t eventNum, Int_t inputNum)
{
  Int_t nofPixels        = CbmRichDigiMapManager::GetInstance().GetPixelAddresses().size();
  Double_t dT            = 50.;  //ns
  Double_t nofRichPoints = fRichPoints->GetEntriesFast();
  Double_t nofNoiseDigis = nofRichPoints * nofPixels * dT * (fNoiseDigiRate / 1.e9);

  LOG(debug) << "CbmRichDigitizer::AddNoiseDigis NofAllPixels:" << nofPixels << " nofNoiseDigis:" << nofNoiseDigis;
  for (Int_t j = 0; j < nofNoiseDigis; j++) {
    Int_t address = CbmRichDigiMapManager::GetInstance().GetRandomPixelAddress();
    CbmLink link(1., -1, eventNum, inputNum);
    Double_t time = fCurrentEventTime + gRandom->Uniform(0, dT);
    AddDigi(address, time, link);
  }
}

void CbmRichDigitizer::AddDigi(Int_t address, Double_t time, const CbmLink& link)
{
  Bool_t wasFired   = fFiredPixelsMap.count(address) > 0;
  Bool_t isDetected = true;
  if (!fEventMode && wasFired) {
    Double_t lastFiredTime = fFiredPixelsMap[address];
    Double_t dt            = std::fabs(time - lastFiredTime);
    if (dt < fPixelDeadTime) {
      isDetected = false;
      //    LOG(info) << "CbmRichDigitizer::AddDigi pixel NOT registered: address:" << address << " cur-last=dT: "<< time << "-"
      //                                << lastFiredTime << "=" <<  dt << " fPixelDeadTime:" << fPixelDeadTime
      //                                << " fFiredPixelsMap.size():" << fFiredPixelsMap.size() << " link: " << link.GetIndex();
    }
    else {
      isDetected = true;
      //    LOG(info) << "CbmRichDigitizer::AddDigi pixel registered: address:" << address << " cur-last=dT: "<< time << "-"
      //                                << lastFiredTime << "=" <<  dt << " fPixelDeadTime:" << fPixelDeadTime
      //                                << " fFiredPixelsMap.size():" << fFiredPixelsMap.size() << " link: " << link.GetIndex();
    }
  }
  else {
    isDetected = true;
    // LOG(info) << "CbmRichDigitizer::AddDigi pixel was not fired before: address:" << address << " time:" << time
    //        << " fFiredPixelsMap.size():" << fFiredPixelsMap.size() << " link: " << link.GetIndex();
  }

  if (isDetected) {
    auto it = fDataMap.find(address);
    if (it == fDataMap.end()) {
      CbmRichDigi* digi = new CbmRichDigi();
      digi->SetAddress(address);
      digi->SetTime(time);
      CbmMatch* match = new CbmMatch();
      match->AddLink(link);
      pair<CbmRichDigi*, CbmMatch*> value(digi, match);
      fDataMap.insert(pair<Int_t, pair<CbmRichDigi*, CbmMatch*>>(address, value));
    }
    else {
      CbmMatch* match = it->second.second;
      match->AddLink(link);
    }
  }

  fFiredPixelsMap[address] = time;
}


ClassImp(CbmRichDigitizer)
