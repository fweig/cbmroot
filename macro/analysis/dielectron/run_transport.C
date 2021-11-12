/* Copyright (C) 2011-2021 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev, Elena Lebedeva [committer] */

// Run this macro with run_local.py for local test and with batch_send(job).py for large productions
void run_transport(const std::string& urqmdFile,  // if "", no urqmd
                   const std::string& plutoFile,  // if "", no pluto particles are embedded into event
                   const std::string& traFile, const std::string& parFile, const std::string& geoFile,
                   const std::string& geoSetup, int nEvents)
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
  if (plutoFile.length() > 0) { run.AddInput(plutoFile.c_str(), kPluto); }
  run.SetOutFileName(traFile.c_str());
  run.SetParFileName(parFile.c_str());
  run.SetGeoFileName(geoFile.c_str());
  run.LoadSetup(geoSetup.c_str());
  run.SetTarget("Gold", 0.0025, 2.5);  // for lmvm thickness = 0.0025; // 25 mum
  run.SetBeamPosition(0., 0., 0.1, 0.1);
  //run.SetEngine(kGeant4);
  //run.SetRandomEventPlane();
  run.Run(nEvents);


  timer.Stop();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "Output file is " << traFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Geometry file is " << geoFile << std::endl;
  std::cout << "Real time " << timer.RealTime() << " s, CPU time " << timer.CpuTime() << "s" << std::endl;
  std::cout << "Test passed" << std::endl << "All ok" << std::endl;

  // if (plutoFile.length() > 0 && urqmdFile.length() > 0) {
  //     Float_t bratioEta[6];
  //     Int_t modeEta[6];

  //     TGeant3* gMC3 = (TGeant3*) gMC;

  //     for (Int_t kz = 0; kz < 6; ++kz) {
  //         bratioEta[kz] = 0.;
  //         modeEta[kz]   = 0;
  //     }

  //     Int_t ipa    = 17;
  //     bratioEta[0] = 39.38;  //2gamma
  //     bratioEta[1] = 32.20;  //3pi0
  //     bratioEta[2] = 22.70;  //pi+pi-pi0
  //     bratioEta[3] = 4.69;   //pi+pi-gamma
  //     bratioEta[4] = 0.60;   //e+e-gamma
  //     bratioEta[5] = 4.4e-2; //pi02gamma

  //     modeEta[0] = 101;    //2gamma
  //     modeEta[1] = 70707;  //3pi0
  //     modeEta[2] = 80907;  //pi+pi-pi0
  //     modeEta[3] = 80901;  //pi+pi-gamma
  //     modeEta[4] = 30201;  //e+e-gamma
  //     modeEta[5] = 10107;  //pi02gamma
  //     gMC3->Gsdk(ipa, bratioEta, modeEta);

  //     Float_t bratioPi0[6];
  //     Int_t modePi0[6];

  //     for (Int_t kz = 0; kz < 6; ++kz) {
  //         bratioPi0[kz] = 0.;
  //         modePi0[kz] = 0;
  //     }

  //     ipa = 7;
  //     bratioPi0[0] = 98.798;
  //     bratioPi0[1] = 1.198;

  //     modePi0[0] = 101;
  //     modePi0[1] = 30201;

  //     gMC3->Gsdk(ipa, bratioPi0, modePi0);

  //     Int_t t = time(NULL);
  //     TRandom *rnd = new TRandom(t);
  //     gMC->SetRandom(rnd);
  // }
}
