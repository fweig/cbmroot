/* Copyright (C) 2009-2020 UGiessen, JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev [committer], Andrey Lebedev */

//
void run_transport(const string& urqmdFile,  // only for "urqmdTest"
                   const string& plutoFile,  // only for "geoTest", if "", BoxGenerator is used
                   const string& traFile, const string& parFile, const string& geoFile, const string& geoSetup,
                   int nEvents, double targetZ = -44.)
{
  TTree::SetMaxTreeSize(90000000000);

  remove(parFile.c_str());
  remove(traFile.c_str());
  remove(geoFile.c_str());

  TStopwatch timer;
  timer.Start();

  CbmTransport run;

  if (urqmdFile.length() > 0) { run.AddInput(urqmdFile.c_str()); }

  if (plutoFile.length() > 0) { run.AddInput(plutoFile.c_str(), kPluto); }
  else {
    FairBoxGenerator* boxGen1 = new FairBoxGenerator(11, 1);
    boxGen1->SetPtRange(0., 3.);
    boxGen1->SetPhiRange(0., 360.);
    boxGen1->SetThetaRange(2.5, 25.);
    //boxGen1->SetYRange(0., 4.);
    //boxGen1->SetXYZ(0., 0., targetZ);
    boxGen1->SetCosTheta();
    boxGen1->Init();
    run.AddInput(boxGen1);

    FairBoxGenerator* boxGen2 = new FairBoxGenerator(-11, 1);
    boxGen2->SetPtRange(0., 3.);
    boxGen2->SetPhiRange(0., 360.);
    boxGen2->SetThetaRange(2.5, 25.);
    //boxGen2->SetYRange(0., 4.);
    //boxGen2->SetXYZ(0., 0., targetZ);
    boxGen2->SetCosTheta();
    boxGen2->Init();
    run.AddInput(boxGen2);
  }

  run.SetOutFileName(traFile.c_str());
  run.SetParFileName(parFile.c_str());
  run.SetGeoFileName(geoFile.c_str());
  run.LoadSetup(geoSetup.c_str());
  run.SetTarget("Gold", 0.025, 2.5, 0, 0, targetZ);
  run.SetBeamPosition(0., 0., 0.1, 0.1);
  //run.SetEngine(kGeant4);
  //run.StoreTrajectories(true);
  run.Run(nEvents);

  timer.Stop();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Transport file is " << traFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Geometry file is " << geoFile << std::endl;
  std::cout << "Real time " << timer.RealTime() << " s, CPU time " << timer.CpuTime() << "s" << std::endl;
  std::cout << std::endl << "Test passed" << std::endl << "All ok" << std::endl;
}
