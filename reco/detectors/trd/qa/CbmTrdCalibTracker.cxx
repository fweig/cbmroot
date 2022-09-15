/* Copyright (C) 2022 Horia Hulubei National Institute of Physics and Nuclear Engineering, Bucharest
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Alexandru Bercuci[committer]*/

/// @file CbmTrdCalibTracker.cxx
/// @author Alexandru Bercuci
/// @date 02.05.2022

#include "CbmTrdCalibTracker.h"

#include "CbmDefs.h"
#include "CbmDigiManager.h"
#include "CbmLink.h"
#include "CbmMCDataArray.h"
#include "CbmMCDataManager.h"
#include "CbmMCEventList.h"
#include "CbmMCTrack.h"
#include "CbmMatch.h"
#include "CbmQaCanvas.h"
//#include "CbmSetup.h"
#include "CbmTimeSlice.h"
#include "CbmTrdCluster.h"
#include "CbmTrdDigi.h"
#include "CbmTrdHit.h"
#include "CbmTrdHitMC.h"
#include "CbmTrdParModDigi.h"  // for CbmTrdModule
#include "CbmTrdParModGeo.h"
#include "CbmTrdParSetDigi.h"  // for CbmTrdParSetDigi
#include "CbmTrdParSetGeo.h"
#include "CbmTrdPoint.h"

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

ClassImp(CbmTrdCalibTracker);

// -------------------------------------------------------------------------

CbmTrdCalibTracker::CbmTrdCalibTracker(const char* name, Int_t verbose) : FairTask(name, verbose)
{
  // Constructor

  // Create a list of histogramms

  fHistList.clear();
  fHistList.reserve(20);
  fHistList.push_back(&fh1DresidualU);
  fHistList.push_back(&fh1DresidualV);
  fHistList.push_back(&fh1DresidualT);
  fHistList.push_back(&fh2DresidualX);
  fHistList.push_back(&fh2DresidualY);
  fHistList.push_back(&fh2DresidualT);
  fHistList.push_back(&fh1DpullU);
  fHistList.push_back(&fh1DpullV);
  fHistList.push_back(&fh1DpullT);
  fHistList.push_back(&fh2DpullX);
  fHistList.push_back(&fh2DpullY);
  fHistList.push_back(&fh2DpullT);

  // Keep the ownership on the histograms to avoid double deletion
  for (unsigned int i = 0; i < fHistList.size(); i++) {
    fHistList[i]->SetDirectory(0);
  }
}

// -------------------------------------------------------------------------
CbmTrdCalibTracker::~CbmTrdCalibTracker()
{  /// Destructor
  DeInit();
}


// -------------------------------------------------------------------------
void CbmTrdCalibTracker::DeInit()
{
  fIsTrdInSetup      = 0;
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

  if (fHitsMc) {
    fHitsMc->Clear("C");
    fHitsMc->Delete();
    delete fHitsMc;
  }
}
// -------------------------------------------------------------------------

void CbmTrdCalibTracker::SetParContainers()
{
  fTrdDigiPar = nullptr;
  fTrdGeoPar  = nullptr;

  // Get run and runtime database
  FairRunAna* ana = FairRunAna::Instance();
  if (!ana) { LOG(fatal) << "No FairRunAna instance"; }

  FairRuntimeDb* rtdb = ana->GetRuntimeDb();
  if (!rtdb) { LOG(fatal) << "No FairRuntimeDb instance"; }

  fTrdDigiPar = dynamic_cast<CbmTrdParSetDigi*>(rtdb->getContainer("CbmTrdParSetDigi"));
  fTrdGeoPar  = dynamic_cast<CbmTrdParSetGeo*>(rtdb->getContainer("CbmTrdParSetGeo"));
}

