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

void unpack_check_time_mcbm_kronos(UInt_t uRunIdx  = 99999,
                                   UInt_t nrEvents = 0,
                                   TString outDir  = "data") {
  UInt_t uRunId = 0;
  if (99999 != uRunIdx) {
    std::vector<UInt_t> vuListRunId = {
      692, 698, 702, 704, 705, 706, 707,            //  7 =>  0 -  6
      744, 750, 759, 760, 761, 762, 799,            //  7 =>  7 - 13
      811, 812, 816, 817, 819,                      //  5 => 14 - 18
      820, 821, 822, 824, 826, 827, 828, 829,       //  8 => 19 - 26
      830, 831, 836,                                //  3 => 27 - 29
      841, 846, 849,                                //  3 => 30 - 32
      850, 851, 852, 854, 855, 856, 857, 858, 859,  //  9 => 33 - 41
      860, 861, 862, 863, 864, 865, 866             //  7 => 42 - 48
                                                    /*
 /// With runs < 1 min due to missmatch!
             811, 812, 816, 817, 818, 819,                     //  6 => 14 - 19
             820, 821, 822, 824, 826, 827, 828, 829,           //  8 => 20 - 27
             830, 831, 836, 839,                               //  4 => 28 - 31
             840, 841, 842, 844, 845, 846, 848, 849,           //  8 => 32 - 39
             850, 851, 852, 854, 855, 856, 857, 858, 859,      //  9 => 40 - 48
             860, 861, 862, 863, 864, 865, 866                 //  7 => 49 - 55
*/
    };
    if (vuListRunId.size() <= uRunIdx) return kFALSE;
    uRunId = vuListRunId[uRunIdx];
  }  // if( 99999 != uRunIdx )

  if (uRunId < 692 && 0 != uRunId) return kFALSE;

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
  TString paramDir   = srcDir + "/macro/beamtime/mcbm2020/";

  // ---- Load the geometry setup ----
  // This is currently only required by the TRD
  std::string geoSetupTag = "mcbm_beam_2020_03";
  CbmSetup* geoSetup      = CbmSetup::Instance();
  geoSetup->LoadSetup(geoSetupTag.data());

  TString paramFileSts       = paramDir + "mStsPar.par";
  TObjString* parStsFileName = new TObjString(paramFileSts);
  parFileList->Add(parStsFileName);

  TString paramFileMuch       = paramDir + "mMuchPar.par";
  TObjString* parMuchFileName = new TObjString(paramFileMuch);
  parFileList->Add(parMuchFileName);


  // ---- Trd ----
  TString geoTagTrd = "";
  bool isActiveTrd =
    (geoSetup->GetGeoTag(ECbmModuleId::kTrd, geoTagTrd)) ? true : false;
  if (!isActiveTrd) {
    LOG(warning) << Form(
      "TRD - parameter loading - Trd not found in CbmSetup(%s) -> parameters "
      "can not be loaded correctly!",
      geoSetupTag.data());
  } else {
    TString paramFilesTrd(
      Form("%s/parameters/trd/trd_%s", srcDir.Data(), geoTagTrd.Data()));
    std::vector<std::string> paramFilesVecTrd;
    CbmTrdParManager::GetParFileExtensions(&paramFilesVecTrd);
    for (auto parIt : paramFilesVecTrd) {
      parFileList->Add(
        new TObjString(Form("%s.%s.par", paramFilesTrd.Data(), parIt.data())));
    }
  }

  TString paramFileTof       = paramDir + "mTofPar.par";
  TObjString* parTofFileName = new TObjString(paramFileTof);
  parFileList->Add(parTofFileName);

  TString paramFileRich = paramDir + "mRichPar.par";
  if (uRunId > 698) paramFileRich = paramDir + "mRichPar_70.par";
  TObjString* parRichFileName = new TObjString(paramFileRich);
  parFileList->Add(parRichFileName);

  TString paramFilePsd       = paramDir + "mPsdPar.par";
  TObjString* parPsdFileName = new TObjString(paramFilePsd);
  parFileList->Add(parPsdFileName);

  // --- Set debug level
  gDebug = 0;

  std::cout << std::endl;
  std::cout << ">>> unpack_check_time_mcbm_kronos: output file is " << outFile
            << std::endl;

  // ========================================================================
  // ========================================================================
  std::cout << std::endl;
  std::cout << ">>> unpack_tsa: Initialising..." << std::endl;

  CbmMcbm2018UnpackerTaskSts* unpacker_sts = new CbmMcbm2018UnpackerTaskSts();
  CbmMcbm2018UnpackerTaskMuch* unpacker_much =
    new CbmMcbm2018UnpackerTaskMuch();
  CbmMcbm2018UnpackerTaskTrdR* unpacker_trdR =
    new CbmMcbm2018UnpackerTaskTrdR();
  CbmMcbm2018UnpackerTaskTof* unpacker_tof = new CbmMcbm2018UnpackerTaskTof();
  CbmMcbm2018UnpackerTaskRich* unpacker_rich =
    new CbmMcbm2018UnpackerTaskRich();
  CbmMcbm2018UnpackerTaskPsd* unpacker_psd = new CbmMcbm2018UnpackerTaskPsd();

  /*
 * Do not generate plots by default
  unpacker_sts ->SetMonitorMode();
  unpacker_much->SetMonitorMode();
  unpacker_trdR->SetMonitorMode(); // Assume histo server present, not like other unpackers
  unpacker_tof ->SetMonitorMode();
  unpacker_rich->SetMonitorMode();
  unpacker_psd->SetMonitorMode();
*/

  unpacker_sts->SetIgnoreOverlapMs();
  unpacker_much->SetIgnoreOverlapMs();
  //  unpacker_trdR ->SetIgnoreOverlapMs(); /// Default is kTRUE
  unpacker_tof->SetIgnoreOverlapMs();
  unpacker_rich->SetIgnoreOverlapMs();
  unpacker_psd->SetIgnoreOverlapMs();

  /// Starting from first run on Tuesday 28/04/2020, STS uses bin sorter FW
  if (692 <= uRunId) unpacker_sts->SetBinningFwFlag(kTRUE);
  /// Starting from first run on Monday 04/05/2020, MUCH uses bin sorter FW
  if (811 <= uRunId) unpacker_much->SetBinningFwFlag(kTRUE);

  /// Try to cut Noise and reduce walk effect in STS and MUCH
  unpacker_sts->SetAdcCut(20);
  unpacker_much->SetAdcCut(20);

  unpacker_tof->SetSeparateArrayT0();

  unpacker_sts->SetWriteOutputFlag(kFALSE);
  unpacker_much->SetWriteOutputFlag(kFALSE);
  unpacker_trdR->SetWriteOutput(kFALSE);
  unpacker_tof->SetWriteOutputFlag(kFALSE);
  unpacker_rich->SetWriteOutputFlag(kFALSE);
  unpacker_psd->SetWriteOutputFlag(kFALSE);


  // ------------------------------ //
  // Enable Asic type for MUCH data.
  // fFlag = 0 ==> Asic type 2.0 (20) ---> December 2018 and March 2019 Data
  // fFlag = 1 ==> Asic type 2.1 (21) ---> December 2019 Data
  // This is to correct the channel fliping problem in smx 2.1 chip
  Int_t fFlag = 1;
  unpacker_much->EnableAsicType(fFlag);
  // ------------------------------ //
  /// General System offsets (= offsets between sub-systems)
  unpacker_sts->SetTimeOffsetNs(-985);   // Run 811-866
  unpacker_much->SetTimeOffsetNs(-885);  // Run 811-866
  unpacker_trdR->SetTimeOffsetNs(0);     // Run 811-866
  unpacker_tof->SetTimeOffsetNs(25);     // Run 811-866
  unpacker_rich->SetTimeOffsetNs(-310);  // Run 811-866
  unpacker_psd->SetTimeOffsetNs(-240);   // Run 811-866

  switch (uRunId) {
    case 707: {
      /// General System offsets (= offsets between sub-systems)
      //unpacker_sts ->SetTimeOffsetNs( -1750 ); // Run 707
      //unpacker_much->SetTimeOffsetNs( -1750 ); // Run 707

      /// ASIC specific offsets (= offsets inside sub-system)
      unpacker_much->SetTimeOffsetNsAsic(0, 0.0);      // Run 707, DPB 0 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(1, 0.0);      // Run 707, DPB 0 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(2, 0.0);      // Run 707, DPB 0 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(3, 0.0);      // Run 707, DPB 0 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(4, 0.0);      // Run 707, DPB 0 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(5, 0.0);      // Run 707, DPB 0 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(6, 0.0);      // Run 707, DPB 1 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(7, 0.0);      // Run 707, DPB 1 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(8, 0.0);      // Run 707, DPB 1 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(9, 0.0);      // Run 707, DPB 1 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(10, 0.0);     // Run 707, DPB 1 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(11, 0.0);     // Run 707, DPB 1 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(12, 0.0);     // Run 707, DPB 2 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(13, 0.0);     // Run 707, DPB 2 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(14, 0.0);     // Run 707, DPB 2 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(15, 0.0);     // Run 707, DPB 2 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(16, 0.0);     // Run 707, DPB 2 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(17, 0.0);     // Run 707, DPB 2 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(18, 9590.0);  // Run 707, DPB 3 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(19, 9590.0);  // Run 707, DPB 3 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(20, 9630.0);  // Run 707, DPB 3 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(21, 9590.0);  // Run 707, DPB 3 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(22, 0.0);     // Run 707, DPB 3 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(23, 0.0);     // Run 707, DPB 3 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(24, 0.0);     // Run 707, DPB 4 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(25, 0.0);     // Run 707, DPB 4 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(26, 0.0);     // Run 707, DPB 4 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(27, 0.0);     // Run 707, DPB 4 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(28, 0.0);     // Run 707, DPB 4 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(29, 0.0);     // Run 707, DPB 4 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(30, 0.0);     // Run 707, DPB 5 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(31, 0.0);     // Run 707, DPB 5 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(32, 7170.0);  // Run 707, DPB 5 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(33, 7170.0);  // Run 707, DPB 5 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(34, 0.0);     // Run 707, DPB 5 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(35, 0.0);     // Run 707, DPB 5 ASIC 5
      break;
    }  // 707
    case 750: {
      /// General System offsets (= offsets between sub-systems)
      //unpacker_sts ->SetTimeOffsetNs( -1750 ); // Run 750
      //unpacker_much->SetTimeOffsetNs( -1750 ); // Run 750

      /// ASIC specific offsets (= offsets inside sub-system)
      unpacker_much->SetTimeOffsetNsAsic(0, 0.0);      // Run 750, DPB 0 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(1, 0.0);      // Run 750, DPB 0 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(2, 0.0);      // Run 750, DPB 0 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(3, 0.0);      // Run 750, DPB 0 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(4, 0.0);      // Run 750, DPB 0 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(5, 0.0);      // Run 750, DPB 0 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(6, 0.0);      // Run 750, DPB 1 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(7, 0.0);      // Run 750, DPB 1 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(8, 0.0);      // Run 750, DPB 1 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(9, 0.0);      // Run 750, DPB 1 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(10, 0.0);     // Run 750, DPB 1 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(11, 0.0);     // Run 750, DPB 1 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(12, 0.0);     // Run 750, DPB 2 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(13, 0.0);     // Run 750, DPB 2 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(14, 0.0);     // Run 750, DPB 2 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(15, 0.0);     // Run 750, DPB 2 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(16, 0.0);     // Run 750, DPB 2 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(17, 0.0);     // Run 750, DPB 2 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(18, 6400.0);  // Run 750, DPB 3 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(19, 6400.0);  // Run 750, DPB 3 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(20, 6400.0);  // Run 750, DPB 3 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(21, 6400.0);  // Run 750, DPB 3 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(22, 0.0);     // Run 750, DPB 3 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(23, 0.0);     // Run 750, DPB 3 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(24, 0.0);     // Run 750, DPB 4 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(25, 0.0);     // Run 750, DPB 4 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(26, 0.0);     // Run 750, DPB 4 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(27, 0.0);     // Run 750, DPB 4 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(28, 0.0);     // Run 750, DPB 4 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(29, 0.0);     // Run 750, DPB 4 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(30, 0.0);     // Run 750, DPB 5 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(31, 0.0);     // Run 750, DPB 5 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(32, 3170.0);  // Run 750, DPB 5 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(33, 3170.0);  // Run 750, DPB 5 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(34, 0.0);     // Run 750, DPB 5 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(35, 0.0);     // Run 750, DPB 5 ASIC 5
      break;
    }  // 750
    case 759: {
      /// General System offsets (= offsets between sub-systems)
      //unpacker_sts ->SetTimeOffsetNs( -1759 ); // Run 759
      //unpacker_much->SetTimeOffsetNs( -1759 ); // Run 759
      unpacker_trdR->SetTimeOffsetNs(190);  // Run 759

      /// ASIC specific offsets (= offsets inside sub-system)
      unpacker_much->SetTimeOffsetNsAsic(0, 0.0);      // Run 759, DPB 0 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(1, 0.0);      // Run 759, DPB 0 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(2, 0.0);      // Run 759, DPB 0 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(3, 0.0);      // Run 759, DPB 0 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(4, 0.0);      // Run 759, DPB 0 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(5, 0.0);      // Run 759, DPB 0 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(6, 0.0);      // Run 759, DPB 1 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(7, 0.0);      // Run 759, DPB 1 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(8, 0.0);      // Run 759, DPB 1 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(9, 0.0);      // Run 759, DPB 1 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(10, 0.0);     // Run 759, DPB 1 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(11, 0.0);     // Run 759, DPB 1 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(12, 0.0);     // Run 759, DPB 2 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(13, 0.0);     // Run 759, DPB 2 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(14, 0.0);     // Run 759, DPB 2 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(15, 0.0);     // Run 759, DPB 2 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(16, 0.0);     // Run 759, DPB 2 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(17, 0.0);     // Run 759, DPB 2 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(18, 3200.0);  // Run 759, DPB 3 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(19, 3200.0);  // Run 759, DPB 3 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(20, 3200.0);  // Run 759, DPB 3 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(21, 3200.0);  // Run 759, DPB 3 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(22, 0.0);     // Run 759, DPB 3 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(23, 0.0);     // Run 759, DPB 3 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(24, 3200.0);  // Run 759, DPB 4 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(25, 3200.0);  // Run 759, DPB 4 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(26, 3200.0);  // Run 759, DPB 4 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(27, 0.0);     // Run 759, DPB 4 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(28, 0.0);     // Run 759, DPB 4 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(29, 0.0);     // Run 759, DPB 4 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(30, 0.0);     // Run 759, DPB 5 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(31, 0.0);     // Run 759, DPB 5 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(32, -30.0);   // Run 759, DPB 5 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(33, -30.0);   // Run 759, DPB 5 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(34, 0.0);     // Run 759, DPB 5 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(35, 0.0);     // Run 759, DPB 5 ASIC 5
      break;
    }  // 759
    case 760: {
      /// General System offsets (= offsets between sub-systems)
      //unpacker_sts ->SetTimeOffsetNs( -1760 ); // Run 760
      //unpacker_much->SetTimeOffsetNs( -1760 ); // Run 760
      unpacker_trdR->SetTimeOffsetNs(-75);  // Run 760

      /// ASIC specific offsets (= offsets inside sub-system)
      unpacker_much->SetTimeOffsetNsAsic(0, 0.0);      // Run 760, DPB 0 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(1, 0.0);      // Run 760, DPB 0 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(2, 0.0);      // Run 760, DPB 0 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(3, 0.0);      // Run 760, DPB 0 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(4, 0.0);      // Run 760, DPB 0 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(5, 0.0);      // Run 760, DPB 0 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(6, 0.0);      // Run 760, DPB 1 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(7, 0.0);      // Run 760, DPB 1 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(8, 0.0);      // Run 760, DPB 1 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(9, 0.0);      // Run 760, DPB 1 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(10, 0.0);     // Run 760, DPB 1 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(11, 0.0);     // Run 760, DPB 1 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(12, 0.0);     // Run 760, DPB 2 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(13, 0.0);     // Run 760, DPB 2 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(14, 0.0);     // Run 760, DPB 2 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(15, 0.0);     // Run 760, DPB 2 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(16, 0.0);     // Run 760, DPB 2 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(17, 0.0);     // Run 760, DPB 2 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(18, 0.0);     // Run 760, DPB 3 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(19, 0.0);     // Run 760, DPB 3 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(20, 0.0);     // Run 760, DPB 3 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(21, 0.0);     // Run 760, DPB 3 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(22, 0.0);     // Run 760, DPB 3 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(23, 0.0);     // Run 760, DPB 3 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(24, 3160.0);  // Run 760, DPB 4 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(25, 3160.0);  // Run 760, DPB 4 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(26, 3160.0);  // Run 760, DPB 4 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(27, 0.0);     // Run 760, DPB 4 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(28, 0.0);     // Run 760, DPB 4 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(29, 0.0);     // Run 760, DPB 4 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(30, 0.0);     // Run 760, DPB 5 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(31, 0.0);     // Run 760, DPB 5 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(32, -30.0);   // Run 760, DPB 5 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(33, -30.0);   // Run 760, DPB 5 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(34, 0.0);     // Run 760, DPB 5 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(35, 0.0);     // Run 760, DPB 5 ASIC 5
      break;
    }  // 760
    case 761: {
      /// General System offsets (= offsets between sub-systems)
      //unpacker_sts ->SetTimeOffsetNs( -1761 ); // Run 761
      //unpacker_much->SetTimeOffsetNs( -1761 ); // Run 761
      unpacker_trdR->SetTimeOffsetNs(90);  // Run 761

      /// ASIC specific offsets (= offsets inside sub-system)
      unpacker_much->SetTimeOffsetNsAsic(0, 0.0);      // Run 761, DPB 0 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(1, 0.0);      // Run 761, DPB 0 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(2, 0.0);      // Run 761, DPB 0 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(3, 0.0);      // Run 761, DPB 0 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(4, 0.0);      // Run 761, DPB 0 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(5, 0.0);      // Run 761, DPB 0 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(6, 0.0);      // Run 761, DPB 1 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(7, 0.0);      // Run 761, DPB 1 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(8, 0.0);      // Run 761, DPB 1 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(9, 0.0);      // Run 761, DPB 1 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(10, 0.0);     // Run 761, DPB 1 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(11, 0.0);     // Run 761, DPB 1 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(12, 0.0);     // Run 761, DPB 2 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(13, 0.0);     // Run 761, DPB 2 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(14, 0.0);     // Run 761, DPB 2 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(15, 0.0);     // Run 761, DPB 2 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(16, 0.0);     // Run 761, DPB 2 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(17, 0.0);     // Run 761, DPB 2 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(18, 3200.0);  // Run 761, DPB 3 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(19, 3200.0);  // Run 761, DPB 3 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(20, 3200.0);  // Run 761, DPB 3 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(21, 3200.0);  // Run 761, DPB 3 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(22, 0.0);     // Run 761, DPB 3 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(23, 0.0);     // Run 761, DPB 3 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(24, 6360.0);  // Run 761, DPB 4 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(25, 6360.0);  // Run 761, DPB 4 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(26, 6360.0);  // Run 761, DPB 4 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(27, 0.0);     // Run 761, DPB 4 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(28, 0.0);     // Run 761, DPB 4 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(29, 0.0);     // Run 761, DPB 4 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(30, 0.0);     // Run 761, DPB 5 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(31, 0.0);     // Run 761, DPB 5 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(32, 6360.0);  // Run 761, DPB 5 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(33, 6360.0);  // Run 761, DPB 5 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(34, 0.0);     // Run 761, DPB 5 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(35, 0.0);     // Run 761, DPB 5 ASIC 5
      break;
    }  // 761
    case 762: {
      /// General System offsets (= offsets between sub-systems)
      //unpacker_sts ->SetTimeOffsetNs( -1762 ); // Run 762
      //unpacker_much->SetTimeOffsetNs( -1762 ); // Run 762
      unpacker_trdR->SetTimeOffsetNs(60);  // Run 762

      /// ASIC specific offsets (= offsets inside sub-system)
      unpacker_much->SetTimeOffsetNsAsic(0, 0.0);      // Run 762, DPB 0 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(1, 0.0);      // Run 762, DPB 0 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(2, 0.0);      // Run 762, DPB 0 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(3, 0.0);      // Run 762, DPB 0 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(4, 0.0);      // Run 762, DPB 0 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(5, 0.0);      // Run 762, DPB 0 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(6, 0.0);      // Run 762, DPB 1 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(7, 0.0);      // Run 762, DPB 1 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(8, 0.0);      // Run 762, DPB 1 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(9, 0.0);      // Run 762, DPB 1 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(10, 0.0);     // Run 762, DPB 1 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(11, 0.0);     // Run 762, DPB 1 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(12, 0.0);     // Run 762, DPB 2 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(13, 0.0);     // Run 762, DPB 2 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(14, 0.0);     // Run 762, DPB 2 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(15, 0.0);     // Run 762, DPB 2 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(16, 0.0);     // Run 762, DPB 2 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(17, 0.0);     // Run 762, DPB 2 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(18, 4800.0);  // Run 762, DPB 3 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(19, 4800.0);  // Run 762, DPB 3 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(20, 4800.0);  // Run 762, DPB 3 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(21, 4800.0);  // Run 762, DPB 3 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(22, 0.0);     // Run 762, DPB 3 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(23, 0.0);     // Run 762, DPB 3 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(24, 9550.0);  // Run 762, DPB 4 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(25, 9550.0);  // Run 762, DPB 4 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(26, 9550.0);  // Run 762, DPB 4 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(27, 0.0);     // Run 762, DPB 4 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(28, 0.0);     // Run 762, DPB 4 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(29, 0.0);     // Run 762, DPB 4 ASIC 5
      unpacker_much->SetTimeOffsetNsAsic(30, 0.0);     // Run 762, DPB 5 ASIC 0
      unpacker_much->SetTimeOffsetNsAsic(31, 0.0);     // Run 762, DPB 5 ASIC 1
      unpacker_much->SetTimeOffsetNsAsic(32, -30.0);   // Run 762, DPB 5 ASIC 2
      unpacker_much->SetTimeOffsetNsAsic(33, -30.0);   // Run 762, DPB 5 ASIC 3
      unpacker_much->SetTimeOffsetNsAsic(34, 0.0);     // Run 762, DPB 5 ASIC 4
      unpacker_much->SetTimeOffsetNsAsic(35, 0.0);     // Run 762, DPB 5 ASIC 5
      break;
    }  // 762
    case 811: {
      unpacker_trdR->SetTimeOffsetNs(84.38);
      break;
    }  // 811
    case 812: {
      unpacker_trdR->SetTimeOffsetNs(165.62);
      break;
    }  // 812
    case 816: {
      unpacker_trdR->SetTimeOffsetNs(-9.38);
      break;
    }  // 816
    case 819: {
      unpacker_trdR->SetTimeOffsetNs(-140.62);
      break;
    }  // 819
    case 820: {
      unpacker_trdR->SetTimeOffsetNs(109.38);
      break;
    }  // 820
    case 821: {
      unpacker_trdR->SetTimeOffsetNs(-65.62);
      break;
    }  // 821
    case 822: {
      unpacker_trdR->SetTimeOffsetNs(59.38);
      break;
    }  // 822
    case 824: {
      unpacker_trdR->SetTimeOffsetNs(-165.62);
      break;
    }  // 824
    case 826: {
      unpacker_trdR->SetTimeOffsetNs(59.38);
      break;
    }  // 826
    case 827: {
      unpacker_trdR->SetTimeOffsetNs(-15.62);
      break;
    }  // 827
    case 828: {
      unpacker_trdR->SetTimeOffsetNs(-109.38);
      break;
    }  // 828
    case 830: {
      unpacker_trdR->SetTimeOffsetNs(15.62);
      break;
    }  // 830
    case 831: {
      //         unpacker_trdR->SetTimeOffsetNs(   70.00 );
      unpacker_trdR->SetTimeOffsetNs(-25.00);
      break;
    }  // 831
    case 836: {
      unpacker_trdR->SetTimeOffsetNs(-40.62);
      break;
    }  // 836
    default: break;
  }  // switch( uRunId )

  // --- Source task
  CbmMcbm2018Source* source = new CbmMcbm2018Source();
  source->SetWriteOutputFlag(kTRUE);  // For writing TS metadata

  TString inFile =
    Form("/lustre/cbm/users/ploizeau/mcbm2020/data/%3u_pn02_*.tsa;", uRunId);
  inFile +=
    Form("/lustre/cbm/users/ploizeau/mcbm2020/data/%3u_pn04_*.tsa;", uRunId);
  inFile +=
    Form("/lustre/cbm/users/ploizeau/mcbm2020/data/%3u_pn05_*.tsa;", uRunId);
  inFile +=
    Form("/lustre/cbm/users/ploizeau/mcbm2020/data/%3u_pn06_*.tsa;", uRunId);
  inFile +=
    Form("/lustre/cbm/users/ploizeau/mcbm2020/data/%3u_pn08_*.tsa;", uRunId);
  inFile +=
    Form("/lustre/cbm/users/ploizeau/mcbm2020/data/%3u_pn10_*.tsa;", uRunId);
  inFile +=
    Form("/lustre/cbm/users/ploizeau/mcbm2020/data/%3u_pn11_*.tsa;", uRunId);
  inFile +=
    Form("/lustre/cbm/users/ploizeau/mcbm2020/data/%3u_pn12_*.tsa;", uRunId);
  inFile +=
    Form("/lustre/cbm/users/ploizeau/mcbm2020/data/%3u_pn13_*.tsa;", uRunId);
  inFile +=
    Form("/lustre/cbm/users/ploizeau/mcbm2020/data/%3u_pn15_*.tsa", uRunId);

  source->SetFileName(inFile);

  source->AddUnpacker(unpacker_sts, 0x10, ECbmModuleId::kSts);    //STS xyter
  source->AddUnpacker(unpacker_much, 0x50, ECbmModuleId::kMuch);  //MUCH xyter
  if (isActiveTrd)
    source->AddUnpacker(unpacker_trdR, 0x40, ECbmModuleId::kTrd);  // Trd
  source->AddUnpacker(unpacker_tof, 0x60, ECbmModuleId::kTof);  //gDPB A & B & C
  source->AddUnpacker(unpacker_tof, 0x90, ECbmModuleId::kTof);  //gDPB T0 A & B
  source->AddUnpacker(unpacker_rich, 0x30, ECbmModuleId::kRich);  //RICH trb
  source->AddUnpacker(unpacker_psd, 0x80, ECbmModuleId::kPsd);    //PSD

  // --- Event header
  //  FairEventHeader* event = new FairEventHeader();
  //  event->SetRunId(uRunId);

  // --- RootFileSink
  // --- Open next outputfile after 4GB
  //  FairRootFileSink* sink = new FairRootFileSink(outFile);
  //  sink->GetOutTree()->SetMaxTreeSize(4294967295LL);

  // --- Run
  run = new FairRunOnline(source);
  /*
  run->SetSink(sink);
  run->SetEventHeader(event);
*/
  run->SetAutoFinish(kFALSE);


  CbmCheckTiming* timeChecker = new CbmCheckTiming();
  timeChecker->SetCheckInterSystemOffset(kTRUE);
  timeChecker->SetCheckTimeOrder(kTRUE);
  timeChecker->SetStsOffsetSearchRange(10000);
  timeChecker->SetMuchOffsetSearchRange(10000);
  timeChecker->SetTrdOffsetSearchRange(10000);
  timeChecker->SetTofOffsetSearchRange(10000);
  timeChecker->SetRichOffsetSearchRange(10000);
  timeChecker->SetPsdOffsetSearchRange(10000);
  timeChecker->SetT0PulserTotLimits(185, 191);
  if (0 < uRunId)
    timeChecker->SetOutFilename(
      Form("%s/HistosTimeCheck_%03u.root", outDir.Data(), uRunId));
  run->AddTask(timeChecker);

  // -----   Runtime database   ---------------------------------------------
  FairRuntimeDb* rtdb     = run->GetRuntimeDb();
  Bool_t kParameterMerged = kTRUE;
  //  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  FairParAsciiFileIo* parIn = new FairParAsciiFileIo();
  //  parOut->open(parFile.Data());
  parIn->open(parFileList, "in");
  rtdb->setFirstInput(parIn);
  //  rtdb->setOutput(parOut);

  run->Init();

  // --- Start run
  TStopwatch timer;
  timer.Start();
  std::cout << ">>> unpack_check_time_mcbm_kronos: Starting run..."
            << std::endl;
  if (0 == nrEvents) {
    run->Run(nEvents, 0);  // run until end of input file
  } else {
    run->Run(0, nrEvents);  // process  N Events
  }
  run->Finish();

  timer.Stop();

  std::cout << "Processed " << std::dec << source->GetTsCount() << " timeslices"
            << std::endl;

  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> unpack_check_time_mcbm_kronos: Macro finished successfully."
            << std::endl;
  std::cout << ">>> unpack_check_time_mcbm_kronos: Output file is " << outFile
            << std::endl;
  std::cout << ">>> unpack_check_time_mcbm_kronos: Real time " << rtime
            << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << std::endl;

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
