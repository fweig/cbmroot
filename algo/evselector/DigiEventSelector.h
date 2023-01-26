/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Shreya Roy [committer], Pierre-Alain Loizeau, Norbert Herrmann, Volker Friese, Dominik Smith */

#ifndef CBM_ALGO_DIGIEVENTSELECTOR_H
#define CBM_ALGO_DIGIEVENTSELECTOR_H 1

#include "CbmDigiEvent.h"

#include <cstdint>

namespace cbm::algo
{

  struct DigiEventSelectorParams {
    uint8_t fStsMinStations = 0;
    uint8_t fTofMinLayers   = 0;
  };

  /** @class DigiEventSelector
  ** @author Dominik Smith <d.smith@gsi.de>
 ** @author Shreya Roy <s.roy@gsi.de>
** @author Volker Friese <v.friese@gsi.de>
 
** @since 2022
 ** @brief
 **
 **/
  class DigiEventSelector {

  public:
    DigiEventSelector(DigiEventSelectorParams params) { fParams = params; }
    bool operator()(const CbmDigiEvent& event) const;

    void SetParams(DigiEventSelectorParams params) { fParams = params; }

  private:
    DigiEventSelectorParams fParams;

    bool CheckSts(const std::vector<CbmStsDigi>& digis) const;
    bool CheckTof(const std::vector<CbmTofDigi>& digis) const;
  };
}  // namespace cbm::algo

#endif /* CBM_ALGO_DIGIEVENTSELECTOR_H */
