/**
 *    file CbmKresSelectAnn.cxx
 *
 *    author Ievgenii Kres
 *    date 01.06.2017
 *    modified 30.01.2020
 *
 *    Use of ANN to select photon conversion candidates for pi^0 analysis (based on weight parameters).
 *    Weight parameters are obtained from the ANN training procedure.
 *    The selection is based on several parameters: InvariantMass, OpeningAngle, PlaneAngle_last, ZPos, Momentum1, Momentum2
 *
 **/

#include "CbmKresSelectAnn.h"

#include <boost/assign/list_of.hpp>

#include <iostream>
#include <string>
#include <vector>

#include "TMath.h"
#include "TSystem.h"
#include "TTree.h"


using namespace std;

CbmKresSelectAnn::CbmKresSelectAnn() : fAnnWeights(), fNN(nullptr) {}

CbmKresSelectAnn::~CbmKresSelectAnn() {}

void CbmKresSelectAnn::Init() {
  TTree* simu = new TTree("MonteCarlo", "MontecarloData");
  Double_t x[6];
  Double_t xOut;

  simu->Branch("x0", &x[0], "x0/D");
  simu->Branch("x1", &x[1], "x1/D");
  simu->Branch("x2", &x[2], "x2/D");
  simu->Branch("x3", &x[3], "x3/D");
  simu->Branch("x4", &x[4], "x4/D");
  simu->Branch("x5", &x[5], "x5/D");
  simu->Branch("xOut", &xOut, "xOut/D");

  fNN = new TMultiLayerPerceptron("x0,x1,x2,x3,x4,x5:12:xOut", simu);

  fAnnWeights = string(gSystem->Getenv("VMCWORKDIR"))
                + "/analysis/conversion2/KresAnalysis_ann_weights.txt";
  cout << "-I- CbmKresSelectAnn: get ANN weight parameters from: "
       << fAnnWeights << endl;
  fNN->LoadWeights(fAnnWeights.c_str());
}

double CbmKresSelectAnn::DoSelect(double InvariantMass,
                                  double OpeningAngle,
                                  double PlaneAngle_last,
                                  double ZPos,
                                  TVector3 Momentum1,
                                  TVector3 Momentum2) {
  double AnnValue = 0;

  double p1 =
    TMath::Sqrt(Momentum1.X() * Momentum1.X() + Momentum1.Y() * Momentum1.Y()
                + Momentum1.Z() * Momentum1.Z());
  double p2 =
    TMath::Sqrt(Momentum2.X() * Momentum2.X() + Momentum2.Y() * Momentum2.Y()
                + Momentum2.Z() * Momentum2.Z());

  if (InvariantMass > 0.5 || OpeningAngle > 45 || ZPos > 100 || p1 > 10
      || p2 > 10) {
    return AnnValue = -1;
  }


  double params[6];

  params[0] = InvariantMass / 0.1;
  params[1] = OpeningAngle / 30;
  params[2] = PlaneAngle_last / 30;
  params[3] = ZPos / 100;
  params[4] = p1 / 5;
  params[5] = p2 / 5;

  if (params[0] > 1.0) params[0] = 1.0;
  if (params[1] > 1.0) params[1] = 1.0;
  if (params[2] > 1.0) params[2] = 1.0;
  if (params[3] > 1.0) params[3] = 1.0;
  if (params[4] > 1.0) params[4] = 1.0;
  if (params[5] > 1.0) params[5] = 1.0;

  AnnValue = fNN->Evaluate(0, params);

  return AnnValue;
}
