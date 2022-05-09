/* Copyright (C) 2018-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer], David Emschermann */

// clang-format off

// --------------------------------------------------------------------------
//
// Macro for standard transport simulation in mCBM using UrQMD input and GEANT3
//
// V. Friese   15/07/2018
//
// The output file will be named [output].tra.root.
// A parameter file [output].par.root will be created.
// The geometry (TGeoManager) will be written into [output].geo.root.
//
// Specify the input file by the last argument. If none is specified,
// a default input file distributed with the source code will be used.
// --------------------------------------------------------------------------

void SetTrack(CbmTransport*, Double_t, Int_t, Double_t, Double_t, Double_t);

void mcbm_transport(Int_t nEvents = 10,
                    //                  const char* setupName = "mcbm_beam_2022_08",
                    //                  const char* setupName = "mcbm_beam_2022_07",
                    //                  const char* setupName = "mcbm_beam_2022_06",
                    //                  const char* setupName = "mcbm_beam_2022_05",
                    //                  const char* setupName = "mcbm_beam_2022_04",
                    //                  const char* setupName = "mcbm_beam_2022_03",
                    //                  const char* setupName = "mcbm_beam_2022_02",
                    //                  const char* setupName = "mcbm_beam_2022_01",
                    const char* setupName = "mcbm_beam_2022_06_16_gold",
                    //                  const char* setupName = "mcbm_beam_2022_05_23_nickel",
                    //                  const char* setupName = "mcbm_beam_2022_03_28_uranium",
                    //                  const char* setupName = "mcbm_beam_2022_03_27_iron",
                    //                  const char* setupName = "mcbm_beam_2022_03_22_iron",
                    //                  const char* setupName = "mcbm_beam_2022_03_20_iron",
                    //                  const char* setupName = "mcbm_beam_2022_03_09_carbon",
                    //                  const char* setupName = "mcbm_beam_2021_07_surveyed",
                    //                  const char* setupName = "mcbm_beam_2021_04",
                    //                  const char* setupName = "mcbm_beam_2021_03",
                    //                  const char* setupName = "mcbm_beam_2020_03",
                    //                  const char* setupName = "mcbm_beam_2019_11",
                    //                  const char* setupName = "mcbm_beam_2019_03",
                    //                  const char* setupName = "sis18_mcbm_25deg_long",
                    const char* output = "data/test", const char* inputFile = "", Bool_t overwrite = kTRUE)
{
  // --- Define the beam angle ----------------------------------------------
  Double_t beamRotY = 25.;
  // ------------------------------------------------------------------------

  // --- Define the target geometry -----------------------------------------
  //
  // The target is not part of the setup, since one and the same setup can
  // and will be used with different targets.
  // The target is constructed as a tube in z direction with the specified
  // diameter (in x and y) and thickness (in z). It will be placed at the
  // specified position as daughter volume of the volume present there. It is
  // in the responsibility of the user that no overlaps or extrusions are
  // created by the placement of the target.
  //
  TString targetElement = "Gold";
  Double_t targetPosX   = 0.;  // target x position in global c.s. [cm]
  Double_t targetPosY   = 0.;  // target y position in global c.s. [cm]
  Double_t targetPosZ   = 0.;  // target z position in global c.s. [cm]

  //  Double_t targetThickness = 0.1;    // full thickness in cm
  //  Double_t targetDiameter  = 0.5;    // diameter in cm
  //  Double_t targetRotY      = 25.;    // target rotation angle around the y axis [deg]

  Double_t targetThickness = 0.025;  // mCBM thin gold target 0.25 mm = 0.025 cm thickness
  Double_t targetDiameter  = 1.5;    // mCBM target width 15 mm = 1.5 cm
  //  Double_t targetDiameter  = 0.1;      // set small target for window acceptance plots
  Double_t targetRotY = beamRotY;  // target rotation angle around the y axis [deg]
  // ------------------------------------------------------------------------

  // --- Logger settings ----------------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("VERYHIGH");
  // ------------------------------------------------------------------------


  // -----   Environment   --------------------------------------------------
  TString myName = "mcbm_transport";               // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------


  // -----   In- and output file names   ------------------------------------
  TString dataset(output);
  TString outFile = dataset + ".tra.root";
  TString parFile = dataset + ".par.root";
  TString geoFile = dataset + ".geo.root";
  std::cout << std::endl;
  TString defaultInputFile = srcDir + "/input/urqmd.agag.1.65gev.centr.00001.root";
  TString inFile;
  if (dataset.Contains("lam")) { std::cout << "-I- " << myName << ": Generate Lambda " << std::endl; }
  else {
    if (strcmp(inputFile, "") == 0) inFile = defaultInputFile;
    else
      inFile = inputFile;
    std::cout << "-I- " << myName << ": Using input file " << inFile << std::endl;
  }
  // ------------------------------------------------------------------------


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // --- Transport run   ----------------------------------------------------
  CbmTransport run;

  // clang-format off

  // DE  run.AddInput(new FairParticleGenerator(2212, 1, 0., 0., 1.));  // single proton along beam axis

  // ACC // geometrical acceptance
  //
  // ACC  Double_t stszoff = 0.;  // nominal
  // IRO  Double_t stszoff = 5.;  // Iron Carbon 2022
  // CAR  Double_t stszoff = 7.;  // July 2021 and Carbon 2022
  // nominal
  //
  // ACC // mSTS station 0
  // ACC   SetTrack(&run, beamRotY,-13, -5.9, +5.8, 28.5 + stszoff);
  // ACC   SetTrack(&run, beamRotY,-13, -5.9,  0.0, 28.5 + stszoff);
  // ACC   SetTrack(&run, beamRotY,-13, -5.9, -5.8, 28.5 + stszoff);
  // ACC //
  // ACC   SetTrack(&run, beamRotY, 11, -2.9, +8.8, 41.5 + stszoff);
  // ACC   SetTrack(&run, beamRotY, 11, -2.9,  0.0, 41.5 + stszoff);
  // ACC   SetTrack(&run, beamRotY, 11, -2.9, -8.8, 41.5 + stszoff);
  // ACC //
  // ACC   SetTrack(&run, beamRotY, 11,  0.0, +5.8, 28.5 + stszoff);
  // ACC   SetTrack(&run, beamRotY, 11,  0.0,  0.0, 28.5 + stszoff);
  // ACC   SetTrack(&run, beamRotY, 11,  0.0, -5.8, 28.5 + stszoff);
  // ACC //
  // ACC   SetTrack(&run, beamRotY, 11, +2.9, +8.8, 41.5 + stszoff);
  // ACC   SetTrack(&run, beamRotY, 11, +2.9,  0.0, 41.5 + stszoff);
  // ACC   SetTrack(&run, beamRotY, 11, +2.9, -8.8, 41.5 + stszoff);
  // ACC //
  // ACC   SetTrack(&run, beamRotY,-11, +5.9, +5.8, 28.5 + stszoff);
  // ACC   SetTrack(&run, beamRotY,-11, +5.9,  0.0, 28.5 + stszoff);
  // ACC   SetTrack(&run, beamRotY,-11, +5.9, -5.8, 28.5 + stszoff);
  //
  // TOFDS  SetTrack(&run, beamRotY,-13, 71, +5.8, 235);
  // TOFDS  SetTrack(&run, beamRotY,-13, 71,  0.0, 235);
  // TOFDS  SetTrack(&run, beamRotY,-13, 71, -5.8, 235);
  // TOFDS
  // TOFDS  SetTrack(&run, beamRotY,-13, 35, +5.8, 242);
  // TOFDS  SetTrack(&run, beamRotY,-13, 35,  0.0, 242);
  // TOFDS  SetTrack(&run, beamRotY,-13, 35, -5.8, 242);
  //
  // WIN   //  x : cos(25.*acos(-1.)/180.) *  -4.25              : x =  -3.852 cm
  // WIN   //  z : sin(25.*acos(-1.)/180.) *  -4.25 + 15.2 + 0.3 : z =  13.704 cm
  // WIN   //   SetTrack(&run, 0, 13, -3.852, 0.0, 13.704);
  // WIN   SetTrack(&run, 0, 13, -8.0, +5.9, 27.5);
  // WIN   SetTrack(&run, 0, 13, -8.0,  0.0, 27.5);
  // WIN   SetTrack(&run, 0, 13, -8.0, -5.9, 27.5);
  // WIN   //
  // WIN   //  x : cos(25.*acos(-1.)/180.) * -15.75              : x = -14.274 cm
  // WIN   //  z : sin(25.*acos(-1.)/180.) * -15.75 + 15.2 + 0.3 : z =   8.843 cm
  // WIN   //   SetTrack(&run, 0,-13, -14.274, 0.0, 8.843);
  // WIN   SetTrack(&run, 0, 13, -41.5, +5.9, 27.5);
  // WIN   SetTrack(&run, 0, 13, -41.5,  0.0, 27.5);
  // WIN   SetTrack(&run, 0, 13, -41.5, -5.9, 27.5);
  //
  // ACC // mSTS station 1
  // ACC   SetTrack(&run, beamRotY,-13, -8.9, +8.7, 42.5);
  // ACC   SetTrack(&run, beamRotY,-13, -8.9,  0.0, 42.5);
  // ACC   SetTrack(&run, beamRotY,-13, -8.9, -8.7, 42.5);
  // ACC //
  // ACC   SetTrack(&run, beamRotY, 11,  0.0, +8.7, 42.5);
  // ACC   SetTrack(&run, beamRotY, 11,  0.0,  0.0, 42.5);
  // ACC   SetTrack(&run, beamRotY, 11,  0.0, -8.7, 42.5);
  // ACC //
  // ACC   SetTrack(&run, beamRotY,-11, +8.9, +8.7, 42.5);
  // ACC   SetTrack(&run, beamRotY,-11, +8.9,  0.0, 42.5);
  // ACC   SetTrack(&run, beamRotY,-11, +8.9, -8.7, 42.5);
  //
  // STS // mSTS 201903 active area
  // STS   SetTrack(&run, beamRotY,-11, -2.1, -5.9, 27.5);
  // STS   SetTrack(&run, beamRotY,-11, -2.5, -3.0, 27.5);
  // STS   SetTrack(&run, beamRotY,-11, -2.9, -0.1, 27.5);
  // STS   SetTrack(&run, beamRotY,-11, -2.9, -3.0, 27.5);
  // STS   SetTrack(&run, beamRotY,-11, -2.9, -5.9, 27.5);
  // STS
  // STS   SetTrack(&run, beamRotY, 11, -5.1, -5.9, 27.5);
  // STS   SetTrack(&run, beamRotY, 11, -5.5, -3.0, 27.5);
  // STS   SetTrack(&run, beamRotY, 11, -5.9, -0.1, 27.5);
  // STS   SetTrack(&run, beamRotY, 11, -5.9, -3.0, 27.5);
  // STS   SetTrack(&run, beamRotY, 11, -5.9, -5.9, 27.5);

  // clang-format on

  // comment the following line to remove target interaction
  run.AddInput(inFile);
  run.SetOutFileName(outFile, overwrite);
  run.SetParFileName(parFile);
  run.SetGeoFileName(geoFile);
  run.LoadSetup(setupName);
  run.SetField(new CbmFieldConst());
  run.SetTarget(targetElement, targetThickness, targetDiameter, targetPosX, targetPosY, targetPosZ,
                targetRotY * TMath::DegToRad());
  run.SetBeamPosition(0., 0., 0.1, 0.1);  // Beam width 1 mm is assumed
  run.SetBeamAngle(beamRotY * TMath::DegToRad(), 0.);
  if (nEvents <= 10)  // store only for small number of events
    run.StoreTrajectories();
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
}


void SetTrack(CbmTransport* run, Double_t beamRotY, Int_t pdgid, Double_t x, Double_t y, Double_t z)
{
  TVector3 v;
  v.SetXYZ(x, y, z);
  v.RotateY(-beamRotY * acos(-1.) / 180.);
  cout << "X " << v.X() << " Y " << v.Y() << " Z " << v.Z() << endl;

  run->AddInput(new FairParticleGenerator(pdgid, 1, v.X(), v.Y(), v.Z()));  // single electron along beam axis
}
