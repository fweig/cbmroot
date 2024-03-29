/* Copyright (C) 2006-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Denis Bertini [committer], Florian Uhlig */

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
#include "CbmMuchDigitizerQa.h"
#include "CbmMuchHitFinderQa.h"
#include "CbmMuchTransportQa.h"
#include "CbmSetup.h"
#include "CbmStsFindTracksQa.h"

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

void run_qa(TString dataTra = "data/sis100_muon_jpsi_test", TString dataRaw = "data/sis100_muon_jpsi_test",
            TString dataReco = "data/sis100_muon_jpsi_test", TString dataPar = "data/sis100_muon_jpsi_test",
            TString dataSink = "data/sis100_muon_jpsi_test", TString setup = "sis100_muon_jpsi", Int_t nEvents = -1,
            TString dataTra2 = "", TString dataTra3 = "", const char* configName = "./qa_config.fcbm.yaml")
{

  gROOT->SetBatch(kTRUE);

  // ========================================================================
  //          Adjust this part according to your requirements

  // -----   Logger settings   ----------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  fair::Logger::DefineVerbosity(
    "user1", fair::VerbositySpec::Make(fair::VerbositySpec::Info::severity, fair::VerbositySpec::Info::file_line));
  FairLogger::GetLogger()->SetLogVerbosityLevel("user1");
  FairLogger::GetLogger()->SetColoredLog(true);
  // ------------------------------------------------------------------------

  // -----   Environment   --------------------------------------------------
  bool bUseMC    = true;                           // MC flag: used or not
  int verbose    = 3;                              // verbose level
  TString myName = "run_qa";                       // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------

  // -----   In- and output file names   ------------------------------------
  TString traFile  = dataTra + ".tra.root";
  TString tra2File = dataTra2 + ".tra.root";
  TString tra3File = dataTra3 + ".tra.root";
  TString rawFile  = dataRaw + ".raw.root";
  TString parFile  = dataPar + ".par.root";
  TString recFile  = dataReco + ".reco.root";
  TString sinkFile = dataSink + ".qa.root";
  // ------------------------------------------------------------------------

  // -----   Load the geometry setup   -------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Loading setup " << setup << std::endl;
  auto* geo = CbmSetup::Instance();
  geo->LoadSetup(setup);

  // You can modify the pre-defined setup by using
  // CbmSetup::Instance()->RemoveModule(ESystemId) or
  // CbmSetup::Instance()->SetModule(ESystemId, const char*, Bool_t) or
  // CbmSetup::Instance()->SetActive(ESystemId, Bool_t)
  // See the class documentation of CbmSetup.
  // ------------------------------------------------------------------------

  // -----   Some global switches   -----------------------------------------
  //Bool_t eventBased = !sEvBuildRaw.IsNull();
  bool bUseMvd  = geo->IsActive(ECbmModuleId::kMvd);
  bool bUseSts  = geo->IsActive(ECbmModuleId::kSts);
  bool bUseRich = geo->IsActive(ECbmModuleId::kRich);
  bool bUseMuch = geo->IsActive(ECbmModuleId::kMuch);
  bool bUseTrd  = geo->IsActive(ECbmModuleId::kTrd);
  bool bUseTof  = geo->IsActive(ECbmModuleId::kTof);
  bool bUsePsd  = geo->IsActive(ECbmModuleId::kPsd);
  std::cout << "  MVD: " << (bUseMvd ? "ON" : "OFF") << '\n';
  std::cout << "  STS: " << (bUseSts ? "ON" : "OFF") << '\n';
  std::cout << "  RICH: " << (bUseRich ? "ON" : "OFF") << '\n';
  std::cout << "  MUCH: " << (bUseMuch ? "ON" : "OFF") << '\n';
  std::cout << "  TRD: " << (bUseTrd ? "ON" : "OFF") << '\n';
  std::cout << "  TOF: " << (bUseTof ? "ON" : "OFF") << '\n';
  std::cout << "  PSD: " << (bUsePsd ? "ON" : "OFF") << '\n';
  // ------------------------------------------------------------------------

  // -----   Parameter files as input to the runtime database   -------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Defining parameter files " << std::endl;
  TList* parFileList = new TList();
  TString geoTag;

  // - MUCH digitisation parameters
  TString muchParFile {};
  if (CbmSetup::Instance()->GetGeoTag(ECbmModuleId::kMuch, geoTag)) {
    bool mcbmFlag = geoTag.Contains("mcbm", TString::kIgnoreCase);
    muchParFile   = srcDir + "/parameters/much/much_";
    muchParFile += (mcbmFlag) ? geoTag : geoTag(0, 4);
    muchParFile += "_digi_sector.root";
    {  // init geometry from the file
      TFile* f            = new TFile(muchParFile, "R");
      TObjArray* stations = (TObjArray*) f->Get("stations");
      CbmMuchGeoScheme::Instance()->Init(stations, mcbmFlag);
    }
  }

  // - TRD digitisation parameters
  if (CbmSetup::Instance()->GetGeoTag(ECbmModuleId::kTrd, geoTag)) {
    const Char_t* npar[4] = {"asic", "digi", "gas", "gain"};
    TObjString* trdParFile(NULL);
    for (Int_t i(0); i < 4; i++) {
      trdParFile = new TObjString(srcDir + "/parameters/trd/trd_" + geoTag + "." + npar[i] + ".par");
      parFileList->Add(trdParFile);
      std::cout << "-I- " << myName << ": Using parameter file " << trdParFile->GetString() << std::endl;
    }
  }

  // - TOF digitisation parameters
  if (CbmSetup::Instance()->GetGeoTag(ECbmModuleId::kTof, geoTag)) {
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
  if (!dataTra2.IsNull()) inputSource->AddFriend(tra2File);
  if (!dataTra3.IsNull()) inputSource->AddFriend(tra3File);

  FairRunAna* run = new FairRunAna();
  run->SetSource(inputSource);
  run->SetGenerateRunInfo(kFALSE);

  FairRootFileSink* sink = new FairRootFileSink(sinkFile);
  run->SetSink(sink);

  TString monitorFile {sinkFile};
  monitorFile.ReplaceAll("qa", "qa.monitor");
  FairMonitor::GetMonitor()->EnableMonitor(kTRUE, monitorFile);
  // ------------------------------------------------------------------------

  // -----   MCDataManager  -----------------------------------
  CbmMCDataManager* mcManager = new CbmMCDataManager("MCDataManager", 0);
  mcManager->AddFile(traFile);
  if (!dataTra2.IsNull()) mcManager->AddFile(tra2File);
  if (!dataTra3.IsNull()) mcManager->AddFile(tra3File);

  run->AddTask(mcManager);
  // ------------------------------------------------------------------------

  run->AddTask(new CbmTrackingDetectorInterfaceInit());  // Geometry interface initializer for tracker

  // ----- Match reco to MC ------
  CbmMatchRecoToMC* matchTask = new CbmMatchRecoToMC();
  run->AddTask(matchTask);

  // ----- MUCH QA  ---------------------------------
  if (CbmSetup::Instance()->IsActive(ECbmModuleId::kMuch)) {
    run->AddTask(new CbmMuchTransportQa());
    run->AddTask(new CbmMuchDigitizerQa());
    CbmMuchHitFinderQa* muchHitFinderQa = new CbmMuchHitFinderQa();
    muchHitFinderQa->SetGeoFileName(muchParFile);
    run->AddTask(muchHitFinderQa);

    auto* pCaInputQaMuch = new CbmCaInputQaMuch(verbose, bUseMC);
    pCaInputQaMuch->SetEfficiencyThrsh(0.5, 0, 100);
    run->AddTask(pCaInputQaMuch);
  }

  // ----- TRD QA  ---------------------------------
  if (CbmSetup::Instance()->IsActive(ECbmModuleId::kTrd)) {
    run->AddTask(new CbmTrdMCQa());
    //run->AddTask(new CbmTrdHitRateQa());  //opens lots of windows
    //run->AddTask(new CbmTrdDigitizerPRFQa()); //works put currently doesn't do anything
    //run->AddTask(new CbmTrdHitRateFastQa());  //opens lots of windows
    CbmTrdHitProducerQa* trdHitProducerQa = new CbmTrdHitProducerQa();
    if ("sis300_electron" == setup) {
      /// Larger number of stations, needed to fit geometry and avoid warning
      /// => fast-fix to accomodate the 4+4+2 complexity of tentative SIS300 geometry
      trdHitProducerQa->SetNumberStations(10);
    }
    run->AddTask(trdHitProducerQa);
    run->AddTask(new CbmTrdCalibTracker());
    auto* pCaInputQaTrd = new CbmCaInputQaTrd(verbose, bUseMC);
    pCaInputQaTrd->SetEfficiencyThrsh(0.5, 0, 100);
    run->AddTask(pCaInputQaTrd);
  }
  // ------------------------------------------------------------------------

  // ----- TOF QA  ---------------------------------
  if (CbmSetup::Instance()->IsActive(ECbmModuleId::kTof)) {
    auto* pCaInputQaTof = new CbmCaInputQaTof(verbose, bUseMC);
    pCaInputQaTof->SetEfficiencyThrsh(0.5, 0, 100);
    run->AddTask(pCaInputQaTof);
  }
  // ------------------------------------------------------------------------

  // ----- STS QA  ---------------------------------
  if (CbmSetup::Instance()->IsActive(ECbmModuleId::kSts)) {
    //run->AddTask(new CbmStsDigitizeQa()); //opens lots of windows
    run->AddTask(new CbmStsFindTracksQa());
    auto* pCaInputQaSts = new CbmCaInputQaSts(verbose, bUseMC);
    pCaInputQaSts->SetEfficiencyThrsh(0.5, 0, 100);
    run->AddTask(pCaInputQaSts);
  }
  // ------------------------------------------------------------------------

  // ----- Event builder QA  ---------------------------------
  CbmBuildEventsQa* evBuildQA = new CbmBuildEventsQa();
  run->AddTask(evBuildQA);
  // ------------------------------------------------------------------------

  // ----- Tracking QA ------------------------------------------------------
  // KF is currently needed to access magnetic field. In future we will
  // delegate track fit routines to CbmKF as well.
  run->AddTask(new CbmKF());

  TString caParFile = recFile;
  caParFile.ReplaceAll(".root", ".L1Parameters.dat");

  auto* pCaOutputQa = new cbm::ca::OutputQa(verbose, bUseMC);
  pCaOutputQa->SetStsTrackingMode();
  pCaOutputQa->SetConfigName(configName);
  pCaOutputQa->ReadParameters(caParFile.Data());
  pCaOutputQa->SetUseMvd(bUseMvd);
  pCaOutputQa->SetUseSts(bUseSts);
  //pCaOutputQa->SetUseMuch(bUseMuch);
  //pCaOutputQa->SetUseTrd(bUseTrd);
  //pCaOutputQa->SetUseTof(bUseTof);
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
