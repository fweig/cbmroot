/* Copyright (C) 2008 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov [committer] */

#ifndef CBM_L1_STTHIT_h
#define CBM_L1_STTHIT_h

#include "CbmKFHit.h"
//#include "CbmKFPixelMeasurement.h"
#include "CbmKFUMeasurement.h"

class CbmSttHit;
class CbmKFTrackInterface;

class CbmL1SttHit : public CbmKFHit {
public:
  CbmL1SttHit() {}
  CbmL1SttHit(CbmSttHit* h, int index) : CbmKFHit() { Create(h, index); }
  ~CbmL1SttHit() {}

  void Create(CbmSttHit* h, int index);
  Int_t Filter(CbmKFTrackInterface& track, Bool_t downstream, Double_t& QP0);

  //CbmKFPixelMeasurement FitPoint;
  CbmKFUMeasurement FitPoint;

  int index;
  int iStation;
  Double_t time;
  bool busy;

public:
  ClassDef(CbmL1SttHit, 1);
};

#endif
