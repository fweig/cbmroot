// --------------------------------------------------------------------------
//
// Macro for standard transport simulation using UrQMD input and GEANT3
//
// V. Friese   22/02/2007
//
// Version 2018-06-09
//
// For the setup (geometry and field), predefined setups can be chosen
// by the second argument. Available setups are in geometry/setup.
// The input is a file in UniGen format. It can be specified by the
// second last argument. If none is specified, a default input file
// distributed with the source code is selected.
// If the argument iDecay is specified, the corresponding decay mode
// from KFPartEfficiencies is enforced.
//
// The output file will be named [output].tra.root.
// A parameter file [output].par.root will be created.
// The geometry (TGeoManager) will be written into [output].geo.root.
// --------------------------------------------------------------------------


/** @brief kf_transport_new
 ** @param nEvents    Number of events from input to transport
 ** @param setupName  Name of predefined geometry setup
 ** @param output     Name of output data set
 ** @param inputFile  Name of input file
 ** @param iDecay     Decay mode from KFPartEfficiencies
 **/
void kf_transport_new(Int_t nEvents = 2, const char* setupName = "sis100_electron", const char* output = "test",
                      const char* inputFile = "", Int_t iDecay = -1)
{

  // --- Logger settings ----------------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  // ------------------------------------------------------------------------


  // -----   Environment   --------------------------------------------------
  TString myName = "kf_transport";                 // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------


  // -----   In- and output file names   ------------------------------------
  TString dataset(output);
  TString outFile = dataset + ".tra.root";
  TString parFile = dataset + ".par.root";
  TString geoFile = dataset + ".geo.root";
  std::cout << std::endl;
  TString defaultInputFile = srcDir + "/input/urqmd.auau.10gev.centr.root";
  TString inFile;
  if (strcmp(inputFile, "") == 0) inFile = defaultInputFile;
  else
    inFile = inputFile;
  std::cout << "-I- " << myName << ": Using input file " << inFile << std::endl;
  // ------------------------------------------------------------------------


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // --- Transport run   ----------------------------------------------------
  CbmTransport run;
  run.AddInput(inFile);
  run.SetOutFileName(outFile);
  run.SetParFileName(parFile);
  run.SetGeoFileName(geoFile);
  run.LoadSetup(setupName);
  run.SetTarget("Gold", 0.025, 2.5);
  run.SetBeamPosition(0., 0., 0.1, 0.1);
  // ------------------------------------------------------------------------


  // --- Force the desired decay for the chosen particle   ------------------
  if (iDecay > -1) {

    KFPartEfficiencies eff;
    Int_t pdgid = eff.partPDG[iDecay];
    std::cout << "-I- " << myName << ": Setting decay " << iDecay << " ( " << eff.partTitle[iDecay] << ", pdg " << pdgid
              << " )" << std::endl;

    // Check if particle is in TDatabasePDG. If yes, it is probably known to the VMC.
    // Unfortunately, there is no better check available
    if (!TDatabasePDG::Instance()->GetParticle(pdgid)) {
      auto particle = new FairParticle(pdgid,                         // PDG code
                                       eff.partTitle[iDecay].data(),  // name
                                       kPTHadron,                     // type
                                       eff.partMass[iDecay],          // mass
                                       eff.partCharge[iDecay],        // charge
                                       eff.partLifeTime[iDecay],      // life time
                                       "hadron",                      // type string
                                       0.,                            // width
                                       1, 1,
                                       0,  // spin, parity, conjugation
                                       1, 1,
                                       0,  // isospin, isospin-z, g-Parity
                                       0,
                                       1,        // lepton number, baryon number
                                       kFALSE);  // stable
      FairRunSim::Instance()->AddNewParticle(particle);
      std::cout << "-I- " << myName << ": Registering particle " << eff.partTitle[iDecay] << " with PDG code " << pdgid
                << " for transport." << std::endl;
    }  //? Not in PDG database

    // TODO: I am not particularly fond of the above code. KFPartEfficiencies uses
    // a different, privately defined PDG for different decay modes of the same particles.
    // This alone is dubious, because the PDG definition will not be available after
    // the transport stage. Moreover, many of the particles defined in KFPartEfficiencies
    // are not hadrons (there are mesons, nuclei and even hypernuclei). These will not
    // be transported correctly, in particular not be GEANT4.
    // The better way would be to define a common list of particles needed by CBM and
    // make them centrally available through TDatabasePDG.

    // TODO: The scheme will not work for eta and pi0, since their branching ratios are
    // re-defined afterwards. This was also the case in the old registerGeantDecays.C macro.


    // Force the decay specified in KFPartEfficiencies
    Int_t nDaughters = eff.GetNDaughters(iDecay);
    Int_t daughterPdg[nDaughters];
    for (Int_t iDaughter = 0; iDaughter < nDaughters; iDaughter++) {
      daughterPdg[iDaughter] = eff.GetDaughterPDG(iDecay, iDaughter);
    }  //# daughters
    run.SetDecayMode(pdgid, nDaughters, daughterPdg);

  }  //? iDecay > -1
  // ------------------------------------------------------------------------


  // -----   Execute transport run   ----------------------------------------
  run.Run(nEvents);
  // ------------------------------------------------------------------------


  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Output file is " << outFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << "s" << std::endl << std::endl;
  // ------------------------------------------------------------------------


  // -----   Resource monitoring   ------------------------------------------
  FairSystemInfo sysInfo;
  Float_t maxMemory = sysInfo.GetMaxMemory();
  std::cout << "<DartMeasurement name=\"MaxMemory\" type=\"numeric/double\">";
  std::cout << maxMemory;
  std::cout << "</DartMeasurement>" << std::endl;

  Float_t cpuUsage = ctime / rtime;
  std::cout << "<DartMeasurement name=\"CpuLoad\" type=\"numeric/double\">";
  std::cout << cpuUsage;
  std::cout << "</DartMeasurement>" << std::endl;


  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
  // ------------------------------------------------------------------------

}  // End of macro
