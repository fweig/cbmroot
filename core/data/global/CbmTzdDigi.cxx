/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau, Volker Friese [committer] */

#include "CbmTzdDigi.h"

#include "CbmTofDigi.h"

CbmTzdDigi::CbmTzdDigi(const CbmTofDigi& digi)
  : fAddress(digi.GetAddress())
  , fTime(digi.GetTime())
  , fCharge(digi.GetCharge())
{
}

CbmTzdDigi::CbmTzdDigi(const CbmTofDigi* digi)
  : fAddress(digi->GetAddress())
  , fTime(digi->GetTime())
  , fCharge(digi->GetCharge())
{
}
