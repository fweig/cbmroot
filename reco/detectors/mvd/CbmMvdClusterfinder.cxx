/* Copyright (C) 2014-2020 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Philipp Sitzmann [committer] */

// -------------------------------------------------------------------------
// -----                    CbmMvdClusterfinder source file                -----
// -------------------------------------------------------------------------
#include "CbmMvdClusterfinder.h"

#include "CbmDefs.h"                        // for ECbmModuleId
#include "CbmDigiManager.h"                 // for CbmDigiManager
#include "CbmMvdDetector.h"                 // for CbmMvdDetector
#include "CbmMvdDigi.h"                     // for CbmMvdDigi
#include "CbmMvdSensor.h"                   // for CbmMvdSensor
#include "CbmMvdSensorClusterfinderTask.h"  // for CbmMvdSensorCluste...

#include <FairRootManager.h>  // for FairRootManager
#include <FairTask.h>         // for InitStatus, FairTask
#include <Logger.h>           // for Logger, LOG

#include <TClonesArray.h>  // for TClonesArray

#include <iomanip>   // for setprecision, setw
#include <iostream>  // for operator<<, endl
#include <map>       // for allocator, __map_i...
#include <utility>   // for pair


using std::endl;
using std::fixed;
using std::setprecision;
using std::setw;

// -----   Default constructor   ------------------------------------------
CbmMvdClusterfinder::CbmMvdClusterfinder()
  : FairTask("MVDClusterfinder")
  , fMode(0)
  , fShowDebugHistos(kFALSE)
  , fDetector(nullptr)
  , fDigiMan(nullptr)
  , fCluster(nullptr)
  , fClusterPluginNr()
  , fBranchName("")
  , fTimer()
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
CbmMvdClusterfinder::CbmMvdClusterfinder(const char* name, Int_t iMode, Int_t iVerbose)
  : FairTask(name, iVerbose)
  , fMode(iMode)
  , fShowDebugHistos(kFALSE)
  , fDetector(nullptr)
  , fDigiMan(nullptr)
  , fCluster(nullptr)
  , fClusterPluginNr(0)
  , fBranchName("MvdDigi")
  , fTimer()
{
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmMvdClusterfinder::~CbmMvdClusterfinder()
{

  if (fCluster) {
    fCluster->Delete();
    delete fCluster;
  }
}
// -----------------------------------------------------------------------------

// -----   Exec   --------------------------------------------------------------
void CbmMvdClusterfinder::Exec(Option_t* /*opt*/)
{
  // --- Start timer
  fTimer.Start();
  LOG(debug) << "CbmMvdClusterfinder::Exec : Starting Exec ";
  fCluster->Delete();
  if (fDigiMan->GetNofDigis(ECbmModuleId::kMvd)) {
    if (fVerbose) LOG(debug) << "//----------------------------------------//";
    if (fVerbose) LOG(debug) << "Send Input";

    Int_t nTargetPlugin = fDetector->DetectPlugin(200);
    CbmMvdDigi* digi    = 0;

    Int_t nDigis = fDigiMan->GetNofDigis(ECbmModuleId::kMvd);

    LOG(debug) << "CbmMvdClusterfinder::Exec - nDigis= " << nDigis;

    for (Int_t i = 0; i < nDigis; i++) {
      digi = new CbmMvdDigi(*(fDigiMan->Get<CbmMvdDigi>(i)));
      digi->SetRefId(i);

      fDetector->SendInputToSensorPlugin(digi->GetDetectorId(), nTargetPlugin, static_cast<TObject*>(digi));
    }


    LOG(debug) << "CbmMvdClusterfinder::Exec : Communicating with Plugin: " << nTargetPlugin;


    //fDetector->SendInputDigis(fDigiMan);
    if (fVerbose) LOG(debug) << "Execute ClusterPlugin Nr. " << fClusterPluginNr;
    fDetector->Exec(nTargetPlugin);
    if (fVerbose) LOG(debug) << "End Chain";
    if (fVerbose) LOG(debug) << "Start writing Cluster";

    fDetector->GetOutputArray(nTargetPlugin, fCluster);
    //fDetector->GetMatchArray (nTargetPlugin, fTmpMatch);
    //fCluster->AbsorbObjects(fDetector->GetOutputCluster(), 0, fDetector->GetOutputCluster()->GetEntriesFast() - 1);

    if (fVerbose) LOG(debug) << "Total of " << fCluster->GetEntriesFast() << " Cluster in this Event";
    if (fVerbose) LOG(debug) << "//----------------------------------------//";
    LOG(info) << "+ " << setw(20) << GetName() << ": Created: " << fCluster->GetEntriesFast() << " cluster in " << fixed
              << setprecision(6) << fTimer.RealTime() << " s";
  }

  fTimer.Stop();
}
// -----------------------------------------------------------------------------

// -----   Init   --------------------------------------------------------------
InitStatus CbmMvdClusterfinder::Init()
{
  LOG(info) << GetName() << ": Initialisation...";

  // **********  RootManager
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman) {
    LOG(error) << GetName() << "::Init: No FairRootManager!";
    return kFATAL;
  }

  // **********  Get input array
  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->Init();
  if (!fDigiMan->IsPresent(ECbmModuleId::kMvd)) {
    LOG(error) << "No MvdDigi branch found. There was no MVD in the "
                  "simulation. Switch this task off";
    return kERROR;
  }

  // **********  Register output array
  fCluster = new TClonesArray("CbmMvdCluster", 10000);
  ioman->Register("MvdCluster", "Mvd Clusters", fCluster, IsOutputBranchPersistent("MvdCluster"));

  fDetector = CbmMvdDetector::Instance();

  if (fDetector->GetSensorArraySize() > 1) {
    if (fVerbose) LOG(info) << "succesfully loaded Geometry from file";
  }
  else {
    LOG(fatal) << "Geometry couldn't be loaded from file. No MVD digitizer available.";
  }

  // Add the cluster finder plugin to all sensors
  std::map<int, CbmMvdSensor*>& sensorMap = fDetector->GetSensorMap();
  UInt_t plugincount                      = fDetector->GetPluginCount();

  for (auto itr = sensorMap.begin(); itr != sensorMap.end(); itr++) {
    CbmMvdSensorClusterfinderTask* clusterTask = new CbmMvdSensorClusterfinderTask();

    itr->second->AddPlugin(clusterTask);
    itr->second->SetClusterPlugin(plugincount);
  }
  fDetector->SetSensorArrayFilled(kTRUE);
  fDetector->SetPluginCount(plugincount + 1);
  fClusterPluginNr = (UInt_t)(fDetector->GetPluginArraySize());

  if (fShowDebugHistos) fDetector->ShowDebugHistos();
  fDetector->Init();


  // Screen output
  LOG(info) << GetName() << " initialised with parameters: ";
  //PrintParameters();


  return kSUCCESS;
}

