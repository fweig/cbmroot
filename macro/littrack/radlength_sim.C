/* Copyright (C) 2013-2021 GSI/JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer], Florian Uhlig */

#include <algorithm>
#include <iostream>
using std::cout;
using std::endl;

void radlength_sim(const string& mcFile   = "/Users/slebedev/Development/cbm/data/sim/rich/radlen/mc.ac.root",
                   const string& parFile  = "/Users/slebedev/Development/cbm/data/sim/rich/radlen/param.ac.root",
                   const string& geoSetup = "sis100_electron_rich_pal_bcarb", Int_t nofEvents = 0)
{

  TTree::SetMaxTreeSize(90000000000);

  remove(parFile.c_str());
  remove(mcFile.c_str());

  TStopwatch timer;
  timer.Start();

  //   const Double_t minX   = -550;  // cm
  //   const Double_t maxX   = 550;   // cm
  //   const Double_t stepX  = 10.;    // cm
  //   const Int_t nofBinsX  = (maxX - minX) / stepX;
  //   const Double_t minY   = -550;  // cm
  //   const Double_t maxY   = 550;   // cm
  //   const Double_t stepY  = 10.;    // cm

  const Double_t minX  = -250;  // cm
  const Double_t maxX  = 250;   // cm
  const Double_t stepX = 1.;    // cm
  const Int_t nofBinsX = (maxX - minX) / stepX;
  const Double_t minY  = -250;  // cm
  const Double_t maxY  = 250;   // cm
  const Double_t stepY = 1.;    // cm
  const Int_t nofBinsY = (maxY - minY) / stepY;
  nofEvents            = nofBinsX * nofBinsY;

  std::vector<Double_t> vectorX, vectorY;
  for (Int_t iX = 0; iX < nofBinsX; iX++) {
    Double_t x = minX + iX * stepX;
    for (Int_t iY = 0; iY < nofBinsY; iY++) {
      Double_t y = minY + iY * stepY;
      vectorX.push_back(x);
      vectorY.push_back(y);
    }
  }

  CbmTransport run;
  CbmLitRadLengthGenerator* generator = new CbmLitRadLengthGenerator();
  generator->SetXY(vectorX, vectorY);
  run.AddInput(generator);

  /*
   const int RMax = 700; // Maximum radius of the station
   FairBoxGenerator* box = new FairBoxGenerator(0, 1);
   box->SetBoxXYZ(-RMax, -RMax, RMax, RMax, 0.);
   box->SetPRange(0.1, 10);
   box->SetThetaRange(0., 0.);
   box->SetPhiRange(0., 0.);
   primGen->AddGenerator(box);
   */

  /*
   FairBoxGenerator* box = new FairBoxGenerator(0, 1);
   box->SetPRange(0.1, 10);
   box->SetXYZ(0., 0., 0.);
   box->SetPhiRange(0., 360.);
   box->SetThetaRange(0., 50.);
   primGen->AddGenerator(box);
*/

  run.SetOutFileName(mcFile.c_str());
  run.SetParFileName(parFile.c_str());
  run.LoadSetup(geoSetup.c_str());
  run.SetTarget("Gold", 0.025, 2.5);
  run.SetBeamPosition(0., 0., 0., 0.);
  //run.SetEngine(kGeant4);
  //run.StoreTrajectories(true);
  run.RegisterRadLength(true);
  run.Run(nofEvents);

  timer.Stop();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
  std::cout << "MC file is " << mcFile << std::endl;
  std::cout << "Parameter file is " << parFile << std::endl;
  std::cout << "Real time " << timer.RealTime() << " s, CPU time " << timer.CpuTime() << "s" << std::endl;
  std::cout << std::endl << "Test passed" << std::endl << "All ok" << std::endl;
}
