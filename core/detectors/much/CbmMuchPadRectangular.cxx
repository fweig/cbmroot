/* Copyright (C) 2007-2020 Petersburg Nuclear Physics Institute named by B.P.Konstantinov of National Research Centre "Kurchatov Institute", Gatchina
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Evgeny Kryshen [committer] */

// -------------------------------------------------------------------------
// -----          CbmMuchPadRectangular source file                    -----
// -----                  Created 26/10/07  by E. Kryshen              -----
// -------------------------------------------------------------------------

#include "CbmMuchPadRectangular.h"

#include <TColor.h>  // for TColor
#include <TPave.h>   // for TPave

// -----  Default constructor  ----------------------------------------------
CbmMuchPadRectangular::CbmMuchPadRectangular() : CbmMuchPad(), TPave() {}
// -------------------------------------------------------------------------

// -----  Standard constructor  --------------------------------------------
CbmMuchPadRectangular::CbmMuchPadRectangular(Int_t address, Double_t x, Double_t y, Double_t lx, Double_t ly)
  : CbmMuchPad(address, x, y, lx, ly)
  , TPave(x - lx / 2, y - ly / 2, x + lx / 2, y + ly / 2, 1)
{
  SetFillColor(kYellow);
  SetLineWidth(1);
  SetLineColor(34);
}
// -------------------------------------------------------------------------

//
//// -------------------------------------------------------------------------
//void CbmMuchPadRectangular::Reset(){
//  fDigiIndex = -1;
//  SetFillColor(kYellow);
//}
//// -------------------------------------------------------------------------
//
//
// -------------------------------------------------------------------------
void CbmMuchPadRectangular::SetFired(Int_t iDigi, Int_t ADCcharge, Int_t nADCChannels)
{
  fDigiIndex = iDigi;
  if (fDigiIndex >= 0) SetFillColor(TColor::GetColor(nADCChannels - 1 - ADCcharge, nADCChannels - 1 - ADCcharge, 245));
  else
    SetFillColor(kYellow);
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMuchPadRectangular::DrawPad()
{
  Draw("f");
  Draw();
}
// -------------------------------------------------------------------------
//
//
//// -------------------------------------------------------------------------
//TString CbmMuchPadRectangular::GetInfo(){
//  return Form("Channel:%i fired:%i digiId:%i",
//      CbmMuchModuleGem::GetChannelIndex(fChannelId),fDigiIndex>=0,fDigiIndex);
//}
//// -------------------------------------------------------------------------

ClassImp(CbmMuchPadRectangular)
