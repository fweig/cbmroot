/** @file CbmTrdMCQa.h
  * @copyright Copyright (C) 2017 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

#ifndef CBMTRDMCQA_H_
#define CBMTRDMCQA_H_

#include "FairTask.h"

#include "Rtypes.h"

class TClonesArray;
class CbmHistManager;

class CbmTrdMCQa : public FairTask {
public:
  CbmTrdMCQa();

  virtual ~CbmTrdMCQa();

  virtual InitStatus Init();

  virtual void Exec(Option_t*);

  virtual void Finish();

  void CreateHistograms();

  void CreateNofObjectsHistograms();

  void CreatePointHistograms();

  void ProcessPoints(const TClonesArray*);

private:
  void ReadDataBranches();

  CbmHistManager* fHM;
  TClonesArray* fTrdPoints;
  TClonesArray* fMCTracks;
  Int_t fNofStation;

  CbmTrdMCQa(const CbmTrdMCQa&);
  CbmTrdMCQa& operator=(const CbmTrdMCQa&);

  ClassDef(CbmTrdMCQa, 1);
};

#endif
