/** @file Cern2017Monitor.C
 ** @author Pierre-Alain Loizeau <p.-a.loizeau@gsi.de>
 ** @date 26.07.2017
 **
 ** ROOT macro to read tsa files which have been produced with StsXyter + DPB + FLIB
 ** Convert data into cbmroot format.
 ** Uses CbmFlibTestSource as source task.
 */

// In order to call later Finish, we make this global
FairRunOnline* run = NULL;

void GoodMonitor_kronos(Int_t iRunIdx = 84) {
  const Int_t kiNbRuns        = 89;
  TString sFilePath[kiNbRuns] = {"/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0001_20180220_1034/r0001_20180220_1034",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0002_20180220_1046/r0002_20180220_1046",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0003_20180220_1413/r0003_20180220_1413",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0004_20180222_1622/r0004_20180222_1622",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0005_20180222_1636/r0005_20180222_1636",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0006_20180222_1646/r0006_20180222_1646",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0007_20180227_1058/r0007_20180227_1058",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0008_20180227_1624/r0008_20180227_1624",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0009_20180228_1146/r0009_20180228_1146",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0010_20180228_1246/r0010_20180228_1246",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0011_20180228_1753/r0011_20180228_1753",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0012_20180228_1831/r0012_20180228_1831",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0013_20180228_1905/r0013_20180228_1905",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0014_20180301_0951/r0014_20180301_0951",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0015_20180301_1011/r0015_20180301_1011",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0016_20180301_1054/r0016_20180301_1054",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0017_20180301_1116/r0017_20180301_1116",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0018_20180301_1131/r0018_20180301_1131",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0019_20180301_1149/r0019_20180301_1149",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0020_20180301_1813/r0020_20180301_1813",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0021_20180301_1836/r0021_20180301_1836",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0022_20180301_1900/r0022_20180301_1900",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0023_20180301_1923/r0023_20180301_1923",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0024_20180301_1929/r0024_20180301_1929",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0025_20180301_1938/r0025_20180301_1938",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0026_20180301_2006/r0026_20180301_2006",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0027_20180301_2056/r0027_20180301_2056",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0028_20180301_2104/r0028_20180301_2104",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0029_20180301_2120/r0029_20180301_2120",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0030_20180301_2139/r0030_20180301_2139",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0031_20180302_0810/r0031_20180302_0810",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0032_20180302_0914/r0032_20180302_0914",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0033_20180302_0923/r0033_20180302_0923",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0034_20180302_1025/r0034_20180302_1025",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0035_20180302_1247/r0035_20180302_1247",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0036_20180302_1307/r0036_20180302_1307",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0037_20180302_1320/r0037_20180302_1320",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0038_20180302_1336/r0038_20180302_1336",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0039_20180302_1437/r0039_20180302_1437",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0040_20180302_1449/r0040_20180302_1449",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0041_20180302_1503/r0041_20180302_1503",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0042_20180302_1517/r0042_20180302_1517",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0043_20180302_1532/r0043_20180302_1532",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0044_20180302_1555/r0044_20180302_1555",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0045_20180302_1642/r0045_20180302_1642",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0046_20180302_1726/r0046_20180302_1726",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0047_20180302_2041/r0047_20180302_2041",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0048_20180303_0852/r0048_20180303_0852",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0049_20180303_0900/r0049_20180303_0900",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0050_20180303_0918/r0050_20180303_0918",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0051_20180303_0945/r0051_20180303_0945",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0052_20180303_1006/r0052_20180303_1006",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0053_20180303_1022/r0053_20180303_1022",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0054_20180303_1033/r0054_20180303_1033",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0055_20180303_1048/r0055_20180303_1048",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0056_20180303_1100/r0056_20180303_1100",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0057_20180303_1120/r0057_20180303_1120",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0058_20180303_1140/r0058_20180303_1140",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0059_20180303_1714/r0059_20180303_1714",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0060_20180303_1903/r0060_20180303_1903",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0061_20180303_1944/r0061_20180303_1944",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0062_20180303_2035/r0062_20180303_2035",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0063_20180303_2115/r0063_20180303_2115",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0064_20180303_2134/r0064_20180303_2134",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0065_20180303_2157/r0065_20180303_2157",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0066_20180303_2211/r0066_20180303_2211",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0067_20180303_2224/r0067_20180303_2224",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0068_20180303_2244/r0068_20180303_2244",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0069_20180304_0813/r0069_20180304_0813",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0070_20180304_0847/r0070_20180304_0847",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0071_20180304_0905/r0071_20180304_0905",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0072_20180304_0930/r0072_20180304_0930",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0073_20180304_0956/r0073_20180304_0956",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0074_20180304_1019/r0074_20180304_1019",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0075_20180304_1041/r0075_20180304_1041",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0076_20180304_1056/r0076_20180304_1056",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0077_20180304_1111/r0077_20180304_1111",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0078_20180304_1130/r0078_20180304_1130",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0079_20180304_1149/r0079_20180304_1149",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0080_20180304_1429/r0080_20180304_1429",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0081_20180304_1456/r0081_20180304_1456",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0082_20180304_1504/r0082_20180304_1504",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0083_20180304_1517/r0083_20180304_1517",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0084_20180304_1627/r0084_20180304_1627",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0085_20180304_1721/r0085_20180304_1721",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0086_20180304_1729/r0086_20180304_1729",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0087_20180304_1753/r0087_20180304_1753",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0088_20180304_1807/r0088_20180304_1807",
                                 "/lustre/nyx/cbm/prod/beamtime/2018/02/cosy/"
                                 "r0089_20180304_1826/r0089_20180304_1826"};
  Int_t iTsaFileNb[kiNbRuns]  = {
    1,  1,  1,  3,  3,  2,  2,  14, 41, 26, 37, 25, 45, 13, 36, 7,  4,  6,
    6,  11, 10, 12, 11, 8,  11, 26, 1,  8,  9,  8,  3,  3,  22, 17, 6,  4,
    5,  4,  1,  5,  5,  5,  9,  13, 27, 45, 6,  9,  4,  10, 5,  4,  3,  5,
    4,  5,  7,  56, 34, 13, 13, 18, 6,  10, 7,  6,  8,  9,  3,  8,  10, 11,
    11, 11, 6,  6,  8,  6,  6,  9,  1,  3,  39, 44, 15, 35, 6,  7,  10};

  if (iRunIdx <= 0 || kiNbRuns < iRunIdx) return;

  TString inFile = sFilePath[iRunIdx - 1];
  //  Int_t   iStopFile = iTsaFileNb[ iRunIdx - 1 ];
  Int_t iStopFile = 1;

  // --- Specify number of events to be produced.
  // --- -1 means run until the end of the input file.
  Int_t nEvents = -1;

  // --- Specify output file name (this is just an example)
  TString outFile = Form("data/good_out_r%04u.root", iRunIdx);
  TString parFile = Form("data/good_param_r%04u.root", iRunIdx);

  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("INFO");
  //  gLogger->SetLogScreenLevel("DEBUG");
  gLogger->SetLogVerbosityLevel("LOW");

  // --- Define parameter files
  TList* parFileList = new TList();
  TString paramDir   = "./";

  TString paramFileHodo          = paramDir + "HodoUnpackPar.par";
  TObjString* tutDetDigiFileHodo = new TObjString(paramFileHodo);
  parFileList->Add(tutDetDigiFileHodo);

  TString paramFileSts          = paramDir + "StsUnpackPar.par";
  TObjString* tutDetDigiFileSts = new TObjString(paramFileSts);
  parFileList->Add(tutDetDigiFileSts);

  // --- Set debug level
  gDebug = 0;

  std::cout << std::endl;
  std::cout << ">>> Cern2017Monitor: output file is " << outFile << std::endl;

  // ========================================================================
  // ========================================================================

  std::cout << std::endl;
  std::cout << ">>> Cern2017Monitor: Initialising..." << std::endl;

  // Hodoscopes Monitor
  CbmCosy2018MonitorSetupGood* monitorGood = new CbmCosy2018MonitorSetupGood();
  monitorGood->SetHistoFileName(Form("data/GoodHistos_r%04u.root", iRunIdx));
  //  monitorGood->SetPrintMessage();
  monitorGood->SetMsOverlap();
  monitorGood->EnableDualStsMode(kTRUE);
  //  monitorGood->SetLongDurationLimits( 3600, 10 );
  monitorGood->SetLongDurationLimits(7200, 60);
  monitorGood->SetCoincidenceBorder(150);
  monitorGood->SetStripsOffset1(69, -67);
  monitorGood->SetStripsOffset2(69, -67);

  // --- Source task
  CbmTofStar2018Source* source = new CbmTofStar2018Source();
  if ("" != inFile) {
    for (Int_t iFileIdx = 0; iFileIdx < iStopFile; ++iFileIdx) {
      TString sFilePath = Form("%s_%04u.tsa", inFile.Data(), iFileIdx);
      source->AddFile(sFilePath);
      std::cout << "Added " << sFilePath << std::endl;
    }  // for( Int_t iFileIdx = iStartFile; iFileIdx < iStopFile; ++iFileIdx )
  }    // if( "" != inFile )
  else {
    source->SetHostName("cbmin002");
    source->SetPortNumber(5556);
  }

  source->AddUnpacker(monitorGood, 0x10, 6);  // stsXyter DPBs

  // --- Event header
  FairEventHeader* event = new CbmTbEvent();
  event->SetRunId(1);

  // --- Run
  run = new FairRunOnline(source);
  run->SetOutputFile(outFile);
  run->SetEventHeader(event);
  //  run->ActivateHttpServer( iServerRefreshRate, iServerHttpPort ); // refresh each 100 events
  run->SetAutoFinish(kFALSE);

  // -----   Runtime database   ---------------------------------------------
  FairRuntimeDb* rtdb       = run->GetRuntimeDb();
  Bool_t kParameterMerged   = kTRUE;
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
  std::cout << ">>> Cern2017Monitor: Starting run..." << std::endl;
  run->Run(nEvents, 0);  // run until end of input file
  timer.Stop();

  std::cout << "Processed " << std::dec << source->GetTsCount() << " timeslices"
            << std::endl;

  run->Finish();

  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> Cern2017Monitor: Macro finished successfully." << std::endl;
  std::cout << ">>> Cern2017Monitor: Output file is " << outFile << std::endl;
  std::cout << ">>> Cern2017Monitor: Real time " << rtime << " s, CPU time "
            << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
