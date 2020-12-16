#// --------------------------------------------------------------------------
//
// Macro for reconstruction of simulated raw data
//
// Default event-by-event reconstruction chain,
// defined in macro/run/modules/reconstruct.C
//
// The following naming conventions are assumed:
// Raw data file:  [dataset].event.raw.root
// Transport file: [dataset].tra.root
// Parameter file: [dataset].par.root
// Output file:    [dataset].rec.root
//
// V. Friese   10/06/2018
//
// --------------------------------------------------------------------------

// Includes needed for IDE
#if !defined(__CLING__)
#include "CbmDefs.h"
#include "CbmMCDataManager.h"
#include "CbmSetup.h"
#include "FairSystemInfo.h"
#include <FairFileSource.h>
#include <FairMonitor.h>
#include <FairParAsciiFileIo.h>
#include <FairParRootFileIo.h>
#include <FairRunAna.h>
#include <FairRuntimeDb.h>
#include <TStopwatch.h>
#endif


void run_reco_event(Int_t nEvents   = 2,
                    TString dataset = "test",
                    TString setup   = "sis100_electron",
                    Bool_t useMC    = kFALSE,
                    Bool_t findPV   = kTRUE) {

  // ========================================================================
  //          Adjust this part according to your requirements

  // --- Logger settings ----------------------------------------------------
  TString logLevel     = "INFO";
  TString logVerbosity = "LOW";
  // ------------------------------------------------------------------------


  // -----   Environment   --------------------------------------------------
  TString myName = "run_reco_event";  // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------


  // -----   In- and output file names   ------------------------------------
  TString rawFile = dataset + ".event.raw.root";
  TString traFile = dataset + ".tra.root";
  TString parFile = dataset + ".par.root";
  TString outFile = dataset + ".rec.root";
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
  FairRunAna* run             = new FairRunAna();
  FairFileSource* inputSource = new FairFileSource(rawFile);
  if (useMC) inputSource->AddFriend(traFile);
  run->SetSource(inputSource);
  run->SetOutputFile(outFile);
  run->SetGenerateRunInfo(kTRUE);

  TString monitorFile {outFile};
  monitorFile.ReplaceAll("rec", "rec.monitor");
  FairMonitor::GetMonitor()->EnableMonitor(kTRUE, monitorFile);
  // ------------------------------------------------------------------------


  // -----   MCDataManager (legacy mode)  -----------------------------------
  if (useMC) {
    CbmMCDataManager* mcManager = new CbmMCDataManager("MCDataManager", 1);
    mcManager->AddFile(traFile);
    run->AddTask(mcManager);
  }
  // ------------------------------------------------------------------------


  // -----   Logger settings   ----------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel(logLevel.Data());
  FairLogger::GetLogger()->SetLogVerbosityLevel(logVerbosity.Data());
  // ------------------------------------------------------------------------


  // -----   Reconstruction tasks   -----------------------------------------
  std::cout << std::endl;
  TString macroName = srcDir + "/macro/run/modules/reconstruct.C";
  std::cout << "Loading macro " << macroName << std::endl;
  gROOT->LoadMacro(macroName);
  TString command = "reconstruct(";
  command += (useMC ? "kTRUE," : "kFALSE,");
  command += (findPV ? "kTRUE)" : "kFALSE)");
  std::cout << "Calling " << command << std::endl;
  Bool_t recoSuccess = gROOT->ProcessLine(command.Data());
  if (!recoSuccess) {
    std::cerr << "-E- " << myName << ": error in executing " << macroName
              << std::endl;
    return;
  }
  std::cout << "-I- " << myName << ": " << macroName << " excuted successfully"
            << std::endl;
  // ------------------------------------------------------------------------


  // -----  Parameter database   --------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Set runtime DB" << std::endl;
  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  FairParRootFileIo* parIo1  = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.Data(), "UPDATE");
  rtdb->setFirstInput(parIo1);
  if (!parFileList->IsEmpty()) {
    parIo2->open(parFileList, "in");
    rtdb->setSecondInput(parIo2);
  }
  // ------------------------------------------------------------------------


  // -----   Run initialisation   -------------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Initialise run" << std::endl;
  run->Init();
  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  rtdb->print();
  // ------------------------------------------------------------------------


  // -----   Register light ions (d, t, He3, He4)   -------------------------
  std::cout << std::endl;
  TString registerLightIonsMacro = gSystem->Getenv("VMCWORKDIR");
  registerLightIonsMacro += "/macro/KF/registerLightIons.C";
  std::cout << "Loading macro " << registerLightIonsMacro << std::endl;
  gROOT->LoadMacro(registerLightIonsMacro);
  gROOT->ProcessLine("registerLightIons()");
  // ------------------------------------------------------------------------


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
  std::cout << "Output file is " << outFile << std::endl;
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
