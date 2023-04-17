/* Copyright (C) 2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer] */

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
#include "CbmDefs.h"
#include "CbmMCDataManager.h"
#include "CbmMuchTransportQa.h"
#include "CbmSetup.h"

#include <FairFileSource.h>
#include <FairMonitor.h>
#include <FairParAsciiFileIo.h>
#include <FairParRootFileIo.h>
#include <FairRootFileSink.h>
#include <FairRunAna.h>
#include <FairRuntimeDb.h>
#include <FairSystemInfo.h>

#include <TStopwatch.h>
#endif

void mcbm_qa(Int_t nEvents = 0, TString dataset = "data/mcbm_beam_2020_03_test",
             TString setupName = "mcbm_beam_2020_03", Bool_t bUseMC = kTRUE)
{

  // ========================================================================
  //          Adjust this part according to your requirements

  // -----   Logger settings   ----------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  // ------------------------------------------------------------------------

  // -----   Environment   --------------------------------------------------
  int verbose    = 6;                              // verbose level
  TString myName = "mcbm_qa";                      // this macro's name for screen output
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
  std::cout << '\n';
  TString setupFile  = srcDir + "/geometry/setup/setup_" + setupName + ".C";
  TString setupFunct = "setup_";
  setupFunct         = setupFunct + setupName + "()";
  std::cout << "-I- " << myName << ": Loading macro " << setupFile << '\n';
  gROOT->LoadMacro(setupFile);
  gROOT->ProcessLine(setupFunct);
  CbmSetup* setup = CbmSetup::Instance();
  //  setup->RemoveModule(ECbmModuleId::kTrd);
  // ------------------------------------------------------------------------

  // -----   Some global switches   -----------------------------------------
  if (setupName == "mcbm_beam_2022_05_23_nickel") { setup->RemoveModule(ECbmModuleId::kMuch); }

  //bool eventBased = !sEvBuildRaw.IsNull();
  bool bUseMvd  = setup->IsActive(ECbmModuleId::kMvd);
  bool bUseSts  = setup->IsActive(ECbmModuleId::kSts);
  bool bUseRich = setup->IsActive(ECbmModuleId::kRich);
  bool bUseMuch = setup->IsActive(ECbmModuleId::kMuch);
  bool bUseTrd  = setup->IsActive(ECbmModuleId::kTrd);
  bool bUseTof  = setup->IsActive(ECbmModuleId::kTof);
  bool bUsePsd  = setup->IsActive(ECbmModuleId::kPsd);
  std::cout << "  MVD: " << (bUseMvd ? "ON" : "OFF") << '\n';
  std::cout << "  STS: " << (bUseSts ? "ON" : "OFF") << '\n';
  std::cout << "  RICH: " << (bUseRich ? "ON" : "OFF") << '\n';
  std::cout << "  MUCH: " << (bUseMuch ? "ON" : "OFF") << '\n';
  std::cout << "  TRD: " << (bUseTrd ? "ON" : "OFF") << '\n';
  std::cout << "  TOF: " << (bUseTof ? "ON" : "OFF") << '\n';
  std::cout << "  PSD: " << (bUsePsd ? "ON" : "OFF") << '\n';
  // ------------------------------------------------------------------------

  // -----   Parameter files as input to the runtime database   -------------
  std::cout << '\n';
  std::cout << "-I- " << myName << ": Defining paramete files\n";
  TList* parFileList = new TList();
  TString geoTag;

  // - MUCH digitisation parameters
  TString muchParFile {};
  if (setup->GetGeoTag(ECbmModuleId::kMuch, geoTag)) {
    bool mcbmFlag   = geoTag.Contains("mcbm", TString::kIgnoreCase);
    muchParFile     = srcDir + "/parameters/much/much_";
    muchParFile += (mcbmFlag) ? geoTag : geoTag(0, 4);
    muchParFile += "_digi_sector.root";
    {  // init geometry from the file
      TFile* f            = new TFile(muchParFile, "R");
      TObjArray* stations = f->Get<TObjArray>("stations");
      assert(stations);
      CbmMuchGeoScheme::Instance()->Init(stations, mcbmFlag);
    }
  }

  // - TRD digitisation parameters
  if (setup->GetGeoTag(ECbmModuleId::kTrd, geoTag)) {
    const Char_t* npar[4] = {"asic", "digi", "gas", "gain"};
    TObjString* trdParFile(NULL);
    for (Int_t i(0); i < 4; i++) {
      trdParFile = new TObjString(srcDir + "/parameters/trd/trd_" + geoTag + "." + npar[i] + ".par");
      parFileList->Add(trdParFile);
      std::cout << "-I- " << myName << ": Using parameter file " << trdParFile->GetString() << std::endl;
    }
  }

  // - TOF digitisation parameters
  if (setup->GetGeoTag(ECbmModuleId::kTof, geoTag)) {
    TObjString* tofBdfFile = new TObjString(srcDir + "/parameters/tof/tof_" + geoTag + ".digibdf.par");
    parFileList->Add(tofBdfFile);
    std::cout << "-I- " << myName << ": Using parameter file " << tofBdfFile->GetString() << std::endl;
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
  if (bUseMC) {
    std::cout << "-I- " << myName << ": Adding MC manager and MC to reco matching tasks\n";

    auto* mcManager = new CbmMCDataManager("MCDataManager", 1);
    mcManager->AddFile(traFile);
    run->AddTask(mcManager);

    auto* matchRecoToMC = new CbmMatchRecoToMC();
    run->AddTask(matchRecoToMC);
  }
  // ------------------------------------------------------------------------

  // ----- MUCH QA  ---------------------------------
  if (setup->IsActive(ECbmModuleId::kMuch)) {
    run->AddTask(new CbmMuchTransportQa());
    run->AddTask(new CbmMuchDigitizerQa());
    CbmMuchHitFinderQa* muchHitFinderQa = new CbmMuchHitFinderQa();
    muchHitFinderQa->SetGeoFileName(muchParFile);
    run->AddTask(muchHitFinderQa);
  }
  // ------------------------------------------------------------------------


  // ----- CA tracking QA ---------------------------------------------------
  // Tracking detector interface initialization
  run->AddTask(new CbmTrackingDetectorInterfaceInit());

  // Kalman Filter (currently needed to access the magnetic filed, to be
  // removed soon)
  run->AddTask(new CbmKF());

  // Tracking parameters file is required
  TString caParFile = recFile;
  caParFile.ReplaceAll(".root", ".L1Parameters.dat");

  auto* pCaOutputQa = new cbm::ca::OutputQa(verbose, bUseMC);
  pCaOutputQa->SetMcbmTrackingMode();
  pCaOutputQa->ReadParameters(caParFile.Data());
  pCaOutputQa->SetUseSts(bUseSts);
  //pCaOutputQa->SetUseMuch(bUseMuch);
  pCaOutputQa->SetUseTrd(bUseTrd);
  pCaOutputQa->SetUseTof(bUseTof);
  run->AddTask(pCaOutputQa);

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
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
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
