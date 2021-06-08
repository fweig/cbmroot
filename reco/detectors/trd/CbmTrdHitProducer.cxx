/* Copyright (C) 2018-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig, Florian Uhlig [committer] */

#include "CbmTrdHitProducer.h"

#include "CbmDefs.h"
#include "CbmDigiManager.h"
#include "CbmTrdAddress.h"
#include "CbmTrdClusterFinder.h"
#include "CbmTrdDigi.h"
#include "CbmTrdGeoHandler.h"
#include "CbmTrdHit.h"
#include "CbmTrdModuleRec.h"
#include "CbmTrdModuleRecR.h"
#include "CbmTrdModuleRecT.h"
#include "CbmTrdParAsic.h"
#include "CbmTrdParModDigi.h"
#include "CbmTrdParModGain.h"
#include "CbmTrdParModGas.h"
#include "CbmTrdParSetAsic.h"
#include "CbmTrdParSetDigi.h"
#include "CbmTrdParSetGain.h"
#include "CbmTrdParSetGas.h"
#include "CbmTrdParSetGeo.h"

#include <FairRootManager.h>
#include <FairRunAna.h>
#include <FairRuntimeDb.h>
#include <Logger.h>

#include <TGeoManager.h>
#include <TGeoPhysicalNode.h>
#include <TStopwatch.h>
#include <TVector3.h>

#include <iomanip>
#include <map>

using std::fixed;
using std::left;
using std::right;
using std::setprecision;
using std::setw;
using std::stringstream;

//____________________________________________________________________________________
CbmTrdHitProducer::CbmTrdHitProducer() : FairTask("TrdHitProducer") {}

//____________________________________________________________________________________
CbmTrdHitProducer::~CbmTrdHitProducer()
{
  fHits->Clear();
  delete fHits;
  if (fGeoPar) delete fGeoPar;
}

//____________________________________________________________________________________
void CbmTrdHitProducer::addModuleHits(CbmTrdModuleRec* mod, Int_t* hitCounter, CbmEvent* event)
{

  /** Absorb the TClonesArrays of the individual modules into the global
      TClonesArray.
   */

  if (!mod) return;

  auto hits = mod->GetHits();

  if (!hits) return;

  while (!hits->IsEmpty()) {
    fHits->AbsorbObjects(hits, 0, 0);
    if (event) event->AddData(ECbmDataType::kTrdHit, *hitCounter);
    (*hitCounter)++;
    fNrHits++;
  }
}

//____________________________________________________________________________________
CbmTrdModuleRec* CbmTrdHitProducer::AddModule(Int_t address, TGeoPhysicalNode* node)
{
  TString s(node->GetName());
  Int_t typ = TString(s[s.Index("module") + 6]).Atoi();

  CbmTrdModuleRec* module(NULL);
  if (typ == 9) { module = fModules[address] = new CbmTrdModuleRecT(address); }
  else {
    module = fModules[address] = new CbmTrdModuleRecR(address);
  }

  // Try to load geometry parameters for the module
  const CbmTrdParModGeo* pGeo = nullptr;
  if (!fGeoPar) LOG(warn) << GetName() << ": No geometry parameter container!";
  else
    pGeo = (const CbmTrdParModGeo*) fGeoPar->GetModulePar(address);
  if (!pGeo) LOG(warn) << GetName() << ": No geometry parameters for module " << address;
  else
    module->SetGeoPar(pGeo);


  // try to load read-out parameters for module
  const CbmTrdParModDigi* pDigi(NULL);
  if (!fDigiPar || !(pDigi = (const CbmTrdParModDigi*) fDigiPar->GetModulePar(address))) {
    LOG(warn) << GetName() << "::AddModule : No Read-Out params for modAddress " << address << ". Using default.";
  }
  else
    module->SetDigiPar(pDigi);

  // try to load ASIC parameters for module
  CbmTrdParSetAsic* pAsic(NULL);
  if (!fAsicPar || !(pAsic = (CbmTrdParSetAsic*) fAsicPar->GetModuleSet(address))) {
    LOG(warn) << GetName() << "::AddModule : No ASIC params for modAddress " << address << ". Using default.";
    //    module->SetAsicPar(); // map ASIC channels to read-out channels - need ParModDigi already loaded
  }
  else
    module->SetAsicPar(pAsic);

  // try to load Chamber parameters for module
  const CbmTrdParModGas* pChmb(NULL);
  if (!fGasPar || !(pChmb = (const CbmTrdParModGas*) fGasPar->GetModulePar(address))) {
    LOG(warn) << GetName() << "::AddModule : No Gas params for modAddress " << address << ". Using default.";
  }
  else
    module->SetChmbPar(pChmb);

  // try to load Gain parameters for module
  if (typ == 9) {
    const CbmTrdParModGain* pGain(NULL);
    if (!fGainPar || !(pGain = (const CbmTrdParModGain*) fGainPar->GetModulePar(address))) {
      //LOG(warn) << GetName() << "::AddModule : No Gain params for modAddress "<< address <<". Using default.";
    }
    else
      module->SetGainPar(pGain);
  }
  return module;
}


