/** @file run_transport.C
  * @copyright Copyright (C) 2021 UGiessen, JINR-LIT
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Semen Lebedev [committer] **/

void run_transport(const string& urqmdFile = "/Users/slebedev/Development/cbm/data/urqmd/auau/8gev/centr/"
                                             "urqmd.auau.8gev.centr.00001.root",  // if "", no urqmd
                   const string& traFile   = "/Users/slebedev/Development/cbm/data/sim/rich/reco/tra.00000.root",
                   const string& parFile   = "/Users/slebedev/Development/cbm/data/sim/rich/reco/par.00000.root",
                   const string& geoFile   = "/Users/slebedev/Development/cbm/data/sim/rich/reco/geo.00000.root",
                   int nofElectrons        = 5,   // number of e- to be generated, if <=0 no e- are embedded into event
                   int nofPositrons        = 5,   // number of e+ to be generated, if <=0 no e+ are embedded into event
                   const string& plutoFile = "",  // if "", no pluto particles are embedded into event
                   const string& geoSetup = "sis100_electron", int nEvents = 3)
{

  TTree::SetMaxTreeSize(90000000000);
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");

  remove(parFile.c_str());
  remove(traFile.c_str());
  remove(geoFile.c_str());

  TStopwatch timer;
  timer.Start();

  CbmTransport run;

  if (urqmdFile.length() > 0) { run.AddInput(urqmdFile.c_str()); }

  if (nofElectrons > 0) {
    FairBoxGenerator* boxGen1 = new FairBoxGenerator(11, nofElectrons);
    boxGen1->SetPtRange(0., 3.);
    boxGen1->SetPhiRange(0., 360.);
    boxGen1->SetThetaRange(2.5, 25.);
    boxGen1->SetCosTheta();
    boxGen1->Init();
    run.AddInput(boxGen1);
  }
  if (nofPositrons > 0) {
    FairBoxGenerator* boxGen2 = new FairBoxGenerator(-11, nofPositrons);
    boxGen2->SetPtRange(0., 3.);
    boxGen2->SetPhiRange(0., 360.);
    boxGen2->SetThetaRange(2.5, 25.);
    boxGen2->SetCosTheta();
    boxGen2->Init();
    run.AddInput(boxGen2);
  }

  if (plutoFile.length() > 0) { run.AddInput(plutoFile.c_str(), kPluto); }

  run.SetOutFileName(traFile.c_str());
  run.SetParFileName(parFile.c_str());
  run.SetGeoFileName(geoFile.c_str());
  run.LoadSetup(geoSetup.c_str());
  run.SetTarget("Gold", 0.025, 2.5);
  run.SetBeamPosition(0., 0., 0.1, 0.1);
  run.SetRandomEventPlane();
  run.Run(nEvents);


  timer.Stop();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Output file is " << traFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Geometry file is " << geoFile << std::endl;
  std::cout << "Real time " << timer.RealTime() << " s, CPU time " << timer.CpuTime() << "s" << std::endl;
  std::cout << "Test passed" << std::endl << "All ok" << std::endl;
}
