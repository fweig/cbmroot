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

/// FIXME: Disable clang formatting to keep easy parameters overview
/* clang-format off */
Bool_t unpack_tsa_mcbm(TString inFile       = "",
                       UInt_t uRunId        = 0,
                       UInt_t uNbTimeslices = 0,
                       TString sOutDir      = "data",
                       Int_t iSpillIndex    = -1,
                       Int_t iSpillnumber   = 3,
                       UInt_t uSpillLimType = 1)
{
  /// FIXME: Re-enable clang formatting after parameters initial values setting
  /* clang-format on */

  TString srcDir = gSystem->Getenv("VMCWORKDIR");

  // --- Specify output file name (this is just an example)
  TString runId   = TString::Format("%03u", uRunId);
  TString outFile = sOutDir + "/unp_mcbm_" + runId;
  TString parFile = sOutDir + "/unp_mcbm_params_" + runId;
  if (0 <= iSpillIndex) {
    outFile += TString::Format("_%02i", iSpillIndex);
    parFile += TString::Format("_%02i", iSpillIndex);
  }  // if( 0 <= iSpillIndex )
  outFile += ".root";
  parFile += ".root";

  /*
  std::cout << inFile << std::endl << sOutDir << std::endl;
  std::cout << parFile << std::endl
            << outFile << std::endl;
  std::cout << uRunId << " " << uNbTimeslices << std::endl;

  return kTRUE;
  */

  // --- Set log output levels
  FairLogger::GetLogger();
  gLogger->SetLogScreenLevel("INFO");
  //gLogger->SetLogScreenLevel("DEBUG4");
  gLogger->SetLogVerbosityLevel("MEDIUM");
  //gLogger->SetLogVerbosityLevel("LOW");

  // --- Define parameter files
  TList* parFileList = new TList();
  TString paramDir   = srcDir + "/macro/beamtime/mcbm2020/";

  // --- Load the geometry setup ----
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
    // Add timeshift calibration, currently only available for run 831 others to come
    if (uRunId == 831)
      parFileList->Add(new TObjString(Form(
        "%s/parameters/trd/mcbm2020_special/CbmMcbm2020TrdTshiftPar_run%d.par",
        srcDir.Data(),
        uRunId)));
  }

  TString paramFileTof = paramDir + "mTofPar.par";
  if (uRunId >= 708 && uRunId < 754)
    paramFileTof = paramDir + "mTofPar_2Stack.par";
  else if (uRunId >= 754)
    paramFileTof = paramDir + "mTofPar_3Stack.par";

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
  std::cout << ">>> unpack_tsa: output file is " << outFile << std::endl;

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

  /// FIXME: Disable clang formatting for parameters tuning for now
  /* clang-format off */
  //  unpacker_sts ->SetAdcCut( 3 );
  unpacker_sts ->MaskNoisyChannel(1,768 ,  true );
  unpacker_sts ->MaskNoisyChannel(1,894 ,  true );
  unpacker_sts ->MaskNoisyChannel(1,896 ,  true );

  unpacker_sts ->MaskNoisyChannel(2,930 ,  true );
  unpacker_sts ->MaskNoisyChannel(2,926 ,  true );
  unpacker_sts ->MaskNoisyChannel(2,892 ,  true );

  unpacker_sts ->MaskNoisyChannel(3,770 ,  true );

  unpacker_tof->SetSeparateArrayT0();

  // ------------------------------ //
  // Enable Asic type for MUCH data.
  // fFlag = 0 ==> Asic type 2.0 (20) ---> December 2018 and March 2019 Data
  // fFlag = 1 ==> Asic type 2.1 (21) ---> December 2019 Data
  // This is to correct the channel fliping problem in smx 2.1 chip
  Int_t fFlag = 1;
  unpacker_much->EnableAsicType(fFlag);
  // ------------------------------ //
  /// General System offsets (= offsets between sub-systems)
  unpacker_sts->SetTimeOffsetNs(-936);   // Run 811-866
  unpacker_much->SetTimeOffsetNs(-885);  // Run 811-866
  unpacker_trdR->SetTimeOffsetNs(0);     // Run 811-866
  unpacker_tof->SetTimeOffsetNs(30);     // Run 811-866
  unpacker_rich->SetTimeOffsetNs(-310);  // Run 811-866
  unpacker_psd->SetTimeOffsetNs(-225);   // Run 811-866

  // ----------- ASIC by ASIC STS ----------------
  // the first 8 Unused
  unpacker_sts ->SetTimeOffsetNsAsic(  0,       0.0  ); // Unused
  unpacker_sts ->SetTimeOffsetNsAsic(  1,       0.0  ); // Unused
  unpacker_sts ->SetTimeOffsetNsAsic(  2,       0.0  ); // Unused
  unpacker_sts ->SetTimeOffsetNsAsic(  3,       0.0  ); // Unused
  unpacker_sts ->SetTimeOffsetNsAsic(  4,       0.0  ); // Unused
  unpacker_sts ->SetTimeOffsetNsAsic(  5,       0.0  ); // Unused
  unpacker_sts ->SetTimeOffsetNsAsic(  6,       0.0  ); // Unused
  unpacker_sts ->SetTimeOffsetNsAsic(  7,       0.0  ); // Unused
  //
  unpacker_sts ->SetTimeOffsetNsAsic(8,  -0.360078  );
  unpacker_sts ->SetTimeOffsetNsAsic(9,  2.73976    );
  unpacker_sts ->SetTimeOffsetNsAsic(10,  -0.507079  );
  unpacker_sts ->SetTimeOffsetNsAsic(11,  1.74695    );
  unpacker_sts ->SetTimeOffsetNsAsic(12,  -0.909864  );
  unpacker_sts ->SetTimeOffsetNsAsic(13,  -0.255514  );
  unpacker_sts ->SetTimeOffsetNsAsic(14,  1.44034    );
  unpacker_sts ->SetTimeOffsetNsAsic(15,  2.64009    );
  // this side: revert order 23->16
  unpacker_sts ->SetTimeOffsetNsAsic(23,  -0.442762  );
  unpacker_sts ->SetTimeOffsetNsAsic(22,  1.76543    );
  unpacker_sts ->SetTimeOffsetNsAsic(21, -0.94728   );
  unpacker_sts ->SetTimeOffsetNsAsic(20, -2.18516   );
  unpacker_sts ->SetTimeOffsetNsAsic(19, -0.68254   );
  unpacker_sts ->SetTimeOffsetNsAsic(18, -2.32241   );
  unpacker_sts ->SetTimeOffsetNsAsic(17, -1.53483   );
  unpacker_sts ->SetTimeOffsetNsAsic(16, -2.12455   );
  //
  unpacker_sts ->SetTimeOffsetNsAsic(24, -0.41084   );
  unpacker_sts ->SetTimeOffsetNsAsic(25, 0.230455   );
  unpacker_sts ->SetTimeOffsetNsAsic(26, -0.206921  );
  unpacker_sts ->SetTimeOffsetNsAsic(27, 0.0913657  );
  unpacker_sts ->SetTimeOffsetNsAsic(28, -0.17252   );
  unpacker_sts ->SetTimeOffsetNsAsic(29, -0.32990   );
  unpacker_sts ->SetTimeOffsetNsAsic(30, 1.43535    );
  unpacker_sts ->SetTimeOffsetNsAsic(31, -0.155741  );
  // this side: revert order 39->32
  unpacker_sts ->SetTimeOffsetNsAsic(39, 1.53865    );
  unpacker_sts ->SetTimeOffsetNsAsic(38, 3.6318     );
  unpacker_sts ->SetTimeOffsetNsAsic(37, 1.3153     );
  unpacker_sts ->SetTimeOffsetNsAsic(36, -1.90278   );
  unpacker_sts ->SetTimeOffsetNsAsic(35, 2.00051    );
  unpacker_sts ->SetTimeOffsetNsAsic(34, -2.85656   );
  unpacker_sts ->SetTimeOffsetNsAsic(33, 1.28834    );
  unpacker_sts ->SetTimeOffsetNsAsic(32, 0.657113   );

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

      std::cout << "MUCH: Feb by feb time offset correction......" << std::endl;
      UInt_t uRun, uNx;
      Double_t offset;
      ifstream infile_off(paramDir + "/parameters/time_offset_much.txt");
      if (!infile_off) {
        std::cout << "can not open time offset MUCH parameter List" << std::endl;
        return kFALSE;
      } // if (!infile_off)
      while (!infile_off.eof())  {
        infile_off >> uRun >> uNx >> offset;
        if(uRun != 831) continue;
        unpacker_much->SetTimeOffsetNsAsic(uNx, offset);
      } // while (!infile_off.eof())
      infile_off.close();
      std::cout << "masking noisy channels......" << std::endl;
      UInt_t uChan = 0;
      ifstream infile_noise(paramDir + "parameters/much_noisy_channel_list.txt");
      if (!infile_noise) {
        std::cout << "can not open MUCH noisy channel List" << std::endl;
        return kFALSE;
      } // if (!infile_noise)
      while (!infile_noise.eof())  {
        infile_noise >> uRun >> uNx >> uChan;
        if(uRun != 831) continue;
        unpacker_much->MaskNoisyChannel(uNx, uChan, kTRUE );
      } // while (!infile_noise.eof())
      infile_noise.close();
      break;
    }  // 831
    case 836: {
      unpacker_trdR->SetTimeOffsetNs(-40.62);
      break;
    }  // 836
    default: break;
  }  // switch( uRunId )
  /// FIXME: Re-enable clang formatting after parameters tuning
  /* clang-format on */

  // --- Source task
  CbmMcbm2018Source* source = new CbmMcbm2018Source();
  source->SetWriteOutputFlag(kTRUE);  // For writing TS metadata

  source->SetFileName(inFile);

  source->AddUnpacker(unpacker_sts, 0x10, ECbmModuleId::kSts);    // STS xyter
  source->AddUnpacker(unpacker_much, 0x50, ECbmModuleId::kMuch);  // MUCH xyter
  if (isActiveTrd)
    source->AddUnpacker(unpacker_trdR, 0x40, ECbmModuleId::kTrd);  // Trd
  source->AddUnpacker(unpacker_tof, 0x60, ECbmModuleId::kTof);     // gDPB TOF
  source->AddUnpacker(unpacker_tof, 0x90, ECbmModuleId::kTof);     // gDPB T0
  source->AddUnpacker(unpacker_rich, 0x30, ECbmModuleId::kRich);   // RICH trb
  source->AddUnpacker(unpacker_psd, 0x80, ECbmModuleId::kPsd);     // PSD

  source->UnpackSelectSpills(iSpillIndex * iSpillnumber, iSpillIndex * iSpillnumber + iSpillnumber - 1, uSpillLimType);
  /// Disable clang formatting around vectors initialization by lists
  /* clang-format off */
  switch (uRunId) {
    case 831: {
      source->LoadTsListSpillBreakBegin(std::vector<ULong64_t>(
        {    1,   933,  1941,  2949,  4349,  5357,  6365,  7373,  8773,  9781,
         10789, 11801, 13197, 14209, 15217, 16225, 17625, 18633, 19645, 20653,
         22053, 23061, 24069, 25077, 26473, 27481, 28493, 29501, 30897, 31905,
         32913, 33921, 35321, 36329, 37337, 38345, 39745, 40753, 41761, 42769,
         44169, 45177, 46185, 47193, 48593}));
      source->LoadTsListSpillBreakMiddle(std::vector<ULong64_t>(
        {  111,  1081,  2087,  3299,  4495,  5503,  6513,  7721,  8921,  9927,
         10935, 12149, 13347, 14355, 15363, 16573, 17773, 18781, 19791, 21001,
         22199, 23209, 24215, 25423, 26619, 27629, 28637, 29847, 31045, 32051,
         33059, 34275, 35469, 36475, 37483, 38693, 39889, 40899, 41907, 43117,
         44315, 45323, 46333, 47543, 48689}));
      source->LoadTsListSpillBreakEnd(std::vector<ULong64_t>(
        {  221, 1229,  2233,  3649,  4641,  5649,  6661,  8069,  9069,  10073,
         11081, 12497, 13497, 14501, 15509, 16921, 17921, 18929, 19937, 21349,
         22345, 23357, 24361, 25769, 26765, 27777, 28781, 30193, 31193, 32197,
         33205, 34629, 35617, 36621, 37629, 39041, 40033, 41045, 42053, 43465,
         44461, 45469, 46481, 47893, 48786}));
      break;
    }  // case 831
  }    // switch (uRunId)
  /// Re-enable clang formatting after vectors initialization by lists
  /* clang-format on */

  // --- Event header
  FairEventHeader* event = new FairEventHeader();
  event->SetRunId(uRunId);

  // --- RootFileSink
  FairRootFileSink* sink = new FairRootFileSink(outFile);

  // --- Run
  run = new FairRunOnline(source);
  run->SetSink(sink);
  run->SetEventHeader(event);
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
  std::cout << ">>> unpack_tsa_mcbm: Starting run..." << std::endl;
  if (0 == uNbTimeslices) {
    run->Run(-1, 0);  // run until end of input file
  }
  else {
    run->Run(0, uNbTimeslices);  // process  N Timeslices
  }
  run->Finish();

  timer.Stop();

  std::cout << "Processed " << std::dec << source->GetTsCount() << " timeslices"
            << std::endl;

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

  return kTRUE;
}