// ---- processCluster ----
UInt_t CbmTrdHitProducer::processClusters()
{
  Int_t nclusters = fClusters->GetEntriesFast();

  for (Int_t icluster = 0; icluster < nclusters; icluster++) {
    processCluster(icluster);
  }
  auto nhits = addHits();
  if (CbmTrdClusterFinder::DoDebugPrintouts()) {
    LOG(info) << GetName() << "::processClusters: "
              << " Clusters : " << nclusters;
    LOG(info) << GetName() << "::processClusters: "
              << " Hits     : " << nhits;
  }
  return nhits;
}

// ---- processCluster ----
UInt_t CbmTrdHitProducer::processClusters(CbmEvent* event)
{
  Int_t nclusters = event->GetNofData(ECbmDataType::kTrdCluster);

  for (Int_t icluster = 0; icluster < nclusters; icluster++) {
    auto clusterIdx = event->GetIndex(ECbmDataType::kTrdCluster, icluster);
    processCluster(clusterIdx);
  }
  auto nhits = addHits(event);

  if (CbmTrdClusterFinder::DoDebugPrintouts()) {
    LOG(info) << GetName() << "::processClusters: "
              << " Clusters : " << nclusters;
    LOG(info) << GetName() << "::processClusters: "
              << " Hits     : " << nhits;
  }
  return nhits;
}


// ---- processCluster ----
void CbmTrdHitProducer::processCluster(const Int_t clusterIdx)
{
  auto cluster = static_cast<const CbmTrdCluster*>(fClusters->At(clusterIdx));
  if (!cluster) return;

  // get/build module for current cluster
  auto imod = fModules.find(cluster->GetAddress());
  auto mod  = imod->second;

  std::vector<const CbmTrdDigi*> digivec = {};
  // get digis for current cluster
  for (Int_t iDigi = 0; iDigi < cluster->GetNofDigis(); iDigi++) {
    const CbmTrdDigi* digi = CbmDigiManager::Instance()->Get<CbmTrdDigi>(cluster->GetDigi(iDigi));

    if (digi->GetType() == CbmTrdDigi::kSPADIC && digi->GetCharge() <= 0) continue;
    digivec.emplace_back(digi);
  }

  mod->MakeHit(clusterIdx, cluster, &digivec);

  fNrClusters++;
}

// ---- addHits ----
Int_t CbmTrdHitProducer::addHits(CbmEvent* event)
{
  Int_t hitCounter(0);
  for (std::map<Int_t, CbmTrdModuleRec*>::iterator imod = fModules.begin(); imod != fModules.end(); imod++) {
    auto mod = imod->second;

    mod->Finalize();

    addModuleHits(mod, &hitCounter, event);
  }

  // AbsorberObjects takes care of cleaning up.
  // Hence, remove local data from all modules is not needed
  return hitCounter;
}

//____________________________________________________________________________________
InitStatus CbmTrdHitProducer::Init()
{
  FairRootManager* ioman = FairRootManager::Instance();
  if (NULL == ioman) {
    LOG(error) << GetName() << "::Init: "
               << "ROOT manager is not instantiated!";
    return kFATAL;
  }

  CbmDigiManager::Instance()->Init();
  if (!CbmDigiManager::Instance()->IsPresent(ECbmModuleId::kTrd)) LOG(fatal);

  fClusters = (TClonesArray*) ioman->GetObject("TrdCluster");
  if (!fClusters) {
    LOG(error) << GetName() << "::Init: "
               << "no TrdCluster array!";
    return kFATAL;
  }

  fHits = new TClonesArray("CbmTrdHit", 100);
  ioman->Register("TrdHit", "TRD", fHits, IsOutputBranchPersistent("TrdHit"));

  // If not deactivated by the user, the hitproducer will look for the CbmEvent branch, to only use Digis connected to a CbmEvent. If no CbmEvent branch is found all digis in the TrdDigi branch are automatically used.
  if (CbmTrdClusterFinder::UseOnlyEventDigis()) {
    fEvents = dynamic_cast<TClonesArray*>(ioman->GetObject("CbmEvent"));
    if (fEvents == nullptr) { CbmTrdClusterFinder::SetUseOnlyEventDigis(kFALSE); }
  }

  // Get the full geometry information of the detector gas layers and store
  // them with the CbmTrdModuleRec. This information can then be used for
  // transformation calculations
  CbmTrdGeoHandler geoHandler;
  std::map<Int_t, TGeoPhysicalNode*> moduleMap = geoHandler.FillModuleMap();

  Int_t nrModules = fDigiPar->GetNrOfModules();
  Int_t nrNodes   = moduleMap.size();
  if (nrModules != nrNodes)
    LOG(fatal) << "CbmTrdHitProducer::Init() - Geometry(" << nrNodes << ") and parameter files(" << nrModules
               << ") have different number of modules.";
  for (Int_t loop = 0; loop < nrModules; ++loop) {
    Int_t address                                   = fDigiPar->GetModuleId(loop);
    std::map<Int_t, TGeoPhysicalNode*>::iterator it = moduleMap.find(address);
    if (it == moduleMap.end()) {
      LOG(fatal) << "Expected module with address " << address << " wasn't found in the map with TGeoNode information.";
    }
    AddModule(address, it->second);
  }
  return kSUCCESS;
}

