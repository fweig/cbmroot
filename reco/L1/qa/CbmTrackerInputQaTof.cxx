/* Copyright (C) 2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov[committer]*/

/// @file CbmTrackerInputQaTof.cxx
/// @author Sergey Gorbunov
/// @date 02.11.2021

#include "CbmTrackerInputQaTof.h"

#include "CbmDefs.h"
#include "CbmDigiManager.h"
#include "CbmLink.h"
#include "CbmMCDataArray.h"
#include "CbmMCDataManager.h"
#include "CbmMCEventList.h"
#include "CbmMCTrack.h"
#include "CbmMatch.h"
#include "CbmQaCanvas.h"
#include "CbmTofCell.h"
#include "CbmTofDigiPar.h"  // in tof/TofParam
#include "CbmTofTrackingInterface.h"
//#include "CbmSetup.h"
#include "CbmTimeSlice.h"
//#include "CbmTofCluster.h"
#include "CbmTofDigi.h"
#include "CbmTofHit.h"
//#include "CbmTofParModDigi.h"  // for CbmTofModule
//#include "CbmTofParModGeo.h"
//#include "CbmTofParSetDigi.h"  // for CbmTofParSetDigi
//#include "CbmTofParSetGeo.h"
#include "CbmTofPoint.h"

#include <FairRootManager.h>
#include <FairRunAna.h>
#include <FairRuntimeDb.h>
#include <FairSink.h>
#include <FairTask.h>
#include <Logger.h>

#include <TClonesArray.h>
#include <TDatabasePDG.h>
#include <TGenericClassInfo.h>
#include <TGeoManager.h>
#include <TGeoNode.h>
#include <TMathBase.h>
#include <TObjArray.h>
#include <TParameter.h>
#include <TParticlePDG.h>
#include <TString.h>
#include <TStyle.h>

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

#include <stdio.h>

ClassImp(CbmTrackerInputQaTof);

// -------------------------------------------------------------------------

CbmTrackerInputQaTof::CbmTrackerInputQaTof(const char* name, Int_t verbose) : FairTask(name, verbose)
{
  // Constructor

  // Create a list of histogramms

  fHistList.clear();
  fHistList.reserve(20);
  fHistList.push_back(&fhResidualX);
  fHistList.push_back(&fhResidualY);
  fHistList.push_back(&fhResidualT);
  //fHistList.push_back(&fhResidualZ);
  fHistList.push_back(&fhPullX);
  fHistList.push_back(&fhPullY);
  fHistList.push_back(&fhPullT);


  // Keep the ownership on the histograms to avoid double deletion
  for (unsigned int i = 0; i < fHistList.size(); i++) {
    fHistList[i]->SetDirectory(0);
  }
}

// -------------------------------------------------------------------------
CbmTrackerInputQaTof::~CbmTrackerInputQaTof()
{  /// Destructor
  DeInit();
}


// -------------------------------------------------------------------------
void CbmTrackerInputQaTof::DeInit()
{
  fIsTofInSetup      = 0;
  fIsMcPresent       = false;
  fNtrackingStations = 0;

  fTimeSlice   = nullptr;
  fDigiManager = nullptr;

  fMcManager = nullptr;
  fMcTracks  = nullptr;
  fMcPoints  = nullptr;

  fClusters   = nullptr;
  fHits       = nullptr;
  fHitMatches = nullptr;

  fOutFolder.Clear();

  fHistFolder = nullptr;

  fNevents.SetVal(0);

  fhPointsPerHit.clear();
  fhHitsPerPoint.clear();
}
// -------------------------------------------------------------------------

void CbmTrackerInputQaTof::SetParContainers()
{
  fTofDigiPar = nullptr;
  fTofGeoPar  = nullptr;

  // Get run and runtime database
  FairRunAna* ana = FairRunAna::Instance();
  if (!ana) { LOG(fatal) << "No FairRunAna instance"; }

  FairRuntimeDb* rtdb = ana->GetRuntimeDb();
  if (!rtdb) { LOG(fatal) << "No FairRuntimeDb instance"; }

  // fTofDigiPar = dynamic_cast<CbmTofDigiPar*>(rtdb->getContainer("CbmTofDigiPar"));
  // fTofGeoPar  = dynamic_cast<CbmTofParSetGeo*>(rtdb->getContainer("CbmTofParSetGeo"));

  fTofDigiPar = (CbmTofDigiPar*) rtdb->getContainer("CbmTofDigiPar");
  // fTofGeoPar  = rtdb->getContainer("CbmGeoTofPar");
}

