/* Copyright (C) 2019 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andreas Redelbach [committer] */

// -------------------------------------------------------------------------
// -----                    CbmMvdDigiToHit source file                -----
// -------------------------------------------------------------------------

// Includes from MVD
#include "CbmMvdDigiToHit.h"

#include "CbmMvdDetector.h"
#include "CbmMvdGeoHandler.h"
#include "CbmMvdPoint.h"
#include "CbmMvdSensorDigiToHitTask.h"

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

using std::endl;
using std::fixed;
using std::setprecision;
using std::setw;


// -----   Default constructor   ------------------------------------------
CbmMvdDigiToHit::CbmMvdDigiToHit()
  : FairTask("CbmMvdDigiToHit")
  , fMode(0)
  , fShowDebugHistos(kFALSE)
  , fDetector(nullptr)
  , fInputDigis(nullptr)
  , fHit(nullptr)
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
  , fDetector(nullptr)
  , fInputDigis(nullptr)
  , fHit(nullptr)
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
    if (fVerbose) LOG(debug) << "//----------------------------------------//";
    if (fVerbose) LOG(debug) << "Send Input";
    fDetector->SendInputDigisToHits(fInputDigis);  //Version for DigisToHits
    if (fVerbose) LOG(debug) << "Execute HitPlugin Nr. " << fHitPluginNr;
    fDetector->Exec(fHitPluginNr);
    if (fVerbose) LOG(debug) << "End Chain";
    if (fVerbose) LOG(debug) << "Start writing Hit";
    fHit->AbsorbObjects(fDetector->GetOutputHits(), 0, fDetector->GetOutputHits()->GetEntriesFast() - 1);
    if (fVerbose) LOG(debug) << "Total of " << fHit->GetEntriesFast() << " Hit in this Event";
    if (fVerbose) LOG(debug) << "//----------------------------------------//";
    LOG(info) << "+ " << setw(20) << GetName() << ": Created: " << fHit->GetEntriesFast() << " Hit in " << fixed
              << setprecision(6) << fTimer.RealTime() << " s";
  }

  fTimer.Stop();
}
// -----------------------------------------------------------------------------

// -----   Init   --------------------------------------------------------------
InitStatus CbmMvdDigiToHit::Init()
{
  LOG(info) << GetName() << ": Initialisation..." << endl;

  // **********  RootManager
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman) {
    LOG(error) << GetName() << "::Init: No FairRootManager!";
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
    if (fVerbose) LOG(info) << "succesfully loaded Geometry from file";
  }
  else {
    LOG(fatal) << "Geometry couldn't be loaded from file. No MVD digitizer available.";
  }

  // Add the digi2hit plugin to all sensors
  std::map<int, CbmMvdSensor*>& sensorMap = fDetector->GetSensorMap();
  UInt_t plugincount                      = fDetector->GetPluginCount();

  for (auto itr = sensorMap.begin(); itr != sensorMap.end(); itr++) {
    CbmMvdSensorDigiToHitTask* hitTask = new CbmMvdSensorDigiToHitTask();

    itr->second->AddPlugin(hitTask);
    itr->second->SetHitPlugin(plugincount);
  }
  fDetector->SetSensorArrayFilled(kTRUE);
  fDetector->SetPluginCount(plugincount + 1);
  fHitPluginNr = (UInt_t)(fDetector->GetPluginArraySize());

  if (fShowDebugHistos) fDetector->ShowDebugHistos();
  fDetector->Init();


  // Screen output
  LOG(info) << GetName() << " initialised with parameters: ";
  //PrintParameters();


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
void CbmMvdDigiToHit::PrintParameters() const { LOG(info) << ParametersToString(); }

// -----   Private method ParametersToString   -----------------------------
void CbmMvdDigiToHit::ParametersToString() const
{
  std::stringstream ss;
  ss << "============================================================" << endl;
  ss << "============== Parameters DigiToHit ====================" << endl;
  ss << "============================================================" << endl;
  ss << "=============== End Task ===================================" << endl;
  return ss.str()
}
// -------------------------------------------------------------------------


ClassImp(CbmMvdDigiToHit);
