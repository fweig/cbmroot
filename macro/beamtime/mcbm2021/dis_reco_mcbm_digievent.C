/* Copyright (C) 2020-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer], Dominik Smith */

/** @file run_reco.C
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 14 November 2020
 **/


// --- Includes needed for IDE
#include <RtypesCore.h>
#if !defined(__CLING__)
#include "CbmBuildEventsFromTracksReal.h"
#include "CbmBuildEventsIdeal.h"
#include "CbmBuildEventsQa.h"
#include "CbmDefs.h"
#include "CbmFindPrimaryVertex.h"
#include "CbmKF.h"
#include "CbmL1.h"
#include "CbmL1StsTrackFinder.h"
#include "CbmLitFindGlobalTracks.h"
#include "CbmMCDataManager.h"
#include "CbmMatchRecoToMC.h"
#include "CbmMuchFindHitsGem.h"
#include "CbmMvdClusterfinder.h"
#include "CbmMvdHitfinder.h"
#include "CbmPVFinderKF.h"
#include "CbmPrimaryVertexFinder.h"
#include "CbmPsdHitProducer.h"
#include "CbmRecoSts.h"
#include "CbmRichHitProducer.h"
#include "CbmRichReconstruction.h"
#include "CbmSetup.h"
#include "CbmStsFindTracks.h"
#include "CbmStsFindTracksEvents.h"
#include "CbmStsTrackFinder.h"
#include "CbmTaskBuildRawEvents.h"
#include "CbmTofSimpClusterizer.h"
#include "CbmTrdClusterFinder.h"
#include "CbmTrdHitProducer.h"

#include <FairFileSource.h>
#include <FairMonitor.h>
#include <FairParAsciiFileIo.h>
#include <FairParRootFileIo.h>
#include <FairRunAna.h>
#include <FairRuntimeDb.h>
#include <FairSystemInfo.h>

#include <TStopwatch.h>
#endif


/** @brief Macro for CBM reconstruction
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since  14 November 2020
 ** @param input          Name of input file (w/o extension .raw.root)
 ** @param nTimeSlices    Number of time-slices to process
 ** @param firstTimeSlice First time-slice (entry) to be processed
 ** @param output         Name of output file (w/o extension .rec.root)
 ** @param sEvBuildRaw    Option for raw event building
 ** @param setup          Name of predefined geometry setup
 ** @param paramFile      Parameter ROOT file (w/o extension .par.root)
 ** @param debugWithMC          Option to provide the trackfinder with MC information
 **
 ** This macro performs event-by-event reconstruction from from the digis in DigiEvents.
 ** It can be used for real data after unpacking, triggering and event building or
 ** for simulated data after triggering and event building with macro/reco/reco_digi.C.
 **
 ** The file names must be specified without extensions. The convention is
 ** that the raw (input) file is [input].raw.root. ThecalMode output file
 ** will be [input].rec.root if not specified by the user. The parameter file
 ** has the extension .par.root. It is assumed to be [input].par.root if
 ** not specified by the user.
 **
 ** If no argument is specified, the input will be set to "test". This allows
 ** to execute the macro chain (run_tra_file.C, run_digi.C and run_reco.C)
 ** from the ROOT prompt without user intervention.
 **
 **/
