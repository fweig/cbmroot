/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer], Dominik Smith */

#include "TimeClusterTrigger.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <vector>

using std::vector;

namespace cbm::algo
{

  vector<double> TimeClusterTrigger::operator()(const vector<double>& dataVec, double winSize, int32_t minNumData,
                                                double deadTime) const
  {

    assert(std::is_sorted(dataVec.begin(), dataVec.end()));

    vector<double> triggerVec;
    auto winStart = dataVec.begin();
    auto current  = dataVec.begin();

    while (current != dataVec.end()) {

      // If window size is exceeded, adjust window start
      while (*current - *winStart > winSize)
        winStart++;

      // Create trigger if threshold is reached
      if (std::distance(winStart, current) >= minNumData - 1) {
        triggerVec.push_back(0.5 * (*current + *winStart));

        // Start new window after dead time
        winStart = current + 1;
        while (winStart != dataVec.end() && *winStart - *current <= deadTime)
          winStart++;
        current = winStart;
      }

      // If threshold is not reached, check with next element
      else
        current++;
    }

    return triggerVec;
  }

}  // namespace cbm::algo
