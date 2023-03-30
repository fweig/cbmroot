/* Copyright (C) 2014-2021 Institut fuer Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Philipp Sitzmann [committer], Florian Uhlig */

// -------------------------------------------------------------------------
// -----                    CbmMvdHitfinder source file                -----
// -------------------------------------------------------------------------
#include "CbmMvdHitfinder.h"

#include "CbmDefs.h"                               // for ECbmModuleId
#include "CbmDigiManager.h"                        // for CbmDigiManager
#include "CbmMvdCluster.h"                         // for CbmMvdCluster
#include "CbmMvdDetector.h"                        // for CbmMvdDetector
#include "CbmMvdDetectorId.h"                        // for CbmMvdDetector
#include "CbmMvdDigi.h"                            // for CbmMvdDigi
#include "CbmMvdSensor.h"                          // for CbmMvdSensor
#include "CbmMvdSensorFindHitTask.h"               // for CbmMvdSensorFindHi...
#include "CbmMvdSensorHitfinderTask.h"             // for CbmMvdSensorHitfin...

#include <FairTask.h>                              // for InitStatus, FairTask
#include <FairRootManager.h>                       // for FairRootManager
#include <Logger.h>                                // for Logger, LOG

#include <TClonesArray.h>                          // for TClonesArray

#include <iomanip>                                 // for setprecision, setw
#include <iostream>                                // for operator<<, endl
#include <map>                                     // for allocator, __map_i...
#include <utility>                                 // for pair

using std::cout;
using std::endl;
using std::fixed;
using std::ios_base;
using std::setprecision;
using std::setw;

// -----   Default constructor   ------------------------------------------
CbmMvdHitfinder::CbmMvdHitfinder()
  : FairTask("MVDHitfinder")
  , fDetector(nullptr)
  , fDigiMan(nullptr)
  , fInputCluster(nullptr)
  , fHits(nullptr)
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
  , fDetector(nullptr)
  , fDigiMan(nullptr)
  , fInputCluster(nullptr)
  , fHits(nullptr)
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
  , fDetector(nullptr)
  , fDigiMan(nullptr)
  , fInputCluster(nullptr)
  , fHits(nullptr)
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
  Int_t nTargetPlugin= fDetector->DetectPlugin(fMyPluginID);
  //Int_t nDigis=0;
  //CbmMvdDigi* digi=0;
  CbmMvdCluster* cluster=0;

  if (fDigiMan->IsPresent(ECbmModuleId::kMvd) || fInputCluster) { //checks if data sources are available
    if (fVerbose) cout << endl << "//----------------------------------------//" << endl;
    if (!fUseClusterfinder) {
      /*
      fDigiMan->GetNofDigis(ECbmModuleId::kMvd);
      for (Int_t i = 0; i < nDigis; i++) {
        digi = new CbmMvdDigi(*(fDigiMan->Get<CbmMvdDigi>(i)));
        digi->SetRefId(i);

        fDetector->SendInputToSensorPlugin(digi->GetDetectorId(), nTargetPlugin, static_cast<TObject*>(digi));
        */
      Fatal("CbmMvdHitFinder - Mode without cluster finder is currently not supported ", "CbmMvdHitFinder - Mode without cluster finder is currently not supported ");
      }



      //fDetector->SendInputDigis(fDigiMan);



    else // of if (!fUseClusterfinder)
    {


      CbmMvdDetectorId tmp;
      Int_t nEntries = fInputCluster->GetEntriesFast();
      for (Int_t i = 0; i < nEntries; i++) {
        cluster = (CbmMvdCluster*) fInputCluster->At(i);
        cluster->SetRefId(i);
        fDetector->SendInputToSensorPlugin(tmp.DetectorId(cluster->GetSensorNr()), nTargetPlugin, static_cast<TObject*>(cluster));

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

    //cout << "Total of " << fHits->GetEntriesFast() << " hits found" << endl;
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
    Fatal("CbmMvdHitFinder - Mode without cluster finder is currently not supported ", "CbmMvdHitFinder - Mode without cluster finder is currently not supported ");
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


  // Add the hit finder plugin to all sensors
  std::map<int, CbmMvdSensor*>& sensorMap = fDetector->GetSensorMap();
  UInt_t plugincount=fDetector->GetPluginCount();

    /*
  if (!fUseClusterfinder) {

    for (auto itr = sensorMap.begin(); itr != sensorMap.end(); itr++) {
      CbmMvdSensorFindHitTask* hitfinderTask = new CbmMvdSensorFindHitTask();

      itr->second->AddPlugin(hitfinderTask);
      itr->second->SetHitPlugin(plugincount);
      fMyPluginID=400;

    }

  else {*/
    for (auto itr = sensorMap.begin(); itr != sensorMap.end(); itr++) {
      CbmMvdSensorHitfinderTask* hitfinderTask = new CbmMvdSensorHitfinderTask();

      itr->second->AddPlugin(hitfinderTask);
      itr->second->SetHitPlugin(plugincount);
      fMyPluginID=300;
    }
  //}

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
