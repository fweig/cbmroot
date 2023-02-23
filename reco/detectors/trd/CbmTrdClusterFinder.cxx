/* Copyright (C) 2010-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer], Pascal Raisig, Alexandru Bercuci */

#include "CbmTrdClusterFinder.h"

#include "CbmDefs.h"
#include "CbmDigiManager.h"
#include "CbmTrdCluster.h"
#include "CbmTrdDigi.h"
#include "CbmTrdGeoHandler.h"
#include "CbmTrdModuleRec2D.h"
#include "CbmTrdModuleRecR.h"
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

#include <RtypesCore.h>
#include <TArray.h>
#include <TClonesArray.h>
#include <TGeoPhysicalNode.h>
#include <TStopwatch.h>
// #include "TCanvas.h"
// #include "TImage.h"

#include <iomanip>
#include <iostream>

#include <cmath>

using std::fixed;
using std::left;
using std::right;
using std::setprecision;
using std::setw;
using std::stringstream;


Int_t CbmTrdClusterFinder::fgConfig            = 0;
Float_t CbmTrdClusterFinder::fgMinimumChargeTH = .5e-06;
//_____________________________________________________________________
CbmTrdClusterFinder::CbmTrdClusterFinder() : FairTask("TrdClusterFinder", 1) { SetUseOnlyEventDigis(); }
// --------------------------------------------------------------------

// ---- Destructor ----------------------------------------------------
CbmTrdClusterFinder::~CbmTrdClusterFinder()
{

  if (fClusters) {
    fClusters->Clear("C");
    fClusters->Delete();
    delete fClusters;
  }
  // if (fGeoPar) { delete fGeoPar; }
  //   if(fModuleInfo){
  //     delete fModuleInfo;
  //   }
}

//_____________________________________________________________________
Bool_t CbmTrdClusterFinder::AddCluster(CbmTrdCluster* c)
{
  Int_t ncl(fClusters->GetEntriesFast());
  new ((*fClusters)[ncl++]) CbmTrdCluster(*c);
  return kTRUE;
}

// ---- addDigisToModules ----
UInt_t CbmTrdClusterFinder::addDigisToModules()
{
  UInt_t ndigis = static_cast<UInt_t>(std::abs(CbmDigiManager::Instance()->GetNofDigis(ECbmModuleId::kTrd)));
  if (ndigis == 0) return 0;
  for (size_t idigi = 0; idigi < ndigis; idigi++) {
    addDigiToModule(idigi);
  }
  return ndigis;
}

// ---- addDigisToModules ----
UInt_t CbmTrdClusterFinder::addDigisToModules(CbmEvent* event)
{
  UInt_t ndigis = static_cast<UInt_t>(std::abs(event->GetNofData(ECbmDataType::kTrdDigi)));
  if (ndigis == 0) return 0;
  for (size_t idigi = 0; idigi < ndigis; idigi++) {
    auto digiindex = event->GetIndex(ECbmDataType::kTrdDigi, idigi);
    addDigiToModule(digiindex);
  }
  return ndigis;
}


// ---- addDigiToModule ----
void CbmTrdClusterFinder::addDigiToModule(UInt_t digiIdx)
{
  CbmTrdModuleRec* mod = nullptr;

  const CbmTrdDigi* digi = CbmDigiManager::Instance()->Get<CbmTrdDigi>(digiIdx);
  if (!digi) return;
  Int_t moduleAddress = digi->GetAddressModule();

  std::map<Int_t, CbmTrdModuleRec*>::iterator imod = fModules.find(moduleAddress);
  if (imod == fModules.end()) mod = AddModule(digi);
  else
    mod = imod->second;

  mod->AddDigi(digi, digiIdx);
}

