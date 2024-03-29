/* Copyright (C) 2006-2017 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev, Andrey Lebedev, Denis Bertini [committer] */

#include "CbmRichRingSelectAnn.h"

#include "CbmRichGeoManager.h"
#include "CbmRichRingLight.h"
#include "CbmRichRingSelectImpl.h"

#include "TMultiLayerPerceptron.h"
#include "TSystem.h"
#include "TTree.h"

#include <iostream>

using std::cout;
using std::endl;

CbmRichRingSelectAnn::CbmRichRingSelectAnn() : fAnnWeights(""), fNN(NULL), fSelectImpl(NULL)
{
  if (CbmRichGeoManager::GetInstance().fGP->fGeometryType == CbmRichGeometryTypeCylindrical) {
    fAnnWeights = string(gSystem->Getenv("VMCWORKDIR")) + "/parameters/rich/rich_v17a_select_ann_weights.txt";
  }
  else if (CbmRichGeoManager::GetInstance().fGP->fGeometryType == CbmRichGeometryTypeTwoWings) {
    fAnnWeights = string(gSystem->Getenv("VMCWORKDIR")) + "/parameters/rich/rich_v16a_select_ann_weights.txt";
  }
  else {
    fAnnWeights = string(gSystem->Getenv("VMCWORKDIR")) + "/parameters/rich/rich_v17a_select_ann_weights.txt";
  }
}

CbmRichRingSelectAnn::~CbmRichRingSelectAnn() {}

void CbmRichRingSelectAnn::Init()
{
  fSelectImpl = new CbmRichRingSelectImpl();

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

  fNN = new TMultiLayerPerceptron("x0,x1,x2,x3,x4,x5:10:xOut", simu);
  cout << "-I- CbmRichRingSelectAnn: get ANN weight parameters from: " << fAnnWeights << endl;
  fNN->LoadWeights(fAnnWeights.c_str());
}

void CbmRichRingSelectAnn::DoSelect(CbmRichRingLight* ring)
{
  if (ring->GetRadius() >= 10.f || ring->GetRadius() <= 0.f || ring->GetNofHits() <= 5.f
      || ring->GetRadialPosition() <= 0.f || ring->GetRadialPosition() >= 999.f) {

    ring->SetSelectionNN(-1.f);
    return;
  }
  ring->SetNofHitsOnRing(fSelectImpl->GetNofHitsOnRingCircle(ring));
  if (ring->GetNofHitsOnRing() < 5) {
    ring->SetSelectionNN(-1.f);
    return;
  }

  ring->SetAngle(fSelectImpl->GetAngle(ring));
  if (ring->GetAngle() < 0.f || ring->GetAngle() > 6.3f) {
    ring->SetSelectionNN(-1.f);
    return;
  }

  double params[6];
  params[0] = ring->GetNofHits() / 45.;
  params[1] = ring->GetAngle() / 6.28;
  params[2] = ring->GetNofHitsOnRing() / 45.;
  params[3] = ring->GetRadialPosition() / 110.;
  params[4] = ring->GetRadius() / 10.;
  params[5] = (ring->GetChi2() / ring->GetNofHits()) / 0.4;

  float nnEval = fNN->Evaluate(0, params);

  /*  float nofHitsEval = ring->GetNofHits() / 35.;
    if (nofHitsEval > 1) nofHitsEval = 1.;
    float angleEval = 1. - ring->GetAngle() / 6.29;
    if (angleEval > 1.) angleEval = 1.;
    if (angleEval < 0.) angleEval = 0.;
    float radialPosEval = ring->GetRadialPosition() / 100;
    if (radialPosEval > 1.) radialPosEval = 1.;
    
    
    nnEval = nofHitsEval * angleEval * radialPosEval;*/

  ring->SetSelectionNN(nnEval);
}
