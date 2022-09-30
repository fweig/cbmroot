/* Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

#include <cstdint>
#include <string>

namespace cbm
{
  namespace mcbm
  {
    std::string GetSetupFromRunId(uint64_t uRunId);

    /// Class needed to trigger loading of the library as no fct dict in ROOT6 and CLING
    class ToForceLibLoad {
    };
  }  // namespace mcbm
}  // namespace cbm
