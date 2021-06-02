/** @file run_sim_geotest.C
  * @copyright Copyright (C) 2009-2020 UGiessen, JINR-LIT
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Semen Lebedev [committer], Andrey Lebedev **/

void run_sim_geotest(const string& plutoFile = "",  // if "", BoxGenerator is used
                     const string& mcFile    = "/Users/slebedev/Development/cbm/data/sim/rich/geotest/mc.00000.root",
                     const string& parFile   = "/Users/slebedev/Development/cbm/data/sim/rich/geotest/param.00000.root",
                     const string& geoFile  = "/Users/slebedev/Development/cbm/data/sim/rich/geotest/geosim.00000.root",
                     const string& geoSetup = "sis100_electron",  //"mirror12_42",
                     int nEvents            = 10)
{
  TTree::SetMaxTreeSize(90000000000);

  remove(parFile.c_str());
  remove(mcFile.c_str());
  remove(geoFile.c_str());

  TStopwatch timer;
  timer.Start();

  CbmTransport run;

  if (plutoFile.length() > 0) { run.AddInput(plutoFile.c_str(), kPluto); }
  else {
    FairBoxGenerator* boxGen1 = new FairBoxGenerator(11, 1);
    boxGen1->SetPtRange(0., 3.);
    boxGen1->SetPhiRange(0., 360.);
    boxGen1->SetThetaRange(2.5, 25.);
    boxGen1->SetCosTheta();
    boxGen1->Init();
    run.AddInput(boxGen1);

    FairBoxGenerator* boxGen2 = new FairBoxGenerator(-11, 1);
    boxGen2->SetPtRange(0., 3.);
    boxGen2->SetPhiRange(0., 360.);
    boxGen2->SetThetaRange(2.5, 25.);
    boxGen2->SetCosTheta();
    boxGen2->Init();
    run.AddInput(boxGen2);
  }

  run.SetOutFileName(mcFile.c_str());
  run.SetParFileName(parFile.c_str());
  run.SetGeoFileName(geoFile.c_str());
  run.LoadSetup(geoSetup.c_str());
  run.SetTarget("Gold", 0.025, 2.5);
  run.SetBeamPosition(0., 0., 0.1, 0.1);
  //run.SetEngine(kGeant4);
  //run.StoreTrajectories(true);
  run.Run(nEvents);

  timer.Stop();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Output file is " << mcFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Geometry file is " << geoFile << std::endl;
  std::cout << "Real time " << timer.RealTime() << " s, CPU time " << timer.CpuTime() << "s" << std::endl;
  std::cout << std::endl << "Test passed" << std::endl << "All ok" << std::endl;
}
