/* Copyright (C) 2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau, Adrian Weber [committer] */

// --------------------------------------------------------------------------
//
// Macro for reconstruction of mcbm data (2021)
// Combined reconstruction (cluster + hit finder) for different subsystems.
//
// --------------------------------------------------------------------------

#include <math.h>
#include <stdio.h>
#include <string.h>

/// FIXME: Disable clang formatting to keep easy parameters overview
/* clang-format off */
Bool_t mcbm_event_reco_L1(UInt_t uRunId                   = 2391,
			  Int_t nTimeslices               = 10,
			  TString sInpDir                 = "./data/",
			  TString sOutDir                 = "./data/",
			  Int_t iUnpFileIndex             = -1,
			  Bool_t bMVD                     = kFALSE,
			  Bool_t bSTS                     = kTRUE,
			  Bool_t bTRD                     = kTRUE,
			  Bool_t bTRD2d                   = kTRUE,
			  Bool_t bRICH                    = kTRUE,
			  Bool_t bMUCH                    = kFALSE,
			  Bool_t bTOF                     = kTRUE,
			  Bool_t bTOFtr                   = kTRUE,
			  Bool_t bPSD                     = kFALSE,
			  Bool_t bAli                     = kTRUE,
			  Bool_t bEvB                     = kTRUE,
			  Bool_t bL1                      = kFALSE,
			  Bool_t bQA                      = kFALSE,
			  TString sInpFile                = ""
			  )
{
  /// FIXME: Re-enable clang formatting after parameters initial values setting
  /* clang-format on */

  // --- Logger settings ----------------------------------------------------
  TString logLevel     = "INFO";     //"INFO";
  TString logVerbosity = "VERYLOW";  //"VERYLOW";
  // ------------------------------------------------------------------------


  // -----   Environment   --------------------------------------------------
  TString myName = "mcbm_event_reco";              // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------


  // -----   In- and output file names   ------------------------------------
  /// Standardized RUN ID
  TString sRunId = TString::Format("%04u", uRunId);

  /// Initial pattern
  TString inFile  = sInpFile;  // + "/" + sRunId + ".digi";
  TString cFileId = sRunId;

  //TString parFileIn  = sInpDir + "/unp_mcbm_params_" + sRunId;
  TString parFileOut = sOutDir + "/reco_event_mcbm_test_params_" + sRunId;
  TString outFile    = sOutDir + "/reco_event_mcbm_test" + sRunId;

  // Your folder with the Tof Calibration files;
  TString TofFileFolder = "";
  // TString TofFileFolder = "/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2021/";


  /// Add index of splitting at unpacking level if needed
  if (0 <= iUnpFileIndex) {
    inFile += TString::Format("_%02u", iUnpFileIndex);
    // the input par file is not split during unpacking!
    parFileOut += TString::Format("_%02u", iUnpFileIndex);
    outFile += TString::Format("_%02u", iUnpFileIndex);
  }  // if ( 0 <= uUnpFileIndex )
  /// Add ROOT file suffix
  inFile += ".root";
  //  parFileIn += ".root";
  parFileOut += ".root";
  outFile += ".root";

  if ("" != sInpFile) inFile = sInpFile;
  // ---------------------------------------------


  // -----   TOF defaults ------------------------
  Int_t calMode      = 93;
  Int_t calSel       = 1;
  Int_t calSm        = 2;
  Int_t RefSel       = 11;
  Double_t dDeadtime = 50.;
  Int_t iSel2        = -1;

  // Tracking
  Int_t iSel           = 22002;
  Int_t iTrackingSetup = 1;  // 2 for checking without beam counter;
  Int_t iGenCor        = 1;
  Double_t dScalFac    = 2.5;
  Double_t dChi2Lim2   = 4.;
  Bool_t bUseSigCalib  = kFALSE;
  Int_t iCalOpt        = 110;  // 0=do not call CbmTofCalibrator
  Int_t iTrkPar        = 0;    // 4 for check without beam counter
  // ------------------------------------------------------------------------

  // -----   TOF Calibration Settings ---------------------------------------
  TString cCalId = "490.100.5.0";
  if (uRunId >= 759) cCalId = "759.100.4.0";
  if (uRunId >= 812) cCalId = "831.100.4.0";
  if (uRunId >= 1588) cCalId = "1588.50.6.0";
  if (uRunId >= 2160) cCalId = "2160.50.4.0";
  if (uRunId >= 2352) cCalId = "2391.5.000";
  Int_t iCalSet = 30040500;  // calibration settings
  if (uRunId >= 759) iCalSet = 10020500;
  if (uRunId >= 812) iCalSet = 10020500;
  if (uRunId >= 1588) iCalSet = 12002002;
  if (uRunId >= 2160) iCalSet = 700900500;
  if (uRunId >= 2352) iCalSet = 22002500;

  Double_t Tint           = 100.;  // coincidence time interval
  Int_t iTrackMode        = 2;     // 2 for TofTracker
  const Int_t iTofCluMode = 1;
  // ------------------------------------------------------------------------

  // --- Load the geometry setup ----
  // This is currently only required by the TRD (parameters)
  TString geoSetupTag = "mcbm_beam_2021_07_surveyed";
  if (2060 <= uRunId) {
    /// Setup changed multiple times between the 2022 carbon and uranium runs
    if (uRunId <= 2065) {
      /// Carbon runs: 2060 - 2065 = 10/03/2022
      geoSetupTag = "mcbm_beam_2022_03_09_carbon";
    }
    else if (2150 <= uRunId && uRunId <= 2160) {
      /// Iron runs: 2150 - 2160 = 24-25/03/2022
      geoSetupTag = "mcbm_beam_2022_03_22_iron";
    }
    else if (2176 <= uRunId && uRunId <= 2310) {
      /// Uranium runs: 2176 - 2310 = 30/03/2022 - 01/04/2022
      geoSetupTag = "mcbm_beam_2022_03_28_uranium";
    }
    else if (2352 <= uRunId) {
      /// Uranium runs: 2176 - 2310 = 30/03/2022 - 01/04/2022
      geoSetupTag = "mcbm_beam_2022_05_23_nickel";
    }
  }

  TString geoFile    = sInpDir + "/" + geoSetupTag + ".geo.root";
  CbmSetup* geoSetup = CbmSetup::Instance();
  geoSetup->LoadSetup(geoSetupTag);

  // You can modify the pre-defined setup by using
  if (bMVD) geoSetup->SetActive(ECbmModuleId::kMvd, kTRUE);
  if (bSTS) geoSetup->SetActive(ECbmModuleId::kSts, kTRUE);
  if (bMUCH) geoSetup->SetActive(ECbmModuleId::kMuch, kTRUE);
  if (bRICH) geoSetup->SetActive(ECbmModuleId::kRich, kTRUE);
  if (bTRD) geoSetup->SetActive(ECbmModuleId::kTrd, kTRUE);
  if (bTRD2d) geoSetup->SetActive(ECbmModuleId::kTrd2d, kTRUE);
  if (bTOF) geoSetup->SetActive(ECbmModuleId::kTof, kTRUE);
  if (bPSD) geoSetup->SetActive(ECbmModuleId::kPsd, kTRUE);

  //-----  Load Parameters --------------------------------------------------
  TList* parFileList = new TList();
  TofFileFolder      = Form("%s/%s", TofFileFolder.Data(), cCalId.Data());

  // ----- TRD digitisation parameters -------------------------------------
  TString geoTagTrd;
  if (geoSetup->IsActive(ECbmModuleId::kTrd)) {
    if (geoSetup->GetGeoTag(ECbmModuleId::kTrd, geoTagTrd)) {
      TString paramFilesTrd(Form("%s/parameters/trd/trd_%s", srcDir.Data(), geoTagTrd.Data()));
      std::vector<TString> paramFilesVecTrd = {"asic", "digi", "gas", "gain"};
      for (auto parIt : paramFilesVecTrd) {
        parFileList->Add(new TObjString(Form("%s.%s.par", paramFilesTrd.Data(), parIt.Data())));
      }
    }
    for (auto parFileVecIt : *parFileList) {
      LOG(debug) << Form("TrdParams - %s - added to parameter file list\n", parFileVecIt->GetName());
    }
  }
  // ----- TOF digitisation parameters -------------------------------------
  TString geoTag;
  if (geoSetup->IsActive(ECbmModuleId::kTof)) {
    geoSetup->GetGeoTag(ECbmModuleId::kTof, geoTag);
    TObjString* tofBdfFile = new TObjString(srcDir + "/parameters/tof/tof_" + geoTag + ".digibdf.par");
    parFileList->Add(tofBdfFile);
    std::cout << "-I- " << myName << ": Using parameter file " << tofBdfFile->GetString() << std::endl;

    // TFile* fgeo         = new TFile(geoFile);
    // TGeoManager* geoMan = (TGeoManager*) fgeo->Get("FAIRGeom");
    // if (NULL == geoMan) {
    //   cout << "<E> FAIRGeom not found in geoFile " << geoFile.Data() << endl;
    //   return 1;
    // }
  }
  // ------------------------------------------------------------------------

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // -----   FairRunAna   ---------------------------------------------------
  FairRunAna* run             = new FairRunAna();
  FairFileSource* inputSource = new FairFileSource(inFile);
  run->SetSource(inputSource);

  FairRootFileSink* outputSink = new FairRootFileSink(outFile);
  run->SetSink(outputSink);
  run->SetGeomFile(geoFile);

  // Define output file for FairMonitor histograms
  TString monitorFile {outFile};
  monitorFile.ReplaceAll("reco", "reco.monitor");
  FairMonitor::GetMonitor()->EnableMonitor(kTRUE, monitorFile);
  // ------------------------------------------------------------------------


  // -----   Logger settings   ----------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel(logLevel.Data());
  FairLogger::GetLogger()->SetLogVerbosityLevel(logVerbosity.Data());
  //FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");
  // ------------------------------------------------------------------------


  // =========================================================================
  // ===                   Alignment Correction                            ===
  // =========================================================================
  // (Fairsoft Apr21p2 or newer is needed)
  if (bAli) {

    TString alignmentMatrixFileName = srcDir + "/parameters/mcbm/AlignmentMatrices_" + geoSetupTag + ".root";
    if (alignmentMatrixFileName.Length() != 0) {
      std::cout << "-I- " << myName << ": Applying alignment for file " << alignmentMatrixFileName << std::endl;

      // Define the basic structure which needs to be filled with information
      // This structure is stored in the output file and later passed to the
      // FairRoot framework to do the (miss)alignment
      std::map<std::string, TGeoHMatrix>* matrices {nullptr};

      // read matrices from disk
      LOG(info) << "Filename: " << alignmentMatrixFileName;
      TFile* misalignmentMatrixRootfile = new TFile(alignmentMatrixFileName, "READ");
      if (misalignmentMatrixRootfile->IsOpen()) {
        gDirectory->GetObject("MisalignMatrices", matrices);
        misalignmentMatrixRootfile->Close();
      }
      else {
        LOG(error) << "Could not open file " << alignmentMatrixFileName << "\n Exiting";
        exit(1);
      }

      if (matrices) { run->AddAlignmentMatrices(*matrices); }
      else {
        LOG(error) << "Alignment required but no matrices found."
                   << "\n Exiting";
        exit(1);
      }
    }
  }
  // ------------------------------------------------------------------------


  // --------------------event builder---------------------------------------
  // -----   EventBuilder Settings----------------
  // mCbm track trigger Tof, T0 & STS (case 4 of mcbm_unp_event.C)
  const Int_t eb_TriggerMinNumberT0 {1};
  const Int_t eb_TriggerMaxNumberT0 {2};
  const Int_t eb_TriggerMinNumberSts {2};
  const Int_t eb_TriggerMinNumberStsLayers {1};
  const Int_t eb_TriggerMinNumberMuch {0};
  const Int_t eb_TriggerMinNumberTof {8};
  const Int_t eb_TriggerMinNumberTofLayers {4};
  const Int_t eb_TriggerMinNumberRich {0};

  const Int_t eb_TrigWinMinT0 {-50};
  const Int_t eb_TrigWinMaxT0 {50};
  const Int_t eb_TrigWinMinSts {-60};
  const Int_t eb_TrigWinMaxSts {60};
  const Int_t eb_TrigWinMinTrd1d {-300};
  const Int_t eb_TrigWinMaxTrd1d {300};
  const Int_t eb_TrigWinMinTrd2d {-200};
  const Int_t eb_TrigWinMaxTrd2d {200};
  const Int_t eb_TrigWinMinTof {-20};
  const Int_t eb_TrigWinMaxTof {60};
  const Int_t eb_TrigWinMinRich {-60};
  const Int_t eb_TrigWinMaxRich {60};

  if (bEvB) {
    CbmTaskBuildRawEvents* evBuildRaw = new CbmTaskBuildRawEvents();

    //Choose between NoOverlap, MergeOverlap, AllowOverlap
    evBuildRaw->SetEventOverlapMode(EOverlapModeRaw::AllowOverlap);

    // Remove detectors where digis not found
    evBuildRaw->RemoveDetector(kRawEventBuilderDetRich);
    if (!bRICH || !geoSetup->IsActive(ECbmModuleId::kRich)) evBuildRaw->RemoveDetector(kRawEventBuilderDetRich);
    if (!bMUCH || !geoSetup->IsActive(ECbmModuleId::kMuch)) evBuildRaw->RemoveDetector(kRawEventBuilderDetMuch);
    if (!bPSD || !geoSetup->IsActive(ECbmModuleId::kPsd)) evBuildRaw->RemoveDetector(kRawEventBuilderDetPsd);
    if ((!bTRD && !bTRD2d) || !geoSetup->IsActive(ECbmModuleId::kTrd))
      evBuildRaw->RemoveDetector(kRawEventBuilderDetTrd);
    if (!bSTS || !geoSetup->IsActive(ECbmModuleId::kSts)) evBuildRaw->RemoveDetector(kRawEventBuilderDetSts);
    if (!bTOF || !geoSetup->IsActive(ECbmModuleId::kTof)) evBuildRaw->RemoveDetector(kRawEventBuilderDetTof);

    // Set T0 as reference detector
    evBuildRaw->SetReferenceDetector(kRawEventBuilderDetT0);

    // Use sliding window seed builder with STS
    //evBuildRaw->SetReferenceDetector(kRawEventBuilderDetUndef);
    //evBuildRaw->AddSeedTimeFillerToList(kRawEventBuilderDetTof);
    //evBuildRaw->SetSlidingWindowSeedFinder(10, 10, 50);
    //evBuildRaw->SetSeedFinderQa(true);  // optional QA information for seed finder
    //evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kSts, 1000);
    //evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kSts, -1);
    //evBuildRaw->SetTriggerWindow(ECbmModuleId::kSts, -500, 500);

    // void SetTsParameters(double TsStartTime, double TsLength, double TsOverLength): TsStartTime=0, TsLength=256ms in 2021, TsOverLength=TS overlap, not used in mCBM2021
    evBuildRaw->SetTsParameters(0.0, 1.28e8, 1.12928e8);

    if (geoSetup->IsActive(ECbmModuleId::kTof)) {
      evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kTof, eb_TriggerMinNumberTof);
      evBuildRaw->SetTriggerMinLayersNumber(ECbmModuleId::kTof, eb_TriggerMinNumberTofLayers);
      evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kTof, -1);
    }

    evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kT0, eb_TriggerMinNumberT0);
    evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kT0, eb_TriggerMaxNumberT0);

    if (geoSetup->IsActive(ECbmModuleId::kSts)) {
      evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kSts, eb_TriggerMinNumberSts);
      evBuildRaw->SetTriggerMinLayersNumber(ECbmModuleId::kSts, eb_TriggerMinNumberStsLayers);
      evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kSts, -1);
    }
    if (geoSetup->IsActive(ECbmModuleId::kRich)) {
      evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kRich, eb_TriggerMinNumberRich);
      evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kRich, -1);
    }

    evBuildRaw->SetTriggerWindow(ECbmModuleId::kT0, eb_TrigWinMinT0, eb_TrigWinMaxT0);
    if (geoSetup->IsActive(ECbmModuleId::kTof))
      evBuildRaw->SetTriggerWindow(ECbmModuleId::kTof, eb_TrigWinMinTof, eb_TrigWinMaxTof);
    if (geoSetup->IsActive(ECbmModuleId::kSts))
      evBuildRaw->SetTriggerWindow(ECbmModuleId::kSts, eb_TrigWinMinSts, eb_TrigWinMaxSts);
    if (geoSetup->IsActive(ECbmModuleId::kTrd))
      evBuildRaw->SetTriggerWindow(ECbmModuleId::kTrd, eb_TrigWinMinTrd1d, eb_TrigWinMaxTrd1d);
    if (geoSetup->IsActive(ECbmModuleId::kTrd))
      evBuildRaw->SetTriggerWindow(ECbmModuleId::kTrd2d, eb_TrigWinMinTrd2d, eb_TrigWinMaxTrd2d);
    if (geoSetup->IsActive(ECbmModuleId::kRich))
      evBuildRaw->SetTriggerWindow(ECbmModuleId::kRich, eb_TrigWinMinRich, eb_TrigWinMaxRich);

    run->AddTask(evBuildRaw);
  }
  // ------------------------------------------------------------------------


  // -----   Reconstruction tasks   -----------------------------------------


  // =========================================================================
  // ===                 local STS Reconstruction                          ===
  // =========================================================================

  if (bSTS && geoSetup->IsActive(ECbmModuleId::kSts)) {
    CbmRecoSts* recoSts = new CbmRecoSts();
    recoSts->SetMode(kCbmRecoEvent);

    recoSts->SetTimeCutDigisAbs(20.0);     // cluster finder: time cut in ns
    recoSts->SetTimeCutClustersAbs(20.0);  // hit finder: time cut in ns

    // Sensor params
    CbmStsParSensor sensor6cm(CbmStsSensorClass::kDssdStereo);
    sensor6cm.SetPar(0, 6.2092);  // Extension in x
    sensor6cm.SetPar(1, 6.2);     // Extension in y
    sensor6cm.SetPar(2, 0.03);    // Extension in z
    sensor6cm.SetPar(3, 5.9692);  // Active size in y
    sensor6cm.SetPar(4, 1024.);   // Number of strips front side
    sensor6cm.SetPar(5, 1024.);   // Number of strips back side
    sensor6cm.SetPar(6, 0.0058);  // Strip pitch front side
    sensor6cm.SetPar(7, 0.0058);  // Strip pitch back side
    sensor6cm.SetPar(8, 0.0);     // Stereo angle front side
    sensor6cm.SetPar(9, 7.5);     // Stereo angle back side

    CbmStsParSensor sensor12cm(sensor6cm);  // copy all parameters, change then only the y size
    sensor12cm.SetPar(1, 12.4);             // Extension in y
    sensor12cm.SetPar(3, 12.1692);          // Active size in y

    // --- Addresses for sensors
    // --- They are defined in each station as sensor 1, module 1, halfladderD (2), ladder 1
    //  Int_t GetAddress(UInt_t unit = 0, UInt_t ladder = 0, UInt_t halfladder = 0, UInt_t module = 0, UInt_t sensor = 0,
    //                   UInt_t side = 0, UInt_t version = kCurrentVersion);

    Int_t stsAddress01 = CbmStsAddress::GetAddress(0, 0, 1, 0, 0, 0);  // U0 L0 M0  6 cm
    Int_t stsAddress02 = CbmStsAddress::GetAddress(0, 0, 1, 1, 0, 0);  // U0 L0 M1  6 cm
    Int_t stsAddress03 = CbmStsAddress::GetAddress(0, 1, 1, 0, 0, 0);  // U0 L1 M0  6 cm
    Int_t stsAddress04 = CbmStsAddress::GetAddress(0, 1, 1, 1, 0, 0);  // U0 L1 M1  6 cm
    Int_t stsAddress05 = CbmStsAddress::GetAddress(1, 0, 1, 0, 0, 0);  // U1 L0 M0  6 cm
    Int_t stsAddress06 = CbmStsAddress::GetAddress(1, 0, 1, 1, 0, 0);  // U1 L0 M1 12 cm
    Int_t stsAddress07 = CbmStsAddress::GetAddress(1, 1, 1, 0, 0, 0);  // U1 L1 M0  6 cm
    Int_t stsAddress08 = CbmStsAddress::GetAddress(1, 1, 1, 1, 0, 0);  // U1 L1 M1 12 cm
    Int_t stsAddress09 = CbmStsAddress::GetAddress(1, 2, 1, 0, 0, 0);  // U1 L2 M0  6 cm
    Int_t stsAddress10 = CbmStsAddress::GetAddress(1, 2, 1, 1, 0, 0);  // U1 L2 M1  6 cm
    Int_t stsAddress11 = CbmStsAddress::GetAddress(1, 2, 1, 2, 0, 0);  // U1 L2 M2  6 cm


    std::cout << "STS address01 " << std::dec << stsAddress01 << " " << std::hex << stsAddress01 << std::endl;
    std::cout << "STS address02 " << std::dec << stsAddress02 << " " << std::hex << stsAddress02 << std::endl;
    std::cout << "STS address03 " << std::dec << stsAddress03 << " " << std::hex << stsAddress03 << std::endl;
    std::cout << "STS address04 " << std::dec << stsAddress04 << " " << std::hex << stsAddress04 << std::endl;
    std::cout << "STS address05 " << std::dec << stsAddress05 << " " << std::hex << stsAddress05 << std::endl;
    std::cout << "STS address06 " << std::dec << stsAddress06 << " " << std::hex << stsAddress06 << std::endl;
    std::cout << "STS address07 " << std::dec << stsAddress07 << " " << std::hex << stsAddress07 << std::endl;
    std::cout << "STS address08 " << std::dec << stsAddress08 << " " << std::hex << stsAddress08 << std::endl;
    std::cout << "STS address09 " << std::dec << stsAddress09 << " " << std::hex << stsAddress09 << std::endl;
    std::cout << "STS address10 " << std::dec << stsAddress10 << " " << std::hex << stsAddress10 << std::endl;
    std::cout << "STS address11 " << std::dec << stsAddress11 << " " << std::hex << stsAddress11 << std::endl;

    // --- Now we can define the sensor parameter set and tell recoSts to use it
    auto sensorParSet = new CbmStsParSetSensor("CbmStsParSetSensor", "STS sensor parameters"
                                                                     "mcbm2021");
    sensorParSet->SetParSensor(stsAddress01, sensor6cm);
    sensorParSet->SetParSensor(stsAddress02, sensor6cm);
    sensorParSet->SetParSensor(stsAddress03, sensor6cm);
    sensorParSet->SetParSensor(stsAddress04, sensor6cm);
    sensorParSet->SetParSensor(stsAddress05, sensor6cm);
    sensorParSet->SetParSensor(stsAddress06, sensor12cm);
    sensorParSet->SetParSensor(stsAddress07, sensor6cm);
    sensorParSet->SetParSensor(stsAddress08, sensor12cm);
    sensorParSet->SetParSensor(stsAddress09, sensor6cm);
    sensorParSet->SetParSensor(stsAddress10, sensor6cm);
    sensorParSet->SetParSensor(stsAddress11, sensor6cm);

    recoSts->UseSensorParSet(sensorParSet);

    // ASIC params: #ADC channels, dyn. range, threshold, time resol., dead time,
    // noise RMS, zero-threshold crossing rate
    auto parAsic = new CbmStsParAsic(128, 31, 75000., 3000., 5., 800., 1000., 3.9789e-3);

    // Module params: number of channels, number of channels per ASIC
    auto parMod = new CbmStsParModule(2048, 128);
    parMod->SetAllAsics(*parAsic);
    recoSts->UseModulePar(parMod);

    // Sensor conditions: full depletion voltage, bias voltage, temperature,
    // coupling capacitance, inter-strip capacitance
    auto sensorCond = new CbmStsParSensorCond(70., 140., 268., 17.5, 1.);
    recoSts->UseSensorCond(sensorCond);

    run->AddTask(recoSts);
    std::cout << "-I- : Added task " << recoSts->GetName() << std::endl;
    // ------------------------------------------------------------------------
  }


  // =========================================================================
  // ===                 local TRD Reconstruction                          ===
  // =========================================================================

  if (bTRD && geoSetup->IsActive(ECbmModuleId::kTrd)) {
    CbmTrdClusterFinder* trdCluster;
    Double_t triggerThreshold = 0.5e-6;  // SIS100

    trdCluster = new CbmTrdClusterFinder();
    trdCluster->SetNeighbourEnable(true, false);
    trdCluster->SetMinimumChargeTH(triggerThreshold);
    trdCluster->SetRowMerger(true);
    run->AddTask(trdCluster);
    std::cout << "-I- : Added task " << trdCluster->GetName() << std::endl;

    CbmTrdHitProducer* trdHit = new CbmTrdHitProducer();
    run->AddTask(trdHit);
    std::cout << "-I- : Added task " << trdHit->GetName() << std::endl;
  }

  // =========================================================================
  // ===                    RICH Reconstruction                            ===
  // =========================================================================

  if (bRICH && geoSetup->IsActive(ECbmModuleId::kRich)) {
    // -----   Local reconstruction of RICH Hits ------------------------------
    CbmRichMCbmHitProducer* hitProd = new CbmRichMCbmHitProducer();
    hitProd->SetMappingFile(std::string(srcDir.Data())
                            + "/macro/rich/mcbm/beamtime/mRICH_Mapping_vert_20190318_elView.geo");
    hitProd->SetIcdFilenameBase(std::string(srcDir.Data()) + "/macro/beamtime/mcbm2022/icd_offset_it");
    hitProd->setToTLimits(23.7, 30.0);
    hitProd->applyToTCut();
    hitProd->applyICDCorrection();
    run->AddTask(hitProd);
    // ------------------------------------------------------------------------

    // -----   Local reconstruction in RICh -> Finding of Rings ---------------
    CbmRichReconstruction* richReco = new CbmRichReconstruction();
    richReco->UseMCbmSetup();
    run->AddTask(richReco);
    // ------------------------------------------------------------------------
  }

  // =========================================================================
  // ===                        TOF Hitfinding                             ===
  // =========================================================================

  TString parPath = srcDir + "/parameters/mcbm/";
  if (bTOF && geoSetup->IsActive(ECbmModuleId::kTof)) {
    TString cFname;
    switch (iTofCluMode) {
      case 1: {
        // -----   TOF defaults ------------------------
        Int_t calMode      = 93;
        Int_t calSel       = 1;
        Int_t calSm        = 0;
        Int_t RefSel       = 0;
        Double_t dDeadtime = 50.;
        Int_t iSel2        = 500;
        Bool_t bOut        = kFALSE;

        // ------------------------------------------------------------------------
        gROOT->LoadMacro(srcDir + "/macro/beamtime/mcbm2022/ini_tof_clusterizer.C");
        Char_t* cCmd =
          Form("ini_tof_clusterizer(%d,%d,%d,%d,\"%s\",%d,%d,%d,%f,\"%s\",\"%s\")", calMode, calSel, calSm, RefSel,
               cFileId.Data(), iCalSet, (Int_t) bOut, iSel2, dDeadtime, cCalId.Data(), parPath.Data());
        cout << "<I> " << cCmd << endl;
        gInterpreter->ProcessLine(cCmd);
        // disable histogramming
        CbmTofEventClusterizer* tofClust = CbmTofEventClusterizer::Instance();
        tofClust->SetDutId(-1);  // to disable histogramming
      } break;

      default: {
        ;
      }
    }

    // =========================================================================
    // ===                   Tof Tracking                                    ===
    // =========================================================================

    if (bTOFtr) {
      cout << "<I> Initialize Tof tracker by ini_trks" << endl;

      Double_t dTOffScal = 1.;
      gROOT->LoadMacro(srcDir + "/macro/beamtime/mcbm2022/ini_tof_trks.C");
      Char_t* cCmd =
        Form("ini_tof_trks(%d,%d,%d,%6.2f,%8.1f,\"%s\",%d,%d,%d,%f,\"%s\")", iSel, iTrackingSetup, iGenCor, dScalFac,
             dChi2Lim2, cCalId.Data(), (Int_t) bUseSigCalib, iCalOpt, iTrkPar, dTOffScal, parPath.Data());
      cout << "<I> " << cCmd << endl;
      gInterpreter->ProcessLine(cCmd);

      CbmTofFindTracks* tofFindTracks = CbmTofFindTracks::Instance();
      Int_t iNStations                = tofFindTracks->GetNStations();
    }
  }


  //Constant Field
  // CbmConstField *fMagField = new CbmConstField();
  // fMagField->SetFieldXYZ(0, 0 ,0 ); // values are in kG
  // fMagField->SetFieldRegions(-10, -10 ,-10 , 10, 10 , 10 );
  // run->SetField(fMagField);

  // =========================================================================
  // ===                             L1                                    ===
  // =========================================================================
  if (bL1) {
    run->AddTask(new CbmTrackingDetectorInterfaceInit());

    CbmKF* kalman = new CbmKF();
    run->AddTask(kalman);

    CbmL1* l1 = new CbmL1();
    l1->SetLegacyEventMode(1);
    l1->SetMcbmMode();

    //    if (strcmp(geoSetupTag.data(), "mcbm_beam_2021_07_surveyed") == 0) l1->SetMissingHits(1);

    // --- Material budget file names
    TString mvdGeoTag;
    if (geoSetup->GetGeoTag(ECbmModuleId::kMvd, mvdGeoTag)) {
      TString parFile = gSystem->Getenv("VMCWORKDIR");
      parFile         = parFile + "/parameters/mvd/mvd_matbudget_" + mvdGeoTag + ".root";
      std::cout << "Using material budget file " << parFile << std::endl;
      l1->SetMvdMaterialBudgetFileName(parFile.Data());
    }
    TString stsGeoTag;
    if (geoSetup->GetGeoTag(ECbmModuleId::kSts, stsGeoTag)) {
      TString parFile = gSystem->Getenv("VMCWORKDIR");
      parFile         = parFile + "/parameters/sts/sts_matbudget_v19a.root";
      std::cout << "Using material budget file " << parFile << std::endl;
      l1->SetStsMaterialBudgetFileName(parFile.Data());
    }

    TString muchGeoTag;
    if (geoSetup->GetGeoTag(ECbmModuleId::kMuch, muchGeoTag)) {

      // --- Parameter file name
      TString geoTag;
      geoSetup->GetGeoTag(ECbmModuleId::kMuch, geoTag);
      Int_t muchFlag = 0;
      if (geoTag.Contains("mcbm")) muchFlag = 1;


      //       TString parFile2 = gSystem->Getenv("VMCWORKDIR");
      //       parFile2         = parFile2 + "/parameters/much/much_matbudget_" + geoTag + ".root ";
      //       std::cout << "Using material budget file " << parFile2 << std::endl;
      //       l1->SetMuchMaterialBudgetFileName(parFile2.Data());
    }

    TString trdGeoTag;
    if (geoSetup->GetGeoTag(ECbmModuleId::kTrd, trdGeoTag)) {
      TString parFile = gSystem->Getenv("VMCWORKDIR");
      parFile = parFile + "/parameters/trd/trd_matbudget_v22a_mcbm.root";  //trd_matbudget_" + trdGeoTag + ".root ";
      std::cout << "Using material budget file " << parFile << std::endl;
      l1->SetTrdMaterialBudgetFileName(parFile.Data());
    }

    TString tofGeoTag;
    if (geoSetup->GetGeoTag(ECbmModuleId::kTof, tofGeoTag)) {
      TString parFile = gSystem->Getenv("VMCWORKDIR");
      //  parFile         = parFile + "/parameters/tof/tof_matbudget_" + tofGeoTag + ".root ";

      parFile = parFile + "/parameters/tof/tof_matbudget_v21d_mcbm.root";
      std::cout << "Using material budget file " << parFile << std::endl;
      l1->SetTofMaterialBudgetFileName(parFile.Data());
    }


    //   Workaround to get it running:
    //    1) Change fUseGlobal  in line 129 of CbmStsParSetModule.h to
    //         Bool_t fUseGlobal = kTRUE;
    //    2) Change fUseGlobal  in line 114 of CbmStsParSetSensor.h to
    //         Bool_t fUseGlobal = kTRUE;
    run->AddTask(l1);

    CbmL1GlobalTrackFinder* globalTrackFinder = new CbmL1GlobalTrackFinder();
    FairTask* globalFindTracks                = new CbmL1GlobalFindTracksEvents(globalTrackFinder);
    run->AddTask(globalFindTracks);
  }
  // =========================================================================
  // ===                            QA                                     ===
  // =========================================================================
  if (bQA) {
    // e.g for RICH:
    CbmRichMCbmQaReal* qaTask = new CbmRichMCbmQaReal();
    Int_t taskId              = 1;
    if (taskId < 0) { qaTask->SetOutputDir(Form("result_run%d", uRunId)); }
    else {
      qaTask->SetOutputDir(Form("result_run%d_%05d", uRunId, taskId));
    }
    //qaTask->XOffsetHistos(+25.0);
    qaTask->XOffsetHistos(-4.1);
    if (uRunId > 2351) qaTask->XOffsetHistos(0.0);
    qaTask->SetMaxNofDrawnEvents(100);
    qaTask->SetTotRich(23.7, 30.0);
    qaTask->SetTriggerRichHits(eb_TriggerMinNumberRich);
    qaTask->SetTriggerTofHits(0);  // eb_TriggerMinNumberTof);
    qaTask->SetSEDisplayRingOnly();
    run->AddTask(qaTask);
  }
  // ------------------------------------------------------------------------

  // -----  Parameter database   --------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Set runtime DB" << std::endl;
  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  FairParRootFileIo* parIo1  = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  FairParRootFileIo* parIo3  = new FairParRootFileIo();
  //parIo1->open(parFileIn.Data(), "READ");
  //rtdb->setFirstInput(parIo1);
  parIo2->open(parFileList, "in");
  rtdb->setSecondInput(parIo2);
  parIo3->open(parFileOut.Data(), "RECREATE");
  // ------------------------------------------------------------------------
  rtdb->setOutput(parIo3);
  rtdb->saveOutput();
  rtdb->print();

  // -----   Run initialisation   -------------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Initialise run" << std::endl;
  run->Init();

  // ------------------------------------------------------------------------


  // -----   Start run   ----------------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Starting run" << std::endl;
  run->Run(0, nTimeslices);
  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  FairMonitor::GetMonitor()->Print();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Output file is " << outFile << std::endl;
  std::cout << "Parameter file is " << parFileOut << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
  std::cout << std::endl;
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
  //  RemoveGeoManager();
  // ------------------------------------------------------------------------

  /// --- Screen output for automatic tests
  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;

  return kTRUE;
}
