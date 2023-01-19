/* Copyright (C) 2017-2020 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Philipp Sitzmann [committer] */

// -------------------------------------------------------------------------
// -----                    CbmMvdClusterfinder source file                -----
// -------------------------------------------------------------------------
#include "CbmMvdReadout.h"

#include "CbmDefs.h"                               // for ECbmModuleId
#include "CbmDigiManager.h"                        // for CbmDigiManager
#include "CbmMvdDetector.h"                        // for CbmMvdDetector
#include "CbmMvdDigi.h"                            // for CbmMvdDigi
#include "CbmMvdSensor.h"                          // for CbmMvdSensor
#include "CbmMvdSensorReadoutTask.h"               // for CbmMvdSensorReadoudoutTask

#include <FairRootManager.h>                       // for FairRootManager
#include <Logger.h>                                // for LOG

#include <iostream>                                // for operator<<, endl

using std::cout;
using std::endl;

// -----   Default constructor   ------------------------------------------
CbmMvdReadout::CbmMvdReadout() : CbmMvdReadout("MVDClusterfinder") {}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
CbmMvdReadout::CbmMvdReadout(const char* name)
  : FairTask(name)
  , fMode(-1)
  , fPluginNr(0)
  , fShowDebugHistos(kFALSE)
  , fDetector(nullptr)
  , fDigiMan(nullptr)
{
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmMvdReadout::~CbmMvdReadout() {}
// -----------------------------------------------------------------------------

// -----   Exec   --------------------------------------------------------------
void CbmMvdReadout::Exec(Option_t* /*opt*/)
{

  if (fDigiMan->GetNofDigis(ECbmModuleId::kMvd) > 0) {
    if (fVerbose) cout << "//----------------------------------------//";
    if (fVerbose) cout << endl << "Send Input" << endl;
    Int_t nTargetPlugin=fDetector->DetectPlugin(200);
    CbmMvdDigi* digi=0;

    Int_t nDigis = fDigiMan->GetNofDigis(ECbmModuleId::kMvd);

    for (Int_t i = 0; i < nDigis; i++) {
      digi = new CbmMvdDigi(*(fDigiMan->Get<CbmMvdDigi>(i)));
      digi->SetRefId(i);

      fDetector->SendInputToSensorPlugin(digi->GetDetectorId(), nTargetPlugin, static_cast<TObject*>(digi));
    }
    if (fVerbose) cout << "Execute ReadoutPlugin Nr. " << fPluginNr << endl;
    fDetector->Exec(fPluginNr);
    if (fVerbose) cout << "End Chain" << endl;
  }
}
// -----------------------------------------------------------------------------

// -----   Init   --------------------------------------------------------------
InitStatus CbmMvdReadout::Init()
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
  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->Init();

  if (!fDigiMan->IsPresent(ECbmModuleId::kMvd)) {
    LOG(error) << "No MvdDigi branch found. There was no MVD in the "
                  "simulation. Switch this task off";
    return kERROR;
  }

  fDetector = CbmMvdDetector::Instance();

  if (fDetector->GetSensorArraySize() > 1) {
    if (fVerbose) cout << endl << "-I- succesfully loaded Geometry from file -I-" << endl;
  }
  else {
    LOG(fatal) << "Geometry couldn't be loaded from file. No MVD digitizer available.";
  }

  // Add the readout plugin to all sensors
  std::map<int, CbmMvdSensor*>& sensorMap = fDetector->GetSensorMap();
  UInt_t plugincount=fDetector->GetPluginCount();

  for (auto itr = sensorMap.begin();
              itr != sensorMap.end(); itr++) {
    CbmMvdSensorReadoutTask* readerTask = new CbmMvdSensorReadoutTask();

    itr->second->AddPlugin(readerTask);
    // Don't know why type of plugin it is
    // Decide to use Cluster Plugin since it works on CbmMvdDigis as the
    // normal cluster plugin does
    itr->second->SetClusterPlugin(plugincount);
  }
  fDetector->SetSensorArrayFilled(kTRUE);
  fDetector->SetPluginCount(plugincount+1);

  fPluginNr = (UInt_t)(fDetector->GetPluginArraySize());
  fDetector->Init();


  // Screen output
  cout << GetName() << " initialised with parameters: " << endl;
  //PrintParameters();
  cout << "---------------------------------------------" << endl;


  return kSUCCESS;
}

// -----   Virtual public method Reinit   ----------------------------------
InitStatus CbmMvdReadout::ReInit() { return kSUCCESS; }
// -------------------------------------------------------------------------


// -----   Virtual method Finish   -----------------------------------------
void CbmMvdReadout::Finish()
{
  fDetector->Finish();
  PrintParameters();
}
// -------------------------------------------------------------------------


// -----   Private method Reset   ------------------------------------------
void CbmMvdReadout::Reset()
{
  // fCluster->Delete();
}
// -------------------------------------------------------------------------

// -----   Private method PrintParameters   --------------------------------
void CbmMvdReadout::PrintParameters() const
{

  //cout.setf(ios_base::fixed, ios_base::floatfield);
  cout << "============================================================" << endl;
  cout << "============== Parameters Readout ===== ====================" << endl;
  cout << "============================================================" << endl;
  cout << "=============== End Task ===================================" << endl;
}
// -------------------------------------------------------------------------


ClassImp(CbmMvdReadout);
