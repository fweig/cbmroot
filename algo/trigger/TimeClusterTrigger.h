/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer], Dominik Smith */

#ifndef CBM_ALGO_TIMECLUSTERTRIGGER_H
#define CBM_ALGO_TIMECLUSTERTRIGGER_H 1


#include <cstdint>
#include <vector>

namespace cbm::algo
{

  /** @class TimeClusterTrigger
   ** @author Volker Friese <v.friese@gsi.de>
   ** @author Dominik Smith <d.smith@gsi.de>
   ** @since 2021
   ** @brief Finds clusters in time-series data
   **
   ** A trigger is generated when the number density of data exceeds a given threshold. Each datum
   ** can contribute to only one trigger. Consecutive triggers are separated by at least the dead time.
   **
   ** The input vector must be sorted, otherwise the behaviour is undefined.
   **/
  class TimeClusterTrigger {

  public:
    /** @brief Execution
     ** @param  dataVec     Source data vector
     ** @param  winSize     Size of trigger window
     ** @param  minNumData  Threshold on number of data within the trigger window
     ** @param  deadTime    Minimum time between two triggers
     ** @return Vector of trigger times
     **/
    std::vector<double> operator()(const std::vector<double>& dataVec, double winSize, int32_t minNumData,
                                   double deadTime) const;
  };


}  // namespace cbm::algo

#endif /* CBM_ALGO_TIMECLUSTERTRIGGER_H */
