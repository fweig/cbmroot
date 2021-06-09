/* Copyright (C) 2006-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Ivan Kisel, Sergey Gorbunov, Denis Bertini [committer], Igor Kulakov */

/*
 *====================================================================
 *
 *  CBM Level 1 Reconstruction 
 *  
 *  Authors: I.Kisel,  S.Gorbunov
 *
 *  e-mail : ikisel@kip.uni-heidelberg.de 
 *
 *====================================================================
 *
 *  Standalone RICH ring finder based on the Elastic Neural Net
 *
 *====================================================================
 */

#ifndef _CBM_L1_RICH_ENN_RING_FINDER_H_
#define _CBM_L1_RICH_ENN_RING_FINDER_H_

#include "CbmRichRingFinder.h"

#include "TStopwatch.h"
#include "TString.h"

#include <vector>

class ENNHit;
class ENNRing;

class CbmL1RichENNRingFinderParallel;
class CbmL1RichENNRingFinder : public CbmRichRingFinder {
private:
  CbmL1RichENNRingFinder(const CbmL1RichENNRingFinder&);
  CbmL1RichENNRingFinder operator=(const CbmL1RichENNRingFinder&);

public:
  /** Standard constructor **/
  CbmL1RichENNRingFinder(Int_t verbose = 0);

  /** Destructor **/
  ~CbmL1RichENNRingFinder();

  /** Initialisation **/
  void Init();

  /** Ring finding algorithm
   *
   *@param hitArray   Array of rich hits
   *@param projArray  Array of projected tracks (not needed here)
   *@param ringArray  Array of CbmRichRing
   **
   *@value Number of tracks created
   **/
  Int_t DoFind(CbmEvent* event, TClonesArray* hitArray, TClonesArray* projArray, TClonesArray* ringArray);

private:
  CbmL1RichENNRingFinderParallel* finder;
  Float_t fRecoTime;
  Int_t fNEvents;

public:
  ClassDef(CbmL1RichENNRingFinder, 1);
};

#endif