// -------------------------------------------------------------------------
InitStatus CbmTrackerInputQaTof::ReInit()
{
  // Initialize and check the setup

  DeInit();

  // Check if Tof is present in the CBM setup
  // A straightforward way to do so is CbmSetup::Instance()->IsActive(ECbmModuleId::kTof),
  // but unfortunatelly CbmSetup class is unaccesible.
  // ( CbmSimSteer library requires libfreetype that has linking problems on MacOsX )
  // Therefore let's simply check if any Tof material is present in the geometry.
  //
  fIsTofInSetup = 0;
  {
    TObjArray* topNodes = gGeoManager->GetTopNode()->GetNodes();
    for (Int_t iTopNode = 0; iTopNode < topNodes->GetEntriesFast(); iTopNode++) {
      TGeoNode* topNode = static_cast<TGeoNode*>(topNodes->At(iTopNode));
      if (TString(topNode->GetName()).Contains("tof")) {
        fIsTofInSetup = 1;
        break;
      }
    }
  }

  if (!fIsTofInSetup) {
    LOG(info) << "Tof is not in the setup, do nothing";
    return kSUCCESS;
  }

  // check parameter containers

  if (!fTofDigiPar) {
    LOG(error) << "No CbmTofParSetDigi container in FairRuntimeDb";
    return kERROR;
  }

  //   if (!fTofGeoPar) {
  //     LOG(error) << "No CbmTofParSetGeo container in FairRuntimeDb";
  //     return kERROR;
  //   }

  FairRootManager* manager = FairRootManager::Instance();
  fDigiManager             = CbmDigiManager::Instance();
  fDigiManager->Init();

  fTimeSlice = dynamic_cast<CbmTimeSlice*>(manager->GetObject("TimeSlice."));
  if (!fTimeSlice) {
    LOG(error) << "No time slice found";
    return kERROR;
  }

  fHits = dynamic_cast<TClonesArray*>(manager->GetObject("TofHit"));

  if (!fHits) {
    LOG(error) << "No Tof hit array found";
    return kERROR;
  }

  //   fClusters = dynamic_cast<TClonesArray*>(manager->GetObject("TofCluster"));
  //
  //   if (!fClusters) {
  //     LOG(error) << "No Tof cluster array found";
  //     return kERROR;
  //   }

  fMcManager = dynamic_cast<CbmMCDataManager*>(manager->GetObject("MCDataManager"));

  fIsMcPresent = (fMcManager != nullptr);

  if (fIsMcPresent) {

    fMcEventList = dynamic_cast<CbmMCEventList*>(manager->GetObject("MCEventList."));
    fMcTracks    = fMcManager->InitBranch("MCTrack");
    fMcPoints    = fMcManager->InitBranch("TofPoint");
    fHitMatches  = dynamic_cast<TClonesArray*>(manager->GetObject("TofHitMatch"));

    fDigiManager = CbmDigiManager::Instance();
    fDigiManager->Init();

    fHitDigiMatches = dynamic_cast<TClonesArray*>(manager->GetObject("TofDigiMatch"));
    fDigis          = dynamic_cast<TClonesArray*>(manager->GetObject("TofCalDigi"));
    if (nullptr == fDigis) {
      LOG(info) << "No calibrated tof digi vector in the input files => trying original vector";
      fDigis = dynamic_cast<TClonesArray*>(manager->GetObject("TofDigi"));
      if (nullptr == fDigis) { LOG(info) << "No original tof digi vector in the input files! Ignore TOF!"; }
    }
    fTofCalDigiMatch = dynamic_cast<TClonesArray*>(manager->GetObject("TofCalDigiMatch"));


    // we assume that when Tof is in the setup and the MC manager is present,
    // the Tof MC data must be present too

    if (!fMcEventList) {
      LOG(error) << ": No MCEventList data!";
      return kERROR;
    }

    if (!fMcTracks) {
      LOG(error) << "No MC tracks found";
      return kERROR;
    }

    if (!fMcPoints) {
      LOG(error) << "No MC points found";
      return kERROR;
    }

    if (!fHitMatches) {
      LOG(error) << "No Tof hit matches found";
      return kERROR;
    }

    //         if (!fDigis) {
    //       LOG(error) << "No Tof digis found";
    //       return kERROR;
    //     }

    if (!fDigiManager->IsMatchPresent(ECbmModuleId::kTof)) {
      LOG(error) << "No Tof digi matches found";
      return kERROR;
    }
  }

  GeometryQa();

  if (fNtrackingStations <= 0) {
    LOG(error) << "can not count Tof tracking stations";
    return kERROR;
  }

  // initialise histograms
  fOutFolder.SetOwner(false);
  fHistFolder = fOutFolder.AddFolder("rawHist", "Raw Histograms");
  gStyle->SetOptStat(0);

  fNevents.SetVal(0);
  fHistFolder->Add(&fNevents);

  for (unsigned int i = 0; i < fHistList.size(); i++) {
    fHistList[i]->Reset();
    fHistFolder->Add(fHistList[i]);
  }

  fhPointsPerHit.clear();
  fhHitsPerPoint.clear();
  fhEfficiencyR.clear();
  fhEfficiencyXY.clear();

  fhPointsPerHit.reserve(fNtrackingStations);
  fhHitsPerPoint.reserve(fNtrackingStations);
  fhEfficiencyR.reserve(fNtrackingStations);
  fhEfficiencyXY.reserve(fNtrackingStations);

  for (Int_t i = 0; i < fNtrackingStations; i++) {
    fhPointsPerHit.emplace_back(Form("hMcPointsPerHit%i", i),
                                Form("MC Points per Hit: Station %i;N mc points / hit", i), 10, -0.5, 9.5);
    fhPointsPerHit[i].SetDirectory(0);
    fhPointsPerHit[i].SetLineWidth(2);
    fhPointsPerHit[i].SetOptStat(110);
    fHistFolder->Add(&fhPointsPerHit[i]);
  }

  for (Int_t i = 0; i < fNtrackingStations; i++) {
    fhHitsPerPoint.emplace_back(Form("hHitsPerMcPoint%i", i),
                                Form("Hits per MC Point: Station %i; N hits / mc point", i), 10, -0.5, 9.5);
    fhHitsPerPoint[i].SetDirectory(0);
    fhHitsPerPoint[i].SetLineWidth(2);
    fhHitsPerPoint[i].SetOptStat(110);
    fHistFolder->Add(&fhHitsPerPoint[i]);
  }

  for (Int_t i = 0; i < fNtrackingStations; i++) {

    double dx = 350;
    double dy = 350;
    double dr = sqrt(dx * dx + dy * dy);

    fhEfficiencyR.emplace_back(Form("hEfficiencyR%i", i), Form("Efficiency R: Station %i;R [cm]", i), 100, 0, dr);
    fhEfficiencyR[i].SetDirectory(0);
    fhEfficiencyR[i].SetOptStat(1110);
    fHistFolder->Add(&fhEfficiencyR[i]);

    fhEfficiencyXY.emplace_back(Form("hEfficiencyXY%i", i), Form("Efficiency XY: Station %i;X [cm];Y [cm]", i), 50, -dx,
                                dx, 50, -dy, dy);
    fhEfficiencyXY[i].SetDirectory(0);
    fhEfficiencyXY[i].SetOptStat(10);
    fhEfficiencyXY[i].GetYaxis()->SetTitleOffset(1.4);
    fHistFolder->Add(&fhEfficiencyXY[i]);
  }

  fCanvResidual.Clear();
  fCanvResidual.Divide2D(6);

  fCanvPull.Clear();
  fCanvPull.Divide2D(6);

  fCanvEfficiencyR.Clear();
  fCanvEfficiencyR.Divide2D(fNtrackingStations);

  fCanvEfficiencyXY.Clear();
  fCanvEfficiencyXY.Divide2D(fNtrackingStations);

  fCanvPointsPerHit.Clear();
  fCanvPointsPerHit.Divide2D(fNtrackingStations);

  fCanvHitsPerPoint.Clear();
  fCanvHitsPerPoint.Divide2D(fNtrackingStations);

  fOutFolder.Add(&fCanvResidual);
  fOutFolder.Add(&fCanvPull);
  fOutFolder.Add(&fCanvEfficiencyR);
  fOutFolder.Add(&fCanvEfficiencyXY);
  fOutFolder.Add(&fCanvPointsPerHit);
  fOutFolder.Add(&fCanvHitsPerPoint);

  // analyse the geometry setup
  return GeometryQa();
}


