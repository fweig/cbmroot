// --------------------------------------------------------------------------
//
// Macro for reconstruction of simulated time slices
// Using time-based reconstruction in the STS
//
// STS cluster finder (time-based)
// STS hit finder (time-based)
// STS track finder (time-based)
// Event building from STS tracks
// Simple QA
//
// V. Friese   14/03/2017
//
// --------------------------------------------------------------------------

// Includes needed for IDE
#if !defined(__CLING__)
#include "CbmDefs.h"
#include "CbmMuchFindHitsGem.h"
#include "CbmRecoSts.h"
#include "CbmRichHitProducer.h"
#include "CbmSetup.h"
#include "CbmTofSimpClusterizer.h"
#include "CbmTrdClusterFinder.h"
#include "CbmTrdHitProducer.h"
#include <FairLogger.h>
#include <FairMonitor.h>
#include <FairParAsciiFileIo.h>
#include <FairParRootFileIo.h>
#include <FairRunAna.h>
#include <FairRuntimeDb.h>
#include <TGeoManager.h>
#include <TStopwatch.h>
#include <TString.h>

#endif


using std::cout;
using std::endl;

void run_reco_tb_track(TString dataSet = "test",
                       Int_t nSlices   = -1,
                       TString setup   = "sis100_electron") {

  // =========================================================================
  // ===                      Settings                                     ===
  // =========================================================================


  // --- File names
  TString outDir  = "data/";
  TString inFile  = dataSet + ".raw.root";     // Input file (digis)
  TString parFile = dataSet + ".par.root";     // Parameter file
  TString outFile = dataSet + ".tb.rec.root";  // Output file

  // Log level
  TString logLevel     = "INFO";  // switch to DEBUG or DEBUG1,... for more info
  TString logVerbosity = "LOW";   // switch to MEDIUM or HIGH for more info


  // ----    Debug option   -------------------------------------------------
  gDebug = 0;

  // ========================================================================


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // -----   Reconstruction run   -------------------------------------------
  FairRunAna* run             = new FairRunAna();
  FairFileSource* inputSource = new FairFileSource(inFile);
  run->SetSource(inputSource);
  run->SetOutputFile(outFile);
  run->SetGenerateRunInfo(kTRUE);

  TString monitorFile {outFile};
  monitorFile.ReplaceAll("rec", "rec.monitor");
  FairMonitor::GetMonitor()->EnableMonitor(kTRUE, monitorFile);
  // ------------------------------------------------------------------------


  // ---- Set the log level   -----------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel(logLevel.Data());
  FairLogger::GetLogger()->SetLogVerbosityLevel(logVerbosity.Data());
  // ------------------------------------------------------------------------

  // -----   Environment   --------------------------------------------------
  TString myName = "run_reco_tb_track";  // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------

  // -----   Load the geometry setup   -------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Loading setup " << setup << std::endl;
  CbmSetup::Instance()->LoadSetup(setup);
  // You can modify the pre-defined setup by using
  // CbmSetup::Instance()->RemoveModule(ESystemId) or
  // CbmSetup::Instance()->SetModule(ESystemId, const char*, Bool_t) or
  // CbmSetup::Instance()->SetActive(ESystemId, Bool_t)
  // See the class documentation of CbmSetup.
  // ------------------------------------------------------------------------

  // =========================================================================
  // ===                      Reconstruction chain                         ===
  // =========================================================================


  if (CbmSetup::Instance()->IsActive(ECbmModuleId::kSts)) {
    // --- STS local reconstruction
    run->AddTask(new CbmRecoSts());
  }

  if (CbmSetup::Instance()->IsActive(ECbmModuleId::kMuch)) {

    // --- Parameter file name
    TString geoTag;
    CbmSetup::Instance()->GetGeoTag(ECbmModuleId::kMuch, geoTag);
    Int_t muchFlag = 0;
    if (geoTag.Contains("mcbm")) muchFlag = 1;

    std::cout << geoTag(0, 4) << std::endl;
    TString parFile = gSystem->Getenv("VMCWORKDIR");
    parFile =
      parFile + "/parameters/much/much_" + geoTag(0, 4) + "_digi_sector.root";
    std::cout << "Using parameter file " << parFile << std::endl;

    // --- Hit finder for GEMs
    FairTask* muchHitGem = new CbmMuchFindHitsGem(parFile.Data(), muchFlag);
    run->AddTask(muchHitGem);
  }

  if (CbmSetup::Instance()->IsActive(ECbmModuleId::kRich)) {
    // -----   RICH reconstruction   ------------------------------------------
    CbmRichHitProducer* richHitProd = new CbmRichHitProducer();
    run->AddTask(richHitProd);
    std::cout << "-I- : Added task " << richHitProd->GetName() << std::endl;
  }

  if (CbmSetup::Instance()->IsActive(ECbmModuleId::kTrd)) {
    // ---- Local reco in TRD  -----------------------------
    Double_t triggerThreshold       = 0.5e-6;  // SIS100
    CbmTrdClusterFinder* trdCluster = new CbmTrdClusterFinder();
    trdCluster->SetNeighbourEnable(true, false);
    trdCluster->SetMinimumChargeTH(triggerThreshold);
    trdCluster->SetRowMerger(true);
    run->AddTask(trdCluster);
    std::cout << "-I- : Added task " << trdCluster->GetName() << std::endl;
    CbmTrdHitProducer* trdHit = new CbmTrdHitProducer();
    run->AddTask(trdHit);
    std::cout << "-I- : Added task " << trdHit->GetName() << std::endl;
  }

  if (CbmSetup::Instance()->IsActive(ECbmModuleId::kTof)) {
    // ---- Local reconstruction in TOF   ----------------------------------------
    CbmTofSimpClusterizer* tofCluster =
      new CbmTofSimpClusterizer("TOF Simple Clusterizer", 0);
    tofCluster->SetOutputBranchPersistent("TofHit", kTRUE);
    tofCluster->SetOutputBranchPersistent("TofDigiMatch", kTRUE);
    run->AddTask(tofCluster);
    std::cout << "-I- : Added task " << tofCluster->GetName() << std::endl;
  }

  //    // -----   Local reconstruction in PSD reconstruction   --------------------
  //    CbmPsdHitProducer* psdHit = new CbmPsdHitProducer();
  //    run->AddTask(ECbmModuleId::psdHit);
  //    std::cout << "-I- : Added task CbmPsdHitProducer" << std::endl;

  // --- STS track finder
  run->AddTask(new CbmKF());
  CbmL1* l1 = new CbmL1();
  l1->SetDataMode(1);
  run->AddTask(l1);
  CbmStsTrackFinder* stsTrackFinder = new CbmL1StsTrackFinder();
  FairTask* stsFindTracks           = new CbmStsFindTracks(0, stsTrackFinder);
  run->AddTask(stsFindTracks);
  //
  //  // --- Event builder (track-based)
  //  run->AddTask(new CbmBuildEventsFromTracksReal());
  //
  //  // --- Simple QA task to check basics
  //  run->AddTask(new CbmStsRecoQa());

  // -----  Parameter database   --------------------------------------------
  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  FairParRootFileIo* parIo1  = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.Data(), "UPDATE");
  rtdb->setFirstInput(parIo1);
  // ------------------------------------------------------------------------

  // -----   Initialise and run   --------------------------------------------
  run->Init();

  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  rtdb->print();

  cout << "Starting run " << gGeoManager << endl;
  if (nSlices < 0)
    run->Run();
  else
    run->Run(nSlices);
  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  FairMonitor::GetMonitor()->Print();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished successfully." << endl;
  cout << "Output file is " << outFile << endl;
  cout << "Parameter file is " << parFile << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
  cout << endl;
  // ------------------------------------------------------------------------


  cout << " Test passed" << endl;
  cout << " All ok " << endl;
  RemoveGeoManager();
}