// -------------------------------------------------------------------------
InitStatus CbmTrdCalibTracker::ReInit()
{
  // Initialize and check the setup

  DeInit();

  // Check if TRD is present in the CBM setup
  // A straightforward way to do so is CbmSetup::Instance()->IsActive(ECbmModuleId::kTrd),
  // but unfortunatelly CbmSetup class is unaccesible.
  // ( CbmSimSteer library requires libfreetype that has linking problems on MacOsX )
  // Therefore let's simply check if any TRD material is present in the geometry.
  //
  fIsTrdInSetup = 0;
  {
    TObjArray* topNodes = gGeoManager->GetTopNode()->GetNodes();
    for (Int_t iTopNode = 0; iTopNode < topNodes->GetEntriesFast(); iTopNode++) {
      TGeoNode* topNode = static_cast<TGeoNode*>(topNodes->At(iTopNode));
      if (TString(topNode->GetName()).Contains("trd")) {
        fIsTrdInSetup = 1;
        break;
      }
    }
  }

  if (!fIsTrdInSetup) {
    LOG(info) << "TRD is not in the setup, do nothing";
    return kSUCCESS;
  }

  // check parameter containers

  if (!fTrdDigiPar) {
    LOG(error) << "No CbmTrdParSetDigi container in FairRuntimeDb";
    return kERROR;
  }

  if (!fTrdGeoPar) {
    LOG(error) << "No CbmTrdParSetGeo container in FairRuntimeDb";
    return kERROR;
  }

  FairRootManager* manager = FairRootManager::Instance();
  fDigiManager             = CbmDigiManager::Instance();
  fDigiManager->Init();

  fTimeSlice = dynamic_cast<CbmTimeSlice*>(manager->GetObject("TimeSlice."));
  if (!fTimeSlice) {
    LOG(error) << "No time slice found";
    return kERROR;
  }

  fHits = dynamic_cast<TClonesArray*>(manager->GetObject("TrdHit"));

  if (!fHits) {
    LOG(error) << "No TRD hit array found";
    return kERROR;
  }

  fClusters = dynamic_cast<TClonesArray*>(manager->GetObject("TrdCluster"));

  if (!fClusters) {
    LOG(error) << "No TRD cluster array found";
    return kERROR;
  }

  fMcManager = dynamic_cast<CbmMCDataManager*>(manager->GetObject("MCDataManager"));

  fIsMcPresent = (fMcManager != nullptr);

  if (fIsMcPresent) {

    fMcEventList = dynamic_cast<CbmMCEventList*>(manager->GetObject("MCEventList."));
    fMcTracks    = fMcManager->InitBranch("MCTrack");
    fMcPoints    = fMcManager->InitBranch("TrdPoint");
    fHitMatches  = dynamic_cast<TClonesArray*>(manager->GetObject("TrdHitMatch"));

    // we assume that when TRD is in the setup and the MC manager is present,
    // the TRD MC data must be present too

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
      LOG(error) << "No TRD hit matches found";
      return kERROR;
    }

    if (!fDigiManager->IsMatchPresent(ECbmModuleId::kTrd)) {
      LOG(error) << "No TRD digi matches found";
      return kERROR;
    }
  }

  // count TRD stations
  // TODO: put the code below to some TRD geometry interface

  fNtrackingStations = 0;
  {
    Int_t layerCounter  = 0;
    TObjArray* topNodes = gGeoManager->GetTopNode()->GetNodes();
    for (Int_t iTopNode = 0; iTopNode < topNodes->GetEntriesFast(); iTopNode++) {
      TGeoNode* topNode = static_cast<TGeoNode*>(topNodes->At(iTopNode));
      if (TString(topNode->GetName()).Contains("trd")) {
        TObjArray* layers = topNode->GetNodes();
        for (Int_t iLayer = 0; iLayer < layers->GetEntriesFast(); iLayer++) {
          TGeoNode* layer = static_cast<TGeoNode*>(layers->At(iLayer));
          if (TString(layer->GetName()).Contains("layer")) { layerCounter++; }
        }
      }
    }
    fNtrackingStations = layerCounter;
  }

  LOG(debug) << "Init(): fNtrackingStations = " << fNtrackingStations;

  if (fNtrackingStations <= 0) {
    LOG(error) << "can not count TRD tracking stations";
    return kERROR;
  }

  // init output tree
  fHitsMc = new TClonesArray("CbmTrdHitMC", 100);
  manager->Register("TrdHitMC", "TRD", fHitsMc, IsOutputBranchPersistent("TrdHitMC"));

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
void CbmTrdCalibTracker::Exec(Option_t*)
{
  if (!fIsTrdInSetup) { return; }

  // update number of processed events
  fNevents.SetVal(fNevents.GetVal() + 1);
  LOG(debug) << "Event: " << fNevents.GetVal();
  ResolutionQa();
}