// ---- processDigisInModules ----
void CbmTrdClusterFinder::processDigisInModules(UInt_t ndigis, CbmEvent* event)
{
  CbmTrdModuleRec* mod(NULL);
  Int_t digiCounter(0), clsCounter(0);
  for (std::map<Int_t, CbmTrdModuleRec*>::iterator imod = fModules.begin(); imod != fModules.end(); imod++) {
    mod = imod->second;
    digiCounter += mod->GetOverThreshold();
    clsCounter += mod->FindClusters();
    AddClusters(mod->GetClusters(), event, kTRUE);
  }

  // remove local data from all modules
  for (std::map<Int_t, CbmTrdModuleRec*>::iterator imod = fModules.begin(); imod != fModules.end(); imod++)
    imod->second->Clear("cls");

  fNrDigis += ndigis;
  fNrClusters += clsCounter;

  if (DoDebugPrintouts()) {
    LOG(info) << GetName() << "::Exec : Digis    : " << ndigis << " / " << digiCounter << " above threshold ("
              << 1e6 * fgMinimumChargeTH << " keV)";
    LOG(info) << GetName() << "::Exec : Clusters : " << clsCounter;
  }
}

//____________________________________________________________________________________
CbmTrdModuleRec* CbmTrdClusterFinder::AddModule(const CbmTrdDigi* digi)
{
  Int_t address = digi->GetAddressModule();
  CbmTrdModuleRec* module(NULL);
  if (digi->GetType() == CbmTrdDigi::eCbmTrdAsicType::kFASP)
    module = fModules[address] = new CbmTrdModuleRec2D(address);
  else
    module = fModules[address] = new CbmTrdModuleRecR(address);
  LOG(debug) << GetName() << "::AddModule : " << module->GetName();

  // try to load Geometry parameters for module
  const CbmTrdParModGeo* pGeo(NULL);
  if (!fGeoPar || !(pGeo = (const CbmTrdParModGeo*) fGeoPar->GetModulePar(address))) {
    LOG(fatal) << GetName() << "::AddModule : No Geo params for module " << address << ". Using default.";
  }
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
  if (digi->GetType() == CbmTrdDigi::eCbmTrdAsicType::kFASP) {
    const CbmTrdParModGain* pGain(NULL);
    if (!fGainPar || !(pGain = (const CbmTrdParModGain*) fGainPar->GetModulePar(address))) {
      //LOG(warn) << GetName() << "::AddModule : No Gain params for modAddress "<< address <<". Using default.";
    }
    else
      module->SetGainPar(pGain);
  }
  return module;
}

//_____________________________________________________________________
void CbmTrdClusterFinder::SetParContainers()
{
  FairRuntimeDb* rtdb = FairRunAna::Instance()->GetRuntimeDb();
  fAsicPar            = static_cast<CbmTrdParSetAsic*>(rtdb->getContainer("CbmTrdParSetAsic"));
  fGasPar             = static_cast<CbmTrdParSetGas*>(rtdb->getContainer("CbmTrdParSetGas"));
  fDigiPar            = static_cast<CbmTrdParSetDigi*>(rtdb->getContainer("CbmTrdParSetDigi"));
  fGainPar            = static_cast<CbmTrdParSetGain*>(rtdb->getContainer("CbmTrdParSetGain"));
  fGeoPar             = static_cast<CbmTrdParSetGeo*>(rtdb->getContainer("CbmTrdParSetGeo"));
}

