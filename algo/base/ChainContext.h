/* Copyright (C) 2023 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer] */
#ifndef CBM_ALGO_BASE_CHAINCONTEXT_H
#define CBM_ALGO_BASE_CHAINCONTEXT_H

#include "Options.h"
#include "RecoParams.h"

namespace cbm::algo
{
  struct ChainContext {
    Options opts;
    RecoParams recoParams;
  };
}  // namespace cbm::algo

#endif