// -------------------------------------------------------------------------
void CbmTrdCalibTracker::Finish()
{
  FairSink* sink = FairRootManager::Instance()->GetSink();
  if (sink) { sink->WriteObject(&GetQa(), nullptr); }
}


// -------------------------------------------------------------------------
InitStatus CbmTrdCalibTracker::GeometryQa()
{
  // check geometry of the TRD modules

  double lastZ = -1;
  for (int iStation = 0; iStation < fNtrackingStations; iStation++) {

    int ModuleId = fTrdDigiPar->GetModuleId(iStation);

    const CbmTrdParModGeo* pGeo = dynamic_cast<const CbmTrdParModGeo*>(fTrdGeoPar->GetModulePar(ModuleId));
    if (!pGeo) {
      LOG(fatal) << " No Geo params for station " << iStation << " module " << ModuleId << " found ";
      return kERROR;
    }

    double staZ = pGeo->GetZ();

    // check that the stations are properly ordered in Z
    if ((iStation > 0) && (staZ <= lastZ)) {
      LOG(error) << "TRD trackig stations are not properly ordered in Z";
      return kERROR;
    }
    lastZ = staZ;

    //TODO: what are these 3 and 6 here in L1 code? Why are they hardcoed?
    // if (num == 0 || num == 2 || num == 4) geo.push_back(3);
    // if (num == 1 || num == 3) geo.push_back(6);
  }

  return kSUCCESS;
}

