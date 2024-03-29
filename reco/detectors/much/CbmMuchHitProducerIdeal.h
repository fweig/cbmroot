/* Copyright (C) 2006-2017 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Anna Senger, Andrey Lebedev, Denis Bertini [committer] */

/**
 * \file CbmMuchHitProducerIdeal.h
 * \author  A.Kiseleva
 * \date 13.04.06
 * \brief Ideal hit producer for MUon CHambers detector.
 */

#ifndef CBMMUCHHITPRODUCERIDEAL_H_
#define CBMMUCHHITPRODUCERIDEAL_H_

#include "FairTask.h"

#include "TString.h"

class TClonesArray;
class CbmMuchGeoScheme;

class CbmMuchHitProducerIdeal : public FairTask {
public:
  CbmMuchHitProducerIdeal(const char* digiFileName, Int_t Id);

  virtual ~CbmMuchHitProducerIdeal();

  InitStatus Init();
  void Exec(Option_t* option);
  void Finish();

  void SetSigmaX(Double_t sigma) { fSigmaX = sigma; }
  void SetSigmaY(Double_t sigma) { fSigmaY = sigma; }
  void SetSigmaZ(Double_t sigma) { fSigmaZ = sigma; }

private:
  TClonesArray* fMuchPoints;     // Much MC points
  TClonesArray* fMuchPixelHits;  // Much hits

  Double_t fSigmaX;
  Double_t fSigmaY;
  Double_t fSigmaZ;

  Int_t fId;

  TString fDigiFile;             // Digitization file
  CbmMuchGeoScheme* fGeoScheme;  // Geometry scheme

  CbmMuchHitProducerIdeal(const CbmMuchHitProducerIdeal&);
  CbmMuchHitProducerIdeal& operator=(const CbmMuchHitProducerIdeal&);

  ClassDef(CbmMuchHitProducerIdeal, 1);
};

#endif
