// --------------------------------------------------------------------------
//
// Macro for reconstruction of simulated events with standard settings
//
// HitProducers in MVD, RICH, TRD, TOF, ECAL
// Digitizer and HitFinder in STS
// FAST MC for ECAL
// STS track finding and fitting (L1 / KF)
// TRD track finding and fitting (L1 / KF)
// RICH ring finding (ideal) and fitting
// Global track finding (ideal), rich assignment
// Primary vertex finding (ideal)
// Matching of reconstructed and MC tracks in STS, RICH and TRD
//
// V. Friese   24/02/2006
// Version     04/03/2015 (V. Friese)
//
// --------------------------------------------------------------------------

void eventDisplay_reco(TString cSys          = "lam",
                       TString cEbeam        = "2.5gev",
                       TString cCentr        = "-",
                       Int_t iRun            = 0,
                       Int_t parSet          = 0,
                       const char* setupName = "sis18_mcbm") {
  // -----   Environment   --------------------------------------------------
  TString myName = "run_reco_nh";  // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ========================================================================
  //          Adjust this part according to your requirements

  // Verbosity level (0=quiet, 1=event level, 2=track level, 3=debug)
  Int_t iVerbose = 0;

  TString outDir = "data/";
  TString inFile = outDir + setupName + "_" + cSys + "." + cEbeam + "." + cCentr
                   + ".mc." + Form("%05d", iRun)
                   + ".root";  // Input file (MC events)
  TString parFile = outDir + setupName + "_" + cSys + "." + cEbeam + "."
                    + cCentr + ".params." + Form("%05d", iRun)
                    + ".root";  // Parameter file
  TString outFile = outDir + setupName + "_" + cSys + "." + cEbeam + "."
                    + cCentr + ".evt." + Form("%05d", iRun)
                    + ".root";  // Output file

  //FairLogger::GetLogger()->SetLogScreenLevel("WARNING");
  //FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  //FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");
  FairLogger::GetLogger()->SetLogScreenLevel("DEBUG1");
  FairLogger::GetLogger()->SetLogVerbosityLevel("MEDIUM");

  // -----   Load the geometry setup   -------------------------------------
  std::cout << std::endl;

  TString setupFile  = srcDir + "/geometry/setup/setup_" + setupName + ".C";
  TString setupFunct = "setup_";
  setupFunct         = setupFunct + setupName + "()";
  std::cout << "-I- mcbm_reco: Loading macro " << setupFile << std::endl;
  gROOT->LoadMacro(setupFile);
  gROOT->ProcessLine(setupFunct);
  CbmSetup* setup = CbmSetup::Instance();
  // ------------------------------------------------------------------------


  // -----   Parameter files as input to the runtime database   -------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Defining parameter files " << std::endl;
  TList* parFileList = new TList();
  TString geoTag;

  // - TRD digitisation parameters
  if (setup->GetGeoTag(kTrd, geoTag)) {
    TObjString* trdFile =
      new TObjString(srcDir + "/parameters/trd/trd_" + geoTag + ".digi.par");
    parFileList->Add(trdFile);
    std::cout << "-I- " << myName << ": Using parameter file "
              << trdFile->GetString() << std::endl;
  }

  // - TOF digitisation parameters
  if (setup->GetGeoTag(kTof, geoTag)) {
    TObjString* tofBdfFile =
      new TObjString(srcDir + "/parameters/tof/tof_" + geoTag + ".digibdf.par");
    parFileList->Add(tofBdfFile);
    std::cout << "-I- " << myName << ": Using parameter file "
              << tofBdfFile->GetString() << std::endl;
  }
  // ------------------------------------------------------------------------

  // In general, the following parts need not be touched
  // ========================================================================


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------


  // -----   Input file   ---------------------------------------------------
  std::cout << std::endl;
  std::cout << "-I- " << myName << ": Using input file " << inFile << std::endl;
  // ------------------------------------------------------------------------


  // -----   FairRunAna   ---------------------------------------------------
  FairRunAna* run = new FairRunAna();
  run->SetInputFile(inFile);
  run->SetOutputFile(outFile);
  run->SetGenerateRunInfo(kTRUE);
  run->SetGenerateRunInfo(kTRUE);
  Bool_t hasFairMonitor = Has_Fair_Monitor();
  if (hasFairMonitor) FairMonitor::GetMonitor()->EnableMonitor(kTRUE);
  // ------------------------------------------------------------------------

  // ----- MC Data Manager   ------------------------------------------------
  CbmMCDataManager* mcManager = new CbmMCDataManager("MCManager", 1);
  mcManager->AddFile(inFile);
  run->AddTask(mcManager);
  // ------------------------------------------------------------------------


  // -----   Digitisers   ---------------------------------------------------
  std::cout << std::endl;
  TString macroName = gSystem->Getenv("VMCWORKDIR");
  macroName += "/macro/mcbm/modules/digitize.C";
  std::cout << "Loading macro " << macroName << std::endl;
  gROOT->LoadMacro(macroName);
  gROOT->ProcessLine("digitize()");
  // ------------------------------------------------------------------------


  // -----   Reconstruction tasks   -----------------------------------------
  std::cout << std::endl;
  macroName = srcDir + "/macro/mcbm/modules/reconstruct.C";
  std::cout << "Loading macro " << macroName << std::endl;
  gROOT->LoadMacro(macroName);
  Bool_t recoSuccess = gROOT->ProcessLine("reconstruct()");
  if (!recoSuccess) {
    std::cerr << "-E-" << myName << ": error in executing " << macroName
              << std::endl;
    return;
  }

  // =========================================================================
  // ===                    Matching to Monte-carlo                        ===
  // =========================================================================
  CbmMatchRecoToMC* matchTask = new CbmMatchRecoToMC();
  run->AddTask(matchTask);
  // Digitizer/custerizer testing
  CbmTofHitFinderQa* tofQa = new CbmTofHitFinderQa("TOF QA");
  tofQa->SetHistoFileName("TofQA.root");
  run->AddTask(tofQa);


  CbmHadronAnalysis* HadronAna = new CbmHadronAnalysis();  // interpret event
  HadronAna->SetRecSec(kTRUE);  // enable lambda reconstruction
  switch (parSet) {
    case 0:  // with background
      HadronAna->SetDistPrimLim(
        1.2);  // Max Tof-Sts trans distance for primaries
      HadronAna->SetDistPrimLim2(
        0.3);  // Max Sts-Sts trans distance for primaries
      HadronAna->SetDistSecLim2(
        0.3);  // Max Sts-Sts trans distance from TOF direction for secondaries
      HadronAna->SetD0ProtLim(
        0.5);                       // Min impact parameter for secondary proton
      HadronAna->SetOpAngMin(0.1);  // Min opening angle for accepting pair
      HadronAna->SetDCALim(0.1);    // Max DCA for accepting pair
      HadronAna->SetVLenMin(
        5.);  // Min Lambda flight path length for accepting pair
      HadronAna->SetVLenMax(
        25.);  // Max Lambda flight path length for accepting pair
      HadronAna->SetDistTRD(
        10.);  // max accepted distance of Trd Hit from STS-TOF line
      HadronAna->SetTRDHmulMin(
        0.);  // min associated Trd Hits to Track candidates
      HadronAna->SetNMixedEvents(10);  // Number of events to be mixed with
      break;
    case 1:  // signal with background Ni+Ni
      HadronAna->SetDistPrimLim(
        1.);  // Max Tof-Sts trans distance for primaries
      HadronAna->SetDistPrimLim2(
        0.3);  // Max Sts-Sts trans distance for primaries
      HadronAna->SetDistSecLim2(
        0.3);  // Max Sts-Sts trans distance from TOF direction for secondaries
      HadronAna->SetD0ProtLim(
        0.4);                       // Min impact parameter for secondary proton
      HadronAna->SetOpAngMin(0.1);  // Min opening angle for accepting pair
      HadronAna->SetDCALim(0.1);    // Max DCA for accepting pair
      HadronAna->SetVLenMin(
        5.);  // Min Lambda flight path length for accepting pair
      HadronAna->SetVLenMax(
        25.);  // Max Lambda flight path length for accepting pair
      HadronAna->SetDistTRD(
        10.);  // max accepted distance of Trd Hit from STS-TOF line
      HadronAna->SetTRDHmulMin(
        0.);  // min associated Trd Hits to Track candidates
      HadronAna->SetNMixedEvents(10);  // Number of events to be mixed with
      break;
    case 2:  // signal with background Au+Au
      HadronAna->SetDistPrimLim(
        1.);  // Max Tof-Sts trans distance for primaries
      HadronAna->SetDistPrimLim2(
        0.3);  // Max Sts-Sts trans distance for primaries
      HadronAna->SetDistSecLim2(
        0.3);  // Max Sts-Sts trans distance from TOF direction for secondaries
      HadronAna->SetD0ProtLim(
        0.4);                       // Min impact parameter for secondary proton
      HadronAna->SetOpAngMin(0.1);  // Min opening angle for accepting pair
      HadronAna->SetDCALim(0.1);    // Max DCA for accepting pair
      HadronAna->SetVLenMin(
        8.);  // Min Lambda flight path length for accepting pair
      HadronAna->SetVLenMax(
        25.);  // Max Lambda flight path length for accepting pair
      HadronAna->SetDistTRD(
        10.);  // max accepted distance of Trd Hit from STS-TOF line
      HadronAna->SetTRDHmulMin(
        0.);  // min associated Trd Hits to Track candidates
      HadronAna->SetNMixedEvents(10);  // Number of events to be mixed with
      break;
    case 10:  // "0" with TRD Mul 1
      HadronAna->SetDistPrimLim(
        1.2);  // Max Tof-Sts trans distance for primaries
      HadronAna->SetDistPrimLim2(
        0.3);  // Max Sts-Sts trans distance for primaries
      HadronAna->SetDistSecLim2(
        0.3);  // Max Sts-Sts trans distance from TOF direction for secondaries
      HadronAna->SetD0ProtLim(
        0.5);                       // Min impact parameter for secondary proton
      HadronAna->SetOpAngMin(0.1);  // Min opening angle for accepting pair
      HadronAna->SetDCALim(0.1);    // Max DCA for accepting pair
      HadronAna->SetVLenMin(
        5.);  // Min Lambda flight path length for accepting pair
      HadronAna->SetVLenMax(
        25.);  // Max Lambda flight path length for accepting pair
      HadronAna->SetDistTRD(
        10.);  // max accepted distance of Trd Hit from STS-TOF line
      HadronAna->SetTRDHmulMin(
        1.);  // min associated Trd Hits to Track candidates
      HadronAna->SetNMixedEvents(10);  // Number of events to be mixed with
      break;
    case 20:  // "0" with TRD Mul 2
      HadronAna->SetDistPrimLim(
        1.2);  // Max Tof-Sts trans distance for primaries
      HadronAna->SetDistPrimLim2(
        0.3);  // Max Sts-Sts trans distance for primaries
      HadronAna->SetDistSecLim2(
        0.3);  // Max Sts-Sts trans distance from TOF direction for secondaries
      HadronAna->SetD0ProtLim(
        0.5);                       // Min impact parameter for secondary proton
      HadronAna->SetOpAngMin(0.1);  // Min opening angle for accepting pair
      HadronAna->SetDCALim(0.1);    // Max DCA for accepting pair
      HadronAna->SetVLenMin(
        5.);  // Min Lambda flight path length for accepting pair
      HadronAna->SetVLenMax(
        25.);  // Max Lambda flight path length for accepting pair
      HadronAna->SetDistTRD(
        10.);  // max accepted distance of Trd Hit from STS-TOF line
      HadronAna->SetTRDHmulMin(
        2.);  // min associated Trd Hits to Track candidates
      HadronAna->SetNMixedEvents(10);  // Number of events to be mixed with
      break;
    default:
      cout << "Cut value set " << parSet << " not existing, stop macro "
           << endl;
      return;
  }
  run->AddTask(HadronAna);

  // -----  Parameter database   --------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Set runtime DB" << std::endl;
  FairRuntimeDb* rtdb        = run->GetRuntimeDb();
  FairParRootFileIo* parIo1  = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(parFile.Data());
  parIo2->open(parFileList, "in");
  rtdb->setFirstInput(parIo1);
  rtdb->setSecondInput(parIo2);
  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  // ------------------------------------------------------------------------

  FairEventManager* fMan = new FairEventManager();
  FairMCTracks* Track    = new FairMCTracks("Monte-Carlo Tracks");

  FairMCPointDraw* MvdPoint =
    new FairMCPointDraw("MvdPoint", kBlack, kFullSquare);
  FairMCPointDraw* StsPoint =
    new FairMCPointDraw("StsPoint", kGreen, kFullSquare);
  FairMCPointDraw* MuchPoint =
    new FairMCPointDraw("MuchPoint", kOrange, kFullSquare);
  FairMCPointDraw* RichPoint =
    new FairMCPointDraw("RichPoint", kRed, kFullSquare);
  FairMCPointDraw* TrdPoint =
    new FairMCPointDraw("TrdPoint", kBlue, kFullSquare);
  FairMCPointDraw* TofPoint =
    new FairMCPointDraw("TofPoint", kGreen, kFullSquare);
  FairMCPointDraw* EcalPoint =
    new FairMCPointDraw("EcalPoint", kYellow, kFullSquare);
  FairMCPointDraw* RefPlanePoint =
    new FairMCPointDraw("RefPlanePoint", kPink, kFullSquare);

  fMan->AddTask(Track);

  fMan->AddTask(MvdPoint);
  fMan->AddTask(StsPoint);
  fMan->AddTask(MuchPoint);
  fMan->AddTask(RichPoint);
  fMan->AddTask(TrdPoint);
  fMan->AddTask(TofPoint);
  fMan->AddTask(EcalPoint);
  fMan->AddTask(RefPlanePoint);
  CbmPixelHitSetDraw* StsHits =
    new CbmPixelHitSetDraw("StsHit", kRed, kOpenCircle);  // kFullSquare);
  fMan->AddTask(StsHits);
  CbmPixelHitSetDraw* TrdHits =
    new CbmPixelHitSetDraw("TrdHit", kRed, kOpenCircle);  // kFullSquare);
  fMan->AddTask(TrdHits);
  CbmPixelHitSetDraw* TofHits =
    new CbmPixelHitSetDraw("TofHit", kRed, kOpenCircle);  // kFullSquare);
  fMan->AddTask(TofHits);
  CbmPixelHitSetDraw* TofUHits =
    new CbmPixelHitSetDraw("TofUHit", kRed, kOpenCross);
  fMan->AddTask(TofUHits);
  CbmEvDisTracks* Tracks = new CbmEvDisTracks("Tof Tracks", 1);
  Tracks->SetVerbose(4);
  fMan->AddTask(Tracks);

  //  fMan->Init(1,4,10000);
  //  fMan->Init(1,5,10000);  // make STS visible by default
  fMan->Init(1, 6, 10000);  // make MVD visible by default

  //TGeoVolume* top = gGeoManager->GetTopVolume();
  //gGeoManager->SetVisOption(1);
  //gGeoManager->SetVisLevel(5);
  TObjArray* allvolumes = gGeoManager->GetListOfVolumes();
  //cout<<"GeoVolumes  "  << gGeoManager->GetListOfVolumes()->GetEntries()<<endl;
  for (Int_t i = 0; i < allvolumes->GetEntries(); i++) {
    TGeoVolume* vol = (TGeoVolume*) allvolumes->At(i);
    TString name    = vol->GetName();
    cout << " GeoVolume " << i << " Name: " << name << endl;
    vol->SetTransparency(99);
  }

  cout << "gEve " << gEve << endl;
  gEve->GetDefaultGLViewer()->SetClearColor(kYellow - 10);
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
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file is " << outFile << endl;
  cout << "Parameter file is " << parFile << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
  cout << endl;
  // ------------------------------------------------------------------------

  //  delete run;

  cout << " Test passed" << endl;
  cout << " All ok " << endl;
  RemoveGeoManager();
}