// -----   Virtual public method Reinit   ----------------------------------
InitStatus CbmMvdClusterfinder::ReInit() { return kSUCCESS; }
// -------------------------------------------------------------------------


// -----   Virtual method Finish   -----------------------------------------
void CbmMvdClusterfinder::Finish()
{
  fDetector->Finish();
  PrintParameters();
}
// -------------------------------------------------------------------------


// -----   Private method Reset   ------------------------------------------
void CbmMvdClusterfinder::Reset() { fCluster->Delete(); }
// -------------------------------------------------------------------------

// -----   Private method GetMvdGeometry   ---------------------------------
void CbmMvdClusterfinder::GetMvdGeometry() {}
// -------------------------------------------------------------------------

// -----   Private method PrintParameters   --------------------------------
void CbmMvdClusterfinder::PrintParameters() const { LOG(info) << ParametersToString(); }

// -----   Private method ParametersToString   -----------------------------
std::string CbmMvdClusterfinder::ParametersToString() const
{
  std::stringstream ss;
  ss << "============================================================" << endl;
  ss << "============== Parameters Clusterfinder ====================" << endl;
  ss << "============================================================" << endl;
  ss << "=============== End Task ===================================" << endl;
  return ss.str();
}
// -------------------------------------------------------------------------


ClassImp(CbmMvdClusterfinder);
