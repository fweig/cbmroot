/* Copyright (C) 2014-2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Mikhail Ryzhinskiy, Florian Uhlig [committer], Volker Friese, David Emschermann */

// TODO comment to be changed
/** CbmMuchBeamTimeDigi.cxx
 **@author M.Ryzhinskiy <m.ryzhinskiy@gsi.de>
 **@since 19.03.07
 **@version 1.0
 **@author Vikas Singhal <vikas@vecc.gov.in>
 **@since 06.03.19
 **@version 2.0
 **
 ** Data class for digital MUCH information collected during BeamTime
 ** Data level: RAW
 ** To use reconstruction classes for CbmMuchBeamTimeDigi deriving it from CbmMuchDigi. VS
 **
 **/
#include "CbmMuchBeamTimeDigi.h"

// -------------------------------------------------------------------------
CbmMuchBeamTimeDigi::CbmMuchBeamTimeDigi()
  : CbmMuchDigi()
  , fPadX(-1)
  , fPadY(-1)
  , fRocId(-1)
  , fNxId(-1)
  , fNxCh(-1)
  , fElink(-1)
{
}
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
CbmMuchBeamTimeDigi::CbmMuchBeamTimeDigi(int32_t address, int32_t charge, uint64_t time)
  : CbmMuchDigi(address, charge, time)
  , fPadX(-1)
  , fPadY(-1)
  , fRocId(-1)
  , fNxId(-1)
  , fNxCh(-1)
  , fElink(-1)
{
}
// -------------------------------------------------------------------------

CbmMuchBeamTimeDigi::CbmMuchBeamTimeDigi(CbmMuchBeamTimeDigi* digi)
  : CbmMuchDigi(*digi)
  , fPadX(digi->GetPadX())
  , fPadY(digi->GetPadY())
  , fRocId(digi->GetRocId())
  , fNxId(digi->GetNxId())
  , fNxCh(digi->GetNxCh())
  , fElink(digi->GetElink())
{
}

CbmMuchBeamTimeDigi::CbmMuchBeamTimeDigi(const CbmMuchBeamTimeDigi& rhs)
  : CbmMuchDigi(rhs)
  , fPadX(rhs.fPadX)
  , fPadY(rhs.fPadY)
  , fRocId(rhs.fRocId)
  , fNxId(rhs.fNxId)
  , fNxCh(rhs.fNxCh)
  , fElink(rhs.fElink)
{
}

CbmMuchBeamTimeDigi& CbmMuchBeamTimeDigi::operator=(const CbmMuchBeamTimeDigi& rhs)
{

  if (this != &rhs) {
    CbmMuchDigi::operator=(rhs);
    fPadX                = rhs.fPadX;
    fPadY                = rhs.fPadY;
    fRocId               = rhs.fRocId;
    fNxId                = rhs.fNxId;
    fNxCh                = rhs.fNxCh;
    fElink               = rhs.fElink;
  }
  return *this;
}

ClassImp(CbmMuchBeamTimeDigi)
