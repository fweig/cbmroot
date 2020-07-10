// -------------------------------------------------------------------------
// -----                    CbmMvdClusterfinderTB source file                -----
// -------------------------------------------------------------------------

// Includes from MVD
#include "CbmMvdClusterfinderTB.h"
#include "CbmDigiManager.h"
#include "CbmEvent.h"
#include "CbmMvdDetector.h"
#include "CbmMvdPoint.h"
#include "plugins/tasks/CbmMvdSensorClusterfinderTask.h"
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
CbmMvdClusterfinderTB::CbmMvdClusterfinderTB()
  : CbmMvdClusterfinderTB("MVDCLusterfinderTB", 0, 0) {}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
CbmMvdClusterfinderTB::CbmMvdClusterfinderTB(const char* name,
                                             Int_t iMode,
                                             Int_t iVerbose)
  : FairTask(name, iVerbose)
  , fMode(iMode)
  , fShowDebugHistos(kFALSE)
  , fDetector(NULL)
  , fEvents(NULL)
  , fDigiMan(nullptr)
  , fEventDigis(NULL)
  , fCluster(NULL)
  , fClusterPluginNr(0)
  , fBranchName("MvdDigi")
  , fTimer() {}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmMvdClusterfinderTB::~CbmMvdClusterfinderTB() {

  if (fCluster) {
    fCluster->Delete();
    delete fCluster;
  }
}
// -----------------------------------------------------------------------------

// -----   Exec   --------------------------------------------------------------
void CbmMvdClusterfinderTB::Exec(Option_t* /*opt*/) {
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
      Int_t iDigi      = event->GetIndex(ECbmDataType::kMvdDigi, nDigi);
      CbmMvdDigi* digi = new CbmMvdDigi(*(fDigiMan->Get<CbmMvdDigi>(iDigi)));
      fEventDigis->AddLast(digi);
    }
    LOG(debug) << "//----------------------------------------//";
    LOG(debug) << endl << "Send Input";
    fDetector->SendInputDigis(fEventDigis);
    LOG(debug) << "Execute ClusterPlugin Nr. " << fClusterPluginNr;
    fDetector->Exec(fClusterPluginNr);
    LOG(debug) << "End Chain";
    LOG(debug) << "Start writing Cluster";
    fCluster->AbsorbObjects(fDetector->GetOutputCluster(),
                            0,
                            fDetector->GetOutputCluster()->GetEntriesFast()
                              - 1);
    LOG(debug) << "Total of " << fCluster->GetEntriesFast()
               << " Cluster in this Event";
    LOG(debug) << "//----------------------------------------//";
    LOG(info) << "+ " << setw(20) << GetName()
              << ": Created: " << fCluster->GetEntriesFast() << " cluster in "
              << fixed << setprecision(6) << fTimer.RealTime() << " s";
  }
  fTimer.Stop();
}
// -----------------------------------------------------------------------------

// -----   Init   --------------------------------------------------------------
InitStatus CbmMvdClusterfinderTB::Init() {
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
  fEvents = (TClonesArray*) ioman->GetObject("Event");

  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->Init();

  fEventDigis = new TClonesArray("CbmMvdDigi", 10000);
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
    LOG(debug) << "-I- succesfully loaded Geometry from file -I-";
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
  LOG(info) << GetName() << " initialised";

  return kSUCCESS;
}

// -----   Virtual public method Reinit   ----------------------------------
InitStatus CbmMvdClusterfinderTB::ReInit() { return kSUCCESS; }
// -------------------------------------------------------------------------


// -----   Virtual method Finish   -----------------------------------------
void CbmMvdClusterfinderTB::Finish() {
  fDetector->Finish();
  PrintParameters();
}
// -------------------------------------------------------------------------


// -----   Private method Reset   ------------------------------------------
void CbmMvdClusterfinderTB::Reset() { fCluster->Delete(); }
// -------------------------------------------------------------------------

// -----   Private method GetMvdGeometry   ---------------------------------
void CbmMvdClusterfinderTB::GetMvdGeometry() {}
// -------------------------------------------------------------------------


// -----   Private method PrintParameters   --------------------------------
void CbmMvdClusterfinderTB::PrintParameters() {

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


ClassImp(CbmMvdClusterfinderTB);
