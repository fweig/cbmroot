/* Copyright (C) 2022 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer]*/
#ifndef CBM_ALGO_BASE_UTIL_TIMINGSFORMAT_H
#define CBM_ALGO_BASE_UTIL_TIMINGSFORMAT_H

#include <string>
#include <string_view>

#include "Prelude.h"

namespace xpu
{
  class timings;
}

namespace cbm::algo
{
  std::string MakeReport(std::string_view title, const xpu::timings& t, size_t align = 40);
  std::string MakeReportSubtimers(std::string_view title, const xpu::timings& t, size_t align = 40);
  std::string MakeReportSummary(std::string_view, const xpu::timings& t, size_t align = 40);
}  // namespace cbm::algo

#endif
