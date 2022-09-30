/* Copyright (C) 2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Norbert Herrmann, Pierre-Alain Loizeau, Adrian Weber [committer] */

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
Bool_t mcbm_digievent_reco(UInt_t uRunId               = 2365,
                       Int_t nTimeslices               = 10,
                       Int_t iFirstTimeslice           = 0,
                       TString cFId                    = "5.lxbk0598",
                       TString sInpDir                 = "./data/",
                       TString sOutDir                 = "./rec/",
                       Int_t iUnpFileIndex             = -1)
{
  /// FIXME: Re-enable clang formatting after parameters initial values setting
  /* clang-format on */

  // --- Logger settings ----------------------------------------------------
  TString logLevel     = "INFO";
  TString logVerbosity = "HIGH";
  // ------------------------------------------------------------------------


  // -----   Environment   --------------------------------------------------
  TString myName = "mcbm_event_reco";              // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------


  // -----   In- and output file names   ------------------------------------
  /// Standardized RUN ID
  TString sRunId = TString::Format("%04u", uRunId);

  /// Initial pattern
  TString cFileId = sRunId + "." + cFId;
  TString inFile  = sInpDir + "/" + cFileId + ".digievents";

  //TString parFileIn  = sInpDir + "/unp_mcbm_params_" + sRunId;
  TString cAnaOpt    = Form("_%d_%d_%s", iFirstTimeslice, nTimeslices, cFId.Data());
  TString parFileOut = sOutDir + "reco_event_mcbm_params_" + sRunId;
  TString outFile    = sOutDir + "reco_event_mcbm_" + sRunId + cAnaOpt;
  TString cHstFile   = sOutDir + "reco_digievent_" + cFileId + cAnaOpt + ".hst.root";

  // Your folder with the Tof Calibration files;
  TString TofFileFolder = srcDir + "/macro/beamtime/mcbm2022/";
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
  // ---------------------------------------------

  // -----   EventBuilder Settings----------------
  const Int_t eb_TriggerMinNumberT0 {1};
  const Int_t eb_TriggerMaxNumberT0 {2};
  const Int_t eb_TriggerMinNumberSts {0};
  const Int_t eb_TriggerMinNumberMuch {0};
  const Int_t eb_TriggerMinNumberTof {16};
  const Int_t eb_TriggerMinNumberTofLayers {4};
  const Int_t eb_TriggerMinNumberRich {5};

  // -----   TOF Calibration Settings ---------------------------------------
  TString cCalId = "490.100.5.0";
  if (uRunId >= 759) cCalId = "759.100.4.0";
  if (uRunId >= 812) cCalId = "831.100.4.0";
  if (uRunId >= 1588) cCalId = "1588.50.6.0";
  if (uRunId >= 2160) cCalId = "2160.50.4.0";
  if (uRunId >= 2352) cCalId = "2365.5.lxbk0600";
  if (uRunId >= 2389) cCalId = "2389.5.lxbk0598";
  if (uRunId >= 2390) cCalId = "2391.5.lxbk0598";
  if (uRunId >= 2393) cCalId = "2393.5.lxbk0598";

  Int_t iCalSet = 22002500;  // calibration settings
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
  cbm::mcbm::ToForceLibLoad dummy;  /// Needed to trigger loading of the library as no fct dict in ROOT6 and CLING
  TString geoSetupTag = "";
  try {
    geoSetupTag = cbm::mcbm::GetSetupFromRunId(uRunId);
  }
  catch (const std::invalid_argument& e) {
    std::cout << "Error in mapping from runID to setup name: " << e.what() << std::endl;
    return kFALSE;
  }

  TString geoFile    = srcDir + "/macro/mcbm/data/" + geoSetupTag + ".geo.root";
  CbmSetup* geoSetup = CbmSetup::Instance();
  geoSetup->LoadSetup(geoSetupTag);

  // You can modify the pre-defined setup by using
  geoSetup->SetActive(ECbmModuleId::kMvd, kFALSE);
  geoSetup->SetActive(ECbmModuleId::kSts, kTRUE);
  geoSetup->SetActive(ECbmModuleId::kMuch, kFALSE);
  geoSetup->SetActive(ECbmModuleId::kRich, kTRUE);
  geoSetup->SetActive(ECbmModuleId::kTrd, kFALSE);
  geoSetup->SetActive(ECbmModuleId::kTrd2d, kFALSE);
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


  TString alignmentMatrixFileName = "AlignmentMatrices_" + geoSetupTag + ".root";
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

  // ----   Make Reco Events   ----------------------------------------------
  // ---- This is required if the input is in DigiEvent format
  auto makeEvents = std::make_unique<CbmTaskMakeRecoEvents>();
  //LOG(info) << "-I- Adding task " << makeEvents->GetName();
  run->AddTask(makeEvents.release());
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
    hitProd->SetMappingFile("mRICH_Mapping_vert_20190318_elView.geo");
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

  if (geoSetup->IsActive(ECbmModuleId::kTof)) {
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
        gROOT->LoadMacro("ini_Clusterizer.C");
        Char_t* cCmd = Form("ini_Clusterizer(%d,%d,%d,%d,\"%s\",%d,%d,%d,%f,\"%s\")", calMode, calSel, calSm, RefSel,
                            cFileId.Data(), iCalSet, (Int_t) bOut, iSel2, dDeadtime, cCalId.Data());
        cout << "<I> " << cCmd << endl;
        gInterpreter->ProcessLine(cCmd);
        // disable histogramming
        CbmTofEventClusterizer* tofClust = CbmTofEventClusterizer::Instance();
        tofClust->SetDutId(-1);
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
    // Tracking
    Int_t iSel           = 22002;  //500;//910041;
    Int_t iTrackingSetup = 10;
    Int_t iGenCor        = 1;
    Double_t dScalFac    = 1.;
    Double_t dChi2Lim2   = 5.;
    Bool_t bUseSigCalib  = kFALSE;
    Int_t iCalOpt        = 0;
    Int_t iTrkPar        = 0;
    Double_t dTOffScal   = 1.;
    gROOT->LoadMacro("ini_trks.C");
    Char_t* cCmd = Form("ini_trks(%d,%d,%d,%6.2f,%8.1f,\"%s\",%d,%d,%d,%f)", iSel, iTrackingSetup, iGenCor, dScalFac,
                        dChi2Lim2, cCalId.Data(), (Int_t) bUseSigCalib, iCalOpt, iTrkPar, dTOffScal);
    cout << "<I> " << cCmd << endl;
    gInterpreter->ProcessLine(cCmd);

    CbmTofFindTracks* tofFindTracks = CbmTofFindTracks::Instance();
    Int_t iNStations                = tofFindTracks->GetNStations();
  }


  // =========================================================================
  // ===                             L1                                    ===
  // =========================================================================

  //   CbmKF* kalman = new CbmKF();
  //   run->AddTask(kalman);
  //   CbmL1* l1 = new CbmL1();
  //   l1->SetLegacyEventMode(1);
  //   l1->SetMcbmMode();
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

  // =========================================================================
  // ===                            QA                                     ===
  // =========================================================================

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
  qaTask->SetMaxNofDrawnEvents(0);
  qaTask->SetTotRich(23.7, 30.0);
  qaTask->SetTriggerRichHits(eb_TriggerMinNumberRich);
  qaTask->SetTriggerTofHits(eb_TriggerMinNumberTof);
  //qaTask->SetSEDisplayRingOnly();
  run->AddTask(qaTask);
  // ------------------------------------------------------------------------
  // --- Analysis by TOF track extension
  //
  CbmTofExtendTracks* tofExtendTracks = new CbmTofExtendTracks("TofExtAna");
  tofExtendTracks->SetCalParFileName("TofExtTracksPar.root");
  tofExtendTracks->SetCalOutFileName("TofExtTracksOut.root");
  tofExtendTracks->SetStationUT(2);  //
  //iLev: 0 update alignment with deviation from original tracklet
  //iLev: 1 update alignment with deviation from extended and refitted tracklet
  tofExtendTracks->SetCorSrc(1);     // [iLev]0 - all hits, [ilev]1 - pulls,
  tofExtendTracks->SetCorMode(210);  // 2 - Y coordinate, 1 - X coordinat, 0 Time offset
  tofExtendTracks->SetTrkHitsMin(4);
  tofExtendTracks->SetAddStations(1);
  tofExtendTracks->SetReqStations(1);
  tofExtendTracks->SetCutDX(10.);
  tofExtendTracks->SetCutDY(10.);
  tofExtendTracks->SetCutDT(50.);
  tofExtendTracks->SetChi2Max(10.);
  tofExtendTracks->SetCutStationMaxHitMul(100.);
  tofExtendTracks->SetNTrkTofMax(50);
  //run->AddTask(tofExtendTracks);

  // ------------------------------------------------------------------------
  // Hadron analysis, lambda search
  //
  CbmHadronAnalysis* HadronAna = new CbmHadronAnalysis();  // in hadron
  HadronAna->SetBeamMomentum(1.65);                        // beam momentum in GeV/c
  HadronAna->SetDY(0.5);                                   // flow analysis exclusion window
  HadronAna->SetRecSec(kTRUE);                             // enable lambda reconstruction
  Int_t parSet = 0;
  switch (parSet) {
    case 0:                             // with background
      HadronAna->SetDistPrimLim(1.);    // Max Tof-Sts trans distance for primaries
      HadronAna->SetDistPrimLim2(0.3);  // Max Sts-Sts trans distance for primaries
      HadronAna->SetDistSecLim2(0.3);   // Max Sts-Sts trans distance from TOF direction for secondaries
      HadronAna->SetD0ProtLim(0.5);     // Min impact parameter for secondary proton
      HadronAna->SetOpAngMin(0.1);      // Min opening angle for accepting pair
      HadronAna->SetDCALim(0.7);        // Max DCA for accepting pair
      HadronAna->SetVLenMin(5.);        // Min Lambda flight path length for accepting pair
      HadronAna->SetVLenMax(25.);       // Max Lambda flight path length for accepting pair
      HadronAna->SetNMixedEvents(10);   // Number of events to be mixed with
      break;
    case 1:                             // signal only, debugging
      HadronAna->SetDistPrimLim(0.5);   // Max Tof-Sts trans distance for primaries
      HadronAna->SetDistPrimLim2(0.3);  // Max Sts-Sts trans distance for primaries
      HadronAna->SetDistSecLim2(0.3);   // Max Sts-Sts trans distance from TOF direction for secondaries
      HadronAna->SetD0ProtLim(0.4);     // Min impact parameter for secondary proton
      HadronAna->SetOpAngMin(0.1);      // Min opening angle for accepting pair
      HadronAna->SetDCALim(0.2);        // Max DCA for accepting pair
      HadronAna->SetVLenMin(5.);        // Min Lambda flight path length for accepting pair
      HadronAna->SetVLenMax(25.);       // Max Lambda flight path length for accepting pair
      HadronAna->SetNMixedEvents(10);   // Number of events to be mixed with
      break;
  }
  run->AddTask(HadronAna);

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
  run->Run(iFirstTimeslice, iFirstTimeslice + nTimeslices);
  // ------------------------------------------------------------------------

  gROOT->LoadMacro("save_hst.C");
  TString SaveToHstFile = "save_hst(\"" + cHstFile + "\")";
  gInterpreter->ProcessLine(SaveToHstFile);

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
