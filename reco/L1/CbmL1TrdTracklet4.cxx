/* Copyright (C) 2006-2010 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Igor Kulakov, Denis Bertini [committer] */

#include "CbmL1TrdTracklet4.h"

ClassImp(CbmL1TrdTracklet4);

// ----------------------- Default constructor ---------------------------
CbmL1TrdTracklet4::CbmL1TrdTracklet4()
  : vAccostTracklet()
  , vAccostLeft()
  , vAccostRight()
  , fVal(0)
  , fY(0)
  , fIsAlone(0)
  , fIndex(0)
  , fExt1(0)
  , fExt2(0) {};

// ----------------------- Default destructor ---------------------------
CbmL1TrdTracklet4::~CbmL1TrdTracklet4() {};