// -------------------------------------------------------------------------
void CbmTrackerInputQaTof::Exec(Option_t*)
{
  if (!fIsTofInSetup) { return; }

  // update number of processed events
  fNevents.SetVal(fNevents.GetVal() + 1);
  LOG(debug) << "Event: " << fNevents.GetVal();
  ResolutionQa();
}


// -------------------------------------------------------------------------
void CbmTrackerInputQaTof::Finish()
{
  FairSink* sink = FairRootManager::Instance()->GetSink();
  if (sink) { sink->WriteObject(&GetQa(), nullptr); }
}

int CbmTrackerInputQaTof::GetStationIndex(CbmTofPoint* p)
{
  //    if (p->GetZ() > 800) return -1;
  float dist        = 1000;
  int iSta          = -1;
  auto tofInterface = CbmTofTrackingInterface::Instance();
  for (int iSt = 0; iSt < fNtrackingStations; iSt++) {
    if (fabs(p->GetZ() - tofInterface->GetZ(iSt)) < dist) {
      dist = fabs(p->GetZ() - tofInterface->GetZ(iSt));
      iSta = iSt;
    }
  }
  return iSta;
}


// -------------------------------------------------------------------------
InitStatus CbmTrackerInputQaTof::GeometryQa()
{
  // check geometry of the Tof modules

  auto tofInterface = CbmTofTrackingInterface::Instance();

  fNtrackingStations = tofInterface->GetNtrackingStations();

  double lastZ = -1;
  for (int iStation = 0; iStation < fNtrackingStations; iStation++) {

    //     tofInterface->GetZ(iStation);
    //     tofInterface->GetTimeResolution(iStation);
    //     tofInterface->IsTimeInfoProvided(iStation);

    double staZ = tofInterface->GetZ(iStation);

    // check that the stations are properly ordered in Z
    if (((iStation > 0) && (staZ <= lastZ)) || ((staZ != staZ))) {
      LOG(error) << "Tof trackig stations are not properly ordered in Z";
      return kERROR;
    }
    lastZ = staZ;
  }

  return kSUCCESS;
}

