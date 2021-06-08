/* Copyright (C) 2017-2018 GSI/JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer] */

#include <stdio.h>

void run_rich_sim_tb(Int_t nEvents = 1000)
{
  TTree::SetMaxTreeSize(90000000000);

  TString script = TString(gSystem->Getenv("SCRIPT"));

  TString myName = "run_sim_tb";                   // this macro's name for screen output
  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory

  TString geoSetupFile = srcDir + "/macro/rich/geosetup/rich_setup_sis300_tb.C";

  TString urqmdFile = "/Users/slebedev/Development/cbm/data/urqmd/auau/25gev/"
                      "mbias/urqmd.auau.25gev.mbias.00001.root";
  TString outDir  = "/Users/slebedev/Development/cbm/data/sim/rich/tb/";
  TString parFile = outDir + "param.00001.root";
  TString mcFile  = outDir + "mc.00001.root";
  TString geoFile = outDir + "geofile.00001.root";

  remove(parFile.Data());
  remove(mcFile.Data());
  remove(geoFile.Data());


  // Target geometry
  TString targetElement    = "Gold";
  Double_t targetThickness = 0.025;  // full thickness in cm
  Double_t targetDiameter  = 2.5;    // diameter in cm
  Double_t targetPosX      = 0.;     // target x position in global c.s. [cm]
  Double_t targetPosY      = 0.;     // target y position in global c.s. [cm]
  Double_t targetPosZ      = 0.;     // target z position in global c.s. [cm]
  Double_t targetRotY      = 0.;     // target rotation angle around the y axis [deg]


  // primary vertex
  Bool_t smearVertexXY = kTRUE;
  Bool_t smearVertexZ  = kTRUE;
  Double_t beamWidthX  = 0.1;  // Gaussian sigma of the beam profile in x [cm]
  Double_t beamWidthY  = 0.1;  // Gaussian sigma of the beam profile in y [cm]


  TStopwatch timer;
  timer.Start();
  gDebug = 0;

  FairRunSim* run = new FairRunSim();
  run->SetName("TGeant3");
  run->SetOutputFile(mcFile);
  run->SetGenerateRunInfo(kTRUE);


  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");

  TString setupFunct = "do_setup()";
  std::cout << "-I- " << myName << ": Loading macro " << geoSetupFile << std::endl;
  gROOT->LoadMacro(geoSetupFile);
  gROOT->ProcessLine(setupFunct);
  // You can modify the pre-defined setup by using
  // CbmSetup::Instance()->RemoveModule(ESystemId) or
  // CbmSetup::Instance()->SetModule(ESystemId, const char*, Bool_t) or
  // CbmSetup::Instance()->SetActive(ESystemId, Bool_t)
  // See the class documentation of CbmSetup.


  std::cout << std::endl << "-I- " << myName << ": Setting media file" << std::endl;
  run->SetMaterials("media.geo");  // Materials


  TString macroName = gSystem->Getenv("VMCWORKDIR");
  macroName += "/macro/run/modules/registerSetup.C";
  std::cout << std::endl << "Loading macro " << macroName << std::endl;
  gROOT->LoadMacro(macroName);
  gROOT->ProcessLine("registerSetup()");


  std::cout << std::endl << "-I- " << myName << ": Registering target" << std::endl;
  CbmTarget* target = new CbmTarget(targetElement.Data(), targetThickness, targetDiameter);
  target->SetPosition(targetPosX, targetPosY, targetPosZ);
  target->SetRotation(targetRotY);
  target->Print();
  run->AddModule(target);


  // Magnetic field
  std::cout << std::endl << "-I- " << myName << ": Registering magnetic field" << std::endl;
  CbmFieldMap* magField = CbmSetup::Instance()->CreateFieldMap();
  if (!magField) {
    std::cout << "-ERROR- : No valid field!";
    return;
  }
  run->SetField(magField);


  //  PrimaryGenerator
  std::cout << std::endl << "-I- " << myName << ": Registering event generators" << std::endl;
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  // --- Uniform distribution of event plane angle
  primGen->SetEventPlane(0., 2. * TMath::Pi());
  // --- Get target parameters
  Double_t tX  = 0.;
  Double_t tY  = 0.;
  Double_t tZ  = 0.;
  Double_t tDz = 0.;
  if (target) {
    target->GetPosition(tX, tY, tZ);
    tDz = target->GetThickness();
  }
  primGen->SetTarget(tZ, tDz);
  primGen->SetBeam(0., 0., beamWidthX, beamWidthY);
  primGen->SmearGausVertexXY(smearVertexXY);
  primGen->SmearVertexZ(smearVertexZ);

  CbmUnigenGenerator* urqmdGen = new CbmUnigenGenerator(urqmdFile);
  urqmdGen->SetEventPlane(0., 360.);
  urqmdGen->SetEventPlane(0., 360.);
  primGen->AddGenerator(urqmdGen);

  FairBoxGenerator* boxGen1 = new FairBoxGenerator(11, 5);
  boxGen1->SetPtRange(0., 3.);
  boxGen1->SetPhiRange(0., 360.);
  boxGen1->SetThetaRange(2.5, 25.);
  boxGen1->SetCosTheta();
  boxGen1->Init();
  primGen->AddGenerator(boxGen1);

  FairBoxGenerator* boxGen2 = new FairBoxGenerator(-11, 5);
  boxGen2->SetPtRange(0., 3.);  //4
  boxGen2->SetPhiRange(0., 360.);
  boxGen2->SetThetaRange(2.5, 25.);  //35
  boxGen2->SetCosTheta();
  boxGen2->Init();
  primGen->AddGenerator(boxGen2);


  run->SetGenerator(primGen);


  std::cout << std::endl << "-I- " << myName << ": Initialise run" << std::endl;
  //run->SetStoreTraj(kTRUE);
  run->Init();


  // -----   Runtime database   ---------------------------------------------
  std::cout << std::endl << std::endl;
  std::cout << "-I- " << myName << ": Set runtime DB" << std::endl;
  FairRuntimeDb* rtdb   = run->GetRuntimeDb();
  CbmFieldPar* fieldPar = (CbmFieldPar*) rtdb->getContainer("CbmFieldPar");
  fieldPar->SetParameters(magField);
  fieldPar->setChanged();
  fieldPar->setInputVersion(run->GetRunId(), 1);
  Bool_t kParameterMerged   = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(parFile.Data());
  rtdb->setOutput(parOut);
  rtdb->saveOutput();
  rtdb->print();


  std::cout << std::endl << std::endl << "-I- " << myName << ": Starting run" << std::endl;
  run->Run(nEvents);


  run->CreateGeometryFile(geoFile);

  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Output file is " << mcFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Geometry file is " << geoFile << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime << "s" << std::endl << std::endl;
  // ------------------------------------------------------------------------


  // -----   Resource monitoring   ------------------------------------------
  //    if ( Has_Fair_Monitor() ) {      // FairRoot Version >= 15.11
  //      // Extract the maximal used memory an add is as Dart measurement
  //      // This line is filtered by CTest and the value send to CDash
  //      FairSystemInfo sysInfo;
  //      Float_t maxMemory=sysInfo.GetMaxMemory();
  //      std::cout << "<DartMeasurement name=\"MaxMemory\" type=\"numeric/double\">";
  //      std::cout << maxMemory;
  //      std::cout << "</DartMeasurement>" << std::endl;
  //
  //      Float_t cpuUsage=ctime/rtime;
  //      std::cout << "<DartMeasurement name=\"CpuLoad\" type=\"numeric/double\">";
  //      std::cout << cpuUsage;
  //      std::cout << "</DartMeasurement>" << std::endl;
  //    }

  std::cout << " Test passed" << std::endl;
  std::cout << " All ok " << std::endl;
}
