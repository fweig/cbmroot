/* Copyright (C) 2006-2012 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer], Igor Kulakov */

#ifndef CbmKFStsHit_H
#define CbmKFStsHit_H 1

#include "CbmKFHit.h"
#include "CbmKFMaterial.h"
#include "CbmKFPixelMeasurement.h"

class CbmStsHit;
class CbmMvdHit;

class CbmKFStsHit : public CbmKFHit {

public:
  CbmKFStsHit() : FitPoint(), tube(0) {};
  ~CbmKFStsHit() {};

  CbmKFPixelMeasurement FitPoint;
  CbmKFTube* tube;

  void Create(CbmStsHit* h);
  void Create(CbmMvdHit* h);

  Int_t Filter(CbmKFTrackInterface& track, Bool_t downstream, Double_t& QP0);

  static void FilterPDAF(CbmKFTrackInterface& track, std::vector<CbmKFStsHit*>& vpHits, Bool_t downstream,
                         Double_t* QP0, double gateX, double gateY, int& best_hit_idx);


  const CbmKFStsHit& operator=(const CbmKFStsHit& a)
  {
    tube     = a.tube;
    FitPoint = a.FitPoint;
    return *this;
  };

  CbmKFStsHit(const CbmKFStsHit& a) : FitPoint(a.FitPoint), tube(a.tube) {};


  ClassDef(CbmKFStsHit, 1);
};

#endif
