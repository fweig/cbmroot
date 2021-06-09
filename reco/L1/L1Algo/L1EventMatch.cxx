/* Copyright (C) 2017 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Maksym Zyzak [committer] */

#include "L1EventMatch.h"

void L1EventMatch::Clear()
{
  fMCEvent.clear();
  fNTracks = 0;
}
