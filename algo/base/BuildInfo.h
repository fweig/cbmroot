/* Copyright (C) 2022 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer]*/
#ifndef CBM_ALGO_BUILD_INFO_H
#define CBM_ALGO_BUILD_INFO_H

#include <string>

namespace cbm::algo::BuildInfo
{

  extern const std::string GIT_HASH;
  extern const std::string BUILD_TYPE;
  extern const bool GPU_DEBUG;

}  // namespace cbm::algo::BuildInfo

#endif  // CBM_ALGO_BUILD_INFO_H
