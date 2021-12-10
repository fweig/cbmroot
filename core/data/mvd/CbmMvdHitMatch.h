/* Copyright (C) 2017-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

// -------------------------------------------------------------------------
// -----                    CbmMvdHitMatch source file                 -----
// -----                  Created 07/11/06  by V. Friese               -----
// -----            Based on CbmStsMapsHitInfo by M. Deveaux           -----
// -----           Update to new CbmMatch Class by P. Sitzmann         -----
// -------------------------------------------------------------------------


#ifndef CBMMVDHITMATCH_H
#define CBMMVDHITMATCH_H 1

#include "CbmMatch.h"  // for CbmMatch

#include <Rtypes.h>      // for ClassDef

#include <cstdint>

class CbmMvdHitMatch : public CbmMatch {

public:
  /** Default constructor **/
  CbmMvdHitMatch();


  /** Constructor with all parameters **/
  CbmMvdHitMatch(double weight, int32_t index, int32_t entry = -1, int32_t file = -1);

  CbmMvdHitMatch(int32_t /*par1*/, int32_t /*par2*/, int32_t /*par3*/, int32_t /*par4*/, int32_t /*par5*/)
    : CbmMatch()
    , fFileNumber(-1)
    , fIndex(-1)
    , fWeight(-1.)
    , fEntry(-1)
  {
    ;
  }  //quick solution for error in CbmMvdHitProducer


  /** Destructor **/
  virtual ~CbmMvdHitMatch();


  /** Accessors **/
  int32_t GetFileNumber() const { return fFileNumber; }
  int32_t GetIndexNumber() const { return fIndex; }
  int32_t GetEntryNumber() const { return fEntry; }
  float GetWeight() const { return fWeight; }

  int32_t GetPointId() const { return 0; }
  int32_t GetTrackId() const { return 0; }
  int32_t GetNMerged() const { return 0; }
  void AddMerged() { ; }


  /** Reset data members **/
  virtual void Clear(Option_t*) { ; }

private:
  int32_t fFileNumber;
  int32_t fIndex;
  double fWeight;
  int32_t fEntry;

  ClassDef(CbmMvdHitMatch, 1);
};


#endif
