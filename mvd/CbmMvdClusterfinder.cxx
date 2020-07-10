// -------------------------------------------------------------------------
// -----                    CbmMvdClusterfinder source file                -----
// -------------------------------------------------------------------------

// Includes from MVD
#include "CbmMvdClusterfinder.h"
#include "CbmMvdDetector.h"
#include "CbmMvdPoint.h"
#include "SensorDataSheets/CbmMvdMimosa26AHR.h"
#include "plugins/tasks/CbmMvdSensorClusterfinderTask.h"
#include "tools/CbmMvdGeoHandler.h"

#include "CbmDigiManager.h"

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
CbmMvdClusterfinder::CbmMvdClusterfinder()
  : FairTask("MVDClusterfinder")
  , fMode(0)
  , fShowDebugHistos(kFALSE)
  , fDetector(NULL)
  , fDigiMan(nullptr)
  , fCluster(NULL)
  , fClusterPluginNr()
  , fBranchName("")
  , fTimer() {}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
CbmMvdClusterfinder::CbmMvdClusterfinder(const char* name,
                                         Int_t iMode,
                                         Int_t iVerbose)
  : FairTask(name, iVerbose)
  , fMode(iMode)
  , fShowDebugHistos(kFALSE)
  , fDetector(NULL)
  , fDigiMan(nullptr)
  , fCluster(NULL)
  , fClusterPluginNr(0)
  , fBranchName("MvdDigi")
  , fTimer() {}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmMvdClusterfinder::~CbmMvdClusterfinder() {

  if (fCluster) {
    fCluster->Delete();
    delete fCluster;
  }
}
// -----------------------------------------------------------------------------

// -----   Exec   --------------------------------------------------------------
void CbmMvdClusterfinder::Exec(Option_t* /*opt*/) {
  // --- Start timer
  fTimer.Start();

  fCluster->Delete();
  if (fDigiMan->GetNofDigis(ECbmModuleId::kMvd)) {
    if (fVerbose) cout << "//----------------------------------------//";
    if (fVerbose) cout << endl << "Send Input" << endl;
    fDetector->SendInputDigis(fDigiMan);
    if (fVerbose)
      cout << "Execute ClusterPlugin Nr. " << fClusterPluginNr << endl;
    fDetector->Exec(fClusterPluginNr);
    if (fVerbose) cout << "End Chain" << endl;
    if (fVerbose) cout << "Start writing Cluster" << endl;
    fCluster->AbsorbObjects(fDetector->GetOutputCluster(),
                            0,
                            fDetector->GetOutputCluster()->GetEntriesFast()
                              - 1);
    if (fVerbose)
      cout << "Total of " << fCluster->GetEntriesFast()
           << " Cluster in this Event" << endl;
    if (fVerbose)
      cout << "//----------------------------------------//" << endl;
    LOG(info) << "+ " << setw(20) << GetName()
              << ": Created: " << fCluster->GetEntriesFast() << " cluster in "
              << fixed << setprecision(6) << fTimer.RealTime() << " s";
  }

  fTimer.Stop();
}
// -----------------------------------------------------------------------------

// -----   Init   --------------------------------------------------------------
InitStatus CbmMvdClusterfinder::Init() {
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
  ioman->Register("MvdCluster",
                  "Mvd Clusters",
                  fCluster,
                  IsOutputBranchPersistent("MvdCluster"));

  fDetector = CbmMvdDetector::Instance();

  if (fDetector->GetSensorArraySize() > 1) {
    if (fVerbose)
      cout << endl << "-I- succesfully loaded Geometry from file -I-" << endl;
  } else {
    LOG(fatal)
      << "Geometry couldn't be loaded from file. No MVD digitizer available.";
  }

  CbmMvdSensorClusterfinderTask* clusterTask =
    new CbmMvdSensorClusterfinderTask();

  fDetector->AddPlugin(clusterTask);
  fClusterPluginNr = (UInt_t)(fDetector->GetPluginArraySize());
  if (fShowDebugHistos) fDetector->ShowDebugHistos();
  fDetector->Init();


  // Screen output
  cout << GetName() << " initialised with parameters: " << endl;
  //PrintParameters();
  cout << "---------------------------------------------" << endl;


  return kSUCCESS;
}

// -----   Virtual public method Reinit   ----------------------------------
InitStatus CbmMvdClusterfinder::ReInit() { return kSUCCESS; }
// -------------------------------------------------------------------------


// -----   Virtual method Finish   -----------------------------------------
void CbmMvdClusterfinder::Finish() {
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
void CbmMvdClusterfinder::PrintParameters() {

  cout << "============================================================"
       << endl;
  cout << "============== Parameters Clusterfinder ===================="
       << endl;
  cout << "============================================================"
       << endl;
  cout << "=============== End Task ==================================="
       << endl;
}
// -------------------------------------------------------------------------


ClassImp(CbmMvdClusterfinder);