void dis_reco_mcbm_digievent(TString input = "", Int_t nTimeSlices = 1, Int_t firstTimeSlice = 0, TString output = "",
                             TString setup = "mcbm_beam_2021_07_surveyed", TString paramFile = "reco_mcbm_params_")
{

  // ========================================================================
  //          Adjust this part according to your requirements

  // --- Logger settings ----------------------------------------------------
  TString logLevel     = "DEBUG";
  TString logVerbosity = "VERYHIGH";
  // ------------------------------------------------------------------------

  // -----   Environment   --------------------------------------------------
  TString myName = "run_reco_digievent";           // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------

  TString DataPath = "./data/";
  //TString DataPath = "/home/nh/LUSTRE/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2021/data/";

  // -----   In- and output file names   ------------------------------------
  if (input.IsNull()) input = "1588";
  TString rawFile = DataPath + input + ".digievents.root";
  if (output.IsNull()) output = input;
  TString outFile = DataPath + "dis_reco_mcbm_event_" + output + ".root";
  TString monFile = DataPath + output + ".moni_reco.root";
  if (paramFile.IsNull()) paramFile = input;
  TString parFile = DataPath + paramFile + output + ".root";
  std::cout << "Inputfile " << rawFile << std::endl;
  std::cout << "Outfile " << outFile << std::endl;
  std::cout << "Parfile " << parFile << std::endl;

  TString shcmd = "rm -v " + parFile;
  gSystem->Exec(shcmd.Data());

  // -----   Load the geometry setup   -------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Loading setup " << setup << std::endl;
  CbmSetup* geo = CbmSetup::Instance();
  geo->LoadSetup(setup);

  if (kTRUE) {  // get geometry including TGeoManager
    TString TofGeo      = "v21d_mcbm";
    TString geoDir      = gSystem->Getenv("VMCWORKDIR");
    TString geoFile     = geoDir + "/geometry/tof/geofile_tof_" + TofGeo + ".root";
    TFile* fgeo         = new TFile(geoFile);
    TGeoManager* geoMan = (TGeoManager*) fgeo->Get("FAIRGeom");
    if (NULL == geoMan) {
      cout << "<E> FAIRGeom not found in geoFile" << endl;
      return;
    }
  }
  // ------------------------------------------------------------------------

  // -----   Some global switches   -----------------------------------------
  Bool_t useMvd  = kFALSE;  // geo->IsActive(ECbmModuleId::kMvd);
  Bool_t useSts  = kFALSE;  // geo->IsActive(ECbmModuleId::kSts);
  Bool_t useRich = kFALSE;  // geo->IsActive(ECbmModuleId::kRich);
  Bool_t useMuch = kFALSE;  // geo->IsActive(ECbmModuleId::kMuch);
  Bool_t useTrd  = kFALSE;  // geo->IsActive(ECbmModuleId::kTrd);
  Bool_t useTof  = geo->IsActive(ECbmModuleId::kTof);
  Bool_t usePsd  = kFALSE;  // geo->IsActive(ECbmModuleId::kPsd);
  // ------------------------------------------------------------------------


  // -----   Parameter files as input to the runtime database   -------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Defining parameter files " << std::endl;
  TList* parFileList = new TList();
  TString geoTag;

  // - TRD digitisation parameters
  if (CbmSetup::Instance()->GetGeoTag(ECbmModuleId::kTrd, geoTag)) {
    const Char_t* npar[4] = {"asic", "digi", "gas", "gain"};
    TObjString* trdParFile(NULL);
    for (Int_t i(0); i < 4; i++) {
      trdParFile = new TObjString(srcDir + "/parameters/trd/trd_" + geoTag + "." + npar[i] + ".par");
      parFileList->Add(trdParFile);
      std::cout << "-I- " << myName << ": Using parameter file " << trdParFile->GetString() << std::endl;
    }
  }

  // - TOF digitisation parameters
  if (CbmSetup::Instance()->GetGeoTag(ECbmModuleId::kTof, geoTag)) {
    TObjString* tofBdfFile = new TObjString(srcDir + "/parameters/tof/tof_" + geoTag + ".digibdf.par");
    parFileList->Add(tofBdfFile);
    std::cout << "-I- " << myName << ": Using parameter file " << tofBdfFile->GetString() << std::endl;
  }
  // ------------------------------------------------------------------------

  // In general, the following parts need not be touched
  // ========================================================================


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // -----   FairRunAna   ---------------------------------------------------
  FairRunAna* run             = new FairRunAna();
  FairFileSource* inputSource = new FairFileSource(rawFile);
  run->SetSource(inputSource);
  run->SetOutputFile(outFile);
  run->SetGenerateRunInfo(kTRUE);
  FairMonitor::GetMonitor()->EnableMonitor(kTRUE, monFile);
  // ------------------------------------------------------------------------


  // -----   Logger settings   ----------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel(logLevel.Data());
  FairLogger::GetLogger()->SetLogVerbosityLevel(logVerbosity.Data());
  // ------------------------------------------------------------------------


  // ----   Make Reco Events   ----------------------------------------------
  // ---- This is required if the input is in DigiEvent format
  auto makeEvents = std::make_unique<CbmTaskMakeRecoEvents>();
  LOG(info) << "-I- " << myName << ": Adding task " << makeEvents->GetName();
  run->AddTask(makeEvents.release());
  // ------------------------------------------------------------------------


  // -----   Local reconstruction in STS   ----------------------------------
  if (useSts) {
    //auto recoSts = std::make_unique<CbmRecoSts>(kCbmRecoEvent);
    //std::cout << "-I- " << myName << ": Adding task " << recoSts->GetName();

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
    Int_t stsAddress06 = CbmStsAddress::GetAddress(1, 0, 1, 1, 0, 0);  // U1 L0 M1  12 cm
    Int_t stsAddress07 = CbmStsAddress::GetAddress(1, 1, 1, 0, 0, 0);  // U1 L1 M0  6 cm
    Int_t stsAddress08 = CbmStsAddress::GetAddress(1, 1, 1, 1, 0, 0);  // U1 L1 M1  12 cm
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
    //run->AddTask(recoSts.release());
    std::cout << "-I- : Added task " << recoSts->GetName() << std::endl;
  }
  // ------------------------------------------------------------------------

  // -----   Local reconstruction in TOF   ----------------------------------

  if (useTof) {
    /*
    auto recoTof = std::make_unique<CbmRecoTof>(kCbmRecoEvent);
    std::cout << "-I- " << myName << ": Adding task " << recoTof->GetName();
    run->AddTask(recoTof.release());
    */
    // =========================================================================
    // ===                       Tof Hit Building                            ===
    // =========================================================================
    gROOT->LoadMacro("ini_Clusterizer.C");
    int calMode      = 93;
    int calSel       = 1;
    int calSm        = 1;
    int RefSel       = 1;
    TString cFileId  = "1588.50.6.0.2";
    int iCalSet      = 12002002;
    bool bOut        = kFALSE;
    int iSel2        = -1;
    double dDeadtime = 50.;
    TString cCalId   = "1588.50.6.0.2";
    Char_t* cCmd     = Form("ini_Clusterizer(%d,%d,%d,%d,\"%s\",%d,%d,%d,%f,\"%s\")", calMode, calSel, calSm, RefSel,
                        cFileId.Data(), iCalSet, (Int_t) bOut, iSel2, dDeadtime, cCalId.Data());
    cout << "<I> " << cCmd << endl;
    gInterpreter->ProcessLine(cCmd);

    // =========================================================================
    // ===                       Tof Tracking                                ===
    // =========================================================================
    int iSel           = 12022;
    int iTrackingSetup = 3;
    int iGenCor        = 3;
    double dScalFac    = 2.;
    double dChi2Lim2   = 5.;
    bool bUseSigCalib  = kFALSE;
    int iCalOpt        = 1;
    int iTrkPar        = 3;
    gROOT->LoadMacro("ini_trks.C");
    cCmd = Form("ini_trks(%d,%d,%d,%6.2f,%8.1f,\"%s\",%d,%d,%d)", iSel, iTrackingSetup, iGenCor, dScalFac, dChi2Lim2,
                cCalId.Data(), (Int_t) bUseSigCalib, iCalOpt, iTrkPar);
    cout << "<I> " << cCmd << endl;
    gInterpreter->ProcessLine(cCmd);

    CbmTofFindTracks* tofFindTracks = CbmTofFindTracks::Instance();
    Int_t iNStations                = tofFindTracks->GetNStations();
  }

  // ------------------------------------------------------------------------


  // -----  Parameter database   --------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Set runtime DB" << std::endl;
  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  FairParRootFileIo* parIo1  = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.Data(), "UPDATE");
  rtdb->setFirstInput(parIo1);
  if (!parFileList->IsEmpty()) {
    parIo2->open(parFileList, "in");
    rtdb->setSecondInput(parIo2);
  }
  // ------------------------------------------------------------------------


  // -----   Run initialisation   -------------------------------------------
  //std::cout << std::endl;
  //std::cout << "-I- " << myName << ": Initialise run" << std::endl;
  //run->Init();
  //rtdb->setOutput(parIo1);
  //rtdb->saveOutput();
  //rtdb->print();
  // ------------------------------------------------------------------------


  // -----   Start run   ----------------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Starting run" << std::endl;
  //run->Run(firstTimeSlice, nTimeSlices);
  // ------------------------------------------------------------------------

  FairEventManager* fMan = new FairEventManager();

  CbmEvDisTracks* Tracks = new CbmEvDisTracks("Tof Tracks", 1, kFALSE,
                                              kTRUE);  //name, verbosity, RnrChildren points, RnrChildren track
                                                       //  CbmEvDisTracks *Tracks =  new CbmEvDisTracks("Tof Tracks",1);
  fMan->AddTask(Tracks);
  CbmPixelHitSetDraw* TofUHits = new CbmPixelHitSetDraw("TofUHit", kRed, kOpenCross);
  fMan->AddTask(TofUHits);
  CbmPointSetArrayDraw* TofHits =
    new CbmPointSetArrayDraw("TofHit", 1, 1, 4,
                             kTRUE);  //name, colorMode, markerMode, verbosity, RnrChildren
  //  CbmPixelHitSetDraw *TofHits = new CbmPixelHitSetDraw ("TofHit", kRed, kOpenCircle, 4);// kFullSquare);
  fMan->AddTask(TofHits);

  TGeoVolume* top = gGeoManager->GetTopVolume();
  gGeoManager->SetVisOption(1);
  gGeoManager->SetVisLevel(5);
  TObjArray* allvolumes = gGeoManager->GetListOfVolumes();
  //cout<<"GeoVolumes  "  << gGeoManager->GetListOfVolumes()->GetEntries()<<endl;
  for (Int_t i = 0; i < allvolumes->GetEntries(); i++) {
    TGeoVolume* vol = (TGeoVolume*) allvolumes->At(i);
    TString name    = vol->GetName();
    //    cout << " GeoVolume "<<i<<" Name: "<< name << endl;
    vol->SetTransparency(90);
  }
  //  gGeoManager->SetVisLevel(3);
  //  top->SetTransparency(80);
  //  top->Draw("ogl");

  //  fMan->Init(1,4,10000);
  fMan->Init(1, 5);

  cout << "customize TEveManager gEve " << gEve << endl;
  gEve->GetDefaultGLViewer()->SetClearColor(kYellow - 10);
  TGLViewer* v       = gEve->GetDefaultGLViewer();
  TGLAnnotation* ann = new TGLAnnotation(v, input, 0.01, 0.98);
  ann->SetTextSize(0.03);  // % of window diagonal
  ann->SetTextColor(4);
  //  gEve->TEveProjectionAxes()->SetDrawOrigin(kTRUE);
  {  // from readCurrentCamera(const char* fname)
    TGLCamera& c      = gEve->GetDefaultGLViewer()->CurrentCamera();
    const char* fname = "Cam.sav";
    TFile* f          = TFile::Open(fname, "READ");
    if (!f) return;
    if (f->GetKey(c.ClassName())) {
      f->GetKey(c.ClassName())->Read(&c);
      c.IncTimeStamp();
      gEve->GetDefaultGLViewer()->RequestDraw();
    }
  }

  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  FairMonitor::GetMonitor()->Print();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Output file is    " << outFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << std::endl;
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


  // -----   This is to prevent a malloc error when exiting ROOT   ----------
  // The source of the error is unknown. Related to TGeoManager.
  //RemoveGeoManager();
  // ------------------------------------------------------------------------

}  // End of main macro function
