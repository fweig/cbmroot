/*
 * File: /unpack_trd_mcbm2020.C
 * Created Date: Wednesday March 25th 2020
 * Author: Pascal Raisig -- praisig@ikf.uni-frankfurt.de
 * -----
 * Last Modified: Wednesday March 25th 2020 15:00:38
 * Modified By: Pascal Raisig
 * -----
 * Purpose: macro to test and run mCbm2020 trd unpacking
 * -----
 */


// This macro is based on the general tsa unpacker macro as used by mcbm2019 (/macro/beamtime/mcbm2019/unpack_tsa_mcbm.C) and suppose to allow unpacking of desy 2019 data with the new scheme for testing purpose of the new unpacker

#include <vector>

#include <TString.h>
#include <TList.h>
#include <TObjString.h>

#include "FairLogger.h"
#include "FairRootFileSink.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"

// #include "CbmMcbm2018Source.h"
// #include "CbmMcbm2018UnpackerTaskTrdR.h"
// #include "CbmTbEvent.h"

FairRunOnline *run = nullptr;

void unpack_trd_mcbm2020(TString inFile = "", UInt_t uRunId = 0, Int_t nEvents = -1, TString outDir="data", TString inDir="")
{
  // inFile = "/home/praisig/CBM/software/testEnv/data/desy2019/r0070_20190831_0159_0000.tsa"; // FIXME: This is just for testing smarter solution needed! One can probably iterate over files via SetInputDir and the code behind it.
  // inFile = "/home/dspicker/desy2019/r0004_20200220_1951_0000.tsa";
  inFile = "/scratch/mTrd/tsa/r0012_20200408_0939_0000.tsa";    // long pulser file
  // outDir = "/home/praisig/CBM/software/testEnv/data/desy2019/data/";
  outDir = "data";

  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  // nEvents = 30;

  // --- Specify output file name (this is just an example)
  TString runId = TString::Format("%u", uRunId);
  TString outFile = outDir + "/unp_mcbm_trd_" + runId + ".root";
  TString parFile = outDir + "/unp_mcbm_trd_params_" + runId + ".root";

  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("info");
  // gLogger->SetLogScreenLevel("debug4");
  gLogger->SetLogVerbosityLevel("medium");
  //gLogger->SetLogVerbosityLevel("low");

  // --- Define parameter files
  TList *parFileList = new TList();

  //  adjust to required parameter files
  // TString paramDirTrd = srcDir + "/parameters/trd/trd_ikfLabOneSpadic";
  TString paramDirTrd = "/home/praisig/cbm/software/cbmRoot/trdRepo/cbmroot_parameter/trd/trd_v18q_mcbm";

  parFileList->Add(new TObjString(Form("%s.asic.par", paramDirTrd.Data())));
  // parFileList->Add(new TObjString(Form("%s.digi.par", paramDirTrd.Data())));
  // parFileList->Add(new TObjString(Form("%s.gas.par", paramDirTrd.Data())));
  // parFileList->Add(new TObjString(Form("%s.gain.par", paramDirTrd.Data())));

  for (auto parFileVecIt : *parFileList)
  {
    LOG(debug) << Form("TrdParams - %s - added to parameter file list\n", parFileVecIt->GetName());
  }


  // --- Set debug level
  gDebug = 0;

  std::cout << std::endl;
  std::cout << ">>> unpack_tsa: output file is " << outFile << std::endl;

  // ========================================================================
  // ========================================================================
  std::cout << std::endl;
  std::cout << ">>> unpack_tsa: Initialising..." << std::endl;

  CbmMcbm2018UnpackerTaskTrdR * unpacker_trdR = new CbmMcbm2018UnpackerTaskTrdR();
/*
  unpacker_trdR->SetMonitorMode();
  unpacker_trdR->SetDebugMonitorMode();
  unpacker_trdR->SetDebugWriteOutput(); // write rawMessage vector to file
*/
//     // TODO: check trdR task for further needed settings

//     // Use this switch to pass run specific settings to the unpacker task
//   switch( uRunId )
//   {
//     /*
//      case 159:
//      {

//         break;
//      } // 159
//     */

//      default:
//         break;
//   } // switch( uRunId )

//   // --- Source task
  CbmMcbm2018Source* source = new CbmMcbm2018Source();

  source->SetFileName(inFile);
  // source->SetInputDir(inDir);
  source->AddUnpacker(unpacker_trdR,  0x40, ECbmModuleId::kTrd);// Trd flibId (0x40) as at desy2019. kTrd defined in CbmDefs.h

  // --- Event header
  FairEventHeader* event = new CbmTbEvent();
  event->SetRunId(uRunId);

  // --- RootFileSink
  // --- Open next outputfile after 4GB
  FairRootFileSink* sink = new FairRootFileSink(outFile);
  // sink->GetOutTree()->SetMaxTreeSize(4294967295LL);

  // --- Run
  run = new FairRunOnline(source);
  run->SetSink(sink);
  run->SetEventHeader(event);
  run->SetAutoFinish(kFALSE);


  // -----   Runtime database   ---------------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  FairParAsciiFileIo* parIn = new FairParAsciiFileIo();
  parOut->open(parFile.Data());
  parIn->open(parFileList, "in");
  rtdb->setFirstInput(parIn);
  rtdb->setOutput(parOut);

  run->Init();

  // --- Start run
  TStopwatch timer;
  timer.Start();
  std::cout << ">>> unpack_tsa_mcbm: Starting run..." << std::endl;
  if ( 0 >= nEvents) {
    run->Run(nEvents, 0); // run until end of input file
  } else {
    run->Run(0, nEvents); // process  N Events
  }

  run->Finish();

  timer.Stop();

  std::cout << "Processed " << std::dec << source->GetTsCount() << " timeslices" << std::endl;

  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> unpack_tsa_mcbm: Macro finished successfully." << std::endl;
  std::cout << ">>> unpack_tsa_mcbm: Output file is " << outFile << std::endl;
  std::cout << ">>> unpack_tsa_mcbm: Real time " << rtime << " s, CPU time "
	    << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;

}
