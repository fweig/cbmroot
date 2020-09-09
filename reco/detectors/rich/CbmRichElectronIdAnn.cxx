/**
* \file CbmRichElectronIdAnn.cxx
*
* \author Semen Lebedev
* \date 2008
**/

#include "CbmRichElectronIdAnn.h"
#include "CbmGlobalTrack.h"
#include "CbmRichGeoManager.h"
#include "CbmRichRing.h"
#include "CbmRichUtil.h"
#include <Logger.h>
#include "FairRootManager.h"
#include "TClonesArray.h"
#include "TMath.h"
#include "TMultiLayerPerceptron.h"
#include "TSystem.h"
#include "TTree.h"

#include <iostream>

using std::cout;
using std::endl;

CbmRichElectronIdAnn::CbmRichElectronIdAnn()
  : fAnnWeights(""), fNN(NULL), fGlobalTracks(NULL), fRichRings(NULL) {
  Init();
}

CbmRichElectronIdAnn::~CbmRichElectronIdAnn() {}

void CbmRichElectronIdAnn::Init() {
  if (fNN != NULL) { delete fNN; }

  if (CbmRichGeoManager::GetInstance().fGP->fGeometryType
      == CbmRichGeometryTypeCylindrical) {
    fAnnWeights = string(gSystem->Getenv("VMCWORKDIR"))
                  + "/parameters/rich/rich_v17a_elid_ann_weights.txt";
  } else if (CbmRichGeoManager::GetInstance().fGP->fGeometryType
             == CbmRichGeometryTypeTwoWings) {
    fAnnWeights = string(gSystem->Getenv("VMCWORKDIR"))
                  + "/parameters/rich/rich_v16a_elid_ann_weights.txt";
  } else {
    fAnnWeights = string(gSystem->Getenv("VMCWORKDIR"))
                  + "/parameters/rich/rich_v17a_elid_ann_weights.txt";
  }

  TTree* simu = new TTree("MonteCarlo", "MontecarloData");
  Double_t x[9];
  Double_t xOut;

  simu->Branch("x0", &x[0], "x0/D");
  simu->Branch("x1", &x[1], "x1/D");
  simu->Branch("x2", &x[2], "x2/D");
  simu->Branch("x3", &x[3], "x3/D");
  simu->Branch("x4", &x[4], "x4/D");
  simu->Branch("x5", &x[5], "x5/D");
  simu->Branch("x6", &x[6], "x6/D");
  simu->Branch("x7", &x[7], "x7/D");
  simu->Branch("x8", &x[8], "x8/D");
  simu->Branch("xOut", &xOut, "xOut/D");

  fNN = new TMultiLayerPerceptron("x0,x1,x2,x3,x4,x5,x6,x7,x8:18:xOut", simu);
  cout << "-I- CbmRichElIdAnn: get NeuralNet weight parameters from: "
       << fAnnWeights << endl;
  fNN->LoadWeights(fAnnWeights.c_str());

  FairRootManager* ioman = FairRootManager::Instance();
  if (ioman != NULL) {
    fRichRings = (TClonesArray*) ioman->GetObject("RichRing");
    if (fRichRings == NULL) {
      LOG(error) << "CbmRichElectronIdAnn::Init() fRichRings == NULL";
    }
    fGlobalTracks = (TClonesArray*) ioman->GetObject("GlobalTrack");
    if (fGlobalTracks == NULL) {
      LOG(error) << "CbmRichElectronIdAnn::Init() fGlobalTracks == NULL";
    }
  } else {
    LOG(error) << "FairRootManager::Instance() == NULL";
  }
}

double CbmRichElectronIdAnn::CalculateAnnValue(int globalTrackIndex,
                                               double momentum) {
  double errorValue = -1.;
  if (globalTrackIndex < 0) return errorValue;

  if (fGlobalTracks == NULL || fRichRings == NULL) return -1;

  const CbmGlobalTrack* globalTrack =
    static_cast<const CbmGlobalTrack*>(fGlobalTracks->At(globalTrackIndex));
  if (globalTrack == NULL) return errorValue;

  Int_t richId = globalTrack->GetRichRingIndex();
  if (richId == -1) return errorValue;
  const CbmRichRing* richRing =
    static_cast<const CbmRichRing*>(fRichRings->At(richId));
  if (richRing == NULL) return errorValue;

  double rtDistance = CbmRichUtil::GetRingTrackDistance(globalTrackIndex);

  if (richRing->GetAaxis() >= 10. || richRing->GetAaxis() <= 0.
      || richRing->GetBaxis() >= 10. || richRing->GetBaxis() <= 0.
      || richRing->GetNofHits() <= 5. || rtDistance <= 0. || rtDistance >= 999.
      || richRing->GetRadialPosition() <= 0.
      || richRing->GetRadialPosition() >= 999. || richRing->GetPhi() <= -6.5
      || richRing->GetPhi() >= 6.5 || richRing->GetRadialAngle() <= -6.5
      || richRing->GetRadialAngle() >= 6.5) {

    return -1.;
  }
  double params[9];
  params[0] = richRing->GetAaxis() / 10.;
  params[1] = richRing->GetBaxis() / 10.;
  params[2] = (richRing->GetPhi() + 1.57) / 3.14;
  params[3] = richRing->GetRadialAngle() / 6.28;
  params[4] = (richRing->GetChi2() / richRing->GetNDF()) / 1.2;
  params[5] = richRing->GetRadialPosition() / 110.;
  params[6] = richRing->GetNofHits() / 45.;
  params[7] = rtDistance / 5.;
  params[8] = momentum / 15.;

  for (int k = 0; k < 9; k++) {
    if (params[k] < 0.0) params[k] = 0.0;
    if (params[k] > 1.0) params[k] = 1.0;
  }

  double nnEval = fNN->Evaluate(0, params);

  if (TMath::IsNaN(nnEval) == 1) {
    //cout << " -W- CbmRichElectronIdAnn: nnEval nan " << endl;
    nnEval = -1.;
  }

  return nnEval;
}