//____________________________________________________________________________________
void CbmTrdHitProducer::Exec(Option_t*)
{
  fHits->Delete();

  TStopwatch timer;
  TStopwatch timerTs;
  timerTs.Start();

  Long64_t nClusters  = fClusters->GetEntriesFast();
  UInt_t hitCounterTs = 0;
  UInt_t nEvents      = 0;

  if (CbmTrdClusterFinder::UseOnlyEventDigis()) {
    for (auto eventobj : *fEvents) {
      timer.Start();
      auto event = static_cast<CbmEvent*>(eventobj);
      if (!event) continue;
      hitCounterTs += processClusters(event);
      fNrEvents++;
      nEvents++;
      timer.Stop();
      if (CbmTrdClusterFinder::DoDebugPrintouts()) {
        LOG(info) << GetName() << "::Exec : Event Nr: " << fNrEvents;
        LOG(info) << GetName() << "::Exec : real time=" << timer.RealTime() << " CPU time=" << timer.CpuTime();
      }
      fProcessTime += timer.RealTime();
      timer.Reset();
    }
  }

  if (!CbmTrdClusterFinder::UseOnlyEventDigis()) {
    timer.Start();
    hitCounterTs = processClusters();
    fNrEvents++;
    timer.Stop();
    if (CbmTrdClusterFinder::DoDebugPrintouts()) {
      LOG(info) << GetName() << "::Exec : Event Nr: " << fNrEvents;
      LOG(info) << GetName() << "::Exec : real time=" << timer.RealTime() << " CPU time=" << timer.CpuTime();
    }
    fProcessTime += timer.RealTime();
    timer.Reset();
  }


  timer.Stop();
  timerTs.Stop();
  if (CbmTrdClusterFinder::DoDebugPrintouts())
    LOG(info) << GetName() << "::Exec: real time=" << timer.RealTime() << " CPU time=" << timer.CpuTime();
  fProcessTime += timer.RealTime();

  stringstream logOut;
  logOut << setw(20) << left << GetName() << " [";
  logOut << fixed << setw(8) << setprecision(1) << right << timerTs.RealTime() * 1000. << " ms] ";
  logOut << "TS " << fNrTs;
  if (CbmTrdClusterFinder::UseOnlyEventDigis()) logOut << ", events " << nEvents;
  logOut << ", clusters " << nClusters << ", hits " << hitCounterTs;
  LOG(info) << logOut.str();
  fNrTs++;
}

//____________________________________________________________________________________
void CbmTrdHitProducer::Finish()
{
  std::cout << std::endl;
  LOG(info) << "=====================================";
  LOG(info) << GetName() << ": Finish run";
  LOG(info) << GetName() << ": Run summary ";
  LOG(info) << GetName() << ": Processing time      : " << std::fixed << std::setprecision(3) << fProcessTime;
  LOG(info) << GetName() << ": Nr of events         : " << fNrEvents;
  LOG(info) << GetName() << ": Nr of input clusters : " << fNrClusters;
  LOG(info) << GetName() << ": Nr of produced hits  : " << fNrHits;
  LOG(info) << GetName() << ": Nr of hits / event   : " << std::fixed << std::setprecision(2)
            << (fNrEvents > 0 ? fNrHits / (Double_t) fNrEvents : 0);
  LOG(info) << GetName() << ": Nr of hits / clusters: " << std::fixed << std::setprecision(2)
            << (fNrClusters > 0 ? fNrHits / (Double_t) fNrClusters : 0);
  LOG(info) << "=====================================";
  std::cout << std::endl;
}

//________________________________________________________________________________________
void CbmTrdHitProducer::SetParContainers()
{
  fAsicPar = static_cast<CbmTrdParSetAsic*>(FairRunAna::Instance()->GetRuntimeDb()->getContainer("CbmTrdParSetAsic"));
  fGasPar  = static_cast<CbmTrdParSetGas*>(FairRunAna::Instance()->GetRuntimeDb()->getContainer("CbmTrdParSetGas"));
  fDigiPar = static_cast<CbmTrdParSetDigi*>(FairRunAna::Instance()->GetRuntimeDb()->getContainer("CbmTrdParSetDigi"));
  fGainPar = static_cast<CbmTrdParSetGain*>(FairRunAna::Instance()->GetRuntimeDb()->getContainer("CbmTrdParSetGain"));
  fGeoPar  = new CbmTrdParSetGeo();
}


ClassImp(CbmTrdHitProducer);
