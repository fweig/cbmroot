/* Copyright (C) 2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig, Pierre-Alain Loizeau [committer] */

/** @file MCBM DATA unpacking
 ** @author Florian Uhlig <f.uhlig@gsi.de>
 ** @date 20.06.2016
 ** Modified by P.-A. Loizeau
 ** @date 30.01.2019
 ** ROOT macro to read tsa files which have been produced with the new data transport
 ** Convert data into cbmroot format.
 ** Uses CbmMcbm2018Source as source task.
 */
// In order to call later Finish, we make this global
FairRunOnline* run = NULL;

void unpack_pulser_mcbm(UInt_t uRunId = 0, TString sHostname = "localhost", UInt_t nrEvents = 0,
                        TString outDir = "data", Int_t iServerRefreshRate = 100, Int_t iServerHttpPort = 8080)
{
  if (uRunId < 353 && 0 != uRunId) return kFALSE;

  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  Int_t nEvents = -1;
  // --- Specify output file name (this is just an example)
  TString runId   = TString::Format("%03u", uRunId);
  TString outFile = outDir + "/unp_mcbm_" + runId + ".root";
  TString parFile = outDir + "/unp_mcbm_params_" + runId + ".root";

  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("INFO");
  //gLogger->SetLogScreenLevel("DEBUG4");
  gLogger->SetLogVerbosityLevel("MEDIUM");
  //gLogger->SetLogVerbosityLevel("LOW");

  // --- Define parameter files
  TList* parFileList = new TList();
  TString paramDir   = "./";

  // ---- Load the geometry setup ----
  // This is currently only required by the TRD
  std::string geoSetupTag = "mcbm_beam_2020_03";
  CbmSetup* geoSetup      = CbmSetup::Instance();
  geoSetup->LoadSetup(geoSetupTag.data());

  TString paramFileSts       = paramDir + "mStsPar_pulser.par";
  TObjString* parStsFileName = new TObjString(paramFileSts);
  parFileList->Add(parStsFileName);

  TString paramFileMuch       = paramDir + "mMuchPar.par";
  TObjString* parMuchFileName = new TObjString(paramFileMuch);
  parFileList->Add(parMuchFileName);

  // ---- Trd ----
  TString geoTagTrd = "";
  bool isActiveTrd  = (geoSetup->GetGeoTag(ECbmModuleId::kTrd, geoTagTrd)) ? true : false;
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

  TString paramFileTof       = paramDir + "mTofPar.par";
  TObjString* parTofFileName = new TObjString(paramFileTof);
  parFileList->Add(parTofFileName);

  TString paramFileRich       = paramDir + "mRichPar.par";
  TObjString* parRichFileName = new TObjString(paramFileRich);
  parFileList->Add(parRichFileName);

  TString paramFilePsd       = paramDir + "mPsdPar.par";
  TObjString* parPsdFileName = new TObjString(paramFilePsd);
  parFileList->Add(parPsdFileName);

  // --- Set debug level
  gDebug = 0;

  std::cout << std::endl;
  std::cout << ">>> unpack_tsa: output file is " << outFile << std::endl;

  // ========================================================================
  // ========================================================================
  std::cout << std::endl;
  std::cout << ">>> unpack_tsa: Initialising..." << std::endl;

  CbmMcbm2018UnpackerTaskSts* unpacker_sts   = new CbmMcbm2018UnpackerTaskSts();
  CbmMcbm2018UnpackerTaskMuch* unpacker_much = new CbmMcbm2018UnpackerTaskMuch();
  CbmMcbm2018UnpackerTaskTrdR* unpacker_trdR = new CbmMcbm2018UnpackerTaskTrdR();
  CbmMcbm2018UnpackerTaskTof* unpacker_tof   = new CbmMcbm2018UnpackerTaskTof();
  CbmMcbm2018UnpackerTaskRich* unpacker_rich = new CbmMcbm2018UnpackerTaskRich();
  CbmMcbm2018UnpackerTaskPsd* unpacker_psd   = new CbmMcbm2018UnpackerTaskPsd();

  unpacker_sts->SetMonitorMode();
  unpacker_much->SetMonitorMode();
  unpacker_trdR->SetMonitorMode();
  unpacker_tof->SetMonitorMode();
  unpacker_rich->SetMonitorMode();
  unpacker_psd->SetMonitorMode();

  unpacker_sts->SetIgnoreOverlapMs();
  unpacker_much->SetIgnoreOverlapMs();
  //  unpacker_trdR ->SetIgnoreOverlapMs(); /// Default is kTRUE
  unpacker_tof->SetIgnoreOverlapMs();
  unpacker_rich->SetIgnoreOverlapMs();
  unpacker_psd->SetIgnoreOverlapMs();

  unpacker_sts->SetWriteOutputFlag(kFALSE);
  unpacker_much->SetWriteOutputFlag(kFALSE);
  unpacker_trdR->SetWriteOutput(kFALSE);
  unpacker_tof->SetWriteOutputFlag(kFALSE);
  unpacker_rich->SetWriteOutputFlag(kFALSE);
  unpacker_psd->SetWriteOutputFlag(kFALSE);

  unpacker_sts->SetAdcCut(3);
  unpacker_tof->SetSeparateArrayT0();

  switch (uRunId) {
      /*
     case 159:
     {
        /// General System offsets (= offsets between sub-systems)
        unpacker_sts ->SetTimeOffsetNs( -1750 ); // Run 159
        unpacker_much->SetTimeOffsetNs( -1750 ); // Run 159
        unpacker_tof ->SetTimeOffsetNs(   -50 ); // Run 159
        unpacker_rich->SetTimeOffsetNs( -1090 ); // Run 159

        /// ASIC specific offsets (= offsets inside sub-system)
        unpacker_sts ->SetTimeOffsetNsAsic(  0,       0.0  ); // Unused
        unpacker_sts ->SetTimeOffsetNsAsic(  1,       0.0  ); // Unused
        unpacker_sts ->SetTimeOffsetNsAsic(  2,       0.0  ); // Unused
        unpacker_sts ->SetTimeOffsetNsAsic(  3,       0.0  ); // Unused
        unpacker_sts ->SetTimeOffsetNsAsic(  4,       0.0  ); // Unused
        unpacker_sts ->SetTimeOffsetNsAsic(  5,       0.0  ); // Unused
        unpacker_sts ->SetTimeOffsetNsAsic(  6,       0.0  ); // Unused
        unpacker_sts ->SetTimeOffsetNsAsic(  7,       0.0  ); // Unused
        unpacker_sts ->SetTimeOffsetNsAsic(  8,       0.0  ); // Run 160, Ladder 0, Module 1, N, Asic 0
        unpacker_sts ->SetTimeOffsetNsAsic(  9,      18.75 ); // Run 160, Ladder 0, Module 1, N, Asic 1
        unpacker_sts ->SetTimeOffsetNsAsic( 10,       0.0  ); // Run 160, Ladder 0, Module 1, N, Asic 2
        unpacker_sts ->SetTimeOffsetNsAsic( 11,      25.0  ); // Run 160, Ladder 0, Module 1, N, Asic 3
        unpacker_sts ->SetTimeOffsetNsAsic( 12,       0.0  ); // Run 160, Ladder 0, Module 1, N, Asic 4
        unpacker_sts ->SetTimeOffsetNsAsic( 13,      56.25 ); // Run 160, Ladder 0, Module 1, N, Asic 5
        unpacker_sts ->SetTimeOffsetNsAsic( 14,       0.0  ); // Run 160, Ladder 0, Module 1, N, Asic 6
        unpacker_sts ->SetTimeOffsetNsAsic( 15,      37.5  ); // Run 160, Ladder 0, Module 1, N, Asic 7
        unpacker_sts ->SetTimeOffsetNsAsic( 16,       0.0  ); // Run 160, Ladder 0, Module 1, P, Asic 0
        unpacker_sts ->SetTimeOffsetNsAsic( 17,       0.0  ); // Run 160, Ladder 0, Module 1, P, Asic 1
        unpacker_sts ->SetTimeOffsetNsAsic( 18,       0.0  ); // Run 160, Ladder 0, Module 1, P, Asic 2
        unpacker_sts ->SetTimeOffsetNsAsic( 19,      50.0  ); // Run 160, Ladder 0, Module 1, P, Asic 3
        unpacker_sts ->SetTimeOffsetNsAsic( 20,       0.0  ); // Run 160, Ladder 0, Module 1, P, Asic 4
        unpacker_sts ->SetTimeOffsetNsAsic( 21,       0.0  ); // Run 160, Ladder 0, Module 1, P, Asic 5
        unpacker_sts ->SetTimeOffsetNsAsic( 22,       0.0  ); // Run 160, Ladder 0, Module 1, P, Asic 6
        unpacker_sts ->SetTimeOffsetNsAsic( 23,      25.0  ); // Run 160, Ladder 0, Module 1, P, Asic 7
        unpacker_sts ->SetTimeOffsetNsAsic( 24,      50.0  ); // Run 160, Ladder 0, Module 0, N, Asic 0
        unpacker_sts ->SetTimeOffsetNsAsic( 25,      25.0  ); // Run 160, Ladder 0, Module 0, N, Asic 1
        unpacker_sts ->SetTimeOffsetNsAsic( 26,      50.0  ); // Run 160, Ladder 0, Module 0, N, Asic 2
        unpacker_sts ->SetTimeOffsetNsAsic( 27,      31.25 ); // Run 160, Ladder 0, Module 0, N, Asic 3
        unpacker_sts ->SetTimeOffsetNsAsic( 28,       0.0  ); // Run 160, Ladder 0, Module 0, N, Asic 4
        unpacker_sts ->SetTimeOffsetNsAsic( 29,       6.25 ); // Run 160, Ladder 0, Module 0, N, Asic 5
        unpacker_sts ->SetTimeOffsetNsAsic( 30,      50.0  ); // Run 160, Ladder 0, Module 0, N, Asic 6
        unpacker_sts ->SetTimeOffsetNsAsic( 31,      31.25 ); // Run 160, Ladder 0, Module 0, N, Asic 7
        unpacker_sts ->SetTimeOffsetNsAsic( 32,       0.0  ); // Run 160, Ladder 0, Module 0, P, Asic 0
        unpacker_sts ->SetTimeOffsetNsAsic( 33,      31.25 ); // Run 160, Ladder 0, Module 0, P, Asic 1
        unpacker_sts ->SetTimeOffsetNsAsic( 34,       0.0  ); // Run 160, Ladder 0, Module 0, P, Asic 2
        unpacker_sts ->SetTimeOffsetNsAsic( 35,      25.0  ); // Run 160, Ladder 0, Module 0, P, Asic 3
        unpacker_sts ->SetTimeOffsetNsAsic( 36,      25.0  ); // Run 160, Ladder 0, Module 0, P, Asic 4
        unpacker_sts ->SetTimeOffsetNsAsic( 37,      25.0  ); // Run 160, Ladder 0, Module 0, P, Asic 5
        unpacker_sts ->SetTimeOffsetNsAsic( 38,       0.0  ); // Run 160, Ladder 0, Module 0, P, Asic 6
        unpacker_sts ->SetTimeOffsetNsAsic( 39,       0.0  ); // Run 160, Ladder 0, Module 0, P, Asic 7

        unpacker_much->SetTimeOffsetNsAsic(  0,       0.0 ); // Run 159, DPB 0 ASIC 0
        unpacker_much->SetTimeOffsetNsAsic(  1,     109.0 ); // Run 159, DPB 0 ASIC 1
        unpacker_much->SetTimeOffsetNsAsic(  2,     142.0 ); // Run 159, DPB 0 ASIC 2
        unpacker_much->SetTimeOffsetNsAsic(  3,      84.0 ); // Run 159, DPB 0 ASIC 3
        unpacker_much->SetTimeOffsetNsAsic(  4,     109.0 ); // Run 159, DPB 0 ASIC 4
        unpacker_much->SetTimeOffsetNsAsic(  5,       0.0 ); // Run 159, DPB 0 ASIC 5
        unpacker_much->SetTimeOffsetNsAsic(  6, 2820915.0 ); // Run 159, DPB 1 ASIC 0
        unpacker_much->SetTimeOffsetNsAsic(  7, 2820905.0 ); // Run 159, DPB 1 ASIC 1
        unpacker_much->SetTimeOffsetNsAsic(  8, 2820785.0 ); // Run 159, DPB 1 ASIC 2
        unpacker_much->SetTimeOffsetNsAsic(  9, 2820915.0 ); // Run 159, DPB 1 ASIC 3
        unpacker_much->SetTimeOffsetNsAsic( 10,       0.0 ); // Run 159, DPB 1 ASIC 4
        unpacker_much->SetTimeOffsetNsAsic( 11, 2820805.0 ); // Run 159, DPB 1 ASIC 5
        unpacker_much->SetTimeOffsetNsAsic( 12,    8144.0 ); // Run 159, DPB 2 ASIC 0
        unpacker_much->SetTimeOffsetNsAsic( 13,    8133.0 ); // Run 159, DPB 2 ASIC 1
        unpacker_much->SetTimeOffsetNsAsic( 14,       0.0 ); // Run 159, DPB 2 ASIC 2
        unpacker_much->SetTimeOffsetNsAsic( 15,       0.0 ); // Run 159, DPB 2 ASIC 3
        unpacker_much->SetTimeOffsetNsAsic( 16,       0.0 ); // Run 159, DPB 2 ASIC 4
        unpacker_much->SetTimeOffsetNsAsic( 17,       0.0 ); // Run 159, DPB 2 ASIC 5
        unpacker_much->SetTimeOffsetNsAsic( 18,     136.0 ); // Run 159, DPB 3 ASIC 0
        unpacker_much->SetTimeOffsetNsAsic( 19,     119.0 ); // Run 159, DPB 3 ASIC 1
        unpacker_much->SetTimeOffsetNsAsic( 20,     141.0 ); // Run 159, DPB 3 ASIC 2
        unpacker_much->SetTimeOffsetNsAsic( 21,       0.0 ); // Run 159, DPB 3 ASIC 3
        unpacker_much->SetTimeOffsetNsAsic( 22,       0.0 ); // Run 159, DPB 3 ASIC 4
        unpacker_much->SetTimeOffsetNsAsic( 23,       0.0 ); // Run 159, DPB 3 ASIC 5

        break;
     } // 159
*/
    default: break;
  }  // switch( uRunId )

  // --- Source task
  CbmMcbm2018Source* source = new CbmMcbm2018Source();
  /*
  TString inFile = Form( "/scratch/mcbm_data/mcbm_all/data/%3u_pn02_0000.tsa;", uRunId );
  inFile += Form( "/scratch/mcbm_data/mcbm_all/data/%3u_pn04_0000.tsa;", uRunId );
  inFile += Form( "/scratch/mcbm_data/mcbm_all/data/%3u_pn05_0000.tsa;", uRunId );
  inFile += Form( "/scratch/mcbm_data/mcbm_all/data/%3u_pn06_0000.tsa;", uRunId );
  inFile += Form( "/scratch/mcbm_data/mcbm_all/data/%3u_pn08_0000.tsa;", uRunId );
  inFile += Form( "/scratch/mcbm_data/mcbm_all/data/%3u_pn10_0000.tsa;", uRunId );
  inFile += Form( "/scratch/mcbm_data/mcbm_all/data/%3u_pn11_0000.tsa;", uRunId );
  inFile += Form( "/scratch/mcbm_data/mcbm_all/data/%3u_pn12_0000.tsa;", uRunId );
  inFile += Form( "/scratch/mcbm_data/mcbm_all/data/%3u_pn13_0000.tsa;", uRunId );
  inFile += Form( "/scratch/mcbm_data/mcbm_all/data/%3u_pn15_0000.tsa", uRunId );
*/
  /*
  TString inFile = Form( "/scratch/mcbm_data/mcbm_all/data/%3u_pn02_0003.tsa;", uRunId );
  inFile += Form( "/scratch/mcbm_data/mcbm_all/data/%3u_pn04_0003.tsa;", uRunId );
  inFile += Form( "/scratch/mcbm_data/mcbm_all/data/%3u_pn05_0003.tsa;", uRunId );
  inFile += Form( "/scratch/mcbm_data/mcbm_all/data/%3u_pn06_0003.tsa;", uRunId );
  inFile += Form( "/scratch/mcbm_data/mcbm_all/data/%3u_pn08_0003.tsa;", uRunId );
  inFile += Form( "/scratch/mcbm_data/mcbm_all/data/%3u_pn10_0003.tsa;", uRunId );
  inFile += Form( "/scratch/mcbm_data/mcbm_all/data/%3u_pn11_0003.tsa;", uRunId );
  inFile += Form( "/scratch/mcbm_data/mcbm_all/data/%3u_pn12_0003.tsa;", uRunId );
  inFile += Form( "/scratch/mcbm_data/mcbm_all/data/%3u_pn13_0003.tsa;", uRunId );
  inFile += Form( "/scratch/mcbm_data/mcbm_all/data/%3u_pn15_0003.tsa", uRunId );
*/

  TString inFile = Form("/scratch/mcbm_data/mcbm_all/data/%3u_pn02_*.tsa;", uRunId);
  inFile += Form("/scratch/mcbm_data/mcbm_all/data/%3u_pn04_*.tsa;", uRunId);
  inFile += Form("/scratch/mcbm_data/mcbm_all/data/%3u_pn05_*.tsa;", uRunId);
  inFile += Form("/scratch/mcbm_data/mcbm_all/data/%3u_pn06_*.tsa;", uRunId);
  inFile += Form("/scratch/mcbm_data/mcbm_all/data/%3u_pn08_*.tsa;", uRunId);
  inFile += Form("/scratch/mcbm_data/mcbm_all/data/%3u_pn10_*.tsa;", uRunId);
  inFile += Form("/scratch/mcbm_data/mcbm_all/data/%3u_pn11_*.tsa;", uRunId);
  inFile += Form("/scratch/mcbm_data/mcbm_all/data/%3u_pn12_*.tsa;", uRunId);
  inFile += Form("/scratch/mcbm_data/mcbm_all/data/%3u_pn13_*.tsa;", uRunId);
  inFile += Form("/scratch/mcbm_data/mcbm_all/data/%3u_pn15_*.tsa", uRunId);

  if (415 < uRunId) {
    inFile = Form("/scratch/mcbm_data/mcbm_all/pulser/%3u_pn02_*.tsa;", uRunId);
    inFile += Form("/scratch/mcbm_data/mcbm_all/pulser/%3u_pn04_*.tsa;", uRunId);
    inFile += Form("/scratch/mcbm_data/mcbm_all/pulser/%3u_pn05_*.tsa;", uRunId);
    inFile += Form("/scratch/mcbm_data/mcbm_all/pulser/%3u_pn06_*.tsa;", uRunId);
    inFile += Form("/scratch/mcbm_data/mcbm_all/pulser/%3u_pn08_*.tsa;", uRunId);
    inFile += Form("/scratch/mcbm_data/mcbm_all/pulser/%3u_pn10_*.tsa;", uRunId);
    inFile += Form("/scratch/mcbm_data/mcbm_all/pulser/%3u_pn11_*.tsa;", uRunId);
    inFile += Form("/scratch/mcbm_data/mcbm_all/pulser/%3u_pn12_*.tsa;", uRunId);
    inFile += Form("/scratch/mcbm_data/mcbm_all/pulser/%3u_pn13_*.tsa;", uRunId);
    inFile += Form("/scratch/mcbm_data/mcbm_all/pulser/%3u_pn15_*.tsa", uRunId);
  }  // if( 415 < uRunId )
  if (430 < uRunId) {
    inFile = Form("/scratch/mcbm_data/mcbm_all/data/%3u_pn02_*.tsa;", uRunId);
    inFile += Form("/scratch/mcbm_data/mcbm_all/data/%3u_pn04_*.tsa;", uRunId);
    inFile += Form("/scratch/mcbm_data/mcbm_all/data/%3u_pn06_*.tsa;", uRunId);
    inFile += Form("/scratch/mcbm_data/mcbm_all/data/%3u_pn08_*.tsa;", uRunId);
    inFile += Form("/scratch/mcbm_data/mcbm_all/data/%3u_pn10_*.tsa;", uRunId);
    inFile += Form("/scratch/mcbm_data/mcbm_all/data/%3u_pn11_*.tsa;", uRunId);
    inFile += Form("/scratch/mcbm_data/mcbm_all/data/%3u_pn12_*.tsa;", uRunId);
    inFile += Form("/scratch/mcbm_data/mcbm_all/data/%3u_pn13_*.tsa;", uRunId);
    inFile += Form("/scratch/mcbm_data/mcbm_all/data/%3u_pn15_*.tsa", uRunId);
  }  // if( 430 < uRunId )

  if (0 < uRunId) { source->SetFileName(inFile); }  // if( "" != inFile )
  else {
    source->SetHostName(sHostname);
    source->SetSubscriberHwm(10);
  }  // else of if( "" != inFile )

  //  source->SetInputDir(inDir);
  source->AddUnpacker(unpacker_sts, 0x10, ECbmModuleId::kSts);                    //STS xyter
  source->AddUnpacker(unpacker_much, 0x50, ECbmModuleId::kMuch);                  //MUCH xyter
  if (isActiveTrd) source->AddUnpacker(unpacker_trdR, 0x40, ECbmModuleId::kTrd);  // Trd
  source->AddUnpacker(unpacker_tof, 0x60, ECbmModuleId::kTof);                    //gDPB A & B & C
  source->AddUnpacker(unpacker_tof, 0x90, ECbmModuleId::kTof);                    //gDPB T0 A & B
  source->AddUnpacker(unpacker_rich, 0x30, ECbmModuleId::kRich);                  //RICH trb
  source->AddUnpacker(unpacker_psd, 0x80, ECbmModuleId::kPsd);                    //PSD


  // --- Run
  run = new FairRunOnline(source);
  run->ActivateHttpServer(iServerRefreshRate,
                          iServerHttpPort);  // refresh each 100 events
  /// To avoid the server sucking all Histos from gROOT when no output file is used
  /// ===> Need to explicitely add the canvases to the server in the task!
  run->GetHttpServer()->GetSniffer()->SetScanGlobalDir(kFALSE);
  run->SetAutoFinish(kFALSE);

  CbmMcbm2019CheckPulser* pulserChecker = new CbmMcbm2019CheckPulser();
  pulserChecker->SetT0PulserTotLimits(182, 191);
  pulserChecker->SetStsPulserAdcLimits(0, 31);
  pulserChecker->SetMuchPulserAdcLimits(5, 25);  /// mCBM 2019
  pulserChecker->SetTrdPulserChargeLimits(0, 100000);
  pulserChecker->SetTofPulserTotLimits(90, 100);
  pulserChecker->SetRichPulserTotLimits(0, 255);
  pulserChecker->SetPsdPulserAdcLimits(0, 170000);

  pulserChecker->SetStsOffsetSearchRange(2000);
  pulserChecker->SetMuchOffsetSearchRange(5000);
  pulserChecker->SetTrdOffsetSearchRange(1000);
  pulserChecker->SetTofOffsetSearchRange(1000);
  pulserChecker->SetRichOffsetSearchRange(1000);
  pulserChecker->SetPsdOffsetSearchRange(500);

  //  pulserChecker->SetStsOffsetSearchRange(  1000000 );
  //  pulserChecker->SetMuchOffsetSearchRange( 1000000 );
  //  pulserChecker->SetTrdOffsetSearchRange(  1000000 );
  //  pulserChecker->SetTofOffsetSearchRange(  1000000 );
  //  pulserChecker->SetRichOffsetSearchRange( 1000000 );
  pulserChecker->SetPsdOffsetSearchRange(1000000);

  if (0 < uRunId) pulserChecker->SetOutFilename(Form("data/HistosPulserCheck_%03u.root", uRunId));

  /// mCBM 2019-2020
  //  pulserChecker->SetStsAddress( 0x99999999 );
  pulserChecker->SetMuchAsic(9);
  pulserChecker->SetMuchChanRange(63);
  /// For PSD pick channel 0

  run->AddTask(pulserChecker);


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
  std::cout << ">>> unpack_tsa_mcbm: Starting run..." << std::endl;
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
  std::cout << ">>> unpack_tsa_mcbm: Macro finished successfully." << std::endl;
  std::cout << ">>> unpack_tsa_mcbm: Output file is " << outFile << std::endl;
  std::cout << ">>> unpack_tsa_mcbm: Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