// -------------------------------------------------------------------------
void CbmTrackerInputQaTof::ResolutionQa()
{
  if (!fDigiManager->IsMatchPresent(ECbmModuleId::kTof)) return;

  if (!(fHitMatches && fHits)) return;

  //  if (!fDigiManager) return;

  Int_t nHits       = fHits->GetEntriesFast();
  Int_t nHitMatches = fHitMatches->GetEntriesFast();
  // Int_t nClusters = fClusters->GetEntriesFast();
  //Int_t nDigis    = fDigiManager->GetNofDigis(ECbmModuleId::kTof);
  //  Int_t nTofDigis       = fDigis->GetEntriesFast();

  auto tofInterface = CbmTofTrackingInterface::Instance();

  fNtrackingStations = tofInterface->GetNtrackingStations();

  GeometryQa();

  int nMcEvents = fMcEventList->GetNofEvents();

  // Vector of integers parallel to mc points
  std::vector<std::vector<int>> pointNhits;
  pointNhits.resize(nMcEvents);
  for (int iE = 0; iE < nMcEvents; iE++) {
    int fileId  = fMcEventList->GetFileIdByIndex(iE);
    int eventId = fMcEventList->GetEventIdByIndex(iE);
    int nPoints = fMcPoints->Size(fileId, eventId);
    pointNhits[iE].resize(nPoints, 0);
  }

  std::vector<int> TofSinglePoints[nMcEvents];  // choose only one MC point per plane per MC track

  for (int iE = 0; iE < nMcEvents; iE++) {

    int fileId  = fMcEventList->GetFileIdByIndex(iE);
    int eventId = fMcEventList->GetEventIdByIndex(iE);

    int nMcTracks  = fMcTracks->Size(fileId, eventId);
    int nHitPoints = fMcPoints->Size(fileId, eventId);

    std::vector<char> singleMcPointPerStation[fNtrackingStations];

    for (Int_t iS = 0; iS < fNtrackingStations; iS++)
      singleMcPointPerStation[iS].resize(nMcTracks, 0);

    std::vector<char> isTofPointMatched;
    isTofPointMatched.resize(nHitPoints, 0);
    // check whether point was matched
    for (Int_t iHit = 0; iHit < nHits; iHit++) {
      CbmMatch* pHitMatch = static_cast<CbmMatch*>(fHitMatches->At(iHit));
      for (int iLink = 0; iLink < pHitMatch->GetNofLinks(); iLink++) {
        Int_t iMC  = pHitMatch->GetLink(iLink).GetIndex();
        int iFile  = pHitMatch->GetLink(iLink).GetFile();
        int iEvent = pHitMatch->GetLink(iLink).GetEntry();
        if ((eventId != iEvent) || (fileId != iFile)) continue;
        isTofPointMatched[iMC] = 1;
      }
    }
    // store matched points first
    for (Int_t iMC = 0; iMC < fMcPoints->Size(fileId, eventId); iMC++) {
      if (isTofPointMatched[iMC] == 0) continue;

      CbmTofPoint* p = dynamic_cast<CbmTofPoint*>(fMcPoints->Get(fileId, eventId, iMC));

      int PointStation = GetStationIndex(p);

      if (PointStation < 0) continue;

      if (!singleMcPointPerStation[PointStation][p->GetTrackID()]) TofSinglePoints[eventId].push_back(iMC);
      singleMcPointPerStation[PointStation][p->GetTrackID()] = 1;
    }

    // store unmatched points if quota not exceeded (one point per mc track per tof plane)
    for (Int_t iMC = 0; iMC < fMcPoints->Size(fileId, eventId); iMC++) {

      CbmTofPoint* p = dynamic_cast<CbmTofPoint*>(fMcPoints->Get(fileId, eventId, iMC));

      int PointStation = GetStationIndex(p);

      if (PointStation < 0) continue;

      if (!singleMcPointPerStation[PointStation][p->GetTrackID()]) TofSinglePoints[eventId].push_back(iMC);
      singleMcPointPerStation[PointStation][p->GetTrackID()] = 1;
    }
  }


  if (nHits != nHitMatches)
    LOG(fatal) << "CbmTrackerInputQaTof::ResolutionQa => Nb hits in vector not matching nb matches in vector: " << nHits
               << " VS " << nHitMatches;

  for (Int_t iHit = 0; iHit < nHits; iHit++) {

    CbmTofHit* hit = dynamic_cast<CbmTofHit*>(fHits->At(iHit));
    if (!hit) {
      LOG(error) << "Tof hit N " << iHit << " doesn't exist";
      // return;
    }
    const int address = hit->GetAddress();

    if (0x00202806 == address || 0x00002806 == address) continue;  //should ignore these hits for some reason

    int StationIndex = tofInterface->GetTrackingStationIndex(hit);

    if (StationIndex < 0 || StationIndex >= fNtrackingStations) {
      LOG(fatal) << "Tof hit layer id " << StationIndex << " is out of range";
      //   return;
    }

    CbmMatch* myHitMatch = dynamic_cast<CbmMatch*>(fHitMatches->At(iHit));

    // take corresponding MC point

    const CbmLink& bestLink = myHitMatch->GetMatchedLink();

    // skip hits from the noise digis
    if (bestLink.GetIndex() < 0) { continue; }

    CbmTofPoint* p = dynamic_cast<CbmTofPoint*>(fMcPoints->Get(bestLink));
    //     if (p == nullptr) {
    //       LOG(error) << /*ADD ALERT HERE;*/ "link points to a non-existing MC point";
    //       // return;
    //     }
    //
    //     if (p->GetDetectorID() != (int) hit->GetAddress()) {
    //       LOG(error) << /*ADD ALERT HERE;*/ "mc point module address differs from the hit module address";
    //       // return;
    //     }

    // how many MC points? fill N hits per mc point
    int nHitPoints = 0;
    for (int i = 0; i < myHitMatch->GetNofLinks(); i++) {
      const CbmLink& link = myHitMatch->GetLink(i);
      if (link.GetIndex() >= 0) {

        int iE         = fMcEventList->GetEventIndex(link);
        int indexPoint = link.GetIndex();

        bool mcPointValid = 0;

        int nPointsEv = TofSinglePoints[iE].size();
        for (int ip = 0; ip < nPointsEv; ip++) {
          if (TofSinglePoints[iE][ip] == indexPoint) mcPointValid = 1;
        }

        if (!mcPointValid) continue;

        nHitPoints++;


        if (iE < 0 || iE >= nMcEvents) {
          LOG(error) << "link points to a non-existing MC event";
          return;
        }
        if (link.GetIndex() >= (int) pointNhits[iE].size()) {
          LOG(error) << "link points to a non-existing MC point";
          return;
        }
        pointNhits[iE][link.GetIndex()]++;
      }
    }
    fhPointsPerHit[StationIndex].Fill(nHitPoints);

    //     if (p == nullptr) {
    //       LOG(error) << "link points to a non-existing MC point";
    //       return;
    //     }
    //
    //     if (p->GetDetectorID() != (int) hit->GetAddress()) {
    //       LOG(error) << "mc point module address differs from the hit module address";
    //       return;
    //     }

    // check that the nominal station Z is not far from the active material

    // the cut of 1 cm is choosen arbitrary and can be changed
    //
    //       const CbmTofParModGeo* pGeo = dynamic_cast<const CbmTofParModGeo*>(fTofGeoPar->GetModulePar(ModuleId));
    //       if (!pGeo) {
    //         LOG(fatal) << " No Geo params for station " << StationIndex << " module " << ModuleId << " found ";
    //         return;
    //       }

    //       double staZ = tofInterface->GetZ(StationIndex);  // module->GetZ();  //+ 410;
    //       if ((staZ < p->GetZ() - 1.) || (staZ > p->GetZ() + 1.)) {
    //         LOG(error) << "Tof station " << StationIndex << ": active material Z[" << p->GetZ() << " cm," << p->GetZ()
    //                    << " cm] is too far from the nominal station Z " << staZ << " cm";
    //         return;
    //       }
    //       // the cut of 1 cm is choosen arbitrary and can be changed
    //       if (fabs(hit->GetZ() - staZ) > 1.) {
    //         LOG(error) << "Tof station " << StationIndex << ": hit Z " << hit->GetZ()
    //                    << " cm, is too far from the nominal station Z " << staZ << " cm";
    //         return;
    //       }
    //     }

    // residual and pull

    if (nHitPoints != 1) continue;  // only check residual for non-mixed hits

    double t0 = fMcEventList->GetEventTime(bestLink);
    if (t0 < 0) {
      LOG(error) << "MC event time doesn't exist for a Tof link";
      return;
    }

    double x  = p->GetX();  // take the "In" point since the time is stored only for this point
    double y  = p->GetY();
    double z  = p->GetZ();
    double t  = t0 + p->GetTime();
    double px = p->GetPx();
    double py = p->GetPy();
    double pz = p->GetPz();

    // skip very slow particles
    if (fabs(p->GetPz()) < 0.01) continue;

    // transport the particle from the MC point to the hit Z
    double dz = hit->GetZ() - z;
    x += dz * px / pz;
    y += dz * py / pz;

    fhResidualZ.Fill(dz);

    // get particle mass
    double mass = 0;
    {
      CbmLink mcTrackLink = bestLink;
      mcTrackLink.SetIndex(p->GetTrackID());
      CbmMCTrack* mcTrack = dynamic_cast<CbmMCTrack*>(fMcTracks->Get(mcTrackLink));
      if (!mcTrack) {
        LOG(error) << "MC track " << p->GetTrackID() << " doesn't exist";
        return;
      }
      Int_t pdg = mcTrack->GetPdgCode();
      if (pdg < 9999999 && ((TParticlePDG*) TDatabasePDG::Instance()->GetParticle(pdg))) {
        mass = TDatabasePDG::Instance()->GetParticle(pdg)->Mass();
      }
    }

    constexpr double speedOfLight = 29.979246;  // cm/ns
    TVector3 mom3;
    p->Momentum(mom3);
    t += dz / (pz * speedOfLight) * sqrt(mass * mass + mom3.Mag2());

    double dx = hit->GetX() - x;
    double dy = hit->GetY() - y;
    double dt = hit->GetTime() - t;
    double sx = hit->GetDx();
    double sy = hit->GetDy();
    double st = hit->GetTimeError();

    // residuals
    fhResidualX.Fill(dx);
    fhResidualY.Fill(dy);
    fhResidualT.Fill(dt);

    // pulls
    if ((sx < 1.e-5) || (sy < 1.e-5) || (st < 1.e-5)) {
      LOG(error) << "Tof hit errors are not properly set: errX " << hit->GetDx() << " errY " << hit->GetDy() << " errT "
                 << st;
      // return;
    }

    fhPullX.Fill(dx / sx);
    fhPullY.Fill(dy / sy);
    fhPullT.Fill(dt / st);


  }  // iHit                                                     // for (Int_t iHit = 0; iHit < nofHits; iHit++)

  // fill efficiency: N hits per MC point

  for (int iE = 0; iE < nMcEvents; iE++) {
    int fileId    = fMcEventList->GetFileIdByIndex(iE);
    int eventId   = fMcEventList->GetEventIdByIndex(iE);
    int nPointsEv = TofSinglePoints[eventId].size();
    for (int ip = 0; ip < nPointsEv; ip++) {

      int iPoint     = TofSinglePoints[eventId][ip];
      CbmTofPoint* p = dynamic_cast<CbmTofPoint*>(fMcPoints->Get(fileId, eventId, iPoint));
      if (p == nullptr) {
        LOG(error) << "MC point doesn't exist";
        return;
      }

      int iSt = GetStationIndex(p);
      if (iSt < 0) continue;

      int StationIndex = iSt;  //CbmTofTrackingInterface::Instance()->GetTrackingStationIndex(p);
      if (StationIndex < 0 || StationIndex >= fNtrackingStations) {
        LOG(fatal) << "Tof point layer id " << StationIndex << " is out of range, z of point = " << p->GetZ();
        return;
      }
      fhHitsPerPoint[StationIndex].Fill(pointNhits[iE][iPoint]);
      fhEfficiencyR[StationIndex].Fill(sqrt(p->GetX() * p->GetX() + p->GetY() * p->GetY()),
                                       (pointNhits[iE][iPoint] > 0));
      fhEfficiencyXY[StationIndex].Fill(p->GetX(), p->GetY(), (pointNhits[iE][iPoint] > 0));
    }
  }
}


// -------------------------------------------------------------------------
TFolder& CbmTrackerInputQaTof::GetQa()
{
  gStyle->SetPaperSize(20, 20);

  for (Int_t i = 0; i < fNtrackingStations; i++) {
    fCanvHitsPerPoint.cd(i + 1);
    fhHitsPerPoint[i].DrawCopy("", "");
    fCanvPointsPerHit.cd(i + 1);
    fhPointsPerHit[i].DrawCopy("", "");

    fCanvEfficiencyR.cd(i + 1);
    fhEfficiencyR[i].DrawCopy("colz", "");

    fCanvEfficiencyXY.cd(i + 1);
    fhEfficiencyXY[i].DrawCopy("colz", "");
  }

  for (UInt_t i = 0; i < fHistList.size(); i++) {
    fHistList[i]->Fit("gaus", "Q");  // Q for the quiet mode
  }

  for (UInt_t i = 0; i < 3; i++) {
    fCanvResidual.cd(i + 1);
    fHistList[i]->DrawCopy("", "");
    fCanvPull.cd(i + 1);
    fHistList[i + 3]->DrawCopy("", "");
  }

  return fOutFolder;
}
