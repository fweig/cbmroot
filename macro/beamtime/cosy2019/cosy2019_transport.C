// --------------------------------------------------------------------------
//
// Macro for standard transport simulation in COSY 2019 using ion generator and GEANT3
//
// P.-A. Loizeau   25/09/2019
//
// The output file will be named [output].tra.root.
// A parameter file [output].par.root will be created.
// The geometry (TGeoManager) will be written into [output].geo.root.
//
// 1 proton per event, no target
// Specify the beam spot properties, angular spread and momentum.
// --------------------------------------------------------------------------

void cosy2019_transport( Int_t nEvents = 1,
                         Double_t dMinX = -0.6, Double_t dMaxX = 0.6,
                         Double_t dMinY = -0.6, Double_t dMaxY = 0.6,
                         Double_t dMinP = 2.71,   Double_t dMaxP = 2.74,
                         Double_t dThetaMin = 0.0, Double_t dThetaMax = 1.0,
                         const char* output = "data/cosy2019" )
{
  // --- Logger settings ----------------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  // ------------------------------------------------------------------------

  // -----   Environment   --------------------------------------------------
  TString myName = "cosy2019_transport.C";  // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------

  // -----   In- and output file names   ------------------------------------
  TString dataset(output);
  TString outFile = dataset + ".tra.root";
  TString parFile = dataset + ".par.root";
  TString geoFile = dataset + ".geo.root";
  std::cout << std::endl;
  // ------------------------------------------------------------------------

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  // --- Transport run   ----------------------------------------------------
  CbmTransport run;

  // --- Primary protons (PDG=2212) generator , 1 per events.
  FairBoxGenerator* boxGen = new FairBoxGenerator(2212,  1);

  // The starting points in x and y or the angles are chosen based on previous beamtimes
  // Cartesian coord: in z direction, starting at z = 0 for the beam pipe exit.
  boxGen->SetBoxXYZ( dMinX, dMinY, dMaxX, dMaxY, 0.);
  boxGen->SetPRange( dMinP, dMaxP); // arbitrary, not sure if any effect to expect here
  boxGen->SetThetaRange( dThetaMin, dThetaMax);
  boxGen->SetPhiRange(0.,360.);

  run.AddInput(boxGen);
  // ------------------------------------------------------------------------

  run.SetOutFileName(outFile);
  run.SetParFileName(parFile);
  run.SetGeoFileName(geoFile);
  run.LoadSetup("cosy_beam_2019_11");
/*
// -----   Load the geometry setup   -------------------------------------
  std::cout << std::endl;
  // -----  Geometry Tags  --------------------------------------------------
  TString hodoGeoTag       = "hodo_v19a_cosy";    // 2019 COSY beamtime
  // ------------------------------------------------------------------------

  // -----  Create setup  ---------------------------------------------------
  CbmSetup* setup = CbmSetup::Instance();
  if ( ! setup->IsEmpty() ) {
    std::cout << "-W- setup_sis18_mcbm: overwriting existing setup"
              << setup->GetTitle() << std::endl;
    setup->Clear();
  }
  setup->SetTitle("Hodoscopes LAB Setup");
  setup->SetModule(kSts,  hodoGeoTag);
  // ------------------------------------------------------------------------

  // -----   Screen output   ------------------------------------------------
  setup->Print();
  // ------------------------------------------------------------------------
*/

  // ------------------------------------------------------------------------
  run.SetField(new CbmFieldConst());

  run.SetVertexSmearZ(kFALSE);
  run.SetBeamPosition(0., 0., 0., 0., 0.);
  run.SetBeamAngle(0., 0., 0., 0.);
  run.ForceVertexInTarget(kFALSE);

//  run.StoreTrajectories();
  // ------------------------------------------------------------------------

  // ------------------------------------------------------------------------
  run.Run(nEvents);
  // ------------------------------------------------------------------------

  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Output file is "    << outFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Geometry file is "  << geoFile << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime
      << "s" << std::endl << std::endl;
  // ------------------------------------------------------------------------


  // -----   Resource monitoring   ------------------------------------------
  FairSystemInfo sysInfo;
  Float_t maxMemory=sysInfo.GetMaxMemory();
  std::cout << "<DartMeasurement name=\"MaxMemory\" type=\"numeric/double\">";
  std::cout << maxMemory;
  std::cout << "</DartMeasurement>" << std::endl;

  Float_t cpuUsage=ctime/rtime;
  std::cout << "<DartMeasurement name=\"CpuLoad\" type=\"numeric/double\">";
  std::cout << cpuUsage;
  std::cout << "</DartMeasurement>" << std::endl;


  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;

  // ------------------------------------------------------------------------
}
