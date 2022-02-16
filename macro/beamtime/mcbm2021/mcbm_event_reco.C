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
Bool_t mcbm_event_reco(UInt_t uRunId                   = 1588,
                       Int_t nTimeslices               = 20,
                       TString sInpDir                 = "/data/cbmroot/files/mTofCriPar2",
                       TString sOutDir                 = "rec/tofPar2_noAlign_L1/",
                       TString alignmentMatrixFileName = "AlignmentMatrices.root",
                       Int_t iUnpFileIndex             = -1)
{
  /// FIXME: Re-enable clang formatting after parameters initial values setting
  /* clang-format on */

  // --- Logger settings ----------------------------------------------------
  TString logLevel     = "INFO";
  TString logVerbosity = "LOW";
  // ------------------------------------------------------------------------


  // -----   Environment   --------------------------------------------------
  TString myName = "mcbm_event_reco";              // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------


  // -----   In- and output file names   ------------------------------------
  /// Standardized RUN ID
  TString sRunId = TString::Format("%04u", uRunId);

  /// Initial pattern
  TString inFile = sInpDir + "/" + sRunId + ".digi";

  //TString parFileIn  = sInpDir + "/unp_mcbm_params_" + sRunId;
  TString parFileOut = sOutDir + "/reco_event_mcbm_params_" + sRunId;
  TString outFile    = sOutDir + "/reco_event_mcbm_" + sRunId;

  // Your folder with the Tof Calibration files;
  TString TofFileFolder = "/data/cbmroot/files/tofCal/mTofCriPar2/";


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
  // ---------------------------------------------

  // -----   EventBuilder Settings----------------
  const Int_t eb_TriggerMinNumberT0 {0};
  const Int_t eb_TriggerMinNumberSts {0};
  const Int_t eb_TriggerMinNumberMuch {0};
  const Int_t eb_TriggerMinNumberTof {2};
  const Int_t eb_TriggerMinNumberRich {5};

  // -----   TOF defaults ------------------------
  Int_t calMode      = 93;
  Int_t calSel       = 1;
  Int_t calSm        = 0;
  Int_t RefSel       = 0;
  Double_t dDeadtime = 50.;
  Int_t iSel2        = 20;  //500;

  // Tracking
  Int_t iSel           = 1;  //500;//910041;
  Int_t iTrackingSetup = 2;
  Int_t iGenCor        = 1;
  Double_t dScalFac    = 1.;
  Double_t dChi2Lim2   = 500.;
  Bool_t bUseSigCalib  = kFALSE;
  Int_t iCalOpt        = 0;
  Int_t iTrkPar        = 3;
  // ------------------------------------------------------------------------

  // -----   TOF Calibration Settings ---------------------------------------
  TString cCalId = "490.100.5.0";
  if (uRunId >= 759) cCalId = "759.100.4.0";
  if (uRunId >= 812) cCalId = "831.100.4.0";
  if (uRunId >= 1588) cCalId = "1588.50.6.0";
  Int_t iCalSet = 30040500;  // calibration settings
  if (uRunId >= 759) iCalSet = 10020500;
  if (uRunId >= 812) iCalSet = 10020500;
  if (uRunId >= 1588) iCalSet = 12002002;

  Double_t Tint           = 100.;  // coincidence time interval
  Int_t iTrackMode        = 2;     // 2 for TofTracker
  const Int_t iTofCluMode = 1;
  // ------------------------------------------------------------------------

  // --- Load the geometry setup ----
  // This is currently only required by the TRD (parameters)
  TString geoSetupTag = "mcbm_beam_2021_07_surveyed";
  TString geoFile     = srcDir + "/macro/mcbm/data/" + geoSetupTag + ".geo.root";
  CbmSetup* geoSetup  = CbmSetup::Instance();
  geoSetup->LoadSetup(geoSetupTag);

  // You can modify the pre-defined setup by using
  geoSetup->SetActive(ECbmModuleId::kMvd, kFALSE);
  geoSetup->SetActive(ECbmModuleId::kSts, kTRUE);
  geoSetup->SetActive(ECbmModuleId::kMuch, kFALSE);
  geoSetup->SetActive(ECbmModuleId::kRich, kFALSE);
  geoSetup->SetActive(ECbmModuleId::kTrd, kTRUE);
  geoSetup->SetActive(ECbmModuleId::kTof, kTRUE);
  geoSetup->SetActive(ECbmModuleId::kPsd, kFALSE);

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
  // ------------------------------------------------------------------------


  // --------------------event builder---------------------------------------
  CbmTaskBuildRawEvents* evBuildRaw = new CbmTaskBuildRawEvents();

  //Choose between NoOverlap, MergeOverlap, AllowOverlap
  evBuildRaw->SetEventOverlapMode(EOverlapModeRaw::AllowOverlap);

  // Remove detectors where digis not found
  if (!geoSetup->IsActive(ECbmModuleId::kRich)) evBuildRaw->RemoveDetector(kRawEventBuilderDetRich);
  if (!geoSetup->IsActive(ECbmModuleId::kMuch)) evBuildRaw->RemoveDetector(kRawEventBuilderDetMuch);
  if (!geoSetup->IsActive(ECbmModuleId::kPsd)) evBuildRaw->RemoveDetector(kRawEventBuilderDetPsd);
  if (!geoSetup->IsActive(ECbmModuleId::kTrd)) evBuildRaw->RemoveDetector(kRawEventBuilderDetTrd);
  if (!geoSetup->IsActive(ECbmModuleId::kSts)) evBuildRaw->RemoveDetector(kRawEventBuilderDetSts);
  if (!geoSetup->IsActive(ECbmModuleId::kTof)) evBuildRaw->RemoveDetector(kRawEventBuilderDetTof);

  // Set TOF as reference detector
  evBuildRaw->SetReferenceDetector(kRawEventBuilderDetTof);

  // void SetTsParameters(double TsStartTime, double TsLength, double TsOverLength): TsStartTime=0, TsLength=256ms in 2021, TsOverLength=TS overlap, not used in mCBM2021
  evBuildRaw->SetTsParameters(0.0, 2.56e8, 0.0);

  if (geoSetup->IsActive(ECbmModuleId::kTof))
    evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kTof, eb_TriggerMinNumberTof);

  if (geoSetup->IsActive(ECbmModuleId::kTof)) evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kTof, -1);

  //evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kSts, eb_TriggerMinNumberSts);
  //evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kSts, -1);

  //evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kRich, eb_TriggerMinNumberRich);
  //evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kRich, -1);

  if (geoSetup->IsActive(ECbmModuleId::kTof)) evBuildRaw->SetTriggerWindow(ECbmModuleId::kTof, -50, 50);
  if (geoSetup->IsActive(ECbmModuleId::kSts)) evBuildRaw->SetTriggerWindow(ECbmModuleId::kSts, -50, 50);
  if (geoSetup->IsActive(ECbmModuleId::kTrd)) evBuildRaw->SetTriggerWindow(ECbmModuleId::kTrd, -200, 200);
  if (geoSetup->IsActive(ECbmModuleId::kRich)) evBuildRaw->SetTriggerWindow(ECbmModuleId::kRich, -50, 50);

  run->AddTask(evBuildRaw);
  // ------------------------------------------------------------------------


  // -----   Reconstruction tasks   -----------------------------------------


  // =========================================================================
  // ===                 local STS Reconstruction                          ===
  // =========================================================================

  if (geoSetup->IsActive(ECbmModuleId::kSts)) {
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
    auto parAsic = new CbmStsParAsic(128, 32, 75000., 3000., 5., 800., 1000., 3.9789e-3);

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

  CbmTrdClusterFinder* trdCluster;
  if (geoSetup->IsActive(ECbmModuleId::kTrd)) {
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

  if (geoSetup->IsActive(ECbmModuleId::kRich)) {
    // -----   Local reconstruction of RICH Hits ------------------------------
    CbmRichMCbmHitProducer* hitProd = new CbmRichMCbmHitProducer();
    hitProd->setToTLimits(23.7, 30.0);
    hitProd->applyToTCut();
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

  if (geoSetup->IsActive(ECbmModuleId::kTof)) {
    TString cFname;
    switch (iTofCluMode) {
      case 1: {
        CbmTofEventClusterizer* tofCluster = new CbmTofEventClusterizer("TOF Event Clusterizer", 0, 1);
        cFname = Form("/%s_set%09d_%02d_%01dtofClust.hst.root", cCalId.Data(), iCalSet, calMode, calSel);
        tofCluster->SetCalParFileName(TofFileFolder + cFname);
        tofCluster->SetCalMode(calMode);
        tofCluster->SetCalSel(calSel);
        tofCluster->SetCaldXdYMax(300.);            // geometrical matching window in cm
        tofCluster->SetCalCluMulMax(3.);            // Max Counter Cluster Multiplicity for filling calib histos
        tofCluster->SetCalRpc(calSm);               // select detector for calibration update
        tofCluster->SetTRefId(RefSel);              // reference trigger for offset calculation
        tofCluster->SetTotMax(20.);                 // Tot upper limit for walk corection
        tofCluster->SetTotMin(0.);                  //(12000.);  // Tot lower limit for walk correction
        tofCluster->SetTotPreRange(5.);             // effective lower Tot limit  in ns from peak position
        tofCluster->SetTotMean(5.);                 // Tot calibration target value in ns
        tofCluster->SetMaxTimeDist(1.0);            // default cluster range in ns
        tofCluster->SetDelTofMax(50.);              // acceptance range for cluster distance in ns (!)
        tofCluster->SetSel2MulMax(3);               // limit Multiplicity in 2nd selector
        tofCluster->SetChannelDeadtime(dDeadtime);  // artificial deadtime in ns
        tofCluster->SetEnableAvWalk(kFALSE);
        //tofCluster->SetEnableMatchPosScaling(kFALSE); // turn off projection to nominal target
        tofCluster->SetYFitMin(1.E4);
        tofCluster->SetToDAv(0.04);
        tofCluster->SetIdMode(1);        // calibrate on module level
        tofCluster->SetTRefDifMax(2.0);  // in ns
        tofCluster->PosYMaxScal(0.75);   //in % of length
        Int_t iBRef    = iCalSet % 1000;
        Int_t iSet     = (iCalSet - iBRef) / 1000;
        Int_t iRSel    = 0;
        Int_t iRSelTyp = 0;
        Int_t iRSelSm  = 0;
        Int_t iRSelRpc = 0;
        iRSel          = iBRef;  // use diamond
        Int_t iRSelin  = iRSel;
        iRSelRpc       = iRSel % 10;
        iRSelTyp       = (iRSel - iRSelRpc) / 10;
        iRSelSm        = iRSelTyp % 10;
        iRSelTyp       = (iRSelTyp - iRSelSm) / 10;
        tofCluster->SetBeamRefId(iRSelTyp);  // define Beam reference counter
        tofCluster->SetBeamRefSm(iRSelSm);
        tofCluster->SetBeamRefDet(iRSelRpc);
        tofCluster->SetBeamAddRefMul(-1);
        tofCluster->SetBeamRefMulMax(3);
        Int_t iSel2in  = iSel2;
        Int_t iSel2Rpc = iSel2 % 10;
        iSel2          = (iSel2 - iSel2Rpc) / 10;
        Int_t iSel2Sm  = iSel2 % 10;
        iSel2          = (iSel2 - iSel2Sm) / 10;
        if (iSel2 > -1) {
          tofCluster->SetSel2Id(iSel2);
          tofCluster->SetSel2Sm(iSel2Sm);
          tofCluster->SetSel2Rpc(iSel2Rpc);
        }
        Int_t iRef    = iSet % 1000;
        Int_t iDut    = (iSet - iRef) / 1000;
        Int_t iDutRpc = iDut % 10;
        iDut          = (iDut - iDutRpc) / 10;
        Int_t iDutSm  = iDut % 10;
        iDut          = (iDut - iDutSm) / 10;
        tofCluster->SetDutId(iDut);
        tofCluster->SetDutSm(iDutSm);
        tofCluster->SetDutRpc(iDutRpc);

        Int_t iRefRpc = iRef % 10;
        iRef          = (iRef - iRefRpc) / 10;
        Int_t iRefSm  = iRef % 10;
        iRef          = (iRef - iRefSm) / 10;

        tofCluster->SetSelId(iRef);
        tofCluster->SetSelSm(iRefSm);
        tofCluster->SetSelRpc(iRefRpc);

        run->AddTask(tofCluster);
        std::cout << "-I- " << myName << ": Added task " << tofCluster->GetName() << std::endl;
      } break;

      default: {
        ;
      }
    }
    // -------------------------------------------------------------------------


    // =========================================================================
    // ===                   Tof Tracking                                    ===
    // =========================================================================

    cout << "<I> Initialize Tof tracker by ini_trks" << endl;
    TString cTrkFile = Form("%s/%s_tofFindTracks.hst.root", TofFileFolder.Data(), cCalId.Data());

    // -----   Local selection variables  --------------------------------------

    Int_t iRef    = iSel % 1000;
    Int_t iDut    = (iSel - iRef) / 1000;
    Int_t iDutRpc = iDut % 10;
    iDut          = (iDut - iDutRpc) / 10;
    Int_t iDutSm  = iDut % 10;
    iDut          = (iDut - iDutSm) / 10;
    Int_t iBucRpc = 0;

    // =========================================================================
    // ===                       Tracking                                    ===
    // =========================================================================

    CbmTofTrackFinder* tofTrackFinder = new CbmTofTrackFinderNN();
    tofTrackFinder->SetMaxTofTimeDifference(0.2);  // in ns/cm
    Int_t TrackerPar = 0;
    switch (TrackerPar) {
      case 0:                           // for full mTof setup
        tofTrackFinder->SetTxLIM(0.3);  // max slope dx/dz
        tofTrackFinder->SetTyLIM(0.3);  // max dev from mean slope dy/dz
        tofTrackFinder->SetTxMean(0.);  // mean slope dy/dz
        tofTrackFinder->SetTyMean(0.);  // mean slope dy/dz
        break;
      case 1:                             // for double stack test counters
        tofTrackFinder->SetTxMean(0.21);  // mean slope dy/dz
        tofTrackFinder->SetTyMean(0.18);  // mean slope dy/dz
        tofTrackFinder->SetTxLIM(0.15);   // max slope dx/dz
        tofTrackFinder->SetTyLIM(0.18);   // max dev from mean slope dy/dz
        break;
    }

    CbmTofTrackFitter* tofTrackFitter = new CbmTofTrackFitterKF(0, 211);
    TFitter* MyFit                    = new TFitter(1);  // initialize Minuit
    tofTrackFinder->SetFitter(tofTrackFitter);
    CbmTofFindTracks* tofFindTracks = new CbmTofFindTracks("TOF Track Finder");
    tofFindTracks->UseFinder(tofTrackFinder);
    tofFindTracks->UseFitter(tofTrackFitter);
    tofFindTracks->SetCalOpt(iCalOpt);           // 1 - update offsets, 2 - update walk, 0 - bypass
    tofFindTracks->SetCorMode(iGenCor);          // valid options: 0,1,2,3,4,5,6, 10 - 19
    tofFindTracks->SetTtTarg(0.042);             // target value Mar2021, after T0 fix (double stack run 1058)
    tofFindTracks->SetCalParFileName(cTrkFile);  // Tracker parameter value file name
    tofFindTracks->SetBeamCounter(5, 0, 0);      // default beam counter
    tofFindTracks->SetR0Lim(20.);
    tofFindTracks->SetStationMaxHMul(30);  // Max Hit Multiplicity in any used station

    tofFindTracks->SetT0MAX(dScalFac);            // in ns
    tofFindTracks->SetSIGT(0.08);                 // default in ns
    tofFindTracks->SetSIGX(0.3);                  // default in cm
    tofFindTracks->SetSIGY(0.45);                 // default in cm
    tofFindTracks->SetSIGZ(0.05);                 // default in cm
    tofFindTracks->SetUseSigCalib(bUseSigCalib);  // ignore resolutions in CalPar file
    tofTrackFinder->SetSIGLIM(dChi2Lim2 * 2.);    // matching window in multiples of chi2
    tofTrackFinder->SetChiMaxAccept(dChi2Lim2);   // max tracklet chi2

    Int_t iMinNofHits   = -1;
    Int_t iNStations    = 0;
    Int_t iNReqStations = 3;

    switch (iTrackingSetup) {
      case 0:  // bypass mode
        iMinNofHits = -1;
        iNStations  = 1;
        tofFindTracks->SetStation(0, 5, 0, 0);  // Diamond
        break;

      case 1:  // for calibration mode of full setup
      {
        Double_t dTsig = dScalFac * 0.03;
        tofFindTracks->SetSIGT(dTsig);  // allow for variable deviations in ns
      }
        iMinNofHits   = 3;
        iNStations    = 32;
        iNReqStations = 4;
        tofFindTracks->SetStation(0, 5, 0, 0);
        tofFindTracks->SetStation(1, 0, 2, 2);
        tofFindTracks->SetStation(2, 0, 1, 2);
        tofFindTracks->SetStation(3, 0, 0, 2);
        tofFindTracks->SetStation(4, 0, 2, 1);
        tofFindTracks->SetStation(5, 0, 1, 1);
        tofFindTracks->SetStation(6, 0, 0, 1);
        tofFindTracks->SetStation(7, 0, 2, 3);
        tofFindTracks->SetStation(8, 0, 1, 3);
        tofFindTracks->SetStation(9, 0, 0, 3);
        tofFindTracks->SetStation(10, 0, 2, 0);
        tofFindTracks->SetStation(11, 0, 1, 0);
        tofFindTracks->SetStation(12, 0, 0, 0);
        tofFindTracks->SetStation(13, 0, 2, 4);
        tofFindTracks->SetStation(14, 0, 1, 4);
        tofFindTracks->SetStation(15, 0, 0, 4);
        tofFindTracks->SetStation(16, 0, 4, 0);
        tofFindTracks->SetStation(17, 0, 3, 0);
        tofFindTracks->SetStation(18, 0, 4, 1);
        tofFindTracks->SetStation(19, 0, 3, 1);
        tofFindTracks->SetStation(20, 0, 4, 2);
        tofFindTracks->SetStation(21, 0, 3, 2);
        tofFindTracks->SetStation(22, 0, 4, 3);
        tofFindTracks->SetStation(23, 0, 3, 3);
        tofFindTracks->SetStation(24, 0, 4, 4);
        tofFindTracks->SetStation(25, 0, 3, 4);
        tofFindTracks->SetStation(26, 9, 0, 0);
        tofFindTracks->SetStation(27, 9, 1, 0);
        tofFindTracks->SetStation(28, 9, 0, 1);
        tofFindTracks->SetStation(29, 9, 1, 1);
        tofFindTracks->SetStation(30, 6, 0, 0);
        tofFindTracks->SetStation(31, 6, 0, 1);
        break;

      case 11:  // for calibration mode of 2-stack & test counters
        iMinNofHits   = 4;
        iNStations    = 9;
        iNReqStations = 5;
        tofFindTracks->SetStation(0, 0, 4, 1);
        tofFindTracks->SetStation(1, 9, 0, 0);
        tofFindTracks->SetStation(2, 9, 1, 0);
        tofFindTracks->SetStation(3, 9, 0, 1);
        tofFindTracks->SetStation(4, 9, 1, 1);
        tofFindTracks->SetStation(5, 0, 3, 1);
        tofFindTracks->SetStation(6, 0, 4, 0);
        tofFindTracks->SetStation(7, 0, 3, 2);
        tofFindTracks->SetStation(8, 5, 0, 0);
        break;

      case 2:
        iMinNofHits   = 5;
        iNStations    = 28;
        iNReqStations = 5;
        tofFindTracks->SetStation(0, 0, 2, 2);
        tofFindTracks->SetStation(1, 0, 0, 2);
        tofFindTracks->SetStation(2, 0, 1, 2);
        tofFindTracks->SetStation(3, 0, 2, 1);
        tofFindTracks->SetStation(4, 0, 0, 1);
        tofFindTracks->SetStation(5, 0, 1, 1);
        tofFindTracks->SetStation(6, 0, 2, 3);
        tofFindTracks->SetStation(7, 0, 0, 3);
        tofFindTracks->SetStation(8, 0, 1, 3);
        tofFindTracks->SetStation(9, 0, 2, 0);
        tofFindTracks->SetStation(10, 0, 0, 0);
        tofFindTracks->SetStation(11, 0, 1, 0);
        tofFindTracks->SetStation(12, 0, 2, 4);
        tofFindTracks->SetStation(13, 0, 0, 4);
        tofFindTracks->SetStation(14, 0, 1, 4);
        tofFindTracks->SetStation(15, 0, 4, 0);
        tofFindTracks->SetStation(16, 0, 3, 0);
        tofFindTracks->SetStation(17, 0, 4, 1);
        tofFindTracks->SetStation(18, 0, 3, 1);
        tofFindTracks->SetStation(19, 0, 4, 2);
        tofFindTracks->SetStation(20, 0, 3, 2);
        tofFindTracks->SetStation(21, 0, 4, 3);
        tofFindTracks->SetStation(22, 0, 3, 3);
        tofFindTracks->SetStation(23, 0, 4, 4);
        tofFindTracks->SetStation(24, 0, 3, 4);
        tofFindTracks->SetStation(25, 9, 0, 0);
        tofFindTracks->SetStation(26, 9, 0, 1);
        tofFindTracks->SetStation(27, 5, 0, 0);
        break;

      case 3:
        iMinNofHits   = 3;
        iNStations    = 16;
        iNReqStations = 4;
        tofFindTracks->SetStation(0, 5, 0, 0);
        tofFindTracks->SetStation(1, 0, 2, 2);
        tofFindTracks->SetStation(2, 0, 1, 2);
        tofFindTracks->SetStation(3, 0, 0, 2);

        tofFindTracks->SetStation(4, 0, 2, 1);
        tofFindTracks->SetStation(5, 0, 1, 1);
        tofFindTracks->SetStation(6, 0, 0, 1);

        tofFindTracks->SetStation(7, 0, 2, 3);
        tofFindTracks->SetStation(8, 0, 1, 3);
        tofFindTracks->SetStation(9, 0, 0, 3);

        tofFindTracks->SetStation(10, 0, 2, 0);
        tofFindTracks->SetStation(11, 0, 1, 0);
        tofFindTracks->SetStation(12, 0, 0, 0);

        tofFindTracks->SetStation(13, 0, 2, 4);
        tofFindTracks->SetStation(14, 0, 1, 4);
        tofFindTracks->SetStation(15, 0, 0, 4);

        /*
       tofFindTracks->SetStation(16, 0, 3, 2);         
       tofFindTracks->SetStation(17, 0, 4, 2);  
       tofFindTracks->SetStation(18, 0, 3, 1);         
       tofFindTracks->SetStation(19, 0, 4, 1);
       tofFindTracks->SetStation(20, 0, 3, 3);         
       tofFindTracks->SetStation(21, 0, 4, 3);
       tofFindTracks->SetStation(22, 0, 3, 0);         
       tofFindTracks->SetStation(23, 0, 4, 0);
       tofFindTracks->SetStation(24, 0, 3, 4);         
       tofFindTracks->SetStation(25, 0, 4, 4); 
       */
        break;

      case 4:  // for USTC evaluation (dut=910,911)
        iMinNofHits   = 4;
        iNStations    = 6;
        iNReqStations = 6;
        tofFindTracks->SetStation(0, 0, 4, 1);
        tofFindTracks->SetStation(1, 0, 3, 1);
        tofFindTracks->SetStation(2, 9, 0, 1);
        tofFindTracks->SetStation(3, 9, 0, 0);
        tofFindTracks->SetStation(4, 5, 0, 0);
        tofFindTracks->SetStation(5, iDut, iDutSm, iDutRpc);
        break;

      case 14:
        iMinNofHits   = 3;
        iNStations    = 15;
        iNReqStations = 4;
        tofFindTracks->SetStation(0, 0, 2, 2);
        tofFindTracks->SetStation(1, 0, 1, 2);
        tofFindTracks->SetStation(2, 0, 0, 2);
        tofFindTracks->SetStation(0, 0, 2, 1);
        tofFindTracks->SetStation(1, 0, 1, 1);
        tofFindTracks->SetStation(2, 0, 0, 1);
        tofFindTracks->SetStation(0, 0, 2, 0);
        tofFindTracks->SetStation(1, 0, 1, 0);
        tofFindTracks->SetStation(2, 0, 0, 0);
        tofFindTracks->SetStation(0, 0, 2, 3);
        tofFindTracks->SetStation(1, 0, 1, 3);
        tofFindTracks->SetStation(2, 0, 0, 3);
        tofFindTracks->SetStation(0, 0, 2, 4);
        tofFindTracks->SetStation(1, 0, 1, 4);
        tofFindTracks->SetStation(2, 0, 0, 4);
        break;

      case 5:  // for calibration of 2-stack and add-on counters (STAR2, BUC)
        iMinNofHits   = 3;
        iNStations    = 7;
        iNReqStations = 4;
        tofFindTracks->SetStation(6, 0, 4, 1);
        tofFindTracks->SetStation(1, 6, 0, 1);
        tofFindTracks->SetStation(2, 9, 0, 0);
        tofFindTracks->SetStation(3, 9, 0, 1);
        tofFindTracks->SetStation(4, 6, 0, 0);
        tofFindTracks->SetStation(5, 0, 3, 1);
        tofFindTracks->SetStation(0, 5, 0, 0);
        break;

      case 6:  // for double stack USTC counter evaluation
        iMinNofHits   = 5;
        iNStations    = 6;
        iNReqStations = 6;
        tofFindTracks->SetStation(0, 5, 0, 0);
        tofFindTracks->SetStation(1, 6, 0, 1);
        tofFindTracks->SetStation(2, 0, 4, 1);
        tofFindTracks->SetStation(3, 6, 0, 0);
        tofFindTracks->SetStation(4, 0, 3, 1);
        tofFindTracks->SetStation(5, iDut, iDutSm, iDutRpc);
        break;

      case 7:  // for double stack USTC counter evaluation
        iMinNofHits   = 3;
        iNStations    = 4;
        iNReqStations = 4;
        tofFindTracks->SetStation(0, 0, 4, 1);
        tofFindTracks->SetStation(1, 6, 0, 1);
        tofFindTracks->SetStation(2, 6, 0, 0);
        tofFindTracks->SetStation(3, iDut, iDutSm, iDutRpc);
        break;

      case 8:  // evaluation of add-on counters (BUC)
        iMinNofHits   = 5;
        iNStations    = 6;
        iNReqStations = 6;
        tofFindTracks->SetStation(0, 5, 0, 0);
        tofFindTracks->SetStation(1, 9, 0, 1);
        tofFindTracks->SetStation(2, 0, 4, 1);
        tofFindTracks->SetStation(3, 9, 0, 0);
        tofFindTracks->SetStation(4, 0, 3, 1);
        tofFindTracks->SetStation(5, iDut, iDutSm, iDutRpc);
        break;

      case 9:  // calibration of Star2
        iMinNofHits   = 4;
        iNStations    = 5;
        iNReqStations = 5;
        tofFindTracks->SetStation(0, 5, 0, 0);
        tofFindTracks->SetStation(2, 9, 0, 1);
        tofFindTracks->SetStation(1, 0, 4, 1);
        tofFindTracks->SetStation(3, 9, 0, 0);
        tofFindTracks->SetStation(4, 0, 3, 1);
        break;

      case 10:
        iMinNofHits   = 3;
        iNStations    = 4;
        iNReqStations = 4;
        tofFindTracks->SetStation(0, 5, 0, 0);
        tofFindTracks->SetStation(3, 0, 1, 2);
        tofFindTracks->SetStation(2, 0, 0, 2);
        tofFindTracks->SetStation(1, 0, 2, 2);
        break;

      default:
        cout << "Tracking setup " << iTrackingSetup << " not implemented " << endl;
        return 1;
        ;
    }
    tofFindTracks->SetMinNofHits(iMinNofHits);
    tofFindTracks->SetNStations(iNStations);
    tofFindTracks->SetNReqStations(iNReqStations);
    tofFindTracks->PrintSetup();
    std::cout << "MinNofHitsPerTrack: " << iMinNofHits << std::endl;
    run->AddTask(tofFindTracks);
  }


  // =========================================================================
  // ===                             L1                                    ===
  // =========================================================================

  //   CbmKF* kalman = new CbmKF();
  //   run->AddTask(kalman);
  //   CbmL1* l1 = new CbmL1();
  //   l1->SetLegacyEventMode(1);
  //   l1->SetMcbmMode();
  //   l1->SetUseHitErrors(1);
  //   if (strcmp(geoSetupTag.data(), "mcbm_beam_2021_07_surveyed") == 0) l1->SetMissingHits(1);
  //
  //   // --- Material budget file names
  //   TString mvdGeoTag;
  //   if (geoSetup->GetGeoTag(ECbmModuleId::kMvd, mvdGeoTag)) {
  //     TString parFile = gSystem->Getenv("VMCWORKDIR");
  //     parFile         = parFile + "/parameters/mvd/mvd_matbudget_" + mvdGeoTag + ".root";
  //     std::cout << "Using material budget file " << parFile << std::endl;
  //     l1->SetMvdMaterialBudgetFileName(parFile.Data());
  //   }
  //   TString stsGeoTag;
  //   if (geoSetup->GetGeoTag(ECbmModuleId::kSts, stsGeoTag)) {
  //     TString parFile = gSystem->Getenv("VMCWORKDIR");
  //     parFile         = parFile + "/parameters/sts/sts_matbudget_v19a.root";
  //     std::cout << "Using material budget file " << parFile << std::endl;
  //     l1->SetStsMaterialBudgetFileName(parFile.Data());
  //   }
  //
  //   TString muchGeoTag;
  //   if (geoSetup->GetGeoTag(ECbmModuleId::kMuch, muchGeoTag)) {
  //
  //     // --- Parameter file name
  //     TString geoTag;
  //     geoSetup->GetGeoTag(ECbmModuleId::kMuch, geoTag);
  //     Int_t muchFlag = 0;
  //     if (geoTag.Contains("mcbm")) muchFlag = 1;
  //
  //     TString parFile = gSystem->Getenv("VMCWORKDIR");
  //     parFile         = parFile + "/parameters/much/much_" + geoTag(0, 4) + "_digi_sector.root";
  //     std::cout << "L1: Using parameter file " << parFile << std::endl;
  //     l1->SetMuchPar(parFile);
  //
  //     TString parFile2 = gSystem->Getenv("VMCWORKDIR");
  //     parFile2         = parFile2 + "/parameters/much/much_matbudget_" + geoTag + ".root ";
  //     std::cout << "Using material budget file " << parFile2 << std::endl;
  //     l1->SetMuchMaterialBudgetFileName(parFile2.Data());
  //   }
  //
  //   TString trdGeoTag;
  //   if (geoSetup->GetGeoTag(ECbmModuleId::kTrd, trdGeoTag)) {
  //     TString parFile = gSystem->Getenv("VMCWORKDIR");
  //     parFile         = parFile + "/parameters/trd/trd_matbudget_" + trdGeoTag + ".root ";
  //     std::cout << "Using material budget file " << parFile << std::endl;
  //     l1->SetTrdMaterialBudgetFileName(parFile.Data());
  //   }
  //
  //   TString tofGeoTag;
  //   if (geoSetup->GetGeoTag(ECbmModuleId::kTof, tofGeoTag)) {
  //     TString parFile = gSystem->Getenv("VMCWORKDIR");
  //     parFile         = parFile + "/parameters/tof/tof_matbudget_" + tofGeoTag + ".root ";
  //     std::cout << "Using material budget file " << parFile << std::endl;
  //     l1->SetTofMaterialBudgetFileName(parFile.Data());
  //   }
  //

  // Workaround to get it running:
  //  1) Change fUseGlobal  in line 129 of CbmStsParSetModule.h to
  //       Bool_t fUseGlobal = kTRUE;
  //  2) Change fUseGlobal  in line 114 of CbmStsParSetSensor.h to
  //       Bool_t fUseGlobal = kTRUE;
  //run->AddTask(l1);

  //   CbmL1GlobalTrackFinder* globalTrackFinder = new CbmL1GlobalTrackFinder();
  //   FairTask* globalFindTracks                = new CbmL1GlobalFindTracksEvents(globalTrackFinder);
  //run->AddTask(globalFindTracks);


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

  // =========================================================================
  // ===                            QA                                     ===
  // =========================================================================

  // e.g for RICH:
  // CbmRichMCbmQaReal* qaTask = new CbmRichMCbmQaReal();
  // if (taskId < 0) { qaTask->SetOutputDir(Form("result_run%d", uRunId)); }
  // else {
  //   qaTask->SetOutputDir(Form("result_run%d_%05d", uRunId, taskId));
  // }
  // qaTask->XOffsetHistos(+25.3);
  // qaTask->SetMaxNofDrawnEvents(100);
  // qaTask->SetTotRich(23.7, 30.0);
  // qaTask->SetTriggerRichHits(eb_TriggerMinNumberRich);
  // qaTask->SetTriggerTofHits(1);
  // run->AddTask(qaTask);
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
