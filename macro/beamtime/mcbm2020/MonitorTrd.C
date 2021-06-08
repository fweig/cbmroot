/* Copyright (C) 2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig, Pierre-Alain Loizeau [committer] */

/*
 * -----
 * Purpose: macro to test and run mCbm2020 trd unpacking
 * -----
 */


// This macro is based on the general tsa unpacker macro as used by mcbm2019 (/macro/beamtime/mcbm2019/unpack_tsa_mcbm.C) and suppose to allow unpacking of desy 2019 data with the new scheme for testing purpose of the new unpacker

#include <TList.h>
#include <TObjString.h>
#include <TString.h>
#include <TSystem.h>

#include <vector>

// Includes needed for IDE
#if !defined(__CLING__)

#include "CbmDefs.h"
#include "CbmMcbm2018Source.h"
#include "CbmMcbm2018UnpackerTaskTrdR.h"
#include "CbmMcbmUnpack.h"
#include "CbmSetup.h"

#include "FairEventHeader.h"
#include "FairLogger.h"
#include "FairParAsciiFileIo.h"
#include "FairRootFileSink.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"

#endif

FairRunOnline* run = nullptr;

void MonitorTrd(TString inFile = "", TString sHostname = "localhost", Int_t iServerHttpPort = 8080,
                Int_t iServerRefreshRate = 100, UInt_t uRunId = 0, UInt_t nrEvents = 0,
                std::string geoSetupTag = "mcbm_beam_2020_03")
{

  std::string myName = "MonitorTrd";

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  Int_t nEvents = -1;


  if ("" == inFile && "" == sHostname) inFile = "/local/dschmidt/tsa/pulser07.tsa";  // long pulser file

  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  // nEvents = 30;

  // --- Specify output file name (this is just an example)
  TString runId = TString::Format("%u", uRunId);

  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("info");
  // gLogger->SetLogScreenLevel("debug4");
  gLogger->SetLogVerbosityLevel("medium");
  //gLogger->SetLogVerbosityLevel("low");

  // -----   Load the geometry setup   -------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName.data() << ": Loading setup " << geoSetupTag << std::endl;
  CbmSetup* geoSetup = CbmSetup::Instance();
  geoSetup->LoadSetup(geoSetupTag.data());
  // geoSetup->Print();

  // --- Define parameter files
  // ---- Trd ----
  TList* parFileList = new TList();
  TString geoTagTrd  = "";
  bool isActiveTrd   = (geoSetup->GetGeoTag(ECbmModuleId::kTrd, geoTagTrd)) ? true : false;
  if (!isActiveTrd) {
    LOG(warning) << Form("TRD - parameter loading - Trd not found in CbmSetup(%s) -> parameters "
                         "can not be loaded correctly!",
                         geoSetupTag.data());
  }
  else {
    TString paramFilesTrd(Form("%s/parameters/trd/trd_%s", srcDir.Data(), geoTagTrd.Data()));
    std::vector<std::string> paramFilesVecTrd;
    CbmTrdParManager::GetParFileExtensions(&paramFilesVecTrd);
    for (auto parIt : paramFilesVecTrd) {
      parFileList->Add(new TObjString(Form("%s.%s.par", paramFilesTrd.Data(), parIt.data())));
    }
  }

  // --- Set debug level
  gDebug = 0;

  std::cout << std::endl;

  // ========================================================================
  // ========================================================================
  std::cout << std::endl;
  std::cout << ">>> MonitorTrd: Initialising..." << std::endl;

  CbmMcbm2018UnpackerTaskTrdR* unpacker_trdR = new CbmMcbm2018UnpackerTaskTrdR();

  unpacker_trdR->SetMonitorMode();
  unpacker_trdR->SetDebugMonitorMode();

  //  unpacker_trdR ->SetIgnoreOverlapMs(); /// Default is kTRUE

  unpacker_trdR->SetWriteOutput(kFALSE);

  //   // --- Source task
  CbmMcbm2018Source* source = new CbmMcbm2018Source();

  if (0 < uRunId || "" != inFile) { source->SetFileName(inFile); }  // if( "" != inFile )
  else {
    source->SetHostName(sHostname);
    source->SetSubscriberHwm(10);
  }  // else of if( "" != inFile )

  if (isActiveTrd) source->AddUnpacker(unpacker_trdR, 0x40, ECbmModuleId::kTrd);
  // Trd flibId (0x40) as at desy2019. kTrd defined in CbmDefs.h

  source->SetSubscriberHwm(1000);

  // --- Event header
  /*
  FairEventHeader* event = new CbmTbEvent();
  event->SetRunId(uRunId);
*/
  // --- RootFileSink
  // --- Open next outputfile after 4GB
  //  FairRootFileSink* sink = new FairRootFileSink(outFile);
  // sink->GetOutTree()->SetMaxTreeSize(4294967295LL);

  // --- Run
  run = new FairRunOnline(source);
  //  run->SetSink(sink);
  //  run->SetEventHeader(event);
  run->ActivateHttpServer(iServerRefreshRate,
                          iServerHttpPort);  // refresh each 100 events
  /// To avoid the server sucking all Histos from gROOT when no output file is used
  /// ===> Need to explicitely add the canvases to the server in the task!
  run->GetHttpServer()->GetSniffer()->SetScanGlobalDir(kFALSE);
  run->SetAutoFinish(kFALSE);


  // -----   Runtime database   ---------------------------------------------
  FairRuntimeDb* rtdb       = run->GetRuntimeDb();
  FairParAsciiFileIo* parIn = new FairParAsciiFileIo();
  parIn->open(parFileList, "in");
  rtdb->setFirstInput(parIn);

  run->Init();

  // --- Start run
  TStopwatch timer;
  timer.Start();
  std::cout << ">>> MonitorTrd: Starting run..." << std::endl;
  if (0 == nrEvents) {
    run->Run(nEvents, 0);  // run until end of input file
  }
  else {
    run->Run(0, nrEvents);  // process  N Events
  }

  run->Finish();

  timer.Stop();

  std::cout << "Processed " << std::dec << source->GetTsCount() << " timeslices" << std::endl;

  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> MonitorTrd: Macro finished successfully." << std::endl;
  std::cout << ">>> MonitorTrd: Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
