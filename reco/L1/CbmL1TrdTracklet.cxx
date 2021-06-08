/* Copyright (C) 2006-2010 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Igor Kulakov, Denis Bertini [committer], Sergey Gorbunov */

#include "CbmL1TrdTracklet.h"

ClassImp(CbmL1TrdTracklet);

// ----------------------- Default constructor ---------------------------
CbmL1TrdTracklet::CbmL1TrdTracklet()
  : vAccostTracklet()
  , fIndLeft(0)
  , fIndRight(0)
  , fVal(0)
  , fY(0)
  , fIsAlone(0)
  , fIndex(0) {};

// ----------------------- Default destructor ---------------------------
CbmL1TrdTracklet::~CbmL1TrdTracklet() {};
