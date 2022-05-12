/* Copyright (C) 2018-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Valentina Akishina  [committer], Volker Friese, David Emschermann */

// --------------------------------------------------------------------------
//
// V. Friese   15/07/2018
//
// The output file will be named [output].tra.root.
// A parameter file [output].par.root will be created.
// The geometry (TGeoManager) will be written into trd.geo.root.
//
// T. Balog, 11.05.2015
//
// Macro for transport simulation with the TRD
// to determine the material budget of the TRD stations.
// ROOTinos parallel to the z axis will be transported,
// using the RadLenRegister functionality of FairRunSim.
// From the output MC file, the material budget can be
// determined with the macro matbudget_ana.C
// --------------------------------------------------------------------------

void matbudget_sim(const char* setupName = "mcbm_beam_2022_08", const char* output = "matbudget",
                   Int_t nEvents = 1000000)
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
  // ------------------------------------------------------------------------


  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // --- Transport run   ----------------------------------------------------
  CbmTransport run;


  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  // run.SetGenerator(primGen);

  // --- Primary particles
  // Generated are ROOTinos (PDG=0) in z direction (one per event),
  // starting at z = 0.
  // The starting points in x and y are chosen such as to illuminate the STS.
  FairBoxGenerator* boxGen = new FairBoxGenerator(0, 1);
  //boxGen->SetBoxXYZ(-100., -100., 100., 100., 0.);  // STS SIS100
  boxGen->SetBoxXYZ(-500., -500., 500., 500., 0.);  // TRD
  //boxGen->SetBoxXYZ(-15., -15., 15., 15., 0.);  // STS mCBM
  boxGen->SetPRange(0.1, 0.5);
  boxGen->SetThetaRange(0., 0.);
  boxGen->SetPhiRange(0., 360.);

  primGen->AddGenerator(boxGen);

  run.RegisterRadLength(kTRUE);

  // comment the following line to remove target interaction
  run.AddInput(boxGen);
  run.SetOutFileName(outFile);
  run.SetParFileName(parFile);
  run.SetGeoFileName(geoFile);
  run.LoadSetup(setupName);
  run.SetField(new CbmFieldConst());
  // run.SetTarget(targetElement, targetThickness, targetDiameter, targetPosX, targetPosY, targetPosZ,
  //             targetRotY * TMath::DegToRad());
  //run.SetBeamPosition(0., 0., 0.1, 0.1);  // Beam width 1 mm is assumed
  // run.SetBeamAngle(beamRotY * TMath::DegToRad(), 0.);
  //run.StoreTrajectories();
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
