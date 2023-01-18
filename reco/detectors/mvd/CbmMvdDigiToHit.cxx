/* Copyright (C) 2019 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andreas Redelbach [committer] */

// -------------------------------------------------------------------------
// -----                    CbmMvdDigiToHit source file                -----
// -------------------------------------------------------------------------

// Includes from MVD
#include "CbmMvdDigiToHit.h"

#include "CbmMvdDetector.h"
#include "CbmMvdPoint.h"
#include "SensorDataSheets/CbmMvdMimosa26AHR.h"
#include "plugins/tasks/CbmMvdSensorDigiToHitTask.h"
#include "tools/CbmMvdGeoHandler.h"

// Includes from FAIR
#include "FairModule.h"
#include "FairRootManager.h"


// Includes from ROOT
#include "TClonesArray.h"
#include "TGeoManager.h"
#include "TMath.h"
#include "TStopwatch.h"
#include "TString.h"

#include <chrono>
//#include <omp.h>
#include <cstring>
#include <fstream>


// Includes from C++
#include <iomanip>
#include <iostream>

using std::cout;
using std::endl;
using std::fixed;
using std::setprecision;
using std::setw;


// -----   Default constructor   ------------------------------------------
CbmMvdDigiToHit::CbmMvdDigiToHit()
  : FairTask("CbmMvdDigiToHit")
  , fMode(0)
  , fShowDebugHistos(kFALSE)
  , fDetector(NULL)
  , fInputDigis(NULL)
  , fHit(NULL)
  , fHitPluginNr()
  , fBranchName("")
  , fTimer()
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
CbmMvdDigiToHit::CbmMvdDigiToHit(const char* name, Int_t iMode, Int_t iVerbose)
  : FairTask(name, iVerbose)
  , fMode(iMode)
  , fShowDebugHistos(kFALSE)
  , fDetector(NULL)
  , fInputDigis(NULL)
  , fHit(NULL)
  , fHitPluginNr(0)
  , fBranchName("MvdDigi")
  , fTimer()
{
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmMvdDigiToHit::~CbmMvdDigiToHit()
{

  if (fHit) {
    fHit->Delete();
    delete fHit;
  }
}
// -----------------------------------------------------------------------------

// -----   Exec   --------------------------------------------------------------
void CbmMvdDigiToHit::Exec(Option_t* /*opt*/)
{
  // --- Start timer

  fTimer.Start();

  fHit->Delete();
  if (fInputDigis && fInputDigis->GetEntriesFast() > 0) {
    if (fVerbose) cout << "//----------------------------------------//";
    if (fVerbose) cout << endl << "Send Input" << endl;
    fDetector->SendInputDigisToHits(fInputDigis);  //Version for DigisToHits
    if (fVerbose) cout << "Execute HitPlugin Nr. " << fHitPluginNr << endl;
    fDetector->Exec(fHitPluginNr);
    if (fVerbose) cout << "End Chain" << endl;
    if (fVerbose) cout << "Start writing Hit" << endl;
    fHit->AbsorbObjects(fDetector->GetOutputHits(), 0, fDetector->GetOutputHits()->GetEntriesFast() - 1);
    if (fVerbose) cout << "Total of " << fHit->GetEntriesFast() << " Hit in this Event" << endl;
    if (fVerbose) cout << "//----------------------------------------//" << endl;
    LOG(info) << "+ " << setw(20) << GetName() << ": Created: " << fHit->GetEntriesFast() << " Hit in " << fixed
              << setprecision(6) << fTimer.RealTime() << " s";
  }

  fTimer.Stop();
}
// -----------------------------------------------------------------------------

// -----   Init   --------------------------------------------------------------
InitStatus CbmMvdDigiToHit::Init()
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
  fInputDigis = (TClonesArray*) ioman->GetObject("MvdDigi");

  if (!fInputDigis) {
    LOG(error) << "No MvdDigi branch found. There was no MVD in the "
                  "simulation. Switch this task off";
    return kERROR;
  }

  // **********  Register output array
  fHit = new TClonesArray("CbmMvdHit", 10000);
  ioman->Register("MvdHit", "Mvd Hit", fHit, IsOutputBranchPersistent("MvdHit"));

  fDetector = CbmMvdDetector::Instance();

  if (fDetector->GetSensorArraySize() > 1) {
    if (fVerbose) cout << endl << "-I- succesfully loaded Geometry from file -I-" << endl;
  }
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
  cout << GetName() << " initialised with parameters: " << endl;
  //PrintParameters();
  cout << "---------------------------------------------" << endl;


  return kSUCCESS;
}

// -----   Virtual public method Reinit   ----------------------------------
InitStatus CbmMvdDigiToHit::ReInit() { return kSUCCESS; }
// -------------------------------------------------------------------------


// -----   Virtual method Finish   -----------------------------------------
void CbmMvdDigiToHit::Finish()
{
  fDetector->Finish();
  PrintParameters();
}
// -------------------------------------------------------------------------


// -----   Private method Reset   ------------------------------------------
void CbmMvdDigiToHit::Reset() { fHit->Delete(); }
// -------------------------------------------------------------------------

// -----   Private method GetMvdGeometry   ---------------------------------
void CbmMvdDigiToHit::GetMvdGeometry() {}
// -------------------------------------------------------------------------


// -----   Private method PrintParameters   --------------------------------
void CbmMvdDigiToHit::PrintParameters()
{

  cout << "============================================================" << endl;
  cout << "============== Parameters DigiToHit ====================" << endl;
  cout << "============================================================" << endl;
  cout << "=============== End Task ===================================" << endl;
}
// -------------------------------------------------------------------------


ClassImp(CbmMvdDigiToHit);
