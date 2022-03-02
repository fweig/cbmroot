/* Copyright (C) 2019-2020 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig, Adrian Amatus Weber [committer] */

/** @file MCBM DATA unpacking and ToT Shift calculation
 ** @author Florian Uhlig <f.uhlig@gsi.de>
 ** @date 20.06.2016
 ** Modified by P.-A. Loizeau
 ** @date 30.01.2019
 ** Modified by A. Weber
 ** @date 07.02.2022
 ** ROOT macro to read digi.root file which have been produced with latest unpacker.
 ** Calculate the ToT Shifts (relative to current paramters) in all channels of diriches for Parameter File.
 */
// In order to call later Finish, we make this global

void getToTOffsetDigi(UInt_t uRunId = 831, UInt_t nrEvents = 10000, TString outDir = "./data/ToTOffset/",
                      TString inDir = "")  //1Event is 1TS
{
  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  TString inputDir = "/data/cbmroot/cbmsource/macro/run/data";
  TString inFile   = Form("%s/2022038090316_0.digi.root", inputDir.Data());
  //TString inFile   = Form("%s/%u.digi.root", inputDir.Data(),uRunId);

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  Int_t nEvents = -1;

  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("INFO");
  //gLogger->SetLogScreenLevel("DEBUG4");
  gLogger->SetLogVerbosityLevel("MEDIUM");
  //gLogger->SetLogVerbosityLevel("LOW");

  // --- Define parameter files
  TList* parFileList = new TList();
  TString paramDir   = srcDir + "/macro/beamtime/mcbm2021/";

  TString paramFileRich       = paramDir + "mRichPar_70.par";
  TObjString* parRichFileName = new TObjString(paramFileRich);
  parFileList->Add(parRichFileName);

  // --- Set debug level
  gDebug = 0;

  // ------------------------------------------------------------------------

  FairRunAna* run             = new FairRunAna();
  FairFileSource* inputSource = new FairFileSource(inFile.Data());
  run->SetSource(inputSource);

  // Add ToT Correction Finder
  CbmRichMCbmToTShifter* tot = new CbmRichMCbmToTShifter();
  //tot->GeneratePDF();
  tot->ShowTdcId(true);
  run->AddTask(tot);

  // -----   Runtime database   ---------------------------------------------
  FairRuntimeDb* rtdb       = run->GetRuntimeDb();
  Bool_t kParameterMerged   = kTRUE;
  FairParAsciiFileIo* parIn = new FairParAsciiFileIo();
  parIn->open(parFileList, "in");
  rtdb->setFirstInput(parIn);

  run->Init();

  // --- Start run
  TStopwatch timer;
  timer.Start();
  std::cout << ">>> getToTOffsetDigi: Starting run..." << std::endl;
  if (0 == nrEvents) {
    run->Run(nEvents, 0);  // run until end of input file
  }
  else {
    run->Run(0, nrEvents);  // process  2000 Events
  }

  timer.Stop();

  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> getToTOffsetDigi: Macro finished successfully." << std::endl;
  //std::cout << ">>> unpack_tsa_mcbm: Output file is " << outFile << std::endl;
  std::cout << ">>> getToTOffsetDigi: Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << std::endl;
}
