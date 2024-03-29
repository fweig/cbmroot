/* Copyright (C) 2016-2021 Laboratory of Information Technologies, Joint Institute for Nuclear Research, Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Timur Ablyazimov [committer] */

#ifndef LX_CALC_STATS
#define LX_CALC_STATS

#include "FairTask.h"

#include "TH1F.h"

class TClonesArray;

class LxCalcStats : public FairTask {
public:
  LxCalcStats();
  LxCalcStats(const LxCalcStats& orig) = delete;
  LxCalcStats& operator=(const LxCalcStats& orig) = delete;
  InitStatus Init();         // Overridden from FairTask
  void Exec(Option_t* opt);  // Overridden from FairTask
  void Finish();             // Overridden from FairTask

private:
  TClonesArray* fMCTracks;
  TClonesArray* fMuchPoints;
  TClonesArray* fTrdPoints;
  TH1F* xHistos[4][3];
  TH1F* yHistos[4][3];
  TH1F* trdDeltaThetaXHistos[3];
  TH1F* trdDeltaThetaYHistos[3];

  ClassDef(LxCalcStats, 1)
};

#endif  //LX_CALC_STATS
