// --------------------------------------------------------------------------
//
// Macro for simulation & reconstruction QA
//
// The following naming conventions are assumed:
// Raw data file:  [dataset].event.raw.root
// Transport file: [dataset].tra.root
// Parameter file:  [dataset].par.root
// Reconstruction file: [dataset].rec.root
//
// S. Gorbunov 28/09/2020
//
// --------------------------------------------------------------------------

// Includes needed for IDE
#if !defined(__CLING__)
#include <FairFileSource.h>
#include <FairMonitor.h>
#include <FairParAsciiFileIo.h>
#include <FairParRootFileIo.h>
#include <FairRootFileSink.h>
#include <FairRunAna.h>
#include <FairRuntimeDb.h>
#include <FairSystemInfo.h>

#include "CbmDefs.h"
#include "CbmMCDataManager.h"
#include "CbmMuchDigitizerQa.h"
#include "CbmMuchHitFinderQa.h"
#include "CbmMuchTransportQa.h"
#include "CbmSetup.h"

#include <TStopwatch.h>
#endif

void run_qa(Int_t nEvents   = 0,
            TString dataset = "data/sis100_muon_jpsi_test",
            TString setup   = "sis100_muon_jpsi") {

  // ========================================================================
  //          Adjust this part according to your requirements

  // -----   Logger settings   ----------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  // ------------------------------------------------------------------------

  // -----   Environment   --------------------------------------------------
  TString myName = "run_qa";  // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------

  // -----   In- and output file names   ------------------------------------
  TString rawFile  = dataset + ".event.raw.root";
  TString traFile  = dataset + ".tra.root";
  TString parFile  = dataset + ".par.root";
  TString recFile  = dataset + ".rec.root";
  TString sinkFile = dataset + ".qa.root";
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

  // -----   Parameter files as input to the runtime database   -------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Defining parameter files " << std::endl;
  TList* parFileList = new TList();
  TString geoTag;

  // - MUCH digitisation parameters
  if (CbmSetup::Instance()->GetGeoTag(ECbmModuleId::kMuch, geoTag)) {
    bool mcbmFlag   = geoTag.Contains("mcbm", TString::kIgnoreCase);
    TString parFile = srcDir + "/parameters/much/much_";
    parFile += (mcbmFlag) ? geoTag : geoTag(0, 4);
    parFile += "_digi_sector.root";
    {  // init geometry from the file
      TFile* f            = new TFile(parFile, "R");
      TObjArray* stations = (TObjArray*) f->Get("stations");
      CbmMuchGeoScheme::Instance()->Init(stations, mcbmFlag);
    }
  }

  // - TRD digitisation parameters
  if (CbmSetup::Instance()->GetGeoTag(ECbmModuleId::kTrd, geoTag)) {
    const Char_t* npar[4] = {"asic", "digi", "gas", "gain"};
    TObjString* trdParFile(NULL);
    for (Int_t i(0); i < 4; i++) {
      trdParFile = new TObjString(srcDir + "/parameters/trd/trd_" + geoTag + "."
                                  + npar[i] + ".par");
      parFileList->Add(trdParFile);
      std::cout << "-I- " << myName << ": Using parameter file "
                << trdParFile->GetString() << std::endl;
    }
  }

  // - TOF digitisation parameters
  if (CbmSetup::Instance()->GetGeoTag(ECbmModuleId::kTof, geoTag)) {
    TObjString* tofBdfFile =
      new TObjString(srcDir + "/parameters/tof/tof_" + geoTag + ".digibdf.par");
    parFileList->Add(tofBdfFile);
    std::cout << "-I- " << myName << ": Using parameter file "
              << tofBdfFile->GetString() << std::endl;
  }
  // ------------------------------------------------------------------------

  // In general, the following parts need not be touched
  // ========================================================================

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------

  // -----   FairRunAna   ---------------------------------------------------
  FairFileSource* inputSource = new FairFileSource(rawFile);
  inputSource->AddFriend(traFile);
  inputSource->AddFriend(recFile);

  FairRunAna* run = new FairRunAna();
  run->SetSource(inputSource);
  run->SetGenerateRunInfo(kFALSE);

  FairRootFileSink* sink = new FairRootFileSink(sinkFile);
  run->SetSink(sink);

  TString monitorFile {sinkFile};
  monitorFile.ReplaceAll("qa", "qa.monitor");
  FairMonitor::GetMonitor()->EnableMonitor(kTRUE, monitorFile);
  // ------------------------------------------------------------------------

  // -----   MCDataManager (legacy mode)  -----------------------------------
  CbmMCDataManager* mcManager = new CbmMCDataManager("MCDataManager", 1);
  mcManager->AddFile(traFile);
  run->AddTask(mcManager);
  // ------------------------------------------------------------------------

  // ----- MUCH QA  ---------------------------------
  if (CbmSetup::Instance()->IsActive(ECbmModuleId::kMuch)) {
    run->AddTask(new CbmMuchTransportQa());
    run->AddTask(new CbmMuchDigitizerQa());
    run->AddTask(new CbmMuchHitFinderQa());
  }
  // ------------------------------------------------------------------------

  // -----  Parameter database   --------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Set runtime DB" << std::endl;
  FairRuntimeDb* rtdb       = run->GetRuntimeDb();
  FairParRootFileIo* parIo1 = new FairParRootFileIo();
  parIo1->open(parFile.Data(), "in");
  rtdb->setFirstInput(parIo1);
  if (!parFileList->IsEmpty()) {
    FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
    parIo2->open(parFileList, "in");
    rtdb->setSecondInput(parIo2);
  }
  rtdb->print();
  // ------------------------------------------------------------------------

  // -----   Run initialisation   -------------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Initialise run" << std::endl;
  run->Init();

  // -----   Start run   ----------------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Starting run" << std::endl;
  run->Run(0, nEvents);
  // ------------------------------------------------------------------------

  // -----   Finish   -------------------------------------------------------
  timer.Stop();

  FairMonitor::GetMonitor()->Print();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Output file is " << sinkFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << " s"
            << std::endl;
  std::cout << std::endl;
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
  // ------------------------------------------------------------------------

  // -----   Resource monitoring   ------------------------------------------
  // Extract the maximal used memory an add is as Dart measurement
  // This line is filtered by CTest and the value send to CDash
  FairSystemInfo sysInfo;
  Float_t maxMemory = sysInfo.GetMaxMemory();
  std::cout << "<DartMeasurement name=\"MaxMemory\" type=\"numeric/double\">";
  std::cout << maxMemory;
  std::cout << "</DartMeasurement>" << std::endl;

  Float_t cpuUsage = ctime / rtime;
  std::cout << "<DartMeasurement name=\"CpuLoad\" type=\"numeric/double\">";
  std::cout << cpuUsage;
  std::cout << "</DartMeasurement>" << std::endl;
  // ------------------------------------------------------------------------

  // -----   Function needed for CTest runtime dependency   -----------------
  RemoveGeoManager();
  // ------------------------------------------------------------------------
}
