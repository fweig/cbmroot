/* Copyright (C) 2006-2017 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese, Florian Uhlig [committer] */

// -------------------------------------------------------------------------
// -----                    CbmMvdHitMatch source file                 -----
// -----                  Created 07/11/06  by V. Friese               -----
// -----            Based on CbmStsMapsHitInfo by M. Deveaux           -----
// -----           Update to new CbmMatch Class by P. Sitzmann         -----
// -------------------------------------------------------------------------

#include "CbmMvdHitMatch.h"

// -----   Default constructor   -------------------------------------------
CbmMvdHitMatch::CbmMvdHitMatch() : CbmMatch(), fFileNumber(-1), fIndex(0), fWeight(0), fEntry(-1)
{
  AddLink(0., 0, -1, -1);
}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
CbmMvdHitMatch::CbmMvdHitMatch(double weight, int32_t index, int32_t entry, int32_t file)
  : CbmMatch()
  , fFileNumber(file)
  , fIndex(index)
  , fWeight(weight)
  , fEntry(entry)
{
  AddLink(weight, index, entry, file);
}
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
CbmMvdHitMatch::~CbmMvdHitMatch() {}
// -------------------------------------------------------------------------

ClassImp(CbmMvdHitMatch);
