/** @file mcbm_transport_nh.C
  * @copyright Copyright (C) 2018-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Volker Friese, Florian Uhlig [committer] **/

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
// Includes needed for IDE
#if !defined(__CLING__)
#include "CbmTransport.h"

#include "FairSystemInfo.h"

#include "TStopwatch.h"
#endif

void SetTrack(CbmTransport*, Double_t, Int_t, Double_t, Double_t, Double_t);

void mcbm_transport_nh(Int_t nEvents = 10, const char* setupName = "mcbm_beam_2021_03",
                       //                  const char* setupName = "mcbm_beam_2019_11",
                       //                  const char* setupName = "mcbm_beam_2019_03",
                       //                  const char* setupName = "sis18_mcbm_25deg_long",
                       const char* output = "data/test", const char* inputFile = "")
{
  // --- Logger settings ----------------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("WARN");
  FairLogger::GetLogger()->SetLogVerbosityLevel("VERYHIGH");
  // ------------------------------------------------------------------------

  // -----   Environment   --------------------------------------------------
  TString myName = "mcbm_transport";               // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory
  // ------------------------------------------------------------------------

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
  TString Output(output);
  if (!Output.Contains("auau")) {
    if (Output.Contains("nini")) targetElement = "Nickel";
    else {
      if (Output.Contains("lam")) { std::cout << "Lambda signal simulation " << std::endl; }
      else {
        std::cout << "Collision system " << Output << " not known." << std::endl;
        exit(1);
      }
    }
  }
  /*
   if ( system.CompareTo("auau") ) {
    if ( ! system.CompareTo("nini") ) {
      targetElement   = "Nickel";
    } else {
      std::cout << "Collision syste " << system << " not known." << std::endl;
      exit(1);
    }
  }
  */

  Double_t targetPosX = 0.;  // target x position in global c.s. [cm]
  Double_t targetPosY = 0.;  // target y position in global c.s. [cm]
  Double_t targetPosZ = 0.;  // target z position in global c.s. [cm]

  //  Double_t targetThickness = 0.1;    // full thickness in cm
  //  Double_t targetDiameter  = 0.5;    // diameter in cm
  //  Double_t targetRotY      = 25.;    // target rotation angle around the y axis [deg]

  Double_t targetThickness = 0.025;  // mCBM thin gold target 0.25 mm = 0.025 cm thickness
  Double_t targetDiameter  = 1.5;    // mCBM target width 15 mm = 1.5 cm
  //  Double_t targetDiameter  = 0.1;      // set small target for window acceptance plots
  Double_t targetRotY = beamRotY;  // target rotation angle around the y axis [deg]
  // ------------------------------------------------------------------------

  // -----   In- and output file names   ------------------------------------
  TString dataset(output);
  TString outFile = dataset + ".tra.root";
  TString parFile = dataset + ".par.root";
  TString geoFile = dataset + ".geo.root";
  std::cout << " dataset: " << dataset << std::endl;

  // cleanup
  TString shcmd = "rm -v " + parFile + " " + outFile + " " + geoFile;
  gSystem->Exec(shcmd.Data());

  TString defaultInputFile = srcDir + "/input/urqmd.agag.1.65gev.centr.00001.root";
  TString inFile;

  CbmTransport run;

  if (dataset.Contains("lam")) {
    //(pdg,mul,px, py, pz, vx,vy,vz)
    Double_t pz = 2.;
    Int_t iL    = dataset.Index("gev");
    TString cp  = dataset(iL - 3, 3);  // 2 characters only
    pz          = cp.Atof();
    //std::cout<<"iL = "<<iL<<" "<<cp<<" "<<pz<<std::endl;
    //sscanf(cEbeam,"%lfgev",&pz);
    std::cout << "simulate single lambda with pz = " << pz << " from " << dataset << std::endl;
    //FairParticleGenerator *fPartGen= new FairParticleGenerator(3122, 1,0.0,0., pz, 0.,0.,0.); //lambda
    //  primGen->AddGenerator(fPartGen);

    SetTrack(&run, beamRotY, 3122, 0.0, 0.0, pz);
    std::cout << "-I- " << myName << ": Generate Lambda " << std::endl;
  }
  else {
    if (strcmp(inputFile, "") == 0) inFile = defaultInputFile;
    else
      inFile = inputFile;
    std::cout << "-I- " << myName << ": Using input file " << inFile << std::endl;
    run.AddInput(inFile);
  }

  std::cout << "-I- " << myName << ": Using output file " << outFile << std::endl;
  std::cout << "-I- " << myName << ": Using parameter file " << parFile << std::endl;
  std::cout << "-I- " << myName << ": Using geometry file " << geoFile << std::endl;
  // ------------------------------------------------------------------------

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // --- Transport run   ----------------------------------------------------

  run.SetOutFileName(outFile);
  run.SetParFileName(parFile);
  run.SetGeoFileName(geoFile);
  run.LoadSetup(setupName);
  run.SetField(new CbmFieldConst());
  run.SetTarget(targetElement, targetThickness, targetDiameter, targetPosX, targetPosY, targetPosZ,
                targetRotY * TMath::DegToRad());
  run.SetBeamPosition(0., 0., 0.1, 0.1);  // Beam width 1 mm is assumed
  run.SetBeamAngle(beamRotY * TMath::DegToRad(), 0., 0., 0.);
  run.SetRandomEventPlane();
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


void SetTrack(CbmTransport* run, Double_t beamRotY, Int_t pdgid, Double_t x, Double_t y, Double_t z)
{
  TVector3 v;
  v.SetXYZ(x, y, z);
  v.RotateY(-beamRotY * acos(-1.) / 180.);
  cout << "X " << v.X() << " Y " << v.Y() << " Z " << v.Z() << endl;

  run->AddInput(new FairParticleGenerator(pdgid, 1, v.X(), v.Y(), v.Z()));  // single electron along beam axis
}
