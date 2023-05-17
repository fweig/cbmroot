/* Copyright (C) 2023 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer] */
#ifndef CBM_ALGO_BASE_FILESYSTEM_H
#define CBM_ALGO_BASE_FILESYSTEM_H

#include <boost/filesystem.hpp>

namespace cbm::algo
{

  // Use boost::filesystem by default instead of std::filesystem for
  // compatibility with older compilers and ROOT versions
  namespace fs = boost::filesystem;

}  // namespace cbm::algo

#endif  // CBM_ALGO_BASE_FILESYSTEM_H
