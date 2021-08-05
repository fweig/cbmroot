/* Copyright (C) 2010 - 2021 Goethe-University Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig */

#include "CbmTsEventHeader.h"

// -----   Constructor   ------------------------------------------------------
CbmTsEventHeader::CbmTsEventHeader() {}
// ----------------------------------------------------------------------------


// ---- Reset ----
void CbmTsEventHeader::Reset()
{
  // Reset the digi counters
  fNDigisPsd   = 0;
  fNDigisRich  = 0;
  fNDigisSts   = 0;
  fNDigisTof   = 0;
  fNDigisTrd1D = 0;
  fNDigisTrd2D = 0;
}

ClassImp(CbmTsEventHeader)
