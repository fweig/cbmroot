/** @file CbmMuchLayerSide.cxx
  * @copyright Copyright (C) 2008-2020 St. Petersburg Polytechnic University, St. Petersburg
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Mikhail Ryzhinskiy [committer], Florian Uhlig **/

/** CbmMuchLayerSide.cxx
 *@author  M.Ryzhinskiy <m.ryzhinskiy@gsi.de>
 *@version 1.0
 *@since   11.02.08
 **
 ** This class holds the transport geometry parameters
 ** of one MuCh tracking layer side.
 **/
#include "CbmMuchLayerSide.h"

#include "CbmMuchAddress.h"  // for CbmMuchAddress
#include "CbmMuchModule.h"   // for CbmMuchModule

#include <TObjArray.h>  // for TObjArray

// -----   Default constructor   -------------------------------------------
CbmMuchLayerSide::CbmMuchLayerSide() : TObject(), fDetectorId(0), fZ(0.), fModules() {}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
CbmMuchLayerSide::CbmMuchLayerSide(Int_t detId, Double_t z) : TObject(), fDetectorId(detId), fZ(z), fModules() {}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
CbmMuchLayerSide::CbmMuchLayerSide(Int_t iStation, Int_t iLayer, Bool_t iSide, Double_t z)
  : TObject()
  , fDetectorId(CbmMuchAddress::GetAddress(iStation, iLayer, iSide))
  , fZ(z)
  , fModules()
{
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmMuchLayerSide::~CbmMuchLayerSide() {}
// -------------------------------------------------------------------------

// -----   Public method AddModule   ---------------------------------------
void CbmMuchLayerSide::AddModule(CbmMuchModule* module) { fModules.Add(module); }
// -------------------------------------------------------------------------

void CbmMuchLayerSide::DrawModules(Color_t color, Bool_t modulesVisible, Bool_t)
{
  for (Int_t m = 0; m < GetNModules(); m++) {
    CbmMuchModule* module = GetModule(m);
    if (modulesVisible) {
      module->SetFillColor(color);
      module->SetFillStyle(3001);
      module->Draw("f");
    }
    module->DrawModule(color);
  }
}
ClassImp(CbmMuchLayerSide)
