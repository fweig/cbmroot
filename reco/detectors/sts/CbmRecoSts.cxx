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
#include "CbmStsPhysics.h"
#include "CbmStsRecoModule.h"
#include "CbmStsSetup.h"

#include <FairField.h>
#include <FairRootManager.h>
#include <FairRun.h>
#include <FairRuntimeDb.h>

#include <TClonesArray.h>
#include <TGeoBBox.h>
#include <TGeoPhysicalNode.h>

#include <iomanip>

#include <xpu/host.h>

#if __has_include(<omp.h>)
#include <omp.h>
#endif

using std::fixed;
using std::left;
using std::right;
using std::setprecision;
using std::setw;
using std::stringstream;
using std::vector;


ClassImp(CbmRecoSts);

// -----   Constructor   ---------------------------------------------------
CbmRecoSts::CbmRecoSts(ECbmRecoMode mode, Bool_t writeClusters)
  : FairTask("RecoSts", 1)
  , fMode(mode)
  , fWriteClusters(writeClusters)
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

  std::vector<cbm::algo::StsModulePar> gpuModules;  // for gpu reco
  // std::vector<int> moduleAddrs;
  // std::vector<experimental::CbmStsHitFinderConfig> hfCfg;

  for (Int_t iModule = 0; iModule < fSetup->GetNofModules(); iModule++) {

    // --- Setup module and sensor
    CbmStsModule* setupModule = fSetup->GetModule(iModule);
    assert(setupModule);
    Int_t moduleAddress = Int_t(setupModule->GetAddress());
    assert(setupModule->GetNofDaughters() == 1);
    CbmStsElement* setupSensor = setupModule->GetDaughter(0);
    assert(setupSensor);
    Int_t sensorAddress = Int_t(setupSensor->GetAddress());

    // --- Module parameters
    const CbmStsParModule& modPar       = fParSetModule->GetParModule(moduleAddress);
    const CbmStsParSensor& sensPar      = fParSetSensor->GetParSensor(sensorAddress);
    const CbmStsParSensorCond& sensCond = fParSetCond->GetParSensor(sensorAddress);

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

    // Get Transformation Matrix
    cbm::algo::StsModuleTransformationMatrix localToGlobal;
    TGeoHMatrix* matrix = recoModule->getMatrix();
    std::copy_n(matrix->GetRotationMatrix(), 9, localToGlobal.rotation.begin());
    std::copy_n(matrix->GetTranslation(), 3, localToGlobal.translation.begin());

    // Collect GPU parameters
    cbm::algo::StsModulePar gpuModulePars {
      .address       = moduleAddress,
      .dY            = sensPar.GetPar(3),
      .pitch         = sensPar.GetPar(6),
      .stereoF       = sensPar.GetPar(8),
      .stereoB       = sensPar.GetPar(9),
      .lorentzF      = float(lorentzF),
      .lorentzB      = float(lorentzB),
      .localToGlobal = localToGlobal,
    };
    gpuModules.emplace_back(gpuModulePars);
  }

  const CbmStsParModule& firstModulePars = fParSetModule->GetParModule(gpuModules[0].address);

  CbmStsParAsic asic = firstModulePars.GetParAsic(0);
  cbm::algo::StsAsicPar algoAsic {
    .nAdc           = asic.GetNofAdc(),
    .dynamicRange   = float(asic.GetDynRange()),
    .threshold      = float(asic.GetThreshold()),
    .timeResolution = float(asic.GetTimeResol()),
    .deadTime       = float(asic.GetDeadTime()),
    .noise          = float(asic.GetNoise()),
    .zeroNoiseRate  = float(asic.GetZeroNoiseRate()),
  };

  int nChannels = firstModulePars.GetNofChannels();

  auto [landauValues, landauStepSize] = CbmStsPhysics::Instance()->GetLandauWidthTable();
  std::vector<float> landauValuesF;
  std::copy(landauValues.begin(), landauValues.end(), std::back_inserter(landauValuesF));
  cbm::algo::StsHitfinderPar pars {
    .asic      = algoAsic,
    .nChannels = nChannels,
    .modules   = gpuModules,
    .landauTable =
      {
        .values   = landauValuesF,
        .stepSize = float(landauStepSize),
      },
  };
  if (fUseGpuReco) fGpuReco.SetParameters(pars);

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

  if (fUseGpuReco) {

    ProcessDataGpu();
    // fNofDigis     = fGpuReco.nDigis;
    // fNofDigisUsed = fGpuReco.nDigisUsed;
    // fNofClusters  = fGpuReco.nCluster;
    // fNofHits      = fGpuReco.nHits;

    // Old reco in time based mode
  }
  else if (fMode == kCbmRecoTimeslice) {

    ProcessData(nullptr);

    // --- Event mode: loop over events
  }
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

  // allDigis = fNofDigisUsed;
}
// -------------------------------------------------------------------------