//_____________________________________________________________________
InitStatus CbmTrdClusterFinder::Init()
{

  FairRootManager* ioman = FairRootManager::Instance();


  CbmDigiManager::Instance()->Init();
  if (!CbmDigiManager::Instance()->IsPresent(ECbmModuleId::kTrd)) LOG(fatal) << GetName() << "Missing Trd digi branch.";

  fClusters = new TClonesArray("CbmTrdCluster", 100);
  ioman->Register("TrdCluster", "TRD", fClusters, IsOutputBranchPersistent("TrdCluster"));

  // If not deactivated by the user, the clusterizer will look for the CbmEvent branch, to only use Digis connected to a CbmEvent. If no CbmEvent branch is found all digis in the TrdDigi branch are automatically used.
  if (UseOnlyEventDigis()) {
    fEvents = dynamic_cast<TClonesArray*>(ioman->GetObject("CbmEvent"));
    if (fEvents == nullptr) { SetUseOnlyEventDigis(kFALSE); }
  }
  if (!IsTimeBased() && nullptr == dynamic_cast<TClonesArray*>(ioman->GetObject("CbmEvent"))) {
    LOG(warn) << GetName()
              << ": Event mode selected but no event array found! Run in "
                 "time-based mode.";
    SetTimeBased();
  }

  //   // Get the full geometry information of the detector gas layers and store
  //   // them with the CbmTrdModuleRec. This information can then be used for
  //   // transformation calculations
  //   std::map<Int_t, TGeoPhysicalNode*> moduleMap = fGeoHandler->FillModuleMap();
  //
  //   Int_t nrModules = fDigiPar->GetNrOfModules();
  //   Int_t nrNodes = moduleMap.size();
  //   if (nrModules != nrNodes) LOG(fatal) << "Geometry and parameter files have different number of modules.";
  //   for (Int_t loop=0; loop< nrModules; ++loop) {
  //      Int_t address = fDigiPar->GetModuleId(loop);
  //      std::map<Int_t, TGeoPhysicalNode*>::iterator it = moduleMap.find(address);
  //      if ( it  == moduleMap.end() ) {
  //        LOG(fatal) << "Expected module with address " << address << " wasn't found in the map with TGeoNode information.";
  //      }
  //      AddModule(address, it->second);
  //   }

  //   // new call needed when parameters are initialized from ROOT file
  //   fDigiPar->Initialize();

  LOG(info) << "================ TRD Cluster Finder ===============";
  LOG(info) << " Free streaming       : " << (IsTimeBased() ? "yes" : "no");
  LOG(info) << " Multi hit detect     : " << (HasMultiHit() ? "yes" : "no");
  LOG(info) << " Row merger           : " << (HasRowMerger() ? "yes" : "no");
  LOG(info) << " c-Neighbour enable   : " << (HasNeighbourCol() ? "yes" : "no");
  LOG(info) << " r-Neighbour enable   : " << (HasNeighbourRow() ? "yes" : "no");
  LOG(info) << " Write clusters       : " << (HasDumpClusters() ? "yes" : "no");
  LOG(info) << " Use only event digis : " << (UseOnlyEventDigis() ? "yes" : "no");

  return kSUCCESS;
}

//_____________________________________________________________________
void CbmTrdClusterFinder::Exec(Option_t* /*option*/)
{
  /**
  * Digis are sorted according to the moduleAddress. A combiId is calculted based
  * on the rowId and the colId to have a neighbouring criterion for digis within
  * the same pad row. The digis of each module are sorted according to this combiId.
  * All sorted digis of one pad row are 'clustered' into rowCluster. For a new row
  * the rowClusters are compared to the rowClusters of the last row. If an overlap
  * is found they are marked to be parents(last row) and childrens(activ row)
  * (mergeRowCluster()). After this, the finale clusters are created. Therefor
  * walkCluster() walks along the list of marked parents and markes every visited
  * rowCluster to avoid multiple usage of one rowCluster. drawCluster() can be used to
  * get a visual output.
  */

  fClusters->Delete();

  TStopwatch timer;
  TStopwatch timerTs;
  timerTs.Start();
  Long64_t nDigisAll  = CbmDigiManager::Instance()->GetNofDigis(ECbmModuleId::kTrd);
  Long64_t nDigisUsed = 0;
  UInt_t nDigis       = 0;
  UInt_t nEvents      = 0;

  if (UseOnlyEventDigis()) {
    for (auto eventobj : *fEvents) {
      timer.Start();
      auto event = static_cast<CbmEvent*>(eventobj);
      nDigis     = addDigisToModules(event);
      processDigisInModules(nDigis, event);
      fNrEvents++;
      nEvents++;
      nDigisUsed += nDigis;
      timer.Stop();
      if (DoDebugPrintouts()) {
        LOG(info) << GetName() << "::Exec : Event Nr: " << fNrEvents;
        LOG(info) << GetName() << "::Exec : real time=" << timer.RealTime() << " CPU time=" << timer.CpuTime();
      }
      fProcessTime += timer.RealTime();
      timer.Reset();
    }
  }

  if (!UseOnlyEventDigis()) {
    timer.Start();
    nDigis = addDigisToModules();
    processDigisInModules(nDigis);
    fNrEvents++;
    nDigisUsed = nDigis;
    timer.Stop();
    if (DoDebugPrintouts()) {
      LOG(info) << GetName() << "::Exec : Event Nr: " << fNrEvents;
      LOG(info) << GetName() << "::Exec : real time=" << timer.RealTime() << " CPU time=" << timer.CpuTime();
    }
    fProcessTime += timer.RealTime();
    timer.Reset();
  }

  timerTs.Stop();
  stringstream logOut;
  logOut << setw(20) << left << GetName() << " [";
  logOut << fixed << setw(8) << setprecision(1) << right << timerTs.RealTime() * 1000. << " ms] ";
  logOut << "TS " << fNrTs;
  if (UseOnlyEventDigis()) logOut << ", events " << nEvents;
  logOut << ", digis " << nDigisUsed << " / " << nDigisAll;
  LOG(info) << logOut.str();
  fNrTs++;
}

