/* Copyright (C) 2007-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Mikhail Ryzhinskiy, Florian Uhlig, Volker Friese [committer], Evgeny Kryshen */

/** CbmMuchDigi.cxx
 **@author M.Ryzhinskiy <m.ryzhinskiy@gsi.de>
 **@since 19.03.07
 **@version 1.0
 **
 **@author Vikas Singhal <vikas@vecc.gov.in>
 **@since 17/05/16
 **@version 2.0
 ** Data class for digital MUCH information
 ** Data level: RAW
 **
 **
 **/
#include "CbmMuchDigi.h"

CbmMuchDigi::CbmMuchDigi(CbmMuchDigi* digi) : fTime(digi->fTime), fAddress(digi->fAddress), fCharge(digi->fCharge) {}


CbmMuchDigi::CbmMuchDigi(const CbmMuchDigi& rhs) : fTime(rhs.fTime), fAddress(rhs.fAddress), fCharge(rhs.fCharge) {}

CbmMuchDigi& CbmMuchDigi::operator=(const CbmMuchDigi& rhs)
{

  if (this != &rhs) {
    fAddress = rhs.fAddress;
    fTime    = rhs.fTime;
    fCharge  = rhs.fCharge;
  }
  return *this;
}

// -----   Set new charge   ------------------------------------------------
void CbmMuchDigi::SetAdc(Int_t adc)
{
  //ADC value should not be more than saturation
  if (adc < 0) { fCharge = 0; }
  else {
    fCharge = adc;
  }
  // if Saturation
  //       	Int_t saturation = (1<<12); //2 ^ 12 - 1;
  Int_t saturation = (1 << 5);  //2 ^ 5 - 1; // 32 for 5 bit adc*** modified by Ekata Nandy on 25/06/19***
  if (fCharge >= saturation) {
    //fCharge=saturation-1;
    fCharge         = saturation;  //As ADC value starts from 1, so -1 removed. Modified by Ekata Nandy on 25/06/19
    fSaturationFlag = 1;
  }
  //	if(fCharge < 0) fCharge=0;
}
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
void CbmMuchDigi::SetTime(ULong64_t time) { fTime = time; }
// -------------------------------------------------------------------------

ClassImp(CbmMuchDigi)
