/** @file CbmTofHitFinderTBQA.h
  * @copyright Copyright (C) 2017 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Timur Ablyazimov, Pierre-Alain Loizeau [committer] **/

#ifndef CBMTOFHITFINDERTBQA_H
#define CBMTOFHITFINDERTBQA_H

#include "CbmMCDataArray.h"
#include "CbmMCEventList.h"
#include "CbmTimeSlice.h"

#include "FairTask.h"

#include "TClonesArray.h"

class CbmTofHitFinderTBQA : public FairTask {
public:
  CbmTofHitFinderTBQA();
  CbmTofHitFinderTBQA(const CbmTofHitFinderTBQA&) = delete;
  CbmTofHitFinderTBQA& operator=(const CbmTofHitFinderTBQA&) = delete;

  InitStatus Init();
  void Exec(Option_t* option);
  void Finish();
  void SetIsEvByEv(bool v) { isEvByEv = v; }

private:
  bool isEvByEv;
  TClonesArray* fTofHits;
  TClonesArray* fTofDigiMatchs;
  TClonesArray* fTofDigis;
  TClonesArray* fTofDigiPointMatchs;
  CbmMCDataArray* fTofMCPoints;
  CbmMCDataArray* fMCTracks;
  CbmTimeSlice* fTimeSlice;
  CbmMCEventList* fEventList;

  ClassDef(CbmTofHitFinderTBQA, 1)
};

#endif /* CBMTOFHITFINDERTBQA_H */
