/* Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

#include "CbmMcbmUtils.h"

#include <stdexcept>

namespace cbm
{
  namespace mcbm
  {
    std::string GetSetupFromRunId(uint64_t ulRunId)
    {
      /// General remark: only runs known to exist on disk/tape are mapped so "holes" are not an oversight
      /// => if necessary exception throwing can also be added for them but anyway analysis will crash (no raw data)

      /// 2021 "CRI" runs: 1575 - 1588 = 15/07/2021
      std::string sSetupName = "mcbm_beam_2021_07_surveyed";
      if (ulRunId < 1575) {
        /// Only runs at earliest in the 2021 benchmark beamtime and with all 6 systems are supported by this function
        /// From mCBM redmine wiki page: "run 1575: 1st run with 6 subsystems"
        throw(std::invalid_argument("RunId smaller than the earliest run mapped (1575 in 2021 campaign)"));
      }
      /// Setup changed multiple times between the 2022 carbon and uranium runs
      else if (2060 <= ulRunId && ulRunId <= 2065) {
        /// Carbon runs: 2060 - 2065 = 10/03/2022
        sSetupName = "mcbm_beam_2022_03_09_carbon";
      }
      else if (2150 <= ulRunId && ulRunId <= 2160) {
        /// Iron runs: 2150 - 2160 = 24-25/03/2022
        sSetupName = "mcbm_beam_2022_03_22_iron";
      }
      else if (2176 <= ulRunId && ulRunId <= 2310) {
        /// Uranium runs: 2176 - 2310 = 30/03/2022 - 01/04/2022
        sSetupName = "mcbm_beam_2022_03_28_uranium";
      }
      else if (2350 <= ulRunId && ulRunId <= 2397) {
        /// Nickel runs: 2350 - 2397 = 23/05/2022 - 25/05/2022 (Lambda Benchmark but mTOF troubles)
        sSetupName = "mcbm_beam_2022_05_23_nickel";
      }
      else if (2454 <= ulRunId && ulRunId <= 2497) {
        /// Lambda Benchmark Gold runs: 2454 - 2497 = 16/06/2022 - 18/06/2022
        sSetupName = "mcbm_beam_2022_06_16_gold";
      }
      else if (2610 < ulRunId) {
        /// Future runs, exception there to force implementation and support from users side
        throw(std::invalid_argument("RunId bigger than latest run mapped (2610, mCBM 2022)! Please complete the map!"));
      }

      return sSetupName;
    }
  }  // namespace mcbm
}  // namespace cbm
