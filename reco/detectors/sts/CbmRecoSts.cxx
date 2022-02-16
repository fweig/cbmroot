/* Copyright (C) 2020-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

/** @file CbmRecoSts.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 22.03.2020
 **/

#include "CbmRecoSts.h"

#include "CbmAddress.h"
#include "CbmDigiManager.h"
#include "CbmEvent.h"
#include "CbmStsDigi.h"
#include "CbmStsModule.h"
#include "CbmStsParSetModule.h"
#include "CbmStsParSetSensor.h"
#include "CbmStsParSetSensorCond.h"
#include "CbmStsParSim.h"
#include "CbmStsRecoModule.h"
#include "CbmStsSetup.h"

#include <FairField.h>
#include <FairRun.h>
#include <FairRuntimeDb.h>

#include <TClonesArray.h>
#include <TGeoBBox.h>
#include <TGeoPhysicalNode.h>

#include <iomanip>

using std::fixed;
using std::left;
using std::right;
using std::setprecision;
using std::setw;
using std::stringstream;
using std::vector;


ClassImp(CbmRecoSts)


  // -----   Constructor   ---------------------------------------------------
  CbmRecoSts::CbmRecoSts(ECbmRecoMode mode, Bool_t writeClusters, Bool_t runParallel)
  : FairTask("RecoSts", 1)
  , fMode(mode)
  , fWriteClusters(writeClusters)
  , fRunParallel(runParallel)
{
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmRecoSts::~CbmRecoSts() {}
// -------------------------------------------------------------------------


// -----   Initialise the cluster finding modules   ------------------------
UInt_t CbmRecoSts::CreateModules()
{

  assert(fSetup);
  for (Int_t iModule = 0; iModule < fSetup->GetNofModules(); iModule++) {

    // --- Setup module and sensor
    CbmStsModule* setupModule = fSetup->GetModule(iModule);
    assert(setupModule);
    Int_t moduleAddress = Int_t(setupModule->GetAddress());
    assert(setupModule->GetNofDaughters() == 1);
    CbmStsElement* setupSensor = setupModule->GetDaughter(0);
    assert(setupSensor);
    Int_t sensorAddress = Int_t(setupSensor->GetAddress());

    // --- Parameter sets from database or user-defined
    CbmStsParSetModule* modulePars      = (fUserParSetModule ? fUserParSetModule : fParSetModule);
    CbmStsParSetSensor* sensorPars      = (fUserParSetSensor ? fUserParSetSensor : fParSetSensor);
    CbmStsParSetSensorCond* sensorConds = (fUserParSetCond ? fUserParSetCond : fParSetCond);

    // --- Module parameters
    const CbmStsParModule& modPar = (fUserParModule ? *fUserParModule : modulePars->GetParModule(moduleAddress));

    // --- Sensor parameters
    const CbmStsParSensor& sensPar = (fUserParSensor ? *fUserParSensor : sensorPars->GetParSensor(sensorAddress));

    // --- Sensor conditions
    const CbmStsParSensorCond& sensCond =
      (fUserParSensorCond ? *fUserParSensorCond : sensorConds->GetParSensor(sensorAddress));

    // --- Calculate and set average Lorentz shift
    // --- This will be used in hit finding for correcting the position.
    Double_t lorentzF = 0.;
    Double_t lorentzB = 0.;
    if (fParSim->LorentzShift()) {

      TGeoBBox* shape = dynamic_cast<TGeoBBox*>(setupSensor->GetPnode()->GetShape());
      assert(shape);
      Double_t dZ = 2. * shape->GetDZ();  // Sensor thickness

      // Get the magnetic field in the sensor centre
      Double_t by = 0.;
      if (FairRun::Instance()->GetField()) {
        Double_t local[3] = {0., 0., 0.};  // sensor centre in local C.S.
        Double_t global[3];                // sensor centre in global C.S.
        setupSensor->GetPnode()->GetMatrix()->LocalToMaster(local, global);
        Double_t field[3] = {0., 0., 0.};  // magnetic field components
        FairRun::Instance()->GetField()->Field(global, field);
        by = field[1] / 10.;  // kG->T
      }                       //? field present

      // Calculate average Lorentz shift on sensor sides.
      // This is needed in hit finding for correcting the cluster position.
      auto lorentzShift = LorentzShift(sensCond, dZ, by);
      lorentzF          = lorentzShift.first;
      lorentzB          = lorentzShift.second;
    }  //? Lorentz-shift correction

    // --- Create reco module
    CbmStsRecoModule* recoModule = new CbmStsRecoModule(setupModule, modPar, sensPar, lorentzF, lorentzB);
    assert(recoModule);

    recoModule->SetTimeCutDigisAbs(fTimeCutDigisAbs);
    recoModule->SetTimeCutDigisSig(fTimeCutDigisSig);
    recoModule->SetTimeCutClustersAbs(fTimeCutClustersAbs);
    recoModule->SetTimeCutClustersSig(fTimeCutClustersSig);

    auto result = fModules.insert({moduleAddress, recoModule});
    assert(result.second);
    fModuleIndex.push_back(recoModule);
  }

  return fModules.size();
}
// -------------------------------------------------------------------------


// -----   Task execution   ------------------------------------------------
void CbmRecoSts::Exec(Option_t*)
{

  // --- Time
  TStopwatch timer;
  timer.Start();

  // --- Clear hit output array
  fHits->Delete();

  // --- Reset cluster output array
  fClusters->Delete();

  // --- Local variables
  Int_t nEvents    = 0;
  fNofDigis        = 0;
  fNofDigisUsed    = 0;
  fNofDigisIgnored = 0;
  fNofClusters     = 0;
  fNofHits         = 0;
  fTimeTot         = 0.;
  fTime1           = 0.;
  fTime2           = 0.;
  fTime3           = 0.;
  fTime4           = 0.;
  CbmEvent* event  = nullptr;

  // --- Time-slice mode: process entire array
  if (fMode == kCbmRecoTimeslice) ProcessData(nullptr);

  // --- Event mode: loop over events
  else {
    assert(fEvents);
    nEvents = fEvents->GetEntriesFast();
    LOG(debug) << setw(20) << left << GetName() << ": Processing time slice " << fNofTs << " with " << nEvents
               << (nEvents == 1 ? " event" : " events");
    for (Int_t iEvent = 0; iEvent < nEvents; iEvent++) {
      event = dynamic_cast<CbmEvent*>(fEvents->At(iEvent));
      assert(event);
      ProcessData(event);
    }  //# events
  }    //? event mode

  // --- Timeslice log
  timer.Stop();
  stringstream logOut;
  logOut << setw(20) << left << GetName() << " [";
  logOut << fixed << setw(8) << setprecision(1) << right << timer.RealTime() * 1000. << " ms] ";
  logOut << "TS " << fNofTs;
  if (fEvents) logOut << ", events " << nEvents;
  logOut << ", digis " << fNofDigisUsed << " / " << fNofDigis;
  logOut << ", clusters " << fNofClusters << ", hits " << fNofHits;
  LOG(info) << logOut.str();

  // --- Update run counters
  fNofTs++;
  fNofEvents += nEvents;
  fNofDigisRun += fNofDigis;
  fNofDigisUsedRun += fNofDigisUsed;
  fNofClustersRun += fNofClusters;
  fNofHitsRun += fNofHits;
  fTimeRun += fTimeTot;
  fTime1Run += fTime1;
  fTime2Run += fTime2;
  fTime3Run += fTime3;
  fTime4Run += fTime4;
}
// -------------------------------------------------------------------------


// -----   End-of-run action   ---------------------------------------------
void CbmRecoSts::Finish()
{

  std::cout << std::endl;
  Double_t digiCluster = Double_t(fNofDigisUsed) / Double_t(fNofClusters);
  Double_t clusterHit  = Double_t(fNofClusters) / Double_t(fNofHits);
  LOG(info) << "=====================================";
  LOG(info) << GetName() << ": Run summary";
  LOG(info) << "Time slices            : " << fNofTs;
  if (fMode == kCbmRecoEvent) LOG(info) << "Events                 : " << fNofEvents;
  LOG(info) << "Digis / TSlice         : " << fixed << setprecision(2) << fNofDigisRun / Double_t(fNofTs);
  LOG(info) << "Digis used / TSlice    : " << fixed << setprecision(2) << fNofDigisUsed / Double_t(fNofTs);
  LOG(info) << "Digis ignored / TSlice : " << fixed << setprecision(2) << fNofDigisIgnored / Double_t(fNofTs);
  LOG(info) << "Clusters / TSlice      : " << fixed << setprecision(2) << fNofClusters / Double_t(fNofTs);
  LOG(info) << "Hits / TSlice          : " << fixed << setprecision(2) << fNofHits / Double_t(fNofTs);
  LOG(info) << "Digis per cluster      : " << fixed << setprecision(2) << digiCluster;
  LOG(info) << "Clusters per hit       : " << fixed << setprecision(2) << clusterHit;
  LOG(info) << "Time per TSlice        : " << fixed << setprecision(2) << 1000. * fTimeRun / Double_t(fNofTs) << " ms ";

  fTimeRun /= Double_t(fNofEvents);
  fTime1Run /= Double_t(fNofEvents);
  fTime2Run /= Double_t(fNofEvents);
  fTime3Run /= Double_t(fNofEvents);
  fTime4Run /= Double_t(fNofEvents);
  LOG(info) << "Time Reset       : " << fixed << setprecision(1) << setw(6) << 1000. * fTime1Run << " ms ("
            << setprecision(1) << setw(4) << 100. * fTime1Run / fTimeRun << " %)";
  LOG(info) << "Time Distribute  : " << fixed << setprecision(1) << setw(6) << 1000. * fTime2Run << " ms ("
            << setprecision(1) << 100. * fTime2Run / fTimeRun << " %)";
  LOG(info) << "Time Reconstruct : " << fixed << setprecision(1) << setw(6) << 1000. * fTime3Run << " ms ("
            << setprecision(1) << setw(4) << 100. * fTime3Run / fTimeRun << " %)";
  LOG(info) << "Time Output      : " << fixed << setprecision(1) << setw(6) << 1000. * fTime4Run << " ms ("
            << setprecision(1) << setw(4) << 100. * fTime4Run / fTimeRun << " %)";
  LOG(info) << "=====================================";
}
// -------------------------------------------------------------------------


// -----   Initialisation   ------------------------------------------------
InitStatus CbmRecoSts::Init()
{

  // --- Something for the screen
  std::cout << std::endl;
  LOG(info) << "==========================================================";
  LOG(info) << GetName() << ": Initialising ";

  // --- Check IO-Manager
  FairRootManager* ioman = FairRootManager::Instance();
  assert(ioman);

  // --- Digi Manager
  fDigiManager = CbmDigiManager::Instance();
  fDigiManager->Init();

  // --- In event mode: get input array (CbmEvent)
  if (fMode == kCbmRecoEvent) {
    LOG(info) << GetName() << ": Using event-by-event mode";
    fEvents = dynamic_cast<TClonesArray*>(ioman->GetObject("CbmEvent"));
    if (nullptr == fEvents) {
      LOG(warn) << GetName() << ": Event mode selected but no event array found!";
      return kFATAL;
    }  //? Event branch not present
  }    //? Event mode
  else
    LOG(info) << GetName() << ": Using time-based mode";

  // --- Check input array (StsDigis)
  if (!fDigiManager->IsPresent(ECbmModuleId::kSts)) LOG(fatal) << GetName() << ": No StsDigi branch in input!";

  // --- Register output array
  fClusters = new TClonesArray("CbmStsCluster", 1);
  ioman->Register("StsCluster", "Clusters in STS", fClusters, IsOutputBranchPersistent("StsCluster"));

  // --- Register output array
  fHits = new TClonesArray("CbmStsHit", 1);
  ioman->Register("StsHit", "Hits in STS", fHits, IsOutputBranchPersistent("StsHit"));

  // --- Simulation settings
  assert(fParSim);
  LOG(info) << GetName() << ": Sim settings " << fParSim->ToString();

  // --- Module parameters
  assert(fParSetModule);
  LOG(info) << GetName() << ": Module parameters " << fParSetModule->ToString();

  // --- Sensor parameters
  assert(fParSetSensor);
  LOG(info) << GetName() << ": Sensor parameters " << fParSetModule->ToString();

  // --- Sensor conditions
  assert(fParSetCond);
  //assert(fParSetCond->IsSet());
  LOG(info) << GetName() << ": Sensor conditions " << fParSetCond->ToString();

  // --- Initialise STS setup
  fSetup = CbmStsSetup::Instance();
  fSetup->Init(nullptr);
  //fSetup->SetSensorParameters(fParSetSensor);

  // --- Create reconstruction modules
  UInt_t nModules = CreateModules();
  LOG(info) << GetName() << ": Created " << nModules << " modules";

  LOG(info) << GetName() << ": Initialisation successful.";
  LOG(info) << "==========================================================";

  return kSUCCESS;
}
// -------------------------------------------------------------------------


// -----   Calculate the mean Lorentz shift in a sensor   ------------------
std::pair<Double_t, Double_t> CbmRecoSts::LorentzShift(const CbmStsParSensorCond& conditions, Double_t dZ, Double_t bY)
{

  Double_t vBias  = conditions.GetVbias();  // Bias voltage
  Double_t vFd    = conditions.GetVfd();    // Full-depletion voltage
  Double_t eField = (vBias + vFd) / dZ;     // Electric field

  // --- Integrate in 1000 steps over the sensor thickness
  Int_t nSteps     = 1000;
  Double_t deltaZ  = dZ / nSteps;
  Double_t dxMeanE = 0.;
  Double_t dxMeanH = 0.;
  for (Int_t j = 0; j <= nSteps; j++) {
    eField -= 2 * vFd / dZ * deltaZ / dZ;  // Electric field [V/cm]
    Double_t muHallE = conditions.GetHallMobility(eField, 0);
    Double_t muHallH = conditions.GetHallMobility(eField, 1);
    dxMeanE += muHallE * (dZ - Double_t(j) * deltaZ);
    dxMeanH += muHallH * Double_t(j) * deltaZ;
  }
  dxMeanE /= Double_t(nSteps);
  dxMeanH /= Double_t(nSteps);
  Double_t shiftF = dxMeanE * bY * 1.e-4;
  Double_t shiftB = dxMeanH * bY * 1.e-4;
  // The factor 1.e-4 is because bZ is in T = Vs/m**2, but muHall is in
  // cm**2/(Vs) and z in cm.

  return {shiftF, shiftB};
}
// -------------------------------------------------------------------------


// -----   Process one time slice or event   -------------------------------
void CbmRecoSts::ProcessData(CbmEvent* event)
{

  // --- Reset all modules
  fTimer.Start();
  Int_t nDigisIgnored = 0;
  Int_t nClusters     = 0;
  Int_t nHits         = 0;

  //  #pragma omp parallel for schedule(static) if(fParallelism_enabled)
  for (UInt_t it = 0; it < fModuleIndex.size(); it++) {
    fModuleIndex[it]->Reset();
  }
  fTimer.Stop();
  Double_t time1 = fTimer.RealTime();  // Time for resetting

  // --- Number of input digis
  fTimer.Start();
  Int_t nDigis = (event ? event->GetNofData(ECbmDataType::kStsDigi) : fDigiManager->GetNofDigis(ECbmModuleId::kSts));


  // --- Distribute digis to modules
  //#pragma omp parallel for schedule(static) if(fParallelism_enabled)
  for (Int_t iDigi = 0; iDigi < nDigis; iDigi++) {
    Int_t digiIndex        = (event ? event->GetIndex(ECbmDataType::kStsDigi, iDigi) : iDigi);
    const CbmStsDigi* digi = fDigiManager->Get<const CbmStsDigi>(digiIndex);
    assert(digi);

    // Check system ID. There are pulser digis in which will be ignored here.
    Int_t systemId = CbmAddress::GetSystemId(digi->GetAddress());
    if (systemId != ToIntegralType(ECbmModuleId::kSts)) {
      nDigisIgnored++;
      continue;
    }

    // Get proper reco module
    Int_t moduleAddress = CbmStsAddress::GetMotherAddress(digi->GetAddress(), kStsModule);
    auto it             = fModules.find(moduleAddress);
    if (it == fModules.end()) {
      LOG(warn) << "Unknown module address: " << CbmStsAddress::ToString(moduleAddress);
      ;
    }
    assert(it != fModules.end());
    CbmStsRecoModule* module = it->second;
    assert(module);
    module->AddDigiToQueue(digi, digiIndex);
  }
  fTimer.Stop();
  Double_t time2 = fTimer.RealTime();  // Time for digi distribution


  // --- Execute reconstruction in the modules
  fTimer.Start();
  //  #pragma omp parallel for schedule(static) if(fParallelism_enabled)
  for (UInt_t it = 0; it < fModuleIndex.size(); it++) {
    assert(fModuleIndex[it]);
    fModuleIndex[it]->Reconstruct();
  }
  fTimer.Stop();
  Double_t time3 = fTimer.RealTime();  // Time for reconstruction


  // --- Collect clusters and hits from modules
  // Here, the hits (and optionally clusters) are copied to the
  // TClonesArrays in the ROOT tree. This is surely not the last word
  // in terms of framework. It thus cannot be considered optimised.
  // The output shall eventually be tailored to provide the proper
  // input for further reconstruction (track finding).
  fTimer.Start();
  ULong64_t offsetClustersF = 0;
  ULong64_t offsetClustersB = 0;
  for (UInt_t it = 0; it < fModuleIndex.size(); it++) {

    const vector<CbmStsCluster>& moduleClustersF = fModuleIndex[it]->GetClustersF();
    offsetClustersF                              = fClusters->GetEntriesFast();
    for (auto& cluster : moduleClustersF) {
      UInt_t index = fClusters->GetEntriesFast();
      new ((*fClusters)[index]) CbmStsCluster(cluster);
      if (event) event->AddData(ECbmDataType::kStsCluster, index);
      nClusters++;
    }  //# front-side clusters in module

    const vector<CbmStsCluster>& moduleClustersB = fModuleIndex[it]->GetClustersB();
    offsetClustersB                              = fClusters->GetEntriesFast();
    for (auto& cluster : moduleClustersB) {
      UInt_t index = fClusters->GetEntriesFast();
      new ((*fClusters)[index]) CbmStsCluster(cluster);
      if (event) event->AddData(ECbmDataType::kStsCluster, index);
      nClusters++;
    }  //# back-side clusters in module

    const vector<CbmStsHit>& moduleHits = fModuleIndex[it]->GetHits();
    for (auto& hit : moduleHits) {
      UInt_t index   = fHits->GetEntriesFast();
      CbmStsHit* out = new ((*fHits)[index]) CbmStsHit(hit);
      out->SetFrontClusterId(out->GetFrontClusterId() + offsetClustersF);
      out->SetBackClusterId(out->GetBackClusterId() + offsetClustersB);
      if (event) event->AddData(ECbmDataType::kStsHit, index);
      nHits++;
    }  //# hits in module
  }
  fTimer.Stop();
  Double_t time4 = fTimer.RealTime();  // Time for data I/O

  // --- Bookkeeping
  Double_t realTime = time1 + time2 + time3 + time4;
  fNofDigis += nDigis;
  fNofDigisUsed += nDigis - nDigisIgnored;
  fNofDigisIgnored += nDigisIgnored;
  fNofClusters += nClusters;
  fNofHits += nHits;
  fTimeTot += realTime;
  fTime1 += time1;
  fTime2 += time2;
  fTime3 += time3;
  fTime4 += time4;

  // --- Screen log
  if (event) {
    LOG(debug) << setw(20) << left << GetName() << "[" << fixed << setprecision(4) << realTime << " s] : Event "
               << right << setw(6) << event->GetNumber() << ", digis: " << nDigis << ", ignored: " << nDigisIgnored
               << ", clusters: " << nClusters << ", hits " << nHits;
  }  //? event mode
  else {
    LOG(debug) << setw(20) << left << GetName() << "[" << fixed << setprecision(4) << realTime << " s] : TSlice "
               << right << setw(6) << fNofTs << ", digis: " << nDigis << ", ignored: " << nDigisIgnored
               << ", clusters: " << nClusters << ", hits " << nHits;
  }
}
// -------------------------------------------------------------------------


// -----   Connect parameter container   -----------------------------------
void CbmRecoSts::SetParContainers()
{
  FairRuntimeDb* db = FairRun::Instance()->GetRuntimeDb();
  fParSim           = dynamic_cast<CbmStsParSim*>(db->getContainer("CbmStsParSim"));
  fParSetModule     = dynamic_cast<CbmStsParSetModule*>(db->getContainer("CbmStsParSetModule"));
  fParSetSensor     = dynamic_cast<CbmStsParSetSensor*>(db->getContainer("CbmStsParSetSensor"));
  fParSetCond       = dynamic_cast<CbmStsParSetSensorCond*>(db->getContainer("CbmStsParSetSensorCond"));
}
// -------------------------------------------------------------------------
