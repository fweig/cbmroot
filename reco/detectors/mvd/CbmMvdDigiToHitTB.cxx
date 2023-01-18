/* Copyright (C) 2019-2020 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andreas Redelbach [committer] */

// -------------------------------------------------------------------------
// -----                    CbmMvdDigiToHitTB source file                -----
// -------------------------------------------------------------------------

// Includes from MVD
#include "CbmMvdDigiToHitTB.h"

#include "CbmEvent.h"
#include "CbmMvdDetector.h"
#include "CbmMvdPoint.h"
#include "plugins/tasks/CbmMvdSensorDigiToHitTask.h"
#include "tools/CbmMvdGeoHandler.h"

// Includes from FAIR
#include "FairModule.h"
#include "FairRootManager.h"


// Includes from ROOT
#include "TClonesArray.h"
#include "TGeoManager.h"
#include "TMath.h"
#include "TString.h"


// Includes from C++
#include <iomanip>
#include <iostream>

using std::cout;
using std::endl;
using std::fixed;
using std::setprecision;
using std::setw;

// -----   Default constructor   ------------------------------------------
CbmMvdDigiToHitTB::CbmMvdDigiToHitTB() : CbmMvdDigiToHitTB("MVDDigiToHitTB", 0, 0) {}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
CbmMvdDigiToHitTB::CbmMvdDigiToHitTB(const char* name, Int_t iMode, Int_t iVerbose)
  : FairTask(name, iVerbose)
  , fMode(iMode)
  , fShowDebugHistos(kFALSE)
  , fDetector(NULL)
  , fEvents(NULL)
  , fInputDigis(NULL)
  , fEventDigis(NULL)
  , fCluster(NULL)
  , fClusterPluginNr(0)
  , fBranchName("MvdDigi")
  , fTimer()
{
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmMvdDigiToHitTB::~CbmMvdDigiToHitTB()
{

  if (fCluster) {
    fCluster->Delete();
    delete fCluster;
  }
}
// -----------------------------------------------------------------------------

// -----   Exec   --------------------------------------------------------------
void CbmMvdDigiToHitTB::Exec(Option_t* /*opt*/)
{
  // --- Start timer
  fTimer.Start();

  fCluster->Delete();

  Int_t nEvents = fEvents->GetEntriesFast();
  for (Int_t iEv = 0; iEv < nEvents; ++iEv) {
    LOG(debug) << "Getting data from CbmEvent";
    CbmEvent* event = dynamic_cast<CbmEvent*>(fEvents->At(iEv));
    Int_t nrOfDigis = event->GetNofData(ECbmDataType::kMvdDigi);
    fEventDigis->Delete();
    for (Int_t nDigi = 0; nDigi < nrOfDigis; ++nDigi) {
      Int_t iDigi = event->GetIndex(ECbmDataType::kMvdDigi, nDigi);
      fEventDigis->AddLast((CbmMvdDigi*) fInputDigis->At(iDigi));
    }
    LOG(debug) << "//----------------------------------------//";
    LOG(debug) << endl << "Send Input";
    fDetector->SendInputDigisToHits(fEventDigis);  //Version for DigisToHits
    LOG(debug) << "Execute HitPlugin Nr. " << fClusterPluginNr;
    fDetector->Exec(fClusterPluginNr);
    LOG(debug) << "End Chain";
    LOG(debug) << "Start writing Hit";
    fCluster->AbsorbObjects(fDetector->GetOutputHits(), 0, fDetector->GetOutputHits()->GetEntriesFast() - 1);
    LOG(debug) << "Total of " << fCluster->GetEntriesFast() << " Hit in this Event";
    LOG(debug) << "//----------------------------------------//";
    LOG(info) << "+ " << setw(20) << GetName() << ": Created: " << fCluster->GetEntriesFast() << " hit in " << fixed
              << setprecision(6) << fTimer.RealTime() << " s";
  }
  fTimer.Stop();
}
// -----------------------------------------------------------------------------

// -----   Init   --------------------------------------------------------------
InitStatus CbmMvdDigiToHitTB::Init()
{
  cout << "-I- " << GetName() << ": Initialisation..." << endl;
  cout << endl;
  cout << "---------------------------------------------" << endl;
  cout << "-I- Initialising " << GetName() << " ...." << endl;

  // **********  RootManager
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman) {
    cout << "-E- " << GetName() << "::Init: No FairRootManager!" << endl;
    return kFATAL;
  }

  // **********  Get input arrays
  fEvents = dynamic_cast<TClonesArray*>(ioman->GetObject("CbmEvent"));

  fInputDigis = (TClonesArray*) ioman->GetObject("MvdDigi");
  fEventDigis = new TClonesArray("CbmMvdDigi", 10000);
  if (!fInputDigis) {
    LOG(error) << "No MvdDigi branch found. There was no MVD in the "
                  "simulation. Switch this task off";
    return kERROR;
  }

  // **********  Register output array
  fCluster = new TClonesArray("CbmMvdHit", 10000);
  ioman->Register("MvdHit", "Mvd Hits", fCluster, IsOutputBranchPersistent("MvdHit"));

  fDetector = CbmMvdDetector::Instance();

  if (fDetector->GetSensorArraySize() > 1) { LOG(debug) << "-I- succesfully loaded Geometry from file -I-"; }
  else {
    LOG(fatal) << "Geometry couldn't be loaded from file. No MVD digitizer available.";
  }

  // Add the digi2hit plugin to all sensors
  std::map<int, CbmMvdSensor*>& sensorMap = fDetector->GetSensorMap();
  UInt_t plugincount=fDetector->GetPluginCount();

  for (auto itr = sensorMap.begin();
              itr != sensorMap.end(); itr++) {
    CbmMvdSensorDigiToHitTask* hitTask = new CbmMvdSensorDigiToHitTask();

    itr->second->AddPlugin(hitTask);
    itr->second->SetHitPlugin(plugincount);
  }
  fDetector->SetSensorArrayFilled(kTRUE);
  fDetector->SetPluginCount(plugincount+1);
  fHitPluginNr = (UInt_t)(fDetector->GetPluginArraySize());

  if (fShowDebugHistos) fDetector->ShowDebugHistos();
  fDetector->Init();


  // Screen output
  LOG(info) << GetName() << " initialised";

  return kSUCCESS;
}

// -----   Virtual public method Reinit   ----------------------------------
InitStatus CbmMvdDigiToHitTB::ReInit() { return kSUCCESS; }
// -------------------------------------------------------------------------


// -----   Virtual method Finish   -----------------------------------------
void CbmMvdDigiToHitTB::Finish()
{
  fDetector->Finish();
  PrintParameters();
}
// -------------------------------------------------------------------------


// -----   Private method Reset   ------------------------------------------
void CbmMvdDigiToHitTB::Reset() { fCluster->Delete(); }
// -------------------------------------------------------------------------

// -----   Private method GetMvdGeometry   ---------------------------------
void CbmMvdDigiToHitTB::GetMvdGeometry() {}
// -------------------------------------------------------------------------


// -----   Private method PrintParameters   --------------------------------
void CbmMvdDigiToHitTB::PrintParameters()
{

  cout << "============================================================" << endl;
  cout << "============== Parameters DigiToHit ====================" << endl;
  cout << "============================================================" << endl;
  cout << "=============== End Task ===================================" << endl;
}
// -------------------------------------------------------------------------


ClassImp(CbmMvdDigiToHitTB);
