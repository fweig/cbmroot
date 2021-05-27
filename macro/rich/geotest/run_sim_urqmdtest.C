/** @file run_sim_urqmdtest.C
  * @copyright Copyright (C) 2013-2020 UGiessen, JINR-LIT
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Semen Lebedev [orginator], Andrey Lebedev **/

void run_sim_urqmdtest(
  const string& urqmdFile = "/Users/slebedev/Development/cbm/data/urqmd/auau/"
                            "8gev/centr/urqmd.auau.8gev.centr.00001.root",
  const string& mcFile   = "/Users/slebedev/Development/cbm/data/sim/rich/urqmdtest/mc.00000.root",
  const string& parFile  = "/Users/slebedev/Development/cbm/data/sim/rich/urqmdtest/param.00000.root",
  const string& geoFile  = "/Users/slebedev/Development/cbm/data/sim/rich/urqmdtest/geosim.00000.root",
  const string& geoSetup = "sis100_electron", int nEvents = 1000)
{
  TTree::SetMaxTreeSize(90000000000);

  remove(parFile.c_str());
  remove(mcFile.c_str());
  remove(geoFile.c_str());

  TStopwatch timer;
  timer.Start();

  CbmTransport run;
  run.AddInput(urqmdFile.c_str());
  run.SetOutFileName(mcFile.c_str());
  run.SetParFileName(parFile.c_str());
  run.SetGeoFileName(geoFile.c_str());
  run.LoadSetup(geoSetup.c_str());
  run.SetTarget("Gold", 0.025, 2.5);
  run.SetBeamPosition(0., 0., 0.1, 0.1);
  run.SetEngine(kGeant4);
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
