/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#ifndef CBM_KRES_TRAIN_ANN_DIRECT_PHOTONS
#define CBM_KRES_TRAIN_ANN_DIRECT_PHOTONS

#include "TCanvas.h"
#include "TH2D.h"
#include "TVector3.h"
#include <TClonesArray.h>

#include <vector>

using namespace std;

class CbmKresTrainAnnDirectPhotons {

public:
  //***** brief Standard constructor.
  CbmKresTrainAnnDirectPhotons();
  //***** brief Standard destructor.
  virtual ~CbmKresTrainAnnDirectPhotons();


  void Init();
  void InitHistograms();

  void Exec(int event, int IdForANN, double InvariantMass, double OpeningAngle, double PlaneAngle_last, double ZPos,
            TVector3 Momentum1, TVector3 Momentum2);
  void TrainAndTestAnn();
  void Draw();


private:
  unsigned int fMaxNofTrainSamples;
  double fAnnCut;
  int fNofWrongLikeCorrect;
  int fNofCorrectLikeWrong;

  vector<double> IM_correct;
  vector<double> OA_correct;
  vector<double> Angle_correct;
  vector<double> Z_correct;
  vector<double> Mom1_correct;
  vector<double> Mom2_correct;
  vector<double> IM_wrong;
  vector<double> OA_wrong;
  vector<double> Angle_wrong;
  vector<double> Z_wrong;
  vector<double> Mom1_wrong;
  vector<double> Mom2_wrong;

  vector<TH1*> fHists;
  TH1D* fhAnnOutput_correct;
  TH1D* fhAnnOutput_wrong;
  TH1D* fhCumProb_correct;
  TH1D* fhCumProb_wrong;


  //***** brief Copy constructor.
  CbmKresTrainAnnDirectPhotons(const CbmKresTrainAnnDirectPhotons&);

  //***** brief Assignment operator.
  CbmKresTrainAnnDirectPhotons operator=(const CbmKresTrainAnnDirectPhotons&);


  ClassDef(CbmKresTrainAnnDirectPhotons, 1)
};

#endif
