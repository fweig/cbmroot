/** @file CbmMuchPad.cxx
  * @copyright Copyright (C) 2007-2017 St. Petersburg Polytechnic University, St. Petersburg
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Evgeny Kryshen, Mikhail Ryzhinskiy [orginator], Florian Uhlig, Vikas Singhal **/

#include "CbmMuchPad.h"

// -------------------------------------------------------------------------
CbmMuchPad::CbmMuchPad()
  : fAddress(0)
  , fX(0.)
  , fY(0.)
  , fDx(0.)
  , fDy(0.)
  , fDigiIndex(-1)
  ,
  //fDigi(NULL),
  //fMatch(NULL),
  fNeighbours()
{
  fNeighbours.resize(20);
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
CbmMuchPad::CbmMuchPad(Int_t address, Double_t x, Double_t y, Double_t dx, Double_t dy)
  : fAddress(address)
  , fX(x)
  , fY(y)
  , fDx(dx)
  , fDy(dy)
  , fDigiIndex(-1)
  ,
  ////Can be removed as we will Buffer Digi in the CbmMuchReadoutBuffer
  //fDigi(new CbmMuchDigi(address)),
  //fMatch(new CbmMuchDigiMatch()),
  fNeighbours()
{
  fNeighbours.resize(20);
}
// -------------------------------------------------------------------------
