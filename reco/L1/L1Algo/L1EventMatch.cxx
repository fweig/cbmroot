/** @file L1EventMatch.cxx
  * @copyright Copyright (C) 2017 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Maxim Zyzak [committer] **/

#include "L1EventMatch.h"

void L1EventMatch::Clear()
{
  fMCEvent.clear();
  fNTracks = 0;
}
