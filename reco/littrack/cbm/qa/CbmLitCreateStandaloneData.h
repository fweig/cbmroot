/* Copyright (C) 2010-2013 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer] */

#ifndef CBMLITCREATESTANDALONEDATA_H_
#define CBMLITCREATESTANDALONEDATA_H_

#include "FairTask.h"

#include <fstream>

class TClonesArray;

class CbmLitCreateStandaloneData : public FairTask {
public:
  CbmLitCreateStandaloneData();
  virtual ~CbmLitCreateStandaloneData();

  virtual InitStatus Init();
  virtual void Exec(Option_t* opt);
  virtual void Finish();
  virtual void SetParContainers();

private:
  TClonesArray* fStsTracks;      // CbmStsTrack array
  TClonesArray* fMuchPixelHits;  // CbmMuchPixelHits array

  std::ofstream fHitsFile;
  std::ofstream fSeedsFile;

  Int_t fEventNo;

  CbmLitCreateStandaloneData(const CbmLitCreateStandaloneData&);
  CbmLitCreateStandaloneData& operator=(const CbmLitCreateStandaloneData&);

  ClassDef(CbmLitCreateStandaloneData, 1);
};

#endif /* CBMLITCREATESTANDALONEDATA_H_ */