// -----   End-of-run action   ---------------------------------------------
void CbmRecoSts::Finish()
{
  int ompThreads = -1;
#ifdef _OPENMP
  ompThreads = omp_get_max_threads();
#endif

  Double_t digiCluster = Double_t(fNofDigisUsed) / Double_t(fNofClusters);
  Double_t clusterHit  = Double_t(fNofClusters) / Double_t(fNofHits);
  LOG(info) << "=====================================";
  LOG(info) << GetName() << ": Run summary";
  if (fUseGpuReco) LOG(info) << "Ran new GPU STS reconstruction.";
  else if (ompThreads < 0)
    LOG(info) << "STS reconstruction ran single threaded (No OpenMP).";
  else
    LOG(info) << "STS reconstruction ran multithreaded with OpenMP (nthreads = " << ompThreads << ")";
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


  // Aggregate times for substeps of reconstruction
  // Note: These times are meaningless when reconstruction runs with > 1 thread.
  CbmStsRecoModule::Timings timingsTotal;
  for (const auto* m : fModuleIndex) {
    auto moduleTime = m->GetTimings();
    timingsTotal.timeSortDigi += moduleTime.timeSortDigi;
    timingsTotal.timeCluster += moduleTime.timeCluster;
    timingsTotal.timeSortCluster += moduleTime.timeSortCluster;
    timingsTotal.timeHits += moduleTime.timeHits;
  }

  // Avoid division by zero if no events present
  double nEvent = std::max(fNofEvents, 1);

  fTimeTot /= nEvent;
  fTime1 /= nEvent;
  fTime2 /= nEvent;
  fTime3 /= nEvent;
  fTime4 /= nEvent;

  if (not fUseGpuReco) {
    LOG(info) << "NofEvents        : " << fNofEvents;
    LOG(info) << "Time Reset       : " << fixed << setprecision(1) << setw(6) << 1000. * fTime1 << " ms ("
              << setprecision(1) << setw(4) << 100. * fTime1 / fTimeTot << " %)";
    LOG(info) << "Time Distribute  : " << fixed << setprecision(1) << setw(6) << 1000. * fTime2 << " ms ("
              << setprecision(1) << 100. * fTime2 / fTimeTot << " %)";
    LOG(info) << "Time Reconstruct: " << fixed << setprecision(1) << setw(6) << 1000. * fTime3 << " ms ("
              << setprecision(1) << setw(4) << 100. * fTime3 / fTimeTot << " %)";
    LOG(info) << "Time by step:\n"
              << "  Sort Digi   : " << fixed << setprecision(1) << setw(6) << 1000. * timingsTotal.timeSortDigi
              << " ms\n"
              << "  Find Cluster: " << fixed << setprecision(1) << setw(6) << 1000. * timingsTotal.timeCluster
              << " ms\n"
              << "  Sort Cluster: " << fixed << setprecision(1) << setw(6) << 1000. * timingsTotal.timeSortCluster
              << " ms\n"
              << "  Find Hits   : " << fixed << setprecision(1) << setw(6) << 1000. * timingsTotal.timeHits << " ms\n";
  }
  else {
    cbm::algo::StsHitfinderTimes times = fGpuReco.GetHitfinderTimes();

    double gpuHitfinderTimeTotal = times.timeSortDigi + times.timeCluster + times.timeSortCluster + times.timeHits;
    LOG(info) << "Time Reconstruct (GPU) : " << fixed << setprecision(1) << setw(6) << gpuHitfinderTimeTotal << " ms";
    LOG(info) << "Time by step:\n"
              << "  Sort Digi   : " << fixed << setprecision(1) << setw(6) << times.timeSortDigi << " ms\n"
              << "  Find Cluster: " << fixed << setprecision(1) << setw(6) << times.timeCluster << " ms\n"
              << "  Sort Cluster: " << fixed << setprecision(1) << setw(6) << times.timeSortCluster << " ms\n"
              << "  Find Hits   : " << fixed << setprecision(1) << setw(6) << times.timeHits << " ms";
  }
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

  // Initialize xpu.
  // TODO: This call can be relatively expensive.
  //       We need a way to ensure this happens only once at the beginning.
  if (fUseGpuReco) {
    setenv("XPU_PROFILE", "1", 1);  // Always enable profiling in xpu
    xpu::initialize();
  }

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

  // --- Parameters
  InitParams();

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


// -----   Parameter initialisation   --------------------------------------
void CbmRecoSts::InitParams()
{

  // --- Module parameters
  TString sourceModu = "database";
  assert(fParSetModule);
  if (fUserParSetModule) {
    fParSetModule->clear();
    *fParSetModule = *fUserParSetModule;
    fParSetModule->setChanged();
    fParSetModule->setInputVersion(-2, 1);
    sourceModu = "user-defined";
  }
  if (fUserParModule) {  // global settings override
    fParSetModule->clear();
    fParSetModule->SetGlobalPar(*fUserParModule);
    fParSetModule->setChanged();
    fParSetModule->setInputVersion(-2, 1);
    sourceModu = "user-defined, global";
  }
  LOG(info) << GetName() << ": Module parameters (" << sourceModu << ") " << fParSetModule->ToString();

  // --- Sensor parameters
  TString sourceSens = "database";
  assert(fParSetSensor);
  if (fUserParSetSensor) {
    fParSetSensor->clear();
    *fParSetSensor = *fUserParSetSensor;
    fParSetSensor->setChanged();
    fParSetSensor->setInputVersion(-2, 1);
    sourceSens = "user-defined";
  }
  if (fUserParSensor) {  // global settings override
    fParSetSensor->clear();
    fParSetSensor->SetGlobalPar(*fUserParSensor);
    fParSetSensor->setChanged();
    fParSetSensor->setInputVersion(-2, 1);
    sourceSens = "user-defined, global";
  }
  LOG(info) << GetName() << ": Sensor parameters (" << sourceSens << ")" << fParSetSensor->ToString();

  // --- Sensor conditions
  TString sourceCond = "database";
  assert(fParSetCond);
  if (fUserParSetCond) {
    fParSetCond->clear();
    *fParSetCond = *fUserParSetCond;
    fParSetCond->setChanged();
    fParSetCond->setInputVersion(-2, 1);
    sourceSens = "user-defined";
  }
  if (fUserParCond) {  // global settings override
    fParSetCond->clear();
    fParSetCond->SetGlobalPar(*fUserParCond);
    fParSetCond->setChanged();
    fParSetCond->setInputVersion(-2, 1);
    sourceCond = "user-defined, global";
  }
  LOG(info) << GetName() << ": Sensor conditions (" << sourceCond << ")" << fParSetCond->ToString();
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

#ifdef _OPENMP
#pragma omp parallel for schedule(static)
#endif
  for (UInt_t it = 0; it < fModuleIndex.size(); it++) {
    fModuleIndex[it]->Reset();
  }
  fTimer.Stop();
  Double_t time1 = fTimer.RealTime();  // Time for resetting

  // --- Number of input digis
  fTimer.Start();
  Int_t nDigis = (event ? event->GetNofData(ECbmDataType::kStsDigi) : fDigiManager->GetNofDigis(ECbmModuleId::kSts));
  auto digis   = fDigiManager->GetArray<CbmStsDigi>();

  // --- Distribute digis to modules
  //#pragma omp parallel for schedule(static) if(fParallelism_enabled)
  for (Int_t iDigi = 0; iDigi < nDigis; iDigi++) {
    Int_t digiIndex        = (event ? event->GetIndex(ECbmDataType::kStsDigi, iDigi) : iDigi);
    const CbmStsDigi* digi = &digis[digiIndex];

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
#ifdef _OPENMP
#pragma omp parallel for schedule(static)
#endif
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

    offsetClustersF = fClusters->GetEntriesFast();
    for (auto& cluster : moduleClustersF) {
      UInt_t index = fClusters->GetEntriesFast();
      new ((*fClusters)[index]) CbmStsCluster(cluster);
      if (event) event->AddData(ECbmDataType::kStsCluster, index);
      nClusters++;
    }  //# front-side clusters in module

    const vector<CbmStsCluster>& moduleClustersB = fModuleIndex[it]->GetClustersB();

    offsetClustersB = fClusters->GetEntriesFast();
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

void CbmRecoSts::ProcessDataGpu()
{
  if (fMode == kCbmRecoEvent) throw std::runtime_error("STS GPU Reco does not yet support event-by-event mode.");

  auto digis = fDigiManager->GetArray<CbmStsDigi>();
  fGpuReco(digis);
  auto [nClustersForwarded, nHitsForwarded] = ForwardGpuClusterAndHits();

  fNofDigis     = digis.size();
  fNofDigisUsed = digis.size();
  fNofClusters  = nClustersForwarded;
  fNofHits      = nHitsForwarded;
}

std::pair<size_t, size_t> CbmRecoSts::ForwardGpuClusterAndHits()
{
  size_t nClustersForwarded = 0, nHitsForwarded = 0;

  const cbm::algo::StsHitfinderHost& hfc = fGpuReco.GetHitfinderBuffers();
  const cbm::algo::StsHitfinderPar& pars = fGpuReco.GetParameters();

  for (int module = 0; module < hfc.nModules; module++) {

    auto* gpuClusterF    = &hfc.clusterDataPerModule.h()[module * hfc.maxClustersPerModule];
    auto* gpuClusterIdxF = &hfc.clusterIdxPerModule.h()[module * hfc.maxClustersPerModule];
    int nClustersFGpu    = hfc.nClustersPerModule.h()[module];

    nClustersForwarded += nClustersFGpu;

    for (int i = 0; i < nClustersFGpu; i++) {
      auto& cidx       = gpuClusterIdxF[i];
      auto& clusterGpu = gpuClusterF[cidx.fIdx];

      unsigned int outIdx = fClusters->GetEntriesFast();
      auto* clusterOut    = new ((*fClusters)[outIdx])::CbmStsCluster {};

      clusterOut->SetAddress(pars.modules[module].address);
      clusterOut->SetProperties(clusterGpu.fCharge, clusterGpu.fPosition, clusterGpu.fPositionError, cidx.fTime,
                                clusterGpu.fTimeError);
      clusterOut->SetSize(clusterGpu.fSize);
    }

    auto* gpuClusterB    = &hfc.clusterDataPerModule.h()[(module + hfc.nModules) * hfc.maxClustersPerModule];
    auto* gpuClusterIdxB = &hfc.clusterIdxPerModule.h()[(module + hfc.nModules) * hfc.maxClustersPerModule];
    int nClustersBGpu    = hfc.nClustersPerModule.h()[module + hfc.nModules];

    nClustersForwarded += nClustersBGpu;

    for (int i = 0; i < nClustersBGpu; i++) {
      auto& cidx          = gpuClusterIdxB[i];
      auto& clusterGpu    = gpuClusterB[cidx.fIdx];
      unsigned int outIdx = fClusters->GetEntriesFast();
      auto* clusterOut    = new ((*fClusters)[outIdx])::CbmStsCluster {};

      clusterOut->SetAddress(pars.modules[module].address);
      clusterOut->SetProperties(clusterGpu.fCharge, clusterGpu.fPosition, clusterGpu.fPositionError, cidx.fTime,
                                clusterGpu.fTimeError);
      clusterOut->SetSize(clusterGpu.fSize);
    }

    auto* gpuHits = &hfc.hitsPerModule.h()[module * hfc.maxHitsPerModule];
    int nHitsGpu  = hfc.nHitsPerModule.h()[module];

    nHitsForwarded += nHitsGpu;

    for (int i = 0; i < nHitsGpu; i++) {
      auto& hitGpu = gpuHits[i];

      unsigned int outIdx = fHits->GetEntriesFast();
      new ((*fHits)[outIdx])::CbmStsHit {pars.modules[module].address,
                                         TVector3 {hitGpu.fX, hitGpu.fY, hitGpu.fZ},
                                         TVector3 {hitGpu.fDx, hitGpu.fDy, hitGpu.fDz},
                                         hitGpu.fDxy,
                                         0,
                                         0,
                                         double(hitGpu.fTime),
                                         hitGpu.fTimeError,
                                         hitGpu.fDu,
                                         hitGpu.fDv};
    }

  }  // for (int module = 0; module < hfc.nModules; module++)

  return {nClustersForwarded, nHitsForwarded};
}


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

void CbmRecoSts::DumpNewHits()
{
  std::ofstream out {"newHits.csv"};
  const cbm::algo::StsHitfinderHost& hfc = fGpuReco.GetHitfinderBuffers();
  out << "module, x, y, z, deltaX, deltaY, deltaZ, deltaXY, time, timeError, deltaU, deltaV" << std::endl;
  for (size_t m = 0; m < fModuleIndex.size(); m++) {
    int nHitsGpu  = hfc.nHitsPerModule.h()[m];
    auto* gpuHits = &hfc.hitsPerModule.h()[m * hfc.maxHitsPerModule];
    for (int i = 0; i < nHitsGpu; i++) {
      auto& h = gpuHits[i];
      out << m << ", " << h.fX << ", " << h.fY << ", " << h.fZ << ", " << h.fDx << ", " << h.fDy << ", " << h.fDz
          << ", " << h.fDxy << ", " << h.fTime << ", " << h.fTimeError << ", " << h.fDu << ", " << h.fDv << std::endl;
    }
  }
}

void CbmRecoSts::DumpOldHits()
{
  std::ofstream out {"oldHits.csv"};

  out << "module, x, y, z, deltaX, deltaY, deltaZ, deltaXY, time, timeError, deltaU, deltaV" << std::endl;

  for (size_t m = 0; m < fModuleIndex.size(); m++) {
    const auto& hits = fModuleIndex[m]->GetHits();

    for (const auto& h : hits) {
      out << m << ", " << h.GetX() << ", " << h.GetY() << ", " << h.GetZ() << ", " << h.GetDx() << ", " << h.GetDy()
          << ", " << h.GetDz() << ", " << h.GetDxy() << ", " << h.GetTime() << ", " << h.GetTimeError() << ", "
          << h.GetDu() << ", " << h.GetDv() << std::endl;
    }
  }
}