//_____________________________________________________________________
Int_t CbmTrdClusterFinder::AddClusters(TClonesArray* clusters, CbmEvent* event, Bool_t /* move*/)
{
  if (!clusters) return 0;
  CbmTrdCluster *cls(NULL), *clsSave(NULL);
  const CbmTrdDigi* digi(NULL);
  CbmTrdParModDigi* digiPar(NULL);
  TBits cols, rows;
  Int_t ncl(fClusters->GetEntriesFast()), mcl(0), ncols(0);

  for (Int_t ic(0); ic < clusters->GetEntriesFast(); ic++) {
    if (!(cls = (CbmTrdCluster*) (*clusters)[ic])) continue;

    if (!cls->HasFaspDigis()) {  // only for rectangular/SPADIC clusters
      if (!ncols) {
        digiPar = (CbmTrdParModDigi*) fDigiPar->GetModulePar(cls->GetAddress());
        if (!digiPar) {
          LOG(error) << "CbmTrdClusterFinder::AddClusters : Can't find "
                        "ParModDigi for address"
                     << cls->GetAddress();
          continue;
        }
        ncols = digiPar->GetNofColumns();
      }
      cols.Clear();
      rows.Clear();
      for (Int_t id = 0; id < cls->GetNofDigis(); id++) {
        digi              = CbmDigiManager::Instance()->Get<CbmTrdDigi>(cls->GetDigi(id));
        Int_t digiChannel = digi->GetAddressChannel();
        Int_t colId       = digiChannel % ncols;
        Int_t globalRow   = digiChannel / ncols;

        Int_t combiId = globalRow * ncols + colId;
        cols.SetBitNumber(combiId);
        rows.SetBitNumber(globalRow);
      }
      // store information in cluster
      cls->SetNCols(cols.CountBits());
      cls->SetNRows(rows.CountBits());
    }
    clsSave = new ((*fClusters)[ncl]) CbmTrdCluster(*cls);  // TODO implement copy constructor
    // In case we have an event branch and we did only use digis from within the event, add the cluster to the event. This allows the hit producer to identify wether or not to add the corresponding hit to the event.
    if (event) event->AddData(ECbmDataType::kTrdCluster, ncl);
    ncl++;
    clsSave->SetFaspDigis(cls->HasFaspDigis());
    if (cls->GetMatch() != NULL)
      delete cls;  //only the matches have pointers to allocated memory, so otherwise the clear does the trick
    mcl++;
  }
  //clusters->Clear();
  return mcl;
}

//_____________________________________________________________________
void CbmTrdClusterFinder::Finish()
{
  std::cout << std::endl;
  LOG(info) << "=====================================";
  LOG(info) << GetName() << ": Finish run";
  LOG(info) << GetName() << ": Run summary ";
  LOG(info) << GetName() << ": Processing time         : " << std::fixed << std::setprecision(3) << fProcessTime;
  LOG(info) << GetName() << ": Nr of events            : " << fNrEvents;
  LOG(info) << GetName() << ": Nr of input digis       : " << fNrDigis;
  LOG(info) << GetName() << ": Nr of produced clusters : " << fNrClusters;
  LOG(info) << GetName() << ": Nr of clusters / event  : " << std::fixed << std::setprecision(2)
            << (fNrEvents > 0 ? fNrClusters / (Double_t) fNrEvents : 0);
  LOG(info) << GetName() << ": Nr of digis / cluster   : " << std::fixed << std::setprecision(2)
            << (fNrClusters > 0 ? fNrDigis / (Double_t) fNrClusters : 0);
  LOG(info) << "=====================================";
  std::cout << std::endl;
}

ClassImp(CbmTrdClusterFinder)