// -------------------------------------------------------------------------
void CbmTrdCalibTracker::ResolutionQa()
{
  if (!fDigiManager->IsMatchPresent(ECbmModuleId::kTrd)) return;

  if (!fIsMcPresent) return;

  Int_t nHits     = fHits->GetEntriesFast();
  Int_t nClusters = fClusters->GetEntriesFast();
  Int_t nDigis    = fDigiManager->GetNofDigis(ECbmModuleId::kTrd);

  int nMcEvents = fMcEventList->GetNofEvents();
  int imc(0);  // index of hit->MC QA objects
  fHitsMc->Delete();

  // Vector of integers parallel to mc points
  std::vector<std::vector<int>> pointNhits;
  pointNhits.resize(nMcEvents);
  for (int iE = 0; iE < nMcEvents; iE++) {
    int fileId  = fMcEventList->GetFileIdByIndex(iE);
    int eventId = fMcEventList->GetEventIdByIndex(iE);
    int nPoints = fMcPoints->Size(fileId, eventId);
    pointNhits[iE].resize(nPoints, 0);
  }

  for (Int_t iHit = 0; iHit < nHits; iHit++) {

    const CbmTrdHit* hit = dynamic_cast<const CbmTrdHit*>(fHits->At(iHit));
    if (!hit) {
      LOG(error) << "TRD hit N " << iHit << " doesn't exist";
      return;
    }

    if ((int) hit->GetClassType() > 1) {
      // class type 0: trd-1D
      // class type 1: trd-2D
      // the return type is (currently) boolean, so this error should never happen
      LOG(error) << "Unknown detector class type " << hit->GetClassType() << " for TRD hit N " << iHit;
      return;
    }

    const int address = hit->GetAddress();
    int StationIndex  = CbmTrdAddress::GetLayerId(address);

    if (StationIndex < 0 || StationIndex >= fNtrackingStations) {
      LOG(fatal) << "TRD hit layer id " << StationIndex << " is out of range";
      return;
    }

    Int_t clusterId = hit->GetRefId();
    if (clusterId < 0 || clusterId >= nClusters) {
      LOG(error) << "TRD cluster id " << clusterId << " is out of range";
      return;
    }

    CbmTrdCluster* cluster = dynamic_cast<CbmTrdCluster*>(fClusters->At(clusterId));
    if (!cluster) {
      LOG(error) << "TRD cluster N " << clusterId << " doesn't exist";
      return;
    }

    if (cluster->GetAddress() != address) {
      LOG(error) << "TRD hit address " << address << " differs from its cluster address " << cluster->GetAddress();
      return;
    }

    Int_t nClusterDigis = cluster->GetNofDigis();

    if (nClusterDigis <= 0) {
      LOG(error) << "hit match for TRD cluster " << clusterId << " has no digis ";
      return;
    }

    // construct QA object
    CbmTrdHitMC* hmc = new ((*fHitsMc)[imc++]) CbmTrdHitMC(*hit);
    hmc->AddCluster(cluster);
    uint64_t tdigi = 0;

    // custom finder of the digi matches
    CbmMatch myHitMatch;
    for (Int_t iDigi = 0; iDigi < nClusterDigis; iDigi++) {
      Int_t digiIdx = cluster->GetDigi(iDigi);
      if (digiIdx < 0 || digiIdx >= nDigis) {
        LOG(fatal) << "TRD cluster: digi index " << digiIdx << " out of range ";
        return;
      }
      const CbmTrdDigi* digi = CbmDigiManager::Instance()->Get<CbmTrdDigi>(digiIdx);
      if (!digi) {
        LOG(fatal) << "TRD digi " << iDigi << " not found";
        return;
      }

      if (digi->GetAddressModule() != address) {
        std::stringstream ss;
        ss << "TRD hit address " << address << " differs from its digi address " << digi->GetAddressModule();
        hmc->SetErrorMsg(ss.str());
        LOG(error) << ss.str();
        return;
      }
      switch (digi->GetType()) {
        case CbmTrdDigi::eCbmTrdAsicType::kFASP:
          if (!tdigi) tdigi = digi->GetTimeDAQ();
          break;
        case CbmTrdDigi::eCbmTrdAsicType::kSPADIC:
          if (!tdigi) tdigi = digi->GetTime();
          break;
        default: LOG(fatal) << "TRD digi type neither SPADIC or FASP"; return;
      }
      hmc->AddSignal(digi, tdigi);

      const CbmMatch* match = dynamic_cast<const CbmMatch*>(fDigiManager->GetMatch(ECbmModuleId::kTrd, digiIdx));
      if (!match) {
        LOG(fatal) << "TRD digi match " << digiIdx << " not found";
        return;
      }
      myHitMatch.AddLinks(*match);
    }
    hmc->PurgeSignals();

    if (myHitMatch.GetNofLinks() == 0) { continue; }

    const CbmLink& bestLink = myHitMatch.GetMatchedLink();

    {  // check if the hit match is correct
      CbmMatch* hitMatch = dynamic_cast<CbmMatch*>(fHitMatches->At(iHit));
      if (hitMatch == nullptr) {
        std::stringstream ss;
        ss << "hit match for TRD hit " << iHit << " doesn't exist";
        hmc->SetErrorMsg(ss.str());
        LOG(error) << ss.str();
      }
      else {
        const CbmLink& link = hitMatch->GetMatchedLink();
        if ((link != bestLink) && (link.GetWeight() != bestLink.GetWeight())) {
          std::stringstream ss;
          ss << "hit match for TRD hit " << iHit << " doesn't correspond to digi matches";
          hmc->SetErrorMsg(ss.str());
          LOG(error) << ss.str();
        }
      }
    }

    // how many MC points? fill N hits per mc point

    int nHitPoints = 0;
    for (int i = 0; i < myHitMatch.GetNofLinks(); i++) {
      const CbmLink& link = myHitMatch.GetLink(i);
      if (link.GetIndex() >= 0) {  // not a noise digi
        nHitPoints++;
        int iE = fMcEventList->GetEventIndex(link);
        if (iE < 0 || iE >= nMcEvents) {
          std::stringstream ss;
          ss << "link points to a non-existing MC event";
          hmc->SetErrorMsg(ss.str());
          LOG(error) << ss.str();
          return;
        }
        if (link.GetIndex() >= (int) pointNhits[iE].size()) {
          std::stringstream ss;
          ss << "link points to a non-existing MC index";
          hmc->SetErrorMsg(ss.str());
          LOG(error) << ss.str();
          return;
        }
        pointNhits[iE][link.GetIndex()]++;
      }
    }

    fhPointsPerHit[StationIndex].Fill(nHitPoints);

    // take corresponding MC point

    // skip hits from the noise digis
    if (bestLink.GetIndex() < 0) {
      std::stringstream ss;
      ss << "hit from noise [INFO]";
      hmc->SetErrorMsg(ss.str());
      continue;
    }

    CbmTrdPoint* p = dynamic_cast<CbmTrdPoint*>(fMcPoints->Get(bestLink));
    if (p == nullptr) {
      std::stringstream ss;
      ss << "link points to a non-existing MC point";
      hmc->SetErrorMsg(ss.str());
      LOG(error) << ss.str();
      return;
    }

    if (p->GetModuleAddress() != (int) CbmTrdAddress::GetModuleAddress(address)) {
      std::stringstream ss;
      ss << "mc point module address differs from the hit module address";
      hmc->SetErrorMsg(ss.str());
      LOG(error) << ss.str();
      return;
    }

    // check that the nominal station Z is not far from the active material
    {
      // the cut of 1 cm is choosen arbitrary and can be changed

      int ModuleId = fTrdDigiPar->GetModuleId(StationIndex);

      const CbmTrdParModGeo* pGeo = dynamic_cast<const CbmTrdParModGeo*>(fTrdGeoPar->GetModulePar(ModuleId));
      if (!pGeo) {
        LOG(fatal) << " No Geo params for station " << StationIndex << " module " << ModuleId << " found ";
        return;
      }

      double staZ = pGeo->GetZ();  // module->GetZ();  //+ 410;
      if ((staZ < p->GetZIn() - 1.) || (staZ > p->GetZOut() + 1.)) {
        std::stringstream ss;
        ss << "TRD station " << StationIndex << ": active material Z[" << p->GetZIn() << " cm," << p->GetZOut()
           << " cm] is too far from the nominal station Z " << staZ << " cm";
        hmc->SetErrorMsg(ss.str());
        LOG(error) << ss.str();
        return;
      }
      // the cut of 1 cm is choosen arbitrary and can be changed
      if (fabs(hit->GetZ() - staZ) > 1.) {
        std::stringstream ss;
        ss << "TRD station " << StationIndex << ": hit Z " << hit->GetZ()
           << " cm, is too far from the nominal station Z " << staZ << " cm";
        hmc->SetErrorMsg(ss.str());
        LOG(error) << ss.str();
        return;
      }
    }

    // residual and pull

    if (nHitPoints != 1) {
      std::stringstream ss;
      ss << "hit from mixed hit [INFO] nPoints=" << nHitPoints;
      hmc->SetErrorMsg(ss.str());
      continue;  // only check residual for non-mixed hits
    }

    double t0 = fMcEventList->GetEventTime(bestLink);
    if (t0 < 0) {
      std::stringstream ss;
      ss << "MC event time doesn't exist for a TRD link";
      hmc->SetErrorMsg(ss.str());
      LOG(error) << ss.str();
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
    if (fabs(p->GetPzOut()) < 0.01) continue;

    // transport the particle from the MC point to the hit Z
    double dz = hit->GetZ() - z;
    x += dz * px / pz;
    y += dz * py / pz;

    // get particle mass
    Int_t pdg(0);
    double mass = 0;
    {
      CbmLink mcTrackLink = bestLink;
      mcTrackLink.SetIndex(p->GetTrackID());
      CbmMCTrack* mcTrack = dynamic_cast<CbmMCTrack*>(fMcTracks->Get(mcTrackLink));
      if (!mcTrack) {
        std::stringstream ss;
        ss << "MC track " << p->GetTrackID() << " doesn't exist";
        hmc->SetErrorMsg(ss.str());
        LOG(error) << ss.str();
        return;
      }
      pdg = mcTrack->GetPdgCode();
      if (pdg < 9999999 && ((TParticlePDG*) TDatabasePDG::Instance()->GetParticle(pdg))) {
        mass = TDatabasePDG::Instance()->GetParticle(pdg)->Mass();
      }
    }
    hmc->AddPoint(p, t0, pdg);
    //std::cout << hmc->ToString() << "\n";
    constexpr double speedOfLight = 29.979246;  // cm/ns
    TVector3 mom3;
    p->Momentum(mom3);
    t += dz / (pz * speedOfLight) * sqrt(mass * mass + mom3.Mag2());

    double du = hit->GetX() - x;
    double dv = hit->GetY() - y;     //hmc->GetDy();  //hit->GetY() - y;
    double dt = hit->GetTime() - t;  // hmc->GetDt();  //hit->GetTime() - t;
    double su = hmc->GetDx();
    double sv = hit->GetDy();
    double st = hit->GetTimeError();

    // residuals
    if (hit->GetClassType() == 0) {
      if (StationIndex % 2) {
        std::swap(du, dv);
        std::swap(su, sv);
      }
      fh1DresidualU.Fill(du);
      fh1DresidualV.Fill(dv);
      fh1DresidualT.Fill(hit->GetTime() - t);
    }
    else {
      fh2DresidualX.Fill(du);
      fh2DresidualY.Fill(dv);
      fh2DresidualT.Fill(hit->GetTime() - t);
    }

    // pulls
    if ((su < 1.e-5) || (sv < 1.e-5) || (st < 1.e-5)) {
      std::stringstream ss;
      ss << "TRD hit errors are not properly set: errX " << hit->GetDx() << " errY " << hit->GetDy() << " errT ";
      hmc->SetErrorMsg(ss.str());
      LOG(error) << ss.str();
      return;
    }

    if (hit->GetClassType() == 0) {
      fh1DpullU.Fill(du / su);
      fh1DpullV.Fill(dv / sv);
      fh1DpullT.Fill(dt / st);
    }
    else {
      fh2DpullX.Fill(du / su);
      fh2DpullY.Fill(dv / sv);
      fh2DpullT.Fill(dt / st);
    }

  }  // iHit

  // fill efficiency: N hits per MC point

  for (int iE = 0; iE < nMcEvents; iE++) {
    int fileId  = fMcEventList->GetFileIdByIndex(iE);
    int eventId = fMcEventList->GetEventIdByIndex(iE);
    int nPoints = fMcPoints->Size(fileId, eventId);
    for (int ip = 0; ip < nPoints; ip++) {
      CbmTrdPoint* p = dynamic_cast<CbmTrdPoint*>(fMcPoints->Get(fileId, eventId, ip));
      if (p == nullptr) {
        LOG(error) << "MC point doesn't exist";
        return;
      }
      auto address     = p->GetModuleAddress();
      int StationIndex = CbmTrdAddress::GetLayerId(address);
      if (StationIndex < 0 || StationIndex >= fNtrackingStations) {
        LOG(fatal) << "TRD hit layer id " << StationIndex << " for module " << address << " is out of range";
        return;
      }
      fhHitsPerPoint[StationIndex].Fill(pointNhits[iE][ip]);
      fhEfficiencyR[StationIndex].Fill(sqrt(p->GetX() * p->GetX() + p->GetY() * p->GetY()), (pointNhits[iE][ip] > 0));
      fhEfficiencyXY[StationIndex].Fill(p->GetX(), p->GetY(), (pointNhits[iE][ip] > 0));
    }
  }
}


// -------------------------------------------------------------------------
TFolder& CbmTrdCalibTracker::GetQa()
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

  for (UInt_t i = 0; i < 6; i++) {
    fCanvResidual.cd(i + 1);
    fHistList[i]->DrawCopy("", "");
    fCanvPull.cd(i + 1);
    fHistList[6 + i]->DrawCopy("", "");
  }

  return fOutFolder;
}
