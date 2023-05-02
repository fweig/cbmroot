/* Copyright (C) 2017-2019 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Philipp Sitzmann [committer] */

// -------------------------------------------------------------------------
// -----                    CbmMvdHitfinderTB source file                -----
// -------------------------------------------------------------------------

// Includes from MVD
#include "CbmMvdHitfinderTB.h"

#include "CbmMvdGeoHandler.h"
#include "CbmMvdPoint.h"
#include "CbmMvdSensorFindHitTask.h"
#include "CbmMvdSensorHitfinderTask.h"


// Includes from FAIR
#include "FairModule.h"
#include "FairRootManager.h"

// Includes from ROOT
#include "TClonesArray.h"


// Includes from C++
#include <iomanip>
#include <iostream>

using std::endl;
using std::fixed;
using std::ios_base;
using std::setprecision;
using std::setw;

// -----   Default constructor   ------------------------------------------
CbmMvdHitfinderTB::CbmMvdHitfinderTB()
  : FairTask("MVDHitfinder")
  , fDetector(nullptr)
  , fInputDigis(nullptr)
  , fInputCluster(nullptr)
  , fHits(nullptr)
  , fHitfinderPluginNr(0)
  , useClusterfinder(kFALSE)
  , fShowDebugHistos(kFALSE)
  , fTimer()
  , fmode(-1)
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
CbmMvdHitfinderTB::CbmMvdHitfinderTB(const char* name, Int_t iVerbose)
  : FairTask(name, iVerbose)
  , fDetector(nullptr)
  , fInputDigis(nullptr)
  , fInputCluster(nullptr)
  , fHits(nullptr)
  , fHitfinderPluginNr(0)
  , useClusterfinder(kFALSE)
  , fShowDebugHistos(kFALSE)
  , fTimer()
  , fmode(-1)
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
CbmMvdHitfinderTB::CbmMvdHitfinderTB(const char* name, Int_t mode, Int_t iVerbose)
  : FairTask(name, iVerbose)
  , fDetector(nullptr)
  , fInputDigis(nullptr)
  , fInputCluster(nullptr)
  , fHits(nullptr)
  , fHitfinderPluginNr(0)
  , useClusterfinder(kFALSE)
  , fShowDebugHistos(kFALSE)
  , fTimer()
  , fmode(mode)
{
  //    fmode = mode;
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmMvdHitfinderTB::~CbmMvdHitfinderTB()
{

  if (fHits) {
    fHits->Delete();
    delete fHits;
  }
}
// -----------------------------------------------------------------------------

// -----   Exec   --------------------------------------------------------------
void CbmMvdHitfinderTB::Exec(Option_t* /*opt*/)
{

  using namespace std;

  fHits->Clear();
  fTimer.Start();

  LOG(debug) << endl << "//----------------------------------------//";
  fDetector->SendInputCluster(fInputCluster);
  LOG(debug) << "Execute HitfinderPlugin Nr. " << fHitfinderPluginNr;
  fDetector->Exec(fHitfinderPluginNr);
  LOG(debug) << "End Chain";
  LOG(debug) << "Start writing Hits";
  fHits->AbsorbObjects(fDetector->GetOutputHits(), 0, fDetector->GetOutputHits()->GetEntriesFast() - 1);
  LOG(debug) << "Total of " << fHits->GetEntriesFast() << " hits found";
  LOG(debug) << "Finished writing Hits";
  LOG(debug) << "//----------------------------------------//";
  LOG(info) << "+ " << setw(20) << GetName() << ": Created: " << fHits->GetEntriesFast() << " hits in " << fixed
            << setprecision(6) << fTimer.RealTime() << " s";
  fTimer.Stop();
}
// -----------------------------------------------------------------------------

// -----   Init   --------------------------------------------------------------
InitStatus CbmMvdHitfinderTB::Init()
{

  using namespace std;

  LOG(info) << GetName() << ": Initialisation...";

  // **********  RootManager
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman) {
    LOG(error) << GetName() << "::Init: No FairRootManager!";
    return kFATAL;
  }

  // **********  Get input arrays

  fInputCluster = (TClonesArray*) ioman->GetObject("MvdCluster");
  if (!fInputCluster) {
    LOG(error) << "No MvdCluster branch found. There was no MVD in the "
                  "simulation. Switch this task off";
    return kERROR;
  }

  // **********  Register output array
  fHits = new TClonesArray("CbmMvdHit", 10000);
  ioman->Register("MvdHit", "Mvd Hits", fHits, IsOutputBranchPersistent("MvdHit"));

  fDetector = CbmMvdDetector::Instance();


  // Add the hit finder plugin to all sensors
  std::map<int, CbmMvdSensor*>& sensorMap = fDetector->GetSensorMap();
  UInt_t plugincount                      = fDetector->GetPluginCount();

  for (auto itr = sensorMap.begin(); itr != sensorMap.end(); itr++) {
    CbmMvdSensorHitfinderTask* hitfinderTask = new CbmMvdSensorHitfinderTask();

    itr->second->AddPlugin(hitfinderTask);
    itr->second->SetHitPlugin(plugincount);
  }
  fDetector->SetSensorArrayFilled(kTRUE);
  fDetector->SetPluginCount(plugincount + 1);
  fHitfinderPluginNr = (UInt_t)(fDetector->GetPluginArraySize());

  if (fShowDebugHistos) fDetector->ShowDebugHistos();
  fDetector->Init();

  // Screen output
  LOG(info) << GetName() << " initialised with parameters: ";
  //PrintParameters();


  return kSUCCESS;
}

// -----   Virtual public method Reinit   ----------------------------------
InitStatus CbmMvdHitfinderTB::ReInit() { return kSUCCESS; }
// -------------------------------------------------------------------------


// -----   Virtual method Finish   -----------------------------------------
void CbmMvdHitfinderTB::Finish() { PrintParameters(); }
// -------------------------------------------------------------------------


// -----   Private method Reset   ------------------------------------------
void CbmMvdHitfinderTB::Reset() { fHits->Delete(); }
// -------------------------------------------------------------------------

// -----   Private method GetMvdGeometry   ---------------------------------
void CbmMvdHitfinderTB::GetMvdGeometry() {}
// -------------------------------------------------------------------------

void CbmMvdHitfinderTB::PrintParameters() const { LOG(info) << ParametersToString(); }

// -----   Private method PrintParameters   --------------------------------
std::string CbmMvdHitfinderTB::ParametersToString() const
{

  std::stringstream ss;
  ss.setf(std::ios_base::fixed, std::ios_base::floatfield);
  ss << "============================================================" << endl;
  ss << "============== Parameters MvdHitfinder =====================" << endl;
  ss << "============================================================" << endl;
  ss << "=============== End Task ===================================" << endl;
  return ss.str();
}
// -------------------------------------------------------------------------

ClassImp(CbmMvdHitfinderTB);
