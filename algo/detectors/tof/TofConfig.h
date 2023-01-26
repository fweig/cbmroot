/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer] */

#ifndef CBM_ALGO_TOFCONFIG
#define CBM_ALGO_TOFCONFIG 1

#include <cstdint>

namespace cbm::algo
{
  /** @class TofConfig
  ** @author Dominik Smith <d.smith@gsi.de>
 
** @since 2022
 ** @brief
 **
 **/
  class TofConfig {

  public:
    static uint32_t GetTofTrackingStation(const uint32_t smType, const uint32_t sm, const uint32_t rpc)
    {
      const uint8_t numSmTypes                 = 10;
      const uint8_t numRpc[numSmTypes]         = {5, 3, 5, 1, 1, 1, 2, 2, 1, 2};
      const uint8_t numSm[numSmTypes]          = {5, 0, 1, 0, 0, 1, 1, 1, 2, 1};
      const uint8_t trkStation[numSmTypes][25] = {
        {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3},
        {0, 0, 0, 0, 0},
        {2, 2, 2, 2, 2},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {1, 1, 2, 2, 0}};

      if (smType < numSmTypes) {
        if (sm < numSm[smType] && rpc < numRpc[smType]) return trkStation[smType][sm * numRpc[smType] + rpc];
        else
          return 0;
      }
      else
        return 0;
    }
  };
}  // namespace cbm::algo

#endif /* CBM_ALGO_TOFCONFIG */
