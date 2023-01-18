/* Copyright (C) 2014-2021 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Philipp Sitzmann [committer], Florian Uhlig */

// -------------------------------------------------------------------------
// -----                    CbmMvdHitfinder source file                -----
// -------------------------------------------------------------------------

// Includes from MVD
#include "CbmMvdHitfinder.h"

#include "CbmMvdPoint.h"
#include "SensorDataSheets/CbmMvdMimosa26AHR.h"
//#include "plugins/tasks/CbmMvdSensorFindHitTask.h"
#include "CbmDigiManager.h"
#include "plugins/tasks/CbmMvdSensorHitfinderTask.h"
#include "tools/CbmMvdGeoHandler.h"


// Includes from FAIR
#include "FairModule.h"
#include "FairRootManager.h"

// Includes from ROOT
#include "TClonesArray.h"


// Includes from C++
#include <iomanip>
#include <iostream>

using std::cout;
using std::endl;
using std::fixed;
using std::ios_base;
using std::setprecision;
using std::setw;

// -----   Default constructor   ------------------------------------------
CbmMvdHitfinder::CbmMvdHitfinder()
  : FairTask("MVDHitfinder")
  , fDetector(NULL)
  , fDigiMan(nullptr)
  , fInputCluster(NULL)
  , fHits(NULL)
  , fHitfinderPluginNr(0)
  , fUseClusterfinder(kFALSE)
  , fShowDebugHistos(kFALSE)
  , fTimer()
  , fmode(-1)
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
CbmMvdHitfinder::CbmMvdHitfinder(const char* name, Int_t iVerbose)
  : FairTask(name, iVerbose)
  , fDetector(NULL)
  , fDigiMan(nullptr)
  , fInputCluster(NULL)
  , fHits(NULL)
  , fHitfinderPluginNr(0)
  , fUseClusterfinder(kFALSE)
  , fShowDebugHistos(kFALSE)
  , fTimer()
  , fmode(-1)
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
CbmMvdHitfinder::CbmMvdHitfinder(const char* name, Int_t mode, Int_t iVerbose)
  : FairTask(name, iVerbose)
  , fDetector(NULL)
  , fDigiMan(nullptr)
  , fInputCluster(NULL)
  , fHits(NULL)
  , fHitfinderPluginNr(0)
  , fUseClusterfinder(kFALSE)
  , fShowDebugHistos(kFALSE)
  , fTimer()
  , fmode(mode)
{
  //    fmode = mode;
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmMvdHitfinder::~CbmMvdHitfinder()
{

  if (fHits) {
    fHits->Delete();
    delete fHits;
  }
}
// -----------------------------------------------------------------------------

// -----   Exec   --------------------------------------------------------------
void CbmMvdHitfinder::Exec(Option_t* /*opt*/)
{

  using namespace std;

  fHits->Clear();
  fTimer.Start();
  Int_t nTargetPlugin= fDetector->DetectPlugin(300);
  Int_t nDigis=0;
  CbmMvdDigi* digi=0;
  CbmMvdCluster* cluster=0;

  if (fDigiMan->IsPresent(ECbmModuleId::kMvd) || fInputCluster) { //checks if data sources are available
    if (fVerbose) cout << endl << "//----------------------------------------//" << endl;
    if (!fUseClusterfinder) {
      fDigiMan->GetNofDigis(ECbmModuleId::kMvd);
      for (Int_t i = 0; i < nDigis; i++) {
        digi = new CbmMvdDigi(*(fDigiMan->Get<CbmMvdDigi>(i)));
        digi->SetRefId(i);

        fDetector->SendInputToSensorPlugin(digi->GetDetectorId(), nTargetPlugin, static_cast<TObject*>(digi));
      }
    }


      //fDetector->SendInputDigis(fDigiMan);



    else // of if (!fUseClusterfinder)
    {


      Int_t nEntries = fInputCluster->GetEntriesFast();
      for (Int_t i = 0; i < nEntries; i++) {
        cluster = (CbmMvdCluster*) fInputCluster->At(i);
        cluster->SetRefId(i);
        fDetector->SendInputToSensorPlugin(cluster->GetDetectorId(), nTargetPlugin, static_cast<TObject*>(cluster));

      }
    }

        //fDetector->SendInputCluster(fInputCluster);


    if (fVerbose) cout << "Execute HitfinderPlugin Nr. " << fHitfinderPluginNr << endl;


    fDetector->Exec(fHitfinderPluginNr);
    if (fVerbose) cout << "End Chain" << endl;
    if (fVerbose) cout << "Start writing Hits" << endl;
    fDetector->GetOutputArray(nTargetPlugin, fHits);

    //fDetector->GetMatchArray (nTargetPlugin, fTmpMatch);
    //fHits->AbsorbObjects(fDetector->GetOutputHits(), 0, fDetector->GetOutputHits()->GetEntriesFast() - 1);

    cout << "Total of " << fHits->GetEntriesFast() << " hits found" << endl;
    if (fVerbose) cout << "Finished writing Hits" << endl;
    if (fVerbose) cout << "//----------------------------------------//" << endl << endl;
    LOG(info) << "+ " << setw(20) << GetName() << ": Created: " << fHits->GetEntriesFast() << " hits in " << fixed
              << setprecision(6) << fTimer.RealTime() << " s";
  }
  fTimer.Stop();
}
// -----------------------------------------------------------------------------

// -----   Init   --------------------------------------------------------------
InitStatus CbmMvdHitfinder::Init()
{

  using namespace std;

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
  if (!fUseClusterfinder) {
    fDigiMan = CbmDigiManager::Instance();
    fDigiMan->Init();
    if (!fDigiMan->IsPresent(ECbmModuleId::kMvd)) {
      LOG(error) << "No MvdDigi branch found. There was no MVD in the "
                    "simulation. Switch this task off";
      return kERROR;
    }
  }
  else {
    fInputCluster = (TClonesArray*) ioman->GetObject("MvdCluster");
    if (!fInputCluster) {
      LOG(error) << "No MvdCluster branch found. There was no MVD in the "
                    "simulation. Switch this task off";
      return kERROR;
    }
  }


  // **********  Register output array
  fHits = new TClonesArray("CbmMvdHit", 10000);
  ioman->Register("MvdHit", "Mvd Hits", fHits, IsOutputBranchPersistent("MvdHit"));

  fDetector = CbmMvdDetector::Instance();


  // Add the digitizer plugin to all sensors
  std::map<int, CbmMvdSensor*>& sensorMap = fDetector->GetSensorMap();
  UInt_t plugincount=fDetector->GetPluginCount();

  for (auto itr = sensorMap.begin();
              itr != sensorMap.end(); itr++) {
    CbmMvdSensorHitfinderTask* hitfinderTask = new CbmMvdSensorHitfinderTask();

    itr->second->AddPlugin(hitfinderTask);
    itr->second->SetDigiPlugin(plugincount);
  }
  fDetector->SetSensorArrayFilled(kTRUE);
  fDetector->SetPluginCount(plugincount+1);
  fHitfinderPluginNr = (UInt_t)(fDetector->GetPluginArraySize());

  if (fShowDebugHistos) fDetector->ShowDebugHistos();
  fDetector->Init();


  // Screen output
  cout << GetName() << " initialised with parameters: " << endl;
  //PrintParameters();
  cout << "---------------------------------------------" << endl;


  return kSUCCESS;
}

// -----   Virtual public method Reinit   ----------------------------------
InitStatus CbmMvdHitfinder::ReInit() { return kSUCCESS; }
// -------------------------------------------------------------------------


// -----   Virtual method Finish   -----------------------------------------
void CbmMvdHitfinder::Finish() { PrintParameters(); }
// -------------------------------------------------------------------------


// -----   Private method Reset   ------------------------------------------
void CbmMvdHitfinder::Reset() { fHits->Delete(); }
// -------------------------------------------------------------------------

// -----   Private method GetMvdGeometry   ---------------------------------
void CbmMvdHitfinder::GetMvdGeometry() {}
// -------------------------------------------------------------------------


// -----   Private method PrintParameters   --------------------------------
void CbmMvdHitfinder::PrintParameters()
{

  using namespace std;

  cout.setf(ios_base::fixed, ios_base::floatfield);
  cout << "============================================================" << endl;
  cout << "============== Parameters MvdHitfinder =====================" << endl;
  cout << "============================================================" << endl;
  cout << "=============== End Task ===================================" << endl;
}
// -------------------------------------------------------------------------

ClassImp(CbmMvdHitfinder);
