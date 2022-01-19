/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov [committer] */

/** @file CbmRecoStsPixel.cxx
 ** @author Sergey Gorbunov
 ** @since 09.12.2021
 **/

#include "CbmRecoStsPixel.h"

#include "CbmAddress.h"
#include "CbmDigiManager.h"
#include "CbmEvent.h"
#include "CbmMCDataArray.h"
#include "CbmMCDataManager.h"
#include "CbmStsDigi.h"
#include "CbmStsModule.h"
#include "CbmStsParSetModule.h"
#include "CbmStsParSetSensor.h"
#include "CbmStsParSetSensorCond.h"
#include "CbmStsParSim.h"
#include "CbmStsPoint.h"
#include "CbmStsRecoModule.h"
#include "CbmStsSetup.h"
#include "CbmStsStation.h"

#include <FairField.h>
#include <FairRun.h>
#include <FairRuntimeDb.h>

#include <TClonesArray.h>
#include <TGeoBBox.h>
#include <TGeoPhysicalNode.h>
#include <TRandom.h>

#include <iomanip>

using std::fixed;
using std::left;
using std::right;
using std::setprecision;
using std::setw;
using std::stringstream;
using std::vector;


ClassImp(CbmRecoStsPixel)


  // -----   Constructor   ---------------------------------------------------
  CbmRecoStsPixel::CbmRecoStsPixel(ECbmRecoMode mode)
  : FairTask("RecoStsPixel", 1)
  , fMode(mode)
{
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmRecoStsPixel::~CbmRecoStsPixel() {}
// -------------------------------------------------------------------------


// -----   End-of-run action   ---------------------------------------------
void CbmRecoStsPixel::Finish() {}
// -------------------------------------------------------------------------


// -----   Initialisation   ------------------------------------------------
InitStatus CbmRecoStsPixel::Init()
{

  // --- Something for the screen

  LOG(info) << "==========================================================";
  LOG(info) << GetName() << ": Initialising ";

  // --- Check IO-Manager
  FairRootManager* ioman = FairRootManager::Instance();
  assert(ioman);

  // --- In event mode: get input array (CbmEvent)
  if (fMode == kCbmRecoEvent) {
    LOG(info) << GetName() << ": Using event-by-event mode";
    fEvents = dynamic_cast<TClonesArray*>(ioman->GetObject("CbmEvent"));
    if (nullptr == fEvents) {
      LOG(warn) << GetName() << ": Event mode selected but no event array found!";
      return kFATAL;
    }  //? Event branch not present
  }    //? Event mode
  else {
    LOG(info) << GetName() << ": Using time-based mode";
  }


  // --- Digi Manager
  fDigiManager = CbmDigiManager::Instance();
  fDigiManager->Init();

  // --- Check input array (StsDigis)
  if (!fDigiManager->IsPresent(ECbmModuleId::kSts)) {
    LOG(fatal) << GetName() << ": No StsDigi branch in input!";
    return kERROR;
  }

  if (!fDigiManager->IsMatchPresent(ECbmModuleId::kSts)) {
    LOG(error) << GetName() << " sts digi matches are not present";
    return kERROR;
  }

  CbmMCDataManager* mcManager = (CbmMCDataManager*) ioman->GetObject("MCDataManager");
  if (!mcManager) {
    LOG(error) << GetName() << ": No CbmMCDataManager!";
    return kERROR;
  }

  fStsPoints = mcManager->InitBranch("StsPoint");

  if (!fStsPoints) {
    LOG(fatal) << GetName() << ": No StsPoint branch in input!";
    return kERROR;
  }


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
  if (!fSetup->IsInit()) { fSetup->Init(nullptr); }
  if (!fSetup->IsModuleParsInit()) { fSetup->SetModuleParameters(fParSetModule); }
  if (!fSetup->IsSensorParsInit()) { fSetup->SetSensorParameters(fParSetSensor); }
  if (!fSetup->IsSensorCondInit()) { fSetup->SetSensorConditions(fParSetCond); }

  // make sure that the STS digis were produced by the experimental Pixel digitiser
  if (strcmp(fParSetSensor->getDescription(), "Experimental STS Pixels")) {
    LOG(error) << GetName() << " STS digis must be produced by the CbmStsDigitizePixel digitizer";
    return kERROR;
  }

  LOG(info) << GetName() << ": Initialisation successful.";
  LOG(info) << "==========================================================";

  return kSUCCESS;
}
// -------------------------------------------------------------------------


// -----   Task execution   ------------------------------------------------
void CbmRecoStsPixel::Exec(Option_t*)
{

  // --- Clear hit output array
  fHits->Delete();

  // --- Reset cluster output array
  fClusters->Delete();

  if (fMode == kCbmRecoTimeslice) {
    // --- Time-slice mode: process entire array
    ProcessData(nullptr);
  }
  else {
    // --- Event mode: loop over events
    assert(fEvents);
    for (Int_t iEvent = 0; iEvent < fEvents->GetEntriesFast(); iEvent++) {
      CbmEvent* event = dynamic_cast<CbmEvent*>(fEvents->At(iEvent));
      assert(event);
      ProcessData(event);
    }  //# events
  }    //? event mode
}
// -------------------------------------------------------------------------


// -----   Process one time slice or event   -------------------------------
void CbmRecoStsPixel::ProcessData(CbmEvent* event)
{
  if (!fDigiManager->IsMatchPresent(ECbmModuleId::kSts)) {
    LOG(error) << GetName() << ": No StsDigi branch in input!";
    return;
  }

  Int_t nDigis = (event ? event->GetNofData(ECbmDataType::kStsDigi) : fDigiManager->GetNofDigis(ECbmModuleId::kSts));
  int nHits    = 0;

  for (Int_t iDigi = 0; iDigi < nDigis; iDigi++) {

    UInt_t digiIndex       = (event ? event->GetIndex(ECbmDataType::kStsDigi, iDigi) : iDigi);
    const CbmStsDigi* digi = fDigiManager->Get<const CbmStsDigi>(digiIndex);
    assert(digi);

    // Check system ID. There are pulser digis in which will be ignored here.
    Int_t systemId = CbmAddress::GetSystemId(digi->GetAddress());
    if (systemId != ToIntegralType(ECbmModuleId::kSts)) { continue; }

    const CbmMatch* match = fDigiManager->GetMatch(ECbmModuleId::kSts, digiIndex);
    assert(match);

    // make sure that the digi was produced by CbmStsDigitizePixel task
    if (digi->GetChannel() != 0 || match->GetNofLinks() != 1) {
      LOG(error) << GetName() << " sts digis were not produced by CbmStsDigitizePixel task";
      break;
    }

    // the digi was produced by one MC point

    const CbmLink& link = match->GetLink(0);
    CbmStsPoint* pt     = dynamic_cast<CbmStsPoint*>(fStsPoints->Get(link.GetFile(), link.GetEntry(), link.GetIndex()));

    // create one cluster and one hit per digi

    UInt_t iCluster        = fClusters->GetEntriesFast();
    CbmStsCluster* cluster = new ((*fClusters)[iCluster]) CbmStsCluster;
    cluster->AddDigi(iDigi);
    if (event) event->AddData(ECbmDataType::kStsCluster, iCluster);

    UInt_t iHit    = fHits->GetEntriesFast();
    CbmStsHit* hit = new ((*fHits)[iHit]) CbmStsHit;

    hit->SetAddress(digi->GetAddress());
    hit->SetFrontClusterId(iCluster);
    hit->SetBackClusterId(iCluster);

    int ista = CbmStsSetup::Instance()->GetStationNumber(hit->GetAddress());
    if (ista < 0 || ista >= CbmStsSetup::Instance()->GetNofStations()) {
      LOG(error) << "wrong Sts station number " << ista;
      break;
    }

    CbmStsStation* station = CbmStsSetup::Instance()->GetStation(ista);
    double staZ            = station->GetZ();

    if (fabs(pt->GetZ() - staZ) > 1.) {
      LOG(error) << "Sts point Z " << pt->GetZ() << " is far from the station Z " << staZ;
      break;
    }

    hit->SetX(0.5 * (pt->GetXIn() + pt->GetXOut()));
    hit->SetY(0.5 * (pt->GetYIn() + pt->GetYOut()));
    hit->SetZ(0.5 * (pt->GetZIn() + pt->GetZOut()));

    Double_t resX = station->GetSensorPitch(0) / sqrt(12.);
    Double_t resY = station->GetSensorPitch(1) / sqrt(12.);

    assert(resX > 1.e-5);
    assert(resY > 1.e-5);

    auto gaus = []() {
      double x = 5;
      while (fabs(x) > 3.5) {
        x = gRandom->Gaus(0., 1.);
      }
      return x;
    };

    hit->SetX(hit->GetX() + resX * gaus());
    hit->SetY(hit->GetY() + resY * gaus());
    hit->SetDx(resX);
    hit->SetDy(resY);
    hit->SetDxy(0.);
    hit->SetDu(hit->GetDx());
    hit->SetDv(hit->GetDy());

    hit->SetTime(digi->GetTime());
    const CbmStsParModule& module = fParSetModule->GetParModule(digi->GetAddress());
    const CbmStsParAsic& asic     = module.GetParAsic(0);
    hit->SetTimeError(asic.GetTimeResol());

    if (event) event->AddData(ECbmDataType::kStsHit, iHit);
    nHits++;
  }  // digis

  LOG(info) << GetName() << " Processed " << nDigis << " digis, created " << nHits << " hits";
}
// -------------------------------------------------------------------------


// -----   Connect parameter container   -----------------------------------
void CbmRecoStsPixel::SetParContainers()
{
  FairRuntimeDb* db = FairRun::Instance()->GetRuntimeDb();
  fParSim           = dynamic_cast<CbmStsParSim*>(db->getContainer("CbmStsParSim"));
  fParSetModule     = dynamic_cast<CbmStsParSetModule*>(db->getContainer("CbmStsParSetModule"));
  fParSetSensor     = dynamic_cast<CbmStsParSetSensor*>(db->getContainer("CbmStsParSetSensor"));
  fParSetCond       = dynamic_cast<CbmStsParSetSensorCond*>(db->getContainer("CbmStsParSetSensorCond"));
}
// -------------------------------------------------------------------------
